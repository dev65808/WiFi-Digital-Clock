#include <DMD2.h>
#include <SPI.h>
#include <Wire.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/SystemFont5x7.h>

SPIDMD dmd(2, 1);  // 64x16 display (2 panels side by side)

String timeStr = "--:--:--";
String tempVal = "0.0";
String humVal = "0";
String coVal = "0";

unsigned long lastSwitch = 0;
int displayState = 0;
unsigned long lastTimeUpdate = 0;
bool wifiConnected = false;

String incomingData = "";

void setup() {
  Serial.begin(9600);  // Communication from ESP
  dmd.begin();
  dmd.setBrightness(255);
  dmd.selectFont(Arial_Black_16);
  dmd.clearScreen();
}

void loop() {
  // --- Handle Serial Input ---
  if (Serial.available()) {
    incomingData = Serial.readStringUntil('\n');
    incomingData.trim();

    if (incomingData == "Waiting...") {
      wifiConnected = false;
    } else {
      wifiConnected = true;
      lastTimeUpdate = millis();

      if (incomingData.indexOf("T:") != -1) {
        // Parse sensor data
        int tIndex = incomingData.indexOf("T:");
        int hIndex = incomingData.indexOf("H:");
        int coIndex = incomingData.indexOf("CO:");

        tempVal = incomingData.substring(tIndex + 2, hIndex - 1);
        humVal = incomingData.substring(hIndex + 2, coIndex - 1);
        coVal = incomingData.substring(coIndex + 3);
      } else {
        // Time data
        timeStr = incomingData;
      }
    }
  }

  dmd.clearScreen();

  // --- Display Logic ---
  if (!wifiConnected) {
    dmd.selectFont(Arial_Black_16);
    dmd.drawString(0, 0, "Waiting");
  } else {
    unsigned long nowMillis = millis();

    if ((displayState == 0 && nowMillis - lastSwitch >= 10000) ||
        (displayState != 0 && nowMillis - lastSwitch >= 5000)) {
      displayState = (displayState + 1) % 3;
      lastSwitch = nowMillis;
    }

    if (displayState == 0) {
      // Show TIME
      dmd.selectFont(Arial_Black_16);
      String shownTime = (nowMillis - lastTimeUpdate > 3000) ? "--:--:--" : timeStr;

      int textWidth = dmd.stringWidth(shownTime.c_str());
      int x = (64 - textWidth) / 2;
      if (x < 0) x = 0;
      dmd.drawString(x, 0, shownTime.c_str());

    } else if (displayState == 1) {
      // Show Temperature and Humidity
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(1, 0, "Temp:");
      dmd.drawString(33, 0, (tempVal + "C").c_str());  // Adjust 30 as needed
      dmd.drawString(7, 8, "Hum:");
      dmd.drawString(40, 8, (humVal + "%").c_str());

    } else if (displayState == 2) {
      // Show CO Level
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(7, 0, "CO Level:");
      dmd.drawString(15, 8, (coVal + " ppm").c_str());
    }
  }

  delay(200);
}