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
    
    int diff = getRawDiff();
    if(diff >= 6 && !justPassed){
        justPassed = true;
        // TODO
        // in meters, distance travelled since last detection
        float wheelCirc = .376;
        prev = counts++;
        uint32_t current = millis();
        float dt = (int) (current - timestamp); // in ms
        dt /= 1000; // convert dt -> seconds

        // meters per second 
        speed = .376/ dt;
        timestamp = current;
    }
    else if(diff < 4) justPassed = false;

    Serial.println(toString());
    Serial.println(millis() - timestamp);
    if(justPassed) digitalWriteFast(13, HIGH);
    else digitalWriteFast(13, LOW);


}

float HallEffect::get(){
    return (float) counts;
}

int HallEffect::getRawDiff(){
    return abs(analogRead(pin) - zeroVal);
}

String HallEffect::toString(){
    return "diff: " + String(getRawDiff())+ "\tcounts: " + String(counts) + "\tspeed (m/s): " + String(speed);
}