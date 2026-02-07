#pragma once

#include "../game.h"
#include <SDL.h>
#include <memory>

// 暂停图标
class ButtonPause
{
  public:
    ButtonPause();
    ~ButtonPause() = default;

    void render(SDL_Renderer* renderer);
    bool isInside(int x, int y); // 检测点是否在按钮内

  private:
    SDL_Rect rect;
    std::unique_ptr<SDL_Texture, DeleteTexture> texture;
};