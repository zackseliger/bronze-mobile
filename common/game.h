#ifndef _GAME_H_
#define _GAME_H_

#ifdef __ANDROID__
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

// utilities
void LOG(char* message);

// main stuff
GLuint loadShader(GLenum type, const char* shaderSrc);
void glSetup(double width, double height);
void glRender();

// handle events
void handleResize(double width, double height);

// touch events
void startTouch(int id, float x, float y);
void moveTouch(int id, float x, float y);
void endTouch(int id, float x, float y);

#endif
