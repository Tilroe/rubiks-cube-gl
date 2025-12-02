#include "kernels.h"

#define CL_TARGET_OPENCL_VERSION 200

#include <stdio.h>
#include <stdbool.h>

#include "CL/opencl.h"

#include "ray.h"
#include "read_file.h"

#define CL_CHECK(exp) ((exp) == CL_SUCCESS)

static cl_platform_id platform;
static cl_device_id device;
static cl_context context;
static cl_command_queue queue;

int opencl_init() {
    cl_int error_code_ret;
    if (!CL_CHECK(error_code_ret = clGetPlatformIDs(1, &platform, NULL))) return 0;
    if (!(CL_CHECK(error_code_ret = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1, &device, NULL))
        || CL_CHECK(error_code_ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL)))) return 0;
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;
    queue = clCreateCommandQueueWithProperties(context, device, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;
    return 1;
}


void vector_add(const int n, float a[n], float b[n], float c[n]) {
    static bool initialized = false;
    static cl_kernel kernel;
    static cl_mem buffer_a, buffer_b, buffer_c;
    cl_int error_code;

    if (!initialized) {
        // Compile kernel
        const char* program_source = read_file("../kernels/opencl/vector_add.cl");
        const cl_program program = clCreateProgramWithSource(context, 1, &program_source, NULL, &error_code);
        free(program_source);
        clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
        kernel = clCreateKernel(program, "vector_add", &error_code);

        // Create memory objects
        buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n, NULL, &error_code);
        buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n, NULL, &error_code);
        buffer_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n, NULL, &error_code);

        initialized = true;
    }

    // Copy data to input
    error_code = clEnqueueWriteBuffer(queue, buffer_a, CL_FALSE, 0, sizeof(float) * n, a, 0, NULL, NULL);
    error_code = clEnqueueWriteBuffer(queue, buffer_b, CL_FALSE, 0, sizeof(float) * n, b, 0, NULL, NULL);

    // Execute kernel
    error_code = clSetKernelArg(kernel, 0, sizeof(buffer_a), &buffer_a);
    error_code = clSetKernelArg(kernel, 1, sizeof(buffer_b), &buffer_b);
    error_code = clSetKernelArg(kernel, 2, sizeof(buffer_c), &buffer_c);
    const size_t global_dimensions[] = {n, 0, 0};
    error_code = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL);

    // Read back results
    error_code = clEnqueueReadBuffer(queue, buffer_c, CL_FALSE, 0, sizeof(float) * n, c, 0, NULL, NULL);

    // Wait for everything to finish
    error_code = clFinish(queue);
}


int ray_triangle_intersect(const ray r, const float triangles, const int i) {

    return 0;
}