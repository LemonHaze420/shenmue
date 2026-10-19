#include "s2/areas/area_krct.hpp"
#include "s2/areas/krct_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstdint>
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name branchButton{SHEN_NAME(BTFT)}, handTorch{SHEN_NAME(a435)};
inline constexpr shen_name character0507{SHEN_NAME(a350)}, leaf{SHEN_NAME(a540)}, photos{SHEN_NAME(a436)};
inline constexpr shen_name character0530[]{SHEN_NAME(a199), SHEN_NAME(a197), SHEN_NAME(a271), SHEN_NAME(a272), SHEN_NAME(a273)};
inline constexpr shen_name character0509[]{SHEN_NAME(a528), SHEN_NAME(a526)};
inline constexpr shen_name torchObjects[]{SHEN_NAME(a435), SHEN_NAME(a466), SHEN_NAME(a468), SHEN_NAME(a470)};
inline constexpr shen_name house{SHEN_NAME(a016)}, l180{SHEN_NAME(l180)}, l240{SHEN_NAME(l240)}, l250{SHEN_NAME(l250)}, M001{SHEN_NAME(M001)},
    l310{SHEN_NAME(l310)};
void SuspendCharacter(shen_name name, bool human) {
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    if (human) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
        EV_SetCharacterAlphaEnabled(name, false);
    }
}
void RestoreCharacter(shen_name name, bool human) {
    if (human)
        EV_EnableCharacterDisplay(name);
    else
        EV_DisableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Enabled);
    if (human)
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
    else
        EV_SetCharacterAlphaEnabled(name, true);
}
void SuspendParty() {
    SuspendCharacter(KRCT_RYO, true);
    SuspendCharacter(KRCT_SHENHUA, true);
}
void RestoreParty() {
    RestoreCharacter(KRCT_RYO, true);
    RestoreCharacter(KRCT_SHENHUA, true);
}
void ConfigurePhotoCharacters(std::int32_t selection) {
    SuspendParty();
    if (selection == 2)
        for (shen_name name : character0509)
            SuspendCharacter(name, false);
    else if (selection == 4)
        SuspendCharacter(leaf, false);
    else if (selection == 6)
        SuspendCharacter(photos, false);
}
void RestorePhotoCharacters(std::int32_t selection) {
    RestoreParty();
    RestoreCharacter(leaf, false);
    if (selection == 6)
        RestoreCharacter(photos, false);
    for (shen_name name : character0509)
        RestoreCharacter(name, false);
}
void ConfigureMorningCharacters() {
    SuspendParty();
    for (shen_name name : character0530)
        SuspendCharacter(name, false);
    SuspendCharacter(KRCT_SHENHUA, true);
}
void RestoreMorningCharacters() {
    RestoreParty();
    for (shen_name name : character0530)
        RestoreCharacter(name, false);
    RestoreCharacter(house, false);
}
void ConfigureWindCharacters() {
    SuspendParty();
    for (const shen_name *name = krctWindObjects; name->name != KRCT_INVALID.name; ++name)
        SuspendCharacter(*name, false);
}
void RestoreWindCharacters() {
    RestoreParty();
    for (const shen_name *name = krctWindObjects; name->name != KRCT_INVALID.name; ++name)
        RestoreCharacter(*name, false);
}
void Reveal() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void Hide() {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
}
void EndLoop(KrctAuthorLoopWork *loop) {
    loop->parent->state = 4;
    HLib::DestroyCurrentTask();
}
void PhotoConversationLoop(void *data);
void PrepareNullsub(std::int32_t) {
}
void PrepareFirstConversation(std::int32_t) {
    EV_LoadCutsceneFromCurrentScene("0507A000");
    SetMapModelSlotEnabled(4, 0);
    SetMapModelSlotEnabled(0, 0);
}
void PrepareTorchConversation(std::int32_t) {
    const Vector3 position{154.78f, 100.14f, 392.16f};
    krctFire = static_cast<std::int32_t>(CreateFireEffect(position, .5f, .5f, 0, 0));
    SetFirePalette(krctFire, krctFirePalette.data());
    SetFireFalloff(krctFire, 3.1764705181121826f, 6.784313678741455f, .7058823704719543f);
    SetFireFalloffScalar(krctFire, -.2980392277240753f);
    SetParticleEffectSize(4, krctFire, .5098039507865906f, 1.0196079015731812f);
    SetParticleEffectPosition(4, krctFire, &position, &position);
    krctHandFire = static_cast<std::int32_t>(CreateFireEffect(position, .5f, .5f, 0, 0));
    SetFirePalette(krctHandFire, krctFirePalette.data());
    SetFireFalloffScalar(krctHandFire, -.05f);
    SetFireFalloff(krctHandFire, 3.14f, 4.94f, .78f);
    SetParticleEffectSize(4, krctHandFire, .025f, .05f);
    EV_LoadCutsceneFromCurrentScene("0518A000");
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(l180);
    PushGlobalLightingProgram();
    SetDefaultVSByIndex(4);
}
void PreparePhotoConversation(std::int32_t selection) {
    SetPlayerControlEnabled(0);
    if (selection != 5) {
        EV_LoadCutsceneFromCurrentScene("0509A000");
        EV_LoadAudio("0509A000", 0);
    }
}
void PrepareLateConversation(std::int32_t selection) {
    SetPlayerControlEnabled(0);
    if (selection != 7) {
        SetScenarioCharacterDrawControl(KRCT_SHENHUA, 0, 0);
        return;
    }
    EV_LoadCutsceneFromCurrentScene("0524A000");
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(l240);
    PushGlobalLightingProgram();
    SetDefaultVSByIndex(4);
}
void PrepareBranchConversation(std::int32_t) {
    SetPlayerControlEnabled(0);
    EV_LoadCutsceneFromCurrentScene("0525A000");
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(l250);
    PushGlobalLightingProgram();
    SetDefaultVSByIndex(4);
}
void PrepareMorningConversation(std::int32_t) {
    SetPlayerControlEnabled(0);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_SetWeatherInstruction(0x780, 0);
    SetMapModelSlotEnabled(0, 0);
}
void PrepareWindConversation(std::int32_t) {
    SetPlayerControlEnabled(0);
    EV_lightLoad(M001);
    AssignLightProgram(0);
    AssignLightProgram(6);
    AssignLightProgram(7);
    AssignLightProgram(8);
    EV_lightLoad(l310);
    AssignLightProgram(3);
    AssignLightProgram(5);
    EV_LoadCutsceneFromCurrentScene("0531A000");
}
void FinishFirstConversation(KrctAuthorWork *work) {
    SetMapModelSlotEnabled(4, 1);
    SetMapModelSlotEnabled(0, 1);
    *work->result = -1;
    HLib::DestroyTask(work->task);
    ProcessAicaCommand(0xaa0, 1);
}
void FinishTorchConversation(KrctAuthorWork *work) {
    clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 0);
    SetPluralControlFor(FindCharacterByScenarioName(KRCT_SHENHUA), 0);
    SetPluralControlFor(FindCharacterByScenarioName(KRCT_RYO), 0);
    EV_SetWeatherInstruction(12, 1);
    SuspendParticleEffect(4, krctFire);
    SuspendParticleEffect(4, krctHandFire);
    FreeParticleEffect(4, krctFire);
    FreeParticleEffect(4, krctHandFire);
    *work->result = -1;
    HLib::DestroyTask(work->task);
    ProcessAicaCommand(0xaa0, 1);
}
void FinishPhotoConversation(KrctAuthorWork *work) {
    clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 0);
    *work->result = -1;
    HLib::DestroyTask(work->task);
}
void FinishBranchConversation(KrctAuthorWork *work) {
    EV_SetWeatherInstruction(0x780, 1);
    EV_SetWeatherInstruction(12, 1);
    clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 0);
    *work->result = -1;
    HLib::DestroyTask(work->task);
}
void FinishMorningConversation(KrctAuthorWork *work) {
    SetMapModelSlotEnabled(0, 1);
    *work->result = -1;
    HLib::DestroyTask(work->task);
}
void FinishWindConversation(KrctAuthorWork *work) {
    *work->result = -1;
    HLib::DestroyTask(work->task);
}
void FinishReturnConversation(KrctAuthorWork *work) {
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    EV_stepSetFlags(0, 952);
    *work->result = -1;
    HLib::DestroyTask(work->task);
    spmFadeOut();
}
void FirstConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Hide();
        RestoreParty();
        RestoreCharacter(character0507, false);
        EndLoop(loop);
    }
}
void TorchConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    std::int16_t plural = 1;
    if (frame == 0) {
        SetLightSceneIntensity(27, krctFireHeight);
        plural = 0;
    } else if (frame == 1003)
        ResumeParticleEffect(4, krctHandFire);
    else if (frame == 1150) {
        ResumeParticleEffect(4, krctFire);
        SuspendParticleEffect(4, krctHandFire);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    } else if (frame == 1240) {
        SetFireFalloffScalar(krctFire, -.2705882489681244f);
        SetParticleEffectSize(4, krctFire, .27450981736183167f, .5098039507865906f);
    } else if (frame == 2679) {
        EV_SetIndexForLightEntry(32, 1);
        ResumeParticleEffect(4, krctHandFire);
    } else if (frame < 624)
        plural = 0;
    SetPluralControlFor(FindCharacterByScenarioName(KRCT_SHENHUA), plural);
    SetPluralControlFor(FindCharacterByScenarioName(KRCT_RYO), plural);
    if (static_cast<std::uint32_t>(frame - 1240) <= 259) {
        krctFireHeight += .0015384615f;
        SetLightSceneIntensity(27, krctFireHeight);
    } else if (frame == 1500) {
        krctFireHeight = .6f;
        SetLightSceneIntensity(27, krctFireHeight);
    }
    Vector3 root, position{0, .03f, 0};
    Vector3i rotation;
    EV_GetCharaPosition(handTorch, CHARACTER_TRANSLATION_ROOT, &root, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(handTorch, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&root, &rotation, &position);
    SetParticleEffectPosition(4, krctHandFire, &position, &position);
    if (static_cast<std::uint32_t>(frame - 1003) <= 66 || frame >= 2679)
        SetLightRuntimePosition(32, root);
    else if (frame == 1070)
        SetLightRuntimePosition(32, {154.5f, 101.69f, 392.75f});
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Hide();
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        RestoreParty();
        for (shen_name name : torchObjects)
            RestoreCharacter(name, false);
        EndLoop(loop);
    }
}
void PhotoConversationResume(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
    HLib::SetCurrentTaskTo(PhotoConversationLoop);
}
void PhotoConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    loop->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    const std::int32_t selection = loop->parent->selection;
    if (selection == 4 || selection == 6) {
        if (loop->frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 2) {
            EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
            spmFadeOut();
            HLib::SetCurrentTaskTo(PhotoConversationResume);
            HLib::SetupTaskDelay(15);
        }
    } else if (selection != 5 && loop->frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 15)
        spmFadeOut();
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestorePhotoCharacters(selection);
        Hide();
        EndLoop(loop);
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    }
}
void LateConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreParty();
        EndLoop(loop);
    }
}
void BranchConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    KrctAuthorWork *const work = loop->parent;
    loop->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop->frame == 0)
        SuspendParty();
    else if (loop->frame == 45 && work->selection == 11)
        scenCvsCtrlFree(GetConverseDataForChara(KRCT_SHENHUA));
    else if (loop->frame == 1200 && work->selection == 9)
        work->state = 3;
    else if (loop->frame == 1501) {
        if (work->selection == 10)
            clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 2);
        SuspendParty();
    } else if (loop->frame == 2450 && work->selection == 10)
        work->state = 2;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    switch (work->selection) {
    case 9:
        work->state = 1;
        work->selection = 10;
        HLib::DestroyCurrentTask();
        break;
    case 10:
        work->state = 1;
        work->selection = 12;
        HLib::DestroyCurrentTask();
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        break;
    case 11:
        work->selection = 12;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[12].name));
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        break;
    case 12:
        Hide();
        RestoreParty();
        EndLoop(loop);
        break;
    default:
        break;
    }
}
void MorningConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    loop->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop->frame == 0)
        ConfigureMorningCharacters();
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Hide();
        RestoreMorningCharacters();
        EndLoop(loop);
    }
}
void WindConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    if (--krctFireTicks <= 0) {
        krctFireTicks = shenGetRandomInt32(60) + 30;
        krctFirePhaseStep = shenGetRandomInt32(2048) + 2048;
    }
    krctFirePhase += krctFirePhaseStep;
    const float wave = sin_fixed16(static_cast<std::int16_t>(krctFirePhase));
    SetLightSceneIntensity(1, (wave * .1f) + .6f);
    SetLightSceneIntensity(3, (wave * .1f) + .6f);
    SetLightSceneIntensity(4, (wave * .1f) + .6f);
    loop->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop->frame == 410)
        SetNewWindPhaseAndSpeed(4096, .2f);
    else if (loop->frame == 559)
        SyncWindWithWDATWeatherState();
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Hide();
        spmFadeOut();
        RestoreWindCharacters();
        EndLoop(loop);
    }
}
void ReturnConversationLoop(void *data) {
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(data);
    loop->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop->frame == 0)
        SuspendParty();
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Hide();
        loop->parent->state = 4;
        RestoreParty();
        HLib::DestroyCurrentTask();
    }
}
KrctAuthorLoopWork *QueueLoop(KrctAuthorWork *parent, HLib::TaskCallback callback) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callback, 4, 0x0b, sizeof(KrctAuthorLoopWork), AREA_EVENT_TASK);
    KrctAuthorLoopWork *const loop = static_cast<KrctAuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop->parent = parent;
    loop->frame = 0;
    return loop;
}
void BeginFirstConversation(KrctAuthorWork *work) {
    QueueLoop(work, FirstConversationLoop);
    Reveal();
    SuspendParty();
    SuspendCharacter(character0507, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = 2;
}
void BeginTorchConversation(KrctAuthorWork *work) {
    QueueLoop(work, TorchConversationLoop);
    Reveal();
    clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 11);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[1].name));
    work->state = 2;
}
void BeginPhotoConversation(KrctAuthorWork *work) {
    QueueLoop(work, PhotoConversationLoop);
    if (work->selection == 2 || work->selection == 5) {
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 5);
    } else if (work->selection == 3) {
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 8);
    }
    if (work->selection == 2)
        Reveal();
    ConfigurePhotoCharacters(work->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = 2;
}
void BeginLateConversation(KrctAuthorWork *work) {
    QueueLoop(work, LateConversationLoop);
    ST_LoadTitleFade();
    SuspendParty();
    EV_SetObjectFlagByIndex(7, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = 2;
}
void BeginBranchConversation(KrctAuthorWork *work) {
    QueueLoop(work, BranchConversationLoop);
    clmdUtil(FindCharacterByScenarioName(KRCT_SHENHUA), 1);
    if (work->selection == 9) {
        EV_LoadAudio("0525A000", 0);
        EV_spmBeginSampleExecute();
        Reveal();
        ST_LoadTitleFade();
    }
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = work->selection == 10 ? 3 : 2;
}
void BeginMorningConversation(KrctAuthorWork *work) {
    QueueLoop(work, MorningConversationLoop);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    Reveal();
    ST_LoadTitleFade();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = 2;
}
void BeginWindConversation(KrctAuthorWork *work) {
    QueueLoop(work, WindConversationLoop);
    EV_LoadAudio("0531A000", 0);
    EV_spmBeginSampleExecute();
    Reveal();
    ConfigureWindCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = 2;
}
void BeginReturnConversation(KrctAuthorWork *work) {
    QueueLoop(work, ReturnConversationLoop);
    Reveal();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(krctAuthorNames[work->selection].name));
    work->state = 2;
}
using Prepare = void (*)(std::int32_t);
using Action = void (*)(KrctAuthorWork *);
constexpr Prepare prepare[]{PrepareFirstConversation, PrepareTorchConversation, PreparePhotoConversation, PreparePhotoConversation, PreparePhotoConversation,
    PreparePhotoConversation, PreparePhotoConversation, PrepareLateConversation, PrepareLateConversation, PrepareBranchConversation, PrepareBranchConversation,
    PrepareBranchConversation, PrepareBranchConversation, PrepareMorningConversation, PrepareWindConversation, PrepareNullsub};
constexpr Action begin[]{BeginFirstConversation, BeginTorchConversation, BeginPhotoConversation, BeginPhotoConversation, BeginPhotoConversation,
    BeginPhotoConversation, BeginPhotoConversation, BeginLateConversation, BeginLateConversation, BeginBranchConversation, BeginBranchConversation,
    BeginBranchConversation, BeginBranchConversation, BeginMorningConversation, BeginWindConversation, BeginReturnConversation};
constexpr Action finish[]{FinishFirstConversation, FinishTorchConversation, FinishPhotoConversation, FinishPhotoConversation, FinishPhotoConversation,
    FinishPhotoConversation, FinishPhotoConversation, FinishPhotoConversation, FinishPhotoConversation, FinishBranchConversation, FinishBranchConversation,
    FinishBranchConversation, FinishBranchConversation, FinishMorningConversation, FinishWindConversation, FinishReturnConversation};
} // namespace
void QueueKrctAuthor(std::int32_t *result) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KrctAuthorMain, 4, 0x0b, sizeof(KrctAuthorWork), KRCT_AUTHOR_TASK);
    KrctAuthorWork *const work = static_cast<KrctAuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    work->selection = *result;
    work->result = result;
    prepare[work->selection](work->selection);
}
void KrctAuthorMain(void *data) {
    KrctAuthorWork *const work = static_cast<KrctAuthorWork *>(data);
    switch (work->state) {
    case 1:
        begin[work->selection](work);
        break;
    case 3:
        if (GetBattleDataConfig(2, branchButton)) {
            work->selection = 11;
            work->state = 1;
        } else {
            GetBattleDataConfig(0, branchButton);
            CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
        }
        break;
    case 4:
        finish[work->selection](work);
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
