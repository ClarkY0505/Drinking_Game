cmake_minimum_required(VERSION 3.22)

project(DrinkingGame
    VERSION 0.1.0
    LANGUAGES CXX
)

option(DRINKING_GAME_BUILD_DESKTOP "Build the desktop executable when src/core/main.cpp contains an entry point" ON)
set(DRINKING_GAME_SDL2_DIR "${SDL2_DIR}" CACHE PATH "Path to a vendored SDL2 source or SDL2 CMake package directory")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DRINKING_GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE DRINKING_GAME_SOURCES CONFIGURE_DEPENDS
    "${DRINKING_GAME_ROOT}/src/*.cpp"
)
list(SORT DRINKING_GAME_SOURCES)

function(drinking_game_find_sdl2 out_target)
    set(_sdl2_target "")

    if(DRINKING_GAME_SDL2_DIR)
        if(EXISTS "${DRINKING_GAME_SDL2_DIR}/CMakeLists.txt")
            add_subdirectory("${DRINKING_GAME_SDL2_DIR}" "${CMAKE_BINARY_DIR}/SDL2" EXCLUDE_FROM_ALL)
        elseif(EXISTS "${DRINKING_GAME_SDL2_DIR}/SDL2Config.cmake"
            OR EXISTS "${DRINKING_GAME_SDL2_DIR}/sdl2-config.cmake")
            list(PREPEND CMAKE_PREFIX_PATH "${DRINKING_GAME_SDL2_DIR}")
        endif()
    endif()

    find_package(SDL2 CONFIG QUIET)

    if(TARGET SDL2::SDL2)
        set(_sdl2_target SDL2::SDL2)
    elseif(TARGET SDL2)
        set(_sdl2_target SDL2)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
        set(_sdl2_target PkgConfig::SDL2)
    endif()

    set(${out_target} "${_sdl2_target}" PARENT_SCOPE)
endfunction()

drinking_game_find_sdl2(DRINKING_GAME_SDL2_TARGET)

function(drinking_game_find_sdl2_image out_target)
    find_package(SDL2_image CONFIG QUIET)

    if(TARGET SDL2_image::SDL2_image)
        set(_sdl2_image_target SDL2_image::SDL2_image)
    elseif(TARGET SDL2_image::SDL2_image-static)
        set(_sdl2_image_target SDL2_image::SDL2_image-static)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
        set(_sdl2_image_target PkgConfig::SDL2_IMAGE)
    endif()

    set(${out_target} "${_sdl2_image_target}" PARENT_SCOPE)
endfunction()

drinking_game_find_sdl2_image(DRINKING_GAME_SDL2_IMAGE_TARGET)

if(ANDROID)
    add_library(main SHARED ${DRINKING_GAME_SOURCES})
    set(DRINKING_GAME_TARGET main)
else()
    file(SIZE "${DRINKING_GAME_ROOT}/src/core/main.cpp" DRINKING_GAME_MAIN_SIZE)

    if(DRINKING_GAME_BUILD_DESKTOP AND DRINKING_GAME_MAIN_SIZE GREATER 0)
        add_executable(drinking_game ${DRINKING_GAME_SOURCES})
        set(DRINKING_GAME_TARGET drinking_game)
    else()
        add_library(drinking_game OBJECT ${DRINKING_GAME_SOURCES})
        set(DRINKING_GAME_TARGET drinking_game)

        if(DRINKING_GAME_BUILD_DESKTOP)
            message(STATUS "src/core/main.cpp is empty; building objects only. Add a main function to enable the desktop executable.")
        endif()
    endif()
endif()

target_include_directories(${DRINKING_GAME_TARGET}
    PRIVATE
        "${DRINKING_GAME_ROOT}/inc"
)

target_link_libraries(${DRINKING_GAME_TARGET}
    PRIVATE
        ${DRINKING_GAME_SDL2_TARGET}
        ${DRINKING_GAME_SDL2_IMAGE_TARGET}
)

if(MSVC)
    target_compile_options(${DRINKING_GAME_TARGET} PRIVATE /W4)
else()
    target_compile_options(${DRINKING_GAME_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()
