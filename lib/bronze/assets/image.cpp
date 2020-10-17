#include "image.h"
#include "../utils.h"
#include <stdlib.h>
#include <assert.h>
#include <map>

static strmap textures; // all the textures we'll load
static bool antialias = true; // anti-alias our textures or don't

static void read_png_data_callback(png_structp png_ptr, png_byte* png_data, png_size_t read_length);
static PngInfo read_and_update_info(const png_structp png_ptr, const png_infop info_ptr);
static PngHandle read_entire_png_image(const png_structp png_ptr, const png_infop info_ptr, const png_uint_32 height);
static GLenum get_gl_color_format(const int png_color_format);

RawImageData getImageData(const char* filename) {
  FileData img = getAsset(filename);
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop info_ptr = png_create_info_struct(png_ptr);
    
  png_set_interlace_handling(png_ptr);

  ReadDataHandle png_data_handle = (ReadDataHandle) {{(const png_byte*)img.data, (png_size_t)img.size}, 0};
  png_set_read_fn(png_ptr, &png_data_handle, read_png_data_callback);

  const PngInfo png_info = read_and_update_info(png_ptr, info_ptr);
  const PngHandle raw_image = read_entire_png_image(png_ptr, info_ptr, png_info.height);

  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    
  freeAsset(img); // NOT TESTED

  return (RawImageData) {
    (int)png_info.width,
    (int)png_info.height,
    (int)raw_image.size,
    get_gl_color_format(png_info.color_type),
    raw_image.data
  };
}

void releaseImage(const RawImageData* data) {
  free((void*)data->data);
}

static void read_png_data_callback(png_structp png_ptr, png_byte* raw_data, png_size_t read_length) {
  ReadDataHandle* handle = (ReadDataHandle*)png_get_io_ptr(png_ptr);
  const png_byte* png_src = handle->data.data + handle->offset;

  memcpy(raw_data, png_src, read_length);
  handle->offset += read_length;
}

static PngInfo read_and_update_info(const png_structp png_ptr, const png_infop info_ptr) {
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_read_info(png_ptr, info_ptr);
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

  // Convert transparency to full alpha
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);

  // Convert grayscale, if needed.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png_ptr);

  // Convert paletted images, if needed.
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);

  // Add alpha channel, if there is none (rationale: GL_RGBA is faster than GL_RGB on many GPUs)
  if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)
    png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

  // Ensure 8-bit packing
  if (bit_depth < 8)
    png_set_packing(png_ptr);
  else if (bit_depth == 16)
    png_set_scale_16(png_ptr);

  png_read_update_info(png_ptr, info_ptr);

  // Read the new color type after updates have been made.
  color_type = png_get_color_type(png_ptr, info_ptr);

  return (PngInfo) {width, height, color_type};
}

static PngHandle read_entire_png_image(const png_structp png_ptr, const png_infop info_ptr, const png_uint_32 height) {
  const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
  const int data_length = row_size * height;
  assert(row_size > 0);

  png_byte* raw_image = (png_byte*)malloc(data_length);
  assert(raw_image != NULL);

  png_byte* row_ptrs[height];

  png_uint_32 i;
  for (i = 0; i < height; i++) {
    row_ptrs[i] = raw_image + i * row_size;
  }

  png_read_image(png_ptr, &row_ptrs[0]);

  return (PngHandle) {raw_image, (png_size_t)data_length};
}

static GLenum get_gl_color_format(const int png_color_format) {
  assert(png_color_format == PNG_COLOR_TYPE_GRAY
         || png_color_format == PNG_COLOR_TYPE_RGB_ALPHA
         || png_color_format == PNG_COLOR_TYPE_GRAY_ALPHA);

  switch (png_color_format) {
    case PNG_COLOR_TYPE_GRAY:
      return GL_LUMINANCE;
    case PNG_COLOR_TYPE_RGB_ALPHA:
      return GL_RGBA;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      return GL_LUMINANCE_ALPHA;
  }

  return 0;
}

void setShouldAlias(bool useLinear) {
  antialias = useLinear;
}

GLuint loadTexture(const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels) {
  GLuint textureId;
  glGenTextures(1, &textureId);

  glBindTexture(GL_TEXTURE_2D, textureId);
  if (antialias) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  return textureId;
}

GLuint loadImage(const char* filename, const char* name) {
  RawImageData imageData = getImageData(filename);
  GLuint buffer = loadTexture(imageData.width, imageData.height, imageData.gl_color_format, imageData.data);
  releaseImage(&imageData);
  
  textures.insert(strmap::value_type(name, buffer));
  
  return buffer;
}

GLuint getImage(const char* name) {
  strmap::iterator it;
  
  it = textures.find(name);
  if (it == textures.end()) {
    LOG("COULDN'T FIND TEXTURE \"%s\"", name);
    return 0;
  }
  
  return it->second;
}
