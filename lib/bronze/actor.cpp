#include "actor.h"
#include "application.h"

Actor::Actor() {
  this->x = 0;
  this->y = 0;
  this->rotation = 0;
  this->context = getCurrentApplication()->context;
}
Actor::Actor(float x, float y) {
  this->x = x;
  this->y = y;
  this->rotation = 0;
  this->context = getCurrentApplication()->context;
}
Actor::Actor(float x, float y, float rot) {
  this->x = x;
  this->y = y;
  this->rotation = rot;
  this->context = getCurrentApplication()->context;
}

void Actor::draw() {
  this->context->translate(this->x, this->y);
  this->context->rotate(this->rotation);
  
  this->render();
  
  this->context->rotate(-this->rotation);
  this->context->translate(-this->x, -this->y);
}
