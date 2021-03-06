/*
 * Pot.h
 * Version: 0.1.0
 * Since: 0.1.0
 */

#ifndef POT_H_
#define POT_H_

#include <mozzi_analog.h>
//#include <RollingAverage.h>
#include <Smooth.h>

#define POT_LOCK_RADIUS 16
#define POT_AVERAGE_WINDOW 32
const float potSmoothness = 0.5f; // 0.96f;
#define POT_MIN 0
#define POT_MAX 1023

typedef uint16_t (*MapFunctionPointer)(uint16_t);

class Pot {
public:

    Pot(uint8_t pin) {
        _pin = pin;
    }
    virtual ~Pot() {}

    void setMapPointer(MapFunctionPointer mapFunctionPointer = NULL) {
        _mapFunctionPointer = mapFunctionPointer;
    }

    bool read() {
        updated = false;

        uint16_t raw = mozziAnalogRead(_pin);
        raw = Pot::nextSmooth(_pin, raw);

        if (locked && (
            (_value <= POT_MIN + POT_LOCK_RADIUS * 2 && raw < POT_MIN + POT_LOCK_RADIUS * 2) ||
            (_value >= POT_MAX - POT_LOCK_RADIUS * 2 && raw > POT_MAX - POT_LOCK_RADIUS * 2) ||
            (raw >= _value - POT_LOCK_RADIUS && raw <= _value + POT_LOCK_RADIUS))) {
            locked = false;
        }
        if (locked) return false;

        if (_value == raw) {
            return false;
        } else {
            set(raw);
            return true;
        }
    }

    // Override analog read
    void set(uint16_t value) {
        _value = value;
        updated = true;
    }

    uint16_t get(bool mapped = true) {
        if (!!mapped) {
            if (_mapFunctionPointer) {
                return _mapFunctionPointer(_value);
            } else {
                return map(_value, POT_MIN, POT_MAX, _min, _max);
            }
        } else {
            return _value;
        }
    }

    void setMinMax(uint16_t min = POT_MIN, uint16_t max = POT_MAX) {
        _min = min;
        _max = max;
    }

    void lock() {
        locked = true;
    }
    void unlock() {
        locked = false;
    }
    bool isLocked() {
        return !!locked;
    }

    bool isUpdated() {
        return !!updated;
    }

    static void prepare();
    static uint16_t nextSmooth(uint8_t pin, uint16_t value);

private:

    uint8_t _pin;
    uint16_t _value = 0;
    bool locked = false;
    bool updated = false;

    uint16_t _min = POT_MIN, _max = POT_MAX;

    MapFunctionPointer _mapFunctionPointer;

    static Smooth <uint16_t> smooth[STATE_NUM_POTS];

};

#endif /* POT_H_ */
