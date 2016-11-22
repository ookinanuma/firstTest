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
//int dataReceived;  // Data that will be received from the transmitter
int dataReceived[4];

int pwm1 = 5;//default 977Hz  ≈1kHz
int pwm2 = 6;//default 977Hz  ≈1kHz
int pwm3 = 9;//default 490Hz  ≈500Hz
int pwm4 = 10;//default 490Hz ≈500Hz
int pwm5 = 3;//default 490Hz  ≈500Hz
int pwm6 = 11;//default 490Hz ≈500Hz

void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Use the serial Monitor (Symbol on far right). Set speed to 115200 (Bottom Right)
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Simple Receive data Test"));
  Serial.println(F("Questions: terry@yourduino.com"));

  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  //myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setPALevel(RF24_PA_MAX);

  myRadio.openReadingPipe(1, addresses[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  myRadio.startListening();
  
pinMode(pwm1, OUTPUT) ;
pinMode(pwm2, OUTPUT) ;
 
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  if ( myRadio.available()) // Check for incoming data from transmitter
  {
    while (myRadio.available())  // While there is data ready
    {
      myRadio.read( &dataReceived, sizeof(dataReceived) ); // Get the data payload (You must have defined that already!)
    }
  
  analogWrite(pwm1,dataReceived[2]);
  analogWrite(pwm2,dataReceived[3]);
    
    Serial.print("Data received  ");
    for(int i;i<4;i++){
    Serial.print("  ---   "); 
    Serial.print(dataReceived[i]);   
   }
    Serial.println("do");    
    delay(10);
  } //END Radio available

}//--(end main loop )---
