#include "s2/areas/area_maki.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_characters.hpp"
#include "s2/lighting.hpp"
#include "s2/math.hpp"
#include "s2/npc.hpp"

namespace shenmue::s2::areas {
Vector4 makiLighterColour;
Vector3 makiLighterParameters, makiLighterOffset;
std::int32_t makiLighterPhase;

HLib::Task *QueueMakiLighterLight() {
    const Vector3 position{.x = 0.12800001f, .y = 0.046f, .z = -0.024f};
    const Vector3i rotation{.x = 61440, .y = 14927, .z = 57344};
    const Vector4 reflector = MAKI_LIGHT_COLOUR;
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(MakiLighterLightCallback, 4, 0x0b, static_cast<std::int32_t>(MAKI_LIGHT_TASK.name));
    AttachScenarioCharacter(MAKI_LIGHTER, MAKI_RYO, 14, &position, &rotation);
    EV_EnableCharacterDisplay(MAKI_LIGHTER);
    makiLighterColour = MAKI_LIGHT_COLOUR;
    makiLighterParameters.z = 0.0f;
    makiLighterParameters.x = 4.5f;
    makiLighterParameters.y = 0.61000001f;
    makiLighterOffset.z = 0.60000002f;
    makiLighterOffset.y = 0.2f;
    makiLighterOffset.x = 0.0f;
    makiLighterPhase = 0;
    SetScenarioReflectorBoardValues(MAKI_RYO, &reflector);
    EV_SetRefBoardParamsFor(MAKI_RYO, 1);
    return task;
}

void MakiLighterLightCallback(void *) {
    Vector3 lightPosition;
    Vector3i rotation;
    const float intensity = sin_fixed16(static_cast<std::int16_t>(makiLighterPhase)) * 0.05f + 0.92000002f;
    makiLighterPhase = static_cast<std::int32_t>(static_cast<std::uint32_t>(makiLighterPhase) + 0x2000U);
    EV_GetCharaPosition(MAKI_LIGHTER, CHARACTER_TRANSLATION_ROOT, &lightPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(MAKI_RYO, &rotation, EventTransformFlags::World);
    Vector3 offset = makiLighterOffset;
    ApplyScenarioTranslationAndRotation(nullptr, &rotation, &offset);
    lightPosition.x += offset.x;
    lightPosition.y += offset.y;
    lightPosition.z += offset.z;
    SetLightFlagForIndex(14, 3);
    SetLightRuntimeColour(14, makiLighterColour);
    SetLightSceneIntensity(14, intensity);
    SetLightRange(14, makiLighterParameters.x);
    SetLightFalloff(14, &makiLighterParameters.y);
    SetLightRuntimePosition(14, lightPosition);
    SetLightEntryIndex(14, 1);
}

void MakiSetLocalBaseCallback(void *data) {
    const MakiLocalBaseWork &work = *static_cast<MakiLocalBaseWork *>(data);
    AT_SetLocalBaseChara(work.character, work.base);
    if (work.cleanup)
        HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2::areas
