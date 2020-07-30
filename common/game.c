#include "game.h"

float topX = 0;
float topY = -100;
GLuint programObject;
double screenWidth;
double screenHeight;
float green = 0.1;
GLint uProjLoc;
GLint uCamLoc;

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
          "attribute vec2 vPosition; \n"
          "uniform mat4 uProj; \n"
          "uniform mat4 uCam; \n"
          "void main() \n"
          "{ \n"
          " gl_Position = uProj * uCam * vec4(vPosition, 0.0, 1.0); \n"
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
  //get locations
  uProjLoc = glGetUniformLocation(programObject, "uProj");
  uCamLoc = glGetUniformLocation(programObject, "uCam");

  GLint linked;
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  if (!linked) {
    green = 1.0;
    glDeleteProgram(programObject);
  }

  // setup initial matrix values
  handleResize(width, height);
  glUniformMatrix4fv(uCamLoc, 1, GL_FALSE,
          (GLfloat[]){1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1});
}

void glRender() {
  glClearColor(0.2, green, 0.9, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat verticies[] = {topX,topY, -100,100, 100,100};
  glUseProgram(programObject);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, verticies);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
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
  glUseProgram(programObject);
  float right = screenWidth / 2;
  float left = -screenWidth / 2;
  float top = -screenHeight / 2;
  float bot = screenHeight / 2;
  glUniformMatrix4fv(uProjLoc, 1, GL_FALSE,(GLfloat[]){
    2.0 / (right - left), 0, 0, -(right + left) / (right - left),
    0, 2.0 / (top - bot), 0, -(top + bot) / (top - bot),
    0, 0, -2.0 / (1.0 + 1.0), -(1.0 - 1.0) / (1.0 + 1.0),
    0, 0, 0, 1
  });
}