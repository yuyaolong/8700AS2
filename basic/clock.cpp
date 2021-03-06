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
  sumOfTicks(0),
  lastTime(0),
  nowTime(0),
  fpsCounter(0),
  fps(0),
  totleTime(0)
  {
  start();
}

Clock::Clock(const Clock& c) :
  ticks(c.ticks), 
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  sumOfTicks(c.sumOfTicks),
  lastTime(c.lastTime),
  nowTime(c.nowTime),
  fpsCounter(c.fpsCounter),
  fps(c.fps),
  totleTime(c.totleTime)
  {
  start();
}

void Clock::draw() const { 
}

void Clock::update() { 
	if(started)
	{
		
		if(paused)
		{
			ticks = 0;
      			fps = 0;
		//	fpsCounter = 0;
		}
		else
		{
			fpsCounter++;
			ticks = 5;
			sumOfTicks += ticks;

      nowTime = clock();
      if( (nowTime - lastTime) > (CLOCKS_PER_SEC/4) )
      {

        lastTime = nowTime;
        fps = fpsCounter*2;
        fpsCounter = 0;
        totleTime++;
        //std::cout<<rand()*1.0/RAND_MAX<<std::endl;
      }
		}
		
	}
	else
	{
		ticks = 0;
	}

  
}

unsigned int Clock::getTicksSinceLastFrame() const {
  return ticks;
}

void Clock::toggleSloMo() {
}

int Clock::getFps() const { 
  return fps;
}

int Clock::getTime() const{
  return totleTime/2;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  lastTime = clock();
}

void Clock::pause() { 
	paused = true;
  //std::cout << "pause: Not implemented yet" << std::endl;
}

void Clock::unpause() { 
	paused = false;
  //std::cout << "unpause: Not implemented yet" << std::endl;
}



