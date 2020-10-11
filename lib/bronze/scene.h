#ifndef SCENE_H
#define SCENE_H

#include <map>

class Scene {
public:
  virtual void update(float) = 0;
  virtual void render() = 0;
  virtual void onLoad() = 0;
  virtual void onUnload() = 0;
};

typedef std::map<const char*, Scene*> scenemap;

class SceneManager {
private:
  scenemap scenes;
  scenemap::iterator mapit;
  
public:
  const char* currentScene;
  const char* prevScene;
  
  void addScene(const char*, Scene*);
  void change(const char*);
  void update(float);
  void render();
};

#endif
