// teensy 4.1
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
  
}

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
  if( millis() - timeout2 > 1000){
    CAN_message_t msg;
    msg.id = 0x360;
    msg.buf[0] = 4;
    msg.buf[1] = 132;
    msg.buf[2] = 17;
    msg.buf[3] = 25;
    msg.buf[4] = 23;
    msg.buf[5] = 17;
    Can0.write(msg);

    CAN_message_t msg2;
    msg2.id = 0x361;
    msg2.buf[0] = 24;
    msg2.buf[1] = 18;
    Can0.write(msg2);
    timeout2 = millis();
  }
  
}
