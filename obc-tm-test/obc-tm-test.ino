

#include <inttypes.h>
//#include <String.h>

extern "C"{
  #include "I2C.h"
  #include "I2C_configuration.h"
  #include "ccsds.h"
  #include "ax25.h"
  #include "crc.h"
  #include "trxvu_sim_lib.h"
}

uint8_t avail = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  InitLTBL();
  Serial.print("started!");
  Serial.println();
  Serial.flush();  
}

void loop() {
  // put your main code here, to run repeatedly:

  //uint8_t data[2] = {0x18, 13};

  //I2C_write(0x08, data, 2);

    I2C_start(I2C_BIT_RATE , I2C_TIMEOUT_RATE);
  sendHKdata();
  I2C_stop();


}
int i = 0;

void sendHKdata(){


  
  uint8_t data[80] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};

  uint8_t len = 235;
  uint8_t error;

  uint8_t ccsds_buffer[255];
  uint8_t ccsds_buffer_length;
  
  uint8_t ax25_buffer[255]; 
  uint8_t ax25_buffer_length;


  
  //pack ccsdsds

  error = CCSDS_GenerateTelemetryPacket(ccsds_buffer, &ccsds_buffer_length, 10, 1, 2, data, len, 0 );
  
  if(error){
    
    Serial.print("Error occurred during packing ccsds: ");
    Serial.print(error);
    Serial.println();
    Serial.flush();
    
  }
  else{
    //Serial.println("succeeded in packing ccsds");
  }
  
  //generate AX25
  error = addAX25Frame(ax25_buffer, &ax25_buffer_length, ccsds_buffer, ccsds_buffer_length);
  if(error){
    
    Serial.print("Error occurred during packing ax25: ");
    Serial.print(error);
    Serial.println();
    Serial.flush();
    
  }
  else{
    //Serial.println("succeeded in packing ax25");
  }

  ax25_buffer_length = 235;
  int i = 0;
  for(int i =0; i< 235; i++)
  {
    ax25_buffer[i] = 1;
  }

  //I2C_write(0x08, ax25_packet, ax25_packet_length);
  error = SimTrxvu_tcSendAX25DefClSign(0x08, ax25_buffer, ax25_buffer_length, &avail);
  //delay(1000);
   
  if(error){
    Serial.print("Error occurred from I2C: ");
    Serial.print(error);
    Serial.println();
    Serial.flush();
    delay(5000);
  }

  if(avail > 40){
    Serial.print("Error occurred during sending: ");
    Serial.print(avail);
    Serial.println();
    Serial.flush();
    delay(30000);
  }
  
  if(avail == 0){
    Serial.println("full buffer");
    delay(5000);    
  }

  else{
    Serial.println(avail);
  }
  
  
}

