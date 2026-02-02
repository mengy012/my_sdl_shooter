#include "game.h"
#include "./scene/scene_main.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    Mix_Music* music = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    Mix_PlayMusic(music, -1);

    while (is_running)
    {
        SDL_Event event;
        handleEvent(event); // 事件处理

        update(); // 游戏逻辑更新

        render(); // 游戏画面更新
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
            else if (event.key.keysym.sym == SDLK_d)
            { // 测试
                SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "press d");
            }
            break;
        }
        current_scene->handleEvent(event);
    }
}

void Game::update()
{
    current_scene->update();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());
    // 测试
    SDL_Texture* img_texture = IMG_LoadTexture(renderer.get(), "../assets/image/bonus_life.png");
    SDL_Rect img{0, 0, 100, 100};
    SDL_RenderCopy(renderer.get(), img_texture, NULL, &img);

    SDL_Color white{255, 255, 255, 255};
    SDL_Surface* surface_text{TTF_RenderUTF8_Blended(font.get(), "hello,世界", white)};
    SDL_Rect text{200, 200, surface_text->w, surface_text->h};
    SDL_Texture* texture_text{SDL_CreateTextureFromSurface(renderer.get(), surface_text)};
    SDL_RenderCopy(renderer.get(), texture_text, NULL, &text);

    current_scene->render();
    SDL_RenderPresent(renderer.get());
}

SDL_Renderer* Game::getRenderer() const
{
    return renderer.get();
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

// 资源已使用智能指针管理,可能删除
void Game::clean() {}

void Game::init()
{ // sdl初始化
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl init failed %s\n", SDL_GetError());
        is_running = false;
        return;
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
}
