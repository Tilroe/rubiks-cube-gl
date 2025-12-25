# Main executable
add_executable(rubix_cube main.c)

target_link_libraries(rubix_cube 
	PRIVATE window
	PRIVATE renderer
    PRIVATE cl_kernels
)

# Custom libraries
add_subdirectory(read_file)
add_subdirectory(cl_kernels)

add_library(window window.c window.h)
target_link_libraries(window
	PRIVATE glad_gl_core_33
	PRIVATE glfw
	PRIVATE mouse_handler
)

add_library(renderer renderer.c renderer.h)
target_link_libraries(renderer
	PRIVATE glad_gl_core_33
	PRIVATE glfw
	PRIVATE stb_image
	PRIVATE shader
	PRIVATE window
	PRIVATE camera
	PRIVATE cube
)

add_library(shader shader.c shader.h)
target_link_libraries(shader
	PRIVATE glad_gl_core_33
	PRIVATE read_file
	PUBLIC matrix
)

#add_library(kernels kernels.c kernels.h)
#target_link_libraries(kernels
#    PRIVATE OpenCL::Headers
#    PRIVATE OpenCL::OpenCL
#)



add_library(mouse_handler mouse_handler.c mouse_handler.h)
target_link_libraries(mouse_handler
	PUBLIC glfw
	PRIVATE window
	PRIVATE camera
	PRIVATE cube
	PRIVATE quaternion
	PRIVATE ray
)

add_library(vector vector.c vector.h)

add_library(matrix matrix.c matrix.h)
target_link_libraries(matrix 
	PUBLIC vector
)

add_library(quaternion quaternion.c quaternion.h)
target_link_libraries(quaternion
	PUBLIC matrix
)

add_library(ray ray.c ray.h)
target_link_libraries(ray
	PUBLIC vector
)

add_library(camera camera.c camera.h)
target_link_libraries(camera
	PUBLIC matrix
)

add_library(cube cube.c cube.h)
target_link_libraries(cube
	PUBLIC vector
	PUBLIC quaternion
)