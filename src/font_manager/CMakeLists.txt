add_library(font_manager STATIC ./font_manager.cpp)

target_include_directories(font_manager PUBLIC ./)

target_link_libraries(font_manager
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
)


if (MSVC)
    target_compile_options(font_manager PRIVATE
        /W4 /utf-8
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /MT>
    )
else()
    target_compile_options(font_manager PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

