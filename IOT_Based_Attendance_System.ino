/*
 * ==========================================================
 *  IoT Based Smart Attendance System
 * ----------------------------------------------------------
 *  Hardware : ESP32 + MFRC522 RFID Reader + I2C 16x2 LCD + Buzzer
 *  Cloud    : Google Sheets (via Google Apps Script Web App)
 *
 *  How it works:
 *   - Each student is assigned an RFID card (UID).
 *   - Scanning the card toggles Check-IN / Check-OUT status.
 *   - The LCD shows the student's name and status.
 *   - The buzzer gives audio feedback (IN / OUT / Unknown card).
 *   - Attendance is logged in real time to a Google Sheet.
 *
 *  Author: Rajat
 * ==========================================================
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Secrets (WiFi creds + Google Script URL) live in config.h,
// which is NOT committed to GitHub. See config.h.example.
#include "config.h"

// ========= Pins =========
#define RST_PIN 27
#define SS_PIN 5
#define BUZZER_PIN 16

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ========= Students =========
// Replace these UIDs and names with your own registered cards.
struct Student {
  String uid;
  String name;
  bool isCheckedIn;
};

Student students[] = {
  {"UID_CARD_1", "STUDENT_NAME_1", false},
  {"UID_CARD_2", "STUDENT_NAME_2", false},
  {"UID_CARD_3", "STUDENT_NAME_3", false},
  {"UID_CARD_4", "STUDENT_NAME_4", false},
  {"UID_CARD_5", "STUDENT_NAME_5", false}
};

int totalStudents = sizeof(students) / sizeof(students[0]);

// ========= URL Encode =========
String urlEncode(String str) {
  String encoded = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) encoded += c;
    else {
      encoded += '%';
      code0 = (c >> 4) & 0xF;
      code1 = c & 0xF;
      encoded += char(code0 + (code0 < 10 ? '0' : 'A' - 10));
      encoded += char(code1 + (code1 < 10 ? '0' : 'A' - 10));
    }
  }
  return encoded;
}

// ========= SETUP =========
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // LCD
  lcd.begin();
  lcd.backlight();

  // RFID
  SPI.begin(18, 19, 23, SS_PIN);
  mfrc522.PCD_Init();

  // Start screen
  lcd.setCursor(0, 0);
  lcd.print("SMART ATTENDENCE");
  lcd.setCursor(0, 1);
  lcd.print("BY RAJAT");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your ID");
  lcd.setCursor(0, 1);
  lcd.print("Card...");
}

// ========= LOOP =========
void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    char buf[4];
    sprintf(buf, "%02X", mfrc522.uid.uidByte[i]);
    uidString += buf;
  }

  Serial.println(uidString);

  bool found = false;

  for (int i = 0; i < totalStudents; i++) {
    if (uidString == students[i].uid) {
      found = true;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(students[i].name);

      if (!students[i].isCheckedIn) {
        // CHECK IN
        students[i].isCheckedIn = true;

        lcd.setCursor(0, 1);
        lcd.print("Checked IN");

        // Buzzer: long beep
        digitalWrite(BUZZER_PIN, HIGH);
        delay(300);
        digitalWrite(BUZZER_PIN, LOW);

        sendToGoogleSheet(students[i].name, "IN");

      } else {
        // CHECK OUT
        students[i].isCheckedIn = false;

        lcd.setCursor(0, 1);
        lcd.print("Checked OUT");

        // Buzzer: double beep
        for (int j = 0; j < 2; j++) {
          digitalWrite(BUZZER_PIN, HIGH);
          delay(100);
          digitalWrite(BUZZER_PIN, LOW);
          delay(100);
        }

        sendToGoogleSheet(students[i].name, "OUT");
      }

      delay(100);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scan your ID");
      lcd.setCursor(0, 1);
      lcd.print("Card...");
      break;
    }
  }

  if (!found) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown Card!");

    // Buzzer: error beep (3 times)
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
    }

    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan your ID");
    lcd.setCursor(0, 1);
    lcd.print("Card...");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// ========= SEND DATA =========
void sendToGoogleSheet(String name, String status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(scriptURL) + "?name=" + urlEncode(name) + "&status=" + urlEncode(status);

    Serial.println(url); // debug

    http.begin(url);
    http.GET();
    http.end();
  }
}
