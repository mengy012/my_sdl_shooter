#include "background.h"
#include "game.h"

Background::Background(SDL_Texture* texture, float speed) : texture_(texture)
{
    if (!texture_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load background img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }
    else
    {
        SDL_QueryTexture(texture_, NULL, NULL, &width_, &height_);
        // 纹理缩放
        width_ *= 0.5f;
        height_ *= 0.5f;
    }
    speed_ = speed;
}

Background::~Background()
{
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "delete background texture");
    }
}

Background& Background::operator=(Background&& other) noexcept
{
    //
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
    }
    // 转移资源
    texture_ = other.texture_;
    width_ = other.width_;
    height_ = other.height_;
    position_ = other.position_;
    y_offset_ = other.y_offset_;
    speed_ = other.speed_;
    // 置空原对象
    other.texture_ = nullptr;
    return *this;
}

void Background::update(double delta_time)
{
    position_.y += speed_ * delta_time;
    if (position_.y >= 0)
    {
        position_.y -= height_;
    }
}

void Background::render()
{

    // 窗口宽高
    int window_width = Game::instance().get_window_width();
    int window_height = Game::instance().get_window_height();

    // 渲染器
    SDL_Renderer* renderer = Game::instance().getRenderer();

    for (int pos_y = position_.y; pos_y < window_height; pos_y += height_)
    {
        for (int pos_x = 0; pos_x < window_width; pos_x += width_)
        {
            SDL_Rect dst_rect = {pos_x, pos_y, width_, height_};
            SDL_RenderCopy(renderer, texture_, NULL, &dst_rect);
        }
    }
}
