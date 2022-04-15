# Teensy 4.X boilerplate code
## Purpose
A starting point for a Teensy 4.X project, without using the Arduino IDE
## How to use
- Download Teensyduino [here](https://www.pjrc.com/teensy/td_download.html).
- Put your code in `src/`
- Put libraries you use in `contrib/` (TODO)
- make a `core/` folder and copy paste the teensy4 core from Teensyduino (minus `main.cpp` and `Makefile`)
- edit `Makefile` according to your teensy version
- use `make` to build code
- use `make upload` to upload
## TODO
- [x] don't automatically upload when building 
- [x] set up `Makefile` to handle `contrib`
- [x] include Arduino libraries (such as `SD.h`)
- [ ] have `setup.sh` do things (i.e. set ARDUINOPATH)
- [ ] have `Makefile` use core, libraries, etc. from the arduino installation instead of copy pasting here (since we don't need to edit core files)
## Where things came from
- `core/` is from [Teensyduino 1.56](https://github.com/PaulStoffregen/cores/tree/master/teensy4) 
- `Makefile` is edited from the Teensyduino 1.56 core
