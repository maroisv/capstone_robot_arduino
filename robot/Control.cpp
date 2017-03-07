/*
  Control.cpp - Library for controlling the robot rover.
*/

#include "Arduino.h"
#include "Control.h"

#define pin_speed_1 6
#define pin_speed_2 5
#define pin_dir_1 8
#define pin_dir_2 7

#define pin_transducer 12
#define microPeriod 800

int _speed;

/**
 * Create the control object and set the desired speed.
 */
Control::Control(int speed) {
	_speed = speed;
}

/**
 * Initialize the control related pins.
 */
void Control::initialize(void) {
  pinMode(pin_dir_1, OUTPUT);
  pinMode(pin_dir_2, OUTPUT);
  pinMode(pin_transducer, OUTPUT);
}

/**
 * Move forward indefinitely.
 */
void Control::forward()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,LOW);
  Serial.println("forward");
}

/**
 * Move backward indefinitely.
 */
void Control::reverse()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,HIGH);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,HIGH);
  Serial.println("reverse");
}

/**
 * Turn left indefinitely.
 */
void Control::left ()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,HIGH);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,LOW);
  Serial.println("left");
}

/**
 * Turn right indefinitely.
 */
void Control::right ()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,HIGH);
  Serial.println("right");
}

/**
 * Stop the motors.
 */
void Control::stop()
{
  digitalWrite(pin_speed_1,LOW);
  digitalWrite(pin_speed_2,LOW);
  Serial.println("stop");
}

/**
 * Generate a constant sounds.
 * 
 * len: Duration of the sound (milli-seconds)
 */
void Control::beep(int len) {
  int start = millis();
  while (millis() - start < len) {
    digitalWrite(pin_transducer, HIGH);
    delayMicroseconds(microPeriod);
    digitalWrite(pin_transducer, LOW);
    delayMicroseconds(microPeriod);
  }
}

/**
 * Generate an alternative sounds.
 * 
 * period: Period of the alternative sound (milli-seconds)
 * len: duration of the sound (milli-seconds)
 */
void Control::beepBeep(int period, int len) {
  int start = millis();
  while (millis() - start < len) {
    beep(period/2);
    delay(period/2);
  }
}

