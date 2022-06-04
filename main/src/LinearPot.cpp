#include "LinearPot.h"

LinearPot::LinearPot(){

}

void LinearPot::initialize(const int pin, float scale){
    this.pin = pin;
    this.scale = scale;
}

float LinearPot::get(){
    return scale * abs(analogRead(pin));
}

int LinearPot::getRaw(){
    return analogRead(pin);
}