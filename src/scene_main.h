#pragma once

#include "scene.h"

// 主场景
class SceneMain : public Scene
{
  public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event& event) override;
    void update() override;
    void render() override;
    void clean() override;

  private:
};