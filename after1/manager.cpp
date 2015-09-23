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
	SDL_FreeSurface(backSurface);
  delete orbFrame;
  
  //delete Gamedata::getInstance();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  //backRed( Gamedata::getInstance().getXmlInt("back/red") ),
  //backGreen( Gamedata::getInstance().getXmlInt("back/green") ),
  //backBlue( Gamedata::getInstance().getXmlInt("back/blue") ),

  orbSurface( io.loadAndSet(
    Gamedata::getInstance().getXmlStr("greenorb/file"), 
    Gamedata::getInstance().getXmlBool("greenorb/transparency")) ),
  orbFrame( new Frame("greenorb", orbSurface) ),
  orb("greenorb", orbFrame),

  backSurface( io.loadAndSet(
    Gamedata::getInstance().getXmlStr("backpicture/file"), 
    Gamedata::getInstance().getXmlBool("backpicture/transparency")) ),
  backgroundfram("backpicture", backSurface),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance().getXmlStr("screenTitle") ),
  svector(),
  spriteNum(Gamedata::getInstance().getXmlInt("greenorb/spriteNum"))
{
  svector.reserve(spriteNum);
  for (unsigned int i = 0; i < spriteNum; ++i)
  {
      svector.push_back(Sprite("greenorb", orbFrame));
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
}


void Manager::drawText() const{
	io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("text/stringTitle"),
                            Gamedata::getInstance().getXmlInt("text/Titley"));

 	  io.printMessageValueAt(Gamedata::getInstance().getXmlStr("text/stringFps"),
                            clock.getFps(),
                            Gamedata::getInstance().getXmlInt("text/Fpsx"),
                            Gamedata::getInstance().getXmlInt("text/Fpsy")
                            );

     io.printMessageValueAt(Gamedata::getInstance().getXmlStr("text/stringSec"),
                            clock.getTime(),
                            Gamedata::getInstance().getXmlInt("text/Secx"),
                            Gamedata::getInstance().getXmlInt("text/Secy")
                            );

  	io.printMessageAt(Gamedata::getInstance().getXmlStr("text/stringSign"),
                      Gamedata::getInstance().getXmlInt("text/Signx"),
                      Gamedata::getInstance().getXmlInt("text/Signy"));
}

void Manager::draw() const {
  backgroundfram.draw(0,0);
  orb.draw();
  for (unsigned int i = 0; i < spriteNum; ++i)
  {
    svector[i].draw();
  }
  clock.draw();
  drawText();

  


  SDL_Flip(screen);
  
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  orb.update(ticks);
  for (unsigned int i = 0; i < spriteNum; ++i)
  {
    svector[i].update(ticks);
  }
	
	
  
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

        if (keystate[SDLK_s] && !makeVideo) {
          if (!clock.isPaused())
          {
             if ( clock.isSloMo() ) clock.toggleUnSloMo();
                else clock.toggleSloMo();;
            
          }
        }
      }
    }

    update();
    draw();
    
  }
}
