#include <serialize.h>
#include <stdarg.h>
#include <math.h>

#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "packet.h"
#include "constants.h"

/*
   Alex's configuration constants
*/

// Number of ticks per revolution from the
// wheel encoder.

#define COUNTS_PER_REV      195

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          23.5

// Constants for turning
#define PI              3.141592
#define ALEX_LENGTH     16
#define ALEX_BREADTH    6

float AlexDiagonal = 0.0;
float AlexCirc = 0.0;

// Motor control pins. You need to adjust these till
// Alex moves in the correct direction
//#define LF                  6   // Left forward pin
//#define LR                  5   // Left reverse pin
//#define RF                  10  // Right forward pin
//#define RR                  11  // Right reverse pin

#define LF                  5   // Left forward pin correct
#define LR                  6   // Left reverse pin
#define RF                  11  // Right forward pin
#define RR                  10  // Right reverse pin

// Motor Power Factor for making sure robot goes straight
// Values will be multiplied to power supplied to each motor
#define MOTOR_L_FACTOR  1.000
#define MOTOR_R_FACTOR  0.996

// For sleep mode and power savings
#define PRR_TWI_MASK            0b10000000
#define PRR_SPI_MASK            0b00000100
#define ADCSRA_ADC_MASK         0b10000000
#define PRR_ADC_MASK            0b00000001
#define PRR_TIMER2_MASK         0b01000000
#define PRR_TIMER0_MASK         0b00100000
#define PRR_TIMER1_MASK         0b00001000
#define SMCR_SLEEP_ENABLE_MASK  0b00000001
#define SMCR_IDLE_MODE_MASK     0b11110001

/*
      Alex's State Variables
*/

// Store the ticks from Alex's left and
// right encoders.
volatile unsigned long leftForwardTicks;
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightReverseTicks;

// Left and right encoder tiks for turning
volatile unsigned long leftForwardTicksTurns;
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns;
volatile unsigned long rightReverseTicksTurns;

// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

// Variables to keep track of whether we have moved a commanded distance
unsigned long deltaDist;
unsigned long newDist;

// Variables to keep track of stuff
unsigned long deltaTicks;
unsigned long targetTicks;

typedef enum {
  STOP = 0,
  FORWARD = 1,
  BACKWARD = 2,
  LEFT = 3,
  RIGHT = 4,
} TDirection;

volatile TDirection dir = STOP;

/*
   Alex's setup and run codes

*/

void setup() {
  // put your setup code here, to run once:

  AlexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH * ALEX_BREADTH));
  AlexCirc = PI * AlexDiagonal;

  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupPowerSaving();
  enablePullups();
  setupMotors();
  startMotors();
  initializeState();
  sei();
  stop();
}

void loop() {

  TPacket recvPacket; // This holds commands from the Pi

  TResult result = readPacket(&recvPacket);

  if (result == PACKET_OK)
    handlePacket(&recvPacket);
  else if (result == PACKET_BAD)
  {
    sendBadPacket();
  }
  else if (result == PACKET_CHECKSUM_BAD)
  {
    sendBadChecksum();
  }

  // Code to tell how much distance Alex has moved.
  if (deltaDist > 0) {
    if (dir == FORWARD) {
      if (forwardDist > newDist) {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    } else if (dir == BACKWARD) {
      if (reverseDist > newDist) {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    } else if (dir == STOP) {
      deltaDist = 0;
      newDist = 0;
      stop();
    }
  }

  // Code to tell how much angle Alex has turned.
  if (deltaTicks > 0) {
    if (dir == LEFT) {
      if (leftReverseTicksTurns >= targetTicks) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } else if (dir == RIGHT) {
      if (rightReverseTicksTurns >= targetTicks) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } else if (dir == STOP) {
      deltaTicks = 0;
      targetTicks = 0;
      stop();
    }
  }
  
}
