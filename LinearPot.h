#pragma once

#include "Sensor.h"

class LinearPot : public Sensor {
    public:
    LinearPot(); // TODO
    void initialize();
    void calibrate();
    void reset();
    float get();
    String toString();
};