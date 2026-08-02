#include <WiFi.h>
#include <HTTPClient.h>

#define LDR_PIN 34      // Pin connected to the LDR (light sensor)
#define LIGHT_PIN 25    // Pin connected to the relay/LED controlling the light

const int THRESHOLD = 800; // Adjust based on your LDR and environment

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverUrl = "http://YOUR_FLASK_SERVER_IP:5000/log";

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);

  if (lightLevel > THRESHOLD) {
    digitalWrite(LIGHT_PIN, HIGH); // Dark outside -> turn light ON
  } else {
    digitalWrite(LIGHT_PIN, LOW);  // Enough daylight -> turn light OFF
  }

  sendDataToServer(lightLevel, digitalRead(LIGHT_PIN));
  delay(2000);
}

void sendDataToServer(int lightLevel, int status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"light_level\":" + String(lightLevel) +
                          ",\"status\":\"" + (status == HIGH ? "ON" : "OFF") + "\"}";

    int httpResponseCode = http.POST(jsonPayload);
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi not connected, skipping data send.");
  }
}
