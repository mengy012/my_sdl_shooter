#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>
#include <unordered_map>

enum class ChunkType
{
    Effect_player_shoot,
    Effect_enemy_shoot,
    Effect_player_explode,
    Effect_enemy_explode,
    Effect_hit,
    Effect_get_item,
};

enum class MusicType
{
    Title,
    Main,
};

class MusicManager
{
  public:
    MusicManager() = default;
    ~MusicManager();

    // 获取音效
    Mix_Chunk* getChunk(ChunkType type);
    // 获取背景音乐
    Mix_Music* getBackgroundMusic(MusicType type);
  private:
    std::unordered_map<ChunkType, Mix_Chunk*> chunks_;
    std::unordered_map<MusicType, Mix_Music*> background_musics_;
};
