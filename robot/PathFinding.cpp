/*
  PathFinding.cpp - Library for analysign the obstacles and map to
  decide on the next step.
*/

#include "Arduino.h"
#include "PathFinding.h"

int prevForwardDist = 0;
int obstacles[3] = {0, 0, 0};

// Create the PathFinding object.
PathFinding::PathFinding() {}

// Initialize the PathFinding..
void PathFinding::initialize() {
}

/*
 * Perform the next movement forward from the distance sensors output.
 * Return 1 on error or 0 on success.
 */
int PathFinding::advance(Control control, Sensors sensors) {
  // Get distance from sensors.
  prevForwardDist = obstacles[1];
  sensors.fillDistanceArray(obstacles);
  Serial.print(obstacles[0]);
  Serial.print(" ");
  Serial.print(obstacles[1]);
  Serial.print(" ");
  Serial.println(obstacles[2]);

  // Take the average from the two most recent forward distance.
  if ((prevForwardDist + obstacles[1])/2 > 10) {
    // Go forward or continue to go forward.
    control.forward();
  } else {
    // Turn toward the space with the most space. 
    (obstacles[0] > obstacles[2]) ? control.left() : control.right();
    delay(1000);
    control.stop();
  }
  delay(2000);
}

