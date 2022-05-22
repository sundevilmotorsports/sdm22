#include "SDMSerial.h"

SDMSerial::SDMSerial(std::vector<int> ports, bool isServer){
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
            Serial.begin(9600);
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

bool SDMSerial::send(int port, SDMSerial::PacketType type, String message){
    String outgoing = "(";
    switch(type){
        case SDMSerial::PacketType::ERRO:{
            outgoing += "e,";
            break;
        }
        case SDMSerial::PacketType::INIT:{
            outgoing += "i,";
            break;
        }
        case SDMSerial::PacketType::DATA:{
            outgoing += "d,";
            break;
        }
        default:{
            break;
        }
    }
    outgoing += message;
    outgoing += ")";

    switch(port){
        case 1:{
            Serial1.print(outgoing);
            break;
        }
        case 2:{
            Serial2.print(outgoing);
            break;
        }
        case 5:{
            Serial5.print(outgoing);
            break;
        }
        default:{
            Serial.print(outgoing);
            break;
        }
    }
    return true;
}

std::pair<bool, std::vector<int>> SDMSerial::isMessageReady(){
    bool isReady = false;
    std::vector<int> readyPorts;
    for(int i = 0; i < flags.size(); i++){
        if(flags[i]){
            isReady = true;
            readyPorts.emplace_back(i);
        }
    }
    std::pair<bool, std::vector<int>> output;
    output.first = isReady;
    output.second = readyPorts;
    return output;
}

String SDMSerial::getMessage(int port){
    String message = buffers[port];
    flags[port] = false;
    buffers[port] = "";
    return message;
}