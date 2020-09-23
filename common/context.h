#ifndef CONTEXT_H
#define CONTEXT_H

#include "opengl.h"
#include "assets/fileasset.h"
#include <freetype-gl.h>

class OpenGLContext {
public:
  // program for color stuff
  GLuint colorProgram;
  GLint c_uProjLoc;
  GLint c_uCamLoc;
  GLint c_posLoc;
  GLint c_color;

  // program for textures
  GLuint textureProgram;
  GLint p_uProjLoc;
  GLint p_uCamLoc;
  GLint p_posLoc;
  GLint p_texLoc;
  GLint p_texCoord;
  GLuint p_color;
  
  // freetype stuff
  FileData fontFile;
  ftgl::texture_atlas_t* texAtlas;
  ftgl::texture_font_t* texFont;
  void setFont(const char*);
  
  // function to set up programs
  void init();
  
  // projecetion and camera stuff
  float projection[16];
  void setViewport(float, float, float, float);
  void setProjection(GLfloat*);
  
  // drawing functions
  void drawRect(float, float, float, float);
  void drawImage(GLuint, float, float, float, float);
  void drawText(const char*, float, float, float, float, float);
};

#endif // CONTEXT_H
