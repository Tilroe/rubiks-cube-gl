add_executable(rubix-cube main.c)

target_link_libraries(rubix-cube 
	PRIVATE glfw
	PRIVATE glad_gl_core_33
)