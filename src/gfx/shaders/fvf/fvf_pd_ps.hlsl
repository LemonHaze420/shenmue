#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct PixelInput {
    float4 inputColor0 : COLOR0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float4 inputColor0 = 0;
    float4 v1 = 0;
    float4 _14 = 0;

    inputColor0 = input.inputColor0;
    float4 _17 = _14;
    _17.x = inputColor0.x;
    float4 _19 = _17;
    _19.y = inputColor0.y;
    float4 _21 = _19;
    _21.z = inputColor0.z;
    float4 _23 = _21;
    _23.w = inputColor0.w;
    v1 = _23;
    o0 = v1;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
