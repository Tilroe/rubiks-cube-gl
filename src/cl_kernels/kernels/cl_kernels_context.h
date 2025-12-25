#ifndef CL_CONTEXT_H
#define CL_CONTEXT_H

#ifndef KERNEL_SOURCE_DIR
#error "KERNEL_SOURCE_DIR not specified"
#endif

#include "CL/opencl.h"

#define CL_CHECK(exp) ((exp) == CL_SUCCESS)

extern cl_context context;
extern cl_command_queue write_queue, kernel_queue, read_queue;

#endif //CL_CONTEXT_H