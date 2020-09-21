#include "application.h"

Application* currentApplication;

Application* getCurrentApplication() {
    return currentApplication;
}

void setCurrentApplication(Application* application) {
    currentApplication = application;
}

