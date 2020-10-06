#include "collection.h"

Actor* Collection::add(Actor* obj) {
  this->objects.push_back(obj);
  return obj;
}

void Collection::remove(Actor* obj) {
  for (auto i = this->objects.begin(); i != this->objects.end();) {
    if ((*i) == obj) i = this->objects.erase(i);
    else i++;
  }
}

void Collection::remove(int index) {
  this->objects.erase(this->objects.begin() + index);
}

void Collection::clear() {
  this->objects.clear();
}

void Collection::update(float dt) {
  for (auto i = this->objects.begin(); i != this->objects.end(); i++) {
    (*i)->update(dt);
  }
}

void Collection::draw() {
  for (auto i = this->objects.begin(); i != this->objects.end(); i++) {
    (*i)->draw();
  }
}
