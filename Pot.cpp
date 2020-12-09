/*
 * Pot.cpp
 * Version: 0.1.1
 * Since: 0.1.1
 */

#include "Constants.h"
#include "Pot.h"

Smooth <uint16_t> Pot::smooth[STATE_NUM_POTS] = {
    Smooth<uint16_t>(),
    Smooth<uint16_t>(),
    Smooth<uint16_t>(),
    Smooth<uint16_t>(),
    Smooth<uint16_t>(),
    Smooth<uint16_t>()
};

void Pot::prepare() {
    for (uint8_t i = 0; i < STATE_NUM_POTS; i++) {
        Pot::smooth[i].setSmoothness(potSmoothness);
    }
    for (uint8_t i = A0; i <= A5; i++) {
        pinMode(i, INPUT);
    }
}

uint16_t Pot::nextSmooth(uint8_t pin, uint16_t value) {
    return Pot::smooth[pin - A0].next(value);
}
