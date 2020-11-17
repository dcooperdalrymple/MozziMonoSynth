/**
 * Title: MozziMonoSynth
 * Version: 0.1.0
 * Author: D Cooper Dalrymple
 * Created: 11/04/2020
 * Updated: 11/17/2020
 * Description: 14-bit monophonic synthesizer using the Mozzi audio synthesis library. Best used for bass voices. Uses HiFi mode on Arduino Uno with hex inverter buffer and hardware low pass filter.
 * https://dcooperdalrymple.com/
 */

// Constants
//#define SERIAL_DEBUG

/// Hardware
#define STATUS_LED 13

/// Mozzi
#define CONTROL_RATE 256 // Defaults to 64

// Libraries
#ifndef SERIAL_DEBUG
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

#include <MozziGuts.h>
#include <mozzi_fixmath.h>
#include <mozzi_midi.h>
#include <Line.h>

// Global Constants
#include "Constants.h"

// Local Classes
#include "NoteBank.h"
#include "Controls.h"
#include "Voice.h"
#include "Program.h"

// Objects
NoteBank note_bank;
Voice voice;
Program program;

// Local Function Declarations
void loadProgramVoice(uint8_t index = 0);
void updateVoiceControls();

// Begin Setup

void setup() {

    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);

    Controls::setup();
    Controls::setUpdateHandler(updateVoiceControl);
    Controls::setWriteProgramCallback(writeProgramVoice);
    Controls::setReadProgramCallback(loadProgramVoice);

    voice.init();

    //loadProgramVoice(0);

    startMozzi(CONTROL_RATE);

    #ifndef SERIAL_DEBUG
    MIDI.setHandleNoteOn(receiveNoteOn);
    MIDI.setHandleNoteOff(receiveNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    #else
    Serial.begin(9600);
    Serial.println("MozziMonoSynth v0.1.0 - D Cooper Dalrymple 2020");
    #endif
}

void loop() {
    audioHook();
}

void updateControl() {

    #ifndef SERIAL_DEBUG
    // Flush Midi buffer
    while (MIDI.read()) { };
    #endif

    // Check for control updates and update voice with handler
    Controls::update();

    // Update control rate parameters of voice
    if (Controls::getState() != STATE_PROGRAM) voice.update();

    // LED display
    switch (Controls::getState()) {
        case STATE_PRIMARY:
            // Fade LED with LFO and Envelope
            Controls::setLed(LED_1_KEY, voice.getCurrentLFO());
            Controls::setLed(LED_2_KEY, voice.getCurrentGain());
        case STATE_SECONDARY:
            Controls::setLed(LED_1_KEY, LED_ON);
            Controls::setLed(LED_2_KEY, LED_OFF);
            break;
        case STATE_TERTIARY:
            Controls::setLed(LED_1_KEY, LED_OFF);
            Controls::setLed(LED_2_KEY, LED_ON);
            break;
        case STATE_PROGRAM:
            // Indicate the selected program
            uint8_t p = Controls::getSelectedProgram();
            Controls::setLed(LED_1_KEY, p & B10 ? LED_ON : LED_QUARTER);
            Controls::setLed(LED_2_KEY, p & B01 ? LED_ON : LED_QUARTER);
            break;
    }

}

int16_t updateAudio() {
    Controls::updateLeds();

    if (Controls::getState() == STATE_PROGRAM) {
        return 0; // Mute when reading/writing program
    } else {
        return voice.next();
    }
}

// Midi Callbacks

void receiveNoteOn(byte channel, byte note, byte velocity) {
    digitalWrite(STATUS_LED, HIGH);

    note_bank.removeNote(note); // Prevent duplicates
    note_bank.addNote(note);

    voice.noteOn(note_bank.getNote());
}

void receiveNoteOff(byte channel, byte note, byte velocity) {
    if (note_bank.removeNote(note) == false) return;
    if (note_bank.hasNote()) {
        voice.noteOn(note_bank.getNote());
    } else {
        digitalWrite(STATUS_LED, LOW);
        voice.noteOff();
    }
}

// Controls transfer to voice

void updateVoiceControl(uint8_t key, uint16_t value) {
    switch (key) {

        case OSC_1_WAVEFORM_KEY:
            voice.setWaveform((uint8_t)value);
            break;
        case LFO_AMOUNT_KEY:
            voice.setVibratoIntensity((uint8_t)value);
            break;

        case LPF_FREQUENCY_KEY:
            if (Controls::isUpdated(LPF_RESONANCE_KEY)) break;
        case LPF_RESONANCE_KEY:
            voice.setFilterFrequencyResonance((uint8_t)Controls::getPot(LPF_FREQUENCY_KEY), (uint8_t)Controls::getPot(LPF_RESONANCE_KEY));
            break;

        case ENV_1_ATTACK_KEY:
            if (Controls::isUpdated(ENV_1_RELEASE_KEY)) break;
        case ENV_1_RELEASE_KEY:
            voice.setEnvelopeAttackRelease(Controls::getPot(ENV_1_ATTACK_KEY), Controls::getPot(ENV_1_RELEASE_KEY));
            break;

    }
}

// Program transfer to controls and voice

void writeProgramVoice(uint8_t index) {
    for (uint8_t i = 0; i < CONTROLS_NUM_POTS; i++) {
        program.set(i, Controls::getPot(i, false));
    }
    program.write(index);
}
void loadProgramVoice(uint8_t index) {
    program.read(index);
    for (uint8_t i = 0; i < CONTROLS_NUM_POTS; i++) {
        Controls::setPot(i, program.get(i), true);
        updateVoiceControl(i, program.get(i));
    }
}
