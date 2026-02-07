#include "scene_main.h"
#include "../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>

SceneMain::SceneMain() {}

SceneMain::~SceneMain() {}

void SceneMain::init() {}

void SceneMain::updatePauseTextLayout()
{
    std::string_view pause_text("游戏已暂停");
    std::unique_ptr<SDL_Texture, DeleteTexture> continue_picture;
    continue_picture.reset(
        IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/play_fill.png"));
    if (!continue_picture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load continue picture: %s\n",
                     SDL_GetError());
        Game::instance().getIsRunning() = false;
    }
    int pause_w = 0;
    int pause_h = 0;
    int continue_w = 0;
    int continue_h = 0;

    TTF_SizeUTF8(Game::instance().getFont(), pause_text.data(), &pause_w, &pause_h);
    SDL_QueryTexture(continue_picture.get(), nullptr, nullptr, &continue_w, &continue_h);

    const int center_x = Game::instance().get_window_width() / 2;
    const int center_y = Game::instance().get_window_height() / 2;

    pause_text_rect = {center_x - pause_w / 2, center_y - pause_h - 10, pause_w, pause_h};
    continue_text_rect = {center_x - continue_w / 2, center_y + 10, continue_w, continue_h};
}

void SceneMain::handleEvent(SDL_Event& event)
{
    // 处理暂停按钮点击事件
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        int x = event.button.x;
        int y = event.button.y;
        if (!is_paused && pauseButton.isInside(x, y))
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Pause button clicked");
            is_paused = true;
            updatePauseTextLayout();
        }
        else if (is_paused)
        {
            const bool inside_continue =
                (x >= continue_text_rect.x) && (x <= continue_text_rect.x + continue_text_rect.w) &&
                (y >= continue_text_rect.y) && (y <= continue_text_rect.y + continue_text_rect.h);
            if (inside_continue)
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Continue clicked");
                is_paused = false;
            }
        }
    }
}
void SceneMain::update(double delta_time)
{
    if (is_paused)
    {
        return;
    }
    player.keyBoardControl(delta_time);
}

void SceneMain::render()
{
    // 测试
    // std::unique_ptr<SDL_Texture, DeleteTexture> background_texture(
    //     IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/bg.png"));
    // if (!background_texture)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load background img failed %s\n", IMG_GetError());
    //     Game::instance().getIsRunning() = false;
    // }
    // SDL_Rect bg_rect{0, 0, 600, 800};
    // SDL_RenderCopy(Game::instance().getRenderer(), background_texture.get(), NULL, &bg_rect);
    // SDL_SetTextureColorMod(player.texture.get(), 100, 100, 100);

    player.render(Game::instance().getRenderer());
    pauseButton.render(Game::instance().getRenderer());

    if (is_paused)
    {
        SDL_Renderer* renderer = Game::instance().getRenderer();

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
        SDL_Rect overlay{0, 0, Game::instance().get_window_width(),
                         Game::instance().get_window_height()};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Color white{255, 255, 255, 255};
        std::unique_ptr<SDL_Surface, DeleteSurface> pause_surface;
        pause_surface.reset(
            TTF_RenderUTF8_Blended(Game::instance().getFont(), "游戏已暂停", white));
        std::unique_ptr<SDL_Texture, DeleteTexture> pause_texture(
            SDL_CreateTextureFromSurface(renderer, pause_surface.get()));
        SDL_RenderCopy(renderer, pause_texture.get(), NULL, &pause_text_rect);

        std::unique_ptr<SDL_Texture, DeleteTexture> continue_picture;
        continue_picture.reset(
            IMG_LoadTexture(Game::instance().getRenderer(), "../assets/image/play_fill.png"));
        if (!continue_picture)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load continue picture: %s\n",
                         SDL_GetError());
            Game::instance().getIsRunning() = false;
        }
        SDL_RenderCopy(renderer, continue_picture.get(), NULL, &continue_text_rect);
    }
}

void SceneMain::clean() {}