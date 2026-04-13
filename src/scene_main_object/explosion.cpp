#include "explosion.h"

Explosion::Explosion(float x, float y, int width, int height,
                     std::chrono::steady_clock::time_point start_time)
    : start_time(start_time), texture(GetTexture())
{
    SDL_QueryTexture(texture.get(), NULL, NULL, &frame_width, &frame_height);

    total_frames = frame_width / frame_height;
    frame_width = frame_height;

    position.x = x + width / 2 - frame_width / 2;
    position.y = y + height / 2 - frame_height / 2;
}

void Explosion::render(SDL_Renderer* renderer)
{
    SDL_Rect src_rect = {current_frame * frame_width, 0, frame_width, frame_height};
    SDL_Rect dst_rect = {(int)position.x, (int)position.y, frame_width, frame_height};
    SDL_RenderCopy(renderer, texture.get(), &src_rect, &dst_rect);
}

void Explosion::update(double)
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

std::shared_ptr<SDL_Texture> Explosion::GetTexture(bool destroy)
{
    static std::shared_ptr<SDL_Texture> texture = std::shared_ptr<SDL_Texture>(
        IMG_LoadTexture(Game::instance().getRenderer(), "../../assets/effect/explosion.png"),
        [](SDL_Texture* tex)
        {
            if (tex)
            {
                SDL_DestroyTexture(tex);
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "explosion texture destroyed\n");
            }
        });

    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "load explosion img failed %s\n", IMG_GetError());
        Game::instance().getIsRunning() = false;
    }

    if (destroy)
    {
        texture.reset();
    }
    return texture;
}
