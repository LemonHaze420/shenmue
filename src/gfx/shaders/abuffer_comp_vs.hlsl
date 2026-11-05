#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb1_struct : register(b4) {
    float4 cb4_4_m0[1] : packoffset(c0);
};

struct VertexInput {
    uint vertexId : SV_VertexID;
};

struct VertexOutput {
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    int vertexId = 0;
    float2 outputTexcoord0 = 0;
    uint4 r0 = 0;
    float4 v0 = 0;
    float2 o1 = 0;

    vertexId = int(input.vertexId);
    v0.x = asfloat(vertexId - 0);
    r0.x = asuint(v0.x) >> (1u & 31u);
    r0.x = asuint(float(r0.x));
    position.x = mad(asfloat(r0.x), 4.0f, -1.0f);
    o1.x = dot(asfloat(r0.xx), cb4_4_m0[0u].yy);
    r0.x = asuint(v0.x) & 1u;
    r0.x = asuint(float(r0.x));
    position.y = mad(asfloat(r0.x), 4.0f, -1.0f);
    r0.x = asuint(mad((-asfloat(r0.xxxx)).x, 2.0f, 1.0f));
    o1.y = asfloat(r0.x) * cb4_4_m0[0u].z;
    position = float4(position.x, position.y, float2(0.0f, 1.0f).x, float2(0.0f, 1.0f).y);
    outputTexcoord0 = float2(o1);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
