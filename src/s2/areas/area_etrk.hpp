#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name ETRK_AREA{SHEN_NAME(ETRK)}, ETRK_TASK{SHEN_NAME(EVNT)}, ETRK_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name ETRK_HAND{SHEN_NAME(elvy)}, ETRK_CAMERA{SHEN_NAME(elvd)}, ETRK_KEY_BASE{SHEN_NAME(_kdm)};
inline constexpr shen_name ETRK_QF00{SHEN_NAME(QF00)}, ETRK_QUG0{SHEN_NAME(QUG0)}, ETRK_BOARD{SHEN_NAME(SWB0)};
inline constexpr shen_name ETRK_QF_BOARD{SHEN_NAME(_h09)}, ETRK_QUG_BOARD{SHEN_NAME(_h10)};
inline constexpr shen_name ETRK_QF_PROP{SHEN_NAME(F080)}, ETRK_QUG_PROP{SHEN_NAME(F001)}, ETRK_INVALID{0xffffffffU}, ETRK_NONE{0};
inline constexpr shen_name ETRK_ACCEPT{SHEN_NAME(BTEV)}, ETRK_CANCEL{SHEN_NAME(BTCC)};
struct EtrkKey {
    shen_name area;
    std::int32_t flag, item;
    shen_name character;
};
struct EtrkSwitch {
    Vector3 position;
    Vector3i rotation;
    std::int32_t offPart, onPart, flag;
    Vector3 pressedPosition;
    shen_name elevator;
};
struct EtrkKeyAvailability {
    std::int32_t available, ordinal;
};
struct EtrkWork {
    HLib::Task *task;
    std::int32_t entry, state, reserved10, direction, keyCount, key, selection, disabledSelection, moving, remaining;
    const EtrkKey *keys;
    const EtrkSwitch *switches;
    EtrkKeyAvailability available[6];
    shen_name board;
    std::uint32_t reserved74;
};
struct EtrkOpenWork {
    shen_name character;
    std::int32_t remaining;
};
struct EtrkPartWork {
    shen_name character;
    std::int32_t part;
    Vector3 position;
    Vector3i rotation;
};
struct EtrkLightWork {
    std::int32_t operation, enabled;
};
static_assert(sizeof(EtrkKey) == 16 && sizeof(EtrkSwitch) == 52 && offsetof(EtrkSwitch, flag) == 32 && offsetof(EtrkSwitch, elevator) == 48);
static_assert(sizeof(EtrkWork) == 0x78 && offsetof(EtrkWork, keys) == 0x30 && offsetof(EtrkWork, available) == 0x40 && offsetof(EtrkWork, board) == 0x70);
static_assert(sizeof(EtrkOpenWork) == 8 && sizeof(EtrkPartWork) == 32 && sizeof(EtrkLightWork) == 8);
extern const EtrkKey etrkKeys[6];
extern const EtrkSwitch etrkSwitches[4];
extern const Vector3i etrkOpenRotations[30], etrkSelectorHandRotation;
extern Vector3i etrkPlayerHandRotation;
extern std::int32_t etrkRelaxedHand[19][3], etrkOpenHand[19][3], etrkKeyHand[19][3], etrkPressHand[19][3], etrkReturnHand[19][3];
void InitializeEtrk();
void RunEtrk(std::uintptr_t entry);
void EtrkMain(void *data);
void EtrkSetParts(EtrkWork &work, std::int32_t delay);
void EtrkPressSwitch(EtrkWork &work);
inline constexpr ScriptAreaDefinition etrkScriptDefinition{.areaId = ETRK_AREA, .field04 = 0, .init = InitializeEtrk, .main = RunEtrk};
} // namespace shenmue::s2::areas
