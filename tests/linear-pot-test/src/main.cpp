#include <Arduino.h>

// pin on the teensy
const int pin = A5;

// multiplier
float scale = 0.00244379;

// gets the value of the potentiometer in inches 
float get(){
    return scale * abs(analogRead(pin));
}

// gets the raw analog value of the potentiometer
int getRaw(){
    return analogRead(pin);
}

////////////////////////////////

// zero'd location of the potentiometer
float zeroValue = 0.0;

// zero's out the potentiometer
// i.e., set the zero'd location
void calibrate(int samples){

}

// returns the displacement of the potentiometer from its zero'd position
float getDisplacement(){
    float displacement = 0.0;

    return displacement;
}


void setup() {
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:

}