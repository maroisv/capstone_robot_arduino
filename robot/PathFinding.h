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
    int backtrack(Control control, Sensors sensors);
    int getPositionX();
    int getPositionY();
    int getOrientation();
    int getObstacle(int pos);
    

  private:
    void updatePosition(int dist);
    void updateOrientation(int angle);
    void savePosition();
    int getAngleToTurnTo(int dx, int dy);
    void setPath(int num, int x, int y);
    int getPathX(int num);
    int getPathY(int num);
    int getTurn(int newOrientation, int orientation);

    // int path[][2];
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
    int numSteps;
    int obstacle0;
    int obstacle1;
    int obstacle2;
    int x;
    int y;
    int orientation;
};

#endif
