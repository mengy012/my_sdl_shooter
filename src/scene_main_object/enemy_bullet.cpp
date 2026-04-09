#include "enemy_bullet.h"
#include <SDL_image.h>

// 归一化方向向量
SDL_FPoint SDL_FPointNormalize(SDL_FPoint vec)
{
    SDL_FPoint result = {0.0f, 0.0f};
    float length = sqrtf(vec.x * vec.x + vec.y * vec.y);

    if (length > 0.0f)
    {
        result.x = vec.x / length;
        result.y = vec.y / length;
    }
    return result;
}
EnemyBullet::EnemyBullet(float x, float y, const EnemyBullet& template_bullet)
{
    position.x = x;
    position.y = y;

    texture = template_bullet.texture;
    width = template_bullet.width;
    height = template_bullet.height;
}

EnemyBullet::EnemyBullet()
{
    position.x = 0.f;
    position.y = 0.f;

    // 仅在首次调用时加载纹理
    if (!texture)
    {
        texture = std::shared_ptr<SDL_Texture>(
            IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/laser-1.png"),
            [](SDL_Texture* tex)
            {
                if (tex)
                {
                    SDL_DestroyTexture(tex);
                    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "enemyBullet texture destroyed\n");
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
    else
    {
        // 纹理已加载，直接复用宽高
        SDL_QueryTexture(texture.get(), NULL, NULL, &width, &height);
        width /= 4;
        height /= 4;
    }
}

EnemyBullet::~EnemyBullet() {}

SDL_FPoint& EnemyBullet::getPosition()
{
    return position;
}

int EnemyBullet::getWidth() const
{
    return width;
}

int EnemyBullet::getHeight() const
{
    return height;
}

void EnemyBullet::setDirection(SDL_FPoint player_position, int player_width, int player_height)
{
    direction = {(player_position.x + player_width / 2.f) - (position.x + width / 2.f),
                 (player_position.y + player_height / 2.f) - (position.y + height / 2.f)};
    direction = SDL_FPointNormalize(direction); // 归一化方向向量

    // 处理零向量情况，确保子弹有一个默认方向
    if (direction.x == 0.0f && direction.y == 0.0f)
    {
        direction.y = 1.0f; // 默认向下移动
    }
}

void EnemyBullet::render(SDL_Renderer* renderer)
{
    SDL_Rect bullet_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};
    float angle = atan2(direction.y, direction.x) * 180.0f / M_PI - 270.f;
    SDL_RenderCopyEx(renderer, texture.get(), NULL, &bullet_rect, angle, nullptr, SDL_FLIP_NONE);
}

void EnemyBullet::update(double delta_time)
{
    // 确保子弹总是移动，即使方向向量为零
    position.x += direction.x * speed * delta_time;
    position.y += direction.y * speed * delta_time;
}