#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb9_struct : register(b1) {
    float4 cb1_1_m0[9] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float2 inputTexcoord0 = 0;
    float2 outputTexcoord0 = 0;
    uint4 r0 = 0;
    float3 v0 = 0;
    float2 v2 = 0;
    float2 o1 = 0;
    float3 _65 = 0;
    float2 _75 = 0;

    inputPosition0 = input.inputPosition0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _68 = _65;
    _68.x = inputPosition0.x;
    float3 _70 = _68;
    _70.y = inputPosition0.y;
    float3 _72 = _70;
    _72.z = inputPosition0.z;
    v0 = _72;
    float2 _78 = _75;
    _78.x = inputTexcoord0.x;
    float2 _80 = _78;
    _80.y = inputTexcoord0.y;
    v2 = _80;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    position.w = dot(asfloat(r0), cb1_1_m0[8u]);
    o1 = v2;
    outputTexcoord0 = float2(o1);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
