//configuration
const char *ssid = "WIFI-NAME";
const char *password = "WIFI-PASSWORD";

const char *mastercard = "";//the RFID-Chip UUID of one Card, that always work even if there are problems with the server.

const char *serverName = "SERVER-ADDRESS/FILENAME.json"; //the server from which the Arduino should get the UUIDs

#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

WiFiClient client;

#define SS_PIN 21
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

boolean isOn = false; //safes wheter the relay is open or closed
bool timeIsUp = false;

long int previousMillis;                //start of the last turn on of the relay if over the specified minute/delay iniate automatic shutdown timer
long int previousMillis2;               //start of automatic shutdown logic if over 5 minutes turn off the system
long int previousMillis3;               //millis to block card spamming new card can be read every 1.5 seconds
long int previousMillis4;               //for the automated reload of UUID's
long int oneDay = 86400000;             //one day in milliseconds
long int automaticReloadDelay = oneDay; //time until update of UUID's

int minutes = 5; //Time to wait before starting the Automatic Shutdown Logic

long int interval = minutes * 60000; //calculate the shutdown interval in milliseconds

int relay = 16; //digital Pin of the Relay

String uuids[] = {mastercard}; //all the registered uuids who have access

//a function to get and return a string loaded from any webserver
String getJsonString(const char *serverName)
{
  //initialize the httpclient
  HTTPClient http;
  http.begin(serverName);

  //check if the response is ok
  int httpResponseCode = http.GET();
  String payload = "{}";
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    //if the response is ok load the string to later return it!
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
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
  String input = getJsonString(serverName);
  //check if it has been any usefull output
  if (input == "")
  {
    //retry once if it still fails set the delay for the next reload to 5 minutes instead of a day
    input = getJsonString(serverName);
    if (input == "")
    {
      //loading failed again
      automaticReloadDelay = 60000; //set next reload to 1 minute
    }
  }

  //decode json string to json object
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  //get status and check if it is "ok"
  String status = obj["status"];
  Serial.println("status: " + status);
  if (status != "ok")
  {
    return;
  }
  //get delay and set the system delay
  int delay = obj["delay"];

  Serial.println(String(delay));

  //parse uuids and iterate through them
  JsonArray uuids = obj["cards"].as<JsonArray>();
  for (JsonVariant id : uuids)
  {
    Serial.println("id: " + id.as<String>());
    uuids.add(id.as<String>());
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

void turnOn()
{
  timeIsUp = false;
  previousMillis = millis();
  Serial.println("Relais ON");
  isOn = true;
  digitalWrite(relay, HIGH);
}

void turnOff()
{
  Serial.println("Relais OFF");
  isOn = false;
  digitalWrite(relay, LOW);
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
  /*
    //Show UID on Serial monitor
    Serial.print("UID tag :");

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
  */
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  //if (content.substring(1) == "BD 31 15 2B" 29 F8 A7 48) //change here the UID of the card/cards that you want to give access
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
  }
}

void setup()
{

  pinMode(relay, OUTPUT);
  Serial.begin(115200); // Initiate a Serial communication
  WiFiStart();

  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522running for: " + interval
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  loadUUIDs();
}

void loop()
{

  unsigned long currentMillis = millis();

  //handle the card logic (check if new card is present and if it has access)
  handleCard();

  //when the system is on check onTime...
  if (isOn)
  {
    //check if the relay has been on longer the the specified minutes/delay
    if (currentMillis - previousMillis > interval)
    {
      //if the time is up variable is not set yet set it and set the previousMillis2
      if (!timeIsUp)
      {
        timeIsUp = true;
        Serial.println("Time up TV turns off in 5 Minutes");
        previousMillis2 = currentMillis;
      }
      //when the start of timisup is longer than 5 seconds turn off the relay
      if (currentMillis - previousMillis2 > 5000)
      {
        Serial.println("Time Up");
        timeIsUp = false;
        turnOff();
      }
    }
  }

  //check if the system has been up for longer than a day then reload from the server
  if (currentMillis - previousMillis4 < automaticReloadDelay)
  {
    automaticReloadDelay = oneDay;
    loadUUIDs();
  }
}
