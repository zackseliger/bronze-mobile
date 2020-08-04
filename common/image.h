#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <png.h>

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

struct PngHandle {
    const png_byte* data;
    const png_size_t size;
};

struct ReadDataHandle {
    const PngHandle data;
    png_size_t offset;
};

struct PngInfo {
    const png_uint_32 width;
    const png_uint_32 height;
    const int color_type;
};

typedef struct {
    const int width;
    const int height;
    const int size;
    const GLenum gl_color_format;
    const void* data;
} RawImageData;

RawImageData getImage(const char*);
void releaseImage(const RawImageData* data);
GLuint loadTexture(const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels);

GLuint createVbo(const GLsizeiptr size, const GLvoid* data, const GLenum usage);

#endif
