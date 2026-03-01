#pragma once

#include "../game.h"
#include "player_bullet.h"
#include <SDL.h>
#include <chrono>
#include <memory>
#include <vector>

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

    void render(SDL_Renderer* renderer);
    void keyBoardControl(double delta_time); // 使用键盘控制玩家移动

    void shoot();                               // 射击子弹
    void updateBullets(double delta_time);      // 更新子弹位置
    void renderBullets(SDL_Renderer* renderer); // 渲染子弹
  private:
    std::unique_ptr<SDL_Texture, DeleteTexture> texture;
    SDL_FPoint position{0, 0};
    int width{0};
    int height{0};

    Vec2 velocity{0, 0};  // 当前速度
    float accel = 3000.f; // 加速度（像素 / s²）
    float damping = 10.f; // 阻尼系数（越大减速越快）

    std::chrono::steady_clock::time_point last_shoot_time;                   // 上次射击时间
    std::chrono::nanoseconds shoot_cooldown{std::chrono::milliseconds(150)}; // 射击冷却时间

    PlayerBullet
        bullet_template; // 子弹对象模板,存放在玩家对象中,每次发射子弹时以它为基础创建子弹对象
    std::vector<PlayerBullet> bullets; // 玩家子弹列表
};
