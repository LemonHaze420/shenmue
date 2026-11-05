#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb21_struct : register(b1) {
    float4 cb1_1_m0[21] : packoffset(c0);
};

SamplerState s1 : register(s1);
Texture2D<float4> t8 : register(t8);

struct PixelInput {
    float4 inputColor0 : COLOR0;
    float4 inputProjscreenpos0 : PROJSCREENPOS0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float4 inputColor0 = 0;
    float4 inputProjscreenpos0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    float4 v1 = 0;
    float4 v2 = 0;
    float4 _555 = 0;
    float4 _566 = 0;

    inputColor0 = input.inputColor0;
    inputProjscreenpos0 = input.inputProjscreenpos0;
    float4 _558 = _555;
    _558.x = inputColor0.x;
    float4 _560 = _558;
    _560.y = inputColor0.y;
    float4 _562 = _560;
    _562.z = inputColor0.z;
    float4 _564 = _562;
    _564.w = inputColor0.w;
    v1 = _564;
    float4 _569 = _566;
    _569.x = inputProjscreenpos0.x;
    float4 _571 = _569;
    _571.y = inputProjscreenpos0.y;
    float4 _573 = _571;
    _573.z = inputProjscreenpos0.z;
    float4 _575 = _573;
    _575.w = inputProjscreenpos0.w;
    v2 = _575;
    float2 _36 = mad(v2.xy, float2(0.5f, -0.5f), 0.5f.xx);
    r0 = uint4(asuint(_36.x), asuint(_36.y), r0.z, r0.w);
    float2 _47 = asfloat(r0.xy) * cb1_1_m0[1u].yz;
    r0 = uint4(asuint(_47.x), asuint(_47.y), r0.z, r0.w);
    float2 _57 = t8.Sample(s1, asfloat(r0.xyxx).xy).xy;
    r0 = uint4(asuint(_57.x), asuint(_57.y), r0.z, r0.w);
    r0.x = asuint(asfloat(r0.x) + ((-cb1_1_m0[20u].xxxx).x));
    o0.w = asfloat(r0.y);
    r0.y = asuint((-cb1_1_m0[20u].xxxx).y + cb1_1_m0[20u].y);
    r0.x = asuint(asfloat(r0.x) / (asfloat(r0.y)));
    r0.x = asuint(asfloat(r0.x) * (cb1_1_m0[20u].z));
    r0.x = asuint(min(asfloat(r0.x), 255.9998931884765625f));
    r0.x = asuint(max(asfloat(r0.x), 1.0f));
    r0.y = asuint(log2(asfloat(r0.x)));
    uint _122 = 0u & 31u;
    r0.x = gfxInsertBits(1056964608u, r0.x, _122, min((23u & 31u), (32u - _122)));
    r0.x = asuint(asfloat(r0.x) * (512.0f));
    float2 _142 = max(asfloat(r0.xy), (-2147483648.0f).xx);
    bool2 _147 = bool2(_142.x >= 2147483648.0f.xx.x, _142.y >= 2147483648.0f.xx.y);
    int2 _148 = int2(_142);
    int2 _149 = int2(_147.x ? int2(2147483647, 2147483647).x : _148.x, _147.y ? int2(2147483647, 2147483647).y : _148.y);
    uint2 _577 = r0.xy & uint2(2147483647u, 2147483647u);
    bool2 _152 = bool2(_577.x > uint2(2139095040u, 2139095040u).x, _577.y > uint2(2139095040u, 2139095040u).y);
    uint2 _154 = uint2(int2(_152.x ? int2(0, 0).x : _149.x, _152.y ? int2(0, 0).y : _149.y));
    r0 = uint4(_154.x, _154.y, r0.z, r0.w);
    uint _161 = 4u & 31u;
    r0.z = gfxExtractBits(r0.x, _161, min((4u & 31u), (32u - _161)));
    r0.x = r0.x & 15u;
    r0.x = asuint(float(asint(r0.x)));
    r0.y = (asint(r0.y) * 16) + asint(r0.z);
    r0.z = asint(r0.y) + 1;
    float2 _201 = float2(asint(r0.yz));
    r0 = uint4(r0.x, asuint(_201.x), asuint(_201.y), r0.w);
    r0.z = asuint(min(asfloat(r0.z), 127.0f));
    float2 _214 = (-asfloat(r0.yyzy)).yz + 127.0f.xx;
    r0 = uint4(r0.x, asuint(_214.x), asuint(_214.y), r0.w);
    float _219 = max(asfloat(r0.z), -2147483648.0f);
    r0.w = ((r0.z & 2147483647u) > 2139095040u) ? 0 : ((_219 >= 2147483648.0f) ? 2147483647 : int(_219));
    r1.x = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
    r1.y = r1.x & 2139095040u;
    r1.y = asint(r1.y) + (-1056964608);
    r1.z = (!(asfloat(r1.x) == 0.0f)) ? 4294967295u : 0u;
    uint _260 = 0u & 31u;
    r1.x = gfxInsertBits(1056964608u, r1.x, _260, min((23u & 31u), (32u - _260)));
    uint2 _275 = uint2(r1.zz & r1.xy);
    r1 = uint4(_275.x, _275.y, r1.z, r1.w);
    r1.y = asint(r1.y) >> (23 & int(31u));
    r1.y = asint(r1.y) << (4 & int(31u));
    r1.x = asuint(asfloat(r1.x) * (32.0f));
    float _301 = max(asfloat(r1.x), -2147483648.0f);
    r1.x = ((r1.x & 2147483647u) > 2139095040u) ? 0 : ((_301 >= 2147483648.0f) ? 2147483647 : int(_301));
    uint2 _315 = uint2(asint(r1.xy) + int2(-16, -16));
    r1 = uint4(_315.x, _315.y, r1.z, r1.w);
    r1.x = r1.x | r1.y;
    r1.y = (asint(r0.w) < asint(r1.x)) ? 4294967295u : 0u;
    r0.w = (asint(r0.w) >= asint(r1.x)) ? 4294967295u : 0u;
    r1.z = asuint(float(asint(r1.x)));
    r1.w = asuint((-asfloat(r0.zzzz)).w + asfloat(r1.z));
    r0.z = asuint((-asfloat(r0.zzzz)).z + 128.0f);
    float2 _372 = asfloat(r0.xz) * float2(0.066666670143604278564453125f, 0.0078125f);
    r0 = uint4(asuint(_372.x), r0.y, asuint(_372.y), r0.w);
    r1.w = asuint(asfloat(r1.w) / (asfloat(r1.z)));
    r1.y = r1.w & r1.y;
    r0.w = asuint((r0.w != 0u) ? 0.0f : asfloat(r1.y));
    r1.y = asuint(cb1_1_m0[20u].y * 0.00390625f);
    r1.y = (asfloat(r1.y) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
    r0.z = asuint((r1.y != 0u) ? asfloat(r0.z) : asfloat(r0.w));
    r0.w = asuint((-asfloat(r0.yyyy)).w + asfloat(r1.z));
    r0.w = asuint(asfloat(r0.w) / (asfloat(r1.z)));
    float _439 = max(asfloat(r0.y), -2147483648.0f);
    r1.z = ((r0.y & 2147483647u) > 2139095040u) ? 0 : ((_439 >= 2147483648.0f) ? 2147483647 : int(_439));
    r0.y = asuint((-asfloat(r0.yyyy)).y + 128.0f);
    r0.y = asuint(asfloat(r0.y) * (0.0078125f));
    r1.w = (asint(r1.z) < asint(r1.x)) ? 4294967295u : 0u;
    r1.x = (asint(r1.z) >= asint(r1.x)) ? 4294967295u : 0u;
    r0.w = r0.w & r1.w;
    r0.w = asuint((r1.x != 0u) ? 0.0f : asfloat(r0.w));
    r0.y = asuint((r1.y != 0u) ? asfloat(r0.y) : asfloat(r0.w));
    r0.z = asuint((-asfloat(r0.yyyy)).z + asfloat(r0.z));
    r0.x = asuint(mad(asfloat(r0.x), asfloat(r0.z), asfloat(r0.y)));
    r0.y = asuint(asfloat(r0.x) * cb1_1_m0[0u].w);
    r0.x = asuint(mad((-asfloat(r0.xxxx)).x, cb1_1_m0[0u].w, 1.0f));
    float3 _542 = asfloat(r0.yyy) * cb1_1_m0[0u].xyz;
    r0 = uint4(r0.x, asuint(_542.x), asuint(_542.y), asuint(_542.z));
    float3 _551 = mad(asfloat(r0.xxx), v1.xyz, asfloat(r0.yzw));
    o0 = float4(_551.x, _551.y, _551.z, o0.w);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
