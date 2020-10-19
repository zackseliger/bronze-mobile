#include "event.h"

#include <functional>
#include <map>

typedef std::map<EventType, EventListener*> evtmap;
static evtmap eventmap;

TouchEvent::TouchEvent(int id, float x, float y) {
  this->id = id;
  this->x = x;
  this->y = y;
}

WindowEvent::WindowEvent(float width, float height) {
  this->width = width;
  this->height = height;
}

EventListener::EventListener(std::function<void(Event*)> func) {
  this->function = func;
}

void setEventListener(EventType type, EventListener* listener) {
  eventmap[type] = listener;
}

void removeEventListener(EventType type) {
  eventmap.erase(type);
}

void emitEvent(EventType type, Event* event) {
  evtmap::iterator it;
  it = eventmap.find(type);
  if (it != eventmap.end()) {
    it->second->function(event);
  }
}
