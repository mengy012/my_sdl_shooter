#include "scene_title.h"
#include "../game.h"

SceneTitle::SceneTitle() {}

SceneTitle::~SceneTitle()
{
    Mix_HaltMusic();
}

void SceneTitle::init()
{
    Mix_PlayMusic(Game::instance().getBackgroundMusic(MusicType::Title), -1);
}

void SceneTitle::handleEvent(SDL_Event& event) {}

void SceneTitle::update(double delta_time)
{
    // 控制文本闪烁显示
    time_count_ += delta_time;
    if (time_count_ >= 2.0)
    {
        time_count_ = .0;
    }
}

void SceneTitle::render()
{
    SDL_Renderer* renderer = Game::instance().getRenderer();
    TTF_Font* font = Game::instance().getFont(FontType::Title);

    // 设置文本颜色为白色
    SDL_Color textColor = {255, 255, 255, 255};
    // 渲染文本
    renderText(renderer, font, "SDL太空战机", textColor, 0.25f);
    if (time_count_ >= 0.75f)
        renderText(renderer, font, "按J开始游戏", textColor, 0.75f, 0.5f);
}

void SceneTitle::clean() {}

SceneState SceneTitle::getState()
{
    return state;
}

void SceneTitle::renderText(SDL_Renderer* renderer, TTF_Font* font, std::string_view text, SDL_Color textColor, float posY, float fontSize)
{
    // 创建文本表面
    std::unique_ptr<SDL_Surface, DeleteSurface> textSurface(TTF_RenderUTF8_Solid(font, text.data(), textColor));
    if (!textSurface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ttf_rendertext_solid failed %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
        return;
    }

    // 创建纹理
    std::unique_ptr<SDL_Texture, DeleteTexture> textTexture(SDL_CreateTextureFromSurface(renderer, textSurface.get()));
    if (!textTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl_createtexturefromsurface failed %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
        return;
    }

    // 获取纹理宽高
    int textWidth = textSurface->w * fontSize;
    int textHeight = textSurface->h * fontSize;

    // 设置渲染目标为窗口指定位置
    int windowWidth = Game::instance().get_window_width();
    int windowHeight = Game::instance().get_window_height();
    SDL_Rect renderQuad = {windowWidth / 2 - textWidth / 2, windowHeight * posY - textHeight / 2, textWidth, textHeight};

    // 渲染纹理
    SDL_RenderCopy(renderer, textTexture.get(), nullptr, &renderQuad);
}
