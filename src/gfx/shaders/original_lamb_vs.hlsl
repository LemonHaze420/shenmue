#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb25_struct : register(b1) {
    float4 cb1_1_m0[25] : packoffset(c0);
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
    float3 v0 = 0;
    float3 v1 = 0;
    float2 v2 = 0;
    float3 o1 = 0;
    float4 o2 = 0;
    float3 _692 = 0;
    float3 _701 = 0;
    float2 _711 = 0;

    inputPosition0 = input.inputPosition0;
    inputNormal0 = input.inputNormal0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _695 = _692;
    _695.x = inputPosition0.x;
    float3 _697 = _695;
    _697.y = inputPosition0.y;
    float3 _699 = _697;
    _699.z = inputPosition0.z;
    v0 = _699;
    float3 _704 = _701;
    _704.x = inputNormal0.x;
    float3 _706 = _704;
    _706.y = inputNormal0.y;
    float3 _708 = _706;
    _708.z = inputNormal0.z;
    v1 = _708;
    float2 _714 = _711;
    _714.x = inputTexcoord0.x;
    float2 _716 = _714;
    _716.y = inputTexcoord0.y;
    v2 = _716;
    r0 = uint4(asuint(v0.x), asuint(v0.y), asuint(v0.z), r0.w);
    r0.w = asuint(1.0f);
    position.x = dot(asfloat(r0), cb1_1_m0[5u]);
    position.y = dot(asfloat(r0), cb1_1_m0[6u]);
    position.z = dot(asfloat(r0), cb1_1_m0[7u]);
    r0.x = asuint(dot(asfloat(r0), cb1_1_m0[8u]));
    int4 _74 = asint(cb1_1_m0[4u].xxxx);
    bool4 _82 = bool4(_74.x == int4(0, 1, 2, 4).x, _74.y == int4(0, 1, 2, 4).y, _74.z == int4(0, 1, 2, 4).z, _74.w == int4(0, 1, 2, 4).w);
    r1 = uint4(_82.x ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).x : uint4(0u, 0u, 0u, 0u).x,
        _82.y ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).y : uint4(0u, 0u, 0u, 0u).y,
        _82.z ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).z : uint4(0u, 0u, 0u, 0u).z,
        _82.w ? uint4(4294967295u, 4294967295u, 4294967295u, 4294967295u).w : uint4(0u, 0u, 0u, 0u).w);
    r0.y = r1.y | r1.x;
    r0.z = asuint(dot(v1, cb1_1_m0[24u].xyz));
    r0.z = asuint(max(asfloat(r0.z), 0.0f));
    float3 _117 = asfloat(r0.zzz) + cb1_1_m0[9u].xyz;
    r2 = uint4(asuint(_117.x), asuint(_117.y), asuint(_117.z), r2.w);
    r0.z = asuint(dot(v1, cb1_1_m0[13u].xyz));
    r0.z = asuint(max(asfloat(r0.z), 0.0f));
    float3 _135 = asfloat(r0.zzz) + asfloat(r2.xyz);
    r2 = uint4(asuint(_135.x), asuint(_135.y), asuint(_135.z), r2.w);
    float3 _141 = mad(v1, 0.5f.xxx, 0.5f.xxx);
    r3 = uint4(asuint(_141.x), asuint(_141.y), asuint(_141.z), r3.w);
    r3.w = asuint(1.0f);
    uint4 _147 = r1.zzzz;
    bool4 _151 =
        bool4(_147.x != uint4(0u, 0u, 0u, 0u).x, _147.y != uint4(0u, 0u, 0u, 0u).y, _147.z != uint4(0u, 0u, 0u, 0u).z, _147.w != uint4(0u, 0u, 0u, 0u).w);
    r3 = uint4(asuint(_151.x ? asfloat(r3.x) : cb1_1_m0[9u].x), asuint(_151.y ? asfloat(r3.y) : cb1_1_m0[9u].y),
        asuint(_151.z ? asfloat(r3.z) : cb1_1_m0[9u].z), asuint(_151.w ? asfloat(r3.w) : cb1_1_m0[9u].w));
    r2.w = asuint(cb1_1_m0[9u].w);
    uint4 _160 = r0.yyyy;
    bool4 _163 =
        bool4(_160.x != uint4(0u, 0u, 0u, 0u).x, _160.y != uint4(0u, 0u, 0u, 0u).y, _160.z != uint4(0u, 0u, 0u, 0u).z, _160.w != uint4(0u, 0u, 0u, 0u).w);
    r2 = uint4(asuint(_163.x ? asfloat(r2.x) : asfloat(r3.x)), asuint(_163.y ? asfloat(r2.y) : asfloat(r3.y)), asuint(_163.z ? asfloat(r2.z) : asfloat(r3.z)),
        asuint(_163.w ? asfloat(r2.w) : asfloat(r3.w)));
    float3 _171 = asfloat(r2.xyz) * cb1_1_m0[10u].xyz;
    r0 = uint4(r0.x, asuint(_171.x), asuint(_171.y), asuint(_171.z));
    uint4 _177 = asuint(cb1_1_m0[4u].xxxx);
    bool4 _182 =
        bool4(_177.x != uint4(0u, 0u, 0u, 0u).x, _177.y != uint4(0u, 0u, 0u, 0u).y, _177.z != uint4(0u, 0u, 0u, 0u).z, _177.w != uint4(0u, 0u, 0u, 0u).w);
    float3 _184 = float4(_182.x ? asfloat(r2.xyzx).x : asfloat(r0.yzwy).x, _182.y ? asfloat(r2.xyzx).y : asfloat(r0.yzwy).y,
        _182.z ? asfloat(r2.xyzx).z : asfloat(r0.yzwy).z, _182.w ? asfloat(r2.xyzx).w : asfloat(r0.yzwy).w)
                      .xyz;
    bool3 _748 = isnan(_184);
    bool3 _749 = isnan(0.0f.xxx);
    float3 _750 = max(_184, 0.0f.xxx);
    float3 _751 = float3(_748.x ? 0.0f.xxx.x : _750.x, _748.y ? 0.0f.xxx.y : _750.y, _748.z ? 0.0f.xxx.z : _750.z);
    float3 _746 = float3(_749.x ? _184.x : _751.x, _749.y ? _184.y : _751.y, _749.z ? _184.z : _751.z);
    bool3 _753 = isnan(_746);
    bool3 _754 = isnan(1.0f.xxx);
    float3 _755 = min(_746, 1.0f.xxx);
    float3 _756 = float3(_753.x ? 1.0f.xxx.x : _755.x, _753.y ? 1.0f.xxx.y : _755.y, _753.z ? 1.0f.xxx.z : _755.z);
    float3 _187 = float3(_754.x ? _746.x : _756.x, _754.y ? _746.y : _756.y, _754.z ? _746.z : _756.z);
    r3 = uint4(asuint(_187.x), asuint(_187.y), asuint(_187.z), r3.w);
    r0.y = asuint(cb1_1_m0[12u].y * cb1_1_m0[21u].w);
    float4 _204 = asfloat(r0.yyyy) * asfloat(r2.wwww);
    float _205 = _204.w;
    float _757 = isnan(0.0f) ? _205 : (isnan(_205) ? 0.0f : max(_205, 0.0f));
    r3.w = asuint(isnan(1.0f) ? _757 : (isnan(_757) ? 1.0f : min(_757, 1.0f)));
    if (!(asuint(cb1_1_m0[4u].x) != 0u)) {
        r0.y = asuint(asfloat(r0.x) + (-cb1_1_m0[20u].xxxx).y);
        r0.z = asuint((-cb1_1_m0[20u].xxxx).z + cb1_1_m0[20u].y);
        r0.y = asuint(asfloat(r0.y) / (asfloat(r0.z)));
        r0.y = asuint(asfloat(r0.y) * (cb1_1_m0[20u].z));
        r0.y = asuint(min(asfloat(r0.y), 255.9998931884765625f));
        r0.y = asuint(max(asfloat(r0.y), 1.0f));
        uint _264 = 0u & 31u;
        r0.z = gfxInsertBits(1056964608u, r0.y, _264, min((23u & 31u), (32u - _264)));
        r0.y = asuint(log2(asfloat(r0.y)));
        r0.z = asuint(asfloat(r0.z) * (512.0f));
        float2 _287 = max(asfloat(r0.yz), (-2147483648.0f).xx);
        bool2 _291 = bool2(_287.x >= 2147483648.0f.xx.x, _287.y >= 2147483648.0f.xx.y);
        int2 _292 = int2(_287);
        int2 _293 = int2(_291.x ? int2(2147483647, 2147483647).x : _292.x, _291.y ? int2(2147483647, 2147483647).y : _292.y);
        uint2 _734 = r0.yz & uint2(2147483647u, 2147483647u);
        bool2 _295 = bool2(_734.x > uint2(2139095040u, 2139095040u).x, _734.y > uint2(2139095040u, 2139095040u).y);
        uint2 _297 = uint2(int2(_295.x ? int2(0, 0).x : _293.x, _295.y ? int2(0, 0).y : _293.y));
        r0 = uint4(r0.x, _297.x, _297.y, r0.w);
        uint _303 = 4u & 31u;
        r0.w = gfxExtractBits(r0.z, _303, min((4u & 31u), (32u - _303)));
        r0.z = r0.z & 15u;
        r0.z = asuint(float(asint(r0.z)));
        r0.z = asuint(asfloat(r0.z) * (0.066666670143604278564453125f));
        r0.y = (asint(r0.y) * 16) + asint(r0.w);
        r0.w = asuint(float(asint(r0.y)));
        r0.w = asuint((-asfloat(r0.wwww)).w + 127.0f);
        float _352 = max(asfloat(r0.w), -2147483648.0f);
        r1.x = ((r0.w & 2147483647u) > 2139095040u) ? 0 : ((_352 >= 2147483648.0f) ? 2147483647 : int(_352));
        r1.y = asuint(cb1_1_m0[20u].y * 0.00390625f);
        r1.y = (asfloat(r1.y) >= cb1_1_m0[20u].x) ? 4294967295u : 0u;
        r1.z = asuint((-asfloat(r0.wwww)).z + 128.0f);
        r1.z = asuint(asfloat(r1.z) * (0.0078125f));
        r2.x = asuint(cb1_1_m0[20u].y / cb1_1_m0[20u].x);
        r2.y = (!(asfloat(r2.x) == 0.0f)) ? 4294967295u : 0u;
        r2.z = r2.x & 2139095040u;
        r2.z = asint(r2.z) + (-1056964608);
        r2.z = r2.y & r2.z;
        r2.z = asint(r2.z) >> (23 & int(31u));
        uint _435 = 0u & 31u;
        r2.x = gfxInsertBits(1056964608u, r2.x, _435, min((23u & 31u), (32u - _435)));
        r2.x = r2.y & r2.x;
        r2.x = asuint(asfloat(r2.x) * (32.0f));
        float _458 = max(asfloat(r2.x), -2147483648.0f);
        r2.x = ((r2.x & 2147483647u) > 2139095040u) ? 0 : ((_458 >= 2147483648.0f) ? 2147483647 : int(_458));
        r2.y = asint(r2.z) << (4 & int(31u));
        uint2 _479 = uint2(asint(r2.xy) + int2(-16, -16));
        r2 = uint4(_479.x, _479.y, r2.z, r2.w);
        r2.x = r2.x | r2.y;
        r2.y = (asint(r1.x) < asint(r2.x)) ? 4294967295u : 0u;
        r2.z = asuint(float(asint(r2.x)));
        r0.w = asuint((-asfloat(r0.wwww)).w + asfloat(r2.z));
        r0.w = asuint(asfloat(r0.w) / (asfloat(r2.z)));
        r0.w = r0.w & r2.y;
        r1.x = (asint(r1.x) >= asint(r2.x)) ? 4294967295u : 0u;
        r0.w = asuint((r1.x != 0u) ? 0.0f : asfloat(r0.w));
        r0.w = asuint((r1.y != 0u) ? asfloat(r1.z) : asfloat(r0.w));
        r0.y = asint(r0.y) + 1;
        r0.y = asuint(float(asint(r0.y)));
        r0.y = asuint(min(asfloat(r0.y), 127.0f));
        r0.y = asuint((-asfloat(r0.yyyy)).y + 127.0f);
        float _580 = max(asfloat(r0.y), -2147483648.0f);
        r1.x = ((r0.y & 2147483647u) > 2139095040u) ? 0 : ((_580 >= 2147483648.0f) ? 2147483647 : int(_580));
        r1.z = asuint((-asfloat(r0.yyyy)).z + 128.0f);
        r1.z = asuint(asfloat(r1.z) * (0.0078125f));
        r2.y = (asint(r1.x) < asint(r2.x)) ? 4294967295u : 0u;
        r0.y = asuint((-asfloat(r0.yyyy)).y + asfloat(r2.z));
        r0.y = asuint(asfloat(r0.y) / (asfloat(r2.z)));
        r0.y = r0.y & r2.y;
        r1.x = (asint(r1.x) >= asint(r2.x)) ? 4294967295u : 0u;
        r0.y = asuint((r1.x != 0u) ? 0.0f : asfloat(r0.y));
        r0.y = asuint((r1.y != 0u) ? asfloat(r1.z) : asfloat(r0.y));
        r0.y = asuint((-asfloat(r0.wwww)).y + asfloat(r0.y));
        r1.x = asuint(mad(asfloat(r0.z), asfloat(r0.y), asfloat(r0.w)));
    }
    uint4 _677 = r1.wwww;
    bool4 _680 =
        bool4(_677.x != uint4(0u, 0u, 0u, 0u).x, _677.y != uint4(0u, 0u, 0u, 0u).y, _677.z != uint4(0u, 0u, 0u, 0u).z, _677.w != uint4(0u, 0u, 0u, 0u).w);
    o2 = float4(
        _680.x ? 0.0f.xxxx.x : asfloat(r3.x), _680.y ? 0.0f.xxxx.y : asfloat(r3.y), _680.z ? 0.0f.xxxx.z : asfloat(r3.z), _680.w ? 0.0f.xxxx.w : asfloat(r3.w));
    position.w = asfloat(r0.x);
    r1 = uint4(r1.x, asuint(v2.x), asuint(v2.y), r1.w);
    o1 = asfloat(r1.yzx);
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
