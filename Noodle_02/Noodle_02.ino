// Melodic noodle 02 - base euclidean sequence //

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

#include "AKWF_0001.h"
#include "AKWF_0002.h"
#include "AudioSampleKick.h"
#include "AudioSampleHihat.h"
#include "AudioSampleSnare.h"

#define BPM   120

  int steps = 11;
  int hits = 7;

  int nx;
  int ny;
  
  bool kk, ll;
  int tx, ty, tt;

AudioPlayMemory     kick;
AudioPlayMemory     hihat;
AudioPlayMemory     snare;
AudioSynthWaveform  wave1;
AudioSynthWaveform  wave2;
AudioSynthWaveformSineModulated sine1;
AudioSynthWaveformSineModulated sine2;
AudioEffectEnvelope env;
AudioMixer4         mix;
AudioMixer4         mix2;
AudioOutputMQS      out;

AudioConnection c1(wave1, 0, sine1, 0);
AudioConnection c2(sine1, 0, mix, 0);
AudioConnection c3(wave2, 0, sine2, 0);
AudioConnection c4(sine2, 0, mix, 1);
AudioConnection c5(kick, 0, mix, 2);
AudioConnection c6(hihat, 0, mix, 3);
AudioConnection c7(mix, 0, mix2, 0);
AudioConnection c8(snare, 0, mix2, 1);
AudioConnection c9(mix2, 0, env, 0);
AudioConnection c10(env, 0, out, 0);


void setup() {
 
  AudioMemory(10);
  
  wave1.begin(1.0,1,WAVEFORM_ARBITRARY);
  wave1.arbitraryWaveform(AKWF_0001,600);
  
  wave2.begin(1.0,1,WAVEFORM_ARBITRARY);
  wave2.arbitraryWaveform(AKWF_0002,600);
  
  mix.gain(0, 0.4);
  mix.gain(1, 0.4);
  mix.gain(2, 0.3);
  mix.gain(3, 0.2);
  
  mix2.gain(0, 0.8);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.0);
  mix2.gain(3, 0.0);

  env.attack(1.5);
  env.decay(50.0);
  env.sustain(50.0);
  env.release(120.0);

}

void loop() {
  
  nx = tx;
  ny = ty;
       
  if (ny == 0) {
          
    tx = steps;
    ty = rand() % hits; 
          
  } else { 
          
    tx = ny;
    ty = nx % ny;
          
  }

  wave1.frequency(110 * nx);
  sine1.frequency(110 + 440 * ny);
  
  kk = !kk;
  
  if (kk) {
    
    kick.play(AudioSampleKick);
    sine2.frequency(110 + 110 * ny);
    
    tt ++;
    
  }
  
  if (nx > hits && ny < steps && kk == true) {
    
    hihat.play(AudioSampleHihat); 
    ll = !ll;
  
  }
  
  if (tt == 4) {
    
  snare.play(AudioSampleSnare);
  hihat.play(AudioSampleHihat);
  wave2.frequency(50*nx);
  tt = 0;
  
  }
  
  env.noteOn();

  delay (5);
  
  env.noteOff();

  int tempo = 60000 / BPM;
  
  if (ll) delay((tempo / 4) - 5);
  else delay((tempo / 2) - 5);

}