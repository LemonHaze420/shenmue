#include "s2/areas/area_kach.hpp"
#include "s2/areas/kach_tables.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/pak.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
namespace {
constexpr shen_name taskName{SHEN_NAME(kach)}, eventName{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, base{SHEN_NAME(base)}, hand{SHEN_NAME(rhnd)},
    ball{SHEN_NAME(BALL)}, money{SHEN_NAME(KANE)}, dummy{SHEN_NAME(dumy)}, area{SHEN_NAME(area)};
KachRuntime machine;
HLib::Task *mainTask;
EventReadCharactersWork *readCharacters;
shen_name authorCharacters[]{ryo, SHEN_NAME(EYR_), shen_name{0xffffffffU}};
std::int32_t boardIndex, motionStyle, startingCash, selectionTimer, queuedVoiceCount;
char voicePrefixes[4][16], voiceQueue[16][12];
void ResetState() {
    machine.state = 0;
    machine.result = 0;
    machine.timer = 0;
}
void Next(HLib::TaskCallback callback) {
    HLib::SetCurrentTaskTo(callback);
}
void Camera(std::int32_t id, shen_name eye = base, shen_name target = base) {
    EventSetCamera request;
    request.cleanup = 0;
    request.camera = id;
    request.eye = eye;
    request.target = target;
    request.context = 0;
    request.angle = 0;
    request.delay = 0;
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void Place(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{0, character, parent, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Turn(shen_name character, shen_name parent, float angle) {
    EventSetAngle request{0, character, parent, angle, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Rotate(shen_name character, shen_name parent, const Vector3i &angles) {
    EventSetRawAngle request{0, character, parent, angles, 0};
    EVT_AT_SetAng_Callback(&request);
}
void Motion(shen_name character, std::uint32_t animation, float speed = 1, std::uint32_t flags = 0x40000000U) {
    EventSetMotion request{0, character, animation, 0, speed, flags, 0};
    EVT_AT_SetMot_Callback(&request);
    (void)GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
void Face(FacialExpressionId expression) {
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(machine.dealer), expression, 16, 1);
}
void Pose(shen_name character, std::int32_t *commands, std::int16_t ticks) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(character), 0, reinterpret_cast<std::byte *>(commands), ticks);
}
void Help(std::uint32_t visible) {
    CheckHelpIcon(visible, 0, CheckHelpIconOp::Set);
}
void Append(std::int32_t speaker, const char *suffix) {
    char *const destination = voiceQueue[queuedVoiceCount];
    std::strcpy(destination, voicePrefixes[speaker]);
    std::strcat(destination, suffix);
    ++queuedVoiceCount;
}
void Immediate(const char *suffix) {
    char voice[64];
    std::strcpy(voice, voicePrefixes[1]);
    std::strcat(voice, suffix);
    EVT_AT_SetVoice(voice, 1, 0);
}
void WaitVoices(std::uint32_t flags) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KachVoiceWaitCallback, 6, 0x0b, sizeof(KachVoiceWaitWork), eventName);
    auto *const work = static_cast<KachVoiceWaitWork *>(HLib::GetTaskParameter(task));
    work->parent = mainTask;
    work->delay = 8;
    work->index = 0;
    work->flags = flags;
    voiceQueue[queuedVoiceCount][0] = 0;
    queuedVoiceCount = 0;
    if (work->parent)
        HLib::SetupTimedTask(work->parent, 0);
}
// @verified - p
std::uint32_t ChooseCashContinuation() {
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= machine.minimumBet) {
        Next(KachSelectRuleCallback);
        return 0;
    }
    Next(KachNoMoneyCallback);
    return 1;
}
void ResetBall() {
    machine.ballPosition.y = 0.86300004f;
    machine.bowlOrigin.y = 0.75f;
    machine.bowlOrigin.x = 0;
    if (motionStyle == 2) {
        machine.ballPosition.x = -0.090000004f;
        machine.bowlOrigin.z = -0.1f;
        machine.ballPosition.z = -0.2f;
    } else {
        machine.ballPosition.x = -0.12f;
        machine.ballPosition.z = 0;
        machine.bowlOrigin.z = 0;
    }
    machine.ballVelocity.x = 0.1f;
    const float random = GetRandomNumberInRange(0.2f);
    machine.soundFlags = 0;
    machine.ballVelocity.z = -0.60000002f - random;
    machine.ballVelocity.y = (-0.60000002f - random) / -3;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(base, &rotation, EventTransformFlags::Local);
    (void)UpdateKachBall(&machine.ballPosition, &machine.ballVelocity, &machine.bowlOrigin, static_cast<std::uint16_t>(rotation.y));
    EV_AT_SetPos(ball, EventTransformFlags::AllAxes, {});
}
void InviteAgain(bool won) {
    const std::int32_t choice = shenGetRandomInt32(3);
    Append(1, choice == 0 ? "035" : choice == 1 ? "036" : "037");
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) > startingCash)
        Append(1, shenGetRandomInt32(2) == 0 ? (won ? "039" : "043") : (won ? "038" : "042"));
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < startingCash)
        Append(1, shenGetRandomInt32(2) == 0 ? (won ? "041" : "045") : (won ? "040" : "044"));
    const std::uint32_t flags = ChooseCashContinuation() ? 0 : 0x20000000U;
    WaitVoices(flags);
}
} // namespace
// @verified - p
void InitKach() {
    motionStyle = 0;
    std::memset(&machine, 0, sizeof(machine));
    readCharacters = nullptr;
    std::memset(voicePrefixes, 0, sizeof(voicePrefixes));
    mainTask = nullptr;
    startingCash = 0;
    selectionTimer = 0;
    queuedVoiceCount = 0;
    std::memset(voiceQueue, 0, sizeof(voiceQueue));
}
// @verified - p
void QueueKach(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    mainTask = HLib::EnqueueTaskWithoutParameter(KachInitializeCallback, 4, 0x0d, taskName.name);
    boardIndex = static_cast<std::int32_t>(argument);
    machine.state = 0;
    machine.dealer = kachDealers[boardIndex].character;
    authorCharacters[1] = machine.dealer;
    readCharacters = Queue_EVT_AT_ReadChr(mainTask, authorCharacters);
    startingCash = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash));
    machine.maximumBet = kowloonBS[boardIndex].secondDigits;
    machine.minimumBet = kowloonBS[boardIndex].firstDigits;
}
// @verified - p
void KachVoiceWaitCallback(void *parameter) {
    auto &work = *static_cast<KachVoiceWaitWork *>(parameter);
    if (cvsHasValidConverseData(nullptr)) {
        if (work.voiceFrames++ > 600)
            scenCvsCtrlFree(nullptr);
        if (((work.flags & 0x40000000U) && CheckMediatedDoublePress()) || HasPressedCancel())
            scenCvsCtrlFree(nullptr);
    } else {
        const char *const voice = voiceQueue[work.index];
        if (voice[0]) {
            EVT_AT_SetVoice(voice, 1, work.delay);
            if (work.delay > 0)
                HLib::SetupTaskDelay(static_cast<std::uint16_t>(work.delay));
            ++work.index;
            work.voiceFrames = 0;
            if (voiceQueue[work.index][0] || !(work.flags & 0x20000000U))
                return;
        }
        if (work.parent)
            HLib::ResolveDeferredCallback(work.parent);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void KachInitializeCallback(void *) {
    SetLoadingDrawFlags(3);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KACH"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KACH"), 0);
    EV_EventSetUpHelpIcon_0(0x76, 0);
    Help(1);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0292E000");
    EV_LoadAudio("0292E000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0292");
    Place(base, shen_name{0xffffffffU}, kowloonBS[boardIndex].position);
    Turn(base, shen_name{0xffffffffU}, kowloonBS[boardIndex].angleY);
    npcSetAreaName(area, &kowloonBS[boardIndex].position, 2, 1);
    const Vector3 scale{0.5f, 0.5f, 0.5f};
    SetMotionworkFlagForCharaById(ball, &scale);
    modelSetCharaAlpha(ctrlGetCharaByName(hand), 0.40000001f);
    AT_SetLocalBaseChara(ball, base);
    AT_SetLocalBaseChara(hand, base);
    SetScenarioCharacterDrawControl(ball, 2, 0);
    EV_DisableCharacterDisplay(ball);
    Place(dummy, base, {0, 0, 0.85000002f});
    Turn(dummy, base, 0);
    Place(hand, base, {0.31f, 0.85000002f, 0.30000001f});
    Rotate(hand, base, {32768, 32768, 45056});
    if (GetMotionRigBoneId(scenFindMotionwork(machine.dealer)) == 6)
        motionStyle = 0;
    else
        motionStyle = (GetMotionRigBoneId(scenFindMotionwork(machine.dealer)) == 5) + 1;
    (void)FindMotionWork(ctrlGetCharaByName(machine.dealer));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(machine.dealer), FacialNodeReplacement);
    LoadBlurModelForCharacter(machine.dealer, nullptr, 0);
    SetLowQualityHands(ctrlGetCharaByName(hand));
    SetLowQualityHands(ctrlGetCharaByName(machine.dealer));
    SetHighModelXYZ(ctrlGetCharaByName(hand), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(machine.dealer), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(machine.dealer), 1, 1, 0);
    ResetBall();
    EV_NPC_NewLoadSw(1);
    std::strcpy(voicePrefixes[0], "0292A");
    switch (machine.dealer.name) {
    case SHEN_NAME(KD3_):
        std::strcpy(voicePrefixes[1], "0292D");
        break;
    case SHEN_NAME(KD4_):
        std::strcpy(voicePrefixes[1], "0292E");
        break;
    case SHEN_NAME(KD5_):
        std::strcpy(voicePrefixes[1], "0292F");
        break;
    case SHEN_NAME(EYR_):
        std::strcpy(voicePrefixes[1], "0292C");
        break;
    case SHEN_NAME(TSY_):
        std::strcpy(voicePrefixes[1], "0292B");
        break;
    default:
        break;
    }
    ResetState();
    Next(KachPostLoadCallback);
    QueueSkNpcWait(mainTask);
}
// @verified - p
void KachPostLoadCallback(void *) {
    ResetState();
    Next(KachGreetingCallback);
    RetireLoadingDraw();
}
// @verified - p
void KachGreetingCallback(void *) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Place(ryo, base, {0, 0, 1.9f});
    Turn(ryo, base, 0);
    Place(machine.dealer, base, {0, 0, -0.69999999f});
    Turn(machine.dealer, base, 180);
    Motion(ryo, 0xf002);
    Motion(machine.dealer, kachIdleMotions[motionStyle], 1, 0x50000000);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetPoint_Callback, 6, 0x0b, sizeof(EventSetPoint), eventName);
    RegisterEventTask(task);
    auto *const point = static_cast<EventSetPoint *>(HLib::GetTaskParameter(task));
    point->character = machine.dealer;
    point->target = ryo;
    point->targetBone = 5;
    point->trackingMode = 2;
    point->positionMode = 0;
    point->duration = 0;
    point->flags = 0x20000000;
    point->frame = 0;
    point->cleanup = 1;
    point->delay = 2;
    HLib::Task *const cameraTask = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), eventName);
    RegisterEventTask(cameraTask);
    auto *const camera = static_cast<EventSetCamera *>(HLib::GetTaskParameter(cameraTask));
    camera->cleanup = 1;
    camera->camera = 1001;
    camera->eye = machine.dealer;
    camera->target = machine.dealer;
    camera->context = 0;
    std::memset(camera->reserved11, 0, sizeof(camera->reserved11));
    camera->delay = 1;
    const std::int32_t choice = shenGetRandomInt32(3);
    Append(1, choice == 0 ? "001" : choice == 1 ? "002" : "003");
    (void)ChooseCashContinuation();
    WaitVoices(0);
}
// @verified - p
void KachSelectRuleCallback(void *) {
    if (!machine.state) {
        Camera(1102);
        EventSetPositionAngle request{0, ryo, base, {0, 0, 1.2f}, 0, 0};
        EVT_AT_SetPosAng_Callback(&request);
        Place(machine.dealer, base, {0, 0, -0.69999999f});
        Turn(machine.dealer, base, 180);
        Motion(ryo, 0xf002);
        Motion(machine.dealer, kachIdleMotions[motionStyle], 1, 0x50000000);
        Face(Normal);
        EVT_AT_SetPoint(ryo, machine.dealer, 5, 3, 0, 0, 0, 0x20000000);
        EVT_AT_SetPoint(machine.dealer, ryo, 5, 3, 0, 0, 0, 0x20000000);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkChoiceWaitCallback, 6, 0x0b, sizeof(SkChoiceWaitWork), eventName);
        auto *const work = static_cast<SkChoiceWaitWork *>(HLib::GetTaskParameter(task));
        EV_SetAuthorActiveTo(0);
        work->self = task;
        work->parent = mainTask;
        work->choices[0] = -1;
        work->choices[1] = 36;
        work->choices[2] = 369;
        work->choices[3] = 34;
        work->timeout = 0x7fff;
        work->reserved1a = 0;
        work->defaultChoice = 154;
        work->phase = 0;
        HLib::SetupTimedTask(mainTask, 0);
        ++machine.state;
    } else if (machine.state == 1) {
        const auto choice = shenGetCtrlSpeakIconFlag();
        ResetState();
        Next(choice == 36 ? KachAcceptRulesCallback : choice == 369 ? KachExplainRulesCallback : KachQuitCallback);
        machine.state = 0;
    }
}
// @verified - p
void KachAcceptRulesCallback(void *) {
    Camera(1103, ryo, ryo);
    Append(0, shenGetRandomInt32(2) == 0 ? "002" : "001");
    Append(1, "004");
    Append(1, "005");
    ResetState();
    Next(KachBetCallback);
    WaitVoices(0);
}
// @verified - p
void KachQuitCallback(void *) {
    Camera(1103, ryo, ryo);
    Face(Normal);
    Append(0, shenGetRandomInt32(2) == 0 ? "011" : "010");
    Append(1, "046");
    ResetState();
    Next(KachExitCallback);
    WaitVoices(0);
}
// @verified - p
void KachNoMoneyCallback(void *) {
    Camera(1103, ryo, ryo);
    Face(Normal);
    Append(0, "004");
    Append(1, "010");
    ResetState();
    Next(KachExitCallback);
    WaitVoices(0);
}
// @verified - p
void KachExplainRulesCallback(void *) {
    if (!machine.state) {
        Camera(1103, ryo, ryo);
        Append(0, "003");
        ++machine.state;
        WaitVoices(0);
    } else if (machine.state == 1) {
        Camera(motionStyle == 2 ? 5021 : 1021);
        Append(1, "006");
        Append(1, "007");
        Append(1, "008");
        Append(1, "009");
        machine.state = 0;
        if (!shenGetMinutesUntilNextFrameMinute()) {
            Next(KachSleepCallback);
            WaitVoices(0);
        } else
            WaitVoices(ChooseCashContinuation() ? 0 : 0x20000000U);
    }
}
// @verified - p
void KachSleepCallback(void *) {
    Face(Normal);
    Camera(1104);
    Motion(machine.dealer, kachSleepMotions[motionStyle]);
    Append(1, "047");
    Append(0, "012");
    Append(1, "048");
    Next(KachExitCallback);
    WaitVoices(0);
}
// @verified - p
void KachBetCallback(void *) {
    if (!machine.state) {
        Camera(1100);
        QueueSkMoneyIconWait(mainTask, static_cast<std::int16_t>(machine.maximumBet), static_cast<std::int16_t>(machine.minimumBet));
        EVT_AT_SetMot(machine.dealer, kachIdleMotions[motionStyle], 0, 1, 0x50000000, 0);
        EVT_AT_SetPoint(ryo, machine.dealer, 5, 3, 0, 0, 0, 0x20000000);
        ++machine.state;
    } else if (machine.state == 1) {
        machine.bet = GetNumBet();
        if (machine.bet <= 0) {
            ResetState();
            Next(KachQuitCallback);
        } else {
            SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash) - machine.bet));
            Append(1, shenGetRandomInt32(2) == 0 ? "012" : "011");
            ResetState();
            Next(KachSelectPointCallback);
            WaitVoices(0x20000000);
        }
    }
}
// @verified - p
void KachSelectPointCallback(void *) {
    if (!machine.state) {
        selectionTimer = 0;
        Pose(hand, kachSelectHandPose, 0);
        Camera(1020);
        machine.handPosition = kachChoicePositions[0];
        machine.selection = 0;
        machine.handTarget = {};
        EV_EnableCharacterDisplay(hand);
        EV_EnableCharacterDisplay(money);
        Help(0);
        Help(32);
        AT_SetLocalBaseChara(money, hand);
        Place(money, hand, {0.2f, -0.18f, -0.5f});
        Rotate(money, hand, {4096, 0, 20480});
        EV_AT_SetPos(hand, EventTransformFlags::AllAxes, machine.handPosition);
        Rotate(hand, base, {32768, 32768, 45056});
        ++machine.state;
        return;
    }
    if (machine.state != 1)
        return;
    GetBattleDataConfig(0, SHEN_NAME(BTBA));
    if (selectionTimer++ > 10800) {
        Help(0);
        Append(0, "011");
        EV_EnableCharacterDisplay(ryo);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), eventName);
        RegisterEventTask(task);
        *static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task)) = {1, static_cast<std::uint32_t>(machine.bet), 40};
        Next(KachExitCallback);
        WaitVoices(0);
        return;
    }
    std::int32_t nearest = 0;
    float closest = 1000;
    for (std::int32_t i = 0; i < 4; ++i) {
        const auto &p = kachChoicePositions[i];
        const float x = machine.handPosition.x - p.x, y = machine.handPosition.y - p.y, z = machine.handPosition.z - p.z;
        const float distance = x * x + y * y + z * z;
        if (closest > distance) {
            closest = distance;
            nearest = i;
        }
    }
    const auto pressed = static_cast<std::uint16_t>(GetCurrentControlInput(7));
    std::int32_t direction = -1;
    switch (pressed) {
    case 0x1000:
        direction = 0;
        break;
    case 0x5000:
        direction = 1;
        break;
    case 0x4000:
        direction = 2;
        break;
    case 0x6000:
        direction = 3;
        break;
    case 0x2000:
        direction = 4;
        break;
    case 0xa000:
        direction = 5;
        break;
    case 0x8000:
        direction = 6;
        break;
    case 0x9000:
        direction = 7;
        break;
    default:
        break;
    }
    if (direction >= 0) {
        machine.selection = kachNeighbours[nearest][direction];
        const char *const label = kachChoiceLabels[machine.selection];
        if (label) {
            ctrlStopSubtitles();
            const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            SCEN_ShowEventMessage(control->scenarioTask, 0, label);
        }
    }
    machine.handTarget = kachChoicePositions[machine.selection];
    machine.handPosition.x = machine.handTarget.x * 0.2f + machine.handPosition.x * 0.80000001f;
    machine.handPosition.y = machine.handTarget.y * 0.2f + machine.handPosition.y * 0.80000001f;
    machine.handPosition.z = machine.handTarget.z * 0.2f + machine.handPosition.z * 0.80000001f;
    EV_AT_SetPos(hand, EventTransformFlags::AllAxes, machine.handPosition);
    Rotate(hand, base, {32768, 32768, 45056});
    const float x = machine.handPosition.x - machine.handTarget.x, y = machine.handPosition.y - machine.handTarget.y,
                z = machine.handPosition.z - machine.handTarget.z;
    if ((pressed & 0x400) && std::sqrt(x * x + y * y + z * z) < 0.01f) {
        Help(0);
        Help(1);
        Pose(hand, kachReleaseCoinPose, 4);
        auto *const task = HLib::EnqueueTaskWithoutParameter(KachFallMoneyCallback, 4, 0x0b, SHEN_NAME(mony));
        HLib::SetupTimedTask(task, 4);
        AT_SetLocalBaseChara(money, shen_name{0xffffffffU});
        Append(0, shenGetRandomInt32(2) == 0 ? "009" : "008");
        ResetState();
        Next(KachThrowCallback);
        WaitVoices(0);
    }
}
// @verified - p
void KachFallMoneyCallback(void *) {
    AT_SetLocalBaseChara(money, base);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(money, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(money, &rotation, EventTransformFlags::Local);
    position.y += -0.0099999998f;
    if (position.y < 0.80000001f) {
        position.y = 0.80000001f;
        Loop3DSoundEffect(0x4cc);
        HLib::DestroyCurrentTask();
    }
    EV_AT_SetPos(money, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(money, EventTransformFlags::AllAxes, rotation);
}
// @verified - p
void KachLocalDebugNullsub() {
}
// @verified - p
void KachThrowCallback(void *) {
    ++machine.timer;
    const char *const outcomes[]{"015", "016", "017", "018", "019", "020", "024", "025", "026", "021", "022", "023"};
    if (!machine.state) {
        const auto flag = kachDealers[boardIndex].playedFlag;
        if (flag)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, flag, 1);
        EV_DisableCharacterDisplay(hand);
        EV_DisableCharacterDisplay(money);
        Immediate(shenGetRandomInt32(2) == 0 ? "014" : "013");
        EVT_AT_SetMot(machine.dealer, kachThrowMotions[motionStyle], 0, 1, 0x50000000, 0);
        Camera(motionStyle == 2 ? 5003 : 1003);
        EVT_AT_DispBlur(machine.dealer, 4, 15, 3, 45, 0);
        Pose(machine.dealer, kachDealerRestPose, 0);
        Queue_EVT_AT_CtrlHandUp_1(machine.dealer, 0, reinterpret_cast<std::byte *>(kachDealerThrowPose), 10, 90);
        Queue_EVT_AT_CtrlHandUp_1(machine.dealer, 0, reinterpret_cast<std::byte *>(kachDealerFinishPose), 10, 150);
        EVT_AT_SetPoint(machine.dealer, shen_name{}, 0, 3, 0, 0, 0, 0x20000000);
        ResetBall();
        ++machine.state;
        machine.result = 0;
        HLib::SetupTaskDelay(90);
    } else if (machine.state == 1) {
        KachLocalDebugNullsub();
        Help(0);
        EV_EnableCharacterDisplay(ball);
        HLib::EnqueueTaskWithoutParameter(KachRollCallback, 4, 0x0d, taskName.name);
        Loop3DSoundEffect(0x4c3);
        ++machine.state;
    } else if (machine.state == 2 && machine.result) {
        machine.result &= 3;
        FinishLoading();
        Immediate(outcomes[machine.result * 3 + shenGetRandomInt32(3)]);
        const bool won = machine.selection == static_cast<std::int32_t>(machine.result);
        ResetState();
        HLib::AssignCallbackForTask(mainTask, won ? KachWinCallback : KachLoseCallback);
        HLib::SetupTimedTask(mainTask, 60);
    }
}
// @verified - p
void KachWinCallback(void *) {
    if (!machine.state) {
        Camera(1101, machine.dealer, machine.dealer);
        Motion(machine.dealer, kachWinMotions[motionStyle], 0.69999999f);
        Face(Pitiable);
        AddThisToCashOrSegaCoins(4 * machine.bet, ECashOrCoins::Cash2);
        const auto choice = shenGetRandomInt32(3);
        Append(1, choice == 0 ? "027" : choice == 1 ? "028" : "029");
        ++machine.state;
        WaitVoices(0);
    } else if (!GetShouldRenderIcons(RenderIconSlot::Any)) {
        if (!shenGetMinutesUntilNextFrameMinute()) {
            Next(KachSleepCallback);
            machine.state = 0;
            return;
        }
        InviteAgain(true);
        machine.state = 0;
    }
}
// @verified - p
void KachLoseCallback(void *) {
    Camera(1101, machine.dealer, machine.dealer);
    Motion(machine.dealer, kachLoseMotions[motionStyle], 0.69999999f);
    Face(Smile);
    const auto choice = shenGetRandomInt32(3);
    Append(1, choice == 0 ? "030" : choice == 1 ? "031" : "032");
    Append(1, shenGetRandomInt32(2) == 0 ? "034" : "033");
    if (!shenGetMinutesUntilNextFrameMinute()) {
        Next(KachSleepCallback);
        WaitVoices(0);
    } else
        InviteAgain(false);
}
// @verified - p
void KachExitCallback(void *) {
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    ResetState();
    HLib::AssignCallbackForTask(mainTask, KachCleanupCallback);
    HLib::SetupTimedTask(mainTask, 30);
    spmFadeOut();
}
// @verified - p
void KachCleanupCallback(void *) {
    npcSetAreaName(area, nullptr, 0, 0);
    EventSetPoint point;
    point.character = ryo;
    point.target = {};
    point.targetBone = 0;
    point.trackingMode = 3;
    point.positionMode = 0;
    point.duration = 0;
    point.flags = 0x10000000;
    point.cleanup = 0;
    point.delay = 0;
    EVT_AT_SetPoint_Callback(&point);
    point.character = machine.dealer;
    EVT_AT_SetPoint_Callback(&point);
    EV_EventSetUpHelpIcon_0(0, 0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    (void)FindMotionWork(ctrlGetCharaByName(machine.dealer));
    hndlSetFlags(ctrlGetCharaByName(hand));
    Help(0);
    EV_FreeAuthorPak(0, "0292");
    RefreshPadControlData(0, 1);
    RefreshPadControlData(1, 1);
    camCtrlSetFovTransitionTo(false);
    ET_AT_KillWork(readCharacters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(mainTask);
    Motion(ryo, 0xf002);
    EV_NPC_NewLoadSw(1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KACH"), 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
// @verified - p
void KachRollCallback(void *) {
    machine.result = UpdateKachBall(&machine.ballPosition, &machine.ballVelocity, &machine.bowlOrigin, 0x2000);
    EV_AT_SetPos(ball, EventTransformFlags::AllAxes, machine.ballPosition);
    if ((machine.result & 0xffffff00U) && !(machine.soundFlags & 0xff00)) {
        if (machine.soundFlags & 2)
            Loop3DSoundEffect(1264);
        else {
            Loop3DSoundEffect(1261);
            machine.soundFlags |= 2;
        }
        machine.soundFlags = static_cast<std::uint8_t>(machine.soundFlags) | 0x1000U;
    }
    if (machine.soundFlags & 0xff00)
        machine.soundFlags -= 256;
    machine.result = static_cast<std::uint8_t>(machine.result);
    if (machine.result)
        HLib::DestroyCurrentTask();
}
const ScriptAreaDefinition kachScriptDefinition{.areaId = SHEN_NAME(KACH), .field04 = 0, .init = InitKach, .main = QueueKach};
} // namespace shenmue::s2
