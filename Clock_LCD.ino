#include "definitions.h"
#include <ESP8266WiFi.h>
#include <time.h>
#include <EEPROM.h>

#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "src/hw/include/Button.h"
#include "src/screens/include/TimeDisplay.h"
#include "src/screens/include/DisplayManager.h"
#include "src/screens/include/Settings.hpp"
#include "src/screens/include/Background.hpp"
#include "src/hw/include/SSD1306Brightness.h"
#include "src/lib/include/TimeManager.hpp"
#include "src/lib/include/AlarmManager.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

SSD1306Brightness display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int alarmHourToday;
int alarmMinuteToday;


Screen* t = new TimeDisplay();
Screen* conWiFi = new ConnectingDisplay("WiFi");
Screen* conNTP = new ConnectingDisplay("NTP");
Screen* conMQTT = new ConnectingDisplay("MQTT");
Background* bg;
Settings mainMenu;


DisplayManager* dm = new DisplayManager(display);


byte currentBrightnessLevel = 0;
bool fan = false;
bool light = false;
WiFiClient wifiClient;
PubSubClient client(MQTT_HOST, 1883, wifiClient); // 1883 is the listener port for the Broker
HTTPClient http;
String weeklyAlarmSchedule[7];

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);  // initialize with the I2C addr 0x3D (for the 128x64)
  Serial.begin(115200);
  TimeManager::update();
  display.initBrightnessControl();
  dm->init(t);
  
  bg = new Background();

  delay(20);
  
  dm->addEnd(conWiFi);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(10);
  }

  dm->replace(conNTP);

  configTime(CLOCK_TZ_OFFSET, DST_ENABLED, "pool.ntp.org", "time.nist.gov");

  while (!TimeManager::isReady()) {
    delay(30);
  }
  
  Serial.println();

  //mqttConnect();

  //Get back to time display.
  dm->popStack();


  dm->addEnd(bg);

  Serial.println ("Finished Setup");
}
void loop() {
  //if (!client.connected())  mqttConnect();

  TimeManager::update();
  BuzzerManager::instance().update();
  AlarmManagerInstance::instance().update();

  dm->update();
  //client.loop();
  delay(10);
}



bool mqttConnect() {  // Connect to MQTT Server and subscribe to the topic
  dm->addEnd(conMQTT);
  
  client.setCallback(mqttHandle);
  if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    client.subscribe(MQTT_SUBSCRIPTION_TOPIC);
    dm->popStack();
    return true;
  }

  dm->popStack();
  return false;
}

void mqttHandle(char* topic, byte * payload, unsigned int length) {
  Serial.printf("Topic: %s Data: ", topic);
  for (auto i = 0; i < length; i++){
    Serial.print(payload[i]);
    Serial.print(" ");
  }

  Serial.println();
}















long monthday;
long month;
long year;
boolean colon;
boolean updateDisplayNextCycle = true;
long hour;
long second;
long minute;
long weekday;
String weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
long alarmSkipTime = -1;
boolean alarmToday = false;
long lastSync = 0;
long lastBrightnessUpdate = 0;
long lastColonChange = 0;

void updateDisplay() {
  updateDisplayNextCycle = false;
  Serial.println ("Updating Screen");
  updateTimes();
  //writeTimeToScreen();
  display.display();
}



void checkTimers() {
  if (millis() - lastSync > 3600000) {
    lastSync = millis();
  }
  if (millis() - lastColonChange > colonFlashInterval) {
    lastColonChange = millis();
    colon = !colon;
    updateDisplayNextCycle = true;
  }
  if (millis() - lastBrightnessUpdate > 10000) {
    lastBrightnessUpdate = millis();
  }
}

void checkalarms() {
  updateTimes();

  String alarmTodayTime = weeklyAlarmSchedule[weekday];
  alarmHourToday = (alarmTodayTime.substring(0, alarmTodayTime.indexOf(":"))).toInt();
  alarmMinuteToday = (alarmTodayTime.substring(alarmTodayTime.indexOf(":") + 1, alarmTodayTime.length())).toInt();
  alarmToday = (!alarmTodayTime.equals(":")) && hour <= alarmHourToday //The hour is before or at the alarm hour
               &&   (minute <= alarmMinuteToday || hour < alarmHourToday); //up to an including alarm hour
  if (alarmToday && minute == alarmMinuteToday && hour == alarmHourToday && second > 4) {
    alarmToday = false;

  }


  if (alarmToday) {
    if (alarmHourToday == hour && alarmMinuteToday == minute && second < 2) {
      if (alarmHourToday * alarmMinuteToday != alarmSkipTime)
        alarm();
      else {
        alarmSkipTime = -1;
        display.clearDisplay();
        display.setTextSize(3);
        display.setCursor (0, 0);
        display.println ("Mute");
        display.print ("Alarm");
        display.display();
        //tone(13, 1200, 10);
        delay(3000);
      }
      //writeTimeToScreen();
    }
  }
}

void restorePreviousState() {
  for (int i = 0; i < 7; i++)
    weeklyAlarmSchedule[i] = ":";
  String msg = loadAlarmsFromEeprom();
  Serial.println ("REC:" + msg);
  commitAlarmsToEeprom (msg);
  for (int i = 0; i < 7; i ++) {
    msg = msg.substring (msg.indexOf("#") + 1);
    weeklyAlarmSchedule[i] = (msg.substring (0, msg.indexOf("#")));
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor (0, 0);
  display.println("Loaded State");
  display.display();
  delay(500);
  checkalarms();
  //writeTimeToScreen();
}






void alarm () {

  pinMode (4, INPUT_PULLUP);
  delay (10);
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(String(hour % 12) + ":");
  if (minute < 10)
    display.print("0");
  display.print(String(minute));
  display.setTextSize(1.1);
  display.setTextColor(WHITE);
  display.setCursor(0, 35);
  display.println("OK - Bottom") ;
  display.print ("Button");
  display.display();
  weeklyAlarmSchedule[weekday] = ":";
  boolean alarmstate = 0;
  randomSeed(analogRead(0));
  while (alarmstate == 0) {
    int randNumber = random(3, 11);
    int randdelay = random(300, 2000);

    tone(13, randNumber * 100);
    delay(randdelay);                       // wait for a second
    alarmstate = !digitalRead(2) || alarmstate;
    delay (50);
    alarmstate = alarmstate && !digitalRead(2);
    noTone(13);
  }
  while (!digitalRead(2)) {
    delay (10);
  }
  digitalWrite(13, LOW);
  commitAlarmsToEeprom(createEepromString());
  //writeTimeToScreen();
}
void updateTimes() {
  Serial.println ("Decoding Times");
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  hour = p_tm->tm_hour;
  minute =  p_tm->tm_min;
  second = p_tm->tm_sec;
  weekday = p_tm->tm_wday;
  year = p_tm->tm_year;
  month = p_tm->tm_mon;
  monthday = p_tm->tm_mday;
}



String loadAlarmsFromEeprom() {
  EEPROM.begin(512);
  String eepromString = "";
  for (int i = 0; i < 64; i++) {
    eepromString += (char)EEPROM.read(i);
  }
  Serial.print ("Loaded from eeprom: ");
  Serial.println (eepromString.substring (0, eepromString.indexOf ("@")));
  EEPROM.end();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor (0, 0);

  display.println("Loaded EEPROM");
  display.display();
  delay(500);

  return eepromString.substring (0, eepromString.indexOf ("@"));
}
String createEepromString () {
  String str = "";
  for (int i = 0; i < 7; i++ ) {
    str += ("#" + weeklyAlarmSchedule[i]);
  }
  str += "#";
  Serial.print ("Created Eeprom String : ");
  Serial.println (str);
  return str;
}
void commitAlarmsToEeprom(String alm) {
  String str = alm + "@";
  EEPROM.begin(512);
  for (int i = 0; i < str.length(); i++) {
    EEPROM.write(i, str.charAt (i));
  }
  EEPROM.commit();
  EEPROM.end();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor (0, 0);
  display.println("Saved to EEPROM");
  display.display();
  delay(500);
}

