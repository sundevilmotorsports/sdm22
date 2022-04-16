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

extern "C" int main(void)
{
#ifdef USING_MAKEFILE
	Serial.begin(9600);
	Serial1.begin(9600);
	Logger logger;
	logger.initializeFile("test", {"i","i+1", "Serial1"});
	pinMode(13, OUTPUT);
	float i = 0;
	while (1) {
		logger.addData("test", "i", i);
		logger.addData("test", "i+1", i+1.0);
		logger.addData("test", "Serial1", (float) Serial1.availableForWrite());
		Serial.println("Available to write on Serial1: " Serial1.availableForWrite());
		i++;
		if(i == 100){
			i = 0;
			digitalWriteFast(13, HIGH);
			delay(100);
			digitalWriteFast(13, LOW);
		}
		logger.writeRow("test");
		delay(20);
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

