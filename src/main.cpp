#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <string.h>

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -18000
#define UTC_OFFSET_DST -14400

#define BUTTON_PIN 2
#define lcdColumns 20
#define lcdRows 4
#define dayNum 2

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* url = "https://api.weather.gov/gridpoints/LWX/103,89/forecast";


LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
HTTPClient http;

String generatedAt;
String periodName;
String temperature;
String precipitationPercent;
String forecast;
int isNightTime;

uint8_t moon[8] = {
  0b00111,
  0b01110,
  0b11100,
  0b11100,
  0b11100,
  0b01110,
  0b00111,
  0b00000,
};

uint8_t sun[8] = {
  0b00100,
  0b10101,
  0b01110,
  0b11111,
  0b01110,
  0b10101,
  0b00100,
  0b00000,
};

uint8_t degree[8] = {
  0b00110,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

uint8_t droplet[8] = {
  0b00100,
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b00100,
  0b00000,
};



void setup(){
  // initialize LCD obj.
  lcd.init();

  //create custom chars
  lcd.createChar(1,moon);
  lcd.createChar(2,sun);
  lcd.createChar(3,degree);
  lcd.createChar(4,droplet);

  // turn on LCD backlight                      
  lcd.backlight();

  //begin serial monitoring
  Serial.begin(115200);

  //displays connection on screen
  lcd.print("Connecting To: <ssid>");

  //connect to wifi
  WiFi.begin(ssid, password, 6);
  http.useHTTP10(true);

  //waits for Wi-Fi Connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    lcd.print(".");
  }

  //syncs to NTP Server
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  //clears screen after connection
  lcd.clear();
}

int call_api() {
  //call api url
  http.begin(url);
  http.GET();
  String result = http.getString();

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, result);

  // test if parsing succeeds
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return -1;
  }

  //retrieve data points
  generatedAt = doc["properties"]["generatedAt"].as<String>();
  periodName = doc["properties"]["periods"][dayNum]["name"].as<String>();
  isNightTime = doc["properties"]["periods"][dayNum]["isDaytime"].as<String>().equals("True");
  temperature = doc["properties"]["periods"][dayNum]["temperature"].as<String>();
  precipitationPercent = doc["properties"]["periods"][dayNum]["probabilityOfPrecipitation"]["value"].as<String>();
  forecast = doc["properties"]["periods"][dayNum]["shortForecast"].as<String>();

  http.end();
  return 0;
}

String getTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    lcd.setCursor(0, 1);
    lcd.println("Connection Err");
    return "";
  }

  char buffer[6]; // "HH:MM" + null
  strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
  return String(buffer);
}

void print_info() {
  lcd.clear();
  String currentLine = periodName;
  !isNightTime ? currentLine += " \x01     " : currentLine += " \x02      ";
  currentLine += getTime();
  lcd.setCursor(0, 0);
  lcd.print(currentLine);

  currentLine = temperature + "\x03"+ "F      \x04:" + precipitationPercent + "%";
  lcd.setCursor(0, 1);
  lcd.print(currentLine);
}

void loop(){
  Serial.println(call_api());
  print_info();
  delay(10000);
}