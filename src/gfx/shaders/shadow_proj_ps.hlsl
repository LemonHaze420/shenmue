#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb22_struct : register(b1) {
    float4 cb1_1_m0[22] : packoffset(c0);
};

SamplerState s0 : register(s0);
Texture2D<float4> t0 : register(t0);

struct PixelInput {
    float2 inputTexcoord0 : TEXCOORD0;
    float4 fragmentPosition : SV_Position;
};

struct PixelOutput {
    float2 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 fragmentPosition = 0;
    float2 o0 = 0;
    float2 inputTexcoord0 = 0;
    uint4 r0 = 0;
    float4 v0 = 0;
    float2 v1 = 0;
    float2 _64 = 0;

    fragmentPosition = input.fragmentPosition;
    fragmentPosition.w = 1.0 / fragmentPosition.w;
    inputTexcoord0 = input.inputTexcoord0;
    float2 _67 = _64;
    _67.x = inputTexcoord0.x;
    float2 _69 = _67;
    _69.y = inputTexcoord0.y;
    v1 = _69;
    float4 _29 = fragmentPosition;
    _29.w = 1.0f / fragmentPosition.w;
    v0 = _29;
    r0.x = asuint(t0.Sample(s0, v1.xyxx.xy).w);
    o0.y = asfloat(r0.x) * cb1_1_m0[21u].w;
    o0.x = v0.w;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
