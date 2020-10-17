#include "application.h"
#include "utils.h"
#include <assert.h>
#include "assets/sound.h"
#include "event.h"

Screen::Screen(float w, float h) {
  this->width = w;
  this->height = h;
}

Application::Application() {
  this->gameWidth = 500;
  this->gameHeight = 500;
  this->xScale = 1;
  this->yScale = 1;
  
  this->sceneManager = new SceneManager();
  this->timestep = new Timestep(60);
  
  setCurrentApplication(this);
}

Application::Application(float w, float h) {
  this->gameWidth = w;
  this->gameHeight = h;
  this->xScale = 1;
  this->yScale = 1;
  
  this->sceneManager = new SceneManager();
  this->timestep = new Timestep(60);
  
  setCurrentApplication(this);
}

Application::Application(float w, float h, Context* ctx) {
  this->gameWidth = w;
  this->gameHeight = h;
  this->xScale = 1;
  this->yScale = 1;
  
  this->sceneManager = new SceneManager();
  this->timestep = new Timestep(60);
  this->context = ctx;
  this->screen = new Screen(0,0);
  
  setCurrentApplication(this);
}

void Application::init() {
  initAudio();
  
  // set default resize callback
  setEventListener(EventType::WindowResize, new EventListener([this](Event* e) {
    WindowEvent* evt = static_cast<WindowEvent*>(e);
    
    this->screen->width = evt->width;
    this->screen->height = evt->height;
      
    // decide to fill with width or full height
    float ratio = this->screen->width / this->screen->height;
    if (ratio > this->gameWidth / gameHeight) {
      this->xScale = this->screen->height / this->gameHeight;
      this->yScale = this->xScale;
    }
    else {
      this->xScale = this->screen->width / this->gameWidth;
      this->yScale = this->xScale;
    }
    
    // redo our projection matrix
    float right = (this->gameWidth/2) + (this->screen->width/this->xScale - this->gameWidth)/2;
    float left = (-this->gameWidth/2) + (-this->screen->width/this->xScale + this->gameWidth)/2;
    float top = (-this->gameHeight/2) + (-this->screen->height/this->yScale + this->gameHeight)/2;
    float bot = (this->gameHeight/2) + (this->screen->height/this->yScale - this->gameHeight)/2;
    
    this->context->setViewport(0, 0, this->screen->width, this->screen->height);
    this->context->setProjection(left, right, top, bot);
  }));
}

void Application::update() {
  this->timestep->update();
  this->sceneManager->update(this->timestep->deltaTime);
}

void Application::render() {
  this->context->renderBegin();
  this->sceneManager->render();
}

// global function to get the current application
Application* getCurrentApplication() {
  assert(currentApplication != nullptr);
  return currentApplication;
}


// global function to set the current application
void setCurrentApplication(Application* application) {
  currentApplication = application;
}
