#include <arduino-timer.h>
#include <array>

const int freq_hz = 100;
const int update_per_period = 8;
const int report_per_update = 5;
const int report_period_us = 1.0e6 / freq_hz / update_per_period / report_per_update;

const int baud_rate = 500000; // irrelevant if using SerialUSB (native USB) on the Arduino Due
const int resolution = 12;


Timer<1, micros> timer;

bool print_info(void *) {
  SerialUSB.println(analogRead(A0));
  return true;
}

void setup() {
  SerialUSB.begin(baud_rate);
  analogReadResolution(resolution);
  timer.every(report_period_us, print_info);
}

void loop() {
  timer.tick();
}
