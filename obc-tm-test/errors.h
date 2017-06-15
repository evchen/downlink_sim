/** @file*/
#ifndef ERRORS_H
#define ERRORS_H

/*
Telecommands acceptance errors
*/
#define ILLEGAL_APID			0
#define INVALID_LENGTH			1
#define INCORRECT_CHECKSUM		2
#define ILLEGAL_PACKET_TYPE		3
#define ILLEGAL_PACKET_SUBTYPE	4
#define ILLEGAL_APP_DATA		5

/*
Telemetry generation error
*/
#define ERR_SUCCESS 				0
#define ERR_MISSING_PARAMETER		1
#define ERR_CCSDS_TO0_MUCH_DATA		2
#define ERR_CCSDS_BUFFER_TOO_SMALL	3


#endif // ERRORS_H
