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
    float forward(int distance);
	  void reverse();
	  void left();
	  void right();
    float turn(int angleToRot);
    float stop();
    void beep(int len);
    void beepBeep(int period, int len);
    void debug_printEncoderCount();
    void resetEncoderCount();
    int getSpeed();
    void setSpeed(int speed);
  private:
    int _speed;
    static volatile int _encoderCountLeft;
    static volatile int _encoderCountRight;

    float getTrackDistance(int num);
    float getDistanceTravelled();
    float getAngleRotation();
    void correctSpeed(int timeDiff);
    void debug_printEncoderSpeedInfo();

    static void incrementEncoderLeft();
    static void incrementEncoderRight();
};

#endif
