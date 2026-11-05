#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
};

struct VertexOutput {
    float4 outputDiffuse0 : DIFFUSE0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float4 outputDiffuse0 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float4 o1 = 0;
    float3 _35 = 0;
    float4 _45 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    float3 _38 = _35;
    _38.x = inputPosition0.x;
    float3 _40 = _38;
    _40.y = inputPosition0.y;
    float3 _42 = _40;
    _42.z = inputPosition0.z;
    v0 = _42;
    float4 _48 = _45;
    _48.x = inputDiffuse0.x;
    float4 _50 = _48;
    _50.y = inputDiffuse0.y;
    float4 _52 = _50;
    _52.z = inputDiffuse0.z;
    float4 _54 = _52;
    _54.w = inputDiffuse0.w;
    v1 = _54;
    float3 _27 = mad(v0, float3(0.00312500004656612873077392578125f, -0.0041666668839752674102783203125f, 1.0f), float3(-1.0f, 1.0f, 0.0f));
    position = float4(_27.x, _27.y, _27.z, position.w);
    position.w = 1.0f;
    o1 = v1;
    outputDiffuse0 = float4(o1);
    VertexOutput output;
    output.position = position;
    output.outputDiffuse0 = outputDiffuse0;
    return output;
}
#endif
