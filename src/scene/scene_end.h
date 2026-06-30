#pragma once

#include "scene.h"

class SceneEnd : public Scene
{
  public:
    SceneEnd();
    ~SceneEnd();

    void init() override;
    void handleEvent(SDL_Event& event) override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;
    SceneState getState() override;

  private:
    SceneState state{SceneState::End};
};
