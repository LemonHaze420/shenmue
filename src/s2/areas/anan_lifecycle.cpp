#include "s2/areas/area_anan.hpp"
#include "s2/areas/anan_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2::areas {
void AnanFinish(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    HLib::Task *caller = nullptr;
    if (work) {
        caller = work->caller;
        if (filmCheckFadeTrackColour())
            return;
        if (work->lifecycle & 2U) {
            AnanFilm(32, 0, 255);
            work->lifecycle &= ~2U;
        }
        spmFadeOut();
        if (work->ryoTask) {
            work->lifecycle |= 0x10000000U;
            return;
        }
        if (work->ananTask) {
            work->lifecycle |= 0x20000000U;
            return;
        }
        const std::uint32_t entry = work->entry, lifecycle = work->lifecycle;
        if (lifecycle & 0x200U) {
            for (const shen_name *name = ananCharacters; name->name != ANAN_INVALID.name; ++name) {
                const bool hasMotion = scenFindMotionwork(*name) != nullptr;
                EV_SetCharacterAlphaEnabled(*name, true);
                if (hasMotion)
                    mtwkSetExtraFlag(scenFindMotionwork(*name), 6, 0);
                EV_SetCharaControllerFlags(*name, 0, NpcCharacterControllerState::Enabled);
                EV_SetCharaControllerFlags(*name, 1, NpcCharacterControllerState::Enabled);
                if (hasMotion)
                    EV_SetRefBoardParamsFor(*name, 0);
                AT_SetLocalBaseChara(*name, ANAN_INVALID);
            }
            work->lifecycle = lifecycle & ~0x200U;
        }
        if (lifecycle & 0x400U) {
            npcSetAreaName(ANAN_NPC_REGION, nullptr, 0, 0);
            work->lifecycle = lifecycle & ~0x400U;
        }
        work->lifecycle &= 0x3fffffffU;
        FindMotionWork(ctrlGetCharaByName(ANAN_MAN));
        FindMotionWork(ctrlGetCharaByName(ANAN_RYO));
        hndlSetFlags(ctrlGetCharaByName(ANAN_MAN));
        hndlSetFlags(ctrlGetCharaByName(ANAN_RYO));
        if (lifecycle & 0x100U) {
            mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 17, 2);
            mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
            mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
            EV_npcFree(ANAN_MAN);
            EV_npcLockUnlock(ANAN_MAN);
            work->lifecycle = lifecycle & ~0x100U;
        }
        AnanFilm(32, 255, 0);
        spmFadeOut();
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        const char *packages[]{"ANANFIR2", "ANANFIR3", "ANANFIR1", "ANANSCC", "ANANSCC", "ANANTIOF", "ANANANGR", "ANANSTRT", "ANAN"};
        for (const char *package : packages) {
            if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
                EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
        }
        if (lifecycle & 4U) {
            EV_FreeAuthorPak(0, "0020");
            work->lifecycle = lifecycle & ~4U;
        }
        const std::int32_t flag = (entry & 1U) ? 28 : 19;
        const std::uint8_t mode = (entry & 0x80000000U) ? 0 : (entry & 0x40000000U) ? 1 : 2;
        EV_stepSetFlags(mode, flag);
        EV_EventSetUpHelpIcon_0(0, 0);
        if (!(work->lifecycle & 0x8000U)) {
            work->lifecycle |= 0x8000U;
            FinishLoading();
        }
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
    } else
        AnanFilm(0, 255, 0);
    SetPlayerControlEnabled(1);
    HLib::MarkCurrentTaskForCleanup();
    if (caller)
        HLib::ResolveDeferredCallback(caller);
    else
        EV_scenQueueLoad(1, ANAN_EXIT_AREA, 80);
}
void AnanFinishAuthor(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        spmFadeOut();
        Vector3 position;
        EV_GetCharaPosition(ANAN_MAN, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 17, 2);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
        EV_AT_SetPos(ANAN_MAN, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanInitialize(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    work->lifecycle |= 0x100U;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    Vector3 center = ANBD_BOARD_ORIGIN;
    npcSetAreaName(ANAN_NPC_REGION, &center, 5.5f, 1);
    work->lifecycle |= 0x400U;
    AnanLoadPackage("ANAN");
    EV_LoadCutsceneFromCurrentScene("0020E000.smp");
    EV_LoadAudio("0020E000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0020");
    work->lifecycle |= 4;
    EV_AT_SetPos(ANAN_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, ANBD_BOARD_ORIGIN);
    EV_AT_SetAngY(ANAN_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, ANBD_BOARD_ROTATION);
    EV_CheckWarmStartNpcRoute(ANAN_MAN);
    EV_ForceLoadNpc(ANAN_MAN);
    HLib::SetCurrentTaskTo(AnanWaitForNpc);
}
void AnanWaitForNpc(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work || EV_npcStreamOrLoadHuman(ANAN_MAN) != 1)
        return;
    for (const shen_name *name = ananCharacters; name->name != ANAN_INVALID.name; ++name) {
        AnanSuspendCharacter(*name);
        AT_SetLocalBaseChara(*name, ANAN_BASE);
    }
    EV_EnableCharacterDisplay(ANAN_MAN);
    EV_EnsureCharacterDraw(ANAN_MAN);
    SetLowQualityHands(ctrlGetCharaByName(ANAN_RYO));
    SetLowQualityHands(ctrlGetCharaByName(ANAN_MAN));
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_MAN), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_RYO), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ANAN_MAN), 1, 1, 0);
    FindMotionWork(ctrlGetCharaByName(ANAN_RYO));
    FindMotionWork(ctrlGetCharaByName(ANAN_MAN));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(ANAN_RYO), FacialNodeReplacement);
    SetFaceFlagsForChara_0(ctrlGetCharaByName(ANAN_MAN), FacialNodeReplacement);
    work->lifecycle |= 0x200U;
    HLib::SetCurrentTaskTo(AnanBeginEntry);
}
void AnanBeginEntry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (!(work->entry & 1U)) {
        ApplyAnanPlacements(ananRepeatEntryPlacement);
        HLib::SetCurrentTaskTo(AnanExplain);
        return;
    }
    ApplyAnanPlacements(ananFirstEntryPlacement);
    AnanLoadPackage("ANANSTRT");
    HLib::SetCurrentTaskTo(AnanBeginFirstChoice);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    AnanFilm(32, 255, 0);
    work->lifecycle |= 2;
    if (!QueueAnanAuthor(ANAN_START_AUTHOR, 0, 1))
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanBeginFirstChoice(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf002, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
    ApplyAnanPlacements(ananFirstChoicePlacement);
    AnanSelectCamera(1514);
    std::int16_t choices[]{-1, 90, 83, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
    HLib::SetCurrentTaskTo(AnanWaitFirstChoice);
    AnanSelectCamera(1900);
}
void AnanWaitFirstChoice(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 90) {
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c5, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
        SetMotionLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 40, MotionRegion::LowerBody);
        SetMotionRegionEvalTime(scenFindMotionwork(ANAN_MAN), 40, MotionRegion::LowerBody);
        HLib::SetCurrentTaskTo(AnanExplain);
        HLib::SetupTaskDelay(-1);
        QueueAnanVoice("0020A002", 0, 80, nullptr);
        QueueAnanVoice("0020B003", 100, 161, AnanVoiceResumeCaller);
        QueueAnanCamera(1902, 100);
    } else {
        ApplyAnanPlacements(ananDeclinePlacement);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0xf087, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
        HLib::SetCurrentTaskTo(AnanFinish);
        HLib::SetupTaskDelay(-1);
        QueueAnanVoice("0020A001", 0, 120, nullptr);
        QueueAnanVoice("0020B024", 130, 200, AnanVoiceResumeCaller);
        QueueAnanCamera(1902, 130);
    }
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("ANANSTRT"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ANANSTRT"), 0);
}
void AnanExplain(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    ApplyAnanPlacements(ananExplanationPlacement);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(ANAN_RYO), 71, 0, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_RYO), 1.2f, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c6, MotionRegion::LowerBody);
    SetMotionLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 220, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(ANAN_MAN), 220, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(ANAN_MAN), 71, 0, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 1.2f, MotionRegion::LowerBody);
    AnanFilm(32, 255, 0);
    HLib::SetCurrentTaskTo(AnanInitializeGame);
    HLib::SetupTaskDelay(-1);
    QueueAnanVoice("0020B004", 5, 110, nullptr);
    QueueAnanVoice("0020B005", 130, 195, nullptr);
    QueueAnanVoice("0020A003", 200, 250, nullptr);
    QueueAnanVoice("0020B006", 255, 350, nullptr);
    QueueAnanVoice("0020A004", 355, 385, nullptr);
    QueueAnanVoice("0020B007", 400, 445, AnanVoiceResumeCaller);
    AnanSelectCamera(1903);
}
void AnanBeginAngry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }

    if (ananAngryPlacement[0].character.name == ANAN_INVALID.name) {
        const AnanPlacement *row = ananAngryPlacement;
        do {
            EV_AT_SetPos(row->character, EventTransformFlags::AllAxes, row->position);
            EV_AT_SetAngY(row->character, EventTransformFlags::AllAxes, row->rotation);
            ++row;
        } while (row->character.name == ANAN_INVALID.name);
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c4, MotionRegion::LowerBody);
    SetMotionLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 2);
    HLib::SetCurrentTaskTo(AnanPlayAngryAuthor);
    HLib::SetupTaskDelay(-1);
    QueueAnanVoice("0020B028", 10, 85, AnanVoiceResumeCaller);
}
void AnanPlayAngryAuthor(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AnanLoadPackage("ANANANGR");
    HLib::SetCurrentTaskTo(AnanFinishAuthor);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(ANAN_ANGRY_AUTHOR, 1, 0))
        AnanFilm(32, 255, 0);
    else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanBeginTimeOver(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (ananAngryPlacement[0].character.name == ANAN_INVALID.name) {
        std::uint32_t i = 0;
        do {
            EV_AT_SetPos(ananTimeOverPlacement[i].character, EventTransformFlags::AllAxes, ananTimeOverPlacement[i].position);
            EV_AT_SetAngY(ananTimeOverPlacement[i].character, EventTransformFlags::AllAxes, ananTimeOverPlacement[i].rotation);
            ++i;
        } while (ananAngryPlacement[i].character.name == ANAN_INVALID.name);
    }
    const bool straight = AnanIsStraight(work);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), straight ? 0x67c3 : 0x67c8, MotionRegion::LowerBody);
    SetMotionLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(ANAN_MAN), 180, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 2);
    HLib::SetCurrentTaskTo(AnanPlayTimeOverAuthor);
    HLib::SetupTaskDelay(-1);
    QueueAnanVoice("0020B054", 30, 135, AnanVoiceResumeCaller);
}
void AnanPlayTimeOverAuthor(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const bool straight = AnanIsStraight(work);
    work->entry |= straight ? 0x80000000U : 0x40000000U;
    AnanLoadPackage(straight ? "ANANSCC" : "ANANTIOF");
    HLib::SetCurrentTaskTo(straight ? AnanFinishAuthor : AnanFinishTimeOver);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(straight ? ANAN_SUCCESS_AUTHOR : ANAN_TIME_FAIL_AUTHOR, straight ? 1 : 0, straight ? 0 : 1)) {
        AnanFilm(32, 255, 0);
        if (!straight)
            work->lifecycle |= 2;
    } else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanFinishTimeOver(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        HLib::SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_TIME_END_AUTHOR, 1, 0)) {
            work->lifecycle &= ~2U;
            return;
        }
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanSuccess(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    AnanUpdatePose(work);
    AnanSetTargets(false);
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    work->entry |= 0x80000000U;
    work->targets[0] = ANAN_INVALID;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AnanLoadPackage("ANANSCC");
    HLib::SetCurrentTaskTo(AnanFinishAuthor);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(ANAN_SUCCESS_AUTHOR, 1, 0))
        AnanFilm(32, 255, 0);
    else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanFailure(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    AnanUpdatePose(work);
    AnanSetTargets(false);
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    work->targets[0] = ANAN_INVALID;
    const bool fifth = work->failures >= 5;
    if (fifth)
        work->entry |= 0x80000000U;
    else
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AnanLoadPackage(fifth ? "ANANFIR3" : "ANANFIR1");
    HLib::SetCurrentTaskTo(fifth ? AnanFinishFifthFailure : AnanOfferRetry);
    EV_NPC_NewLoadSw(1);
    work->targets[0] = ANAN_INVALID;
    work->targets[1] = ANAN_INVALID;
    if (QueueAnanAuthor(fifth ? ANAN_FIFTH_AUTHOR : ANAN_FAIL_AUTHOR, 0, 1)) {
        AnanFilm(32, 255, 0);
        work->lifecycle |= 2;
    } else
        HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanOfferRetry(void *data) {
    if (!data) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    const Vector3 position{std::bit_cast<float>(0x3ef9db23U), std::bit_cast<float>(0xbba3d70aU), 2.4089999f};
    EV_AT_SetPos(ANAN_RYO, EventTransformFlags::AllAxes, position);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
    const Vector3i rotation{0, 13471, 0};
    EV_AT_SetAngY(ANAN_RYO, EventTransformFlags::AllAxes, rotation);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_RYO), 17, 0);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    std::int16_t choices[]{-1, 152, 34, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
    AnanSelectCamera(1905);
    HLib::SetCurrentTaskTo(AnanWaitRetry);
}
void AnanWaitRetry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("ANANFIR1"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ANANFIR1"), 0);
    if (shenGetCtrlSpeakIconFlag() != 152) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        AnanLoadPackage("ANANFIR2");
        HLib::SetCurrentTaskTo(AnanFinishFailure);
        EV_NPC_NewLoadSw(1);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (!QueueAnanAuthor(ANAN_FAIL_FIRST_AUTHOR, 0, 1))
            HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 17, 0);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_RYO), 17, 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_RYO), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_RYO), 0xf003, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_RYO), 1, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67c2, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ANAN_MAN), .89999998f, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(scenFindMotionwork(ANAN_MAN), 0);
    ApplyAnanPlacements(ananRetryPlacement);
    HLib::SetCurrentTaskTo(AnanRetry);
    HLib::SetupTaskDelay(-1);
    AnanSelectCamera(1906);
    QueueAnanVoice("0020A017", 0, 100, nullptr);
    QueueAnanVoice("0020B022", 101, 150, AnanVoiceResumeCaller);
}
void AnanFinishFailure(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        HLib::SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_FAIL_END_AUTHOR, 1, 0)) {
            work->lifecycle &= ~2U;
            return;
        }
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanFinishFifthFailure(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (work) {
        HLib::SetCurrentTaskTo(AnanFinishAuthor);
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        if (QueueAnanAuthor(ANAN_FIFTH_END_AUTHOR, 1, 0)) {
            work->lifecycle &= ~2U;
            return;
        }
    }
    HLib::SetCurrentTaskTo(AnanFinish);
}
void AnanRetry(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->lifecycle & 2U) {
        AnanFilm(32, 0, 255);
        work->lifecycle &= ~2U;
        return;
    }
    AnanFilm(32, 255, 0);
    work->lifecycle |= 2;
    HLib::SetCurrentTaskTo(AnanRestartGame);
    HLib::SetupTaskDelay(-1);
    QueueAnanWait();
    AnanStopLift(0, 1);
    AnanSelectCamera(1911);
    QueueAnanVoice("0020B023", 32, 142, AnanVoiceUnlockInput);
    work->gameFlags |= 0x80000000U;
    work->targets[0] = ANAN_MAN;
    work->targets[1] = ANAN_RYO;
    ApplyAnanPlacements(ananGamePlacement);
}
void AnanAskStraight(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (work->gameFlags & 0x80000000U)
        return;
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
    HLib::SetCurrentTaskTo(AnanWaitStraight);
    AnanGetHandPosition(work->tilt, &work->positions[0]);
}
void AnanWaitStraight(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    QueueAnanWait();
    work->gameFlags |= 0x80000000U;
    HLib::TaskCallback next;
    if (shenGetCtrlSpeakIconFlag() == 82) {
        work->targets[1] = ANAN_RYO;
        AnanSelectCamera(1716);
        QueueAnanVoice("0020A012", 10, 90, AnanVoiceUnlockInput);
        AnanSelectCamera(1904);
        if (AnanIsStraight(work))
            next = AnanSuccess;
        else {
            ++work->failures;
            next = AnanFailure;
        }
    } else {
        work->targets[0] = ANAN_INVALID;
        work->targets[1] = ANAN_INVALID;
        AnanSelectCamera(1757);
        next = AnanRestartGame;
        QueueAnanVoice("0020B023", 0, 110, AnanVoiceUnlockInput);
        mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
        mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 2);
        AnanSetLiftFrame(work->tilt);
        AnanGetHandPosition(work->tilt, &work->positions[1]);
        work->targets[0] = shen_name{};
        work->positionModes[0] = 0;
        work->positionModes[1] = 0;
        work->targets[1] = ANAN_INVALID;
    }
    HLib::SetCurrentTaskTo(next);
    HLib::SetupTaskDelay(-1);
}
} // namespace shenmue::s2::areas
