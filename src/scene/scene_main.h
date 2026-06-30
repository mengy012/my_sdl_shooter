#pragma once

#include <random>
#include <vector>

#include "../scene_main_object/button_pause.h"
#include "../scene_main_object/enemy.h"
#include "../scene_main_object/enemy_bullet.h"
#include "../scene_main_object/explosion.h"
#include "../scene_main_object/item.h"
#include "../scene_main_object/player.h"

#include "./scene.h"

// 主场景
class SceneMain : public Scene
{
  public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void handleEvent(SDL_Event& event) override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;
    SceneState getState() override;

  private:
      // 爆炸相关
    void enemyExplode(Enemy& enemy); // 敌人爆炸
    void playerExplode();            // 玩家爆炸

    // 生成相关
    void generateEnemy();            // 生成敌人
    void generateItem(Enemy& enemy); // 生成物品

    // 更新相关
    void updateEnemyBullets(double delta_time); // 更新敌人子弹位置
    void updateExplosions(double delta_time);   // 更新爆炸效果
    void updateEnemy(double delta_time);        // 更新敌人位置
    void updateItems(double delta_time);        // 更新物品位置

    // 渲染相关
    void renderExplosions(SDL_Renderer* renderer);   // 渲染爆炸效果
    void renderEnemyBullets(SDL_Renderer* renderer); // 渲染敌人子弹
    void renderEnemy(SDL_Renderer* renderer);        // 渲染敌人
    void renderItems(SDL_Renderer* renderer);        // 渲染物品

    SceneState state{SceneState::Main};

    Player player; // 玩家飞机

    ButtonPause pauseButton; // 暂停按钮

    bool is_paused{false};
    SDL_Rect pause_text_rect{0, 0, 0, 0};
    SDL_Rect continue_button_rect{0, 0, 0, 0};

    // 更新暂停文本布局
    void updatePauseTextLayout();
    // 绘制暂停文本
    void renderPauseText(SDL_Renderer* renderer);

    // 绘制帧数
    void renderFps(SDL_Renderer* renderer);
    // 绘制玩家生命值
    void renderPlayerHealth(SDL_Renderer* renderer);
    // 玩家生命值纹理
    std::unique_ptr<SDL_Texture, DeleteTexture> player_health_texture;

    // 绘制玩家得分
    void renderScore(SDL_Renderer* renderer);
    // 玩家得分
    int score{0};

    // 敌人生成模板
    Enemy enemy_template;

    // 敌人列表
    std::vector<Enemy> enemies;

    // 敌机发射子弹相关
    EnemyBullet enemy_bullet_template;    // 子弹对象模板,存放在敌机对象中,每次发射子弹时以它为基础创建子弹对象
    std::list<EnemyBullet> enemy_bullets; // 敌机子弹列表

    // 爆炸纹理管理器
    ExplosionTextureManager explosion_texture_manager;
    // 爆炸效果列表（包括玩家和敌人爆炸）
    std::list<Explosion> explosions;

    // 物品纹理管理器
    ItemTextureManager item_texture_manager;

    // 物品列表
    std::list<std::unique_ptr<Item>> items;

    // 游戏结束延迟计时器
    float game_end_delay{0.0f};

    // 切换场景状态
    void changeSceneDelay(double delta_time, float delay = 1.5f);
};