#pragma once

#include "scene.h"
#include <string>

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

    // 输入模式开关
    bool is_input_mode{false};

    // 输入名称
    std::string input_name;

    // 输入名称最大字符数
    const int input_max{10};
};
