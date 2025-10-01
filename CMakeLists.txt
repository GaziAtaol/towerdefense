cmake_minimum_required(VERSION 3.20)
project(TowerDefense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find SFML installed on the system
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window audio system)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(TowerDefense ${SOURCES})

target_include_directories(TowerDefense PRIVATE src vendor/include)

target_link_libraries(TowerDefense PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)

if (MSVC)
    target_compile_options(TowerDefense PRIVATE /W4)
else()
    target_compile_options(TowerDefense PRIVATE -Wall -Wextra -Wpedantic)
endif()

