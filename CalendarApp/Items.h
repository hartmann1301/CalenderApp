#ifndef Items_h
#define Items_h

class Coordinate {
  public:
    int16_t posCurrent;
    int16_t posEnd;

    void setTo(int16_t pos) {
      posCurrent = pos;
      posEnd = pos;
    }

    void moveTo(int16_t pos) {
      posEnd = pos;
      if (posEnd == posCurrent) {
        return;
      } else if (posCurrent < posEnd) {
        posCurrent++;
      } else {
        posCurrent--;
      }
    }
};

class CalenderItem {
  public:
    Coordinate x;
    Coordinate y;

    void setTo(int16_t xPos, int16_t yPos) {
      x.setTo(xPos);
      y.setTo(yPos);
    }

    void moveTo(int16_t xPos, int16_t yPos) {
      x.moveTo(xPos);
      y.moveTo(yPos);
    }

    void draw() {};
};

class CalenderTitle : public CalenderItem {
  public:
    void changePosition() {
      if (cMode == CalenderMode::TITLESCREEN) {
        moveTo(9, 0);
      } else {
        moveTo(9, -50);
      }
    }

    void update() {
      changePosition();

      arduboy.drawBitmap(x.posCurrent, y.posCurrent, titleScreen, 110, 44, WHITE);
    }
};


class CalenderMonth : public CalenderItem {
  public:
    void changePosition() {
      if (cMode == CalenderMode::TITLESCREEN) {
        moveTo(19, 50);
      } else if (cMode == CalenderMode::YEARS_AND_MONTH) {
        moveTo(14, 11);
      } else if (cMode == CalenderMode::ALL_DAYS) {
        moveTo(3, 0);
      } else {
        moveTo(3, -10);
      }
    }

    void drawArrows() {
      if (y.posCurrent != y.posEnd)
        return;

      font6x6.setColor(WHITE);
      const uint8_t yPos = 11;
      if (arduboy.pressed(LEFT_BUTTON)) {
        font6x6.setCursor(81, yPos);
      } else {
        font6x6.setCursor(82, yPos);
      }
      font6x6.print(extraFont6x6::HINT_LEFT);

      if (arduboy.pressed(RIGHT_BUTTON)) {
        font6x6.setCursor(121, yPos);
      } else {
        font6x6.setCursor(120, yPos);
      }
      font6x6.print(extraFont6x6::HINT_RIGHT);
    }

    void update() {
      changePosition();

      font6x6.setColor(WHITE);

      //uint8_t len = font6x6.getLength(namesOfTheMonths, date.month, 9);

      font6x6.setCursor(x.posCurrent , y.posCurrent);

      font6x6.print(namesOfTheMonths, date.month, 9);
    }
};

class CalenderYear : public CalenderItem {
  public:
    void changePosition() {
      if (cMode == CalenderMode::TITLESCREEN) {
        moveTo(83, 50);
      } else if (cMode == CalenderMode::YEARS_AND_MONTH) {
        moveTo(91, 11);
      } else if (cMode == CalenderMode::ALL_DAYS) {
        moveTo(99, 0);
      } else {
        moveTo(99, -10);
      }
    }

    void drawArrows() {
      if (y.posCurrent != y.posEnd)
        return;

      font6x6.setColor(WHITE);
      const uint8_t xPos = 28;
      if (arduboy.pressed(UP_BUTTON)) {
        font6x6.setCursor(xPos, 3);
      } else {
        font6x6.setCursor(xPos, 4);
      }
      font6x6.print(extraFont6x6::HINT_UP);

      if (arduboy.pressed(DOWN_BUTTON)) {
        font6x6.setCursor(xPos, 18);
      } else {
        font6x6.setCursor(xPos, 19);
      }
      font6x6.print(extraFont6x6::HINT_DOWN);
    }

    void update() {
      changePosition();

      font6x6.setColor(WHITE);
      font6x6.setCursor(x.posCurrent, y.posCurrent);
      font6x6.print(date.year, DEC);
    }
};

class CalenderDays : public CalenderItem {
  public:
    void changePosition() {
      if (cMode == CalenderMode::TITLESCREEN) {
        moveTo(0, 64);
      } else if (cMode == CalenderMode::YEARS_AND_MONTH) {
        moveTo(0, 28);
      } else if (cMode == CalenderMode::ALL_DAYS) {
        moveTo(0, 8);
      } else if (cMode == CalenderMode::NOTES_VIEW) {
        moveTo(0, -16);
      } else {
        moveTo(0, -64);
      }
    }

    void update() {
      changePosition();

      font6x6.setColor(WHITE);

      for (uint8_t i = 0; i < DAYS_OF_THE_WEEK; i++) {

        font6x6.setCursor(OFFSET_LEFT + i * XOFFSET_FOR_DAYS, y.posCurrent + 1);

        if (i == MON) {
          font6x6.print(F("Mo"));
        } else if (i == TUE) {
          font6x6.print(F("Tu"));
        } else if (i == WED) {
          font6x6.print(F("We"));
        } else if (i == THU) {
          font6x6.print(F("Th"));
        } else if (i == FRI) {
          font6x6.print(F("Fr"));
        } else if (i == SAT) {
          font6x6.print(F("Sa"));
        } else if (i == SUN) {
          font6x6.print(F("Su"));
        }
      }

      uint8_t daysInLine = date.firstWeekday;
      uint8_t daysCounter = date.daysInMonth;

      uint8_t yOffset = 10;
      uint8_t ySpace  = 9;
      if (date.weeksInMonth > 5) {
        yOffset -= 1;
        ySpace  -= 1;
      }

      font6x6.setColor(BLACK);
      arduboy.fillRect(0, y.posCurrent + 8, WIDTH, 48, WHITE);

      for (int16_t yGrid = 0; yGrid < date.weeksInMonth; yGrid++) {

        for (int16_t xGrid = daysInLine; xGrid < DAYS_OF_THE_WEEK && daysCounter != 0; xGrid++) {

          daysCounter--;

          uint8_t day = date.daysInMonth - daysCounter;

          int16_t centerXOffset = (14 - font6x6.getLength(day, DEC)) / 2;

          if (day == date.day) {
            font6x6.setColor(WHITE);
            arduboy.fillRect(OFFSET_LEFT - 2 + xGrid * XOFFSET_FOR_DAYS, y.posCurrent + yOffset - 1 + yGrid * ySpace, 17, 8, BLACK);
          }

          if (dateHasNote[day]) {
            uint8_t col;
            if (day == date.day) {
              col = WHITE;
            } else {
              col = BLACK;
            }
            arduboy.fillRect(OFFSET_LEFT + 13 + xGrid * XOFFSET_FOR_DAYS, y.posCurrent + yOffset - 1 + yGrid * ySpace, 2, 2, col);
            arduboy.drawPixel(OFFSET_LEFT + 13 + xGrid * XOFFSET_FOR_DAYS, y.posCurrent + yOffset + yGrid * ySpace, !col);
          }

          font6x6.setCursor(centerXOffset + OFFSET_LEFT + xGrid * XOFFSET_FOR_DAYS, y.posCurrent + yOffset + yGrid * ySpace);
          font6x6.print(day, DEC);

          if (day == date.day) {
            font6x6.setColor(BLACK);
          }
        }

        daysInLine = 0;
      }
    }
};

class CalenderNotes : public CalenderItem {
  public:
    uint8_t xCursor, yCursor;

    uint8_t writeCursor;
    uint8_t writeLine[EEPROM_BLOCK_SIZE];

    const uint8_t *pTastatur;

    bool isCursorInTastatur;
    bool isYearlyNote;
    bool isOverwriteMode;

    CalenderNotes() {
      xCursor = 0;
      yCursor = 0;
      writeCursor = 0;
      isCursorInTastatur = true;
      pTastatur = tastaturSmallLetters;
      isOverwriteMode = false;

      for (uint8_t i = 0; i < EEPROM_BLOCK_SIZE; i++) {
        writeLine[i] = ' ';
      }
      /*
        writeLine[0] = 'H';
        writeLine[1] = 'e';
        writeLine[2] = 'l';
        writeLine[3] = 'l';
        writeLine[4] = 'o';

        writeLine[10] = 'W';
        writeLine[11] = 'o';
        writeLine[12] = 'r';
        writeLine[13] = 'l';
        writeLine[14] = 'd';
      */
    }

    void init() {
      // read register
      for (uint8_t i = 0; i < EEPROM_NOTES_MAX; i++) {
        uint16_t storedNote;
        EEPROM.get(EEPROM_START_REGISTER + i * sizeof(uint16_t), storedNote);
        if (storedNote != 0) {
          //Serial.println("Found day " + String(storedNote) + " in EEPROM");
        }
        eepromRegister[i] = storedNote;
      }

      if (!keyInEEPROM) {
        writeLine[0] = 'A';
        writeLine[1] = 'r';
        writeLine[2] = 'd';
        writeLine[3] = 'u';
        writeLine[4] = 'b';
        writeLine[5] = 'o';
        writeLine[6] = 'y';
        writeLine[7] = ' ';
        writeLine[8] = 'G';
        writeLine[9] = 'a';
        writeLine[10] = 'm';
        writeLine[11] = 'e';
        writeLine[12] = ' ';
        writeLine[13] = 'J';
        writeLine[14] = 'a';
        writeLine[15] = 'm';
        writeLine[16] = ' ';
        writeLine[17] = '4';

        saveAndQuit();

        cMode = CalenderMode::TITLESCREEN;
      }
      /*
        eepromRegister[7]   = 1433;
        eepromRegister[9]   = 1475;
        eepromRegister[22]  = 1489;
      */
    }

    uint8_t getEepromData(uint8_t reg, uint8_t pos) {
      uint8_t dat = EEPROM.read(EEPROM_START_BLOCKS + reg * EEPROM_BLOCK_SIZE + pos);
      //Serial.println("read " + String(dat) + " at pos " + String(EEPROM_NOTES_MAX + reg * EEPROM_BLOCK_SIZE + pos));
      return dat;
    }

    void setEepromData(uint8_t reg, uint8_t pos, uint8_t dat) {
      //Serial.println("write " + String(uint8_t(dat)) + " at pos " + String(EEPROM_NOTES_MAX + reg * EEPROM_BLOCK_SIZE + pos));
      EEPROM.update(EEPROM_START_BLOCKS + reg * EEPROM_BLOCK_SIZE + pos, dat);

#if defined (ESP8266) || defined (ESP32)
      // commit key
      EEPROM.commit();
#endif
    }

    void saveAndQuit() {
      Serial.println("saveAndQuit");

      // save to eeprom
      bool foundNote = false;
      for (uint8_t i = 0; i < EEPROM_NOTES_MAX && !foundNote; i++) {
        if (eepromRegister[i] == date.daysSince2000) {
          Serial.println("Save day " + String(date.daysSince2000) + " to existing register pos " + String(i));
          foundNote = true;
          for (uint8_t j = 0; j < EEPROM_BLOCK_SIZE; j++) {
            setEepromData(i, j, writeLine[j]);
          }

        }
      }

      if (!foundNote) {
        for (uint8_t i = 0; i < EEPROM_NOTES_MAX && !foundNote; i++) {
          if (eepromRegister[i] == EEPROM_NO_ENTRY) {
            Serial.println("Save day " + String(date.daysSince2000) + " to free register pos " + String(i));
            foundNote = true;
            uint16_t currentDay = date.daysSince2000;
            eepromRegister[i] = currentDay;
            EEPROM.put(EEPROM_START_REGISTER + i * sizeof(uint16_t), currentDay);
            for (uint8_t j = 0; j < EEPROM_BLOCK_SIZE; j++) {
              setEepromData(i, j, writeLine[j]);
            }
          }
        }
      }

      date.updateDateHasNotes();

      EEPROM.update(EEPROM_START_DATE, date.day);
      EEPROM.update(EEPROM_START_DATE + 1, date.month);
      EEPROM.update(EEPROM_START_DATE + 2, (date.year - 2000));

#if defined (ESP8266) || defined (ESP32)
      EEPROM.commit();
#endif

      cMode = CalenderMode::NOTES_VIEW;
    }

    bool readNotes() {
      bool foundNote = false;
      for (uint8_t i = 0; i < EEPROM_NOTES_MAX; i++) {
        if (eepromRegister[i] == date.daysSince2000) {
          Serial.println("Found day  " + String(date.daysSince2000) + " at register pos " + String(i));
          foundNote = true;
          for (uint8_t j = 0; j < EEPROM_BLOCK_SIZE; j++) {
            writeLine[j] = getEepromData(i, j);
          }
        }
      }
      if (!foundNote) {
        for (uint8_t j = 0; j < EEPROM_BLOCK_SIZE; j++) {
          writeLine[j] = ' ';
        }
      }
      return foundNote;
    }

    void deleteChar() {
      //Serial.println("deleteChar");

      if (isOverwriteMode == true) {
        writeLine[writeCursor] = ' ';
      } else {
        if (writeCursor > 0) {
          for (uint8_t i = writeCursor; i < EEPROM_BLOCK_SIZE - 1; i++) {
            writeLine[i - 1] = writeLine[i];
          }
          writeLine[EEPROM_BLOCK_SIZE - 1] = ' ';
        }
      }

      if (writeCursor > 0) {
        writeCursor--;
      }
    }

    void print() {
      uint8_t newChar = pgm_read_byte(pTastatur + yCursor * TASTATUR_COLUMNS + xCursor);

      if (!isOverwriteMode) {
        for (uint8_t i = EEPROM_BLOCK_SIZE - 2; i > writeCursor; i--) {
          writeLine[i] = writeLine[i - 1];
        }
      }

      writeLine[writeCursor] = newChar;

      if (writeCursor < EEPROM_BLOCK_SIZE - 1)
        writeCursor++;
    }

    void printNotes() {
      font6x6.setColor(WHITE);
      font6x6.setCursor(OFFSET_LEFT, y.posCurrent + 13);

      if (cMode == CalenderMode::NOTES_VIEW && dateHasNote[date.day] == false) {
        font6x6.print(F("< create new note >"));
      } else {
        for (uint8_t i = 0; i < EEPROM_BLOCK_SIZE; i++) {
          font6x6.print(writeLine[i]);
          //Serial.println(writeLine[i]);
        }
      }
    }

    void printCursor() {
      if (arduboy.everyXFrames(2))
        return;

      if (cMode != CalenderMode::NOTES_EDIT)
        return;

      //Serial.println("writeCursor: " + String(writeCursor));

      uint8_t xOffsetCursor = 0;
      uint8_t currentLength = 0;
      for (uint8_t i = 0; i < EEPROM_BLOCK_SIZE; i++) {
        uint8_t tmpChar = writeLine[i];

        uint8_t len = font6x6.getCharLength(tmpChar);

        if (writeCursor == i) {
          currentLength = len;
          break;
        }

        xOffsetCursor += len + font6x6.spaceBetween;
      }

      if (isOverwriteMode) {
        arduboy.fillRect(OFFSET_LEFT + xOffsetCursor, y.posCurrent + 12, currentLength, 8, WHITE);
      } else {
        arduboy.fillRect(OFFSET_LEFT + xOffsetCursor - 1, y.posCurrent + 12, 1, 8, WHITE);
      }
    }

    void changePosition() {
      if (cMode == CalenderMode::NOTES_VIEW) {
        moveTo(0, 42);
      } else if (cMode == CalenderMode::NOTES_EDIT) {
        moveTo(0, 0);
      } else {
        moveTo(0, 66);
      }
    }

    void setTastaturCursor(uint8_t xGrid, uint8_t yGrid) {
      font6x6.setCursor(OFFSET_LEFT + xGrid * 9, y.posCurrent + 28 + yGrid * 9);
    }

    void drawTastaturCursor(uint8_t xGrid, uint8_t yGrid) {
      if (isCursorInTastatur) {
        arduboy.fillRect(OFFSET_LEFT - 2 + xGrid * 9, y.posCurrent + 27 + yGrid * 9, 10, 8, BLACK);
      }
    }

    void printTastatur() {
      font6x6.setColor(BLACK);
      for (int16_t yGrid = 0; yGrid < TASTATUR_ROWS - 1; yGrid++) {
        for (int16_t xGrid = 0; xGrid < TASTATUR_COLUMNS; xGrid++) {

          if (xGrid == xCursor && yGrid == yCursor && isCursorInTastatur) {
            font6x6.setColor(WHITE);
            drawTastaturCursor(xGrid, yGrid);
          }

          setTastaturCursor(xGrid, yGrid);
          font6x6.print(pTastatur, xGrid + yGrid * TASTATUR_COLUMNS, 1);

          if (xGrid == xCursor && yGrid == yCursor) {
            font6x6.setColor(BLACK);
          }
        }
      }

      //Serial.println("x:" + String(xCursor) + " y:" + String(yCursor));

      setTastaturCursor(TASTATUR_COLUMNS - 1, TASTATUR_ROWS - 2);
      if (xCursor == TASTATUR_COLUMNS - 1 && yCursor == TASTATUR_ROWS - 2) {
        font6x6.setColor(WHITE);
      } else {
        font6x6.setColor(BLACK);
      }
      if (isYearlyNote) {
        font6x6.print(extraFont6x6::CHECKED);
      } else {
        font6x6.print(extraFont6x6::UNCHECKED);
      }

      setTastaturCursor(TASTATUR_COLUMNS - 1, TASTATUR_ROWS - 1);
      if (yCursor == TASTATUR_ROWS - 1) {
        drawTastaturCursor(TASTATUR_COLUMNS - 1, TASTATUR_ROWS - 1);
        font6x6.setColor(WHITE);
      } else {
        font6x6.setColor(BLACK);
      }
      font6x6.print(extraFont6x6::LITTLE_OKAY);

      font6x6.setCursor(OFFSET_LEFT, y.posCurrent + 57);
      font6x6.setColor(BLACK);
      if (xCursor == TASTATUR_COLUMNS - 1) {
        if (yCursor == 0) {
          font6x6.print(F("Delete"));
        } else if (yCursor == 1) {
          font6x6.print(F("Change Keys"));
        } else if (yCursor == 2) {
          font6x6.print(F("Yearly: "));
          if (isYearlyNote) {
            font6x6.print(F("On"));
          } else {
            font6x6.print(F("Off"));
          }
        } else {
          font6x6.print(F("Save & Quit"));
        }
      }
    }

    void update() {
      changePosition();

      arduboy.fillRect(0, y.posCurrent + 22, WIDTH, 44, WHITE);

      font6x6.setColor(WHITE);
      font6x6.setCursor(x.posCurrent + 10, y.posCurrent + 2);

      //font6x6.print(F("Note:"));
      font6x6.print(date.day, DEC);
      font6x6.print(F("."));
      font6x6.print(namesOfTheMonths, date.month, 9);
      font6x6.print(' ');
      font6x6.print(date.year, DEC);
      font6x6.print(':');

      printCursor();
      printNotes();

      printTastatur();
    }

};

#endif
