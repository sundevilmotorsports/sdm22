#include "Accelerometer.h"


Accelerometer::Accelerometer(){

}

void Accelerometer::set(Axis axis, const int pin, float bias, float rate){
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
        pin = pinX;
        bias = biasX;
        rate = rateX;
        break;
        case Accelerometer::Axis::Y:
        pin = pinY;
        bias = biasY;
        rate = rateY;
        break;
        case Accelerometer::Axis::Z:
        pin = pinZ;
        bias = biasZ;
        rate = rateZ;
        break;
        default:
        return 0.0;
    }
    // TODO double check this math
    // dunno about the - 3.3/2
    return bias + ((analogRead(pin)/RESOLUTION_3V3 * 3.3 - 3.3/2) / rate);
}