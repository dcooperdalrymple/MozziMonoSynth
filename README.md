# MozziMonoSynth
14-bit monophonic synthesizer using the [Mozzi sound synthesis library](https://github.com/sensorium/Mozzi). 3d-printable enclosure included!

#### Features Included:
- 16-bit digital signal processing internally
- 14-bit audio output using Mozzi HIFI mode
- Audio output buffering using MC14069UB hex inverter
- LM386 mono amplifier for headphones or internal speaker _(shutoff switch included for speaker)_
- Hardware Midi input using 6N138 optocoupler

<p float="left">
  <img alt="Early version of faceplate" src="/images/front-early.jpg" width="320" />
  <img alt="View of enclosure in FreeCAD" src="/images/enclosure.jpg" width="320" />
</p>

> Still in early development, but more details and schematics to come soon.

#### Control Mapping

| Knob | Mode 1                | Mode 2 (left)         | Mode 3 (right)    |
| :--: | --------------------- | --------------------- | ----------------- |
| 1    | Oscillator 1 Waveform | Oscillator 2 Waveform | Oscillator Detune |
| 2    | LFO Amount            | LFO Frequency         | LFO Destination   |
| 3    | Filter Frequency      | Envelope Amount       | Oscillator Mix    |
| 4    | Filter Resonance      | Envelope Level        | Oscillator Tune   |
| 5    | Amp Attack            | Envelope Attack       | Amp Decay         |
| 6    | Amp Release           | Envelope Release      | Envelope Decay    |
