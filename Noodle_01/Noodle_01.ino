// Melodic noodle 01 - base euclidean sequence //

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

#include "AKWF_0001.h"
#include "AKWF_0002.h"
#include "AudioSampleKick.h"
#include "AudioSampleHihat.h"

#define BPM   120

  int steps = 8;
  int hits = 5;

  int nx;
  int ny;
  
  boolean kk;
  
  int tx, ty;

AudioPlayMemory     kick;
AudioPlayMemory     hihat;
AudioSynthWaveform  wave1;
AudioSynthWaveform  wave2;
AudioEffectEnvelope env;
AudioMixer4         mix;
AudioOutputMQS      out;

AudioConnection c1(wave1, 0, mix, 0);
AudioConnection c2(wave2, 0, mix, 1);
AudioConnection c3(kick, 0, mix, 2);
AudioConnection c4(hihat, 0, mix, 3);
AudioConnection c5(mix, 0, env, 0);
AudioConnection c6(env, 0, out, 0);


void setup() {
 
  AudioMemory(10);
  
  wave1.begin(1.0,1,WAVEFORM_ARBITRARY);
  wave1.arbitraryWaveform(AKWF_0001,600);
  
  wave2.begin(1.0,1,WAVEFORM_ARBITRARY);
  wave2.arbitraryWaveform(AKWF_0002,600);
  
  mix.gain(0, 0.4);
  mix.gain(1, 0.4);
  mix.gain(2, 0.2);
  mix.gain(3, 0.2);

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
    ty = rand()%hits; 
          
  } else { 
          
    tx = ny;
    ty = nx % ny;
          
  }
  
  wave1.frequency(110+220*nx);
  wave2.frequency(110+110*ny);
  
  kk = !kk;
  
  if (kk) kick.play(AudioSampleKick);
  
  if (nx > hits && ny < steps && kk == true) hihat.play(AudioSampleHihat);
  
  env.noteOn();

  delay (5);
  
  env.noteOff();

  int tempo = 60000 / BPM;
  delay((tempo / 2) - 5);

}