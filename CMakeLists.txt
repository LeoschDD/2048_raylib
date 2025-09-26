cmake_minimum_required(VERSION 4.0)
project(2048_raylib)

set(CMAKE_CXX_STANDARD 23)

add_executable(2048_raylib main.cpp)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
