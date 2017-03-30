/*
  Sensors.cpp - Library for reading the various sensors.
*/

#include "Arduino.h"
#include "Sensors.h"

#define a_pin_sound 4
#define a_pin_temp 2
#define a_pin_gas 3
#define read_to_mvolt 4.8828
#define mvolt_to_cel 0.1
#define mvolt_zero_cel 500

// Left, center, right
const int Sensors::_pin_trig = 4;
const int Sensors::_pins_echo[3] = {9,10,11};

// Variables
int * avgTemp = new int[4];

// Create the Sensors object.
Sensors::Sensors() {}

// Initialize the pins related to the sensors.
void Sensors::initialize() {
  for (int i = 0; i < 3; i++) {
    pinMode(_pins_echo[i], INPUT);
  }
  pinMode(_pin_trig, OUTPUT);
}

// Return the temperature in celsius
int Sensors::getTemperature() {
  avgTemp[0] = (analogRead(a_pin_temp) * read_to_mvolt - mvolt_zero_cel) * mvolt_to_cel;
  avgTemp[1] = (analogRead(a_pin_temp) * read_to_mvolt - mvolt_zero_cel) * mvolt_to_cel;
  avgTemp[2] = (analogRead(a_pin_temp) * read_to_mvolt - mvolt_zero_cel) * mvolt_to_cel;
  avgTemp[3] = (avgTemp[0] + avgTemp[1] + avgTemp[2])/3;
  return (int) avgTemp[3];
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
  digitalWrite(_pin_trig, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(_pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pin_trig, LOW);

  int pulse = pulseIn(_pins_echo[location], HIGH, 29070);

  if (pulse == 0) {
    return 500;
  } else {
    return (int) pulse / 2 * 0.0344;
  }
}

/**
 * Fill an array containing the free distance from the three distance sensor.
 */
void Sensors::fillDistanceArray(int * distArray) {
  for (int i = 0; i < 3; i++) {
    distArray[i] = getDistance(i);
  }
}

