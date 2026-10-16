#pragma once

#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace shenmue::s2 {

namespace HLib { struct Task; }
inline constexpr shen_name MOVES_PREVIEW_CHARACTER{SHEN_NAME(MMJL)};
inline constexpr shen_name MEMORY_MOVES_RYO{SHEN_NAME(WZMD)};
inline constexpr shen_name MEMORY_MOVES_GROUND{SHEN_NAME(WZMZ)};
inline constexpr shen_name MOVE_LEARNED_TASK{SHEN_NAME(EWZO)};
inline constexpr shen_name WAZAMAKI_TASK{SHEN_NAME(WZOG)};
inline constexpr shen_name WAZAMAKI_SPRITE_FILE{SHEN_NAME(WAZ1)};
inline std::int32_t wazamakiPreviewInitialPitch = 0, wazamakiPreviewCharacterYaw = 0x4000, wazamakiPreviewInitialRoll = 0;
inline float wazamakiPreviewCameraAngle = 178.0f, wazamakiPreviewDefaultScale = 1.0f;
inline float wazamakiPreviewDistance = 3.25f, wazamakiPreviewHeight = 1.1f;
inline std::int32_t wazamakiPreviewYawStep = 1456, wazamakiPreviewPitchStep = 1456;
inline constexpr std::array<std::string_view, 4> MOVE_LEARNED_CATEGORY_TOKENS{"", "UIMOVEHAND", "UIMOVELEG0", "UIMOVETHRO"}; // @text UIMOVEHAND EN: "Hand move" | JA: "手技" ; UIMOVELEG0 EN: "Leg move" | JA: "足技" ; UIMOVETHRO EN: "Throw move" | JA: "投技"
extern HLib::Task* g_moveLearnedTask;
extern std::array<std::int16_t, 3> g_moveLearnedTextSurfaces;
extern std::uint16_t g_moveLearnedSavedHelpFlags;
extern std::int16_t g_moveLearnedPendingCategory, g_moveLearnedPendingMove;
extern std::int8_t g_moveLearnedSoundDelay;
extern std::uint16_t g_moveUiClockAndNavigationHold;
extern std::array<std::array<char, 256>, 3> g_moveLearnedUiText;
static_assert(sizeof(g_moveLearnedTextSurfaces) == 6);
static_assert(sizeof(g_moveLearnedUiText) == 0x300);
inline constexpr std::string_view MOVE_NAME_TOKENS_HAND[]{
    "UIWAHMTIKN", "UIWAHMELSL", "UIWAHMRATH", "UIWAHMTWKN", "UIWAHMUPKN", "UIWAHMELAS", "UIWAHMKMS0", // @text UIWAHMTIKN EN: "Tiger Knuckle" | JA: "虎菱" ; UIWAHMELSL EN: "Elbow Slam" | JA: "小車打ち" ; UIWAHMRATH EN: "Rain Thrust" | JA: "村雨突き" ; UIWAHMTWKN EN: "Twist Knuckle" | JA: "捻り菱" ; UIWAHMUPKN EN: "Upper Knuckle" | JA: "昇り菱" ; UIWAHMELAS EN: "Elbow Assault" | JA: "裡門頂肘" ; UIWAHMKMS0 EN: "Katana Mist Slash" | JA: "太刀霞"
    "UIWAHMSLST", "UIWAHMBIWH", "UIWAHMTHWS0", "UIWAHMBAWI", "UIWAHMAVLA", "UIWAHMMIFL", "UIWAHMRIFL", // @text UIWAHMSLST EN: "Sleeve Strike" | JA: "袖突き" ; UIWAHMBIWH EN: "Big Wheel" | JA: "大車" ; UIWAHMTHWS0 EN: "Twin Hand Waves" | JA: "双手波" ; UIWAHMBAWI EN: "Backfist Willow" | JA: "裏柳" ; UIWAHMAVLA EN: "Avalanche Lance" | JA: "崩山槍" ; UIWAHMMIFL EN: "Mistral Flash" | JA: "薙風" ; UIWAHMRIFL EN: "Rising Flash" | JA: "裂晃閃"
    "UIWAHMTWBL", "UIWAHMPIBL", "UIWAHMSTAR", "UIWAHMDOBL", "UIWAHMSWFL", "UIWAHMSTST", "UIWAHMBRUP", // @text UIWAHMTWBL EN: "Twin Blades" | JA: "双刃" ; UIWAHMPIBL EN: "Pit Blow" | JA: "水月突き" ; UIWAHMSTAR EN: "Stab Armor" | JA: "鎧通" ; UIWAHMDOBL EN: "Double Blow" | JA: "重ね当て" ; UIWAHMSWFL EN: "Swallow Flip" | JA: "燕旋擺柳" ; UIWAHMSTST EN: "Stepping Strike" | JA: "順歩捶" ; UIWAHMBRUP EN: "Brawling Uppercut" | JA: "喧嘩アッパー"
    "UIWAHMIRPA", "UIWAHMLUST", "UIWAHMTPT0", "UIWAHMCEA0", "UIWAHMMACO", "UIWAHMOBSL", "UIWAHMDIWI"}; // @text UIWAHMIRPA EN: "Iron Palm" | JA: "小檎打" ; UIWAHMLUST EN: "Lunging Strike" | JA: "冲捶" ; UIWAHMTPT0 EN: "Twin Palm Thrust" | JA: "双掌打" ; UIWAHMCEA0 EN: "Counter Elbow Assault" | JA: "外門頂肘" ; UIWAHMMACO EN: "Mantis Combo" | JA: "連環劈" ; UIWAHMOBSL EN: "Oblique Slam" | JA: "斜歩劈斬" ; UIWAHMDIWI EN: "Diagonal Wipe" | JA: "斜弧捶"
inline constexpr std::string_view MOVE_NAME_TOKENS_LEG[]{
    "UIWALMCRKI", "UIWALMTRKI", "UIWALMHAL0", "UIWALMSRK0", "UIWALMDSK0", "UIWALMTHKI", "UIWALMSUSL", // @text UIWALMCRKI EN: "Crescent Kick" | JA: "三日月蹴り" ; UIWALMTRKI EN: "Trample Kick" | JA: "踏み蹴り" ; UIWALMHAL0 EN: "Hold Against Leg" | JA: "出足止め" ; UIWALMSRK0 EN: "Side Reaper Kick" | JA: "払い薙" ; UIWALMDSK0 EN: "Double Storm Kick" | JA: "連環腿" ; UIWALMTHKI EN: "Thunder Kick" | JA: "雷蹴り" ; UIWALMSUSL EN: "Surplice Slash" | JA: "袈裟斬り"
    "UIWALMAGCA", "UIWALMBRTI", "UIWALMDAMO", "UIWALMCYKI", "UIWALMWIND", "UIWALMSWDI", "UIWALMTOKI", // @text UIWALMAGCA EN: "Against Cascade" | JA: "瀧昇り" ; UIWALMBRTI EN: "Brutal Tiger" | JA: "虎乱" ; UIWALMDAMO EN: "Dark Moon" | JA: "新月" ; UIWALMCYKI EN: "Cyclone Kick" | JA: "疾風" ; UIWALMWIND EN: "Windmill" | JA: "風車" ; UIWALMSWDI EN: "Swallow Dive" | JA: "燕旋降脚" ; UIWALMTOKI EN: "Tornado Kick" | JA: "竜巻キック"
    "UIWALMMUSP", "UIWALMCRCY", "UIWALMHOKI", "UIWALMTSL0", "UIWALMSHRE", "UIWALMHORK", "UIWALMHESW", // @text UIWALMMUSP EN: "Mud Spider" | JA: "土蜘蛛" ; UIWALMCRCY EN: "Crawl Cyclone" | JA: "旋風" ; UIWALMHOKI EN: "Hook Kick" | JA: "刈り蹴り" ; UIWALMTSL0 EN: "Twin Swallow Leap" | JA: "飛燕連刈" ; UIWALMSHRE EN: "Shadow Reaper" | JA: "影狩り" ; UIWALMHORK EN: "Horseshoe Kick" | JA: "馬蹄脚" ; UIWALMHESW EN: "Heel Sweep" | JA: "伏身脚"
    "UIWALMOAK0", "UIWALMDRSP"}; // @text UIWALMOAK0 EN: "Oblique Air Kick" | JA: "旋空脚" ; UIWALMDRSP EN: "Dragon Spin" | JA: "降龍脚"
inline constexpr std::string_view MOVE_NAME_TOKENS_THROW[]{
    "UIWATMOVER", "UIWATMSWTH", "UIWATMVOTH", "UIWATMMIRE", "UIWATMDEDR", "UIWATMSHBU", "UIWATMTEDR", // @text UIWATMOVER EN: "Overthrow" | JA: "背負い投げ" ; UIWATMSWTH EN: "Sweep Throw" | JA: "払い腰" ; UIWATMVOTH EN: "Vortex Throw" | JA: "巴投げ" ; UIWATMMIRE EN: "Mist Reaper" | JA: "霞刈り" ; UIWATMDEDR EN: "Demon Drop" | JA: "羅刹落とし" ; UIWATMSHBU EN: "Shoulder Buster" | JA: "巻き固め" ; UIWATMTEDR EN: "Tengu Drop" | JA: "天狗勝"
    "UIWATMDAHA", "UIWATMBTD0", "UIWATMSHST", "UIWATMABF0", "UIWATMTIST", "UIWATMSHBL", "UIWATMCRCH", // @text UIWATMDAHA EN: "Darkside Hazuki" | JA: "裏芭月" ; UIWATMBTD0 EN: "Back Twist Drop" | JA: "裏落とし" ; UIWATMSHST EN: "Shadow Step" | JA: "影身" ; UIWATMABF0 EN: "Arm Break Fire" | JA: "朧無双" ; UIWATMTIST EN: "Tiger Storm" | JA: "虎身崩し" ; UIWATMSHBL EN: "Shadow Blade" | JA: "影刀" ; UIWATMCRCH EN: "Cross Charge" | JA: "残月"
    "UIWATMHIBL", "UIWATMWITH", "UIWATMRFS0", "UIWATMMAFI", "UIWATMSECO", "UIWATMMRD0", "UIWATMDETR", "UIWATMPREX"}; // @text UIWATMHIBL EN: "Hind Blow" | JA: "斜行" ; UIWATMWITH EN: "Wild Throw" | JA: "シュツ法" ; UIWATMRFS0 EN: "Rear Foot Sweep" | JA: "大展伯" ; UIWATMMAFI EN: "Machinegun Fist" | JA: "連手砲" ; UIWATMSECO EN: "Serpent Coil" | JA: "蛇身貼" ; UIWATMMRD0 EN: "Monkey Roll Drop" | JA: "白猿落" ; UIWATMDETR EN: "Demon's Triangle" | JA: "閻王三点手" ; UIWATMPREX EN: "Predictive Explosion" | JA: "聴勁"
inline constexpr std::span<const std::string_view> MOVE_NAME_TOKENS[]{
    {}, MOVE_NAME_TOKENS_HAND, MOVE_NAME_TOKENS_LEG, MOVE_NAME_TOKENS_THROW};

inline constexpr std::array<std::array<std::uint8_t, 7>, 28> MOVE_INPUT_COMMANDS_HAND{{
    {1, 0}, {2, 2, 0}, {2, 2, 0}, {2, 3, 0}, {2, 3, 0}, {3, 2, 2, 0}, {3, 2, 3, 0}, {3, 3, 2, 0},
    {3, 0, 7, 1}, {4, 2, 0, 7, 1}, {4, 3, 0, 7, 1}, {5, 2, 2, 0, 7, 1}, {2, 4, 0}, {3, 3, 3, 0},
    {3, 2, 3, 0}, {3, 3, 2, 0}, {5, 2, 2, 0, 7, 1}, {5, 3, 3, 0, 7, 1}, {3, 3, 0, 1}, {2, 2, 0},
    {3, 3, 3, 0}, {3, 0, 7, 1}, {4, 2, 0, 7, 1}, {4, 2, 0, 7, 1}, {4, 3, 0, 7, 1}, {5, 3, 3, 0, 7, 1},
    {4, 2, 0, 7, 6}, {4, 3, 0, 7, 6},
}};
inline constexpr std::array<std::array<std::uint8_t, 7>, 23> MOVE_INPUT_COMMANDS_LEG{{
    {1, 1}, {1, 1}, {2, 2, 1}, {2, 3, 1}, {4, 2, 2, 1, 1}, {3, 2, 3, 1}, {3, 2, 3, 1}, {3, 3, 2, 1},
    {5, 2, 3, 0, 7, 1}, {5, 3, 2, 0, 7, 1}, {2, 4, 1}, {4, 4, 0, 7, 1}, {2, 3, 1}, {4, 2, 2, 1, 1},
    {3, 3, 3, 1}, {3, 3, 3, 1}, {3, 3, 2, 1}, {5, 3, 2, 0, 7, 1}, {4, 4, 6, 7, 1}, {5, 2, 3, 0, 7, 1},
    {3, 3, 3, 1}, {2, 4, 1}, {4, 4, 0, 7, 1},
}};
inline constexpr std::array<std::array<std::uint8_t, 7>, 22> MOVE_INPUT_COMMANDS_THROW{{
    {1, 5}, {2, 2, 5}, {2, 3, 5}, {3, 2, 2, 5}, {3, 3, 3, 5}, {3, 2, 3, 5}, {3, 3, 2, 5}, {1, 5}, {1, 5},
    {4, 2, 6, 7, 5}, {4, 2, 3, 3, 5}, {5, 3, 2, 2, 5, 5}, {5, 2, 6, 7, 5}, {5, 2, 2, 6, 7, 5}, {3, 6, 7, 5},
    {1, 5}, {2, 2, 5}, {3, 2, 2, 5}, {3, 2, 3, 5}, {4, 2, 3, 3, 5}, {6, 3, 2, 2, 6, 7, 5}, {1, 7},
}};
inline constexpr std::array<std::uint32_t, 3> MOVE_CRITICAL_MASKS{(1U << 20) | (1U << 24), 0,
    (1U << 10) | (1U << 11) | (1U << 17) | (1U << 19) | (1U << 20)};
inline constexpr std::array<std::int32_t, 8> MOVE_INPUT_ICON_ACTIONS{24, 22, 55, 54, 56, 23, 25, 70};
inline constexpr std::array<std::uint16_t, 8> WAZAMAKI_INPUT_ICONS{778, 776, 2048, 2048, 780, 777, 779, 770};
inline constexpr std::array<std::uint32_t, 8> WAZAMAKI_INPUT_COLOURS{
    0xff000050U, 0xff000000U, 0xff9f9f9fU, 0xff9f9f9fU, 0xff200020U, 0xff000000U, 0xff000000U, 0xff333333U};
inline constexpr std::span<const std::array<std::uint8_t, 7>> WAZAMAKI_GROUP_COMMANDS[]{
    {}, MOVE_INPUT_COMMANDS_HAND, MOVE_INPUT_COMMANDS_LEG, MOVE_INPUT_COMMANDS_THROW};

struct WazamakiMoveInfo {
    std::uint8_t category{};
    std::uint8_t id{};
    std::uint8_t group{};
    std::array<std::uint8_t, 3> stats{};
    std::int16_t proficiency{};
    std::int16_t usage{};
    std::uint32_t motionId{};
    std::string_view textToken;
    bool equipped{};
    std::array<std::uint8_t, 7> inputCommands{};
    bool critical{};
};


struct NativeWazamakiGroup {
    std::uint8_t count, selected;
    std::array<std::uint8_t, 3> moves;
};
static_assert(sizeof(NativeWazamakiGroup) == 5);
static_assert(offsetof(NativeWazamakiGroup, selected) == 1);
static_assert(offsetof(NativeWazamakiGroup, moves) == 2);

struct WazamakiDescriptionWindow {
    float x, y;
    std::int32_t width, height;
    float scale, alpha;
    float textX, textY;
    std::int32_t columns, rows;
    std::array<std::array<std::int32_t, 2>, 8> textures;
};
static_assert(sizeof(WazamakiDescriptionWindow) == 0x68);
static_assert(offsetof(WazamakiDescriptionWindow, scale) == 0x10);
static_assert(offsetof(WazamakiDescriptionWindow, textX) == 0x18);
static_assert(offsetof(WazamakiDescriptionWindow, columns) == 0x20);
static_assert(offsetof(WazamakiDescriptionWindow, textures) == 0x28);

struct WazamakiLearnedGroup {
    std::uint8_t category{};
    std::uint8_t group{};
    std::uint8_t count{};
    std::uint8_t selected{};
    std::array<std::uint8_t, 3> moves{};
};
struct WazamakiSelection {
    std::uint8_t category{1};
    std::uint8_t row{};
    std::uint8_t variant{};
};



std::span<const WazamakiMoveInfo>
GetWazamakiMoveCategory(std::uint8_t category) noexcept;
WazamakiSelection GetWazamakiSelection() noexcept;
bool SetWazamakiSelection(std::uint8_t category,
                          std::uint8_t row) noexcept;
// SelectWazamakiMove operation zero: equip this move and clear every learned
// sibling that shares its category/group key.
bool SelectWazamakiMove(std::uint8_t category, std::uint8_t id) noexcept;
std::int64_t SetMoveEquipment(std::int8_t operation, std::int32_t category, std::uint8_t move) noexcept;
void RefreshWazamakiLearnedGroups();
std::span<const WazamakiLearnedGroup> GetWazamakiLearnedGroups() noexcept;
void PlayWazamakiGroupMove(std::uint32_t row);
void LoadRyoSceneForMovesScreen();
void UpdateWazamakiPreviewFrame();
void DrawWazamakiPreview();
void ReleaseWazamakiPreviewModels();



void QueueWazamakiTask();
bool UpdateWazamakiMenu();
void CancelWazamakiMenu(std::int32_t force);



std::uint32_t IsWazamakiInactive() noexcept;
void SetupWZ2_CtrlWazamaki() noexcept;

void ResetMoveLearningPresentationState() noexcept;
std::int64_t ResetWazamakiState() noexcept;

std::int32_t SetNewEwzoTypeForMoveLearned(
    std::int8_t mode, std::int16_t category, std::int16_t moveIndex) noexcept;
void ReleaseMoveLearnedOwner() noexcept;
bool IsMoveLearnedActive() noexcept;

} // namespace shenmue::s2
