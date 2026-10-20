#include "s2/warehouse_script.hpp"
#include "s2/map_control.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/memo.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"
#include "s2/ui.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"
#include "s2/search.hpp"
#include "s2/face.hpp"
#include <bit>
#include <cstdio>
#include <array>
namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, invalid{0xffffffffU};
ConversationBuffer *dialogue{};
const char *ScriptString(std::uint32_t offset) {
    return static_cast<const char *>(HLib::GetPointerFromDynamicHeapOffsetBase(static_cast<std::int32_t>(offset)));
}
std::array<shen_name, 17> authorCharacters{};
char characterPackage[9]{};
char authorPackage[11]{};
std::uint32_t authorLoadMask{};
std::uint32_t authorFlags{};
std::int32_t savedFilmBars{};
HLib::TaskCallback authorContinuation{};
void WarehousePackageRetry(void *);
void WarehousePackageWait(void *);
void WarehouseExitAuthorWait(void *);
void WarehouseChooseExitAuthor(void *);
void WarehouseReturnToTemple(void *);
std::uint32_t lockonTicks{};
void WarehouseLockonFirst(void *);
void WarehouseLockonSecond(void *);
void EnableWarehouseController(shen_name name, bool enabled) {
    for (std::uint32_t flags : {1U, 2U, 3U})
        EV_SetCharaControllerFlags(name, flags, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
}
void WarehouseScriptCallback(void *data) {
    ExecuteWarehouseScript(*static_cast<std::uint32_t **>(data));
}
}
// @verified - p
void StopWarehouseSerifCallback(void *) {
    ctrlStopSubtitles();
    HLib::DestroyCurrentTask();
}
// @verified - p
HLib::Task *QueueWarehouseScriptLoop(std::uint32_t *script) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WarehouseScriptCallback, 4, 0x0b, 4, SHEN_NAME(EVNT));
    script[0] = static_cast<std::uint32_t>(HLib::GetDynamicHeapOffset(task));
    *static_cast<std::uint32_t **>(HLib::GetTaskParameter(task)) = script;
    script[2] = script[3] = 0;
    script[1] = 4;
    return task;
}
// @verified - p
void SeekWarehouseScriptLabel(std::uint32_t *script, std::uint16_t label) {
    std::uint32_t cursor = 4;
    for (;;) {
        const std::uint32_t command = script[cursor++];
        if ((command & 0xf0000000U) == 0x40000000U) {
            if (static_cast<std::uint16_t>(command) == label)
                break;
        } else if ((command & 0xf0000000U) == 0x80000000U)
            cursor += (command >> 16U) & 15U;
    }
    script[1] = cursor;
    script[2] = 0;
}
// @verified - p
std::int64_t ExecuteWarehouseScript(std::uint32_t *script) {
    using enum CameraState;
    std::int64_t result = 0;
    for (;;) {
        const std::uint32_t cursor = script[1], op = script[cursor];
        const auto arg = [&](std::uint32_t i) { return script[cursor + i]; };
        const auto name = [&](std::uint32_t i) { return shen_name{arg(i)}; };
        const auto advance = [&] {
            script[1] += ((op >> 16U) & 15U) + 1U;
            script[2] = 0;
        };
        const auto pending = [&] {
            ++script[2];
            return result;
        };
        switch (op) {
        case 0x80000002:
            result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
            if (result)
                return result;
            break;
        case 0x80000004:
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 0x8000001f:
            if (!script[2]) {
                result = RefreshPadTask(2, 1);
                return pending();
            }
            result = IsItemSelectable();
            if (result)
                return pending();
            break;
        case 0x80000020:
            SetPlayerControlEnabled(1);
            break;
        case 0x80000021:
            SetPlayerControlEnabled(0);
            break;
        case 0x80000023:
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            break;
        case 0x80000025:
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            EnableWarehouseController(ryo, true);
            break;
        case 0x8000002b:
            result = GetNpcNewLoadState();
            if (!result)
                return result;
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            break;
        case 0x8000002c: {
            DateTime time{};
            GetCurrentDateTime(time);
            ++time.day;
            time.hour = 8;
            time.minute = 0;
            SetCurrentDateTimeTo(time);
            EV_stepSetFlags(0, 100);
            break;
        }
        case 0x8000002e:
            ctrlStopSubtitles();
            break;
        case 0x8000002f:
            CheckHelpIcon(4, 0, CheckHelpIconOp::FrameOverlay);
            result = EVT_GetControllerInput(0, 1);
            if (result != 1024)
                return result;
            break;
        case 0x80000032:
            result = cvsHasValidConverseData(dialogue);
            if (result)
                return result;
            advance();
            return result;
        case 0x80010001: {
            const std::uint32_t author = arg(1);
            std::int32_t stateFlag = 0;
            if (author == SHEN_NAME(AUT0) || author == SHEN_NAME(AUT5) || author == SHEN_NAME(AUT7))
                EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
            else if (author == SHEN_NAME(AUT1) || author == SHEN_NAME(AUT2) || author == SHEN_NAME(AUT6)) {
                EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
                stateFlag = author == SHEN_NAME(AUT1) ? 36 : author == SHEN_NAME(AUT2) ? 34 : 35;
            }
            if (stateFlag && IsAuthorStateFlagSet(stateFlag))
                EV_SetAuthorActiveTo(1);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author));
            break;
        }
        case 0x80010005:
            if (!script[2]) {
                result = EV_CheckWarmStartNpcRoute(name(1));
                return pending();
            }
            result = EV_ForceLoadNpc(name(1));
            if (!result)
                return pending();
            EV_SetCharacterAlphaEnabled(name(1), false);
            break;
        case 0x80010008:
            SeekWarehouseScriptLabel(script, static_cast<std::uint16_t>(arg(1)));
            continue;
        case 0x80010009:
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
                const std::int32_t target = static_cast<std::int32_t>(arg(1)) + static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining));
                result = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
                if (result <= target)
                    return pending();
            }
            break;
        case 0x8001000a:
            CheckHelpIcon(arg(1), 0, CheckHelpIconOp::Set);
            break;
        case 0x8001000b:
            CheckHelpIcon(arg(1), arg(1), CheckHelpIconOp::Remove);
            break;
        case 0x8001000c:
            script[3] = arg(1);
            break;
        case 0x8001000d:
            ++script[2];
            if (arg(1) < script[2]) {
                advance();
                return script[1];
            }
            return cursor + 1U;
        case 0x8001000e:
            script[0] = 0;
            script[3] = arg(1);
            return HLib::DestroyCurrentTask();
        case 0x80010012:
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
                result = EVT_GetControllerInput(0, 1);
                if (result != 1024)
                    return result;
                SeekWarehouseScriptLabel(script, static_cast<std::uint16_t>(arg(1)));
                continue;
            }
            break;
        case 0x80010015:
            EV_CheckWarmStartNpcRoute(name(1));
            EV_npcFree(name(1));
            break;
        case 0x80010018:
            EV_CheckWarmStartNpcRoute(name(1));
            break;
        case 0x80010019:
            EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(arg(1)), 0);
            break;
        case 0x8001001d:
            ResetScenarioCharacterAttachment(name(1));
            break;
        case 0x80010022: {
            EnableWarehouseController(name(1), false);
            MotionWork *motion = scenFindMotionwork(name(1));
            mtwkSetRequestedRootMode(motion, 0);
            SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(motion, 0xf003, MotionRegion::LowerBody);
            break;
        }
        case 0x80010024:
            EV_npcLockUnlock(name(1));
            break;
        case 0x80010026:
            if (arg(1))
                ProcessAicaCommand(0xaa0, static_cast<std::int32_t>(arg(1)));
            else
                ProcessAicaCommand(0x1200a0, 0);
            break;
        case 0x80010028:
            EV_LoadCutsceneFromCurrentScene(ScriptString(arg(1)));
            break;
        case 0x80010029:
            if (!script[2]) {
                Queue_ST_LoadTitleFadeIn(shen_name{0});
                return pending();
            }
            if (script[2] != arg(1))
                return pending();
            ST_LoadTitleFade();
            break;
        case 0x8001002a: {
            const std::uint8_t alpha = static_cast<std::uint8_t>(255U * arg(1));
            SetScreenFilmFade(0, alpha, 0, 0, 0, alpha, 0, 0, 0);
            break;
        }
        case 0x8001002d:
            dbgPrintEventMessage(ScriptString(arg(1)));
            break;
        case 0x80010031: {
            char voice[32];
            std::snprintf(voice, 9, "00%x", arg(1));
            dialogue = EV_PlayDialogue(voice);
            break;
        }
        case 0x80010034:
            if (!script[2]) {
                result = EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                    reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>(ScriptString(arg(1))));
                return pending();
            }
            result = EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0);
            if (result)
                return pending();
            break;
        case 0x80010035:
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, arg(1), 0);
            break;
        case 0x80020007:
            EV_stepSetFlags(static_cast<std::uint8_t>(arg(2)), static_cast<std::int32_t>(arg(1)));
            break;
        case 0x80020014:
        case 0x8002001c: {
            ++script[2];
            if (script[2] == 1) {
                EV_SetCharaControllerFlags(name(1), 3, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(name(2), 3, NpcCharacterControllerState::Disabled);
                const Vector3 pos =
                    op == 0x80020014 ? Vector3{0.15000000596046448f, 0, -0.03500000014901161f} : Vector3{0.10000000149011612f, 0, -0.029999999329447746f};
                const Vector3i rot{0, 0, op == 0x80020014 ? 49152 : 61895};
                AttachScenarioCharacter(name(1), name(2), 14, &pos, &rot);
                return result;
            }
            if (script[2] == 2)
                advance();
            return result;
        }
        case 0x80020016:
            EVS_LoopCamera(arg(2), name(1), name(1));
            break;
        case 0x80020017: {
            HLib::Task *task = HLib::EnqueueTaskWithoutParameter(StopWarehouseSerifCallback, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
            dbgPrintEventMessage(ScriptString(arg(1)));
            HLib::SetupTimedTask(task, arg(2));
            break;
        }
        case 0x8002001b:
            mtwkRequestMotion(scenFindMotionwork(name(1)), static_cast<std::uint16_t>(arg(2)), MotionRegion::LowerBody);
            break;
        case 0x8002001e: {
            if (!script[2]) {
                SetRenderClockAndCompassTo(1);
                DateTime time{};
                GetCurrentDateTime(time);
                SetSleepActionIcon(static_cast<std::int16_t>(static_cast<std::uint32_t>(time.hour - 8) > 14 ? 15 : time.hour < 20 ? 266 : 271));
                shenSetTimeSimulationTo(0);
                return pending();
            }
            result = GetSleepActionIconResult();
            if (result != 1 && result != 2)
                return pending();
            SetRenderClockAndCompassTo(0);
            shenSetTimeSimulationTo(1);
            SeekWarehouseScriptLabel(script, static_cast<std::uint16_t>(arg(result == 2 ? 1 : 2)));
            continue;
        }
        case 0x80020027:
            EV_LoadAudio(ScriptString(arg(1)), arg(2));
            break;
        case 0x80020030:
            EV_scenQueueLoad(g_actNumber, name(1), static_cast<std::int32_t>(arg(2)));
            advance();
            return script[1];
        case 0x80020036: {
            DateTime time{};
            GetCurrentDateTime(time);
            time.hour = static_cast<std::uint8_t>(arg(1));
            time.minute = static_cast<std::uint8_t>(arg(2));
            SetCurrentDateTimeTo(time);
            break;
        }
        case 0x80030006:
            SeekWarehouseScriptLabel(script, static_cast<std::uint16_t>(arg(EV_GetShenmueFlag(arg(1)) ? 2 : 3)));
            continue;
        case 0x80030033: {
            const std::int32_t selection = shenGetRandomInt32(3);
            SeekWarehouseScriptLabel(script, static_cast<std::uint16_t>(arg(1 + selection)));
            continue;
        }
        case 0x80040003:
        case 0x8006001a: {
            if (!script[2]) {
                std::int16_t choices[]{-1, static_cast<std::int16_t>(arg(1)), static_cast<std::int16_t>(arg(2)), -1};
                SetupControlSpeakAndUI(
                    choices, op == 0x80040003 ? 1800 : static_cast<std::uint16_t>(arg(6)), static_cast<std::int16_t>(arg(op == 0x80040003 ? 2 : 5)), 2);
                SetRenderClockAndCompassTo(1);
                shenSetTimeSimulationTo(0);
                return pending();
            }
            const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
            result = GetShouldRenderIcons(RenderIconSlot::ActionSelector);
            if (result)
                return pending();
            SetRenderClockAndCompassTo(0);
            shenSetTimeSimulationTo(1);
            SeekWarehouseScriptLabel(script, static_cast<std::uint16_t>(arg(choice == arg(1) ? 3 : 4)));
            if (op == 0x80040003 && choice != arg(1))
                return pending();
            continue;
        }
        case 0x8004000f: {
            SetMotionRegionFlag(scenFindMotionwork(name(1)), 71, 0, MotionRegion::LowerBody);
            const Vector3 position{static_cast<float>(arg(2)) / 100.0f, static_cast<float>(arg(3)) / 100.0f, static_cast<float>(arg(4)) / 100.0f};
            EV_AT_SetPos(name(1), EventTransformFlags::AllAxes, position);
            break;
        }
        case 0x80040010: {
            SetMotionRegionFlag(scenFindMotionwork(name(1)), 71, 0, MotionRegion::LowerBody);
            const Vector3i rotation{static_cast<std::int32_t>(arg(2)), static_cast<std::int32_t>(arg(3)), static_cast<std::int32_t>(arg(4))};
            EV_AT_SetAngY(name(1), EventTransformFlags::AllAxes, rotation);
            break;
        }
        case 0x80050011: {
            SetMotionRegionFlag(scenFindMotionwork(name(1)), 71, 0, MotionRegion::LowerBody);
            const Vector3 position{static_cast<float>(arg(2)) / 100.0f, static_cast<float>(arg(3)) / 100.0f, static_cast<float>(arg(4)) / 100.0f};
            const Vector3i rotation{0, static_cast<std::int32_t>(arg(5)), 0};
            EV_AT_SetPos(name(1), EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(name(1), EventTransformFlags::AllAxes, rotation);
            break;
        }
        default:
            continue;
        }
        advance();
    }
}
// @verified - p
void HideWarehouseAuthorCharacter(shen_name character) {
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    mtwkSetRequestedRootMode(scenFindMotionwork(character), 0);
    SetMotionRegionFlag(scenFindMotionwork(character), 71, 0, MotionRegion::LowerBody);
    const Vector3 position{-1000, -1000, -1000};
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
}
// @verified - p
void LoadWarehouseAuthorPackages() {
    authorContinuation = WarehouseChooseExitAuthor;
    authorLoadMask = 0;
    std::snprintf(characterPackage, sizeof(characterPackage), "SUS3");
    std::snprintf(authorPackage, sizeof(authorPackage), "0031");
    authorCharacters[0] = SHEN_NAME(SYE_);
    authorCharacters[1] = SHEN_NAME(KNK_);
    authorCharacters[2] = shen_name{0};
    if (authorPackage[0])
        EV_FreeAuthorPak(1, authorPackage);
    for (std::uint32_t i = 0; authorCharacters[i].name; ++i) {
        authorLoadMask |= 2U << i;
        EV_CheckWarmStartNpcRoute(authorCharacters[i]);
    }
    if (characterPackage[0]) {
        authorLoadMask |= 1;
        HLib::SetCurrentTaskTo(WarehousePackageRetry);
    } else
        HLib::SetCurrentTaskTo(WarehousePackageWait);
}
// @verified - p
std::int64_t ReleaseWarehouseAuthorPackages() {
    if (characterPackage[0])
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(characterPackage), 0);
    for (std::uint32_t i = 0; authorCharacters[i].name; ++i) {
        EV_npcFree(authorCharacters[i]);
        EV_npcLockUnlock(authorCharacters[i]);
    }
    return authorPackage[0] ? EV_FreeAuthorPak(0, authorPackage) : 0;
}
// @verified - p
void StartWarehouseExitAuthor(std::uint32_t author) {
    authorContinuation = WarehouseReturnToTemple;
    authorFlags = 0x1c0;
    EV_spmBeginSampleExecute();
    if (authorFlags & 0x20)
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    if (!(authorFlags & 0x40)) {
        savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author));
    HLib::SetCurrentTaskTo(WarehouseExitAuthorWait);
}
namespace {
// @verified - p
void WarehousePackageRetry(void *) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(characterPackage)))
        HLib::SetCurrentTaskTo(WarehousePackageWait);
}
// @verified - p
void WarehousePackageWait(void *) {
    if ((authorLoadMask & 1) && !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
        authorLoadMask &= ~1U;
    for (std::uint32_t i = 0; authorCharacters[i].name; ++i)
        if ((authorLoadMask & (2U << i)) && EV_ForceLoadNpc(authorCharacters[i]))
            authorLoadMask &= ~(2U << i);
    if (!authorLoadMask) {
        for (std::uint32_t i = 0; authorCharacters[i].name; ++i)
            HideWarehouseAuthorCharacter(authorCharacters[i]);
        HideWarehouseAuthorCharacter(ryo);
        HLib::SetCurrentTaskTo(authorContinuation);
    }
}
// @verified - p
void WarehouseChooseExitAuthor(void *) {
    SetPlayerGameFlags(PlayerGameFlagMode::DisableAreaControl, nullptr);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    const std::int32_t choice = shenGetRandomInt32(1);
    constexpr std::uint32_t authors[]{SHEN_NAME(GI__)};
    StartWarehouseExitAuthor(authors[choice]);
}
// @verified - p
void WarehouseExitAuthorWait(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 31) {
        if (authorFlags & 2)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (authorFlags & 0x100)
            spmFadeOut();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        for (std::uint32_t i = 0; authorCharacters[i].name; ++i)
            HideWarehouseAuthorCharacter(authorCharacters[i]);
        if (authorFlags & 0x20)
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (!(authorFlags & 0x40))
            EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        HLib::SetCurrentTaskTo(authorContinuation);
    }
}
// @verified - p
void WarehouseReturnToTemple(void *) {
    const std::int32_t flag = EV_GetShenmueFlag(0x47) ? 0x47 : EV_GetShenmueFlag(0x25) ? 0x25 : 0x2e1;
    EV_stepSetFlags(0, flag);
    ReleaseWarehouseAuthorPackages();
    EV_FreeAuthorPak(0, "0031");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SUS2"), 0);
    EV_scenQueueLoad(1, SHEN_NAME(WB00), 3);
}
// @verified - p
void WarehouseLockonFirst(void *) {
    using enum CameraState;
    switch (++lockonTicks) {
    case 1: {
        EV_PlayDialogue("0031A067");
        const Vector3 position{335.78f, 127.88f, 239.67f}, look{336.276f, 130.235f, 237.6859f};
        const Vector3i rotation{};
        EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, rotation);
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &look, 1);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), 1, 20, 0.60000002f);
        break;
    }
    case 120:
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SUS2"), 0);
        EV_EnableCharacterDisplay(ryo);
        mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
        SetMotionFrameTime(scenFindMotionwork(ryo), 0, MotionRegion::LowerBody);
        mtwkRequestMotion(scenFindMotionwork(ryo), 0xf002, MotionRegion::LowerBody);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SUS2"), 0);
        EVS_LoopCamera(1037, invalid, invalid);
        EV_PlayDialogue("0031A068");
        break;
    case 270:
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
// @verified - p
void WarehouseLockonSecond(void *) {
    using enum CameraState;
    if (++lockonTicks == 1) {
        EV_PlayDialogue("0031A066");
        const Vector3 position{337.16f, 127.88f, 239.66f},
            look{std::bit_cast<float>(1135122186U), std::bit_cast<float>(1124224860U), std::bit_cast<float>(1131256218U)};
        const Vector3i rotation{};
        EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, rotation);
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &look, 1);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), 1, 20, 0.60000002f);
    } else if (lockonTicks == 120) {
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        HLib::DestroyCurrentTask();
    }
}
}
// @verified - p
void ResetWarehouseLockonSequence() {
    lockonTicks = 0;
}
// @verified - p
std::int64_t StartWarehouseLockonSequence() {
    using enum CameraState;
    const shen_name target = srchGetTargetName(0);
    if (target.name != SHEN_NAME(MA01) && target.name != SHEN_NAME(MA02))
        return target.name;
    constexpr shen_name button{SHEN_NAME(BTEV)};
    if (!GetBattleDataConfig(2, button))
        return GetBattleDataConfig(0, button);
    HLib::EnqueueTaskWithoutParameter(target.name == SHEN_NAME(MA01) ? WarehouseLockonFirst : WarehouseLockonSecond, 4, 0x0b, 1601200499);
    lockonTicks = 0;
    srchReset();
    EV_EnableCharacterDisplay(ryo);
    SetPlayerControlEnabled(0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SUS2"), 0);
    camCtrlSetState(Init);
    return EVS_LoopCamera(target.name == SHEN_NAME(MA01) ? 1038 : 1039, shen_name{0}, shen_name{0});
}
}
