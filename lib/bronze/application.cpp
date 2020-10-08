#include "application.h"
#include "utils.h"
#include <assert.h>

Application::Application() {
  this->gameWidth = 500;
  this->gameHeight = 500;
  this->xScale = 1;
  this->yScale = 1;
  setCurrentApplication(this);
}

Application::Application(float w, float h) {
  this->gameWidth = w;
  this->gameHeight = h;
  this->xScale = 1;
  this->yScale = 1;
  setCurrentApplication(this);
}

void Application::init() { LOG("Default Application init"); }

void Application::update() {
  this->timestep->update();
}

void Application::render() {
  this->context->renderBegin();
}

void Application::handleResize(float w, float h) {
  this->screenWidth = w;
  this->screenHeight = h;
    
  // decide to fill with width or full height
  float ratio = this->screenWidth / this->screenHeight;
  if (ratio > this->gameWidth / gameHeight) {
    this->xScale = this->screenHeight / this->gameHeight;
    this->yScale = this->xScale;
  }
  else {
    this->xScale = this->screenWidth / this->gameWidth;
    this->yScale = this->xScale;
  }
  
  // redo our projection matrix
  float right = (this->gameWidth/2) + (this->screenWidth/this->xScale - this->gameWidth)/2;
  float left = (-this->gameWidth/2) + (-this->screenWidth/this->xScale + this->gameWidth)/2;
  float top = (-this->gameHeight/2) + (-this->screenHeight/this->yScale + this->gameHeight)/2;
  float bot = (this->gameHeight/2) + (this->screenHeight/this->yScale - this->gameHeight)/2;
  
  this->context->setViewport(0, 0, this->screenWidth, this->screenHeight);
  this->context->setProjection(left, right, top, bot);
}

void Application::handleTouchStart(int, float, float) { LOG("Default Application handleTouchStart"); }

void Application::handleTouchMove(int, float, float) { LOG("Default Application handleTouchMove"); }

void Application::handleTouchEnd(int, float, float) { LOG("Default Application handleTouchEnd"); }


// global function to get the current application
Application* getCurrentApplication() {
  assert(currentApplication != nullptr);
  return currentApplication;
}


// global function to set the current application
void setCurrentApplication(Application* application) {
  currentApplication = application;
}
