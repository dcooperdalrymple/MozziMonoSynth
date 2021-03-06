/*
 * Button.h
 * Description: Debounce button input and handle press types.
 * Version: 0.1.0
 * Since: 0.1.0
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#define DEBOUNCE_TIME 50
#define PRESS_TIME 100
#define LONG_PRESS_TIME 500

typedef void (*ButtonFunctionPointer)(void);

class Button {
public:

    Button(uint8_t pin) {
        _pin = pin;
        pinMode(pin, INPUT_PULLUP);
    }
    virtual ~Button() {}

    void setDownHandler(ButtonFunctionPointer downFunctionPointer) {
        _downFunctionPointer = downFunctionPointer;
    }
    void setUpHandler(ButtonFunctionPointer upFunctionPointer) {
        _upFunctionPointer = upFunctionPointer;
    }
    void setPressHandler(ButtonFunctionPointer pressFunctionPointer) {
        _pressFunctionPointer = pressFunctionPointer;
    }
    void setLongPressHandler(ButtonFunctionPointer longPressFunctionPointer) {
        _longPressFunctionPointer = longPressFunctionPointer;
    }

    void update() {
        /*
        bool reading = false;
        if (_pin > 8) {
            reading = !(PORTB & (1 << (_pin - 8)));
        } else {
            reading = !(PORTD & (1 << _pin));
        }
        */

        bool reading = digitalRead(_pin) != HIGH;

        if (reading != lastState) lastDebounceTime = millis();

        if (millis() - lastDebounceTime > DEBOUNCE_TIME) {
            if (reading != currentState) {
                currentState = reading;
                if (!!currentState) {
                    lastPressTime = millis();
                    if (_downFunctionPointer) _downFunctionPointer();
                } else {
                    if (_upFunctionPointer) _upFunctionPointer();
                    resetPress(); // Do this after to allow for press checks on up function pointer
                }
            }
        }

        // Check Press Time
        if (!!currentState && !_isPressed && millis() - lastPressTime >= PRESS_TIME) {
            _isPressed = true;
            #ifdef SERIAL_DEBUG
            Serial.println("Button Short Press");
            #endif
            if (_pressFunctionPointer) _pressFunctionPointer();
        } else if (!!currentState && !_isLongPressed && millis() - lastPressTime >= LONG_PRESS_TIME) {
            _isLongPressed = true;
            #ifdef SERIAL_DEBUG
            Serial.println("Button Long Press");
            #endif
            if (_longPressFunctionPointer) _longPressFunctionPointer();
        }

        lastState = reading;
    }

    bool isDown() {
        return currentState;
    }
    bool isPressed() {
        return _isPressed;
    }
    bool isLongPressed() {
        return _isLongPressed;
    }
    void resetPress() {
        _isPressed = false;
        _isLongPressed = false;
    }

private:

    uint8_t _pin;
    bool currentState = false;
    bool lastState = false;
    unsigned long lastDebounceTime = 0;
    unsigned long lastPressTime = 0;
    bool _isPressed = false;
    bool _isLongPressed = false;

    ButtonFunctionPointer _downFunctionPointer;
    ButtonFunctionPointer _upFunctionPointer;
    ButtonFunctionPointer _pressFunctionPointer;
    ButtonFunctionPointer _longPressFunctionPointer;

};

#endif /* BUTTON_H_ */
