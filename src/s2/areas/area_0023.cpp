#include "s2/areas/area_0023.hpp"
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name kek{SHEN_NAME(KEK_)};
constexpr shen_name characters[]{kek, AREA_AUTHOR_INVALID};
struct LoopParameter {
    AreaAuthorTaskParameter *parent;
    std::uint32_t smoke;
    std::uint32_t reserved0c;
};
static_assert(sizeof(LoopParameter) == 0x10);
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
void SetCharacters(bool restore) {
    for (shen_name name : {AREA_AUTHOR_RYO, kek}) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, restore);
    }
    EV_SetCharacterAlphaEnabled(SHEN_NAME(a016) | 0x1020000U, restore);
    EV_SetCharacterAlphaEnabled(SHEN_NAME(a009), restore);
}
// @verified - p
void SetupCharacters() {
    SetCharacters(false);
}
// @verified - p
void RestoreCharacters() {
    SetCharacters(true);
}
// @verified - p
void InitializeSmoke(LoopParameter *child) {
    child->smoke =
        static_cast<std::uint32_t>(CreateSmokeEffect({59, 103.55999755859375f, 309.20001220703125f}, {}, 100, 0.0010000000474974513f, shen_name{0}, 3));
    SetSmokeEffectFalloff(child->smoke, 10);
    SetSmokeEffectFadeFactor(child->smoke, 255);
    SetParticleEffectColour(0, child->smoke, 0, 142, 125, 110);
    SetParticleEffectInitialScale(0, child->smoke, 0.43529412150382996f);
    SetParticleEffectSize(0, child->smoke, 3.1372549533843994f, 3.1764705181121826f);
    SetParticleEffectBirthRate(0, child->smoke, 0);
    SetParticleEffectVelocity(0, child->smoke, {0, 0.009999999776482582f, 0}, {});
    SetSmokeEffectSpread(child->smoke, 55.86399841308594f, 1.058823585510254f);
    SetSmokeEffectRadialMotion(child->smoke, 0.772549033164978f, 0.46666666865348816f);
    SetParticleEffectBirthRate(0, child->smoke, 0.10000000149011612f);
}
// @verified - p
void Cleanup(AreaAuthorTaskParameter *parent) {
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0023A"), 0);
    EV_FreeAuthorPak(0, "0023");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_stepSetFlags(0, 22);
    if (g_currentMap == AREA_WKA0) {
        EV_LoadCutsceneFromCurrentScene("WKA0");
        ExecuteCleanupFuncForAreaWithCachedScene();
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {61.20000076293945f, 103.375f, 307.1000061035156f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 25486, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        SetMapModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void LoopCallback(void *data) {
    auto *child = static_cast<LoopParameter *>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!frame)
        InitializeSmoke(child);
    else if (frame == 1337)
        ResumeParticleEffect(0, child->smoke);
    else if (frame == 1347)
        SetParticleEffectBirthRate(0, child->smoke, 0);
    else if (frame == 1420)
        SuspendParticleEffect(0, child->smoke);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        FreeParticleEffect(0, child->smoke);
        RestoreCharacters();
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaAuthorTaskParameter *>(data);
    if (parent->state == 1) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        auto *child = static_cast<LoopParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->smoke = 0;
        SetupCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(0023)));
        parent->state = 2;
    } else if (parent->state == 3)
        Cleanup(parent);
}
// @verified - p
void Main(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0023");
    EV_LoadCutsceneFromCurrentScene("0023A000");
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
}
} // namespace
const ScriptAreaDefinition area0023ScriptDefinition{.areaId = SHEN_NAME(0023), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
