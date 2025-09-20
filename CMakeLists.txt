cmake_minimum_required(VERSION 3.20)
project(TowerDefense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(USE_SFML_STUB "Use bundled stub implementation of SFML" ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(TowerDefense ${SOURCES})

if (USE_SFML_STUB)
    target_include_directories(TowerDefense PRIVATE vendor/sfml_stub/include)
    target_compile_definitions(TowerDefense PRIVATE SFML_STUB=1)
else()
    find_package(SFML 2.6 COMPONENTS graphics window audio system)
    if (SFML_FOUND)
        target_link_libraries(TowerDefense PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)
        target_include_directories(TowerDefense PRIVATE ${SFML_INCLUDE_DIRS})
    else()
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
        target_link_libraries(TowerDefense PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)
    endif()
endif()

target_include_directories(TowerDefense PRIVATE src vendor/include)

if (MSVC)
    target_compile_options(TowerDefense PRIVATE /W4)
else()
    target_compile_options(TowerDefense PRIVATE -Wall -Wextra -Wpedantic)
endif()

