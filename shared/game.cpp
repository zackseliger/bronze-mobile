#include "game.h"
#include <png.h>
#include <utils.h>
#include <actor.h>
#include <collection.h>
#include <scene.h>
#include <cmath>

// rand and strings
#include <stdlib.h>
#include <time.h>
#include <string>

// assets
#include <assets/fileasset.h>
#include <assets/image.h>
#include <assets/sound.h>

// graphics
#include <opengl.h>
#include <application.h>
#include <context.h>

// follows our finger
float pointX = 0;
float pointY = -100;

class TestActor : public Actor {
public:
  float width;
  float height;
  float speed;
  
  TestActor(float x, float y) : super(x, y) {
    this->speed = 1.0;
    this->width = 50;
    this->height = 100;
  }
  
  void render() {
    this->context->setColor(0.9, 0.9, 0.9, 0.9);
    this->context->drawRect(-this->width/2, -this->height/2, this->width, this->height);
  }
  
  void update(float dt) {
    this->x += this->speed*dt;
    
    if (this->x > 250) this->speed = -1;
    else if (this->x < -250) this->speed = 1;
  }
};

class TestScene : public Scene {
public:
  // Actor stuff
  Collection* mainCollection;
  
  // oop
  Context* context;
  
  TestScene() {
    this->context = getCurrentApplication()->context;
    this->mainCollection = new Collection();
    this->mainCollection->add(new TestActor(-250, 0));
    this->mainCollection->add(new TestActor(-250, 200));
    this->mainCollection->add(new TestActor(-250, -200));
  }
  
  void update(float dt) {
    this->mainCollection->update(dt);
  }
  
  void render() {
    // draw textures
    this->context->drawImage(getImage("testImage"), -100, 100, 200, 200);
    
    // draw text
    this->context->setColor(0.8,0.2,0.2,std::abs(pointX/200));
    this->context->drawText("Hello world. I am here!", -150, -250);
    
    this->context->setColor(0.4,0.4,0.4,1);
    this->context->drawRect(pointX-5, pointY-5, 10, 10);
    this->context->drawRect(-10,-10,20,20);//20x20 block in the center
    
    //testing out translating and rotating and stuff
    this->context->translate(20,20);
    this->context->rotate(pointX/100);
      this->context->setColor(0.2,0.1,0.7,0.7);
      this->context->drawRect(-10,-10,20,20);
      this->context->setColor(0.5,0.9,0.1,1.0);
      this->context->drawRect(100,100,20,20);
    this->context->rotate(-pointX/100);
    this->context->translate(-20,-20);
    
    this->mainCollection->draw();
  }
  
  void onLoad() {
    
  }
  
  void onUnload() {
    
  }
};

class TestApplication : public Application {
public:
  TestApplication() : super(500, 500, new OpenGLContext()) {}
  
  void init() {
    super::init();
    
    // set font
    this->context->setFont("Poetsen.ttf");
    
    // setup image
    loadImage("image/pngtest.png", "testImage");

    // load sounds
    loadSound("audio/thud1.wav", "thud1");
    loadSound("audio/thud2.wav", "thud2");
    loadSound("audio/thud3.wav", "thud3");
    loadSound("audio/thud4.wav", "thud4");
    loadSound("audio/thud5.wav", "thud5");
    loadSound("audio/thud6.wav", "thud6");
    loadSound("audio/thud7.wav", "thud7");
    srand(time(NULL));
    
    // create scenes and switch to initial one
    this->sceneManager->addScene("test", new TestScene());
    this->sceneManager->change("test");
    
    // reset timestep so deltaTime isn't massive on first frame
    this->timestep->resetTime();
  }
  
  // touch events
  void handleTouchStart(int id, float x, float y) {
    pointX = (x - screenWidth/2) / xScale;
    pointY = (y - screenHeight/2) / yScale;
    
    // play a thud sound
    playSound(("thud" + std::to_string(rand() % 7 + 1)).c_str());
  }
  void handleTouchMove(int id, float x, float y) {
    pointX = (x - screenWidth/2) / xScale;
    pointY = (y - screenHeight/2) / yScale;
  }
  void handleTouchEnd(int id, float x, float y) {
    pointX = (x - screenWidth/2) / xScale;
    pointY = (y - screenHeight/2) / yScale;
  }
};

Application* currentApplication = new TestApplication();
