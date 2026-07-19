#include "explosion.h"

Explosion::Explosion(float x, float y, int width, int height, std::chrono::steady_clock::time_point start_time, SDL_Texture* texture) : start_time(start_time), texture_(texture)
{
    SDL_QueryTexture(texture_, NULL, NULL, &frame_width, &frame_height);

    total_frames = frame_width / frame_height;
    frame_width = frame_height;

    position.x = x + width / 2 - frame_width / 2;
    position.y = y + height / 2 - frame_height / 2;
}

void Explosion::render(SDL_Renderer* renderer)
{
    SDL_Rect src_rect = {current_frame * frame_width, 0, frame_width, frame_height};
    SDL_Rect dst_rect = {(int)position.x, (int)position.y, frame_width * 2, frame_height * 2};
    SDL_RenderCopy(renderer, texture_, &src_rect, &dst_rect);
}

void Explosion::update(double delta_time)
{
    auto current_time{std::chrono::steady_clock::now()};
    if (current_time - start_time >= std::chrono::milliseconds(50))
    {
        current_frame++;
        start_time = current_time;
        if (current_frame > total_frames)
        {
            is_finished = true;
        }
    }
}

ExplosionTextureManager::ExplosionTextureManager()
{
    texture_ = IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/effect/explosion.png");
    if (!texture_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load explosion img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }
}

ExplosionTextureManager::~ExplosionTextureManager()
{
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "explosion texture destroyed\n");
    }
}

SDL_Texture* ExplosionTextureManager::get_texture()
{
    return texture_;
}
