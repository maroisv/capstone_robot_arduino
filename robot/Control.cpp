/*
  Control.cpp - Library for controlling the robot rover.
*/

#include "Arduino.h"
#include "Control.h"

#define pin_speed_1 6
#define pin_speed_2 5
#define pin_dir_1 8
#define pin_dir_2 7

int _speed;

Control::Control(int speed) {
	_speed = speed;
}

void Control::initialize(void) {
  for(int i=5;i<=8;i++)
    pinMode(i, OUTPUT);
}

void Control::forward()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,LOW);
  Serial.println("forward");
}

void Control::reverse()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,HIGH);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,HIGH);
  Serial.println("reverse");
}

void Control::left ()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,HIGH);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,LOW);
  Serial.println("left");
}

void Control::right ()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,HIGH);
  Serial.println("right");
}

void Control::stop()
{
  digitalWrite(pin_speed_1,LOW);
  digitalWrite(pin_speed_2,LOW);
  Serial.println("stop");
}
