/*
   Alex's motor drivers.

*/

// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors()
{
  /* Our motor set up is:
        A1IN - Pin 5, PD5, OC0B
        A2IN - Pin 6, PD6, OC0A
        B1IN - Pin 10, PB2, OC1B
        B2In - pIN 11, PB3, OC2A
  */
}

// Start the PWM for Alex's motors.
// We will implement this later. For now it is
// blank.
void startMotors()
{

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

  analogWrite(LF, MOTOR_L_FACTOR * val);
  analogWrite(RF, MOTOR_R_FACTOR * val);
  analogWrite(LR, 0);
  analogWrite(RR, 0);

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

  analogWrite(LR, MOTOR_L_FACTOR * val);
  analogWrite(RR, MOTOR_R_FACTOR * val);
  analogWrite(LF, 0);
  analogWrite(RF, 0);

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
  analogWrite(LR, MOTOR_L_FACTOR * val);
  analogWrite(RF, MOTOR_R_FACTOR * val);
  analogWrite(LF, 0);
  analogWrite(RR, 0);
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
  analogWrite(RR, MOTOR_R_FACTOR * val);
  analogWrite(LF, MOTOR_L_FACTOR * val);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
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
  analogWrite(LF, 0);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
  analogWrite(RR, 0);
}
