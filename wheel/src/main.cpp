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
#include "HallEffect.h"
#include "SDMSerial.h"
#include "LinearPot.h"

#define USING_MAKEFILE 1
extern "C" int main(void)
{
#ifdef USING_MAKEFILE
	pinMode(13, OUTPUT);
	HallEffect he(A6);
	LinearPot pot;
	pot.initialize(A7, 0.00244379);
	SDMSerial comm({0,1}, false);
	uint32_t timestamp = millis();
	while (1) {
		//digitalWriteFast(13, HIGH);
		comm.onLoop();

		// get data
		he.onLoop();
		float speed = he.getSpeed();
		float shockTravel = pot.get();

		// send data
		uint32_t current = millis();
		if(current - timestamp > 125){
			String data = String(speed, 1) + "," + String(shockTravel, 2);
			comm.send(1, SDMSerial::PacketType::DATA, data);
			comm.flush();
			timestamp = current;
		}

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

