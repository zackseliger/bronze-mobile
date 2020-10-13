#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <time.h>

class Timestep {
private:
  struct timespec tp;
  float currentTime;
  float lastTime;
  int targetFPS;
  
  float getTime();
  
public:
  Timestep();
  Timestep(int);
  
  float deltaTime;
  
  // to reset lastTime and currentTime before update
  // is called after not being called for a while
  // (prevents huge or extremely negative deltaTime)
  void resetTime();
  
  void update();
  void setTargetFPS(int);
};

#endif
