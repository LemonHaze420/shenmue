#include "s2/areas/area_bgsm.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2::areas {
void BgsmShakePot(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (work.state == 3) {
        work.state = 0;
        work.elapsed = 0;
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), BGSM_TASK);
        RegisterEventTask(task);
        EventCling *cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        cling->cleanup = 1;
        cling->target = BGSM_POT;
        cling->mode = 1;
        cling->delay = 60;
        EVT_AT_ObjectDisp_1(BGSM_POT, 60, 1);
        task = HLib::EnqueueTaskWithParameter(EVT_AT_CtrlHandUp_Callback, 6, 0x0b, sizeof(EventControlHand), BGSM_TASK);
        RegisterEventTask(task);
        EventControlHand *hand = static_cast<EventControlHand *>(HLib::GetTaskParameter(task));
        hand->command = reinterpret_cast<std::byte *>(bgsmOpenHand);
        hand->cleanup = 1;
        hand->character = BGSM_RYO;
        hand->ticks = 8;
        hand->delay = 60;
        HLib::SetCurrentTaskTo(BgsmSayDice);
        return;
    }
    if (work.state == 2) {
        Loop3DSoundEffect(0x117);
        work.dice[0] = static_cast<std::uint8_t>(shenGetRandomInt32(6) + 1);
        work.dice[1] = static_cast<std::uint8_t>(shenGetRandomInt32(6) + 1);
        work.dice[2] = static_cast<std::uint8_t>(shenGetRandomInt32(6) + 1);
        EVT_AT_SetPoint(BGSM_RYO, {}, 0, 3, 0, 0, 0, 0x50000000U);
        EVT_AT_SetPos(BGSM_DICE[0], BGSM_BASE, {-.08f, .76f, -.1f}, 30);
        EVT_AT_SetPos(BGSM_DICE[1], BGSM_BASE, {-.1f, .76f, -.12f}, 30);
        EVT_AT_SetPos(BGSM_DICE[2], BGSM_BASE, {-.1f, .76f, -.08f}, 30);
        for (shen_name die : BGSM_DICE)
            EVT_AT_ObjectDisp_1(die, 50, 0);
        EVT_AT_SetMot(BGSM_RYO, 4482, 0, .8f, 0x40000000U, -1);
        EventSetPoint point{.character = BGSM_RYO,
            .target = BGSM_BASE,
            .targetBone = -1,
            .trackingMode = 3,
            .positionMode = 0,
            .duration = 0,
            .flags = 0x20000000U,
            .frame = 0,
            .cleanup = 0,
            .delay = 0};
        EVT_AT_SetPoint_Callback(&point);
        BgsmCamera(4550);
        EVT_AT_SetCam(4551, BGSM_BASE, BGSM_BASE, 0, 0, 20);
        static constexpr Vector3 angles[]{{0, 0, 0}, {270, 0, 0}, {0, 0, 90}, {270, 0, 270}, {90, 0, 0}, {180, 0, 0}};
        static constexpr std::uint8_t randomAxes[][3]{{0, 1, 0}, {0, 1, 0}, {1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 1, 0}};
        for (std::int32_t index = 0; index < 3; ++index) {
            const float x = GetRandomNumberInRange(360), y = GetRandomNumberInRange(360), z = GetRandomNumberInRange(360);
            const std::uint32_t face = work.dice[index] - 1;
            EVT_AT_SetAng_Enqueue(BGSM_DICE[index], BGSM_BASE, randomAxes[face][0] * x + angles[face].x, randomAxes[face][1] * y + angles[face].y,
                randomAxes[face][2] * z + angles[face].z, 30);
        }
        for (std::int32_t i = 0; i < 3; ++i)
            for (std::int32_t j = 0; j < 3; ++j)
                if (work.dice[i] < work.dice[j]) {
                    const std::uint8_t value = work.dice[i];
                    work.dice[i] = work.dice[j];
                    work.dice[j] = value;
                }
        ++work.state;
        HLib::SetupTaskDelay(80);
        return;
    }
    if (!work.state) {
        for (shen_name die : BGSM_DICE)
            EV_DisableCharacterDisplay(die);
        EV_EnableCharacterDisplay(BGSM_RYO);
        EV_EnableCharacterDisplay(BGSM_POT);
        EV_DisableCharacterDisplay(BGSM_HAND);
        BgsmPosition(BGSM_RYO, BGSM_BASE, {-.1f, -.03f, -.55f});
        BgsmAngle(BGSM_RYO, BGSM_BASE, 180);
        BgsmMotion(BGSM_RYO, 4483, 0x40000000U, .5f);
        BgsmMotion(work.opponent, BGSM_TALK_MOTIONS[bgsmOpponentPose], 0x10000000U);
        BgsmHand(BGSM_RYO, bgsmPotHand, 0);
        EventCling cling{.cleanup = 0,
            .target = BGSM_POT,
            .character = BGSM_RYO,
            .bone = 14,
            .position = {.11f, 0, -.01f},
            .rotation = {16384, 35498, 32768},
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&cling);
        EventSetPoint point{.character = BGSM_RYO,
            .target = {},
            .targetBone = 0,
            .trackingMode = 3,
            .positionMode = 0,
            .duration = 0,
            .flags = 0x50000000U,
            .frame = 0,
            .cleanup = 0,
            .delay = 0};
        EVT_AT_SetPoint_Callback(&point);
        EVT_AT_SetPoint(BGSM_RYO, BGSM_POT, -1, 3, 0, 1, 0, 0x20000000U);
        EVT_AT_SetPoint(work.opponent, BGSM_POT, -1, 3, 0, 1, 0, 0x20000000U);
        BgsmCamera(4500);
        ++work.state;
        work.elapsed = 0;
    } else if (work.state != 1)
        return;
    if (work.elapsed == 1500)
        BgsmPlayVoice(1, shenGetRandomInt32(2) ? "024" : "025");
    if (work.elapsed++ >= 1800) {
        ++work.state;
        return;
    }
    GetBattleDataConfig(0, BGSM_BATTLE_INPUT);
    if (GetBattleDataConfig(1, BGSM_BATTLE_INPUT) && GetBattleDataConfig(2, BGSM_BATTLE_INPUT))
        ++work.state;
}
} // namespace shenmue::s2::areas
