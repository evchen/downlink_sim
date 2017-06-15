/*
 * debug.cpp
 *
 * Authors: Johan Sjöblom & John Wikman
 *
 * Description:
 * Contains implementations of debugging prints for arduino.
 */

#include "debug.h"

#include "HardwareSerial.h"

#include "Arduino.h"

void single_line_message_debug(const char *msg)
{
	Serial.print("[");
	Serial.print(msg);
	Serial.println("]");
  Serial.flush();
}

void single_line_hexnumber_message_debug(const char *msg, uint32_t hex)
{
	Serial.print("[");
  if (*msg != 0) {
    Serial.print(msg);
    Serial.print(": ");
  }
  Serial.print("0x");
	Serial.print(hex, HEX);
	Serial.println("]");
  Serial.flush();
}

void single_line_decnumber_message_debug(const char *msg, int32_t dec)
{
  Serial.print("[");
  if (*msg != 0) {
    Serial.print(msg);
    Serial.print(": ");
  }
  Serial.print(dec, DEC);
  Serial.println("]");
  Serial.flush();
}

void message_debug(const char *msg)
{
	Serial.print("[");
	Serial.print(msg);
	Serial.print("]");
  Serial.flush();
}

void hexnumber_message_debug(const char *msg, uint32_t hex)
{
	Serial.print("[");
  if (*msg != 0) {
    Serial.print(msg);
    Serial.print(": ");
  }
  Serial.print("0x");
	Serial.print(hex, HEX);
	Serial.print("]");
  Serial.flush();
}

void decnumber_message_debug(const char *msg, int32_t dec)
{
  Serial.print("[");
  if (*msg != 0) {
    Serial.print(msg);
    Serial.print(": ");
  }
  Serial.print(dec, DEC);
  Serial.print("]");
  Serial.flush();
}

