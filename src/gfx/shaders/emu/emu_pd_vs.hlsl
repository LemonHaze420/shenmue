#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
};

struct VertexOutput {
    float4 outputDiffuse0 : DIFFUSE0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float4 outputDiffuse0 = 0;
    uint4 r0 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float4 o1 = 0;
    float3 _45 = 0;
    float4 _54 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    float3 _48 = _45;
    _48.x = inputPosition0.x;
    float3 _50 = _48;
    _50.y = inputPosition0.y;
    float3 _52 = _50;
    _52.z = inputPosition0.z;
    v0 = _52;
    float4 _57 = _54;
    _57.x = inputDiffuse0.x;
    float4 _59 = _57;
    _59.y = inputDiffuse0.y;
    float4 _61 = _59;
    _61.z = inputDiffuse0.z;
    float4 _63 = _61;
    _63.w = inputDiffuse0.w;
    v1 = _63;
    float3 _29 = mad(v0, float3(0.00312500004656612873077392578125f, -0.0041666668839752674102783203125f, 1.0f), float3(-1.0f, 1.0f, 0.0f));
    position = float4(_29.x, _29.y, _29.z, position.w);
    position.w = 1.0f;
    r0 = asuint(log2(v1));
    r0 = asuint(asfloat(r0) * (2.2000000476837158203125f.xxxx));
    o1 = exp2(asfloat(r0));
    outputDiffuse0 = float4(o1);
    VertexOutput output;
    output.position = position;
    output.outputDiffuse0 = outputDiffuse0;
    return output;
}
#endif
