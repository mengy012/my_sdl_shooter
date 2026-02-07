#include "game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    Game::instance().init().setLogCategoryPriority(SDL_LOG_CATEGORY_APPLICATION,
                                                   SDL_LOG_PRIORITY_INFO);
    Game::instance().run();
    return 0;
}