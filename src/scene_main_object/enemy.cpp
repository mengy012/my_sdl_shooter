#include "enemy.h"
#include <SDL_image.h>
#include <iostream>

Enemy::Enemy()
{
    texture = std::shared_ptr<SDL_Texture>(
        IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/insect-1.png"),
        [](SDL_Texture* tex)
        {
            if (tex)
            {
                SDL_DestroyTexture(tex);
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Enemy texture destroyed\n");
            }
        });

    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load enemy img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }
    else
    {
        SDL_QueryTexture(texture.get(), NULL, NULL, &width, &height);
        width /= 4;
        height /= 4;
    }
}

Enemy::Enemy(bool normal, const Enemy& templateEnemy)
{
    texture = templateEnemy.texture;
    width = templateEnemy.width;
    height = templateEnemy.height;

    auto& game = Game::instance();
    position.x = game.getRandomFloat() * (game.get_window_width() - width);
    position.y = -static_cast<float>(height); // 从屏幕上方出现
}

Enemy::~Enemy() {}

SDL_Texture* Enemy::getTexture()
{
    return texture.get();
}

SDL_FPoint& Enemy::getPosition()
{
    return position;
}

int Enemy::getWidth()
{
    return width;
}

int Enemy::getHeight()
{
    return height;
}

int& Enemy::getHealth()
{
    return health;
}

bool& Enemy::getIsDestroyed()
{
    return is_destroyed;
}

void Enemy::render(SDL_Renderer* renderer)
{
    SDL_Rect dstRect = {static_cast<int>(position.x), static_cast<int>(position.y), width, height};
    SDL_RenderCopy(renderer, texture.get(), nullptr, &dstRect);
}

void Enemy::update(double delta_time)
{
    position.y += speed * static_cast<float>(delta_time);
}
