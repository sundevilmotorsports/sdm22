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
                if(Serial1.available() && !flags[p]){
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
    for(size_t i = 0; i < flags.size(); i++){
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

void SDMSerial::status(){

}

void SDMSerial::flush(){
    for(auto& p : ports){
        switch(p){
            case 1:
            Serial1.flush();
            break;
            case 2:
            Serial2.flush();
            break;
            case 3:
            Serial3.flush();
            break;
            case 5:
            Serial5.flush();
            break;
            default:
            break;
        }
    }
}

std::pair<SDMSerial::PacketType, std::vector<String>> SDMSerial::parseMessage(String message){
    std::pair<SDMSerial::PacketType, std::vector<String>> output;
    bool ok = true;
    String buffer = "";
    for(size_t i = 0; i < message.length(); i++){
        // check for some format errors
        if((i == 0 && message[i] != '(') || (i == message.length()-1 && message[i] != ')')){
            ok = false;
            break;
        }

        // determine type
        if(i == 1){
            switch(message[i]){
                case 'i':
                output.first = SDMSerial::PacketType::INIT;
                break;
                case 'e':
                output.first = SDMSerial::PacketType::ERRO;
                break;
                case 'd':
                output.first = SDMSerial::PacketType::DATA;
                break;
                default:
                output.first = SDMSerial::PacketType::ERRO;
                break;
            }
            continue;
        } // end if i == 1

        if(i == 2 && message[i] != ','){
            ok = false;
            break;
        }
        else if(i == 2 && message[i] == ','){
            continue;
        }

        // deal with parsing
        if(message[i] == ',' || message[i] == ')'){
            output.second.emplace_back(buffer);
            buffer = "";
        }
        else{
            buffer += message[i];
        }
    } // end for c : message

    // error handling
    if(!ok){
        output.first = SDMSerial::PacketType::ERRO;
        output.second.clear();
    }
    return output;
}