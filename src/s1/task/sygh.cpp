#include "s1/task/sygh.hpp"

#include "s1/boot.hpp"
#include "s1/input.hpp"
#include "s1/font.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/vfma.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/sond.hpp"
#include "s1/subtitles.hpp"
#include "s1/task/sygu.hpp"
#include "s1/task/wzog.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {
void HLib_InertTaskCallback(HLib::TaskData *);
SYGHDrawState g_syghDrawState = kSyghInitialDrawState;
std::uint8_t g_moveHelpPopupState{};
std::array<std::int16_t, 7> g_moveHelpSurfaceIds{-1, -1, -1, -1, -1, -1, -1};
namespace {

constexpr std::int32_t kSyghToken = static_cast<std::int32_t>(SHEN_NAME(SYGH));
constexpr std::uint16_t kAcceptButton = 0x0004;
constexpr std::uint16_t kHelpButton = 0x0200;
constexpr std::uint32_t kToggleSoundCommand = 264363;
constexpr float kClosedX = 2.0f;
constexpr float kClosedY = 2.0f;
constexpr float kOpenX = 520.0f;
constexpr float kOpenY = 296.0f;
constexpr float kSlideX = 152.0f;
constexpr float kSlideY = 25.0f;

HLib::Task *g_syghTask = nullptr;
PadCaptureEntry *g_syghPad = nullptr;
enum class MoveSkillOp : std::uint8_t {
    Set,
    Remove,
    Get,
};

constexpr std::array<std::uint8_t, 3> kMoveSkillBankCounts{19, 18, 14};
constexpr std::array<std::uint8_t, 3> kMoveSkillBankBases{0, 0x20, 0x40};
constexpr std::array<std::array<std::int16_t, 20>, 3> kMoveSkillGroups{{
        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 8, 13, 14, 15, 16, -1}},
        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 11, 6, 12, 13, 14, 15, -1, -1}},
        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, -1, -1, -1, -1, -1, -1}},
}};
constexpr std::array<std::array<std::uint32_t, 20>, 3> kMoveSkillWords{{
        {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00800000, 0x04000000,
        0x40400000, 0x00000040, 0x00000002, 0x00000020, 0}},
        {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40200000, 0x01000000,
          0x00000004, 0x00000010, 0x00000008, 0x00001000, 0x00000200, 0, 0}},
        {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00000100, 0x00002000, 0x00000080,
          0x00004000, 0x00008000, 0, 0, 0, 0, 0, 0}},
}};

std::uint16_t UpdateMoveSkill(MoveSkillOp operation, std::uint8_t animationSet, std::int16_t moveId, std::int16_t moveValue) {
    if (animationSet < 1 || animationSet > 3)
        return 1;
    const std::size_t bankIndex = animationSet - 1;
    const std::int32_t maxMoveId = kMoveSkillBankCounts[bankIndex];
    if (moveId < 0 || moveId >= maxMoveId)
        return 1;

    MoveSetRuntimeState &moveSet = g_playerStats.moveSets[bankIndex];
    const std::int32_t count = std::min<std::int32_t>(moveSet.count, 20);
    const std::int16_t requestedGroup = kMoveSkillGroups[bankIndex][moveId];
    std::int32_t groupIndex = -1;
    std::int32_t exactIndex = -1;
    std::uint16_t currentValue = 0;
    bool removeExactAfterReplacement = false;
    for (std::int32_t index = 0; index < count; ++index) {
        const std::int16_t entryMoveId = moveSet.moveValue[index];
        if (entryMoveId == moveId) {
            currentValue = moveSet.learnValue[index];
            exactIndex = index;
        }
        if (moveValue < 12 || entryMoveId < 0 || entryMoveId >= maxMoveId) {
            continue;
        }
        const std::int16_t entryGroup = kMoveSkillGroups[bankIndex][entryMoveId];
        if (requestedGroup == entryGroup && groupIndex == -1) {
            groupIndex = index;
            removeExactAfterReplacement = exactIndex != index;
        }
        if (animationSet == 2 && entryGroup == 3 && requestedGroup == 11) {
            groupIndex = index;
            removeExactAfterReplacement = exactIndex != index;
        }
    }

    if (operation == MoveSkillOp::Set) {
        if (groupIndex == -1) {
            if (exactIndex == -1) {
                groupIndex = std::min(count, 19);
                moveSet.count = static_cast<std::uint8_t>(std::min(count + 1, 20));
            } else {
                groupIndex = exactIndex;
            }
        }
        g_pendingMovePopupMoveId = static_cast<std::uint16_t>(moveId);
        g_pendingMovePopupAnimSetId = animationSet;
        moveSet.moveValue[groupIndex] = static_cast<std::uint16_t>(moveId);
        moveSet.learnValue[groupIndex] = static_cast<std::uint16_t>(moveValue);

        if (moveId < 20) {
            const std::uint32_t skillWord = kMoveSkillWords[bankIndex][moveId];
            const std::uint32_t skillIndex = skillWord >> 30;
            g_playerStats.currentVfSkill[skillIndex] |= static_cast<std::int32_t>(skillWord & 0x3fffffff);

            const std::uint8_t currentMove = kMoveSkillBankBases[bankIndex] + static_cast<std::uint8_t>(moveId);
            if (currentMove != 0xffU) {
                g_playerStats.currentMove = static_cast<std::uint16_t>((g_playerStats.currentMove & 0xff00U) | currentMove);
            }
        }
        if (!currentValue || moveValue < 12 || !removeExactAfterReplacement) {
            return currentValue;
        }
    } else if (operation == MoveSkillOp::Get) {
        return currentValue;
    }

    if (exactIndex < 0 || exactIndex >= count)
        return currentValue;

    for (std::int32_t index = exactIndex; index + 1 < count; ++index) {
        moveSet.moveValue[index] = moveSet.moveValue[index + 1];
        moveSet.learnValue[index] = moveSet.learnValue[index + 1];
    }

    --moveSet.count;
    return currentValue;
}

std::uint16_t ReadMoveSkill(std::uint16_t animationSet, std::uint16_t moveId) {
    return UpdateMoveSkill(MoveSkillOp::Get, static_cast<std::uint8_t>(animationSet), static_cast<std::int16_t>(moveId), 0);
}

std::array<std::uint16_t, 64> g_moveList{};
std::uint16_t g_moveCount = 0;
std::uint16_t g_selectedMove = 0;
std::uint8_t g_animationSet = 1;
SYGHOverlayState g_overlay{false, false, kClosedX, kClosedY, 0, 0, 0, {}, {}, {}};
struct MovesRuntimeTable {
    std::array<std::int16_t, 20> progress{};
    std::array<std::int16_t, 20> moveIds{};
    std::array<std::int16_t, 20> adjustments{};
    std::int16_t count{};
};

std::array<MovesRuntimeTable, 3> g_movesRuntimeTables{};
std::uint16_t g_movesScreenCurrentSelection{};
bool g_movesRuntimeInitialized = false;

void ResetMoveAnimationAdjustments() {
    for (MovesRuntimeTable &table : g_movesRuntimeTables)
        table.adjustments.fill(0);
}

std::uint16_t ResolveMoveAnimationValue(std::uint8_t animationSet, std::int16_t moveId, std::uint16_t adjustment) {
    if (animationSet < 1 || animationSet > 3)
        return adjustment;
    MovesRuntimeTable &table = g_movesRuntimeTables[animationSet - 1];
    for (std::int16_t index = 0; index < table.count; ++index) {
        if (table.moveIds[static_cast<std::size_t>(index)] != moveId) {
            continue;
        }
        if (adjustment != 0) {
            table.adjustments[static_cast<std::size_t>(index)] = static_cast<std::int16_t>(table.adjustments[static_cast<std::size_t>(index)] + adjustment);
            return adjustment;
        }
        const std::int16_t extra = table.adjustments[static_cast<std::size_t>(index)];
        return static_cast<std::uint16_t>(table.progress[static_cast<std::size_t>(index)] + (extra > 0 ? extra : 0));
    }
    return adjustment;
}

std::uint16_t ResolveMovesScreenEntry(std::int16_t entry, std::int16_t mode, std::int16_t animationSet) {
    std::int16_t resolvedSet = animationSet;
    std::int16_t index = entry;
    if (resolvedSet == 0) {
        resolvedSet = 1;
        while (resolvedSet <= 3 && index >= g_movesRuntimeTables[static_cast<std::size_t>(resolvedSet - 1)].count) {
            index = static_cast<std::int16_t>(index - g_movesRuntimeTables[static_cast<std::size_t>(resolvedSet - 1)].count);
            ++resolvedSet;
        }
    }
    if (resolvedSet < 1 || resolvedSet > 3)
        return 0;
    MovesRuntimeTable &table = g_movesRuntimeTables[static_cast<std::size_t>(resolvedSet - 1)];
    if (index < 0 || index >= table.count)
        return 0;
    const std::int16_t moveId = table.moveIds[static_cast<std::size_t>(index)];
    if (moveId == -1)
        return 0;
    if (mode == 2) {
        return static_cast<std::uint16_t>(moveId | (resolvedSet << 8));
    }
    if (mode == 1) {
        if (table.progress[static_cast<std::size_t>(index)] < 12) {
            shenExecuteAICACommand(264364U, 0, 0);
            return 1;
        }
        const std::uint16_t selection = static_cast<std::uint16_t>(resolvedSet | (static_cast<std::uint16_t>(index) << 8));
        if (g_movesScreenCurrentSelection == selection)
            return 1;
        g_movesScreenCurrentSelection = selection;
        g_animationSet = static_cast<std::uint8_t>(resolvedSet);
        g_selectedMove = static_cast<std::uint16_t>(index);
        g_playerStats.movesScreenMoveSetAndSelectionState = selection;
        if (moveId < 20) {
            const std::uint8_t currentMove = static_cast<std::uint8_t>(kMoveSkillBankBases[static_cast<std::size_t>(resolvedSet - 1)] + moveId);
            if (currentMove != 0xffU) {
                g_playerStats.currentMove = static_cast<std::uint16_t>((g_playerStats.currentMove & 0xff00U) | currentMove);
            }
        }
        return 0;
    }
    g_overlay.animationSet = static_cast<std::uint8_t>(resolvedSet);
    g_overlay.moveId = static_cast<std::uint16_t>(moveId);
    g_overlay.skillValue = table.progress[static_cast<std::size_t>(index)];
    return 0;
}

constexpr std::array<const char *, 19> kHandMoveNameIds{{
    "UIMOVETGKN", "UIMOVEELSL", "UIMOVETWKN", "UIMOVEELAS", // @text UIMOVETGKN EN: "Tiger Knuckle" | JA: "虎菱" ; UIMOVEELSL EN: "Elbow Slam" | JA: "小車打ち" ; UIMOVETWKN EN: "Twist Knuckle" | JA: "捻り菱" ; UIMOVEELAS EN: "Elbow Assault" | JA: "裡門頂肘"
    "UIMOVEUPKN", "UIMOVESLST", "UIMOVERATH", "UIMOVEBIWH", // @text UIMOVEUPKN EN: "Upper Knuckle" | JA: "昇り菱" ; UIMOVESLST EN: "Sleeve Strike" | JA: "袖突き" ; UIMOVERATH EN: "Rain Thrust" | JA: "村雨突き" ; UIMOVEBIWH EN: "Big Wheel" | JA: "大車"
    "UIMOVETWHA", "UIMOVEBAWI", "UIMOVEAVLA", "UIMOVEKAMI", // @text UIMOVETWHA EN: "Twin Hand Waves" | JA: "双手波" ; UIMOVEBAWI EN: "Backfist Willow" | JA: "裏柳" ; UIMOVEAVLA EN: "Avalanche Lance" | JA: "崩山槍" ; UIMOVEKAMI EN: "Katana Mist Slash" | JA: "太刀霞"
    "UIMOVEMIFL", "UIMOVEPITB", "UIMOVEDBLB", "UIMOVESWAL", // @text UIMOVEMIFL EN: "Mistral Flash" | JA: "薙風" ; UIMOVEPITB EN: "Pit Blow" | JA: "水月突き" ; UIMOVEDBLB EN: "Double Blow" | JA: "重ね当て" ; UIMOVESWAL EN: "Swallow Flip" | JA: "燕旋擺柳"
    "UIMOVERISF", "UIMOVETWIB", "UIMOVESTBA", // @text UIMOVERISF EN: "Rising Flash" | JA: "裂晃閃" ; UIMOVETWIB EN: "Twin Blades" | JA: "双刃" ; UIMOVESTBA EN: "Stab Armor" | JA: "鎧通"
}};
constexpr std::array<const char *, 18> kLegMoveNameIds{{
    "UIMOVECRKI", "UIMOVETRKI", "UIMOVESIRE", "UIMOVEAGCA", // @text UIMOVECRKI EN: "Crescent Kick" | JA: "三日月蹴り" ; UIMOVETRKI EN: "Trample Kick" | JA: "踏み蹴り" ; UIMOVESIRE EN: "Side Reaper Kick" | JA: "払い薙" ; UIMOVEAGCA EN: "Against Cascade" | JA: "瀧昇り"
    "UIMOVESUSL", "UIMOVETHKI", "UIMOVEHOAG", "UIMOVEBRTI", // @text UIMOVESUSL EN: "Surplice Slash" | JA: "袈裟斬り" ; UIMOVETHKI EN: "Thunder Kick" | JA: "雷蹴り" ; UIMOVEHOAG EN: "Hold Against Leg" | JA: "出足止め" ; UIMOVEBRTI EN: "Brutal Tiger" | JA: "虎乱"
    "UIMOVEDAMO", "UIMOVECYKI", "UIMOVEWIND", "UIMOVESWDI", // @text UIMOVEDAMO EN: "Dark Moon" | JA: "新月" ; UIMOVECYKI EN: "Cyclone Kick" | JA: "疾風" ; UIMOVEWIND EN: "Windmill" | JA: "風車" ; UIMOVESWDI EN: "Swallow Dive" | JA: "燕旋降脚"
    "UIMOVETOKI", "UIMOVENOSK", "UIMOVECRCY", "UIMOVEMUDS", // @text UIMOVETOKI EN: "Tornado Kick" | JA: "竜巻キック" ; UIMOVENOSK EN: "Nothing Skill" | JA: "未修得" ; UIMOVECRCY EN: "Crawl Cyclone" | JA: "旋風" ; UIMOVEMUDS EN: "Mud Spider" | JA: "土蜘蛛"
    "UIMOVETWSW", "UIMOVESHRE", // @text UIMOVETWSW EN: "Twin Swallow Leap" | JA: "飛燕連刈" ; UIMOVESHRE EN: "Shadow Reaper" | JA: "影狩り"
}};
constexpr std::array<const char *, 14> kThrowMoveNameIds{{
    "UIMOVEOVER", "UIMOVESWTH", "UIMOVEVOTH", "UIMOVEMIRE", // @text UIMOVEOVER EN: "Overthrow" | JA: "背負い投げ" ; UIMOVESWTH EN: "Sweep Throw" | JA: "払い腰" ; UIMOVEVOTH EN: "Vortex Throw" | JA: "巴投げ" ; UIMOVEMIRE EN: "Mist Reaper" | JA: "霞刈り"
    "UIMOVEDEDR", "UIMOVESHBU", "UIMOVETEDR", "UIMOVEDAHA", // @text UIMOVEDEDR EN: "Demon Drop" | JA: "羅刹落とし" ; UIMOVESHBU EN: "Shoulder Buster" | JA: "巻き固め" ; UIMOVETEDR EN: "Tengu Drop" | JA: "天狗勝" ; UIMOVEDAHA EN: "Darkside Hazuki" | JA: "裏芭月"
    "UIMOVEBATW", "UIMOVESHTE", "UIMOVEARBR", "UIMOVETIST", // @text UIMOVEBATW EN: "Back Twist Drop" | JA: "裏落とし" ; UIMOVESHTE EN: "Shadow Step" | JA: "影身" ; UIMOVEARBR EN: "Arm Break Fire" | JA: "朧無双" ; UIMOVETIST EN: "Tiger Storm" | JA: "虎身崩し"
    "UIMOVESHBL", "UIMOVECRCH", // @text UIMOVESHBL EN: "Shadow Blade" | JA: "影刀" ; UIMOVECRCH EN: "Cross Charge" | JA: "残月"
}};

struct NativeMoveTextBank {
    const char *const *moveNameIds;
    std::size_t count;
    const char *descriptionSuffix;
};

NativeMoveTextBank MoveTextBank(std::uint8_t animationSet) {
    switch (animationSet) {
    case 1:
        return {kHandMoveNameIds.data(), kHandMoveNameIds.size(), "HND"};
    case 2:
        return {kLegMoveNameIds.data(), kLegMoveNameIds.size(), "LEG"};
    case 3:
        return {kThrowMoveNameIds.data(), kThrowMoveNameIds.size(), "THR"};
    default:
        return {};
    }
}

SubtitleStringId MakeStringId(const char *text) {
    SubtitleStringId result{};
    if (text) {
        std::memcpy(&result, text, std::min<std::size_t>(std::strlen(text), sizeof(result)));
    }
    return result;
}

const char *LocalizedMoveText(const char *id) {
    const char *const text = ReadStringFromStringTableById(MakeStringId(id));
    return text ? text : "";
}

bool MoveScreenActive() {
    return (g_movesScreenTransitionFlags & 0xf0U) != 0 || g_syghDrawState.moveListState == 8;
}

void AuthorSelectedMove() {
    if (!g_moveCount)
        return;

    g_selectedMove = std::min<std::uint16_t>(g_selectedMove, static_cast<std::uint16_t>(g_moveCount - 1));
    const std::uint16_t selectedMove = (WZOG_GetTransitionFlags() & 0xf0U) != 0 ? 0 : g_selectedMove;
    ResolveMovesScreenEntry(static_cast<std::int16_t>(selectedMove), 0, static_cast<std::int16_t>(g_animationSet));
    const std::uint16_t moveId = g_moveList[selectedMove];
    g_overlay.animationSet = g_animationSet;
    g_overlay.moveId = moveId;
    g_overlay.skillValue = ReadMoveSkill(g_animationSet, moveId);
    g_overlay.moveName.fill(0);
    g_overlay.instruction.fill(0);
    for (auto &line : g_overlay.descriptionLines)
        line.fill(0);

    const NativeMoveTextBank bank = MoveTextBank(g_animationSet);
    if (!bank.moveNameIds || moveId >= bank.count)
        return;

    const char *const moveName = LocalizedMoveText(bank.moveNameIds[moveId]);
    char descriptionId[12]{};
    std::snprintf(descriptionId, sizeof(descriptionId), "UIWAZ%s%02u", bank.descriptionSuffix, static_cast<unsigned>(moveId));
    const char *const description = LocalizedMoveText(descriptionId);
    std::snprintf(g_overlay.moveName.data(), g_overlay.moveName.size(), "[ %s ]", moveName);
    std::snprintf(g_overlay.instruction.data(), g_overlay.instruction.size(), "%s.", description);

    constexpr std::size_t kNativeLineBytes = 36;
    const std::size_t descriptionBytes = std::strlen(g_overlay.instruction.data());
    for (std::size_t lineIndex = 0; lineIndex < g_overlay.descriptionLines.size(); ++lineIndex) {
        const std::size_t offset = lineIndex * kNativeLineBytes;
        if (offset >= descriptionBytes)
            break;

        auto &line = g_overlay.descriptionLines[lineIndex];
        const std::size_t bytes = std::min(kNativeLineBytes, descriptionBytes - offset);
        std::memcpy(line.data(), g_overlay.instruction.data() + offset, bytes);
    }
}
}

void InitializeMovesScreenRuntimeData() {
    constexpr std::array<std::int16_t, 3> kCounts{13, 11, 9};
    constexpr std::array<std::array<std::int16_t, 20>, 3> kInitialProgress{{
        {{28, 22, 23, 36, 20, 8, 5, 21, 20, 18, 19, 20, 19, 1, 1, 1, 1, 1, 1, 1}},
        {{19, 21, 23, 18, 17, 3, 7, 6, 1, 18, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
        {{26, 27, 27, 22, 17, 2, 4, 24, 17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
    }};

    for (std::size_t set = 0; set < g_movesRuntimeTables.size(); ++set) {
        MovesRuntimeTable &table = g_movesRuntimeTables[set];
        table.count = kCounts[set];
        table.progress = kInitialProgress[set];
        table.moveIds.fill(-1);
        for (std::int16_t index = 0; index < table.count; ++index) {
            table.moveIds[static_cast<std::size_t>(index)] = index;
        }
        MoveSetRuntimeState &moveSet = g_playerStats.moveSets[set];
        for (std::size_t index = 0; index < moveSet.learnValue.size(); ++index) {
            moveSet.learnValue[index] = static_cast<std::uint16_t>(table.progress[index]);
            moveSet.moveValue[index] = static_cast<std::uint16_t>(table.moveIds[index]);
        }
        moveSet.count = static_cast<std::uint8_t>(table.count);
        moveSet.countOrUnknown = 0;
        g_playerStats.moveUsageCounts[set].fill(0);
    }
    ResetMoveAnimationAdjustments();
    g_moveList.fill(1);
    g_moveCount = 0;
    g_selectedMove = 0;
    g_movesScreenCurrentSelection = 0;
    g_pendingMovePopupMoveId = 0;
    g_pendingMovePopupAnimSetId = 0;
    g_movesRuntimeInitialized = true;
}

HLib::Task *EnqueueSYGH() {

    g_syghPad = AllocateClearedPADCEntry();
    EnqueueSYGU();

    g_syghTask = HLib_EnqueueTaskWithoutParameter(CtrlTrainingHelp_SYGH_Callback, 5, 0, kSyghToken);
    g_syghDrawState.slideX = -304.0f;
    g_syghDrawState.slideFlags = {};
    g_movesScreenTextBank = nullptr;
    g_movesScreenMoveTable = nullptr;
    g_movesScreenRefreshState = 0;
    g_pMovesScreenTask = nullptr;
    g_pMovePopupTask = nullptr;
    g_syghDrawState.slideY = 240.0f;
    g_syghDrawState.scale = 1.0f;
    g_syghDrawState.slideState = 0;
    g_syghDrawState.depth = 1.0f;
    g_syghDrawState.selectionLimit = 90;
    g_movesScreenCurrentAnimSet = 1;
    g_movesScreenSyncedMoveSet = 0;
    g_playerStats.movesScreenSelectedMoveIndex = 0;
    g_syghDrawState.moveListState = 0;
    g_selectedMoveListIndex = 0;
    g_syghDrawState.runtime = 0;
    g_moveHelpPopupState = 0;
    g_syghDrawState.reservedState = 0;
    g_playerStats.movesScreenMoveSetAndSelectionState = 0xff00;
    SyncMovesScreenRuntimeState();
    const float depth = g_syghDrawState.depth;
    g_syghDrawState.frame.surface = 0;
    g_syghDrawState.frame.width = 128;
    g_syghDrawState.frame.height = 128;
    g_syghDrawState.frame.scaleX = 1.0f;
    g_syghDrawState.frame.scaleY = 1.0f;
    g_syghDrawState.frame.u = g_syghDrawState.frame.v = 0;
    g_syghDrawState.frame.flags = 5;
    g_moveListMoveIds = {};
    g_syghDrawState.move.surface = 0;
    g_syghDrawState.move.flags = 5;
    g_syghDrawState.move.u = g_syghDrawState.move.v = 0;
    g_syghDrawState.frame.depth = depth + 1.0f;
    g_syghDrawState.move.depth = depth + 7.0f;
    const float buttonY = g_syghDrawState.slideY + 70.0f;
    for (std::size_t index = 0; index < 2; ++index) {
        SYGHSprite &button = g_syghDrawState.buttons[index];
        button.x = index == 0 ? 240.0f : 400.0f;
        button.y = buttonY;
        button.depth = depth + 7.0f;
        button.width = 64;
        button.height = 32;
        button.surface = 0;
        button.scaleX = 1.0f;
        button.scaleY = 1.0f;
        button.u = button.v = 0;
        button.flags = 5;
    }
    g_syghDrawState.description.y = 152.0f;
    g_syghDrawState.background.surface = 0;
    g_syghDrawState.background.width = 256;
    g_syghDrawState.background.height = 256;
    g_syghDrawState.background.scaleX = 1.0f;
    g_syghDrawState.background.scaleY = 1.0f;
    g_syghDrawState.background.depth = depth;
    g_syghDrawState.background.u = g_syghDrawState.background.v = 0;
    g_syghDrawState.background.flags = 5;
    g_syghDrawState.description.scaleX = 1.0f;
    g_syghDrawState.description.scaleY = 1.0f;
    g_syghDrawState.description.width = 32;
    g_syghDrawState.description.height = 512;
    g_syghDrawState.description.surface = 0;
    g_syghDrawState.description.u = g_syghDrawState.description.v = 0;
    g_syghDrawState.description.flags = 5;
    g_syghDrawState.help.scaleX = 1.0f;
    g_syghDrawState.side.y = 215.0f;
    g_syghDrawState.help.scaleY = 1.0f;
    g_syghDrawState.help.width = 32;
    g_syghDrawState.help.height = 512;
    g_syghDrawState.help.surface = 0;
    g_syghDrawState.help.u = g_syghDrawState.help.v = 0;
    g_syghDrawState.description.depth = depth + 3.0f;
    g_syghDrawState.help.depth = depth + 3.0f;
    g_syghDrawState.popup.x = 264.0f;
    g_syghDrawState.underline.y = 404.0f;
    g_syghDrawState.help.flags = 5;
    g_syghDrawState.side.width = 48;
    g_syghDrawState.side.height = 379;
    g_syghDrawState.side.colour = 0x1f0000ff;
    g_syghDrawState.popup.centreX = 320.0f;
    g_syghDrawState.popup.width = 256;
    g_syghDrawState.popup.height = 176;
    g_syghDrawState.popup.y = 8.0f;
    g_syghDrawState.footer.x = 288.0f;
    g_syghDrawState.popup.colour = 0x7f000000;
    g_syghDrawState.underline.x = 320.0f;
    g_syghDrawState.underline.width = 32;
    g_syghDrawState.underline.height = 1;
    g_syghDrawState.underline.scaleX = 1.0f;
    g_syghDrawState.side.depth = depth + 1.0f;
    g_syghDrawState.underline.depth = g_syghDrawState.side.depth;
    g_syghDrawState.underline.scaleY = 1.0f;
    g_syghDrawState.underline.colour = 0x7f507dae;
    g_syghDrawState.underline.reserved1c = 0;
    g_syghDrawState.underline.flags = 1;
    g_syghDrawState.footer.y = 403.0f;
    g_syghDrawState.footer.width = 608;
    g_syghDrawState.footer.height = 1;
    g_syghDrawState.footer.colour = 0x7f0000ff;
    g_syghDrawState.marker.surface = 0;
    g_syghDrawState.marker.x = 616.0f;
    g_syghDrawState.marker.y = 403.0f;
    g_syghDrawState.marker.width = 32;
    g_syghDrawState.marker.height = 64;
    g_syghDrawState.marker.scaleX = 1.0f;
    g_syghDrawState.marker.scaleY = 1.0f;
    g_syghDrawState.footer.depth = depth + 2.0f;
    g_syghDrawState.marker.u = g_syghDrawState.marker.v = 0;
    g_syghDrawState.marker.flags = 5;
    g_syghDrawState.cursor.surface = 0;
    g_syghDrawState.cursor.width = 16;
    g_syghDrawState.cursor.height = 16;
    g_syghDrawState.marker.depth = depth + 5.0f;
    g_syghDrawState.cursor.y = 432.0f;
    g_syghDrawState.title.depth = 12.0f;
    g_syghDrawState.cursor.depth = g_syghDrawState.footer.depth;
    g_syghDrawState.cursor.scaleY = 1.0f;
    g_syghDrawState.cursor.u = g_syghDrawState.cursor.v = 0;
    g_syghDrawState.cursor.flags = 1;
    g_syghDrawState.title.surface = 0;
    g_syghDrawState.title.x = 320.0f;
    g_syghDrawState.title.width = 512;
    g_syghDrawState.title.height = 32;
    g_syghDrawState.title.scaleX = 1.0f;
    g_syghDrawState.title.scaleY = 1.0f;
    g_syghDrawState.title.u = g_syghDrawState.title.v = 0;
    g_syghDrawState.title.flags = 5;
    g_overlay = {false, false, 256.0f, 176.0f, 0, 0, 0, {}, {}, {}};
    return g_syghTask;
}

void CtrlTrainingHelp_SYGH_Callback(HLib::TaskData *) {
    if (!MoveScreenActive())
        return;
    if ((Shenmue_nPauseState & 2) != 0) {
        if (g_moveHelpPopupState != 0)
            HLib_InertTaskCallback(nullptr);
        return;
    }
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if ((pressed & kAcceptButton) != 0 || (g_moveHelpPopupState != 0 && (pressed & kHelpButton) != 0)) {
        shenExecuteAICACommand(kToggleSoundCommand, 0, 0);
        g_moveHelpPopupState = static_cast<std::uint8_t>((g_moveHelpPopupState + 1) & 0xf1);
        if ((g_moveHelpPopupState & 1) == 0) {
            g_moveHelpPopupState |= 0x10;
        } else {
            const std::int16_t set = (g_movesScreenTransitionFlags & 0xf0U) != 0 ? 0 : static_cast<std::int8_t>(g_selectedMoveListIndex);
            const std::int16_t entry = static_cast<std::int8_t>(g_moveListMoveIds[set]);
            const std::uint16_t selection = static_cast<std::uint16_t>(InitMoves(entry, 2, set));
            const std::uint8_t move = static_cast<std::uint8_t>(selection);
            const std::uint8_t animationSet = static_cast<std::uint8_t>(selection >> 8);
            const NativeMoveTextBank bank = MoveTextBank(animationSet);
            if (bank.moveNameIds && move < bank.count) {
                const char *const *descriptions = animationSet == 1 ? g_handMoveNames.data() :
                    animationSet == 2                               ? g_legMoveNames.data() :
                                                                      g_throwMoveNames.data();
                char description[255];
                std::int32_t length = 0;
                while (length < 252 && descriptions[move][length] != 0) {
                    description[length] = descriptions[move][length];
                    ++length;
                }
                if (length < 252)
                    description[length++] = '.';
                description[length] = 0;
                LoadFont("DC_KANJI.FON");
                if (HLib_CanAllocate64ByteBlock(512) == 1) {
                    char title[48];
                    std::sprintf(title, "[ %s ]", bank.moveNameIds[move]);
                    g_moveHelpSurfaceIds[0] = AllocatePVRSurface(title, 512, 0xffff, 0xf000, 0, true);
                }
                bool success = true;
                for (std::size_t lineIndex = 0; lineIndex < 6; ++lineIndex) {
                    if (HLib_CanAllocate64ByteBlock(512) != 1) {
                        success = false;
                        break;
                    }
                    char line[48];
                    std::size_t count = 0;
                    while (count < 36 && description[lineIndex * 36 + count] != 0) {
                        line[count] = description[lineIndex * 36 + count];
                        ++count;
                    }
                    for (std::size_t space = count; space < 36; ++space)
                        line[space] = ' ';
                    line[36] = 0;
                    g_moveHelpSurfaceIds[lineIndex + 1] = AllocatePVRSurface(line, 512, 0xffff, 0xf000, 0, true);
                    if (count < 36)
                        break;
                }
                LoadFont("WAZA.FON");
                if (success) {
                    g_syghDrawState.popup.x = 216.0f;
                    g_syghDrawState.popup.y = 11.0f;
                    g_syghDrawState.popup.width = 2;
                    g_syghDrawState.popup.height = 2;
                    g_syghDrawState.popup.colour = 0x7f0000ff;
                    g_overlay.animationSet = animationSet;
                    g_overlay.moveId = move;
                    g_overlay.skillValue = ReadMoveSkill(animationSet, move);
                    std::snprintf(g_overlay.moveName.data(), g_overlay.moveName.size(), "[ %s ]", bank.moveNameIds[move]);
                    std::snprintf(g_overlay.instruction.data(), g_overlay.instruction.size(), "%s", description);
                } else {
                    g_moveHelpPopupState = static_cast<std::uint8_t>((g_moveHelpPopupState + 1) & 0xf1);
                }
            } else {
                g_moveHelpPopupState = static_cast<std::uint8_t>((g_moveHelpPopupState + 1) & 0xf1);
            }
        }
        if ((g_moveHelpPopupState & 1) == 0) {
            for (std ::int16_t &surface : g_moveHelpSurfaceIds) {
                if (surface != -1)
                    PVR_FreeAllocatedSurfaceByIndex(surface);

                surface = -1;
            }
        }
    }
    if ((g_moveHelpPopupState & 1) != 0) {
        g_syghDrawState.popup.height = static_cast<std::int16_t>(static_cast<std::uint16_t>(g_syghDrawState.popup.height) + 25U);
        if (g_syghDrawState.popup.height >= 296) {
            g_syghDrawState.popup.height = 296;
            g_syghDrawState.popup.width = static_cast<std::int16_t>(static_cast<std::uint16_t>(g_syghDrawState.popup.width) + 152U);
            if (g_syghDrawState.popup.width > 520)
                g_syghDrawState.popup.width = 520;
        }
    } else if ((g_moveHelpPopupState & 0x10) != 0) {
        g_syghDrawState.popup.width = static_cast<std::int16_t>(static_cast<std::uint16_t>(g_syghDrawState.popup.width) - 152U);
        if (g_syghDrawState.popup.width < 2) {
            g_syghDrawState.popup.width = 2;
            g_syghDrawState.popup.height = static_cast<std::int16_t>(static_cast<std::uint16_t>(g_syghDrawState.popup.height) - 25U);
            if (g_syghDrawState.popup.height < 2) {
                g_syghDrawState.popup.height = 2;
                g_moveHelpPopupState = 0;
                return;
            }
        }
        if (g_syghDrawState.popup.height == 2) {
            g_moveHelpPopupState = 0;
            return;
        }
    }
    if (g_moveHelpPopupState != 0)
        HLib_InertTaskCallback(nullptr);
}

void SYGH_Destroy() {
    HLib::Task *const task = g_syghTask;
    g_syghTask = nullptr;

    if (task)
        HLib_DestroyTask(task);

    SYGU_Destroy();
    if (g_syghPad)
        ReleasePADCEntry(g_syghPad);

    g_syghPad = nullptr;
    g_moveCount = 0;
    g_selectedMove = 0;
    g_overlay = {false, false, kClosedX, kClosedY, 0, 0, 0, {}, {}, {}};
}

void SYGH_SetMoveSkill(std::uint16_t animationSet, std::uint16_t moveId, std::uint16_t value) {
    UpdateMoveSkill(MoveSkillOp::Set, static_cast<std::uint8_t>(animationSet), static_cast<std::int16_t>(moveId), static_cast<std::int16_t>(value));
}

void SYGH_RemoveMoveSkill(std::uint16_t animationSet, std::uint16_t moveId) {
    UpdateMoveSkill(MoveSkillOp::Remove, static_cast<std::uint8_t>(animationSet), static_cast<std::int16_t>(moveId), 1);
}

std::uint16_t SYGH_GetMoveSkill(std::uint16_t animationSet, std::uint16_t moveId) {
    return ReadMoveSkill(animationSet, moveId);
}

void SYGH_ApplyMajorStepMoveSkillProgression() {
    const std::uint8_t moveSetId = static_cast<std::uint8_t>(g_playerStats.movesScreenMoveSetAndSelectionState);
    if (moveSetId == 0) {
        for (std::size_t bank = 0; bank < g_playerStats.moveSets.size(); ++bank) {
            for (std::size_t index = 0; index < 20; ++index) {
                const std::uint16_t increment = static_cast<std::int16_t>(g_playerStats.moveSets[bank].learnValue[index]) < 12 ? 1 : 30;
                g_playerStats.moveUsageCounts[bank][index] = static_cast<std::uint16_t>(g_playerStats.moveUsageCounts[bank][index] + increment);
            }
        }
        (void)VFMA_RequestMove(VFMoveRequestFlags::ApplyEasedCommitTrigger);
        return;
    }
    if (moveSetId > 3)
        return;

    MoveSetRuntimeState &moveSet = g_playerStats.moveSets[moveSetId - 1];
    const std::int32_t count = static_cast<std::int8_t>(moveSet.count);
    if (count <= 0)
        return;

    const std::int8_t selectionMode = static_cast<std::int8_t>(g_playerStats.movesScreenMoveSetAndSelectionState >> 8);
    const std::int8_t selectedIndex = static_cast<std::int8_t>(g_playerStats.movesScreenSelectedMoveIndex);

    for (std::int32_t index = 0; index < count; ++index) {
        std::int16_t increment = selectionMode == 1 && index == selectedIndex ? 80 : 45;
        std::int16_t progress = static_cast<std::int16_t>(moveSet.learnValue[index]);

        const std::int16_t oldProgress = progress;
        if (progress < 12)
            increment = index == selectedIndex ? 2 : 1;

        if (progress < 56) {
            if (progress >= 12) {
                if (progress + increment / 10 < 56) {
                    increment = static_cast<std::int16_t>(increment / 10);
                } else {
                    const std::int16_t toThreshold = static_cast<std::int16_t>(56 - progress);
                    increment = static_cast<std::int16_t>(toThreshold + (increment - toThreshold) / 20);
                }
            }
        } else {
            increment = static_cast<std::int16_t>(increment / 20);
        }

        if (increment > 0)
            progress = static_cast<std::int16_t>(progress + increment);
        progress = std::min<std::int16_t>(progress, 100);
        moveSet.learnValue[index] = static_cast<std::uint16_t>(progress);
        if (oldProgress < 12 && progress >= 12) {
            UpdateMoveSkill(MoveSkillOp::Set, moveSetId, static_cast<std::int16_t>(moveSet.moveValue[index]), progress);
        }
    }
}

void SYGH_OpenMoveList(std::uint16_t animationSet) {
    if (animationSet == 0 || animationSet > 3)
        animationSet = 1;

    g_animationSet = static_cast<std::uint8_t>(animationSet);
    const NativeMoveTextBank bank = MoveTextBank(g_animationSet);
    g_moveCount = 0;
    for (std::uint16_t move = 0; move < bank.count; ++move) {
        if (ReadMoveSkill(animationSet, move) != 0)
            g_moveList[g_moveCount++] = move;
    }
    g_selectedMove = 0;
    g_playerStats.movesScreenMoveSetAndSelectionState = static_cast<std::uint16_t>(0x0100 | animationSet);
    g_playerStats.movesScreenSelectedMoveIndex = 0;
    EnqueueSYGH();
}

void SYGH_SetSelectedMove(std::uint16_t selection) {
    if (!g_moveCount)
        return;

    g_selectedMove = std::min<std::uint16_t>(selection, static_cast<std::uint16_t>(g_moveCount - 1));
    g_playerStats.movesScreenSelectedMoveIndex = static_cast<std::uint8_t>(g_selectedMove);
    if (g_overlay.visible)
        AuthorSelectedMove();
}

const SYGHOverlayState &SYGH_GetOverlayState() noexcept {
    g_overlay.visible = g_moveHelpPopupState != 0;
    g_overlay.opening = (g_moveHelpPopupState & 1) != 0;
    g_overlay.x = static_cast<float>(g_syghDrawState.popup.width);
    g_overlay.y = static_cast<float>(g_syghDrawState.popup.height);
    return g_overlay;
}

} // namespace shenmue::s1
