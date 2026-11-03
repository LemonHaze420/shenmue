#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_krk2.hpp"
#include "s2/event_queued_dc.h"

void AreaMain(unsigned int startParameters)
{
    FTBL.WSCN_LoadWeatherData(FTBL.Filepath_Gen(0), "W_COND.BIN");
    FTBL.HLib_EnqueueTaskWithoutParameter(eKRH2_ET_WAVE_KRK2_Callback, 3, 2, KRK2_EVENT_TASK);
    FTBL.BeginSecondarySampleExecution(0);
    FTBL.EV_LoadAudio("0519E000.sbt", 0);
    FTBL.EV_SetWeatherInstruction(KRK2_ENABLE_FOG_EFFECT, 0);
    FTBL.SetFogRed(6.0f);
    FTBL.SetFogGreen(1000.0f);
    FTBL.SetFogBlue(1.0f);
    FTBL.SetFogDistance(0xffb0e0e0U);
    FTBL.ReinitFogTasks(1);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.ReinitFogTasks(1);
    FTBL.CreateEventCharacter(3, 0, KRK2_CHARACTER);
    EVT_AT_SetPos(KRK2_CHARACTER, 0, 30.0f, 100.0f, 50.0f, 0);
    EVT_AT_SetAngY(KRK2_CHARACTER, 0, 0.0f, 0);
}

void AreaCleanup(int afternoon)
{
}

void eKRH2_ET_WAVE_KRK2_Callback(void *taskData)
{
    FTBL.nlSetTexanimUV(0.0f, 0.0046999999f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(1));
    FTBL.nlSetTexanimUV(0.0f, 0.0046999999f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(0));
    FTBL.nlSetTexanimUV(0.0f, -0.039999999f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(3));
}
#else
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
#endif
