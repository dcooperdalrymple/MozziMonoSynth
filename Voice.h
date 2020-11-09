/*
 * Voice.h
 *
 */

#ifndef VOICE_H_
#define VOICE_H_

class Voice {
public:

  virtual Voice() {}
  virtual ~Voice() {}

  bool setOscWaveform(uint16_t value) {
    value = map(value, 0, 1023, 0, OSC_WAVEFORM_NUM - 1);
    if (oscWaveform != value) {
      oscWaveform = (uint8_t)value;
      return true;
    } else {
      return false;
    }
  }
  uint8_t getOscWaveform() {
    return oscWaveform;
  }

  bool setOscVibrato(uint16_t value) {
    value = map(value, 0, 1023, OSC_VIBRATO_MIN, OSC_VIBRATO_MAX);
    if (oscVibrato != value) {
      oscVibrato = (uint8_t)value;
      return true;
    } else {
      return false;
    }
  }
  uint8_t getOscVibrato() {
    return oscVibrato;
  }

  bool setLpfFrequency(uint16_t value) {
    value = map(value, 0, 1023, 0, 255);
    if (lpfFrequency != value) {
      lpfFrequency = (uint8_t)value;
      return true;
    } else {
      return false;
    }
  }
  uint8_t getLpfFrequency() {
    return lpfFrequency;
  }

  bool setLpfResonance(uint16_t value) {
    value = map(value, 0, 1023, 0, 255);
    if (lpfResonance != value) {
      lpfResonance = (uint8_t)value;
      return true;
    } else {
      return false;
    }
  }
  uint8_t getLpfResonance() {
    return lpfResonance;
  }

  bool setEnvAttack(uint16_t value) {
    if (envAttack != value) {
      envAttack = value;
      return true;
    } else {
      return false;
    }
  }
  uint16_t getEnvAttack() {
    return map(envAttack, 0, 1023, ENV_ATTACK_MIN, ENV_ATTACK_MAX);
  }

  bool setEnvRelease(uint16_t value) {
    if (envRelease != value) {
      envRelease = value;
      return true;
    } else {
      return false;
    }
  }
  uint16_t getEnvRelease() {
    return map(envRelease, 0, 1023, ENV_RELEASE_MIN, ENV_RELEASE_MAX);
  }

private:

  uint8_t oscWaveform;
  uint8_t oscVibrato;
  
  uint8_t lpfFrequency;
  uint8_t lpfResonance;

  uint16_t envAttack; // Use potentiometer values
  uint16_t envRelease;
  
};

#endif /* VOICE_H_ */
