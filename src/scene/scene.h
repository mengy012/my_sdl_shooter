#pragma once

#include <SDL.h>

// 游戏场景状态枚举
enum class SceneState
{
    Title,
    Main,
    End
};

// 场景基类
class Scene
{
  public:
    Scene();
    virtual ~Scene();

    virtual void init() = 0;
    virtual void handleEvent(SDL_Event& event) = 0;
    virtual void update(double delta_time) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual SceneState getState() = 0;

  private:
};
