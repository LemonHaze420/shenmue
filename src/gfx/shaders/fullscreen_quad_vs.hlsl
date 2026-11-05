#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float4 inputPosition0 : POSITION0;
    float2 inputTexcoord0 : TEXCOORD0;
    float2 inputTexcoord1 : TEXCOORD1;
    float2 inputTexcoord2 : TEXCOORD2;
    float2 inputTexcoord3 : TEXCOORD3;
};

struct VertexOutput {
    float2 outputTexcoord0 : TEXCOORD0;
    float2 outputTexcoord1 : TEXCOORD1;
    float2 outputTexcoord2 : TEXCOORD2;
    float2 outputTexcoord3 : TEXCOORD3;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float4 inputPosition0 = 0;
    float2 inputTexcoord0 = 0;
    float2 inputTexcoord1 = 0;
    float2 inputTexcoord2 = 0;
    float2 inputTexcoord3 = 0;
    float2 outputTexcoord0 = 0;
    float2 outputTexcoord1 = 0;
    float2 outputTexcoord2 = 0;
    float2 outputTexcoord3 = 0;
    float4 v0 = 0;
    float2 v1 = 0;
    float2 v2 = 0;
    float2 v3 = 0;
    float2 v4 = 0;
    float4 o1 = 0;
    float4 o2 = 0;
    float4 _43 = 0;
    float2 _55 = 0;
    float2 _62 = 0;
    float2 _69 = 0;
    float2 _76 = 0;

    inputPosition0 = input.inputPosition0;
    inputTexcoord0 = input.inputTexcoord0;
    inputTexcoord1 = input.inputTexcoord1;
    inputTexcoord2 = input.inputTexcoord2;
    inputTexcoord3 = input.inputTexcoord3;
    float4 _46 = _43;
    _46.x = inputPosition0.x;
    float4 _48 = _46;
    _48.y = inputPosition0.y;
    float4 _50 = _48;
    _50.z = inputPosition0.z;
    float4 _52 = _50;
    _52.w = inputPosition0.w;
    v0 = _52;
    float2 _58 = _55;
    _58.x = inputTexcoord0.x;
    float2 _60 = _58;
    _60.y = inputTexcoord0.y;
    v1 = _60;
    float2 _65 = _62;
    _65.x = inputTexcoord1.x;
    float2 _67 = _65;
    _67.y = inputTexcoord1.y;
    v2 = _67;
    float2 _72 = _69;
    _72.x = inputTexcoord2.x;
    float2 _74 = _72;
    _74.y = inputTexcoord2.y;
    v3 = _74;
    float2 _79 = _76;
    _79.x = inputTexcoord3.x;
    float2 _81 = _79;
    _81.y = inputTexcoord3.y;
    v4 = _81;
    position = float4(v0.x, v0.y, v0.z, position.w);
    position.w = 1.0f;
    o1 = float4(v1.x, v1.y, o1.z, o1.w);
    o1 = float4(o1.x, o1.y, v2.x, v2.y);
    o2 = float4(v3.x, v3.y, o2.z, o2.w);
    o2 = float4(o2.x, o2.y, v4.x, v4.y);
    outputTexcoord0 = float2(o1.xy);
    outputTexcoord1 = float2(o1.zw);
    outputTexcoord2 = float2(o2.xy);
    outputTexcoord3 = float2(o2.zw);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    output.outputTexcoord1 = outputTexcoord1;
    output.outputTexcoord2 = outputTexcoord2;
    output.outputTexcoord3 = outputTexcoord3;
    return output;
}
#endif
