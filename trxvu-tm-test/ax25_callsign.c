#include <stdint.h>
#include <stdlib.h>

#include "ax25_callsign.h"
#include "crc.h"


void addCallsign(uint8_t *ax25_packet, uint32_t* ax25_packet_length, uint8_t* data, uint32_t data_length) {
  
  uint32_t i;
  uint16_t chk;

  // calculating the length of ax25 packet
  *ax25_packet_length = data_length + 1 + 7 + 7 + 1 + 1 + 2 + 1;
  
  // add flag
  ax25_packet[0] = 0x7E;
  // add destincation callsign
  for ( i = 1 ; i < CALLSIGN_LENGTH; i ++ ) {
    ax25_packet[i] = (uint8_t) DEFAULT_CALLSIGN_GS[ i - 1 ] << 1;
  }

  // add ssid
  ax25_packet[CALLSIGN_LENGTH] = 0x60;
  
  // add source callsign
  for ( i = CALLSIGN_LENGTH+1 ; i <( CALLSIGN_LENGTH << 1) ; i ++ ){
    ax25_packet[i] = (uint8_t) DEFAULT_CALLSIGN_TRXVU[ i - 1 - CALLSIGN_LENGTH ] << 1;
  }

  ax25_packet[ CALLSIGN_LENGTH <<1 ] = 0x61;

  ax25_packet[15] = 0x3;   // control bits
  ax25_packet[16] = 0xF0;  // protocol identifier

  // adding data
  for ( i = 0 ; i < data_length ; i ++ ){
    ax25_packet[ i + 16 ] = data[i];
  }

  chk = 0xFFFF;
  // adding CRC
  for (i = 0; i < *ax25_packet_length - 3; i++) {
    chk = CRC_Optimized(ax25_packet[i], chk);
  }
  
  ax25_packet[*ax25_packet_length - 3] = (uint8_t)(chk >> 8);
  ax25_packet[*ax25_packet_length - 2] = (uint8_t)(chk);

  // add end flag
  ax25_packet[*ax25_packet_length - 1] = 0x7E;
  
  
}

void addData(uint8_t* array, uint32_t* offset, uint8_t data){
    
    array[*offset] = data;
    (*offset) ++;
    
}

void addCallsign_array(uint8_t* ax25_packet, uint32_t* ax25_packet_length, uint8_t* data, uint32_t data_length){

  uint32_t i;
  uint32_t offset = 0;
  uint16_t chk;

  // calculating the length of ax25 packet
  *ax25_packet_length = data_length + 7 + 7 + 1 + 1  ;
  
  
  for (i = 0; i < CALLSIGN_LENGTH - 1 ; i ++ ){
        addData(ax25_packet, &offset, DEFAULT_CALLSIGN_TRXVU[i]<<1);
  }
  
  addData(ax25_packet, &offset, 0xE0);
  
  // add source callsign
  for (i = 0; i < CALLSIGN_LENGTH - 1 ; i ++ ){
        addData(ax25_packet, &offset, DEFAULT_CALLSIGN_GS[i]<<1);
  }
  
  addData(ax25_packet, &offset, 0x61);
 
  addData(ax25_packet, &offset, 0x3);   // control bits
  addData(ax25_packet, &offset, 0xF0);  // protocol identifier

  // adding data
  for ( i = 0 ; i < data_length ; i ++ ){
    addData(ax25_packet, &offset, data[i]);
  }
/*
  chk = 0xFFFF;
  // adding CRC
  for (i = 0; i < *ax25_packet_length - 3; i++) {
    chk = CRC_Optimized(ax25_packet[i], chk);
  }
  addData(ax25_packet, &offset,(uint8_t)(chk >> 8) );
  addData(ax25_packet, &offset,(uint8_t)(chk) );
 */ 
  
}

uint8_t* addCallsign_withoutflag(uint32_t* ax25_packet_length, uint8_t* data, uint32_t data_length) {

  uint32_t i;
  uint32_t offset = 0;
  uint16_t chk;

  // calculating the length of ax25 packet
  *ax25_packet_length = data_length + 7 + 7 + 1 + 1  ;
  
  uint8_t* ax25_packet = (uint8_t*) malloc(sizeof(*ax25_packet_length));
  
  for (i = 0; i < CALLSIGN_LENGTH - 1 ; i ++ ){
        addData(ax25_packet, &offset, DEFAULT_CALLSIGN_TRXVU[i]<<1);
  }
  
  addData(ax25_packet, &offset, 0xE0);
  
  // add source callsign
  for (i = 0; i < CALLSIGN_LENGTH - 1 ; i ++ ){
        addData(ax25_packet, &offset, DEFAULT_CALLSIGN_GS[i]<<1);
  }
  
  addData(ax25_packet, &offset, 0x61);
 
  addData(ax25_packet, &offset, 0x3);   // control bits
  addData(ax25_packet, &offset, 0xF0);  // protocol identifier

  // adding data
  for ( i = 0 ; i < data_length ; i ++ ){
    addData(ax25_packet, &offset, data[i]);
  }
/*
  chk = 0xFFFF;
  // adding CRC
  for (i = 0; i < *ax25_packet_length - 3; i++) {
    chk = CRC_Optimized(ax25_packet[i], chk);
  }
  addData(ax25_packet, &offset,(uint8_t)(chk >> 8) );
  addData(ax25_packet, &offset,(uint8_t)(chk) );
 */ 
  
}
