#include "mat_mul.cl"

constant float epsilon = 0.00001f;

kernel void moller_trumbore(
    global float *vertices,
    global float4 *model_mat,
    float3 origin,
    float3 dir,
    global bool *intersect)
{
    int id = get_global_id(0);

    float4 v0_4 = float4(vertices[id * 9 + 0], vertices[id * 9 + 1], vertices[id * 9 + 2], 1.0f);
    float4 v1_4 = float4(vertices[id * 9 + 3], vertices[id * 9 + 4], vertices[id * 9 + 5], 1.0f);
    float4 v2_4 = float4(vertices[id * 9 + 6], vertices[id * 9 + 7], vertices[id * 9 + 8], 1.0f);

    v0_4 = mat_mul(model_mat, v0_4);
    v1_4 = mat_mul(model_mat, v1_4);
    v2_4 = mat_mul(model_mat, v2_4);

    float3 v0 = as_float3(v0_4);
    float3 v1 = as_float3(v1_4);
    float3 v2 = as_float3(v2_4);

    float3 v0v1 = v1 - v0;
    float3 v0v2 = v2 - v0;
    float3 pvec = cross(dir, v0v2);
    float det = dot(pvec, v0v1);

    if ((det < epsilon) || (fabs(det) < epsilon)) { intersect[id] = false; return; }

    float inv_det = 1 / det;

    float3 tvec = origin - v0;
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) { intersect[id] = false; return; }

    float3 qvec = cross(tvec, v0v1);
    float v = dot(dir, qvec) * inv_det;
    if (v < 0 || u + v > 1) { intersect[id] = false; return; }

    intersect[id] = true;
}