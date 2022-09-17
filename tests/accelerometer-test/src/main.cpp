#include <Arduino.h>
#include "Accelerometer.h"

Accelerometer a;
void setup() {
  // put your setup code here, to run once:

  // axis, pin, bias, rate
  a.set(Accelerometer::Axis::X, A1, 0.0, 0.55);
  a.set(Accelerometer::Axis::Y, A2, 0.0, 0.55);
  a.set(Accelerometer::Axis::Z, A3, 0.0, 0.55);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  float accelX = a.calculateAcceleration(Accelerometer::Axis::X);

  Serial.println(accelX);
}