/*
  PathFinding.cpp - Library for analysign the obstacles and map to
  decide on the next step.
*/

#include "Arduino.h"
#include "PathFinding.h"

#define TO_RAD (1000 / 57296)

int obstacle0= 0;
int obstacle1= 0;
int obstacle2= 0;
int x = 0; // x,y in cm
int y = 0;
int orientation = 0; // 0 - 360 deg, starts at 0.
int prevDist = 0;

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
  obstacle0 = sensors.getDistance(0);
  obstacle1 = sensors.getDistance(1);
  obstacle2 = sensors.getDistance(2);

  // Take the average from the two most recent forward distance.
  if (obstacle1 > 25) {
    // Go forward or continue to go forward.
    control.forward();
    delay(500);
    updatePosition((int) control.stop()); // TODO: Get the distance travelled
  } else {
    // Turn toward the space with the most space. 
    updateOrientation( (int)
      (obstacle0 > obstacle2) ? control.turn(270) : control.turn(90)); 
    prevDist = 0;
  }
}

int PathFinding::getPositionX() {
  return x;
}

int PathFinding::getPositionY() {
  return y;
}


int PathFinding::getOrientation() {
  return orientation;
}

int PathFinding::getObstacle(int pos) {
  switch(pos) {
    case 0:
      return obstacle0;
      break;
    case 1:
      return obstacle1;
      break;
    case 2:
      return obstacle2;
      break;
  }
}

void PathFinding::updateOrientation(int angle) {
  orientation += angle;
  orientation = orientation%360;
}

void PathFinding::updatePosition(int dist) {
  dist = dist - prevDist;
  prevDist += dist;
  
  x = x + dist * sin(orientation * TO_RAD);
  y = y + dist * cos(orientation * TO_RAD);
}

