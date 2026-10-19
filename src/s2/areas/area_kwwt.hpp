#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name KWWT_TASK{SHEN_NAME(EVNT)}, KWWT_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name KWWT_RYO{SHEN_NAME(RYO_)}, KWWT_SHENHUA{SHEN_NAME(SIN_)}, KWWT_PROP{SHEN_NAME(a636)};
inline constexpr shen_name KWWT_FIRE_SOURCE{SHEN_NAME(a287)}, KWWT_FIRE_TARGET{SHEN_NAME(a288)}, KWWT_RETURN_AREA{SHEN_NAME(KES1)};
inline constexpr shen_name KWWT_CHARACTERS[]{KWWT_SHENHUA, shen_name{0xffffffffU}};
inline constexpr shen_name KWWT_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01), SHEN_NAME(AT02), SHEN_NAME(AT03)};
inline constexpr std::uint16_t KWWT_FIRE_PALETTE[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
struct KWWTTaskParameter {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int32_t selection;
    std::int16_t state;
    std::byte reserved12[6];
};
struct KWWTLoopParameter {
    KWWTTaskParameter *parent;
    std::int16_t currentFrame;
    std::byte reserved0a[6];
};
static_assert(sizeof(KWWTTaskParameter) == 24);
static_assert(offsetof(KWWTTaskParameter, selection) == 12);
static_assert(offsetof(KWWTTaskParameter, state) == 16);
static_assert(sizeof(KWWTLoopParameter) == 16);
static_assert(offsetof(KWWTLoopParameter, currentFrame) == 8);

void AreaKWWTInit();
void AreaKWWTMain(std::uintptr_t argument);
void KWWTAuthorMainCallback(void *parameter);
void QueueKWWTAuthorLoop(KWWTTaskParameter *parameter);
void KWWTAuthorLoopCallback(void *parameter);
void KWWTAdvanceAuthorCallback(void *parameter);
void InitializeKWWTFires();
void KWWTPrepareAuthorCharacters();
void KWWTRestoreAuthorCharacters();
void KWWTFinishEvent(KWWTTaskParameter *parameter);
extern const ScriptAreaDefinition areaKWWTScriptDefinition;

} // namespace shenmue::s2::areas
