#include <SDL.h>
#include <string>
#include <deque>
class Manager;

class Clock {
public:
  static Clock& getInstance();  // This class is a Singleton
  unsigned int getTicksSinceLastFrame() const;
  unsigned int getTicksSinceInit() const { return sumOfTicks; }

  void toggleSloMo();
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  int getFps() const;
  int getTime() const;

  void start();
  void pause();
  void unpause();
  void draw() const;  // For debugging
  void update();  // Increments time/ticks

private:
  unsigned int ticks;
  bool started;
  bool paused;
  bool sloMo;
  unsigned int sumOfTicks;


  int lastTime;
  int nowTime;
  int fpsCounter;
  int fps;
  int totleTime;
		
  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
