#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/lighting_dc.hpp"

namespace shenmue { namespace s2 {

#define CHARACTER_DRAW_TASK SHEN_NAME4('D', 'R', 'W', 'P')
#define HUMAN_DRAW_TASK SHEN_NAME4('D', 'R', 'W', 'H')
#define OBJECT_DRAW_TASK SHEN_NAME4('D', 'R', 'W', 'O')
#define DRAW_AUTHOR_AREA_WTA0 SHEN_NAME4('W', 'T', 'A', '0')
#define DRAW_AUTHOR_AREA_WSY0 SHEN_NAME4('W', 'S', 'Y', '0')
#define DRAW_AUTHOR_AREA_ARK0 SHEN_NAME4('A', 'R', 'K', '0')
static const std::uint32_t CHARACTER_COLLISION_CALLBACK_ENABLED = 0x10;
static const std::int32_t kHumanDrawListCapacity = 50, kObjectDrawListCapacity = 100;
static const float kCharacterDrawUnprojectedDistance = 1000.0f;
static const float kCharacterLightNormalizationThreshold = 0.001f;
static const float kCharacterLightTieMargin = 0.005f, kCharacterLightFadeStep = 0.1f;
static const float kCharacterLightRangeMargin = 1.4f, kCharacterLightDistanceBias = 0.05f;
static const float kCharacterLightFallbackThreshold = 0.01f, kCharacterLightMinimumAmbient = 0.3333f;
struct CharacterDrawRecord {
    float distance;
    Character *character;
};
struct CharacterDrawTaskParameters {
    CharacterDrawRecord humans[kHumanDrawListCapacity + 1];
    std::int32_t humanCount;
    CharacterDrawRecord objects[kObjectDrawListCapacity + 1];
    std::int32_t objectCount;
    Character *player;
    std::int32_t previousActiveLight;
    Vector4 globalLighting, normalizedLighting, minimumGlobalLighting;
    LightRuntimeEntry savedLights[16];
    Vector3 lightRoot;
};
typedef char Layout_character_draw_tasks_dc_0[(sizeof(CharacterDrawRecord) == 8) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_1[(offsetof(CharacterDrawRecord, character) == 4) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_2[(offsetof(CharacterDrawTaskParameters, humanCount) == 0x198) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_3[(offsetof(CharacterDrawTaskParameters, objects) == 0x19c) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_4[(offsetof(CharacterDrawTaskParameters, objectCount) == 0x4c4) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_5[(offsetof(CharacterDrawTaskParameters, player) == 0x4c8) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_6[(offsetof(CharacterDrawTaskParameters, previousActiveLight) == 0x4cc) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_7[(offsetof(CharacterDrawTaskParameters, globalLighting) == 0x4d0) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_8[(offsetof(CharacterDrawTaskParameters, normalizedLighting) == 0x4e0) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_9[(offsetof(CharacterDrawTaskParameters, minimumGlobalLighting) == 0x4f0) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_10[(offsetof(CharacterDrawTaskParameters, savedLights) == 0x500) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_11[(offsetof(CharacterDrawTaskParameters, lightRoot) == 0x880) ? 1 : -1];
typedef char Layout_character_draw_tasks_dc_12[(sizeof(CharacterDrawTaskParameters) == 0x88c) ? 1 : -1];
extern HLib::Task *characterDrawTask;
extern std::int32_t characterHumanDrawLimit, characterObjectDrawLimit, npcPoolSizingModeOrOverride, characterLightAuthorOverride;
extern float characterMinimumRed, characterMinimumGreen, characterMinimumBlue;
extern std::uint32_t characterClipPointCount;
static const float kCharacterMotionClipRadius = 0.4f;
void CharDispCheck_Callback(void *data);
std::int32_t TestCharacterVisibility(Character *character, void *data);
void CharDraw_Callback(void *data);
void CharDrawHuman_Callback(void *data);
void CharDrawObject_Callback(void *data);
HLib::Task *CharDraw_Human_Object_Enqueue(std::uint8_t nextFunctionIndex);
std::int32_t GatherHumanDrawList(Character *character, void *data);
std::int32_t GatherObjectDrawList(Character *character, void *data);
void DrawHumanList(CharacterDrawTaskParameters *data);
void DrawObjectList(CharacterDrawTaskParameters *data);
void ExecuteCharacterCoreDraw(Character *character);
bool IsCharacterCollisionCallbackEnabled(const Character *character);
void ExecuteCharacterCollisionDraw(Character *character);
void charaHandlerDrawHuman(Character *character);
void DrawLights(Character *character, CharacterDrawTaskParameters *data);
void ExchangeLegacyModelColour(const Vector4 *colour, Vector4 *previous);
void SetupCharacterLightFrame(CharacterDrawTaskParameters *data);
void RestoreCharacterLightFrame(CharacterDrawTaskParameters *data);
void RunModelComponentCoreCallbacks(Character *character);

} } // namespace shenmue::s2

#endif
