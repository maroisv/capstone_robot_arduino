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

// Left, center, right
const int Sensors::_pins_echo[3] = {10,3,9};
const int Sensors::_pins_trig[3] = {11,2,4};

// Create the Sensors object.
Sensors::Sensors() {}

// Initialize the pins related to the sensors.
void Sensors::initialize() {
  for (int i = 0; i < 3; i++) {
    pinMode(_pins_echo[i], INPUT);
    pinMode(_pins_trig[i], OUTPUT);
  }
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

  int pulse = pulseIn(_pins_echo[location], HIGH, 29070);

  if (pulse == 0) {
    return 500;
  } else {
    return (int) pulse / 2 * 0.0344;
  }
}

/**
 * Return an array containing the free distance from the three distance sensor.
 */
int * Sensors::getDistanceArray() {
  int * obstacles = new int[3];
  for (int i = 0; i < 3; i++) {
    obstacles[i] = getDistance(i);
  }
  return obstacles;
}

