#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb5_struct : register(b1) {
    float4 cb1_1_m0[5] : packoffset(c0);
};

SamplerState s0 : register(s0);
Texture2D<float4> t0 : register(t0);
RWTexture2DArray<uint4> u2 : register(u2);
RWTexture2DArray<float4> u3 : register(u3);
RWTexture2D<uint> u4 : register(u4);

struct PixelInput {
    float4 inputColor0 : COLOR0;
    float2 inputDepth0 : DEPTH0;
    float inputFog0 : FOG0;
    float2 inputTexcoord0 : TEXCOORD0;
    float4 fragmentPosition : SV_Position;
    bool frontFacing : SV_IsFrontFace;
};

void _discard(bool _463) {
    if (_463) {
        discard;
    }
}

[earlydepthstencil] void main(PixelInput input) {
    float4 fragmentPosition = 0;
    bool frontFacing = 0;
    float2 inputTexcoord0 = 0;
    float inputFog0 = 0;
    float4 inputColor0 = 0;
    float2 inputDepth0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    uint4 r2 = 0;
    float4 v0 = 0;
    float4 v4 = 0;
    float3 v1 = 0;
    float4 v2 = 0;
    float2 v3 = 0;
    float3 _470 = 0;
    float4 _479 = 0;
    float2 _491 = 0;

    fragmentPosition = input.fragmentPosition;
    fragmentPosition.w = 1.0 / fragmentPosition.w;
    frontFacing = input.frontFacing;
    inputTexcoord0 = input.inputTexcoord0;
    inputFog0 = input.inputFog0;
    inputColor0 = input.inputColor0;
    inputDepth0 = input.inputDepth0;
    float3 _473 = _470;
    _473.x = inputTexcoord0.x;
    float3 _475 = _473;
    _475.y = inputTexcoord0.y;
    float3 _477 = _475;
    _477.z = inputFog0;
    v1 = _477;
    float4 _482 = _479;
    _482.x = inputColor0.x;
    float4 _484 = _482;
    _484.y = inputColor0.y;
    float4 _486 = _484;
    _486.z = inputColor0.z;
    float4 _488 = _486;
    _488.w = inputColor0.w;
    v2 = _488;
    float2 _494 = _491;
    _494.x = inputDepth0.x;
    float2 _496 = _494;
    _496.y = inputDepth0.y;
    v3 = _496;
    float4 _40 = fragmentPosition;
    _40.w = 1.0f / fragmentPosition.w;
    v0 = _40;
    v4.x = asfloat(frontFacing ? 4294967295u : 0u);
    r0 = asuint(t0.Sample(s0, v1.xyxx.xy));
    int2 _73 = asint(cb1_1_m0[4u].xx);
    bool2 _78 = bool2(_73.x == int2(0, 3).x, _73.y == int2(0, 3).y);
    uint2 _83 = uint2(_78.x ? uint2(4294967295u, 4294967295u).x : uint2(0u, 0u).x, _78.y ? uint2(4294967295u, 4294967295u).y : uint2(0u, 0u).y);
    r1 = uint4(_83.x, _83.y, r1.z, r1.w);
    r1.x = r1.y | r1.x;
    r0 = asuint(asfloat(r0) * (v2));
    r1.y = asuint(v1.z * cb1_1_m0[0u].w);
    r1.z = asuint(mad((-v1.zzzz).z, cb1_1_m0[0u].w, 1.0f));
    float3 _125 = asfloat(r1.yyy) * cb1_1_m0[0u].xyz;
    r2 = uint4(asuint(_125.x), asuint(_125.y), asuint(_125.z), r2.w);
    float3 _134 = mad(asfloat(r1.zzz), asfloat(r0.xyz), asfloat(r2.xyz));
    r1 = uint4(r1.x, asuint(_134.x), asuint(_134.y), asuint(_134.z));
    uint3 _141 = asuint(cb1_1_m0[1u].xxx);
    bool3 _148 = bool3(_141.x != uint3(0u, 0u, 0u).x, _141.y != uint3(0u, 0u, 0u).y, _141.z != uint3(0u, 0u, 0u).z);
    float3 _149 =
        float3(_148.x ? asfloat(r1.yzw).x : asfloat(r0.xyz).x, _148.y ? asfloat(r1.yzw).y : asfloat(r0.xyz).y, _148.z ? asfloat(r1.yzw).z : asfloat(r0.xyz).z);
    r0 = uint4(asuint(_149.x), asuint(_149.y), asuint(_149.z), r0.w);
    uint4 _155 = r1.xxxx;
    bool4 _160 =
        bool4(_155.x != uint4(0u, 0u, 0u, 0u).x, _155.y != uint4(0u, 0u, 0u, 0u).y, _155.z != uint4(0u, 0u, 0u, 0u).z, _155.w != uint4(0u, 0u, 0u, 0u).w);
    r0 = uint4(asuint(_160.x ? asfloat(r0.x) : v2.x), asuint(_160.y ? asfloat(r0.y) : v2.y), asuint(_160.z ? asfloat(r0.z) : v2.z),
        asuint(_160.w ? asfloat(r0.w) : v2.w));
    r0.w = asuint((asuint(cb1_1_m0[4u].z) != 0u) ? 1.0f : asfloat(r0.w));
    if (asuint(cb1_1_m0[2u].x) != 0u) {
        switch (asint(cb1_1_m0[2u].z)) {
        case 0: {
            _discard(4294967295u != 0u);
            break;
        }
        case 1: {
            r1.x = (asfloat(r0.w) < cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _205 = uint(r1.x);
            _discard(_205 == 0u);
            break;
        }
        case 2: {
            r1.x = asuint(asfloat(r0.w) + (-cb1_1_m0[2u].yyyy).x);
            r1.x = (abs(asfloat(r1.xxxx)).x < 9.9999997473787516355514526367188e-06f) ? 4294967295u : 0u;
            uint _228 = uint(r1.x);
            _discard(_228 == 0u);
            break;
        }
        case 3: {
            r1.x = (cb1_1_m0[2u].y >= asfloat(r0.w)) ? 4294967295u : 0u;
            uint _242 = uint(r1.x);
            _discard(_242 == 0u);
            break;
        }
        case 4: {
            r1.x = (cb1_1_m0[2u].y < asfloat(r0.w)) ? 4294967295u : 0u;
            uint _256 = uint(r1.x);
            _discard(_256 == 0u);
            break;
        }
        case 5: {
            r1.x = asuint(asfloat(r0.w) + (-cb1_1_m0[2u].yyyy).x);
            r1.x = (9.9999997473787516355514526367188e-06f < abs(asfloat(r1.xxxx)).x) ? 4294967295u : 0u;
            uint _278 = uint(r1.x);
            _discard(_278 == 0u);
            break;
        }
        case 6: {
            r1.x = (asfloat(r0.w) >= cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _292 = uint(r1.x);
            _discard(_292 == 0u);
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
    r1.x = (0.0f < asfloat(r0.w)) ? 4294967295u : 0u;
    if (r1.x != 0u) {
        float2 _315 = max(v0.xy, (-2147483648.0f).xx);
        bool2 _318 = bool2(_315.x >= 2147483648.0f.xx.x, _315.y >= 2147483648.0f.xx.y);
        int2 _319 = int2(_315);
        int2 _320 = int2(_318.x ? int2(2147483647, 2147483647).x : _319.x, _318.y ? int2(2147483647, 2147483647).y : _319.y);
        uint2 _498 = asuint(v0.xy) & uint2(2147483647u, 2147483647u);
        bool2 _322 = bool2(_498.x > uint2(2139095040u, 2139095040u).x, _498.y > uint2(2139095040u, 2139095040u).y);
        uint2 _324 = uint2(int2(_322.x ? int2(0, 0).x : _320.x, _322.y ? int2(0, 0).y : _320.y));
        r1 = uint4(_324.x, _324.y, r1.z, r1.w);
        uint _332;
        InterlockedAdd(u4[asint(r1.xy)], 1u, _332);
        r1.z = _332;
        r1.w = (asint(r1.z) < 16) ? 4294967295u : 0u;
        if (r1.w != 0u) {
            bool4 _505 = isnan(asfloat(r0));
            bool4 _506 = isnan(0.0f.xxxx);
            float4 _507 = max(asfloat(r0), 0.0f.xxxx);
            float4 _508 = float4(_505.x ? 0.0f.xxxx.x : _507.x, _505.y ? 0.0f.xxxx.y : _507.y, _505.z ? 0.0f.xxxx.z : _507.z, _505.w ? 0.0f.xxxx.w : _507.w);
            float4 _503 =
                float4(_506.x ? asfloat(r0.x) : _508.x, _506.y ? asfloat(r0.y) : _508.y, _506.z ? asfloat(r0.z) : _508.z, _506.w ? asfloat(r0.w) : _508.w);
            bool4 _510 = isnan(_503);
            bool4 _511 = isnan(1.0f.xxxx);
            float4 _512 = min(_503, 1.0f.xxxx);
            float4 _513 = float4(_510.x ? 1.0f.xxxx.x : _512.x, _510.y ? 1.0f.xxxx.y : _512.y, _510.z ? 1.0f.xxxx.z : _512.z, _510.w ? 1.0f.xxxx.w : _512.w);
            r0 = uint4(asuint(_511.x ? _503.x : _513.x), asuint(_511.y ? _503.y : _513.y), asuint(_511.z ? _503.z : _513.z), asuint(_511.w ? _503.w : _513.w));
            float3 _359 = mad(asfloat(r0.xyz), 255.0f.xxx, 0.5f.xxx);
            r0 = uint4(asuint(_359.x), asuint(_359.y), asuint(_359.z), r0.w);
            float3 _364 = floor(asfloat(r0.xyz));
            r0 = uint4(asuint(_364.x), asuint(_364.y), asuint(_364.z), r0.w);
            float3 _372 = max(asfloat(r0.xyz), 0.0f.xxx);
            bool3 _374 = bool3(_372.x >= 4294967296.0f.xxx.x, _372.y >= 4294967296.0f.xxx.y, _372.z >= 4294967296.0f.xxx.z);
            uint3 _375 = uint3(_372);
            uint3 _377 = uint3(_374.x ? uint3(4294967295u, 4294967295u, 4294967295u).x : _375.x,
                _374.y ? uint3(4294967295u, 4294967295u, 4294967295u).y : _375.y, _374.z ? uint3(4294967295u, 4294967295u, 4294967295u).z : _375.z);
            r0 = uint4(_377.x, _377.y, _377.z, r0.w);
            r0.x = (asint(r0.y) * 256) + asint(r0.x);
            r0.x = (asint(r0.z) * 65536) + asint(r0.x);
            r0.y = asuint(mad(asfloat(r0.w), 255.0f, 0.5f));
            r0.y = asuint(floor(asfloat(r0.y)));
            float _412 = max(asfloat(r0.y), 0.0f);
            r0.y = (_412 >= 4294967296.0f) ? 4294967295u : uint(_412);
            r0.x = (asint(r0.y) * 16777216) + asint(r0.x);
            u2[asint(r1.xyz)] = r0.xxxx;
            r0.x = asuint(v3.x / v3.y);
            r0.y = asuint(v4.x) & 897988541u;
            r0.x = asuint(asfloat(r0.y) + asfloat(r0.x));
            u3[asint(r1.xyz)] = asfloat(r0.xxxx);
        }
    }
}
#endif
