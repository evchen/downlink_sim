#ifndef _TRXVU_SIM_H 
#define _TRXVU_SIM_H

#include <stdint.h>

#define SEND_TM_DEF_CALLSIGN 0x10 //sent telemetry using default callsign

#define DOWNLINK_BUFFER_FULL            255
#define INVALID_CALLSIGN                254
#define INVALID_COMMAND                 253
#define FRAME_0_LENGTH                  255
#define FRAME_TOO_LARGE                 255

#define SEND_TM_DEF_CALLSIGN        0x10 //sent telemetry using default callsign
#define SET_DEFAULT_TO_CALLSIGN     0x22

#define MAXIMUM_BUFFER_SIZE         235

#define NUMBER_OF_BUFFER            40
/*
 * 
 */
//void process_command(uint8_t* command, uint32_t command_length, uint8_t* response, uint32_t* response_length);

#endif
