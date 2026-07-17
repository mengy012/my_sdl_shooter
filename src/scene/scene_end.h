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

    // 输入字符时光标闪烁时间阈值（计时器大于等于该值时，渲染下划线“_”）
    double display_threshold{0.5};

    // 闪烁控制计时器
    double cursor_timer{0.0};

    // 渲染下划线“_”
    void render_cursor(float x, float y, bool is_flickering);
};
