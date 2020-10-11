#include "scene.h"

void SceneManager::addScene(const char* name, Scene* scene) {
  this->scenes.insert(scenemap::value_type(name, scene));
}

void SceneManager::change(const char* name) {
  this->prevScene = this->currentScene;
  this->currentScene = name;
  
  // get previous scene and unload it
  scenemap::iterator it;
  it = this->scenes.find(this->prevScene);
  if (it != this->scenes.end()) {
    it->second->onUnload();
  }
  
  this->scenes.at(this->currentScene)->onLoad();
}

void SceneManager::update(float dt) {
  this->mapit = this->scenes.find(this->currentScene);
  if (this->mapit != this->scenes.end()) {
    this->mapit->second->update(dt);
  }
}

void SceneManager::render() {
  this->mapit = this->scenes.find(this->currentScene);
  if (this->mapit != this->scenes.end()) {
    this->mapit->second->render();
  }
}
