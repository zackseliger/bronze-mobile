#ifndef _GAME_H_
#define _GAME_H_

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#elif __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

typedef struct FileData {
    const long size;
    const void* data;
    const void* handle;
} FileData;

// utilities
void LOG(const char* message);

// assets
#ifdef __ANDROID__
void initAssetManager(AAssetManager* manager);
#endif
FileData getAsset(const char* filename);
void freeAsset(FileData asset);

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
