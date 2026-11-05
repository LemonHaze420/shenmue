#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb1_struct : register(b4) {
    float4 cb4_4_m0[1] : packoffset(c0);
};

SamplerState s0 : register(s0);
Texture2D<float4> t8 : register(t8);
RWTexture2DArray<uint> u2 : register(u2);
RWTexture2DArray<float> u3 : register(u3);
RWTexture2D<uint> u4 : register(u4);

struct PixelInput {
    float2 inputTexcoord0 : TEXCOORD0;
    float4 fragmentPosition : SV_Position;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

PixelOutput main(PixelInput input) {
    float4 fragmentPosition = 0;
    float4 o0 = 0;
    float2 inputTexcoord0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    uint4 r3 = 0;
    uint4 r4 = 0;
    uint4 r5 = 0;
    float4 v0 = 0;
    uint4 x0[16] = {(uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0,
        (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0, (uint4)0};
    float2 v1 = 0;
    float2 _459 = 0;

    fragmentPosition = input.fragmentPosition;
    fragmentPosition.w = 1.0 / fragmentPosition.w;
    inputTexcoord0 = input.inputTexcoord0;
    float2 _462 = _459;
    _462.x = inputTexcoord0.x;
    float2 _464 = _462;
    _464.y = inputTexcoord0.y;
    v1 = _464;
    float4 _47 = fragmentPosition;
    _47.w = 1.0f / fragmentPosition.w;
    v0 = _47;
    float2 _62 = max(v0.xy, (-2147483648.0f).xx);
    bool2 _67 = bool2(_62.x >= 2147483648.0f.xx.x, _62.y >= 2147483648.0f.xx.y);
    int2 _68 = int2(_62);
    int2 _69 = int2(_67.x ? int2(2147483647, 2147483647).x : _68.x, _67.y ? int2(2147483647, 2147483647).y : _68.y);
    uint2 _466 = asuint(v0.xy) & uint2(2147483647u, 2147483647u);
    bool2 _72 = bool2(_466.x > uint2(2139095040u, 2139095040u).x, _466.y > uint2(2139095040u, 2139095040u).y);
    uint2 _74 = uint2(int2(_72.x ? int2(0, 0).x : _69.x, _72.y ? int2(0, 0).y : _69.y));
    r0 = uint4(r0.x, _74.x, _74.y, r0.w);
    r1.x = u4[asint(r0.yz)].xxxx.x;
    r1.x = min(r1.x, 16u);
    x0[0u].x = asuint(0.0f);
    x0[1u].x = 0x00000001u;
    x0[2u].x = 0x00000002u;
    x0[3u].x = 0x00000003u;
    x0[4u].x = 0x00000004u;
    x0[5u].x = 0x00000005u;
    x0[6u].x = 0x00000006u;
    x0[7u].x = 0x00000007u;
    x0[8u].x = 0x00000008u;
    x0[9u].x = 0x00000009u;
    x0[10u].x = 0x0000000au;
    x0[11u].x = 0x0000000bu;
    x0[12u].x = 0x0000000cu;
    x0[13u].x = 0x0000000du;
    x0[14u].x = 0x0000000eu;
    x0[15u].x = 0x0000000fu;
    r2 = uint4(r2.x, r0.y, r0.z, r2.w);
    r1.y = 0x00000001u;
    [loop] for (;;) {
        r1.z = (asint(r1.y) >= asint(r1.x)) ? 4294967295u : 0u;
        if (r1.z != 0u) {
            break;
        }
        r0.x = x0[asint(r1.y)].x;
        r1.z = asuint(u3[asint(r0.yzx)].xxxx.x);
        uint2 _203 = uint2(asint(r1.yy) + int2(-1, 1));
        r3 = uint4(_203.x, _203.y, r3.z, r3.w);
        r0.w = x0[asint(r3.x)].x;
        r0.w = asuint(u3[asint(r0.yzw)].xxxx.x);
        r1.w = r1.y;
        r2.w = r0.w;
        [loop] for (;;) {
            r3.x = (0 < asint(r1.w)) ? 4294967295u : 0u;
            r3.z = (asfloat(r1.z) < asfloat(r2.w)) ? 4294967295u : 0u;
            r3.x = r3.z & r3.x;
            if (!(r3.x != 0u)) {
                break;
            }
            r3.x = asint(r1.w) + (-1);
            r3.z = x0[asint(r3.x)].x;
            x0[asint(r1.w)].x = r3.z;
            r3.z = (0 < asint(r3.x)) ? 4294967295u : 0u;
            if (r3.z != 0u) {
                r3.z = asint(r1.w) + (-2);
                r2.x = x0[asint(r3.z)].x;
                r2.w = asuint(u3[asint(r2.yzx)].xxxx.x);
            }
            r1.w = r3.x;
            continue;
        }
        x0[asint(r1.w)].x = r0.x;
        r1.y = r3.y;
        continue;
    }
    r2 = asuint(t8.Sample(s0, v1.xyxx.xy));
    r3 = r2;
    r0.w = asuint(0.0f);
    [loop] for (;;) {
        r1.y = (asint(r0.w) >= asint(r1.x)) ? 4294967295u : 0u;
        if (r1.y != 0u) {
            break;
        }
        r0.x = x0[asint(r0.w)].x;
        r0.x = u2[asint(r0.yzx)].xxxx.x;
        r1.y = r0.x & 255u;
        r1.y = asuint(float(r1.y));
        r4.x = asuint(asfloat(r1.y) * 0.0039215688593685626983642578125f);
        uint _390 = 8u & 31u;
        uint _398 = 16u & 31u;
        uint2 _405 = uint2(gfxExtractBits(r0.x, _390, min((8u & 31u), (32u - _390))), gfxExtractBits(r0.x, _398, min((8u & 31u), (32u - _398))));
        r1 = uint4(r1.x, _405.x, _405.y, r1.w);
        float2 _411 = float2(r1.yz);
        r1 = uint4(r1.x, asuint(_411.x), asuint(_411.y), r1.w);
        float2 _417 = asfloat(r1.yz) * 0.0039215688593685626983642578125f.xx;
        r4 = uint4(r4.x, asuint(_417.x), asuint(_417.y), r4.w);
        r0.x = r0.x >> (24u & 31u);
        r0.x = asuint(float(r0.x));
        r4.w = asuint(asfloat(r0.x) * 0.0039215688593685626983642578125f);
        r5 = asuint((-asfloat(r3)) + asfloat(r4));
        r3 = asuint(mad(asfloat(r4.wwww), asfloat(r5), asfloat(r3)));
        r0.w = asint(r0.w) + 1;
        continue;
    }
    o0 = asfloat(r3);
    u4[asint(r0.yz)] = uint4(0u, 0u, 0u, 0u).x;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
