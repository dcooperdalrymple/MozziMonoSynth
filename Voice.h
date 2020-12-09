/*
 * Voice.h
 * Version: 0.1.1
 * Since: 0.1.0
 */

#ifndef VOICE_H_
#define VOICE_H_

// Components
#include <Oscil.h>
#include <LowPassFilter.h>
#include <ADSR.h> // Line.h required

// Wavetables
#include <tables/sin1024_int8.h>
#include <tables/triangle1024_int8.h>
#include <tables/saw1024_int8.h>
#include "tables/square1024_int8.h"

#define VOICE_NUM_OSC 2
#define VOICE_NUM_ENV 2

class Voice {
public:

    virtual Voice() {}
    virtual ~Voice() {}

    void init() {
        for (uint8_t i = 0; i < VOICE_NUM_OSC; i++) {
            setOscillatorWaveform(0, i);
            oscillator[i].setPhase(0);
        }
        oscillator_mix = OSC_MIX_MID;
        setOscillatorTune(1.f);
        setOscillatorDetune(oscDetuneMin);
        setOscillatorFrequency(440.f);

        lfo.setTable(SIN1024_DATA);
        lfo.setPhase(0);
        lfo.setFreq(lfoFrequencyMin);
        lfo_amount = 0;

        for (uint8_t i = 0; i < VOICE_NUM_ENV; i++) {
            envelope[i].setADLevels(ENV_ATTACK_LEVEL, ENV_DECAY_LEVEL);
            envelope[i].setTimes(ENV_ATTACK_MIN, ENV_DECAY_TIME, ENV_SUSTAIN_TIME, ENV_RELEASE_MIN);
            envelope_gain[i] = 0;
            envelope_level[i] = 255;
        }

        filter.setCutoffFreqAndResonance(LPF_FREQUENCY_MAX, LPF_RESONANCE_MIN);
    }

    void setOscillatorWaveform(uint8_t osc_num, uint8_t table) {
        switch (table) {
            case 0:
                oscillator[osc_num].setTable(SIN1024_DATA);
                break;
            case 1:
                oscillator[osc_num].setTable(TRIANGLE1024_DATA);
                break;
            case 2:
                oscillator[osc_num].setTable(SAW1024_DATA);
                break;
            case 3:
                oscillator[osc_num].setTable(SQUARE1024_DATA);
                break;
        }
    }
    void setOscillatorMix(uint8_t mix) {
        oscillator_mix = mix;
    }
    void setOscillatorTune(uint16_t value) {
        oscillator_tune = ((float)value - (float)POT_MIN) * (oscTuneMax - oscTuneMin) / ((float)POT_MAX - (float)POT_MIN) + oscTuneMin;
        setOscillatorFrequency(oscillator_freq);
    }
    void setOscillatorDetune(uint16_t value) {
        oscillator_detune = ((float)value - (float)POT_MIN) * (oscDetuneMax - oscDetuneMin) / ((float)POT_MAX - (float)POT_MIN) + oscDetuneMin;
        setOscillatorFrequency(oscillator_freq);
    }
    void setOscillatorFrequency(float freq) {
        oscillator_freq = freq;
        freq = freq * oscillator_tune;
        oscillator[0].setFreq(freq * (1.f - oscillator_detune));
        oscillator[1].setFreq(freq * (1.f + oscillator_detune));
    }

    void setLfoAmount(uint8_t amount) {
        lfo_amount = amount;
    }
    void setLfoFrequency(uint16_t amount) { // 0 -> 1024
        lfo.setFreq(((float)amount - (float)POT_MIN) * (lfoFrequencyMax - lfoFrequencyMin) / ((float)POT_MAX - (float)POT_MIN) + lfoFrequencyMin);
    }

    void setFilterFrequencyResonance(uint8_t frequency, uint8_t resonance, bool force = false) {
        if (_frequency == frequency && _resonance == resonance && !force) return;
        _frequency = frequency;
        _resonance = resonance;

        filter.setCutoffFreqAndResonance(frequency, resonance);
    }

    void setEnvelopeTimes(uint8_t env_num, uint16_t attackTime, uint16_t releaseTime, uint16_t decayTime = ENV_DECAY_TIME) {
        envelope[env_num].setTimes(attackTime, decayTime, ENV_SUSTAIN_TIME, releaseTime);
        envelope[env_num].update(); // Allows for mid-transition updates
    }
    void setEnvelopeLevel(uint8_t env_num, uint8_t decay_level) {
        envelope[env_num].setADLevels(ENV_ATTACK_LEVEL, decay_level);
        envelope[env_num].update();
    }
    void setEnvelopeAmount(uint8_t env_num, uint8_t amount) {
        envelope_level[env_num] = amount;
    }

    void noteOn(uint8_t note) {
        setOscillatorFrequency(mtof(float(note)));
        for (uint8_t i = 0; i < VOICE_NUM_OSC; i++) {
            envelope[i].noteOn(false); // reset = true
        }
    }
    void noteOff() {
        for (uint8_t i = 0; i < VOICE_NUM_OSC; i++) {
            envelope[i].noteOff(); // reset = true
        }
    }

    void update() {
        // Envelope
        for (uint8_t i = 0; i < VOICE_NUM_OSC; i++) {
            envelope[i].update();
            envelope_gain[i] = envelope[i].next();
        }

        // Vibrato LFO
        lfo_current = (int16_t)lfo_amount * lfo.next();
    }

    uint8_t getCurrentLFO() {
        return (uint8_t)map(lfo_current, -32768, 32767, 0, 255);
    }
    uint8_t getCurrentGain() {
        return envelope_gain[0];
    }

    int16_t next() {
        // 16-bit -> 14-bit & extra two bits to prevent distortion from filter and oscillator summing

        oscillator_current[0] = oscillator[0].phMod((Q15n16)lfo_current) * (OSC_MIX_MAX - oscillator_mix) >> 8;
        oscillator_current[1] = oscillator[1].phMod((Q15n16)lfo_current) * oscillator_mix >> 8;

        return (int16_t)((int32_t)(envelope_gain[0] * filter.next((oscillator_current[0] + oscillator_current[1]) >> 2)) >> 2);
    }

private:

    Oscil <OSC_NUM_CELLS, AUDIO_RATE> oscillator[VOICE_NUM_OSC];
    uint8_t oscillator_mix;
    int16_t oscillator_current[VOICE_NUM_OSC];
    float oscillator_freq, oscillator_tune, oscillator_detune;
    Oscil <LFO_NUM_CELLS, CONTROL_RATE> lfo;
    int16_t lfo_current;
    uint8_t lfo_amount;
    LowPassFilter filter;
    ADSR <CONTROL_RATE, CONTROL_RATE> envelope[VOICE_NUM_ENV];
    uint8_t envelope_gain[VOICE_NUM_ENV];
    uint8_t envelope_level[VOICE_NUM_ENV];

    // Setters
    uint8_t _frequency, _resonance;

};

#endif /* VOICE_H_ */
