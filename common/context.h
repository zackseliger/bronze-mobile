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
  
  // general program
  GLuint generalProgram;
  GLint g_uProjLoc;
  GLint g_uCamLoc;
  GLint g_posLoc;
  GLint g_texLoc;
  GLint g_texCoord;
  GLint g_color;
  GLint g_uColorTint;
  float red;
  float green;
  float blue;
  float alpha;
  
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
  void setProjection(float, float, float, float);
  void translate(float, float);
  void rotate(float);
  void scale(float, float);
  
  // drawing functions
  void initRender();
  void drawRect(float, float, float, float);
  void drawImage(GLuint, float, float, float, float);
  void drawText(const char*, float, float, float, float, float);
  
  // temp
  void drawRectGeneral(float, float, float, float);
  void setColorGeneral(float, float, float, float);
  void drawImageGeneral(GLuint, float, float, float, float);
  GLuint whiteTex;
};

#endif // CONTEXT_H
