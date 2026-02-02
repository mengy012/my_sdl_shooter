#include "player.h"
#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>

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

void Player::keyBoardControl()
{
    auto keyboard_state = SDL_GetKeyboardState(NULL);

    if (keyboard_state[SDL_SCANCODE_W])
    {
        position.y -= 0.4f;
    }
    if (keyboard_state[SDL_SCANCODE_A])
    {
        position.x -= 0.4f;
    }
    if (keyboard_state[SDL_SCANCODE_S])
    {
        position.y += 0.4f;
    }
    if (keyboard_state[SDL_SCANCODE_D])
    {
        position.x += 0.4f;
    }

    position.x = std::clamp(position.x, 0.f,
                            static_cast<float>(Game::instance().get_window_width()) - width);
    position.y = std::clamp(position.y, 0.f,
                            static_cast<float>(Game::instance().get_window_height()) - height);
}
