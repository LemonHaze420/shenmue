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
    float4 _22 = 0;

    inputColor0 = input.inputColor0;
    float4 _25 = _22;
    _25.x = inputColor0.x;
    float4 _27 = _25;
    _27.y = inputColor0.y;
    float4 _29 = _27;
    _29.z = inputColor0.z;
    float4 _31 = _29;
    _31.w = inputColor0.w;
    v1 = _31;
    o0 = float4(v1.x, v1.y, v1.z, o0.w);
    o0.w = 1.0f;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
