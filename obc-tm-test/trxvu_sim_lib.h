/** @file */

#ifndef _TRXVU_SIM_LIB_H
#define _TRXVU_SIM_LIB_H

// error definitions
#define GENERAL_ERROR               7
#define TIME_OUT_ERROR              6
#define WRITE_ERROR                 4
#define NO_ERROR                    0
#define DOWNLINK_BUFFER_FULL            255
#define INVALID_CALLSIGN                254
#define PACKET_TOO_LARGE                252


#define SEND_TM_DEF_CALLSIGN        0x10 //sent telemetry using default callsign
#define SET_DEFAULT_TO_CALLSIGN     0x22 //change default destincation callsign

// #define SEND_TM_OVR_CALLSIGN        0x11
#define MAXIMUM_BUFFER_SIZE         235

void generate_command(uint8_t* command_buffer, uint32_t* command_length, uint8_t command, uint8_t* parameter, uint8_t parameter_length );

int SimTrxvu_tcSetDefToClSign	( unsigned char index,  unsigned char * fromCallsign, unsigned char *feedback );

/*! 
 * function is devied into 2 steps
 * 1. sending a byte to indicate the command for trxvu
 * 2. sending data to trxvu .
 * @param index I2C address
 * @param data data to be sent downinked
 * @param length length of data to be downlinked
 * @param avail pointer to buffer, where the available frame count will be stored
 * @return 
 * @note what would be written in doxygen? :0
 *    
 */

int SimTrxvu_tcSendAX25DefClSign ( unsigned char index, unsigned char *data, unsigned char length, unsigned char* avail);


#endif
