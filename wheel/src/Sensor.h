// abstract Sensor class
#pragma once

#include <Arduino.h>

class Sensor {
    public:
    Sensor() {};
    virtual void initialize() = 0;
    virtual void calibrate() = 0;
    virtual void reset() = 0;
    virtual float get() = 0;
    virtual String toString() = 0;
};