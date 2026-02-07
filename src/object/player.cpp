#include "player.h"
#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <cmath>

Player::Player()
{
    texture.reset(IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/SpaceShip.png"));
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

void Player::render(SDL_Renderer* renderer)
{

    SDL_Rect player_rect{static_cast<int>(position.x), static_cast<int>(position.y), width, height};

    SDL_RenderCopy(renderer, texture.get(), NULL, &player_rect);
}

void Player::keyBoardControl(double delta_time)
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
    position.x = std::clamp(position.x, 0.f,
                            static_cast<float>(Game::instance().get_window_width()) - width);
    position.y = std::clamp(position.y, 0.f,
                            static_cast<float>(Game::instance().get_window_height()) - height);
}
