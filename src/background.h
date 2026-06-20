#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>

class Background
{
  public:
    Background(SDL_Texture* texture, float speed = 30.0f);
    ~Background();

    // 默认构造函数
    Background() = default;

    // 移动赋值
    Background& operator=(Background&& other) noexcept;

    // 更新背景位置
    void update(double delta_time);
    // 渲染背景
    void render();

  private:
    SDL_Texture* texture_{nullptr};
    // 背景纹理宽高
    int width_{0};
    int height_{0};
    // 背景纹理位置
    SDL_FPoint position_{0.0f, 0.0f};
    // y轴偏移
    float y_offset_{0.0f};
    // 背景纹理速度
    float speed_{30.0f};
};