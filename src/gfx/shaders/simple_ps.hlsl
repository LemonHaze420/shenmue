#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb5_struct : register(b1) {
    float4 cb1_1_m0[5] : packoffset(c0);
};

SamplerState s0 : register(s0);
Texture2D<float4> t0 : register(t0);

struct PixelInput {
    float4 inputColor0 : COLOR0;
    float inputFog0 : FOG0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

void _discard(bool _295) {
    if (_295) {
        discard;
    }
}

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float2 inputTexcoord0 = 0;
    float inputFog0 = 0;
    float4 inputColor0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    float3 v1 = 0;
    float4 v2 = 0;
    float3 _303 = 0;
    float4 _312 = 0;

    inputTexcoord0 = input.inputTexcoord0;
    inputFog0 = input.inputFog0;
    inputColor0 = input.inputColor0;
    float3 _306 = _303;
    _306.x = inputTexcoord0.x;
    float3 _308 = _306;
    _308.y = inputTexcoord0.y;
    float3 _310 = _308;
    _310.z = inputFog0;
    v1 = _310;
    float4 _315 = _312;
    _315.x = inputColor0.x;
    float4 _317 = _315;
    _317.y = inputColor0.y;
    float4 _319 = _317;
    _319.z = inputColor0.z;
    float4 _321 = _319;
    _321.w = inputColor0.w;
    v2 = _321;
    r0 = asuint(t0.Sample(s0, v1.xyxx.xy));
    int2 _47 = asint(cb1_1_m0[4u].xx);
    bool2 _53 = bool2(_47.x == int2(0, 3).x, _47.y == int2(0, 3).y);
    uint2 _59 = uint2(_53.x ? uint2(4294967295u, 4294967295u).x : uint2(0u, 0u).x, _53.y ? uint2(4294967295u, 4294967295u).y : uint2(0u, 0u).y);
    r1 = uint4(_59.x, _59.y, r1.z, r1.w);
    r1.y = r1.y | r1.x;
    r0 = asuint(asfloat(r0) * (v2));
    uint4 _79 = r1.yyyy;
    bool4 _84 = bool4(_79.x != uint4(0u, 0u, 0u, 0u).x, _79.y != uint4(0u, 0u, 0u, 0u).y, _79.z != uint4(0u, 0u, 0u, 0u).z, _79.w != uint4(0u, 0u, 0u, 0u).w);
    r0 = uint4(
        asuint(_84.x ? asfloat(r0.x) : v2.x), asuint(_84.y ? asfloat(r0.y) : v2.y), asuint(_84.z ? asfloat(r0.z) : v2.z), asuint(_84.w ? asfloat(r0.w) : v2.w));
    r0.w = asuint((asuint(cb1_1_m0[4u].z) != 0u) ? 1.0f : asfloat(r0.w));
    if (asuint(cb1_1_m0[2u].x) != 0u) {
        switch (asint(cb1_1_m0[2u].z)) {
        case 0: {
            _discard(4294967295u != 0u);
            break;
        }
        case 1: {
            r1.y = (asfloat(r0.w) < cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _133 = uint(r1.y);
            _discard(_133 == 0u);
            break;
        }
        case 2: {
            r1.y = asuint(asfloat(r0.w) + (-cb1_1_m0[2u].yyyy).y);
            r1.y = (abs(asfloat(r1.yyyy)).y < 9.9999997473787516355514526367188e-06f) ? 4294967295u : 0u;
            uint _157 = uint(r1.y);
            _discard(_157 == 0u);
            break;
        }
        case 3: {
            r1.y = (cb1_1_m0[2u].y >= asfloat(r0.w)) ? 4294967295u : 0u;
            uint _171 = uint(r1.y);
            _discard(_171 == 0u);
            break;
        }
        case 4: {
            r1.y = (cb1_1_m0[2u].y < asfloat(r0.w)) ? 4294967295u : 0u;
            uint _185 = uint(r1.y);
            _discard(_185 == 0u);
            break;
        }
        case 5: {
            r1.y = asuint(asfloat(r0.w) + (-cb1_1_m0[2u].yyyy).y);
            r1.y = (9.9999997473787516355514526367188e-06f < abs(asfloat(r1.yyyy)).y) ? 4294967295u : 0u;
            uint _207 = uint(r1.y);
            _discard(_207 == 0u);
            break;
        }
        case 6: {
            r1.y = (asfloat(r0.w) >= cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _221 = uint(r1.y);
            _discard(_221 == 0u);
            break;
        }
        case 7: {
            break;
        }
        default: {
            break;
        }
        }
    }
    r1.y = (asint(cb1_1_m0[1u].x) != 0) ? 4294967295u : 0u;
    r1.x = r1.y & r1.x;
    r1.y = asuint(v1.z * cb1_1_m0[0u].w);
    r1.z = asuint(mad((-v1.zzzz).z, cb1_1_m0[0u].w, 1.0f));
    float3 _264 = asfloat(r1.yyy) * cb1_1_m0[0u].xyz;
    r2 = uint4(asuint(_264.x), asuint(_264.y), asuint(_264.z), r2.w);
    float3 _273 = mad(asfloat(r1.zzz), asfloat(r0.xyz), asfloat(r2.xyz));
    r1 = uint4(r1.x, asuint(_273.x), asuint(_273.y), asuint(_273.z));
    uint3 _279 = r1.xxx;
    bool3 _286 = bool3(_279.x != uint3(0u, 0u, 0u).x, _279.y != uint3(0u, 0u, 0u).y, _279.z != uint3(0u, 0u, 0u).z);
    float3 _287 =
        float3(_286.x ? asfloat(r1.yzw).x : asfloat(r0.xyz).x, _286.y ? asfloat(r1.yzw).y : asfloat(r0.xyz).y, _286.z ? asfloat(r1.yzw).z : asfloat(r0.xyz).z);
    o0 = float4(_287.x, _287.y, _287.z, o0.w);
    o0.w = asfloat(r0.w);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
