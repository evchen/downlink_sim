#include <stdint.h>
#include <stdlib.h>

#define FEND    0xc0
#define FESC    0xdb
#define TFEND   0xdc
#define TFESC   0xdd

void put_byte(uint8_t * out, uint32_t * offset, const uint8_t c) 
{ 
        if (c == FEND) 
        { 
                out[(*offset)++] = FESC; 
                out[(*offset)++] = TFEND; 
        } 
        else if (c == FESC) 
        { 
                out[(*offset)++] = FESC; 
                out[(*offset)++] = TFESC; 
        } 
        else 
        { 
                out[(*offset)++] = c; 
        } 
}


void kiss_encode(uint8_t* kiss_packet, uint32_t* packet_length, uint8_t* data, uint32_t data_length){
	
	uint8_t i;
	
	// kiss header
	kiss_packet[0] = FEND;
	kiss_packet[1] = 0x00;
	
	*packet_length = 2;
	// convert and store in buffer
	for ( i = 0 ; i < data_length ; i ++){		
		put_byte(kiss_packet, packet_length, data[i]);
	}
	
	kiss_packet[(*packet_length)++] = FEND;
	
}
