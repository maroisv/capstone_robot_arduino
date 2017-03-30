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
int * avgRead = new int[3];

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
  avgRead[0] = (analogRead(a_pin_temp) * read_to_mvolt - mvolt_zero_cel) * mvolt_to_cel;
  avgRead[1] = (analogRead(a_pin_temp) * read_to_mvolt - mvolt_zero_cel) * mvolt_to_cel;
  avgRead[2] = (analogRead(a_pin_temp) * read_to_mvolt - mvolt_zero_cel) * mvolt_to_cel;
  return (int) (avgRead[0] + avgRead[1] + avgRead[2])/3;
}

// Return the sound level on a scal of 0 - 1024
int Sensors::getSoundLevel() {
  avgRead[0] = analogRead(a_pin_sound);
  avgRead[1] = analogRead(a_pin_sound);
  avgRead[2] = analogRead(a_pin_sound);
  return (int) (avgRead[0] + avgRead[1] + avgRead[2])/3;
}

// Return the gas level on a scal of 0 - 1024
int Sensors::getGasLevel() {
  avgRead[0] = analogRead(a_pin_gas);
  avgRead[1] = analogRead(a_pin_gas);
  avgRead[2] = analogRead(a_pin_gas);
  return (int) (avgRead[0] + avgRead[1] + avgRead[2])/3;
}

/* 
 * Return the distance in cm from a specific range finder location.
 * 
 * location: 0-left, 1-front, 2-right
 */
int Sensors::getDistance(int location) {
  for (int i = 0; i < 3; i++){
    digitalWrite(_pin_trig, LOW); 
    delayMicroseconds(2);
    digitalWrite(_pin_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_pin_trig, LOW);
  
    int pulse = pulseIn(_pins_echo[location], HIGH, 29070);
  
    if (pulse == 0) {
      avgRead[i] = 500;
    } else {
      avgRead[i] = pulse / 2 * 0.0344;
    }
  }
  return (avgRead[0] + avgRead[1] + avgRead[2])/3;
}

/**
 * Fill an array containing the free distance from the three distance sensor.
 */
void Sensors::fillDistanceArray(int * distArray) {
  for (int i = 0; i < 3; i++) {
    distArray[i] = getDistance(i);
  }
}

