/*
 * Author: Johan Sjöblom
 */

#ifndef DEBUG2_H
#define DEBUG2_H

#include <inttypes.h>

#ifdef __cplusplus
	#define EXTERNC extern "C"
#else
	#define EXTERNC
#endif

EXTERNC void debugSetup();
EXTERNC void writeDebug(char *msg);
EXTERNC void printDebugs();

#undef EXTERNC

void debugSetup();
void writeDebug(char *msg);
void printDebugs();

#endif
