
cmake_minimum_required(VERSION 3.15)

#Project Name
project(directory_test)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# Executable Name
set(EXECUTABLE_NAME main)
set(TEST_EXECUTABLE_NAME unit_test)

set(INCLUDE ./include)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#Bring the headers, such as Student.h into the project
include_directories(${INCLUDE})

#Generate the shared library from the sources
# add_library(testStudent SHARED ${SOURCES})

#Generate the static library from the sources
# add_library(testStudent STATIC ${SOURCES})

#For the shared library:
# set ( PROJECT_LINK_LIBS libtestStudent.so )
# link_directories( ~/exploringBB/extras/cmake/studentlib_shared/build )

#For the static library:
# set ( PROJECT_LINK_LIBS libtestStudent.a )
# link_directories( ~/exploringBB/extras/cmake/studentlib_static/build )

#Can manually add the sources using the set command as follows:
# set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB MAIN "main.cpp")
file(GLOB SOURCES "./src/*.cpp")
file(GLOB TEST_SOURCES "./tests/*.c")
file(GLOB HEADERS "./include/*.h")

add_executable(${EXECUTABLE_NAME} ${MAIN} ${SOURCES} ${HEADERS})

# add_library(criterion_lib STATIC IMPORTED)
#set_target_properties(criterion_lib PROPERTIES IMPORTED_LOCATION
# "/usr/lib/x86_64-linux-gnu/my_logical_ext_lib.so")

# find_package(Criterion REQUIRED)
# include_directories(${CRITERION_INCLUDE_DIR})
# file(GLOB CRITERION_HEADERS "criterion/*.h")
# include_directories(CRITERION_HEADERS)

# add_executable(The_project ${TEST_SOURCES})
# target_link_libraries(app ${CRITERION_LIBRARY})

add_executable(${TEST_EXECUTABLE_NAME} ${SOURCES} ${HEADERS} ${TEST_SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} criterion)
#target_compile_features(${TEST_EXECUTABLE_NAME} PUBLIC cxx_std_17)
# target_link_libraries(l${EXECUTABLE_NAME} ${PROJECT_LINK_LIBS} )
# add_executable(${EXECUTABLE_NAME} ${SOURCES})