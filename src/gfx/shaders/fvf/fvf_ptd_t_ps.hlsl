#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb3_struct : register(b1) {
    float4 cb1_1_m0[3] : packoffset(c0);
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

void _discard(bool _181) {
    if (_181) {
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
    float3 v1 = 0;
    float4 v2 = 0;
    float3 _190 = 0;
    float4 _199 = 0;

    inputTexcoord0 = input.inputTexcoord0;
    inputFog0 = input.inputFog0;
    inputColor0 = input.inputColor0;
    float3 _193 = _190;
    _193.x = inputTexcoord0.x;
    float3 _195 = _193;
    _195.y = inputTexcoord0.y;
    float3 _197 = _195;
    _197.z = inputFog0;
    v1 = _197;
    float4 _202 = _199;
    _202.x = inputColor0.x;
    float4 _204 = _202;
    _204.y = inputColor0.y;
    float4 _206 = _204;
    _206.z = inputColor0.z;
    float4 _208 = _206;
    _208.w = inputColor0.w;
    v2 = _208;
    r0 = asuint(t0.Sample(s0, v1.xyxx.xy));
    r1 = asuint(asfloat(r0) * v2);
    if (asuint(cb1_1_m0[2u].x) != 0u) {
        switch (asint(cb1_1_m0[2u].z)) {
        case 0: {
            _discard(4294967295u != 0u);
            break;
        }
        case 1: {
            r0.x = (asfloat(r1.w) < cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _83 = uint(r0.x);
            _discard(_83 == 0u);
            break;
        }
        case 2: {
            r0.x = asuint(mad(v2.w, asfloat(r0.w), (-cb1_1_m0[2u].yyyy).x));
            r0.x = (abs(asfloat(r0.xxxx)).x < 9.9999997473787516355514526367188e-06f) ? 4294967295u : 0u;
            uint _110 = uint(r0.x);
            _discard(_110 == 0u);
            break;
        }
        case 3: {
            r0.x = (cb1_1_m0[2u].y >= asfloat(r1.w)) ? 4294967295u : 0u;
            uint _124 = uint(r0.x);
            _discard(_124 == 0u);
            break;
        }
        case 4: {
            r0.x = (cb1_1_m0[2u].y < asfloat(r1.w)) ? 4294967295u : 0u;
            uint _138 = uint(r0.x);
            _discard(_138 == 0u);
            break;
        }
        case 5: {
            r0.x = asuint(mad(v2.w, asfloat(r0.w), (-cb1_1_m0[2u].yyyy).x));
            r0.x = (9.9999997473787516355514526367188e-06f < abs(asfloat(r0.xxxx)).x) ? 4294967295u : 0u;
            uint _162 = uint(r0.x);
            _discard(_162 == 0u);
            break;
        }
        case 6: {
            r0.x = (asfloat(r1.w) >= cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _176 = uint(r0.x);
            _discard(_176 == 0u);
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
    o0 = asfloat(r1);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
