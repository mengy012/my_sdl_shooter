#include "item_life_restoring.h"

ItemLifeRestoring::ItemLifeRestoring(float x, float y, SDL_Texture* texture)
{
    position.x = x;
    position.y = y;

    texture_ = texture;
    if (texture_)
    {
        SDL_QueryTexture(texture_, NULL, NULL, &width, &height);
        width /= 2;
        height /= 2;
    }

    auto random_v = Game::instance().getRandomFloat();
    float angle = random_v * 2 * (float)M_PI;
    direction.x = cos(angle);
    direction.y = sin(angle);
}

ItemType ItemLifeRestoring::getType() const
{
    return type;
}

SDL_FPoint& ItemLifeRestoring::getPosition()
{
    return position;
}

int ItemLifeRestoring::getWidth() const
{
    return width;
}

int ItemLifeRestoring::getHeight() const
{
    return height;
}

int ItemLifeRestoring::getDamage() const
{
    return 0;
}

bool& ItemLifeRestoring::getIsDestroyed()
{
    return is_destroyed;
}

void ItemLifeRestoring::render(SDL_Renderer* renderer)
{
    if (texture_)
    {
        SDL_Rect dest_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};
        SDL_RenderCopy(renderer, texture_, NULL, &dest_rect);
    }
}

void ItemLifeRestoring::update(double delta_time)
{
    position.x += direction.x * delta_time * speed;
    position.y += direction.y * delta_time * speed;

    // 边缘反弹
    auto& game = Game::instance();

    if (position.x < 0)
    {
        position.x = 0;
        direction.x = -direction.x;
        bounce_count++;
    }
    else if (position.x + width > game.get_window_width())
    {
        position.x = game.get_window_width() - width;
        direction.x = -direction.x;
        bounce_count++;
    }
    if (position.y < 0)
    {
        position.y = 0;
        direction.y = -direction.y;
        bounce_count++;
    }
    else if (position.y + height > game.get_window_height())
    {
        position.y = game.get_window_height() - height;
        direction.y = -direction.y;
        bounce_count++;
    }
    if (bounce_count > bounce_count_max)
    {
        is_destroyed = true;
    }
}
