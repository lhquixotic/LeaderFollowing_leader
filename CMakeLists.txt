# cmake_minimum_required(VERSION <specify CMake version here>)
project(leaderfollowing)

set(CMAKE_CXX_STANDARD 11)
add_executable(leaderfollowing Leader_following.h main.cpp Communication.cpp Communication.h)
# Find thread package
find_package(Threads REQUIRED)
target_link_libraries(leaderfollowing Threads::Threads)
