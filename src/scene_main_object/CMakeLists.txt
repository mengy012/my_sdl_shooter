add_library(scene_main_object STATIC
    ./player.cpp
    ./button_pause.cpp
    ./player_bullet.cpp
    ./enemy.cpp
    ./enemy_bullet.cpp
    ./explosion.cpp
    ./item.h
    ./item_life_restoring.h
    ./item_life_restoring.cpp

)

target_include_directories(scene_main_object PUBLIC ./)
target_link_libraries(scene_main_object
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
)

if (MSVC)
    target_compile_options(scene_main_object PRIVATE
        /W4 /utf-8
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(scene_main_object PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

