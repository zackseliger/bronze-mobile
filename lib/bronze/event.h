#ifndef EVENT_H
#define EVENT_H

#include <functional>

enum EventType {
  TouchStart, TouchMove, TouchEnd,
  WindowResize
};

class Event {
public:
  EventType type;
};

class TouchEvent : public Event {
public:
  TouchEvent(int, float, float);
  
  int id;
  float x;
  float y;
};

class WindowEvent : public Event {
public:
  WindowEvent(float, float);
  
  float width;
  float height;
};

class EventListener {
public:
  EventListener(std::function<void(Event*)>);
  
  std::function<void(Event*)> function;
};

void setEventListener(EventType, EventListener*);
void removeEventListener(EventType);
void emitEvent(EventType, Event*);

#endif
