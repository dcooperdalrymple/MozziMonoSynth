/*
 * Led.h
 * Description: Manually control LED PWM. Based on /examples/11.Communication/Sinewave_PWM_leds_HIFI/Sinewave_PWM_leds_HIFI.ino
 * Version: 0.1.0
 * Since: 0.1.0
 */

#ifndef LED_H_
#define LED_H_

#define LED_ON 255
#define LED_HALF 127
#define LED_QUARTER 63
#define LED_DIM 32
#define LED_OFF 0

class Led {
public:

    virtual Led() {}
    virtual ~Led() {}

    void set(uint8_t value) {
        _value = value;
    }
    uint8_t get() {
        return _value;
    }

    virtual void setPin(uint8_t pin) = 0;
    virtual void update() = 0;

protected:

    uint8_t _pin;
    uint8_t _value = 0;
    uint8_t _count = 0;

};

class LedD : public Led {
public:

    virtual void setPin(uint8_t pin) override {
        _pin = pin;
        pinMode(pin, OUTPUT);
    }

    virtual void update() override {
        (_count++ >= _value) ? PORTD &= ~(1 << _pin) : PORTD |= (1 << _pin);
    }

};

class LedB : public Led {
public:

    virtual void setPin(uint8_t pin) override {
        _pin = pin - 8;
        pinMode(pin, OUTPUT);
    }

    virtual void update() override {
        (_count++ >= _value) ? PORTB &= ~(1 << _pin) : PORTB |= (1 << _pin);
    }

};

#endif /* LED_H_ */
