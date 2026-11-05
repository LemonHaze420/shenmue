#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb77_struct : register(b1) {
    float4 cb1_1_m0[77] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float3 inputNormal0 : NORMAL0;
    float4 inputDiffuse0 : DIFFUSE0;
    float4 inputDiffuse1 : DIFFUSE1;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float outputFog0 : FOG0;
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float3 inputNormal0 = 0;
    float2 inputTexcoord0 = 0;
    float4 inputDiffuse0 = 0;
    float4 inputDiffuse1 = 0;
    float2 outputTexcoord0 = 0;
    float outputFog0 = 0;
    float4 outputColor0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    uint4 r3 = 0;
    uint4 r4 = 0;
    uint4 r5 = 0;
    uint4 r6 = 0;
    float3 v0 = 0;
    float3 v1 = 0;
    float2 v2 = 0;
    float4 v3 = 0;
    float4 v4 = 0;
    float3 o1 = 0;
    float4 o2 = 0;
    float3 _1518 = 0;
    float3 _1527 = 0;
    float2 _1537 = 0;
    float4 _1544 = 0;
    float4 _1555 = 0;

    inputPosition0 = input.inputPosition0;
    inputNormal0 = input.inputNormal0;
    inputTexcoord0 = input.inputTexcoord0;
    inputDiffuse0 = input.inputDiffuse0;
    inputDiffuse1 = input.inputDiffuse1;
    float3 _1521 = _1518;
    _1521.x = inputPosition0.x;
    float3 _1523 = _1521;
    _1523.y = inputPosition0.y;
    float3 _1525 = _1523;
    _1525.z = inputPosition0.z;
    v0 = _1525;
    float3 _1530 = _1527;
    _1530.x = inputNormal0.x;
    float3 _1532 = _1530;
    _1532.y = inputNormal0.y;
    float3 _1534 = _1532;
    _1534.z = inputNormal0.z;
    v1 = _1534;
    float2 _1540 = _1537;
    _1540.x = inputTexcoord0.x;
    float2 _1542 = _1540;
    _1542.y = inputTexcoord0.y;
    v2 = _1542;
    float4 _1547 = _1544;
    _1547.x = inputDiffuse0.x;
    float4 _1549 = _1547;
    _1549.y = inputDiffuse0.y;
    float4 _1551 = _1549;
    _1551.z = inputDiffuse0.z;
    float4 _1553 = _1551;
    _1553.w = inputDiffuse0.w;
    v3 = _1553;
    float4 _1558 = _1555;
    _1558.x = inputDiffuse1.x;
    float4 _1560 = _1558;
    _1560.y = inputDiffuse1.y;
    float4 _1562 = _1560;
    _1562.z = inputDiffuse1.z;
    float4 _1564 = _1562;
    _1564.w = inputDiffuse1.w;
    v4 = _1564;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    r0.x = asuint(dot(asfloat(r0), cb1_1_m0[8u]));
    int3 _80 = asint(cb1_1_m0[4u].xxx);
    bool3 _87 = bool3(_80.x == int3(2, 0, 3).x, _80.y == int3(2, 0, 3).y, _80.z == int3(2, 0, 3).z);
    uint3 _93 = uint3(_87.x ? uint3(4294967295u, 4294967295u, 4294967295u).x : uint3(0u, 0u, 0u).x,
        _87.y ? uint3(4294967295u, 4294967295u, 4294967295u).y : uint3(0u, 0u, 0u).y,
        _87.z ? uint3(4294967295u, 4294967295u, 4294967295u).z : uint3(0u, 0u, 0u).z);
    r0 = uint4(r0.x, _93.x, _93.y, _93.z);
    if (asuint(cb1_1_m0[4u].y) != 0u) {
        r1 = uint4(asuint(1.0f.xxx.x), asuint(1.0f.xxx.y), asuint(1.0f.xxx.z), r1.w);
    } else {
        int2 _109 = asint(cb1_1_m0[4u].xx);
        bool2 _113 = bool2(_109.x == int2(0, 1).x, _109.y == int2(0, 1).y);
        uint2 _118 = uint2(_113.x ? uint2(4294967295u, 4294967295u).x : uint2(0u, 0u).x, _113.y ? uint2(4294967295u, 4294967295u).y : uint2(0u, 0u).y);
        r1 = uint4(_118.x, _118.y, r1.z, r1.w);
        r1.x = r1.y | r1.x;
        if (r1.x != 0u) {
            float3 _143 = min(cb1_1_m0[22u].xyz, 1.0f.xxx);
            r1 = uint4(asuint(_143.x), asuint(_143.y), asuint(_143.z), r1.w);
            r2 = uint4(r1.x, r1.y, r1.z, r2.w);
            r1.w = asuint(0.0f);
            [loop] for (;;) {
                r2.w = asuint(float(r1.w));
                r2.w = (asfloat(r2.w) >= cb1_1_m0[23u].x) ? 4294967295u : 0u;
                if (r2.w != 0u) {
                    break;
                }
                r2.w = asint(r1.w) << (1 & int(31u));
                r3.x = asuint(dot(cb1_1_m0[uint(asint(r2.w)) + 24u].xyz, cb1_1_m0[uint(asint(r2.w)) + 24u].xyz));
                r3.x = asuint(rsqrt(asfloat(r3.x)));
                float3 _213 = asfloat(r3.xxx) * cb1_1_m0[uint(asint(r2.w)) + 24u].xyz;
                r3 = uint4(asuint(_213.x), asuint(_213.y), asuint(_213.z), r3.w);
                r3.x = asuint(dot(v1, asfloat(r3.xyz)));
                r3.y = (0.0f < asfloat(r3.x)) ? 4294967295u : 0u;
                if (r3.y != 0u) {
                    float3 _241 = min(1.0f.xxx, cb1_1_m0[uint(asint(r2.w)) + 25u].xyz);
                    r4 = uint4(asuint(_241.x), asuint(_241.y), asuint(_241.z), r4.w);
                    float3 _248 = asfloat(r3.xxx) * asfloat(r4.xyz);
                    r3 = uint4(asuint(_248.x), r3.y, asuint(_248.y), asuint(_248.z));
                }
                uint3 _258 = uint3(r3.xzw & r3.yyy);
                r3 = uint4(_258.x, _258.y, _258.z, r3.w);
                float3 _265 = asfloat(r2.xyz) + asfloat(r3.xyz);
                r2 = uint4(asuint(_265.x), asuint(_265.y), asuint(_265.z), r2.w);
                r1.w = asint(r1.w) + 1;
                continue;
            }
            r1.x = (v4.w == 0.0f) ? 4294967295u : 0u;
            if (r1.x != 0u) {
                r1 = uint4(r2.x, r2.y, r2.z, r1.w);
                r1.w = asuint(0.0f);
                [loop] for (;;) {
                    r2.w = asuint(float(r1.w));
                    r2.w = (asfloat(r2.w) >= cb1_1_m0[23u].y) ? 4294967295u : 0u;
                    if (r2.w != 0u) {
                        break;
                    }
                    r3 = uint4(65535u, 65535u, 65535u, 65535u) & asuint(cb1_1_m0[uint(asint(r1.w)) + 30u]);
                    r4 = asuint(cb1_1_m0[uint(asint(r1.w)) + 30u]) >> (uint4(16u, 16u, 16u, 16u) & uint4(31u, 31u, 31u, 31u));
                    r3 = uint4(asuint(gfxUnpackHalf2(r3.x).x), asuint(gfxUnpackHalf2(r3.y).x), asuint(gfxUnpackHalf2(r3.z).x), asuint(gfxUnpackHalf2(r3.w).x));
                    r4 = uint4(asuint(gfxUnpackHalf2(r4.x).x), asuint(gfxUnpackHalf2(r4.z).x), asuint(gfxUnpackHalf2(r4.y).x), asuint(gfxUnpackHalf2(r4.w).x));
                    r5 = uint4(r3.x, r5.y, r3.y, r5.w);
                    r5.y = r4.x;
                    r2.w = asuint(dot(asfloat(r5.xyz), cb1_1_m0[17u].xyz));
                    r6.x = asuint(asfloat(r2.w) + cb1_1_m0[17u].w);
                    r2.w = asuint(dot(asfloat(r5.xyz), cb1_1_m0[18u].xyz));
                    r6.y = asuint(asfloat(r2.w) + cb1_1_m0[18u].w);
                    r2.w = asuint(dot(asfloat(r5.xyz), cb1_1_m0[19u].xyz));
                    r6.z = asuint(asfloat(r2.w) + cb1_1_m0[19u].w);
                    float3 _439 = asfloat(r6.xyz) + (-v0.xyzx).xyz;
                    r5 = uint4(asuint(_439.x), asuint(_439.y), asuint(_439.z), r5.w);
                    r2.w = asuint(dot(asfloat(r5.xyz), asfloat(r5.xyz)));
                    float2 _452 = asfloat(r4.wz) * asfloat(r4.wz);
                    r3 = uint4(asuint(_452.x), asuint(_452.y), r3.z, r3.w);
                    r3.x = (asfloat(r3.x) >= asfloat(r2.w)) ? 4294967295u : 0u;
                    r2.w = asuint(sqrt(asfloat(r2.w)));
                    r2.w = asuint(1.0f / asfloat(r2.w));
                    r2.w = asuint((r3.x != 0u) ? asfloat(r2.w) : (-1.0f));
                    r3.x = (0.0f < asfloat(r2.w)) ? 4294967295u : 0u;
                    r4.w = asuint(dot(v1, asfloat(r5.xyz)));
                    r5.x = (0.0f < asfloat(r4.w)) ? 4294967295u : 0u;
                    r4.w = asuint(asfloat(r2.w) * asfloat(r4.w));
                    r2.w = asuint(asfloat(r2.w) * (asfloat(r2.w)));
                    r2.w = asuint(asfloat(r2.w) * (asfloat(r2.w)));
                    r2.w = asuint(asfloat(r2.w) * (asfloat(r2.w)));
                    r3.y = asuint(asfloat(r3.y) * (asfloat(r3.y)));
                    r3.y = asuint(asfloat(r3.y) * (asfloat(r3.y)));
                    r2.w = asuint(asfloat(r2.w) * (asfloat(r3.y)));
                    r3.y = (1.0f < asfloat(r2.w)) ? 4294967295u : 0u;
                    r2.w = asuint((r3.y != 0u) ? 1.0f : asfloat(r2.w));
                    r2.w = asuint(asfloat(r2.w) * (asfloat(r4.w)));
                    r4 = uint4(r3.z, r4.y, r3.w, r4.w);
                    float3 _564 = min(asfloat(r4.xyz), 1.0f.xxx);
                    r3 = uint4(r3.x, asuint(_564.x), asuint(_564.y), asuint(_564.z));
                    float3 _571 = asfloat(r2.www) * asfloat(r3.yzw);
                    r3 = uint4(r3.x, asuint(_571.x), asuint(_571.y), asuint(_571.z));
                    r2.w = r3.x & r5.x;
                    uint3 _590 = uint3(r3.yzw & r2.www);
                    r3 = uint4(_590.x, _590.y, _590.z, r3.w);
                    float3 _597 = asfloat(r1.xyz) + asfloat(r3.xyz);
                    r1 = uint4(asuint(_597.x), asuint(_597.y), asuint(_597.z), r1.w);
                    r1.w = asint(r1.w) + 1;
                    continue;
                }
                r2 = uint4(r1.x, r1.y, r1.z, r2.w);
            }
            r1 = uint4(r2.x, r2.y, r2.z, r1.w);
            r1.w = asuint(0.0f);
            [loop] for (;;) {
                r2.w = asuint(float(r1.w));
                r2.w = (asfloat(r2.w) >= cb1_1_m0[23u].z) ? 4294967295u : 0u;
                if (r2.w != 0u) {
                    break;
                }
                r2.w = asint(r1.w) * 5;
                float3 _655 = (-v0.xyzx).xyz + cb1_1_m0[uint(asint(r2.w)) + 62u].xyz;
                r3 = uint4(asuint(_655.x), asuint(_655.y), asuint(_655.z), r3.w);
                r3.w = asuint(dot(asfloat(r3.xyz), asfloat(r3.xyz)));
                r4.x = asuint(cb1_1_m0[uint(asint(r2.w)) + 65u].y * cb1_1_m0[uint(asint(r2.w)) + 65u].y);
                r4.x = (asfloat(r4.x) >= asfloat(r3.w)) ? 4294967295u : 0u;
                r3.w = asuint(sqrt(asfloat(r3.w)));
                r3.w = asuint(1.0f / asfloat(r3.w));
                r3.w = asuint((r4.x != 0u) ? asfloat(r3.w) : (-1.0f));
                r4.x = (0.0f < asfloat(r3.w)) ? 4294967295u : 0u;
                if (r4.x != 0u) {
                    r4.x = asuint(dot(v1, asfloat(r3.xyz)));
                    r4.y = (0.0f < asfloat(r4.x)) ? 4294967295u : 0u;
                    if (r4.y != 0u) {
                        r3.x = asuint(dot(asfloat(r3.xyz), (-cb1_1_m0[uint(asint(r2.w)) + 63u].xyzx).xyz));
                        r3.x = asuint(mad(asfloat(r3.x), asfloat(r3.w), (-cb1_1_m0[uint(asint(r2.w)) + 66u].xxxx).x));
                        r3.y = (asfloat(r3.x) >= 0.0f) ? 4294967295u : 0u;
                        if (r3.y != 0u) {
                            r3.z = asuint(asfloat(r3.w) * asfloat(r4.x));
                            float4 _793 = 1.0f.xxxx + (-cb1_1_m0[uint(asint(r2.w)) + 66u].xxxx);
                            float _794 = _793.x;
                            float _1593 = isnan(0.0f) ? _794 : (isnan(_794) ? 0.0f : max(_794, 0.0f));
                            r4.x = asuint(isnan(1.0f) ? _1593 : (isnan(_1593) ? 1.0f : min(_1593, 1.0f)));
                            r4.x = asuint(1.0f / asfloat(r4.x));
                            r3.x = asuint(asfloat(r3.x) * (asfloat(r4.x)));
                            r3.w = asuint(asfloat(r3.w) * (asfloat(r3.w)));
                            r3.w = asuint(asfloat(r3.w) * (asfloat(r3.w)));
                            r3.w = asuint(asfloat(r3.w) * (asfloat(r3.w)));
                            r3.w = asuint(asfloat(r3.w) * (cb1_1_m0[uint(asint(r2.w)) + 65u].x));
                            r4.x = (1.0f < asfloat(r3.w)) ? 4294967295u : 0u;
                            r3.w = asuint((r4.x != 0u) ? 1.0f : asfloat(r3.w));
                            r3.x = asuint(asfloat(r3.x) * (asfloat(r3.w)));
                            r3.x = asuint(asfloat(r3.x) * (asfloat(r3.z)));
                            float3 _870 = min(1.0f.xxx, cb1_1_m0[uint(asint(r2.w)) + 64u].xyz);
                            r4 = uint4(asuint(_870.x), asuint(_870.y), asuint(_870.z), r4.w);
                            float3 _877 = asfloat(r3.xxx) * asfloat(r4.xyz);
                            r3 = uint4(asuint(_877.x), r3.y, asuint(_877.y), asuint(_877.z));
                        }
                    } else {
                        r3.y = asuint(0.0f);
                    }
                } else {
                    r3.y = asuint(0.0f);
                }
                uint3 _889 = uint3(r3.xzw & r3.yyy);
                r3 = uint4(_889.x, _889.y, _889.z, r3.w);
                float3 _896 = asfloat(r1.xyz) + asfloat(r3.xyz);
                r1 = uint4(asuint(_896.x), asuint(_896.y), asuint(_896.z), r1.w);
                r1.w = asint(r1.w) + 1;
                continue;
            }
            float3 _909 = asfloat(r1.xyz) + v4.xyz;
            r1 = uint4(asuint(_909.x), asuint(_909.y), asuint(_909.z), r1.w);
        } else {
            r1 = uint4(asuint(1.0f.xxx.x), asuint(1.0f.xxx.y), asuint(1.0f.xxx.z), r1.w);
        }
    }
    float3 _919 = mad(v1, 0.5f.xxx, 0.5f.xxx);
    r2 = uint4(asuint(_919.x), asuint(_919.y), asuint(_919.z), r2.w);
    uint3 _924 = r0.yyy;
    bool3 _929 = bool3(_924.x != uint3(0u, 0u, 0u).x, _924.y != uint3(0u, 0u, 0u).y, _924.z != uint3(0u, 0u, 0u).z);
    float3 _930 =
        float3(_929.x ? asfloat(r2.xyz).x : asfloat(r1.xyz).x, _929.y ? asfloat(r2.xyz).y : asfloat(r1.xyz).y, _929.z ? asfloat(r2.xyz).z : asfloat(r1.xyz).z);
    r1 = uint4(asuint(_930.x), asuint(_930.y), asuint(_930.z), r1.w);
    r1.w = asuint(1.0f);
    r0.y = r0.w | r0.z;
    r2 = asuint(asfloat(r1) * cb1_1_m0[10u]);
    r2 = asuint(asfloat(r2) * (v3));
    uint4 _953 = r0.yyyy;
    bool4 _958 =
        bool4(_953.x != uint4(0u, 0u, 0u, 0u).x, _953.y != uint4(0u, 0u, 0u, 0u).y, _953.z != uint4(0u, 0u, 0u, 0u).z, _953.w != uint4(0u, 0u, 0u, 0u).w);
    r1 = uint4(asuint(_958.x ? asfloat(r2.x) : asfloat(r1.x)), asuint(_958.y ? asfloat(r2.y) : asfloat(r1.y)), asuint(_958.z ? asfloat(r2.z) : asfloat(r1.z)),
        asuint(_958.w ? asfloat(r2.w) : asfloat(r1.w)));
    r0.y = asuint(cb1_1_m0[12u].y * cb1_1_m0[21u].w);
    r1.w = asuint(asfloat(r0.y) * asfloat(r1.w));
    r2.x = asuint(dot(v1, cb1_1_m0[14u].xyz));
    r2.y = asuint(dot(v1, cb1_1_m0[15u].xyz));
    float2 _993 = mad(asfloat(r2.xy), 0.5f.xx, 0.5f.xx);
    r0 = uint4(r0.x, asuint(_993.x), r0.z, asuint(_993.y));
    uint2 _999 = asuint(cb1_1_m0[4u].ww);
    bool2 _1003 = bool2(_999.x != uint2(0u, 0u).x, _999.y != uint2(0u, 0u).y);
    float2 _1004 = float2(_1003.x ? asfloat(r0.yw).x : v2.x, _1003.y ? asfloat(r0.yw).y : v2.y);
    o1 = float3(_1004.x, _1004.y, o1.z);
    r0.y = (asint(cb1_1_m0[1u].x) != 0) ? 4294967295u : 0u;
    r0.y = r0.y & r0.z;
    if (r0.y != 0u) {
        r0.y = asuint(asfloat(r0.x) + (-cb1_1_m0[20u].xxxx).y);
        r0.z = asuint((-cb1_1_m0[20u].xxxx).z + cb1_1_m0[20u].y);
        r0.y = asuint(asfloat(r0.y) / (asfloat(r0.z)));
        r0.y = asuint(asfloat(r0.y) * (cb1_1_m0[20u].z));
        r0.y = asuint(min(asfloat(r0.y), 255.9998931884765625f));
        r0.y = asuint(max(asfloat(r0.y), 1.0f));
        uint _1078 = 0u & 31u;
        r0.z = gfxInsertBits(1056964608u, r0.y, _1078, min((23u & 31u), (32u - _1078)));
        r0.y = asuint(log2(asfloat(r0.y)));
        r0.z = asuint(asfloat(r0.z) * (512.0f));
        float2 _1100 = max(asfloat(r0.yz), (-2147483648.0f).xx);
        bool2 _1103 = bool2(_1100.x >= 2147483648.0f.xx.x, _1100.y >= 2147483648.0f.xx.y);
        int2 _1104 = int2(_1100);
        int2 _1105 = int2(_1103.x ? int2(2147483647, 2147483647).x : _1104.x, _1103.y ? int2(2147483647, 2147483647).y : _1104.y);
        uint2 _1581 = r0.yz & uint2(2147483647u, 2147483647u);
        bool2 _1107 = bool2(_1581.x > uint2(2139095040u, 2139095040u).x, _1581.y > uint2(2139095040u, 2139095040u).y);
        uint2 _1109 = uint2(int2(_1107.x ? int2(0, 0).x : _1105.x, _1107.y ? int2(0, 0).y : _1105.y));
        r0 = uint4(r0.x, _1109.x, _1109.y, r0.w);
        uint _1115 = 4u & 31u;
        r0.w = gfxExtractBits(r0.z, _1115, min((4u & 31u), (32u - _1115)));
        r0.z = r0.z & 15u;
        r0.z = asuint(float(asint(r0.z)));
        r0.z = asuint(asfloat(r0.z) * (0.066666670143604278564453125f));
        r0.y = (asint(r0.y) * 16) + asint(r0.w);
        r0.w = asuint(float(asint(r0.y)));
        r0.w = asuint((-asfloat(r0.wwww)).w + 127.0f);
        float _1163 = max(asfloat(r0.w), -2147483648.0f);
        r2.x = ((r0.w & 2147483647u) > 2139095040u) ? 0 : ((_1163 >= 2147483648.0f) ? 2147483647 : int(_1163));
        r2.y = asuint(cb1_1_m0[20u].y * 0.00390625f);
        r2.y = (asfloat(r2.y) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
        r2.z = asuint((-asfloat(r0.wwww)).z + 128.0f);
        r2.w = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
        r3.x = (!(asfloat(r2.w) == 0.0f)) ? 4294967295u : 0u;
        r3.y = r2.w & 2139095040u;
        r3.y = asint(r3.y) + (-1056964608);
        r3.y = r3.x & r3.y;
        r3.y = asint(r3.y) >> (23 & int(31u));
        uint _1241 = 0u & 31u;
        r2.w = gfxInsertBits(1056964608u, r2.w, _1241, min((23u & 31u), (32u - _1241)));
        r2.w = r3.x & r2.w;
        float2 _1262 = asfloat(r2.zw) * float2(0.0078125f, 32.0f);
        r2 = uint4(r2.x, r2.y, asuint(_1262.x), asuint(_1262.y));
        float _1267 = max(asfloat(r2.w), -2147483648.0f);
        r2.w = ((r2.w & 2147483647u) > 2139095040u) ? 0 : ((_1267 >= 2147483648.0f) ? 2147483647 : int(_1267));
        r2.w = asint(r2.w) + (-16);
        r3.x = asint(r3.y) << (4 & int(31u));
        r3.x = asint(r3.x) + (-16);
        r2.w = r2.w | r3.x;
        r3.x = (asint(r2.x) < asint(r2.w)) ? 4294967295u : 0u;
        r3.y = asuint(float(asint(r2.w)));
        r0.w = asuint((-asfloat(r0.wwww)).w + asfloat(r3.y));
        r0.w = asuint(asfloat(r0.w) / (asfloat(r3.y)));
        r0.w = r0.w & r3.x;
        r2.x = (asint(r2.x) >= asint(r2.w)) ? 4294967295u : 0u;
        r0.w = asuint((r2.x != 0u) ? 0.0f : asfloat(r0.w));
        r0.w = asuint((r2.y != 0u) ? asfloat(r2.z) : asfloat(r0.w));
        r0.y = asint(r0.y) + 1;
        r0.y = asuint(float(asint(r0.y)));
        r0.y = asuint(min(asfloat(r0.y), 127.0f));
        r0.y = asuint((-asfloat(r0.yyyy)).y + 127.0f);
        float _1394 = max(asfloat(r0.y), -2147483648.0f);
        r2.x = ((r0.y & 2147483647u) > 2139095040u) ? 0 : ((_1394 >= 2147483648.0f) ? 2147483647 : int(_1394));
        r2.z = asuint((-asfloat(r0.yyyy)).z + 128.0f);
        r2.z = asuint(asfloat(r2.z) * (0.0078125f));
        r3.x = (asint(r2.x) < asint(r2.w)) ? 4294967295u : 0u;
        r0.y = asuint((-asfloat(r0.yyyy)).y + asfloat(r3.y));
        r0.y = asuint(asfloat(r0.y) / (asfloat(r3.y)));
        r0.y = r0.y & r3.x;
        r2.x = (asint(r2.x) >= asint(r2.w)) ? 4294967295u : 0u;
        r0.y = asuint((r2.x != 0u) ? 0.0f : asfloat(r0.y));
        r0.y = asuint((r2.y != 0u) ? asfloat(r2.z) : asfloat(r0.y));
        r0.y = asuint((-asfloat(r0.wwww)).y + asfloat(r0.y));
        o1.z = mad(asfloat(r0.z), asfloat(r0.y), asfloat(r0.w));
    } else {
        o1.z = 0.0f;
    }
    r0.y = (asint(cb1_1_m0[4u].x) == 4) ? 4294967295u : 0u;
    uint4 _1500 = r0.yyyy;
    bool4 _1503 =
        bool4(_1500.x != uint4(0u, 0u, 0u, 0u).x, _1500.y != uint4(0u, 0u, 0u, 0u).y, _1500.z != uint4(0u, 0u, 0u, 0u).z, _1500.w != uint4(0u, 0u, 0u, 0u).w);
    r1 = uint4(asuint(_1503.x ? v3.x : asfloat(r1.x)), asuint(_1503.y ? v3.y : asfloat(r1.y)), asuint(_1503.z ? v3.z : asfloat(r1.z)),
        asuint(_1503.w ? v3.w : asfloat(r1.w)));
    float3 _1507 = min(asfloat(r1.xyz), 1.0f.xxx);
    o2 = float4(_1507.x, _1507.y, _1507.z, o2.w);
    position.w = asfloat(r0.x);
    o2.w = asfloat(r1.w);
    outputTexcoord0 = float2(o1.xy);
    outputFog0 = o1.z;
    outputColor0 = float4(o2);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    output.outputFog0 = outputFog0;
    output.outputColor0 = outputColor0;
    return output;
}
#endif
