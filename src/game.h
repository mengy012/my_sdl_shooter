#pragma once

#include "./background.h"
#include "./font_manager/font_manager.h"
#include "./music_manager/music_manager.h"
#include "./scene/scene.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <map>
#include <memory>
#include <random>
#include <string>

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

struct DeleteTexture // 纹理删除器
{
    void operator()(SDL_Texture* texture) const noexcept
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }
};

struct DeleteSurface
{
    void operator()(SDL_Surface* surface) const noexcept
    {
        if (surface)
        {
            SDL_FreeSurface(surface);
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

    Game& init();
    void run();
    void changeScene(std::unique_ptr<Scene> Scene);
    void clean();

    void handleEvent(SDL_Event& event);
    void update();
    void render();

    SDL_Renderer* getRenderer() const;
    TTF_Font* getFont(FontType font_type);
    bool& getIsRunning();
    double getCurrentFps() const;
    int get_window_width() const;
    int get_window_height() const;

    // 设置日志种类级别
    void setLogCategoryPriority(SDL_LogCategory category, SDL_LogPriority priority);

    // 获取随机数种子
    std::random_device::result_type getRandomSeed() const;

    // 获取0-1之间的随机浮点数
    float getRandomFloat() const noexcept;

    // gets
    // 获取音效
    Mix_Chunk* getChunk(ChunkType type);
    // 获取背景音乐
    Mix_Music* getBackgroundMusic(MusicType type);
    // 获取最终游戏得分
    int getFinalScore() const;
    // 获取排行榜
    const std::multimap<int, std::string, std::greater<int>>& getLeaderboardEntries() const;

    // sets
    // 设置最终游戏得分
    void setFinalScore(int score);

    // 添加排行榜条目
    void addLeaderboardEntry(int score, std::string name);

  private:
    Game();
    ~Game();

    // 当前游戏状态
    SceneState current_state{SceneState::Title};
    bool is_running{true};
    std::unique_ptr<Scene> current_scene;                   // 当前场景
    std::unique_ptr<SDL_Window, DeleteWindow> window;       // 窗口
    std::unique_ptr<SDL_Renderer, DeleteRenderer> renderer; // 渲染器
    FontManager* font_manager_;                              // 字体管理器
    int window_width = 600;                                 // 窗口宽
    int window_height = 800;                                // 窗口高

    int fps = 320;                       // 游戏帧数
    std::chrono::nanoseconds frame_time; // 游戏帧数对应帧时间
    double current_fps;                  // 实时游戏帧数

    std::chrono::steady_clock::time_point frame_start; // 帧起始时刻
    std::chrono::nanoseconds delta_time;               // 实际每帧用时,控制飞机速度

    std::random_device::result_type random_seed{std::random_device{}()}; // 随机数种子

    // 远处背景
    Background* far_stars_;
    // 近处背景
    Background* near_stars_;

    // 音乐管理器
    MusicManager* music_manager_;

    // 最终游戏得分
    int final_score_{0};

    // 游戏排行榜
    std::multimap<int, std::string, std::greater<int>> leaderboard_;

    // 加载排行榜
    void loadLeaderboard();
    // 保存排行榜
    void saveLeaderboard();
};