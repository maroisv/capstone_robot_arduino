#include <SoftwareSerial.h>

//pin numbers must be replaced
const int RX_PIN = 2;
const int TX_PIN = 3;
SoftwareSerial serial(RX_PIN, TX_PIN);
char commandChar;

void setup() {
  // put your setup code here, to run once:
  serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:
  if(serial.available())
  {
    commandChar = serial.read();
    switch(commandChar) //command given by android app, waiting to recieve number
    {
      case '*':
      serial.print(random(1000) + '#');
      break;

      //when working
      /*
       * case 'temp'
       * serial.print(tempsensordata + 'K');
       * etc.
       */
    }
  }
}
