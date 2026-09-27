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
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.4
    GIT_SHALLOW TRUE
)

set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3_image)

add_executable(Game2026
    main.cpp

    Engine/Core/Engine.cpp
    Engine/Input/Input.cpp

    Engine/Object/GameObject.cpp
    Engine/Object/ObjectManager.cpp

    Engine/Physics/Collision.cpp
    Engine/Graphics/Texture.cpp

    Engine/Scene/SceneManager.cpp
    Engine/Graphics/Camera.cpp  

    Game/Player.cpp
    Game/Enemy.cpp
    Game/GameScene.cpp
    
)

target_link_libraries(Game2026 PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

if(EMSCRIPTEN)
    set_target_properties(Game2026 PROPERTIES
        SUFFIX ".html"
    )

    target_link_options(Game2026 PRIVATE
        "--preload-file"
        "${CMAKE_SOURCE_DIR}/Assets@/Assets"
    )
endif()