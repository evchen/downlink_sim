/*
 * Author: Johan Sjöblom
 */

#include "debug2.h"
#include <string.h>
#include "HardwareSerial.h"
#include <Arduino.h>
#include <assert.h>

static int inited;
static int MAX_PRINT_SIZE = 32;
static int WRAP_LIMIT = 600; //I2C_BUFFER_SZIZE + some extra
static char *defaultPrint = "Default print";
static uint16_t readIndex; 	//index to start print from
static uint16_t writeIndex; //index to start write to
static int justWrapped;
char **debugPrints;

void debugSetup() {
	readIndex = 0;
	writeIndex = 0;
	justWrapped = 0; //false
	debugPrints = (char **) malloc(sizeof(char *) * WRAP_LIMIT);
	int i;
	for(i = 0; i < WRAP_LIMIT; i++) {
		debugPrints[i] = (char *) malloc(sizeof(char) * (MAX_PRINT_SIZE + 1)); //+1 for null-termination
	}
	inited = 1;
	Serial.println("Debug2 setup.");
}

void writeDebug(char *msg) {
	assert ( inited );
	int i;
	char *string = debugPrints[writeIndex];
	//clear any old data
	for (i = 0; i < MAX_PRINT_SIZE; i++){
		string[i] = (char)0;
	}
	//write new data
	i = 0;
	while ((msg[i] != '\0') && (i < MAX_PRINT_SIZE)) {
		string[i] = msg[i];
    i++;
	}
	string[i] = '\0'; //null terminate
	writeIndex++;
	if (writeIndex == WRAP_LIMIT) {
    assert (justWrapped == 0); //else we wrapped twice before printing, all hell break lose
		justWrapped = 1; //true
    writeIndex = 0;
	}
}

void printDebugs() {
	assert ( inited );
	if (justWrapped) {
		//print remainder of buffer
		while(readIndex != WRAP_LIMIT) {
			//Serial.print(readIndex); Serial.print(": "); 
			Serial.println(debugPrints[readIndex++]);
		}
		justWrapped = 0; //reset flag
    readIndex = 0;
	}
	//readIndex is now guaranteed <= writeIndex
	while(readIndex < writeIndex) {
		//Serial.print(readIndex); Serial.print(": "); 
		Serial.println(debugPrints[readIndex++]);
	}
}
