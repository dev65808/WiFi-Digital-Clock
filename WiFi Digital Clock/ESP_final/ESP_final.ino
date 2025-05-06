#include <WiFiManager.h>
#include <WiFi.h>
#include <Wire.h>
#include <RTClib.h>
#include <time.h>
#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;

#define MQ7_PIN 36
#define RESET_BTN_PIN 25   // Your reset button connected here

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // GMT+5:30 (IST)
const int daylightOffset_sec = 0;

unsigned long lastSensorUpdate = 0;
const unsigned long sensorInterval = 5000;  // Send sensor data every 5 sec

float temperature = 0;
float humidity = 0;
int coValue = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);  // Communication with Arduino Nano
  Wire.begin();
  dht.begin();
  pinMode(RESET_BTN_PIN, INPUT_PULLUP);

  if (digitalRead(RESET_BTN_PIN) == LOW) {
    Serial.println("Reset button held → Clearing WiFi credentials...");
    WiFiManager wm;
    wm.resetSettings();
    delay(1000);
    ESP.restart();
  }

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  WiFiManager wm;
  WiFi.mode(WIFI_STA);

  if (!wm.autoConnect("ESP-Clock", "12345678")) {
    Serial.println("Failed to connect or timeout. Restarting...");
    delay(2000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi!");

  // Sync RTC with NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
                        timeinfo.tm_mday, timeinfo.tm_hour,
                        timeinfo.tm_min, timeinfo.tm_sec));
    Serial.println("RTC synced with NTP.");
  } else {
    Serial.println("NTP failed. Using last RTC time.");
  }
}

void loop() {
  DateTime now = rtc.now();
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  // Send time every second
  Serial2.println(String(timeStr));
  Serial.println("To Nano: " + String(timeStr));

  // Update and send sensor data every 5 seconds
  if (millis() - lastSensorUpdate >= sensorInterval) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    coValue = analogRead(MQ7_PIN);

    if (isnan(temperature)) temperature = 0;
    if (isnan(humidity)) humidity = 0;

    String sensorMsg = "T:" + String(temperature, 1) + " H:" + String(humidity, 0) + " CO:" + String(coValue);
    Serial2.println(sensorMsg);
    Serial.println("To Nano: " + sensorMsg);

    lastSensorUpdate = millis();
  }

  delay(1000);  // Keep in sync with seconds
}