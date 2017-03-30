/*
  Control.h - Library for controlling the rover robot.
*/
#ifndef control_h
#define control_h

#include "Arduino.h"

class Control
{
  public:
    Control();
    void initialize(int speed);
    void forward();
    void forward(int speed);
	  void reverse();
	  void left();
	  void right();
    void stop();
    void beep(int len);
    void beepBeep(int period, int len);
    void debug_printEncoderCount();
    void resetEncoderCount();
  private:
    int _speed;
    static volatile int _encoderCountLeft;
    static volatile int _encoderCountRight;

    static void incrementEncoderLeft();
    static void incrementEncoderRight();
};

#endif
