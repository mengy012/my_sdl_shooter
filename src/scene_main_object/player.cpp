#include "player.h"
#include "../game.h"
#include "./music_manager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <cmath>

Player::Player()
{
    texture.reset(IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/SpaceShip.png"));
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load spaceship img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }

    SDL_QueryTexture(texture.get(), NULL, NULL, &width, &height);
    width /= 4;
    height /= 4;

    position.x = Game::instance().get_window_width() / 2.f - width / 2.f;
    position.y = Game::instance().get_window_height() - static_cast<float>(height);

    // 初始化上次射击时间
    last_shoot_time = std::chrono::steady_clock::now();
}

Player::~Player() {}

SDL_Texture* Player::getTexture()
{
    return texture.get();
}

SDL_FPoint& Player::getPosition()
{
    return position;
}

int Player::getWidth()
{
    return width;
}

int Player::getHeight()
{
    return height;
}

int& Player::getHealth()
{
    return health;
}

int Player::getHealthMax()
{
    return health_max;
}

bool& Player::getIsLive()
{
    return is_live;
}

void Player::render(SDL_Renderer* renderer)
{

    SDL_Rect player_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};

    SDL_RenderCopy(renderer, texture.get(), NULL, &player_rect);
}

void Player::keyBoardControl(double delta_time, MusicManager& music_manager)
{

    auto keyboard_state = SDL_GetKeyboardState(NULL);

    float cur_accel = accel;
    if (keyboard_state[SDL_SCANCODE_LSHIFT])
    {
        cur_accel *= 3.f;
    }

    // 1. 输入方向
    float dir_x = 0.f;
    float dir_y = 0.f;

    if (keyboard_state[SDL_SCANCODE_W])
        dir_y -= 1.f;
    if (keyboard_state[SDL_SCANCODE_S])
        dir_y += 1.f;
    if (keyboard_state[SDL_SCANCODE_A])
        dir_x -= 1.f;
    if (keyboard_state[SDL_SCANCODE_D])
        dir_x += 1.f;

    // 2. 归一化（防止斜向更快）
    float len = std::hypot(dir_x, dir_y);
    if (len > 0.f)
    {
        dir_x /= len;
        dir_y /= len;
    }

    // 3. 加速
    velocity.x += dir_x * cur_accel * delta_time;
    velocity.y += dir_y * cur_accel * delta_time;

    // 4. 阻尼（平滑减速）
    float damp = std::exp(-damping * delta_time);
    velocity.x *= damp;
    velocity.y *= damp;

    // 5. 更新位置
    position.x += velocity.x * delta_time;
    position.y += velocity.y * delta_time;
    // 6. 边界控制
    position.x = std::clamp(position.x, 0.f, static_cast<float>(Game::instance().get_window_width()) - width);
    position.y = std::clamp(position.y, 0.f, static_cast<float>(Game::instance().get_window_height()) - height);

    // 控制子弹发射
    if (keyboard_state[SDL_SCANCODE_J])
    {
        auto now = std::chrono::steady_clock::now();
        if (now - last_shoot_time >= shoot_cooldown)
        {
            shoot();
            // 播放射击音效
            Mix_PlayChannel(0, music_manager.getChunk(ChunkType::Effect_player_shoot), 0);
            last_shoot_time = now;
        }
    }
}

void Player::update(std::vector<Enemy>& enemies, std::list<EnemyBullet>& enemy_bullets, std::list<std::unique_ptr<Item>>& items, MusicManager& music_manager)
{
    SDL_Rect player_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};

    for (auto& bullet : enemy_bullets)
    {
        SDL_Rect bullet_rect{
            static_cast<int>(bullet.getPosition().x),
            static_cast<int>(bullet.getPosition().y),
            bullet.getWidth(),
            bullet.getHeight(),
        };
        if (SDL_HasIntersection(&player_rect, &bullet_rect))
        {
            health -= bullet.getDamage();
            bullet.getIsDestroyed() = true;
            // 播放被敌人击中音效
            Mix_PlayChannel(-1, music_manager.getChunk(ChunkType::Effect_hit), 0);
        }
    }
    enemy_bullets.remove_if([](EnemyBullet& b) { return b.getIsDestroyed(); }); // 移除已销毁的子弹

    for (auto& enemy : enemies)
    {
        SDL_Rect enemy_rect{static_cast<int>(enemy.getPosition().x), static_cast<int>(enemy.getPosition().y), enemy.getWidth(), enemy.getHeight()};
        if (SDL_HasIntersection(&player_rect, &enemy_rect))
        {
            health -= 1;
            enemy.getHealth() = 0; // 设置生命值为0，让updateEnemy()统一处理爆炸和删除
        }
    }

    for (auto& item : items)
    {

        SDL_Rect item_rect{static_cast<int>(item->getPosition().x), static_cast<int>(item->getPosition().y), item->getWidth(), item->getHeight()};
        if (SDL_HasIntersection(&player_rect, &item_rect))
        {
            switch (item->getType())
            {
            case ItemType::Life:
                if (health < health_max)
                {
                    health += 1;
                }
                item->getIsDestroyed() = true; // 让updateItems()移除已销毁的物品
                break;
            case ItemType::Shield:
                item->getIsDestroyed() = true;
                break;
            case ItemType::Time:
                item->getIsDestroyed() = true;
                break;
            }
            // 播放拾取物品音效
            Mix_PlayChannel(-1, music_manager.getChunk(ChunkType::Effect_get_item), 0);
        }
    }

    if (health <= 0)
    {
        is_live = false;
    }
}

void Player::shoot()
{
    // 以玩家飞机位置为基础创建子弹对象
    float bullet_x = position.x + width / 2.f - bullet_template.getWidth() / 2.f;
    float bullet_y = position.y;

    bullets.emplace_back(bullet_x, bullet_y, bullet_template);
}

void Player::updateBullets(double delta_time, std::vector<Enemy>& enemies, MusicManager& music_manager)
{
    for (auto& bullet : bullets)
    {
        bullet.update(delta_time);
    }

    // 检查子弹是否击中敌机
    for (auto bullet = bullets.begin(); bullet != bullets.end();)
    {
        SDL_Rect bullet_rect{static_cast<int>(bullet->getPosition().x), static_cast<int>(bullet->getPosition().y), bullet->getWidth(), bullet->getHeight()};
        for (auto enemy = enemies.begin(); enemy != enemies.end();)
        {
            SDL_Rect enemy_rect{static_cast<int>(enemy->getPosition().x), static_cast<int>(enemy->getPosition().y), enemy->getWidth(), enemy->getHeight()};
            if (SDL_HasIntersection(&enemy_rect, &bullet_rect))
            {
                enemy->getHealth() -= bullet->getDamage();
                bullet->getIsDestroyed() = true;
                // 播放击中敌人音效
                Mix_PlayChannel(-1, music_manager.getChunk(ChunkType::Effect_hit), 0);
                break;
            }
            ++enemy;
        }
        ++bullet;
    }
    // 移除已销毁的子弹
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](PlayerBullet& b) { return b.getIsDestroyed(); }), bullets.end());
    // 移除飞出屏幕的子弹
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](PlayerBullet& b) { return b.getPosition().y + b.getHeight() < 0; }), bullets.end());
}

void Player::renderBullets(SDL_Renderer* renderer)
{
    for (auto& bullet : bullets)
    {
        bullet.render(renderer);
    }
}
