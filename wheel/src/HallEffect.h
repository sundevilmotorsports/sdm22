/**
 * @file HallEffect.h
 * @author Joshua Tenorio
 * 
 * Represents a hall effect sensor, which maintains a count of
 * how many times a wheel has rotated. This can be used to estimate
 * the wheel's speed.
 */
#pragma once
#include "Sensor.h"

class HallEffect : public Sensor {
    public:
    // combines setPin, calibrate
    HallEffect(int pin_);
    void setPin(int pin_);

    // will block, be careful when calling this!
    // samples: number of samples to get when getting zero'd value
    int calibrate(int samples = 500); 
    
    // resets encoder count
    void reset();

    // call this in the main loop
    void onLoop();

    // returns encoder count
    float get(); 

    // returns raw analog value (or abs(raw-zeroVal))
    int getRaw();
    int getRawDiff();

    // returns speed of wheel in mph
    float getSpeed();
    
    // format:
    // he sensor: pin\tgetRaw()\tget()\tgetSpeed (mph)
    String toString();

    protected:
    int pin;
    int zeroVal = 0;
    int counts = 0;
    bool justPassed = false;
    uint32_t timestamp = 0; 
};