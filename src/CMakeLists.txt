# Main executable
add_executable(rubix_cube main.c)

target_link_libraries(rubix_cube 
	PRIVATE glfw
	PRIVATE glad_gl_core_33
	PRIVATE shader
)

# Custom libraries
add_library(read_file read_file.c read_file.h)

add_library(shader shader.h shader.c)
target_link_libraries(shader
	PRIVATE glad_gl_core_33
	PRIVATE read_file
)