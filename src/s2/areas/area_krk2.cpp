#include "s2/areas/area_krk2.hpp"

#include "s2/event_runtime.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t enableFogEffect = 0x780;

void eKRH2_ET_WAVE_KRK2_Callback(void *) {
    SetTextureAnimationUV(0.0f, 0.0046999999f);
    ApplyTextureAnimationResource(1);
    SetTextureAnimationUV(0.0f, 0.0046999999f);
    ApplyTextureAnimationResource(0);
    SetTextureAnimationUV(0.0f, -0.039999999f);
    ApplyTextureAnimationResource(3);
}

} // namespace

void AreaKRK2Init() {
}

void AreaKRK2Main(std::uint32_t) {
    (void)WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    HLib::EnqueueTaskWithoutParameter(eKRH2_ET_WAVE_KRK2_Callback, 3, 2, static_cast<std::int32_t>(KRK2_EVENT_TASK.name));
    BeginSecondarySampleExecution(0);
    EV_LoadAudio("0519E000.sbt", 0);
    EV_SetWeatherInstruction(enableFogEffect, 0);
    SetFogRed(6.0f);
    SetFogGreen(1000.0f);
    SetFogBlue(1.0f);
    SetFogDistance(0xffb0e0e0U);
    ReinitFogTasks(1);
    ReinitFogTasks(1);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, KRK2_CHARACTER);
    EventSetPosition position{
        .cleanup = 0,
        .character = KRK2_CHARACTER,
        .parent = shen_name{},
        .position = KRK2_CHARACTER_POSITION,
        .delay = 0.0f,
    };
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = KRK2_CHARACTER, .parent = shen_name{}, .degrees = 0.0f, .delay = 0.0f};
    EVT_AT_SetAngY_Callback(&angle);
}

void AreaKRK2Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaKRK2GameplayDefinition{
    .areaId = SHEN_NAME(KRK2),
    .padding04 = 0,
    .init = &AreaKRK2Init,
    .main = &AreaKRK2Main,
    .cleanup = &AreaKRK2Cleanup,
};

} // namespace shenmue::s2::areas
