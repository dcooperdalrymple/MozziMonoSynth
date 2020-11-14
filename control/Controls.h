/*
 * Controls.h
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "Pot.h"
#include "Button.h"
#include "Led.h"

#define STATE_PRIMARY 0
#define STATE_SECONDARY 1
#define STATE_TERTIARY 2
#define STATE_PROGRAM 3
#define STATE_NUM_POTS 6

#define CONTROLS_NUM_LEDS 2
#define CONTROLS_NUM_BUTTONS 2
#define CONTROLS_NUM_STATES 3 // Not including program state
#define CONTROLS_NUM_POTS STATE_NUM_POTS*CONTROLS_NUM_STATES

#define BUTTON_1_KEY 0
#define BUTTON_1_PIN 12
#define BUTTON_2_KEY 1
#define BUTTON_2_PIN 8

#define LED_1_KEY 0
#define LED_1_PIN 11
#define LED_2_KEY 1
#define LED_2_PIN 6

#define POT_1_PIN A5
#define POT_2_PIN A4
#define POT_3_PIN A3
#define POT_4_PIN A2
#define POT_5_PIN A1
#define POT_6_PIN A0

#define OSC_WAVEFORM_KEY 0
#define OSC_WAVEFORM_PIN POT_1_PIN
#define OSC_WAVEFORM_STATE STATE_PRIMARY

#define LFO_GAIN_KEY 1
#define LFO_GAIN_PIN POT_2_PIN
#define LFO_GAIN_STATE STATE_PRIMARY

#define LPF_FREQUENCY_KEY 2
#define LPF_FREQUENCY_PIN POT_3_PIN
#define LPF_FREQUENCY_STATE STATE_PRIMARY

#define LPF_RESONANCE_KEY 3
#define LPF_RESONANCE_PIN POT_4_PIN
#define LPF_RESONANCE_STATE STATE_PRIMARY

#define ENV_ATTACK_KEY 4
#define ENV_ATTACK_PIN POT_5_PIN
#define ENV_ATTACK_STATE STATE_PRIMARY

#define ENV_RELEASE_KEY 5
#define ENV_RELEASE_PIN POT_6_PIN
#define ENV_RELEASE_STATE STATE_PRIMARY

typedef void (*ProgramFunctionPointer)(uint8_t);

class Controls {
public:

    virtual Controls() {}
    virtual ~Controls() {}

    void setup() {
        pots[OSC_WAVEFORM_KEY].setMapPointer(mapOscWaveform);
        //pots[LFO_GAIN_KEY].setMapPointer(mapLfoGain);
        pots[LFO_GAIN_KEY].setMinMax(LFO_GAIN_MIN, LFO_GAIN_MAX);

        pots[LPF_FREQUENCY_KEY].setMinMax(LPF_FREQUENCY_MIN, LPF_FREQUENCY_MAX);
        pots[LPF_RESONANCE_KEY].setMinMax(LPF_RESONANCE_MIN, LPF_RESONANCE_MAX);

        pots[ENV_ATTACK_KEY].setMinMax(ENV_ATTACK_MIN, ENV_ATTACK_MAX);
        pots[ENV_RELEASE_KEY].setMinMax(ENV_RELEASE_MIN, ENV_RELEASE_MAX);

        leds[LED_1_KEY]->setPin(LED_1_PIN);
        leds[LED_2_KEY]->setPin(LED_2_PIN);
        for (i = 0; i < CONTROLS_NUM_LEDS; i++) {
            leds[i]->set(0);
        }

        buttons[BUTTON_1_KEY].setDownHandler(setSecondaryState);
        buttons[BUTTON_1_KEY].setUpHandler(setPrimaryState);
        buttons[BUTTON_1_KEY].setPressHandler(setProgramState);

        buttons[BUTTON_2_KEY].setDownHandler(setTertiaryState);
        buttons[BUTTON_2_KEY].setUpHandler(setPrimaryState);
        buttons[BUTTON_2_KEY].setPressHandler(setProgramState);

        setState(STATE_PRIMARY, true);
    }

    uint8_t updated[STATE_NUM_POTS];
    bool update() {
        for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            buttons[i].update();
        }

        if (currentState > CONTROLS_NUM_STATES) return; // Check if we're in program mode

        for (i = STATE_NUM_POTS * currentState; i < STATE_NUM_POTS * (currentState + 1); i++) {
            pots[i].read();
        }

        return checkUpdated();
    }
    bool checkUpdated() {
        for (i = 0; i < STATE_NUM_POTS; i++) {
            updated[i] = 255;
        }
        j = 0;
        _updated = false;
        for (i = STATE_NUM_POTS * currentState; i < STATE_NUM_POTS * (currentState + 1); i++) {
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

    uint16_t getPot(uint8_t key, bool mapped = true) {
        return pots[key].get(mapped);
    }
    void setPot(uint8_t key, uint16_t value, bool lock = true) {
        pots[key].set(value);
        if (!!lock) pots[key].lock();
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

    static uint16_t mapLfoGain(uint16_t value) {
        if (value <= LFO_GAIN_GAP) return 0;
        return map(value - LFO_GAIN_GAP, POT_MIN, POT_MAX - LFO_GAIN_GAP, LFO_GAIN_MIN, LFO_GAIN_MAX);
    }

    void setPrimaryState() {
        if (currentState == STATE_PROGRAM)  {
            // Check that both buttons are pressed
            for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
                if (!buttons[i].isPressed()) return;
            }
            if (_readProgramPointer) _readProgramPointer(selectedProgram);
        }
        setState(STATE_PRIMARY);
    }
    void setSecondaryState() {
        setState(STATE_SECONDARY);
    }
    void setTertiaryState() {
        setState(STATE_TERTIARY);
    }

    void setProgramState() {
        if (currentState == STATE_PROGRAM) {
            // Change selected program
            if (buttons[BUTTON_1_KEY].isPressed() && !buttons[BUTTON_2_KEY].isPressed()) {
                if (selectedProgram > 0) selectedProgram--;
            } else if (!buttons[BUTTON_1_KEY].isPressed() && buttons[BUTTON_2_KEY].isPressed()) {
                if (selectedProgram < PROGRAM_COUNT - 1) selectedProgram++;
            }
            return;
        }

        // Check if all buttons are pressed
        for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            if (!buttons[i].isPressed()) return;
        }

        setState(STATE_PROGRAM, false);
    }
    void checkWriteProgram() {
        if (currentState != STATE_PROGRAM) return;

        // Check if all buttons long pressed
        for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            if (!buttons[i].isLongPressed()) return;
        }

        if (_writeProgramPointer) _writeProgramPointer(selectedProgram);
    }
    uint8_t getCurrentProgram() {
        return currentProgram;
    }
    void setWriteProgramPointer(ProgramFunctionPointer writeProgramPointer) {
        _writeProgramPointer = writeProgramPointer;
    }
    void setReadProgramPointer(ProgramFunctionPointer) {
        _readProgramPointer = readProgramPointer;
    }

    bool setState(uint8_t state, bool unlock = false) {
        if (currentState == state) return false;
        currentState = state;

        // Lock all pots
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            pots[i].lock();
        }

        // Only unlock those in current state
        if (!!unlock) {
            for (i = STATE_NUM_POTS * currentState; i < STATE_NUM_POTS * (currentState + 1); i++) {
                pots[i].unlock();
            }
        }
    }
    uint8_t getState() {
        return currentState;
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

    Button[CONTROLS_NUM_BUTTONS] buttons = {
        Button(BUTTON_1_PIN),
        Button(BUTTON_2_PIN)
    };

    uint8_t i = 0, j = 0;
    bool _updated = false;
    uint8_t currentState = STATE_PRIMARY;

    // Variables for programming
    uint8_t selectedProgram = 0;
    ProgramFunctionPointer _writeProgramPointer;
    ProgramFunctionPoitner _readProgramPointer;

};

#endif /* CONTROLS_H_ */
