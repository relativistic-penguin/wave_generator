#include <arduino-timer.h>
#include "WaveLUT.hpp"

const int freq_hz = 100;
const int amplitude_mV = 25;
const int update_per_period = 10;
const int self_test = 1; // 0 for no voltage output to PC via USB; 
                         // 1 for DAC0 output to PC via USB; 
                         // 2 for A0 input to PC via USB (where A0 is to be connected to DAC0)

const int resolution = 12;
const int division = pow(2, resolution) - 1;


const float v_in_prec_mV = 5000.0 / division; // Analogue input precision, from doc

const float v_out_min_mV = 808.0; // Manually calibrated +- 5mV drift (Needs to be calibrated per board)
const float v_out_max_mV = 4175.0; // Manually calibrated +- 5mV drift (Needs to be calibrated per board)
const float v_out_prec_mV = (v_out_max_mV - v_out_min_mV) / division;
const int amplitude_div = int(amplitude_mV / v_out_prec_mV);
const float rising_fraction = 1.0;
const int update_period_us = 1.0e6 / freq_hz / update_per_period;
WaveLUT::SawWave<update_per_period> v_lut(amplitude_div, rising_fraction);

const int report_per_update = 5;
const int report_period_us = update_period_us / report_per_update;

int current_v_out_id = 0;
int current_v_out = 0;

Timer<2, micros> timer;

bool update_DAC(void *) {
  current_v_out = v_lut.v_lut[current_v_out_id++];
  analogWrite(DAC0, current_v_out);
  if (current_v_out_id >= update_per_period) {
    current_v_out_id = 0;
  }
  return true;
}

bool report_output_in_mV(void *) {
  int v_in_mV = int(current_v_out * v_out_prec_mV);
  SerialUSB.println(v_in_mV);
  return true;
}

bool report_input_in_mV(void *) {
  int v_in_mV = int(analogRead(A0) * v_in_prec_mV);
  SerialUSB.println(v_in_mV);
  return true;
}

void setup() {
  SerialUSB.begin(2000000);
  analogWriteResolution(resolution);
  analogReadResolution(resolution);
  timer.every(update_period_us, update_DAC);
  if (self_test == 1) {
    timer.every(report_period_us, report_output_in_mV);
  }
  else if (self_test == 2) {
    timer.every(report_period_us, report_input_in_mV);
  }
}

void loop() {
  timer.tick();
}
