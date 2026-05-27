#pragma once

#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <unordered_map>

// 物品类型枚举
enum class ItemType
{
    Life,
    Shield,
    Time
};

// 物品基类
class Item
{
  public:
    virtual ~Item() = default; // 虚析构函数，确保派生类对象被正确销毁

    virtual ItemType getType() const = 0; // 获取物品类型
    virtual SDL_FPoint& getPosition() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getDamage() const = 0;
    virtual bool& getIsDestroyed() = 0;

    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void update(double delta_time) = 0; // 玩家碰撞检测 边缘反弹
};

// 物品纹理管理类
class ItemTextureManager
{
  public:
    ItemTextureManager() = default;
    ~ItemTextureManager()
    {
        for (auto& pair : textures_)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "item texture destroyed\n");
            if (pair.second)
            {
                SDL_DestroyTexture(pair.second);
            }
        }
    }

    SDL_Texture* getTexture(ItemType type)
    {
        auto it = textures_.find(type);
        if (it != textures_.end())
        {
            return it->second;
        }

        switch (type)
        {
        case ItemType::Life:
            textures_[type] = IMG_LoadTexture(Game::instance().getRenderer(),
                                              "../../assets/image/bonus_life.png");
            return textures_[type];
            break;
        case ItemType::Shield:
            textures_[type] = IMG_LoadTexture(Game::instance().getRenderer(),
                                              "../../assets/image/bonus_shield.png");
            return textures_[type];
            break;
        case ItemType::Time:
            textures_[type] = IMG_LoadTexture(Game::instance().getRenderer(),
                                              "../../assets/image/bonus_time.png");
            return textures_[type];
            break;
        }
        return nullptr;
    }

  private:
    std::unordered_map<ItemType, SDL_Texture*> textures_;
};
