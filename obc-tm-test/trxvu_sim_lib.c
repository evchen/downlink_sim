#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#include "I2C.h"
#include "I2C_configuration.h"
#include "trxvu_sim_lib.h"

void pack_byte(uint8_t* pack_to_buffer, uint32_t* offset, uint8_t data){
    pack_to_buffer[*offset] = data;
    (*offset) ++ ;
}


// generate one I2C command
void generate_command(uint8_t* command_buffer, uint32_t* command_length, uint8_t command, uint8_t* parameter, uint8_t parameter_length ){
    
    uint32_t offset = 0;
    uint32_t i;
    
    // pack command to first byte
    pack_byte(command_buffer, &offset, command);
    
    // pack rest of the data
    for(i = 0; i < parameter_length ; i++){
        pack_byte(command_buffer, &offset, parameter[i]);
    }
    
    *command_length = offset;
}

/*
 * generate I2C error in return error
 * 
 * generate communication error in avail
 * 
 * 
 */

int SimTrxvu_tcSetDefToClSign	(	unsigned char index,  unsigned char * toCallsign, unsigned char* feedback ){
 
    
    int error;              // storing error code    
    uint8_t command[255];   // storing the generated command according to trxvu document
    uint32_t command_length;
    //uint8_t feedback[7];       // storing the response from trxvu
    
    
    //fromCallsign[6] = fromCallsign[6]-'0';
    generate_command(command, &command_length, SET_DEFAULT_TO_CALLSIGN, toCallsign, 7);

    // write command to prepare to send data
    
    error = I2C_write((uint32_t) index, command, command_length);
    if(error)
        return error;
    
    error = I2C_read((uint32_t) index, feedback, 7);
    if(error)
        return error;
 
    return NO_ERROR;
    
}



int SimTrxvu_tcSendAX25DefClSign(unsigned char index, unsigned char * data, unsigned char len, unsigned char* avail){

    int error = 0;              // storing error code    
    uint8_t command[255];   // storing the generated command according to trxvu document
    uint32_t command_length;
    
    
    // check length of the packet
    if (len > MAXIMUM_BUFFER_SIZE)
        return PACKET_TOO_LARGE;

    
    generate_command(command, &command_length, SEND_TM_DEF_CALLSIGN, data, len);

    // write command to prepare to send data
    
    error = I2C_write((uint32_t) index, command, command_length);
    if(error)
        return error;


    error = I2C_read((uint32_t) index, avail, 1);
    if(error)
        return error<<8;
    
    return NO_ERROR;
    
    
}



