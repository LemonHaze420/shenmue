#include "s2/areas/area_chaw.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/duck_race.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {

std::uint32_t chawDuckReplayAlignment;
shen_name ReadCleanupPlacementCharacter(std::int32_t index) {
    if (index < 22)
        return chawPlacements[index].character;
    if (index == 22)
        return chawMapProp;
    if (index == 23)
        return shen_name{static_cast<std::uint32_t>(duckRaceRuntime.finishTicks[5])};
    if (index == 24)
        return shen_name{chawDuckReplayAlignment};
    std::uint32_t word;
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(index) * 32 - 772;
    std::memcpy(&word, reinterpret_cast<const std::byte *>(duckRaceRuntime.replay.data()) + offset, sizeof(word));
    return shen_name{word};
}
} // namespace

void ChawCleanup(ChawEventWork *work, bool restoreMedia) {
    using enum CameraState;
    DateTime date;
    GetCurrentDateTime(date);
    if (work->advanceTime)
        npcAdvanceTime(date.hour, date.minute);
    mtwkRequestMotion(scenFindMotionwork(CHAW_RYO), 0xf003, MotionRegion::LowerBody);
    scenCvsCtrlFree(nullptr);
    EV_FreeAuthorPak(0, "0038");
    ctrlStopSubtitles();
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    ChawStoreSign(work);
    if (restoreMedia) {
        EV_LoadCutsceneFromCurrentScene(work->savedBgm);
        EV_LoadAudio(work->savedSbt, work->savedAudioScene);
        EV_spmBeginSampleExecute();
    }
    if (work->entry == 15 && static_cast<std::uint32_t>(work->progression) <= 1) {
        Vector3 position;
        EV_GetCharaPosition(CHAW_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        position.x -= 0.6f;
        position.z += 0.3f;
        EV_AT_SetPos(CHAW_RYO, EventTransformFlags::AllAxes, position);
    }
    if (work->entry == 9 && static_cast<std::uint32_t>(work->progression) <= 1) {
        Vector3 position;
        EV_GetCharaPosition(CHAW_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        position.z += 0.3f;
        EV_AT_SetPos(CHAW_RYO, EventTransformFlags::AllAxes, position);
    }
    AT_SetLocalBaseChara(CHAW_RYO, CHAW_INVALID);
    AT_SetLocalBaseChara(CHAW_HAND, CHAW_INVALID);
    EV_ToggleCharacterFlag(CHAW_HAND);
    EV_DisableCharacterDisplay(CHAW_HAND);
    for (std::int32_t cup = 0; cup < 4; ++cup) {
        const shen_name name = work->cups[cup];
        if (work->entry != 21) {
            const std::int32_t index = work->entry + (g_actNumber != 2 ? 16 : 0);
            const shen_name original{ReadCleanupPlacementCharacter(index).name + (static_cast<std::uint32_t>(cup) << 24)};
            if (DoesScenarioCharacterExist(original)) {
                Vector3 position;
                EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
                EV_AT_SetPos(original, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
                EV_EnableCharacterDisplay(original);
            }
        }
        AT_SetLocalBaseChara(name, CHAW_INVALID);
        SetCharaKill(name, 1);
    }
    SetCharaKill(CHAW_BASE, 1);
    if (work->lectureLoaded)
        for (const shen_name *name = CHAW_LECTURE_CHARACTERS; name->name != CHAW_INVALID.name; ++name)
            if (DoesScenarioCharacterExist(*name)) {
                EV_npcLockUnlock(*name);
                EV_npcFree(*name);
            }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1021, 0);
    shenSetClockAndCompassRenderTo(0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CHAWAN"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_SetRefBoardParamsFor(CHAW_RYO, 1);
    EV_SetCharaControllerFlags(CHAW_RYO, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(CHAW_RYO, 3, NpcCharacterControllerState::Enabled);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    SetPlayerControlEnabled(1);
    FinishLoading();
    EV_NPC_NewLoadSw(1);
}
} // namespace shenmue::s2
