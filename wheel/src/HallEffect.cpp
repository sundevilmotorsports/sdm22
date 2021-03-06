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
    
    //Serial.println(toString());
    if(getRaw() < 8 && !justPassed){
        counts++;
        justPassed = true;
        uint32_t current = millis();
        float dt = (float) (current - timestamp); // in ms
        dt /= 1000; // convert dt to seconds

        // meters per second
        // we divide by 4 bc 4 magnets 
        speed = (wheelCirc / 4.0)/ dt;
        timestamp = current;
    }
    else if(getRaw() > 6){
        justPassed = false;
    }

    if(justPassed) digitalWriteFast(13, HIGH);
    else digitalWriteFast(13, LOW);
}

float HallEffect::get(){
    return getSpeed();
}

int HallEffect::getRaw(){
    return analogRead(pin);
}

int HallEffect::getRawDiff(){
    return abs(getRaw() - zeroVal);
}

float HallEffect::getSpeed(){
    return speed;
}

int HallEffect::getEncoder(){
    return counts;
}

String HallEffect::toString(){
    return String(millis()/1000.0) + "," + String(pin) + "," + String(getRaw()) + "," + String(getRawDiff()) + "," + String(getSpeed()) + "," + String(justPassed) + "," + String(zeroVal);
}