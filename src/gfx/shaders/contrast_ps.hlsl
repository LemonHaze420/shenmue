#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb1_struct : register(b4) {
    float4 cb4_4_m0[1] : packoffset(c0);
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main() {
    float4 o0 = 0;

    o0 = float4(cb4_4_m0[0u].x, cb4_4_m0[0u].x, cb4_4_m0[0u].x, o0.w);
    o0.w = 1.0f;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
