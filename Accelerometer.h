// class that represents an accelerometer, axdl335 or something like that
#pragma once

#include <Arduino.h>
#include "Sensor.h"

#define RESOLUTION_3V3 1023.0

class Accelerometer : public Sensor {
    public:
    Accelerometer(); // TODO
    Accelerometer(int pinX, int pinY, int pinZ); // calibrate 
    void initialize();
    void calibrate();
    void reset();

    // returns magnitude of acceleration vector
    float get();
    float getRaw();
    float getRawX();
    float getRawY();
    float getRawZ();
    float getX();
    float getY();
    float getZ();
    std::string toString();

    enum Axis {
        X,
        Y,
        Z
    };

    protected:
    float calculateAcceleration(Axis axis);
    // pins
    const int pinX;
    const int pinY;
    const int pinZ;

    // tuning constants
    // biases
    float biasX;
    float biasY;
    float biasZ;

    // volts -> g conversion rate (i.e. V/g)
    float rateX;
    float rateY;
    float rateZ;

    // calculated in calibrate
    float avgX;
    float avgY;
    float avgZ;

    // TODO 
    // need to do some linear algebra magic
    // to make sure the board's Z axis is the up direction
    // since accelerometer likely won't be mounted with Z up
    float yawOffset;
    float rollOffset;
    float pitchOffset;
};