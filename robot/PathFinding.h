/*
  PathFinding.h - Library for analysign the obstacle and map to
  decide on where to go.
*/
#ifndef path_finding_h
#define path_finding_h

#include "Arduino.h"
#include "Control.h"
#include "Sensors.h"

class PathFinding
{
  public:
    PathFinding();
    void initialize();
    int advance(Control control, Sensors sensors);
    int getPositionX();
    int getPositionY();
    int getOrientation();
    int getObstacle(int pos);

  private:
    void updatePosition(int dist);
    void updateOrientation(int angle);

    int obstacle0;
    int obstacle1;
    int obstacle2;
    int x;
    int y;
    int orientation;
};

#endif
