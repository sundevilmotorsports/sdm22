// teensy 4.1
#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "Logger.h"
#include <Accelerometer.h>
#include <Adafruit_GPS.h>


#define GPSSerial Serial8
#define GPSECHO false

Adafruit_GPS GPS(&GPSSerial);

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;
Logger logger;

Accelerometer accelerometer;

// Forward define function
void canSniff(const CAN_message_t &msg);

void setup(void) {
  logger.setup();
  logger.initializeFile("data",
  {
    "RPM", "Manifold Pressure", "Throttle Position", 
    "Fuel Pressure", "Oil Pressure", "Wideband", 
    "Coolant Temp", "Air Temp", "ECU vehicle speed (m/s)",
    "Gear", "FL wheel speed (m/s)",
    "X accel (g)", "Y accel (g)", "Z accel (g)",
    "latitude", "longitude", "GPS speed (m/s)"
  });

  accelerometer.set(Accelerometer::Axis::X, A14, 0.0, 0.55);
  accelerometer.set(Accelerometer::Axis::Y, A15, 0.0, 0.55);
  accelerometer.set(Accelerometer::Axis::Z, A16, 0.0, 0.55);

	GPS.begin(57600);
  GPS.sendCommand(PMTK_SET_BAUD_57600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	GPS.sendCommand(PGCMD_ANTENNA);
	delay(1000);


  Serial2.begin(115200);
  Serial.begin(9600); delay(400);

  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
  pinMode(13, OUTPUT);
}

void canSniff(const CAN_message_t &msg) {
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  

  // throttle, manifold, oil, fuel pressure, air temp, 
  // coolant temp , knock sensor, wideband, ECU temp, wheel speed, RPM

  // couldn't find, knock sensor, ECU temp

  
  if(msg.id == 0x360){
    // demonstration of parsing haltech RPM message
    Serial.print(" RPM: ");
    short rpmData = msg.buf[1];
    rpmData = (msg.buf[0] << 8) | rpmData;
    Serial.print(rpmData);
    logger.addData("data", "RPM", rpmData);

    // parse manafold pressure
    Serial.print(" Manifold Pressure: ");
    short manifoldPressureData = msg.buf[3];
    manifoldPressureData = (msg.buf[2] << 8) | manifoldPressureData;
    manifoldPressureData = manifoldPressureData / 10;
    Serial.print(manifoldPressureData); Serial.print("kPa");
    logger.addData("data", "Manifold Pressure", manifoldPressureData);
    
    // parse throttle position 
    Serial.print(" Throttle Position: ");
    short throttlePosData = msg.buf[5];
    throttlePosData = (msg.buf[4] << 8) | throttlePosData;
    throttlePosData = throttlePosData / 10;
    Serial.print(throttlePosData); Serial.println("%");
    logger.addData("data", "Throttle Position", throttlePosData);

    return;
  }

  if(msg.id == 0x361){
    // parse fuel pressure data
    Serial.print(" Fuel Pressure: ");
    short fuelPressureData = msg.buf[1];
    fuelPressureData = (msg.buf[0] << 8) | fuelPressureData;
    fuelPressureData = fuelPressureData / 10 - 101.3;
    Serial.print(fuelPressureData); Serial.println("kPa");
    logger.addData("data", "Fuel Pressure", fuelPressureData);

    // parse oil pressure data
    Serial.print(" Oil Pressure: ");
    short oilPressureData = msg.buf[3];
    oilPressureData = (msg.buf[2] << 8) | oilPressureData;
    oilPressureData = oilPressureData / 10 - 101.3;
    Serial.print(oilPressureData); Serial.println("kPa");
    logger.addData("data", "Oil Pressure", oilPressureData);
    return;
  }

  if(msg.id == 0x368){
    Serial.print(" Wideband: ");
    short widebandData = msg.buf[3];
    widebandData = (msg.buf[2] << 8) | widebandData;
    widebandData = widebandData / 1000;
    Serial.print(widebandData);
    logger.addData("data", "Wideband", widebandData);
  }
   if(msg.id == 0x370){
    // vehicle speed
    // 0-1, divide it by 10 for km/h
    // divide by 3.6 for m/s
    int packet = msg.buf[1];
    packet = (msg.buf[0] << 8) | packet;
    float vSpeed = (float) packet / 10.0;
    vSpeed /= 3.6;
    logger.addData("data", "ECU vehicle speed (m/s)", vSpeed);

  }

  if(msg.id == 0x3E0){
    // parse coolant temperature data
    Serial.print("Coolant Temp: ");
    short coolantTempData = msg.buf[1];
    coolantTempData = (msg.buf[0] << 8) | coolantTempData;
    coolantTempData = coolantTempData / 10;
    Serial.print(coolantTempData); Serial.print("K");
    logger.addData("data", "Coolant Temp", coolantTempData);
    //return;

  // parse air temperature data
    Serial.print(" Air Temp: ");
    short airTempData = msg.buf[3];
    airTempData = (msg.buf[2] << 8) | airTempData;
    airTempData = airTempData / 10;
    Serial.print(airTempData); Serial.print("K");
    logger.addData("data", "Air Temp", airTempData);
    return;
  }
   if(msg.id == 0x470){
    // Gear
    int gear = msg.buf[7];
    logger.addData("data", "Gear", (float) gear);
    return;
  }
  
  /*Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();*/
}

void loop() {

  // CAN
  Can0.events();

  // accelerometer
  logger.addData("data", "X accel (g)", accelerometer.calculateAcceleration(Accelerometer::Axis::X));
  logger.addData("data", "Y accel (g)", accelerometer.calculateAcceleration(Accelerometer::Axis::Y));
  logger.addData("data", "Z accel (g)", accelerometer.calculateAcceleration(Accelerometer::Axis::Z));


  // wheel speed
  if(Serial2.available())
  {
    uint8_t incoming = (uint8_t) Serial2.read();
    float speed = (float) incoming / 10.0;
    Serial.println(speed);
    logger.addData("data", "FL wheel speed (m/s)", speed);
  }

  // GPS
  //GPS.read();
  bool okGPS = true;
  if(GPS.newNMEAreceived()){
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false

    if(!GPS.parse(GPS.lastNMEA())){
      //logger.log("GPS", LogLevel::Error, "NMEA", "Couldn't parse");
      okGPS = false;
    }
  }

  if(okGPS && GPS.fix){
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    Serial.print(", ");
    Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    Serial.print("Speed (knots): "); Serial.println(GPS.speed);

    logger.addData("data", "latitude", (float) GPS.latitude);
    logger.addData("data", "longitude", (float) GPS.longitude);
    float gpsSpeed = ((float) GPS.speed) / 1.944;
    logger.addData("data", "GPS speed (m/s)", gpsSpeed);
  }
  else{
    //Serial.println(accelerometer.toString());
    //Serial.println("!okGPS || !GPS.fix");
  }


  digitalWrite(13, HIGH);
  logger.writeRow("data");
}

