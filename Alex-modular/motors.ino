/*
   Alex's motor drivers.

*/

// BARE METAL
// Set up Alex's motors using PWM.
void setupMotors()
{
  /* Our motor set up is:
        A1IN - Pin 5, PD5, OC0B
        A2IN - Pin 6, PD6, OC0A
        B1IN - Pin 10, PB2, OC1B
        B2In - pIN 11, PB3, OC2A
  */
  // Set motor pins to output
  DDRD |= 0b01100000;
  DDRB |= 0b00001100;
  
  // Enable timer interrupts
  TIMSK0 |= 0b110;
  TIMSK1 |= 0b100;
  TIMSK2 |= 0b10;

  // Init all count and compare values to 0
  TCNT0 = 0;
  TCNT1 = 0;
  TCNT2 = 0;
  OCR0A = 0; // Pin 6,LR
  OCR0B = 0; // Pin 5, LF
  OCR1B = 0; // Pin 10, RR
  OCR2A = 0; // Pin 11, RF
}

// BARE METAL
// Start the PWM for Alex's motors.
void startMotors()
{
  // start the timers
  TCCR0A = 0b10100001; // phase correct PWM, clear on match
  TCCR0B = 0b00000100; // prescaler 256
  TCCR1A = 0b00100001; // phase correct PWM, 8-bit, clear on match
  TCCR1B = 0b00000100; // prescaler 256
  TCCR2A = 0b10000001; // phase correct PWM, clear on match
  TCCR2B = 0b00000100; // prescaler 256
}

// Convert percentages to PWM values
int pwmVal(float speed)
{
  if (speed < 0.0)
    speed = 0;

  if (speed > 100.0)
    speed = 100.0;

  return (int) ((speed / 100.0) * 255.0);
}

// Computes ticks required to turn a certain angle
unsigned long computeDeltaTicks(float ang)
{
  unsigned long ticks = (unsigned long) ((ang * AlexCirc * COUNTS_PER_REV) / (360.0 * WHEEL_CIRC));

  return ticks;
}

// BARE METAL
// Replacement function for analogWrite
void writeMotor(int motor, int pwmVal) {
  switch(motor)
  {
    case LR:
      OCR0A = pwmVal;
      break;
    case LF:
      OCR0B = pwmVal;
    case RR:
      OCR1B = pwmVal;
      break;
    case RF:
      OCR2A = pwmVal;
      break;
  }
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward(float dist, float speed)
{
  // Code to tell us how far to move
  if (dist > 0) deltaDist = dist;
  else deltaDist = 9999999;
  newDist = forwardDist + deltaDist;

  dir = FORWARD;
  int val = pwmVal(speed);

  // For now we will ignore dist and move
  // forward indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.

//  analogWrite(LF, MOTOR_L_FACTOR * val);
//  analogWrite(RF, MOTOR_R_FACTOR * val);
//  analogWrite(LR, 0);
//  analogWrite(RR, 0);
  writeMotor(LF, MOTOR_L_FACTOR * val);
  writeMotor(RF, MOTOR_R_FACTOR * val);
  writeMotor(LR, 0);
  writeMotor(RR, 0);
}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverse(float dist, float speed)
{

  if (dist > 0) deltaDist = dist;
  else deltaDist = 9999999;
  newDist = reverseDist + deltaDist;

  dir = BACKWARD;
  int val = pwmVal(speed);

  // For now we will ignore dist and
  // reverse indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.

//  analogWrite(LR, MOTOR_L_FACTOR * val);
//  analogWrite(RR, MOTOR_R_FACTOR * val);
//  analogWrite(LF, 0);
//  analogWrite(RF, 0);
  writeMotor(LR, MOTOR_L_FACTOR * val);
  writeMotor(RR, MOTOR_R_FACTOR * val);
  writeMotor(LF, 0);
  writeMotor(RF, 0);
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left(float ang, float speed)
{
  dir = LEFT;
  int val = pwmVal(speed);

  if (ang > 0) deltaTicks = computeDeltaTicks(ang);
  else deltaTicks = 9999999;
  targetTicks = leftReverseTicksTurns + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
//  analogWrite(LR, MOTOR_L_FACTOR * val);
//  analogWrite(RF, MOTOR_R_FACTOR * val);
//  analogWrite(LF, 0);
//  analogWrite(RR, 0);
  writeMotor(LR, MOTOR_L_FACTOR * val);
  writeMotor(RF, MOTOR_R_FACTOR * val);
  writeMotor(LF, 0);
  writeMotor(RR, 0);
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right(float ang, float speed)
{
  dir = RIGHT;
  int val = pwmVal(speed);

  if (ang > 0) deltaTicks = computeDeltaTicks(ang);
  else deltaTicks = 9999999;
  targetTicks = rightReverseTicksTurns + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
//  analogWrite(RR, MOTOR_R_FACTOR * val);
//  analogWrite(LF, MOTOR_L_FACTOR * val);
//  analogWrite(LR, 0);
//  analogWrite(RF, 0);
  writeMotor(RR, MOTOR_R_FACTOR * val);
  writeMotor(LF, MOTOR_L_FACTOR * val);
  writeMotor(LR, 0);
  writeMotor(RF, 0);
}

void brake(int brakeFactor)
{
  // Braking mechanism, moves in the opposite direction of wherever it is going
  // This is to ensure Alex stops where we want it to stop.
  switch (dir) {
    case (FORWARD):
      reverse(0, 100);
      break;
    case (BACKWARD):
      forward(0, 100);
      break;
    case (LEFT):
      right(0, 100);
      break;
    case (RIGHT):
      left(0, 100);
      break;
  }
  delay(brakeFactor);
}

// Stop Alex. To replace with bare-metal code later.
void stop()
{
  brake(25);

  dir = STOP;
//  analogWrite(LF, 0);
//  analogWrite(LR, 0);
//  analogWrite(RF, 0);
//  analogWrite(RR, 0);
  writeMotor(LF, 0);
  writeMotor(LR, 0);
  writeMotor(RF, 0);
  writeMotor(RR, 0);
}
