#include "s2/areas/area_WKKT.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/scene_data.hpp"
#include "s2/gauge.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/initialization.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/save.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/step_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <xmmintrin.h>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, kem{SHEN_NAME(KEM_)}, base{SHEN_NAME(BASE)}, absolute{0xffffffffU};
constexpr shen_name taskToken{SHEN_NAME(EVNT)};
struct TrainingWork {
    HLib::Task *self;
    HLib::Task *parent;
    EventReadCharactersWork *readCharacters;
    std::uint32_t elapsed, idleTicks, tries, phase;
    std::int32_t level, rank, action, leafA, leafB, smoke;
    std::uint8_t errorCount;
    std::byte reserved41[3];
    std::uint32_t history[65];
};
struct TrainingRoot {
    HLib::Task *self;
    HLib::Task *child;
};
struct FallingWork {
    HLib::Task *self;
    std::uint64_t reserved08;
    std::uint8_t slot;
    std::byte reserved11[3];
    std::uint32_t frame;
    std::int32_t level;
    std::uint32_t reserved1c;
};
static_assert(sizeof(TrainingWork) == 0x148);
static_assert(offsetof(TrainingWork, history) == 0x44);
static_assert(offsetof(TrainingWork, errorCount) == 0x40);
static_assert(sizeof(FallingWork) == 0x20);
const shen_name characters[]{ryo, kem, absolute};
const Vector3 stancePosition{1.75600004196167f, 0, .19220000505447388f};
const Vector3 trainingPosition{1.7f, 0, .2f};
const Vector3 basePosition{39.400001525878906f, 102.30000305175781f, 241.0f};
const Vector3 kemPosition{42.5f, 102.30000305175781f, 242.0f};
const Vector3 exitPosition{40.0f, 102.25f, 242.30000305175781f};
const Vector3 teachingPosition{3.4000000953674316f, 0, -1.399999976158142f};
const Vector3 leavesPosition{39.34000015258789f, 105.5f, 241.10000610351562f};
const Vector3 leavesVolume{.7f, .35f, .7f};
std::int32_t entry{}, maximumLevel{40}, levelDivisor{4}, grade{}, voiceParity{}, displayLevel{};
std::uint32_t animationFrames{};
float strength{}, strengthPeak{}, motionInputMargin{6}, leafCount{250};
bool descending{}, delayedCommand{}, gaugeVisible{};
std::array<std::int32_t, 5> taught{};
std::array<std::uint32_t, 4> pooledLeaves{}, leafSlots{};
HLib::Task *gaugeTask{};
LegacyIconSprite gaugeSprite{};
LegacyColourQuad gaugeBackground{}, gaugeFill{};
void MakeTask(void *);
void Root(void *);
void First(void *);
void Auto1(void *);
void Auto2(void *);
void Auto3(void *);
void Training(void *);
void Action(void *);
void Stop(void *);
void End(void *);
void Talk(void *);
void ActionWait(void *);
void Success(void *);
void Gauge(void *);
void FallLeaves(void *);
void KillLeaves(void *);
void WaitNpc(void *);
void FadeIn(std::uint32_t ticks = 30) {
    SetScreenFilmFade(ticks, 255, 0, 0, 0, 0, 0, 0, 0);
}
void FadeOut(std::uint32_t ticks = 30) {
    SetScreenFilmFade(ticks, 0, 0, 0, 0, 255, 0, 0, 0);
}
void Position(shen_name name, shen_name parent, const Vector3 &value) {
    EventSetPosition request{0, name, parent, value, 0};
    EVT_AT_SetPosRot(&request);
}
void Angle(shen_name name, shen_name parent, float degrees) {
    EventSetAngle request{0, name, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Motion(shen_name name, std::uint32_t animation, std::uint32_t flags) {
    EventSetMotion request{0, name, animation, 0, 1, flags, 0};
    EVT_AT_SetMot_Callback(&request);
}
void Camera(std::int32_t id) {
    EventSetCamera request{0, id, base, base, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
}
void QueueCamera(std::int32_t id, std::int32_t delay) {
    EVT_AT_SetCam(id, base, base, 0, 0, delay);
}
void Voice(const char *code, std::int32_t delay = 0) {
    EVT_AT_SetVoice(code, 1, delay);
}
std::uint16_t Animation() {
    return GetMotionAnimationId(scenFindMotionwork(ryo), MotionRegion::LowerBody);
}
void HideGauge() {
    strength = -1;
    displayLevel = -1;
    SetFightGaugeVisible(0);
    gaugeVisible = false;
}
void SetRank(TrainingWork &work) {
    work.rank = work.level == maximumLevel ? 3 : work.level >= 28 ? 2 : work.level >= 14 ? 1 : 0;
}
void Continue(HLib::TaskCallback callback, std::uint32_t delay = 0) {
    HLib::SetCurrentTaskTo(callback);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
std::int32_t RejectInput(std::uint32_t *history) {
    const auto cursor = history[60];
    history[60] = 0;
    const auto input = history[cursor + 30];
    if (input & 4U)
        return 1;
    if (input & 0x400U)
        return 2;
    if (input & 2U)
        return 3;
    return (input & 0x200U) ? 4 : 5;
}
// @verified - p
std::int32_t MatchInput(std::uint32_t *history) {
    history[history[60]] = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_NEW_PRESSES));
    history[history[60] + 30] = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_HELD));
    const auto cursor = history[60];
    if (cursor == 0 && (history[history[61] + 30] & history[30]))
        return 6;
    if (!history[cursor + 30]) {
        if (!history[64]) {
            history[64] = 1;
            history[60] = cursor + 1;
        }
        return 6;
    }
    constexpr std::uint32_t required = 1028;
    const auto complete = [history]() {
        history[60] = history[61] = 0;
        history[0] = history[30] = history[62] = history[64] = 0;
        return 0;
    };
    if ((history[cursor] & required) == required)
        return complete();
    history[64] = 0;
    const auto pressed = history[cursor + 30];
    if (!(pressed & required))
        return RejectInput(history);
    const auto previous = cursor ? cursor - 1 : history[61];
    delayedCommand = true;
    if ((pressed & history[previous + 30]) == required) {
        EV_debug_log("WKKT\\eWKKT_et_striketree.c", 1404, "Delayed Command !!\n");
        return complete();
    }
    if ((pressed & required) == (required & history[previous + 30])) {
        history[61] = 0;
        return RejectInput(history);
    }
    history[60] = cursor + 1 > 30 ? 0 : cursor + 1;
    return 6;
}
// @verified - p
void PlayStrike(std::int32_t action) {
    if (action != 1 && action != 2)
        return;
    const auto animation = action == 1 ? 5185U : 371U;
    Motion(ryo, animation, 0x60000000U);
    const float context = GetMotionFrameCount(static_cast<std::uint16_t>(animation));
    Loop3DSoundEffect(0x88, context);
}
// @verified - p
void BufferIdle(TrainingWork &work) {
    work.action = MatchInput(work.history);
    if (work.action == 1 || work.action == 2) {
        PlayStrike(work.action);
        if (++work.errorCount > 10) {
            Voice("0026A043"); // @text EN: "Gather strength in my hands and\nlegs at the same time." | JA: "足と手に同時に\n力をこめるんだったな…"
            work.errorCount = 0;
        }
    } else if (work.action == 0)
        work.errorCount = 0;
    if (work.action >= 0 && work.action <= 2) {
        work.phase = 0;
        Continue(Action);
    }
    ++work.elapsed;
    ++work.idleTicks;
    if (work.elapsed > 5400) {
        HideGauge();
        work.phase = 0;
        Continue(Auto2);
    }
    if (static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_HELD)))
        work.idleTicks = 0;
}
// @verified - p
void BufferDuringMotion(TrainingWork &work) {
    if (Animation() != 5187) {
        const float end = GetMotionFrameCount(Animation());
        const float frame = GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody);
        if (frame >= end - motionInputMargin) {
            work.action = MatchInput(work.history);
            if (work.action == 1 || work.action == 2) {
                if (++work.errorCount > 10) {
                    Voice("0026A043"); // @text EN: "Gather strength in my hands and\nlegs at the same time." | JA: "足と手に同時に\n力をこめるんだったな…"
                    work.errorCount = 0;
                }
                ++work.phase;
            } else if (work.action == 0)
                ++work.phase;
        }
        if (end - frame < .02f && frame - end < .02f)
            EVT_AT_SetMot(ryo, 5187, 0, 1, 0x60000000U, 0);
    }
    if (SetMotionRegionFlag(scenFindMotionwork(ryo), 65, 1, MotionRegion::LowerBody) && static_cast<std::uint32_t>(work.action) > 2)
        EVT_AT_SetMot(ryo, 5187, 0, 1, 0x60000000U, -1);
}
// @verified - p
void UpdateStrength(bool cycling) {
    static constexpr float peaks[]{75.5999984741211f, 84.0f, 92.4000015258789f, 100.0f};
    const auto selectPeak = []() {
        strengthPeak = peaks[shenGetRandomInt32(4)];
        if (g_entryPoint == 97 && g_currentMap.name == SHEN_NAME(WKKT))
            strengthPeak = 100;
    };
    if (!cycling) {
        const auto animation = Animation();
        if (animation == 371 || animation == 5185)
            strength -= 10;
        else if (animation == 5188) {
            if (animationFrames++ > 45)
                strength -= 2;
        }
    } else if (strength < 0) {
        SetFightGaugeVisible(1);
        gaugeVisible = true;
        descending = false;
        selectPeak();
        strength = 0;
        animationFrames = 0;
    } else if (delayedCommand)
        delayedCommand = false;
    else if (!descending) {
        strength += 5;
        if (strength >= strengthPeak) {
            strength = strengthPeak;
            descending = true;
        }
    } else {
        strength -= 5;
        if (strength <= 0) {
            strength = 0;
            descending = false;
            selectPeak();
        }
    }
    UpdateSingleFightGauge(strength);
}
// @verified - p
void Training(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    GetBattleDataConfig(0, SHEN_NAME(BTPP));
    GetBattleDataConfig(0, SHEN_NAME(BTKK));
    GetBattleDataConfig(0, SHEN_NAME(BTCC));
    if (work.phase == 0) {
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        Position(ryo, base, trainingPosition);
        Angle(ryo, base, 90);
        ++work.phase;
    } else if (work.phase == 1) {
        if (Animation() == 5187)
            BufferIdle(work);
        else
            BufferDuringMotion(work);
    } else if (work.phase == 2 && (SetMotionRegionFlag(scenFindMotionwork(ryo), 65, 1, MotionRegion::LowerBody) || Animation() == 5187)) {
        if (work.action)
            PlayStrike(work.action);
        work.phase = 0;
        Continue(Action);
    }
    displayLevel = work.level;
    const auto animation = Animation();
    UpdateStrength(animation != 371 && animation != 5185 && animation != 5188);
    if (GetCurrentControlInput(INPUT_NEW_PRESSES) == 512 && Animation() == 5187) {
        work.phase = 0;
        Continue(Auto2);
    }
    if (shenGetMinutesUntilNextFrameMinute() == 0) {
        work.phase = 0;
        Continue(Auto3);
    }
}
std::uint32_t qteHand[]{0x5b0U, 0x13e7U, 0xb60eU, 0xe392U, 0xfc74U, 0xfd2aU, 0xb60U, 0x38cU, 0x7d0U, 0x2d6U, 0xfa50U, 0xb556U, 0x0U, 0xfd2aU, 0xb4a0U, 0x0U,
    0x16cU, 0xc225U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xbbbdU, 0x0U, 0x0U, 0xbb07U, 0x0U, 0x0U, 0xc4fbU, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xbf4aU, 0x0U, 0x0U,
    0xb8e4U, 0x0U, 0x0U, 0xc7d5U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xb3eaU, 0x0U, 0x0U, 0xb830U, 0x0U, 0xfbbdU, 0xcd83U, 0x0U, 0x0U, 0xfddeU};
std::uint32_t strikeHand[]{0x7cfU, 0xff4aU, 0xe71fU, 0x666U, 0x21fU, 0xfd2aU, 0x222U, 0xf71eU, 0x12d5U, 0x0U, 0xfd2aU, 0xdbbcU, 0x0U, 0xfa51U, 0xe5b0U, 0x0U,
    0x2d7U, 0x16bU, 0x0U, 0x0U, 0xfc74U, 0x0U, 0x0U, 0xe16dU, 0x0U, 0x0U, 0xe000U, 0x0U, 0xfe94U, 0xff4aU, 0x0U, 0x0U, 0xff4aU, 0x0U, 0x2d9U, 0xeaaaU, 0x0U,
    0xff4bU, 0xe0b6U, 0x0U, 0xfd29U, 0xb6U, 0x0U, 0x0U, 0x0U, 0x0U, 0x38eU, 0xd999U, 0x0U, 0x5b0U, 0xe71cU, 0x0U, 0xf77aU, 0x444U, 0xfa50U, 0xb6U, 0x0U};
// @verified - p
void KillLeaves(void *data) {
    auto &work = *static_cast<FallingWork *>(data);
    leafSlots[work.slot] = 0;
    const Vector3 zero{};
    SetParticleEffectPosition(3, pooledLeaves[work.slot], &zero, &zero);
    HLib::DestroyTaskInternal(work.self);
}
// @verified - p
void FallLeaves(void *data) {
    auto &work = *static_cast<FallingWork *>(data);
    const auto level = work.level / levelDivisor;
    constexpr std::uint32_t sounds[]{451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 460};
    constexpr float rates[]{.008f, .02f, .03f, .045f, .08f, .15f, .2f, .35f, .5f, .7f, 1};
    constexpr float gradeRates[]{0, .2f, .5f, 1};
    Loop3DSoundEffect(sounds[level]);
    std::uint32_t slot = 0;
    while (slot < 4 && leafSlots[slot])
        ++slot;
    work.slot = static_cast<std::uint8_t>(slot);
    leafSlots[slot] = 1;
    const auto effect = pooledLeaves[slot];
    SetParticleEffectPosition(3, effect, &leavesPosition, &leavesVolume);
    SetParticleEffectVelocity(3, effect, {0, -.03f, 0}, {.012f, -.005f, .012f});
    SetParticleEffectSize(3, effect, .03f, .09f);
    SetParticleEffectBirthRate(3, effect, gradeRates[grade] * rates[level]);
    ResumeParticleEffect(3, effect);
    work.frame = 0;
    Continue(KillLeaves, 30);
}
// @verified - p
void FeedbackVoice(TrainingWork &work) {
    constexpr const char *voices[]{"0026A049", "0026A037", "0026A036", "0026A050", "0026A051", "0026A052", "0026A053", "0026A053", "0026A054", "0026A054", // @text 0026A049 EN: "I need to control\nthe timing." | JA: "呼吸を合わせないと…" ; 0026A037 EN: "It's pretty hard." | JA: "けっこう、難しいな。" ; 0026A036 EN: "How's this?" | JA: "こう…か？" ; 0026A050 EN: "Like this?" | JA: "こんな感じか？" ; 0026A051 EN: "I got the hang of it." | JA: "つかめてきたぞ。" ; 0026A052 EN: "Looking good." | JA: "いい感じだ。" ; 0026A053 EN: "Good!" | JA: "よしっ！" ; 0026A054 EN: "Not yet." | JA: "まだまだだな。"
        "0026A055", "0026A056", "0026A041", "0026A057", "0026A058", "0026A058", "0026A059", "0026A059", "0026A055", "0026A061", "0026A042", "0026A052", // @text 0026A055 EN: "I need to control\nmy timing." | JA: "もっとタイミングを合わせるか。" ; 0026A056 EN: "Watch out for\nthe timing." | JA: "もっと呼吸に気をつけるか。" ; 0026A041 EN: "Yeah,\nthis is it." | JA: "この感じだな。" ; 0026A057 EN: "Yes,\nthis is good." | JA: "よし、いいぞ。" ; 0026A058 EN: "Good,\nI got it!" | JA: "よしっ、つかんだぞ！" ; 0026A059 EN: "Need to concentrate\nmore." | JA: "もっと集中しないと…" ; 0026A061 EN: "I could improve." | JA: "まだいけそうだな。" ; 0026A042 EN: "Feels right." | JA: "いい感触だ。" ; 0026A052 EN: "Looking good." | JA: "いい感じだ。"
        "0026A063", "0026A063", "0026A064", "0026A064", "0026A065", "0026A066", "0026A036", "0026A041", "0026A041", "0026A042"}; // @text 0026A063 EN: "This is it!" | JA: "これだっ！" ; 0026A065 EN: "No, I can do\nmore than this." | JA: "違う。こんなもんじゃない。" ; 0026A066 EN: "My timing is\nnot perfect yet." | JA: "まだ呼吸が完全に合ってないな。" ; 0026A036 EN: "How's this?" | JA: "こう…か？" ; 0026A041 EN: "Yeah,\nthis is it." | JA: "この感じだな。" ; 0026A042 EN: "Feels right." | JA: "いい感触だ。"
    voiceParity = (voiceParity + 1) % 2;
    if ((voiceParity == 0 || grade == 0 || grade == 3) && taught[work.level / 10] == 1)
        Voice(voices[8 * work.rank + 2 * grade + shenGetRandomInt32(2)], 60);
}
// @verified - p
void Feedback(TrainingWork &work) {
    const auto offset = work.rank >= 2 ? 50 : 0;
    switch (grade) {
    case 0:
        QueueCamera(3000 + offset, 10);
        break;
    case 1:
        QueueCamera(3100 + shenGetRandomInt32(4) + offset, 10);
        break;
    case 2:
        QueueCamera(3200 + shenGetRandomInt32(4) + offset, 10);
        break;
    case 3:
        QueueCamera(3300 + offset, 10);
        QueueCamera(3301 + offset, 25);
        break;
    default:
        break;
    }
    FeedbackVoice(work);
    if (taught[work.level / 10])
        QueueCamera(work.rank + 2000, 120);
    Continue(work.level >= maximumLevel || taught[work.level / 10] ? Training : ActionWait);
}
// @verified - p
void Action(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    work.phase = 1;
    if (work.action != 0) {
        if (work.action >= 1 && work.action <= 5)
            Continue(Training);
        return;
    }
    grade = strength >= 100 ? 3 : strength >= 75 ? 2 : strength >= 41.66f ? 1 : 0;
    if (work.level >= maximumLevel && grade >= 2 && !(g_entryPoint == 97 && g_currentMap.name == SHEN_NAME(WKKT))) {
        HideGauge();
        work.phase = 0;
        Continue(Success);
        return;
    }
    if (work.level == 0 && grade)
        LcdmSelectMessage(1);
    constexpr std::int32_t gains[]{0, 1, 2, 5};
    work.phase = 0;
    work.level = std::min(work.level + gains[grade], maximumLevel);
    ++work.tries;
    SetRank(work);
    SetFightGaugeTrainingProgress(static_cast<float>(work.level) / static_cast<float>(maximumLevel));
    if (grade) {
        constexpr float shake[]{0, .5f, .75f, .9f};
        QueueCameraShake(.15f, .3f, shake[grade], 27);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(FallLeaves, 6, 0x0b, 0x20, taskToken);
        auto *falling = static_cast<FallingWork *>(HLib::GetTaskParameter(task));
        HLib::SetupTimedTask(task, 30);
        falling->level = work.level;
        falling->self = task;
    }
    constexpr std::uint32_t firstSounds[]{85, 46, 43, 44}, secondSounds[]{206, 449, 450, 947};
    EVT_AT_SetSound(firstSounds[grade], 20);
    EVT_AT_SetSound(948, 10);
    EVT_AT_SetSound(secondSounds[grade], 30);
    EVT_AT_SetMot(ryo, 5188, 40, 1, 0x20000000U, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ryo), 0, reinterpret_cast<std::byte *>(strikeHand), 8);
    Queue_EVT_AT_CtrlHandUp_1(ryo, 0, reinterpret_cast<std::byte *>(qteHand), 8, 60);
    Feedback(work);
}
// @verified - p
void ActionWait(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    if (!SetMotionRegionFlag(scenFindMotionwork(ryo), 65, 1, MotionRegion::LowerBody))
        return;
    if (taught[work.level / 10] || work.action)
        Continue(Training);
    else {
        taught[work.level / 10] = 1;
        Continue(Talk);
    }
}
// @verified - p
void DrawGauge() {
    if (GetDebugForceUIEnabled(UIElement::FightGauge)) {
        ActivateFightGaugePrimary();
        return;
    }
    if (displayLevel == -1)
        return;
    PrepareDrawWithMode(gaugeSprite, static_cast<ScreenMode>(0));
    shenSubmitQuad(gaugeBackground, 10);
    const float right = (static_cast<float>(displayLevel) / static_cast<float>(maximumLevel)) * 47.0f + 41.0f;
    gaugeFill.quad.corners = {41, 403, right, 403, 41, 413, right, 413};
    if (displayLevel)
        shenSubmitQuad(gaugeFill, 11);
}
// @verified - p
void Gauge(void *) {
    if (gaugeVisible)
        HLib::ExecuteCallbackNow([](void *) { DrawGauge(); }, nullptr);
}
// @verified - p
void EnqueueGauge() {
    InitializeLegacyIconSprite(gaugeSprite);
    gaugeSprite.x = 32;
    gaugeSprite.y = 400;
    gaugeSprite.depth = 12;
    gaugeSprite.icon = 2308;
    gaugeSprite.width = 64;
    gaugeSprite.height = 32;
    gaugeBackground = {.quad = {.flags = 0x400, .corners = {34, 380, 95, 380, 34, 420, 95, 420}},
        .colour = 0x80000000,
        .alternateColours = {0x80000000, 0x80000000, 0x80000000}};
    gaugeFill = {.quad = {.flags = 0x400, .corners = {41, 403, 41, 403, 41, 413, 41, 413}},
        .colour = 0xffff0000,
        .alternateColours = {0xffff0000, 0xffff0000, 0xffff0000}};
    gaugeTask = HLib::EnqueueTaskWithoutParameter(Gauge, 8, 0x0b, static_cast<std::int32_t>(0x544b4b57));
}
// @verified - p
void Prepare() {
    SetScenarioFollowCharacter(shen_name{0});
    for (std::size_t slot = 0; slot < 4; ++slot) {
        pooledLeaves[slot] = static_cast<std::uint32_t>(
            CreateFallingParticleEffect(leavesPosition, leavesVolume, _mm_cvttss_si32(_mm_set_ss(leafCount)), 1, shen_name{0x400}, "misc", "REDLEAF.pvr", 102));
        leafSlots[slot] = 0;
    }
    if (entry != 1)
        EV_SetNewSceneIndex();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("WKKT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("WKKT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("WKKT"), 0);
    EV_LoadCutsceneFromCurrentScene("0026A028");
    EV_FreeAuthorPak(1, "0026");
    LoadGauge();
    SetFightGaugeVisible(0);
    gaugeVisible = false;
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    Position(base, absolute, basePosition);
    Angle(base, absolute, 0);
    Character *character = FindCharacterByScenarioName(ryo);
    SetLowQualityHands(character);
    SetHighModelXYZ(character, 1, 0, 0);
    SetHighModelXYZ(character, 1, 1, 0);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(qteHand), 0);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(qteHand), 0);
    FindMotionWork(character);
    EnqueueGauge();
    SetUnusedEventState(3);
}
// @verified - p
void Root(void *) {
    shenIsGamePaused();
}
// @verified - p
void MakeTask(void *) {
    auto *root = HLib::EnqueueTaskWithParameter(Root, 2, 0x0b, 0x10, taskToken);
    auto *child = HLib::EnqueueTaskWithParameter(entry == 1 ? First : Auto1, 5, 0x0b, 0x148, taskToken);
    *static_cast<TrainingRoot *>(HLib::GetTaskParameter(root)) = {root, child};
    auto &work = *static_cast<TrainingWork *>(HLib::GetTaskParameter(child));
    work.self = child;
    work.parent = root;
    work.elapsed = work.idleTicks = work.tries = work.phase = 0;
    work.level = work.rank = 0;
    work.errorCount = 0;
    work.leafA = work.leafB = work.smoke = -1;
    taught[0] = 1;
    taught[1] = taught[2] = taught[3] = 0;
    taught[4] = 1;
    strength = -1;
    delayedCommand = false;
    voiceParity = 0;
    EV_EventSetUpHelpIcon_0(66, 0);
    work.history[60] = work.history[61] = 0;
    work.history[0] = work.history[1] = work.history[30] = work.history[31] = work.history[62] = work.history[64] = 0;
    work.readCharacters = Queue_EVT_AT_ReadChr(child, characters);
    Prepare();
    HLib::DestroyCurrentTask();
}
// @verified - p
void First(void *) {
    FadeIn();
    EV_LoadAudio("0026A028", 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 0, 0);
    Position(ryo, base, stancePosition);
    Angle(ryo, base, 90);
    EVT_AT_SetMot(ryo, 5187, 0, 1, 0x60000000U, -1);
    EV_DisableCharacterDisplay(kem);
    Camera(2000);
    camCtrlDisableFilmBars();
    Continue(Training);
}
// @verified - p
void ResumeTraining(TrainingWork &work) {
    EV_LoadAudio("0026A028", 1);
    Voice("0026A027"); // @text EN: "Practice is what I need." | JA: "とにかく練習するしかないな。"
    EVT_AT_SetMot(ryo, 5192, 0, 1, 0x60000000U, 35);
    EVT_AT_SetMot(ryo, 5187, 0, 1, 0x60000000U, 60);
    QueueCamera(work.rank + 2000, 35);
    Continue(Training, 60);
    work.phase = 0;
}
// @verified - p
void Auto1(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    if (work.phase == 0) {
        FadeIn();
        EV_LoadAudio("0026A028", 0);
        work.level = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 0));
        SetRank(work);
        for (std::int32_t i = 0; i <= work.level / 10; ++i)
            taught[i] = 1;
        if (g_entryPoint == 98 && g_currentMap.name == SHEN_NAME(WKKT)) {
            leafCount = 2500;
            work.level = maximumLevel;
        }
        EV_DisableCharacterDisplay(kem);
        Position(ryo, base, stancePosition);
        Angle(ryo, base, 90);
        EVT_AT_SetMot(ryo, 5191, 0, 1, 0x40000000U, -1);
        EVT_AT_SetMot(ryo, 5190, 0, 1, 0x40000000U, 30);
        Voice("0026A026"); // @text EN: "Enough leaves to cover the ground=@" | JA: "地面をおおうくらい、か…"
        QueueCamera(1000, -1);
        QueueSkChoiceWait(work.self, 0, 151, 153, -1, 153, 0, 90);
        ++work.phase;
    } else if (work.phase == 1) {
        if (shenGetCtrlSpeakIconFlag() == 151)
            ResumeTraining(work);
        else {
            QueueCamera(1200, 35);
            Voice("0026A033"); // @text EN: "I guess not now." | JA: "今はやめておくか…"
            EVT_AT_SetMot(ryo, 5189, 0, 1, 0x40000000U, 0);
            EV_stepSetFlags(1, 32);
            Continue(Stop, 70);
            work.phase = 0;
        }
    }
}
// @verified - p
void Auto2(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    if (work.phase == 0) {
        HideGauge();
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        Position(ryo, base, stancePosition);
        Position(kem, absolute, kemPosition);
        EVT_AT_SetAngY(ryo, absolute, 90, -1);
        EVT_AT_SetMot(ryo, 5191, 0, 1, 0x40000000U, -1);
        EVT_AT_SetMot(ryo, 5190, 0, 1, 0x40000000U, 30);
        QueueCamera(1100, -1);
        Voice("0026A028"); // @text EN: "I should continue=@" | JA: "もう少し続けてみるか…"
        QueueSkChoiceWait(work.self, 0, 152, 34, 0, 34, 0, 60);
        ++work.phase;
        work.elapsed = work.idleTicks = work.tries = 0;
    } else if (work.phase == 1) {
        if (shenGetCtrlSpeakIconFlag() == 152)
            ResumeTraining(work);
        else {
            QueueCamera(1200, 35);
            EV_stepSetFlags(1, 32);
            EVT_AT_SetMot(ryo, 5189, 0, 1, 0x40000000U, 0);
            Continue(Stop, 70);
            work.phase = 0;
        }
    }
}
// @verified - p
void Auto3(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    HideGauge();
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    Position(ryo, base, stancePosition);
    Angle(ryo, base, 90);
    EVT_AT_SetMot(ryo, 5191, 0, 1, 0x40000000U, -1);
    EVT_AT_SetMot(ryo, 5190, 0, 1, 0x40000000U, 30);
    EVT_AT_SetMot(ryo, 5189, 0, 1, 0x40000000U, 60);
    QueueCamera(1100, -1);
    QueueCamera(1200, 60);
    Voice("0026A045"); // @text EN: "That's enough\nfor now." | JA: "これぐらいにしておこう。"
    work.elapsed = work.idleTicks = work.tries = 0;
    EV_stepSetFlags(1, 32);
    Continue(Stop, 130);
}
// @verified - p
void Stop(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    if (g_currentMap.name == SHEN_NAME(WKKT)) {
        if (work.leafA != -1)
            FreeParticleEffect(3, work.leafA);
        if (work.leafB != -1)
            FreeParticleEffect(3, work.leafB);
        if (work.smoke != -1)
            FreeParticleEffect(0, work.smoke);
        work.leafA = work.leafB = work.smoke = -1;
        Continue(Auto1);
    } else {
        spmFadeOut();
        if (EV_GetScreenFilmIsNotShowing())
            FadeOut();
        Continue(End, 30);
    }
    SetUnusedEventState(-1);
}
// @verified - p
void WaitNpc(void *) {
    EV_NPC_NewLoadSw(1);
    EV_npcLockUnlock(kem);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
// @verified - p
void End(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    for (auto effect : pooledLeaves)
        FreeParticleEffect(3, effect);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 0, work.level);
    ReleaseGauge();
    SetScenarioCharacterKillMode(base, 1);
    if (work.leafA != -1)
        FreeParticleEffect(3, work.leafA);
    if (work.leafB != -1)
        FreeParticleEffect(3, work.leafB);
    if (work.smoke != -1)
        FreeParticleEffect(0, work.smoke);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WKKT"), 0);
    EV_FreeAuthorPak(0, "0026");
    InitiateCleanupForSceneChara(kem);
    ET_AT_KillWork(work.readCharacters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.parent);
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    Motion(ryo, 61443, 0x40000000U);
    GetMotionFrameCount(61443);
    Position(ryo, absolute, exitPosition);
    Angle(ryo, absolute, 247.5f);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    LcdmInit(0);
    EV_EventSetUpHelpIcon_0(0, 0);
    scenFreeEvent("WKKT.EV2");
    FadeIn();
    SetPlayerControlEnabled(0);
    if (!EV_GetShenmueFlag(926) || EV_GetShenmueFlag(32)) {
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(WaitNpc, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(wnpc)));
        HLib::SetupTimedTask(task, 30);
        FadeIn();
        EV_CheckWarmStartNpcRoute(kem);
        EV_ForceLoadNpc(kem);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    } else {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour < 19) {
            time.hour = 19;
            time.minute = 0;
            npcResetDateTimeIfNeeded();
            SetCurrentDateTimeTo(time);
            npcAdvanceTime(time.hour, time.minute);
        }
        SetPlayerControlEnabled(1);
        EV_NPC_NewLoadSw(1);
    }
    ClearFightGaugeTrainingVisibility();
    if (gaugeTask)
        HLib::DestroyTaskInternal(gaugeTask);
}
// @verified - p
void Talk(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    HideGauge();
    Motion(ryo, 5200, 0x40000000U);
    GetMotionFrameCount(5200);
    EVT_AT_SetMot(ryo, 5199, 0, 1, 0, 60);
    Motion(kem, 5193, 0x40000000U);
    GetMotionFrameCount(5193);
    Position(kem, base, teachingPosition);
    Angle(kem, base, 135);
    EV_EnableCharacterDisplay(kem);
    std::int32_t duration = 0;
    switch (work.level / 10) {
    case 1:
        Camera(3400);
        camCtrlDisableFilmBars();
        QueueCamera(3401, 170);
        Voice("0026B056"); // @text EN: "You look too stiff.\nEase your shoulders." | JA: "動きがかたいのう。\nもっと肩の力を抜くんじゃ。"
        Voice("0026A035", 170); // @text EN: "Yes." | JA: "あ、はい。"
        duration = 240;
        break;
    case 2:
        Camera(3410);
        camCtrlDisableFilmBars();
        QueueCamera(3411, 150);
        Voice("0026B057"); // @text EN: "Centralize all your\nbody's strength." | JA: "当たる瞬間に全身の力を\n集中させるんじゃ。"
        Voice("0026A038", 150); // @text EN: "I understand." | JA: "わかりました。"
        duration = 210;
        break;
    case 3:
        Camera(3420);
        camCtrlDisableFilmBars();
        QueueCamera(3421, 110);
        Voice("0026B058"); // @text EN: "That's looking much better." | JA: "なかなか、サマになってきたのう。"
        Voice("0026A040", 110); // @text EN: "I think\nI'm getting it." | JA: "だいぶ、つかめてきました。"
        duration = 200;
        break;
    default:
        break;
    }
    QueueCamera(work.rank + 2000, duration - 25);
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), taskToken);
    RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, kem, 1, static_cast<float>(duration - 25)};
    EVT_AT_SetPos(ryo, base, {1.896f, 0, .168f}, duration - 25);
    EVT_AT_SetMot(ryo, 5192, 0, 1, 0x60000000U, duration - 25);
    EVT_AT_SetMot(ryo, 5187, 0, 1, 0x60000000U, duration);
    work.phase = 1;
    Continue(Training, static_cast<std::uint32_t>(duration));
}
// @verified - p
std::int32_t CreateLeafBurst(std::uint32_t count, float rate) {
    const auto effect = static_cast<std::uint32_t>(
        CreateFallingParticleEffect(leavesPosition, {1, .3f, 1.5f}, static_cast<std::int32_t>(count), rate, shen_name{0x400}, "misc", "REDLEAF.pvr", -1));
    SetParticleEffectVelocity(3, effect, {.0001f, -.04f, .0001f}, {.012f, -.01f, .012f});
    SetParticleEffectSize(3, effect, .03f, .09f);
    return static_cast<std::int32_t>(effect);
}
// @verified - p
void CreateSuccessSmoke(TrainingWork &work) {
    const auto effect =
        static_cast<std::uint32_t>(CreateSmokeEffect({40.689998626708984f, 102.19999694824219f, 241.27000427246094f}, {}, 100, .001f, shen_name{0}, 3));
    work.smoke = static_cast<std::int32_t>(effect);
    SetSmokeEffectFalloff(effect, 10);
    SetSmokeEffectFadeFactor(effect, 255);
    SetParticleEffectColour(0, effect, 0, 142, 125, 110);
    SetParticleEffectInitialScale(0, effect, .43529412150382996f);
    SetParticleEffectSize(0, effect, 3.1372549533843994f, 3.1764705181121826f);
    SetParticleEffectBirthRate(0, effect, 0);
    SetParticleEffectVelocity(0, effect, {0, .01f, 0}, {});
    SetSmokeEffectSpread(effect, 55.86399841308594f, 1.058823585510254f);
    SetSmokeEffectRadialMotion(effect, .772549033164978f, .46666666865348816f);
    SetParticleEffectBirthRate(0, effect, .1f);
}
// @verified - p
void AnimateSuccessEffects(TrainingWork &work) {
    switch (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) {
    case 40:
        SuspendParticleEffect(3, work.leafA);
        break;
    case 90:
        ResumeParticleEffect(0, work.smoke);
        break;
    case 119:
        SetParticleEffectBirthRate(0, work.smoke, 0);
        break;
    case 120:
        SuspendParticleEffect(0, work.smoke);
        break;
    case 145:
        ResumeParticleEffect(3, work.leafA);
        break;
    case 440:
        SuspendParticleEffect(3, work.leafA);
        break;
    case 1330:
        SetParticleEffectSize(3, work.leafB, .04f, .12f);
        ResumeParticleEffect(3, work.leafB);
        break;
    default:
        break;
    }
}
// @verified - p
void Success(void *data) {
    auto &work = *static_cast<TrainingWork *>(data);
    if (work.phase == 0) {
        EV_EnableCharacterDisplay(kem);
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(BUTK)));
        CreateSuccessSmoke(work);
        work.leafA = CreateLeafBurst(1000, .02f);
        work.leafB = CreateLeafBurst(2500, .005f);
        ++work.phase;
    } else if (work.phase == 1) {
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            AnimateSuccessEffects(work);
        else {
            work.phase = 0;
            QueueExtraTelop("LL", 1);
            InitiateCleanupForSceneChara(kem);
            EV_stepSetFlags(0, 32);
            Continue(Stop);
        }
    }
}
// @verified - p
void Init() {
    motionInputMargin = 6;
    strength = 0;
    leafCount = 250;
    maximumLevel = 40;
    levelDivisor = 4;
    delayedCommand = false;
    entry = voiceParity = grade = displayLevel = 0;
    taught = {};
    pooledLeaves = {};
    leafSlots = {};
    gaugeVisible = false;
    gaugeTask = nullptr;
}
// @verified - p
void Main(std::uintptr_t value) {
    FadeOut(1);
    entry = static_cast<std::int32_t>(value);
    auto *task = HLib::EnqueueTaskWithoutParameter(MakeTask, 4, 0x0b, static_cast<std::int32_t>(taskToken.name));
    HLib::SetupTimedTask(task, 3);
}
} // namespace

std::int32_t wkktSetGaugeVisible(std::int32_t visible) {
    const std::int32_t previous = gaugeVisible ? 1 : 0;
    gaugeVisible = visible != 0;
    return previous;
}
const ScriptAreaDefinition areaWKKTScriptDefinition{.areaId = SHEN_NAME(WKKT), .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2::areas
