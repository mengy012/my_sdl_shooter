cmake_minimum_required(VERSION 3.15)
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
set(ENGINE_SOURCES
    
    src/game.cpp
    src/scene/scene.cpp
    src/scene/scene_main.cpp
    src/scene_main_object/player.cpp
    src/scene_main_object/button_pause.cpp
    src/scene_main_object/player_bullet.cpp
    src/scene_main_object/enemy.cpp
    src/scene_main_object/enemy_bullet.cpp
)

# 游戏库 游戏在这里实现
add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC src)
target_link_libraries(engine
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
)
# ----------------------------
# 可执行目标
# ----------------------------
add_executable(my_sdl_shooter src/main.cpp)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(my_sdl_shooter
    PRIVATE
        engine
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

if (MSVC)
    target_compile_options(engine PRIVATE
        /W4 /utf-8
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(engine PRIVATE
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

target_compile_definitions(engine PRIVATE
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

set_target_properties(engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release
)