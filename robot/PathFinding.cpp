/*
  PathFinding.cpp - Library for analysign the obstacles and map to
  decide on the next step.
*/

#include "Arduino.h"
#include "PathFinding.h"

#define MAX_STEPS 20
#define TO_RAD 1000.0 / 57296
#define TO_DEG 57296 / 1000.0

int obstacle0= 0;
int obstacle1= 0;
int obstacle2= 0;
int x = 0; // x,y in cm
int y = 0;
int orientation = 0; // 0 - 360 deg, starts at 0.
int prevDist = 0;

int x0;
int y0;
int x1;
int y1;
int x2;
int y2;
int x3;
int y3;
int x4;
int y4;
int x5;
int y5;
int x6;
int y6;
int numSteps = 0;

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
    savePosition();
    // Turn toward the space with the most space. 
    updateOrientation( (int)
      (obstacle0 > obstacle2) ? control.turn(270) : control.turn(90)); 
    prevDist = 0;
  }
}

int PathFinding::backtrack(Control control, Sensors sensors) {
  if (numSteps < 0) {
    return 1;
  }

  int dx = getPathX(numSteps) - x;
  int dy = getPathY(numSteps) - y;

  int turn = getTurn(getAngleToTurnTo(dx, dy), orientation);
  int dist = sqrt(dx*dx + dy*dy);

  updateOrientation(control.turn(turn));
  updatePosition(control.forward(dist));
  numSteps--;
}

int PathFinding::getTurn(int newOrientation, int orientation) {
  if (newOrientation >= orientation) {
    return newOrientation - orientation;
  } else {
    return (360 - orientation + newOrientation) ;
  }
}

int PathFinding::getAngleToTurnTo(int dx, int dy) {
  if (dx >= 0 && dy >= 0) {
    return atan(float(dx) / dy) * TO_DEG;
  } else if (dx >= 0 && dy < 0) {
    return 90 + atan(float(abs(dy)) / dx) * TO_DEG;
  } else if (dx < 0 && dy >= 0) {
    return 270 + atan(float(dy) / abs(dx)) * TO_DEG;
  } else {
    return 180 + atan(float(abs(dx)) / abs(dy)) * TO_DEG;
  }
}

int PathFinding::getPositionX() {
  return x;
}

int PathFinding::getPositionY() {
  return y;
}

void PathFinding::savePosition() {
  numSteps++;
  setPath(numSteps, x, y);
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

void PathFinding::setPath(int num, int x, int y) {
  switch(num) {
    case 0:
      x0 = x;
      y0 = y;
      break;
    case 1:
      x1 = x;
      y1 = y;
      break;
    case 2:
      x2 = x;
      y2 = y;
      break;
    case 3:
      x3 = x;
      y3 = y;
      break;
    case 4:
      x4 = x;
      y4 = y;
      break;
  }
}

int PathFinding::getPathX(int num) {
  switch(num) {
    case 0:
      return x0;
    case 1:
      return x1;
    case 2:
      return x2;
    case 3:
      return x3;
    case 4:
      return x4;
    case 5:
      return x5;
    case 6:
      return x6;
  }
}

int PathFinding::getPathY(int num) {
  switch(num) {
    case 0:
      return y0;
    case 1:
      return y1;
    case 2:
      return y2;
    case 3:
      return y3;
    case 4:
      return y4;
    case 5:
      return y5;
    case 6:
      return y6;
  }
}

void PathFinding::updateOrientation(int angle) {
  orientation += angle;
  orientation = orientation%360;
}

void PathFinding::updatePosition(int dist) {
  dist = dist - prevDist;
  prevDist += dist;

  x = x + dist * sin(orientation * 1000.0 / 57296);
  y = y + dist * cos(orientation * 1000.0 / 57296);
}

