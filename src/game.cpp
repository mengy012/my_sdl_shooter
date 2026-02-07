#include "game.h"
#include "./scene/scene_main.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

using namespace std::chrono_literals;

Game::Game() = default;

Game& Game::instance()
{
    static Game game;
    return game;
}

Game::~Game()
{
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::run()
{ // 测试
    // Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    // Mix_Music* music = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    // Mix_PlayMusic(music, -1);

    uint32_t fps_count{0};
    auto fps_count_time_start = std::chrono::steady_clock::now();
    while (is_running)
    {
        frame_start = std::chrono::steady_clock::now();

        SDL_Event event;
        handleEvent(event); // 事件处理

        update(); // 游戏逻辑更新

        render(); // 游戏画面更新
        fps_count++;
        auto frame_end = std::chrono::steady_clock::now();

        // 控制帧率
        auto elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
        if (elapsed < frame_time)
        {
            auto sleep_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - elapsed);
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "sleep %d ms", sleep_time.count());
            SDL_Delay(static_cast<uint32_t>(sleep_time.count()));
        }

        auto real_frame_end = std::chrono::steady_clock::now();
        delta_time =
            std::chrono::duration_cast<std::chrono::microseconds>(real_frame_end - frame_start);
        // 记录每秒fps
        auto fps_count_elapsed = real_frame_end - fps_count_time_start;
        if (fps_count_elapsed >= 1s)
        {
            current_fps = fps_count / std::chrono::duration_cast<std::chrono::duration<double>>(
                                          fps_count_elapsed)
                                          .count();
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "fps: %f", current_fps);
            fps_count_time_start = real_frame_end;
            fps_count = 0;
        }
    }
}

void Game::changeScene(std::unique_ptr<Scene> Scene)
{
    if (current_scene)
    {
        current_scene->clean();
    }

    current_scene = std::move(Scene);

    if (current_scene)
    {
        current_scene->init();
    }
}

void Game::handleEvent(SDL_Event& event)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            // 处理退出事件
            is_running = false;
            break;
        case SDL_KEYDOWN:
            // 处理键盘按下事件
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                is_running = false;
            }
            // else if (event.key.keysym.sym == SDLK_d)
            // { // 测试
            //     SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "press d");
            // }
            break;
        }
        current_scene->handleEvent(event);
    }
}

void Game::update()
{
    current_scene->update(
        std::chrono::duration_cast<std::chrono::duration<double>>(delta_time).count());
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());
    // 测试
    SDL_Texture* img_texture = IMG_LoadTexture(renderer.get(), "../assets/image/pause.png");
    SDL_Rect img{window_width - 48, 0, 48, 48};
    SDL_RenderCopy(renderer.get(), img_texture, NULL, &img);

    SDL_Color white{255, 255, 255, 255};
    std::string cur_fps_text;
    {
        cur_fps_text += "fps: " + std::to_string(current_fps);
        auto pos = cur_fps_text.find('.');
        pos += 3;
        cur_fps_text = cur_fps_text.substr(0, pos);
    }
    SDL_Surface* surface_text{TTF_RenderUTF8_Blended(font.get(), cur_fps_text.c_str(), white)};
    SDL_Rect text{0, 0, surface_text->w, surface_text->h};
    SDL_Texture* texture_text{SDL_CreateTextureFromSurface(renderer.get(), surface_text)};
    SDL_RenderCopy(renderer.get(), texture_text, NULL, &text);

    current_scene->render();
    SDL_RenderPresent(renderer.get());
}

SDL_Renderer* Game::getRenderer() const
{
    return renderer.get();
}

TTF_Font* Game::getFont() const
{
    return font.get();
}

bool& Game::getIsRunning()
{
    return is_running;
}

int Game::get_window_width() const
{
    return window_width;
}

int Game::get_window_height() const
{
    return window_height;
}

void Game::setLogCategoryPriority(SDL_LogCategory category, SDL_LogPriority priority)
{
    SDL_LogSetPriority(category, priority);
}

// 资源已使用智能指针管理,可能删除
void Game::clean() {}

Game& Game::init()
{ // sdl初始化
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl init failed %s\n", SDL_GetError());
        is_running = false;
    }
    // 创建窗口
    window.reset(SDL_CreateWindow("SDL_Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  window_width, window_height, SDL_WINDOW_RESIZABLE));
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl window init failed %s\n", SDL_GetError());
        is_running = false;
    }
    // 创建渲染器
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl renderer init failed %s\n", SDL_GetError());
        is_running = false;
    }

    // 初始化sdl_image
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl image init failed %s\n", SDL_GetError());
        is_running = false;
    }
    // 初始化sdl_mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl mixer init failed %s\n", SDL_GetError());
        is_running = false;
    }
    // 初始化sdl_ttf
    if (TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl ttf init failed %s\n", SDL_GetError());
        is_running = false;
    }
    else
    {
        font.reset(TTF_OpenFont("../assets/font/VonwaonBitmap-12px.ttf", 24));
        if (!font)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ttf_openfont failed %s\n", SDL_GetError());
            is_running = false;
        }
    }
    // 创建场景
    current_scene = std::make_unique<SceneMain>();
    current_scene->init();

    // 计算游戏帧数对应帧时间
    frame_time = std::chrono::duration_cast<std::chrono::microseconds>(1s) / fps;

    return *this;
}