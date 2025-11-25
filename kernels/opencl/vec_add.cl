kernel void vec_add(global float *in, global float *out, private float c) {
    int id = get_global_id(0);
    out[id] = in[id] + 2.5f + c;
}