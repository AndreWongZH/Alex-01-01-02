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
  //DDRD &= 0b11110011;
  DDRD &= 0b00000000;
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
      reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
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

// BARE METAL
// Set up the external interrupt pins INT0 and INT1
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered.
  EIMSK = 0b00000011; // activate INT1 and INT0 interrupts
  EICRA = 0b00001010; // falling-edge triggered

  // ensure that interrupts are turned on
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

// BARE METAL
// ISRs for serial comms
// Write data received from UDR0 to the receive buffer
//ISR(USART_RX_vect)
//{
//  // Write received data to dataRecv
//  unsigned char data = UDR0;
//  writeBuffer(&_recvBuffer, data);
//}
//
//ISR(USART_UDRE_vect)
//{
//  unsigned char data;
//  TPacket packet = readBuffer(&_xmitBuffer, &data);
//
//  // if available, read from transmit buffer
//  // else disable UDRE interrupt
//  if (packet == BUFFER_OK)
//    UDR0 = data;
//  else if (result == BUFFER_EMPTY)
//    UCSR0B &= 0b11011111; // clrs UDRIE0
//}

ISR(TIMER0_COMPA_vect) {

}

ISR(TIMER0_COMPB_vect) {
  
}

ISR(TIMER1_COMPB_vect) {
  
}

ISR(TIMER2_COMPA_vect){
  
}
