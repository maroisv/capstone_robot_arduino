/*
  Control.h - Library for controlling the rover robot.
*/
#ifndef control_h
#define control_h

#include "Arduino.h"

class Control
{
  public:
    Control(int speed);
    void initialize();
    void forward();
	  void reverse();
	  void left();
	  void right();
    void stop();
  private:
    int _speed;
};

#endif
