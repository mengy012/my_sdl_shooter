#pragma once
#include "scene.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string_view>

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

  private:
    SceneState state{SceneState::Title};

    float time_count_{0.0f};

    /// @brief
    /// @param renderer
    /// @param font
    /// @param text
    /// @param textColor
    /// @param posY 文本垂直位置，0.0到1.0之间
    void renderText(SDL_Renderer* renderer, TTF_Font* font, std::string_view text, SDL_Color textColor, float posY, float fontSize = 1.0f);
};
