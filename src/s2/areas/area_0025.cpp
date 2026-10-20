#include "s2/areas/area_0025.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters[]{SHEN_NAME(SYP_), SHEN_NAME(KNK_), AREA_AUTHOR_INVALID};
constexpr Vector3 smokePositions[]{
    {335.7546081542969f, 129.37100219726562f, 245.04330444335938f},
    {337.7090148925781f, 129.46099853515625f, 245.03529357910156f},
    {335.1780090332031f, 129.4709930419922f, 242.3419952392578f},
    {338.235595703125f, 129.4709930419922f, 242.21009826660156f},
    {336.72760009765625f, 129.26600646972656f, 245.03280639648438f},
    {336.71820068359375f, 129.51100158691406f, 244.4698944091797f},
    {337.51971435546875f, 129.4709930419922f, 242.2375030517578f},
    {337.2933044433594f, 129.62600708007812f, 238.87210083007812f},
    {336.6243896484375f, 129.4011993408203f, 242.2886962890625f},
    {330.544189453125f, 129.44349670410156f, 243.28919982910156f},
    {342.7651062011719f, 129.47210693359375f, 242.7613067626953f},
    {331.84210205078125f, 129.61599731445312f, 238.2852020263672f},
    {341.62579345703125f, 129.38710021972656f, 238.26730346679688f},
    {335.9136962890625f, 129.39810180664062f, 242.23390197753906f},
    {336.04730224609375f, 129.49710083007812f, 238.885498046875f},
};
std::array<std::uint32_t, 16> smokeEffects{};
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
// @verified - p
void InitializeSmoke() {
    auto &first = smokeEffects[0];
    first = static_cast<std::uint32_t>(CreateSmokeEffect({336.6806945800781f, 128.16099548339844f, 250.6540069580078f},
        {0.10000000149011612f, 0, 0.10000000149011612f}, 70, 0.07999999821186066f, shen_name{0}, 0));
    SetSmokeEffectFalloff(first, 21);
    SetSmokeEffectFadeFactor(first, 110);
    SetParticleEffectColour(0, first, 0, 144, 144, 144);
    SetParticleEffectInitialScale(0, first, 0.32899999618530273f);
    SetParticleEffectSize(0, first, 1.559999942779541f, 1.409999966621399f);
    SetParticleEffectBirthRate(0, first, 0.007799999788403511f);
    SetParticleEffectVelocity(0, first, {0, 0.01600000075995922f, 0}, {});
    ResumeParticleEffect(0, first);
    for (std::size_t i = 0; i < 15; ++i) {
        auto &smoke = smokeEffects[i + 1];
        smoke = static_cast<std::uint32_t>(CreateSmokeEffect(smokePositions[i], {}, 40, 0.05999999865889549f, shen_name{0}, 0));
        SetSmokeEffectFalloff(smoke, 13);
        SetSmokeEffectFadeFactor(smoke, 188);
        SetParticleEffectColour(0, smoke, 0, 191, 194, 170);
        SetParticleEffectInitialScale(0, smoke, 0.027450980618596077f);
        SetParticleEffectSize(0, smoke, 0.6666666865348816f, 0.3921568691730499f);
        SetParticleEffectBirthRate(0, smoke, 0.0235294122248888f);
        SetParticleEffectVelocity(0, smoke, {0, 0.0117647061124444f, 0}, {0, 0.005098039284348488f, 0});
        ResumeParticleEffect(0, smoke);
    }
}
void SetCharacters(bool restore) {
    for (shen_name name : {AREA_AUTHOR_RYO, characters[0], characters[1]}) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, restore);
    }
    EV_SetCharacterAlphaEnabled(SHEN_NAME(a115) | 0x80000U, restore);
}
// @verified - p
void SetupCharacters() {
    SetCharacters(false);
}
// @verified - p
void RestoreCharacters() {
    SetCharacters(true);
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {338.3999938964844f, 127, 249.6999969482422f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 36772, 0}, 0};
    EVT_AT_SetAng_Callback(&angle);
}
// @verified - p
void Cleanup(AreaAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    EV_FreeAuthorPak(0, "0025");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0025A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == SHEN_NAME(WB01)) {
        EV_LoadCutsceneFromCurrentScene("WB01");
        EV_LoadAudio("WB01_000", 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void LoopCallback(void *data) {
    auto *parent = *static_cast<AreaAuthorTaskParameter **>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreCharacters();
        HLib::SetupTimedTask(parent->self, 10);
        parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaAuthorTaskParameter *>(data);
    if (parent->state == 1) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
        *static_cast<AreaAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
        SetupCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(0025)));
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
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    Queue_EVT_UTL_LoadNPCChara_3(characters, task);
    SetPlayerControlEnabled(0);
    EV_LoadCutsceneFromCurrentScene("0025A000");
    EV_FreeAuthorPak(1, "0025");
    if (g_currentMap.name == SHEN_NAME(0025)) {
        InitializeSmoke();
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
    }
}
} // namespace
const ScriptAreaDefinition area0025ScriptDefinition{.areaId = SHEN_NAME(0025), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
