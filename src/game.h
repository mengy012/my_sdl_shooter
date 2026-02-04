#pragma once

#include "./scene/scene.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <memory>

class DeleteWindow // sdl_window删除器
{
  public:
    void operator()(SDL_Window* window) const noexcept
    {
        if (window)
        {
            SDL_DestroyWindow(window);
        }
    }
};

class DeleteRenderer // sdl_renderer删除器
{
  public:
    void operator()(SDL_Renderer* renderer) const noexcept
    {
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
    }
};

class DeleteFont // ttf_font删除器
{
  public:
    void operator()(TTF_Font* font) const noexcept
    {
        if (font)
        {
            TTF_CloseFont(font);
        }
    }
};

class Game
{
  public:
    static Game& instance(); // 单例模式
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    void init();
    void run();
    void changeScene(std::unique_ptr<Scene> Scene);
    void clean();

    void handleEvent(SDL_Event& event);
    void update();
    void render();

    SDL_Renderer* getRenderer() const;
    bool& getIsRunning();
    int get_window_width() const;
    int get_window_height() const;

  private:
    Game();
    ~Game();

    bool is_running{true};
    std::unique_ptr<Scene> current_scene;                   // 当前场景
    std::unique_ptr<SDL_Window, DeleteWindow> window;       // 窗口
    std::unique_ptr<SDL_Renderer, DeleteRenderer> renderer; // 渲染器
    std::unique_ptr<TTF_Font, DeleteFont> font;             // 字体
    int window_width = 600;                                 // 窗口宽
    int window_height = 800;                                // 窗口高

    int fps = 160;                        // 游戏帧数
    std::chrono::microseconds frame_time; // 游戏帧数对应帧时间
    double current_fps;                   // 实时游戏帧数

    std::chrono::steady_clock::time_point frame_start; // 帧起始时刻
    std::chrono::microseconds delta_time;              // 实际每帧用时,控制飞机速度
};
