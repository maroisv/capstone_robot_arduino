#include "Control.h"
#include "Sensors.h"

Control control(200);
Sensors sensors;
boolean autonomous = false;

void setup(void)
{
  control.initialize();
  sensors.initialize();
  Serial.begin(9600);
  Serial.println("setup");
}

void loop(void)
{
  if (Serial.available() > 0) {
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
      case 'u': // sound level
      case 'U':
        autonomous = true;
        break;
      default:
        control.stop();
        autonomous = false;
        break;
    }
  }

  if (autonomous) {
    // Move forward or turn
    if (sensors.getDistance(1) > 10) {
      control.forward();
    } 
    else {
      control.right();
      delay(500);
      control.stop();
    }
  }
}
