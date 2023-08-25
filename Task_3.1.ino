#include <WiFiNINA.h>
#include "ThingSpeak.h"
#include <DHT.h>

#define DHTPIN 3      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT sensor type 
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "iphone";     // WiFi SSID
char pass[] = "qazxcvbnm12"; // WiFi password

unsigned long myChannelNumber =  2250503;  // ThingSpeak channel number
const char *myWriteAPIKey = "5S1790LF7S0F8KF1"; // ThingSpeak write API key

WiFiClient client;

void setup() {
  Serial.begin(9600);
  connectWiFi();
  ThingSpeak.begin(client);
  dht.begin();
  }
  void connectWiFi() {
    Serial.print("WiFi Connecting.");
    WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi Connecting...");
  }

  Serial.println("WiFi Connected");
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  ThingSpeak.setField(1, h); // Field 1 is for humidity
  ThingSpeak.setField(2, t); // Field 2 is for temperature

  int updateStatus = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (updateStatus == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(updateStatus);
  }

  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println(" *C\t");

  delay(2000); // delay
}
