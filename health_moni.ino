#include <WiFi.h>
#include <HTTPClient.h>

// -------- WiFi Credentials --------
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// -------- ThingSpeak --------
String server = "http://api.thingspeak.com/update";
String apiKey = "YOUR_THINGSPEAK_API_KEY";

// -------- Sensor Pins --------
#define TEMP_PIN 34   // LM35 Analog Pin

// -------- Thresholds --------
float TEMP_HIGH = 38.0;      // Celsius
int HR_LOW = 50;
int HR_HIGH = 120;
int SPO2_LOW = 92;

// Dummy values (replace with real sensor readings)
int heartRate = 78;
int spo2 = 97;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop() {

  // ---- Temperature Read (LM35) ----
  int temp_adc = analogRead(TEMP_PIN);
  float temperature = (temp_adc * 3.3 / 4095) * 100;

  // ---- Print Values ----
  Serial.println("---- Patient Vitals ----");
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Heart Rate: ");
  Serial.println(heartRate);
  Serial.print("SpO2: ");
  Serial.println(spo2);

  // ---- Upload to Cloud ----
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server + "?api_key=" + apiKey +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(heartRate) +
                 "&field3=" + String(spo2);

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.println("Data uploaded to cloud");
  }

  // ---- Alert Logic ----
  if (temperature > TEMP_HIGH || heartRate < HR_LOW || heartRate > HR_HIGH || spo2 < SPO2_LOW) {
    Serial.println("⚠️ ALERT: Abnormal Health Condition Detected!");
    // Here you can trigger Telegram / Blynk / Email alert
  }

  delay(15000); // ThingSpeak minimum delay
}
