#include "s2/scripts/cwon/area_cwon.hpp"
#include "s2/scripts/cwon/cwon_helpers.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <bit>
#include <cmath>
#include <initializer_list>

namespace shenmue::s2 {
static_assert(sizeof(CwonEventWork) == 0xc8 && offsetof(CwonEventWork, routeState) == 0x2c && offsetof(CwonEventWork, battleResult) == 0x30 &&
    offsetof(CwonEventWork, followHeight) == 0xc0);
namespace {
constexpr shen_name sam{SHEN_NAME(SAM_)}, lly{SHEN_NAME(LLY_)}, clj{SHEN_NAME(CLJ_)}, base{SHEN_NAME(BASE)}, bag{SHEN_NAME(a014)}, club{SHEN_NAME(KEIB)};
constexpr shen_name fighters[]{CWON_RYO, sam, lly};
constexpr shen_name authored[]{CWON_RYO, CWON_WON, sam, lly};
constexpr shen_name streetProps[]{SHEN_NAME(DRA0), SHEN_NAME(DRA1), SHEN_NAME(DRA2), shen_name{}};
constexpr shen_name boxes[]{SHEN_NAME(BHK0), SHEN_NAME(BHK1), SHEN_NAME(BHK2), SHEN_NAME(BHK3), shen_name{}};
constexpr shen_name brokenBoxes[]{SHEN_NAME(bHK0), SHEN_NAME(bHK1), SHEN_NAME(bHK2), SHEN_NAME(bHK3), shen_name{}};
constexpr shen_name tables[]{SHEN_NAME(TBLa), SHEN_NAME(TBLb), SHEN_NAME(TBLc), SHEN_NAME(TBLd), SHEN_NAME(TBLe), shen_name{}};
constexpr shen_name brokenTables[]{SHEN_NAME(bTK0), SHEN_NAME(bTK1), SHEN_NAME(bTK2), SHEN_NAME(bTK3), SHEN_NAME(bTK4), shen_name{}};
constexpr shen_name marketProps[]{SHEN_NAME(IS_1), SHEN_NAME(IS_2), SHEN_NAME(IS_3), SHEN_NAME(IS_4), SHEN_NAME(IS_5), SHEN_NAME(IS_6), SHEN_NAME(IS_7),
    SHEN_NAME(IS_8), SHEN_NAME(IS_9), SHEN_NAME(IS10), SHEN_NAME(IS11), SHEN_NAME(IS12), SHEN_NAME(IS16), SHEN_NAME(IS17), SHEN_NAME(IS18), SHEN_NAME(IS19),
    SHEN_NAME(IS20), SHEN_NAME(IS21), SHEN_NAME(IS22), SHEN_NAME(IS23), shen_name{}};
constexpr shen_name streetAuthorCharacters[]{CWON_RYO, CWON_WON, clj, sam, lly, shen_name{}};
constexpr shen_name marketAuthorCharacters[]{CWON_RYO, CWON_WON, sam, lly, shen_name{}};
constexpr Vector3 marketMachinePosition{257.70001220703125f, 101, 397.1000061035156f};
constexpr Vector3 marketLostPosition{260, 101, 397.5f}, warehouseLostPosition{220, 99.31999969482422f, 400};
constexpr Vector3 marketReturnPosition{258.3999938964844f, 101, 397.6000061035156f};
constexpr Vector3 warehouseFightPositions[]{{220.83999633789062f, 99.32499694824219f, 401.5799865722656f},
    {219.02000427246094f, 99.32499694824219f, 398.54998779296875f}, {218.1199951171875f, 99.32499694824219f, 400.239990234375f}};
void BattleWin(void *);
void BattleAuthorMarket(void *);
void BattleAuthorWarehouse(void *);
float FrameTicks() {
    return g_stabilizeFps ? g_stabilizedFrameTime * 30.0f : 1.0f;
}
void Change(HLib::TaskCallback callback) {
    HLib::SetCurrentTaskTo(callback);
}
void Place(shen_name name, shen_name parent, const Vector3 &position) {
    EventSetPosition request{0, name, parent, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Turn(shen_name name, shen_name parent, float degrees) {
    EventSetAngle request{0, name, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Motion(shen_name name, std::uint32_t id, float frame, float speed, std::uint32_t flags) {
    EventSetMotion request{0, name, id, frame, speed, flags, 0};
    EVT_AT_SetMot_Callback(&request);
}
void Camera(std::int32_t id, shen_name anchor) {
    EventSetCamera request{0, id, anchor, anchor, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void Fade(std::uint32_t ticks, bool out) {
    SetScreenFilmFade(ticks, out ? 0 : 255, 0, 0, 0, out ? 255 : 0, 0, 0, 0);
}
void SetHands(shen_name name, std::int8_t enabled) {
    SetHighModelXYZ(ctrlGetCharaByName(name), enabled, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(name), enabled, 1, 0);
}
void SetAuthorControl(std::int16_t enabled) {
    for (auto name : authored)
        if (DoesScenarioCharacterExist(name)) {
            SetPluralControlFor(ctrlGetCharaByName(name), enabled);
            SetScenarioCharacterType(name, CharacterTypeDefault);
        }
}
void ShowBag() {
    EV_EnsureCharacterDraw(bag);
    EV_EnableCharacterDisplay(bag);
    CwonDisableGravity(bag);
}
void HideList(const shen_name *names) {
    for (; names->name; ++names) {
        EV_ToggleCharacterFlag(*names);
        EV_DisableCharacterDisplay(*names);
    }
}
void EnsurePackage(const char *name) {
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0)) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(name));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
}
void LoadBattleAudio(CwonEventWork *work, std::uint8_t state, std::int32_t scene) {
    if (work->audioScene != state) {
        EV_LoadCutsceneFromCurrentScene("0012E011.smp");
        EV_LoadAudio("0012E011.sbt", scene);
        EV_spmBeginSampleExecute();
        work->audioScene = state;
    }
}
void InitializeFighters() {
    shenInitFightData(ctrlGetCharaByName(CWON_RYO), nullptr, 0, 0, 0, 0);
    shenInitFightData(ctrlGetCharaByName(sam), nullptr, 1, 1, 120, 11);
    shenInitFightData(ctrlGetCharaByName(lly), nullptr, 1, 1, 150, 12);
}
void ClearBattleMotionFlagsAndFaces() {
    for (auto name : fighters)
        EV_vfBattleUtil(FightBattleUtilOp::MotionOrSlotFlag, name, 0);
    for (auto name : fighters)
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(name), FacialExpressionId::Angry, 1, 1);
}
void QueueLoadingFade() {
    CwonQueueLoadingTitle(SHEN_NAME(AR02), 90);
    HLib::SetupTaskDelay(90);
}

void InitializeClub(std::uint32_t slot) {
    EV_EnsureCharacterDraw(club);
    EV_EnableCharacterDisplay(club);
    AllocateVOBJ(3, 4);
    vfStageDefinitionAddHitbox(3, 2, 1, 0, 0, 0, 0.3f);
    vfStageDefinitionAddHitbox(3, 2, 1, -0.3f, 0, 0, 0.3f);
    vfStageDefinitionAddHitbox(3, 2, 1, -0.6f, 0, 0, 0.3f);
    ToggleVOBJArrayIndex(3, 15, -1, 0, 0);
    vfStageDefinitionSetHitResponse(3, 0, 0, 2560, 0, 0, 1, 0.01f);
    vfStageDefinitionSetAttachment(3, 14, 0.05f, 0, -0.05f, 0, 0, std::bit_cast<std::uint32_t>(61440.0f));
    vfAllocateVOBJ(slot, ctrlGetCharaByName(club), 3);
    vfStageObjectAssignMotFlags(slot, 0x405000);
    vfStageObjectReset(slot, ctrlGetCharaByName(sam));
    const Vector3 scale{2, 0.5f, 2};
    SetMotionworkFlagForCharaById(club, &scale);
    EV_DisableCharacterDisplay(club);
}

void InitializeStreetObjects() {
    AllocateVOBJ(0, 2);
    vfStageDefinitionAddHitbox(0, 2, 1, 0.2f, 0, 0, 0.45f);
    vfStageDefinitionAddHitbox(0, 2, 1, -0.2f, 0, 0, 0.45f);
    ToggleVOBJArrayIndex(0, 15, -1, 14, 532);
    vfStageDefinitionSetHitResponse(0, 532, 3075, 64, 0, 29127, 20, 0.4f);
    vfStageDefinitionSetAttachment(0, 14, -0.25f, 0.43f, 0, 0, 0, 0);
    AllocateVOBJ(1, 1);
    vfStageDefinitionAddHitbox(1, 2, 1, 0, 0.3f, 0, 0.5f);
    vfStageDefinitionSetMotion(1, SHEN_NAME(BOX1), 38, nullptr, 2, 2, 169, 118);
    AllocateVOBJ(2, 1);
    vfStageDefinitionAddHitbox(2, 2, 1, 0, 0.3f, 0, 0.5f);
    vfStageDefinitionSetMotion(2, SHEN_NAME(BOX2), 38, nullptr, 2, 2, 103, 118);
    std::uint32_t slot = 0;
    for (const shen_name *name = streetProps; name->name; ++name, ++slot) {
        vfAllocateVOBJ(slot, ctrlGetCharaByName(*name), 0);
        vfStageObjectAssignMotFlags(slot, 3);
        vfStageObjectAssignMotFlags(slot, 0xb01000);
    }
    for (std::uint32_t i = 0; boxes[i].name; ++i, ++slot) {
        vfAllocateVOBJ(slot, ctrlGetCharaByName(boxes[i]), (i & 1) + 1);
        vfStageObjectSetBrokenModel(slot, ctrlGetCharaByName(brokenBoxes[i]), 720);
    }
    InitializeClub(slot + 1);
    vfInitializeStageObjectHitboxes();
}

void InitializeMarketObjects() {
    AllocateVOBJ(0, 1);
    vfStageDefinitionAddHitbox(0, 2, 1, 0, 0.3f, 0, 0.6f);
    vfStageDefinitionAddHitbox(0, 2, 0x8001, 0.9f, 0.5f, 0, 0.5f);
    vfStageDefinitionSetMotion(0, SHEN_NAME(TUKU), 17, nullptr, 2, 2, 299, 117);
    std::uint32_t slot = 0;
    for (std::uint32_t i = 0; tables[i].name; ++i, ++slot) {
        vfAllocateVOBJ(slot, ctrlGetCharaByName(tables[i]), 0);
        vfStageObjectSetBrokenModel(slot, ctrlGetCharaByName(brokenTables[i]), 720);
    }
    AllocateVOBJ(1, 7);
    vfStageDefinitionAddHitbox(1, 2, 1, 0, 0.2f, 0, 0.4f);
    vfStageDefinitionSetHitResponse(1, 2916, 2914, 0x60000, 8192, 0, 17, 0);
    ToggleVOBJArrayIndex(1, 10, -1, 14, 2913);
    for (const shen_name *name = marketProps; name->name; ++name, ++slot) {
        vfAllocateVOBJ(slot, ctrlGetCharaByName(*name), 1);
        vfStageObjectAssignMotFlags(slot, 3);
        vfStageObjectAssignMotFlags(slot, 4096);
    }
    InitializeClub(slot + 1);
    vfInitializeStageObjectHitboxes();
}

void PlaceStreetProps() {
    struct Placement {
        shen_name character;
        Vector3 position;
        float yaw;
    };
    constexpr Placement entries[]{{SHEN_NAME(DRA0), {234, 98.7300033569336f, 469.3500061035156f}, 0},
        {SHEN_NAME(DRA1), {236.60000610351562f, 98.72000122070312f, 469.0899963378906f}, 0},
        {SHEN_NAME(DRA2), {242.39999389648438f, 98.72000122070312f, 466.6499938964844f}, 90},
        {SHEN_NAME(BHK0), {229.6999969482422f, 98.42500305175781f, 463.75f}, 0},
        {SHEN_NAME(BHK1), {242.39999389648438f, 98.32499694824219f, 464.95001220703125f}, 295},
        {SHEN_NAME(BHK2), {239.6999969482422f, 98.32499694824219f, 470.3500061035156f}, 335},
        {SHEN_NAME(BHK3), {237.1999969482422f, 98.42500305175781f, 470.25f}, 0}};
    for (const auto &entry : entries)
        CwonSetPositionAnglesDegrees(entry.character, entry.position, {0, entry.yaw, 0});
}
void InitializeBattle(CwonEventWork *work) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("BATLPACK"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    shenStartFGST(10, 10, 0, 0);
    if (work->entry <= 6) {
        PlaceStreetProps();
        InitializeStreetObjects();
        vfCopyPosFromEntity(0, nullptr, 238, 466, 6);
        InitializeFighters();
        CwonRestoreBattlePoses(work);
        Change(CwonBattleCallback);
    } else if (work->entry == 7) {
        InitializeMarketObjects();
        for (const shen_name *name = tables; name->name; ++name)
            ToggleFlagOnFieldEntity(*name, 0);
        flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 1);
        flddSetPointFlagOnGrid(SHEN_NAME(PROP), 0);
        flddSetPointFlagOnGrid(SHEN_NAME(0000), 0);
        Place(base, base, {258.20001220703125f, 101, 398.8999938964844f});
        Turn(base, base, 90);
        Place(SHEN_NAME(IS_6), CWON_INVALID, marketMachinePosition);
        Place(SHEN_NAME(IS_8), base, {});
        for (auto name : fighters)
            AT_SetLocalBaseChara(name, base);
        Change(CwonBattleAuthorCallback);
    } else {
        Place(base, base, {204.5f, 98.875f, std::bit_cast<float>(1137007002U)});
        Turn(base, base, 270);
        for (auto name : fighters)
            AT_SetLocalBaseChara(name, base);
        Change(CwonBattleAuthorCallback);
        InitializeClub(0);
        vfInitializeStageObjectHitboxes();
    }
    EVS_LoopCamera(0, shen_name{}, shen_name{});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{2}, 1);
}
std::int32_t UpdateStreetWin(CwonEventWork *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (!EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame))
            ShowBag();
        return 0;
    }
    EV_ToggleCharacterFlag(bag);
    EV_DisableCharacterDisplay(bag);
    return 1;
}
std::int32_t UpdateStreetLost() {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return 1;
    ShowBag();
    return 0;
}
std::int32_t UpdateMarketWin(CwonEventWork *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return 1;
    const int frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    switch (frame) {
    case 1: {
        EV_DisableCharacterDisplay(CWON_WON);
        DateTime date;
        GetCurrentDateTime(date);
        if (date.hour < 19)
            return 0;
        SetHands(CWON_RYO, 0);
        SetHands(CWON_WON, 0);
        SetHands(sam, 0);
        SetHands(lly, 0);
        break;
    }
    case 75:
        SetHands(lly, 0);
        break;
    case 190:
        SetHighModelXYZ(ctrlGetCharaByName(sam), 0, 0, 0);
        break;
    case 211:
        SetHighModelXYZ(ctrlGetCharaByName(CWON_RYO), 0, 1, 0);
        EV_DisableCharacterDisplay(lly);
        break;
    case 215:
        SetHighModelXYZ(ctrlGetCharaByName(sam), 0, 1, 0);
        break;
    case 225:
        SetHighModelXYZ(ctrlGetCharaByName(CWON_RYO), 0, 0, 0);
        break;
    case 310:
        EV_EnableCharacterDisplay(CWON_WON);
        break;
    case 636:
        EV_EnableCharacterDisplay(lly);
        break;
    case 674:
        SetHighModelXYZ(ctrlGetCharaByName(CWON_WON), 0, 0, 0);
        break;
    case 688:
        SetHighModelXYZ(ctrlGetCharaByName(CWON_WON), 0, 1, 0);
        break;
    default:
        break;
    }
    return 0;
}

std::int32_t UpdateWarehouseWin(CwonEventWork *work) {
    return UpdateMarketWin(work);
}
void BattleAuthorMarket(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    switch (work->authorState) {
    case 0:
        for (auto name : fighters)
            CwonDisableGravity(name);
        Camera(1001, base);
        EVT_AT_SetCam(1002, base, base, 0, 0, 116);
        EVT_AT_SetCam(1003, base, base, 0, 0, 136);
        EVT_AT_SetCam(1004, base, base, 0, 0, 296);
        EVT_AT_ObjectDisp_1(tables[1], 140, 1);
        EVT_AT_ObjectDisp_1(tables[1], 296, 0);
        Place(CWON_RYO, base, {2.410330057144165f, 0, 3.134819984436035f});
        Turn(CWON_RYO, base, 0);
        Place(sam, base, {});
        Turn(sam, base, 0);
        Place(lly, base, {1.8314100503921509f, 0, -0.49199000000953674f});
        Turn(lly, base, 0);
        Motion(CWON_RYO, 0x1005, 0, 1, 0x44000000);
        Motion(sam, 0x1007, 0, 0, 0x44000000);
        Motion(lly, 0x1001, 0, 0, 0x44000000);
        EVT_AT_SetMot(sam, 0x1007, 0, 1, 0x44000000, 62);
        EVT_AT_SetMot(lly, 0x1001, 0, 1, 0x44000000, 30);
        EVT_AT_SetVoice("0012C022", 1, 105); // @text EN: " " | JA: "なんだぁ?"
        EVT_AT_SetVoice("0012C027", 1, 140); // @text EN: "What do you want?" | JA: "…てめぇ、なんの用だ。"
        EVT_AT_SetVoice("0012A006", 1, 185); // @text EN: "Now,\nwhere's my bag?" | JA: "バッグを返してもらおう！"
        EVT_AT_SetVoice("0012C013", 1, 250); // @text EN: "Get lost!" | JA: "知らねえな。"
        EVT_AT_SetVoice("0012A012", 1, 285); // @text EN: " " | JA: "ふんっ！"
        EVT_AT_SetVoice("0012C014", 1, 300); // @text EN: " " | JA: "いでっ！"
        EVT_AT_SetVoice("0012D009", 1, 330); // @text EN: "You!" | JA: "てめぇ！"
        work->authorState = 10;
        work->followHeight = 440;
        break;
    case 1:
        work->authorState = 2;
        Fade(30, true);
        spmFadeOut();
        HLib::SetupTaskDelay(30);
        break;
    case 2:
        work->authorState = 0;
        for (auto name : fighters)
            AT_SetLocalBaseChara(name, CWON_INVALID);
        vfCopyPosFromEntity(0, nullptr, 260.3999938964844f, 399.1000061035156f, 5);
        InitializeFighters();
        Fade(30, false);
        CwonRestoreBattlePoses(work);
        Change(CwonBattleCallback);
        break;
    case 10:
        work->followHeight -= FrameTicks();
        if (work->followHeight <= 0)
            work->authorState = 1;
        break;
    default:
        break;
    }
}
void BattleAuthorWarehouse(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    switch (work->authorState) {
    case 0:
        Place(sam, base, {-0.699999988079071f, 0, 0});
        Turn(sam, base, 270);
        Place(CWON_RYO, sam, {-3, 0.23f, -0.7765600085258484f});
        Turn(CWON_RYO, sam, 0);
        Place(lly, sam, {0, 0, -1.3559999465942383f});
        Turn(lly, sam, 0);
        Motion(CWON_RYO, 0x1006, 160, 1, 0x40000000);
        Motion(sam, 0x1009, 0, 1, 0x40000000);
        Motion(lly, 0x1003, 0, 1, 0x40000000);
        EV_PlayDialogue("0012C021");
        EVT_AT_SetVoice("0012A017", 1, 185); // @text EN: "I want my bag back." | JA: "バッグを返してもらおう。"
        EVT_AT_SetVoice("0012D018", 1, 260); // @text EN: "Oh hell,\nhe hasn't learned his lesson yet." | JA: "やれやれ…\nまだ、わかんねぇようだなぁ…"
        EVT_AT_SetVoice("0012C024", 1, 400); // @text EN: "You're getting on my nerves!" | JA: "しつこい野郎だぜっ！！"
        Camera(2001, base);
        EVT_AT_SetCam(2003, base, base, 0, 0, 190);
        EVT_AT_SetCam(2004, base, base, 0, 0, 250);
        work->authorState = 10;
        work->followHeight = 518;
        break;
    case 1:
        work->authorState = 2;
        Fade(30, true);
        spmFadeOut();
        HLib::SetupTaskDelay(30);
        break;
    case 2:
        work->authorState = 0;
        for (auto name : fighters)
            AT_SetLocalBaseChara(name, CWON_INVALID);
        for (int i = 0; i < 3; ++i)
            Place(fighters[i], CWON_INVALID, warehouseFightPositions[i]);
        camCtrlSetState(CameraState::Event);
        camCtrlSetNextPosition({224.88999938964844f, 102.58000183105469f, 401.8599853515625f});
        camCtrlSetTargetPosition({220.44000244140625f, 100.2699966430664f, 401.2099914550781f});
        vfCopyPosFromEntity(0, nullptr, 220.2100067138672f, 398.6099853515625f, 6);
        InitializeFighters();
        Fade(30, false);
        CwonRestoreBattlePoses(work);
        Change(CwonBattleCallback);
        break;
    case 10:
        work->followHeight -= FrameTicks();
        if (work->followHeight <= 0)
            work->authorState = 1;
        break;
    default:
        break;
    }
}

void BattleWin(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (!work->authorState) {
        if (filmCheckFadeTrackColour())
            return;
        EV_ToggleCharacterFlag(club);
        EV_DisableCharacterDisplay(club);
        EnsurePackage("CWONHAND");
        ++work->authorState;
        SetAuthorControl(1);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->routeState == 5) {
            HideList(streetProps);
            CwonStartAuthor(3, nullptr, 0, streetAuthorCharacters, nullptr);
        } else if (work->routeState == 7) {
            CwonRestoreMarketProps(2);
            Place(SHEN_NAME(IS_6), CWON_INVALID, marketReturnPosition);
            constexpr shen_name hidden[]{SHEN_NAME(IS_5), SHEN_NAME(IS_8), SHEN_NAME(IS_9), shen_name{}};
            HideList(hidden);
            CwonStartAuthor(6, nullptr, 0, marketAuthorCharacters, nullptr);
        } else if (work->routeState == 8)
            CwonStartAuthor(7, nullptr, 0, marketAuthorCharacters, nullptr);
        ClearBattleMotionFlagsAndFaces();
    }
    if (work->authorState == 1) {
        using Update = std::int32_t (*)(CwonEventWork *);
        constexpr Update updates[]{UpdateStreetWin, nullptr, UpdateWarehouseWin, UpdateMarketWin};
        if (updates[work->routeState - 5](work))
            ++work->authorState;
    } else if (work->authorState == 2) {
        work->temporarySlot = 0;
        work->failures = 1;
        work->routeChoice = 74;
        work->nextArea = SHEN_NAME(AR02);
        SetScenarioFollowCharacter(CWON_WON);
        SetAuthorControl(0);
        work->authorState = 0;
        Change(CwonEndChaseCallback);
    }
}
} // namespace


void CwonInitializeBattleCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (work->authorState == -1)
        work->authorState = 0;
    else if (work->authorState != 0) {
        if (work->authorState == 1) {
            SetHands(sam, 1);
            SetHands(lly, 1);
            CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
            if (work->entry >= 6) {
                work->authorState = 0;
                LoadBattleAudio(work, 4, 1);
                InitializeBattle(work);
                Fade(20, false);
                EV_EnsureCharacterDraw(sam);
                EV_EnableCharacterDisplay(sam);
                EV_EnsureCharacterDraw(lly);
                EV_EnableCharacterDisplay(lly);
            } else {
                Place(CWON_RYO, CWON_RYO, {});
                Turn(CWON_RYO, CWON_RYO, 315);
                Place(CWON_WON, CWON_WON, {});
                Turn(CWON_WON, CWON_WON, 180);
                Place(clj, clj, {});
                Turn(clj, clj, 180);
                Place(sam, sam, {});
                Turn(sam, sam, 150);
                Place(lly, lly, {1, 0, 1});
                Turn(lly, lly, 225);
                EVT_AT_SetMot(CWON_RYO, 0x26c3, 150, 1, 0x44000000, 0);
                EVT_AT_SetMot(sam, 0x26c4, 150, 1, 0x44000000, 0);
                EVT_AT_SetMot(lly, 0x26c2, 150, 1, 0x44000000, 0);
                mtwkRequestMotion(scenFindMotionwork(CWON_WON), 0x26c5, MotionRegion::LowerBody);
                mtwkRequestMotion(scenFindMotionwork(clj), 0x26c1, MotionRegion::LowerBody);
                EVT_AT_SetVoice("0012A011", 1, 0); // @text EN: "I want my bag back." | JA: "バッグを返してもらおう。"
                EVT_AT_SetVoice("0012C017", 1, 35); // @text EN: "Why don't you give up?" | JA: "あきらめなっ！"
                EVT_AT_SetVoice("0012D011", 1, 65); // @text EN: "So, you love your bag that much, eh?" | JA: "そんなにバッグが大事かよっ！"
                EV_EnsureCharacterDraw(bag);
                EV_EnableCharacterDisplay(bag);
                constexpr Vector3 attachment{0.18000000715255737f, 0, -0.09000000357627869f};
                constexpr Vector3i rotation{33496, 0, 8192};
                AttachScenarioCharacter(bag, clj, 14, &attachment, &rotation);
                Camera(7000, CWON_INVALID);
                EVT_AT_SetCam(7001, CWON_INVALID, CWON_INVALID, 0, 0, 35);
                EVT_AT_MenCtrl({1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 120});
                work->authorState = 150;
                work->routeState = 0;
            }
            for (auto name : fighters)
                CwonDisableGravity(name);
        } else if (work->authorState == 2) {
            InitializeBattle(work);
            work->authorState = 0;
        } else {
            float fraction = std::bit_cast<float>(work->routeState) + FrameTicks();
            work->routeState = std::bit_cast<std::int32_t>(fraction);
            if (fraction >= 1) {
                --work->authorState;
                work->routeState = std::bit_cast<std::int32_t>(fraction - 1);
            }
        }
        return;
    }
    if (work->entry >= 7)
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("CWONBATL"), 0);
    ++work->authorState;
}
void CwonBattleAuthorCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (work->entry == 7)
        BattleAuthorMarket(data);
    else
        BattleAuthorWarehouse(data);
}

void CwonRestoreBattlePoses(CwonEventWork *work) {
    for (auto name : fighters)
        SetMotionRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(CWON_RYO), 0x132, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(sam), 0x630, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(lly), 0x610, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(CWON_RYO), 69, 0, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(sam), 69, 0, MotionRegion::LowerBody);
    constexpr float angles[3][3]{{278.29998779296875f, 132.60000610351562f, 98.30000305175781f}, {1.7000000476837158f, 181.6999969482422f, 215.39999389648438f},
        {63.400001525878906f, 211, 243.5f}};
    const int index = work->entry == 7 ? 1 : work->entry == 8 ? 2 : 0;
    for (int i = 0; i < 3; ++i) {
        Vector3i rotation{0, static_cast<std::uint16_t>(static_cast<std::int32_t>((angles[index][i] * 8192.0f) / 45.0f)), 0};
        EV_AT_SetAngY(fighters[i], EventTransformFlags::AllAxes, rotation);
    }
}

void CwonRestoreBattleProps(CwonEventWork *work, std::int32_t restore) {
    const shen_name *const streetLists[]{streetProps, boxes, brokenBoxes, nullptr};
    const shen_name *const marketLists[]{tables, brokenTables, nullptr};
    const shen_name *const *lists = work->entry <= 6 ? streetLists : marketLists;
    if (work->entry > 6 && work->entry != 7)
        return;
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    for (; *lists; ++lists)
        for (const shen_name *name = *lists; name->name; ++name)
            if (ctrlGetCharaByName(*name)) {
                float alpha = 1;
                if (restore != 1) {
                    Vector3 position;
                    EV_GetCharaPosition(*name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
                    const float x = camera.x - position.x, y = camera.y - position.y, z = camera.z - position.z;
                    alpha = std::fmax(0.0f, (std::sqrt((x * x + y * y) + z * z) - 1.0f) * 0.5f);
                }
                if (restore == 1 || alpha >= 1) {
                    EV_SetCharacterAlphaEnabled(*name, false);
                    alpha = 1;
                } else
                    EV_SetCharacterAlphaEnabled(*name, true);
                SetScenarioCharacterTransparency(*name, alpha);
            }
}

void CwonBattleCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (work->entry > 6) {
        CwonHideAtOrigin(CWON_WON);
        CwonHideAtOrigin(clj);
    }
    for (auto name : fighters)
        EV_vfBattleUtil(FightBattleUtilOp::MotionOrSlotFlag, name, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    vfmQueueFightStart(3, 99, 1, 0, -1);
    if (work->entry == 8) {
        Vector3 discarded;
        EV_GetCharaPosition(CWON_RYO, CHARACTER_TRANSLATION_ROOT, &discarded, EventTransformFlags::Local);
    }
    SetAuthorControl(1);
    for (auto name : authored)
        SetHands(name, 0);
    if (work->entry <= 6)
        SetHands(clj, 0);
    LoadBattleAudio(work, 3, 0);
    Change(CwonBattleEndCallback);
    if (work->entry <= 6)
        Fade(30, false);
}

void CwonBattleEndCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (work->authorState)
        return;
    if (IsFightActive()) {
        CwonRestoreBattleProps(work, 0);
        if (work->entry <= 6) {
            Vector3 position;
            EV_GetCharaPosition(CWON_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            position.y += 1.6f;
            for (auto name : {clj, CWON_WON}) {
                UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 3);
                UpdateMotionWorkWithPosition(scenFindMotionwork(name), &position, 1);
            }
        }
        return;
    }
    CwonRestoreBattleProps(work, 1);
    if (work->entry <= 6)
        for (auto name : {clj, CWON_WON})
            UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 0);
    if (work->entry == 7) {
        flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 0);
        flddSetPointFlagOnGrid(SHEN_NAME(PROP), 1);
        flddSetPointFlagOnGrid(SHEN_NAME(0000), 1);
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BATLPACK"), 0);
    if (DoesScenarioCharacterExist(bag))
        ResetScenarioCharacterAttachment(bag);
    SetScenarioCharacterKillMode(base, 1);
    if (GetBtlStatus(ctrlGetCharaByName(CWON_RYO)) == 3) {
        work->battleResult = 1;
        work->routeState = work->entry == 7 ? 7 : work->entry == 8 ? 8 : 5;
        if (work->entry != 7 && work->entry != 8)
            EV_stepSetFlags(0, 12);
        Fade(30, true);
        Change(BattleWin);
    } else {
        work->battleResult = 0;
        work->routeState = work->entry == 7 ? 10 : work->entry == 8 ? 11 : 9;
        if (work->entry != 7 && work->entry != 8)
            EV_stepSetFlags(1, 12);
        Fade(30, true);
        Change(CwonBattleLostCallback);
    }
}

void CwonBattleLostCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    switch (work->authorState) {
    case 0:
        if (filmCheckFadeTrackColour())
            return;
        EV_ToggleCharacterFlag(club);
        EV_DisableCharacterDisplay(club);
        if (work->routeState != 9) {
            work->authorState = 1;
            EV_DisableCharacterDisplay(sam);
            EV_DisableCharacterDisplay(lly);
            EV_DisableCharacterDisplay(CWON_WON);
            LoadBattleAudio(work, 5, 2);
            Change(CwonBattleLostChoiceCallback);
            return;
        }
        ++work->authorState;
        HideList(streetProps);
        EnsurePackage("CWONHAND");
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        CwonStartAuthor(4, nullptr, 0, streetAuthorCharacters, nullptr);
        UpdateStreetLost();
        ClearBattleMotionFlagsAndFaces();
        break;
    case 1:
        if (UpdateStreetLost())
            ++work->authorState;
        break;
    case 2:
        work->authorState = 3;
        QueueLoadingFade();
        break;
    case 3:
        work->authorState = 4;
        EV_DisableCharacterDisplay(CWON_WON);
        Fade(20, false);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        CwonStartAuthor(5, nullptr, 0, nullptr, nullptr);
        [[fallthrough]];
    case 4:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            ResetScenarioCharacterAttachment(bag);
            SetAuthorControl(0);
            work->authorState = 0;
            work->temporarySlot = 0;
            Change(CwonEndChaseCallback);
        }
        break;
    default:
        break;
    }
}

void CwonBattleLostChoiceCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    switch (work->authorState) {
    case 0:
        work->authorState = 1;
        Fade(20, true);
        CwonQueueFadeWait(work->task);
        break;
    case 1:
        work->authorState = 2;
        ClearBattleMotionFlagsAndFaces();
        Place(CWON_RYO, CWON_CENTER, work->routeState == 10 ? marketLostPosition : warehouseLostPosition);
        Turn(CWON_RYO, CWON_CENTER, 0);
        SetMotionRegionFlag(scenFindMotionwork(CWON_RYO), 69, 2, MotionRegion::LowerBody);
        EVT_AT_SetMot(CWON_RYO, 0x7f09, 0, 0, 0, 0);
        QueueLoadingFade();
        break;
    case 2: {
        for (const shen_name *name = tables; name->name; ++name)
            EV_EnableCharacterDisplay(*name);
        for (const shen_name *name = brokenTables; name->name; ++name)
            EV_DisableCharacterDisplay(*name);
        CwonRestoreMarketProps(0);
        Fade(20, false);
        EVT_AT_SetMot(CWON_RYO, 0x7f09, 0, 1, 0, 0);
        EVT_AT_SetVoice("0012A008", 1, 100);
        EVT_AT_SetVoice("0012A010", 1, 300); // @text EN: "Damn,\nthose guys!" | JA: "くそ…あいつら！"
        work->authorState = 12;
        work->followHeight = 550;
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(CWON_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(CWON_RYO, &rotation, EventTransformFlags::Local);
        Place(CWON_DUMMY, CWON_CENTER, position);
        EVT_AT_SetAng_Enqueue(CWON_DUMMY, CWON_CENTER, static_cast<float>(rotation.x), static_cast<float>(rotation.y), static_cast<float>(rotation.z), 0);
        Camera(202, CWON_DUMMY);
        EVT_AT_SetCam(206, CWON_DUMMY, CWON_DUMMY, 0, 0, 270);
        break;
    }
    case 3:
        SetAuthorControl(0);
        work->authorState = 0;
        work->temporarySlot = 0;
        spmFadeOut();
        Fade(20, true);
        Change(CwonEndChaseCallback);
        CwonQueueFadeWait(work->task);
        break;
    case 12:
        if (authGetFinished()) {
            EV_CleanupMapControlTask();
            work->followHeight = 0;
        }
        work->followHeight -= FrameTicks();
        if (work->followHeight <= 0)
            work->authorState = 3;
        break;
    default:
        break;
    }
}

std::int32_t CwonUpdateVictoryAuthorHands(CwonEventWork *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return 1;
    const int frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (!frame) {
        ShowBag();
        return 0;
    }
    const shen_name name = frame == 380 ? sam : frame == 390 ? lly : frame == 420 ? CWON_RYO : shen_name{};
    if (name.name) {
        hmdlSetFlagsFor(ctrlGetCharaByName(name), 0, 0, 16);
        hmdlSetFlagsFor(ctrlGetCharaByName(name), 1, 0, 16);
    }
    return 0;
}
} // namespace shenmue::s2
