#include "timestep.h"
#include <time.h>

Timestep::Timestep() {
  this->targetFPS = 60;
  this->lastTime = this->getTime();
}

Timestep::Timestep(int fps) {
  this->targetFPS = fps;
  this->lastTime = this->getTime();
}

float Timestep::getTime() {
  clock_gettime(CLOCK_MONOTONIC, &this->res);
  return 1000.0 * this->res.tv_sec + (double) this->res.tv_nsec / 1e6;
}

void Timestep::update() {
  clock_gettime(CLOCK_MONOTONIC, &this->res);
  this->currentTime = this->getTime();
  this->deltaTime = (this->currentTime - this->lastTime) / (1000.0 / this->targetFPS);
  this->lastTime = this->currentTime;
}
