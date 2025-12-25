#include "cl_kernels_context.h"
#include "cl_kernels_init.h"
#include "cl_kernels.h"

#include "read_file.h"

static cl_kernel kernel;
static cl_mem buffer_a, buffer_b, buffer_c;

int kernel_vector_add_init() {
    cl_int error_code;

    // Compile kernel
    const char* program_source = read_file(KERNEL_SOURCE_DIR "vector_add.cl");
    const cl_program program = clCreateProgramWithSource(context, 1, &program_source, NULL, &error_code);
    if (!CL_CHECK(error_code)) return 0;
    free(program_source);
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "vector_add", &error_code);
    if (!CL_CHECK(error_code)) return 0;
}
