#include "game.h"
#include "image.h"
#include <fstream>
#include <string>
#include <png.h>
#include <vector>

#define BUFFER_OFFSET(i) ((void*)(i))

#ifdef __ANDROID__
#include <android/log.h>
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,"io.deemo",__VA_ARGS__)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#elif __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

// stuff that should be in the engine
double screenWidth;
double screenHeight;
// programs
GLuint colorProgram;
GLint c_uProjLoc;
GLint c_uCamLoc;
GLint c_posLoc;

GLuint textureProgram;
GLint p_uProjLoc;
GLint p_uCamLoc;
GLint p_posLoc;
GLint p_texLoc;
GLint p_texCoord;
GLuint p_buffer;

// stuff that's just there
float topX = 0;
float topY = -100;
GLint textureId;
static const float rect[] = {-200.0f,-200.0f, 0.0f, 0.0f,
                             -200.0f, 000.0f, 0.0f, 1.0f,
                             000.0f, -200.0f, 1.0f, 0.0f,
                             000.0f,  000.0f, 1.0f, 1.0f};

// utilities
void LOG(const char* message) {
  #ifdef __ANDROID__
  ALOG("%s", message);
  #elif __APPLE__
  printf("%s", message);
  #endif
}
#ifdef __ANDROID__
AAssetManager* assetManager;
FileData getAsset(const char* filename) {
  AAsset* asset = AAssetManager_open(assetManager, filename, AASSET_MODE_STREAMING);
  assert(asset != nullptr);

//  FILE* stream = fopen(path, "r");
//  assert (stream != NULL);
//
//  fseek(stream, 0, SEEK_END);
//  long stream_size = ftell(stream);
//  fseek(stream, 0, SEEK_SET);
//
//  void* buffer = malloc(stream_size);
//  fread(buffer, stream_size, 1, stream); // TODO: put this in

  return (FileData){AAsset_getLength(asset), AAsset_getBuffer(asset), asset};
}
void freeAsset(FileData data) {
    AAsset_close((AAsset*)data.handle);
}
void initAssetManager(AAssetManager* manager) {
  assetManager = manager;
}
// Apple implementation is in AssetsUtils.mm
#endif

GLuint buildShader(GLenum type, const char* shaderSrc) {
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
    LOG("COULDN'T COMPILE SHADER:");
    LOG(errorLog);
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
  // free assets
  freeAsset(vShaderFile);
  freeAsset(fShaderFile);

  // create and return program
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vShader);
  glAttachShader(programId, fShader);
  glLinkProgram(programId);

  // make sure it worked out
  GLint isLinked = 0;
  glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    const char* errorLog = (const char*)malloc(maxLength);
    glGetProgramInfoLog(programId, maxLength, &maxLength, (GLchar*)errorLog);
    LOG(errorLog);
    LOG("COULDN'T LINK PROGRAM:");
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
  // color program
  colorProgram = buildProgram("shaders/color.vsh", "shaders/color.fsh");
  c_posLoc = glGetAttribLocation(colorProgram, "vPosition");
  c_uProjLoc = glGetUniformLocation(colorProgram, "uProj");
  c_uCamLoc = glGetUniformLocation(colorProgram, "uCam");

  // texture program
  textureProgram = buildProgram("shaders/texture.vsh", "shaders/texture.fsh");
  p_posLoc = glGetAttribLocation(textureProgram, "vPosition");
  p_uProjLoc = glGetUniformLocation(textureProgram, "uProj");
  p_uCamLoc = glGetUniformLocation(textureProgram, "uCam");
  p_texLoc = glGetUniformLocation(textureProgram, "uTextureUnit");
  p_texCoord = glGetAttribLocation(textureProgram, "aTextureCoordinates");

  // enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // setup initial matrix values
  handleResize(width, height);
  GLfloat initCamMat[] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};
  glUseProgram(textureProgram);
  glUniformMatrix4fv(p_uCamLoc, 1, GL_FALSE, initCamMat);
  glUseProgram(colorProgram);
  glUniformMatrix4fv(c_uCamLoc, 1, GL_FALSE, initCamMat);

  // setup image
  RawImageData imageData = getImage((const char*)"image/pngtest.png");
  textureId = loadTexture(imageData.width, imageData.height, imageData.gl_color_format, imageData.data);
  releaseImage(&imageData);
  p_buffer = createVbo(sizeof(rect), rect, GL_STATIC_DRAW);
}

void glRender() {
  glClearColor(0.2, 0.1, 0.9, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw texture
  glUseProgram(textureProgram);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glUniform1i(p_texLoc, 0);
  glBindBuffer(GL_ARRAY_BUFFER, p_buffer);
  glVertexAttribPointer(p_posLoc, 2, GL_FLOAT, GL_FALSE,4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
  glVertexAttribPointer(p_texCoord, 2, GL_FLOAT, GL_FALSE,4 * sizeof(GL_FLOAT), BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(p_posLoc);
  glEnableVertexAttribArray(p_texCoord);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(p_posLoc);
  glDisableVertexAttribArray(p_texCoord);


  // draw triangle
  GLfloat verticies[] = {topX,topY, -100,100, 100,100, 200,200,};
  glUseProgram(colorProgram);
  glVertexAttribPointer(c_posLoc, 2, GL_FLOAT, GL_FALSE, 0, verticies);
  glEnableVertexAttribArray(c_posLoc);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(c_posLoc);
}

// input events
void startTouch(int id, float x, float y) {
  topX = x - screenWidth/2;
  topY = y - screenHeight/2;
}
void moveTouch(int id, float x, float y) {
  topX = x - screenWidth/2;
  topY = y - screenHeight/2;
}
void endTouch(int id, float x, float y) {
  topX = x - screenWidth/2;
  topY = y - screenHeight/2;
}

// misc events
void handleResize(double width, double height) {
  screenWidth = width;
  screenHeight = height;
  glViewport(0, 0, screenWidth, screenHeight);

  // redo our projection matrix
  float right = screenWidth / 2;
  float left = -screenWidth / 2;
  float top = -screenHeight / 2;
  float bot = screenHeight / 2;
  GLfloat projMat[] = {
          static_cast<GLfloat>(2.0 / (right - left)), 0, 0, -(right + left) / (right - left),
          0, static_cast<GLfloat>(2.0 / (top - bot)), 0, -(top + bot) / (top - bot),
          0, 0, -2.0 / (1.0 + 1.0), -(1.0 - 1.0) / (1.0 + 1.0),
          0, 0, 0, 1
  };
  glUseProgram(textureProgram);
  glUniformMatrix4fv(p_uProjLoc, 1, GL_FALSE, projMat);
  glUseProgram(colorProgram);
  glUniformMatrix4fv(c_uProjLoc, 1, GL_FALSE, projMat);
}
