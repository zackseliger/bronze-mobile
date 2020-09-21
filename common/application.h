#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>

class Application {
public:
    Application();
    void update();
    void render();
    void init();
};

Application* getCurrentApplication();
void setCurrentApplication(Application*);

#endif // APPLICATION_H
