#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb22_struct : register(b1) {
    float4 cb1_1_m0[22] : packoffset(c0);
};

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
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
    float4 inputDiffuse0 = 0;
    float2 inputTexcoord0 = 0;
    float2 outputTexcoord0 = 0;
    float outputFog0 = 0;
    float4 outputColor0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    uint4 r3 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float2 v2 = 0;
    float3 o1 = 0;
    float4 o2 = 0;
    float3 _611 = 0;
    float4 _620 = 0;
    float2 _632 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _614 = _611;
    _614.x = inputPosition0.x;
    float3 _616 = _614;
    _616.y = inputPosition0.y;
    float3 _618 = _616;
    _618.z = inputPosition0.z;
    v0 = _618;
    float4 _623 = _620;
    _623.x = inputDiffuse0.x;
    float4 _625 = _623;
    _625.y = inputDiffuse0.y;
    float4 _627 = _625;
    _627.z = inputDiffuse0.z;
    float4 _629 = _627;
    _629.w = inputDiffuse0.w;
    v1 = _629;
    float2 _635 = _632;
    _635.x = inputTexcoord0.x;
    float2 _637 = _635;
    _637.y = inputTexcoord0.y;
    v2 = _637;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    r0.x = asuint(dot(asfloat(r0), cb1_1_m0[8u]));
    r1 = asuint(v1 * cb1_1_m0[21u]);
    int2 _80 = asint(cb1_1_m0[4u].xx);
    bool2 _86 = bool2(_80.x == int2(2, 1).x, _80.y == int2(2, 1).y);
    uint2 _92 = uint2(_86.x ? uint2(4294967295u, 4294967295u).x : uint2(0u, 0u).x, _86.y ? uint2(4294967295u, 4294967295u).y : uint2(0u, 0u).y);
    r0 = uint4(r0.x, _92.x, _92.y, r0.w);
    r0.y = r0.z | r0.y;
    uint4 _107 = r0.yyyy;
    bool4 _113 =
        bool4(_107.x != uint4(0u, 0u, 0u, 0u).x, _107.y != uint4(0u, 0u, 0u, 0u).y, _107.z != uint4(0u, 0u, 0u, 0u).z, _107.w != uint4(0u, 0u, 0u, 0u).w);
    r1 = uint4(asuint(_113.x ? float4(0.0f, 0.0f, 0.0f, 1.0f).x : asfloat(r1.x)), asuint(_113.y ? float4(0.0f, 0.0f, 0.0f, 1.0f).y : asfloat(r1.y)),
        asuint(_113.z ? float4(0.0f, 0.0f, 0.0f, 1.0f).z : asfloat(r1.z)), asuint(_113.w ? float4(0.0f, 0.0f, 0.0f, 1.0f).w : asfloat(r1.w)));
    r0.y = asuint(asfloat(r1.w) * cb1_1_m0[10u].w);
    o2.w = asfloat(r0.y) * cb1_1_m0[12u].y;
    if (!(asuint(cb1_1_m0[4u].x) != 0u)) {
        r0.y = asuint(asfloat(r0.x) + (-cb1_1_m0[20u].xxxx).y);
        r0.z = asuint((-cb1_1_m0[20u].xxxx).z + cb1_1_m0[20u].y);
        r0.y = asuint(asfloat(r0.y) / (asfloat(r0.z)));
        r0.y = asuint(asfloat(r0.y) * (cb1_1_m0[20u].z));
        r0.y = asuint(min(asfloat(r0.y), 255.9998931884765625f));
        r0.y = asuint(max(asfloat(r0.y), 1.0f));
        uint _189 = 0u & 31u;
        r0.z = gfxInsertBits(1056964608u, r0.y, _189, min((23u & 31u), (32u - _189)));
        r0.y = asuint(log2(asfloat(r0.y)));
        r0.z = asuint(asfloat(r0.z) * (512.0f));
        float2 _211 = max(asfloat(r0.yz), (-2147483648.0f).xx);
        bool2 _214 = bool2(_211.x >= 2147483648.0f.xx.x, _211.y >= 2147483648.0f.xx.y);
        int2 _215 = int2(_211);
        int2 _216 = int2(_214.x ? int2(2147483647, 2147483647).x : _215.x, _214.y ? int2(2147483647, 2147483647).y : _215.y);
        uint2 _654 = r0.yz & uint2(2147483647u, 2147483647u);
        bool2 _219 = bool2(_654.x > uint2(2139095040u, 2139095040u).x, _654.y > uint2(2139095040u, 2139095040u).y);
        uint2 _221 = uint2(int2(_219.x ? int2(0, 0).x : _216.x, _219.y ? int2(0, 0).y : _216.y));
        r0 = uint4(r0.x, _221.x, _221.y, r0.w);
        uint _227 = 4u & 31u;
        r0.w = gfxExtractBits(r0.z, _227, min((4u & 31u), (32u - _227)));
        r0.z = r0.z & 15u;
        r0.z = asuint(float(asint(r0.z)));
        r0.z = asuint(asfloat(r0.z) * (0.066666670143604278564453125f));
        r0.y = (asint(r0.y) * 16) + asint(r0.w);
        r0.w = asuint(float(asint(r0.y)));
        r0.w = asuint((-asfloat(r0.wwww)).w + 127.0f);
        float _276 = max(asfloat(r0.w), -2147483648.0f);
        r1.w = ((r0.w & 2147483647u) > 2139095040u) ? 0 : ((_276 >= 2147483648.0f) ? 2147483647 : int(_276));
        r2.x = asuint(cb1_1_m0[20u].y * 0.00390625f);
        r2.x = (asfloat(r2.x) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
        r2.y = asuint((-asfloat(r0.wwww)).y + 128.0f);
        r2.z = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
        r2.w = (!(asfloat(r2.z) == 0.0f)) ? 4294967295u : 0u;
        r3.x = r2.z & 2139095040u;
        r3.x = asint(r3.x) + (-1056964608);
        r3.x = r2.w & r3.x;
        r3.x = asint(r3.x) >> (23 & int(31u));
        uint _354 = 0u & 31u;
        r2.z = gfxInsertBits(1056964608u, r2.z, _354, min((23u & 31u), (32u - _354)));
        r2.z = r2.w & r2.z;
        float2 _375 = asfloat(r2.yz) * float2(0.0078125f, 32.0f);
        r2 = uint4(r2.x, asuint(_375.x), asuint(_375.y), r2.w);
        float _380 = max(asfloat(r2.z), -2147483648.0f);
        r2.z = ((r2.z & 2147483647u) > 2139095040u) ? 0 : ((_380 >= 2147483648.0f) ? 2147483647 : int(_380));
        r2.w = asint(r3.x) << (4 & int(31u));
        uint2 _402 = uint2(asint(r2.zw) + int2(-16, -16));
        r2 = uint4(r2.x, r2.y, _402.x, _402.y);
        r2.z = r2.z | r2.w;
        r2.w = (asint(r1.w) < asint(r2.z)) ? 4294967295u : 0u;
        r3.x = asuint(float(asint(r2.z)));
        r0.w = asuint((-asfloat(r0.wwww)).w + asfloat(r3.x));
        r0.w = asuint(asfloat(r0.w) / (asfloat(r3.x)));
        r0.w = r0.w & r2.w;
        r1.w = (asint(r1.w) >= asint(r2.z)) ? 4294967295u : 0u;
        r0.w = asuint((r1.w != 0u) ? 0.0f : asfloat(r0.w));
        r0.w = asuint((r2.x != 0u) ? asfloat(r2.y) : asfloat(r0.w));
        r0.y = asint(r0.y) + 1;
        r0.y = asuint(float(asint(r0.y)));
        r0.y = asuint(min(asfloat(r0.y), 127.0f));
        r0.y = asuint((-asfloat(r0.yyyy)).y + 127.0f);
        float _503 = max(asfloat(r0.y), -2147483648.0f);
        r1.w = ((r0.y & 2147483647u) > 2139095040u) ? 0 : ((_503 >= 2147483648.0f) ? 2147483647 : int(_503));
        r2.y = asuint((-asfloat(r0.yyyy)).y + 128.0f);
        r2.y = asuint(asfloat(r2.y) * (0.0078125f));
        r2.w = (asint(r1.w) < asint(r2.z)) ? 4294967295u : 0u;
        r0.y = asuint((-asfloat(r0.yyyy)).y + asfloat(r3.x));
        r0.y = asuint(asfloat(r0.y) / (asfloat(r3.x)));
        r0.y = r0.y & r2.w;
        r1.w = (asint(r1.w) >= asint(r2.z)) ? 4294967295u : 0u;
        r0.y = asuint((r1.w != 0u) ? 0.0f : asfloat(r0.y));
        r0.y = asuint((r2.x != 0u) ? asfloat(r2.y) : asfloat(r0.y));
        r0.y = asuint((-asfloat(r0.wwww)).y + asfloat(r0.y));
        r2.x = asuint(mad(asfloat(r0.z), asfloat(r0.y), asfloat(r0.w)));
    }
    position.w = asfloat(r0.x);
    o2 = float4(asfloat(r1.x), asfloat(r1.y), asfloat(r1.z), o2.w);
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
