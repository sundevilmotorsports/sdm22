/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Arduino.h>
#include "Logger.h"
#include "Accelerometer.h"
#include "SDMSerial.h"
#include <FlexCAN_T4.h>
#include <Adafruit_GPS.h>


#define USING_MAKEFILE
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;


#define GPSSerial Serial8
#define GPSECHO false

void canSniff(const CAN_message_t &msg);
void canSetup();
void canEvents();


extern "C" int main(void)
{
#ifdef USING_MAKEFILE
	pinMode(13, OUTPUT);
	SDMSerial comm({0,1,2}, true);
	Logger logger;
	Adafruit_GPS GPS(&GPSSerial);
	
	logger.initializeLogFile("serial");
	logger.initializeLogFile("CAN");
	logger.initializeLogFile("GPS");
	logger.initializeFile("data",
	{
		"acceleration X (g)", "acceleration Y (g)", "acceleration Z (g)", "latitude", "longitude", "GPS fix",
		"FL wheel speed (m/s)", "RR wheel speed (m/s)", "FL shock travel (in)", "RR shock travel (in)"
	});
	int i = 0;
	canSetup();
	Accelerometer accelerometer;
	accelerometer.set(Accelerometer::Axis::X, A14, 0.0, 0.55);
	accelerometer.set(Accelerometer::Axis::Y, A15, 0.0, 0.55);
	accelerometer.set(Accelerometer::Axis::Z, A16, 0.0, 0.55);

	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
	GPS.sendCommand(PGCMD_ANTENNA);
	delay(1000);
	
	while (1) {
		digitalWriteFast(13, HIGH);
		comm.onLoop();

		// CAN stuff
		canEvents();

		// accelerometer
		logger.addData("data", "acceleration X (g)", accelerometer.calculateAcceleration(Accelerometer::Axis::X));
		logger.addData("data", "acceleration Y (g)", accelerometer.calculateAcceleration(Accelerometer::Axis::Y));
		logger.addData("data", "acceleration Z (g)", accelerometer.calculateAcceleration(Accelerometer::Axis::Z));

		// GPS
		
		GPS.read();
		bool okGPS = true;
		if(GPS.newNMEAreceived()){
			logger.log("GPS", LogLevel::Status, "NMEA", GPS.lastNMEA());
			if(!GPS.parse(GPS.lastNMEA())){
				logger.log("GPS", LogLevel::Error, "NMEA", "Couldn't parse");
				okGPS = false;
			}
			else{
				logger.addData("data", "GPS fix", (float) ((int) GPS.fix)); // :clown:
				logger.log("GPS", LogLevel::Status, "FixQuality", String((int) GPS.fixquality)); // :clown:
			}
		}

		if(okGPS && GPS.fix){
			logger.addData("data", "latitude", GPS.latitude);
			logger.addData("data", "longitude", GPS.longitude);
		}


		// serial data
		std::pair<bool, std::vector<int>> status = comm.isMessageReady();
		if(status.first){
			// check each port with a ready message
			for(auto p : status.second){
				// get message
				String msg = comm.getMessage(p);
				logger.log("serial", LogLevel::Status, "Rec'd", msg);

				// parse message
				std::vector<float> data;
				std::pair<SDMSerial::PacketType, std::vector<String>> parsed = comm.parseMessage(msg);
				if(parsed.first != SDMSerial::PacketType::DATA){
					logger.log("serial", LogLevel::Error, "not type data", msg);
					continue;
				}
				for(auto& s : parsed.second){
					data.emplace_back(s.toFloat());
				}
				// if we dont have pot and he data, dont bother assigning
				if(data.size() != 2){
				logger.log("serial", LogLevel::Error, "couldn't parse", "");
					continue;
				}

				// assign data
				if(p == 1){ // left front
					logger.addData("data", "FL wheel speed (m/s)", data[0]);
					logger.addData("data", "FL shock travel (in)", data[1]);
				}
				else if(p == 2){ // right rear
					logger.addData("data", "RR wheel speed (m/s)", data[0]);
					logger.addData("data", "RR shock travel (in)", data[1]);
				}
			}
		} // end if packet recieved
		else{
			// no packet recieved
		}

		logger.writeRow("data");
		delay(5);
	}


#else
	// Arduino's main() function just calls setup() and loop()....
	setup();
	while (1) {
		loop();
		yield();
	}
#endif
}



void canSniff(const CAN_message_t &msg) {
	String message = "";
	message += "MB " + String(msg.mb) + "\t";
	message += "OVERRUN: " + String(msg.flags.overrun) + "\t";
	message += "LEN: " + String(msg.len) + "\t";
	message += "EXT: " + String(msg.flags.extended) + "\t";
	message += "TS: " + String(msg.timestamp) + "\t";
	message += "ID: " + String(msg.id, HEX) + "\t";
	message += "Buffer: ";
	for ( uint8_t i = 0; i < msg.len; i++ ) {
		message += String(msg.buf[i], HEX) + " ";
	}
	//logger.log("CAN", LogLevel::Status, "Rec'd", message);
}

void canSetup(){
  Can0.begin();
  Can0.setBaudRate(1000000); // 1 mega bit /s
  Can0.onReceive(canSniff);
}

void canEvents(){
    Can0.events();
}