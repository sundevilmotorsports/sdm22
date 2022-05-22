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

    // buffer gets read into here
    void onLoop();

    // sends a message on the specified port
    bool send(int port, SDMSerial::PacketType, String message);

    // returns true if there is a message ready, and the vector is a list of ports that are ready
    std::pair<bool, std::vector<int>> isMessageReady();

    // copies the message, clear the buffer
    // if message isnt ready we return empty string
    String getMessage(int port);

    // prints debug info on all the things
    // will probably slow down code execution
    void status();

    std::pair<SDMSerial::PacketType, String> parseMessage(String message);


    protected:

    // list of serial ports in use
    std::vector<int> ports;

    // corresponds to a port
    // e.g. buffers[1] refers to Serial1, and so on
    std::vector<String> buffers;
    std::vector<bool> flags;
};