#ifndef ACTOR_H
#define ACTOR_H

#include "context.h"

class Actor {
public:
  float x;
  float y;
  float rotation;
  Context* context;
  
  // constructors
  Actor();
  Actor(float, float);
  Actor(float, float, float);
  
  //functions
  virtual void draw(); // †ranslates and rotates, then renders
  virtual void render() = 0;
  virtual void update(float) = 0;
};

#endif
