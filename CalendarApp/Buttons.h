#ifndef Buttons_h
#define Buttons_h

class Button {
  public:
    bool isPressed(bool pressed) {

      if (pressed == false)
        return false;

      if (isTimeoutActive())
        return false;

      ActivateTimeout();
      
      return true;
    }

  private:
    uint32_t buttonTimeout;

    bool isTimeoutActive() {
      if (millis() < buttonTimeout) {
        return true;
      } else {
        return false;
      }
    }

    void ActivateTimeout() {
        buttonTimeout = millis() + 200;
    }
};

Button button;

#endif
