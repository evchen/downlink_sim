/************************************************************************
CCSDS packeting
*************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "ccsds.h"
#include "crc.h"
#include "errors.h"

uint16_t CCSDS_GetSequenceCount();


uint8_t CCSDS_GenerateTelemetryPacket(uint8_t *telemetryBuffer, uint8_t *telemetryBufferSize, uint16_t apid,
									uint8_t serviceType, uint8_t serviceSubtype, uint8_t *sourceData, uint8_t sourceDataLength,
									uint32_t time) {

	// CRC
	uint16_t chk;

	// Get sequence
	uint16_t sequenceCount = CCSDS_GetSequenceCount();

	if(sourceData == NULL)
		return ERR_MISSING_PARAMETER; // no data
	if(telemetryBuffer == NULL)
		return ERR_MISSING_PARAMETER; // no data
	if(telemetryBufferSize == NULL)
		return ERR_MISSING_PARAMETER; // no data


	// Packet length variabels
	uint8_t packetLength = TM_NONDATASIZE + sourceDataLength;
	uint16_t packetLengthFieldValue;

 	
 	// For copy of source data
 	uint8_t *dataPtr;
 	uint8_t *dataEndPtr;

 	if(*telemetryBufferSize < packetLength)
 		return ERR_CCSDS_BUFFER_TOO_SMALL; // buffer too small


	// Packet header (Packet ID, Packet Sequence Control, Packet Length)
	// PacketID 16 bits 
	// Version number (3 bits): 000 
	// Type (1 bit): 0 
	// Data Field Header Flag (1 bit): 1
	// APID 11 bits
	telemetryBuffer[0] = 0x8 | ((uint8_t)(apid >> 8) & 0x7);  				// Version number, Type, Data Field Header Flag, APID (3 MSB)
	telemetryBuffer[1] = (uint8_t) apid;									// APID (8 LSB)

	// Packet Sequence Control
	// Grouping flags (2 bits): 11
	// Sequence Count 14 bits
	telemetryBuffer[2] = 0xc0 | ((uint8_t) (sequenceCount >> 8) & 0x3f);	// Grouping flags, Sequence Count (6 MSB)
	telemetryBuffer[3] = (uint8_t) sequenceCount;							// Sequence Count (8 LSB)

	// Packet length 16 bits
	packetLengthFieldValue = packetLength - 7;
	telemetryBuffer[4] = (uint8_t) (packetLengthFieldValue >> 8);
	telemetryBuffer[5] = (uint8_t) packetLengthFieldValue;

	// Telemetry Data Field Header
	// Spare (1 bit): 0
	// TM Source Packet PUS Version Number (3 bits): 001
	// Spare (4 bits): 0000
	// Servicetype 8 bits
	// ServiceSubtype 8 bits
	// Time 40 bits
	telemetryBuffer[6] = 0x10;					// Spare, TM Source PacketPUS version, Spare
	telemetryBuffer[7] = serviceType;			// Servicetype
	telemetryBuffer[8] = serviceSubtype;		// Service Subtype

	// Absolute time
	/*
	this part is the original code
	telemetryBuffer[9] = (uint8_t) (time >> 28);
	telemetryBuffer[10] = (uint8_t) (time >> 20);
	telemetryBuffer[11] = (uint8_t) (time >> 12);
	telemetryBuffer[12] = (uint8_t) (time >> 4);
	telemetryBuffer[13] = (uint8_t) (time << 4);
	*/
	telemetryBuffer[9] = (uint8_t)0x20;
	telemetryBuffer[10] = (uint8_t)0x6B;
	telemetryBuffer[11] = (uint8_t)0xBB;
	telemetryBuffer[12] = (uint8_t)0x00;
	telemetryBuffer[13] = (uint8_t)0x00;

	// Copy source data
	dataPtr = (telemetryBuffer + TM_HEADER_SIZE);
	dataEndPtr = dataPtr + sourceDataLength;
	while (dataPtr < dataEndPtr)
		*(dataPtr++) = *(sourceData++);

	// Packet Error Control (16 bits)
	chk = 0xFFFF;

	// Compute CRC
	uint8_t i;
	for (i = 0; i < packetLength - 2; i++) {
		chk = CRC_Optimized(telemetryBuffer[i], chk);
	}

	// Fill CRC field
	telemetryBuffer[packetLength - 2] = (uint8_t)(chk >> 8);
	telemetryBuffer[packetLength - 1] = (uint8_t)(chk);

	*telemetryBufferSize = packetLength;

	return ERR_SUCCESS;
}


uint16_t CCSDS_GetSequenceCount() {
	/*
		original value is 0
		static uint16_t seq = 0;
	*/
	static uint16_t seq = 0xc000;
	return seq++;
}
