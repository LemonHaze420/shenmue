#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

SamplerState s4 : register(s4);
Texture2D<float4> t0 : register(t0);

struct PixelInput {
    float4 inputDiffuse0 : DIFFUSE0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float4 inputDiffuse0 = 0;
    float2 inputTexcoord0 = 0;
    uint4 r0 = 0;
    float4 v1 = 0;
    float2 v2 = 0;
    float4 _35 = 0;
    float2 _47 = 0;

    inputDiffuse0 = input.inputDiffuse0;
    inputTexcoord0 = input.inputTexcoord0;
    float4 _38 = _35;
    _38.x = inputDiffuse0.x;
    float4 _40 = _38;
    _40.y = inputDiffuse0.y;
    float4 _42 = _40;
    _42.z = inputDiffuse0.z;
    float4 _44 = _42;
    _44.w = inputDiffuse0.w;
    v1 = _44;
    float2 _50 = _47;
    _50.x = inputTexcoord0.x;
    float2 _52 = _50;
    _52.y = inputTexcoord0.y;
    v2 = _52;
    r0 = asuint(t0.Sample(s4, v2.xyxx.xy));
    o0 = asfloat(r0.wzyx) * v1;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
