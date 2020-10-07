#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <time.h>

class Timestep {
public:
  Timestep();
  Timestep(int);
  
  struct timespec res;
  float deltaTime;
  float currentTime;
  float lastTime;
  int targetFPS;
  
  void update();
  float getTime();
};

#endif
