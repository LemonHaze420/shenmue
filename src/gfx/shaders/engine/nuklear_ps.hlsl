#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

SamplerState s0 : register(s0);
Texture2D<float4> t8 : register(t8);

struct PixelInput {
    float4 inputColor0 : COLOR0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float4 inputColor0 = 0;
    float2 inputTexcoord0 = 0;
    uint4 r0 = 0;
    float4 v1 = 0;
    float2 v2 = 0;
    float4 _34 = 0;
    float2 _46 = 0;

    inputColor0 = input.inputColor0;
    inputTexcoord0 = input.inputTexcoord0;
    float4 _37 = _34;
    _37.x = inputColor0.x;
    float4 _39 = _37;
    _39.y = inputColor0.y;
    float4 _41 = _39;
    _41.z = inputColor0.z;
    float4 _43 = _41;
    _43.w = inputColor0.w;
    v1 = _43;
    float2 _49 = _46;
    _49.x = inputTexcoord0.x;
    float2 _51 = _49;
    _51.y = inputTexcoord0.y;
    v2 = _51;
    r0 = asuint(t8.Sample(s0, v2.xyxx.xy));
    o0 = asfloat(r0) * v1;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
