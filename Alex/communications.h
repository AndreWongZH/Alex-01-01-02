#ifndef __COMMUNICATIONS_H__
#define __COMMUNICATIONS_H__

/*
 * 
 * Alex Communication Routines.
 * 
 */
 
TResult readPacket(TPacket *packet);
void sendStatus();
void sendMessage(const char *message);
void dbprint(char *format, ...);
void sendBadPacket();
void sendBadChecksum();
void sendBadCommand();
void sendBadResponse();
void sendOK();
void sendResponse(TPacket *packet);

/*
 * Setup and start codes for serial communications
 * 
 */
// Set up the serial connection. For now we are using 
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial();

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.
void startSerial();

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid. 
// This will be replaced later with bare-metal code.
int readSerial(char *buffer);

// Write to the serial port. Replaced later with
// bare-metal code
void writeSerial(const char *buffer, int len);

void handleCommand(TPacket *command);
void waitForHello();
void handlePacket(TPacket *packet);

#endif
