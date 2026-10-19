#include "s2/areas/area_bok2.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/sap_event.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, npc{SHEN_NAME(KNK_)}, invalid{0xffffffffU}, token{1601200499};
constexpr std::uint32_t items[]{3017, 3020, 3018, 3019, 3014, 0};
std::int32_t handRest[]{1999, 65354, 62444, 1638, 180, 63900, 546, 64719, 3365, 0, 64810, 60984, 0, 64628, 61893, 0, 0, 64261, 0, 0, 64628, 0, 0, 60074, 0, 0,
    59710, 0, 65172, 63353, 0, 0, 65354, 0, 729, 61165, 0, 65173, 58073, 0, 65172, 64444, 0, 0, 0, 0, 547, 57161, 0, 1639, 61165, 0, 64810, 64080, 64080, 182,
    0};
std::int32_t handHold[]{0, 726, 60987, 0, 64445, 63535, 0, 62624, 2730, 0, 0, 60987, 0, 0, 62442, 0, 0, 64080, 0, 546, 1820, 0, 0, 60987, 0, 0, 60805, 0, 0,
    63535, 0, 546, 1456, 0, 0, 59167, 0, 0, 64263, 0, 182, 63171, 0, 546, 1273, 0, 0, 62261, 0, 0, 62079, 0, 546, 62989, 0, 727, 1091};
std::int32_t handBow[]{0, 0, 46606, 0, 2182, 64444, 0, 0, 1456, 0, 64080, 49154, 0, 908, 44239, 0, 0, 53340, 0, 0, 0, 0, 1456, 54796, 0, 0, 43692, 0, 727,
    50974, 0, 0, 0, 0, 2912, 54797, 0, 182, 44421, 0, 1456, 50245, 0, 0, 0, 2912, 910, 51336, 726, 726, 47517, 0, 2003, 50974, 65518, 65427, 0};
const Vector3i holdRotation{60987, 0, 60258}, bowRotation{56071, 364, 1638};
shen_name itemModel{1514231647}, localBase{};
std::uint32_t itemId{}, itemIndex{};
void Initialize_Callback(void *);
void Setup_Callback(void *);
void Prepare_Callback(void *);
void Start_Callback(void *);
void Walk_Callback(void *);
void Walk2_Callback(void *);
void Walk3_Callback(void *);
void EndFade_Callback(void *);
void End_Callback(void *);
void Pose(std::int32_t side, std::int32_t *data, std::int16_t ticks) {
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ryo), side, reinterpret_cast<std::byte *>(data), ticks);
}
void ResetMotion(shen_name name) {
    SetMotionRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    SetMotionFrameTime(scenFindMotionwork(name), 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(name), 0);
}

void Move(shen_name name, const Vector3 &pos, std::int32_t yaw) {
    SetMotionRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(name), 0);
    SetMotionFrameTime(scenFindMotionwork(name), 0, MotionRegion::LowerBody);
    EV_AT_SetPos(name, EventTransformFlags::AllAxes, pos);
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{0, yaw, 0});
}

void Restore() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_SetCharaControllerFlags(ryo, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(ryo, true);
    EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Enabled);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 1, 0);
    SetPlayerControlEnabled(1);
    EV_NPC_NewLoadSw(1);
    SetCharaKill(localBase, 1);
    EV_FreeAuthorPak(0, "0035");
}
void ItemWait_Callback(void *) {
    AddInventoryItem(itemId, 1, 1);
    HLib::DestroyCurrentTask();
}
void End_Callback(void *) {
    Restore();
    AT_SetLocalBaseChara(npc, invalid);
    AT_SetLocalBaseChara(ryo, invalid);
    Move(ryo, Vector3{335.85f, 126.97f, 275.56f}, 0x8000);
    if (sapEventData.packageName[0])
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(sapEventData.packageName), 0);
    for (auto *name = sapEventData.characters; name->name; ++name) {
        EV_npcFree(*name);
        EV_npcLockUnlock(*name);
    }
    if (sapEventData.authorBank[0])
        EV_FreeAuthorPak(0, sapEventData.authorBank);
    HLib::DestroyCurrentTask();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BOK2"), 0);
}
void EndFade_Callback(void *) {
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetCurrentTaskTo(End_Callback);
    HLib::SetupTaskDelay(30);
}

void Walk3_Callback(void *) {
    if (GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody) > 50) {
        EV_DisableCharacterDisplay(itemModel);
        EV_EnsureCharacterDraw(itemModel);
    }
    if (GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody) <= 325)
        return;
    EVT_AT_SetCam(1006, localBase, localBase, 0, 6, 0);
    EV_DisableCharacterDisplay(itemModel);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, Vector3i{0, 0x8000, 0});
    mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 1);
    SetMotionFrameTime(scenFindMotionwork(ryo), 20, MotionRegion::LowerBody);
    Pose(1, handRest, 0);
    ResetMotion(ryo);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf005, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ryo), .9f, MotionRegion::LowerBody);
    HLib::SetCurrentTaskTo(EndFade_Callback);
    HLib::SetupTaskDelay(100);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 1, 0);
}
void Walk2_Callback(void *) {
    ResetMotion(ryo);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0x18aa, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ryo), 1, MotionRegion::LowerBody);
    EVS_LoopCamera(1005, localBase, localBase);
    Pose(1, handBow, 30);
    Pose(0, handRest, 30);
    ApplyCharacterHandRotation(FindCharacterByScenarioName(ryo), 0, 21, &bowRotation);
    HLib::SetCurrentTaskTo(Walk3_Callback);
}
void Walk_Callback(void *) {
    SetMotionSpeed(scenFindMotionwork(ryo), 0, MotionRegion::LowerBody);
    EVS_LoopCamera(1004, localBase, localBase);
    HLib::SetCurrentTaskTo(Walk2_Callback);
    HLib::SetupTaskDelay(60);
}
void Start_Callback(void *) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BOK2"), 0);
    Move(ryo, Vector3{0, 0, .8f}, 0);
    Move(npc, Vector3{0, 0, -.8f}, 0x8000);
    mtwkRequestMotion(scenFindMotionwork(npc), 0x18ab, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(npc), 28, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(npc), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(npc), 17, 0);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0x18ac, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(ryo), 80, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ryo), 1, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
    EVS_LoopCamera(1002, localBase, localBase);
    HLib::SetCurrentTaskTo(Walk_Callback);
    HLib::SetupTaskDelay(126);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(ItemWait_Callback, 4, 0x0b, static_cast<std::int32_t>(token.name));
    HLib::SetupTimedTask(task, 120);
}
void Prepare_Callback(void *) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BOK2"), 0);
    EV_EnableCharacterDisplay(itemModel);
    Pose(0, handHold, 10);
    AT_SetLocalBaseChara(ryo, localBase);
    AT_SetLocalBaseChara(npc, localBase);
    Move(ryo, Vector3{0, 0, .8f}, 0);
    Move(npc, Vector3{0, 0, -.8f}, 0x8000);
    ResetMotion(npc);
    mtwkRequestMotion(scenFindMotionwork(npc), 0x1897, MotionRegion::LowerBody);
    ResetMotion(ryo);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0x1897, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ryo), 1, MotionRegion::LowerBody);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    EVT_AT_SetCam(1001, localBase, localBase, 0, 6, 0);
    constexpr const char *voices[]{"0035C011", "0035C012", "0035A056", "0035C013", "0035A057", "0035C014", "0035C015", "0035A058"}; // @text 0035C011 EN: "You're doing what Master Tao\ninstructed you to do." | JA: "老師の言うとおり、\nやっているようだな。" ; 0035C012 EN: "That is a good sign." | JA: "良い心がけだ。" ; 0035A056 EN: "Yes." | JA: "はい…" ; 0035C013 EN: "Here." | JA: "これを。" ; 0035A057 EN: "What is this?" | JA: "これは？" ; 0035C014 EN: "A book of moves." | JA: "技書だ。" ; 0035C015 EN: "Use it to practice." | JA: "…修行の励みにしなさい。" ; 0035A058 EN: "Thank you." | JA: "ありがとうございます。"
    constexpr int frames[]{0, 102, 154, 185, 217, 248, 292, 479};
    for (int i = 0; i < 8; ++i)
        EVT_AT_SetVoice(voices[i], 1, frames[i]);
    HLib::SetCurrentTaskTo(Start_Callback);
    HLib::SetupTaskDelay(185);
}
void Setup_Callback(void *) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BOK2"), 0);
    EVS_LoopCamera(1000, invalid, invalid);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Move(ryo, Vector3{330.8f, 126.97f, 266.38f}, 41870);
    Move(npc, Vector3{335.9f, 126.97f, 268.4f}, 14563);
    ResetMotion(npc);
    mtwkRequestMotion(scenFindMotionwork(npc), 0xf03f, MotionRegion::LowerBody);
    ResetMotion(ryo);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf005, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ryo), .9f, MotionRegion::LowerBody);
    Vector3 position;
    EV_GetCharaPosition(npc, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::TurnMotion);
    position.y += 1.6f;
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &position, 0);
    HLib::SetCurrentTaskTo(Prepare_Callback);
    HLib::SetupTaskDelay(90);
}
void Initialize_Callback(void *) {
    SetLowQualityHands(FindCharacterByScenarioName(ryo));
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    ApplyCharacterHandRotation(FindCharacterByScenarioName(ryo), 0, 21, &holdRotation);
    constexpr shen_name loaded[]{npc, shen_name{0}};
    InitializeSapAuthorEvent("", 0, loaded, Setup_Callback);
}
} // namespace
void AreaBOK2Init() {
    itemModel = shen_name{1514231647};
    localBase = shen_name{};
    itemId = 0;
    itemIndex = 0;
}

void AreaBOK2Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0035");
    localBase = CreateEventCharacter(CharacterCreationHighModel, nullptr, shen_name{});
    EV_AT_SetAngY(localBase, EventTransformFlags::AllAxes, Vector3i{});
    EV_AT_SetPos(localBase, EventTransformFlags::AllAxes, Vector3{335.8f, 126.97f, 268.5f});
    itemId = 0;
    itemIndex = 0;
    std::uint32_t index = 0;
    while (items[index] && GetItemState(items[index]))
        ++index;
    if (items[index]) {
        itemIndex = index;
        itemId = items[index];
    }
    if (itemId) {
        itemModel = shen_name{((itemIndex + 48) << 24) | 0x5a4157};
        Pose(1, handRest, 10);
        Pose(0, handRest, 10);
        EV_SetCharaControllerFlags(itemModel, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
        EV_DisableCharacterDisplay(itemModel);
        EV_EnsureCharacterDraw(itemModel);
        const Vector3 offset{.214f, -.038f, -.043f};
        const Vector3i rotation{46785, 2366, 51556};
        AttachScenarioCharacter(itemModel, ryo, 14, &offset, &rotation);
        HLib::EnqueueTaskWithoutParameter(Initialize_Callback, 4, 0x0b, static_cast<std::int32_t>(token.name));
    } else {
        Restore();
        Move(ryo, Vector3{335.85f, 126.97f, 275.56f}, 0x8000);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BOK2"), 0);
    }
}
const ScriptAreaDefinition areaBOK2ScriptDefinition{.areaId = SHEN_NAME(BOK2), .field04 = 0, .init = AreaBOK2Init, .main = AreaBOK2Main};
} // namespace shenmue::s2::areas
