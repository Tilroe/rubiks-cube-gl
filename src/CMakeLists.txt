# Main executable
add_executable(rubix_cube main.c)

target_link_libraries(rubix_cube 
	PRIVATE glfw
	PRIVATE glad_gl_core_33
	PRIVATE shader
)

# Custom libraries
add_library(shader shader.h shader.c)
target_link_libraries(shader
	PRIVATE glad_gl_core_33
	PRIVATE read_file
)

add_library(read_file read_file.c read_file.h)

add_library(vector vector.c vector.h)
target_link_libraries(vector m) # For math.h

add_library(matrix matrix.c matrix.h)
target_link_libraries(matrix 
	PRIVATE vector
)

add_library(quaternion quaternion.c quaternion.h)
target_link_libraries(quaternion
	PRIVATE m # For math.h
	PRIVATE matrix
)