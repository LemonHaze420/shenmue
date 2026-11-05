#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float3 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
    float4 inputSpecular0 : SPECULAR0;
    float2 inputTexcoord0 : TEXCOORD0;
};

struct VertexOutput {
    float4 outputDiffuse0 : DIFFUSE0;
    float4 outputSpecular0 : SPECULAR0;
    float2 outputTexcoord0 : TEXCOORD0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float3 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float4 inputSpecular0 = 0;
    float2 inputTexcoord0 = 0;
    float4 outputDiffuse0 = 0;
    float4 outputSpecular0 = 0;
    float2 outputTexcoord0 = 0;
    float3 v0 = 0;
    float4 v1 = 0;
    float4 v2 = 0;
    float2 v3 = 0;
    float4 o1 = 0;
    float4 o2 = 0;
    float2 o3 = 0;
    float3 _42 = 0;
    float4 _52 = 0;
    float4 _63 = 0;
    float2 _75 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    inputSpecular0 = input.inputSpecular0;
    inputTexcoord0 = input.inputTexcoord0;
    float3 _45 = _42;
    _45.x = inputPosition0.x;
    float3 _47 = _45;
    _47.y = inputPosition0.y;
    float3 _49 = _47;
    _49.z = inputPosition0.z;
    v0 = _49;
    float4 _55 = _52;
    _55.x = inputDiffuse0.x;
    float4 _57 = _55;
    _57.y = inputDiffuse0.y;
    float4 _59 = _57;
    _59.z = inputDiffuse0.z;
    float4 _61 = _59;
    _61.w = inputDiffuse0.w;
    v1 = _61;
    float4 _66 = _63;
    _66.x = inputSpecular0.x;
    float4 _68 = _66;
    _68.y = inputSpecular0.y;
    float4 _70 = _68;
    _70.z = inputSpecular0.z;
    float4 _72 = _70;
    _72.w = inputSpecular0.w;
    v2 = _72;
    float2 _78 = _75;
    _78.x = inputTexcoord0.x;
    float2 _80 = _78;
    _80.y = inputTexcoord0.y;
    v3 = _80;
    float3 _34 = mad(v0, float3(0.00312500004656612873077392578125f, -0.0041666668839752674102783203125f, 1.0f), float3(-1.0f, 1.0f, 0.0f));
    position = float4(_34.x, _34.y, _34.z, position.w);
    position.w = 1.0f;
    o1 = v1;
    o2 = v2;
    o3 = v3;
    outputDiffuse0 = float4(o1);
    outputSpecular0 = float4(o2);
    outputTexcoord0 = float2(o3);
    VertexOutput output;
    output.position = position;
    output.outputDiffuse0 = outputDiffuse0;
    output.outputSpecular0 = outputSpecular0;
    output.outputTexcoord0 = outputTexcoord0;
    return output;
}
#endif
