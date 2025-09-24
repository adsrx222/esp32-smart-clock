# Arduino Weather Display using Wi-Fi and LCD

This Arduino project connects to Wi-Fi, fetches weather data from the U.S. National Weather Service API, and displays it on a 20x4 I2C LCD. It also syncs the device time using an NTP server and shows current time, temperature, precipitation, and weather conditions with icons.

---

## Features

- Connects to Wi-Fi using ESP32
- Syncs local time via NTP server
- Fetches weather forecast from [weather.gov](https://api.weather.gov/)
- Displays weather data on a 20x4 LCD:
  - Period name (e.g., "Tuesday")
  - Current time
  - Temperature (with custom degree icon)
  - Precipitation chance (with droplet icon)
  - Weather icon (sun/moon based on time of day)

---

## Libraries Used

- `WiFi.h` - Wi-Fi connection for ESP32
- `HTTPClient.h` - HTTP requests
- `ArduinoJson.h` - JSON parsing from API
- `LiquidCrystal_I2C.h` - LCD control over I2C
- `time.h` - Time synchronization via NTP

---

## Setup

1. Connect the I2C LCD to ESP32:
   - SDA to GPIO21
   - SCL to GPIO22
2. Flash the code to your ESP32 device.
3. Open the Serial Monitor at 115200 baud to see debug messages.

---

## API Source

The weather data is fetched from: https://api.weather.gov/
