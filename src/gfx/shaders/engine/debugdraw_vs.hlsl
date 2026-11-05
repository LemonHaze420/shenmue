#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb16_struct : register(b0) {
    float4 cb0_0_m0[16] : packoffset(c0);
};

cbuffer cb4_struct : register(b1) {
    float4 cb1_1_m0[4] : packoffset(c0);
};

struct VertexInput {
    float4 inputPosition0 : POSITION0;
    float4 inputColor0 : COLOR0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float4 inputPosition0 = 0;
    float4 inputColor0 = 0;
    float4 outputColor0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    float4 v0 = 0;
    float4 v1 = 0;
    float4 o1 = 0;
    float4 _86 = 0;
    float4 _97 = 0;

    inputPosition0 = input.inputPosition0;
    inputColor0 = input.inputColor0;
    float4 _89 = _86;
    _89.x = inputPosition0.x;
    float4 _91 = _89;
    _91.y = inputPosition0.y;
    float4 _93 = _91;
    _93.z = inputPosition0.z;
    float4 _95 = _93;
    _95.w = inputPosition0.w;
    v0 = _95;
    float4 _100 = _97;
    _100.x = inputColor0.x;
    float4 _102 = _100;
    _102.y = inputColor0.y;
    float4 _104 = _102;
    _104.z = inputColor0.z;
    float4 _106 = _104;
    _106.w = inputColor0.w;
    v1 = _106;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    r1.x = asuint(dot(asfloat(r0), cb1_1_m0[0u]));
    r1.y = asuint(dot(asfloat(r0), cb1_1_m0[1u]));
    r1.z = asuint(dot(asfloat(r0), cb1_1_m0[2u]));
    r1.w = asuint(dot(asfloat(r0), cb1_1_m0[3u]));
    position.x = dot(asfloat(r1), cb0_0_m0[12u]);
    position.y = dot(asfloat(r1), cb0_0_m0[13u]);
    position.z = dot(asfloat(r1), cb0_0_m0[14u]);
    position.w = dot(asfloat(r1), cb0_0_m0[15u]);
    o1 = v1;
    outputColor0 = float4(o1);
    VertexOutput output;
    output.position = position;
    output.outputColor0 = outputColor0;
    return output;
}
#endif
