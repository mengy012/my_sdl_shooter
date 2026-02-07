#pragma once

#include "../object/button_pause.h"
#include "../object/player.h"
#include "./scene.h"

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

    ButtonPause pauseButton; // 暂停按钮

    bool is_paused{false};
    SDL_Rect pause_text_rect{0, 0, 0, 0};
    SDL_Rect continue_text_rect{0, 0, 0, 0};

    void updatePauseTextLayout();
};