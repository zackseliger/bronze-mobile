#include "application.h"
#include "utils.h"
#include <assert.h>

Application::Application() {
  setCurrentApplication(this);
}

void Application::init() { LOG("Default Application init"); }
void Application::update() { LOG("Default Application update"); }
void Application::render() { LOG("Default Application render"); }

void Application::handleResize(float, float) { LOG("Default Application handleResize"); }
void Application::handleTouchStart(int, float, float) { LOG("Default Application handleTouchStart"); }
void Application::handleTouchMove(int, float, float) { LOG("Default Application handleTouchMove"); }
void Application::handleTouchEnd(int, float, float) { LOG("Default Application handleTouchEnd"); }

Application* getCurrentApplication() {
  assert(currentApplication != nullptr);
  return currentApplication;
}

void setCurrentApplication(Application* application) {
  currentApplication = application;
}
