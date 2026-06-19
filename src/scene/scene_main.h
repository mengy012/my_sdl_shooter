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

    void generateEnemy();                     // 生成敌人
    void updateEnemy(double delta_time);      // 更新敌人位置
    void renderEnemy(SDL_Renderer* renderer); // 渲染敌人

    void updateEnemyBullets(double delta_time);      // 更新敌人子弹位置
    void renderEnemyBullets(SDL_Renderer* renderer); // 渲染敌人子弹

    void enemyExplode(Enemy& enemy);               // 敌人爆炸
    void playerExplode();                          // 玩家爆炸
    void updateExplosions(double delta_time);      // 更新爆炸效果
    void renderExplosions(SDL_Renderer* renderer); // 渲染爆炸效果

    void generateItem(Enemy& enemy);          // 生成物品
    void updateItems(double delta_time);      // 更新物品位置
    void renderItems(SDL_Renderer* renderer); // 渲染物品
  private:
    Player player; // 玩家飞机

    ButtonPause pauseButton; // 暂停按钮

    bool is_paused{false};
    SDL_Rect pause_text_rect{0, 0, 0, 0};
    SDL_Rect continue_button_rect{0, 0, 0, 0};

    void updatePauseTextLayout();

    // 敌人生成模板
    Enemy enemy_template;

    // 敌人列表
    std::vector<Enemy> enemies;

    // 敌机发射子弹相关
    EnemyBullet enemy_bullet_template;    // 子弹对象模板,存放在敌机对象中,每次发射子弹时以它为基础创建子弹对象
    std::list<EnemyBullet> enemy_bullets; // 敌机子弹列表

    // 爆炸效果列表（包括玩家和敌人爆炸）
    std::list<Explosion> explosions;

    // 物品纹理管理器
    ItemTextureManager item_texture_manager;

    // 物品列表
    std::list<std::unique_ptr<Item>> items;

    // 音乐管理器
    MusicManager music_manager;
};