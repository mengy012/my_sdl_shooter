add_library(scene STATIC
    ./scene.cpp
    ./scene_main.cpp
    ./scene_title.cpp
)

target_include_directories(scene PUBLIC ./)
target_link_libraries(scene
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
)

if (MSVC)
    target_compile_options(scene PRIVATE
        /W4 /utf-8
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(scene PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

