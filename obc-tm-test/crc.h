/** @file*/
/*************************************************************
header for crc
*************************************************************/
#include <stdint.h>

#ifndef CRC_H
#define CRC_H


void InitLTBL();
uint16_t CRC_Optimized(uint8_t data, uint16_t check);


#endif // CCSDS_H
