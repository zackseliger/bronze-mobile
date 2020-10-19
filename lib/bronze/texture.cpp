#include "texture.h"
#include <stdlib.h>

Texture::Texture() {
  this->width = 0;
  this->height = 0;
  this->data = nullptr;
  this->id = 0;
}

Texture::Texture(int w, int h, void* data, uint32_t id) {
  this->width = w;
  this->height = h;
  this->data = data;
  this->id = id;
}

Texture::~Texture() {
  free(this->data);
}
