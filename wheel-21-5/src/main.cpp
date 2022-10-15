#include <Arduino.h>
#include <LinearPot.h>
//#include <SDMSerial.h> // compile issues
#include <HallEffect.h>


HallEffect he(A6);

void setup() {
  // put your setup code here, to run once:
  he.calibrate();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  he.onLoop();
  //Serial.println(he.toString());
  Serial.println(he.getSpeed());
  delay(5);
}