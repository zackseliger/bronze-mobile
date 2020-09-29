#include "application.h"
#include "utils.h"
#include <assert.h>

Application* currentApplication = nullptr;

Application::Application() {
  LOG("Application Constructor");
  setCurrentApplication(this);
}

void Application::handleResize(float, float) { LOG("Application handleResize"); }
void Application::initContext() { LOG("Application initContext"); }
void Application::update() { LOG("Application update"); }
void Application::render() { LOG("Application render"); }

Application* getCurrentApplication() {
  assert(currentApplication != nullptr);
  return currentApplication;
}

void setCurrentApplication(Application* application) {
  currentApplication = application;
}
