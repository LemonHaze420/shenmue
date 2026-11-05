#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb9_struct : register(b1) {
    float4 cb1_1_m0[9] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float4 outputColor0 = 0;
    uint4 r0 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float4 o1 = 0;
    float3 _63 = 0;
    float4 _72 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    float3 _66 = _63;
    _66.x = inputPosition0.x;
    float3 _68 = _66;
    _68.y = inputPosition0.y;
    float3 _70 = _68;
    _70.z = inputPosition0.z;
    v0 = _70;
    float4 _75 = _72;
    _75.x = inputDiffuse0.x;
    float4 _77 = _75;
    _77.y = inputDiffuse0.y;
    float4 _79 = _77;
    _79.z = inputDiffuse0.z;
    float4 _81 = _79;
    _81.w = inputDiffuse0.w;
    v1 = _81;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    position.w = dot(asfloat(r0), cb1_1_m0[8u]);
    o1 = v1;
    outputColor0 = float4(o1);
    VertexOutput output;
    output.position = position;
    output.outputColor0 = outputColor0;
    return output;
}
#endif
