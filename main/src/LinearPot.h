#pragma once

#include "Sensor.h"

class LinearPot : public Sensor {
    public:
    LinearPot(); // TODO
    void initialize(const int pin, float scale);
    int calibrate(int samples = 500);
    void reset();
    float get();
    int getRaw();
    String toString();

    protected:
    int pin;
    float avg;
    float scale;
};