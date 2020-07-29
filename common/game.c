#include "game.h"

float topX = 0.5;
GLuint programObject;
double width;
double height;
float green = 0.1;
GLint locAttrib;

GLuint loadShader(GLenum type, const char* shaderSrc) {
  GLuint shader;
  GLint compiled;

  // Create the shader object
  shader = glCreateShader(type);
  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, NULL);
  // Compile the shader
  glCompileShader(shader);
  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if(!compiled) {
    green = 1.0;
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

void glSetup(double width, double height) {
  // load our shaders
  GLbyte vShaderStr[] =
  "attribute vec4 vPosition; \n"
  "void main() \n"
  "{ \n"
  " gl_Position = vPosition; \n"
  "} \n";

  GLbyte fShaderStr[] =
  "precision mediump float; \n"
  "void main() \n"
  "{ \n"
  " gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); \n"
  "} \n";

  GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
  GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);
  programObject = glCreateProgram();

  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);
  glBindAttribLocation(programObject, 0, "vPosition");
  glLinkProgram(programObject);
  // locAttrib = glGetAttribLocation(programObject, "vPosition");

  GLint linked;
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  if (!linked) {
    green = 1.0;
    glDeleteProgram(programObject);
  }

  // setup misc things
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  width = width;
  height = height;
  glViewport(0,0,width,height);
}

void glRender() {
  glClearColor(0.2, green, 0.9, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat verticies[] = {0.0f,0.5f, -0.5f,-0.5f, 0.5f,-0.5f};
  glUseProgram(programObject);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, verticies);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  topX += 0.001;
}