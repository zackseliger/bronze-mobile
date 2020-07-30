#ifndef _GAME_H_
#define _GAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

GLenum loadShader(GLenum type, const char* shaderSrc);
void glSetup(double width, double height);
void glRender();

#ifdef __cplusplus
}
#endif
#endif
