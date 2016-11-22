/* 
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 7
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
 */

/*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
/*-----( Declare Constants and Pin Numbers )-----*/
//None yet
/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio (7, 8); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
byte addresses[][6] = {"1Node"}; // Create address for 1 pipe.
//int dataTransmitted;  // Data that will be Transmitted from the transmitter
int dataTransmitted[4];


int sensorPin1 = A0;    
int sensorPin2 = A1;  
int sensorValue1_raw = 0;  // variable to store the value coming from the sensor
int sensorValue1_pwm =0;
int sensorValue1_s = 0;
int sensorValue2_raw = 0;
int sensorValue2_pwm =0;
int sensorValue2_s =0;
int data[4];
byte inputchar =0;
int count =0;
int forb=0;
int forb_fix=0;
int state;
void setup()  
{

  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Simple Transmit data Test"));
  Serial.println(F("Questions: terry@yourduino.com"));
  dataTransmitted[0] = 100; 
  myRadio.begin();  
  myRadio.setChannel(108);
  myRadio.setPALevel(RF24_PA_MAX);  

  myRadio.openWritingPipe( addresses[0]); 
  delay(1000);
}


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  int inputchar;
 
  // シリアルポートより1文字読み込む
  inputchar = Serial.read();
  if(inputchar != -1 ){
    // 読み込んだデータが -1 以外の場合　以下の処理を行う
    switch(inputchar){
      case 'w':   
      state=8;
         if(forb<240){
          forb=forb+30;
          forb_fix=abs(forb); 
         }
        break;
      case 's':  
        state=8;
          if(forb> -240) {
            forb=forb-30;
            forb_fix=abs(forb); 
          }
        break;       
      case 'a':  
        state=4;
        break; 
      case 'd':  
        state=6;
        break;   
      default:
        state=8;
        forb=0;
        forb_fix=abs(forb); 
        break;
    }
  } else {      
  }
  
  if(forb >= 0 ){
    sensorValue1_s=0;  
    sensorValue2_s=0;
    sensorValue1_pwm=forb_fix;  
    sensorValue2_pwm=forb_fix;           
  }
   if(forb < 0 ){
    sensorValue1_s=1;  
    sensorValue2_s=1;
    sensorValue1_pwm=forb_fix;  
    sensorValue2_pwm=forb_fix;           
  }
   if(forb_fix==0 && state==4){
    sensorValue1_s=1;  
    sensorValue2_s=0;
    sensorValue1_pwm=150;  
    sensorValue2_pwm=150; 
  }if(forb_fix==0 && state==6){
    sensorValue1_s=0;  
    sensorValue2_s=1;
    sensorValue1_pwm=150;  
    sensorValue2_pwm=150; 
  }
  
  data[0]=sensorValue1_s;
  data[1]=sensorValue2_s;  
  data[2]=sensorValue1_pwm;
  data[3]=sensorValue2_pwm;

  Serial.print("state is  ");Serial.print(state);
  Serial.print("  power is  R:");Serial.print(sensorValue1_pwm);
  Serial.print("   L:");Serial.println(sensorValue2_pwm);
  myRadio.write( &data, sizeof(data) ); //  Transmit the data

  delay(10);

}

