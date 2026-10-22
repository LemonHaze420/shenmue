#include "s2/areas/area_slt1.hpp"
#include "s2/areas/area_slot.hpp"
#include "s2/areas/slt1_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/flag.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2 {
namespace {
constexpr shen_name slot{SHEN_NAME(SLOT)}, ryo{SHEN_NAME(RYO_)}, hand{SHEN_NAME(TE_R)}, prizeHand{SHEN_NAME(TE_r)}, drum{SHEN_NAME(DRUM)}, invalid{0xffffffffU},
    eventToken{SHEN_NAME(EVNT)};
constexpr shen_name lamps[]{SHEN_NAME(LMP1), SHEN_NAME(LMP2), SHEN_NAME(LMP3)};
constexpr shen_name coinDisplays[]{SHEN_NAME(C011), SHEN_NAME(C020), SHEN_NAME(C100)};
constexpr shen_name payoutDisplays[]{SHEN_NAME(PC01), SHEN_NAME(PC10), SHEN_NAME(PC30), SHEN_NAME(PC50)};
constexpr shen_name eventButton{SHEN_NAME(BTEV)}, cancelButton{SHEN_NAME(BTCC)}, informationButton{SHEN_NAME(BTIF)};
std::int32_t cashToInsert, prizeDenomination, lightAngles[3];
std::uint32_t scoreFrame;
std::int8_t coinInsertFrame, moneyInsertFrame, leverFrame;
std::uint8_t buttonLampFrames[3];
void Main(void *);
void Score(void *);
void AuthorStart(void *);
void Betting(void *);
void Spin(void *);
void Prize(void *);
void AuthorExit(void *);
void Exit(void *);
void PayOut(void *);
void Bet(void *);
void StartButton(void *);
void SelectCoin(void *);
void SelectMoney(void *);
void PullLever(void *);
void InsertCoin(void *);
void InsertMoney(void *);
void ButtonLampPayOut(void *);
void ButtonLampBet(void *);
void ButtonLampStart(void *);
void DebugNullsub() {
}
Slt1Work *Root(Slt1ScoreWork *work) {
    return static_cast<Slt1Work *>(HLib::GetTaskParameter(work->root));
}
void Change(HLib::TaskCallback callback) {
    HLib::SetCurrentTaskTo(callback);
}
void Camera(std::int32_t id) {
    EventSetCamera request{0, id, slot, slot, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void Place(shen_name name, shen_name parent, const Vector3 &position, std::int32_t delay = 0) {
    EVT_AT_SetPos(name, parent, position, delay);
}
void Turn(shen_name name, shen_name parent, float degrees, std::int32_t delay = 0) {
    EVT_AT_SetAngY(name, parent, degrees, delay);
}
void Motion(shen_name name, std::uint32_t id, float frame = 0, float speed = 1, std::uint32_t flags = 0x40000000U, std::int32_t delay = 0) {
    EVT_AT_SetMot(name, id, frame, speed, flags, delay);
}
void Gravity(shen_name name, bool enabled) {
    const auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(name, 2, state);
    EV_SetCharaControllerFlags(name, 3, state);
}
void Pose(shen_name name, std::int32_t *command, std::int16_t frames = 0) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 0, reinterpret_cast<std::byte *>(command), frames);
}
void HandRotation(shen_name name, const Vector3i &angles) {
    ApplyCharacterHandRotation(ctrlGetCharaByName(name), 0, 21, &angles);
}
void TrackEye(bool enabled) {
    Vector3 position;
    if (enabled) {
        EV_GetCharaPosition(CHAR_SLOT_EYE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &position, 1);
    } else
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
}
void Lamp(std::int8_t index, std::int32_t enabled) {
    std::uint16_t part;
    switch (index) {
    case 1:
        part = 182;
        break;
    case 2:
        part = 183;
        break;
    case 3:
        part = 184;
        break;
    case 4:
        part = 185;
        break;
    case 5:
        part = 186;
        break;
    case 10:
        part = 192;
        break;
    case 11:
        part = 193;
        break;
    case 12:
        part = 194;
        break;
    default:
        return;
    }
    const float value = enabled ? 1.0f : 0.0f;
    Vector3 scale{value, value, value};
    EV_scenApplyPosOrRotToPartOf(slot, part, &scale, nullptr, EventCharacterPartTransformMode::Set);
}
void ClearWinningLamps(Slt1Work *work) {
    for (int i = 1; i <= 5; ++i)
        Lamp(i, 0);
    work->winningLines = 0;
    work->previousWager = 0;
}
void CoinDisplay(Slt1Work *work) {
    if (!work->coins) {
        EV_DisableCharacterDisplay(coinDisplays[0]);
        EV_DisableCharacterDisplay(coinDisplays[1]);
        EV_DisableCharacterDisplay(coinDisplays[2]);
    } else if (work->coins < 15) {
        EV_EnableCharacterDisplay(coinDisplays[0]);
        EV_DisableCharacterDisplay(coinDisplays[1]);
        EV_DisableCharacterDisplay(coinDisplays[2]);
    } else if (work->coins < 50) {
        EV_DisableCharacterDisplay(coinDisplays[0]);
        EV_EnableCharacterDisplay(coinDisplays[1]);
        EV_DisableCharacterDisplay(coinDisplays[2]);
    } else {
        EV_DisableCharacterDisplay(coinDisplays[0]);
        EV_DisableCharacterDisplay(coinDisplays[1]);
        EV_EnableCharacterDisplay(coinDisplays[2]);
    }
    for (auto name : payoutDisplays)
        EV_DisableCharacterDisplay(name);
}
void SetupLights() {
    for (int i = 0; i < 256; ++i)
        EV_SetIndexForLightEntry(i, 0);
    for (int i = 1; i <= 6; ++i)
        ResetLightRuntimeEntry(i);
    for (int i = 1; i <= 6; ++i)
        EV_SetLightFlagForIndex(i, 4);
    const Vector4 *colours[]{&redLight, &greenLight, &blueLight};
    for (int i = 1; i <= 6; ++i)
        SetLightRuntimeColour(i, *colours[(i - 1) % 3]);
    for (int i = 1; i <= 6; ++i)
        SetLightSceneIntensity(i, 8);
    constexpr float ranges[]{8, 8, 0.8f, 0.8f, 0.8f, 8};
    for (int i = 1; i <= 6; ++i)
        SetLightRange(i, ranges[i - 1]);
    for (int i = 4; i <= 6; ++i)
        SetLightConeAngle(i, 10922);
    const float falloff[]{1, 1};
    for (int i = 1; i <= 6; ++i)
        SetLightFalloff(i, falloff);
    for (int i = 1; i <= 6; ++i)
        EV_SetIndexForLightEntry(i, 1);
}
void RestoreLights() {
    for (int i = 0; i < 256; ++i)
        EV_SetIndexForLightEntry(i, 1);
    switch (g_currentMap.name) {
    case SHEN_NAME(WEG0):
        EV_SetObjectFlagByIndex(7, 0);
        EV_AssignLightParams(1, 1);
        EV_lightLoad(SHEN_NAME(0000));
        for (int i = 2; i <= 6; ++i)
            AssignLightProgram(i);
        break;
    case SHEN_NAME(WES1):
        EV_SetObjectFlagByIndex(2, 0);
        EV_AssignLightParams(1, 1);
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        break;
    case SHEN_NAME(QA22):
        EV_SetObjectFlagByIndex(1, 0);
        EV_lightLoad(SHEN_NAME(0001));
        PushGlobalLightingProgram();
        break;
    default:
        break;
    }
    SetupLightConfig();
    CheckIsArgEquZero(1);
}
// @verified - p
void Score(void *data) {
    auto *score = static_cast<Slt1ScoreWork *>(data);
    auto *work = Root(score);
    ++scoreFrame;
    if (work->celebration)
        ++work->celebration;
    if (work->handActive) {
        GetBattleDataConfig(0, eventButton);
        GetBattleDataConfig(0, cancelButton);
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
            GetBattleDataConfig(0, informationButton);
    }
    if (work->celebration) {
        if (work->celebration == 2) {
            SetupLights();
            lightAngles[0] = 0;
            lightAngles[1] = 21845;
            lightAngles[2] = 43690;
            switch (g_currentMap.name) {
            case SHEN_NAME(WEG0):
                EV_SetObjectFlagByIndex(7, 1);
                SetDefaultVSByIndex(4);
                SetDefaultVSByIndex(5);
                break;
            case SHEN_NAME(WES1):
                EV_SetObjectFlagByIndex(2, 1);
                SetDefaultVSByIndex(0);
                SetDefaultVSByIndex(2);
                break;
            case SHEN_NAME(QA22):
                EV_SetObjectFlagByIndex(1, 1);
                SetDefaultVSByIndex(0);
                SetDefaultVSByIndex(1);
                break;
            default:
                break;
            }
        }
        if (static_cast<std::uint32_t>(work->celebration) < 150) {
            for (auto &angle : lightAngles)
                angle -= 1820;
            Vector3 position;
            EV_GetCharaPosition(lamps[0], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            for (int i = 1; i <= 6; ++i)
                SetLightRuntimePosition(i, position);
            for (int i = 0; i < 3; ++i) {
                const Vector3 direction{cos_fixed16(static_cast<std::uint16_t>(lightAngles[i])), 0, sin_fixed16(static_cast<std::int16_t>(lightAngles[i]))};
                SetLightSunDirection(i + 1, direction);
                SetLightSunDirection(i + 4, direction);
            }
            for (auto name : lamps) {
                Vector3i rotation;
                GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::Local);
                rotation.y += 5461;
                EV_AT_SetAngY(name, EventTransformFlags::AllAxes, rotation);
            }
        }
        if (work->celebration == 150) {
            work->celebration = 1000;
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
                for (int i = 0; i < 256; ++i)
                    EV_SetIndexForLightEntry(i, 1);
                EV_SetObjectFlagByIndex(7, 0);
                EV_AssignLightParams(1, 1);
                EV_lightLoad(SHEN_NAME(0000));
                for (int i = 2; i <= 6; ++i)
                    AssignLightProgram(i);
                SetupLightConfig();
                CheckIsArgEquZero(1);
            }
        }
    }
    for (int i = 0; i < 5; ++i)
        if ((work->winningLines & (1U << i)) && (i == 0 || work->previousWager / work->denomination >= i + 1)) {
            if (scoreFrame % 10 == 0)
                Lamp(i + 1, 0);
            if (scoreFrame % 10 == 4)
                Lamp(i + 1, 1);
        }
    const float *bounce[]{reelBounceA, reelBounceB, reelBounceC};
    for (int i = 0; i < 3; ++i)
        if (work->bounce[i]) {
            const std::int32_t recoil = static_cast<std::int32_t>((bounce[shenGetRandomInt32(3)][10 - work->bounce[i]] * 8192.0f) / 45.0f);
            Vector3i rotation{work->angles[i] + static_cast<std::uint16_t>(recoil), 0, 0};
            EV_scenApplyPosOrRotToPartOf(drum, 153 + i, nullptr, &rotation, EventCharacterPartTransformMode::Absolute);
            --work->bounce[i];
        }
    constexpr int thresholds[]{10000, 1000, 200, 4};
    constexpr int increments[]{4000, 400, 40, 3};
    for (int i = 0; i < 3; ++i) {
        if (work->credits >= score->credits + thresholds[i])
            score->credits += increments[i];
        if (work->credits <= score->credits - thresholds[i])
            score->credits -= increments[i];
    }
    if (work->credits >= score->credits + 4)
        score->credits += 3;
    if (work->credits > score->credits)
        ++score->credits;
    if (work->credits <= score->credits - 4)
        score->credits -= 3;
    if (work->credits < score->credits)
        --score->credits;
    const Vector3 step{-0.02f, 0, 0}, creditPosition{0.27599999f, 1.001f, 0.28099999f}, payoutPosition{0.46599999f, 1.001f, 0.28099999f},
        wagerPosition{0.366f, 1.001f, 0.28099999f};
    scenCopyCharaToGame(slot);
    SetPosRotForGame(slot, 256, &step, 999999999, 0);
    UpdateGameNodeFlags(slot, score->credits, &creditPosition, 1, nullptr);
    SetPosRotForGame(slot, 256, &step, 99999, 0);
    UpdateGameNodeFlags(slot, work->payout, &payoutPosition, 1, nullptr);
    SetPosRotForGame(slot, 292, &step, 999, 0);
    UpdateGameNodeFlags(slot, work->wager, &wagerPosition, 1, nullptr);
}
void SetupHand(Slt1Work *work) {
    work->availableCoins = work->credits + work->coins + work->wager;
    work->slideLeft = 0;
    work->slideRight = 0;
    TrackEye(true);
    Place(ryo, slot, Vector3{0.31999999f, 0, 0.81999999f});
    Turn(ryo, slot, 0);
    Camera(3010);
    EV_EnsureCharacterDraw(hand);
    EV_EnableCharacterDisplay(hand);
    EV_DisableCharacterDisplay(ryo);
    Vector3 position;
    const Vector3i zero{};
    switch (work->selection) {
    case 0:
    case 1:
    case 2:
        HandRotation(hand, zero);
        position = {work->selection == 0 ? 0.094999999f : work->selection == 1 ? 0.185f : 0.27500001f, 0.93300003f, 0.917f};
        Pose(hand, handButton);
        break;
    case 3:
    case 4:
        position = {work->selection == 3 ? 0.36500001f : 0.45500001f, 0.93300003f, 0.89450002f};
        if (work->selection == 4)
            EV_EnableCharacterDisplay(hand);
        Pose(hand, handRelax);
        HandRotation(hand, Vector3i{64455, 60407, 0});
        break;
    case 5:
        Camera(3080);
        EV_DisableCharacterDisplay(hand);
        EV_EnableCharacterDisplay(ryo);
        Pose(ryo, handLever, 20);
        HandRotation(ryo, leverHandRotation);
        Motion(ryo, 0x2a03, 10, 1.6f);
        Motion(ryo, 0x2a03, 40, 0, 0x80000000U, 20);
        work->handDelay = 20;
        break;
    default:
        break;
    }
    EV_AT_SetPos(hand, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(hand, EventTransformFlags::AllAxes, Vector3i{2184, 2912, 14199});
    work->handActive = 1;
}
// @verified - p
void Main(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    switch (work->state) {
    case 0: {
        Change(AuthorStart);
        auto *task = HLib::EnqueueTaskWithParameter(Score, 5, 0x0b, sizeof(Slt1ScoreWork), SHEN_NAME(SLT2));
        auto *score = static_cast<Slt1ScoreWork *>(HLib::GetTaskParameter(task));
        score->task = task;
        score->root = work->task;
        score->credits = 0;
        work->score = task;
        work->selection = 1;
        work->authorState = 0;
        HLib::SetupTaskDelay(8);
        break;
    }
    case 1:
        SetupHand(work);
        Change(Betting);
        break;
    case 3:
        Change(Spin);
        break;
    case 4:
        Change(AuthorExit);
        break;
    case 5:
        Change(Exit);
        break;
    default:
        break;
    }
}
int Explain(Slt1Work *work) {
    work->availableCoins = work->coins + work->credits + work->wager;
    if (work->coins) {
        const char *voices[]{"0112A010", "0112A020", "0112A026", "0112A027"};
        EV_PlayDialogue(voices[shenGetRandomInt32(4)]);
    } else
        switch (work->denomination) {
        case 1:
            EV_PlayDialogue("0112A002");
            break;
        case 5:
            EV_PlayDialogue("0112A003");
            break;
        case 10:
            EV_PlayDialogue("0112A004");
            break;
        case 20:
            EV_PlayDialogue("0112A005");
            break;
        default:
            break;
        }
    DebugNullsub();
    return 10;
}
// @verified - p
int Choose(Slt1Work *work) {
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return 0;
    if (shenGetCtrlSpeakIconFlag() == 36) {
        const int total = work->credits + work->coins + work->wager;
        if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 5 && total < work->denomination) {
            EV_PlayDialogue("0112A014");
            work->authorState = 9;
            return 35;
        }
        if (total < work->denomination)
            EV_PlayDialogue("0112A006");
        else {
            const char *voices[]{"0112A007", "0112A008", "0112A009"};
            const char *continuing[]{"0112A016", "0112A017", "0112A018"};
            EV_PlayDialogue(total && !GetPlayerStat(PlayerStatsId::SegaCoins) ? continuing[shenGetRandomInt32(3)] : voices[shenGetRandomInt32(3)]);
        }
        work->authorState = 4;
        return 20;
    }
    if (work->credits + work->coins + work->wager && !GetPlayerStat(PlayerStatsId::SegaCoins)) {
        work->authorState = 9;
        return 0;
    }
    EV_PlayDialogue(shenGetRandomInt32(2) ? "0112A012" : "0112A011");
    work->authorState = 9;
    return 35;
}
int OddsEntry(const Slt1Work *work) {
    return work->entry > 38 ? work->entry - 39 : work->entry > 22 ? work->entry - 23 : work->entry - 1;
}
void InitializeMachine(Slt1Work *work) {
    cashToInsert = 0;
    coinInsertFrame = 0;
    moneyInsertFrame = 0;
    EV_DisableCharacterDisplay(work->machine);
    EV_EnableCharacterDisplay(slot);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(work->machine, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->machine, &rotation, EventTransformFlags::World);
    EV_AT_SetPos(slot, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(slot, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    Place(ryo, slot, Vector3{0.31999999f, 0, 0.81999999f});
    Turn(ryo, slot, 0);
    Motion(ryo, 0xf002);
    EV_EnsureCharacterDraw(drum);
    EV_EnableCharacterDisplay(drum);
    AT_SetLocalBaseChara(drum, slot);
    EV_AT_SetPos(drum, EventTransformFlags::AllAxes, Vector3{});
    EV_AT_SetAngY(drum, EventTransformFlags::AllAxes, Vector3i{});
    for (int i = 1; i <= 5; ++i)
        Lamp(i, 0);
    for (int i = 10; i <= 12; ++i)
        Lamp(i, 0);
    for (auto name : lamps)
        EV_EnsureCharacterDraw(name);
    for (auto name : lamps)
        AT_SetLocalBaseChara(name, slot);
    constexpr float heights[]{1.72f, 1.64f, 1.5599999f};
    constexpr int angles[]{43690, 21845, 0};
    for (int i = 0; i < 3; ++i) {
        EV_AT_SetPos(lamps[i], EventTransformFlags::AllAxes, Vector3{0.30000001f, heights[i], 0.138f});
        EV_AT_SetAngY(lamps[i], EventTransformFlags::AllAxes, Vector3i{0, angles[i], 0});
    }
    for (auto name : lamps)
        EV_EnableCharacterDisplay(name);
    for (auto name : payoutDisplays)
        EV_EnsureCharacterDraw(name);
    for (auto name : payoutDisplays)
        AT_SetLocalBaseChara(name, slot);
    for (auto name : payoutDisplays)
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, Vector3{});
    for (auto name : payoutDisplays)
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{});
    for (auto name : payoutDisplays)
        EV_DisableCharacterDisplay(name);
    for (auto name : coinDisplays)
        EV_EnsureCharacterDraw(name);
    for (auto name : coinDisplays)
        AT_SetLocalBaseChara(name, slot);
    for (auto name : coinDisplays)
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, Vector3{0.28099999f, 0.597f, 0.26899999f});
    for (auto name : coinDisplays)
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{});
    for (auto name : coinDisplays)
        EV_DisableCharacterDisplay(name);
    SetLowQualityHands(ctrlGetCharaByName(ryo));
    SetLowQualityHands(ctrlGetCharaByName(hand));
    SetLowQualityHands(ctrlGetCharaByName(prizeHand));
    SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(hand), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(prizeHand), 1, 0, 0);
    AT_SetLocalBaseChara(hand, slot);
    AT_SetLocalBaseChara(prizeHand, slot);
    EV_DisableCharacterDisplay(prizeHand);
    EV_AT_SetPos(CHAR_SLOT_EYE, EventTransformFlags::AllAxes, Vector3{0.30000001f, 1.25f, 0.25f});
    TrackEye(true);
    DateTime time;
    GetCurrentDateTime(time);
    if (static_cast<std::uint32_t>(time.hour - 5) <= 4 && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2061) &&
        sltOdds[22 * EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 35) + 48 + OddsEntry(work)] == 99) {
        constexpr int angles[]{41692, 59560, 29780};
        for (int i = 0; i < 3; ++i) {
            Vector3i rotation{angles[i], 0, 0};
            EV_scenApplyPosOrRotToPartOf(drum, 153 + i, nullptr, &rotation, EventCharacterPartTransformMode::Absolute);
        }
        work->forceJackpot = 1;
    }
    work->authorState = 1;
}
void AuthorStart(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    auto *score = static_cast<Slt1ScoreWork *>(HLib::GetTaskParameter(work->score));
    int delay = 0;
    switch (work->authorState) {
    case 0:
        InitializeMachine(work);
        break;
    case 1:
        EV_DisableCharacterDisplay(hand);
        EV_EnableCharacterDisplay(ryo);
        Motion(ryo, 0xf002);
        Camera(3000);
        delay = Explain(work);
        work->authorState = 2;
        break;
    case 2: {
        Motion(ryo, 0xf002);
        std::int16_t choices[]{-1, 36, 23, -1};
        SetupControlSpeakAndUI(choices, 200, 0, 2);
        work->authorState = 3;
        break;
    }
    case 3:
        delay = Choose(work);
        break;
    case 4:
        SetPlayerStat(PlayerStatsId::SegaCoins, 0);
        if (!work->credits) {
            for (auto name : payoutDisplays)
                EV_DisableCharacterDisplay(name);
            for (auto name : coinDisplays)
                EV_DisableCharacterDisplay(name);
        }
        score->credits = work->credits;
        if (work->availableCoins >= work->denomination || static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 5)
            work->authorState = 8;
        else {
            SetMoneyRelatedUIElement(100, 0, 5);
            work->authorState = 6;
            work->handActive = 0;
            Camera(3030);
            EV_EnableCharacterDisplay(ryo);
        }
        break;
    case 6:
        if (GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
            break;
        cashToInsert = static_cast<std::int32_t>(GetNumBet());
        if (work->wager && cashToInsert <= 0) {
            work->authorState = 8;
            break;
        }
        if (!work->wager && cashToInsert <= 0) {
            if (work->credits + work->coins < work->denomination)
                EV_PlayDialogue(static_cast<std::uint8_t>(GetRandomNumberInRange(2)) ? "0112A012" : "0112A011");
            else {
                const char *voices[]{"0112A025", "0112A026", "0112A027"};
                EV_PlayDialogue(voices[static_cast<std::uint8_t>(GetRandomNumberInRange(3))]);
            }
            work->authorState = 9;
            delay = 35;
            break;
        }
        SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash)) - cashToInsert);
        work->authorState = 7;
        work->handActive = 0;
        break;
    case 7:
        if (work->selection != 1) {
            const char *voices[]{"0112A007", "0112A008", "0112A009"};
            EV_PlayDialogue(voices[shenGetRandomInt32(3)]);
        }
        EV_DisableCharacterDisplay(hand);
        TrackEye(false);
        EV_EnableCharacterDisplay(ryo);
        Camera(3040);
        Motion(ryo, 0x2a04);
        Pose(ryo, handRelax, 50);
        work->handDelay = -1;
        work->state = 1;
        work->selection = 4;
        work->handActive = 0;
        Change(Main);
        delay = 95;
        break;
    case 8:
        TrackEye(true);
        work->state = 1;
        Change(Main);
        break;
    case 9:
        work->state = 4;
        Change(Main);
        break;
    default:
        break;
    }
    if (delay)
        HLib::SetupTaskDelay(delay);
}
int Symbol(int reel, int stop, int delta) {
    if (delta < 0 && stop <= 0)
        return 0;
    if (delta > 0 && stop + 1 >= 22)
        return 5;
    return sltStrip[22 * reel + stop + delta];
}
// @verified - p
std::uint32_t Payline(std::int16_t line, int first, int second, int third, Slt1Work *work) {
    std::uint32_t payout = 0;
    const int bet = sltBet[work->entry - 1];
    for (int i = 0; i < 17; ++i) {
        if (first != sltPatterns[3 * i])
            continue;
        if (first == 1)
            payout = 2 * bet;
        if (second != sltPatterns[3 * i + 1])
            continue;
        if (second == 1)
            payout = 5 * bet;
        if (third != sltPatterns[3 * i + 2])
            continue;
        if (!first && !second && !third)
            payout = 0;
        else if (first == 5 && second == 5 && third == 5) {
            work->celebration = 1;
            constexpr int jackpots[]{300, 500, 1000, 1500, 3000};
            if (line >= 1 && line <= 5)
                payout = jackpots[line - 1] * bet;
        } else {
            payout = sltPay[i] * bet;
            if (first == 4 && second == 4 && third == 4)
                work->celebration = 1;
        }
    }
    if (payout && line >= 1 && line <= 5)
        work->winningLines |= 1U << (line - 1);
    return payout;
}
// @verified - p
std::int32_t Evaluate(Slt1Work *work) {
    work->payout = 0;
    work->celebration = 0;
    constexpr int offsets[5][3]{{0, 0, 0}, {-1, -1, -1}, {1, 1, 1}, {-1, 0, 1}, {1, 0, -1}};
    for (std::int16_t line = 1; line < work->wager / work->denomination + 1; ++line)
        if (line <= 5)
            work->payout += Payline(line, Symbol(0, work->stops[0], offsets[line - 1][0]), Symbol(1, work->stops[1], offsets[line - 1][1]),
                Symbol(2, work->stops[2], offsets[line - 1][2]), work);
    work->credits += work->payout;
    if (work->credits >= 100000000)
        work->credits = 99999999;
    int delay = 0;
    if (work->payout) {
        Loop3DSoundEffect(0x13c);
        delay = 30;
    }
    if (work->celebration) {
        Loop3DSoundEffect(0x13d);
        Camera(3050);
        delay = 150;
    }
    return delay;
}
// @verified - p
void AdvanceReels(Slt1Work *work) {
    for (int i = static_cast<std::int8_t>(work->stoppedReels); i < 3; ++i) {
        work->reelTimers[i] = static_cast<std::int8_t>(work->reelTimers[i] - 1);
        const int angle = work->angles[i] + work->speeds[i];
        work->angles[i] = angle < 65516 ? angle : 0;
        if (work->reelTimers[i] < 0 && work->angles[i] == (work->stops[i] ? 2978 * (22 - work->stops[i]) : 0)) {
            work->bounce[i] = 10;
            if (i == 2) {
                Loop3DSoundEffect(0x13f);
                work->stoppedReels = 0;
            } else
                ++work->stoppedReels;
            Loop3DSoundEffect(0x140);
            work->speeds[i] = 0;
        }
    }
}
// @verified - p
void Spin(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    switch (work->drumState) {
    case 0: {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2061, 1);
        SlotInitializationNullsub();
        EV_DisableCharacterDisplay(hand);
        EV_DisableCharacterDisplay(ryo);
        EVS_LoopCamera(3020, slot, slot);
        camCtrlDisableFilmBars();
        Loop3DSoundEffect(0x13e);
        float random = GetRandomNumberInRange(100);
        auto oddsIndex = sltOdds[22 * EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 35) + 48 + OddsEntry(work)];
        if (oddsIndex == 99)
            oddsIndex = 3;
        const auto *odds = &sltOdds[6 * oddsIndex];
        int outcome = 0;
        random -= odds[0];
        while (random > 0)
            random -= odds[++outcome];
        const int indices[]{
            static_cast<int>(GetRandomNumberInRange(66)), static_cast<int>(GetRandomNumberInRange(66)), static_cast<int>(GetRandomNumberInRange(66))};
        if (work->forceJackpot) {
            work->stops[0] = 0;
            work->stops[1] = 0;
            work->stops[2] = 0;
            work->forceJackpot = 0;
        } else
            for (int i = 0; i < 3; ++i)
                work->stops[i] = sltStops[198 * outcome + 66 * i + indices[i]];
        for (int i = 0; i < 3; ++i) {
            work->reelTimers[i] = 60 + 30 * i;
            work->speeds[i] = 2978;
        }
        work->drumState = 1;
        break;
    }
    case 1:
        AdvanceReels(work);
        for (int i = 0; i < 3; ++i) {
            Vector3i rotation{work->angles[i], 0, 0};
            EV_scenApplyPosOrRotToPartOf(drum, 153 + i, nullptr, &rotation, EventCharacterPartTransformMode::Absolute);
        }
        if (!work->speeds[2]) {
            work->drumState = 2;
            HLib::SetupTaskDelay(10);
        }
        break;
    case 2: {
        FinishLoading();
        const int delay = Evaluate(work);
        work->drumState = 3;
        if (delay)
            HLib::SetupTaskDelay(delay);
        break;
    }
    case 3:
        work->wager = 0;
        if (static_cast<std::uint32_t>(work->celebration) < 1000 || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            if (work->credits + work->coins >= work->denomination) {
                work->state = 1;
                work->selection = 1;
                Change(Main);
            } else {
                Change(AuthorStart);
                work->authorState = 2;
            }
        } else {
            Change(Prize);
            work->prizeState = 0;
            work->celebration = 0;
        }
        break;
    case 4:
        Change(Main);
        work->state = 1;
        work->selection = 1;
        break;
    default:
        break;
    }
}
void ButtonLamp(int index) {
    auto &frame = buttonLampFrames[index];
    if (!frame)
        Lamp(10 + index, 1);
    if (++frame > 4) {
        Lamp(10 + index, 0);
        HLib::DestroyCurrentTask();
        frame = 0;
    }
}
void ButtonLampPayOut(void *) {
    ButtonLamp(0);
}
void ButtonLampBet(void *) {
    ButtonLamp(1);
}
void ButtonLampStart(void *) {
    ButtonLamp(2);
}
void BeginInsert(bool money) {
    Pose(hand, handInsert, 4);
    Queue_EVT_AT_CtrlHandUp_1(hand, 0, reinterpret_cast<std::byte *>(handRelax), 4, 4);
    if (money)
        moneyInsertFrame = 1;
    else
        coinInsertFrame = 1;
    Change(money ? InsertMoney : InsertCoin);
}
void ShiftLeft(Slt1Work *work) {
    if (work->selection) {
        work->slideLeft = 1;
        --work->selection;
    }
    if (work->selection == 2)
        Pose(hand, handButton, 8);
    work->handDelay = 8;
}
void ShiftRight(Slt1Work *work) {
    if (work->selection == 4) {
        if (work->wager) {
            work->selection = 5;
            work->state = 1;
            Change(Main);
        }
    } else {
        work->slideRight = 1;
        ++work->selection;
    }
    if (work->selection == 3)
        Pose(hand, handRelax, 8);
    work->handDelay = 8;
}
void Betting(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (GetCurrentControlInput(INPUT_NEW_PRESSES) == 0x4000 && !work->slideLeft && !work->slideRight && !work->handDelay) {
        work->insertCoins = 0;
        if (work->selection == 5) {
            Motion(ryo, 0x2a03, 85, 1.8f, 0x80000000);
            Pose(hand, handRelax, 4);
            work->selection = 4;
            work->handDelay = 24;
            work->state = 1;
            Change(Main);
            HLib::SetupTaskDelay(24);
        } else if (work->selection) {
            work->slideLeft = 1;
            --work->selection;
        }
        if (work->selection == 2)
            Pose(hand, handButton, 8);
        work->handDelay = 8;
    }
    if (GetCurrentControlInput(INPUT_NEW_PRESSES) == 0x8000 && !work->slideLeft && !work->slideRight && !work->handDelay) {
        work->insertCoins = 0;
        if (work->selection == 4 && work->wager) {
            work->selection = 5;
            work->state = 1;
            Change(Main);
        }
        if (work->selection < 4) {
            work->slideRight = 1;
            ++work->selection;
        }
        if (work->selection == 3)
            Pose(hand, handRelax, 8);
        work->handDelay = 8;
    }
    if (work->slideLeft) {
        Vector3 position;
        EV_GetCharaPosition(hand, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        const float step = handSlide[work->slideLeft - 1];
        position.x -= step;
        if (work->selection == 2)
            position.z += step * 0.25f;
        EV_AT_SetPos(hand, EventTransformFlags::AllAxes, position);
        if (work->selection == 2) {
            Vector3i angles;
            GetCharacterHandRotation(ctrlGetCharaByName(hand), 0, &angles);
            angles.x += static_cast<int>((handTurn[work->slideLeft - 1] * 8192.0f) / 45.0f);
            angles.y += static_cast<int>(((handTurn[work->slideLeft - 1] * 3.0f) * 8192.0f) / 45.0f);
            HandRotation(hand, angles);
        }
        if (++work->slideLeft > 8) {
            work->slideLeft = 0;
            if (GetCurrentControlInput(INPUT_HELD) == 0x4000)
                ShiftLeft(work);
        }
    }
    if (work->slideRight) {
        Vector3 position;
        EV_GetCharaPosition(hand, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        const float step = handSlide[work->slideRight - 1];
        position.x += step;
        if (work->selection == 3)
            position.z -= step * 0.25f;
        EV_AT_SetPos(hand, EventTransformFlags::AllAxes, position);
        if (work->selection == 3) {
            Vector3i angles;
            GetCharacterHandRotation(ctrlGetCharaByName(hand), 0, &angles);
            angles.x -= static_cast<int>((handTurn[work->slideRight - 1] * 8192.0f) / 45.0f);
            angles.y -= static_cast<int>(((handTurn[work->slideRight - 1] * 3.0f) * 8192.0f) / 45.0f);
            HandRotation(hand, angles);
        }
        if (++work->slideRight > 8) {
            work->slideRight = 0;
            if (GetCurrentControlInput(INPUT_HELD) == 0x8000)
                ShiftRight(work);
        }
    }
    if (!work->statusTimer && GetBattleDataConfig(1, eventButton) && GetBattleDataConfig(2, eventButton) && !work->handDelay) {
        switch (work->selection) {
        case 0:
        case 1:
        case 2: {
            constexpr HLib::TaskCallback lights[]{ButtonLampPayOut, ButtonLampBet, ButtonLampStart};
            constexpr HLib::TaskCallback actions[]{PayOut, Bet, StartButton};
            auto *task = HLib::EnqueueTaskWithoutParameter(lights[work->selection], 4, 0x0b, eventToken.name);
            HLib::SetupTimedTask(task, 3);
            Pose(hand, handPress, 4);
            Change(actions[work->selection]);
            HLib::SetupTaskDelay(work->selection ? 4 : 5);
            break;
        }
        case 3:
            if (!work->wager)
                ClearWinningLamps(work);
            if (work->insertCoins > 0) {
                work->insertCoins -= work->denomination;
                BeginInsert(false);
            } else if (work->coins > 0)
                Change(SelectCoin);
            break;
        case 4:
            Change(SelectMoney);
            break;
        case 5:
            Change(PullLever);
            break;
        default:
            break;
        }
    }
    if (GetBattleDataConfig(1, cancelButton) && GetBattleDataConfig(2, cancelButton) && !work->handDelay) {
        EV_DisableCharacterDisplay(ryo);
        Camera(3010);
        EV_PlayDialogue("0112A027");
        work->authorState = 2;
        Change(AuthorStart);
        HLib::SetupTaskDelay(40);
    }
    if (!work->statusTimer && GetBattleDataConfig(1, informationButton) && GetBattleDataConfig(2, informationButton)) {
        const int total = work->credits + work->coins + work->wager;
        work->statusTimer = 120;
        AnimateMoneyAmount(total, total, 1);
    }
    if (work->statusTimer)
        --work->statusTimer;
    if (work->handDelay > 0)
        --work->handDelay;
    if (work->insertCoins == 5 * work->denomination) {
        work->insertCoins -= work->denomination;
        BeginInsert(false);
    }
    if (cashToInsert > 0) {
        cashToInsert -= 5;
        BeginInsert(true);
    }
    if (!work->slideLeft && !work->slideRight && !work->handDelay && !work->statusTimer && !shenGetMinutesUntilNextFrameMinute()) {
        Change(AuthorStart);
        work->authorState = 9;
    }
}
void PayOut(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (!work->credits && !work->wager) {
        Loop3DSoundEffect(0x143);
        Pose(hand, handButton, 4);
        work->state = 1;
        Change(Main);
        HLib::SetupTaskDelay(4);
        return;
    }
    EV_DisableCharacterDisplay(hand);
    for (int i = 1; i <= 5; ++i)
        Lamp(i, 0);
    work->credits += work->wager;
    work->wager = 0;
    work->previousWager = 0;
    work->winningLines = 0;
    Loop3DSoundEffect(0x143);
    EVT_AT_SetSound(0x147, 10);
    Camera(3060);
    Change(AuthorStart);
    CoinDisplay(work);
    int index, count, delay;
    float soundDelay, end;
    if (work->credits < 5) {
        index = 0;
        count = 1;
        delay = 40;
        soundDelay = 25;
        end = 32;
    } else if (work->credits < 15) {
        index = 1;
        count = 10;
        delay = 70;
        soundDelay = 40;
        end = 68;
    } else if (work->credits < 40) {
        index = 2;
        count = 30;
        delay = 90;
        soundDelay = 50;
        end = 82;
    } else {
        index = 3;
        count = 50;
        delay = 160;
        soundDelay = 100;
        end = 160;
    }
    EVT_AT_SetSound(0x148, static_cast<int>(soundDelay));
    const char *resources[]{"ST01", "ST10", "ST30", "ST50"};
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), eventToken);
    RegisterEventTask(task);
    auto *request = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
    request->character = payoutDisplays[index];
    request->resource = resources[index];
    request->parts = cashParts;
    request->trackCount = count;
    request->reserved14 = 0x40000000U;
    request->endFrame = end;
    request->frameStep = 2;
    request->currentFrame = 2;
    request->delay = 0;
    HLib::SetupTimedTask(work->task, delay);
    EV_EnableCharacterDisplay(payoutDisplays[index]);
    work->coins += work->credits;
    work->credits = 0;
    work->handActive = 0;
    work->authorState = 1;
}
// @verified - p
void Bet(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (work->credits < work->denomination || work->wager >= 5 * work->denomination)
        Loop3DSoundEffect(0x143);
    else {
        if (!work->wager)
            ClearWinningLamps(work);
        Loop3DSoundEffect(0xd75);
        work->wager += work->denomination;
        work->credits -= work->denomination;
        work->previousWager = work->wager;
        Lamp(work->wager / work->denomination, 1);
    }
    if (work->credits >= work->denomination || work->wager / work->denomination >= 5) {
        Pose(hand, handButton, 4);
        work->state = 1;
        Change(Main);
    } else {
        Pose(hand, handButton, 4);
        if (work->coins >= work->denomination) {
            Loop3DSoundEffect(0x143);
            work->state = 1;
            Change(Main);
        } else if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 5) {
            Pose(hand, handButton, 4);
            work->state = 1;
            Change(Main);
        } else {
            SetMoneyRelatedUIElement(100, 0, 5);
            work->handActive = 0;
            work->authorState = 6;
            Change(AuthorStart);
        }
    }
    HLib::SetupTaskDelay(4);
}
void StartButton(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    Pose(hand, handButton, 4);
    if (work->wager) {
        Loop3DSoundEffect(0x141);
        work->drumState = 0;
        work->payout = 0;
        work->handActive = 0;
        Change(Main);
        work->state = 3;
    } else {
        Loop3DSoundEffect(0x143);
        work->state = 1;
        Change(Main);
        HLib::SetupTaskDelay(4);
    }
}
void SelectCoin(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    EV_EnableCharacterDisplay(ryo);
    EV_DisableCharacterDisplay(hand);
    Camera(3070);
    Place(ryo, slot, coinPosition);
    Turn(ryo, slot, 0);
    Motion(ryo, 0x2a0a, 0, 1.6f);
    EVT_AT_SetSound(0x149, 40);
    work->state = 1;
    work->selection = 3;
    work->handActive = 0;
    work->insertCoins = work->coins < 5 * work->denomination ? work->coins : 5 * work->denomination;
    Change(Main);
    HLib::SetupTimedTask(work->task, 60);
}
// @verified - p
void SelectMoney(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 5)
        work->authorState = 8;
    else {
        SetMoneyRelatedUIElement(100, 0, 5);
        work->authorState = 6;
        work->handActive = 0;
        Camera(3030);
        EV_EnableCharacterDisplay(ryo);
    }
    Change(AuthorStart);
}
void PullLever(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (!leverFrame) {
        Motion(ryo, 0x2a03, 40, 1.8f);
        Loop3DSoundEffect(0x146);
        work->drumState = 0;
        work->payout = 0;
        work->handActive = 0;
    }
    constexpr int turns[]{-218, 109, 436, 1310, 982, 218, -218, -436, -1092, -1092};
    Vector3 position{};
    Vector3i rotation{leverFrame >= 0 && leverFrame < 20 ? turns[leverFrame / 2] : 0, 0, 0};
    EV_scenApplyPosOrRotToPartOf(slot, 0xffbe, &position, &rotation, EventCharacterPartTransformMode::Relative);
    if (++leverFrame == 30) {
        work->state = 3;
        work->selection = 1;
        Change(Main);
        leverFrame = 0;
    }
}
void InsertHand(std::int8_t frame) {
    Vector3i rotation;
    GetCharacterHandRotation(ctrlGetCharaByName(hand), 0, &rotation);
    rotation.y += static_cast<int>((insertTurn[frame - 1] * 8192.0f) / 45.0f);
    HandRotation(hand, rotation);
}
void InsertCoin(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (!coinInsertFrame)
        return;
    InsertHand(coinInsertFrame);
    if (++coinInsertFrame >= 9) {
        coinInsertFrame = 0;
        work->state = 1;
        Change(Main);
    }
    if (coinInsertFrame == 5) {
        if (work->coins < work->denomination) {
            work->credits += work->coins;
            work->coins = 0;
        } else if (work->wager >= 5 * work->denomination) {
            work->credits += work->denomination;
            work->coins -= work->denomination;
        } else {
            work->wager += work->denomination;
            work->coins -= work->denomination;
            Lamp(work->wager / work->denomination, 1);
        }
        Loop3DSoundEffect(0x151);
    }
}
void InsertMoney(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    if (moneyInsertFrame) {
        InsertHand(moneyInsertFrame);
        if (++moneyInsertFrame >= 9) {
            moneyInsertFrame = 0;
            work->state = 1;
            Change(Main);
            if (!cashToInsert)
                work->handDelay = 0;
        }
        if (moneyInsertFrame == 5) {
            work->credits += 20;
            Loop3DSoundEffect(0x151);
        }
    }
    if (work->credits >= 100000000)
        work->credits = 99999999;
}
void GrantPrize(void *) {
    switch (prizeDenomination) {
    case 1:
        AddInventoryItem(4144, 1, 1);
        break;
    case 5:
        AddInventoryItem(4153, 1, 1);
        break;
    case 10:
        AddInventoryItem(4146, 1, 1);
        break;
    case 20:
        AddInventoryItem(4145, 1, 1);
        break;
    default:
        break;
    }
    HLib::DestroyCurrentTask();
}
void Point(shen_name name, shen_name target, int delay, std::uint32_t flags) {
    EVT_AT_SetPoint(name, target, 5, 1, 0, delay, 0, flags);
}
void Prize(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    switch (work->prizeState) {
    case 0:
        switch (g_currentMap.name) {
        case SHEN_NAME(WEG0):
            work->dealer = SHEN_NAME(GST_);
            break;
        case SHEN_NAME(WES1):
            work->dealer = SHEN_NAME(XHG_);
            break;
        case SHEN_NAME(QA22):
            work->dealer = SHEN_NAME(AGN_);
            break;
        default:
            break;
        }
        switch (work->denomination) {
        case 1:
            work->prize = SHEN_NAME(KH01);
            break;
        case 5:
            work->prize = SHEN_NAME(KH05);
            break;
        case 10:
            work->prize = SHEN_NAME(KH10);
            break;
        case 20:
            work->prize = SHEN_NAME(KH20);
            break;
        default:
            break;
        }
        prizeDenomination = work->denomination;
        EV_CheckWarmStartNpcRoute(work->dealer);
        mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
        work->prizeState = 1;
        break;
    case 1:
        if (EV_ForceLoadNpc(work->dealer)) {
            SetScenarioReflectorBoardValues(work->dealer, &prizeReflector);
            EV_SetRefBoardParamsFor(work->dealer, 1);
            EV_SetRefBoardParamsFor(work->dealer, 1);
            EV_SetCharacterAlphaEnabled(work->dealer, false);
            Gravity(work->dealer, false);
            Place(work->dealer, ryo, dealerPosition);
            Turn(work->dealer, ryo, 300);
            Point(work->dealer, ryo, 0, 0x80000000U);
            EV_EnsureCharacterDraw(prizeHand);
            Place(prizeHand, ryo, Vector3{0, 1.4f, 0});
            Turn(prizeHand, ryo, 145);
            EV_DisableCharacterDisplay(prizeHand);
            HandRotation(prizeHand, prizeHandRotation);
            Pose(prizeHand, handPrize);
            const Vector3 scale{0.5f, 0.5f, 0.5f};
            SetMotionworkFlagForCharaById(work->prize, &scale);
            Place(work->prize, prizeHand, prizePosition);
            Turn(work->prize, prizeHand, 0);
            EV_DisableCharacterDisplay(work->prize);
            FindMotionWork(ctrlGetCharaByName(work->dealer));
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work->dealer), FacialExpressionId::Scheme, 10, 1);
            work->prizeState = 2;
        }
        break;
    case 2: {
        RestoreLights();
        EV_EnableCharacterDisplay(ryo);
        const bool flower = g_currentMap.name == SHEN_NAME(WEG0);
        if (flower) {
            EVT_AT_SetVoice("0112C010", 0); // @text EN: "Great." | JA: "おめでとさん。"
            Motion(work->dealer, 0xf079);
        }
        if (g_currentMap == AREA_WES1) {
            EVT_AT_SetVoice("0112B011", 0); // @text EN: "You're amazing." | JA: "すげえな、あんた。"
            Motion(work->dealer, 0xf087);
        }
        if (g_currentMap == AREA_QA22) {
            EVT_AT_SetVoice("0112D003", 0); // @text EN: "Great!" | JA: "おめでとうございます。"
            Motion(work->dealer, 0xf087);
        }
        EVT_AT_SetCam(5000, slot, slot, 0, 0, 0);
        Motion(ryo, 0xf022, 0, 1, 0x80000000U, 40);
        Point(ryo, work->dealer, 40, 0x20000000U);
        Point(work->dealer, ryo, 40, 0x20000000U);
        if (flower) {
            EVT_AT_SetVoice("0112C003", 80); // @text EN: "Congratulations. Here's your prize.\nTake it." | JA: "兄ちゃんやるなぁ。\nほれ、景品だ。まあとっときなよ。"
            Motion(work->dealer, 0x2a11, 0, 0.8f, 0x80000000U, 80);
        }
        if (g_currentMap == AREA_WES1) {
            EVT_AT_SetVoice("0112B003", 80); // @text EN: "You're great. Here's the prize.\nKeep it." | JA: "あんた、すげえな。\n景品だ、とっときな。"
            Motion(work->dealer, 0x2a12, 0, 0.8f, 0x80000000U, 80);
        }
        if (g_currentMap == AREA_QA22) {
            EVT_AT_SetVoice("0112D004", 80); // @text EN: "I'll give you a prize.\nTake it." | JA: "景品を差し上げます。\nどうぞお受け取りください。"
            Motion(work->dealer, 0x2a12, 0, 0.8f, 0x80000000U, 80);
        }
        EVT_AT_SetCam(5005, slot, slot, 0, 0, 80);
        Place(ryo, ryo, Vector3{}, 80);
        Turn(ryo, ryo, 90, 80);
        Motion(ryo, 0x2a0f, 110, 1, 0x80000000U, 80);
        Point(ryo, work->dealer, 80, 0x20000000U);
        Point(work->dealer, ryo, 80, 0x10000000U);
        Point(work->dealer, ryo, 120, 0x20000000U);
        Place(ryo, ryo, Vector3{}, 160);
        Turn(ryo, ryo, -90, 160);
        EVT_AT_ObjectDisp_1(prizeHand, 160, 0);
        EVT_AT_ObjectDisp_1(ryo, 160, 1);
        Motion(work->dealer, flower ? 0xf079 : 0xf087, 0, 1, 0x40000000U, 160);
        EVT_AT_SetCam(5010, slot, slot, 0, 0, 160);
        EVT_AT_ObjectDisp_1(work->prize, 160, 0);
        auto *grant = HLib::EnqueueTaskWithoutParameter(GrantPrize, 4, 0x0b, eventToken.name);
        HLib::SetupTimedTask(grant, 160);
        Place(ryo, ryo, Vector3{}, 270);
        Turn(ryo, ryo, 90, 270);
        Motion(ryo, 0x2a10, 0, 1, 0x40000000U, 270);
        Motion(work->dealer, flower ? 0xf079 : 0xf087, 0, 1, 0x40000000U, 270);
        EVT_AT_SetCam(5020, slot, slot, 0, 0, 270);
        EVT_AT_ObjectDisp_1(prizeHand, 270, 1);
        EVT_AT_ObjectDisp_1(work->prize, 270, 1);
        EVT_AT_ObjectDisp_1(ryo, 270, 0);
        EVT_AT_ObjectDisp_1(work->dealer, 270, 0);
        EVT_AT_SetVoice("0112A001", 270); // @text EN: "Thanks." | JA: "ありがとう。"
        EVT_AT_MenCtrl(EventFilmControl{0, 10, 0, 0, 0, 0, 255, 0, 0, 0, 340});
        EVT_AT_MenCtrl(EventFilmControl{0, 10, 255, 0, 0, 0, 0, 0, 0, 0, 350});
        Place(ryo, ryo, Vector3{}, 350);
        Turn(ryo, ryo, -90, 350);
        HLib::SetupTaskDelay(350);
        work->prizeState = 3;
        break;
    }
    case 3:
        Point(ryo, ryo, 0, 0x10000000U);
        Gravity(work->dealer, true);
        EV_npcFree(work->dealer);
        EV_npcLockUnlock(work->dealer);
        Change(Main);
        work->selection = 1;
        work->state = 1;
        break;
    default:
        break;
    }
}
void AuthorExit(void *data) {
    auto *work = static_cast<Slt1Work *>(data);
    std::uint16_t delay = 0;
    if (!shenGetMinutesUntilNextFrameMinute()) {
        EV_PlayDialogue("0112A027");
        delay = 40;
    }
    work->state = 5;
    Change(Main);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
void Exit(void *data) {
    using enum CameraState;
    auto *work = static_cast<Slt1Work *>(data);
    for (auto name : lamps)
        AT_SetLocalBaseChara(name, invalid);
    if (work->coins == -1)
        work->coins = 0;
    work->credits += work->coins + work->wager;
    if (work->credits >= 100000000)
        work->credits = 99999999;
    int ownsCoins = 0;
    if (work->credits) {
        EV_setHoldStatesFlags(0);
        ownsCoins = 1;
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1809, ownsCoins);
    SetPlayerStat(PlayerStatsId::SegaCoins, work->credits);
    AT_SetLocalBaseChara(hand, invalid);
    hndlSetFlags(ctrlGetCharaByName(ryo));
    hndlSetFlags(ctrlGetCharaByName(hand));
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    AT_SetLocalBaseChara(CHAR_SLOT_EYE, invalid);
    SetScenarioCharacterKillMode(CHAR_SLOT_EYE, 1);
    EV_EnableCharacterDisplay(ryo);
    EV_EnableCharacterDisplay(work->machine);
    EV_DisableCharacterDisplay(slot);
    LoadCutsceneAndSoundProgramForArea();
    EV_FreeAuthorPak(0, "0112");
    Gravity(ryo, true);
    EV_NPC_NewLoadSw(1);
    EVS_LoopCamera(0, shen_name{}, shen_name{});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SLT1"), 0);
    HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
}
} // namespace
// @verified - p
void QueueSlt1(std::uintptr_t argument) {
    const auto *parameters = reinterpret_cast<const Slt1EnqueueParams *>(argument);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Main, 5, 0x0b, sizeof(Slt1Work), TASK_SLT1);
    auto *work = static_cast<Slt1Work *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->machine = parameters->machine;
    work->entry = parameters->entry;
    work->state = 0;
    work->credits = 0;
    work->wager = 0;
    work->previousWager = 0;
    work->payout = 0;
    work->stoppedReels = 0;
    work->bounce[0] = 0;
    work->bounce[1] = 0;
    work->bounce[2] = 0;
    work->forceJackpot = 0;
    work->celebration = 0;
    work->coins = 0;
    work->insertCoins = 0;
    work->denomination = sltBet[work->entry - 1];
    work->winningLines = 0;
    work->handActive = 0;
    work->statusTimer = 0;
    work->handDelay = 0;
    work->credits = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::SegaCoins));
    SetPlayerGameFlags(PlayerGameFlagMode::SearchBlocked, reinterpret_cast<void *>(1));
    SetPlayerControlEnabled(0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, CHAR_SLOT_EYE);
    AT_SetLocalBaseChara(CHAR_SLOT_EYE, slot);
    EV_SetNewSceneIndex();
    auto *fade = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), eventToken);
    RegisterEventTask(fade);
    *static_cast<EventFilmControl *>(HLib::GetTaskParameter(fade)) = {1, 16, 255, 0, 0, 0, 0, 0, 0, 0, 10};
    HLib::SetupTimedTask(fade, -1);
    Gravity(ryo, false);
    EV_SetCharacterAlphaEnabled(ryo, false);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SLT1"), 0);
    EV_FreeAuthorPak(1, "0112");
    EV_LoadCutsceneFromCurrentScene("01120000");
    EV_EventSetUpHelpIcon_0(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? 0x94 : 0x1c, 0);
}
const ScriptAreaDefinition slt1ScriptDefinition{.areaId = SHEN_NAME(SLT1), .field04 = 0, .init = SlotInitializationNullsub, .main = QueueSlt1};
} // namespace shenmue::s2
