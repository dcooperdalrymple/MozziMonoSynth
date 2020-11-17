/*
 * Constants.h
 * Version: 0.1.1
 * Since: 0.1.1
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*************
 * Voice     *
 *************/

// Oscillator
#define OSC_NUM_CELLS SIN1024_NUM_CELLS
#define OSC_1_WAVEFORM_NUM 4

// LFO
#define LFO_AMOUNT_GAP 20
#define LFO_AMOUNT_MIN 0
#define LFO_AMOUNT_MAX 255
#define LFO_NUM_CELLS SIN1024_NUM_CELLS
const float lfoFrequency = 6.f;

// Low Pass Filter
#define LPF_FREQUENCY_MIN 20
#define LPF_FREQUENCY_MAX 255
#define LPF_RESONANCE_MIN 0
#define LPF_RESONANCE_MAX 170

// Envelope
#define ENV_1_ATTACK_LEVEL 255
#define ENV_DECAY_LEVEL 255
#define ENV_DECAY_TIME 50
#define ENV_SUSTAIN_TIME 60000
#define ENV_1_ATTACK_MIN 20
#define ENV_1_ATTACK_MAX 2000
#define ENV_1_RELEASE_MIN 20
#define ENV_1_RELEASE_MAX 4000

/*************
 * Note Bank *
 *************/

#define NOTE_BANK_SIZE 32

/*************
 * Controls  *
 *************/

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

// Primary Pots

#define OSC_1_WAVEFORM_KEY 0
#define OSC_1_WAVEFORM_PIN POT_1_PIN
#define OSC_1_WAVEFORM_STATE STATE_PRIMARY

#define LFO_AMOUNT_KEY 1
#define LFO_AMOUNT_PIN POT_2_PIN
#define LFO_AMOUNT_STATE STATE_PRIMARY

#define LPF_FREQUENCY_KEY 2
#define LPF_FREQUENCY_PIN POT_3_PIN
#define LPF_FREQUENCY_STATE STATE_PRIMARY

#define LPF_RESONANCE_KEY 3
#define LPF_RESONANCE_PIN POT_4_PIN
#define LPF_RESONANCE_STATE STATE_PRIMARY

#define ENV_1_ATTACK_KEY 4
#define ENV_1_ATTACK_PIN POT_5_PIN
#define ENV_1_ATTACK_STATE STATE_PRIMARY

#define ENV_1_RELEASE_KEY 5
#define ENV_1_RELEASE_PIN POT_6_PIN
#define ENV_1_RELEASE_STATE STATE_PRIMARY

// Secondary Knobs

#define OSC_2_WAVEFORM_KEY 6
#define OSC_2_WAVEFORM_PIN POT_1_PIN
#define OSC_2_WAVEFORM_STATE STATE_SECONDARY

#define LFO_FREQUENCY_KEY 7
#define LFO_FREQUENCY_PIN POT_2_PIN
#define LFO_FREQUENCY_STATE STATE_SECONDARY

#define ENV_2_AMOUNT_KEY 8
#define ENV_2_AMOUNT_PIN POT_3_PIN
#define ENV_2_AMOUNT_STATE STATE_SECONDARY

#define ENV_2_LEVEL_KEY 9
#define ENV_2_LEVEL_PIN POT_4_PIN
#define ENV_2_LEVEL_STATE STATE_SECONDARY

#define ENV_2_ATTACK_KEY 10
#define ENV_2_ATTACK_PIN POT_5_PIN
#define ENV_2_ATTACK_STATE STATE_SECONDARY

#define ENV_2_RELEASE_KEY 11
#define ENV_2_RELEASE_PIN POT_6_PIN
#define ENV_2_RELEASE_STATE STATE_SECONDARY

// Tertiary Knobs

#define OSC_DETUNE_KEY 12
#define OSC_DETUNE_PIN POT_1_PIN
#define OSC_DETUNE_STATE STATE_TERTIARY

#define LFO_DESTINATION_KEY 13
#define LFO_DESTINATION_PIN POT_2_PIN
#define LFO_DESTINATION_STATE STATE_TERTIARY

#define OSC_MIX_KEY 14
#define OSC_MIX_PIN POT_3_PIN
#define OSC_MIX_STATE STATE_TERTIARY

#define OSC_TUNE_KEY 15
#define OSC_TUNE_PIN POT_4_PIN
#define OSC_TUNE_STATE STATE_TERTIARY

#define ENV_1_DECAY_KEY 16
#define ENV_1_DECAY_PIN POT_5_PIN
#define ENV_1_DECAY_STATE STATE_TERTIARY

#define ENV_2_DECAY_KEY 17
#define ENV_2_DECAY_PIN POT_6_PIN
#define ENV_2_DECAY_STATE STATE_TERTIARY

/*************
 * Program   *
 *************/

#define PROGRAM_COUNT 4
#define PROGRAM_DATA_SIZE (CONTROLS_NUM_POTS*2)

#endif /* CONSTANTS_H_ */
