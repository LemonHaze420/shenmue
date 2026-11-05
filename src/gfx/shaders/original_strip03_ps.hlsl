#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb4_struct : register(b1) {
    float4 cb1_1_m0[4] : packoffset(c0);
};

SamplerState s0 : register(s0);
Texture2D<float4> t0 : register(t0);

struct PixelInput {
    float4 inputDiffuse0 : DIFFUSE0;
    float4 inputSpecular0 : SPECULAR0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

void _discard(bool _256) {
    if (_256) {
        discard;
    }
}

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float4 inputDiffuse0 = 0;
    float4 inputSpecular0 = 0;
    float2 inputTexcoord0 = 0;
    uint4 r0 = 0;
    uint4 r1 = 0;
    float4 v1 = 0;
    float4 v2 = 0;
    float2 v3 = 0;
    float4 _261 = 0;
    float4 _272 = 0;
    float2 _284 = 0;

    inputDiffuse0 = input.inputDiffuse0;
    inputSpecular0 = input.inputSpecular0;
    inputTexcoord0 = input.inputTexcoord0;
    float4 _264 = _261;
    _264.x = inputDiffuse0.x;
    float4 _266 = _264;
    _266.y = inputDiffuse0.y;
    float4 _268 = _266;
    _268.z = inputDiffuse0.z;
    float4 _270 = _268;
    _270.w = inputDiffuse0.w;
    v1 = _270;
    float4 _275 = _272;
    _275.x = inputSpecular0.x;
    float4 _277 = _275;
    _277.y = inputSpecular0.y;
    float4 _279 = _277;
    _279.z = inputSpecular0.z;
    float4 _281 = _279;
    _281.w = inputSpecular0.w;
    v2 = _281;
    float2 _287 = _284;
    _287.x = inputTexcoord0.x;
    float2 _289 = _287;
    _289.y = inputTexcoord0.y;
    v3 = _289;
    r0 = asuint(t0.Sample(s0, v3.xyxx.xy));
    r1.x = asuint(float(asuint(cb1_1_m0[3u].w)));
    float3 _54 = asfloat(r1.xxx) * v2.zyx;
    r1 = uint4(asuint(_54.x), asuint(_54.y), asuint(_54.z), r1.w);
    float3 _64 = mad(v1.zyx, asfloat(r0.xyz), asfloat(r1.xyz));
    o0 = float4(_64.x, _64.y, _64.z, o0.w);
    switch (asint(cb1_1_m0[3u].y)) {
    case 0: {
        r0.x = asuint(v1.w);
        break;
    }
    case 1: {
        r0.x = asuint(v1.w);
        break;
    }
    case 2: {
        r0.x = r0.w;
        break;
    }
    default: {
        r0.x = asuint(0.0f);
        break;
    }
    }
    switch (asint(cb1_1_m0[3u].z)) {
    case 0: {
        r0.w = asuint(v1.w);
        break;
    }
    case 1: {
        r0.w = asuint(v1.w);
        break;
    }
    case 2: {
        break;
    }
    default: {
        r0.w = asuint(0.0f);
        break;
    }
    }
    switch (asint(cb1_1_m0[2u].w)) {
    case 2: {
        r0.w = r0.x;
        break;
    }
    case 3: {
        break;
    }
    default: {
        r0.w = asuint(asfloat(r0.w) * (asfloat(r0.x)));
        break;
    }
    }
    if (asuint(cb1_1_m0[2u].x) != 0u) {
        switch (asint(cb1_1_m0[2u].z)) {
        case 0: {
            _discard(4294967295u != 0u);
            break;
        }
        case 1: {
            r0.x = (asfloat(r0.w) < cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _160 = uint(r0.x);
            _discard(_160 == 0u);
            break;
        }
        case 2: {
            r0.x = asuint(asfloat(r0.w) + (-cb1_1_m0[2u].yyyy).x);
            r0.x = (abs(asfloat(r0.xxxx)).x < 9.9999997473787516355514526367188e-06f) ? 4294967295u : 0u;
            uint _183 = uint(r0.x);
            _discard(_183 == 0u);
            break;
        }
        case 3: {
            r0.x = (cb1_1_m0[2u].y >= asfloat(r0.w)) ? 4294967295u : 0u;
            uint _197 = uint(r0.x);
            _discard(_197 == 0u);
            break;
        }
        case 4: {
            r0.x = (cb1_1_m0[2u].y < asfloat(r0.w)) ? 4294967295u : 0u;
            uint _211 = uint(r0.x);
            _discard(_211 == 0u);
            break;
        }
        case 5: {
            r0.x = asuint(asfloat(r0.w) + (-cb1_1_m0[2u].yyyy).x);
            r0.x = (9.9999997473787516355514526367188e-06f < abs(asfloat(r0.xxxx)).x) ? 4294967295u : 0u;
            uint _233 = uint(r0.x);
            _discard(_233 == 0u);
            break;
        }
        case 6: {
            r0.x = (asfloat(r0.w) >= cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _247 = uint(r0.x);
            _discard(_247 == 0u);
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
    o0.w = asfloat(r0.w);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
