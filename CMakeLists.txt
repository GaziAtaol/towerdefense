cmake_minimum_required(VERSION 3.20)
project(TowerDefense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

find_program(PATCH_EXECUTABLE patch)
if (NOT PATCH_EXECUTABLE)
    message(FATAL_ERROR "Required tool 'patch' was not found. Please install a POSIX patch utility.")
endif()

set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_SYSTEM ON CACHE BOOL "" FORCE)

set(SFML_PATCH_FILE "${CMAKE_SOURCE_DIR}/cmake/patches/sfml-libcxx-char-traits.patch")

FetchContent_Declare(
    sfml
    URL https://github.com/SFML/SFML/archive/refs/tags/2.6.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(sfml)
if (NOT sfml_POPULATED)
    FetchContent_Populate(sfml)

    execute_process(
        COMMAND ${PATCH_EXECUTABLE} -p1 --dry-run -R -i "${SFML_PATCH_FILE}"
        WORKING_DIRECTORY ${sfml_SOURCE_DIR}
        RESULT_VARIABLE SFML_PATCH_ALREADY_APPLIED
        OUTPUT_VARIABLE SFML_PATCH_REVERSE_OUTPUT
        ERROR_VARIABLE SFML_PATCH_REVERSE_ERROR
    )

    if (SFML_PATCH_ALREADY_APPLIED EQUAL 0)
        message(STATUS "SFML libc++ compatibility patch already applied; skipping.")
    else()
        execute_process(
            COMMAND ${PATCH_EXECUTABLE} -p1 -i "${SFML_PATCH_FILE}"
            WORKING_DIRECTORY ${sfml_SOURCE_DIR}
            RESULT_VARIABLE SFML_PATCH_RESULT
            OUTPUT_VARIABLE SFML_PATCH_OUTPUT
            ERROR_VARIABLE SFML_PATCH_ERROR
        )

        if (NOT SFML_PATCH_RESULT EQUAL 0)
            string(JOIN "\n" _sfml_patch_message
                "Failed to apply SFML libc++ compatibility patch."
                "Command: ${PATCH_EXECUTABLE} -p1 -i \"${SFML_PATCH_FILE}\""
                "Working directory: ${sfml_SOURCE_DIR}"
                "stdout: ${SFML_PATCH_OUTPUT}"
                "stderr: ${SFML_PATCH_ERROR}"
            )
            message(FATAL_ERROR "${_sfml_patch_message}")
        endif()
    endif()
endif()

add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(TowerDefense ${SOURCES})

target_include_directories(TowerDefense PRIVATE src vendor/include)

target_link_libraries(TowerDefense PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)

if (MSVC)
    target_compile_options(TowerDefense PRIVATE /W4)
else()
    target_compile_options(TowerDefense PRIVATE -Wall -Wextra -Wpedantic)
endif()

