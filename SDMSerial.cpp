#include "SDMSerial.h"

SDMSerial::SDMSerial(std::vector<int> ports, bool isServer){
    this->isServer = isServer;
    this->ports = ports;
    buffers.resize(9);
    for(auto& b : buffers) b = "";
    flags.resize(9);
    for(auto f : flags) f = false;
    for(auto& p : ports){
        switch(p){
            case 1:
            Serial1.begin(115200);
            break;
            case 2:
            Serial2.begin(115200);
            break;
            case 5:
            Serial5.begin(115200);
            break;
            default:
            break;
        }
 
    }
}

void SDMSerial::onLoop(){
    for(auto& p : ports){
        // TODO: the rest of the ports
        switch(p){
            case 1:{
                if(Serial1.available()){
                    byte incoming = Serial1.read();
                    buffers[p] += String((char) incoming);
                    // end of packet
                    if(incoming == ')'){
                        flags[p] = true;
                    }
                }
                break;
            }
            case 2:{
                if(Serial2.available()){
                    byte incoming = Serial2.read();
                    buffers[p] += String((char) incoming);
                    // end of packet
                    if(incoming == ')'){
                        flags[p] = true;
                    }
                }
                break;
            }
            case 5:{
                if(Serial5.available()){
                    byte incoming = Serial5.read();
                    buffers[p] += String((char) incoming);
                    // end of packet
                    if(incoming == ')'){
                        flags[p] = true;
                    }
                }
                break;
            }
            default:
            break;
        }
    } // end for p : ports
}

bool SDMSerial::send(int port, SDMSerial::PacketType, String message){
    String outgoing = "";
    return true;
}

std::pair<bool, std::vector<int>> SDMSerial::isMessageReady(){
    // TODO
}

String SDMSerial::getMessage(int port){
    String message = buffers[port];
    flags[port] = false;
    buffers[port] = "";
    return message;
}