/*
 * Controls.h
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "Pot.h"
#include "Led.h"

#define CONTROLS_NUM_BUTTONS 2
#define CONTROLS_NUM_LEDS 2
#define CONTROLS_NUM_POTS 6

#define BUTTON_1_PIN 12
#define BUTTON_2_PIN 8

#define LED_1_PIN 11
#define LED_1_KEY 0
#define LED_2_PIN 6
#define LED_2_KEY 1

#define OSC_WAVEFORM_KEY 0
#define OSC_WAVEFORM_PIN A5
#define LFO_GAIN_KEY 1
#define LFO_GAIN_PIN A4
#define LPF_FREQUENCY_KEY 2
#define LPF_FREQUENCY_PIN A3
#define LPF_RESONANCE_KEY 3
#define LPF_RESONANCE_PIN A2
#define ENV_ATTACK_KEY 4
#define ENV_ATTACK_PIN A1
#define ENV_RELEASE_KEY 5
#define ENV_RELEASE_PIN A0

class Controls {
public:

    virtual Controls() {}
    virtual ~Controls() {}

    void setup() {
        pinMode(BUTTON_1_PIN, INPUT_PULLUP);
        pinMode(BUTTON_2_PIN, INPUT_PULLUP);

        leds[LED_1_KEY]->setPin(LED_1_PIN);
        leds[LED_2_KEY]->setPin(LED_2_PIN);
        for (i = 0; i < CONTROLS_NUM_LEDS; i++) {
            leds[i]->set(0);
        }

        pots[OSC_WAVEFORM_KEY].setMapPointer(mapOscWaveform);
        //pots[LFO_GAIN_KEY].setMapPointer(mapOscVibrato);
        pots[LFO_GAIN_KEY].setMinMax(LFO_GAIN_MIN, LFO_GAIN_MAX);

        pots[LPF_FREQUENCY_KEY].setMinMax(LPF_FREQUENCY_MIN, LPF_FREQUENCY_MAX);
        pots[LPF_RESONANCE_KEY].setMinMax(LPF_RESONANCE_MIN, LPF_RESONANCE_MAX);

        pots[ENV_ATTACK_KEY].setMinMax(ENV_ATTACK_MIN, ENV_ATTACK_MAX);
        pots[ENV_RELEASE_KEY].setMinMax(ENV_RELEASE_MIN, ENV_RELEASE_MAX);
    }

    uint8_t updated[CONTROLS_NUM_POTS];
    bool update() {
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            pots[i].read();
        }
        return checkUpdated();
    }
    bool checkUpdated() {
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            updated[i] = 255;
        }
        j = 0;
        _updated = false;
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            if (isUpdated(i)) {
                updated[j++] = i;
                _updated = true;
            }
        }
        return _updated;
    }
    bool isUpdated(uint8_t key) {
        return pots[key].isUpdated();
    }

    uint16_t get(uint8_t key, bool mapped = true) {
        return pots[key].get(mapped);
    }
    void set(uint8_t key, uint16_t value) {
        pots[key].set(value);
        pots[key].lock();
    }

    void setLed(uint8_t key, uint8_t value) {
        leds[key]->set(value);
    }
    void updateLeds() {
        leds[LED_1_KEY]->update();
        leds[LED_2_KEY]->update();
    }

    static uint16_t mapOscWaveform(uint16_t value) {
        if (value > 767) {
            return 3;
        } else if (value < 256) {
            return 0;
        } else if (value > 511) {
            return 2;
        } else { // value > 255
            return 1;
        }
    }

    static uint16_t mapOscVibrato(uint16_t value) {
        if (value <= LFO_GAIN_GAP) return 0;
        return map(value - LFO_GAIN_GAP, POT_MIN, POT_MAX - LFO_GAIN_GAP, LFO_GAIN_MIN, LFO_GAIN_MAX);
    }

private:

    Pot pots[CONTROLS_NUM_POTS] = {
        Pot(OSC_WAVEFORM_PIN),
        Pot(LFO_GAIN_PIN),
        Pot(LPF_FREQUENCY_PIN),
        Pot(LPF_RESONANCE_PIN),
        Pot(ENV_ATTACK_PIN),
        Pot(ENV_RELEASE_PIN)
    };

    LedB led1;
    LedD led2;
    Led *leds[CONTROLS_NUM_LEDS] = {
        &led1,
        &led2
    };

    uint8_t i = 0, j = 0;
    bool _updated = false;

};

#endif /* CONTROLS_H_ */
