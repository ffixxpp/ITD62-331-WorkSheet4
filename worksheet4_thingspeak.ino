#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* writeKey = "TFRKGQCSWG7E2QLQ";
const unsigned long channelID = 2339745;
const unsigned long timeDelay = 15000;

const char* ssid = "Pinsuda";
const char* password = "123456zx";

#define DHTPIN D4    
#define DHTTYPE DHT11 
DHT dht14(DHTPIN, DHTTYPE);

float temperature = 0.0;
float humidity = 0.0;

WiFiClient client;

void init_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  init_wifi();
  dht14.begin();
  ThingSpeak.begin(client);
}

void getSensorData() {
  humidity = dht14.readHumidity();
  temperature = dht14.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);
}

void loop() {
  getSensorData();
  int status = ThingSpeak.writeFields(channelID, writeKey);
  if (status == 200) {
    Serial.println("Successfully sent humidity and temperature to ThingSpeak");
  } else {
    Serial.println("Problem sending to ThingSpeak");
  }
  delay(timeDelay);
}