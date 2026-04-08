#include "scene_main.h"
#include "../game.h"
#include "../scene_main_object/enemy_bullet.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <memory>
#include <string>

SceneMain::SceneMain() {}

SceneMain::~SceneMain()
{
    EnemyBullet::shared_texture.reset();
}

void SceneMain::init() {}

void SceneMain::updatePauseTextLayout()
{
    std::string_view pause_text("游戏已暂停");
    std::unique_ptr<SDL_Texture, DeleteTexture> continue_button;
    continue_button.reset(
        IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/play_fill.png"));
    if (!continue_button)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load continue button: %s\n",
                     SDL_GetError());
        Game::instance().getIsRunning() = false;
    }
    int pause_text_w = 0;
    int pause_text_h = 0;
    int continue_button_w = 0;
    int continue_button_h = 0;

    TTF_SizeUTF8(Game::instance().getFont(), pause_text.data(), &pause_text_w, &pause_text_h);
    SDL_QueryTexture(continue_button.get(), nullptr, nullptr, &continue_button_w,
                     &continue_button_h);

    const int center_x = Game::instance().get_window_width() / 2;
    const int center_y = Game::instance().get_window_height() / 2;

    pause_text_rect = {center_x - pause_text_w / 2, center_y - pause_text_h - 10, pause_text_w,
                       pause_text_h};
    continue_button_rect = {center_x - continue_button_w / 2, center_y + 10, continue_button_w,
                            continue_button_h};
}

void SceneMain::handleEvent(SDL_Event& event)
{
    // 处理暂停按钮点击事件
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        int x = event.button.x;
        int y = event.button.y;
        if (!is_paused && pauseButton.isInside(x, y))
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Pause button clicked");
            is_paused = true;
            updatePauseTextLayout();
        }
        else if (is_paused)
        {
            bool inside_continue = (x >= continue_button_rect.x) &&
                                   (x <= continue_button_rect.x + continue_button_rect.w) &&
                                   (y >= continue_button_rect.y) &&
                                   (y <= continue_button_rect.y + continue_button_rect.h);
            if (inside_continue)
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Continue clicked");
                is_paused = false;
            }
        }
    }
}
void SceneMain::update(double delta_time)
{
    if (is_paused)
    {
        return;
    }
    player.keyBoardControl(delta_time);
    player.updateBullets(delta_time);

    updateEnemy(delta_time);
    updateEnemyBullets(delta_time);
}

void SceneMain::render()
{
    // 测试
    // std::unique_ptr<SDL_Texture, DeleteTexture> background_texture(
    //     IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/bg.png"));
    // if (!background_texture)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load background img failed %s\n", IMG_GetError());
    //     Game::instance().getIsRunning() = false;
    // }
    // SDL_Rect bg_rect{0, 0, 600, 800};
    // SDL_RenderCopy(Game::instance().getRenderer(), background_texture.get(), NULL, &bg_rect);
    // SDL_SetTextureColorMod(player.texture.get(), 100, 100, 100);

    player.renderBullets(Game::instance().getRenderer());
    player.render(Game::instance().getRenderer());
    pauseButton.render(Game::instance().getRenderer());
    renderEnemyBullets(Game::instance().getRenderer());
    renderEnemy(Game::instance().getRenderer());

    if (is_paused)
    {
        SDL_Renderer* renderer = Game::instance().getRenderer();

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
        SDL_Rect overlay{0, 0, Game::instance().get_window_width(),
                         Game::instance().get_window_height()};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Color white{255, 255, 255, 255};
        std::unique_ptr<SDL_Surface, DeleteSurface> pause_text_surface;
        pause_text_surface.reset(
            TTF_RenderUTF8_Blended(Game::instance().getFont(), "游戏已暂停", white));
        std::unique_ptr<SDL_Texture, DeleteTexture> pause_text_texture(
            SDL_CreateTextureFromSurface(renderer, pause_text_surface.get()));
        SDL_RenderCopy(renderer, pause_text_texture.get(), NULL, &pause_text_rect);

        std::unique_ptr<SDL_Texture, DeleteTexture> continue_button;
        continue_button.reset(
            IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/play_fill.png"));
        if (!continue_button)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load continue button: %s\n",
                         SDL_GetError());
            Game::instance().getIsRunning() = false;
        }
        SDL_RenderCopy(renderer, continue_button.get(), NULL, &continue_button_rect);
    }
}

void SceneMain::clean() {}

void SceneMain::generateEnemy()
{
    using namespace std::chrono_literals;
    static auto last_generation_time = std::chrono::steady_clock::now();
    static std::chrono::nanoseconds generation_cooldown{900ms}; // 生成敌人间隔
    auto now = std::chrono::steady_clock::now();
    if (now - last_generation_time >= generation_cooldown)
    {
        last_generation_time = now;
        enemies.emplace_back(true, enemy_template);
    }
}

void SceneMain::updateEnemy(double delta_time)
{ // 生成敌人
    generateEnemy();

    // 更新敌人位置
    for (auto& enemy : enemies)
    {
        enemy.update(delta_time);
    }

    // 移除已飞出屏幕的敌人
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](Enemy& enemy)
                       { return enemy.getPosition().y > Game::instance().get_window_height(); }),
        enemies.end());
}

void SceneMain::renderEnemy(SDL_Renderer* renderer)
{
    for (auto& enemy : enemies)
    {
        enemy.render(renderer);
    }
}

void SceneMain::updateEnemyBullets(double delta_time)
{
    // 发射子弹
    for (auto& enemy : enemies)
    {
        auto now = std::chrono::steady_clock::now();
        if (now - enemy.getLastShootTime() >= enemy.getShootCooldown())
        {
            enemy.getLastShootTime() = now;
            enemy_bullets.emplace_back(enemy.getPosition().x + enemy.getWidth() / 2,
                                       enemy.getPosition().y + enemy.getHeight() / 2,
                                       enemy_bullet_template);
            enemy_bullets.back().setDirection(player.getPosition(), player.getWidth(),
                                              player.getHeight());
        }
    }

    // 更新子弹位置（在循环外，每帧只更新一次）
    for (auto& bullet : enemy_bullets)
    {
        bullet.update(delta_time);
    }

    // 移除已飞出屏幕的子弹
    enemy_bullets.remove_if(
        [](EnemyBullet& bullet)
        {
            auto& game = Game::instance();
            float bullet_x = bullet.getPosition().x;
            float bullet_y = bullet.getPosition().y;
            int bullet_width = bullet.getWidth();
            int bullet_height = bullet.getHeight();

            return bullet_y > game.get_window_height() ||
                   bullet_y < -static_cast<float>(bullet_height) ||
                   bullet_x > game.get_window_width() ||
                   bullet_x < -static_cast<float>(bullet_width);
        });
}

void SceneMain::renderEnemyBullets(SDL_Renderer* renderer)
{
    // 渲染子弹
    for (auto& bullet : enemy_bullets)
    {
        bullet.render(renderer);
    }
}
