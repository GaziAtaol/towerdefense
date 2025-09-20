cmake_minimum_required(VERSION 3.20)
project(TowerDefense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_SYSTEM ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    sfml
    URL https://github.com/SFML/SFML/archive/refs/tags/2.6.0.tar.gz
)
FetchContent_MakeAvailable(sfml)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(TowerDefense ${SOURCES})

target_include_directories(TowerDefense PRIVATE src vendor/include)

target_link_libraries(TowerDefense PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)

if (MSVC)
    target_compile_options(TowerDefense PRIVATE /W4)
else()
    target_compile_options(TowerDefense PRIVATE -Wall -Wextra -Wpedantic)
endif()

