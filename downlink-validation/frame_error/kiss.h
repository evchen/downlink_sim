#ifndef _KISS_H
#define _KISS_H


#include <stdint.h>

void kiss_encode(uint8_t* kiss_packet, uint32_t* packet_length, uint8_t* data, uint32_t data_length);

#endif