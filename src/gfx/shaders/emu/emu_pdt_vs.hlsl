#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float4 outputDiffuse0 : DIFFUSE0;
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float2 inputTexcoord0 = 0;
    float4 outputDiffuse0 = 0;
    float2 outputTexcoord0 = 0;
    uint4 r0 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float2 v2 = 0;
    float4 o1 = 0;
    float2 o2 = 0;
    float3 _50 = 0;
    float4 _59 = 0;
    float2 _71 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _53 = _50;
    _53.x = inputPosition0.x;
    float3 _55 = _53;
    _55.y = inputPosition0.y;
    float3 _57 = _55;
    _57.z = inputPosition0.z;
    v0 = _57;
    float4 _62 = _59;
    _62.x = inputDiffuse0.x;
    float4 _64 = _62;
    _64.y = inputDiffuse0.y;
    float4 _66 = _64;
    _66.z = inputDiffuse0.z;
    float4 _68 = _66;
    _68.w = inputDiffuse0.w;
    v1 = _68;
    float2 _74 = _71;
    _74.x = inputTexcoord0.x;
    float2 _76 = _74;
    _76.y = inputTexcoord0.y;
    v2 = _76;
    float3 _34 = mad(v0, float3(0.00312500004656612873077392578125f, -0.0041666668839752674102783203125f, 1.0f), float3(-1.0f, 1.0f, 0.0f));
    position = float4(_34.x, _34.y, _34.z, position.w);
    position.w = 1.0f;
    r0 = asuint(log2(v1));
    r0 = asuint(asfloat(r0) * (2.2000000476837158203125f.xxxx));
    o1 = exp2(asfloat(r0));
    o2 = v2;
    outputDiffuse0 = float4(o1);
    outputTexcoord0 = float2(o2);
    VertexOutput output;
    output.position = position;
    output.outputDiffuse0 = outputDiffuse0;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
