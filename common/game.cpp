#include "game.h"
#include <png.h>
#include "utils.h"
#include <assert.h>

//assets
#include "assets/fileasset.h"
#include "assets/image.h"
#include "assets/sound.h"

//audio
#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>

//graphics
#include "opengl.h"
#include "context.h"

// stuff that should be in the engine
double screenWidth;
double screenHeight;
double game_width = 500;
double game_height = 500;
float xScale, yScale = 1;
// graphics context
OpenGLContext context;

// stuff that's just there
float topX = 0;
float topY = -100;

GLuint buildShader(GLenum type, const GLchar* shaderSrc) {
  GLuint shader;
  GLint compiled;

  // Create the shader object
  shader = glCreateShader(type);
  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, nullptr);
  // Compile the shader
  glCompileShader(shader);
  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  // check how our shader compiled
  if(compiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    const char* errorLog = (const char*)malloc(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, (GLchar*)errorLog);
    LOG("COULDN'T COMPILE SHADER: %s", errorLog);
    free((void*)errorLog);

    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

GLuint buildProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
  // load files
  FileData vShaderFile = getAsset(vertexShaderPath);
  FileData fShaderFile = getAsset(fragmentShaderPath);
    
  // create shaders
  GLuint vShader = buildShader(GL_VERTEX_SHADER, (const char*)vShaderFile.data);
  GLuint fShader = buildShader(GL_FRAGMENT_SHADER, (const char*)fShaderFile.data);
    
  // create and return program
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vShader);
  glAttachShader(programId, fShader);
  glLinkProgram(programId);
    
  // free assets
  freeAsset(vShaderFile);
  freeAsset(fShaderFile);

  // make sure it worked out
  GLint isLinked = 0;
  glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    const char* errorLog = (const char*)malloc(maxLength);
    glGetProgramInfoLog(programId, maxLength, &maxLength, (GLchar*)errorLog);
    LOG("COULDN'T LINK PROGRAM: %s", errorLog);
    free((void*)errorLog);

    // delete stuff and return 0
    glDeleteProgram(programId);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return 0;
  }

  return programId;
}

void glSetup(double width, double height) {
  context.init();
  
  // initial window resize
  handleResize(width, height);
  
  // set font
  context.setFont("Poetsen.ttf");

  // setup image
  loadImage("image/pngtest.png", "testImage");

  //audio
  ALCdevice *device = alcOpenDevice(NULL);
  if (!device) LOG("COULDN'T OPEN AUDIO DEVICE!!");
  ALCcontext *context = alcCreateContext(device, NULL);
  if (!context) LOG("COULDN'T OPEN AUDIO CONTEXT");
  if (alcMakeContextCurrent(context) == ALC_FALSE) LOG("CONTEXT COULDN'T BE MADE CURRENT");

  // setup audio
  ALuint buffer = loadSound("audio/wavtest.wav", "test");
  ALuint source;
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, (ALint)buffer);
  assert(alGetError()==AL_NO_ERROR && "Failed to open audio source");

  playSound("test");
}

void glRender() {
  glClearColor(0.2, 0.1, 0.9, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw texture
//  drawImage(texAtlas->id, -100, -200, 200, 200);
//
//  // draw triangle
//  GLfloat verticies[] = {topX,topY, -100,100, 100,100, 200,200};
//  GLfloat colors[] = {1.0,0.0,0.0, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.6,0.6,0.3};
//  glUseProgram(colorProgram);
//  glVertexAttribPointer(c_posLoc, 2, GL_FLOAT, GL_FALSE, 0, verticies);
//  glVertexAttribPointer(c_color, 3, GL_FLOAT, GL_FALSE, 0, colors);
//  glEnableVertexAttribArray(c_posLoc);
//  glEnableVertexAttribArray(c_color);
//  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//  glDisableVertexAttribArray(c_posLoc);
//  glDisableVertexAttribArray(c_color);
  
  // draw texture 2
  context.drawImage(getImage("testImage"), -100, 100, 200, 200);
  
  // draw text
  context.drawText("Hello world. I am here!", -150, -250, 0.8, 0.2, 0.2);
  context.drawRect(-150, -250, 10, 10);
  
  context.drawRect(topX-5, topY-5, 10, 10);
  
  context.drawRect(-10,-10,20,20);//20x20 block in the center
  context.translate(20,20);
  context.drawRect(-10,-10,20,20);
  context.translate(-20,-20);
}

// util
float getScreenHeight() {
  return (game_height/yScale) + (screenHeight/yScale - game_height);
}
float getScreenWidth() {
  return (game_width/xScale) + (screenWidth/xScale - game_width);
}

// input events
void startTouch(int id, float x, float y) {
  topX = (x - screenWidth/2) / xScale;
  topY = (y - screenHeight/2) / yScale;
}
void moveTouch(int id, float x, float y) {
  topX = (x - screenWidth/2) / xScale;
  topY = (y - screenHeight/2) / yScale;
}
void endTouch(int id, float x, float y) {
  topX = (x - screenWidth/2) / xScale;
  topY = (y - screenHeight/2) / yScale;
}

// misc events
void handleResize(double width, double height) {
  screenWidth = width;
  screenHeight = height;
    
  float ratio = screenWidth / screenHeight;
  if (ratio > game_width / game_height) {
    xScale = screenHeight / game_height;
    yScale = xScale;
  }
  else {
    xScale = screenWidth / game_width;
    yScale = xScale;
  }
  
  // redo our projection matrix
  float right = (game_width/2) + (screenWidth/xScale - game_width)/2;
  float left = (-game_width/2) + (-screenWidth/xScale + game_width)/2;
  float top = (-game_height/2) + (-screenHeight/yScale + game_height)/2;
  float bot = (game_height/2) + (screenHeight/yScale - game_height)/2;
  
  context.setViewport(0, 0, width, height);
  context.setProjection(left, right, top, bot);
}
