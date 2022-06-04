#pragma once

#include "Sensor.h"

class LinearPot : public Sensor {
    public:
    LinearPot(); // TODO
    void initialize(const int pin, float scale);
    void calibrate();
    void reset();
    float get();
    int getRaw();
    String toString();

    protected:
    const int pin;
    float avg;
    float scale;
};