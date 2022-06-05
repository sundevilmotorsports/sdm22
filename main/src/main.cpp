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
#include "LinearPot.h"
#include "Accelerometer.h"
#include "SDMSerial.h"
#include <FlexCAN_T4.h>


#define USING_MAKEFILE
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;

void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

void canSetup(){
  Can0.begin();
  Can0.setBaudRate(1000000); // 1 mega bit /s
  Can0.onReceive(canSniff);
}

void canEvents(){
    Can0.events();
}

extern "C" int main(void)
{
#ifdef USING_MAKEFILE
	pinMode(13, OUTPUT);
	SDMSerial comm({0,1}, true);
	Logger logger;
	//logger.initializeFile("test-owo", {"i", "owo"});
	logger.initializeFile("data",
	{
		"acceleration X (g)", "acceleration Y (g)", "acceleration Z (g)", "latitude", "longitude", "gpsVelocity (knots)",
		"FL wheel speed (m/s)", "RR wheel speed (m/s)", "FL shock travel (in)", "RR shock travel (in)"
	});
	int i = 0;
	canSetup();
	Accelerometer accelerometer;
	accelerometer.set(Accelerometer::Axis::X, A14, 0.0, 0.55);
	accelerometer.set(Accelerometer::Axis::Y, A15, 0.0, 0.55);
	accelerometer.set(Accelerometer::Axis::Z, A16, 0.0, 0.55);
	
	while (1) {
		digitalWriteFast(13, HIGH);
		comm.onLoop();
		//canEvents();
		//Serial.println(accelerometer.toString());
		// serial data
		std::pair<bool, std::vector<int>> status = comm.isMessageReady();
		if(status.first){
			for(auto p : status.second){
				Serial.println(comm.getMessage(p));
			}
		}
		else{ // no packet recieved
		}

		//logger.writeRow("test-owo");
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

