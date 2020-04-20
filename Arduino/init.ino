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
