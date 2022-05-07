#pragma once

#include <vector>
#include <utility>
#include <Arduino.h>

class SDMSerial {
    public:
    SDMSerial(std::vector<int> ports, bool isServer);

    enum PacketType {
        INIT, // i
        ERRO, // e
        DATA  // d
    };

    void onLoop();
    bool send(int port, SDMSerial::PacketType, String message);
    std::pair<bool, std::vector<int>> isMessageReady();

    // clear the buffer
    String getMessage(int port);


    protected:

    // list of serial ports in use
    std::vector<int> ports;

    // corresponds to a port
    std::vector<String> buffers;
    std::vector<bool> flags;
};