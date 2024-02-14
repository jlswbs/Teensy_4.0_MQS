// Melodic noodle 03 - random sequence with modulated delay //

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "AudioSampleKick.h"

#define BPM   120

AudioSynthWaveform       gen;
AudioSynthWaveform       lfo;
AudioPlayMemory          kick;
AudioSynthNoisePink      pink;
AudioSynthWaveformSine   sine;
AudioSynthWaveformSineModulated sine_fm;
AudioEffectMultiply      multiply;
AudioEffectEnvelope      envelope2;
AudioEffectEnvelope      envelope1;
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioFilterStateVariable filter;
AudioEffectDelay         delay1;
AudioOutputMQS           out;

AudioConnection          patchCord0(filter, 0, mixer2, 2);
AudioConnection          patchCord1(kick, 0, mixer2, 1);
AudioConnection          patchCord2(pink, envelope2);
AudioConnection          patchCord3(sine, 0, multiply, 0);
AudioConnection          patchCord4(sine, sine_fm);
AudioConnection          patchCord5(sine_fm, 0, multiply, 1);
AudioConnection          patchCord6(multiply, envelope1);
AudioConnection          patchCord7(envelope2, 0, mixer1, 2);
AudioConnection          patchCord8(envelope1, 0, mixer1, 0);
AudioConnection          patchCord9(mixer2, 0, out, 0);
AudioConnection          patchCord10(mixer1, delay1);
AudioConnection          patchCord11(delay1, 0, mixer1, 1);
AudioConnection          patchCord12(gen, 0, filter, 0);
AudioConnection          patchCord13(lfo, 0, filter, 1);
AudioConnection          patchCord14(mixer1, 0, mixer2, 0);

bool d = true;

void setup() {
  
  AudioMemory(30);

  mixer1.gain(0,0.6);
  mixer1.gain(1,0.8);
  mixer1.gain(2,0.6);
  
  mixer2.gain(0,0.7);
  mixer2.gain(1,0.9);
  mixer2.gain(2,0.3);
  
  sine.amplitude(1.0);
  sine_fm.amplitude(1.0);
  
  delay1.delay(0,70);
  
  envelope1.attack(1.5);
  envelope1.decay(50);
  envelope1.sustain(50);
  envelope1.release(50);
  
  envelope2.attack(0.5);
  envelope2.decay(1);
  envelope2.sustain(1);
  envelope2.release(1);
  
  pink.amplitude(1.0);
  
  gen.begin(1.0, 1.0, WAVEFORM_SAWTOOTH);
  lfo.begin(0.5, 2.0, WAVEFORM_SQUARE); 
  
  filter.resonance(2);
  filter.frequency(440);
  filter.octaveControl(1);
  
}

void loop() {
  
  if (d == true) { 

    kick.play(AudioSampleKick);
    gen.frequency(110 + rand()%220);

  }
  
  sine.frequency(110 + rand()%440);
  sine_fm.frequency(rand()%2000);
  
  delay1.delay(0, 10 + rand()%60);
  
  envelope1.noteOn();
  envelope2.noteOn();
  
  delay(10);
 
  envelope1.noteOff();
  envelope2.noteOff();

  int tempo = 60000 / BPM;
  
  delay((tempo / 8) - 10);

  envelope2.noteOn();

  delay(10);

  envelope2.noteOff(); 

  delay((tempo / 4) - 10);
  
  d = !d;

}