/*
 * Controls.h
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "Pot.h"

#define OSC_OCTAVE_DEC_BUTTON 12
#define OSC_OCTAVE_INC_BUTTON 8
#define OSC_OCTAVE_LED_1 11
#define OSC_OCTAVE_LED_2 6

#define CONTROLS_NUM_POTS 6

#define OSC_WAVEFORM_KEY 0
#define OSC_WAVEFORM_PIN A5
#define OSC_VIBRATO_KEY 1
#define OSC_VIBRATO_PIN A4
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

    Controls() {
        pinMode(OSC_OCTAVE_DEC_BUTTON, INPUT_PULLUP);
        pinMode(OSC_OCTAVE_INC_BUTTON, INPUT_PULLUP);

        pinMode(OSC_OCTAVE_LED_1, OUTPUT);
        pinMode(OSC_OCTAVE_LED_2, OUTPUT);

        pots[OSC_WAVEFORM_KEY].setMapPointer(mapOscWaveform);
        pots[OSC_VIBRATO_KEY].setMapPointer(mapOscVibrato);

        pots[LPF_FREQUENCY_KEY].setMinMax(LPF_FREQUENCY_MIN, LPF_FREQUENCY_MAX);
        pots[LPF_RESONANCE_KEY].setMinMax(LPF_RESONANCE_MIN, LPF_RESONANCE_MAX);

        pots[ENV_ATTACK_KEY].setMinMax(ENV_ATTACK_MIN, ENV_ATTACK_MAX);
        pots[ENV_RELEASE_KEY].setMinMax(ENV_RELEASE_MIN, ENV_RELEASE_MAX);
    }

    virtual ~Controls() {}

    uint8_t updated[CONTROLS_NUM_POTS];
    bool update() {
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            pots[i].read();
        }
        return checkUpdated();
    }
    bool checkUpdated() {
        for (j = CONTROLS_NUM_POTS - 1; j >= 0; j++) {
            updated[j] = 255;
        }
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
        if (value <= OSC_VIBRATO_GAP) return 0;
        return map(value - OSC_VIBRATO_GAP, POT_MIN, POT_MAX - OSC_VIBRATO_GAP, OSC_VIBRATO_MIN, OSC_VIBRATO_MAX);
    }

private:

    Pot pots[CONTROLS_NUM_POTS] = {
        Pot(OSC_WAVEFORM_PIN),
        Pot(OSC_VIBRATO_PIN),
        Pot(LPF_FREQUENCY_PIN),
        Pot(LPF_RESONANCE_PIN),
        Pot(ENV_ATTACK_PIN),
        Pot(ENV_RELEASE_PIN)
    };
    uint8_t i = 0, j = 0;
    bool _updated = false;

};

#endif /* CONTROLS_H_ */
