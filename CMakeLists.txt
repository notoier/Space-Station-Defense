cmake_minimum_required(VERSION 3.10)
project(SpaceStationDefense LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# =========================
# SFML
# =========================
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

# =========================
# Sources
# =========================
# Pick main.cpp from root or src/
set(MAIN_CANDIDATES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(MAIN_SOURCE "")
foreach(candidate IN LISTS MAIN_CANDIDATES)
    if(EXISTS ${candidate})
        set(MAIN_SOURCE ${candidate})
        break()
    endif()
endforeach()

if(MAIN_SOURCE STREQUAL "")
    message(FATAL_ERROR "Could not find main.cpp. Put it in project root or in src/main.cpp")
endif()

# Collect all cpp from src/
file(GLOB_RECURSE SRC_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Ensure main is included even if it's already in SRC_SOURCES
set(PROJECT_SOURCES ${MAIN_SOURCE} ${SRC_SOURCES})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}
        src/Core/Game.cpp
        include/Core/Game.h
        src/Gameplay/Entity.cpp
        include/Gameplay/Entity.h
        src/Render/CompositeShape.cpp
        include/Render/CompositeShape.h
        src/Gameplay/Station.cpp
        include/Gameplay/Station.h
        src/Gameplay/Enemy.cpp
        include/Gameplay/Enemy.h
        src/Utils/MathUtils.cpp
        include/Utils/MathUtils.h)

# =========================
# Includes
# =========================
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# =========================
# Compile / Link
# =========================
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================
# Windows: copy OpenAL DLL
# =========================
if(WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            VERBATIM
    )
endif()
