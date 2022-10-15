// teensy 4.1
#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "Logger.h"
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
Logger logger;

// Forward define function
void canSniff(const CAN_message_t &msg);

void setup(void) {
  logger.setup();
  logger.initializeFile("data",
  {
    "RPM", "Manifold Pressure", "Throttle Position", 
    "Fuel Pressure", "Oil Pressure", "Wideband", 
    "Coolant Temp", "Air Temp"
  });

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
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
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

  if(msg.id == 0x3E0){
    // parse coolant temperature data
    Serial.print("Coolant Temp: ");
    short coolantTempData = msg.buf[1];
    coolantTempData = (msg.buf[0] << 8) | coolantTempData;
    coolantTempData = coolantTempData / 10;
    Serial.print(coolantTempData); Serial.print("K");
    logger.addData("data", "Coolant Temp", coolantTempData);
    return;

  // parse air temperature data
    Serial.print(" Air Temp: ");
    short airTempData = msg.buf[3];
    airTempData = (msg.buf[2] << 8) | airTempData;
    airTempData = airTempData / 10;
    Serial.print(airTempData); Serial.print("K");
    logger.addData("data", "Air Temp", airTempData);
    return;
  }

  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

void loop() {
  Can0.events();

  static uint32_t timeout = millis();
  static uint32_t timeout2 = millis();
  if ( millis() - timeout > 200 ) {
    CAN_message_t msg;
    msg.id = random(0x1,0x7FE);
    for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = i + 1;
    Can0.write(msg);
    timeout = millis();
  }
  if( millis() - timeout2 > 2000) {
    timeout2 = millis();
    //Can0.mailboxStatus();
  }
  
  digitalWrite(13, HIGH);
  logger.writeRow("data");
}

