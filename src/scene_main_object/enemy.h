#pragma once

#include "../game.h"
#include "enemy_bullet.h"
#include <SDL.h>
#include <list>
#include <memory>
#include <random>

class Enemy
{
  public:
    Enemy(); // 默认构造函数,只用于构造敌人模板
    Enemy(bool normal,
          const Enemy& templateEnemy); // 第一个参数无意义,用于区分拷贝构造函数,随机生成敌人初始位置

    ~Enemy();

    SDL_Texture* getTexture();
    SDL_FPoint& getPosition();
    int getWidth();
    int getHeight();
    // 获取上次射击时间,可以修改为当前时间
    std::chrono::steady_clock::time_point& getLastShootTime()
    {
        return last_shoot_time;
    }
    // 获取射击冷却时间
    std::chrono::nanoseconds getShootCooldown()
    {
        return shoot_cooldown;
    }

    void render(SDL_Renderer* renderer);
    void update(double delta_time); // 更新敌机位置

  private:
    std::shared_ptr<SDL_Texture> texture;
    SDL_FPoint position{0, 0};
    int width{0};
    int height{0};
    float speed{200.f}; // 敌机速度（像素 / s）

    std::chrono::steady_clock::time_point last_shoot_time;                    // 上次射击时间
    std::chrono::nanoseconds shoot_cooldown{std::chrono::milliseconds(1000)}; // 射击冷却时间
};