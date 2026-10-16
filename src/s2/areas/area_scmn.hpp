#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SCMN_TASK{SHEN_NAME(EVNT)}, SCMN_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name SCMN_CHUNYAN{SHEN_NAME(CHN_)}, SCMN_BASE{SHEN_NAME(BASE)}, SCMN_INVALID{0xffffffffU};
inline constexpr shen_name SCMN_QTB1{SHEN_NAME(QTB1)}, SCMN_QSFA{SHEN_NAME(QSFA)}, SCMN_QSFB{SHEN_NAME(QSFB)};
inline constexpr shen_name SCMN_QSFC{SHEN_NAME(QSFC)}, SCMN_QSFD{SHEN_NAME(QSFD)}, SCMN_QGBT{SHEN_NAME(QGBT)};
inline constexpr shen_name SCMN_QJBT{SHEN_NAME(QJBT)}, SCMN_QLBT{SHEN_NAME(QLBT)}, SCMN_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name SCMN_CHARACTERS[]{SCMN_PLAYER, SCMN_CHUNYAN, SCMN_INVALID};
inline constexpr Vector3 SCMN_QTB1_POSITION{199.7F, 100.0F, 222.31F};
inline constexpr Vector3 SCMN_QSFA_POSITION{180.4F, 100.0F, 220.98F};
inline constexpr Vector3 SCMN_QSFB_POSITION{310.12F, 100.0F, 108.88F};
inline constexpr Vector3 SCMN_QSFC_POSITION{264.72F, 94.83F, 52.57F};
inline constexpr Vector3 SCMN_QSFD_POSITION{162.62F, 103.6F, 120.37F};
inline constexpr Vector3 SCMN_QGBT_POSITION{354.8F, 77.27F, 120.85F};
inline constexpr Vector3 SCMN_QJBT_POSITION{280.1F, 96.4F, 221.65F};
inline constexpr Vector3 SCMN_QLBT_POSITION{243.93F, 139.6F, 85.1F};
inline constexpr Vector3 SCMN_CHUNYAN_LOCAL_POSITION{-1.5F, 0.0F, -1.3F};
struct ScmnEventWork {
    HLib::Task *self;
    std::byte reserved08[48];
    std::uint32_t state;
    std::byte reserved3c[8];
    std::uint32_t motions[16];
    std::int32_t previousNpcBatchCount;
    EventReadCharactersWork *characters;
    std::byte reserved90[232];
    std::uint32_t changeScene;
    std::uint32_t reserved17c;
};
static_assert(sizeof(ScmnEventWork) == 0x180);
static_assert(offsetof(ScmnEventWork, state) == 0x38);
static_assert(offsetof(ScmnEventWork, motions) == 0x44);
static_assert(offsetof(ScmnEventWork, previousNpcBatchCount) == 0x84);
static_assert(offsetof(ScmnEventWork, characters) == 0x88);
static_assert(offsetof(ScmnEventWork, changeScene) == 0x178);
extern Vector3 scmnSavedPlayerPosition;
extern Vector3i scmnSavedPlayerRotation;
extern std::uint32_t scmnSavedFloor;
extern std::int32_t scmnEntryMode;
extern std::int32_t scmnEyeFrame;
extern std::int32_t scmnSequenceFinished;
extern std::uint32_t scmnChoicePhase;
extern std::int32_t scmnDefaultYaw, scmnQjbtYaw, scmnQsfaYaw, scmnPlayerLocalYaw;
extern Vector3 scmnPlayerLocalPosition;
void ScmnInit();
void ScmnMain(std::uintptr_t argument);
void ScmnWaitVoiceCallback(void *);
void ScmnFadeOutCallback(void *);
void QueueScmnSceneCallback(void *);
void ScmnMainCallback(void *);
void LoadScmnScene();
void FinishScmnSequence();
void PrepareScmnCharacters();
std::int32_t PlayScmnInvitationVoice(void *);
std::int32_t PlayScmnInvitationScene(void *);
std::uint32_t PollScmnChoice(std::int32_t mode);
void ScmnEyeControlCallback(void *);
void ScmnCleanupCallback(void *);
extern const ScriptAreaDefinition scmnScriptDefinition;
} // namespace shenmue::s2
