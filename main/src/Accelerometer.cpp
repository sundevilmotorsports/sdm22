#include "Accelerometer.h"


Accelerometer::Accelerometer(){

}

void Accelerometer::initialize(){

}

void Accelerometer::reset(){

}

float Accelerometer::get(){
    return 0.0;
}
int Accelerometer::calibrate(int samples){
    return 0;
}

void Accelerometer::set(Accelerometer::Axis axis, const int pin, float bias, float rate){
    switch(axis){
        case Accelerometer::Axis::X:
        settingsX.pin = pin;
        settingsX.bias = bias;
        settingsX.rate = rate;
        break;
        case Accelerometer::Axis::Y:
        settingsY.pin = pin;
        settingsY.bias = bias;
        settingsY.rate = rate;
        break;
        case Accelerometer::Axis::Z:
        settingsZ.pin = pin;
        settingsZ.bias = bias;
        settingsZ.rate = rate;
        break;
        default:
        settingsX.pin = pin;
        settingsX.bias = bias;
        settingsX.rate = rate;
        break;
    }
}

float Accelerometer::calculateAcceleration(Axis axis){
    int pin;
    float bias, rate;
    switch(axis){
        case Accelerometer::Axis::X:
        pin = settingsX.pin;
        bias = settingsX.bias;
        rate = settingsX.rate;
        break;
        case Accelerometer::Axis::Y:
        pin = settingsY.pin;
        bias = settingsY.bias;
        rate = settingsY.rate;
        break;
        case Accelerometer::Axis::Z:
        pin = settingsZ.pin;
        bias = settingsZ.bias;
        rate = settingsZ.rate;
        break;
        default:
        return 0.0;
    }
    // TODO double check this math
    // dunno about the - 3.3/2
    return bias + ((analogRead(pin)/RESOLUTION_3V3 * 3.3 - 3.3/2) / rate);
}

String Accelerometer::toString(){
    String output = "";
    String xOutput = String(calculateAcceleration(Accelerometer::Axis::X), 3);
    String yOutput = String(calculateAcceleration(Accelerometer::Axis::Y), 3);
    String zOutput = String(calculateAcceleration(Accelerometer::Axis::Z), 3);

    output = "X: " + xOutput + "\tY: " + yOutput + "\tZ: " + zOutput;
    return output;
}