#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include "context.h"

class Application {
public:
  Application();
  
  Context* context;
  
  virtual void init();
  virtual void update();
  virtual void render();
  
  // events
  virtual void handleResize(float, float);
  virtual void handleTouchStart(int, float, float);
  virtual void handleTouchMove(int, float, float);
  virtual void handleTouchEnd(int, float, float);
};

Application* getCurrentApplication();
void setCurrentApplication(Application*);

extern Application* currentApplication;

#endif // APPLICATION_H
