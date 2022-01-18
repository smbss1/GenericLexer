cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(Lexer VERSION 1.0.0 LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(lexer STATIC ${SRCS})
target_link_libraries(lexer Threads::Threads)
target_include_directories(lexer PUBLIC "include")
