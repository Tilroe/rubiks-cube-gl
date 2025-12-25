#include "cl_kernels_context.h"
#include "cl_kernels_init.h"

#include "CL/opencl.h"

cl_platform_id platform;
cl_device_id device;
cl_context context;
cl_command_queue write_queue, kernel_queue, read_queue;

int cl_kernels_init() {
    cl_int error_code_ret;
    if (!CL_CHECK(error_code_ret = clGetPlatformIDs(1, &platform, NULL))) return 0;
    if (!(CL_CHECK(error_code_ret = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1, &device, NULL))
        || CL_CHECK(error_code_ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL)))) return 0;
    context         = clCreateContext(NULL, 1, &device, NULL, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;
    write_queue     = clCreateCommandQueueWithProperties(context, device, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;
    kernel_queue    = clCreateCommandQueueWithProperties(context, device, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;
    read_queue      = clCreateCommandQueueWithProperties(context, device, NULL, &error_code_ret); if (!CL_CHECK(error_code_ret)) return 0;

    kernel_vector_add_init();

    return 1;
}