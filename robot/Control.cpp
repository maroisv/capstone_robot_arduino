/*
  Control.cpp - Library for controlling the robot rover.
*/

#include "Arduino.h"
#include "Control.h"

#define correction 1.12
// Motor control
#define pin_speed_1 6
#define pin_speed_2 5
#define pin_dir_1 8
#define pin_dir_2 7

// Encoders Pin
#define pin_enc_left 2
#define pin_enc_right 3

// Ratios
#define gearRatio 207.0 // Output rotation per motor rotation
#define encoderRatio 3.0 // Count per motor rotation
#define distPerRot 11.0 // in cm
#define robotTurnCirc 38 // Robot turn circonference in cm

// Magnetic Transducer
#define pin_transducer 12
#define microPeriod 800

// Speed constants
#define MAX_SPEED 240
#define MIN_SPEED 150

/**
 * Momemtum delay
 * Duration (in ms) to wait to ensure the robot
 * has stopped moving. Helpful to make sure the
 * encode count is right.
 */
#define momDelay 200

// PD control
#define P 2
#define D 1
#define UPDATE_DELAY 100

int _speed;
int _speedR;
int _speedL;
int oldDiff = 0;

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
 
  setSpeed(speed);
}


/**
 * Return the speed value. (0 - 255)
 */
int Control::getSpeed() {
  return _speed;
}


/**
 * Set the robot speed. (100 to 255)
 */
void Control::setSpeed(int speed) {
  if (speed < MIN_SPEED) speed = MIN_SPEED;
  if (speed > MAX_SPEED) speed = MAX_SPEED;
  _speed = speed;
  _speedR = speed;
  _speedL = speed;
}


/**
 * Move forward indefinitely at default speed.
 */
void Control::forward() {
  analogWrite(pin_speed_1,_speedL);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,_speedR);
  digitalWrite(pin_dir_2,LOW);
}

/**
 * Move forward a specified distance.
 * 
 * distance: Distance to move forwrd to (cm)
 * 
 * Return: exact distance it moved (cm).
 */
float Control::forward(int distance)
{
  // Make sure the robot is stopped and the encoder count is resetted.
  stop();
  delay(momDelay);
  resetEncoderCount();

  // Save speed
  int prevSpeed = _speed;

  float diff = distance - getDistanceTravelled();
  
  while (diff > 0.2) {
    // Set speed
    setSpeed(prevSpeed * diff * 0.05);
    correctSpeed(UPDATE_DELAY);

    // debug_printEncoderSpeedInfo();
    // Go forward
    forward();
    
    delay(UPDATE_DELAY);
    
    // Update difference
    diff = distance - getDistanceTravelled();
  }

  // Wait for complete stop.
  stop();
  delay(momDelay);

  // Return speed to previous value
  setSpeed(prevSpeed);

  // Print final rotation angle and reset encoder count.
  float dist = getDistanceTravelled();
  resetEncoderCount();
  return dist;
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
}

/**
 * Turn right indefinitely.
 */
void Control::right()
{
  analogWrite(pin_speed_1,_speed);
  digitalWrite(pin_dir_1,LOW);
  analogWrite(pin_speed_2,_speed);
  digitalWrite(pin_dir_2,HIGH);
}

/**
 * Turn to the specified angle.
 * 
 * angleToRot: Orientation relative to the robot to turn to (degrees).
 * 
 * Return: Exact orientation turned to relative to previous orientation (degrees).
 */
float Control::turn(int angleToRot) {
  // Make sure the robot is stopped and the encoder count is resetted.
  stop();
  delay(momDelay);
  resetEncoderCount();
  
  // Figure out the side to turn
  bool turnRight = true;
  if (angleToRot > 180) {
    angleToRot = 360 - angleToRot;
    turnRight = false; // turn left
  }

  // Save speed
  int prevSpeed = _speed;

  int diff = angleToRot - getAngleRotation();
  
  while (diff > 2) {
    // Set speed
    setSpeed(prevSpeed * 0.005 * diff);
    correctSpeed(UPDATE_DELAY);

    // debug_printEncoderSpeedInfo();
    // Start turning
    (turnRight) ? right() : left();

    delay(UPDATE_DELAY);
    
    // Update difference
    diff = angleToRot - getAngleRotation();
  }

  // Wait for complete stop.
  stop();
  delay(momDelay);
    
  // Return speed to previous value
  setSpeed(prevSpeed);

  // Print final rotation angle and reset encoder count.
  float angle = getAngleRotation();
  (turnRight) ? angle = angle : angle = 360 - angle;
  resetEncoderCount();
  return angle;
}

/**
 * Stop the motors.
 * 
 * Return: distance travelled (cm).
 * 
 * NOTE: The distance returned only works 
 * if the robot was going forward.
 */
float Control::stop()
{
  digitalWrite(pin_speed_1,LOW);
  digitalWrite(pin_speed_2,LOW);

  return getDistanceTravelled();
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

/**
 * Get the distance the specified track travelled in cm.
 * num: 0 - left track, 1 - right track
 */
float Control::getTrackDistance(int num) {
  if (num) {
    // Left track
    return distPerRot * _encoderCountLeft / (gearRatio * encoderRatio);
  } else {
    // Right track
    return distPerRot * correction * _encoderCountRight / (gearRatio * encoderRatio);
  }
}

/**
 * Return the distance travelled in cm.
 * Takes the average of both tracks
 */
float Control::getDistanceTravelled() {
  return (getTrackDistance(0) + getTrackDistance(1)) / 2.0;
}

/**
 * Get the angle the robot has rotated.
 * Return angle in degrees.
 */
float Control::getAngleRotation() {
  return (getDistanceTravelled() / robotTurnCirc) * 360;
}

/**
 * Correct the speed of the motors.
 * Use PD control to maintain the speeds of the two motors at a
 * similar level. 
 * 
 * timeDiff: Delay since the last correction. (in ms)
 */
void Control::correctSpeed(int timeDiff) {
  // Calculate change to apply
  int diff = correction * _encoderCountRight - _encoderCountLeft;
  int change = abs(diff * P + (diff - oldDiff) / float(timeDiff) * D);

  // Apply change
  if (_speed > 200) {
    (diff > 0) ? _speedR = _speed - change : _speedL = _speed - change;
  } else {
    (diff > 0) ? _speedL = _speed + change : _speedR = _speed + change;
  }
  

  // Ensure it respect the limit
  if (_speedR < MIN_SPEED) _speedR = MIN_SPEED;
  if (_speedL < MIN_SPEED) _speedL = MIN_SPEED;
  if (_speedR > MAX_SPEED) _speedR = MAX_SPEED;
  if (_speedL > MAX_SPEED) _speedL = MAX_SPEED;
  
  oldDiff = diff;
}

void Control::debug_printEncoderSpeedInfo() {
  Serial.print("Speed: ");
  Serial.println(getSpeed());
  Serial.print("encLeft=");
  Serial.print(_encoderCountLeft);
  Serial.print(" encRight=");
  Serial.println(_encoderCountRight);
  Serial.print("SpeedL=");
  Serial.print(_speedL);
  Serial.print(" SpeedR=");
  Serial.println(_speedR);
}

void Control::incrementEncoderLeft() {
  _encoderCountLeft++;
}

void Control::incrementEncoderRight() {
  _encoderCountRight++;
}

