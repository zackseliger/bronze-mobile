#ifndef CONTEXT_H
#define CONTEXT_H

#include "opengl.h"
#include "assets/fileasset.h"
#include "texture.h"
#include <freetype-gl.h>

class Context {
public:
  
  // init stuff that's needed outside of the constructor
  virtual void init() = 0;
  
  // fonts
  virtual void setFont(const char*) = 0;
  
  // projection/camera stuff
  virtual void setViewport(float, float, float, float) = 0;
  virtual void setProjection(float, float, float, float) = 0;
  virtual void translate(float, float) = 0;
  virtual void rotate(float) = 0;
  virtual void scale(float, float) = 0;
  
  // drawing functions
  virtual void renderBegin() = 0;
  virtual void setBackground(float, float, float) = 0;
  virtual void setColor(float, float, float, float) = 0;
  virtual void setColor(float, float, float) = 0;
  virtual void drawRect(float, float, float, float) = 0;
  virtual void drawImage(Texture*, float, float, float, float) = 0;
  virtual void drawText(const char*, float, float) = 0;
};

class OpenGLContext : public Context {
public:
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
  GLuint whiteTex;
  float backRed = 0;
  float backGreen = 0;
  float backBlue = 0;
  
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
  void renderBegin();
  void setBackground(float, float, float);
  void setColor(float, float, float, float);
  void setColor(float, float, float);
  void drawRect(float, float, float, float);
  void drawImage(Texture*, float, float, float, float);
  void drawText(const char*, float, float);
};

#endif // CONTEXT_H
