#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <time.h>

class Timestep {
public:
  Timestep();
  Timestep(int);
  
  struct timespec tp;
  float deltaTime;
  float currentTime;
  float lastTime;
  int targetFPS;
  
  // to reset lastTime and currentTime before update
  // is called after not being called for a while
  // (prevents huge or extremely negative deltaTime)
  void resetTime();
  
  void update();
  float getTime();
};

#endif
