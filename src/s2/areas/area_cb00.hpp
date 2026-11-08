#pragma once
#include "s2/step_event.hpp"
#include "s2/date_time.hpp"
namespace shenmue::s2 {
inline constexpr shen_name CB00_AREA{SHEN_NAME(CB00)}, CB00_TASK{SHEN_NAME(EVNT)}, CB00_EXIT_AREA{SHEN_NAME(AB00)};
inline constexpr shen_name CB00_RYO{SHEN_NAME(RYO_)}, CB00_REN{SHEN_NAME(REN_)}, CB00_DUMMY{SHEN_NAME(DUM0)};
inline constexpr shen_name CB00_CENTRE{SHEN_NAME(CENT)}, CB00_PIPE{SHEN_NAME(PIPE)}, CB00_BOARD{SHEN_NAME(BORD)};
inline constexpr shen_name CB00_PLANK_LEFT{SHEN_NAME(ITK0)}, CB00_PLANK_RIGHT{SHEN_NAME(ITK1)}, CB00_TRUCK{SHEN_NAME(TKGM)};
inline constexpr shen_name CB00_BRIDGE{SHEN_NAME(1BAS)}, CB00_BRIDGE_BASE{SHEN_NAME(TMAR)}, CB00_LIGHT{SHEN_NAME(INBL)};
inline constexpr shen_name CB00_TRUCKS[]{SHEN_NAME(TRC1), SHEN_NAME(TRC4), SHEN_NAME(TRC2)};
inline constexpr shen_name CB00_READ_CHARACTERS[]{CB00_RYO, CB00_REN, shen_name{0xffffffffU}};
inline constexpr shen_name CB00_AUTHORS[]{SHEN_NAME(0000), SHEN_NAME(0001), SHEN_NAME(0002), SHEN_NAME(0003), SHEN_NAME(0004), SHEN_NAME(0005), SHEN_NAME(0006),
    SHEN_NAME(0007), SHEN_NAME(0008), SHEN_NAME(0009), SHEN_NAME(0010), SHEN_NAME(0011), SHEN_NAME(0012), SHEN_NAME(0013), SHEN_NAME(0014), SHEN_NAME(0015),
    SHEN_NAME(0016), SHEN_NAME(0017), SHEN_NAME(0018), SHEN_NAME(0019), SHEN_NAME(0020)};
inline constexpr std::int32_t CB00_PIPE_PARTS[]{152, 153, 154, 155, 156, 157, 158};
inline constexpr std::int32_t CB00_BOARD_PARTS[]{152, 153, 154, 155, 156, 157, 158, 159, 160, 161};
inline constexpr std::int32_t CB00_TAKE_PARTS[]{
    181, 180, 179, 178, 160, 165, 164, 163, 167, 168, 158, 170, 161, 173, 176, 175, 177, 174, 171, 159, 162, 169, 172, 166, 156, 155, 154, 152, 153, 157};
inline constexpr std::int32_t CB00_BRIDGE_PARTS[]{162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161};
inline constexpr Vector3 CB00_PIPE_POSITION{-2.5999999046325684f, 0, 17.700000762939453f};
inline constexpr Vector3 CB00_BOARD_POSITION{-4.488999843597412f, 2.7100000381469727f, 13.958000183105469f};
inline constexpr Vector3 CB00_PLANK_POSITION{0, 0, 0.30000001192092896f};
inline constexpr Vector3 CB00_TRUCK_INITIAL_POSITION{-7.068600177764893f, 7.168590068817139f, 4.763020038604736f};
inline constexpr Vector3 CB00_TRUCK_EXIT_POSITION{-6.840000152587891f, 7.168590068817139f, 4.763020038604736f};
inline constexpr Vector3 CB00_CROSSING_POSITION{-3.200000047683716f, 8.119999885559082f, 18.75f};
struct Cb00EventWork {
    HLib::Task *task;
    shen_name exitArea;
    std::int32_t exitEntry, stepFlag;
    std::byte reserved14[8];
    std::int32_t phase, reserved20, frames, boardFailure, authorVariant;
    std::int16_t qteStatus;
    std::byte reserved32[6];
    EventReadCharactersWork *characters;
    std::int32_t smoke;
    DateTime date;
    std::byte reserved4b[5];
    std::int32_t failures, battleFailure;
};
struct Cb00QteWork {
    std::int16_t status, maximumFrames;
    std::uint32_t reserved04;
    HLib::Task *parent;
};
struct Cb00TruckQuakeWork {
    Vector3 baseY;
    float amplitude, decay;
    std::int32_t angle, angleStep, frames;
};
struct Cb00BridgeProfile {
    float span;
    shen_name character, base;
    std::int32_t attachedCharacter, partCount, reserved14;
    const std::int32_t *parts;
};
struct Cb00BridgeWork {
    std::int32_t reserved00;
    Vector3 position;
    Vector3i rotation;
    std::int32_t angle, angleStep, maximumAngle;
    shen_name character;
    float length, gain, speed;
    std::int32_t pairs, reserved3c;
    const Cb00BridgeProfile *profile;
};
struct Cb00BridgeCharacterWork {
    shen_name character;
    std::int32_t cleanup;
};
struct Cb00BridgeRequestWork {
    std::uint32_t flags, maximumAngle;
    float gain;
    std::uint32_t reserved0c;
    std::uint64_t gainBits;
};
struct Cb00SmokeWork {
    Vector3 position, velocity, volume;
    std::int32_t killImmediately;
    shen_name character;
    std::uint32_t reserved2c;
    std::int32_t *effect;
    std::int32_t frames, reserved3c;
};
struct Cb00BlurWork {
    shen_name character;
    std::int32_t scale, frames, maximumScale;
};
struct Cb00DormantRetireWork {
    shen_name source, target;
    std::uint32_t reserved08;
    std::int32_t frames;
};
static_assert(sizeof(Cb00EventWork) == 0x58 && offsetof(Cb00EventWork, phase) == 0x1c && offsetof(Cb00EventWork, frames) == 0x24);
static_assert(offsetof(Cb00EventWork, qteStatus) == 0x30 && offsetof(Cb00EventWork, characters) == 0x38 && offsetof(Cb00EventWork, date) == 0x44);
static_assert(offsetof(Cb00EventWork, failures) == 0x50 && sizeof(Cb00QteWork) == 0x10 && sizeof(Cb00TruckQuakeWork) == 0x20);
static_assert(sizeof(Cb00BridgeProfile) == 0x20 && sizeof(Cb00BridgeWork) == 0x48 && offsetof(Cb00BridgeWork, profile) == 0x40);
static_assert(sizeof(Cb00SmokeWork) == 0x40 && offsetof(Cb00SmokeWork, effect) == 0x30 && sizeof(Cb00BlurWork) == 0x10);
static_assert(sizeof(Cb00BridgeRequestWork) == 0x18 && sizeof(Cb00DormantRetireWork) == 0x10);
inline constexpr Cb00BridgeProfile CB00_BRIDGE_PROFILE{0.34285715222358704f, CB00_BRIDGE, CB00_BRIDGE_BASE, 0, 20, 0, CB00_BRIDGE_PARTS};
extern std::int32_t cb00QteFrames;
extern HLib::Task *cb00BridgeTask;
extern HLib::Task *cb00BridgeRequestTask;
void Cb00Init();
void Cb00Main(std::uintptr_t);
void Cb00Initialize(HLib::Task *);
void Cb00Callback(void *);
void Cb00CameraCallback(void *);
void Cb00WaitFrameCallback(void *);
void Cb00WaitSkipCallback(void *);
void Cb00WaitFadeCallback(void *);
void Cb00Exit(Cb00EventWork *);
void Cb00QteStartCallback(void *);
void Cb00QteWaitCallback(void *);
void QueueCb00Qte(HLib::Task *, std::int16_t, std::int16_t, std::int32_t);
void Cb00SetupCharacters();
void Cb00ResetProps();
void Cb00SetupSceneLighting();
void Cb00RestoreSceneLighting();
void Cb00SetupRetryLighting();
void Cb00SetupAuthorLighting();
void Cb00BridgeEnqueue(std::int32_t delay);
void Cb00BridgeCallback(void *);
void Cb00BridgeCharacterCallback(void *);
void Cb00BridgeEndCallback(void *);
void Cb00BridgeRequestCallback(void *);
void QueueCb00BridgeRequest(std::uint32_t, float, std::int32_t);
void Cb00ResetBridge(Cb00BridgeWork *);
void Cb00UpdateBridge(Cb00BridgeWork *);
void Cb00UpdateBridgeCharacter(Cb00BridgeWork *);
void Cb00PlaceBridgeCharacter(Cb00BridgeWork *, float);
std::int32_t Cb00StartBridge();
std::int32_t Cb00BeginCrossing();
std::int32_t Cb00CrossingFirstSuccess();
std::int32_t Cb00CrossingSecondSuccess();
std::int32_t Cb00FinishCrossing();
std::int32_t Cb00CrossingFailure(Cb00EventWork *, std::int32_t);
void Cb00SmokeCreateCallback(void *);
void Cb00SmokeStopCallback(void *);
void Cb00SmokeKillCallback(void *);
void QueueCb00Smoke(Cb00EventWork *, shen_name, const Vector3 *, const Vector3 &, std::int32_t, std::int32_t, std::int32_t);
HLib::Task *QueueCb00Blur(std::int32_t, std::int32_t);
void Cb00BlurCreateCallback(void *);
void Cb00BlurDestroyCallback(void *);
void Cb00TruckQuakeBaseCallback(void *);
void Cb00TruckQuakeCallback(void *);
void QueueCb00TruckQuake(float);
void Cb00DormantRetireCallback(void *);
void Cb00InitializationNullsub();
extern const ScriptAreaDefinition cb00Definition;
} // namespace shenmue::s2
