#ifndef __ISRS_H__
#define __ISRS_H__

/*
 * Setup and start codes for external interrupts and 
 * pullup resistors.
 * 
 */
// Enable pull up resistors on pins 2 and 3
void enablePullups();

// Functions to be called by INT0 and INT1 ISRs.
void leftISR();

void rightISR();

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT();

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.
ISR(INT0_vect);

ISR(INT1_vect);

#endif