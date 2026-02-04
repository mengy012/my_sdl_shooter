#pragma once

#include "../object/player.h"
#include "scene.h"

// 主场景
class SceneMain : public Scene
{
  public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event& event) override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;

  private:
    Player player; // 玩家飞机
};