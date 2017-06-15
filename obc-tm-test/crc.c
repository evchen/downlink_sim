/************************************************************************
CRC checking
*************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "crc.h"

uint16_t table[256];

uint16_t CRC_Optimized(uint8_t data, uint16_t check) {
	return (((check << 8) & 0xFF00) ^ table[(((check >> 8) ^ data) & 0xFF)]);;
}

void InitLTBL() {
	uint16_t i, tmp;

	for (i = 0; i < 256; i++) {
		tmp = 0;
		if ((i & 1) != 0) tmp = tmp ^ 0x1021;
		if ((i & 2) != 0) tmp = tmp ^ 0x2042;
		if ((i & 4) != 0) tmp = tmp ^ 0x4084;
		if ((i & 8) != 0) tmp = tmp ^ 0x8108;
		if ((i & 16) != 0) tmp = tmp ^ 0x1231;
		if ((i & 32) != 0) tmp = tmp ^ 0x2462;
		if ((i & 64) != 0) tmp = tmp ^ 0x48C4;
		if ((i & 128) != 0) tmp = tmp ^ 0x9188;
		table[i] = tmp;
	}
}
