#pragma once
#include "scene.h"

class SceneTitle : public Scene
{
  public:
    SceneTitle();
    ~SceneTitle();

    void init() override;
    void handleEvent(SDL_Event& event) override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;
    SceneState getState() override;
};
