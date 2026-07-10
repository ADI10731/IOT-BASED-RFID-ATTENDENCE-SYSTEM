# 📡 IoT-Based Smart Attendance System

> Tap a card. Get logged. Never touch a paper register again.

An automated attendance system built with an **ESP32**, an **RFID reader**, an **I2C LCD**, and a **buzzer** — that logs every check-in and check-out straight to a **live Google Sheet**, in real time, with zero manual entry.

Scan once to check **IN**. Scan again to check **OUT**. The LCD confirms your name, the buzzer gives you a beep of approval (or three angry beeps if your card isn't registered), and the sheet updates itself before you've even walked away.

---

## 🎬 See It In Action

📊 **[Live Attendance Sheet →](https://docs.google.com/spreadsheets/d/1Zj-Y5Hd2nO7ANKTF43BUEZzYgxa1j9TEY9QSslNBZaA/edit?gid=0#gid=0)**

Every scan from the hardware appears here automatically — timestamp, name, and status (IN/OUT).

---

## ✨ Features

| | |
|---|---|
| 🔑 | **RFID identification** — every student gets a unique card, no typing, no passwords |
| 🔁 | **Smart toggle logic** — first tap = IN, next tap = OUT, automatically |
| 📟 | **16x2 I2C LCD** — shows the scanned student's name and live status |
| 🔊 | **Buzzer feedback** — long beep (IN), double beep (OUT), triple beep (unknown card) |
| ☁️ | **Instant cloud logging** — pushes straight to Google Sheets, no server or database required |
| 📶 | **WiFi-native** — runs entirely on the ESP32's built-in WiFi, no extra modules |

---

## 🧰 Hardware Used

- ESP32 Dev Board
- MFRC522 RFID Reader + Cards/Tags
- 16x2 I2C LCD Display
- Active Buzzer
- Jumper wires + breadboard/PCB

### 🔌 Wiring

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
| LCD SDA / SCL | GPIO 21 / GPIO 22 |

For the full visual layout, check the diagrams included in this repo:

📷 [`IOT BASED ATTENDENCE CIRCUIT DIAGRAM.jpg`](IOT%20BASED%20ATTENDENCE%20CIRCUIT%20DIAGRAM.jpg) — complete circuit schematic
📷 [`IOT BASED ATTENDENCE CONNECTIONS.jpeg`](IOT%20BASED%20ATTENDENCE%20CONNECTIONS.jpeg) — physical wiring/connections photo

---

## 📂 Repository Structure

```
IOT-BASED-RFID-ATTENDENCE-SYSTEM/
├── IOT_BASED_RFID_ATTENDENCE_SYSTEM.ino     # Main ESP32 firmware
├── IOT BASED ATTENDENCE CIRCUIT DIAGRAM.jpg # Circuit schematic
├── IOT BASED ATTENDENCE CONNECTIONS.jpeg    # Physical wiring photo
├── LICENSE                                  # MIT License
└── README.md
```

---

## 🚀 Getting Started

### 1. Set up the Google Sheet
Use (or duplicate) the sheet linked above, and connect it to a Google Apps Script Web App that accepts `name` and `status` as GET parameters and appends them as a new row with a timestamp.

### 2. Install Arduino libraries
- `MFRC522` (by GithubCommunity)
- `LiquidCrystal_I2C`
- ESP32 board support (via Boards Manager)

### 3. Add your credentials
Open `IOT_BASED_RFID_ATTENDENCE_SYSTEM.ino` and update:
- Your WiFi SSID and password
- Your Google Apps Script deployment URL

> ⚠️ **Heads up:** if you fork or clone this repo, swap in your own WiFi credentials and Script URL before flashing — and avoid committing real credentials to a public repo.

### 4. Register your cards
Upload the sketch, open the Serial Monitor at `115200` baud, and scan each card to grab its UID. Drop those UIDs (and names) into the `students[]` array in the code.

### 5. Flash and scan
Upload to your ESP32. On boot it connects to WiFi, shows a welcome screen, then waits for a scan. Tap a card and watch it show up on the [live sheet](https://docs.google.com/spreadsheets/d/1Zj-Y5Hd2nO7ANKTF43BUEZzYgxa1j9TEY9QSslNBZaA/edit?gid=0#gid=0) instantly.

---

## 🧠 How It Works

```
 Card Tap → ESP32 reads UID → Match found in students[]?
     │
     ├── Yes → Toggle IN/OUT → Update LCD → Beep → Push to Google Sheets
     │
     └── No  → "Unknown Card!" on LCD → Triple beep
```

---

## 📄 License

Released under the [MIT License](LICENSE) — free to use, modify, and build on.

---

## 🙌 Author

Built with ❤️ by **Adithya R Kulkarni**
