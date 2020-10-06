#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include "context.h"

class Application {
public:
  Application();
  Application(float, float); // game width and height
  
  // drawing context
  Context* context;
  
  // screen information
  float screenWidth;
  float screenHeight;
  float gameWidth;
  float gameHeight;
  float xScale;
  float yScale;
  
  // general functions
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
