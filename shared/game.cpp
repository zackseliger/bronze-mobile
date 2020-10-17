#include "game.h"
#include <png.h>
#include <utils.h>
#include <actor.h>
#include <collection.h>
#include <scene.h>
#include <cmath>
#include <event.h>

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

class Player : public Actor {
public:
  float width;
  float height;
  float maxSpeed;
  float speed;
  uint32_t image;
  
  Player(float x, float y) : super(x, y) {
    this->maxSpeed = 5.0;
    this->speed = 0.0;
    this->width = 100;
    this->height = 100;
    this->image = getImage("playerIdle");
  }
  
  void render() {
    this->context->drawImage(this->image, -this->width/2, -this->height/2, this->width, this->height);
  }
  
  void update(float dt) {
    
  }
};

class TestScene : public Scene {
public:
  // Actor stuff
  Collection* mainCollection;
  
  float pointX = 0;
  float pointY = -100;
  
  // stuff I'm gonna take from the application
  Application* app;
  
  TestScene() {
    this->app = getCurrentApplication();
    this->mainCollection = new Collection();
    this->mainCollection->add(new Player(0, 0));
  }
  
  void update(float dt) {
    this->mainCollection->update(dt);
  }
  
  void render() {
    this->mainCollection->draw();
  }
  
  void onLoad() {
    setEventListener(TouchStart, new EventListener([this](Event* e) {
      TouchEvent* evt = static_cast<TouchEvent*>(e);
      
      this->pointX = (evt->x - this->app->screen->width/2) / this->app->xScale;
      this->pointY = (evt->y - this->app->screen->height/2) / this->app->yScale;
    }));
    setEventListener(TouchMove, new EventListener([this](Event* e) {
      TouchEvent* evt = static_cast<TouchEvent*>(e);
      
      this->pointX = (evt->x - this->app->screen->width/2) / this->app->xScale;
      this->pointY = (evt->y - this->app->screen->height/2) / this->app->yScale;
    }));
    setEventListener(TouchEnd, new EventListener([this](Event* e) {
      TouchEvent* evt = static_cast<TouchEvent*>(e);
      
      this->pointX = (evt->x - this->app->screen->width/2) / this->app->xScale;
      this->pointY = (evt->y - this->app->screen->height/2) / this->app->yScale;
    }));
  }
  
  void onUnload() {
    
  }
};

class TestApplication : public Application {
public:
  TestApplication() : super(500, 500, new OpenGLContext()) {}
  
  void init() {
    super::init();
    
    setShouldAlias(false); // to keep hard edges on textures
    
    // background color
    this->context->setBackground(0.13, 0.12, 0.2);
    
    // set font
    this->context->setFont("Poetsen.ttf");
    
    // setup image
    loadImage("image/player/idle.png", "playerIdle");
    loadImage("image/player/walk1.png", "playerWalk1");
    loadImage("image/player/walk2.png", "playerWalk2");

    // load sounds
    loadSound("audio/thud1.wav", "thud1");
    loadSound("audio/thud2.wav", "thud2");
    loadSound("audio/thud3.wav", "thud3");
    loadSound("audio/thud4.wav", "thud4");
    loadSound("audio/thud5.wav", "thud5");
    loadSound("audio/thud6.wav", "thud6");
    loadSound("audio/thud7.wav", "thud7");
    srand((uint)time(NULL));
    
    // create scenes and switch to initial one
    this->sceneManager->addScene("test", new TestScene());
    this->sceneManager->change("test");
    
    // reset timestep so deltaTime isn't massive on first frame
    this->timestep->resetTime();
  }
};

Application* currentApplication = new TestApplication();
