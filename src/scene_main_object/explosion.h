#pragma once

#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <memory>

class ExplosionTextureManager
{
  public:
    ExplosionTextureManager();
    ~ExplosionTextureManager();
    SDL_Texture* get_texture();

  private:
    SDL_Texture* texture_{nullptr};
};

class Explosion
{
  public:
    Explosion() = default;
    Explosion(float x, float y, int width, int height,
              std::chrono::steady_clock::time_point start_time, SDL_Texture* texture); // 传入敌人坐标,计算爆炸位置
    ~Explosion() = default;

    void render(SDL_Renderer* renderer);
    std::chrono::steady_clock::time_point getStartTime() const
    {
        return start_time;
    }
    bool getIsFinished() const
    {
        return is_finished;
    }
    void update(double delta_time);

  private:
    SDL_FPoint position;                              // 爆炸位置
    int frame_width{0};                               // 爆炸帧宽度
    int frame_height{0};                              // 爆炸帧高度
    int total_frames{0};                              // 总帧数
    int current_frame{0};                             // 当前帧索引
    std::chrono::steady_clock::time_point start_time; // 开始时间
    bool is_finished{false};                          // 是否完成

    SDL_Texture* texture_{nullptr}; // 爆炸纹理
};