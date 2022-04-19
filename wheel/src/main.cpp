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
#define USING_MAKEFILE 0
#define FLBOARD 0
extern "C" int main(void)
{
#ifdef USING_MAKEFILE

	// To use Teensy 4.0 without Arduino, simply put your code here.
	// For example:
	pinMode(13, OUTPUT);
	Serial.begin(9600);
	Serial1.begin(115200);
	float i = 0.0;
	bool readingPacket = false;
	String message = "";
	while (1) {
		#if defined(FLBOARD)
		i++;
		if(i == 255){
			i = 0;
		}
		
		if(Serial1.available()){
			int incoming = Serial1.read();
			if(incoming == '('){
				message = "";
				readingPacket = true;
			}
			if(readingPacket){
				message += (char) incoming;
				if(incoming == ')'){
					readingPacket = false;
					Serial.println("R: " + message);
					String output = "";
					if(message.equals("(RACC)")){
						output = "1.23,-15.43,15.21";
					}
					else if(message.equals("(RPOT)")){
						output = "pot";
					}
					Serial1.print("(" + output + ")");
				}
			} // end if reading
		} // if serial 1 available
		
		delay(5);

		#elif defined(BRBOARD)
		i += 25;
		if (i == 255){
			i = 0;
		}
		Serial.print("A0: " + String(analogRead(A0)));
		Serial.print("\tA1: " + String(analogRead(A1)));
		Serial.print("\tA2: " + String(analogRead(A2)));
		Serial.print("\tA6: " + String(analogRead(A6)));
		Serial.println("\taA7: " + String(analogRead(A7)));
		digitalWriteFast(13, HIGH);
		delay(500);
		digitalWriteFast(13, LOW);
		delay(500);
		#endif
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

