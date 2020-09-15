

#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <WiFi.h>


WiFiClient client;

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

#define SS_PIN 21
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

boolean isOn = false;
bool authorized = false;
bool timeIsUp = false;

long int previousMillis;
long int previousMillis2;
long int previousMillis3;

int minutes = 10;

long int interval = 10000;

int relay = 16;

void turnOn()
{
  timeIsUp = false;
  previousMillis = millis();
  Serial.println("Relais ON");
  isOn = true;
  authorized = false;
  digitalWrite(relay, HIGH);
}

void turnOff()
{
  Serial.println("Relais OFF");
  isOn = false;
  authorized = false;
  timeIsUp = false;
  previousMillis = millis();
  digitalWrite(relay, LOW);
}

void timeUp()
{

  if (millis() - previousMillis2 > 5000)
  {
    Serial.println("Time Up");
    turnOff();
  }
  else if (authorized)
  {
    Serial.print("Override TV-Staying On");
    authorized = false;
    timeIsUp = false;
    previousMillis = millis();
  }
}

void authorize()
{
  if (timeIsUp)
  {
    authorized = true;
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

void handleCard() {
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
  if (content.substring(1) == "BB D0 59 D3")
  {
    if (millis() - previousMillis3 > 1500)
    {
      Serial.println("Authorized access");
      Serial.println();
      authorize();
      delay(2000);
      previousMillis3 = millis();
    }
  }

  else
  {
    if (millis() - previousMillis3 > 1500)
    {
      Serial.println(" Access denied");
      delay(2000);
    }
  }
}


const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;


void setup()
{

  pinMode(relay, OUTPUT);
  Serial.begin(115200); // Initiate a Serial communication
  WiFiStart();
  DynamicJsonDocument doc(capacity);
  doc = getJson();

  Serial.print(doc["data"].as<char *>());

  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522running for: " + interval
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop()
{

  unsigned long currentMillis = millis();

  if (isOn)
  {
    if (currentMillis - previousMillis > interval)
    {
      if (!timeIsUp)
      {
        timeIsUp = true;
        Serial.println("Time up TV turns off in 5 Minutes");
        previousMillis2 = currentMillis;
      }
      timeUp();
    }
  }
  
 handleCard();
}
