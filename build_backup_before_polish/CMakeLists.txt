cmake_minimum_required(VERSION 3.16)
project(achilles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force wide-character ncurses. Ubuntu: /usr/include/ncursesw/, libncursesw.so
find_path(CURSES_WIDE_INCLUDE_DIR
    NAMES ncursesw/curses.h
    PATHS /usr/include /usr/local/include
    REQUIRED
)
find_library(CURSES_WIDE_LIB
    NAMES ncursesw
    REQUIRED
)

message(STATUS "ncursesw include: ${CURSES_WIDE_INCLUDE_DIR}")
message(STATUS "ncursesw lib:     ${CURSES_WIDE_LIB}")

find_library(STARK_LIB stark REQUIRED)
find_path(STARK_INC stark.hpp REQUIRED)

add_executable(achilles
    src/main.cpp
    src/save_manager.cpp
    src/game_context.cpp
    src/ui.cpp
    src/game_app.cpp
    src/combat.cpp
    src/enemies.cpp
    src/enemies_l2.cpp
    src/enemies_l3.cpp
    src/tutorial.cpp                # NEW
    src/screens_run.cpp
    src/screens_combat.cpp
    src/screens_loom.cpp
    src/screens_death.cpp
    src/screens_boon.cpp
    src/screens_boot.cpp
    src/screens_help.cpp
    src/screens_tutorial.cpp        # NEW
    src/screens_manual.cpp          # NEW
)

target_include_directories(achilles PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/json
    ${STARK_INC}
    ${CURSES_WIDE_INCLUDE_DIR}
)

target_link_libraries(achilles PRIVATE ${STARK_LIB} ${CURSES_WIDE_LIB})