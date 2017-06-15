/** @file
/*
 * debug.h
 *
 * Author: John Wikman
 *
 * Description:
 * Defines prototypes for debugging prints.
 *
 * NOTE!
 * Only the following functions
 * should be called when debugging:
 * - debugln
 * - debughexln
 * - debug
 * - debughex
 *
 * The file that uses the debugging must define/undefine
 * ENABLE_DEBUG in order for debug prints to work.
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef ENABLE_DEBUG
/* Uncomment this to disable debug throughout the program. */
  #undef ENABLE_DEBUG
#endif

/* Re-enable debug in a file if enforced */
#if (defined ENFORCE_DEBUG) && !(defined ENABLE_DEBUG)
  #define ENABLE_DEBUG
#endif

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void single_line_message_debug(const char *msg);
void single_line_hexnumber_message_debug(const char *msg, uint32_t hex);
void single_line_decnumber_message_debug(const char *msg, int32_t dec);
void message_debug(const char *msg);
void hexnumber_message_debug(const char *msg, uint32_t hex);
void decnumber_message_debug(const char *msg, int32_t dec);

#ifdef __cplusplus
}
#endif


#ifdef ENABLE_DEBUG
  #define debugln(MSG)          single_line_message_debug(MSG);
  #define debughexln(MSG, HEX)  single_line_hexnumber_message_debug(MSG,HEX);
  #define debugdecln(MSG, DEC)  single_line_decnumber_message_debug(MSG,DEC);
  #define debug(MSG)            message_debug(MSG);
  #define debughex(MSG, HEX)    hexnumber_message_debug(MSG,HEX);
  #define debugdec(MSG, DEC)    decnumber_message_debug(MSG,DEC);
#else
  #define debugln(MSG)
  #define debughexln(MSG, HEX)
  #define debugdecln(MSG, DEC)
  #define debug(MSG)
  #define debughex(MSG, HEX)
  #define debugdec(MSG, DEC)
#endif


#endif
