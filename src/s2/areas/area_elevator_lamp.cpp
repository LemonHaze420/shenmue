#include "s2/events/event_api.h"
#include "area_elevator_lamp.hpp"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/elevator.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
namespace shenmue::s2::areas {
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
int ConfigureAreaElevatorLamp(int enabled) {
    shen_name selected = 0;
    int index;
    ElevatorLampChoice *choice;
    if (enabled) {
        if (g_currentMap == AREA_QF39) {
#else
std::int64_t ConfigureAreaElevatorLamp(std::int32_t enabled) {
    if (!enabled) {
        (void)SetElevatorLamp(shen_name{0}, 0);
        for (const ElevatorLampChoice &choice : ELEVATOR_LAMP_CHOICES)
            (void)SetElevatorDisplayMode(choice.elevator, 1, 2);
        return 1;
    }
    shen_name selected{0};
    if (g_currentMap.name == AREA_QF39.name) {
#endif
            FTBL.EV_SetPlayerFlag(12, 627, 0);
            FTBL.EV_SetPlayerFlag(12, 629, 1);
            FTBL.EV_SetPlayerFlag(12, 630, 0);
        }
        SetElevatorLamp((shen_name)0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        for (index = 0, choice = ELEVATOR_LAMP_CHOICES; index < 3; ++index, ++choice) {
            if (FTBL.EV_GetPlayerFlag(11, choice->flag)) {
                selected = choice->elevator;
#else
    for (const ElevatorLampChoice &choice : ELEVATOR_LAMP_CHOICES) {
        if (static_cast<std::uint32_t>(FTBL.EV_GetPlayerFlag(11, static_cast<std::int16_t>(choice.flag))) != 0) {
            selected = choice.elevator;
#endif
                FTBL.SetElevatorDisplayMode(selected, 1, 0);
            } else {
#if defined(SHENMUE_EVENT_OVERLAY)
                FTBL.SetElevatorDisplayMode(choice->elevator, 1, 2);
#else
            (void)SetElevatorDisplayMode(choice.elevator, 1, 2);
#endif
            }
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        if (selected)
#else
    if (selected.name != 0)
#endif
            SetElevatorLamp(selected, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    } else {
        SetElevatorLamp(0, 0);
        for (index = 0, choice = ELEVATOR_LAMP_CHOICES; index < 3; ++index, ++choice)
            FTBL.SetElevatorDisplayMode(choice->elevator, 1, 2);
    }
#endif
    return 1;
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
int SetElevatorLamp(shen_name elevator, int enabled) {
    Vector3 position;
    unsigned int yaw;
    int lightIndex = 13;
    float colour[4] = {1.0f, 1.0f, 0.8f, 0.5f};
    struct {
        float x, y;
    } falloff = {1.0f, 1.0f};
    Vector3 lightPosition = {0.0f, 2.695f, 0.2f};
    Vector3 characterPosition = {0.0f, 2.695f, 0.2f};
    Vector3i rotation;
#else
std::int64_t SetElevatorLamp(shen_name elevator, std::int32_t enabled) {
    constexpr std::uint32_t lightIndex = 13;
    Vector3 lightPosition = ELEVATOR_LAMP_OFFSET;
    Vector3 characterPosition = ELEVATOR_LAMP_OFFSET;
#endif
    if (enabled != 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
        if (FTBL.GetElevatorTransform(elevator, &position, &yaw) == 1)
            return 0;
        if (g_currentMap == SHEN_NAME4('Q', 'U', 'G', '0'))
            position.y = 96.4f;
        FTBL.matrix_stack_Matrix4x4_Push(0);
        FTBL.matrix_stack_UnitCurrentMatrix();
        FTBL.matrix_stack_TranslateVector(&position);
        FTBL.Matrix4x4_RotateAroundY_Wrapper(yaw);
        FTBL.Matrix4x4_TransformPoint(&lightPosition, &lightPosition);
        FTBL.PopCurrentMatrix(1);
#else
        Vector3 position;
        std::uint32_t yaw;
        if (GetElevatorTransform(elevator, position, yaw) == 1)
            return 0;
        if (g_currentMap.name == AREA_QUG0.name)
            position.y = 96.400002f;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const std::uint16_t angle = static_cast<std::uint16_t>(yaw);
        const std::uint16_t folded = angle <= 0x8000U ? angle : static_cast<std::uint16_t>(0U - angle);
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        Matrix4x4::RotateAroundY(sine, cosine);
        Matrix4x4::TransformPoint(lightPosition, lightPosition);
        Matrix4x4::Pop(1);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetLightEntryIndex(lightIndex, 1);
        FTBL.SetLightFlagForIndex(lightIndex, 3);
#else
        SetLightEntryIndex(13, 1);
        SetLightFlagForIndex(13, 3);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetLightRuntimeColour(lightIndex, colour);
#else
        SetLightRuntimeColour(13, ELEVATOR_LAMP_COLOUR);
#endif
        FTBL.SetLightSceneIntensity(lightIndex, 1.0f);
        FTBL.SetLightRange(lightIndex, 3.0f);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetLightFalloff(lightIndex, &falloff.x);
        FTBL.SetLightRuntimePosition(lightIndex, &lightPosition);
#else
        const float falloff = 1.0f;
        SetLightFalloff(13, &falloff);
        SetLightRuntimePosition(13, lightPosition);
#endif
        FTBL.SetLightGeometryEnabled(lightIndex, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.matrix_stack_Matrix4x4_Push(0);
        FTBL.matrix_stack_UnitCurrentMatrix();
        FTBL.matrix_stack_TranslateVector(&position);
        FTBL.Matrix4x4_RotateAroundY_Wrapper(yaw);
        FTBL.Matrix4x4_TransformPoint(&characterPosition, &characterPosition);
        FTBL.PopCurrentMatrix(1);
        rotation.x = 0;
        rotation.y = yaw;
        rotation.z = 0;
#else
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const float characterCosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
        const float characterSine = sin_fixed16(static_cast<std::int16_t>(angle));
        Matrix4x4::RotateAroundY(characterSine, characterCosine);
        Matrix4x4::TransformPoint(characterPosition, characterPosition);
        Matrix4x4::Pop(1);
        const Vector3i rotation{.x = 0, .y = static_cast<std::int32_t>(yaw), .z = 0};
#endif
        if (FTBL.DoesScenarioCharacterExist(ELEVATOR_LAMP_CHARACTER)) {
            FTBL.EV_EnsureCharacterDraw(ELEVATOR_LAMP_CHARACTER);
            FTBL.EV_EnableCharacterDisplay(ELEVATOR_LAMP_CHARACTER);
#if defined(SHENMUE_EVENT_OVERLAY)
            FTBL.EV_AT_SetPos(ELEVATOR_LAMP_CHARACTER, 0x78000000, &characterPosition);
            FTBL.EV_AT_SetAngY_0(ELEVATOR_LAMP_CHARACTER, 0x78000000, &rotation, 0);
#else
            EV_AT_SetPos(ELEVATOR_LAMP_CHARACTER, EventTransformFlags::World | EventTransformFlags::AllAxes, characterPosition);
            (void)EV_AT_SetAngY(ELEVATOR_LAMP_CHARACTER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
#endif
        }
    } else {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetLightEntryIndex(lightIndex, 0);
#else
        SetLightEntryIndex(13, 0);
#endif
        if (FTBL.DoesScenarioCharacterExist(ELEVATOR_LAMP_CHARACTER)) {
            FTBL.EV_DisableCharacterDisplay(ELEVATOR_LAMP_CHARACTER);
            FTBL.EV_ToggleCharacterFlag(ELEVATOR_LAMP_CHARACTER);
        }
    }
    return 1;
}

#if !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void AreaElevatorLampInit() {
}

// @verified - p
void AreaElevatorLampMainLoop(void *) {
}

// @verified - p
void AreaElevatorLampCleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    (void)ConfigureAreaElevatorLamp();
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
}

} // namespace shenmue::s2::areas

#endif
