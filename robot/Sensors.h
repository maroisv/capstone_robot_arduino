/*
  Sensors.h - Library for reading the various sensors.
*/
#ifndef sensors_h
#define sensors_h

#include "Arduino.h"

class Sensors
{
  public:
    Sensors();
    void initialize();
    int getDistance(int location);
	  int getTemperature();
	  int getSoundLevel();
	  int getGasLevel();
    int * getDistanceArray();
  private:
    static const int _pin_trig;
	  static const int _pins_echo[];
};

#endif
