#include "s2/areas/area_wssf.hpp"
#include "s2/pause.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_characters.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/gauge.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, sos{SHEN_NAME(SOS_)}, dummy{SHEN_NAME(DUMY)}, dummy2{SHEN_NAME(DUM2)};
constexpr shen_name stone{SHEN_NAME(a041)}, support{SHEN_NAME(a040)}, fragments{SHEN_NAME(ISID)}, invalid{0xffffffffU};
constexpr shen_name people[]{ryo, sos, SHEN_NAME(Y01_), SHEN_NAME(Y02_), SHEN_NAME(Y05_), SHEN_NAME(SHI_), SHEN_NAME(SYO_), SHEN_NAME(M01_), SHEN_NAME(TAO_)};
constexpr shen_name loadNames[]{sos, people[2], people[3], people[4], people[5], people[6], people[7], people[8], invalid};
constexpr char fragmentNames[3][8]{"ISIS", "ISIM", "ISIL"};
constexpr std::int32_t fragmentFrames[]{120, 100, 130};
constexpr Vector3 attachmentPosition{0.126300007f, 0.0136f, 0.0408f};
constexpr Vector3i attachmentRotation{10922, 0, 0};
std::int32_t rightHand[]{0, 0, 62989, 0, 0, 0, 0, 61897, 2365, 0, 0, 62079, 0, 0, 64445, 0, 0, 64080, 0, 0, 0, 0, 0, 64263, 0, 0, 62806, 0, 0, 64445, 0, 0, 0,
    0, 0, 63716, 0, 0, 63898, 0, 0, 64080, 0, 0, 0, 0, 0, 61897, 0, 0, 63898, 0, 0, 63535, 0, 0, 0};
std::int32_t leftHand[]{0, 682, 63534, 0, 0, 64859, 0, 63208, 3821, 0, 0, 60257, 0, 0, 61167, 0, 0, 64079, 0, 546, 1820, 0, 0, 59746, 0, 0, 59922, 0, 0, 64664,
    0, 546, 1681, 0, 0, 60261, 0, 0, 61831, 0, 0, 65, 0, 546, 1584, 0, 0, 62293, 0, 0, 64812, 0, 0, 64182, 0, 635, 1811};
HLib::Task *mainTask{};
std::int32_t entry{}, maxPower{}, hitGate{}, battleInput{}, helpOverlay{}, cameraIndex{}, reward{}, debugPower{};
std::int64_t savedScene{};
char savedBgm[16]{}, savedSbt[16]{};
void State(WssfMainWork &w, std::int8_t phase) {
    w.phase = phase;
    w.step = 0;
}
void State(WssfChildWork &w, std::int8_t phase, std::int8_t step = 0) {
    w.phase = phase;
    w.step = step;
}
WssfChildWork &Child(HLib::Task *task) {
    return *static_cast<WssfChildWork *>(HLib::GetTaskParameter(task));
}
WssfMainWork &Parent(WssfChildWork &w) {
    return *static_cast<WssfMainWork *>(HLib::GetTaskParameter(w.parent));
}
MotionWork *Motion(shen_name name) {
    return FindMotionWork(FindCharacterByScenarioName(name));
}
void Fade(std::int32_t frames, std::int32_t to) {
    SetScreenFilmFade(frames, 255, 0, 0, 0, to, 0, 0, 0);
}
void Cam(std::int32_t id, shen_name base = dummy2) {
    (void)EVS_LoopCamera(id, base, base);
}
void Mot(shen_name name, std::uint32_t id, float frame = 0, std::uint32_t flags = 0) {
    (void)EVT_AT_SetMot(name, id, frame, 1, flags, 0);
}
void Pose(shen_name name, const Vector3 &position, const Vector3i &rotation) {
    EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
    (void)EV_AT_SetAngY(name, EventTransformFlags::AllAxes, rotation);
}
void Hide(shen_name name) {
    EV_DisableCharacterDisplay(name);
    EV_ToggleCharacterFlag(name);
}
void Show(shen_name name) {
    EV_EnableCharacterDisplay(name);
    EV_EnsureCharacterDraw(name);
}
void Author(std::int64_t command) {
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(command));
}
bool AuthorLoaded() {
    return EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0;
}
void Face(std::int32_t expression) {
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), expression, 0, 1);
}
void ResetFace() {
    Face(0);
    SetUpperFacialExpression(FindCharacterByScenarioName(ryo), 0, 0, 1);
}
void Controllers(shen_name name, bool enabled) {
    const auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    (void)EV_SetCharaControllerFlags(name, UpdateHeight, state);
    (void)EV_SetCharaControllerFlags(name, ApplyGravity, state);
    (void)EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, state);
    EV_SetCharacterAlphaEnabled(name, enabled);
}
void PrepareCharacters() {
    const Vector4 reflector{1, .3f, .3f, .3f};
    for (auto name : people) {
        Show(name);
        (void)EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_mtwkSetEnabled(Motion(name), 1);
        mtwkSetExtraFlag(Motion(name), 0, 0);
        (void)EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Disabled);
        (void)EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
        SetScenarioReflectorBoardValues(name, &reflector);
        EV_SetRefBoardParamsFor(name, 1);
        for (std::int32_t side = 0; side < 2; ++side)
            hmdlSetFlagsFor(FindCharacterByScenarioName(name), side, 8, 16);
    }
    for (auto name : {stone, support}) {
        EV_EnableCharacterDisplay(name);
        (void)EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Disabled);
        (void)EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
    }
}
void SetupOrigins() {
    Pose(dummy, {138.127197f, 102.150002f, 115.203796f}, {0, 49152, 0});
    Pose(dummy2, {138.127197f, 101.950005f, 115.203796f}, {0, 49152, 0});
    Show(stone);
    (void)AT_SetLocalBaseChara(support, dummy);
    Pose(support, {.16f, .767f, -.6f}, {0, 5461, 0});
    (void)AT_SetLocalBaseChara(fragments, dummy);
    Hide(fragments);
}
void ResetChildren(WssfMainWork &w) {
    State(Child(w.player), 1);
    State(Child(w.stone), 1);
    State(Child(w.partner), 1);
    State(Child(w.spectators), 1);
    SetupOrigins();
}
void ResetChallenge(WssfMainWork &w) {
    ResetChildren(w);
    w.difficulty = std::min<std::int16_t>(static_cast<std::int16_t>(w.difficulty + 1), 2);
    w.tier = 2;
    w.attempts = 0;
    State(w, 3);
}
void AttachStone() {
    AttachScenarioCharacter(stone, ryo, 21, &attachmentPosition, &attachmentRotation);
}
void SetupGrip() {
    Character *const character = FindCharacterByScenarioName(ryo);
    (void)SetHighModelXYZ(character, 1, 0, 0);
    (void)SetHighModelXYZ(character, 1, 1, 0);
    const Vector3i rightRotation{}, leftRotation{61307, 62342, 449};
    (void)ApplyCharacterHandRotation(character, 0, 21, &rightRotation);
    (void)ApplyCharacterHandRotation(character, 1, 21, &leftRotation);
    (void)ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(rightHand), 0);
    (void)ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(leftHand), 0);
}
void FailureSounds(const WssfMainWork &w) {
    (void)EVT_AT_SetSound(0xbc, 60);
    const std::uint32_t first = w.attempts == 1 ? 0xb0 : 0xb4;
    for (std::int32_t i = 0; i < 4; ++i)
        (void)EVT_AT_SetSound(first + i, 180 + 60 * i);
}
void PartnerDialogue(const WssfMainWork &w) {
    if (w.attempts == 1) {
        EVT_AT_SetVoice("0028C031", 1, 0); // @text EN: "What are you doing?\nCome on!" | JA: "何やってんだ。しっかりしろ！"
        EVT_AT_SetVoice("0028C058", 1, 80); // @text EN: "Swing your arm high and\nas you bring it down=@" | JA: "大きく振りかぶって\n打ち下ろしたら、"
        EVT_AT_SetVoice("0028C059", 1, 155); // @text EN: "Concentrate your power\nthe moment you hit." | JA: "石に当たる瞬間に、\nガツンと力をこめるんだ。"
        EVT_AT_SetVoice("0028A038", 1, 260); // @text EN: "I know that." | JA: "わかってる。"
        EVT_AT_SetVoice("0028C060", 1, 280); // @text EN: "Well then, do it again.\nHit it this time." | JA: "よし、もう１回だ。\nしっかり頼むぞ。"
    } else {
        EVT_AT_SetVoice("0028C033", 1, 0); // @text EN: "You fool! The people\nhere are all angry!" | JA: "バカヤローっ！\nお客さんカンカンじゃねえか！"
        EVT_AT_SetVoice("0028C059", 1, 115); // @text EN: "Concentrate your power\nthe moment you hit." | JA: "石に当たる瞬間に、\nガツンと力をこめるんだ。"
        EVT_AT_SetVoice("0028A038", 1, 220); // @text EN: "I know that." | JA: "わかってる。"
        EVT_AT_SetVoice("0028C034", 1, 240); // @text EN: "This is your last chance!\nDo it right!" | JA: "これが最後だ！\n絶対に失敗すんな！"
    }
}
void PartnerWarning(WssfMainWork &w) {
    (void)EVT_AT_SetAngY(sos, dummy, 112.5f, 0);
    Mot(sos, 4380, 0, 0x10000000U);
    Cam(4);
    (void)UpdateMotionWorkWithPosition(Motion(sos), nullptr, 0);
    const auto count = Child(w.partner).counter;

    EVT_AT_SetVoice(count == 1 ? "0028C028" : count == 2 ? "0028C029" : "0028C030", 1, 0); // @text 0028C028 EN: "Don't keep us waiting.\nHurry up." | JA: "おい、まだかよ。\nさっさとやれ。" ; 0028C029 EN: "What are you waiting for?\nGet on with it!" | JA: "まだやんねーのか！\nいつまで待たせるんだ！" ; 0028C030 EN: "What are you waiting for?\nHurry up and break it!" | JA: "おい、いいかげんにしろ！\n早く割らねえか！"
    Child(w.player).step = 10;
}
void HideSpectators() {
    for (std::size_t i = 2; i < std::size(people); ++i)
        Hide(people[i]);
}
void Finish(WssfMainWork &w) {
    using enum CameraState;
    (void)AT_SetLocalBaseChara(ryo, invalid);
    HLib::DestroyTask(w.player);
    HLib::DestroyTask(w.stone);
    HLib::DestroyTask(w.partner);
    HLib::DestroyTask(w.spectators);
    for (auto name : people) {
        EV_EnableCharacterDisplay(name);
        Controllers(name, true);
    }
    for (auto name : {stone, support}) {
        EV_EnableCharacterDisplay(name);
        Controllers(name, true);
    }
    (void)EV_FreeAuthorPak(0, "0028");
    (void)EV_LoadCutsceneFromCurrentScene(savedBgm);
    (void)EV_LoadAudio(savedSbt, savedScene);
    (void)EV_spmBeginSampleExecute();
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ISIWARI"), 0);
    for (auto name : loadNames) {
        if (name.name == 0xffffffffU)
            break;
        (void)EV_npcLockUnlock(name);
        (void)EV_npcFree(name);
    }
    (void)EV_NPC_NewLoadSw(1);
    HLib::DestroyTask(w.task);
    if (g_currentMap.name == SHEN_NAME(WESM)) {
        Pose(ryo, {138, 102.150002f, 114}, {0, 57344, 0});
        Fade(30, 0);
    } else {
        (void)scenSetTaskFlags(6);
        EV_scenQueueLoad(1, SHEN_NAME(WESM), 50);
    }
    EV_EnableCharacterDisplay(ryo);
    Controllers(ryo, true);
    SetPlayerControlEnabled(1);
    camCtrlSetFovTransitionTo(true);
    (void)camCtrlSetState(Init);
}
void FinishChallenge(WssfMainWork &w) {
    std::int32_t flag = -1;
    if (w.tier >= 0 && w.tier <= 1)
        flag = 54;
    else if (w.tier == 2)
        flag = 29;
    else if (w.tier == 3)
        flag = 30;
    if (w.result == 3) {
        if (flag >= 0)
            EV_stepSetFlags(0, flag);
        SetRenderClockAndCompassTo(0);
        (void)SetFightGaugeVisible(0);
        (void)ReleaseGauge();
        EV_EventSetUpHelpIcon_0(0, 0);
        Finish(w);
    } else {
        if (flag >= 0)
            EV_stepSetFlags(1, flag);
        w.phase = 6;
        w.timer = 120;
        helpOverlay = 1;
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(TRR1));
    }
}
void PartnerInit(WssfChildWork &w);
void InitializeChildren(WssfMainWork &w) {
    if (w.step == 1) {
        State(w, 3);
        return;
    }
    if (w.step != 0)
        return;
    (void)EV_FreeAuthorPak(1, "0028");
    const HLib::TaskCallback callbacks[]{WssfPlayerCallback, WssfPartnerCallback, WssfStoneCallback, WssfSpectatorCallback};
    HLib::Task **const destinations[]{&w.player, &w.partner, &w.stone, &w.spectators};
    for (std::size_t i = 0; i < 4; ++i) {
        auto *const task = HLib::EnqueueTaskWithParameter(callbacks[i], 4, 0xb, 0x30, SHEN_NAME(EVNT));
        auto &child = Child(task);
        child.self = &child;
        child.task = task;
        child.parent = w.task;
        State(child, 1);
        child.timer = 0;
        child.variant = 0;
        child.reserved20 = 0;
        *destinations[i] = task;
    }
    SetupOrigins();
    SetRenderClockAndCompassTo(1);
    mtwkSetExtraFlag(Motion(ryo), 13, 0);
    mtwkSetRequestedRootMode(Motion(ryo), 0);
    mtwkSetRequestedRootMode(Motion(sos), 0);
    ++w.step;
}
void Opening(WssfMainWork &w) {
    if (w.step == 0) {
        PrepareCharacters();
        for (auto name : people)
            (void)AT_SetLocalBaseChara(name, invalid);
        (void)AT_SetLocalBaseChara(stone, invalid);
        (void)AT_SetLocalBaseChara(support, invalid);
        Hide(people[3]);
        Author(-7);
        if (w.tier == 1) {
            Author(SHEN_NAME(OPEN));
            Fade(30, 0);
        } else {
            Author(-16);
            Author(SHEN_NAME(OPN2));
        }
        ++w.step;
    } else if (w.step == 1 && !AuthorLoaded()) {
        Fade(0, 255);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        Show(people[3]);
        State(w, 4);
    }
}
void Challenge(WssfMainWork &w) {
    auto &player = Child(w.player);
    if (w.step == 0) {
        ResetChildren(w);
        Fade(30, 0);
        (void)EV_LoadAudio("0028A000", 0);
        (void)EV_spmBeginSampleExecute();
        ++w.step;
    } else if (w.step == 1) {
        (void)LoadGauge();
        (void)SetFightGaugeVisible(1);
        State(player, 3);
        ++w.step;
    } else if (w.step == 2 && player.phase == 7)
        State(w, g_entryPoint == 0 ? 5 : 7);
}
std::int16_t ComputePower(float frame, float end) {
    const float value = frame >= 30 ? (34 - std::fmax(34.0f, frame)) * (1 / (end - 34)) : (std::fmin(28.0f, frame) - 28) * 0.035714287f;
    return static_cast<std::int16_t>(std::min<std::int64_t>(static_cast<std::int64_t>((value + 1) * 100), 100));
}
void PlayerInit(WssfChildWork &w) {
    if (w.step != 0)
        return;
    (void)AT_SetLocalBaseChara(ryo, dummy);
    Pose(ryo, {}, {});
    (void)FindMotionWork(FindCharacterByScenarioName(ryo));
    (void)FindMotionWork(FindCharacterByScenarioName(ryo));
    Mot(ryo, 0xf003, 0, 0x10000000U);
    Cam(1, dummy);
    State(w, 2);
    w.timer = 0;
}
void Power(WssfChildWork &w) {
    auto &main = Parent(w);
    auto &partner = Child(main.partner);
    auto &rock = Child(main.stone);
    switch (w.step) {
    case -1: {
        const auto input = GetCurrentControlInput(2);
        if (input == 0x4000)
            --debugPower;
        else if (input == 0x8000)
            ++debugPower;
        (void)UpdateDualFightGauge(static_cast<float>(debugPower), 0);
        break;
    }
    case 0:
        helpOverlay = 1;
        SetupGrip();
        SetFightGaugeDisplayLayout(2);
        SetFightGaugeSecondaryOverlay(1);
        (void)UpdateDualFightGauge(0, 0);
        Mot(ryo, 4377);
        ResetFace();
        State(rock, 3);
        State(partner, 1);
        ++w.step;
        ++main.attempts;
        main.power = 0;
        PartnerInit(partner);
        Cam(2);
        break;
    case 1:
        if (SetMotionRegionFlag(Motion(ryo), 65, 1, MotionRegion::LowerBody))
            ++w.step;
        break;
    case 2: {
        battleInput = helpOverlay = 1;
        Vector3 p;
        EV_GetCharaPosition(stone, 0xffffffffU, &p, EventTransformFlags::World);
        (void)UpdateMotionWorkWithPosition(Motion(ryo), &p, 0);
        Cam(10);
        Mot(ryo, 4369);
        EV_EnableCharacterDisplay(people[7]);
        ++partner.step;
        ++w.step;
        w.timer = 60;
        break;
    }
    case 3: {
        (void)GetParentBoneWrapper(Motion(ryo), 17);
        if (w.timer == 26)
            (void)Loop3DSoundEffect(0x3a4);
        if (SetMotionRegionFlag(Motion(ryo), 65, 1, MotionRegion::LowerBody))
            w.timer = 60;
        if (!camCtrlCheckIsEventCam())
            Cam(10);
        if (GetCurrentControlInput(1) & 0x400) {
            ++w.step;
            ++partner.step;
            w.timer = 30;
            main.latchedPower = main.power;
            battleInput = 0;
            const float frame = GetMotionFrame(Motion(ryo), MotionRegion::LowerBody);
            hitGate = frame <= 34;
            (void)Loop3DSoundEffect(0x3fc, frame);
            scenCvsCtrlFree();
        } else {
            main.endFrame = GetMotionEndFrame(Motion(ryo), MotionRegion::LowerBody);
            main.power = ComputePower(GetMotionFrame(Motion(ryo), MotionRegion::LowerBody), main.endFrame);
            maxPower = main.power;
            (void)UpdateDualFightGauge(static_cast<float>(main.power), 0);
        }
        break;
    }
    case 4: {
        const float frame = GetMotionFrame(Motion(ryo), MotionRegion::LowerBody);
        if (frame < 30)
            hitGate = 1;
        else if (!hitGate)
            break;
        if (frame > 30) {
            battleInput = helpOverlay = 1;
            const float offset = static_cast<float>(main.difficulty) * 5;
            Mot(ryo, 4376, 30 - offset);
            SetMotionLayerEndFrame(Motion(ryo), 50, MotionRegion::LowerBody);
            Cam(3);
            (void)Loop3DSoundEffect(0x32, offset);
            (void)EVT_AT_SetSound(0xa8, 3);
            figpSetFlagsBlendForBlur(ryo, 5, 1);
            figpAssignModeFor(ryo, 4);
            ++w.step;
        }
        break;
    }
    case 5: {
        if (GetCurrentControlInput(1) & 0x400) {
            (void)Loop3DSoundEffect(0x3fc);
            w.frame = GetMotionFrame(Motion(ryo), MotionRegion::LowerBody);
            ++w.step;
            break;
        }
        if (SetMotionRegionFlag(Motion(ryo), 65, 1, MotionRegion::LowerBody)) {
            main.power = 0;
            ++w.step;
        } else {
            float frame = GetMotionFrame(Motion(ryo), MotionRegion::LowerBody);
            if (frame > 40)
                frame = 40 - (frame - 40) / 3;
            const float start = 30 - static_cast<float>(main.difficulty) * 5;
            main.power = static_cast<std::int16_t>(std::min(static_cast<std::int32_t>(static_cast<double>((frame - start) / (40 - start)) * 100), 100));
        }
        main.power = static_cast<std::int16_t>(std::min<std::int32_t>(main.power, maxPower));
        (void)UpdateDualFightGauge(static_cast<float>(maxPower), static_cast<float>(main.power));
        break;
    }
    case 6:
        if (!SetMotionRegionFlag(Motion(ryo), 65, 1, MotionRegion::LowerBody))
            break;
        [[fallthrough]];
    case 7:
        (void)UpdateMotionWorkWithPosition(Motion(ryo), nullptr, 0);
        (void)UpdateMotionWorkWithPosition(Motion(sos), nullptr, 0);
        figpAssignModeFor(ryo, 0);
        State(w, 4);
        ++rock.step;
        break;
    case 10: {
        const float frame = GetMotionFrame(Motion(ryo), MotionRegion::LowerBody);
        const float speed = GetMotionSpeed(Motion(ryo), MotionRegion::LowerBody);
        SetMotionRegionEvalTime(Motion(ryo), frame - speed, MotionRegion::LowerBody);
        break;
    }
    default:
        break;
    }
}
void CheckHit(WssfChildWork &w) {
    if (w.step != 0)
        return;
    auto &main = Parent(w);
    auto &rock = Child(main.stone);
    battleInput = 0;
    if (Child(main.partner).counter > 4)
        main.result = 0;
    else if (static_cast<std::uint16_t>(main.power - 1) > 99)
        main.result = 2;
    else if (main.power < 84)
        main.result = 1;
    else {
        main.result = 3;
        rock.variant = main.power < 92 ? 0 : main.power < 100 ? 1 : 2;
    }
    if (main.result == 3) {
        State(rock, 4);
        State(w, 6);
        SetFightGaugeSecondaryOverlay(0);
    } else {
        State(rock, 4, 10);
        if (main.result != 0 && main.attempts < 3) {
            (void)Loop3DSoundEffect(0xa3);
            State(w, 5);
        } else
            State(w, 6);
    }
}
void FailedAgain(WssfChildWork &w) {
    auto &main = Parent(w);
    auto &partner = Child(main.partner);
    if (w.step == 0) {
        EV_DisableCharacterDisplay(people[7]);
        Mot(ryo, 4364);
        Cam(5);
        camCtrlSetRotationXYZ(.05f, .05f, .9f);
        Face(6);
        FailureSounds(main);
        ++w.step;
        w.timer = 120;
        cameraIndex = 6;
    } else if (w.step == 1) {
        if (w.timer <= 0) {
            State(partner, 3);
            ++w.step;
            w.timer = 330;
        }
        if (!camCtrlCheckIsEventCam()) {
            Cam(cameraIndex);
            if (cameraIndex == 7)
                ResetFace();
            ++cameraIndex;
        }
    } else if (w.step == 2) {
        if (!camCtrlCheckIsEventCam() && cameraIndex <= 8) {
            ResetFace();
            Cam(cameraIndex++);
        }
        if (w.timer <= 0) {
            Pose(ryo, {}, {});
            Cam(2);
            Mot(ryo, 0xf003, 0, 0x10000000U);
            State(w, 3);
            State(Child(main.stone), 1);
        }
    }
}
void Complete(WssfChildWork &w) {
    auto &main = Parent(w);
    auto &rock = Child(main.stone);
    switch (w.step) {
    case 0:
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        (void)SetFightGaugeVisible(0);
        for (auto name : people)
            mtwkSetExtraFlag(Motion(name), 0, 0);
        for (std::size_t i = 0; i < 8; ++i)
            (void)AT_SetLocalBaseChara(people[i], invalid);
        (void)AT_SetLocalBaseChara(stone, invalid);
        (void)AT_SetLocalBaseChara(support, invalid);
        ++w.step;
        reward = 0;
        Author(-8);
        if (main.result == 3) {
            Author(1397048147);
            (void)EVT_AT_SetSound(0xbb, 90);
            (void)EVT_AT_SetSound(0xa5, 90);
            (void)EVT_AT_SetSound(0xa4, 120);
            (void)EVT_AT_SetSound(0xa6, 150);
        } else {
            Author(main.result ? 1279869254 : 1280527939);
            (void)Loop3DSoundEffect(0xbc);
            (void)EVT_AT_SetSound(0xb8, 60);
            (void)EVT_AT_SetSound(0xb9, 120);
            (void)EVT_AT_SetSound(0xba, 180);
        }
        break;
    case 1:
        for (std::size_t i = 1; i < std::size(people); ++i)
            (void)SetPluralControlFor(FindCharacterByScenarioName(people[i]), 1);
        w.step = main.result == 3 && main.tier < 3 ? 2 : 3;
        break;
    case 2:
        if (!AuthorLoaded()) {
            PrepareCharacters();
            Hide(stone);
            rock.frame = 0;
            Author(-7);
            Author(-11);
            Author(1381254476);
            w.step = 21;
            reward = 35;
        }
        break;
    case 21:
        if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1) {
            HideSpectators();
            Author(-10);
        }
        [[fallthrough]];
    case 3:
        if (!AuthorLoaded()) {
            HideSpectators();
            w.step = 4;
            w.timer = 30;
        }
        break;
    case 4:
        if (w.timer <= 0) {
            if (g_entryPoint == 0)
                Fade(30, 0);
            State(w, 7);
            State(rock, 1);
            SetGameMessageDelay(.5f);
            if (reward)
                (void)AddInventoryItem(reward, 1, 1);
        }
        break;
    default:
        break;
    }
}
void PartnerInit(WssfChildWork &w) {
    if (w.step != 0)
        return;
    (void)AT_SetLocalBaseChara(sos, dummy);
    Pose(sos, {1, 0, -.5f}, {0, 14563, 0});
    Mot(sos, 4384, 0, 0x10000000U);
    State(w, 2);
}
void PartnerWait(WssfChildWork &w) {
    auto &main = Parent(w);
    auto &player = Child(main.player);
    switch (w.step) {
    case 0:
        State(w, 2);
        w.counter = 0;
        break;
    case 1: {
        ++w.counter;
        w.timer = 600;
        w.step = w.counter < 4 ? 2 : 4;
        (void)EV_AT_SetAngY(sos, EventTransformFlags::AllAxes, {0, 14563, 0});
        Vector3 p;
        EV_GetCharaPosition(stone, 0xffffffffU, &p, EventTransformFlags::World);
        (void)UpdateMotionWorkWithPosition(Motion(sos), &p, 1);
        break;
    }
    case 2:
        if (w.timer <= 0) {
            PartnerWarning(main);
            w.step = 10;
            w.timer = 120;
        }
        break;
    case 3:
    case 5:
        w.timer = 0;
        break;
    case 4:
        if (w.timer <= 0) {
            ++w.counter;
            w.step = 11;
        }
        break;
    case 10:
        if (!camCtrlCheckIsEventCam())
            Cam(20);
        if (w.timer <= 0) {
            (void)EVT_AT_SetAngY(sos, shen_name{0}, 80, 0);
            Mot(sos, 4384, 0, 0x10000000U);
            Cam(10);
            player.step = 3;
            w.step = 1;
        }
        break;
    case 11:
        (void)UpdateMotionWorkWithPosition(Motion(sos), nullptr, 0);
        player.step = 7;
        w.step = 3;
        break;
    default:
        break;
    }
}
void PartnerTalk(WssfChildWork &w) {
    if (w.step == 0) {
        Pose(sos, {.4f, 0, .5f}, {0, 4551, 0});
        Mot(sos, 4388, 0, 0x10000000U);
        ++w.step;
    } else if (w.step == 1) {
        PartnerDialogue(Parent(w));
        ++w.step;
    }
}
void StoneFailure(std::int32_t attempts) {
    ResetScenarioCharacterAttachment(stone);
    Vector3 p;
    EV_GetCharaPosition(stone, 0xffffffffU, &p, EventTransformFlags::World);
    Vector3i rotation{};
    if (attempts >= 3) {
        p = {138.604706f, 102.958603f, 115.202103f};
        rotation = {38229, 60074, 0};
    } else {
        p.x += .05f;
        p.y -= .035f;
        rotation = {33678, 0, 0};
        (void)Loop3DSoundEffect(0x5a8, p.x);
    }
    Pose(stone, p, rotation);
}
void EvaluateFragments(WssfChildWork &w, std::int32_t frame, bool doubleScale) {
    for (std::uint16_t part = 152; part <= 170; ++part) {
        Vector3 p;
        Vector3i r;
        (void)dynaReadEvaluate(part - 152, frame, &p.x, &r, reinterpret_cast<const std::uint8_t *>(fragmentNames[w.variant]));
        if (doubleScale) {
            p.x = static_cast<float>(static_cast<double>(p.x) * 10);
            p.y = static_cast<float>(static_cast<double>(p.y) * 10);
            p.z = static_cast<float>(static_cast<double>(p.z) * 10);
        } else {
            p.x *= 10;
            p.y *= 10;
            p.z *= 10;
        }
        EV_scenApplyPosOrRotToPartOf(fragments, part, &p, &r, EventCharacterPartTransformMode::Absolute);
    }
}
void StoneBreak(WssfChildWork &w) {
    auto &main = Parent(w);
    switch (w.step) {
    case 0:
        EV_AT_SetPos(fragments, EventTransformFlags::AllAxes, {0, .5f, 0});
        ApplyCharacterGeometryLighting(FindCharacterByScenarioName(fragments), 0);
        if (w.variant == 0) {
            Pose(fragments, {.037f, -.142f, -.423f}, {0, 63715, 0});
            w.frame = 4;
        } else if (w.variant == 1) {
            Pose(fragments, {-.08f, -.134f, -.429f}, {0, 1820, 63715});
            w.frame = 12;
        } else if (w.variant == 2) {
            Pose(fragments, {.055f, -.142f, -.434f}, {});
            w.frame = 4;
        }
        EV_DisableCharacterDisplay(fragments);
        EvaluateFragments(w, 2, true);
        w.timer = 5;
        ++w.step;
        break;
    case 1:
        if (w.timer == 4) {
            Show(fragments);
            Hide(stone);
        }
        if (w.timer == 0)
            ++w.step;
        break;
    case 2:
        EvaluateFragments(w, static_cast<std::int32_t>(w.frame), false);
        w.frame += 1;
        if (w.frame > fragmentFrames[w.variant])
            ++w.step;
        break;
    case 10:
        if (main.attempts == 3)
            w.timer = 2;
        else {
            AttachStone();
            w.timer = 53;
        }
        ++w.step;
        break;
    case 11:
        if (w.timer <= 0) {
            StoneFailure(main.attempts);
            ++w.step;
        }
        break;
    default:
        break;
    }
}
} // namespace
void WssfInit() {
    mainTask = nullptr;
    entry = maxPower = hitGate = cameraIndex = battleInput = helpOverlay = 0;
    savedScene = 0;
    std::memset(savedBgm, 0, 16);
    std::memset(savedSbt, 0, 16);
}
void WssfMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    (void)SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    FinishLoading();
    EV_EventSetUpHelpIcon_0(0x49, 0);
    Fade(0, 255);
    entry = g_currentMap.name == SHEN_NAME(WESM) ? static_cast<std::int32_t>(argument) : static_cast<std::int32_t>(g_entryPoint);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ISIWARI"), 0);
    (void)SetLowQualityHands(FindCharacterByScenarioName(ryo));
    savedScene = GetSelectedSoundSceneIndex();
    std::memcpy(savedBgm, GetCurrentBgm(), 16);
    std::memcpy(savedSbt, shenGetCurrentSbt(), 16);
    (void)EV_LoadCutsceneFromCurrentScene("0028A000");
    GetCurrentDateTime(eventSavedCurrentDateTime);
    battleInput = helpOverlay = 0;
    mainTask = HLib::EnqueueTaskWithParameter(WssfMainCallback, 4, 0xb, 0x58, SHEN_NAME(EVNT));
    auto &w = *static_cast<WssfMainWork *>(HLib::GetTaskParameter(mainTask));
    w.self = &w;
    w.task = mainTask;
    w.player = w.partner = w.stone = nullptr;
    State(w, 1);
    w.result = w.reserved03 = 0;
    w.tier = static_cast<std::int8_t>(entry);
    w.attempts = 0;
    w.timer = w.difficulty = 0;
    w.reserved40 = 0;
    w.reserved48 = 0;
    w.scale = 1;
    w.power = 0;
    (void)Queue_EVT_UTL_LoadNPCChara_3(loadNames, mainTask);
}
void WssfMainCallback(void *data) {
    auto &w = *static_cast<WssfMainWork *>(data);
    if (w.timer > 0)
        --w.timer;
    if (battleInput)
        (void)GetBattleDataConfig(0, SHEN_NAME(BTAB));
    if (helpOverlay)
        CheckHelpIcon(1, 0, CheckHelpIconOp::FrameOverlay);
    switch (w.phase) {
    case 1:
        InitializeChildren(w);
        break;
    case 2:
        if (w.step == 0) {
            const auto buttons = GetCurrentControlInput(1);
            if (buttons & 0x400)
                State(w, 3);
            else if (buttons & 0x200) {
                State(w, 7);
                w.result = 3;
            }
        }
        break;
    case 3:
        Opening(w);
        break;
    case 4:
        Challenge(w);
        break;
    case 5:
        ResetChallenge(w);
        break;
    case 6:
        if (w.timer <= 0) {
            helpOverlay = 0;
            ResetChallenge(w);
            SetCurrentDateTimeTo(eventSavedCurrentDateTime);
            ST_LoadTitleFade();
            Fade(30, 0);
        }
        break;
    case 7:
        FinishChallenge(w);
        break;
    default:
        break;
    }
}
void WssfPlayerCallback(void *data) {
    auto &w = *static_cast<WssfChildWork *>(data);
    if (w.timer > 0)
        --w.timer;
    switch (w.phase) {
    case 1:
        PlayerInit(w);
        break;
    case 2:
        w.phase = 2;
        break;
    case 3:
        Power(w);
        break;
    case 4:
        CheckHit(w);
        break;
    case 5:
        FailedAgain(w);
        break;
    case 6:
        Complete(w);
        break;
    case 7:
        w.phase = 7;
        break;
    default:
        break;
    }
}
void WssfPartnerCallback(void *data) {
    auto &w = *static_cast<WssfChildWork *>(data);
    if (w.timer > 0)
        --w.timer;
    switch (w.phase) {
    case 1:
        PartnerInit(w);
        break;
    case 2:
        PartnerWait(w);
        break;
    case 3:
        PartnerTalk(w);
        break;
    default:
        break;
    }
}
void WssfStoneCallback(void *data) {
    auto &w = *static_cast<WssfChildWork *>(data);
    if (w.timer > 0)
        --w.timer;
    if (w.phase == 1 && w.step == 0) {
        (void)AT_SetLocalBaseChara(stone, dummy);
        Pose(stone, {.050713f, .77f, -.546707f}, {31517, 7879, 64951});
        State(w, 2);
    } else if (w.phase == 3) {
        switch (w.step) {
        case 0: {
            auto &main = Parent(w);
            if (main.tier == 1 && main.attempts == 1)
                AttachStone();
            ++w.step;
            w.timer = 65;
            break;
        }
        case 1:
            if (w.timer <= 0)
                ++w.step;
            break;
        case 2:
            AttachStone();
            (void)Loop3DSoundEffect(0xa2);
            ++w.step;
            break;
        case 4:
            ResetScenarioCharacterAttachment(stone);
            ++w.step;
            break;
        default:
            break;
        }
    } else if (w.phase == 4)
        StoneBreak(w);
}
void WssfSpectatorCallback(void *data) {
    auto &w = *static_cast<WssfChildWork *>(data);
    if (w.timer > 0)
        --w.timer;
    if (w.phase != 1 || w.step != 0)
        return;
    (void)AT_SetLocalBaseChara(people[4], dummy);
    Pose(people[4], {1.7f, .022f, .1f}, {0, 12724, 0});
    (void)AT_SetLocalBaseChara(people[6], dummy);
    Pose(people[6], {-.5f, .022f, .9f}, {0, 60529, 0});
    Show(people[6]);
    (void)AT_SetLocalBaseChara(people[8], dummy);
    Pose(people[8], {.8f, .022f, .9f}, {0, 6826, 0});
    Show(people[8]);
    constexpr std::uint32_t animations[]{4373, 4363, 4387, 4373, 4386, 4363, 4385};
    for (std::size_t i = 2; i < std::size(people); ++i)
        Mot(people[i], animations[i - 2], i == 6 ? 1.0f : 0.0f, 0x10000000U);
    State(w, 2);
}
const ScriptAreaDefinition wssfScriptDefinition{.areaId = AREA_WSSF, .field04 = 0, .init = WssfInit, .main = WssfMain};
} // namespace shenmue::s2
