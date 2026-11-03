#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_krk4.hpp"

void AreaMain(unsigned int startParameters)
{
    FTBL.WSCN_LoadWeatherData(FTBL.Filepath_Gen(0), "W_COND.BIN");
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaKRK4WaveCallback, 3, 2, TASK_KRK4_WAVE);
}

void AreaCleanup(int afternoon)
{
}

void AreaKRK4WaveCallback(void *taskData)
{
    FTBL.nlSetTexanimUV(0.0f, 0.0049999999f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(2));
    FTBL.nlSetTexanimUV(0.0f, -0.0040000002f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(3));
    FTBL.nlSetTexanimUV(0.0f, 0.02f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(4));
}
#else
#include "s2/areas/area_krk4.hpp"

#include "s2/hlib.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaKRK4WaveCallback(void *) {
    SetTextureAnimationUV(0.0f, 0.0049999999f);
    ApplyTextureAnimationResource(2);
    SetTextureAnimationUV(0.0f, -0.0040000002f);
    ApplyTextureAnimationResource(3);
    SetTextureAnimationUV(0.0f, 0.02f);
    ApplyTextureAnimationResource(4);
}

// @verified - p
void AreaKRK4Init() {
}

// @verified - p
void AreaKRK4Main(std::uint32_t) {
    (void)WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), "W_COND.BIN");
    HLib::EnqueueTaskWithoutParameter(AreaKRK4WaveCallback, 3, 2, static_cast<std::int32_t>(TASK_KRK4_WAVE.name));
}

// @verified - p
void AreaKRK4Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaKRK4GameplayDefinition{
    .areaId = AREA_KRK4,
    .padding04 = 0,
    .init = &AreaKRK4Init,
    .main = &AreaKRK4Main,
    .cleanup = &AreaKRK4Cleanup,
};
} // namespace shenmue::s2::areas
#endif
