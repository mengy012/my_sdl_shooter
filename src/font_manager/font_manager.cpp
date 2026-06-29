#include "font_manager.h"
#include "../game.h"

FontManager::FontManager() {}

FontManager::~FontManager()
{
    for (auto& [font_type, font] : fonts_)
    {
        TTF_CloseFont(font);
    }
}

TTF_Font* FontManager::getFont(FontType font_type)
{
    TTF_Font* font = nullptr;

    auto it = fonts_.find(font_type);
    if (it != fonts_.end())
    {
        font = it->second;
    }

    switch (font_type)
    {
    case FontType::Default:

        font = TTF_OpenFont("../../assets/font/VonwaonBitmap-12px.ttf", 24);
        if (!font)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ttf_openfont failed %s\n", SDL_GetError());
            Game::instance().getIsRunning() = false;
        }
        fonts_[font_type] = font;
        break;
    case FontType::Title:
        font = TTF_OpenFont("../../assets/font/VonwaonBitmap-16px.ttf", 64);
        if (!font)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ttf_openfont failed %s\n", SDL_GetError());
            Game::instance().getIsRunning() = false;
        }
        fonts_[font_type] = font;
        break;
    }

    return font;
}
