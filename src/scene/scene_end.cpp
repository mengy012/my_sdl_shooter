#include "scene_end.h"
#include "../game.h"
#include <memory>
#include <string>

static void renderText(SDL_Renderer* renderer, TTF_Font* font, std::string_view text, SDL_Color textColor, float posY, float fontSize)
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

SceneEnd::SceneEnd() {}

SceneEnd::~SceneEnd() {}

void SceneEnd::init() {}

void SceneEnd::handleEvent(SDL_Event& event) {}

void SceneEnd::update(double delta_time) {}

void SceneEnd::render()
{
    SDL_Renderer* renderer = Game::instance().getRenderer();
    TTF_Font* font = Game::instance().getFont(FontType::Title);
    SDL_Color textColor = {255, 255, 255, 255};

    // 最终得分
    std::string final_score_text("你的得分是：");
    final_score_text += std::to_string(Game::instance().getFinalScore());
    renderText(renderer, font, final_score_text, textColor, 0.2f, 0.5f);
    // "game over"
    renderText(renderer, font, "game over", textColor, 0.4f, 1.f);
    // “输入你的名字，按回车键确认：”
    renderText(renderer, font, "输入你的名字，按回车键确认：", textColor, 0.6f, 0.5f);
}

void SceneEnd::clean() {}

SceneState SceneEnd::getState()
{
    return state;
}
