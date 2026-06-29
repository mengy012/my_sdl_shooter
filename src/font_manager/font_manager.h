#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>

enum class FontType
{
    Default,
    Title,
};

class FontManager
{
  public:
    FontManager();
    ~FontManager();

    // get font
    TTF_Font* getFont(FontType font_type);

  private:
    std::unordered_map<FontType, TTF_Font*> fonts_;
};
