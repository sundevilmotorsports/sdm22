# Sun Devil Motorsports SDM-22 DAQ Package
This repository is currently being reorganized, so please excuse the dust :)

We are currently swapping to PlatformIO for our build system.
---------
## About
Teensy 4.0/4.1 code for SDM22's Data Acquisition (DAQ) Package, which consists of a main board and two wheel boards.

Pictures TODO
## Repository Directory
- `tests` contains various test/example scripts for sensors and communication
- `main` contains the code uploaded to the main board's Teensy 4.1
- `wheel` contains the code uploaded to the wheel board's Teensy 4.0
## Build Instructions
Currently only supporting Linux. MacOS might work.

1. Install the latest version of [Teensyduino](https://www.pjrc.com/teensy/td_download.html).
This project was built using Teensyduino 1.56.

```bash
# Get the project
$ git clone git@github.com:sundevilmotorsports/sdm22.git
$ cd sdm22

# Change directory to the relevant project
$ cd main

# Compile
$ make ARDUINOPATH='abspath/to/arduino/installation'

# if compiling for wheel board, be sure to set the board
# options: FLBOARD (default if not specified), BRBOARD
$ make WHEELBOARD=FLBOARD

# Upload
$ make upload
```
## More Information
- PCB files can be found [here](https://github.com/sundevilmotorsports/pcb-dev)
- Data Analyzer Software TODO
