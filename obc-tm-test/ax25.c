#include <stdint.h>
#include <stdio.h>
#include "ax25.h"
#include "errors.h"


uint8_t ax25_GetFrameCount( void );

uint8_t addAX25Frame (uint8_t *ax25Buffer, uint8_t *ax25BufferSize, uint8_t *telemetryData, uint8_t telemetryDataLength) {


	if (telemetryData == NULL) {
		return ERR_MISSING_PARAMETER;
	}
	if (ax25Buffer == NULL) {
		return ERR_MISSING_PARAMETER;
	}
	if (ax25BufferSize == NULL) {
		return ERR_MISSING_PARAMETER;
	}



	// Get frame count
	uint8_t count = ax25_GetFrameCount();

	// Packet length variabels
	uint8_t packetLength = AX25_NFO_NO_DATA_SIZE + telemetryDataLength;
	uint16_t packetLengthFieldValue;

 	// For copy of telemetry data
 	uint8_t *dataPtr;
 	uint8_t *dataEndPtr;

	// Frame Identification (8 bits)
	// Version number 2bits = 00
	// Virtueal Channel Id 3bits = 000
	// Spare 3 bits = 000
	ax25Buffer[0] = 0;

	// Master Frame Count 8bits 
	ax25Buffer[1] = count;

	// VirtuallChannel Frame Count 8bits
	ax25Buffer[2] = count;

	// First Head Pointer 8bits (1111.1110) if no fragmentation
	ax25Buffer[3] = 0x00;

	// copy telemetry data
	dataPtr = (ax25Buffer + AX25_NFO_HEADER_SIZE);
	dataEndPtr = dataPtr + telemetryDataLength;
	while (dataPtr < dataEndPtr)
		*(dataPtr++) = *(telemetryData++);

	// Frame status 8bits
	// Time flag 4bits (no time = 0000)
	// Spare 2 bits = 00
	// TC Count 2 bits 
	ax25Buffer[packetLength -1] = 0;			// No idea if this works

	*ax25BufferSize = packetLength;

	return ERR_SUCCESS;
}



uint8_t ax25_GetFrameCount() {

	static uint16_t seq = 0x01;
	return seq++;
}
