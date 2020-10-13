#include "timestep.h"
#include <time.h>
#include "utils.h"

Timestep::Timestep() {
  this->targetFPS = 60;
  this->resetTime();
}

Timestep::Timestep(int fps) {
  this->targetFPS = fps;
  this->resetTime();
}

float Timestep::getTime() {
  clock_gettime(CLOCK_MONOTONIC, &this->tp);
  return 1000.0 * this->tp.tv_sec + (double) this->tp.tv_nsec / 1e6;
}

void Timestep::resetTime() {
  this->currentTime = this->getTime();
  this->lastTime = this->getTime();
}

void Timestep::update() {
  this->currentTime = this->getTime();
  this->deltaTime = this->targetFPS * (this->currentTime - this->lastTime) / 1000.0;
  this->lastTime = this->currentTime;
}

void Timestep::setTargetFPS(int fps) {
  this->targetFPS = fps;
}
