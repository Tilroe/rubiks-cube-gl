float4 mat_mul(
    global float4 *mat,
    float4 vec)
{
    return (float4) (dot(mat[0], vec_in), dot(mat[1], vec_in), dot(mat[2], vec_in), dot(mat[3], vec_in));
}