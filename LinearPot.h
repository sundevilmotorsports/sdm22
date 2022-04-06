#pragma once

#include <Arduino.h>
#include "Sensor.h"

class LinearPot : public Sensor {
    public:
    LinearPot(); // TODO
    void initialize();
    void calibrate();
    void reset();
    float get();
    std::string toString();
};