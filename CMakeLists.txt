cmake_minimum_required(VERSION 3.10)
project(my_sdl_shooter VERSION 0.1.0 LANGUAGES C CXX)

# ----------------------------
# C++ 标准
# ----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------
# 查找 SDL2 相关库
# ----------------------------
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# ----------------------------
# 源文件
# ----------------------------
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/scene/scene.cpp
    src/scene/scene_main.cpp
    src/object/player.cpp
    src/object/button_pause.cpp
)

# ----------------------------
# 可执行目标
# ----------------------------
add_executable(my_sdl_shooter ${SOURCES})

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(my_sdl_shooter
    PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
)

# ----------------------------
# 编译选项（按 编译器 + 配置 区分）
# ----------------------------
if (MSVC)
    target_compile_options(my_sdl_shooter PRIVATE
        /W4 /utf-8
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(my_sdl_shooter PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# ----------------------------
# Debug / Release 宏
# ----------------------------
target_compile_definitions(my_sdl_shooter PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# ----------------------------
# 输出目录（不互相覆盖）
# ----------------------------
set_target_properties(my_sdl_shooter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release
)
