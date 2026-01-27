#pragma once

#include <SDL.h>

// 场景基类
class Scene
{
  public:
    Scene();
    virtual ~Scene();

    virtual void init() = 0;
    virtual void handleEvent(SDL_Event& event) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;

  private:
};
