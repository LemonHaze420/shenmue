#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

SamplerState s0 : register(s0);
Texture2D<float4> t0 : register(t0);
Texture2D<float4> t1 : register(t1);
Texture2D<float4> t2 : register(t2);
Texture2D<float4> t3 : register(t3);

struct PixelInput {
    float2 inputTexcoord0 : TEXCOORD0;
    float2 inputTexcoord1 : TEXCOORD1;
    float2 inputTexcoord2 : TEXCOORD2;
    float2 inputTexcoord3 : TEXCOORD3;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float2 inputTexcoord0 = 0;
    float2 inputTexcoord1 = 0;
    float2 inputTexcoord2 = 0;
    float2 inputTexcoord3 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    float4 v1 = 0;
    float4 v2 = 0;
    float4 _72 = 0;
    float4 _85 = 0;

    inputTexcoord0 = input.inputTexcoord0;
    inputTexcoord1 = input.inputTexcoord1;
    inputTexcoord2 = input.inputTexcoord2;
    inputTexcoord3 = input.inputTexcoord3;
    float4 _75 = _72;
    _75.x = inputTexcoord0.x;
    float4 _77 = _75;
    _77.y = inputTexcoord0.y;
    float4 _80 = _77;
    _80.z = inputTexcoord1.x;
    float4 _82 = _80;
    _82.w = inputTexcoord1.y;
    v1 = _82;
    float4 _88 = _85;
    _88.x = inputTexcoord2.x;
    float4 _90 = _88;
    _90.y = inputTexcoord2.y;
    float4 _93 = _90;
    _93.z = inputTexcoord3.x;
    float4 _95 = _93;
    _95.w = inputTexcoord3.y;
    v2 = _95;
    float3 _33 = t0.Sample(s0, v1.xyxx.xy).xyz;
    r0 = uint4(asuint(_33.x), asuint(_33.y), asuint(_33.z), r0.w);
    r1 = asuint(t1.Sample(s0, v1.zwzz.xy));
    r0.w = asuint(1.0f);
    r0 = asuint(asfloat(r0) * (asfloat(r1)));
    r1 = asuint(t2.Sample(s0, v2.xyxx.xy));
    r0 = asuint(asfloat(r0) * (asfloat(r1)));
    r1 = asuint(t3.Sample(s0, v2.zwzz.xy));
    o0 = asfloat(r0) * asfloat(r1);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
