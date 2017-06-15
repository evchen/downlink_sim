#ifndef _AX25_CALLSIGN_H
#define _AX25_CALLSIGN_H


#include <stdint.h>

#define DEFAULT_CALLSIGN_TRXVU "SA0SAT"
#define DEFAULT_CALLSIGN_GS "MIST  "
#define CALLSIGN_LENGTH 7 

void addCallsign_array(uint8_t* ax25_packet, uint32_t* ax25_packet_length, uint8_t* data, uint32_t data_length);
void addCallsign(uint8_t *ax25_packet, uint32_t* ax25_packet_length, uint8_t* data, uint32_t data_length);
uint8_t* addCallsign_withoutflag(uint32_t* ax25_packet_length, uint8_t* data, uint32_t data_length); 

#endif
