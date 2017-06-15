/** @file */

/*************************************************************
header for ax25
*************************************************************/
#ifndef AX25_H
#define AX25_H

#define AX25_NFO_HEADER_SIZE 4
#define AX25_NFO_NO_DATA_SIZE 5

uint8_t addAX25Frame (uint8_t *ax25Buffer, uint8_t *ax25BufferSize, uint8_t *telemetryData, uint8_t telemetryDataLength);


#endif // AX25_H
