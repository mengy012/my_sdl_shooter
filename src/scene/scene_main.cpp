#include "scene_main.h"
#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>

SceneMain::SceneMain() {}

SceneMain::~SceneMain() {}

void SceneMain::init() {}

void SceneMain::handleEvent(SDL_Event& event) {}

void SceneMain::update(double delta_time)
{
    player.keyBoardControl(delta_time);
}

void SceneMain::render()
{
    // 测试
    // std::unique_ptr<SDL_Texture, TextureDeleter> background_texture(
    //     IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/bg.png"));
    // if (!background_texture)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load background img failed %s\n", IMG_GetError());
    //     Game::instance().getIsRunning() = false;
    // }
    // SDL_Rect bg_rect{0, 0, 600, 800};
    // SDL_RenderCopy(Game::instance().getRenderer(), background_texture.get(), NULL, &bg_rect);
    // SDL_SetTextureColorMod(player.texture.get(), 100, 100, 100);

    SDL_Rect player_rect{static_cast<int>(player.getPosition().x),
                         static_cast<int>(player.getPosition().y), player.getWidth(),
                         player.getHeight()};

    SDL_RenderCopy(Game::instance().getRenderer(), player.getTexture(), NULL, &player_rect);
}

void SceneMain::clean() {}
