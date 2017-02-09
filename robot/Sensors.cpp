/*
  Sensors.cpp - Library for reading the various sensors.
*/

#include "Arduino.h"
#include "Sensors.h"

#define a_pin_sound 4
#define a_pin_temp 5
#define a_pin_gas 3
#define read_to_mvolt 4.8828
#define mvolt_to_cel 0.1

int _pins_echo[3] = {2,2,2};
int _pins_trig[3] = {3,3,3};

Sensors::Sensors() {}

void Sensors::initialize() {
  pinMode(_pins_echo[1], INPUT);
  pinMode(_pins_trig[1], OUTPUT);
}

// Return the temperature in celsius
int Sensors::getTemperature() {
  return (int) analogRead(a_pin_temp) * read_to_mvolt * mvolt_to_cel;
}

// Return the sound level on a scal of 0 - 1024
int Sensors::getSoundLevel() {
	return analogRead(a_pin_sound);
}

// Return the gas level on a scal of 0 - 1024
int Sensors::getGasLevel() {
	return analogRead(a_pin_gas);
}

/* 
 * Return the distance in cm from a specific range finder location.
 * 
 * location: 0-left, 1-front, 2-right
 */
int Sensors::getDistance(int location) {
  digitalWrite(_pins_trig[location], LOW); 
  delayMicroseconds(2);
 
  digitalWrite(_pins_trig[location], HIGH);
  delayMicroseconds(10);
  digitalWrite(_pins_trig[location], LOW);
  
  return (int) pulseIn(_pins_echo[location], HIGH) / 2 * 0.0344;
}
