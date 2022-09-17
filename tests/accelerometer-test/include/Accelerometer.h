// class that represents an accelerometer, axdl335 or something like that
#pragma once

#include "Sensor.h"

#define RESOLUTION_3V3 1023.0

typedef struct AccelerometerSettings_s_t {
    int pin;
    float bias;
    float rate;
    float avg;
} AccelerometerSettings;

class Accelerometer : public Sensor {
    public:

    enum Axis {
        X,
        Y,
        Z
    };


    Accelerometer(); // TODO
    Accelerometer(int pinX, int pinY, int pinZ); // calibrate 
    void initialize();
    int calibrate(int samples);
    void reset();
    void set(Axis axis, const int pin, float bias, float rate);

    // returns magnitude of acceleration vector
    float get();
    float getRaw();
    float getRawX();
    float getRawY();
    float getRawZ();
    float getX();
    float getY();
    float getZ();
    String toString();

    // TODO: this should be protected
    float calculateAcceleration(Axis axis);

    protected:
    // pins
    // tuning constants
    // biases
    // volts -> g conversion rate (i.e. V/g)
    // calculated in calibrate
    AccelerometerSettings settingsX;
    AccelerometerSettings settingsY;
    AccelerometerSettings settingsZ;

    // TODO 
    // need to do some linear algebra magic
    // to make sure the board's Z axis is the up direction
    // since accelerometer likely won't be mounted with Z up
    float yawOffset;
    float rollOffset;
    float pitchOffset;
};