extern "C" {
  #include "debug2.h"
  #include "I2Cslave.h"
  #include "SlaveDefinitions.h"
  #include "trxvu_sim.h"
  #include "ax25_callsign.h"
  #include "kiss.h"

}

#define SEND_COUNT_TC 1
#define SEND_NEXT_TC_LENGTH 2
#define SEND_NEXT_TC 3

uint8_t kiss_packet[511];
uint32_t kiss_packet_length;

uint8_t response[255];      // store response to be sent back
uint32_t response_length;   // store response length to be sent back

uint8_t ax25_callsign_frame_buffer[40][255];
uint32_t ax25_callsign_frame_length[40];

static int head_of_queue = 0;
static int tail_of_queue = 0;

char ToCallsign[7] = "MIST  ";   // start up destination callsign
char FromCallsign[7] = "SA0SAT";  // start up source callsign


void setup() {
  SerialUSB.begin(9600);
  Serial.begin(9600);
  debugSetup();
  pinMode(7, OUTPUT); 
   
  I2C_setup(receiveCallback, transmitBeginCallback, transmitCompletedCallback);
}

void loop() {

  if( (tail_of_queue - head_of_queue) > 0){
    
    kiss_encode(kiss_packet, &kiss_packet_length, (uint8_t*)ax25_callsign_frame_buffer[head_of_queue%40], ax25_callsign_frame_length[(head_of_queue)%40] );
    Serial.write(kiss_packet, kiss_packet_length);
    digitalWrite(7,HIGH);
    SerialUSB.print("send frame ");
    SerialUSB.println(head_of_queue);
    digitalWrite(7,LOW);
    head_of_queue++;

  }
  //printDebugs();
}


void setDefaultToCallsign(uint8_t* callsign, uint32_t len, uint8_t* response, uint32_t* response_length ){    
    
    *response_length = 7;
    
    for(int i = 0 ; i < 7 ; i++){
        ToCallsign[i] = callsign[i];
        response[i]  = callsign[i];
    }    
}

void sendTMdefCallsign(uint8_t* telemetry, uint32_t len, uint8_t* response, uint32_t* response_length){

  *response_length = 1;
  if ((tail_of_queue - head_of_queue) == 40) {
    response[0] = DOWNLINK_BUFFER_FULL;
    return;
  }
  
  // pack native callsign
  addCallsign_array((uint8_t*)ax25_callsign_frame_buffer[tail_of_queue%40], ax25_callsign_frame_length+(tail_of_queue)%40, telemetry, len);
  tail_of_queue++;


  response[0] = 40 - (tail_of_queue - head_of_queue);
  
}




void process_command(uint8_t* command, uint32_t command_length, uint8_t* response, uint32_t* response_length){

    switch(command[0]){
        case SET_DEFAULT_TO_CALLSIGN : 
            //setDefaultToCallsign(command + 1, command_length-1, response, response_length);
            return;
        case SEND_TM_DEF_CALLSIGN :
            //sendTMdefCallsign(command + 1, command_length-1 , response, response_length);
            return;         
    }  
}


void receiveCallback (uint8_t *data, uint32_t dataLength) {
  //writeDebug("get I2C");
  //process_command(data, dataLength, response, &response_length);

}


// when OBC does a transmit request
// decied what data to send to OBC
// MSP packet. instert data and packet size. (no bigger than buff size)
void transmitBeginCallback () {
  int i ;

  for( i = 0 ; i < response_length ; i++){
    currentPacket.MSP_packet[i] = 1;
    //response[i];
  }
  currentPacket.packetSize = 1;
  //response_length;
  
}

// when tranmit is done to OBC
void transmitCompletedCallback() {

  }



  






