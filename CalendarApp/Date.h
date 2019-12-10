#ifndef Date_h
#define Date_h

class Date {
  public:
    uint8_t day;
    uint8_t month;
    uint16_t year;

    uint8_t daysInMonth;
    uint8_t firstWeekday;
    uint8_t weeksInMonth;
    uint16_t daysSince2000;

    void changeTo(int8_t m, int16_t y) {
      changeTo(day, m, y);
    }

    void changeTo(int8_t d, int8_t m, int16_t y) {
      Serial.println("change Date to " + String(d) + "," + String(m) + "," + String(y));
      // TODO there are bugs in this function
      if (m == JAN - 1) {
        month = DEZ;
      } else if (m == DEZ + 1) {
        month = JAN;
      } else {
        month = m;
      }
      if (y < 2000) {
        year = 2000;
      } else {
        year = y;
      }
      day = d;
      calculateNew();
      if (day > daysInMonth) {
        day = daysInMonth;
      }
    }

    void changeForward() {
      if (day < daysInMonth) {
        day++;
        calcDaysSince2000();
      } else {
        day = 1;
        if (month < DEC) {
          month++;
        } else {
          month = JAN;
          year++;
        }
        calculateNew();
      }
    }

    void changeBackward() {
      if (day > 1) {
        day--;
        calcDaysSince2000();
      } else {
        if (month > JAN) {
          month--;
        } else {
          month = DEZ;
          if (year > 2000)
            year--;
        }
        calculateNew();

        day = daysInMonth;
      }
    }

    void calculateNew() {
      calcDaysSince2000();
      calcFirstWeekday();
      calcDaysInMonth();
      calcWeeksInMonth();

      updateDateHasNotes();
    }

    void updateDateHasNotes() {
      //Serial.println("updateDateHasNotes");
      for (uint8_t i = 0; i < daysInMonth; i++) {
        uint16_t d = daysSince2000 - day + i;

        //Serial.println("check day " + String(d));

        dateHasNote[i] = false;
        for (uint8_t n = 0; n < EEPROM_NOTES_MAX; n++) {
          if (eepromRegister[n] == d) {
            //Serial.println("found day " + String(d) + " at pos " + String(i + 1));
            dateHasNote[i] = true;
          }
        }
      }
    }

    void initEeprom () {

      keyInEEPROM = true;

      for (uint8_t i = 0; i < EEPROM_KEY_LEN; i++) {

        // it is a uint8_t, but uint8_t is also fine
        uint8_t c = EEPROM.read(EEPROM_START_PASSWORD + i);

        if (c != pgm_read_byte(eepromCalenderKey + i))
          keyInEEPROM = false;
      }

      // DEGUG!!!!!!!!
      //keyInEEPROM = false;

      // return because key was found in eeprom
      if (keyInEEPROM)
        return;

      // clear all
      for (uint16_t i = EEPROM_STORAGE_SPACE_START; i < EEPROM_STORAGE_SPACE_START + 1024; i++) {
        EEPROM.update(i, 0x00);
      }

      // write key to eeprom
      for (uint8_t i = 0; i < EEPROM_KEY_LEN; i++) {
        EEPROM.update(EEPROM_START_PASSWORD + i, pgm_read_byte(eepromCalenderKey + i));
      }

#if defined (ESP8266) || defined (ESP32)
      // commit key
      EEPROM.commit();
#endif
    }

    void readFromEEPROM() {

      day =   EEPROM.read(EEPROM_START_DATE);
      month = EEPROM.read(EEPROM_START_DATE + 1);
      year =  EEPROM.read(EEPROM_START_DATE + 2) + 2000;
      /*
            day = 5;
            month = NOV;
            year = 2019;
      */


    }

  private:

    uint8_t getDaysOfMonth(uint8_t m) {
      if (m == FEB) {
        if (year % 4 == 0) {
          return 29;
        } else {
          return 28;
        }
      } else {
        return pgm_read_byte(daysPerMonth + m);
      }
    }

    void calcDaysInMonth() {
      daysInMonth = getDaysOfMonth(month);
    }

    void calcDaysSince2000() {
      int16_t yearsDiff = year - 2000;
      int16_t tmpCounter = yearsDiff * 365 + yearsDiff / 4;
      if (year % 4 != 0 || month > FEB) {
        tmpCounter += 1;
      }
      for (uint8_t i = 0; i < month; i++) {
        tmpCounter += getDaysOfMonth(i);
      }
      daysSince2000 = tmpCounter + day;
      //Serial.println("Date is " + String(day) + "," + String(month) + "," + String(year));
      //Serial.println("days since Start: " + String(daysSince2000));
    }

    void calcFirstWeekday() {
      // first day 1.1.2000 was a saturday
      firstWeekday = (daysSince2000 - day + SAT) % DAYS_OF_THE_WEEK;
    }

    void calcWeeksInMonth() {
      if (daysInMonth < 30) {
        weeksInMonth = 5;
      } else if (daysInMonth == 30) {
        if (firstWeekday == SUN) {
          weeksInMonth = 6;
        } else {
          weeksInMonth = 5;
        }
      }  else {
        if (firstWeekday == SAT || firstWeekday == SUN) {
          weeksInMonth = 6;
        } else {
          weeksInMonth = 5;
        }
      }
    }
};


#endif
