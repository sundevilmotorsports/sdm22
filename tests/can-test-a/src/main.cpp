// teensy 4.0
#include <Arduino.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

// Forward define function
void canSniff(const CAN_message_t &msg);

void setup(void) {
  Serial.begin(115200); delay(400);
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
  
  if(msg.id == 0x360){
    // demonstration of parsing haltech RPM message
    Serial.print(" RPM: ");
    short rpmData = msg.buf[1];
    rpmData = (msg.buf[0] << 8) | rpmData;
    Serial.print(rpmData);

    // parses manafold pressure
    Serial.print(" Manifold Pressure: ");
    short manifoldPressureData = msg.buf[3];
    manifoldPressureData = (msg.buf[2] << 8) | manifoldPressureData;
    manifoldPressureData = manifoldPressureData / 10;
    Serial.print(manifoldPressureData); Serial.print("kPa");
    
    // parses throttle position 
    Serial.print(" Throttle Position: ");
    short throttlePosData = msg.buf[5];
    throttlePosData = (msg.buf[4] << 8) | throttlePosData;
    throttlePosData = throttlePosData / 10;
    Serial.print(throttlePosData); Serial.println("%");

    return;
  }

  if(msg.id == 0x361){
    Serial.print(" Fuel Pressure: ");
    short fuelPressureData = msg.buf[1];
    fuelPressureData = (msg.buf[0] << 8) | fuelPressureData;
    fuelPressureData = fuelPressureData / 10 - 101.3;
    Serial.print(fuelPressureData); Serial.println("kPa");

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

}
