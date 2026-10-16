#include "s2/areas/area_armw.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
// @verified - p
void ArmwVoiceWaitCallback(void *data) {
    ArmwVoiceWait &work = *static_cast<ArmwVoiceWait *>(data);
    bool finished = false;
    if (!cvsHasValidConverseData(0)) {
        if (armw.voices[work.index][0]) {
            EVT_AT_SetVoice(armw.voices[work.index], work.delay);
            if (work.delay > 0)
                HLib::SetupTaskDelay(work.delay);
            ++work.index;
            work.timeout = 0;
        } else
            finished = true;
    } else {

        const float tickDelta = g_stabilizeFps ? GetFrameTime() * 30.0f : 1.0f;
        work.timeout += tickDelta > 150.0f;
        if (work.timeout != 0)
            scenCvsCtrlFree(0);
        if (HasPressedCancel()) {
            if (work.flags & 0x40000000U)
                scenCvsCtrlFree(0);
            if (work.flags & 0x80000000U) {
                scenCvsCtrlFree(0);
                armw.voiceSkipped = 1;
                finished = true;
            }
        }
    }
    if (finished) {
        if (work.parent)
            HLib::ResolveDeferredCallback(work.parent);
        HLib::DestroyCurrentTask();
    }
    if ((work.flags & 0x80000000U) && HasPressedCancel()) {
        scenCvsCtrlFree(0);
        armw.voiceSkipped = 1;
        if (work.parent)
            HLib::ResolveDeferredCallback(work.parent);
        HLib::DestroyCurrentTask();
    }
}
void Position(shen_name character, shen_name parent, float x, float y, float z, float yaw, std::int32_t delay = 0) {
    EVT_AT_SetPos(character, parent, {x, y, z}, delay);
    EVT_AT_SetAng_Enqueue(character, parent, 0, yaw, 0, delay);
}
void Cling(shen_name character, const Vector3 &position, const Vector3i &rotation, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling &work = *static_cast<EventCling *>(HLib::GetTaskParameter(task));
    work.cleanup = 1;
    work.target = ARMW_MONEY;
    work.character = character;
    work.bone = 14;
    work.position = position;
    work.rotation = rotation;
    work.mode = 0;
    work.delay = static_cast<float>(delay);
}
} // namespace
void ArmwPosition(shen_name character, shen_name parent, const Vector3 &position, std::int32_t yaw) {
    EventSetPosition pos{.cleanup = 0, .character = character, .parent = parent, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&pos);
    EventSetRawAngle angle{.cleanup = 0, .character = character, .parent = parent, .rotation = {0, yaw, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
}
void ArmwCamera(std::int32_t camera, std::int32_t delay) {
    EVT_AT_SetCam(camera, ARMW_BASE, ARMW_BASE, 0, 0, delay);
    if (!delay)
        camCtrlDisableFilmBars();
}
// @verified - p
void ArmwSetVoicePrefixes(shen_name referee, shen_name opponent) {
    std::strcpy(armw.voicePrefixes[0], referee.name == SHEN_NAME(WD1_) ? "0368A" : "0008A");
    switch (referee.name) {
    case SHEN_NAME(SD1_):
        std::strcpy(armw.voicePrefixes[1], "0008M");
        break;
    case SHEN_NAME(WD1_):
        std::strcpy(armw.voicePrefixes[1], "0368I");
        break;
    case SHEN_NAME(AD2_):
        std::strcpy(armw.voicePrefixes[1], "0008I");
        break;
    case SHEN_NAME(WD3_):
        std::strcpy(armw.voicePrefixes[1], "0008G");
        break;
    case SHEN_NAME(GAU_):
        std::strcpy(armw.voicePrefixes[1], "0008E");
        break;
    }
    const char *dialogue = nullptr;
    const char *effort = nullptr;
    switch (opponent.name) {
    case SHEN_NAME(HNG_):
        dialogue = referee.name == SHEN_NAME(WD1_) ? "0368J" : "0008E";
        effort = referee.name == SHEN_NAME(WD1_) ? "0368D" : "0008D";
        break;
    case SHEN_NAME(SF8_):
        dialogue = "0008G";
        effort = "0008F";
        break;
    case SHEN_NAME(SFC_):
        dialogue = "0008I";
        effort = "0008H";
        break;
    case SHEN_NAME(KZN_):
        dialogue = "0008M";
        effort = "0008J";
        break;
    case SHEN_NAME(SSY_):
        dialogue = "0008N";
        effort = "0008K";
        break;
    case SHEN_NAME(SF2_):
        dialogue = "0008O";
        effort = "0008L";
        break;
    case SHEN_NAME(FRS_):
        dialogue = "0368I";
        effort = "0368C";
        break;
    case SHEN_NAME(MHK_):
        dialogue = "0368K";
        effort = "0368E";
        break;
    case SHEN_NAME(DML_):
        dialogue = "0368L";
        effort = "0368F";
        break;
    case SHEN_NAME(MSH_):
        dialogue = "0368M";
        effort = "0368G";
        break;
    case SHEN_NAME(DGY_):
        dialogue = "0368N";
        effort = "0368H";
        break;
    }
    if (dialogue) {
        std::strcpy(armw.voicePrefixes[2], dialogue);
        std::strcpy(armw.voicePrefixes[3], effort);
    }
}
void ArmwAppendVoice(std::int32_t speaker, const char *suffix) {
    std::strcpy(armw.voices[armw.voiceCount], armw.voicePrefixes[speaker]);
    std::strcat(armw.voices[armw.voiceCount++], suffix);
}
void ArmwVoice(std::int32_t speaker, const char *suffix, std::int32_t delay) {
    char voice[64];
    std::strcpy(voice, armw.voicePrefixes[speaker]);
    std::strcat(voice, suffix);
    EVT_AT_SetVoice(voice, delay);
}
// @verified - p
void ArmwWaitVoices(HLib::Task *parent, std::uint32_t flags) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ArmwVoiceWaitCallback, 6, 0x0b, sizeof(ArmwVoiceWait), AREA_EVENT_TASK);
    ArmwVoiceWait &work = *static_cast<ArmwVoiceWait *>(HLib::GetTaskParameter(task));
    work.parent = parent;
    work.delay = 8;
    work.index = 0;
    work.flags = flags;
    work.reserved10 = 0;
    work.timeout = 0;
    armw.voices[armw.voiceCount][0] = 0;
    armw.voiceCount = 0;
    armw.voiceSkipped = 0;
    if (parent)
        HLib::SetupTimedTask(parent, 0);
}
std::int32_t ArmwIntroduceOpponent(ArmwWork &work) {
    std::uint32_t motion = 0;
    std::int32_t camera = 0, frames = 0;
    switch (work.opponent.name) {
    case SHEN_NAME(HNG_):
    case SHEN_NAME(MSH_):
    case SHEN_NAME(SSY_):
        motion = 9305;
        camera = 2502;
        frames = 220;
        break;
    case SHEN_NAME(SF8_):
    case SHEN_NAME(SF2_):
    case SHEN_NAME(DML_):
        motion = 9235;
        camera = 2501;
        frames = 210;
        break;
    case SHEN_NAME(SFC_):
    case SHEN_NAME(MHK_):
    case SHEN_NAME(DGY_):
        motion = 9304;
        camera = 2502;
        frames = 220;
        break;
    case SHEN_NAME(KZN_):
    case SHEN_NAME(FRS_):
        motion = 9363;
        camera = 2503;
        frames = 120;
        break;
    default:
        return 0;
    }
    Position(work.player, ARMW_BASE, 0, 0, -1, 0);
    Position(work.opponent, ARMW_BASE, 0, 0, 0.8f, 0);
    Position(work.referee, ARMW_BASE, 0.6f, 0, 0, 0);
    EVT_AT_SetMot(work.player, 9159, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.opponent, motion, 0, 1, 0x40000000U, 0);
    ArmwCamera(camera);
    return frames;
}
std::int32_t ArmwWarnFalseStart(ArmwWork &work) {
    EVT_AT_SetMot(work.player, 9155, 80, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[6][armw.opponentMotion], 80, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 0);
    ArmwPosition(work.opponent, work.player, {-0.15f, 0, -1.4f}, 32768);
    ArmwPosition(work.referee, ARMW_BASE, {0.8f, 0, 0.3f}, 8192);
    ArmwCamera(2000);
    EVT_AT_SetSound(345, 60);
    EVT_AT_SetSound(188, 70);
    EVT_AT_SetSound(271, 80);
    std::int32_t frames = 30;
    switch (work.falseStarts) {
    case 0:
        Position(work.referee, ARMW_BASE, 0.8f, 0, 0.3f, 90, 30);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[4][armw.refereeMotion], 45, 1, 0x40000000U, 30);
        ArmwVoice(1, "023", 30);
        ArmwVoice(1, "024", 170);
        ArmwCamera(2001, 30);
        ArmwCamera(2002, 120);
        EVT_AT_ObjectDisp_1(work.opponent, 120, 1);
        EVT_AT_ObjectDisp_1(work.opponent, 270, 0);
        ArmwVoice(0, "012", 10);
        frames = 270;
        break;
    case 1:
        Position(work.referee, ARMW_BASE, 0.6f, 0, 0.4f, 67.5f, 30);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[5][armw.refereeMotion], 80, 1, 0x40000000U, 30);
        ArmwVoice(1, "025", 30);
        ArmwVoice(1, "026", 100);
        ArmwCamera(2003, 30);
        ArmwCamera(2002, 100);
        EVT_AT_ObjectDisp_1(work.opponent, 100, 1);
        EVT_AT_ObjectDisp_1(work.opponent, 190, 0);
        ArmwVoice(0, "007", 10);
        frames = 190;
        break;
    case 2:
        Position(work.referee, ARMW_BASE, 0.7f, 0, 0.4f, 45, 30);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[6][armw.refereeMotion], 70, 1, 0x40000000U, 30);
        ArmwVoice(1, "027", 30);
        ArmwVoice(1, "028", 120);
        ArmwCamera(2004, 30);
        ArmwVoice(0, "010", 10);
        frames = 190;
        break;
    }
    EVT_AT_MenCtrl({.cleanup = 0,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(frames)});
    return frames + 30;
}
std::int32_t ArmwOfferRematch(ArmwWork &work) {
    if (work.flags & 0x10000000U) {
        ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
        ArmwPosition(work.referee, ARMW_BASE, {0.3f, 0, 0.6f}, 0);
        EVT_AT_SetMot(work.player, 61442, 0, 1, 0x40000000U, 0);
        const bool box = shenGetRandomInt32(2) == 0;
        if (box) {
            EV_EnableCharacterDisplay(ARMW_BOX);
            EVT_AT_ObjectDisp_1(ARMW_BOX, 300, 1);
            AT_SetLocalBaseChara(ARMW_BOX, ARMW_BASE);
            ArmwPosition(ARMW_BOX, ARMW_BASE, {0.3f, 0, 2.8f}, 12288);
        }
        ArmwPosition(work.opponent, ARMW_BASE, box ? Vector3{-0.3f, 0, 3.5f} : Vector3{-0.7f, 0, 3}, box ? 8192 : 61440);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[box ? 9 : 8][armw.refereeMotion], 90, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[box ? 10 : 9][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[box ? 10 : 9][armw.opponentMotion], 0, 1, 0x40000000U, 50);
        ArmwCamera(box ? 3202 : 3200);
        ArmwCamera(box ? 3203 : 3201, 60);
        if (box)
            ArmwCamera(3204, 180);
        return box ? 260 : 210;
    }
    constexpr const char *betVoices[]{
        "031", "032", "033", "034", "035", "036", "037", "038", "039", "036", "041", "042", "043", "044", "037", "046", "047", "048"};
    EV_DisableCharacterDisplay(work.opponent);
    EVT_AT_ObjectDisp_1(work.opponent, 300, 0);
    ArmwCamera(3400);
    ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
    ArmwPosition(work.referee, ARMW_BASE, {0.3f, 0, 0.6f}, 0);
    EVT_AT_SetMot(work.player, 61442, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[3][armw.refereeMotion], 0, 1, 0x40000000U, 0);
    ArmwVoice(1, "019");
    const std::int32_t index = armw.bet / 10 - 2;
    if (index < 17)
        ArmwVoice(1, betVoices[index], 40);
    ArmwVoice(1, "003", 130);
    ArmwVoice(1, "020", 250);
    return 300;
}
std::int32_t ArmwOpponentCollects(ArmwWork &work) {
    std::uint32_t motion = 0;
    float frame = 0;
    std::int32_t delay = 0, frames = 0, yaw = 8192;
    Vector3 money;
    switch (work.opponent.name) {
    case SHEN_NAME(HNG_):
    case SHEN_NAME(MSH_):
    case SHEN_NAME(SSY_):
        motion = 9300;
        frame = 20;
        delay = 20;
        frames = 80;
        money = {0.07f, 0.9f, 0.24f};
        break;
    case SHEN_NAME(SF8_):
    case SHEN_NAME(SF2_):
    case SHEN_NAME(DML_):
        motion = 9233;
        frame = 30;
        delay = 23;
        frames = 130;
        money = {0.2f, 0.9f, 0.2f};
        break;
    case SHEN_NAME(SFC_):
    case SHEN_NAME(MHK_):
        motion = 9298;
        frame = 80;
        delay = 110;
        frames = 160;
        money = {0.2f, 0.9f, 0.15f};
        break;
    case SHEN_NAME(DGY_):
        motion = 9296;
        frame = 20;
        delay = 10;
        frames = 160;
        money = {0.14f, 0.9f, 0.2f};
        break;
    case SHEN_NAME(KZN_):
    case SHEN_NAME(FRS_):
        motion = 9361;
        frame = 80;
        delay = 90;
        frames = 130;
        money = {0.3f, 0.9f, 0.3f};
        yaw = 57344;
        break;
    default:
        return 0;
    }
    ArmwCamera(3300);
    ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
    ArmwPosition(work.referee, ARMW_BASE, {0.75f, 0, -0.1f}, 8192);
    ArmwPosition(work.opponent, ARMW_BASE, {-0.6f, 0, 0.6f}, yaw);
    ArmwPosition(ARMW_MONEY, ARMW_BASE, money, 0);
    if (yaw == 57344)
        ArmwPosition(ARMW_MONEY, ARMW_BASE, {0.2f, 0.9f, 0.2f}, 0);
    EVT_AT_SetMot(work.player, 9159, 20, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.opponent, motion, frame, 1, 0x40000000U, 0);
    Cling(work.opponent, {0.05f, 0, -0.04f}, {16384, 34679, 14563}, delay);
    return frames;
}
std::int32_t ArmwRefereeCollects(ArmwWork &work) {
    Position(work.player, ARMW_BASE, 0.5f, 0, -0.8f, 157.5f);
    Position(work.referee, ARMW_BASE, 0.5f, 0, 0.5f, 0);
    Position(work.opponent, ARMW_BASE, -0.6f, 0, 0.6f, 315);
    EVT_AT_SetMot(work.player, 9159, 100, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[3][armw.refereeMotion], 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[1][armw.opponentMotion], 0, 1, 0x40000000U, 0);
    ArmwCamera(3390);
    ArmwAppendVoice(1, "021");
    ArmwAppendVoice(1, "004");
    return 260;
}
} // namespace shenmue::s2::areas
