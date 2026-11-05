#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

cbuffer cb3_struct : register(b1) {
    float4 cb1_1_m0[3] : packoffset(c0);
};

struct PixelInput {
    float4 inputDiffuse0 : DIFFUSE0;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

void _discard(bool _159) {
    if (_159) {
        discard;
    }
}

PixelOutput main(PixelInput input) {
    float4 o0 = 0;
    float4 inputDiffuse0 = 0;
    uint4 r0 = 0;
    float4 v1 = 0;
    float4 _164 = 0;

    inputDiffuse0 = input.inputDiffuse0;
    float4 _167 = _164;
    _167.x = inputDiffuse0.x;
    float4 _169 = _167;
    _169.y = inputDiffuse0.y;
    float4 _171 = _169;
    _171.z = inputDiffuse0.z;
    float4 _173 = _171;
    _173.w = inputDiffuse0.w;
    v1 = _173;
    if (asuint(cb1_1_m0[2u].x) != 0u) {
        switch (asint(cb1_1_m0[2u].z)) {
        case 0: {
            _discard(4294967295u != 0u);
            break;
        }
        case 1: {
            r0.x = (v1.w < cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _64 = uint(r0.x);
            _discard(_64 == 0u);
            break;
        }
        case 2: {
            r0.x = asuint(v1.w + (-cb1_1_m0[2u].yyyy).x);
            r0.x = (abs(asfloat(r0.xxxx)).x < 9.9999997473787516355514526367188e-06f) ? 4294967295u : 0u;
            uint _88 = uint(r0.x);
            _discard(_88 == 0u);
            break;
        }
        case 3: {
            r0.x = (cb1_1_m0[2u].y >= v1.w) ? 4294967295u : 0u;
            uint _102 = uint(r0.x);
            _discard(_102 == 0u);
            break;
        }
        case 4: {
            r0.x = (cb1_1_m0[2u].y < v1.w) ? 4294967295u : 0u;
            uint _116 = uint(r0.x);
            _discard(_116 == 0u);
            break;
        }
        case 5: {
            r0.x = asuint(v1.w + (-cb1_1_m0[2u].yyyy).x);
            r0.x = (9.9999997473787516355514526367188e-06f < abs(asfloat(r0.xxxx)).x) ? 4294967295u : 0u;
            uint _138 = uint(r0.x);
            _discard(_138 == 0u);
            break;
        }
        case 6: {
            r0.x = (v1.w >= cb1_1_m0[2u].y) ? 4294967295u : 0u;
            uint _152 = uint(r0.x);
            _discard(_152 == 0u);
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
    o0 = v1.zyxw;
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
