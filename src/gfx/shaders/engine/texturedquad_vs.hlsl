#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float2 inputPosition0 : POSITION0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float2 inputPosition0 = 0;
    float2 inputTexcoord0 = 0;
    float2 outputTexcoord0 = 0;
    float2 v0 = 0;
    float2 v1 = 0;
    float2 o1 = 0;
    float2 _28 = 0;
    float2 _35 = 0;

    inputPosition0 = input.inputPosition0;
    inputTexcoord0 = input.inputTexcoord0;
    float2 _31 = _28;
    _31.x = inputPosition0.x;
    float2 _33 = _31;
    _33.y = inputPosition0.y;
    v0 = _33;
    float2 _38 = _35;
    _38.x = inputTexcoord0.x;
    float2 _40 = _38;
    _40.y = inputTexcoord0.y;
    v1 = _40;
    position = float4(v0.x, v0.y, position.z, position.w);
    position = float4(position.x, position.y, float2(0.0f, 1.0f).x, float2(0.0f, 1.0f).y);
    o1 = v1;
    outputTexcoord0 = float2(o1);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
