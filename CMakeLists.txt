cmake_minimum_required(VERSION 3.16)
project(achilles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- STARKDB (Option A: system install) ---
find_library(STARK_LIB stark REQUIRED)
find_path(STARK_INC stark.hpp REQUIRED)
message(STATUS "STARKDB lib: ${STARK_LIB}")
message(STATUS "STARKDB inc: ${STARK_INC}")

add_executable(achilles
    src/main.cpp
    src/save_manager.cpp
)

target_include_directories(achilles PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/json
    ${STARK_INC}
)

target_link_libraries(achilles PRIVATE ${STARK_LIB})