#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main() {
    float4 o0 = 0;

    o0 = 0.0f.xxxx;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
