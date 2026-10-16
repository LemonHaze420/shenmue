#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/step_event.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace shenmue::s2::areas {
inline constexpr shen_name YCPA_AREA{SHEN_NAME(YCPA)}, YCPA_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name YCPA_RYO{SHEN_NAME(RYO_)}, YCPA_REN{SHEN_NAME(REN_)}, YCPA_YUA{SHEN_NAME(YUA_)};
inline constexpr shen_name YCPA_BASE{SHEN_NAME(BASE)}, YCPA_RYOB{SHEN_NAME(RYOB)}, YCPA_RENB{SHEN_NAME(RENB)};
inline constexpr shen_name YCPA_YUAB{SHEN_NAME(YUAB)}, YCPA_RYOD{SHEN_NAME(RYOD)}, YCPA_INVALID{0xffffffffU};
inline constexpr shen_name YCPA_AM1{SHEN_NAME(AM1_)};
inline constexpr shen_name YCPA_BA6{SHEN_NAME(BA6_)};
inline constexpr shen_name YCPA_CUP{SHEN_NAME(CUP_)};
inline constexpr shen_name YCPA_OB4{SHEN_NAME(OB4_)};
inline constexpr shen_name YCPA_R594{SHEN_NAME(R594)};
inline constexpr shen_name YCPA_RYW{SHEN_NAME(RYW_)};
inline constexpr shen_name YCPA_TB3{SHEN_NAME(TB3_)};
inline constexpr shen_name YCPA_TRSV{SHEN_NAME(TRSV)};
inline constexpr std::uint32_t YCPA_MOTION_NO_BLEND = 0x40000000U, YCPA_MOTION_MIRRORED = 0x20000000U;
inline constexpr std::uint32_t YCPA_POINT_SINGLE_UPDATE = 0x20000000U, YCPA_POINT_STOP = 0x10000000U;
inline constexpr std::uint32_t YCPA_ATTACH_OBJECT = 0, YCPA_DETACH_OBJECT = 1;
inline constexpr std::int32_t YCPA_VOICE_FORCE_PLAYBACK = 1;

struct YcpaWork {
    HLib::Task *task;
    std::uint16_t state, reserved0a;
    std::int32_t qteElapsed;
    std::uint32_t reserved10;
    std::uint16_t finished, reserved16;
    std::int32_t qteSucceeded;
    std::uint32_t reserved1c;
    EventReadCharactersWork *characters;
    std::int32_t countdown;
    std::uint32_t reserved2c;
};
static_assert(offsetof(YcpaWork, qteElapsed) == 0x0c && offsetof(YcpaWork, qteSucceeded) == 0x18);
static_assert(sizeof(YcpaWork) == 0x30 && offsetof(YcpaWork, finished) == 0x14);
extern std::int32_t ycpaEvent, ycpaSavedEvent, ycpaPhase, ycpaPursuitSucceeded;
extern const shen_name *ycpaCharacters;
extern const ScriptAreaDefinition ycpaScriptDefinition;
void *YcpaQueueRecord(HLib::TaskCallback callback, std::uint32_t size);
void YcpaCamera(std::int32_t camera, shen_name eye, shen_name target, std::uint8_t context, float angle, std::int32_t delay);
void YcpaFilm(std::int32_t frames, std::int32_t startAlpha, std::int32_t startRed, std::int32_t startGreen, std::int32_t startBlue, std::int32_t targetAlpha,
    std::int32_t targetRed, std::int32_t targetGreen, std::int32_t targetBlue, std::int32_t delay);
std::int32_t YcpaElevatorDeparture();
std::int32_t YcpaStreetCorner(YcpaWork &work);
std::int32_t YcpaMarketConversation();
std::int32_t YcpaLaneConversation();
std::int32_t YcpaDoorInterruption();
std::int32_t YcpaElevatorArrival();
std::int32_t YcpaDiscovery();
std::int32_t YcpaDiscoveryAgain();
std::int32_t YcpaBriefConversation();
std::int32_t YcpaStreetQteSuccess();
void YcpaPrepareAdvice();
void YcpaAdviceCallback(void *data);
void YcpaEndAdviceCallback(void *data);
void YcpaChangeAreaAfterAdvice(void *data);
void YcpaStopChaseCallback(void *data);
void YcpaEndChaseCallback(void *data);
void YcpaAuthPursuitCallback(void *data);
void YcpaQueuePursuit(YcpaWork &work);
void YcpaQueueReturn();
void YcpaSetCharacterMidpoint(shen_name first, shen_name second);
bool YcpaSelectDiscoveryPlacement(bool followUp);
void YcpaPlaceDiscoveryActors(bool reverse);
bool YcpaDiscoverySightLinesClear();
} // namespace shenmue::s2::areas
