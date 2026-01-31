#include "scene_main.h"
#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>

SceneMain::SceneMain() {}

SceneMain::~SceneMain() {}

void SceneMain::init()
{
    player.texture.reset(
        IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/SpaceShip.png"));
    if (!player.texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load spaceship img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }

    SDL_QueryTexture(player.texture.get(), NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;

    player.position.x = Game::instance().get_window_width() / 2.f - player.width / 2.f;
    player.position.y = Game::instance().get_window_height() - static_cast<float>(player.height);
}

void SceneMain::handleEvent(SDL_Event& event) {}

void SceneMain::update() {}

void SceneMain::render()
{
    // 测试
    std::unique_ptr<SDL_Texture, TextureDeleter> background_texture(
        IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/bg.png"));
    if (!background_texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load background img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }
    SDL_Rect bg_rect{0, 0, 600, 800};
    SDL_RenderCopy(Game::instance().getRenderer(), background_texture.get(), NULL, &bg_rect);
    SDL_SetTextureColorMod(player.texture.get(), 100, 100, 100);

    SDL_Rect player_rect{static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                         player.width, player.height};

    SDL_RenderCopy(Game::instance().getRenderer(), player.texture.get(), NULL, &player_rect);
}

void SceneMain::clean() {}
