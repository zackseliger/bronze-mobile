#ifndef _GAME_H_
#define _GAME_H_

#include "opengl.h"

// main stuff
GLuint buildShader(GLenum type, const char* shaderSrc);
void glSetup(double width, double height);
void glRender();

// handle events
void handleResize(double width, double height);

// touch events
void startTouch(int id, float x, float y);
void moveTouch(int id, float x, float y);
void endTouch(int id, float x, float y);

#endif
