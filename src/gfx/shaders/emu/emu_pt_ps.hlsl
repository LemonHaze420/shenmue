#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

SamplerState s4 : register(s4);
Texture2D<float4> t0 : register(t0);

struct PixelInput {
    float2 inputTexcoord0 : TEXCOORD0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float2 inputTexcoord0 = 0;
    float2 v1 = 0;
    float2 _28 = 0;

    inputTexcoord0 = input.inputTexcoord0;
    float2 _31 = _28;
    _31.x = inputTexcoord0.x;
    float2 _33 = _31;
    _33.y = inputTexcoord0.y;
    v1 = _33;
    o0 = t0.Sample(s4, v1.xyxx.xy);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
