#include "LinearPot.h"

LinearPot::LinearPot(){

}

int LinearPot::calibrate(int samples){
    return 0;
}

void LinearPot::initialize(const int pin, float scale){
    this->pin = pin;
    this->scale = scale;
}

float LinearPot::get(){
    return scale * abs(analogRead(pin));
}

int LinearPot::getRaw(){
    return analogRead(pin);
}

void LinearPot::reset(){

}

String LinearPot::toString(){
    return "temp";
}