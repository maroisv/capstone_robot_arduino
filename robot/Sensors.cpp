/*
  Sensors.cpp - Library for reading the various sensors.
*/

#include "Arduino.h"
#include "Sensors.h"

#define pin_echo_1 13
#define pin_trig_1 12
#define pin_temp 0
#define read_to_mvolt 4.8828
#define mvolt_to_fah 0.1

Sensors::Sensors() {}

void Sensors::initialize() {
  pinMode(pin_echo_1, INPUT);
  pinMode(pin_trig_1, OUTPUT);
}

int Sensors::getTemperature() {
  return (int) ((analogRead(pin_temp) * read_to_mvolt * mvolt_to_fah ) - 32) * 5.0 / 9.0;
}

float Sensors::getDistance(int location) {
  digitalWrite(pin_trig_1, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(pin_trig_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig_1, LOW);
  
  return pulseIn(pin_echo_1, HIGH) / 2 * 0.0344;
}
