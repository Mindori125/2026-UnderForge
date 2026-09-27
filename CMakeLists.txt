cmake_minimum_required(VERSION 3.20)

project(Game2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.22
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

add_executable(Game2026
    main.cpp
    Engine/Core/Engine.cpp
    Game/Player.cpp
)

target_link_libraries(Game2026 PRIVATE SDL3::SDL3)

# 웹 빌드일 경우 HTML 파일까지 생성
if(EMSCRIPTEN)
    set_target_properties(Game2026 PROPERTIES
        SUFFIX ".html"
    )
endif()