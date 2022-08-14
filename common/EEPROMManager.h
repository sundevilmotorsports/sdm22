#pragma once

#include <Arduino.h>
#include <EEPROM.h>

class EEPROMManager {
    public:
    EEPROMManager();
    enum Address {
        LogState,
        RunNo,
    }

    byte read(Address address);
    void write(Address addr, byte data);
};
