# 📡 IoT-Based Smart Attendance System

An automated attendance system built with **ESP32**, an **RFID reader**, an **I2C LCD**, and a **buzzer**, which logs each check-in/check-out event straight to a **Google Sheet** in real time.

Students tap their RFID card once to check **IN** and tap again to check **OUT** — no manual registers, no app installs, no paperwork.

---

## ✨ Features

- 🔑 **RFID-based identification** using an MFRC522 reader/card set
- 🔁 **Toggle logic** — first scan = Check-IN, next scan = Check-OUT
- 📟 **16x2 I2C LCD display** shows the student's name and status
- 🔊 **Buzzer feedback**
  - Long beep → Checked IN
  - Double beep → Checked OUT
  - Triple beep → Unknown/unregistered card
- ☁️ **Cloud logging** to Google Sheets via a Google Apps Script Web App (no external server or database needed)
- 📶 Works over **WiFi** (ESP32 built-in)

---

## 🛠️ Hardware Required

| Component | Notes |
|---|---|
| ESP32 Dev Board | Main microcontroller |
| MFRC522 RFID Reader + Cards/Tags | For scanning student IDs |
| 16x2 I2C LCD Display | Address `0x27` (adjust in code if different) |
| Active Buzzer | Audio feedback |
| Jumper wires, breadboard/PCB | Wiring |

### Wiring (as configured in code)

| MFRC522 Pin | ESP32 Pin |
|---|---|
| SDA (SS) | GPIO 5 |
| RST | GPIO 27 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| SCK | GPIO 18 |
| 3.3V | 3.3V |
| GND | GND |

| Other | ESP32 Pin |
|---|---|
| Buzzer | GPIO 16 |
| LCD SDA/SCL | Default I2C pins (GPIO 21 / GPIO 22) |

---

## 📂 Project Structure

```
iot-attendance/
├── src/
│   ├── IOT_Based_Attendance_System.ino   # Main firmware
│   └── config.h.example                  # Template for WiFi/Script secrets
├── google-apps-script/
│   └── Code.gs                           # Script that writes to Google Sheets
├── .gitignore
├── LICENSE
└── README.md
```

---

## 🚀 Setup Guide

### 1. Google Sheet + Apps Script
1. Create a new Google Sheet.
2. Go to **Extensions → Apps Script**.
3. Paste the contents of [`google-apps-script/Code.gs`](google-apps-script/Code.gs).
4. Click **Deploy → New deployment → Web app**.
   - Execute as: **Me**
   - Who has access: **Anyone**
5. Copy the generated Web App URL — you'll need it in step 3 below.

### 2. Arduino IDE setup
Install these libraries via **Library Manager**:
- `MFRC522` by GithubCommunity
- `LiquidCrystal_I2C`
- ESP32 board support (via Boards Manager URL, if not already installed)

### 3. Configure secrets
1. In `src/`, copy `config.h.example` to a new file named `config.h`.
2. Fill in your WiFi SSID/password and the Google Apps Script URL from step 1.
3. `config.h` is already listed in `.gitignore`, so it will **never** be pushed to GitHub.

### 4. Register your RFID cards
1. Upload the sketch and open the Serial Monitor (115200 baud).
2. Scan each card — its UID will print to the console.
3. Update the `students[]` array in the `.ino` file with the real UIDs and names.

### 5. Upload and go
Flash the sketch to your ESP32. On boot it connects to WiFi, shows a welcome screen, then prompts **"Scan your ID Card..."**.

---

## 🔒 Security Note

This repo keeps WiFi credentials and the Google Apps Script URL in a **local `config.h` file that is git-ignored**. Never commit real credentials to a public repository. If you've already pushed real secrets to GitHub, rotate your WiFi password and redeploy the Apps Script with a new URL, then scrub the old commit from history.

---

## 📸 Demo

*(Add a photo or short video/GIF of the hardware setup here once uploaded.)*

---

## 📄 License

Released under the [MIT License](LICENSE).

---

## 🙌 Author

Built by **Adithya R Kulkarni**
