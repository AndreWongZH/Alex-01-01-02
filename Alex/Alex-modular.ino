#include <serialize.h>
#include <stdarg.h>

#include "packet.h"
#include "constants.h"
#include "otherconstants.h"
#include "communications.h"

volatile TDirection dir = STOP;

/*
 * Setup and start codes for external interrupts and 
 * pullup resistors.
 * 
 */
// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs.
  DDRD &= 0b00000000;
  //DDRD &= 0b11110011;
  PORTD |= 0b00001100;
  
}

// Functions to be called by INT0 and INT1 ISRs.
void leftISR()
{
  switch(dir) {
    case (FORWARD):
      leftForwardTicks++;
      forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
      break;
    case (BACKWARD):
      reverseDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
      leftReverseTicks++;
      break;
    case (LEFT):
      leftReverseTicksTurns++;
      break;
    case (RIGHT):
      leftForwardTicksTurns++;
      break;
  }
}

void rightISR()
{
  switch(dir) {
    case (FORWARD):
      rightForwardTicks++;
      break;
    case (BACKWARD):
      rightReverseTicks++;
      break;
    case (LEFT):
      rightForwardTicksTurns++;
      break;
    case (RIGHT):
      rightReverseTicksTurns++;
      break;
  }
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.       


  //activate INT1 and INT0 interrupts 
  EIMSK = 0b00000011;
  EICRA = 0b00001010;

  //ensure that interrupts are turned on
  sei();
                                                                                                                                                                 
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.
ISR(INT0_vect)
{
  leftISR();
}

ISR(INT1_vect)
{
  rightISR();
}

// Implement INT0 and INT1 ISRs above.

/*
 * Alex's setup and run codes
 * 
 */

// Clears all our counters
void clearCounters()
{
  leftForwardTicks=0;
  rightForwardTicks=0;
  leftForwardTicksTurns = 0;
  rightForwardTicksTurns = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;
  leftReverseTicksTurns = 0;
  rightReverseTicksTurns = 0;
  
  leftRevs=0;
  rightRevs=0;
  forwardDist=0;
  reverseDist=0; 
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();
  /*
  switch(which)
  {
    case 0:
      clearCounters();
      break;

    case 1:
      leftTicks=0;
      break;

    case 2:
      rightTicks=0;
      break;

    case 3:
      leftRevs=0;
      break;

    case 4:
      rightRevs=0;
      break;

    case 5:
      forwardDist=0;
      break;

    case 6:
      reverseDist=0;
      break;
  }
  */
}
// Intialize Alex's internal states

void initializeState()
{
  clearCounters();
}

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
