// class that represents an accelerometer, axdl335 or something like that
#pragma once

#include <Arduino.h>
#include "Sensor.h"

class Accelerometer : public Sensor {
    public:
    Accelerometer(); // TODO
    void initialize();
    void calibrate();
    void reset();

    float get(); // returns magnitude
    float getRawX();
    float getRawY();
    float getRawZ();
    float getX();
    float getY();
    float getZ();
    std::string toString();

    protected:
    // pins
    const int pinX;
    const int pinY;
    const int pinZ;

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

// use to construct an Accelerometer object
// TODO
// parameters: 
// pins for x, y, z data
// tuning constants for x, y, z
class AccelerometerBuilder {

};