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
    float2 outputDepth0 : DEPTH0;
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
    float2 outputDepth0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    uint4 r3 = 0;
    uint4 r4 = 0;
    uint4 r5 = 0;
    uint4 r6 = 0;
    uint4 r7 = 0;
    float3 v0 = 0;
    float3 v1 = 0;
    float2 v2 = 0;
    float4 v3 = 0;
    float4 v4 = 0;
    float3 o1 = 0;
    float4 o2 = 0;
    float2 o3 = 0;
    float3 _1521 = 0;
    float3 _1530 = 0;
    float2 _1540 = 0;
    float4 _1547 = 0;
    float4 _1558 = 0;

    inputPosition0 = input.inputPosition0;
    inputNormal0 = input.inputNormal0;
    inputTexcoord0 = input.inputTexcoord0;
    inputDiffuse0 = input.inputDiffuse0;
    inputDiffuse1 = input.inputDiffuse1;
    float3 _1524 = _1521;
    _1524.x = inputPosition0.x;
    float3 _1526 = _1524;
    _1526.y = inputPosition0.y;
    float3 _1528 = _1526;
    _1528.z = inputPosition0.z;
    v0 = _1528;
    float3 _1533 = _1530;
    _1533.x = inputNormal0.x;
    float3 _1535 = _1533;
    _1535.y = inputNormal0.y;
    float3 _1537 = _1535;
    _1537.z = inputNormal0.z;
    v1 = _1537;
    float2 _1543 = _1540;
    _1543.x = inputTexcoord0.x;
    float2 _1545 = _1543;
    _1545.y = inputTexcoord0.y;
    v2 = _1545;
    float4 _1550 = _1547;
    _1550.x = inputDiffuse0.x;
    float4 _1552 = _1550;
    _1552.y = inputDiffuse0.y;
    float4 _1554 = _1552;
    _1554.z = inputDiffuse0.z;
    float4 _1556 = _1554;
    _1556.w = inputDiffuse0.w;
    v3 = _1556;
    float4 _1561 = _1558;
    _1561.x = inputDiffuse1.x;
    float4 _1563 = _1561;
    _1563.y = inputDiffuse1.y;
    float4 _1565 = _1563;
    _1565.z = inputDiffuse1.z;
    float4 _1567 = _1565;
    _1567.w = inputDiffuse1.w;
    v4 = _1567;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    r1.z = asuint(dot(asfloat(r0), cb1_1_m0[7u]));
    r1.w = asuint(dot(asfloat(r0), cb1_1_m0[8u]));
    int3 _83 = asint(cb1_1_m0[4u].xxx);
    bool3 _90 = bool3(_83.x == int3(2, 0, 3).x, _83.y == int3(2, 0, 3).y, _83.z == int3(2, 0, 3).z);
    uint3 _96 = uint3(_90.x ? uint3(4294967295u, 4294967295u, 4294967295u).x : uint3(0u, 0u, 0u).x,
        _90.y ? uint3(4294967295u, 4294967295u, 4294967295u).y : uint3(0u, 0u, 0u).y,
        _90.z ? uint3(4294967295u, 4294967295u, 4294967295u).z : uint3(0u, 0u, 0u).z);
    r0 = uint4(_96.x, _96.y, _96.z, r0.w);
    if (asuint(cb1_1_m0[4u].y) != 0u) {
        r2 = uint4(asuint(1.0f.xxx.x), asuint(1.0f.xxx.y), asuint(1.0f.xxx.z), r2.w);
    } else {
        int2 _112 = asint(cb1_1_m0[4u].xx);
        bool2 _116 = bool2(_112.x == int2(0, 1).x, _112.y == int2(0, 1).y);
        uint2 _121 = uint2(_116.x ? uint2(4294967295u, 4294967295u).x : uint2(0u, 0u).x, _116.y ? uint2(4294967295u, 4294967295u).y : uint2(0u, 0u).y);
        r1 = uint4(_121.x, _121.y, r1.z, r1.w);
        r0.w = r1.y | r1.x;
        if (r0.w != 0u) {
            float3 _146 = min(cb1_1_m0[22u].xyz, 1.0f.xxx);
            r2 = uint4(asuint(_146.x), asuint(_146.y), asuint(_146.z), r2.w);
            r3 = uint4(r2.x, r2.y, r2.z, r3.w);
            r0.w = asuint(0.0f);
            [loop] for (;;) {
                r1.x = asuint(float(r0.w));
                r1.x = (asfloat(r1.x) >= cb1_1_m0[23u].x) ? 4294967295u : 0u;
                if (r1.x != 0u) {
                    break;
                }
                r1.x = asint(r0.w) << (1 & int(31u));
                r1.y = asuint(dot(cb1_1_m0[uint(asint(r1.x)) + 24u].xyz, cb1_1_m0[uint(asint(r1.x)) + 24u].xyz));
                r1.y = asuint(rsqrt(asfloat(r1.y)));
                float3 _216 = asfloat(r1.yyy) * cb1_1_m0[uint(asint(r1.x)) + 24u].xyz;
                r4 = uint4(asuint(_216.x), asuint(_216.y), asuint(_216.z), r4.w);
                r1.y = asuint(dot(v1, asfloat(r4.xyz)));
                r2.w = (0.0f < asfloat(r1.y)) ? 4294967295u : 0u;
                if (r2.w != 0u) {
                    float3 _244 = min(1.0f.xxx, cb1_1_m0[uint(asint(r1.x)) + 25u].xyz);
                    r4 = uint4(asuint(_244.x), asuint(_244.y), asuint(_244.z), r4.w);
                    float3 _251 = asfloat(r1.yyy) * asfloat(r4.xyz);
                    r4 = uint4(asuint(_251.x), asuint(_251.y), asuint(_251.z), r4.w);
                }
                uint3 _261 = uint3(r2.www & r4.xyz);
                r4 = uint4(_261.x, _261.y, _261.z, r4.w);
                float3 _268 = asfloat(r3.xyz) + asfloat(r4.xyz);
                r3 = uint4(asuint(_268.x), asuint(_268.y), asuint(_268.z), r3.w);
                r0.w = asint(r0.w) + 1;
                continue;
            }
            r0.w = (v4.w == 0.0f) ? 4294967295u : 0u;
            if (r0.w != 0u) {
                r2 = uint4(r3.x, r3.y, r3.z, r2.w);
                r0.w = asuint(0.0f);
                [loop] for (;;) {
                    r1.x = asuint(float(r0.w));
                    r1.x = (asfloat(r1.x) >= cb1_1_m0[23u].y) ? 4294967295u : 0u;
                    if (r1.x != 0u) {
                        break;
                    }
                    r4 = uint4(65535u, 65535u, 65535u, 65535u) & asuint(cb1_1_m0[uint(asint(r0.w)) + 30u]);
                    r5 = asuint(cb1_1_m0[uint(asint(r0.w)) + 30u]) >> (uint4(16u, 16u, 16u, 16u) & uint4(31u, 31u, 31u, 31u));
                    r4 = uint4(asuint(gfxUnpackHalf2(r4.x).x), asuint(gfxUnpackHalf2(r4.y).x), asuint(gfxUnpackHalf2(r4.z).x), asuint(gfxUnpackHalf2(r4.w).x));
                    r5 = uint4(asuint(gfxUnpackHalf2(r5.x).x), asuint(gfxUnpackHalf2(r5.z).x), asuint(gfxUnpackHalf2(r5.y).x), asuint(gfxUnpackHalf2(r5.w).x));
                    r6 = uint4(r4.x, r6.y, r4.y, r6.w);
                    r6.y = r5.x;
                    r1.x = asuint(dot(asfloat(r6.xyz), cb1_1_m0[17u].xyz));
                    r7.x = asuint(asfloat(r1.x) + cb1_1_m0[17u].w);
                    r1.x = asuint(dot(asfloat(r6.xyz), cb1_1_m0[18u].xyz));
                    r7.y = asuint(asfloat(r1.x) + cb1_1_m0[18u].w);
                    r1.x = asuint(dot(asfloat(r6.xyz), cb1_1_m0[19u].xyz));
                    r7.z = asuint(asfloat(r1.x) + cb1_1_m0[19u].w);
                    float3 _442 = asfloat(r7.xyz) + (-v0.xyzx).xyz;
                    r6 = uint4(asuint(_442.x), asuint(_442.y), asuint(_442.z), r6.w);
                    r1.x = asuint(dot(asfloat(r6.xyz), asfloat(r6.xyz)));
                    float2 _455 = asfloat(r5.wz) * asfloat(r5.wz);
                    r4 = uint4(asuint(_455.x), asuint(_455.y), r4.z, r4.w);
                    r1.y = (asfloat(r4.x) >= asfloat(r1.x)) ? 4294967295u : 0u;
                    r1.x = asuint(sqrt(asfloat(r1.x)));
                    r1.x = asuint(1.0f / asfloat(r1.x));
                    r1.x = asuint((r1.y != 0u) ? asfloat(r1.x) : (-1.0f));
                    r1.y = (0.0f < asfloat(r1.x)) ? 4294967295u : 0u;
                    r2.w = asuint(dot(v1, asfloat(r6.xyz)));
                    r3.w = (0.0f < asfloat(r2.w)) ? 4294967295u : 0u;
                    r2.w = asuint(asfloat(r1.x) * asfloat(r2.w));
                    r1.x = asuint(asfloat(r1.x) * (asfloat(r1.x)));
                    r1.x = asuint(asfloat(r1.x) * (asfloat(r1.x)));
                    r1.x = asuint(asfloat(r1.x) * (asfloat(r1.x)));
                    r4.x = asuint(asfloat(r4.y) * asfloat(r4.y));
                    r4.x = asuint(asfloat(r4.x) * (asfloat(r4.x)));
                    r1.x = asuint(asfloat(r1.x) * (asfloat(r4.x)));
                    r4.x = (1.0f < asfloat(r1.x)) ? 4294967295u : 0u;
                    r1.x = asuint((r4.x != 0u) ? 1.0f : asfloat(r1.x));
                    r1.x = asuint(asfloat(r1.x) * (asfloat(r2.w)));
                    r5 = uint4(r4.z, r5.y, r4.w, r5.w);
                    float3 _567 = min(asfloat(r5.xyz), 1.0f.xxx);
                    r4 = uint4(asuint(_567.x), asuint(_567.y), asuint(_567.z), r4.w);
                    float3 _574 = asfloat(r1.xxx) * asfloat(r4.xyz);
                    r4 = uint4(asuint(_574.x), asuint(_574.y), asuint(_574.z), r4.w);
                    r1.x = r1.y & r3.w;
                    uint3 _593 = uint3(r4.xyz & r1.xxx);
                    r4 = uint4(_593.x, _593.y, _593.z, r4.w);
                    float3 _600 = asfloat(r2.xyz) + asfloat(r4.xyz);
                    r2 = uint4(asuint(_600.x), asuint(_600.y), asuint(_600.z), r2.w);
                    r0.w = asint(r0.w) + 1;
                    continue;
                }
                r3 = uint4(r2.x, r2.y, r2.z, r3.w);
            }
            r2 = uint4(r3.x, r3.y, r3.z, r2.w);
            r0.w = asuint(0.0f);
            [loop] for (;;) {
                r1.x = asuint(float(r0.w));
                r1.x = (asfloat(r1.x) >= cb1_1_m0[23u].z) ? 4294967295u : 0u;
                if (r1.x != 0u) {
                    break;
                }
                r1.x = asint(r0.w) * 5;
                float3 _658 = (-v0.xyzx).xyz + cb1_1_m0[uint(asint(r1.x)) + 62u].xyz;
                r4 = uint4(asuint(_658.x), asuint(_658.y), asuint(_658.z), r4.w);
                r1.y = asuint(dot(asfloat(r4.xyz), asfloat(r4.xyz)));
                r2.w = asuint(cb1_1_m0[uint(asint(r1.x)) + 65u].y * cb1_1_m0[uint(asint(r1.x)) + 65u].y);
                r2.w = (asfloat(r2.w) >= asfloat(r1.y)) ? 4294967295u : 0u;
                r1.y = asuint(sqrt(asfloat(r1.y)));
                r1.y = asuint(1.0f / asfloat(r1.y));
                r1.y = asuint((r2.w != 0u) ? asfloat(r1.y) : (-1.0f));
                r2.w = (0.0f < asfloat(r1.y)) ? 4294967295u : 0u;
                if (r2.w != 0u) {
                    r2.w = asuint(dot(v1, asfloat(r4.xyz)));
                    r3.w = (0.0f < asfloat(r2.w)) ? 4294967295u : 0u;
                    if (r3.w != 0u) {
                        r3.w = asuint(dot(asfloat(r4.xyz), (-cb1_1_m0[uint(asint(r1.x)) + 63u].xyzx).xyz));
                        r3.w = asuint(mad(asfloat(r3.w), asfloat(r1.y), (-cb1_1_m0[uint(asint(r1.x)) + 66u].xxxx).w));
                        r4.x = (asfloat(r3.w) >= 0.0f) ? 4294967295u : 0u;
                        if (r4.x != 0u) {
                            r2.w = asuint(asfloat(r1.y) * asfloat(r2.w));
                            float4 _796 = 1.0f.xxxx + (-cb1_1_m0[uint(asint(r1.x)) + 66u].xxxx);
                            float _797 = _796.y;
                            float _1600 = isnan(0.0f) ? _797 : (isnan(_797) ? 0.0f : max(_797, 0.0f));
                            r4.y = asuint(isnan(1.0f) ? _1600 : (isnan(_1600) ? 1.0f : min(_1600, 1.0f)));
                            r4.y = asuint(1.0f / asfloat(r4.y));
                            r3.w = asuint(asfloat(r3.w) * (asfloat(r4.y)));
                            r1.y = asuint(asfloat(r1.y) * (asfloat(r1.y)));
                            r1.y = asuint(asfloat(r1.y) * (asfloat(r1.y)));
                            r1.y = asuint(asfloat(r1.y) * (asfloat(r1.y)));
                            r1.y = asuint(asfloat(r1.y) * (cb1_1_m0[uint(asint(r1.x)) + 65u].x));
                            r4.y = (1.0f < asfloat(r1.y)) ? 4294967295u : 0u;
                            r1.y = asuint((r4.y != 0u) ? 1.0f : asfloat(r1.y));
                            r1.y = asuint(asfloat(r3.w) * asfloat(r1.y));
                            r1.y = asuint(asfloat(r1.y) * (asfloat(r2.w)));
                            float3 _873 = min(1.0f.xxx, cb1_1_m0[uint(asint(r1.x)) + 64u].xyz);
                            r4 = uint4(r4.x, asuint(_873.x), asuint(_873.y), asuint(_873.z));
                            float3 _880 = asfloat(r1.yyy) * asfloat(r4.yzw);
                            r4 = uint4(r4.x, asuint(_880.x), asuint(_880.y), asuint(_880.z));
                        }
                    } else {
                        r4.x = asuint(0.0f);
                    }
                } else {
                    r4.x = asuint(0.0f);
                }
                uint3 _892 = uint3(r4.yzw & r4.xxx);
                r4 = uint4(_892.x, _892.y, _892.z, r4.w);
                float3 _899 = asfloat(r2.xyz) + asfloat(r4.xyz);
                r2 = uint4(asuint(_899.x), asuint(_899.y), asuint(_899.z), r2.w);
                r0.w = asint(r0.w) + 1;
                continue;
            }
            float3 _912 = asfloat(r2.xyz) + v4.xyz;
            r2 = uint4(asuint(_912.x), asuint(_912.y), asuint(_912.z), r2.w);
        } else {
            r2 = uint4(asuint(1.0f.xxx.x), asuint(1.0f.xxx.y), asuint(1.0f.xxx.z), r2.w);
        }
    }
    float3 _922 = mad(v1, 0.5f.xxx, 0.5f.xxx);
    r3 = uint4(asuint(_922.x), asuint(_922.y), asuint(_922.z), r3.w);
    uint3 _927 = r0.xxx;
    bool3 _932 = bool3(_927.x != uint3(0u, 0u, 0u).x, _927.y != uint3(0u, 0u, 0u).y, _927.z != uint3(0u, 0u, 0u).z);
    float3 _933 =
        float3(_932.x ? asfloat(r3.xyz).x : asfloat(r2.xyz).x, _932.y ? asfloat(r3.xyz).y : asfloat(r2.xyz).y, _932.z ? asfloat(r3.xyz).z : asfloat(r2.xyz).z);
    r2 = uint4(asuint(_933.x), asuint(_933.y), asuint(_933.z), r2.w);
    r2.w = asuint(1.0f);
    r0.x = r0.z | r0.y;
    r3 = asuint(asfloat(r2) * cb1_1_m0[10u]);
    r3 = asuint(asfloat(r3) * (v3));
    uint4 _956 = r0.xxxx;
    bool4 _961 =
        bool4(_956.x != uint4(0u, 0u, 0u, 0u).x, _956.y != uint4(0u, 0u, 0u, 0u).y, _956.z != uint4(0u, 0u, 0u, 0u).z, _956.w != uint4(0u, 0u, 0u, 0u).w);
    r2 = uint4(asuint(_961.x ? asfloat(r3.x) : asfloat(r2.x)), asuint(_961.y ? asfloat(r3.y) : asfloat(r2.y)), asuint(_961.z ? asfloat(r3.z) : asfloat(r2.z)),
        asuint(_961.w ? asfloat(r3.w) : asfloat(r2.w)));
    r0.x = asuint(cb1_1_m0[12u].y * cb1_1_m0[21u].w);
    r2.w = asuint(asfloat(r0.x) * asfloat(r2.w));
    r1.x = asuint(dot(v1, cb1_1_m0[14u].xyz));
    r1.y = asuint(dot(v1, cb1_1_m0[15u].xyz));
    float2 _996 = mad(asfloat(r1.xy), 0.5f.xx, 0.5f.xx);
    r0 = uint4(asuint(_996.x), r0.y, asuint(_996.y), r0.w);
    uint2 _1002 = asuint(cb1_1_m0[4u].ww);
    bool2 _1006 = bool2(_1002.x != uint2(0u, 0u).x, _1002.y != uint2(0u, 0u).y);
    float2 _1007 = float2(_1006.x ? asfloat(r0.xz).x : v2.x, _1006.y ? asfloat(r0.xz).y : v2.y);
    o1 = float3(_1007.x, _1007.y, o1.z);
    r0.x = (asint(cb1_1_m0[1u].x) != 0) ? 4294967295u : 0u;
    r0.x = r0.x & r0.y;
    if (r0.x != 0u) {
        r0.x = asuint(asfloat(r1.w) + (-cb1_1_m0[20u].xxxx).x);
        r0.y = asuint((-cb1_1_m0[20u].xxxx).y + cb1_1_m0[20u].y);
        r0.x = asuint(asfloat(r0.x) / (asfloat(r0.y)));
        r0.x = asuint(asfloat(r0.x) * (cb1_1_m0[20u].z));
        r0.x = asuint(min(asfloat(r0.x), 255.9998931884765625f));
        r0.x = asuint(max(asfloat(r0.x), 1.0f));
        uint _1081 = 0u & 31u;
        r0.y = gfxInsertBits(1056964608u, r0.x, _1081, min((23u & 31u), (32u - _1081)));
        r0.x = asuint(log2(asfloat(r0.x)));
        r0.y = asuint(asfloat(r0.y) * (512.0f));
        float2 _1103 = max(asfloat(r0.xy), (-2147483648.0f).xx);
        bool2 _1106 = bool2(_1103.x >= 2147483648.0f.xx.x, _1103.y >= 2147483648.0f.xx.y);
        int2 _1107 = int2(_1103);
        int2 _1108 = int2(_1106.x ? int2(2147483647, 2147483647).x : _1107.x, _1106.y ? int2(2147483647, 2147483647).y : _1107.y);
        uint2 _1588 = r0.xy & uint2(2147483647u, 2147483647u);
        bool2 _1110 = bool2(_1588.x > uint2(2139095040u, 2139095040u).x, _1588.y > uint2(2139095040u, 2139095040u).y);
        uint2 _1112 = uint2(int2(_1110.x ? int2(0, 0).x : _1108.x, _1110.y ? int2(0, 0).y : _1108.y));
        r0 = uint4(_1112.x, _1112.y, r0.z, r0.w);
        uint _1118 = 4u & 31u;
        r0.z = gfxExtractBits(r0.y, _1118, min((4u & 31u), (32u - _1118)));
        r0.y = r0.y & 15u;
        r0.y = asuint(float(asint(r0.y)));
        r0.y = asuint(asfloat(r0.y) * (0.066666670143604278564453125f));
        r0.x = (asint(r0.x) * 16) + asint(r0.z);
        r0.z = asuint(float(asint(r0.x)));
        r0.z = asuint((-asfloat(r0.zzzz)).z + 127.0f);
        float _1166 = max(asfloat(r0.z), -2147483648.0f);
        r0.w = ((r0.z & 2147483647u) > 2139095040u) ? 0 : ((_1166 >= 2147483648.0f) ? 2147483647 : int(_1166));
        r1.x = asuint(cb1_1_m0[20u].y * 0.00390625f);
        r1.x = (asfloat(r1.x) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
        r1.y = asuint((-asfloat(r0.zzzz)).y + 128.0f);
        r1.y = asuint(asfloat(r1.y) * (0.0078125f));
        r3.x = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
        r3.y = (!(asfloat(r3.x) == 0.0f)) ? 4294967295u : 0u;
        r3.z = r3.x & 2139095040u;
        r3.z = asint(r3.z) + (-1056964608);
        r3.z = r3.y & r3.z;
        r3.z = asint(r3.z) >> (23 & int(31u));
        uint _1249 = 0u & 31u;
        r3.x = gfxInsertBits(1056964608u, r3.x, _1249, min((23u & 31u), (32u - _1249)));
        r3.x = r3.y & r3.x;
        r3.x = asuint(asfloat(r3.x) * (32.0f));
        float _1272 = max(asfloat(r3.x), -2147483648.0f);
        r3.x = ((r3.x & 2147483647u) > 2139095040u) ? 0 : ((_1272 >= 2147483648.0f) ? 2147483647 : int(_1272));
        r3.y = asint(r3.z) << (4 & int(31u));
        uint2 _1294 = uint2(asint(r3.xy) + int2(-16, -16));
        r3 = uint4(_1294.x, _1294.y, r3.z, r3.w);
        r3.x = r3.x | r3.y;
        r3.y = (asint(r0.w) < asint(r3.x)) ? 4294967295u : 0u;
        r3.z = asuint(float(asint(r3.x)));
        r0.z = asuint((-asfloat(r0.zzzz)).z + asfloat(r3.z));
        r0.z = asuint(asfloat(r0.z) / (asfloat(r3.z)));
        r0.z = r0.z & r3.y;
        r0.w = (asint(r0.w) >= asint(r3.x)) ? 4294967295u : 0u;
        r0.z = asuint((r0.w != 0u) ? 0.0f : asfloat(r0.z));
        r0.z = asuint((r1.x != 0u) ? asfloat(r1.y) : asfloat(r0.z));
        r0.x = asint(r0.x) + 1;
        r0.x = asuint(float(asint(r0.x)));
        r0.x = asuint(min(asfloat(r0.x), 127.0f));
        r0.x = asuint((-asfloat(r0.xxxx)).x + 127.0f);
        float _1395 = max(asfloat(r0.x), -2147483648.0f);
        r0.w = ((r0.x & 2147483647u) > 2139095040u) ? 0 : ((_1395 >= 2147483648.0f) ? 2147483647 : int(_1395));
        r1.y = asuint((-asfloat(r0.xxxx)).y + 128.0f);
        r1.y = asuint(asfloat(r1.y) * (0.0078125f));
        r3.y = (asint(r0.w) < asint(r3.x)) ? 4294967295u : 0u;
        r0.x = asuint((-asfloat(r0.xxxx)).x + asfloat(r3.z));
        r0.x = asuint(asfloat(r0.x) / (asfloat(r3.z)));
        r0.x = r0.x & r3.y;
        r0.w = (asint(r0.w) >= asint(r3.x)) ? 4294967295u : 0u;
        r0.x = asuint((r0.w != 0u) ? 0.0f : asfloat(r0.x));
        r0.x = asuint((r1.x != 0u) ? asfloat(r1.y) : asfloat(r0.x));
        r0.x = asuint((-asfloat(r0.zzzz)).x + asfloat(r0.x));
        o1.z = mad(asfloat(r0.y), asfloat(r0.x), asfloat(r0.z));
    } else {
        o1.z = 0.0f;
    }
    r0.x = (asint(cb1_1_m0[4u].x) == 4) ? 4294967295u : 0u;
    uint4 _1501 = r0.xxxx;
    bool4 _1504 =
        bool4(_1501.x != uint4(0u, 0u, 0u, 0u).x, _1501.y != uint4(0u, 0u, 0u, 0u).y, _1501.z != uint4(0u, 0u, 0u, 0u).z, _1501.w != uint4(0u, 0u, 0u, 0u).w);
    r0 = uint4(asuint(_1504.x ? v3.x : asfloat(r2.x)), asuint(_1504.y ? v3.y : asfloat(r2.y)), asuint(_1504.z ? v3.z : asfloat(r2.z)),
        asuint(_1504.w ? v3.w : asfloat(r2.w)));
    float3 _1508 = min(asfloat(r0.xyz), 1.0f.xxx);
    o2 = float4(_1508.x, _1508.y, _1508.z, o2.w);
    position = float4(position.x, position.y, asfloat(r1.z), asfloat(r1.w));
    o2.w = asfloat(r0.w);
    o3 = asfloat(r1.zw);
    outputTexcoord0 = float2(o1.xy);
    outputFog0 = o1.z;
    outputColor0 = float4(o2);
    outputDepth0 = float2(o3);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    output.outputFog0 = outputFog0;
    output.outputColor0 = outputColor0;
    output.outputDepth0 = outputDepth0;
    return output;
}
#endif
