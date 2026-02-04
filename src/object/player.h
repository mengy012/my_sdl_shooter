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

struct Vec2
{
    float x{};
    float y{};
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

    void keyBoardControl(double delta_time); // 使用键盘控制玩家移动
  private:
    std::unique_ptr<SDL_Texture, TextureDeleter> texture;
    SDL_FPoint position{0, 0};
    int width{0};
    int height{0};

    Vec2 velocity{0, 0};  // 当前速度
    float accel = 2500.f; // 加速度（像素 / s²）
    float damping = 6.f;  // 阻尼系数（越大减速越快）
};
