#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <chrono>
#include <iostream>
#include <string>

using MyClock = std::chrono::steady_clock;

void UpdateFpsText(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture*& texture, SDL_Rect& rect,
                   double fps)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Color black{0, 0, 0, 255};
    std::string text = "FPS: " + std::to_string(static_cast<int>(fps));

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), black);

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    rect = {10, 10, surface->w, surface->h};

    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[])
{
    std::cout << "hello_world!" << std::endl;

    // sdl初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL init failed\n";
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("hello world", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // sdl_image初始化
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        std::cerr << "img init failed\n";
        return 1;
    }
    // 加载图片
    SDL_Texture* texture = IMG_LoadTexture(renderer, "../assets/image/bonus_life.png");

    // sdl_mix初始化
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        std::cerr << "mix init failed\n";
        return 1;
    }
    // 打开音频设备
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // 载入音频
    Mix_Music* music = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    // 播放音频
    Mix_PlayMusic(music, -1);

    // sdl_ttf初始化
    if (TTF_Init() != 0)
    {
        std::cerr << "ttf init failed\n";
        return 1;
    }
    // 载入字体
    TTF_Font* font = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 24);
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "hello_world 你好世界", color);
    // 创建纹理
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, surface);
    // todo

    // 计算每秒帧数
    auto fps_start = MyClock::now();
    size_t fps_count{0};
    // 用于显示每秒帧数的texture,rect
    SDL_Texture* fps_texture{NULL};
    SDL_Rect fps_showRect{};

    // 循环运行状态
    bool is_running{true};
    // 绘制循环
    while (is_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
        }
        
        // 清屏
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        // 画一个长方形
        SDL_Rect rect{100, 100, 200, 200};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        // 画图片
        SDL_Rect img{200, 200, 200, 200};
        SDL_RenderCopy(renderer, texture, NULL, &img);
        // 绘制文字
        SDL_Rect text{300, 300, surface->w, surface->h};
        SDL_RenderCopy(renderer, text_texture, NULL, &text);

        fps_count++;
        auto now = MyClock::now();
        std::chrono::duration<double> elapsed = now - fps_start;
        if (elapsed.count() >= 1.0)
        {
            double fps = fps_count / elapsed.count();
            std::cout << "fps: " << fps << std::endl;
            UpdateFpsText(renderer, font, fps_texture, fps_showRect, fps);
            fps_count = 0;
            fps_start = now;
        }
        if (fps_texture)
        {
            SDL_RenderCopy(renderer, fps_texture, NULL, &fps_showRect);
        }

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 清理图片资源
    SDL_DestroyTexture(texture);
    IMG_Quit();

    // 清理音频资源
    Mix_FreeMusic(music);
    // 关闭音频设备
    Mix_CloseAudio();
    Mix_Quit();

    // 清理字体资源
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
    TTF_Quit();

    // 销毁渲染器
    SDL_DestroyRenderer(renderer);
    // 销毁窗口
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}