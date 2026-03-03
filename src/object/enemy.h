#pragma once

#include "../game.h"
#include <SDL.h>
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

    void render(SDL_Renderer* renderer);
    void update(double delta_time); // 更新敌机位置

  private:
    std::shared_ptr<SDL_Texture> texture;
    SDL_FPoint position{0, 0};
    int width{0};
    int height{0};
    float speed{200.f}; // 敌机速度（像素 / s）
};