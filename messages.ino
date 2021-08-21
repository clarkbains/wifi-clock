void MESSAGE_BrightnessLevel (int brightnessValue, int totalBrightnessValues) {

}
void MESSAGE_recievedMqttData () {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor (0, 0);
  display.println("Recieved");
  display.print ("MQTT Data");
  display.display();
}
void MESSAGE_alarmMuted () {
  display.println ("Muted");
  display.println (String((alarmHourToday > 12) ? (alarmHourToday - 12) : alarmHourToday) + ":" + ((alarmMinuteToday < 10) ? "0" : "") + String (alarmMinuteToday) + ((alarmHourToday > 12) ? " PM" : " AM"));
  display.print ("Alarm");
  display.display();
}

void writeTimeToScreen() {
  Serial.println ("Fupdate");

}

void displayTemp (float temp, int humidity, String main, String description){
  display.clearDisplay();
  display.setCursor (0,0);
  display.setTextSize (3);
  display.print (temp);
  display.println (" C");
  display.setTextSize (2);
 display.setCursor(0, 25);
  display.println (main);
    display.setTextSize (1);
  display.println (description);
  display.display();

  
  display.display();
   // display.startscrollright(0x00, 0x0F);
  }
