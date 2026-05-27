#pragma once

#include "../game.h"
#include "item.h"
#include <SDL.h>

// 恢复生命值的物品
class ItemLifeRestoring : public Item
{
  public:
    ItemLifeRestoring(float x, float y, SDL_Texture* texture);
    ~ItemLifeRestoring() = default;

    ItemType getType() const override; // 获取物品类型
    SDL_FPoint& getPosition() override;
    int getWidth() const override;
    int getHeight() const override;
    int getDamage() const override; // 这个物品没有伤害,返回0
    bool& getIsDestroyed() override;

    void render(SDL_Renderer* renderer) override;
    void update(double delta_time) override; // 玩家碰撞检测 边缘反弹

  private:
    SDL_FPoint position;  // 物品位置
    int width{0};         // 物品宽度
    int height{0};        // 物品高度
    float speed{150.f};   // 物品速度（像素 / s）
    SDL_FPoint direction; // 物品移动方向

    SDL_Texture* texture_{nullptr}; // 物品纹理

    int heal_amount{1};       // 恢复生命值数量
    bool is_destroyed{false}; // 是否销毁,玩家碰到物品时设置为true
    int bounce_count_max{2};  // 边缘反弹最大次数
    int bounce_count{0};      // 边缘反弹次数

    ItemType type{ItemType::Life}; // 物品类型
};
