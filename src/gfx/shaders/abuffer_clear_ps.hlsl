#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

RWTexture2D<uint4> u4 : register(u4);

struct PixelInput {
    float4 fragmentPosition : SV_Position;
};

struct PixelOutput {
    float4 o0 : SV_Target0;
};

void _discard(bool _60) {
    if (_60) {
        discard;
    }
}

PixelOutput main(PixelInput input) {
    float4 fragmentPosition = 0;
    float4 o0 = 0;
    uint4 r0 = 0;
    float4 v0 = 0;

    fragmentPosition = input.fragmentPosition;
    fragmentPosition.w = 1.0 / fragmentPosition.w;
    float4 _21 = fragmentPosition;
    _21.w = 1.0f / fragmentPosition.w;
    v0 = _21;
    float4 _33 = max(v0.xyyy, (-2147483648.0f).xxxx);
    bool4 _38 = bool4(_33.x >= 2147483648.0f.xxxx.x, _33.y >= 2147483648.0f.xxxx.y, _33.z >= 2147483648.0f.xxxx.z, _33.w >= 2147483648.0f.xxxx.w);
    int4 _39 = int4(_33);
    int4 _40 =
        int4(_38.x ? int4(2147483647, 2147483647, 2147483647, 2147483647).x : _39.x, _38.y ? int4(2147483647, 2147483647, 2147483647, 2147483647).y : _39.y,
            _38.z ? int4(2147483647, 2147483647, 2147483647, 2147483647).z : _39.z, _38.w ? int4(2147483647, 2147483647, 2147483647, 2147483647).w : _39.w);
    uint4 _65 = asuint(v0.xyyy) & uint4(2147483647u, 2147483647u, 2147483647u, 2147483647u);
    bool4 _43 = bool4(_65.x > uint4(2139095040u, 2139095040u, 2139095040u, 2139095040u).x, _65.y > uint4(2139095040u, 2139095040u, 2139095040u, 2139095040u).y,
        _65.z > uint4(2139095040u, 2139095040u, 2139095040u, 2139095040u).z, _65.w > uint4(2139095040u, 2139095040u, 2139095040u, 2139095040u).w);
    r0 = int4(_43.x ? int4(0, 0, 0, 0).x : _40.x, _43.y ? int4(0, 0, 0, 0).y : _40.y, _43.z ? int4(0, 0, 0, 0).z : _40.z, _43.w ? int4(0, 0, 0, 0).w : _40.w);
    u4[asint(r0.xy)] = uint4(0u, 0u, 0u, 0u);
    _discard(4294967295u != 0u);
    PixelOutput output;
    output.o0 = o0;
    return output;
}
#endif
