#include "game.h"
#include "./scene/scene_main.h"
#include "./scene/scene_title.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <charconv>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void Game::setFinalScore(int score)
{

    final_score_ = score;
}

void Game::addLeaderboardEntry(int score, std::string name)
{
    leaderboard_.emplace(score, name);
    if (leaderboard_.size() > 8)
    {
        leaderboard_.erase(--leaderboard_.end());
    }
}

Game::Game() = default;

Game& Game::instance()
{
    static Game game;
    return game;
}

Game::~Game()
{
    // 保存排行榜
    saveLeaderboard();

    // sdl资源释放一定要在sdl_quit之前,否则可能会崩溃(坑死我了😡)
    current_scene.reset();
    // font.reset();
    renderer.reset();
    window.reset();

    far_stars_ = Background();
    near_stars_ = Background();
    music_manager_.~MusicManager();
    font_manager_.~FontManager();

    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::loadLeaderboard()
{
    std::hash<std::string> hasher;
    std::ifstream file("../../assets/leaderboard.data", std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string content(file_size, 0);
        file.read(content.data(), file_size);

        std::string last_hash_text = content.substr(content.find('*') + 1);
        size_t last_hash{};
        std::from_chars(last_hash_text.c_str(), last_hash_text.c_str() + last_hash_text.size(), last_hash);
        size_t now_hash = hasher(content.substr(0, content.find('*')));

        if (last_hash == now_hash)
        {
            SDL_Log("hash ok");
            std::istringstream iss(content.substr(0, content.find('*')));
            // std::cout << content.substr(0, content.find('*')) << std::endl;
            int score{};
            std::string name;
            while (iss >> score >> name)
            {
                // std::cout << score << name << std::endl;
                addLeaderboardEntry(score, name);
            }
        }
        // std::cout << last_hash << '\n' << now_hash << std::endl;
        // std::cout << content << std::endl;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load leaderboard file: %s", SDL_GetError());
    }
}

void Game::saveLeaderboard()
{
    std::ofstream file("../../assets/leaderboard.data", std::ios::binary);
    if (file)
    {
        if (leaderboard_.empty())
        {
            return;
        }

        std::ostringstream oss;
        for (const auto& [score, name] : leaderboard_)
        {
            oss << score << " " << name << std::endl;
        }

        std::string context(oss.str());
        std::hash<std::string> hasher;
        std::string hash = std::to_string(hasher(context));

        file << context << "*" << hash;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to save leaderboard file: %s", SDL_GetError());
    }
}

void Game::run()
{ // 测试
    // Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    // Mix_Music* music = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    // Mix_PlayMusic(music, -1);

    uint32_t fps_count{0};
    auto fps_count_time_start = std::chrono::steady_clock::now();
    auto next_frame_time_point = std::chrono::steady_clock::now();
    while (is_running)
    {
        frame_start = std::chrono::steady_clock::now();

        SDL_Event event;
        handleEvent(event); // 事件处理

        update(); // 游戏逻辑更新

        render(); // 游戏画面更新
        fps_count++;
        auto frame_end = std::chrono::steady_clock::now();

        // 控制帧率: 先粗睡眠,后精细等待,减少SDL_Delay误差
        next_frame_time_point += frame_time;
        if (frame_end > next_frame_time_point)
        {
            next_frame_time_point = frame_end;
        }

        while (true)
        {
            auto now = std::chrono::steady_clock::now();
            auto remaining = next_frame_time_point - now;
            if (remaining <= 0ns)
            {
                break;
            }

            if (remaining > 2ms)
            {
                auto sleep_ms = std::chrono::duration_cast<std::chrono::milliseconds>(remaining - 1ms);
                SDL_Delay(static_cast<uint32_t>(sleep_ms.count()));
            }
            else
            {
                std::this_thread::yield();
            }
        }

        auto real_frame_end = std::chrono::steady_clock::now();

        // 计算每帧实际用时,控制对象移动速度
        delta_time = real_frame_end - frame_start;

        // 记录每秒fps
        auto fps_count_elapsed = real_frame_end - fps_count_time_start;
        if (fps_count_elapsed >= 1s)
        {
            current_fps = fps_count / std::chrono::duration_cast<std::chrono::duration<double>>(fps_count_elapsed).count();
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
    current_state = current_scene->getState();
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
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && current_state == SceneState::Main)
            {
                changeScene(std::make_unique<SceneTitle>());
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_J && current_state == SceneState::Title)
            {
                changeScene(std::make_unique<SceneMain>());
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && current_state == SceneState::Title)
            {
                is_running = false;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && current_state == SceneState::End)
            {
                changeScene(std::make_unique<SceneTitle>());
            }
            break;
        }
        current_scene->handleEvent(event);
    }
}

void Game::update()
{
    // 更新背景
    far_stars_.update(std::chrono::duration_cast<std::chrono::duration<double>>(delta_time).count());
    near_stars_.update(std::chrono::duration_cast<std::chrono::duration<double>>(delta_time).count());

    // 更新场景
    current_scene->update(std::chrono::duration_cast<std::chrono::duration<double>>(delta_time).count());
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());
    // 测试
    // SDL_Texture* img_texture = IMG_LoadTexture(renderer.get(), "../../assets/image/pause.png");
    // SDL_Rect img{window_width - 48, 0, 48, 48};
    // SDL_RenderCopy(renderer.get(), img_texture, NULL, &img);

    // 渲染背景
    far_stars_.render();
    near_stars_.render();

    current_scene->render();
    SDL_RenderPresent(renderer.get());
}

SDL_Renderer* Game::getRenderer() const
{
    return renderer.get();
}

TTF_Font* Game::getFont(FontType font_type)
{
    return font_manager_.getFont(font_type);
}

bool& Game::getIsRunning()
{
    return is_running;
}

double Game::getCurrentFps() const
{
    return current_fps;
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

std::random_device::result_type Game::getRandomSeed() const
{
    return random_seed;
}

float Game::getRandomFloat() const noexcept
{
    static std::mt19937 rng(random_seed);
    static std::uniform_real_distribution<float> dist(0.f, 1.f);
    return dist(rng);
}

Mix_Chunk* Game::getChunk(ChunkType type)
{
    return music_manager_.getChunk(type);
}

Mix_Music* Game::getBackgroundMusic(MusicType type)
{
    return music_manager_.getBackgroundMusic(type);
}

int Game::getFinalScore() const
{
    return final_score_;
}

const std::multimap<int, std::string, std::greater<int>>& Game::getLeaderboardEntries() const
{
    return leaderboard_;
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
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // 创建窗口
    window.reset(SDL_CreateWindow("SDL_Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_ALLOW_HIGHDPI));
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
    // 初始化sdl_audio
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl audio init failed %s\n", SDL_GetError());
        is_running = false;
    }
    // 设置音效channel数量
    Mix_AllocateChannels(32);
    // 设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    // 设置所有音效音量
    Mix_Volume(-1, MIX_MAX_VOLUME / 2);

    // 初始化sdl_ttf
    if (TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "sdl ttf init failed %s\n", SDL_GetError());
        is_running = false;
    }
    // 创建场景
    current_scene = std::make_unique<SceneTitle>();
    current_scene->init();
    current_state = current_scene->getState();

    // 计算游戏帧数对应帧时间
    frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(1s) / fps;

    // 初始化背景
    far_stars_ = Background(IMG_LoadTexture(renderer.get(), "../../assets/image/Stars-B.png"), 20.f);
    near_stars_ = Background(IMG_LoadTexture(renderer.get(), "../../assets/image/Stars-A.png"));

    // 加载排行榜
    loadLeaderboard();

    return *this;
}