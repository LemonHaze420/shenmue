#include "s2/areas/area_bell.hpp"
#include "s2/areas/area_bell_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_call.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/telephone_cord.hpp"
#include "s2/ui.hpp"
#include <array>
#include <bit>

namespace shenmue::s2 {
namespace bell_event {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name ren{SHEN_NAME(REN_)};
constexpr shen_name yang{SHEN_NAME(YAN_)};
constexpr shen_name anchor{SHEN_NAME(a212)};
constexpr shen_name bellDoor{SHEN_NAME(R107)};
BellEventWork *work{};
std::array<SearchTarget, 2> targets{};
void Gravity(shen_name character, bool enabled) {
    const auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(character, 2, state);
    EV_SetCharaControllerFlags(character, 3, state);
}
void Hand(shen_name character, int side, std::array<std::int32_t, 58> &command, int ticks) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(character), side, reinterpret_cast<std::byte *>(command.data()), static_cast<std::int16_t>(ticks));
}
void Pose(shen_name character, const Vector3 &position, std::int32_t yaw) {
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, {0, yaw, 0});
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}
void Motion(shen_name character, std::uint32_t animation, std::uint32_t flags) {
    EventSetMotion request{0, character, animation, 0, 1, flags, 0};
    EVT_AT_SetMot_Callback(&request);
}
void Voice(const char *voice, int delay = 0) {
    EVT_AT_SetVoice(voice, delay);
}
void Camera(int id, shen_name character, int delay = 0) {
    EVT_AT_SetCam(id, character, character, 0, 0, delay);
}
void EyeTowardBell(void *) {
    const Vector3 point{208.10001f, 105, 194.89999f};
    SetEyeToChara(ctrlGetCharaByName(ryo), &point, 10, 2);
}
void ResetEye(void *) {
    SetEyeToChara(ctrlGetCharaByName(ryo), nullptr, 16, 0);
}
void RestoreControl(void *parameter) {
    auto *state = static_cast<BellEventWork *>(parameter);
    state->busy = 0;
    SetPlayerControlEnabled(1);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
}
void QueuePoint(shen_name character, shen_name target, int bone, std::uint32_t flags, int delay) {
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetPoint_Callback, 6, 0x0b, sizeof(EventSetPoint), AREA_EVENT_TASK);
    RegisterEventTask(task);
    auto *point = static_cast<EventSetPoint *>(HLib::GetTaskParameter(task));
    *point = {character, target, bone, 1, 0, 0, flags, 0, 1, static_cast<float>(delay)};
}
int Conversation(std::uint8_t index) {
    switch (index) {
    case 0:
        Camera(210, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A018"); // @text EN: "This place sure doesn't look like\na snitch's dwelling." | JA: "これで本当に情報屋なのか？"
        Camera(220, anchor, 80);
        Voice("0202C013", 80); // @text EN: "Fools you, doesn't it?" | JA: "この店からじゃ\n想像できねえだろ。"
        return 170;
    case 1:
        Camera(210, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202A020", 20); // @text EN: "How does Yang get his information?" | JA: "いったいどうやって\nヤンは情報を？"
        Camera(250, anchor, 100);
        Voice("0202C014", 100); // @text EN: "His sources are the best in Hong Kong." | JA: "ヤツの仕入れルートは\n香港一だ。"
        Camera(230, anchor, 180);
        Voice("0202C015", 180); // @text EN: "He gets the prohibited stuff\nthrough the black market." | JA: "禁制品だろうと\n闇ルートで仕込んでくる。"
        return 280;
    case 2:
        Camera(200, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202A022", 20); // @text EN: "I've never seen these things before." | JA: "みんな見かけない物ばかりだな。"
        Camera(220, anchor, 90);
        Voice("0202C017", 90); // @text EN: "Aren't there any medicine shops in Japan?" | JA: "日本に漢方薬屋はねえのか？"
        Camera(210, ryo, 160);
        Voice("0202A023", 160); // @text EN: "There are.\nBut I've never been inside one." | JA: "あるけど入ったことはない。"
        Camera(230, anchor, 220);
        Voice("0202C018", 220); // @text EN: "Well, you don't look like you'd\never get sick." | JA: "てめえは病気に\n縁がなさそうだしな。"
        return 310;
    case 3:
        Camera(200, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202A024", 20); // @text EN: "Do the medicines here work?" | JA: "ここの薬は効くのか？"
        Camera(250, anchor, 80);
        Voice("0202C019", 80); // @text EN: "They're quite well known.\nThere's even Dongzhongxiacao here." | JA: "その筋じゃ有名だ。\n冬虫夏草とかもあるぜ。"
        Voice("0202A025", 210); // @text EN: "What is that?" | JA: "なんだ、それは？"
        Camera(240, anchor, 250);
        Voice("0202C020", 250); // @text EN: "It's a plant that grows out\nof an insect in the ground." | JA: "土ん中にいる虫とかに\n寄生して育つ植物だ。"
        Voice("0202C021", 350); // @text EN: "An insect in the winter that\nturns into a plant by the summer." | JA: "冬には虫だったものが\n夏には草になっている。"
        Camera(210, ryo, 480);
        Voice("0202A026", 480); // @text EN: "Dongzhongxiacao=@" | JA: "だから冬虫夏草か。"
        Voice("0202A027", 540); // @text EN: "And that becomes a medicine?" | JA: "そんなものも薬になるのか？"
        Camera(230, anchor, 610);
        Voice("0202C022", 610); // @text EN: "I heard it's highly nutritious." | JA: "滋養強壮に効くって話だ。"
        Voice("0202C023", 680); // @text EN: "But Yang here sells fake stuff too,\ndepending on the customer." | JA: "まあ、ヤンの野郎は客によっちゃ、\nまがい物をつかませるからな。"
        return 850;
    case 4:
        Camera(250, anchor);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202C024", 20); // @text EN: "Enough talking now." | JA: "もう、くっちゃべるのは\nいいだろ。"
        Camera(220, anchor, 130);
        Voice("0202C025", 130); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
        Voice("0202A009", 200); // @text EN: "Yeah." | JA: "ああ。"
        return 240;
    default: {
        Voice(static_cast<std::uint8_t>(static_cast<std::int32_t>(GetRandomNumberInRange(2))) == 0 ? "0202A019" : "0202A028"); // @text 0202A019 EN: "Ren." | JA: "レン。" ; 0202A028 EN: "Hey." | JA: "おい。"
        const std::uint8_t choice = static_cast<std::uint8_t>(static_cast<std::int32_t>(GetRandomNumberInRange(3)));
        if (choice == 0) {
            Camera(240, anchor, 20);
            Voice("0202C024", 20); // @text EN: "Enough talking now." | JA: "もう、くっちゃべるのは\nいいだろ。"
            return 140;
        }
        if (choice == 1) {
            Camera(230, anchor, 20);
            Voice("0202C026", 20); // @text EN: "Ring the damn bell." | JA: "呼び鈴を押しゃあ\nいいんだよ。"
            return 110;
        }
        Camera(220, anchor, 20);
        Voice("0202C025", 20); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
        return 100;
    }
    }
}
} // namespace bell_event

void BELL_init_func() {
    bell_event::targets = {};
    bell_event::work = nullptr;
}
void BELL_main_func(std::uintptr_t argument) {
    using namespace bell_event;
    const auto command = *reinterpret_cast<const std::uint32_t *>(argument);
    if (!command) {
        SetPlayerControlEnabled(0);
        ReleaseNpcScriptRouteOwner();
        SetScenarioFollowCharacter({});
        EV_SetNewSceneIndex();
        EV_setHoldStatesFlags(0);
        auto *task = HLib::EnqueueTaskWithParameter(BellLoadNpcCallback, 5, 0x0b, sizeof(BellEventWork), AREA_EVENT_TASK);
        work = static_cast<BellEventWork *>(HLib::GetTaskParameter(task));
        work->task = task;
        work->elapsed = 0;
        work->busy = 0;
        work->escapeTalks = 0;
        work->conversations = 0;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 679, 1);
        EV_NPC_NewLoadSw(0);
        EV_CheckWarmStartNpcRoute(ren);
        EV_CheckWarmStartNpcRoute(yang);
        Gravity(ryo, false);
        EV_LoadCutsceneFromCurrentScene("0202A000.smp");
        EV_FreeAuthorPak(1, "0202");
        EV_FreeAuthorPak(1, "0203");
        EV_SetCanDoAreaJumpTo(0);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("BELL"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BELL"), 0);
        EV_setHoldStatesFlags(0);
    }
    work->command = command;
}
void BellLoadNpcCallback(void *parameter) {
    using namespace bell_event;
    if (!EV_ForceLoadNpc(ren))
        return;
    SetLowQualityHands(ctrlGetCharaByName(ren));
    EV_SetCharacterAlphaEnabled(ren, false);
    HLib::SetCurrentTaskTo(BellInteractionInitCallback);
    QueueSkAuthorStart(static_cast<BellEventWork *>(parameter)->task, SHEN_NAME(BEL0), nullptr, nullptr, 0);
}
void BellInteractionInitCallback(void *parameter) {
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    Pose(ren, {207.64999f, 103.6f, 195.10001f}, 58982);
    Pose(ryo, {208.73f, 103.6f, 195.91f}, 57344);
    Motion(ren, 0x2182, 0x41000000U);
    Motion(ryo, 0xf003, 0x40000000U);
    SetLowQualityHands(ctrlGetCharaByName(ren));
    SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 1, 0);
    Hand(ren, 0, restingHand, 0);
    Hand(ren, 1, restingHand, 0);
    targets = {SearchTarget{SHEN_NAME(E000), 0, 0, {std::bit_cast<float>(0x4350a3d7U), 104.5f, 194.73f}, 0.3f, -32768, 32768}, SearchTarget{}};
    srchSetTarget(2, targets.data());
    Pose(anchor, {std::bit_cast<float>(0x4350a3d7U), 104.5f, 194.73f}, 0);
    DisableScenarioCharacterDisplay(SHEN_NAME(a047));
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    state->renRouteCompletion = static_cast<std::uint32_t>(GetNpcScriptRouteCompletion(ren));
    EV_npcUnknownFunc(ren);
    Gravity(ryo, true);
    EventFilmControl film{0, 10, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(BellInteractionCallback);
}
void BellInteractionCallback(void *parameter) {
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    if (state->command) {
        if (state->command == 1) {
            EV_stepSetFlags(0, 0x19b);
            SetPlayerControlEnabled(0);
            state->busy = 1;
            state->phase = 0;
            HLib::SetCurrentTaskTo(BellRingCallback);
            return;
        }
        if (state->command == 3 && !state->busy && ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            srchReset();
            const auto duration = Conversation(state->conversations);
            ++state->conversations;
            state->command = 0;
            state->busy = 1;
            QueueEventCallFunction(EyeTowardBell, nullptr, 30);
            QueueEventCallFunction(ResetEye, nullptr, 100);
            QueuePoint(ryo, ren, 5, 0x20000000U, 30);
            EVT_AT_SetPoint(ryo, ren, 5, 1, 0, duration, 0, 0x10000000U);
            EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x40000000U, 0);
            QueueEventCallFunction(RestoreControl, state, duration);
            HLib::SetupTaskDelay(duration);
            return;
        }
        if (state->command != 2)
            return;
        SetPlayerControlEnabled(0);
        srchReset();
        Pose(ryo, {208.2f, 103.6f, 196}, 24576);
        int camera = 120;
        if (!state->escapeTalks) {
            Voice("0202C028"); // @text EN: "Where do you think you're going?" | JA: "おい、どこ行くんだ。"
            Voice("0202C036", 60); // @text EN: "Hurry up and ring the bell." | JA: "とっとと呼び鈴を押せよ。"
        } else if (state->escapeTalks == 1) {
            Voice("0202C037"); // @text EN: "What are you doing, stupid?" | JA: "何してんだ、バカ。"
            Voice("0202C038", 70); // @text EN: "Ring the bell and get Yang!" | JA: "呼び鈴押して\nヤンを呼べってんだよ。"
            camera = 121;
        } else {
            Voice("0202C039"); // @text EN: "Where are you going?" | JA: "どこ行くつもりだ。"
            Voice("0202C025", 70); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
            if (state->escapeTalks == 2)
                camera = 122;
            else {
                const std::uint8_t choice = static_cast<std::uint8_t>(static_cast<std::int32_t>(GetRandomNumberInRange(3)));
                camera = choice == 0 ? 120 : choice == 1 ? 121 : 122;
            }
        }
        EVS_LoopCamera(camera, anchor, anchor);
        ++state->escapeTalks;
        state->command = 0;
        state->busy = 1;
        QueueEventCallFunction(EyeTowardBell, nullptr, 30);
        QueueEventCallFunction(ResetEye, nullptr, 120);
        QueuePoint(ryo, ren, 5, 0x20000000U, 30);
        QueuePoint(ryo, {}, 0, 0x10000000U, 120);
        QueuePoint(ren, ryo, 5, 0x20000000U, 1);
        QueuePoint(ren, {}, 0, 0x10000000U, 120);
        Motion(ryo, 0xf003, 0x40000000U);
        QueueEventCallFunction(RestoreControl, state, 120);
        return;
    }
    if (!state->busy && !GetShouldRenderIcons(RenderIconSlot::Any))
        ++state->elapsed;
    if (state->elapsed == 1030 || state->elapsed == 1920 || state->elapsed == 2840)
        SetPlayerControlEnabled(1);
    else if (state->elapsed == 900 || state->elapsed == 1800 || state->elapsed == 2700) {
        SetPlayerControlEnabled(0);
        if (IsItemSelectable())
            RequestMemoClose();
        EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x80000000U, 0);
        if (state->elapsed == 900) {
            Voice("0202C032"); // @text EN: "What are you waiting for?" | JA: "何、ぐずぐずしてんだ。"
            Voice("0202C026", 70); // @text EN: "Ring the damn bell." | JA: "呼び鈴を押しゃあ\nいいんだよ。"
        } else if (state->elapsed == 1800) {
            Voice("0202C033"); // @text EN: "You're wasting time." | JA: "時間がもったいねえだろ。"
            Voice("0202C025", 60); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
        } else {
            Voice("0202C034"); // @text EN: "Give me a break." | JA: "おい、いいかげんにしろ。"
            Voice("0202C035", 70); // @text EN: "Ring the bell and get Yang." | JA: "呼び鈴押して\nヤンを呼べ。"
        }
    }
    if (state->elapsed < 3600)
        return;
    if (IsItemSelectable())
        RequestMemoClose();
    srchReset();
    RefreshPadTask(0, 1);
    SetPlayerControlEnabled(0);
    Pose(ren, {207.7f, 103.6f, 194.96001f}, 58982);
    DisableScenarioCharacterDisplay(ryo);
    Vector3 position{};
    EV_GetCharaPosition(anchor, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ren), &position, 1);
    Vector4 reflector{1, 0.3f, 0.3f, 0.3f};
    SetScenarioReflectorBoardValues(ren, &reflector);
    EV_SetRefBoardParamsFor(ren, 1);
    EV_PlayDialogue("0202C031");
    EVT_AT_SetMot(ren, 0x2183, 0, 1, 0x40000000U, 0);
    state->phase = 4;
    EV_stepSetFlags(0, 0x19c);
    EVS_LoopCamera(110, anchor, anchor);
    EVT_AT_FadeBGM(30, 1, 120);
    EVT_AT_MenCtrl({1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 120});
    HLib::SetCurrentTaskTo(BellRingCallback);
    HLib::SetupTaskDelay(150);
}
void BellRingCallback(void *parameter) {
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    if (!state->phase) {
        EVS_LoopCamera(100, anchor, anchor);
        SetLowQualityHands(ctrlGetCharaByName(ryo));
        SetLowQualityHands(ctrlGetCharaByName(ren));
        SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 1, 0);
        SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 1, 0);
        Hand(ren, 0, ringingHand, 0);
        Hand(ren, 1, ringingHand, 0);
        const Vector3i rotation{0, 4732, 1820};
        ApplyCharacterHandRotation(ctrlGetCharaByName(ryo), 1, 21, &rotation);
        Hand(ryo, 1, ringingHand, 10);
        const Vector3 look{208.5f, 104.5f, 194.5f};
        SetEyeToChara(ctrlGetCharaByName(ryo), &look, 20, 2);
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &look, 1);
        QueueEventCallFunction(ResetEye, nullptr, 170);
        Gravity(ryo, false);
        Gravity(ren, false);
        Pose(ryo, {209.10001f, 103.57f, 194.75f}, 6189);
        Motion(ryo, 0x2185, 0x40000000U);
        EVT_AT_MenCtrl({1, 10, 0, 0, 0, 0, 255, 0, 0, 0, 100});
        EVT_AT_FadeBGM(30, 1, 80);
        HLib::SetupTaskDelay(110);
        state->phase = 4;
        return;
    }
    if (state->phase <= 4) {
        if (!EV_ForceLoadNpc(yang))
            return;
        EV_SetCharacterAlphaEnabled(yang, false);
        EV_SetCharacterAlphaEnabled(ren, false);
        EV_SetCharacterAlphaEnabled(ryo, false);
        EV_stepSetFlags(0, 0x131);
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
        EnableScenarioCharacterDisplay(ren);
        EnableScenarioCharacterDisplay(ryo);
        DisableScenarioCharacterDisplay(SHEN_NAME(a047));
        DisableScenarioCharacterDisplay(SHEN_NAME(a257));
        SetDoorSubjectEnabled(bellDoor, 0);
        EnqueueTelephoneCord();
        if (state->renRouteCompletion)
            EV_npcUpdateFlags(ren);
        else
            EV_npcUnknownFunc(ren);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        EV_LoadAudio("0202A000", 0);
        EV_LoadCutsceneFromCurrentScene("0202A000");
        QueueSkAuthorStart(state->task, SHEN_NAME(BEL1), nullptr, nullptr, 0);
        snapAssignFrmCntFlags(1, 100);
        ++state->phase;
        return;
    }
    if (state->phase == 5) {
        EV_LoadAudio("0203A000", 0);
        EV_LoadCutsceneFromCurrentScene("0203A000");
        QueueSkAuthorStart(state->task, SHEN_NAME(BEL2), nullptr, nullptr, 0);
        ++state->phase;
    } else if (state->phase == 6) {
        HLib::SetCurrentTaskTo(BellExitCallback);
        ++state->phase;
    }
}
void BellExitCallback(void *parameter) {
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    EV_setHoldStatesFlags(1);
    SetDoorSubjectEnabled(bellDoor, 1);
    EV_FreeAuthorPak(0, "0202");
    EV_FreeAuthorPak(0, "0203");
    LoadCutsceneAndSoundProgramForArea();
    EV_setHoldStatesFlags(1);
    srchClearTarget(2);
    EV_SetCanDoAreaJumpTo(1);
    Gravity(ryo, true);
    Gravity(ren, true);
    EV_SetCharacterAlphaEnabled(ren, true);
    EV_SetCharacterAlphaEnabled(yang, true);
    EV_SetCharacterAlphaEnabled(ryo, true);
    hndlSetFlags(ctrlGetCharaByName(ren));
    EV_npcLockUnlock(ren);
    EV_npcLockUnlock(yang);
    EV_npcFree(ren);
    EV_npcFree(yang);
    EV_NPC_NewLoadSw(1);
    DeleteMotion(state->motion);
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BELL"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(state->task);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(QB00), 51);
}
const ScriptAreaDefinition areaBELLScriptDefinition{.areaId = SHEN_NAME(BELL), .field04 = 0, .init = BELL_init_func, .main = BELL_main_func};
} // namespace shenmue::s2
