#include "player_bullet.h"

int PlayerBullet::bullet_count = 0;
PlayerBullet::PlayerBullet(float x, float y, const PlayerBullet& template_bullet)
{
    position.x = x;
    position.y = y;

    texture = template_bullet.texture;
    width = template_bullet.width;
    height = template_bullet.height;

    PlayerBullet::bullet_count++;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Bullet created. Current bullet count: %d\n",
                PlayerBullet::bullet_count);
}

PlayerBullet::PlayerBullet()
{
    position.x = 0.f;
    position.y = 0.f;

    texture = IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/bullet.png");
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load bullet img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }
    else
    {
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        width /= 4;
        height /= 4;
    }
    PlayerBullet::bullet_count = 1;
}

PlayerBullet::~PlayerBullet()
{
    PlayerBullet::bullet_count--;
    if ((PlayerBullet::bullet_count == 0) && texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Bullet texture destroyed\n");
    }
}

SDL_FPoint& PlayerBullet::getPosition()
{
    return position;
}

int PlayerBullet::getWidth() const
{
    return width;
}

int PlayerBullet::getHeight() const
{
    return height;
}

void PlayerBullet::render(SDL_Renderer* renderer)
{
    SDL_Rect bullet_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};
    SDL_RenderCopy(renderer, texture, NULL, &bullet_rect);
}

void PlayerBullet::update(double delta_time)
{
    position.y -= delta_time * speed;
}
