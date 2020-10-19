#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

class Texture {
public:
  Texture();
  Texture(int, int, void*, uint32_t);
  ~Texture();
  
  int width;
  int height;
  void* data;
  uint32_t id;
};

#endif // TEXTURE_H
