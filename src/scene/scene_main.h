#pragma once

#include <random>
#include <vector>

#include "../scene_main_object/button_pause.h"
#include "../scene_main_object/enemy.h"
#include "../scene_main_object/enemy_bullet.h"
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
    EnemyBullet
        enemy_bullet_template; // 子弹对象模板,存放在敌机对象中,每次发射子弹时以它为基础创建子弹对象
    std::list<EnemyBullet> enemy_bullets; // 敌机子弹列表
};