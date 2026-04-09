#include "player_bullet.h"

PlayerBullet::PlayerBullet(float x, float y, const PlayerBullet& template_bullet)
{
    position.x = x;
    position.y = y;

    texture = template_bullet.texture;
    width = template_bullet.width;
    height = template_bullet.height;
}

PlayerBullet::PlayerBullet()
{
    position.x = 0.f;
    position.y = 0.f;

    texture = std::shared_ptr<SDL_Texture>(
        IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/bullet.png"),
        [](SDL_Texture* tex)
        {
            if (tex)
            {
                SDL_DestroyTexture(tex);
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Bullet texture destroyed\n");
            }
        });

    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load bullet img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }
    else
    {
        SDL_QueryTexture(texture.get(), NULL, NULL, &width, &height);
        width /= 4;
        height /= 4;
    }
}

PlayerBullet::~PlayerBullet() {}

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

int PlayerBullet::getDamage() const
{
    return damage;
}

bool& PlayerBullet::getIsDestroyed()
{
    return is_destroyed;
}

void PlayerBullet::render(SDL_Renderer* renderer)
{
    SDL_Rect bullet_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};
    SDL_RenderCopy(renderer, texture.get(), NULL, &bullet_rect);
}

void PlayerBullet::update(double delta_time)
{
    position.y -= delta_time * speed;
}
