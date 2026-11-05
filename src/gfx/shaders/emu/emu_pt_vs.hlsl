#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float2 inputTexcoord0 = 0;
    float2 outputTexcoord0 = 0;
    float3 v0 = 0;
    float2 v1 = 0;
    float2 o1 = 0;
    float3 _40 = 0;
    float2 _50 = 0;

    inputPosition0 = input.inputPosition0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _43 = _40;
    _43.x = inputPosition0.x;
    float3 _45 = _43;
    _45.y = inputPosition0.y;
    float3 _47 = _45;
    _47.z = inputPosition0.z;
    v0 = _47;
    float2 _53 = _50;
    _53.x = inputTexcoord0.x;
    float2 _55 = _53;
    _55.y = inputTexcoord0.y;
    v1 = _55;
    float3 _29 = mad(v0, float3(0.00312500004656612873077392578125f, -0.0041666668839752674102783203125f, 1.0f), float3(-1.0f, 1.0f, 0.0f));
    position = float4(_29.x, _29.y, _29.z, position.w);
    position.w = 1.0f;
    o1 = v1 * float2(1.0f, -1.0f);
    outputTexcoord0 = float2(o1);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
