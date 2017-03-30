/*
  Control.cpp - Library for controlling the robot rover.
*/

#include "Arduino.h"
#include "Control.h"

// Motor control
#define pin_speed_1 6
#define pin_speed_2 5
#define pin_dir_1 8
#define pin_dir_2 7

// Encoders Pin
#define pin_enc_left 2
#define pin_enc_right 3

// Ratios
#define gearRatio 207
#define encoderRatio 6
#define distPerRot 3.2

// Magnetic Transducer
#define pin_transducer 12
#define microPeriod 800

int _speed;

volatile int Control::_encoderCountLeft = 0;
volatile int Control::_encoderCountRight = 0;

/**
 * Create the control object.
 */
Control::Control() {}

/**
 * Initialize the control related pins.
 */
void Control::initialize(int speed) {
  // Initialise Pins
  pinMode(pin_dir_1, OUTPUT);
  pinMode(pin_dir_2, OUTPUT);
  pinMode(pin_transducer, OUTPUT);
  pinMode(pin_enc_left, INPUT);
  pinMode(pin_enc_right, INPUT);

  // Initialize encoder's interrupts
  attachInterrupt(digitalPinToInterrupt(pin_enc_left), incrementEncoderLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin_enc_right), incrementEncoderRight, FALLING);
 
  _speed = speed;
}

/**
 * Move forward indefinitely at default speed.
 */
void Control::forward() {
  forward(_speed);
}

/**
 * Move forward indefinitely at a certain speed
 */
void Control::forward(int speed)
{
  analogWrite(pin_speed_1,speed);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,speed);
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

void Control::debug_printEncoderCount() {
  Serial.print(_encoderCountLeft);
  Serial.print(" ");
  Serial.println(_encoderCountRight);
}

void Control::resetEncoderCount() {
  _encoderCountLeft = 0;
  _encoderCountRight = 0;
}

void Control::incrementEncoderLeft() {
  _encoderCountLeft++;
}

void Control::incrementEncoderRight() {
  _encoderCountRight++;
}

