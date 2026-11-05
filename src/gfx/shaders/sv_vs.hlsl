#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb9_struct : register(b1) {
    float4 cb1_1_m0[9] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 outputColor0 = 0;
    uint4 r0 = 0;
    float3 v0 = 0;
    float4 o1 = 0;
    float3 _61 = 0;

    inputPosition0 = input.inputPosition0;
    float3 _64 = _61;
    _64.x = inputPosition0.x;
    float3 _66 = _64;
    _66.y = inputPosition0.y;
    float3 _68 = _66;
    _68.z = inputPosition0.z;
    v0 = _68;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    position.w = dot(asfloat(r0), cb1_1_m0[8u]);
    outputColor0 = float4(o1);
    VertexOutput output;
    output.position = position;
    output.outputColor0 = outputColor0;
    return output;
}
#endif
