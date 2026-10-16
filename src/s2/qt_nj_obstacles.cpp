#include "s2/qt_nj_obstacles.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/game_runtime.hpp"
#include "s2/step_event.hpp"

#include <bit>

namespace shenmue::s2 {
namespace {
constexpr shen_name obstacleBase{SHEN_NAME(TNJB)};
constexpr shen_name player{SHEN_NAME(RYO_)};
constexpr shen_name ren{SHEN_NAME(REN_)};
constexpr shen_name invalid{0xffffffffU};
constexpr shen_name eventToken{SHEN_NAME(EVNT)};
constexpr std::uint32_t staggerMotion = 0x2143;
constexpr std::uint32_t staggerLoopMotion = 0x2142;
constexpr std::uint32_t renComeOnMotion = 0x2147;
constexpr std::uint32_t breakStickMotion = 0x2155;


constexpr std::int32_t firstObstacleParts[]{
    175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157,
    166, 154, 173, 168, 153, 207, 208, 165, 160, 176, 171, 164,
    159, 161, 156, 197, 196, 195, 205, 206, 204, 203, 202, 201,
    200, 199, 198, 194, 193, 192, 182, 181, 180, 191, 190, 189,
    188, 187, 186, 185, 184, 183, 179, 178, 177,
};
constexpr std::int32_t secondObstacleParts[]{
    175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157,
    166, 154, 173, 168, 153, 165, 160, 176, 171, 164, 159, 161,
    156, 195, 196, 192, 193, 194, 180, 181, 182, 189, 190, 191,
    186, 187, 188, 183, 184, 185, 177, 178, 179,
};
constexpr std::int32_t fourthObstacleParts[]{
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
    164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 186, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
    201, 202, 203, 204, 205, 206,
};
static_assert(sizeof(firstObstacleParts) / sizeof(firstObstacleParts[0]) == 57);
static_assert(sizeof(secondObstacleParts) / sizeof(secondObstacleParts[0]) == 45);
static_assert(sizeof(fourthObstacleParts) / sizeof(fourthObstacleParts[0]) == 54);


void QueueObstacleDynamics(shen_name obstacle, const char *resource, std::int32_t trackCount, const std::int32_t *parts, float endFrame, float delay) {
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), eventToken);
    RegisterEventTask(task);
    auto *work = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
    work->character = obstacle;
    work->resource = resource;
    work->trackCount = trackCount;
    work->parts = parts;
    work->endFrame = endFrame;
    work->frameStep = 1.0f;
    work->currentFrame = 4.0f;
    work->delay = delay;
}

void QueueObstacleDisplay(shen_name obstacle) {
    if (obstacle.name == invalid.name)
        return;
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), eventToken);
    RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, obstacle, 0, 25.0f};
}

void QueueObstacleSmoke(std::int32_t profile, float fraction) {
    auto *task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 0x0b, sizeof(EventSmokeWork), eventToken);
    HLib::SetupTimedTask(task, 85);
    auto *work = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
    work->character = obstacleBase;
    work->bone = 0xffffffffU;

    work->startDelay = 4;
    work->remaining = 0;
    work->finishDelay = 108;
    work->profile = profile;
    work->state = 0;
    work->birthFraction = fraction;
    work->birthStep = fraction;
}
} // namespace


void InitializeQtnjObstacle(shen_name obstacle) {
    switch (obstacle.name) {
    case SHEN_NAME(TNJ0):
        EVT_AT_SetPos(obstacle, obstacleBase, {}, 0);
        EVT_AT_SetAngY(obstacle, obstacleBase, 0, 0);
        QueueObstacleDynamics(obstacle, "TNJ0", 57, firstObstacleParts, 5, 0);
        break;
    case SHEN_NAME(TNJ1):
        EVT_AT_SetPos(obstacle, obstacleBase, {}, 0);
        EVT_AT_SetAngY(obstacle, obstacleBase, 0, 0);
        QueueObstacleDynamics(obstacle, "TNJ1", 45, secondObstacleParts, 5, 0);
        break;
    case SHEN_NAME(TNJ2):
        EVT_AT_SetPos(obstacle, obstacleBase, {}, 0);
        EVT_AT_SetAngY(obstacle, obstacleBase, 180, 0);
        QueueObstacleDynamics(obstacle, "TNJ2", 48, nullptr, 5, 0);
        break;
    case SHEN_NAME(TNJ3):
        EVT_AT_SetPos(obstacle, obstacleBase, {}, 0);
        EVT_AT_SetAngY(obstacle, obstacleBase, 0, 0);
        QueueObstacleDynamics(obstacle, "TNJ3", 54, fourthObstacleParts, 5, 0);
        break;
    default:
        break;
    }
}


std::uint32_t QueueQtnjFirstObstacleImpact(shen_name obstacle) {
    QueueObstacleDisplay(obstacle);
    QueueObstacleDynamics(obstacle, "TNJ0", 57, firstObstacleParts, 250, 25);
    Vector3 position;
    EV_GetCharaPosition(obstacle, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    QueueObstacleSmoke(0, 0.1f);
    EVT_AT_SetSound(0x289, 25);
    EVT_AT_SetSound(0xa5e, 45);
    EVT_AT_SetSound(0x597, 40);
    EVT_AT_SetSound(0x598, 45);
    EVT_AT_SetSound(0x597, 55);
    EVT_AT_SetSound(0x598, 65);
    EVT_AT_SetSound(0x4bb, 85);
    EVT_AT_SetSound(0x288, 86);
    EVT_AT_SetSound(0x287, 87);
    EVT_AT_SetSound(0x287, 92);
    EVT_AT_SetSound(0x288, 105);
    return 271;
}


std::uint32_t QueueQtnjSecondObstacleImpact(shen_name obstacle) {
    QueueObstacleDisplay(obstacle);
    QueueObstacleDynamics(obstacle, "TNJ1", 45, secondObstacleParts, 250, 25);
    Vector3 position;
    EV_GetCharaPosition(obstacle, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    QueueObstacleSmoke(0, 0.1f);
    EVT_AT_SetSound(0x289, 25);
    EVT_AT_SetSound(0x4bb, 85);
    EVT_AT_SetSound(0x288, 86);
    EVT_AT_SetSound(0x287, 87);
    EVT_AT_SetSound(0x597, 55);
    EVT_AT_SetSound(0x598, 58);
    return 271;
}


std::uint32_t QueueQtnjThirdObstacleImpact(shen_name obstacle) {
    QueueObstacleDisplay(obstacle);
    QueueObstacleDynamics(obstacle, "TNJ2", 48, nullptr, 250, 25);
    Vector3 position;
    EV_GetCharaPosition(obstacle, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    QueueObstacleSmoke(0, 0.1f);
    EVT_AT_SetSound(0x289, 25);
    EVT_AT_SetSound(0x4bb, 85);
    EVT_AT_SetSound(0x288, 86);
    EVT_AT_SetSound(0x287, 87);
    EVT_AT_SetSound(0x597, 55);
    EVT_AT_SetSound(0x598, 58);
    return 271;
}


std::uint32_t QueueQtnjFourthObstacleImpact(shen_name obstacle) {
    QueueObstacleDisplay(obstacle);
    QueueObstacleDynamics(obstacle, "TNJ3", 54, fourthObstacleParts, 250, 25);
    Vector3 position;
    EV_GetCharaPosition(obstacle, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    QueueObstacleSmoke(1, 0.05f);
    EVT_AT_SetSound(0x289, 25);
    EVT_AT_SetSound(0x597, 40);
    EVT_AT_SetSound(0x598, 45);
    EVT_AT_SetSound(0x598, 65);
    EVT_AT_SetSound(0xa5e, 53);
    EVT_AT_SetSound(0x4bb, 80);
    EVT_AT_SetSound(0x288, 81);
    EVT_AT_SetSound(0x287, 82);
    EVT_AT_SetSound(0x287, 87);
    EVT_AT_SetSound(0x4bb, 105);
    return 271;
}


std::uint32_t DispatchQtnjObstacleImpact(shen_name obstacle, std::int32_t trial) {
    std::uint32_t result = 1;
    std::int32_t duration = 1;
    switch (obstacle.name) {
    case SHEN_NAME(TNJ0):
        if (trial == 1 || trial == 3) {
            constexpr float negativeZero = std::bit_cast<float>(0x80000000U);
            EVT_AT_SetCam(trial == 1 ? 7423 : 7427, obstacleBase, obstacleBase, negativeZero, 0, 0);
            EVT_AT_SetCam(7424, obstacleBase, obstacleBase, negativeZero, 0, 38);
            QueueCameraShake(0.2f, 0.4f, 0.7f, 80);
        }
        EVT_AT_SetPos(player, obstacleBase, {0.5f, 0, 2.6f}, 0);
        EVT_AT_SetAngY(player, obstacleBase, 0, 0);
        EVT_AT_SetSound(0x664, 10);
        duration = EVT_AT_SetMot(player, breakStickMotion, 2, 1, 0x60000000U, 0);
        result = QueueQtnjFirstObstacleImpact(obstacle);
        break;
    case SHEN_NAME(TNJ1):
        EVT_AT_SetCam(7402, obstacleBase, obstacleBase, 0, 0, 0);
        QueueCameraShake(0.2f, 0.4f, 0.7f, 80);
        EVT_AT_SetPos(player, obstacleBase, {0.5f, 0, 3}, 0);
        EVT_AT_SetAngY(player, obstacleBase, 0, 0);
        duration = EVT_AT_SetMot(player, breakStickMotion, 2, 1, 0x60000000U, 0);
        result = QueueQtnjSecondObstacleImpact(obstacle);
        break;
    case SHEN_NAME(TNJ2):
        EVT_AT_SetCam(7423, obstacleBase, obstacleBase, 0, 0, 0);
        EVT_AT_SetCam(7424, obstacleBase, obstacleBase, 0, 0, 38);
        QueueCameraShake(0.2f, 0.4f, 0.7f, 80);
        EVT_AT_SetPos(player, obstacleBase, {-0.5f, 0, 2.4f}, 0);
        EVT_AT_SetAngY(player, obstacleBase, 0, 0);
        EVT_AT_SetPos(player, player, {0, 0, -0.3f}, 38);
        duration = EVT_AT_SetMot(player, breakStickMotion, 2, 1, 0x40000000U, 0);
        result = QueueQtnjThirdObstacleImpact(obstacle);
        break;
    case SHEN_NAME(TNJ3):
        if (trial == 0 || trial == 2) {
            EVT_AT_SetCam(trial == 0 ? 7421 : 7425, obstacleBase, obstacleBase, 0, 0, 0);
            EVT_AT_SetCam(trial == 0 ? 7422 : 7426, obstacleBase, obstacleBase, 0, 0, 60);
            QueueCameraShake(0.2f, 0.4f, 0.7f, 80);
        }
        EVT_AT_SetPos(player, obstacleBase, {0.5f, 0, 2.4f}, 0);
        EVT_AT_SetAngY(player, obstacleBase, 0, 0);
        EVT_AT_SetSound(0x664, 10);
        duration = EVT_AT_SetMot(player, breakStickMotion, 2, 1, 0x60000000U, 0);
        result = QueueQtnjFourthObstacleImpact(obstacle);
        break;
    default:
        break;
    }
    EVT_AT_SetMot(player, 0xf006, 2, 1, 0, duration);
    return result;
}


std::uint32_t PlayQtnjObstacle(QtnjObstacleWork *work) {
    std::int32_t duration;
    switch (work->trial) {
    case 0:
        EVT_AT_SetVoice("0284A005", 1, 0); // @text EN: " " | JA: "はっ！"
        EVT_AT_SetVoice("0284F002", 1, 80); // @text EN: "Damn!" | JA: "ゲホッ！　ゲホッ！\n…くっそぉ！"
        EVT_AT_SetPos(work->npcs[0], obstacleBase, {-1, 0, 6}, 90);
        EVT_AT_SetAngY(work->npcs[0], obstacleBase, 0, 90);
        duration = EVT_AT_SetMot(work->npcs[0], staggerMotion, 2, 1, 0x60000000U, 90);
        break;
    case 1:
        EVT_AT_SetVoice("0284A002", 1, 0); // @text EN: " " | JA: "うおりゃ！"
        EVT_AT_SetVoice("0284G002", 1, 80); // @text EN: "Those kids!" | JA: "あのガキども！"
        EVT_AT_SetPos(work->npcs[0], obstacleBase, {-1, 0, 7}, 0);
        EVT_AT_SetAngY(work->npcs[0], obstacleBase, 0, 0);
        duration = EVT_AT_SetMot(work->npcs[0], staggerMotion, 2, 1, 0x60000000U, 0);
        break;
    case 2:
        EVT_AT_SetVoice("0284A005", 1, 0); // @text EN: " " | JA: "はっ！"
        EVT_AT_SetVoice("0284H002", 1, 80); // @text EN: "How dare you!" | JA: "ゲホゲホッ！\nやりやがったなッ！"
        EVT_AT_SetPos(work->npcs[0], obstacleBase, {-1, 0, 6}, 90);
        EVT_AT_SetAngY(work->npcs[0], obstacleBase, 0, 90);
        duration = EVT_AT_SetMot(work->npcs[0], staggerMotion, 2, 1, 0x60000000U, 90);
        break;
    default:
        EVT_AT_SetVoice("0284A002", 1, 0); // @text EN: " " | JA: "うおりゃ！"
        EVT_AT_SetVoice("0284I002", 1, 80); // @text EN: "How dare you!" | JA: "ゲホゲホッ！\nやりやがったなッ！"
        EVT_AT_SetPos(work->npcs[0], obstacleBase, {-1, 0, 7.5f}, 38);
        EVT_AT_SetAngY(work->npcs[0], obstacleBase, 0, 38);
        EVT_AT_SetPos(work->npcs[1], invalid, {1000, 0, 0}, 0);
        EVT_AT_SetPos(work->npcs[2], invalid, {1000, 0, 0}, 0);
        duration = EVT_AT_SetMot(work->npcs[0], staggerMotion, 2, 1, 0x60000000U, 38);
        break;
    }
    EVT_AT_DispBlur(player, 1, 6, 1, 40, 10);
    EVT_AT_SetPos(ren, obstacleBase, {0, 0, -4}, 0);
    EVT_AT_SetAngY(ren, obstacleBase, 0, 0);
    EVT_AT_SetMot(ren, renComeOnMotion, 2, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ren, renComeOnMotion, 2, 1, 0x40000000U, 60);
    EVT_AT_SetMot(work->npcs[0], staggerLoopMotion, 2, 1, 0x20000000U, duration);
    const auto result = DispatchQtnjObstacleImpact(work->obstacle, work->trial);
    work->waitFrames = 160;
    return result;
}

} // namespace shenmue::s2
