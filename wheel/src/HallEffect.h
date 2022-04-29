#pragma once
#include "Sensor.h"


class HallEffect : public Sensor {
    public:
    HallEffect();
    void initialize();
    void calibrate();
    void reset();
    float get();
    String toString();
};