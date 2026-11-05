#if defined(SHENMUE_GFX_STANDALONE)
#include "bits.hlsli"

struct VertexInput {
    float4 inputPosition0 : POSITION0;
    float4 inputDiffuse0 : DIFFUSE0;
};

struct VertexOutput {
    float4 outputColor0 : COLOR0;
    float4 outputProjscreenpos0 : PROJSCREENPOS0;
    float4 position : SV_Position;
};

VertexOutput main(VertexInput input) {
    float4 position = 0;
    float4 inputPosition0 = 0;
    float4 inputDiffuse0 = 0;
    float4 outputColor0 = 0;
    float4 outputProjscreenpos0 = 0;
    uint4 r0 = 0;
    float4 v0 = 0;
    float4 v1 = 0;
    float4 o1 = 0;
    float4 o2 = 0;
    float4 _28 = 0;
    float4 _39 = 0;

    inputPosition0 = input.inputPosition0;
    inputDiffuse0 = input.inputDiffuse0;
    float4 _31 = _28;
    _31.x = inputPosition0.x;
    float4 _33 = _31;
    _33.y = inputPosition0.y;
    float4 _35 = _33;
    _35.z = inputPosition0.z;
    float4 _37 = _35;
    _37.w = inputPosition0.w;
    v0 = _37;
    float4 _42 = _39;
    _42.x = inputDiffuse0.x;
    float4 _44 = _42;
    _44.y = inputDiffuse0.y;
    float4 _46 = _44;
    _46.z = inputDiffuse0.z;
    float4 _48 = _46;
    _48.w = inputDiffuse0.w;
    v1 = _48;
    r0 = uint4(asuint(v0.x), asuint(v0.y), r0.z, asuint(v0.w));
    r0.z = asuint(1.0f);
    position = asfloat(r0);
    o2 = asfloat(r0);
    o1 = v1;
    outputColor0 = float4(o1);
    outputProjscreenpos0 = float4(o2);
    VertexOutput output;
    output.position = position;
    output.outputColor0 = outputColor0;
    output.outputProjscreenpos0 = outputProjscreenpos0;
    return output;
}
#endif
