/*
 * Controls.cpp
 * Version: 0.1.1
 * Since: 0.1.1
 */

#include "Constants.h"
#include "Controls.h"

Pot Controls::pots[CONTROLS_NUM_POTS] = {
    // Primary
    Pot(OSC_1_WAVEFORM_PIN),
    Pot(LFO_AMOUNT_PIN),
    Pot(LPF_FREQUENCY_PIN),
    Pot(LPF_RESONANCE_PIN),
    Pot(ENV_1_ATTACK_PIN),
    Pot(ENV_1_RELEASE_PIN),

    // Secondary
    Pot(OSC_2_WAVEFORM_PIN),
    Pot(LFO_FREQUENCY_PIN),
    Pot(ENV_2_AMOUNT_PIN),
    Pot(ENV_2_LEVEL_PIN),
    Pot(ENV_2_ATTACK_PIN),
    Pot(ENV_2_RELEASE_PIN),

    // Tertiary
    Pot(OSC_DETUNE_PIN),
    Pot(LFO_DESTINATION_PIN),
    Pot(OSC_MIX_PIN),
    Pot(OSC_TUNE_PIN),
    Pot(ENV_1_DECAY_PIN),
    Pot(ENV_2_DECAY_PIN)
};

LedB Controls::led1;
LedD Controls::led2;
Led *Controls::leds[CONTROLS_NUM_LEDS];

Button Controls::buttons[CONTROLS_NUM_BUTTONS] = {
    Button(BUTTON_1_PIN),
    Button(BUTTON_2_PIN)
};

UpdateFunctionPointer Controls::_updateFunctionPointer;
bool Controls::_updated = false;
uint8_t Controls::currentState = STATE_PRIMARY;

bool Controls::selectingProgram = false;
uint8_t Controls::selectedProgram = 0;
ProgramFunctionPointer Controls::_writeProgramPointer;
ProgramFunctionPointer Controls::_readProgramPointer;

void Controls::setup() {
    Pot::prepare();

    Controls::pots[OSC_1_WAVEFORM_KEY].setMapPointer(Controls::mapOscWaveform);
    Controls::pots[OSC_2_WAVEFORM_KEY].setMapPointer(Controls::mapOscWaveform);
    Controls::pots[OSC_MIX_KEY].setMinMax(OSC_MIX_MIN, OSC_MIX_MAX);

    //Controls::pots[LFO_AMOUNT_KEY].setMapPointer(Controls::mapLfoGain);
    Controls::pots[LFO_AMOUNT_KEY].setMinMax(LFO_AMOUNT_MIN, LFO_AMOUNT_MAX);

    Controls::pots[LPF_FREQUENCY_KEY].setMinMax(LPF_FREQUENCY_MIN, LPF_FREQUENCY_MAX);
    Controls::pots[LPF_RESONANCE_KEY].setMinMax(LPF_RESONANCE_MIN, LPF_RESONANCE_MAX);

    Controls::pots[ENV_1_ATTACK_KEY].setMinMax(ENV_ATTACK_MIN, ENV_ATTACK_MAX);
    Controls::pots[ENV_1_RELEASE_KEY].setMinMax(ENV_RELEASE_MIN, ENV_RELEASE_MAX);
    Controls::pots[ENV_1_DECAY_KEY].setMinMax(ENV_DECAY_MIN, ENV_DECAY_MAX);

    Controls::pots[ENV_2_AMOUNT_KEY].setMinMax(ENV_AMOUNT_MIN, ENV_AMOUNT_MAX);
    Controls::pots[ENV_2_LEVEL_KEY].setMinMax(ENV_DECAY_LEVEL_MIN, ENV_DECAY_LEVEL_MAX);
    Controls::pots[ENV_2_ATTACK_KEY].setMinMax(ENV_ATTACK_MIN, ENV_ATTACK_MAX);
    Controls::pots[ENV_2_RELEASE_KEY].setMinMax(ENV_RELEASE_MIN, ENV_RELEASE_MAX);
    Controls::pots[ENV_2_DECAY_KEY].setMinMax(ENV_DECAY_MIN, ENV_DECAY_MAX);

    Controls::leds[LED_1_KEY]->setPin(LED_1_PIN);
    Controls::leds[LED_2_KEY]->setPin(LED_2_PIN);
    for (uint8_t i = 0; i < CONTROLS_NUM_LEDS; i++) {
        Controls::leds[i]->set(0);
    }

    Controls::buttons[BUTTON_1_KEY].setDownHandler(Controls::setSecondaryState);
    Controls::buttons[BUTTON_1_KEY].setUpHandler(Controls::setPrimaryState);
    Controls::buttons[BUTTON_1_KEY].setPressHandler(Controls::setProgramState);

    Controls::buttons[BUTTON_2_KEY].setDownHandler(Controls::setTertiaryState);
    Controls::buttons[BUTTON_2_KEY].setUpHandler(Controls::setPrimaryState);
    Controls::buttons[BUTTON_2_KEY].setPressHandler(Controls::setProgramState);

    Controls::selectedProgram = 0;
    Controls::setState(STATE_PRIMARY, true, true);
}

bool Controls::update() {
    uint8_t i;

    for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
        Controls::buttons[i].update();
    }

    if (Controls::currentState >= CONTROLS_NUM_STATES) return; // Check if we're in program mode

    Controls::_updated = false;
    for (i = STATE_NUM_POTS * Controls::currentState; i < STATE_NUM_POTS * (Controls::currentState + 1); i++) {
        if (!Controls::pots[i].read()) continue;
        Controls::_updated = true;
        if (Controls::_updateFunctionPointer) Controls::_updateFunctionPointer(i, Controls::pots[i].get());
    }

    return _updated;
}
bool Controls::isUpdated(uint8_t key) {
    return Controls::pots[key].isUpdated();
}
void Controls::setUpdateHandler(UpdateFunctionPointer updateFunctionPointer) {
    Controls::_updateFunctionPointer = updateFunctionPointer;
}

uint16_t Controls::getPot(uint8_t key, bool mapped = true) {
    return Controls::pots[key].get(mapped);
}
void Controls::setPot(uint8_t key, uint16_t value, bool lock = true) {
    Controls::pots[key].set(value);
    if (!!lock) Controls::pots[key].lock();
}

void Controls::setLed(uint8_t key, uint8_t value) {
    Controls::leds[key]->set(value);
}
void Controls::updateLeds() {
    Controls::leds[LED_1_KEY]->update();
    Controls::leds[LED_2_KEY]->update();
}

uint16_t Controls::mapOscWaveform(uint16_t value) {
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

uint16_t Controls::mapLfoGain(uint16_t value) {
    if (value <= LFO_AMOUNT_GAP) return 0;
    return map(value - LFO_AMOUNT_GAP, POT_MIN, POT_MAX - LFO_AMOUNT_GAP, LFO_AMOUNT_MIN, LFO_AMOUNT_MAX);
}

void Controls::setPrimaryState() {
    if (Controls::currentState == STATE_PROGRAM) {
        // Make sure that we didn't just get into the program state
        if (Controls::selectingProgram == true) {
            Controls::selectingProgram = false;
            return;
        }

        // Check that both buttons are pressed and not long pressed
        for (uint8_t i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            if (!Controls::buttons[i].isPressed() || Controls::buttons[i].isLongPressed()) return;
        }

        if (Controls::_readProgramPointer) Controls::_readProgramPointer(selectedProgram);
    }

    Controls::setState(STATE_PRIMARY);
}
void Controls::setSecondaryState() {
    if (Controls::currentState == STATE_PROGRAM) return;
    Controls::setState(STATE_SECONDARY);
}
void Controls::setTertiaryState() {
    if (Controls::currentState == STATE_PROGRAM) return;
    Controls::setState(STATE_TERTIARY);
}

void Controls::setProgramState() {
    if (Controls::currentState == STATE_PROGRAM) {
        // Change selected program
        if (Controls::buttons[BUTTON_1_KEY].isPressed() && !Controls::buttons[BUTTON_2_KEY].isDown()) {
            if (Controls::selectedProgram > 0) Controls::selectedProgram--;
        } else if (!Controls::buttons[BUTTON_1_KEY].isDown() && Controls::buttons[BUTTON_2_KEY].isPressed()) {
            if (Controls::selectedProgram < PROGRAM_COUNT - 1) Controls::selectedProgram++;
        }
        return;
    }

    // Check if all buttons are pressed
    for (uint8_t i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
        if (!Controls::buttons[i].isPressed()) return;
    }

    Controls::selectingProgram = true;
    Controls::setState(STATE_PROGRAM, false);
}
void Controls::checkWriteProgram() {
    if (Controls::currentState != STATE_PROGRAM) return;

    // Check if all buttons long pressed
    for (uint8_t i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
        if (!Controls::buttons[i].isLongPressed()) return;
    }

    if (Controls::_writeProgramPointer) Controls::_writeProgramPointer(selectedProgram);
}
uint8_t Controls::getSelectedProgram() {
    return Controls::selectedProgram;
}
void Controls::setWriteProgramCallback(ProgramFunctionPointer writeProgramPointer) {
    Controls::_writeProgramPointer = writeProgramPointer;
}
void Controls::setReadProgramCallback(ProgramFunctionPointer readProgramPointer) {
    Controls::_readProgramPointer = readProgramPointer;
}

bool Controls::setState(uint8_t state, bool unlock = false, bool force = false) {
    uint8_t i;

    if (!force && Controls::currentState == state) return false;
    Controls::currentState = state;

    // Lock all pots
    for (i = 0; i < CONTROLS_NUM_POTS; i++) {
        Controls::pots[i].lock();
    }

    // Only unlock those in current state
    if (!!unlock) {
        for (i = STATE_NUM_POTS * Controls::currentState; i < STATE_NUM_POTS * (Controls::currentState + 1); i++) {
            Controls::pots[i].unlock();
        }
    }
}
uint8_t Controls::getState() {
    return Controls::currentState;
}
