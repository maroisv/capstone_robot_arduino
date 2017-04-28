#include "Control.h"
#include "Sensors.h"
#include "PathFinding.h"

// Modules
Control control;
Sensors sensors;
PathFinding pathFinding;

// Variables
boolean autonomous = false;
boolean backtracking = false;

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
      case 'f'://Move Forward
      case 'F':
        Serial.println(control.forward(50));
        break;
      case 'r'://Move Backwards
      case 'R':
        Serial.println(control.turn(180));
        break;
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
        Serial.println(sensors.getTemperature());
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
      case 'P': // Backtrack
      case 'p':
        backtracking = true;
        break;
      case 'm': // Send coordinates data
      case 'M':
        Serial.print(pathFinding.getPositionX());
        Serial.print(',');
        Serial.print(pathFinding.getPositionY());
        Serial.print(',');
        Serial.print(pathFinding.getOrientation());
        Serial.print(',');
        Serial.print(pathFinding.getObstacle(0));
        Serial.print(',');
        Serial.print(pathFinding.getObstacle(1));
        Serial.print(',');
        Serial.println(pathFinding.getObstacle(2));
        break;
      default:
        control.resetEncoderCount();
        autonomous = false;
        break;
    }
  }

  if (autonomous) {
    pathFinding.advance(control, sensors);
  } else if (backtracking) {
    pathFinding.backtrack(control, sensors);
  }
}


