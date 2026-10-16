#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
namespace shenmue::s2 {
inline constexpr shen_name TOFB_AREA{SHEN_NAME(TOFB)}, TOFB_NEXT_AREA{SHEN_NAME(YTQB)}, TOFB_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name TOFB_PLAYER{SHEN_NAME(RYO_)}, TOFB_CHO{SHEN_NAME(CHO_)}, TOFB_SBO{SHEN_NAME(SBO_)};
inline constexpr shen_name TOFB_MRC{SHEN_NAME(MRC_)}, TOFB_SCX{SHEN_NAME(SCX_)}, TOFB_CHR{SHEN_NAME(CHR_)};
inline constexpr shen_name TOFB_RKN{SHEN_NAME(RKN_)}, TOFB_SBK{SHEN_NAME(SBK_)}, TOFB_SKH{SHEN_NAME(SKH_)};
inline constexpr shen_name TOFB_A412{SHEN_NAME(a412)}, TOFB_FR00{SHEN_NAME(FR00)}, TOFB_A021{SHEN_NAME(a021)};
inline constexpr shen_name TOFB_A418{SHEN_NAME(a418)}, TOFB_TABLE{SHEN_NAME(TBL0)}, TOFB_BROKEN_TABLE{SHEN_NAME(TBLB)};
inline constexpr shen_name TOFB_GRID{SHEN_NAME(BTC1)}, TOFB_DEFAULT_GRID{SHEN_NAME(0000)}, TOFB_LIGHT{SHEN_NAME(l001)};
inline constexpr shen_name TOFB_AUTHOR{SHEN_NAME(ATH0)}, TOFB_RETRY_TITLE{SHEN_NAME(RTR2)}, TOFB_TABLE_MOTION{SHEN_NAME(TUKU)};
inline constexpr shen_name TOFB_INVALID{0xffffffffU};
inline constexpr shen_name TOFB_LOAD_CHARACTERS[]{TOFB_PLAYER, TOFB_CHO, TOFB_SBO, TOFB_MRC, TOFB_SCX, TOFB_CHR, TOFB_RKN, TOFB_SBK, TOFB_SKH, TOFB_INVALID};
inline constexpr shen_name TOFB_BATTLE_CHARACTERS[]{TOFB_PLAYER, TOFB_SBO, TOFB_MRC, TOFB_SCX, TOFB_CHR};
inline constexpr shen_name TOFB_HIDDEN_CHARACTERS[]{TOFB_CHO, TOFB_RKN, TOFB_SBK, TOFB_SKH};
inline constexpr Vector3 TOFB_BATTLE_POSITIONS[]{{217.0f, 104.5999984741211f, 212.5f}, {219.60000610351562f, 104.5999984741211f, 212.5f},
    {216.60000610351562f, 104.5999984741211f, 210.5f}, {219.6999969482422f, 104.5999984741211f, 212.3000030517578f},
    {215.6999969482422f, 104.5999984741211f, 214.3000030517578f}};
inline constexpr Vector3 TOFB_CAMERA_TARGET{215.9774932861328f, 105.64350128173828f, 212.9739990234375f};
inline constexpr Vector3 TOFB_CAMERA_POSITION{213.4884033203125f, 105.7667007446289f, 213.3881072998047f};
inline constexpr std::int32_t TOFB_DYNAMICS_PARTS[]{152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
    173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
    204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 228, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
    235};
struct TofbEventWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    std::int32_t motionHandle;
    std::uint32_t reserved14;
};
static_assert(sizeof(TofbEventWork) == 0x18);
static_assert(offsetof(TofbEventWork, task) == 8 && offsetof(TofbEventWork, motionHandle) == 0x10);
void TofbInit();
void TofbMain(std::uintptr_t);
void TofbStartCallback(void *);
void InitializeTofbResources(TofbEventWork *);
void TofbOpeningAuthorCallback(void *);
void TofbBattleStartCallback(void *);
void TofbBattleCallback(void *);
void TofbVictoryAuthorCallback(void *);
void TofbRetryCallback(void *);
void MaintainTofbBattleCamera(std::int32_t);
void TofbEndingAuthorCallback(void *);
void TofbEndCallback(void *);
void CleanupTofbResources(TofbEventWork *);
void TofbDormantBoundary();
extern const ScriptAreaDefinition tofbScriptDefinition;
} // namespace shenmue::s2
