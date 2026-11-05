#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb4_struct : register(b0) {
    float4 cb0_0_m0[4] : packoffset(c0);
};

struct VertexInput {
    float2 inputPosition0 : POSITION0;
    float4 inputColor0 : COLOR0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float2 inputPosition0 = 0;
    float2 inputTexcoord0 = 0;
    float4 inputColor0 = 0;
    float4 outputColor0 = 0;
    float2 outputTexcoord0 = 0;
    uint4 r0 = 0;
    float2 v0 = 0;
    float2 v1 = 0;
    float4 v2 = 0;
    float4 o1 = 0;
    float2 o2 = 0;
    float2 _74 = 0;
    float2 _81 = 0;
    float4 _88 = 0;

    inputPosition0 = input.inputPosition0;
    inputTexcoord0 = input.inputTexcoord0;
    inputColor0 = input.inputColor0;
    float2 _77 = _74;
    _77.x = inputPosition0.x;
    float2 _79 = _77;
    _79.y = inputPosition0.y;
    v0 = _79;
    float2 _84 = _81;
    _84.x = inputTexcoord0.x;
    float2 _86 = _84;
    _86.y = inputTexcoord0.y;
    v1 = _86;
    float4 _91 = _88;
    _91.x = inputColor0.x;
    float4 _93 = _91;
    _93.y = inputColor0.y;
    float4 _95 = _93;
    _95.z = inputColor0.z;
    float4 _97 = _95;
    _97.w = inputColor0.w;
    v2 = _97;
    r0 = asuint(v0.yyyy * cb0_0_m0[1u]);
    r0 = asuint(mad(cb0_0_m0[0u], v0.xxxx, asfloat(r0)));
    position = asfloat(r0) + cb0_0_m0[3u];
    float3 _53 = log2(v2.xyz);
    r0 = uint4(asuint(_53.x), asuint(_53.y), asuint(_53.z), r0.w);
    float3 _60 = asfloat(r0.xyz) * 2.2000000476837158203125f.xxx;
    r0 = uint4(asuint(_60.x), asuint(_60.y), asuint(_60.z), r0.w);
    float3 _65 = exp2(asfloat(r0.xyz));
    o1 = float4(_65.x, _65.y, _65.z, o1.w);
    o1.w = v2.w;
    o2 = v1;
    outputColor0 = float4(o1);
    outputTexcoord0 = float2(o2);
    VertexOutput output;
    output.position = position;
    output.outputColor0 = outputColor0;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
