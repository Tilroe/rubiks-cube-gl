cmake_minimum_required(VERSION 4.1)
project(rubix-cube-gl C)
add_subdirectory(src)

# External libs
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad/cmake)


# Specify glad settings
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

# Copy shaders to build directory
file(COPY shaders DESTINATION .)