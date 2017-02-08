#include "Control.h"
#include "Sensors.h"

Control control(200);
Sensors sensors;

void setup(void)
{
  control.initialize();
  sensors.initialize();
  Serial.begin(9600);
  Serial.println("setup");
}

void loop(void)
{
  while (Serial.available() < 1) {} // Wait until a character is received
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
      Serial.println(sensors.getDistance(1));
      break;
    default:
      control.stop();
      break;
  }
}
