#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/areas/wbbk_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {
std::int16_t bookCarryExitTicks;
std::int32_t bookCarryIdleFrames;
namespace {
struct BookRoundPlacement {
    Vector3 position;
    std::int32_t yaw;
};
static_assert(sizeof(BookRoundPlacement) == 16);
constexpr BookRoundPlacement roundPlacements[14]{
    {.position = {.x = 342.019989f, .y = 126.970001f, .z = 235.399994f}, .yaw = 63418},
    {.position = {.x = 342.019989f, .y = 126.970001f, .z = 235.399994f}, .yaw = 63418},
    {.position = {.x = 340.649994f, .y = 126.970001f, .z = 235.190002f}, .yaw = 57455},
    {.position = {.x = 337.019989f, .y = 126.970001f, .z = 230.050003f}, .yaw = 45621},
    {.position = {.x = 337.019989f, .y = 126.970001f, .z = 230.050003f}, .yaw = 45621},
    {.position = {.x = 335.649994f, .y = 126.970001f, .z = 230.300003f}, .yaw = 47153},
    {.position = {.x = 338.850006f, .y = 126.970001f, .z = 235.929993f}, .yaw = 56297},
    {.position = {.x = 338.850006f, .y = 126.970001f, .z = 235.929993f}, .yaw = 56297},
    {.position = {.x = 335.850006f, .y = 126.970001f, .z = 235.520004f}, .yaw = 53395},
    {.position = {.x = 333.420013f, .y = 126.970001f, .z = 229.949997f}, .yaw = 45902},
    {.position = {.x = 333.420013f, .y = 126.970001f, .z = 229.949997f}, .yaw = 45902},
    {.position = {.x = 330.290009f, .y = 126.970001f, .z = 230.009995f}, .yaw = 48419},
    {.position = {.x = 334.779999f, .y = 126.970001f, .z = 235.729996f}, .yaw = 52869},
    {.position = {.x = 332.100006f, .y = 126.970001f, .z = 236.020004f}, .yaw = 53394},
};
constexpr Vector3 placedPositions[14]{
    {.x = 342.100006f, .y = 127.449997f, .z = 237.360001f},
    {.x = 341.200012f, .y = 127.449997f, .z = 237.360001f},
    {.x = 340.299988f, .y = 127.449997f, .z = 237.360001f},
    {.x = 336.850006f, .y = 127.449997f, .z = 228.300003f},
    {.x = 335.75f, .y = 127.449997f, .z = 228.300003f},
    {.x = 334.649994f, .y = 127.449997f, .z = 228.300003f},
    {.x = 338.200012f, .y = 127.449997f, .z = 237.360001f},
    {.x = 337.100006f, .y = 127.449997f, .z = 237.360001f},
    {.x = 336, .y = 127.449997f, .z = 237.360001f},
    {.x = 332, .y = 127.449997f, .z = 228.300003f},
    {.x = 330.899994f, .y = 127.449997f, .z = 228.300003f},
    {.x = 329.799988f, .y = 127.449997f, .z = 228.300003f},
    {.x = 334, .y = 127.449997f, .z = 237.360001f},
    {.x = 332.200012f, .y = 127.449997f, .z = 237.360001f},
};
} // namespace
void ResetBookMotionLayers() {
    SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 73, 0, MotionRegion::RightArm);
    SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 73, 0, MotionRegion::LeftArm);
    SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 73, 0, MotionRegion::Chest);
    SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 73, 0, MotionRegion::Head);
}
void SelectBookStoryVariant() {
    if (EV_GetShenmueFlag(0x2e))
        bookStoryVariant = 1;
    else if (EV_GetShenmueFlag(0xfd))
        bookStoryVariant = 2;
    else if (EV_GetShenmueFlag(0xfc))
        bookStoryVariant = 3;
    else
        bookStoryVariant = (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 420) != 0) + 4;
}
void InitializeBookCarryState() {
    bookCarryDistance = 0;
    bookCarrySegmentFrame = 0;
    bookDesiredCamera = 0;
    bookCurrentCamera = 1;
    bookCarryTrack = bookInitialCarryTracks[bookModelState.currentStack];
    bookCarryMode = 0;
    bookCarryPhase = 0;
    bookModelState.displayMode = 0;
    bookCarryVariant = 0;
    bookCompletion = 0;
    bookCarryIdleFrames = 0;
    bookQteTimingIndex = 0;
    bookCarryMotion.input = 0;
    bookCarryMotion.raised = 1;
    bookCarryMotion.requested = 9;
    bookCarryMotion.current = 9;
    bookCarryMotion.side = 0;
    bookCarryMotion.pendingQte = 0;
    bookCarryMotion.frame = 0;
    bookCarryMotion.radialOffset = 0;
    bookCarryMotion.previousRootX = 0;
    bookCarryExitTicks = 0;
    ResetBookMotionLayers();
    mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), bookMotionPresets[0].motion, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(BOOK_BASE, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(BOOK_BASE, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(BOOK_BASE, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    InitializeBookCarryQte();
}
void InitializeBookActors() {
    bookModelState.currentBook = 0;
    bookCarryDistance = 0;
    bookCarryTrack = bookInitialCarryTracks[bookModelState.currentStack];
    bookCarrySegmentFrame = 0;
    bookDesiredCamera = 0;
    bookCarryMode = 0;
    bookCarryPhase = 0;
    bookCarryIdleFrames = 0;
    EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetFlags, 0, 0);
    shen_name actor = BOOK_FIRST;
    for (std::int32_t index = 0; index < 20; ++index, ++actor.name) {
        SetScenarioImageModule(actor, bookModelTypes[bookHeldPlacements[index].type].model);
        EV_SetCharaControllerFlags(actor, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    }
}
void UpdateBookCompanionPosition() {
    if (GetMotionAnimationId(scenFindMotionwork(BOOK_SAI), MotionRegion::LowerBody) != 0x1899)
        return;
    Vector3 position;
    EV_GetCharaPosition(BOOK_SAI, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x += (bookCompanionTarget.x - position.x) * .003f;
    position.z += (bookCompanionTarget.z - position.z) * .003f;
    EV_AT_SetPos(BOOK_SAI, EventTransformFlags::AllAxes, position);
}
void UpdateBookCarryAnchor() {
    Vector3 left, right;
    EV_GetCharaPosition(BOOK_RYO, 21, &left, EventTransformFlags::World);
    EV_GetCharaPosition(BOOK_RYO, 14, &right, EventTransformFlags::World);
    const std::int32_t yaw = InverseFixedAtan2(left.x - right.x, left.z - right.z) - 16384;
    const Vector3i stackRotation{.x = 0, .y = 0, .z = InverseFixedAtan2(right.y - left.y, 1) + 32768};
    Vector3 centre{.x = (left.x + right.x) * .5f, .y = (left.y + right.y) * .5f, .z = (left.z + right.z) * .5f};
    const Vector3i rotation{.x = 0, .y = yaw, .z = 0};
    Vector3 offset{.x = 0, .y = -.12f, .z = -.1f};
    ApplyScenarioTranslationAndRotation(&centre, &rotation, &offset);
    if (bookCarryVariant == 2)
        offset.y -= .1f;
    const Vector3 anchor{.x = (1 - bookCarryAnchorBlend) * bookCarryAnchor.x + bookCarryAnchorBlend * offset.x,
        .y = (1 - bookCarryAnchorBlend) * bookCarryAnchor.y + bookCarryAnchorBlend * offset.y,
        .z = (1 - bookCarryAnchorBlend) * bookCarryAnchor.z + bookCarryAnchorBlend * offset.z};
    EV_AT_SetAngY(bookStackAnchor, EventTransformFlags::AllAxes, {.x = yaw, .y = 0, .z = 16384});
    EV_AT_SetPos(bookStackAnchor, EventTransformFlags::AllAxes, anchor);
    EV_AT_SetAngY(bookModelState.stacks[bookModelState.currentStack + 1], EventTransformFlags::AllAxes, stackRotation);
}
void BookPickupCameraCallback(void *) {
    Vector3 position;
    const std::uint32_t yaw = SampleBookCarryTrack(bookInitialCarryTracks[bookModelState.currentStack], position, 0, 1);
    position.y = 127.42f;
    EV_AT_SetPos(bookCameraAnchor, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(bookCameraAnchor, EventTransformFlags::AllAxes, {.x = 0, .y = static_cast<std::int32_t>(yaw), .z = 0});
    bookDesiredCamera = 2005;
    bookCurrentCamera = 2005;
    EVS_LoopCamera(2005, bookCameraAnchor, bookCameraAnchor);
    HLib::DestroyCurrentTask();
}
void ResetBookRound() {
    EV_EnsureCharacterDraw(BOOK_SAI);
    EV_EnableCharacterDisplay(BOOK_SAI);
    if (bookModelState.currentStack != 13 && bookModelState.currentBook >= 2) {
        const BookCompanionPlacement &placement = bookCompanionPlacements[bookModelState.currentBook - 1];
        SetBookCompanionPose(placement.position, placement.yaw);
        mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0x1899, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(BOOK_SAI), 66, 0, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_SAI), 1);
        SetMotionFrameTime(scenFindMotionwork(BOOK_SAI), 20, MotionRegion::LowerBody);
        bookCompanionTarget = placement.position;
    }
    if (bookModelState.currentBook >= 2) {
        Vector3 hidden{.x = -1000, .y = -1000, .z = -1000};
        Vector3i rotation;
        constexpr std::uint16_t parts[]{13, 14, 15, 16, 17, 18, 19, 20};
        for (const std::uint16_t part : parts)
            EV_scenApplyPosOrRotToPartOf(
                bookModelState.placed[bookModelState.currentBook - 2], part + 152, &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
    }
    if (bookModelState.currentBook >= 3)
        UpdatePlacedBookStack(bookModelState.currentBook - 3, 2);
    shen_name actor = BOOK_FIRST;
    for (std::int32_t index = 0; index < 20; ++index, ++actor.name)
        SetScenarioImageModule(actor, bookModelTypes[bookModelState.types[bookModelState.currentStack][index]].model);
}
void FinishBookRound() {
    EV_AT_SetPos(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 0, .y = -10, .z = 0});
    EV_AT_SetAngY(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 16384});
    if (bookModelState.currentBook >= 2)
        UpdatePlacedBookStack(bookModelState.currentBook - 2, 1);
    const BookRoundPlacement &pose = roundPlacements[bookModelState.currentBook];
    SetBookActorPose(BOOK_RYO, pose.position, pose.yaw);
    const Vector3 &position = placedPositions[bookModelState.currentBook];
    CreatePlacedBookStack(position.x, position.z);
}
void BookLeaveShelf() {
    switch (bookEventPhase.phase) {
    case 0: {
        Vector3 position;
        EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        if (position.x > 329) {
            position.x = 329;
            EV_AT_SetPos(BOOK_RYO, EventTransformFlags::AllAxes, position);
            SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 71, 0, MotionRegion::LowerBody);
        }
        EVS_LoopCamera(2004, BOOK_RYO, BOOK_RYO);
        QueueBookDialogue(7, 0);
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0xf002, MotionRegion::LowerBody);
        HLib::SetupTaskDelay(70);
        EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        bookFaceControl.player = 10;
        bookFaceControl.companion = 1;
        bookCompanionControl = {.frame = 0, .state = 2, .side = static_cast<std::int16_t>(position.x <= 328.5f ? 0 : 1)};
        ++bookEventPhase.phase;
        return;
    }
    case 1:
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0xf024, MotionRegion::LowerBody);
        ++bookEventPhase.phase;
        return;
    case 2:
        if (!SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 65, 1, MotionRegion::LowerBody))
            return;
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0xf005, MotionRegion::LowerBody);
        EV_AT_SetAngY(BOOK_RYO, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
        ++bookEventPhase.phase;
        return;
    case 3:
        if (!SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 65, 1, MotionRegion::LowerBody))
            return;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
        ++bookEventPhase.phase;
        return;
    case 4: {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        const BookCompanionPlacement &placement = bookCompanionPlacements[bookModelState.currentBook < 2 ? 0 : bookModelState.currentBook - 1];
        SetBookCompanionPose(placement.position, placement.yaw);
        if (bookModelState.currentBook >= 2) {
            mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0x1899, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BOOK_SAI), 66, 0, MotionRegion::LowerBody);
            mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_SAI), 1);
            SetMotionFrameTime(scenFindMotionwork(BOOK_SAI), 20, MotionRegion::LowerBody);
        }
        bookCompanionTarget = placement.position;
        ChangeBookEventState(5);
        break;
    }
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
