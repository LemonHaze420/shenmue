#include "s2/areas/area_chaw.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {

void ChawPreLectureCallback(void *data) {
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    shenSetClockAndCompassRenderTo(1);
    EV_LoadAudio("0038E000", 0);
    EV_spmBeginSampleExecute();
    SetDoorSubjectEnabled(SHEN_NAME(R103), 0);
    AT_SetLocalBaseChara(CHAW_RYO, CHAW_INVALID);
    const shen_name table{SHEN_NAME(tbl_)};
    AT_SetLocalBaseChara(table, CHAW_BASE);
    EV_AT_SetPos(table, EventTransformFlags::AllAxes, {0.008f, 0, 0.15f});
    EV_AT_SetAngY(table, EventTransformFlags::AllAxes, {});
    AT_SetLocalBaseChara(table, CHAW_INVALID);
    EV_EnableCharacterDisplay(table);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(LEC1)));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(ChawPreLectureAuthorCallback);
    work->helpFlags = 32;
    work->lectureLoaded = 1;
}
void ChawPreLectureAuthorCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetDoorSubjectEnabled(SHEN_NAME(R103), 1);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(LEC2)));
        HLib::SetCurrentTaskTo(ChawStartLectureCallback);
    }
    if (GetCurrentAuthorId().name == SHEN_NAME(LEC1)) {
        const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 322)
            snapSetDesc(0);
    }
}
void ChawStartLectureCallback(void *data) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        for (shen_name cup : CHAW_AUTHOR_CUPS)
            EV_DisableCharacterDisplay(cup);
        ChawEventWork *work = static_cast<ChawEventWork *>(data);
        work->lecturePhase = 1;
        work->lectureWord = 0;
        HLib::SetCurrentTaskTo(ChawInitCallback);
    }
}

bool ChawLectureAllowsInput(ChawEventWork *work) {
    if (cvsHasValidConverseData(nullptr) || chawSerifDelay)
        return false;
    switch (work->lecturePhase) {
    case 1:
        ChawAdvanceLecture(work, 2);
        return false;
    case 2:
        return ChawCheckLectureSelection(work, 4);
    case 4:
        return ChawCheckLectureMovement(work, 6);
    case 6:
        return ChawCheckLectureSelection(work, 8);
    case 8:
        return ChawCheckLectureMovement(work, 10);
    case 10:
        if (!IsModeBitActive(1)) {
            HLib::SetCurrentTaskTo(ChawEndLectureCallback);
            work->lecturePhase = 11;
            return false;
        }
        return true;
    default:
        return true;
    }
}

void ChawEndLectureCallback(void *data) {
    using enum CameraState;
    ChawEventWork *work = static_cast<ChawEventWork *>(data);
    switch (work->lecturePhase) {
    case 11:
        if (!cvsHasValidConverseData(nullptr)) {
            ChawSetupChoice(1);
            work->lecturePhase = 12;
        }
        break;
    case 12:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            if (shenGetCtrlSpeakIconFlag() != 149)
                EV_PlayDialogue("0038B045");
            work->lecturePhase = 13;
        }
        break;
    case 13:
        if (!cvsHasValidConverseData(nullptr)) {
            EV_EventSetUpHelpIcon_0(0, 0);
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
            nullsub();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(30);
            work->lecturePhase = 15;
        }
        break;
    case 15:
        for (shen_name cup : work->cups)
            EV_DisableCharacterDisplay(cup);
        for (shen_name cup : CHAW_AUTHOR_CUPS)
            EV_EnableCharacterDisplay(cup);
        EV_DisableCharacterDisplay(CHAW_HAND);
        ChawRestoreLighting(work);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(LEC3)));
        chawSmoke = ChawCreateLectureSmoke();
        work->lecturePhase = 14;
        break;
    case 14:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            const bool after = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) >= 311;
            const shen_name source{after ? SHEN_NAME(a059) : SHEN_NAME(a127)};
            Vector3 point = after ? Vector3{0, 0.1f, 0} : Vector3{0.22f, 0.066f, -0.004f};
            Vector3 position;
            Vector3i rotation;
            EV_GetCharaPosition(source, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(source, &rotation, EventTransformFlags::World);
            ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
            const Vector3 zero{};
            SetParticleEffectPosition(0, chawSmoke, &point, &zero);
        } else {
            if (!EV_GetPlayerFlag(static_cast<PlayerFlagReadMode>(3), 97)) {
                DateTime date;
                GetCurrentDateTime(date);
                date.hour = 23;
                date.minute = 0;
                SetCurrentDateTimeTo(date);
            }
            SuspendParticleEffect(0, chawSmoke);
            FreeParticleEffect(0, chawSmoke);
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("LECTURE"), 0);
            EV_AT_SetPos(CHAW_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {59.6f, 103.4f, 308.7f});
            EV_AT_SetAngY(CHAW_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 57344, 0});
            mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
            camCtrlSetState(Init);
            camCtrlSetFovTransitionTo(true);
            shenSetClockAndCompassRenderTo(0);
            work->state = 1;
            EV_stepSetFlags(0, 48);
            RemoveInventoryItem(14, 1);
            AddInventoryItem(41, 1, 0);
            HLib::SetCurrentTaskTo(ChawFinishCallback);
        }
        break;
    default:
        break;
    }
}

std::uint32_t ChawCreateLectureSmoke() {
    const Vector3 zero{};
    const std::uint32_t smoke = static_cast<std::uint32_t>(CreateSmokeEffect(zero, zero, 50, 0.08f, shen_name{}, 0));
    SetSmokeEffectFadeFactor(smoke, 8);
    SetSmokeEffectFalloff(smoke, 255);
    SetParticleEffectColour(0, smoke, 0, 193, 193, 191);
    SetParticleEffectInitialScale(0, smoke, 0.35686275362968445f);
    SetParticleEffectSize(0, smoke, 0.3921568691730499f, 0.4313725531101227f);
    SetParticleEffectBirthRate(0, smoke, 0.052941177040338516f);
    SetParticleEffectVelocity(0, smoke, {0, 0.00980392155f, 0}, zero);
    ResumeParticleEffect(0, smoke);
    return smoke;
}
} // namespace shenmue::s2
