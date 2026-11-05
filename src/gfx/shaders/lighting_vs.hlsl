#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb77_struct : register(b1) {
    float4 cb1_1_m0[77] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float3 inputNormal0 : NORMAL0;
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
    float3 o1 = 0;
    float4 o2 = 0;
    float3 _1421 = 0;
    float3 _1430 = 0;
    float2 _1440 = 0;

    inputPosition0 = input.inputPosition0;
    inputNormal0 = input.inputNormal0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _1424 = _1421;
    _1424.x = inputPosition0.x;
    float3 _1426 = _1424;
    _1426.y = inputPosition0.y;
    float3 _1428 = _1426;
    _1428.z = inputPosition0.z;
    v0 = _1428;
    float3 _1433 = _1430;
    _1433.x = inputNormal0.x;
    float3 _1435 = _1433;
    _1435.y = inputNormal0.y;
    float3 _1437 = _1435;
    _1437.z = inputNormal0.z;
    v1 = _1437;
    float2 _1443 = _1440;
    _1443.x = inputTexcoord0.x;
    float2 _1445 = _1443;
    _1445.y = inputTexcoord0.y;
    v2 = _1445;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    r0.x = asuint(dot(asfloat(r0), cb1_1_m0[8u]));
    int4 _77 = asint(cb1_1_m0[4u].xxxx);
    bool4 _85 = bool4(_77.x == int4(0, 1, 2, 3).x, _77.y == int4(0, 1, 2, 3).y, _77.z == int4(0, 1, 2, 3).z, _77.w == int4(0, 1, 2, 3).w);
    r1 = uint4(_85.x ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).x : uint4(0u, 0u, 0u, 0u).x,
        _85.y ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).y : uint4(0u, 0u, 0u, 0u).y,
        _85.z ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).z : uint4(0u, 0u, 0u, 0u).z,
        _85.w ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).w : uint4(0u, 0u, 0u, 0u).w);
    uint2 _100 = uint2(r1.yw | r1.xx);
    r0 = uint4(r0.x, _100.x, _100.y, r0.w);
    if (r0.y != 0u) {
        r1 = uint4(asuint(cb1_1_m0[9u].x), asuint(cb1_1_m0[9u].y), r1.z, asuint(cb1_1_m0[9u].z));
        r0.y = asuint(0.0f);
        [loop] for (;;) {
            r0.w = asuint(float(r0.y));
            r0.w = (asfloat(r0.w) >= cb1_1_m0[23u].x) ? 4294967295u : 0u;
            if (r0.w != 0u) {
                break;
            }
            r0.w = asint(r0.y) << (1 & int(31u));
            r2.x = asuint(dot(cb1_1_m0[uint(asint(r0.w)) + 24u].xyz, cb1_1_m0[uint(asint(r0.w)) + 24u].xyz));
            r2.x = asuint(rsqrt(asfloat(r2.x)));
            float3 _181 = asfloat(r2.xxx) * cb1_1_m0[uint(asint(r0.w)) + 24u].xyz;
            r2 = uint4(asuint(_181.x), asuint(_181.y), asuint(_181.z), r2.w);
            r2.x = asuint(dot(v1, asfloat(r2.xyz)));
            r2.y = (0.0f < asfloat(r2.x)) ? 4294967295u : 0u;
            if (r2.y != 0u) {
                float3 _210 = min(1.0f.xxx, cb1_1_m0[uint(asint(r0.w)) + 25u].xyz);
                r3 = uint4(asuint(_210.x), asuint(_210.y), asuint(_210.z), r3.w);
                float3 _217 = asfloat(r2.xxx) * asfloat(r3.xyz);
                r2 = uint4(asuint(_217.x), r2.y, asuint(_217.y), asuint(_217.z));
            }
            uint3 _228 = uint3(r2.xzw & r2.yyy);
            r2 = uint4(_228.x, _228.y, _228.z, r2.w);
            float3 _235 = asfloat(r1.xyw) + asfloat(r2.xyz);
            r1 = uint4(asuint(_235.x), asuint(_235.y), r1.z, asuint(_235.z));
            r0.y = asint(r0.y) + 1;
            continue;
        }
        r2 = uint4(r1.x, r1.y, r1.w, r2.w);
        r0.y = asuint(0.0f);
        [loop] for (;;) {
            r0.w = asuint(float(r0.y));
            r0.w = (asfloat(r0.w) >= cb1_1_m0[23u].y) ? 4294967295u : 0u;
            if (r0.w != 0u) {
                break;
            }
            r3 = uint4(65535u, 65535u, 65535u, 65535u) & asuint(cb1_1_m0[uint(asint(r0.y)) + 30u]);
            r4 = asuint(cb1_1_m0[uint(asint(r0.y)) + 30u]) >> (uint4(16u, 16u, 16u, 16u) & uint4(31u, 31u, 31u, 31u));
            r3 = uint4(asuint(gfxUnpackHalf2(r3.x).x), asuint(gfxUnpackHalf2(r3.y).x), asuint(gfxUnpackHalf2(r3.z).x), asuint(gfxUnpackHalf2(r3.w).x));
            r4 = uint4(asuint(gfxUnpackHalf2(r4.x).x), asuint(gfxUnpackHalf2(r4.z).x), asuint(gfxUnpackHalf2(r4.y).x), asuint(gfxUnpackHalf2(r4.w).x));
            r5 = uint4(r3.x, r5.y, r3.y, r5.w);
            r5.y = r4.x;
            r0.w = asuint(dot(asfloat(r5.xyz), cb1_1_m0[17u].xyz));
            r6.x = asuint(asfloat(r0.w) + cb1_1_m0[17u].w);
            r0.w = asuint(dot(asfloat(r5.xyz), cb1_1_m0[18u].xyz));
            r6.y = asuint(asfloat(r0.w) + cb1_1_m0[18u].w);
            r0.w = asuint(dot(asfloat(r5.xyz), cb1_1_m0[19u].xyz));
            r6.z = asuint(asfloat(r0.w) + cb1_1_m0[19u].w);
            float3 _395 = asfloat(r6.xyz) + (-v0.xyzx).xyz;
            r5 = uint4(asuint(_395.x), asuint(_395.y), asuint(_395.z), r5.w);
            r0.w = asuint(dot(asfloat(r5.xyz), asfloat(r5.xyz)));
            float2 _408 = asfloat(r4.wz) * asfloat(r4.wz);
            r3 = uint4(asuint(_408.x), asuint(_408.y), r3.z, r3.w);
            r2.w = (asfloat(r3.x) >= asfloat(r0.w)) ? 4294967295u : 0u;
            r0.w = asuint(sqrt(asfloat(r0.w)));
            r0.w = asuint(1.0f / asfloat(r0.w));
            r0.w = asuint((r2.w != 0u) ? asfloat(r0.w) : (-1.0f));
            r2.w = (0.0f < asfloat(r0.w)) ? 4294967295u : 0u;
            r3.x = asuint(dot(v1, asfloat(r5.xyz)));
            r4.w = (0.0f < asfloat(r3.x)) ? 4294967295u : 0u;
            r3.x = asuint(asfloat(r0.w) * asfloat(r3.x));
            r0.w = asuint(asfloat(r0.w) * (asfloat(r0.w)));
            r0.w = asuint(asfloat(r0.w) * (asfloat(r0.w)));
            r0.w = asuint(asfloat(r0.w) * (asfloat(r0.w)));
            r3.y = asuint(asfloat(r3.y) * (asfloat(r3.y)));
            r3.y = asuint(asfloat(r3.y) * (asfloat(r3.y)));
            r0.w = asuint(asfloat(r0.w) * (asfloat(r3.y)));
            r3.y = (1.0f < asfloat(r0.w)) ? 4294967295u : 0u;
            r0.w = asuint((r3.y != 0u) ? 1.0f : asfloat(r0.w));
            r0.w = asuint(asfloat(r0.w) * (asfloat(r3.x)));
            r4 = uint4(r3.z, r4.y, r3.w, r4.w);
            float3 _520 = min(asfloat(r4.xyz), 1.0f.xxx);
            r3 = uint4(asuint(_520.x), asuint(_520.y), asuint(_520.z), r3.w);
            float3 _527 = asfloat(r0.www) * asfloat(r3.xyz);
            r3 = uint4(asuint(_527.x), asuint(_527.y), asuint(_527.z), r3.w);
            r0.w = r2.w & r4.w;
            uint3 _546 = uint3(r3.xyz & r0.www);
            r3 = uint4(_546.x, _546.y, _546.z, r3.w);
            float3 _553 = asfloat(r2.xyz) + asfloat(r3.xyz);
            r2 = uint4(asuint(_553.x), asuint(_553.y), asuint(_553.z), r2.w);
            r0.y = asint(r0.y) + 1;
            continue;
        }
        r1 = uint4(r2.x, r2.y, r1.z, r2.z);
        r0.y = asuint(0.0f);
        [loop] for (;;) {
            r0.w = asuint(float(r0.y));
            r0.w = (asfloat(r0.w) >= cb1_1_m0[23u].z) ? 4294967295u : 0u;
            if (r0.w != 0u) {
                break;
            }
            r0.w = asint(r0.y) * 5;
            float3 _608 = (-v0.xyzx).xyz + cb1_1_m0[uint(asint(r0.w)) + 62u].xyz;
            r3 = uint4(asuint(_608.x), asuint(_608.y), asuint(_608.z), r3.w);
            r2.w = asuint(dot(asfloat(r3.xyz), asfloat(r3.xyz)));
            r3.w = asuint(cb1_1_m0[uint(asint(r0.w)) + 65u].y * cb1_1_m0[uint(asint(r0.w)) + 65u].y);
            r3.w = (asfloat(r3.w) >= asfloat(r2.w)) ? 4294967295u : 0u;
            r2.w = asuint(sqrt(asfloat(r2.w)));
            r2.w = asuint(1.0f / asfloat(r2.w));
            r2.w = asuint((r3.w != 0u) ? asfloat(r2.w) : (-1.0f));
            r3.w = (0.0f < asfloat(r2.w)) ? 4294967295u : 0u;
            if (r3.w != 0u) {
                r3.w = asuint(dot(v1, asfloat(r3.xyz)));
                r4.x = (0.0f < asfloat(r3.w)) ? 4294967295u : 0u;
                if (r4.x != 0u) {
                    r3.x = asuint(dot(asfloat(r3.xyz), (-cb1_1_m0[uint(asint(r0.w)) + 63u].xyzx).xyz));
                    r3.x = asuint(mad(asfloat(r3.x), asfloat(r2.w), (-cb1_1_m0[uint(asint(r0.w)) + 66u].xxxx).x));
                    r3.y = (asfloat(r3.x) >= 0.0f) ? 4294967295u : 0u;
                    if (r3.y != 0u) {
                        r3.z = asuint(asfloat(r2.w) * asfloat(r3.w));
                        float4 _746 = 1.0f.xxxx + (-cb1_1_m0[uint(asint(r0.w)) + 66u].xxxx);
                        float _747 = _746.w;
                        float _1474 = isnan(0.0f) ? _747 : (isnan(_747) ? 0.0f : max(_747, 0.0f));
                        r3.w = asuint(isnan(1.0f) ? _1474 : (isnan(_1474) ? 1.0f : min(_1474, 1.0f)));
                        r3.w = asuint(1.0f / asfloat(r3.w));
                        r3.x = asuint(asfloat(r3.w) * asfloat(r3.x));
                        r2.w = asuint(asfloat(r2.w) * (asfloat(r2.w)));
                        r2.w = asuint(asfloat(r2.w) * (asfloat(r2.w)));
                        r2.w = asuint(asfloat(r2.w) * (asfloat(r2.w)));
                        r2.w = asuint(asfloat(r2.w) * (cb1_1_m0[uint(asint(r0.w)) + 65u].x));
                        r3.w = (1.0f < asfloat(r2.w)) ? 4294967295u : 0u;
                        r2.w = asuint((r3.w != 0u) ? 1.0f : asfloat(r2.w));
                        r2.w = asuint(asfloat(r3.x) * asfloat(r2.w));
                        r2.w = asuint(asfloat(r2.w) * (asfloat(r3.z)));
                        float3 _823 = min(1.0f.xxx, cb1_1_m0[uint(asint(r0.w)) + 64u].xyz);
                        r3 = uint4(asuint(_823.x), r3.y, asuint(_823.y), asuint(_823.z));
                        float3 _830 = asfloat(r2.www) * asfloat(r3.xzw);
                        r3 = uint4(asuint(_830.x), r3.y, asuint(_830.y), asuint(_830.z));
                    }
                } else {
                    r3.y = asuint(0.0f);
                }
            } else {
                r3.y = asuint(0.0f);
            }
            uint3 _842 = uint3(r3.xzw & r3.yyy);
            r3 = uint4(_842.x, _842.y, _842.z, r3.w);
            float3 _849 = asfloat(r1.xyw) + asfloat(r3.xyz);
            r1 = uint4(asuint(_849.x), asuint(_849.y), r1.z, asuint(_849.z));
            r0.y = asint(r0.y) + 1;
            continue;
        }
    } else {
        r1 = uint4(asuint(cb1_1_m0[9u].x), asuint(cb1_1_m0[9u].y), r1.z, asuint(cb1_1_m0[9u].z));
    }
    bool3 _1487 = isnan(asfloat(r1.xyxw).xyw);
    bool3 _1488 = isnan(0.0f.xxx);
    float3 _1489 = max(asfloat(r1.xyxw).xyw, 0.0f.xxx);
    float3 _1490 = float3(_1487.x ? 0.0f.xxx.x : _1489.x, _1487.y ? 0.0f.xxx.y : _1489.y, _1487.z ? 0.0f.xxx.z : _1489.z);
    float3 _1485 = float3(_1488.x ? asfloat(r1.xyxw).xyw.x : _1490.x, _1488.y ? asfloat(r1.xyxw).xyw.y : _1490.y, _1488.z ? asfloat(r1.xyxw).xyw.z : _1490.z);
    bool3 _1492 = isnan(_1485);
    bool3 _1493 = isnan(1.0f.xxx);
    float3 _1494 = min(_1485, 1.0f.xxx);
    float3 _1495 = float3(_1492.x ? 1.0f.xxx.x : _1494.x, _1492.y ? 1.0f.xxx.y : _1494.y, _1492.z ? 1.0f.xxx.z : _1494.z);
    float3 _866 = float3(_1493.x ? _1485.x : _1495.x, _1493.y ? _1485.y : _1495.y, _1493.z ? _1485.z : _1495.z);
    r1 = uint4(asuint(_866.x), asuint(_866.y), r1.z, asuint(_866.z));
    float3 _872 = mad(v1, 0.5f.xxx, 0.5f.xxx);
    r2 = uint4(asuint(_872.x), asuint(_872.y), asuint(_872.z), r2.w);
    r0.y = asuint(1.0f);
    uint3 _878 = r1.zzz;
    bool3 _885 = bool3(_878.x != uint3(0u, 0u, 0u).x, _878.y != uint3(0u, 0u, 0u).y, _878.z != uint3(0u, 0u, 0u).z);
    float3 _886 =
        float3(_885.x ? asfloat(r2.xyz).x : asfloat(r1.xyw).x, _885.y ? asfloat(r2.xyz).y : asfloat(r1.xyw).y, _885.z ? asfloat(r2.xyz).z : asfloat(r1.xyw).z);
    r2 = uint4(asuint(_886.x), asuint(_886.y), asuint(_886.z), r2.w);
    r2.w = asuint((r1.z != 0u) ? asfloat(r0.y) : cb1_1_m0[9u].w);
    r1 = asuint(asfloat(r2) * cb1_1_m0[10u]);
    uint4 _907 = r0.zzzz;
    bool4 _910 =
        bool4(_907.x != uint4(0u, 0u, 0u, 0u).x, _907.y != uint4(0u, 0u, 0u, 0u).y, _907.z != uint4(0u, 0u, 0u, 0u).z, _907.w != uint4(0u, 0u, 0u, 0u).w);
    r1 = uint4(asuint(_910.x ? asfloat(r1.x) : asfloat(r2.x)), asuint(_910.y ? asfloat(r1.y) : asfloat(r2.y)), asuint(_910.z ? asfloat(r1.z) : asfloat(r2.z)),
        asuint(_910.w ? asfloat(r1.w) : asfloat(r2.w)));
    r0.y = asuint(cb1_1_m0[12u].y * cb1_1_m0[21u].w);
    r1.w = asuint(asfloat(r0.y) * asfloat(r1.w));
    if (!(asuint(cb1_1_m0[4u].x) != 0u)) {
        r0.y = asuint(asfloat(r0.x) + (-cb1_1_m0[20u].xxxx).y);
        r0.z = asuint((-cb1_1_m0[20u].xxxx).z + cb1_1_m0[20u].y);
        r0.y = asuint(asfloat(r0.y) / (asfloat(r0.z)));
        r0.y = asuint(asfloat(r0.y) * (cb1_1_m0[20u].z));
        r0.y = asuint(min(asfloat(r0.y), 255.9998931884765625f));
        r0.y = asuint(max(asfloat(r0.y), 1.0f));
        uint _982 = 0u & 31u;
        r0.z = gfxInsertBits(1056964608u, r0.y, _982, min((23u & 31u), (32u - _982)));
        r0.y = asuint(log2(asfloat(r0.y)));
        r0.z = asuint(asfloat(r0.z) * (512.0f));
        float2 _1005 = max(asfloat(r0.yz), (-2147483648.0f).xx);
        bool2 _1009 = bool2(_1005.x >= 2147483648.0f.xx.x, _1005.y >= 2147483648.0f.xx.y);
        int2 _1010 = int2(_1005);
        int2 _1011 = int2(_1009.x ? int2(2147483647, 2147483647).x : _1010.x, _1009.y ? int2(2147483647, 2147483647).y : _1010.y);
        uint2 _1462 = r0.yz & uint2(2147483647u, 2147483647u);
        bool2 _1013 = bool2(_1462.x > uint2(2139095040u, 2139095040u).x, _1462.y > uint2(2139095040u, 2139095040u).y);
        uint2 _1015 = uint2(int2(_1013.x ? int2(0, 0).x : _1011.x, _1013.y ? int2(0, 0).y : _1011.y));
        r0 = uint4(r0.x, _1015.x, _1015.y, r0.w);
        uint _1021 = 4u & 31u;
        r0.w = gfxExtractBits(r0.z, _1021, min((4u & 31u), (32u - _1021)));
        r0.z = r0.z & 15u;
        r0.z = asuint(float(asint(r0.z)));
        r0.z = asuint(asfloat(r0.z) * (0.066666670143604278564453125f));
        r0.y = (asint(r0.y) * 16) + asint(r0.w);
        r0.w = asuint(float(asint(r0.y)));
        r0.w = asuint((-asfloat(r0.wwww)).w + 127.0f);
        float _1070 = max(asfloat(r0.w), -2147483648.0f);
        r2.x = ((r0.w & 2147483647u) > 2139095040u) ? 0 : ((_1070 >= 2147483648.0f) ? 2147483647 : int(_1070));
        r2.y = asuint(cb1_1_m0[20u].y * 0.00390625f);
        r2.y = (asfloat(r2.y) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
        r2.z = asuint((-asfloat(r0.wwww)).z + 128.0f);
        r2.w = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
        r3.x = (!(asfloat(r2.w) == 0.0f)) ? 4294967295u : 0u;
        r3.y = r2.w & 2139095040u;
        r3.y = asint(r3.y) + (-1056964608);
        r3.y = r3.x & r3.y;
        r3.y = asint(r3.y) >> (23 & int(31u));
        uint _1148 = 0u & 31u;
        r2.w = gfxInsertBits(1056964608u, r2.w, _1148, min((23u & 31u), (32u - _1148)));
        r2.w = r3.x & r2.w;
        float2 _1169 = asfloat(r2.zw) * float2(0.0078125f, 32.0f);
        r2 = uint4(r2.x, r2.y, asuint(_1169.x), asuint(_1169.y));
        float _1174 = max(asfloat(r2.w), -2147483648.0f);
        r2.w = ((r2.w & 2147483647u) > 2139095040u) ? 0 : ((_1174 >= 2147483648.0f) ? 2147483647 : int(_1174));
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
        float _1301 = max(asfloat(r0.y), -2147483648.0f);
        r2.x = ((r0.y & 2147483647u) > 2139095040u) ? 0 : ((_1301 >= 2147483648.0f) ? 2147483647 : int(_1301));
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
        r2.x = asuint(mad(asfloat(r0.z), asfloat(r0.y), asfloat(r0.w)));
    }
    r0.y = (asint(cb1_1_m0[4u].x) == 4) ? 4294967295u : 0u;
    uint4 _1406 = r0.yyyy;
    bool4 _1409 =
        bool4(_1406.x != uint4(0u, 0u, 0u, 0u).x, _1406.y != uint4(0u, 0u, 0u, 0u).y, _1406.z != uint4(0u, 0u, 0u, 0u).z, _1406.w != uint4(0u, 0u, 0u, 0u).w);
    o2 = float4(_1409.x ? float4(0.0f, 0.0f, 0.0f, 1.0f).x : asfloat(r1.x), _1409.y ? float4(0.0f, 0.0f, 0.0f, 1.0f).y : asfloat(r1.y),
        _1409.z ? float4(0.0f, 0.0f, 0.0f, 1.0f).z : asfloat(r1.z), _1409.w ? float4(0.0f, 0.0f, 0.0f, 1.0f).w : asfloat(r1.w));
    position.w = asfloat(r0.x);
    r2 = uint4(r2.x, asuint(v2.x), asuint(v2.y), r2.w);
    o1 = asfloat(r2.yzx);
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
