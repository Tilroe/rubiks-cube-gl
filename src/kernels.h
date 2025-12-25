#ifndef KERNELS_H
#define KERNELS_H

/*
 * opencl_init:
 * Sets up OpenCL by
 *      - finding platform, devices
 *      - Creating context, command queue
 */
int cl_kernels_init();

/*
 * vector_add:
 * Adds contents of vectors a and b such that c[n] = a[n] + b[n]
 */
void vector_add(int n, float a[n], float b[n], float c[n]);

#endif
