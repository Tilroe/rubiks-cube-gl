cmake_minimum_required(VERSION 3.10)
project(rubix-cube-gl C)

# External libs
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad/cmake)
add_subdirectory(lib/OpenCL-Headers)
add_subdirectory(lib/OpenCL-ICD-Loader)
add_subdirectory(lib/stb_image)

# Specify glad settings
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

# Project source directory
add_subdirectory(src)

# Stuff to copy to build directory
file(COPY shaders DESTINATION .)
file(COPY resources DESTINATION .)
file(COPY kernels DESTINATION .)