#include "s2/areas/area_find.hpp"
#include "s2/scene_data.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <array>

namespace shenmue::s2::areas {
namespace {
std::array<AreaEntityPlacement, 22> findPlacements;
std::int32_t findPlacement;
std::int32_t findCamera;
std::int32_t findFarSign;
std::uint64_t findAccepted;
} // namespace

// @verified - p
void AreaFINDInit() {
    findPlacement = 0;
    findCamera = 0;
    findFarSign = 0;
    findAccepted = 0;
    std::copy_n(AREA_ENTITY_PLACEMENTS, findPlacements.size(), findPlacements.begin());
}

// @verified - p
void FINDBeginSignSearchCallback(void *) {
    scenCvsCtrlFree(nullptr);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FINDCHAW"), 0);
    Vector3 playerPosition;
    EV_GetCharaPosition(FIND_PLAYER, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    const float x = findPlacements[findPlacement].position.x - playerPosition.x;
    const float z = findPlacements[findPlacement].position.z - playerPosition.z;
    if (x * x + z * z >= 4.8400002f) {
        findFarSign = 1;
        findCamera = 1000;
    } else {
        findCamera = 2000;
        findFarSign = 0;
    }
    const Vector3i rotation{0, static_cast<std::uint16_t>(InverseFixedAtan2(x, z)), 0};
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(FIND_PLAYER)), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetAngY(FIND_PLAYER, EventTransformFlags::AllAxes, rotation, nullptr);
    EVS_LoopCamera(findCamera, FIND_PLAYER, FIND_BASE);
    EV_PlayDialogue("0038A027");
    HLib::SetCurrentTaskTo(FINDDescribeSignCallback);
    HLib::SetupTaskDelay(30);
}

// @verified - p
void FINDDescribeSignCallback(void *) {
    EV_PlayDialogue(findFarSign ? "0038A028" : "0038A030");
    EVS_LoopCamera(findCamera + 1, FIND_PLAYER, FIND_BASE);
    HLib::SetCurrentTaskTo(FINDOfferSignCallback);
    HLib::SetupTaskDelay(60);
}

// @verified - p
void FINDOfferSignCallback(void *) {
    if (!camCtrlCheckIsEventCam() && !cvsHasValidConverseData(nullptr)) {
        std::int16_t choices[4]{-1, 25, 26, -1};
        SetupControlSpeakAndUI(choices, 600, -1, 2);
        HLib::SetCurrentTaskTo(FINDWaitChoiceCallback);
    }
}

// @verified - p
void FINDWaitChoiceCallback(void *) {
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 25) {
        EV_PlayDialogue("0038A032");
        EV_EventSetUpHelpIcon_0(0x43, 0);
        findAccepted = (findAccepted & 0xffffffff00000000ULL) | 1;
    } else {
        EV_PlayDialogue("0038A033");
        findAccepted &= 0xffffffff00000000ULL;
    }
    HLib::SetCurrentTaskTo(FINDFinishChoiceCallback);
}

// @verified - p
void FINDFinishChoiceCallback(void *) {
    using enum CameraState;
    if (cvsHasValidConverseData(nullptr))
        return;
    SetCharaKill(FIND_BASE, 1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("FINDCHAW"), 0);
    EV_FreeAuthorPak(0, "0038");
    if (findAccepted) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        HLib::SetCurrentTaskTo(FINDEnterChawanCallback);
    } else {
        EV_NPC_NewLoadSw(1);
        EV_EnableCharacterDisplay(FIND_PLAYER);
        SetPlayerControlEnabled(1);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
void FINDEnterChawanCallback(void *) {
    if (filmCheckFadeTrackColour())
        return;
    EV_EnableCharacterDisplay(FIND_PLAYER);
    HLib::DestroyCurrentTask();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("CHAWAN"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    ExecuteCurrAreaMainFunc(FIND_CHAWAN_SLOT, static_cast<std::uintptr_t>(findPlacement + 22));
}

// @verified - p
void AreaFINDMain(std::uintptr_t argument) {
    const std::int32_t placementIndex = static_cast<std::int32_t>(argument);
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(FIND_PLAYER)), 0xf003, MotionRegion::LowerBody);
    EV_FreeAuthorPak(1, "0038");
    HLib::EnqueueTaskWithoutParameter(FINDBeginSignSearchCallback, 4, 0x0b, static_cast<std::int32_t>(FIND_TASK.name));
    CreateEventCharacter(CharacterCreationHighModel, nullptr, FIND_BASE);
    AreaEntityPlacement &placement = findPlacements[placementIndex];
    const std::uint16_t yaw = static_cast<std::uint16_t>(placement.yaw);
    placement.position.x += sin_fixed16(static_cast<std::int16_t>(yaw)) * 0.40000001f;
    placement.position.z += sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(yaw))) * 0.40000001f;
    EV_AT_SetPos(FIND_BASE, EventTransformFlags::AllAxes, placement.position);
    findPlacement = placementIndex;
    findFarSign = 1;
    findAccepted = (findAccepted & 0xffffffff00000000ULL) | 1;
}

const ScriptAreaDefinition areaFINDScriptDefinition{.areaId = AREA_FIND, .field04 = 0, .init = AreaFINDInit, .main = AreaFINDMain};

} // namespace shenmue::s2::areas
