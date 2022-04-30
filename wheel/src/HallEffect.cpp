#include "HallEffect.h"

HallEffect::HallEffect(int pin_){
    setPin(pin_);
    calibrate();
}

void HallEffect::setPin(int pin_){
    pin = pin_;
}

int HallEffect::calibrate(int samples){
    // get average reading over samples # of samples
    int sum = 0;
    for(int i = 0; i < samples; i++){
        sum += analogRead(pin);
        delay(1);
    }
    if(!samples) return -1; // avoids division by zero if samples = 0
    zeroVal = sum/samples;
    return zeroVal;
}

void HallEffect::reset(){
    counts = 0;
}

void HallEffect::onLoop(){
    /*
    int diff = getRawDiff();
    if(diff > 900 && !justPassed){
        justPassed = true;
        // TODO
    }
    else if(diff < 10) justPassed = false;
    */
   Serial.println(toString());
}

float HallEffect::get(){
    return (float) analogRead(pin);
}

String HallEffect::toString(){
    return "analogRead: " + String(get());
}