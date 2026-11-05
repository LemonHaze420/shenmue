#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb21_struct : register(b1) {
    float4 cb1_1_m0[21] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
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
    float4 inputDiffuse0 = 0;
    float2 inputTexcoord0 = 0;
    float2 outputTexcoord0 = 0;
    float outputFog0 = 0;
    float4 outputColor0 = 0;
    float2 outputDepth0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float2 v3 = 0;
    float3 o1 = 0;
    float4 o2 = 0;
    float2 o3 = 0;
    float3 _561 = 0;
    float4 _570 = 0;
    float2 _582 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _564 = _561;
    _564.x = inputPosition0.x;
    float3 _566 = _564;
    _566.y = inputPosition0.y;
    float3 _568 = _566;
    _568.z = inputPosition0.z;
    v0 = _568;
    float4 _573 = _570;
    _573.x = inputDiffuse0.x;
    float4 _575 = _573;
    _575.y = inputDiffuse0.y;
    float4 _577 = _575;
    _577.z = inputDiffuse0.z;
    float4 _579 = _577;
    _579.w = inputDiffuse0.w;
    v1 = _579;
    float2 _585 = _582;
    _585.x = inputTexcoord0.x;
    float2 _587 = _585;
    _587.y = inputTexcoord0.y;
    v3 = _587;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    r1.z = asuint(dot(asfloat(r0), cb1_1_m0[7u]));
    r1.w = asuint(dot(asfloat(r0), cb1_1_m0[8u]));
    if (asuint(cb1_1_m0[1u].x) != 0u) {
        r0.x = asuint(asfloat(r1.w) + (-cb1_1_m0[20u].xxxx).x);
        r0.y = asuint((-cb1_1_m0[20u].xxxx).y + cb1_1_m0[20u].y);
        r0.x = asuint(asfloat(r0.x) / (asfloat(r0.y)));
        r0.x = asuint(asfloat(r0.x) * (cb1_1_m0[20u].z));
        r0.x = asuint(min(asfloat(r0.x), 255.9998931884765625f));
        r0.x = asuint(max(asfloat(r0.x), 1.0f));
        uint _130 = 0u & 31u;
        r0.y = gfxInsertBits(1056964608u, r0.x, _130, min((23u & 31u), (32u - _130)));
        r0.x = asuint(log2(asfloat(r0.x)));
        r0.y = asuint(asfloat(r0.y) * (512.0f));
        float2 _154 = max(asfloat(r0.xy), (-2147483648.0f).xx);
        bool2 _158 = bool2(_154.x >= 2147483648.0f.xx.x, _154.y >= 2147483648.0f.xx.y);
        int2 _159 = int2(_154);
        int2 _160 = int2(_158.x ? int2(2147483647, 2147483647).x : _159.x, _158.y ? int2(2147483647, 2147483647).y : _159.y);
        uint2 _609 = r0.xy & uint2(2147483647u, 2147483647u);
        bool2 _163 = bool2(_609.x > uint2(2139095040u, 2139095040u).x, _609.y > uint2(2139095040u, 2139095040u).y);
        uint2 _165 = uint2(int2(_163.x ? int2(0, 0).x : _160.x, _163.y ? int2(0, 0).y : _160.y));
        r0 = uint4(_165.x, _165.y, r0.z, r0.w);
        uint _172 = 4u & 31u;
        r0.z = gfxExtractBits(r0.y, _172, min((4u & 31u), (32u - _172)));
        r0.y = r0.y & 15u;
        r0.y = asuint(float(asint(r0.y)));
        r0.y = asuint(asfloat(r0.y) * (0.066666670143604278564453125f));
        r0.x = (asint(r0.x) * 16) + asint(r0.z);
        r0.z = asuint(float(asint(r0.x)));
        r0.z = asuint((-asfloat(r0.zzzz)).z + 127.0f);
        float _221 = max(asfloat(r0.z), -2147483648.0f);
        r0.w = ((r0.z & 2147483647u) > 2139095040u) ? 0 : ((_221 >= 2147483648.0f) ? 2147483647 : int(_221));
        r1.x = asuint(cb1_1_m0[20u].y * 0.00390625f);
        r1.x = (asfloat(r1.x) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
        r1.y = asuint((-asfloat(r0.zzzz)).y + 128.0f);
        r1.y = asuint(asfloat(r1.y) * (0.0078125f));
        r2.x = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
        r2.y = (!(asfloat(r2.x) == 0.0f)) ? 4294967295u : 0u;
        r2.z = r2.x & 2139095040u;
        r2.z = asint(r2.z) + (-1056964608);
        r2.z = r2.y & r2.z;
        r2.z = asint(r2.z) >> (23 & int(31u));
        uint _306 = 0u & 31u;
        r2.x = gfxInsertBits(1056964608u, r2.x, _306, min((23u & 31u), (32u - _306)));
        r2.x = r2.y & r2.x;
        r2.x = asuint(asfloat(r2.x) * (32.0f));
        float _329 = max(asfloat(r2.x), -2147483648.0f);
        r2.x = ((r2.x & 2147483647u) > 2139095040u) ? 0 : ((_329 >= 2147483648.0f) ? 2147483647 : int(_329));
        r2.y = asint(r2.z) << (4 & int(31u));
        uint2 _351 = uint2(asint(r2.xy) + int2(-16, -16));
        r2 = uint4(_351.x, _351.y, r2.z, r2.w);
        r2.x = r2.x | r2.y;
        r2.y = (asint(r0.w) < asint(r2.x)) ? 4294967295u : 0u;
        r2.z = asuint(float(asint(r2.x)));
        r0.z = asuint((-asfloat(r0.zzzz)).z + asfloat(r2.z));
        r0.z = asuint(asfloat(r0.z) / (asfloat(r2.z)));
        r0.z = r0.z & r2.y;
        r0.w = (asint(r0.w) >= asint(r2.x)) ? 4294967295u : 0u;
        r0.z = asuint((r0.w != 0u) ? 0.0f : asfloat(r0.z));
        r0.z = asuint((r1.x != 0u) ? asfloat(r1.y) : asfloat(r0.z));
        r0.x = asint(r0.x) + 1;
        r0.x = asuint(float(asint(r0.x)));
        r0.x = asuint(min(asfloat(r0.x), 127.0f));
        r0.x = asuint((-asfloat(r0.xxxx)).x + 127.0f);
        float _453 = max(asfloat(r0.x), -2147483648.0f);
        r0.w = ((r0.x & 2147483647u) > 2139095040u) ? 0 : ((_453 >= 2147483648.0f) ? 2147483647 : int(_453));
        r1.y = asuint((-asfloat(r0.xxxx)).y + 128.0f);
        r1.y = asuint(asfloat(r1.y) * (0.0078125f));
        r2.y = (asint(r0.w) < asint(r2.x)) ? 4294967295u : 0u;
        r0.x = asuint((-asfloat(r0.xxxx)).x + asfloat(r2.z));
        r0.x = asuint(asfloat(r0.x) / (asfloat(r2.z)));
        r0.x = r0.x & r2.y;
        r0.w = (asint(r0.w) >= asint(r2.x)) ? 4294967295u : 0u;
        r0.x = asuint((r0.w != 0u) ? 0.0f : asfloat(r0.x));
        r0.x = asuint((r1.x != 0u) ? asfloat(r1.y) : asfloat(r0.x));
        r0.x = asuint((-asfloat(r0.zzzz)).x + asfloat(r0.x));
        r0.x = asuint(mad(asfloat(r0.y), asfloat(r0.x), asfloat(r0.z)));
    } else {
        r0.x = asuint(0.0f);
    }
    position = float4(position.x, position.y, asfloat(r1.z), asfloat(r1.w));
    o2 = v1;
    r0 = uint4(r0.x, asuint(v3.x), asuint(v3.y), r0.w);
    o1 = asfloat(r0.yzx);
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
