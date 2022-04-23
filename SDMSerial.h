#pragma once

#include <Arduino.h>

class SDMSerial {
    public:
    SDMSerial();

    enum PacketType {
        RACC,
        RTMP,
        RHES,
        RPOT,
        ERRO,
        DATA
    };

    // TODO should this be blocking?
    bool request(int port, SDMSerial::PacketType type, String& buffer, uint32_t timeout);


    protected:

};