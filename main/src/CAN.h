#pragma once

#include <FlexCAN_T4.h>

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;


void canSetup();
void canSniff(const CAN_message_t &msg);
