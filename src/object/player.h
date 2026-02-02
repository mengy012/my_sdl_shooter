#pragma once

#include <SDL.h>
#include <memory>

struct TextureDeleter // 纹理删除器
{
    void operator()(SDL_Texture* texture) const noexcept
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }
};

class Player
{
  public:
    Player();
    ~Player();

    SDL_Texture* getTexture();
    SDL_FPoint& getPosition();
    int getWidth();
    int getHeight();

    void keyBoardControl(); // 使用键盘控制玩家移动
  private:
    std::unique_ptr<SDL_Texture, TextureDeleter> texture;
    SDL_FPoint position{0, 0};
    int width{0};
    int height{0};
};
