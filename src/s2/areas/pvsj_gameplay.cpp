#include "s2/areas/area_pvsj.hpp"
#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
namespace {
std::int32_t Cash() {
    return static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash));
}
void Face(shen_name character) {
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(character), FacialExpressionId::Normal, 10, 1);
}
void Rebase(shen_name character) {
    UpdateMotionWorkWithPosition(scenFindMotionwork(character), nullptr, 0);
}
void DisableNpcLoads() {
    EV_NPC_NewLoadSw(0);
    npcReleaseUnusedStreams();
}
void SelectCameras(shen_name character) {
    const auto *dealer = PATI_DEALERS;
    for (; dealer->character.name && dealer->character.name != character.name; ++dealer) {
    }
    pvsjPlayerCamera = 901;
    pvsjCustomerCamera = dealer->motions ? dealer->camera : 911;
}
void SeedMotionAges() {
    g_patiMotionCache[0].character = {};
    g_patiMotionCache[0].age = 0;
    g_patiMotionCache[1].age = 1;
    g_patiMotionCache[2].character = {};
    g_patiMotionCache[2].age = 2;
    g_patiMotionCache[3].character = {};
    g_patiMotionCache[3].age = 3;
}
void Camera(PvsjEventWork *work, std::int32_t id, shen_name character) {
    PvsjSetCamera(work, id, character, character, work->delay);
}
void DialogueSubjects(PvsjEventWork *work, shen_name character) {
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    EV_EnableCharacterDisplay(PVSJ_PLAYER);
    GetCharacterMotionCompatibilityResult(FindCharacterByScenarioName(PVSJ_PLAYER));
    GetCharacterMotionCompatibilityResult(FindCharacterByScenarioName(character));
    Face(PVSJ_PLAYER);
    Face(character);
    EV_SetRefBoardParamsFor(PVSJ_PLAYER, 1);
    EV_SetRefBoardParamsFor(character, 1);
}
void ReleaseSubjects(PvsjEventWork *work, shen_name character) {
    Face(PVSJ_PLAYER);
    Face(character);
    HasCharacterMotionWork(FindCharacterByScenarioName(PVSJ_PLAYER));
    HasCharacterMotionWork(FindCharacterByScenarioName(character));
    EV_SetRefBoardParamsFor(character, 0);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_npcLockUnlock(character);
}
std::int32_t Minute() {
    DateTime date;
    GetCurrentDateTime(date);
    return date.minute + 60 * (date.hour + 24 * date.day);
}
shen_name RandomPoint(PvsjEventWork *work) {
    return work->boards->points[work->pointCount ? shenGetRandomInt32(work->pointCount - 1) + 1 : 0];
}
bool FacesCustomer(PvsjEventWork *work) {
    Vector3 position, customer;
    EV_GetCharaPosition(PVSJ_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    EV_GetCharaPosition(work->customer, CHARACTER_TRANSLATION_ROOT, &customer, EventTransformFlags::World);
    Vector3 direction{customer.x - position.x, customer.y - position.y, customer.z - position.z};
    Vector3::NormalizeInPlace(direction);
    Vector3 view{pvsjFilteredTarget.x - pvsjFilteredEye.x, pvsjFilteredTarget.y - pvsjFilteredEye.y, pvsjFilteredTarget.z - pvsjFilteredEye.z};
    Vector3::NormalizeInPlace(view);
    const float dot = (direction.y * view.y + direction.x * view.x) + direction.z * view.z;
    return sin_fixed16(static_cast<std::int16_t>(0x4000 - 3640)) <= dot;
}
void ResetBlend(PvsjEventWork *work) {
    if (pvsjCameraBlend == -1) {
        pvsjCameraBlend = 0;
        PvsjSetOverlay(work, 3);
    } else
        PvsjUpdateOverlay(work, 3);
}
void FinishCharacter(shen_name character) {
    Face(character);
    HasCharacterMotionWork(FindCharacterByScenarioName(character));
    EV_SetRefBoardParamsFor(character, 0);
    Rebase(character);
    EV_SetCharacterAlphaEnabled(character, true);
}
void LoadCustomerHands(PvsjEventWork *work) {
    std::int32_t index = 0;
    if (work->customer.name == work->owner.name)
        EV_CheckWarmStartNpcRoute(work->owner);
    else {
        pvsjCustomerModelIndex = 0;
        for (; PVSJ_CUSTOMER_PARTITIONS[index].character.name && PVSJ_CUSTOMER_PARTITIONS[index].character.name != work->customer.name; ++index) {
        }
    }
    pvsjCustomerModelIndex = index;
    work->customerPartition = index;
    DisableNpcLoads();
    EV_ResetFreeTalkFlags();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::LoadAfs, reinterpret_cast<std::uintptr_t>(work->jobPartition),
        static_cast<std::uintptr_t>(PVSJ_CUSTOMER_PARTITIONS[index].models), static_cast<std::uint32_t>(PVSJ_CUSTOMER_PARTITIONS[index].hands));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    const bool high = PVSJ_CUSTOMER_PARTITIONS[index].highHands == 1;
    Vector3 position{high ? -0.097198486f : -0.11279297f, high ? -0.045295715f : -0.050003052f, high ? 0.073486328f : 0.093688965f};
    Vector3i rotation{0, 0, 61895};
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(0);
    Matrix4x4::RotateAroundX_Wrapper(0);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    EV_scenApplyPosOrRotToPartOf(PATI_OPPONENT_HAND, 0xffff, &position, &rotation, EventCharacterPartTransformMode::Absolute);
    EV_AT_SetAngY(PATI_OPPONENT_HAND, EventTransformFlags::AllAxes, {0, 20935, 0});
}
} // namespace

void PvsjMainCallback(void *data) {
    auto *work = static_cast<PvsjEventWork *>(data);
    const auto previous = work->state;
    g_patiCameraTime = 99999;
    work->delay = 0;
    if (GetShouldRenderIcons(RenderIconSlot::Any))
        return;
    if (!filmCheckFadeTrackColour())
        GetCurrentControlInput(4);
    switch (work->state) {
    case 0:
        if (EV_ForceLoadNpc(work->owner)) {
            EV_CheckWarmStartNpcRoute(work->owner);
            Rebase(PVSJ_PLAYER);
            Rebase(work->owner);
            EV_SetCharacterAlphaEnabled(work->owner, false);
            PvsjSetMotion(work->owner, work->boards->motions[5], 0, 1, 0x40000000U, work->delay);
            work->delay = 2;
            work->state = 1;
        }
        break;
    case 1:
        SelectCameras(work->owner);
        Camera(work, pvsjCustomerCamera, work->owner);
        DisableNpcLoads();
        PvsjLoadVoice(work, work->owner);
        work->state = 2;
        break;
    case 2:
        if (!filmCheckFadeTrackColour()) {
            EV_NPC_NewLoadSw(1);
            work->state = 3;
        }
        break;
    case 3:
        if (EV_ForceLoadNpc(work->owner)) {
            EV_CheckWarmStartNpcRoute(work->owner);
            Rebase(work->owner);
            EV_SetCharacterAlphaEnabled(work->owner, false);
            GetCharacterMotionCompatibilityResult(FindCharacterByScenarioName(PVSJ_PLAYER));
            GetCharacterMotionCompatibilityResult(FindCharacterByScenarioName(work->owner));
            Face(PVSJ_PLAYER);
            Face(work->owner);
            EV_SetRefBoardParamsFor(PVSJ_PLAYER, 1);
            EV_SetRefBoardParamsFor(work->owner, 1);
            work->state = 4;
        }
        break;
    case 4:
        PvsjOfferJob(work);
        work->state = 5;
        break;
    case 5:
        work->previousCash = Cash();
        work->sessionCash = 300;
        SetPlayerStat(PlayerStatsId::Cash, 0);
        Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
        AnimateMoneyAmount(Cash(), work->sessionCash, 0);
        SetPlayerStat(PlayerStatsId::Cash, work->sessionCash);
        work->state = 6;
        break;
    case 6:
        PvsjBeginShift(work);
        work->startMinute = Minute();
        PvsjChooseCustomer(work);
        work->state = 7;
        break;
    case 7: {
        DisableNpcLoads();
        ReleaseSubjects(work, work->owner);
        EV_CheckWarmStartNpcRoute(work->customer);
        PvsjSetOverlay(work, 2);
        EV_SetRefBoardParamsFor(PVSJ_PLAYER, 0);
        const auto point = RandomPoint(work);
        work->customerActive = 1;
        if (work->pointCount) {
            PvsjMoveCustomer(work, work->customer, work->boards->points[0]);
            PvsjMoveCustomer(work, work->owner, point);
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->customer);
            SelectCameras(work->customer);
        } else {
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->owner);
            SelectCameras(work->owner);
        }
        EV_NPC_NewLoadSw(1);
        work->state = 9;
        break;
    }
    case 8: {
        DisableNpcLoads();
        ReleaseSubjects(work, work->customer);
        ResetBlend(work);
        EV_SetRefBoardParamsFor(PVSJ_PLAYER, 0);
        const auto point = RandomPoint(work);
        work->sessionCash = Cash();
        const auto oldCustomer = work->customer;
        const auto now = Minute();
        if ((now - work->startMinute <= 90 || work->firstCustomer) && work->sessionCash >= 50 && shenGetMinutesUntilNextFrameMinute() > 0)
            PvsjChooseCustomer(work);
        else {
            work->customerActive = 0;
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->owner);
            SelectCameras(work->owner);
        }
        work->firstCustomer = 0;
        if (work->pointCount) {
            if (work->customerActive) {
                EV_CheckWarmStartNpcRoute(work->customer);
                PvsjMoveCustomer(work, oldCustomer, point);
                PvsjMoveCustomer(work, work->customer, work->boards->points[0]);
                PvsjReleaseVoice(work);
                PvsjLoadVoice(work, work->customer);
                SelectCameras(work->customer);
            } else {
                EV_CheckWarmStartNpcRoute(work->owner);
                PvsjMoveCustomer(work, work->customer, point);
                PvsjMoveCustomer(work, work->owner, work->boards->points[0]);
                PvsjReleaseVoice(work);
                PvsjLoadVoice(work, work->owner);
                SelectCameras(work->owner);
            }
        } else {
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->owner);
            SelectCameras(work->owner);
        }
        EV_NPC_NewLoadSw(1);
        work->state = 9;
        break;
    }
    case 9:
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(PVSJ_PLAYER), FacialNodeOriginal);
        if (pvsjCameraBlend == -1) {
            pvsjCameraBlend = 0;
            PvsjSetOverlay(work, 3);
        } else {
            PvsjUpdateOverlay(work, 3);
            if (pvsjCameraBlend == 255) {
                work->state = 11;
                break;
            }
        }
        work->stateTicks = 0;
        work->state = 10;
        break;
    case 10:
        SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(5), nullptr);
        modelSetCharaAlpha(FindCharacterByScenarioName(PVSJ_PLAYER), (30.0f - work->stateTicks) / 30.0f);
        pvsjCameraBlend = static_cast<std::int16_t>((static_cast<float>(work->stateTicks) / 30.0f) * 255.0f);
        PvsjUpdateOverlay(work, 3);
        if (work->stateTicks >= 30)
            work->state = 11;
        else
            ++work->stateTicks;
        break;
    case 11:
        modelSetCharaAlpha(FindCharacterByScenarioName(PVSJ_PLAYER), 1);
        EV_DisableCharacterDisplay(PVSJ_PLAYER);
        SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(5), reinterpret_cast<void *>(1));
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(PVSJ_PLAYER), FacialNodeReplacement);
        pvsjCameraBlend = 255;
        PvsjUpdateOverlay(work, 1);
        work->state = 12;
        pvsjSkipMotion = 0;
        work->stateTicks = 0;
        work->callFrames = 0;
        EV_EventSetUpHelpIcon_0(145, 0);
        CheckHelpIcon(129, 0, CheckHelpIconOp::Set);
        SeedMotionAges();
        break;
    case 12: {
        const auto ticks = work->stateTicks;
        if (ticks > 16200) {
            for (std::int32_t i = 0; i < 10; ++i)
                EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 2539, "**** PVSJ:TIME OUT(NPC MoveStoS) ****\n");
            work->state = 60;
            RefreshPadControlData(0, 1);
            break;
        }
        work->stateTicks = ticks + 1;
        bool close = false;
        if (work->customerActive & 1) {
            if (EV_ForceLoadNpc(work->customer)) {
                if (DoesScenarioCharacterExist(work->customer)) {
                    EV_SetCharacterAlphaEnabled(work->customer, false);
                    Vector3 player, customer;
                    EV_GetCharaPosition(PVSJ_PLAYER, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
                    EV_GetCharaPosition(work->customer, CHARACTER_TRANSLATION_ROOT, &customer, EventTransformFlags::World);
                    const float dx = customer.x - player.x, dy = customer.y - player.y, dz = customer.z - player.z;
                    close = (dy * dy + dx * dx) + dz * dz < 25.0f;
                }
                if (!GetNpcScriptRouteCompletion(work->customer)) {
                    if (DoesScenarioCharacterExist(work->owner)) {
                        Rebase(work->owner);
                        EV_SetCharacterAlphaEnabled(work->owner, true);
                    }
                    work->state = 15;
                    break;
                }
            }
        } else if (EV_ForceLoadNpc(work->owner)) {
            if (DoesScenarioCharacterExist(work->owner))
                EV_SetCharacterAlphaEnabled(work->owner, false);
            if (!GetNpcScriptRouteCompletion(work->owner)) {
                if (DoesScenarioCharacterExist(work->customer)) {
                    Rebase(work->customer);
                    EV_SetCharacterAlphaEnabled(work->customer, true);
                }
                work->state = 35;
                work->sessionCash = Cash();
                if (shenGetMinutesUntilNextFrameMinute() <= 0)
                    work->state = 53;
                break;
            }
        }
        if (GetCurrentControlInput(0) & 0x400U) {
            if (close && FacesCustomer(work)) {
                pvsjSkipMotion = 1;
                switch (work->greeting) {
                case 0:
                    work->state = 29;
                    break;
                case 1:
                    work->state = 27;
                    break;
                case 2:
                    work->state = 24;
                    break;
                case 3:
                    work->state = 21;
                    break;
                default:
                    break;
                }
            } else
                work->state = 13;
        } else {
            if (!pvsjSkipMotion)
                PvsjUpdateOverlay(work, 0);
            pvsjSkipMotion = 0;
        }
        break;
    }
    case 13:
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        PvsjCallCustomer(work);
        work->state = 14;
        break;
    case 14:
        CheckHelpIcon(129, 0, CheckHelpIconOp::Set);
        work->state = 12;
        work->stateTicks = 0;
        break;
    case 15:
        PvsjSetMotion(work->customer, pvsjCustomerCamera == 921 ? 6060 : 6061, 0, 1, 0, work->delay);
        PvsjUpdateOverlay(work, 0);
        switch (work->greeting) {
        case 0:
            work->state = 28;
            break;
        case 1:
        case 2:
            work->state = 16;
            break;
        case 3:
            work->state = 20;
            break;
        default:
            break;
        }
        break;
    case 16:
        PvsjUpdateOverlay(work, 0);
        work->stateTicks = 0;
        work->state = 17;
        break;
    case 17:
        CheckHelpIcon(129, 0, CheckHelpIconOp::Set);
        PvsjUpdateOverlay(work, 0);
        if (++work->stateTicks >= 300)
            work->state = 34;
        else if (GetCurrentControlInput(0) & 0x400U) {
            if (FacesCustomer(work)) {
                if (work->greeting == 1)
                    work->state = 26;
                else if (work->greeting == 2)
                    work->state = 23;
            } else
                work->state = 18;
        }
        break;
    case 18:
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        PvsjCallCustomer(work);
        work->state = 19;
        break;
    case 19:
        CheckHelpIcon(129, 0, CheckHelpIconOp::Set);
        work->state = 17;
        break;
    case 20:
    case 21:
        DialogueSubjects(work, work->customer);
        if (work->state == 21) {
            PvsjRestoreCustomerRoute(work, work->customer);
            Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
            PvsjCallCustomer(work);
        }
        PvsjHandleCustomerInquiry(work);
        work->state = 22;
        break;
    case 22:
    case 25:
        work->state = 34;
        break;
    case 23:
    case 24:
        DialogueSubjects(work, work->customer);
        if (work->state == 24)
            PvsjRestoreCustomerRoute(work, work->customer);
        PvsjCustomerDeclines(work);
        work->state = 25;
        break;
    case 26:
    case 27:
        DialogueSubjects(work, work->customer);
        if (work->state == 27)
            PvsjRestoreCustomerRoute(work, work->customer);
        PvsjWelcomeCustomer(work);
        work->state = 30;
        break;
    case 28:
    case 29:
        DialogueSubjects(work, work->customer);
        if (work->state == 29) {
            PvsjRestoreCustomerRoute(work, work->customer);
            Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
            PvsjCallCustomer(work);
        }
        PvsjExplainPartTimeJob(work);
        work->state = 30;
        break;
    case 30:
        PvsjStartCustomerGame(work);
        work->state = 31;
        break;
    case 31: {
        SeedMotionAges();
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        EV_CheckWarmStartNpcRoute(work->customer);
        std::memcpy(pvsjGameArgument, "PVSJ", 4);
        std::memcpy(pvsjGameArgument + 4, &work->customer, 4);
        auto index = shenGetRandomInt32(pvsjBoardCount);
        if (!pvsjBoardPositions[index].character.name)
            index = 0;
        std::memcpy(pvsjGameArgument + 8, &pvsjBoardPositions[index].character, 4);
        std::memcpy(pvsjGameArgument + 12, &work->owner, 4);
        pvsjGameArgument[16] = 0;
        PvsjReleaseVoice(work);
        auto *invalid = reinterpret_cast<PakAfsPartition *>(~std::uintptr_t{});
        if (work->gamePartition != invalid) {
            EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Unmount, reinterpret_cast<std::uintptr_t>(work->gamePartition), 0);
            work->gamePartition = invalid;
        }
        LoadCustomerHands(work);
        PvsjBeginGame(work);
        work->state = 32;
        break;
    }
    case 32:
        EV_EventSetUpHelpIcon_0(145, 0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FreeAfs, reinterpret_cast<std::uintptr_t>(work->jobPartition),
            static_cast<std::uintptr_t>(PVSJ_CUSTOMER_PARTITIONS[work->customerPartition].models),
            static_cast<std::uint32_t>(PVSJ_CUSTOMER_PARTITIONS[work->customerPartition].hands));
        work->customerPartition = -1;
        SetPlayerControlEnabled(0);
        EV_npcLockUnlock(work->customer);
        work->gamePartition = reinterpret_cast<PakAfsPartition *>(EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount,
            reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)), reinterpret_cast<std::uintptr_t>("PATI.afs")));
        if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("PATI"), 0)) {
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
                reinterpret_cast<std::uintptr_t>("PATI"));
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        }
        work->cameraId = 0;
        work->cameraEye = {};
        work->cameraTarget = {};
        work->state = 34;
        break;
    case 34:
        ResetBlend(work);
        work->state = 8;
        break;
    case 35:
        DialogueSubjects(work, work->owner);
        work->delay = 5;
        work->state = shenGetMinutesUntilNextFrameMinute() > 0 ? 36 : 54;
        break;
    case 36:
        PvsjReportShift(work);
        work->state = 37;
        break;
    case 37:
        work->sessionCash = Cash();
        PvsjShowWallet(work);
        work->state = 38;
        break;
    case 38:
        SetPlayerStat(PlayerStatsId::Cash, work->previousCash);
        work->state = work->sessionCash > 300 ? 39 : 44;
        break;
    case 39:
        PvsjOfferPay(work);
        work->state = 40;
        break;
    case 40:
        work->newCash = (work->sessionCash - 299) / 2;
        Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
        PATI_StartMoneyCount(work->newCash, work->delay);
        work->delay += 5;
        work->state = 41;
        break;
    case 41:
        PvsjAskToContinue(work);
        work->state = 42;
        break;
    case 42:
        PvsjShowChoice(work);
        work->state = 43;
        break;
    case 43:
        work->state = shenGetCtrlSpeakIconFlag() == 37 ? 47 : 48;
        break;
    case 44:
        PvsjAskWithoutProfit(work);
        work->state = 45;
        break;
    case 45:
        PvsjShowChoice(work);
        work->state = 46;
        break;
    case 46:
        work->state = shenGetCtrlSpeakIconFlag() == 37 ? 47 : 50;
        break;
    case 47:
        if (shenGetMinutesUntilNextFrameMinute() > 0) {
            PvsjContinueShift(work);
            work->state = 5;
        } else
            work->state = 52;
        break;
    case 48:
        PvsjRestoreSpeech(work);
        work->state = 49;
        break;
    case 49:
        PvsjDeclineContinue(work);
        work->state = 59;
        break;
    case 50:
        PvsjRestoreSpeech(work);
        work->state = 51;
        break;
    case 51:
        PvsjFinishWithoutProfit(work);
        work->state = 59;
        break;
    case 52:
        PvsjTimeExpired(work);
        work->state = 59;
        break;
    case 53:
        DialogueSubjects(work, work->owner);
        work->delay = 5;
        work->state = 54;
        break;
    case 54:
        PvsjClosingReport(work);
        work->state = 55;
        break;
    case 55:
        work->sessionCash = Cash();
        PvsjShowWallet(work);
        work->state = 56;
        break;
    case 56:
        SetPlayerStat(PlayerStatsId::Cash, work->previousCash);
        work->state = work->sessionCash > 300 ? 57 : 51;
        break;
    case 57:
        PvsjOfferPay(work);
        work->state = 58;
        break;
    case 58:
        work->newCash = (work->sessionCash - 299) / 2;
        Camera(work, pvsjPlayerCamera, PVSJ_PLAYER);
        PATI_StartMoneyCount(work->newCash, work->delay);
        work->delay += 5;
        work->state = 59;
        break;
    case 59:
        work->previousCash = Cash();
        work->state = 60;
        break;
    case 60:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ProcessAicaCommand(0xaa0, 30);
        work->state = 61;
        break;
    case 61:
        if (!filmCheckFadeTrackColour()) {
            EV_SetRefBoardParamsFor(PVSJ_PLAYER, 0);
            Face(PVSJ_PLAYER);
            HasCharacterMotionWork(FindCharacterByScenarioName(PVSJ_PLAYER));
            if (DoesScenarioCharacterExist(work->owner))
                FinishCharacter(work->owner);
            EV_npcLockUnlock(work->owner);
            if (work->customer.name) {
                if (DoesScenarioCharacterExist(work->customer))
                    FinishCharacter(work->customer);
                EV_npcLockUnlock(work->customer);
            }
            PvsjFinish(work);
            return;
        }
        break;
    default:
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 3275, "PVSJ:Illegal Main State\n");
        break;
    }
    if (previous != work->state)
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 3282, "PVSJ State = %i\n", work->state);
    if (work->delay > 0)
        HLib::SetupTaskDelay(work->delay);
}
} // namespace shenmue::s2
