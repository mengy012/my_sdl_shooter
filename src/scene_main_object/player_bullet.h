#pragma once

#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <memory>

class PlayerBullet
{
  public:
    PlayerBullet(
        float x, float y,
        const PlayerBullet& template_bullet); // 每次发射子弹时调用,传入玩家飞机位置和子弹对象模板
    PlayerBullet();                           //
    ~PlayerBullet();

    SDL_FPoint& getPosition();
    int getWidth() const;
    int getHeight() const;
    int getDamage() const;
    bool& getIsDestroyed();

    void render(SDL_Renderer* renderer);
    void update(double delta_time); // 更新子弹位置

  private:
    SDL_FPoint position; // 子弹位置
    int width{0};        // 子弹宽度
    int height{0};       // 子弹高度
    float speed{500.f};  // 子弹速度（像素 / s）

    std::shared_ptr<SDL_Texture> texture; // 子弹纹理

    int damage{1};            // 子弹伤害
    bool is_destroyed{false}; // 是否销毁,子弹击中敌人时设置为true
};