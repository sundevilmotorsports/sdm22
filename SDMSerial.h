#pragma once

#include <Arduino.h>

class SDMSerial {
    public:
    SDMSerial(bool isServer);

    enum PacketType {
        INIT, // i
        RACC, // a
        RTMP, // t
        RHES, // h
        RPOT, // p
        ERRO, // e
        DATA  // d
    };

    bool request(int port, SDMSerial::PacketType type, String& buffer, uint32_t timeout);


    protected:
    // if true, packet types will mean different things
    // than if false
    // main board: true
    // wheel board: false
    bool isServer = true;
};