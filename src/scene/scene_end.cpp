#include "scene_end.h"
#include "../game.h"
#include "./scene_main.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <chrono>
#include <memory>
#include <string>

// ，返回当前渲染框右上角坐标
static SDL_FPoint renderText(SDL_Renderer* renderer, TTF_Font* font, std::string_view text, SDL_Color textColor, float posY, float fontSize, bool is_input_mode, bool is_left_align = true,
                             float x = NAN)
{
    // 创建文本表面
    std::unique_ptr<SDL_Surface, DeleteSurface> textSurface(TTF_RenderUTF8_Solid(font, text.data(), textColor));
    if (!textSurface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ttf_rendertext_solid failed %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
        return SDL_FPoint{};
    }

    // 创建纹理
    std::unique_ptr<SDL_Texture, DeleteTexture> textTexture(SDL_CreateTextureFromSurface(renderer, textSurface.get()));
    if (!textTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl_createtexturefromsurface failed %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
        return SDL_FPoint{};
    }

    // 获取纹理宽高
    int textWidth = textSurface->w * fontSize;
    int textHeight = textSurface->h * fontSize;

    // 设置渲染目标为窗口指定位置
    int windowWidth = Game::instance().get_window_width();
    int windowHeight = Game::instance().get_window_height();
    SDL_Rect renderQuad{};
    if (std::isnan(x))
    {
        renderQuad = {static_cast<int>(windowWidth / 2.f - textWidth / 2.f), static_cast<int>(windowHeight * posY - textHeight / 2.f), textWidth, textHeight};
    }
    else
    {
        renderQuad = {static_cast<int>(x), static_cast<int>(windowHeight * posY - textHeight / 2.f), textWidth, textHeight};
    }

    if (!is_left_align)
    {
        renderQuad.x = windowWidth - textWidth - 125;
    }

    // 输入模式下,调用SDL_SetTextInputRect设置输入区域
    if (is_input_mode)
    {
        SDL_Rect input_rect = {renderQuad.x, renderQuad.y + textHeight / 2, renderQuad.w, renderQuad.h};
        SDL_SetTextInputRect(&input_rect);
    }

    // 渲染纹理
    SDL_RenderCopy(renderer, textTexture.get(), nullptr, &renderQuad);

    // 返回渲染框右上角坐标
    return SDL_FPoint{static_cast<float>(renderQuad.x + renderQuad.w), static_cast<float>(renderQuad.y)};
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

static int count_utf8_characters(const std::string& str)
{
    int count{};
    if (str.empty())
    {
        return count;
    }
    int utf8_count{};
    for (const unsigned char c : str)
    {
        if ((c & 0b11000000) == 0b11000000)
        {
            ++utf8_count;
        }
        else if (!(c & 0b10000000))
        {
            ++count;
        }
    }
    count += utf8_count;
    return count;
}

SceneEnd::SceneEnd() {}

SceneEnd::~SceneEnd()
{
    // 停止背景音乐
    if (background_music_)
    {
        Mix_HaltMusic();
    }

    if (SDL_IsTextInputActive())
    {
        SDL_StopTextInput();
    }
}

void SceneEnd::init()
{
    // 初始化背景音乐
    background_music_ = Game::instance().getBackgroundMusic(MusicType::Title);
    if (background_music_)
    {
        Mix_PlayMusic(background_music_, -1);
    }

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
        SDL_Rect input_rect{200, 640, 0, 0};
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

            int input_name_length = count_utf8_characters(input_name);
            for (int i = 0; i < input_name_length - input_max; ++i)
            {
                removeUTF8_character(input_name);
            }
        }

        if (event.type == SDL_KEYDOWN)
        {
            // 处理回车键事件
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
            {
                is_input_mode = false;
                SDL_StopTextInput();
                if (!input_name.empty())
                {
                    size_t pos = input_name.find('*');
                    if (pos != std::string::npos)
                    {
                        input_name.erase(pos, 1);
                    }
                }

                if (input_name.empty())
                {
                    input_name = "Player";
                }
                Game::instance().addLeaderboardEntry(Game::instance().getFinalScore(), input_name);
                is_leaderboard = true;
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
        // 处理J键事件
        if (is_leaderboard)
        {
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_J)
            {
                Game::instance().changeScene(std::make_unique<SceneMain>());
                return;
            }
        }
    }
}

void SceneEnd::update(double delta_time)
{
    cursor_timer += delta_time;
    // 控制计时器在0 - 1s之间
    if (cursor_timer >= 1.f)
    {
        cursor_timer = 0.f;
    }
}

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
            SDL_FPoint pos = renderText(renderer, font, input_name, textColor, 0.7f, 0.5f, true);
            render_cursor(pos.x, pos.y, true);
        }
        else
        {
            if (cursor_timer >= display_threshold)
            {
                renderText(renderer, font, "_", textColor, 0.7f, 0.5f, true);
            }
            // render_cursor(292, 544, true);
        }
    }
    else if (is_leaderboard)
    {
        // "排行榜"
        renderText(renderer, font, "排行榜", textColor, 0.1f, 1.f, false);

        // 渲染排行榜排名
        if (!Game::instance().getLeaderboardEntries().empty())
        {
            int rank = 1;
            for (const auto& [score, name] : Game::instance().getLeaderboardEntries())
            {
                std::string rank_text = std::to_string(rank) + ". " + name;
                renderText(renderer, font, rank_text, textColor, 0.25f + rank * 0.05f, 0.5f, false, true, 75.f);
                std::string score_text = std::to_string(score);
                renderText(renderer, font, score_text, textColor, 0.25f + rank * 0.05f, 0.5f, false, false);
                rank++;
            }
        }
        else
        {
            renderText(renderer, font, "排行榜为空", textColor, 0.4f, 0.5f, false);
        }

        // "按J重新开始游戏"
        if (cursor_timer >= 0.4f)
        {
            renderText(renderer, font, "按J重新开始游戏", textColor, 0.8f, 0.5f, false);
        }
    }
}

void SceneEnd::clean() {}

SceneState SceneEnd::getState()
{
    return state;
}

void SceneEnd::render_cursor(float x, float y, bool is_flickering)
{
    auto renderer = Game::instance().getRenderer();
    auto font = Game::instance().getFont(FontType::Title);
    auto textColor = SDL_Color{255, 255, 255, 255};

    std::string text = "_";

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
    int textWidth = textSurface->w * 0.5f;
    int textHeight = textSurface->h * 0.5f;

    // 设置渲染目标为窗口指定位置
    int windowWidth = Game::instance().get_window_width();
    int windowHeight = Game::instance().get_window_height();
    SDL_Rect renderQuad = {x, y, textWidth, textHeight};

    if (is_flickering)
    {
        // static auto last = std::chrono::steady_clock::now();
        // auto now = std::chrono::steady_clock::now();
        // auto duration = std::chrono::duration<double>(now - last).count();
        //  if (duration >= 1.f)
        //  {
        //    last = now;
        //  }
        if (cursor_timer >= display_threshold)
        {
            SDL_RenderCopy(renderer, textTexture.get(), NULL, &renderQuad);
        }
    }
    else
    {
        SDL_RenderCopy(renderer, textTexture.get(), NULL, &renderQuad);
    }
}
