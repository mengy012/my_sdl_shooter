#include "music_manager.h"
#include "../game.h"


MusicManager::~MusicManager()
{
    for (auto& pair : background_musics_)
    {
        if (pair.second)
        {
            Mix_FreeMusic(pair.second);
            pair.second = nullptr;
        }
    }
    for (auto& pair : chunks_)
    {
        if (pair.second)
        {
            Mix_FreeChunk(pair.second);
            pair.second = nullptr;
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

Mix_Music* MusicManager::getBackgroundMusic(MusicType type)
{

    auto it = background_musics_.find(type);
    if (it != background_musics_.end())
    {
        return it->second;
    }

    Mix_Music* music = nullptr;
    switch (type)
    {
    case MusicType::Title:
        music = Mix_LoadMUS("../../assets/music/06_Battle_in_Space_Intro.ogg");
        if (!music)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background music: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        background_musics_[type] = music;
        break;
    case MusicType::Main:
        music = Mix_LoadMUS("../../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
        if (!music)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background music: %s\n", Mix_GetError());
            Game::instance().getIsRunning() = false;
        }
        background_musics_[type] = music;
        break;
    }
    return music;
}
