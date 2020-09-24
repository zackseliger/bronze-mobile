#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
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
  // load files
//  FileData vShaderFile = getAsset(vertexShaderPath);
//  FileData fShaderFile = getAsset(fragmentShaderPath);
    
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
  // color program shaders
  const char* colorVert = R"(
      attribute vec2 vPosition;
      uniform mat4 uProj;
      uniform mat4 uCam;

      attribute vec3 aColor;
      varying vec3 vColor;

      void main() {
        vColor = aColor;
        gl_Position = vec4(vPosition, 0.0, 1.0) * uProj * uCam;
      }
  )";
  const char* colorFrag = R"(
      precision mediump float;

      varying vec3 vColor;

      void main() {
        gl_FragColor = vec4(vColor, 1.0);
      }
  )";

  // compiling color shader
  this->colorProgram = buildProgramContext(colorVert, colorFrag);
  this->c_posLoc = glGetAttribLocation(this->colorProgram, "vPosition");
  this->c_uProjLoc = glGetUniformLocation(this->colorProgram, "uProj");
  this->c_uCamLoc = glGetUniformLocation(this->colorProgram, "uCam");
  this->c_color = glGetAttribLocation(this->colorProgram, "aColor");

  // texture program shaders
  const char* texVert = R"(
      attribute vec2 vPosition;
      uniform mat4 uProj;
      uniform mat4 uCam;

      attribute vec2 aTextureCoordinates;
      varying vec2 vTextureCoordinates;

      void main() {
        vTextureCoordinates = aTextureCoordinates;
        gl_Position = vec4(vPosition, 0.0, 1.0) * uProj * uCam;
      }
  )";
  const char* texFrag = R"(
      precision mediump float;

      uniform sampler2D uTextureUnit;
      varying vec2 vTextureCoordinates;
      uniform vec4 uColor;

      void main() {
        gl_FragColor = texture2D(uTextureUnit, vTextureCoordinates) + uColor;
      }
  )";

  // compiling texture shader
  this->textureProgram = buildProgramContext(texVert, texFrag);
  this->p_posLoc = glGetAttribLocation(this->textureProgram, "vPosition");
  this->p_uProjLoc = glGetUniformLocation(this->textureProgram, "uProj");
  this->p_uCamLoc = glGetUniformLocation(this->textureProgram, "uCam");
  this->p_texLoc = glGetUniformLocation(this->textureProgram, "uTextureUnit");
  this->p_texCoord = glGetAttribLocation(this->textureProgram, "aTextureCoordinates");
  this->p_color = glGetUniformLocation(this->textureProgram, "uColor");

  // enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // setup initial matrix values
  GLfloat initCamMat[] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};
  glUseProgram(this->colorProgram);
  glUniformMatrix4fv(this->c_uCamLoc, 1, GL_FALSE, initCamMat);
  glUseProgram(this->textureProgram);
  glUniformMatrix4fv(this->p_uCamLoc, 1, GL_FALSE, initCamMat);
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
  
  glUseProgram(this->colorProgram);
  glUniformMatrix4fv(this->c_uProjLoc, 1, GL_FALSE, this->projection);
  glUseProgram(this->textureProgram);
  glUniformMatrix4fv(this->p_uProjLoc, 1, GL_FALSE, this->projection);
}

void OpenGLContext::translate(float x, float y) {
  this->projection[3] += this->projection[0] * x;
  this->projection[7] += this->projection[5] * y;
  
  glUseProgram(this->colorProgram);
  glUniformMatrix4fv(this->c_uProjLoc, 1, GL_FALSE, this->projection);
  glUseProgram(this->textureProgram);
  glUniformMatrix4fv(this->p_uProjLoc, 1, GL_FALSE, this->projection);
}

void OpenGLContext::setFont(const char* filename) {
  if (this->fontFile.data != nullptr) {
    freeAsset(this->fontFile);
  }
  
  this->fontFile = getAsset(filename);//NOTE: THIS ISN'T FREED BC WE NEED TO ADD TO TEX ATLAS IN REALTIME!!
  this->texAtlas = ftgl::texture_atlas_new(512, 512, 1);
  this->texFont = ftgl::texture_font_new_from_memory(this->texAtlas, 32, this->fontFile.data, this->fontFile.size);
}

void OpenGLContext::drawRect(float x, float y, float width, float height) {
  GLfloat verticies[] = {x,y, x,y+height, x+width,y, x+width,y+height};
  GLfloat colors[] = {0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0};
  
  glUseProgram(this->colorProgram);
  glVertexAttribPointer(this->c_posLoc, 2, GL_FLOAT, GL_FALSE, 0, verticies);
  glVertexAttribPointer(this->c_color, 3, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(this->c_posLoc);
  glEnableVertexAttribArray(this->c_color);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(this->c_posLoc);
  glDisableVertexAttribArray(this->c_color);
}

void OpenGLContext::drawImage(GLuint tex, float x, float y, float width, float height) {
  GLfloat positions[] = {x,y, x,y+height, x+width,y, x+width,y+height};
  GLfloat uvCoords[] = {0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f};

  glUseProgram(this->textureProgram);
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(this->p_texLoc, 0);
  glVertexAttribPointer(this->p_posLoc, 2, GL_FLOAT, GL_FALSE, 0, positions);
  glVertexAttribPointer(this->p_texCoord, 2, GL_FLOAT, GL_FALSE, 0, uvCoords);
  glEnableVertexAttribArray(this->p_posLoc);
  glEnableVertexAttribArray(this->p_texCoord);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(this->p_posLoc);
  glDisableVertexAttribArray(this->p_texCoord);
}

void OpenGLContext::drawText(const char* text, float x, float y, float r, float g, float b) {
  GLfloat verticies[24*strlen(text)]; // 24 floats per char
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

    verticies[i*24 + 0] = x0;
    verticies[i*24 + 1] = y0;
    verticies[i*24 + 2] = u0;
    verticies[i*24 + 3] = v0;

    verticies[i*24 + 4] = x0;
    verticies[i*24 + 5] = y1;
    verticies[i*24 + 6] = u0;
    verticies[i*24 + 7] = v1;

    verticies[i*24 + 8] = x1;
    verticies[i*24 + 9] = y0;
    verticies[i*24 + 10] = u1;
    verticies[i*24 + 11] = v0;

    verticies[i*24 + 12] = x1;
    verticies[i*24 + 13] = y1;
    verticies[i*24 + 14] = u1;
    verticies[i*24 + 15] = v1;

    verticies[i*24 + 16] = x0;
    verticies[i*24 + 17] = y1;
    verticies[i*24 + 18] = u0;
    verticies[i*24 + 19] = v1;

    verticies[i*24 + 20] = x1;
    verticies[i*24 + 21] = y0;
    verticies[i*24 + 22] = u1;
    verticies[i*24 + 23] = v0;

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

  glUseProgram(this->textureProgram);
  glUniform4f(this->p_color, r, g, b, 0.0); // color
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texAtlas->id);
  glUniform1i(this->p_texLoc, 0);

  glBindBuffer(GL_ARRAY_BUFFER, bufferLoc);
  glEnableVertexAttribArray(this->p_posLoc);
  glEnableVertexAttribArray(this->p_texCoord);
  glVertexAttribPointer(this->p_posLoc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glVertexAttribPointer(this->p_texCoord, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (const void*)8);

  glDrawArrays(GL_TRIANGLES, 0, strlen(text)*6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDisableVertexAttribArray(this->p_posLoc);
  glDisableVertexAttribArray(this->p_texCoord);
  glUniform4f(this->p_color, 0.0, 0.0, 0.0, 0.0);// reset color
}
