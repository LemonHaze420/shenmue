#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float4 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float2 outputDepth0 : DEPTH0;
    float outputFog0 : FOG0;
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float4 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float2 inputTexcoord0 = 0;
    float2 outputTexcoord0 = 0;
    float outputFog0 = 0;
    float4 outputColor0 = 0;
    float2 outputDepth0 = 0;
    uint4 r0 = 0;
    float4 v0 = 0;
    float4 v1 = 0;
    float2 v2 = 0;
    float3 o1 = 0;
    float4 o2 = 0;
    float2 o3 = 0;
    float4 _51 = 0;
    float4 _62 = 0;
    float2 _74 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    inputTexcoord0 = input.inputTexcoord0;
    float4 _54 = _51;
    _54.x = inputPosition0.x;
    float4 _56 = _54;
    _56.y = inputPosition0.y;
    float4 _58 = _56;
    _58.z = inputPosition0.z;
    float4 _60 = _58;
    _60.w = inputPosition0.w;
    v0 = _60;
    float4 _65 = _62;
    _65.x = inputDiffuse0.x;
    float4 _67 = _65;
    _67.y = inputDiffuse0.y;
    float4 _69 = _67;
    _69.z = inputDiffuse0.z;
    float4 _71 = _69;
    _71.w = inputDiffuse0.w;
    v1 = _71;
    float2 _77 = _74;
    _77.x = inputTexcoord0.x;
    float2 _79 = _77;
    _79.y = inputTexcoord0.y;
    v2 = _79;
    float3 _36 = mad(v0.xyz, float3(0.00312500004656612873077392578125f, -0.0041666668839752674102783203125f, 1.0f), float3(-1.0f, 1.0f, 0.0f));
    r0 = uint4(asuint(_36.x), asuint(_36.y), asuint(_36.z), r0.w);
    r0.w = asuint(1.0f);
    position = asfloat(r0);
    o3 = asfloat(r0.zw);
    o1 = float3(v2.x, v2.y, o1.z);
    o1.z = 0.0f;
    o2 = v1;
    outputTexcoord0 = float2(o1.xy);
    outputFog0 = o1.z;
    outputColor0 = float4(o2);
    outputDepth0 = float2(o3);
    VertexOutput output;
    output.position = position;
    output.outputTexcoord0 = outputTexcoord0;
    output.outputFog0 = outputFog0;
    output.outputColor0 = outputColor0;
    output.outputDepth0 = outputDepth0;
    return output;
}
#endif
