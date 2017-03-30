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
  private:
    int prevForwardDist;
    int obstacles[];
};

#endif
