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
    float getDistance(int location);
	  int getTemperature();
  private:
};

#endif
