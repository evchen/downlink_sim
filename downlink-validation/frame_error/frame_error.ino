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

uint8_t ax25_callsign_frame_buffer[NUMBER_OF_BUFFER][255];
uint32_t ax25_callsign_frame_length[NUMBER_OF_BUFFER];

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
char flag = 0;
void loop() {
  
  // after bufffer is full, print all the data
  if( (tail_of_queue - head_of_queue) >= NUMBER_OF_BUFFER ){
    int i;
    flag = 1;
    for( i = 0 ; i < NUMBER_OF_BUFFER ; i ++ )
    {
      // print one frame
      Serial.print("Frame number ");
      Serial.print(i, DEC);
      Serial.print("\t");
      int j;
      for( j = 0 ; j < ax25_callsign_frame_length[i] ; j ++ ){
        Serial.print(ax25_callsign_frame_buffer[i][j], HEX);
      }
      Serial.println();
       
    kiss_encode(kiss_packet, &kiss_packet_length, (uint8_t*)ax25_callsign_frame_buffer[i], ax25_callsign_frame_length[i] );
    SerialUSB.write(kiss_packet, kiss_packet_length);
    }
  }
  if(flag)
  {
    while(1);
  }
  
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
  if ((tail_of_queue - head_of_queue) == NUMBER_OF_BUFFER) {
    response[0] = DOWNLINK_BUFFER_FULL;
    return;
  }
  if(len == 0){
    response[0] = FRAME_0_LENGTH;
    return;
  }
  if(len >235){
    response[0] = FRAME_TOO_LARGE; 
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
            setDefaultToCallsign(command + 1, command_length-1, response, response_length);
            return;
        case SEND_TM_DEF_CALLSIGN :
            sendTMdefCallsign(command + 1, command_length-1 , response, response_length);
            return;         
    }  
}


void receiveCallback (uint8_t *data, uint32_t dataLength) {
  //writeDebug("get I2C");
  process_command(data, dataLength, response, &response_length);

}


// when OBC does a transmit request
// decied what data to send to OBC
// MSP packet. instert data and packet size. (no bigger than buff size)
void transmitBeginCallback () {
  int i ;

  for( i = 0 ; i < response_length ; i++){
    currentPacket.MSP_packet[i] = response[i];
  }
  currentPacket.packetSize = response_length;
  
}

// when tranmit is done to OBC
void transmitCompletedCallback() {

  }



  






