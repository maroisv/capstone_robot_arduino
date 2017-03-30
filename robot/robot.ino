#include "Control.h"
#include "Sensors.h"

Control control(200);
Sensors sensors;
boolean autonomous = false;
int * obstacles = new int[3];
int distFrwrd = 0;
int * avgTemp = new int[4];

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
      case 'u': // sound level
      case 'U':
        autonomous = true;
        distFrwrd = sensors.getDistance(1);
        break;
      default:
        control.stop();
        autonomous = false;
        break;
    }
  }

  if (autonomous) {
    // Move forward or turn
    distFrwrd = (distFrwrd+sensors.getDistance(1))/2;
    if (distFrwrd > 10) {
      control.forward();
    } 
    else {
      obstacles = sensors.getDistanceArray();
      (obstacles[0] > obstacles[2]) ? control.left() : control.right();
      delay(1000);
      control.stop();
      distFrwrd = sensors.getDistance(1);
    }
  }
}
