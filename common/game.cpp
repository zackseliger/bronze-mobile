#include "game.h"
#include <png.h>
#include "utils.h"
#include <assert.h>
#include "application.h"

//rand and strings
#include <stdlib.h>
#include <time.h>
#include <string>

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
OpenGLContext* context = new OpenGLContext();

// stuff that's just there
float topX = 0;
float topY = -100;

class TestApplication : public Application {
public:
  OpenGLContext context;
  
  TestApplication() {
    LOG("TestApplication Constructor");
  }
  
  void initContext() {
    this->context.init();
    
    // set font
    this->context.setFont("Poetsen.ttf");

    // setup image
    loadImage("image/pngtest.png", "testImage");

    //audio
    initAudio();

    // setup audio
    loadSound("audio/thud1.wav", "thud1");
    loadSound("audio/thud2.wav", "thud2");
    loadSound("audio/thud3.wav", "thud3");
    loadSound("audio/thud4.wav", "thud4");
    loadSound("audio/thud5.wav", "thud5");
    loadSound("audio/thud6.wav", "thud6");
    loadSound("audio/thud7.wav", "thud7");
    srand(time(NULL));
  }
  
  void render() {
    this->context.renderBegin();
    // draw textures
    this->context.drawImage(context.texAtlas->id, -100, -200, 200, 200);
    this->context.drawImage(getImage("testImage"), -100, 100, 200, 200);
    
    // draw text
    this->context.setColor(0.8,0.2,0.2,topX/200);
    this->context.drawText("Hello world. I am here!", -150, -250);
    
    this->context.setColor(0,0,0,1);
    this->context.drawRect(topX-5, topY-5, 10, 10);
    this->context.drawRect(-10,-10,20,20);//20x20 block in the center
    
    //testing out translating and rotating and stuff
    this->context.translate(20,20);
    this->context.rotate(topX/100);
      this->context.setColor(0.2,0.1,0.7,0.7);
      this->context.drawRect(-10,-10,20,20);
      this->context.setColor(0.5,0.9,0.1,1.0);
      this->context.drawRect(100,100,20,20);
    this->context.rotate(-topX/100);
    this->context.translate(-20,-20);
  }
  
  void handleResize(float width, float height) {
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
    
    this->context.setViewport(0, 0, width, height);
    this->context.setProjection(left, right, top, bot);
  }
};
TestApplication* test = new TestApplication();

// input events
void startTouch(int id, float x, float y) {
  topX = (x - screenWidth/2) / xScale;
  topY = (y - screenHeight/2) / yScale;
  
  // play a thud sound
  playSound(("thud" + std::to_string(rand() % 7 + 1)).c_str());
}
void moveTouch(int id, float x, float y) {
  topX = (x - screenWidth/2) / xScale;
  topY = (y - screenHeight/2) / yScale;
}
void endTouch(int id, float x, float y) {
  topX = (x - screenWidth/2) / xScale;
  topY = (y - screenHeight/2) / yScale;
}
