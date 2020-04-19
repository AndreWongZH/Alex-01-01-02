/*
  Alex's Power Management features.
 */

// Turns off the Watchdog Timer.
void WDT_off(void)
{
  // clear global interrupt
  cli();

  MCUSR &= (1<<WDRF);

  WDTCSR |= (1<<WDCE) | (1<<WDE);

  WDTCSR = 0x00;

  sei();
}

// Setup power saving
void setupPowerSaving()
{
  // Turn off the Watchdog Timer
  WDT_off();

  // Modify PRR to shut down TWI
  PRR |= (PRR_TWI_MASK);

  // Modify PRR to shut down SPI
  PRR |= (PRR_SPI_MASK);

  // Modify ADCSRA to disable ADC
  // then modify PRR to shut down ADC
  ADCSRA &= (~ADCSRA_ADC_MASK);
  PRR |= (PRR_ADC_MASK);

  // Set the SMCR to choose the IDLE sleep mode
  // Do not set the Sleep Enable (SE) bit yet
  SMCR &= (SMCR_IDLE_MODE_MASK);

  // Set Port B Pin 5 as output pin, then write a logic LOW
  // to it so that the LED tied to Arduino's Pin 13 is OFF.
  DDRB |= (1<<5);
  PORTB &= 0b11011111;
}

// Puts Alex to sleep
void putArduinoToIdle()
{
  // Modify PRR to shut down TIMER 0, 1, and 2
  PRR |= (PRR_TIMER2_MASK);
  PRR |= (PRR_TIMER1_MASK);
  PRR |= (PRR_TIMER0_MASK);

  // Modify SE bit in SMCR to enable sleep
  SMCR |= (SMCR_SLEEP_ENABLE_MASK);

  //The following function puts ATmega328P's MCU into sleep
  sleep_cpu();

  // Modify SE bit in SMCR to disable sleep
  SMCR &= ~SMCR_IDLE_MODE_MASK;

  // Modify PRR to power up TIMER 0, 1, and 2
  PRR &= (~PRR_TIMER2_MASK);
  PRR &= (~PRR_TIMER1_MASK);
  PRR &= (~PRR_TIMER0_MASK);
}
