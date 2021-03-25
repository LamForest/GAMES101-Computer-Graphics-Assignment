cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(D:\\gao_tianlin\\library\\eigen3\\eigen-3.3.7)

add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp global.hpp Triangle.hpp Triangle.cpp)
add_executable(cross cross_product.cpp)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})

