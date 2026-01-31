# CMake最低版本与工程声明
cmake_minimum_required(VERSION 3.10.0)
project(my_sdl_shooter VERSION 0.1.0 LANGUAGES C CXX)

# 设置cpp标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找并载入cmake预设
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 设置可执行文件名字
set(TARGET ${PROJECT_NAME}_${CMAKE_SYSTEM_NAME}_${PROJECT_VERSION})
# 设置待编译cpp源文件合集
set(SOURCES "./src/main.cpp"
            "./src/game.cpp"
            "./src/scene/scene.cpp"
            "./src/scene/scene_main.cpp"
            "./src/object/player.cpp"
)
# 添加可执行文件
add_executable(${TARGET} ${SOURCES})

# 添加要链接的库
#target_link_libraries(${TARGET} SDL2::SDL2main SDL2::SDL2)
target_link_libraries(${TARGET} ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# 手动设置头文件目录和库目录
#include_directories(C:/Develop_Library/SDL2/include)
#link_directories(C:/Develop_Library/SDL2/lib/x64)