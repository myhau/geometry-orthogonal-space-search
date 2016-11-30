cmake_minimum_required(VERSION 3.6)
project(geo_proj)


#set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc/6.2.0/bin/g++-6)
set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCE_FILES src/*.h src/main.cpp)
file(GLOB TEST_FILES test/*.h test/*.cpp src/*.h)

find_package(Boost)
#
#
#include_directories(/usr/local/Cellar/gcc/6.1.0/include)
#include_directories(/usr/local/Cellar/gcc/6.1.0/include/c++)
#include_directories(/usr/local/Cellar/gcc/6.1.0/include/c++/6.1.0)

include_directories(libs)
include_directories(cget/include)
include_directories(${Boost_INCLUDE_DIR})

add_executable(range_tree ${SOURCE_FILES} src/Rect.h)
add_executable(test_range_tree ${TEST_FILES} )

target_link_libraries(range_tree ${Boost_LIBRARIES})
target_link_libraries(test_range_tree ${Boost_LIBRARIES})

#enable_testing(true)