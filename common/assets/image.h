#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <png.h>
#include "fileasset.h"
#include "../opengl.h"

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

RawImageData getImageData(const char*);
void releaseImage(const RawImageData* data);
GLuint loadTexture(const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels);

GLuint createVbo(const GLsizeiptr size, const GLvoid* data, const GLenum usage);

GLuint loadImage(const char* filename, const char* name);
GLuint getImage(const char* name);

#endif
