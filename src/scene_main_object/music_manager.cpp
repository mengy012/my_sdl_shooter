#include "music_manager.h"

MusicManager::MusicManager()
{
    background_music_ = Mix_LoadMUS("../../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (!background_music_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background music: %s\n", Mix_GetError());
        Game::instance().getIsRunning() = false;
    }
}

MusicManager::~MusicManager()
{
    if (background_music_)
    {
        Mix_FreeMusic(background_music_);
    }
    for (auto& pair : chunks_)
    {
        if (pair.second)
        {
            Mix_FreeChunk(pair.second);
        }
    }
}

Mix_Chunk* MusicManager::getChunk(ChunkType type)
{
    auto it = chunks_.find(type);
    if (it != chunks_.end())
    {
        return it->second;
    }
    Mix_Chunk* chunk = nullptr;
    switch (type)
    {
    case ChunkType::Effect_player_shoot:
        chunk = Mix_LoadWAV("../../assets/sound/laser_shoot4.wav");
        if (!chunk)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        chunks_[type] = chunk;
        break;
    case ChunkType::Effect_enemy_shoot:
        chunk = Mix_LoadWAV("../../assets/sound/xs_laser.wav");
        if (!chunk)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        chunks_[type] = chunk;
        break;
    case ChunkType::Effect_player_explode:
        chunk = Mix_LoadWAV("../../assets/sound/explosion1.wav");
        if (!chunk)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        chunks_[type] = chunk;
        break;
    case ChunkType::Effect_enemy_explode:
        chunk = Mix_LoadWAV("../../assets/sound/explosion3.wav");
        if (!chunk)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        chunks_[type] = chunk;
        break;
    case ChunkType::Effect_hit:
        chunk = Mix_LoadWAV("../../assets/sound/eff11.wav");
        if (!chunk)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        chunks_[type] = chunk;
        break;
    case ChunkType::Effect_get_item:
        chunk = Mix_LoadWAV("../../assets/sound/eff5.wav");
        if (!chunk)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        chunks_[type] = chunk;
        break;
    }
    return chunk;
}

Mix_Music* MusicManager::getBackgroundMusic()
{
    return background_music_;
}
