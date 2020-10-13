#ifndef APPLICATION_H
#define APPLICATION_H

#include "context.h"
#include "timestep.h"
#include "scene.h"

class Application {
public:
  Application();
  Application(float, float); // game width and height
  Application(float, float, Context*); // game width, game height, context
  
  typedef Application super; // so extended classes automatically have a super
  
  // misc
  Context* context; // drawing
  Timestep* timestep; // delta time
  SceneManager* sceneManager; // scenes
  
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
