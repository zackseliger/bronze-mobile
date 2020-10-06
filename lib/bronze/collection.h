#ifndef COLLECTION_H
#define COLLECTION_H

#include "actor.h"
#include <vector>

class Collection {
public:
  std::vector<Actor*> objects;
  
  Actor* add(Actor*);
  void clear();
  void remove(Actor*);
  void remove(int);
  
  void update(float);
  void draw();
};

#endif
