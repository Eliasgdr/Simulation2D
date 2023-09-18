cmake_minimum_required(VERSION 3.25)
project(Simulation2D)

set(CMAKE_CXX_STANDARD 17)

add_executable(Simulation2D main.cpp objects.h vectors.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(Simulation2D sfml-system sfml-window sfml-graphics)