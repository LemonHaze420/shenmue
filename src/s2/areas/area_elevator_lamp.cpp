#include "s2/areas/area_elevator_lamp.hpp"
#include "s2/elevator.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"

namespace shenmue::s2::areas {

// @verified - p
std::int64_t SetElevatorLamp(shen_name elevator, std::int32_t enabled) {
    Vector3 lightPosition = ELEVATOR_LAMP_OFFSET;
    Vector3 characterPosition = ELEVATOR_LAMP_OFFSET;
    if (enabled != 0) {
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
        SetLightEntryIndex(13, 1);
        SetLightFlagForIndex(13, 3);
        SetLightRuntimeColour(13, ELEVATOR_LAMP_COLOUR);
        SetLightSceneIntensity(13, 1.0f);
        SetLightRange(13, 3.0f);
        const float falloff = 1.0f;
        SetLightFalloff(13, &falloff);
        SetLightRuntimePosition(13, lightPosition);
        SetLightGeometryEnabled(13, 1);
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const float characterCosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
        const float characterSine = sin_fixed16(static_cast<std::int16_t>(angle));
        Matrix4x4::RotateAroundY(characterSine, characterCosine);
        Matrix4x4::TransformPoint(characterPosition, characterPosition);
        Matrix4x4::Pop(1);
        const Vector3i rotation{.x = 0, .y = static_cast<std::int32_t>(yaw), .z = 0};
        if (DoesScenarioCharacterExist(ELEVATOR_LAMP_CHARACTER)) {
            EV_EnsureCharacterDraw(ELEVATOR_LAMP_CHARACTER);
            EV_EnableCharacterDisplay(ELEVATOR_LAMP_CHARACTER);
            EV_AT_SetPos(ELEVATOR_LAMP_CHARACTER, EventTransformFlags::World | EventTransformFlags::AllAxes, characterPosition);
            (void)EV_AT_SetAngY(ELEVATOR_LAMP_CHARACTER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        }
    } else {
        SetLightEntryIndex(13, 0);
        if (DoesScenarioCharacterExist(ELEVATOR_LAMP_CHARACTER)) {
            EV_DisableCharacterDisplay(ELEVATOR_LAMP_CHARACTER);
            EV_ToggleCharacterFlag(ELEVATOR_LAMP_CHARACTER);
        }
    }
    return 1;
}

// @verified - p
std::int64_t ConfigureAreaElevatorLamp(std::int32_t enabled) {
    if (!enabled) {
        (void)SetElevatorLamp(shen_name{0}, 0);
        for (const ElevatorLampChoice &choice : ELEVATOR_LAMP_CHOICES)
            (void)SetElevatorDisplayMode(choice.elevator, 1, 2);
        return 1;
    }
    shen_name selected{0};
    if (g_currentMap.name == AREA_QF39.name) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 627, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 629, 1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 630, 0);
    }
    (void)SetElevatorLamp(shen_name{0}, 0);
    for (const ElevatorLampChoice &choice : ELEVATOR_LAMP_CHOICES) {
        if (static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(choice.flag))) != 0) {
            selected = choice.elevator;
            (void)SetElevatorDisplayMode(choice.elevator, 1, 0);
        } else {
            (void)SetElevatorDisplayMode(choice.elevator, 1, 2);
        }
    }
    if (selected.name != 0)
        (void)SetElevatorLamp(selected, 1);
    return 1;
}

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
