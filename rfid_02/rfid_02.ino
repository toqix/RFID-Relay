#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>

//------------------------Configure Settings here --------------------------------
const char *ssid = "WIfI-Name";
const char *password = "WIFI-Password";
const char *mastercard = "";                                    //the RFID-Chip UUID of one Card, that always work even if there are problems with the server.
const char *serverName = "https://your-server.com/path.json"; //the server from which the Arduino should get the UUIDs

#define relay 16  //GPIO Pin of the Relay
#define buzzer 12 //GPIO Pin of the Buzzer
#define SS_PIN 21
#define RST_PIN 22
//--------------------------------------------------------------------------------

//internal objects...
WiFiClient client;                //Create wificlient instance
HTTPClient http;                  //Create httpclient instant
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

bool isOn = false; //safes wheter the relay is open or closed
bool timeIsUp = false;
long int previousMillis;    //start of the last turn on of the relay if over the specified minute/delay iniate automatic shutdown timer
long int previousMillis2;   //start of automatic shutdown logic if over 5 minutes turn off the system
long int previousMillis3;   //millis to block card spamming new card can be read every 1.5 seconds
long int previousMillis4;   //for the automated reload of UUID's
long int oneDay = 86400000; //one day in milliseconds

//variables which will be set from laoded json...
long int automaticReloadDelay = oneDay; //time until update of UUID's
long int turnOffDelay = (45 * 60000);   //calculate the shutdown interval in milliseconds
bool revalidate = true;                 //wheter to revalidate or immediately shutdown
long int revalidateDelay = (5 * 60000); //Time until to shutdown the system after time was up
std::vector<String> uuids;              //all the registered uuids who have access
bool notify = true;                     //wheter to give sound feedback or not

//a function to get and return a string loaded from any webserver
String getJsonString()
{
  //initialize the httpclient
  http.begin(serverName);

  //check if the response is ok
  int httpResponseCode = http.GET();
  String payload = "";
  if (httpResponseCode > 0)
  {
    //successfull connected to server...
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    //check if response is ok
    if (httpResponseCode == 200)
    {
      payload = http.getString();
    }
  }
  else
  {
    Serial.print("Coulnd't reach server Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

//decodes the String loaded from the server checks status and safes set delay and uuids
void loadUUIDs()
{
  //set the json document
  DynamicJsonDocument doc(1024);

  //only for test purposes if no test server is available
  String testInput = "{\"status\":\"ok\",\"delay\":45,\"cards\":[\"UUID1\", \"UUID2\"]}";

  //get the Json string from the server
  String input = getJsonString();
  //check if it has been any usefull output
  if (input == "")
  {
    //retry once if it still fails set the delay for the next reload to 5 minutes instead of a day
    input = getJsonString();
    if (input == "")
    {
      //loading failed again
      automaticReloadDelay = 60000; //set next reload to 1 minute
    }
  }

  //decode json string to json object
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  //decode delay and set the system delay
  turnOffDelay = obj["turnOffDelay"].as<long int>() * 60000;
  //decode revalidate and revalidate delay and set to corresponding setting variable
  revalidateDelay = obj["revalidateDelay"].as<long int>() * 60000;
  //decode notify and store
  notify = obj["notify"].as<bool>();
  //decdoe automatic reload delay an store
  automaticReloadDelay = obj["automaticReloadDelay"].as<long int>() * 3600000;
  //decode uuids and iterate through them
  JsonArray cards = obj["cards"].as<JsonArray>();
  //set public array of uuids...
  uuids.clear();
  for (JsonVariant idCard : cards)
  {
    JsonObject card = idCard.as<JsonObject>();
    String id = card["uuid"].as<String>();
    Serial.println("id: " + id);
    Serial.println("owner: " + card["owner"].as<String>());
    //add id to uuid array
    uuids.push_back(id);
  }
}
void WiFiStart()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("_");
  }
  Serial.println();
  Serial.println("Done");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}
//---------------- Sound system -----------------
enum Sound
{
  on,
  off,
  timeUp,
  accessDenied,
  overrideOn
};

void playSound(Sound sound)
{
  if (notify)
  {
    switch (sound)
    {
    case on:
      ledcWriteNote(0, NOTE_E, 6);
      delay(130);
      ledcWriteNote(0, NOTE_G, 6);
      delay(130);
      ledcWriteNote(0, NOTE_E, 7);
      delay(130);
      ledcWriteNote(0, NOTE_C, 7);
      delay(130);
      ledcWriteNote(0, NOTE_D, 7);
      delay(130);
      ledcWriteNote(0, NOTE_G, 7);
      delay(130);
      ledcWriteTone(0, 0);
      break;
    case off:
      ledcWriteNote(0, NOTE_D, 4);
      delay(170);
      ledcWriteNote(0, NOTE_G, 2);
      delay(300);
      ledcWriteTone(0, 0);
      break;
    case accessDenied:
      ledcWriteNote(0, NOTE_B, 5);
      delay(100);
      ledcWriteNote(0, NOTE_E, 6);
      delay(800);
      ledcWriteTone(0, 0);
      break;
    case overrideOn:
      ledcWriteNote(0, NOTE_D, 7);
      delay(130);
      ledcWriteNote(0, NOTE_G, 7);
      delay(130);
      ledcWriteTone(0, 0);
      break;
    case timeUp:
      ledcWriteNote(0, NOTE_E, 6);
      delay(130);
      ledcWriteNote(0, NOTE_G, 6);
      delay(130);
      ledcWriteNote(0, NOTE_E, 6);
      delay(130);
      ledcWriteNote(0, NOTE_G, 6);
      delay(130);
      ledcWriteNote(0, NOTE_E, 6);
      delay(130);
      ledcWriteNote(0, NOTE_G, 6);
      delay(130);
      ledcWriteTone(0, 0);
      break;
    }
  }
}

void turnOn()
{
  timeIsUp = false;
  previousMillis = millis();
  Serial.println("Relais ON");
  isOn = true;
  digitalWrite(relay, HIGH);
  playSound(on);
}

void turnOff()
{
  Serial.println("Relais OFF");
  isOn = false;
  digitalWrite(relay, LOW);
  playSound(off);
}

void authorize()
{
  //check if the card is holded after automatic shutdown timer started (time is up)
  if (timeIsUp)
  {
    //reset shutdown timer
    Serial.print("Override TV-Staying On");
    timeIsUp = false;
    previousMillis = millis();
    playSound(overrideOn);
  }
  else
  {
    isOn = !isOn;

    if (isOn)
    {
      turnOn();
    }
    else
    {
      turnOff();
    }
  }
}

void handleCard()
{
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the c* 60ards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String content = "";

  // //Show UID on Serial monitor
  // Serial.print("UID tag :");
  // for (byte i = 0; i < mfrc522.uid.size; i++)
  // {
  // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  // Serial.print(mfrc522.uid.uidByte[i], HEX);
  // content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  // content.concat(String(mfrc522.uid.uidByte[i], HEX));
  // }
  // Serial.println();
  // Serial.print("Message : ");

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  //check wheter uuid is the mastercasrd
  if (content.substring(1) == mastercard)
  {
    if (millis() - previousMillis3 > 1500)
    {
      Serial.println("Authorized access using Mastercard!");
      Serial.println();
      authorize();
      previousMillis3 = millis();
      return;
    }
  }

  //check if the uuid is part of the uuid list and give acces
  for (String uuid : uuids)
  {
    if (content.substring(1) == uuid)
    {
      if (millis() - previousMillis3 > 1500)
      {
        Serial.println("Authorized access");
        Serial.println();
        authorize();
        previousMillis3 = millis();
        return;
      }
    }
  }

  if (millis() - previousMillis3 > 1500)
  {
    Serial.println("Access denied");
    previousMillis3 = millis();
    playSound(accessDenied);
  }
}

void setup()
{
  pinMode(relay, OUTPUT);   //declare the relay pin as output
  ledcSetup(0, 1.2E5, 12);  //create tone channel
  ledcAttachPin(buzzer, 0); //attache buzzer to tone channel
  Serial.begin(115200);     // Initiate a Serial communication
  WiFiStart();

  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522running for: " + interval
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  loadUUIDs();
}

void loop()
{

  //handle the card logic (check if new card is present and if it has access)
  handleCard();

  //when the system is on check onTime...
  if (isOn)
  {
    //check if the relay has been on longer the the specified minutes/delay
    if (millis() - previousMillis > turnOffDelay)
    {
      if (!revalidate)
      {
        //turn off instantly
        turnOff();
      }
      else
      {
        //revalidation logic:
        //if the time is up variable is not set yet set it and set the previousMillis2
        if (!timeIsUp)
        {
          timeIsUp = true;
          Serial.println("Time up TV turns off in 5 Minutes");
          //store the time when the specified interval was reached
          previousMillis2 = millis();
          playSound(timeUp);
        }
        //when the start of timisup is longer than 5 seconds turn off the relay
        if (millis() - previousMillis2 > revalidateDelay)
        {
          Serial.println("Time Up");
          timeIsUp = false;
          turnOff();
        }
      }
    }
  }

  //check if the system has been up for longer than a day then reload from the server
  if (millis() - previousMillis4 > automaticReloadDelay)
  {
    //set the reload delay back to one day
    automaticReloadDelay = oneDay;
    //load the uuids from server
    loadUUIDs();
    //save currentMillis
    previousMillis4 = millis();
  }
}
