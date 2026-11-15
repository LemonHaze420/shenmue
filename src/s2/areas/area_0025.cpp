#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0025.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void E0025Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0025MainCallback, 4, 11, sizeof(E0025TaskParameter), TASK_EVNT);
    E0025TaskParameter *work = (E0025TaskParameter *)FTBL.HLib_GetTaskParameter(task);
    work->self = task;
    work->state = 1;
    work->selection = 0;
    Queue_EVT_UTL_LoadNPCChara_3(e0025Characters, work->self);
    ConfigureE0025Environment();
}
#else
shen_name e0025Characters[] = {SHEN_NAME4('S', 'Y', 'P', '_'), SHEN_NAME4('K', 'N', 'K', '_'), INVALID_CHARACTER};
static Vector3 e0025SmokePositions[] = {
    {336.6806945800781f, 128.16099548339844f, 250.6540069580078f},

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
unsigned int e0025Smoke[16];

void E0025MainCallback(void *data)
{
    E0025TaskParameter *work = (E0025TaskParameter *)data;
    switch (work->state) {
    case 1: QueueE0025Loop(work); break;
    case 3: CleanupE0025(work); break;
    }
}

void QueueE0025Loop(E0025TaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0025LoopCallback, 4, 11, sizeof(E0025LoopParameter), TASK_EVNT);
    E0025LoopParameter *work = (E0025LoopParameter *)FTBL.HLib_GetTaskParameter(task);
    work->parent = parent;
    SetupE0025Characters();
    FTBL.EV_authSceneCtrl(SHEN_NAME4('0', '0', '2', '5'));
    parent->state = 2;
}

void E0025LoopCallback(void *data)
{
    E0025LoopParameter *work = (E0025LoopParameter *)data;
    if (FTBL.EV_authSceneCtrl(-1) == 0) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0025Characters();
        FTBL.HLib_SetupTimedTask(work->parent->self, 10);
        work->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void InitializeE0025Smoke(void)
{
    int i;
    for (i = 0; i < 1; i++) {
        Vector3 position = {333.1507873535156f, 127.98090362548828f, 247.7050018310547f};
        Vector3 rotation = {0.1f, 0.0f, 0.1f};
        Vector3 acceleration = {0.0f, 0.0f, 0.0f};
        Vector3 velocity = {0.0f, 0.016f, 0.0f};
        position.x = e0025SmokePositions[i].x;
        position.y = e0025SmokePositions[i].y;
        position.z = e0025SmokePositions[i].z;
        e0025Smoke[i] = FTBL.CreateSmokeEffect(&position, &rotation, 70, 0.08f, 0, 0);
        FTBL.SetSmokeEffectFadeFactor(e0025Smoke[i], 21);
        FTBL.SetSmokeEffectFalloff(e0025Smoke[i], 110);
        FTBL.SetParticleEffectColour(0, e0025Smoke[i], 0, 144, 144, 144);
        FTBL.SetParticleEffectInitialScale(0, e0025Smoke[i], 0.329f);
        FTBL.SetParticleEffectSize(0, e0025Smoke[i], 1.56f, 1.41f);
        FTBL.SetParticleEffectBirthRate(0, e0025Smoke[i], 0.0078f);
        FTBL.SetParticleEffectVelocity(0, e0025Smoke[i], &velocity, &acceleration);
        FTBL.ResumeParticleEffect(0, e0025Smoke[i]);
    }
    for (i = 1; i < 16; i++) {
        Vector3 position;
        Vector3 rotation = {0.0f, 0.0f, 0.0f};
        Vector3 velocity;
        Vector3 acceleration;
        position.x = e0025SmokePositions[i].x;
        position.y = e0025SmokePositions[i].y;
        position.z = e0025SmokePositions[i].z;
        velocity.x = 0.0f;
        velocity.y = 0.0117647061124444f;
        velocity.z = 0.0f;
        acceleration.x = 0.0f;
        acceleration.y = 0.005098039284348488f;
        acceleration.z = 0.0f;
        e0025Smoke[i] = FTBL.CreateSmokeEffect(&position, &rotation, 40, 0.06f, 0, 0);
        FTBL.SetSmokeEffectFadeFactor(e0025Smoke[i], 13);
        FTBL.SetSmokeEffectFalloff(e0025Smoke[i], 188);
        FTBL.SetParticleEffectColour(0, e0025Smoke[i], 0, 191, 194, 170);
        FTBL.SetParticleEffectInitialScale(0, e0025Smoke[i], 0.027450980618596077f);
        FTBL.SetParticleEffectSize(0, e0025Smoke[i], 0.6666666865348816f, 0.3921568691730499f);
        FTBL.SetParticleEffectBirthRate(0, e0025Smoke[i], 0.0235294122248888f);
        FTBL.SetParticleEffectVelocity(0, e0025Smoke[i], &velocity, &acceleration);
        FTBL.ResumeParticleEffect(0, e0025Smoke[i]);
    }
}

void ConfigureE0025Environment(void)
{
    const char *directory;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    directory = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, (void *)directory, (int)"0025A", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "0025A", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0025A000");
    FTBL.EV_FreeAuthorPak(1, "0025");
    if (g_currentMap == SHEN_NAME4('0', '0', '2', '5')) {
        InitializeE0025Smoke();
        FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
        FTBL.PushGlobalLightingProgram();
    }
}

void CleanupE0025(E0025TaskParameter *parent)
{
    EVT_UTL_FreeNPCChara(e0025Characters);
    FTBL.EV_FreeAuthorPak(0, "0025");
    FTBL.EV_ControlPackageResources(0, 6, "0025A", 0, 0);
    FTBL.HLib_DestroyTask(parent->self);
    if (g_currentMap == SHEN_NAME4('W', 'B', '0', '1')) {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

void SetupE0025Characters(void)
{
    PrepareE0025Character(RYO_CHARACTER);
    PrepareE0025Character(SHEN_NAME4('S', 'Y', 'P', '_'));
    PrepareE0025Character(SHEN_NAME4('K', 'N', 'K', '_'));
    HideE0025Prop(SHEN_NAME4('a', '1', '9', '5'));
}

void RestoreE0025Characters(void)
{
    RestoreE0025Character(RYO_CHARACTER);
    RestoreE0025Character(SHEN_NAME4('S', 'Y', 'P', '_'));
    RestoreE0025Character(SHEN_NAME4('K', 'N', 'K', '_'));
    ShowE0025Prop(SHEN_NAME4('a', '1', '9', '5'));
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(RYO_CHARACTER), 0xf003, 0);
    EVT_AT_SetPos(RYO_CHARACTER, INVALID_CHARACTER, 338.4f, 127.0f, 249.7f, 0);
    EVT_AT_SetAng(RYO_CHARACTER, INVALID_CHARACTER, 0.0f, 202.0f, 0.0f, 0);
}

void PrepareE0025Character(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
void RestoreE0025Character(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
void HideE0025Prop(shen_name name) { FTBL.EV_SetCharacterAlphaEnabled(name, 0); }
void ShowE0025Prop(shen_name name) { FTBL.EV_SetCharacterAlphaEnabled(name, 1); }
#endif
#else
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
    SetSmokeEffectFadeFactor(first, 21);
    SetSmokeEffectFalloff(first, 110);
    SetParticleEffectColour(0, first, 0, 144, 144, 144);
    SetParticleEffectInitialScale(0, first, 0.32899999618530273f);
    SetParticleEffectSize(0, first, 1.559999942779541f, 1.409999966621399f);
    SetParticleEffectBirthRate(0, first, 0.007799999788403511f);
    SetParticleEffectVelocity(0, first, {0, 0.01600000075995922f, 0}, {});
    ResumeParticleEffect(0, first);
    for (std::size_t i = 0; i < 15; ++i) {
        auto &smoke = smokeEffects[i + 1];
        smoke = static_cast<std::uint32_t>(CreateSmokeEffect(smokePositions[i], {}, 40, 0.05999999865889549f, shen_name{0}, 0));
        SetSmokeEffectFadeFactor(smoke, 13);
        SetSmokeEffectFalloff(smoke, 188);
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
        if (ctrlGetScenarioChara(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    EV_FreeAuthorPak(0, "0025");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0025A"), 0);
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

#endif
