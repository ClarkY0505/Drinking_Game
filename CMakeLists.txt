cmake_minimum_required(VERSION 3.22)

project(DrinkingGame
    VERSION 0.1.0
    LANGUAGES CXX
)

include(CTest)

option(DRINKING_GAME_BUILD_DESKTOP "Build the desktop executable when src/core/main.cpp contains an entry point" ON)
option(DRINKING_GAME_BUILD_LIB_ONLY "Build only the XunWoEngine shared and static libraries" OFF)
set(DRINKING_GAME_SDL2_DIR "${SDL2_DIR}" CACHE PATH "Path to a vendored SDL2 source or SDL2 CMake package directory")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DRINKING_GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE DRINKING_GAME_SOURCES CONFIGURE_DEPENDS
    "${DRINKING_GAME_ROOT}/src/*.cpp"
)
list(SORT DRINKING_GAME_SOURCES)

file(GLOB_RECURSE DRINKING_GAME_ENGINE_SOURCES CONFIGURE_DEPENDS
    "${DRINKING_GAME_ROOT}/src/engine/*.cpp"
)
list(SORT DRINKING_GAME_ENGINE_SOURCES)

set(DRINKING_GAME_APP_SOURCES ${DRINKING_GAME_SOURCES})
list(REMOVE_ITEM DRINKING_GAME_APP_SOURCES ${DRINKING_GAME_ENGINE_SOURCES})

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

add_library(XunWoEngine SHARED ${DRINKING_GAME_ENGINE_SOURCES})
add_library(XunWoEngine_static STATIC ${DRINKING_GAME_ENGINE_SOURCES})

foreach(_engine_target XunWoEngine XunWoEngine_static)
    set_target_properties(${_engine_target}
        PROPERTIES
            OUTPUT_NAME XunWoEngine
            ARCHIVE_OUTPUT_DIRECTORY "${DRINKING_GAME_ROOT}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${DRINKING_GAME_ROOT}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${DRINKING_GAME_ROOT}/lib"
    )

    target_include_directories(${_engine_target}
        PUBLIC
            "${DRINKING_GAME_ROOT}/inc"
    )

    target_link_libraries(${_engine_target}
        PUBLIC
            ${DRINKING_GAME_SDL2_TARGET}
    )
endforeach()

set_target_properties(XunWoEngine
    PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

if(MSVC)
    foreach(_compile_target XunWoEngine XunWoEngine_static)
        target_compile_options(${_compile_target} PRIVATE /W4)
    endforeach()
else()
    foreach(_compile_target XunWoEngine XunWoEngine_static)
        target_compile_options(${_compile_target} PRIVATE -Wall -Wextra -Wpedantic)
    endforeach()
endif()

if(NOT DRINKING_GAME_BUILD_LIB_ONLY)
    drinking_game_find_sdl2_image(DRINKING_GAME_SDL2_IMAGE_TARGET)

    if(ANDROID)
        add_library(main SHARED ${DRINKING_GAME_APP_SOURCES})
        set(DRINKING_GAME_TARGET main)
    else()
        file(SIZE "${DRINKING_GAME_ROOT}/src/core/main.cpp" DRINKING_GAME_MAIN_SIZE)

        if(DRINKING_GAME_BUILD_DESKTOP AND DRINKING_GAME_MAIN_SIZE GREATER 0)
            add_executable(drinking_game ${DRINKING_GAME_APP_SOURCES})
            set(DRINKING_GAME_TARGET drinking_game)
        else()
            add_library(drinking_game OBJECT ${DRINKING_GAME_APP_SOURCES})
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
            XunWoEngine
            ${DRINKING_GAME_SDL2_TARGET}
            ${DRINKING_GAME_SDL2_IMAGE_TARGET}
    )

    if(MSVC)
        target_compile_options(${DRINKING_GAME_TARGET} PRIVATE /W4)
    else()
        target_compile_options(${DRINKING_GAME_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(BUILD_TESTING AND NOT ANDROID)
        add_executable(screen_command_compile_test
            "${DRINKING_GAME_ROOT}/tests/screen_command_compile_test.cpp"
        )

        target_include_directories(screen_command_compile_test
            PRIVATE
                "${DRINKING_GAME_ROOT}/inc"
        )

        target_link_libraries(screen_command_compile_test
            PRIVATE
                XunWoEngine
        )

        add_test(NAME screen_command_compile_test COMMAND screen_command_compile_test)
    endif()
endif()
