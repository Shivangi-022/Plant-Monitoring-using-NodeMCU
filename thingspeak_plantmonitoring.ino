#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#define DHTPIN 14
#define DHTTYPE DHT11
#define SOIL_PIN 34
const char* ssid = "isro";
const char* password = "Isro1234";
const char* thingSpeakApiKey = "Write_API_KEY";
const char* thingSpeakServer = "http://api.thingspeak.com/update";
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoistureRaw = analogRead(SOIL_PIN);
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Soil Moisture (raw): ");
  Serial.println(soilMoistureRaw);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(thingSpeakServer) +
                 "?api_key=" + thingSpeakApiKey +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(humidity) +
                 "&field3=" + String(soilMoistureRaw);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.println("Data sent to ThingSpeak successfully");
    } else {
      Serial.print("Error sending data. Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(15000);
}
