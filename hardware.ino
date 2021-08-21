
void checkButtons() {/*
  if (!digitalRead(2) || !digitalRead(0))
    delay(100);
  if (!digitalRead(2) && !digitalRead(0)) {
    delay (20);
    if (!digitalRead(2) && !digitalRead(0)) {
      currentBrightnessLevel += 1;
      if (currentBrightnessLevel == totalBrightnessLevels)
        currentBrightnessLevel = 0;
    }
   // setBrightness(displayBrightnessLevels, currentBrightnessLevel);
    MESSAGE_BrightnessLevel(currentBrightnessLevel, totalBrightnessLevels - 1);
    delay (600);

  }
  else if (!digitalRead(0) && digitalRead(2)) {
    delay (20);
    if (!digitalRead(0) && digitalRead(2)) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor (0, 0);
      fan = !fan;
      if (fan) {
        display.println ("Fan On");
        http.begin ("http://maker.ifttt.com/trigger/fon/with/key/ACMyDNm7gvY5XzSJ6HL-X");
        http.GET();
        http.end();
      }
      else {
        display.println ("Fan Off");
        http.begin ("http://maker.ifttt.com/trigger/foff/with/key/ACMyDNm7gvY5XzSJ6HL-X");
        http.GET();
        http.end();
      }
      display.display();
      while (!digitalRead(2) || !digitalRead(0))
        delay (50);
    }
    updateDisplay();
  }
  else if (digitalRead(0) && !digitalRead(2)) {
    delay(20);
    if (digitalRead(0) && !digitalRead(2)) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor (0, 0);
      light = !light;
      if (light) {
        display.println ("Light On");
        http.begin ("http://maker.ifttt.com/trigger/lon/with/key/ACMyDNm7gvY5XzSJ6HL-X");
        http.GET();
        http.end();
      }
      else {
        display.println ("Light Off");
        http.begin ("http://maker.ifttt.com/trigger/loff/with/key/ACMyDNm7gvY5XzSJ6HL-X");
        http.GET();
        http.end();
      }
      display.display();
      while (!digitalRead(2) || !digitalRead(0))
        delay (50);
    }
    updateDisplay();
  }*/
}
