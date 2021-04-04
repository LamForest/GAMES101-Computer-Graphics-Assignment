cmake_minimum_required(VERSION 3.10)
project(RayTracing2)

set(CMAKE_CXX_STANDARD 17)
# add_subdirectory(mingw-std-threads-master)
SET(CMAKE_CXX_COMPILER "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/g++")
SET(CMAKE_C_COMPILER "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/gcc")
add_executable(RayTracing2 main.cpp Object.hpp Vector.cpp Vector.hpp Sphere.hpp global.hpp Triangle.hpp Scene.cpp
        Scene.hpp Light.hpp AreaLight.hpp BVH.cpp BVH.hpp Bounds3.hpp Ray.hpp Material.hpp Intersection.hpp
        Renderer.cpp Renderer.hpp)
# target_link_libraries(RayTracing2 PRIVATE mingw_stdthreads)
