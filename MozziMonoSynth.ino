/**
 * Title: MonoSynth
 * Author: D Cooper Dalrymple
 * Created: 11/04/2020
 * Updated: 11/04/2020
 * Description: 14-bit monophonic synthesizer using the Mozzi audio synthesis library. Best used for bass voices. Uses HiFi mode on Arduino Uno with hex inverter buffer and hardware low pass filter.
 * https://dcooperdalrymple.com/
 */

// Constants

/// Hardware

#define STATUS_LED 13

/// Mozzi

#define CONTROL_RATE 128 // Defaults to 64

// Libraries

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#include <MozziGuts.h>
#include <mozzi_fixmath.h>
#include <mozzi_midi.h>
#include <Line.h>

// Object Classes

#include "Voice.h" // Declared early for definitions
#include "control/NoteBank.h"
#include "control/Controls.h"
#include "Program.h"

// Objects

NoteBank note_bank;
Controls controls;
Voice voice;
Program program;

void setup() {
    MIDI.setHandleNoteOn(receiveNoteOn);
    MIDI.setHandleNoteOff(receiveNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);

    pinMode(STATUS_LED, OUTPUT);

    //controls = Controls();
    //voice = Voice();

    startMozzi(CONTROL_RATE);
}

void loop() {
    audioHook();
}

void updateControl() {

    // Flush Midi buffer
    while (MIDI.read()) { };

    // Transfer control updates to voice
    if (controls.update()) updateVoiceControls();

    // Update control rate parameters of voice
    voice.update();

    // Fade LED with LFO and Envelope
    analogWrite(OSC_OCTAVE_LED_1, voice.getCurrentLFO());
    analogWrite(OSC_OCTAVE_LED_2, voice.getCurrentGain());

}

int16_t updateAudio() {
    return voice.next();
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

void updateVoiceControls() {
    for (uint8_t i = 0; i < CONTROLS_NUM_POTS; i++) {
        if (controls.updated[i] >= CONTROLS_NUM_POTS) break;
        switch (controls.updated[i]) {

            case OSC_WAVEFORM_KEY:
                voice.setWaveform((uint8_t)controls.get(OSC_WAVEFORM_KEY));
                break;
            case OSC_VIBRATO_KEY:
                voice.setVibratoIntensity((uint8_t)controls.get(OSC_VIBRATO_KEY));
                break;

            case LPF_FREQUENCY_KEY:
                if (controls.isUpdated(LPF_RESONANCE_KEY)) break;
            case LPF_RESONANCE_KEY:
                voice.setFilterFrequencyResonance((uint8_t)controls.get(LPF_FREQUENCY_KEY), (uint8_t)controls.get(LPF_RESONANCE_KEY));
                break;

            case ENV_ATTACK_KEY:
                if (controls.isUpdated(ENV_RELEASE_KEY)) break;
            case ENV_RELEASE_KEY:
                voice.setEnvelopeAttackRelease(controls.get(ENV_ATTACK_KEY), controls.get(ENV_RELEASE_KEY));
                break;

        }
    }
}

// Program transfer to controls and voice

void loadProgramVoice(uint8_t index = 0) {
    program.read(index);
    for (uint8_t i = 0; i < CONTROLS_NUM_POTS; i++) {
        controls.set(i, program.get(i));
    }
    controls.checkUpdated();
    updateVoiceControls();
}
