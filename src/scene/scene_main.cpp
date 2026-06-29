#include "scene_main.h"
#include "../game.h"
#include "../scene_main_object/enemy_bullet.h"
#include "../scene_main_object/item_life_restoring.h"
#include "../music_manager/music_manager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <memory>
#include <string>

SceneMain::SceneMain() {}

SceneMain::~SceneMain()
{
    Explosion::GetTexture(true);
    Mix_HaltMusic();
}

void SceneMain::init()
{
    // 播放背景音乐
    Mix_PlayMusic(music_manager.getBackgroundMusic(), -1);

    // 加载玩家生命值纹理
    player_health_texture.reset(IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/heart_fill (1).png"));
    if (!player_health_texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load player health texture: %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
    }
}

void SceneMain::updatePauseTextLayout()
{
    std::string_view pause_text("游戏已暂停");
    std::unique_ptr<SDL_Texture, DeleteTexture> continue_button;
    continue_button.reset(IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/play_fill.png"));
    if (!continue_button)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load continue button: %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
    }
    int pause_text_w = 0;
    int pause_text_h = 0;
    int continue_button_w = 0;
    int continue_button_h = 0;

    TTF_SizeUTF8(Game::instance().getFont(), pause_text.data(), &pause_text_w, &pause_text_h);
    SDL_QueryTexture(continue_button.get(), nullptr, nullptr, &continue_button_w, &continue_button_h);

    const int center_x = Game::instance().get_window_width() / 2;
    const int center_y = Game::instance().get_window_height() / 2;

    pause_text_rect = {center_x - pause_text_w / 2, center_y - pause_text_h - 10, pause_text_w, pause_text_h};
    continue_button_rect = {center_x - continue_button_w / 2, center_y + 10, continue_button_w, continue_button_h};
}

void SceneMain::renderPauseText(SDL_Renderer* renderer)
{

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_Rect overlay{0, 0, Game::instance().get_window_width(), Game::instance().get_window_height()};
    SDL_RenderFillRect(renderer, &overlay);

    SDL_Color white{255, 255, 255, 255};
    std::unique_ptr<SDL_Surface, DeleteSurface> pause_text_surface;
    pause_text_surface.reset(TTF_RenderUTF8_Blended(Game::instance().getFont(), "游戏已暂停", white));
    std::unique_ptr<SDL_Texture, DeleteTexture> pause_text_texture(SDL_CreateTextureFromSurface(renderer, pause_text_surface.get()));
    SDL_RenderCopy(renderer, pause_text_texture.get(), NULL, &pause_text_rect);

    std::unique_ptr<SDL_Texture, DeleteTexture> continue_button;
    continue_button.reset(IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/image/play_fill.png"));
    if (!continue_button)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load continue button: %s\n", SDL_GetError());
        Game::instance().getIsRunning() = false;
    }
    SDL_RenderCopy(renderer, continue_button.get(), NULL, &continue_button_rect);
}

void SceneMain::renderFps(SDL_Renderer* renderer)
{
    SDL_Color white{255, 255, 255, 255};
    std::string cur_fps_text;
    {
        cur_fps_text += "fps: " + std::to_string(Game::instance().getCurrentFps());
        auto pos = cur_fps_text.find('.');
        pos += 3;
        cur_fps_text = cur_fps_text.substr(0, pos);
    }
    std::unique_ptr<SDL_Surface, DeleteSurface> surface_text(TTF_RenderUTF8_Blended(Game::instance().getFont(), cur_fps_text.c_str(), white));
    if (surface_text)
    {
        SDL_Rect text{0, 0, surface_text->w, surface_text->h};
        std::unique_ptr<SDL_Texture, DeleteTexture> texture_text(SDL_CreateTextureFromSurface(renderer, surface_text.get()));
        if (texture_text)
        {
            SDL_RenderCopy(renderer, texture_text.get(), NULL, &text);
        }
    }
}

void SceneMain::renderPlayerHealth(SDL_Renderer* renderer)
{
    SDL_Texture* texture = player_health_texture.get();

    int health_max = player.getHealthMax();
    int health_current = player.getHealth();

    // 渲染玩家最大生命值
    SDL_SetTextureColorMod(texture, 100, 100, 100);
    SDL_Rect health_rect{0, 32, 32, 32};
    for (int i = 0; i < health_max; i++)
    {
        SDL_RenderCopy(renderer, texture, NULL, &health_rect);
        health_rect.x += 32 + 5; // 每个心之间间隔5像素
    }

    // 渲染玩家当前生命值
    SDL_SetTextureColorMod(texture, 255, 255, 255);
    health_rect = SDL_Rect{0, 32, 32, 32};
    for (int i = 0; i < health_current; i++)
    {
        SDL_RenderCopy(renderer, texture, NULL, &health_rect);
        health_rect.x += 32 + 5; // 每个心之间间隔5像素
    }
}

void SceneMain::renderScore(SDL_Renderer* renderer)
{
    // 字体
    TTF_Font* font = Game::instance().getFont();
    // 窗口宽度
    int window_width = Game::instance().get_window_width();
    // 窗口高度
    int window_height = Game::instance().get_window_height();

    // 渲染得分
    SDL_Color white{255, 255, 255, 255};
    std::string score_text;
    score_text += "Score:" + std::to_string(score);
    std::unique_ptr<SDL_Surface, DeleteSurface> surface_text(TTF_RenderUTF8_Blended(font, score_text.c_str(), white));
    if (surface_text)
    {
        SDL_Rect text{window_width - surface_text->w, 48, surface_text->w, surface_text->h};
        std::unique_ptr<SDL_Texture, DeleteTexture> texture_text(SDL_CreateTextureFromSurface(renderer, surface_text.get()));
        if (texture_text)
        {
            SDL_RenderCopy(renderer, texture_text.get(), NULL, &text);
        }
    }
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
            bool inside_continue =
                (x >= continue_button_rect.x) && (x <= continue_button_rect.x + continue_button_rect.w) && (y >= continue_button_rect.y) && (y <= continue_button_rect.y + continue_button_rect.h);
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
    if (!player.getIsLive())
    {
        playerExplode();
    }
    else
    {
        player.keyBoardControl(delta_time, music_manager);
        player.updateBullets(delta_time, enemies, music_manager);
        player.update(enemies, enemy_bullets, items, music_manager, score);

        updateEnemy(delta_time);
        updateEnemyBullets(delta_time);
        updateItems(delta_time);
    }
    updateExplosions(delta_time);
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

    if (player.getIsLive()) // 玩家存活,渲染玩家和敌人
    {
        player.renderBullets(Game::instance().getRenderer());
        player.render(Game::instance().getRenderer());
        renderEnemyBullets(Game::instance().getRenderer());
        renderEnemy(Game::instance().getRenderer());
        renderItems(Game::instance().getRenderer());
    }

    renderExplosions(Game::instance().getRenderer());

    pauseButton.render(Game::instance().getRenderer());

    if (is_paused)
    {
        // 渲染暂停文本
        renderPauseText(Game::instance().getRenderer());
    }

    // 绘制帧数
    renderFps(Game::instance().getRenderer());
    // 绘制玩家生命值
    renderPlayerHealth(Game::instance().getRenderer());
    // 绘制玩家得分
    renderScore(Game::instance().getRenderer());
}

void SceneMain::clean() {}

SceneState SceneMain::getState()
{
    return state;
}

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
{
    // 生成敌人
    generateEnemy();
    // 更新敌人位置
    for (auto& enemy : enemies)
    {
        enemy.update(delta_time);
    }

    // 找出血量<=0的敌人，并标记为已销毁
    for (auto& enemy : enemies)
    {
        if (enemy.getHealth() <= 0)
        {
            enemy.getIsDestroyed() = true;
        }
    }
    // 移除已销毁的敌人
    for (auto enemy = enemies.begin(); enemy != enemies.end();)
    {
        if (enemy->getIsDestroyed())
        {
            enemyExplode(*enemy);
            // 增加得分
            score += 20;
            // 播放敌人爆炸音效
            Mix_PlayChannel(-1, music_manager.getChunk(ChunkType::Effect_enemy_explode), 0);
            // 生成物品
            {
                if (Game::instance().getRandomFloat() > 0.4f)
                {
                    // 生成物品
                    generateItem(*enemy);
                }
            }
            enemy = enemies.erase(enemy);
        }
        else
        {
            enemy = std::next(enemy);
        }
    }
    // 移除已飞出屏幕的敌人
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& enemy) { return enemy.getPosition().y > Game::instance().get_window_height(); }), enemies.end());
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
            enemy_bullets.emplace_back(enemy.getPosition().x + enemy.getWidth() / 2, enemy.getPosition().y + enemy.getHeight() / 2, enemy_bullet_template);
            enemy_bullets.back().setDirection(player.getPosition(), player.getWidth(), player.getHeight());
            // 播放敌人射击音效
            Mix_PlayChannel(-1, music_manager.getChunk(ChunkType::Effect_enemy_shoot), 0);
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

            return bullet_y > game.get_window_height() || bullet_y < -static_cast<float>(bullet_height) || bullet_x > game.get_window_width() || bullet_x < -static_cast<float>(bullet_width);
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

void SceneMain::enemyExplode(Enemy& enemy)
{
    explosions.emplace_back(enemy.getPosition().x, enemy.getPosition().y, enemy.getWidth(), enemy.getHeight(), std::chrono::steady_clock::now());
}

void SceneMain::playerExplode()
{
    static bool exploded = false; // 玩家是否爆炸过
    if (!exploded)
    {
        explosions.emplace_back(player.getPosition().x, player.getPosition().y, player.getWidth(), player.getHeight(), std::chrono::steady_clock::now());
        // 播放玩家爆炸音效
        Mix_PlayChannel(-1, music_manager.getChunk(ChunkType::Effect_player_explode), 0);

        exploded = true;
    }
}

void SceneMain::updateExplosions(double delta_time)
{
    for (auto& explosion : explosions)
    {
        explosion.update(delta_time);
    }
    // 移除已完成的爆炸效果
    explosions.remove_if([](Explosion& explosion) { return explosion.getIsFinished(); });
}

void SceneMain::renderExplosions(SDL_Renderer* renderer)
{
    for (auto& explosion : explosions)
    {
        explosion.render(renderer);
    }
}

void SceneMain::generateItem(Enemy& enemy)
{
    // 目前只生成生命
    ItemType item_type = ItemType::Life;

    switch (item_type)
    {
    case ItemType::Life:
        items.emplace_back(std::make_unique<ItemLifeRestoring>(enemy.getPosition().x, enemy.getPosition().y, item_texture_manager.getTexture(item_type)));
        break;
    case ItemType::Shield:
        break;
    case ItemType::Time:
        break;
    }
}

void SceneMain::updateItems(double delta_time)
{
    for (auto& item : items)
    {
        item->update(delta_time);
    }
    // 移除已销毁的物品
    items.remove_if([](std::unique_ptr<Item>& item) { return item->getIsDestroyed(); });
}

void SceneMain::renderItems(SDL_Renderer* renderer)
{
    for (auto& item : items)
    {
        item->render(renderer);
    }
}
