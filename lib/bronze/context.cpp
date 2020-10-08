#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "utils.h"
#include "assets/fileasset.h"
#include "assets/image.h"
#include <freetype-gl.h>

GLuint buildShaderContext(GLenum type, const GLchar* shaderSrc) {
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

GLuint buildProgramContext(const char* vertSrc, const char* fragSrc) {
  // create shaders
  GLuint vShader = buildShaderContext(GL_VERTEX_SHADER, vertSrc);
  GLuint fShader = buildShaderContext(GL_FRAGMENT_SHADER, fragSrc);
    
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

void OpenGLContext::init() {
  // general program shaders
  const char* genVert = R"(
    attribute vec2 aPosition;
    uniform mat4 uProj;
    uniform mat4 uCam;
  
    attribute vec4 aColor;
    varying vec4 vColor;
  
    attribute vec2 aTexCoords;
    varying vec2 vTexCoords;
  
    void main() {
      vColor = aColor;
      vTexCoords = aTexCoords;
      gl_Position = vec4(aPosition, 0.0, 1.0) * uProj * uCam;
    }
  )";
  const char* genFrag = R"(
    precision mediump float;

    uniform sampler2D uTextureUnit;
    varying vec2 vTexCoords;
    varying vec4 vColor;
    uniform vec4 uColorTint;

    void main() {
      gl_FragColor = texture2D(uTextureUnit, vTexCoords) * vColor + uColorTint;
      //texture2D(uTextureUnit, vTexCoords) * vColor + uColorTint
    }
  )";
  
  // compiling general shader
  this->generalProgram = buildProgramContext(genVert, genFrag);
  this->g_posLoc = glGetAttribLocation(this->generalProgram, "aPosition");
  this->g_uProjLoc = glGetUniformLocation(this->generalProgram, "uProj");
  this->g_uCamLoc = glGetUniformLocation(this->generalProgram, "uCam");
  this->g_texLoc = glGetUniformLocation(this->generalProgram, "uTextureUnit");
  this->g_texCoord = glGetAttribLocation(this->generalProgram, "aTexCoords");
  this->g_color = glGetAttribLocation(this->generalProgram, "aColor");
  this->g_uColorTint = glGetUniformLocation(this->generalProgram, "uColorTint");

  // enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // white texture for general program
  uint32_t whiteTexData = 0xffffffff;
  this->whiteTex = loadTexture(1, 1, GL_RGBA, &whiteTexData);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->whiteTex);

  // setup initial matrix values
  GLfloat initCamMat[] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};
  glUseProgram(this->generalProgram);
  glUniformMatrix4fv(this->g_uCamLoc, 1, GL_FALSE, initCamMat);
}

void OpenGLContext::setViewport(float x, float y, float w, float h) {
  glViewport(x, y, w, h);
}

void OpenGLContext::setProjection(float left, float right, float top, float bot) {
  GLfloat projMat[] = {
          static_cast<GLfloat>(2.0 / (right - left)), 0, 0, -(right + left) / (right - left),
          0, static_cast<GLfloat>(2.0 / (top - bot)), 0, -(top + bot) / (top - bot),
          0, 0, -2.0 / (1.0 + 1.0), -(1.0 - 1.0) / (1.0 + 1.0),
          0, 0, 0, 1
  };
  memcpy(this->projection, projMat, 16 * sizeof(float));
  
  glUniformMatrix4fv(this->g_uProjLoc, 1, GL_FALSE, this->projection);
}

void OpenGLContext::translate(float x, float y) {
  this->projection[3]  += this->projection[0] * x + this->projection[1] * y;
  this->projection[7]  += this->projection[5] * y + this->projection[4] * x;
  this->projection[11] += this->projection[8] * x + this->projection[9] * y;
  this->projection[15] += this->projection[12]* x + this->projection[13]* y;
  
  glUniformMatrix4fv(this->g_uProjLoc, 1, GL_FALSE, this->projection);
}

void OpenGLContext::rotate(float rad) {
  // values we need for later
  // rad should be negative to rotate clockwise, but it seems to work like this
  float cr = cos(rad);
  float sr = sin(rad);
  float temp[16];
  memcpy(temp, this->projection, 16*sizeof(float));
  
  this->projection[0] = temp[0] * cr + temp[1] * sr;
  this->projection[1] = temp[0] * -sr + temp[1] * cr;
  this->projection[4] = temp[4] * cr + temp[5] * sr;
  this->projection[5] = temp[4] * -sr + temp[5] * cr;
  this->projection[8] = temp[8] * cr + temp[9] * sr;
  this->projection[9] = temp[8] * -sr + temp[9] * cr;
  this->projection[12] = temp[12] * cr + temp[13] * sr;
  this->projection[13] = temp[12] * -sr + temp[13] * cr;
  
  glUniformMatrix4fv(this->g_uProjLoc, 1, GL_FALSE, this->projection);
}
void OpenGLContext::scale(float sx, float sy) {
  this->projection[0] *= sx;
  this->projection[1] *= sy;
  this->projection[4] *= sx;
  this->projection[5] *= sy;
  this->projection[8] *= sx;
  this->projection[9] *= sy;
  this->projection[12] *= sx;
  this->projection[13] *= sy;
  
  glUniformMatrix4fv(this->g_uProjLoc, 1, GL_FALSE, this->projection);
}

void OpenGLContext::setFont(const char* filename) {
  this->fontFile = getAsset(filename);//NOTE: THIS ISN'T FREED BC WE NEED TO ADD TO TEX ATLAS IN REALTIME!!
  this->texAtlas = ftgl::texture_atlas_new(512, 512, 1);
  this->texFont = ftgl::texture_font_new_from_memory(this->texAtlas, 32, this->fontFile.data, this->fontFile.size);
}

void OpenGLContext::renderBegin() {
  glClearColor(this->backRed, this->backGreen, this->backBlue, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLContext::setBackground(float r, float g, float b) {
  this->backRed = r;
  this->backGreen = g;
  this->backBlue = b;
}

void OpenGLContext::setColor(float r, float g, float b, float a) {
  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

void OpenGLContext::setColor(float r, float g, float b) {
  this->red = r;
  this->green = g;
  this->blue = b;
}

void OpenGLContext::drawRect(float x, float y, float width, float height) {
  GLfloat verticies[] = {x,y, x,y+height, x+width,y, x+width,y+height};
  GLfloat colors[] = {
    this->red,this->green,this->blue,this->alpha,
    this->red,this->green,this->blue,this->alpha,
    this->red,this->green,this->blue,this->alpha,
    this->red,this->green,this->blue,this->alpha
  };
  GLfloat uvCoords[] = {0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f};
  
  glUseProgram(this->generalProgram);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->whiteTex);
  glUniform1i(this->g_texLoc, 0);
  
  glVertexAttribPointer(this->g_posLoc, 2, GL_FLOAT, GL_FALSE, 0, verticies);
  glVertexAttribPointer(this->g_color, 4, GL_FLOAT, GL_FALSE, 0, colors);
  glVertexAttribPointer(this->g_texCoord, 2, GL_FLOAT, GL_FALSE, 0, uvCoords);
  glEnableVertexAttribArray(this->g_posLoc);
  glEnableVertexAttribArray(this->g_color);
  glEnableVertexAttribArray(this->g_texCoord);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(this->g_posLoc);
  glDisableVertexAttribArray(this->g_color);
  glDisableVertexAttribArray(this->g_texCoord);
}

void OpenGLContext::drawImage(GLuint tex, float x, float y, float width, float height) {
  GLfloat positions[] = {x,y, x,y+height, x+width,y, x+width,y+height};
  GLfloat colors[] = {
    1.0,1.0,1.0,1.0,
    1.0,1.0,1.0,1.0,
    1.0,1.0,1.0,1.0,
    1.0,1.0,1.0,1.0
  };
  GLfloat uvCoords[] = {0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f};
  
  glUseProgram(this->generalProgram);
  glActiveTexture(GL_TEXTURE1);
  
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(this->g_texLoc, 1);
  glVertexAttribPointer(this->g_posLoc, 2, GL_FLOAT, GL_FALSE, 0, positions);
  glVertexAttribPointer(this->g_color, 4, GL_FLOAT, GL_FALSE, 0, colors);
  glVertexAttribPointer(this->g_texCoord, 2, GL_FLOAT, GL_FALSE, 0, uvCoords);
  glEnableVertexAttribArray(this->g_posLoc);
  glEnableVertexAttribArray(this->g_color);
  glEnableVertexAttribArray(this->g_texCoord);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(this->g_posLoc);
  glDisableVertexAttribArray(this->g_color);
  glDisableVertexAttribArray(this->g_texCoord);
}

void OpenGLContext::drawText(const char* text, float x, float y) {
  GLfloat verticies[48*strlen(text)]; // 48 floats per char
  float currx = 0.0; // text goes to the right

  // add in each character to verticies
  bool reload = false;
  for (int i = 0; i < strlen(text); i++) {
    // if glyph isn't already loaded, we need to update our font texture atlas
    if (!texture_font_find_glyph(this->texFont, (const char*)&text[i])) {
      reload = true;
    }

    // get glyph
    const char test[2] = {text[i]};
    ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(this->texFont, test);

    // set up varticies
    float x0 = currx + x + glyph->offset_x;
    float y0 = y - glyph->offset_y;
    float x1 = x0 + glyph->width;
    float y1 = y0 + glyph->height;
    float u0 = glyph->s0;
    float v0 = glyph->t0;
    float u1 = glyph->s1;
    float v1 = glyph->t1;

    verticies[i*48 + 0] = x0;
    verticies[i*48 + 1] = y0;
    verticies[i*48 + 2] = u0;
    verticies[i*48 + 3] = v0;
    verticies[i*48 + 4] = 0.0;
    verticies[i*48 + 5] = 0.0;
    verticies[i*48 + 6] = 0.0;
    verticies[i*48 + 7] = this->alpha;

    verticies[i*48 + 8] = x0;
    verticies[i*48 + 9] = y1;
    verticies[i*48 + 10] = u0;
    verticies[i*48 + 11] = v1;
    verticies[i*48 + 12] = 0.0;
    verticies[i*48 + 13] = 0.0;
    verticies[i*48 + 14] = 0.0;
    verticies[i*48 + 15] = this->alpha;

    verticies[i*48 + 16] = x1;
    verticies[i*48 + 17] = y0;
    verticies[i*48 + 18] = u1;
    verticies[i*48 + 19] = v0;
    verticies[i*48 + 20] = 0.0;
    verticies[i*48 + 21] = 0.0;
    verticies[i*48 + 22] = 0.0;
    verticies[i*48 + 23] = this->alpha;

    verticies[i*48 + 24] = x1;
    verticies[i*48 + 25] = y1;
    verticies[i*48 + 26] = u1;
    verticies[i*48 + 27] = v1;
    verticies[i*48 + 28] = 0.0;
    verticies[i*48 + 29] = 0.0;
    verticies[i*48 + 30] = 0.0;
    verticies[i*48 + 31] = this->alpha;

    verticies[i*48 + 32] = x0;
    verticies[i*48 + 33] = y1;
    verticies[i*48 + 34] = u0;
    verticies[i*48 + 35] = v1;
    verticies[i*48 + 36] = 0.0;
    verticies[i*48 + 37] = 0.0;
    verticies[i*48 + 38] = 0.0;
    verticies[i*48 + 39] = this->alpha;

    verticies[i*48 + 40] = x1;
    verticies[i*48 + 41] = y0;
    verticies[i*48 + 42] = u1;
    verticies[i*48 + 43] = v0;
    verticies[i*48 + 44] = 0.0;
    verticies[i*48 + 45] = 0.0;
    verticies[i*48 + 46] = 0.0;
    verticies[i*48 + 47] = this->alpha;

    currx += glyph->advance_x;
  }

  if (reload) {
    this->texAtlas->id = loadTexture(this->texAtlas->width, this->texAtlas->height, GL_ALPHA, this->texAtlas->data);
  }

  // render verticies
  GLuint bufferLoc;
  glGenBuffers(1, &bufferLoc);
  glBindBuffer(GL_ARRAY_BUFFER, bufferLoc);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glUseProgram(this->generalProgram);
  glUniform4f(this->g_uColorTint, this->red, this->green, this->blue, 0.0); // color
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texAtlas->id);
  glUniform1i(this->g_texLoc, 0);

  glBindBuffer(GL_ARRAY_BUFFER, bufferLoc);
  glEnableVertexAttribArray(this->g_posLoc);
  glEnableVertexAttribArray(this->g_texCoord);
  glEnableVertexAttribArray(this->g_color);
  glVertexAttribPointer(this->g_posLoc, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
  glVertexAttribPointer(this->g_texCoord, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void*)8);
  glVertexAttribPointer(this->g_color, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void*)16);

  glDrawArrays(GL_TRIANGLES, 0, strlen(text)*6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDisableVertexAttribArray(this->g_posLoc);
  glDisableVertexAttribArray(this->g_texCoord);
  glDisableVertexAttribArray(this->g_color);
  glUniform4f(this->g_uColorTint, 0.0, 0.0, 0.0, 0.0); // reset color tint
}
