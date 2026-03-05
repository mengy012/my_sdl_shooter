#include "button_pause.h"
#include "../game.h"
#include <SDL_image.h>

ButtonPause::ButtonPause()
{
    texture.reset(IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/pause.png"));
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load pause button texture: %s\n",
                     SDL_GetError());
        Game::instance().getIsRunning() = false;
    }
    SDL_QueryTexture(texture.get(), nullptr, nullptr, &rect.w, &rect.h);
    const int x = Game::instance().get_window_width() - rect.w;
    const int y = 0;
    rect = {x, y, rect.w, rect.h};
}

void ButtonPause::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture.get(), NULL, &rect);
}

bool ButtonPause::isInside(int x, int y)
{
    return (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);
}
