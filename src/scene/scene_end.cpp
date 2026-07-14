#include "scene_end.h"
#include "../game.h"
#include <memory>
#include <string>

static void renderText(SDL_Renderer* renderer, TTF_Font* font, std::string_view text, SDL_Color textColor, float posY, float fontSize, bool is_input_mode)
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

    // 输入模式下,调用SDL_SetTextInputRect设置输入区域
    if (is_input_mode)
    {
        SDL_Rect input_rect = {renderQuad.x, renderQuad.y + textHeight / 2, renderQuad.w, renderQuad.h};
        SDL_SetTextInputRect(&input_rect);
    }

    // 渲染纹理
    SDL_RenderCopy(renderer, textTexture.get(), nullptr, &renderQuad);
}

static int times_of_backspace(const char* c)
{
    int times = 1;

    while ((static_cast<unsigned char>(*c) & 0b11000000) == 0b10000000)
    {
        ++times;
        --c;
    }

    return times;
}

static void removeUTF8_character(std::string& str)
{
    int bytes = times_of_backspace(str.c_str() + str.size() - 1);
    for (int i = 0; i < bytes; ++i)
    {
        str.pop_back();
    }
}

SceneEnd::SceneEnd() {}

SceneEnd::~SceneEnd()
{
    if (SDL_IsTextInputActive())
    {
        SDL_StopTextInput();
    }
}

void SceneEnd::init()
{
    // 初始化输入模式
    if (!SDL_IsTextInputActive())
    {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl_starttextinput failed %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
        return;
    }
    else
    {
        is_input_mode = true;
        SDL_Rect input_rect{0, 0, 0, 0};
        SDL_SetTextInputRect(&input_rect);
    }
}

void SceneEnd::handleEvent(SDL_Event& event)
{
    if (is_input_mode)
    {

        // 输入模式下,处理文本输入事件
        if (event.type == SDL_TEXTINPUT)
        {
            input_name += event.text.text;
        }

        if (event.type == SDL_KEYDOWN)
        {
            // 处理回车键事件
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
            {
                is_input_mode = false;
                SDL_StopTextInput();
            }

            // 处理退格事件
            if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                if (!input_name.empty())
                {
                    removeUTF8_character(input_name);
                }
            }
        }
    }
    else
    {
    }
}

void SceneEnd::update(double delta_time) {}

void SceneEnd::render()
{
    SDL_Renderer* renderer = Game::instance().getRenderer();
    TTF_Font* font = Game::instance().getFont(FontType::Title);
    SDL_Color textColor = {255, 255, 255, 255};
    if (is_input_mode)
    { // 最终得分
        std::string final_score_text("你的得分是：");
        final_score_text += std::to_string(Game::instance().getFinalScore());
        renderText(renderer, font, final_score_text, textColor, 0.2f, 0.5f, false);
        // "game over"
        renderText(renderer, font, "game over", textColor, 0.4f, 1.f, false);
        // “输入你的名字，按回车键确认：”
        renderText(renderer, font, "输入你的名字，按回车键确认：", textColor, 0.6f, 0.5f, false);

        // 输入名称
        if (!input_name.empty())
        {
            renderText(renderer, font, input_name, textColor, 0.7f, 0.5f, true);
        }
    }
}

void SceneEnd::clean() {}

SceneState SceneEnd::getState()
{
    return state;
}
