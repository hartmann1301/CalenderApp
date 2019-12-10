#include <EEPROM.h>
#include <Arduboy2.h>

#define DAYS_OF_THE_WEEK 7

#if defined (ESP8266) || defined (ESP32)
#include <PS2X_lib.h>
PS2X ps2x;

#if defined (ESP32)
TFT_eSPI screen = TFT_eSPI();
#elif defined (ESP8266)
SSD1306Brzo screen(OLED_I2C_ADRESS, I2C_SDA, I2C_SCL);
#endif
#endif

Arduboy2Base arduboy;

#include "Font.h"
#include "Globals.h"
#include "Buttons.h"
#include "Date.h"

Font6x6 font6x6;
Date date;

#include "Items.h"
#include "Calendar.h"
CalendarApp calendar;

void getButtons() {
#if defined (ESP8266) || defined (ESP32)
  // this function is called inside arduboy whenever buttonStates() is called
  arduboy.setExternalButtons(ps2x.getArduboyButtons());
#endif
}

void setup() {
  Serial.begin(57600);

  delay(100);

  arduboy.setFrameRate(60);
  arduboy.begin();

#if defined (ESP8266) || defined (ESP32)
  ps2x.begin();

  // this function must be called or the esp will crash with the first call
  arduboy.setExternalButtonsHandler(getButtons);

  // i think this is only needed for the esp8266 eeprom emulation
  EEPROM.begin(1024);

  Serial.println();
  Serial.println("Calendar App");
#endif

  date.initEeprom();

  if (keyInEEPROM) {
    date.readFromEEPROM();
  } else {
    date.changeTo(1, NOV, 2019);
  }

  calendar.init();
  date.calculateNew();

  //Serial.println("size of uint16_t " + String(sizeof(uint16_t)));
}


void loop() {
  if (!(arduboy.nextFrame()))
    return;

#if defined(ESP32)
  arduboy.printFramesPerSecond();
#endif

  // first we clear our screen to black
  arduboy.fillScreen(BLACK);
  arduboy.pollButtons();

  calendar.update();

  arduboy.display();
}
