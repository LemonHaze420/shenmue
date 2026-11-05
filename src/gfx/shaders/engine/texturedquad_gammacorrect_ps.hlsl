#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

SamplerState s0 : register(s0);
Texture2D<float4> t8 : register(t8);

struct PixelInput {
    float2 inputTexcoord0 : TEXCOORD0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float2 inputTexcoord0 = 0;
    uint4 r0 = 0;
    float2 v1 = 0;
    float2 _33 = 0;

    inputTexcoord0 = input.inputTexcoord0;
    float2 _36 = _33;
    _36.x = inputTexcoord0.x;
    float2 _38 = _36;
    _38.y = inputTexcoord0.y;
    v1 = _38;
    r0 = asuint(t8.Sample(s0, v1.xyxx.xy));
    o0 = asfloat(r0) * asfloat(r0);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
