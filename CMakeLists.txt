cmake_minimum_required(VERSION 3.6)
project(Pymacs)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g")

INCLUDE(FindPkgConfig)

set(SOURCE_FILES src/main.cpp src/SDL2/Timer.cpp src/SDL2/Timer.h src/Buffer.cpp src/Buffer.h src/python.cpp src/python.h src/Editor.cpp src/Editor.h src/PlatformLayer.h src/SDL2/SDL2Layer.cpp src/SDL2/SDL2Layer.h src/Viewport.cpp src/Viewport.h)
add_executable(Pymacs ${SOURCE_FILES})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_TTF REQUIRED SDL2_ttf)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

find_package(Boost COMPONENTS python3)
include_directories(${Boost_INCLUDE_DIRS})

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})

target_link_libraries(Pymacs ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})