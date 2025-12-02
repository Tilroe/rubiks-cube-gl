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

void load_kernels();
void vec_add_test(const int n_items, float in[n_items]);

int opencl_init() {
    cl_int error_code_ret;
    if (!CL_CHECK(error_code_ret = clGetPlatformIDs(1, &platform, NULL))) return 0;
    if (!(CL_CHECK(error_code_ret = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1, &device, NULL))
        || CL_CHECK(error_code_ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL)))) return 0;
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;
    queue = clCreateCommandQueueWithProperties(context, device, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;

    vec_add_test(512, (float[512]){0.f});
    vec_add_test(512, (float[512]){0.f});

    return 1;
}


void vec_add_test(const int n_items, float in[n_items]) {
    static bool initialized = false;
    static cl_kernel kernel;
    static cl_mem buffer_in, buffer_out;
    cl_int error_code;

    if (!initialized) {
        // Compile kernel
        const char* program_source = read_file("../kernels/opencl/vec_add.cl");
        const cl_program program = clCreateProgramWithSource(context, 1, &program_source, NULL, &error_code);
        free(program_source);
        clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
        kernel = clCreateKernel(program, "vec_add", &error_code);

        // Create memory objects
        buffer_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n_items, NULL, &error_code);
        buffer_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n_items, NULL, &error_code);

        initialized = true;
    }

    // Copy data to input
    error_code = clEnqueueWriteBuffer(queue, buffer_in, CL_FALSE, 0, sizeof(cl_float) * n_items, in, 0, NULL, NULL);

    // Execute kernel
    error_code = clSetKernelArg(kernel, 0, sizeof(buffer_in), &buffer_in);
    error_code = clSetKernelArg(kernel, 1, sizeof(buffer_out), &buffer_out);
    const cl_float c = 1.0f;
    error_code = clSetKernelArg(kernel, 2, sizeof(float), &c);
    const size_t global_dimensions[] = {n_items, 0, 0};
    error_code = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL);

    // Read back results
    float out[n_items];
    error_code = clEnqueueReadBuffer(queue, buffer_out, CL_FALSE, 0, sizeof(cl_float) * n_items, out, 0, NULL, NULL);

    // Wait for everything to finish
    error_code = clFinish(queue);

    for (int i = 0; i < n_items; i++)
        printf("%d: %.1f\n", i, out[i]);
}


int ray_triangle_intersect(const ray r, const float triangles, const int i) {

    return 0;
}