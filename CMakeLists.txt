cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 14)

project(Lexer VERSION 1.0.0 LANGUAGES CXX)

include_directories(
    "include"
)

file(GLOB_RECURSE SRCS "src/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(lexer STATIC ${SRCS})

target_include_directories(lexer PUBLIC "include")