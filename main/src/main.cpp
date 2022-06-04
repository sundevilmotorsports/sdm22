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
#include <FlexCAN_T4.h>
#include "Logger.h"
#include "LinearPot.h"
#include "Accelerometer.h"
#include "SDMSerial.h"
#include "CAN.h"
#define USING_MAKEFILE


extern "C" int main(void)
{
#ifdef USING_MAKEFILE
	pinMode(13, OUTPUT);
	SDMSerial comm({0,1}, true);
	Logger logger;
	logger.initializeFile("test-owo", {"i", "owo"});
	int i = 0;
	canSetup();
	Accelerometer accelerometer;
	accelerometer.set(Accelerometer::Axis::X, A14, 0.0, 1.0);
	accelerometer.set(Accelerometer::Axis::Y, A15, 0.0, 1.0);
	accelerometer.set(Accelerometer::Axis::Z, A16, 0.0, 1.0);
	LinearPot frontRight;
	LinearPot rearLeft;
	frontRight.initialize(A10, 0.00210396);
	rearLeft.initialize(A11, 0.00210396);
	while (1) {
		comm.onLoop();
		Can0.events();
		Serial.println(accelerometer.toString());
		// serial data
		std::pair<bool, std::vector<int>> status = comm.isMessageReady();
		if(status.first){
			for(auto p : status.second){
				Serial.print(comm.getMessage(p));
			}
		}
		else{ // no packet recieved
		}

		logger.writeRow("test-owo");
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

