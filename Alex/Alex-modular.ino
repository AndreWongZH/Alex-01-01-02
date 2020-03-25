#include <serialize.h>
#include <stdarg.h>

#include "packet.h"
#include "constants.h"
#include "otherconstants.h"
#include "communications.h"
#include "motors.h"
#include "ISRs.h"

volatile TDirection dir = STOP;

/*
 * Alex's setup and run codes
 * 
 */

void setup() {
  // put your setup code here, to run once:

  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupMotors();
  startMotors();
  enablePullups();
  initializeState();
  sei();
}

void loop() {

// Uncomment the code below for Step 2 of Activity 3 in Week 8 Studio 2

 //forward(0, 50);
 //reverse(0, 50);
 //analogWrite(RR, 0);
 //analogWrite(RF, 75);

// Uncomment the code below for Week 9 Studio 2


 // put your main code here, to run repeatedly:
  TPacket recvPacket; // This holds commands from the Pi

  TResult result = readPacket(&recvPacket);
  
  if(result == PACKET_OK)
    handlePacket(&recvPacket);
  else
    if(result == PACKET_BAD)
    {
      sendBadPacket();
    }
    else
      if(result == PACKET_CHECKSUM_BAD)
      {
        sendBadChecksum();
      } 
}
