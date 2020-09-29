#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>

class Application {
public:
  Application();
  virtual void handleResize(float, float);
  virtual void initContext();
  virtual void update();
  virtual void render();
};

Application* getCurrentApplication();
void setCurrentApplication(Application*);

#endif // APPLICATION_H
