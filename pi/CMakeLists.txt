cmake_minimum_required(VERSION 3.10)
project(SquirrelShooter)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(SquirrelShooter src/main.cpp)

target_include_directories(SquirrelShooter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_sources(SquirrelShooter
        PRIVATE
        src/camera.cpp
        src/camera.hpp
        src/image_processing.cpp
        src/image_processing.hpp
        src/main.cpp
)

target_link_libraries(SquirrelShooter ${OpenCV_LIBS})