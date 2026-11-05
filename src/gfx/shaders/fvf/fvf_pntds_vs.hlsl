#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb9_struct : register(b1) {
    float4 cb1_1_m0[9] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float2 inputTexcoord0 = 0;
    float4 outputColor0 = 0;
    float2 outputTexcoord0 = 0;
    uint4 r0 = 0;
    float3 v0 = 0;
    float4 v2 = 0;
    float2 v4 = 0;
    float4 o1 = 0;
    float2 o2 = 0;
    float3 _68 = 0;
    float4 _77 = 0;
    float2 _89 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _71 = _68;
    _71.x = inputPosition0.x;
    float3 _73 = _71;
    _73.y = inputPosition0.y;
    float3 _75 = _73;
    _75.z = inputPosition0.z;
    v0 = _75;
    float4 _80 = _77;
    _80.x = inputDiffuse0.x;
    float4 _82 = _80;
    _82.y = inputDiffuse0.y;
    float4 _84 = _82;
    _84.z = inputDiffuse0.z;
    float4 _86 = _84;
    _86.w = inputDiffuse0.w;
    v2 = _86;
    float2 _92 = _89;
    _92.x = inputTexcoord0.x;
    float2 _94 = _92;
    _94.y = inputTexcoord0.y;
    v4 = _94;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    position.w = dot(asfloat(r0), cb1_1_m0[8u]);
    o1 = v2;
    o2 = v4;
    outputColor0 = float4(o1);
    outputTexcoord0 = float2(o2);
    VertexOutput output;
    output.position = position;
    output.outputColor0 = outputColor0;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
