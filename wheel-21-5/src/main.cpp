#include <Arduino.h>
#include <LinearPot.h>
#include <SDMSerial.h>
#include <HallEffect.h>


HallEffect he(A7);
//SDMSerial comms({1}, false);

uint32_t timestamp;
void setup() {
  // put your setup code here, to run once:
  he.calibrate();
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(13, OUTPUT);

  timestamp = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  he.onLoop();
  float speed = he.getSpeed(); // at max, it should be ~35 mph = 15 m/s

  // convert speed to packet
  int converted = (int) (speed * 10);
  if(converted > 255) converted = 255;
  uint8_t packet = (uint8_t) converted;

  // send packet every 50-80ms
  uint32_t current = millis();
  if(current - timestamp > 50)
  {
    Serial1.write(packet);
    Serial1.flush();
    timestamp = current;
  }

  // debug prints
  Serial.print(packet);
  Serial.print(" ");
  Serial.println(he.getSpeed());
  //Serial.println(he.getSpeed());
  delay(5);
}