#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  SDL_FreeSurface(orbSurface);
  delete orbFrame;
  //delete Gamedata::getInstance();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io->getScreen() ),
  backRed( Gamedata::getInstance()->getXmlInt("back/red") ),
  backGreen( Gamedata::getInstance()->getXmlInt("back/green") ),
  backBlue( Gamedata::getInstance()->getXmlInt("back/blue") ),

  orbSurface( io->loadAndSet(
    Gamedata::getInstance()->getXmlStr("greenorb/file"), 
    Gamedata::getInstance()->getXmlBool("greenorb/transparency")) ),
  orbFrame( new Frame("greenorb", orbSurface) ),
  orb("greenorb", orbFrame),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance()->getXmlStr("username") ),
  frameMax( Gamedata::getInstance()->getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance()->getXmlStr("screenTitle") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
}

void Manager::drawBackground() const {
  SDL_FillRect( screen, NULL, 
    SDL_MapRGB(screen->format, backRed, backGreen, backBlue) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void Manager::drawText() const{
	io->printMessageCenteredAt("Project #1 Solution",0);

 	std::stringstream ss;
  	ss << "Avg Fps: " << clock.getFps();
  	io->printMessageAt(ss.str(),0,0);

  	ss.str("");
  	ss<< "Seconds: " << clock.getTime();
  	io->printMessageAt(ss.str(),0,20);

  	io->printMessageAt("Yaolong Yu's project",0,400);
}

void Manager::draw() const {
  drawBackground();
  orb.draw();
  clock.draw();
  drawText();

  


  SDL_Flip(screen);
  
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  orb.update(ticks);
	
	
  
//make viedo fram
  if ( makeVideo && frameCount < frameMax ) {
    std::stringstream strm;
    strm << "frames/" << username<< '.'  
         << std::setfill('0') << std::setw(4) 
         << frameCount++ << ".bmp";
    std::string filename( strm.str() );
    std::cout << "Making frame: " << filename << std::endl;
    SDL_SaveBMP(screen, filename.c_str());
  }
 

}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  while ( not done ) {

    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if (event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }

        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
      }
    }

    draw();
    update();
  }
}
