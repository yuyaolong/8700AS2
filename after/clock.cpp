#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include "clock.h"
#include "gamedata.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  started(false), 
  paused(false), 
  sloMo(false), 
  sumOfTicks(SDL_GetTicks()),
  totalTicks(0),
  seconds(0),
  pauseTicks(0),
  pauseStartTicks(0),
  sumOfPauseTicks(0)
  {
  start();
}

Clock::Clock(const Clock& c) :
  ticks(c.ticks), 
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  sumOfTicks(c.sumOfTicks),
  totalTicks(c.totalTicks),
  seconds(c.seconds),
  pauseTicks(c.pauseTicks),
  pauseStartTicks(c.pauseStartTicks),
  sumOfPauseTicks(c.sumOfPauseTicks)
  {
  start();
}

void Clock::draw() const { 
}

void Clock::update() { 
	if(started)
	{
    totalTicks = SDL_GetTicks();
    ticks = totalTicks - sumOfTicks;
    sumOfTicks += ticks;
    seconds = (sumOfTicks-sumOfPauseTicks-pauseTicks)/1000;
    
    if(paused == true)
    {
      pauseTicks = totalTicks - pauseStartTicks;
    }
    else
    {
      pauseTicks = 0;
	  }
  }
  
}

unsigned int Clock::getTicksSinceLastFrame() const {
  if (paused == true)
  {
    return 0;
  }
  else
  {
    if (sloMo == false)
    {
      return ticks;
    }
    else
    {
      return ticks/2;
    }
    
  }
}

void Clock::toggleSloMo() {
    sloMo = true;
}

void Clock::toggleUnSloMo()
{
    sloMo = false;
}

int Clock::getFps() const { 
    static unsigned int lastSeconds = 0;
    static unsigned int count = 0;
    static unsigned int fps = 0;

    if (paused == true)
    {
      fps = 0;
    }
    else
    {
      if (seconds != lastSeconds)
      {
        
        lastSeconds = seconds;
        fps = count;
        count = 0;

      }
      else
      {
         count++;
      }
    }
  
    return fps;
    
    
}

int Clock::getTime() const{
    return seconds;
}

void Clock::start() { 
  started = true; 
  paused = false; 
}

void Clock::pause() { 
	paused = true;
  pauseStartTicks = sumOfTicks;
  //std::cout << "pause: Not implemented yet" << std::endl;
}

void Clock::unpause() { 
	paused = false;
  sumOfPauseTicks += pauseTicks;
  //std::cout << "unpause: Not implemented yet" << std::endl;
}



