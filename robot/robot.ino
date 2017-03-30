#include "Control.h"
#include "Sensors.h"
#include "PathFinding.h"

// Modules
Control control;
Sensors sensors;
PathFinding pathFinding;

// Variables
boolean autonomous = false;
int distFrwrd = 0;
int * avgTemp = new int[4];


void setup(void)
{
  control.initialize(200);
  sensors.initialize();
  pathFinding.initialize();

  Serial.begin(9600);
  Serial.println("Setup finished");
}

void loop(void)
{
  if (Serial.available() > 0) { // TODO: Hide communication in Module
    char val = Serial.read();
    switch(val) // Perform an action depending on the command
    {
      case 'w'://Move Forward
      case 'W':
        control.forward();
        break;
      case 's'://Move Backwards
      case 'S':
        control.reverse();
        break;
      case 'a'://Turn Left
      case 'A':
        control.left();
        break;
      case 'd'://Turn Right
      case 'D':
        control.right();
        break;
      case 't': // Temperature
      case 'T':
        avgTemp[0] = sensors.getTemperature();
        avgTemp[1] = sensors.getTemperature();
        avgTemp[2] = sensors.getTemperature();
        avgTemp[3] = (avgTemp[0]+avgTemp[1]+avgTemp[2])/3;
        Serial.println(avgTemp[3]);
        break;
      case 'e':
      case 'E':
        Serial.println(sensors.getDistance(0));
        Serial.println(sensors.getDistance(1));
        Serial.println(sensors.getDistance(2));
        break;
      case 'g': // gas level
      case 'G':
        Serial.println(sensors.getGasLevel());
        break;
      case 'o': // sound level
      case 'O':
        Serial.println(sensors.getSoundLevel());
        break;
      case 'b': // Beep for 5 seconds
      case 'B':
        control.beep(2000);
        control.beepBeep(200,3000);
        break;
      case 'u': // Go autonomous
      case 'U':
        autonomous = true;
        break;
      default:
        control.stop();
        control.debug_printEncoderCount();
        control.resetEncoderCount();
        autonomous = false;
        break;
    }
  }

  if (autonomous) {
    pathFinding.advance(control, sensors);
  }
}


