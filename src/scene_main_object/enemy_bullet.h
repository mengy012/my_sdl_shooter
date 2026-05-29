#pragma once

#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <memory>

class EnemyBullet
{
  public:
    EnemyBullet(float x, float y,
                const EnemyBullet& template_bullet); // 每次发射子弹时调用,传入玩家飞机位置和子弹对象模板
    EnemyBullet();                                   //
    ~EnemyBullet();

    SDL_FPoint& getPosition();
    int getWidth() const;
    int getHeight() const;
    int getDamage() const;
    bool& getIsDestroyed();
    void setDirection(SDL_FPoint player_position, int player_width, int player_height);

    void render(SDL_Renderer* renderer);
    void update(double delta_time); // 更新子弹位置

  private:
    SDL_FPoint position;  // 子弹位置
    int width{0};         // 子弹宽度
    int height{0};        // 子弹高度
    float speed{200.f};   // 子弹速度（像素 / s）
    SDL_FPoint direction; // 子弹方向

    std::shared_ptr<SDL_Texture> texture; // 子弹纹理

    int damage{1};            // 子弹伤害
    bool is_destroyed{false}; // 是否销毁,子弹击中玩家时设置为true
};