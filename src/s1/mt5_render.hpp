#pragma once

#include "s1/d3t/namespace.hpp"
#include "d3t/buffer.hpp"
#include "common/math.hpp"

#include <cstdint>

namespace shenmue::s1 {

struct PackModel;
struct Character;
struct Mt5RuntimeNode;
struct EffectLightRenderSlot;
using D3TDeviceContext = d3t::D3TDeviceContext;

struct Mt5DrawDiagnostics {
    std::uint32_t ordinaryMaterials;
    std::uint32_t effectMaterials;
    std::uint32_t ordinaryDraws;
    std::uint32_t effectDraws;
    std::uint32_t truncatedStreams;
};

struct CharacterDrawDiagnostics {
    std::uint32_t submittedCharacters;
    std::uint32_t ryoVisitedNodes;
    std::uint32_t ryoBindings;
    std::uint32_t ryoDraws;
    std::uint32_t ryoMissingSceneEntries;
    std::uint32_t ryoPreparedNodes;
    std::uint32_t ryoOsageSlots;
    std::uint32_t ryoOsageWrites;
    float ryoFirstViewX;
    float ryoFirstViewY;
    float ryoFirstViewZ;
};


extern const std::int32_t *g_effectBufferHandleOverride;
void SubmitSceneObjectForDrawing(void *commandStreamOwner, void *modelBase);

void TransformPrelightLights(EffectLightRenderSlot *lights, std::int32_t count);
std::int32_t ModelCopyToGPU(void *binding, std::int32_t *attachment, EffectLightRenderSlot *lights, std::int32_t count);
void HMDL_PrelightNodeRecursive(Mt5RuntimeNode *node, EffectLightRenderSlot *lights, std::int32_t count);
void HMDL_PrelightSpotLights(void *model, EffectLightRenderSlot *lights, std::int32_t count);
void DrawSceneObjectFromCommandStream(void *commandStreamOwner, float alphaScale, std::uint8_t shaderMode);
void DrawSceneObjectFromEffectBuffer(void *commandStreamOwner, float alphaScale, std::uint8_t shaderMode, void *modelBase);
namespace d3t {
void MT5_TransformAndSubmitNodeTree(void *node, float alpha, std::uint32_t mode);
void MT5_PushAndReadNodeTree(void *node, const std::int32_t *transform, float alpha);
void MT5_PushAndSubmitNodeTree(void *node, float alpha);
void MT5_SubmitObjectNode(void *node, float alpha);
} // namespace d3t
void Recursive_SubmitRiggedNodes(void *node, float alpha, const Matrix4x4 &absoluteTransformRoot, Character *character);

std::uint32_t SubmitRiggedSceneObjectForDrawing(void *model, Character *character, std::int32_t mode, std::uint32_t flags);
void PrepareRiggedSceneObjectForDrawing(void *model, const Matrix4x4 &absoluteTransformRoot, Character *character);
void SubmitRiggedCharacterModel(void *model, Character *character, float alpha);
void SubmitBoneEffectCharacterModel(void *model, Character *character);
void SubmitRiggedSceneAttachment(
    void *model, float alpha, const Vector3 &rootPosition, const Vector3i &rootRotation, const Vector3 &localPosition, const Vector3i &localRotation);
void HMDL_ClearNodeTransformLinks(Mt5RuntimeNode *node);
void HMDL_LinkNodeTransformsByKind(Mt5RuntimeNode *source, Mt5RuntimeNode *target);


void SubmitCharacterShadow(Character *character, void *posedModel, void *model, const Vector3 &position, const Vector3i &rotation, const Vector3 &scale,
    float groundHeight, float direction);
void SubmitPosedCharacterShadow(Character *character, void *model, const Vector3 &position, float groundHeight, float direction);
void DrawCharacterObjects();
void DrawCharacterShadows();
void BindLegacyPixelTexture(D3TDeviceContext *context, std::uint32_t texture);
void CommitLegacyScreenRenderState(D3TDeviceContext *context, bool transparent);
void SetMt5RenderShaderSelection(std::int32_t inputLayoutIndex, std::int32_t vertexShaderIndex, std::int32_t pixelShaderIndex);
bool SetRenderPass(std::int32_t renderPass, std::int32_t pipelinePreset);
void ConvertAndPushCurrentMatrixToShaderBlock();
extern std::uint8_t g_forcePrimaryCachedModel;

bool CachedAttachmentOwnsSubmission(void *model, float alpha, const Matrix4x4 &absoluteTransformRoot);
void SetMt5AlphaMaskedDepthPass(bool enabled);
Mt5DrawDiagnostics GetMt5DrawDiagnostics();
CharacterDrawDiagnostics GetCharacterDrawDiagnostics();
} // namespace shenmue::s1
