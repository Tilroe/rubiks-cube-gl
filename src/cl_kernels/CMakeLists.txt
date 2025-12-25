add_library(cl_kernels
        cl_kernels_init.c
        kernels/kernel_vector_add/kernel_vector_add.c
)
target_link_libraries(cl_kernels
        PRIVATE OpenCL::Headers
        PRIVATE OpenCL::OpenCL
        PRIVATE read_file
)
target_include_directories(cl_kernels
        PUBLIC include
        PRIVATE kernels # for cl_kernels_context.h
)
set(KERNEL_SOURCE_DIR "${RESOURCE_DIR}/kernels/")
target_compile_definitions(cl_kernels PRIVATE KERNEL_SOURCE_DIR="${KERNEL_SOURCE_DIR}")