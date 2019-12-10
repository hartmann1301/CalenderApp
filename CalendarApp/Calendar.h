#ifndef Calendar_h
#define Calendar_h

class CalendarApp {

  public:
    void update() {
      checkButtons();

      title.update();
      month.update();
      year.update();
      daysCurrent.update();
      notes.update();
    }

    void init() {
      cMode = CalenderMode::TITLESCREEN;
      notes.init();
    }

    CalendarApp() {
      title.setTo(9, 0);
      month.setTo(19, 50);
      year.setTo(83, 50);
      daysCurrent.setTo(0, 64);
      notes.setTo(0, 66);
    }

    void checkButtons() {

      switch (cMode) {
        case CalenderMode::TITLESCREEN:
          if (arduboy.justPressed(A_BUTTON)) {
            cMode = CalenderMode::YEARS_AND_MONTH;
          }

          break;

        case CalenderMode::YEARS_AND_MONTH:

          month.drawArrows();
          year.drawArrows();

          if (button.isPressed(arduboy.pressed(DOWN_BUTTON))) {
            date.changeTo(date.month + 1, date.year);
          }

          if (button.isPressed(arduboy.pressed(UP_BUTTON))) {
            date.changeTo(date.month - 1, date.year);
          }

          if (button.isPressed(arduboy.pressed(RIGHT_BUTTON))) {
            date.changeTo(date.month, date.year + 1);
          }

          if (button.isPressed(arduboy.pressed(LEFT_BUTTON))) {
            date.changeTo(date.month, date.year - 1);
          }

          if (arduboy.justPressed(B_BUTTON)) {
            cMode = CalenderMode::TITLESCREEN;
            //Serial.println("enter cMode TITLESCREEN");
          }

          if (arduboy.justPressed(A_BUTTON)) {
            cMode = CalenderMode::ALL_DAYS;
            //Serial.println("enter cMode ALL_DAYS");
          }
          break;

        case CalenderMode::ALL_DAYS:
          if (arduboy.justPressed(LEFT_BUTTON)) {
            date.changeBackward();
          }

          if (arduboy.justPressed(RIGHT_BUTTON)) {
            date.changeForward();
          }

          if (arduboy.justPressed(UP_BUTTON)) {
            for (uint8_t i = 0; i < DAYS_OF_THE_WEEK; i++) {
              date.changeBackward();
            }
          }

          if (arduboy.justPressed(DOWN_BUTTON)) {
            for (uint8_t i = 0; i < DAYS_OF_THE_WEEK; i++) {
              date.changeForward();
            }
          }

          if (arduboy.justPressed(B_BUTTON)) {
            cMode = CalenderMode::YEARS_AND_MONTH;
            //Serial.println("enter cMode ALL_DAYS");
          }

          if (arduboy.justPressed(A_BUTTON)) {
            cMode = CalenderMode::NOTES_VIEW;

            notes.readNotes();
          }
          break;

        case CalenderMode::NOTES_VIEW:
          if (arduboy.justPressed(B_BUTTON) ||
              arduboy.justPressed(UP_BUTTON) ||
              arduboy.justPressed(DOWN_BUTTON) ||
              arduboy.justPressed(LEFT_BUTTON) ||
              arduboy.justPressed(RIGHT_BUTTON)
             ) {
            cMode = CalenderMode::ALL_DAYS;
          }

          if (arduboy.justPressed(A_BUTTON)) {
            cMode = CalenderMode::NOTES_EDIT;

            notes.writeCursor = 0;
            notes.xCursor = 0;
            notes.yCursor = 0;
          }
          break;

        case CalenderMode::NOTES_EDIT:
          if (arduboy.justPressed(B_BUTTON)) {
            cMode = CalenderMode::NOTES_VIEW;
          }

          if (arduboy.justPressed(A_BUTTON)) {
            if (notes.isCursorInTastatur) {
              if (notes.xCursor == TASTATUR_COLUMNS - 1) {
                if (notes.yCursor == 0) {
                  notes.deleteChar();
                } else if (notes.yCursor == 1) {
                  if (notes.pTastatur == tastaturSmallLetters) {
                    notes.pTastatur = tastaturBigLetters;
                  } else {
                    notes.pTastatur = tastaturSmallLetters;
                  }
                } else if (notes.yCursor == 2) {
                  notes.isYearlyNote = !notes.isYearlyNote;
                } else {
                  notes.saveAndQuit();
                }
              } else {
                notes.print();
              }
            } else {
              notes.isOverwriteMode = !notes.isOverwriteMode;
            }
          }

          if (button.isPressed(arduboy.pressed(DOWN_BUTTON))) {
            if (notes.isCursorInTastatur) {
              if (notes.yCursor < 3) {
                notes.yCursor++;
                if (notes.yCursor == TASTATUR_ROWS - 1) {
                  notes.xCursor = TASTATUR_COLUMNS - 1;
                }
              }
            } else {
              if (!notes.isCursorInTastatur) {
                notes.isCursorInTastatur = true;
              }
            }
          }

          if (button.isPressed(arduboy.pressed(UP_BUTTON))) {
            if (notes.isCursorInTastatur) {
              if (notes.yCursor > 0) {
                if (notes.yCursor == TASTATUR_ROWS - 1) {
                  notes.xCursor =  TASTATUR_COLUMNS - 1;
                }
                notes.yCursor--;
              } else {
                notes.isCursorInTastatur = false;
              }
            }
          }

          if (button.isPressed(arduboy.pressed(RIGHT_BUTTON))) {
            if (notes.isCursorInTastatur) {
              if (notes.yCursor != TASTATUR_ROWS - 1) {
                if (notes.xCursor < TASTATUR_COLUMNS - 1) {
                  notes.xCursor++;
                } else {
                  notes.xCursor = 0;
                }
              }
            } else {
              if (notes.writeCursor < EEPROM_BLOCK_SIZE - 1) {
                notes.writeCursor++;
              }
            }
          }

          if (button.isPressed(arduboy.pressed(LEFT_BUTTON))) {
            if (notes.isCursorInTastatur) {
              if (notes.yCursor != TASTATUR_ROWS - 1) {
                if (notes.xCursor > 0) {
                  notes.xCursor--;
                } else {
                  notes.xCursor = TASTATUR_COLUMNS - 1;
                }
              }
            } else {
              if (notes.writeCursor > 0) {
                notes.writeCursor--;
              }
            }
          }
          break;
      }

    }

  private:
    CalenderTitle title;
    CalenderYear year;
    CalenderMonth month;
    CalenderDays daysCurrent;
    //CalenderDays daysLast;
    CalenderNotes notes;
};


#endif
