#include "s1/task/sygh.hpp"

#include "s1/boot.hpp"
#include "s1/input.hpp"
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
    return (WZOG_GetTransitionFlags() & 0xf0U) != 0 || WZOG_GetRefreshState() == 8;
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

void BeginOpen() {
    if (!g_moveCount)
        return;

    AuthorSelectedMove();
    g_overlay.visible = true;
    g_overlay.opening = true;
    g_overlay.x = kClosedX;
    g_overlay.y = kClosedY;
}

void BeginClose() {
    if (!g_overlay.visible)
        return;

    g_overlay.opening = false;
    g_overlay.moveName.fill(0);
    g_overlay.instruction.fill(0);
    for (auto &line : g_overlay.descriptionLines)
        line.fill(0);
}

void AdvanceOverlay() {
    if (!g_overlay.visible)
        return;

    if (g_overlay.opening) {
        if (g_overlay.y < kOpenY) {
            g_overlay.y = std::min(kOpenY, g_overlay.y + kSlideY);
            if (g_overlay.y < kOpenY)
                return;
        }
        g_overlay.x = std::min(kOpenX, g_overlay.x + kSlideX);
        return;
    }

    if (g_overlay.x > kClosedX) {
        g_overlay.x = std::max(kClosedX, g_overlay.x - kSlideX);
        if (g_overlay.x > kClosedX)
            return;
    }

    g_overlay.y = std::max(kClosedY, g_overlay.y - kSlideY);
    if (g_overlay.x == kClosedX && g_overlay.y == kClosedY) {
        g_overlay.visible = false;
        g_overlay.animationSet = 0;
        g_overlay.moveId = 0;
        g_overlay.skillValue = 0;
        g_overlay.moveName.fill(0);
        g_overlay.instruction.fill(0);
        for (auto &line : g_overlay.descriptionLines)
            line.fill(0);
    }
}

} // namespace



void InitializeMovesScreenRuntimeData() {
    constexpr std::array<std::int16_t, 3> kCounts{13, 11, 9};
    constexpr std::array<std::array<std::int16_t, 20>, 3> kInitialProgress{{
            {{28, 22, 23, 36, 20, 8, 5, 21, 20, 18, 19,
              20, 19, 1, 1, 1, 1, 1, 1, 1}},
            {{19, 21, 23, 18, 17, 3, 7, 6, 1, 18, 15,
              1, 1, 1, 1, 1, 1, 1, 1, 1}},
            {{26, 27, 27, 22, 17, 2, 4, 24, 17, 1, 1,
              1, 1, 1, 1, 1, 1, 1, 1, 1}},
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
    if (g_syghTask && g_syghTask->mainTaskCallbackFnPtr && g_syghTask->taskToken == static_cast<std::int32_t>(kSyghToken))
        return g_syghTask;
    if (!g_movesRuntimeInitialized)
        InitializeMovesScreenRuntimeData();

    g_syghPad = AllocateClearedPADCEntry();
    if (!EnqueueSYGU()) {
        if (g_syghPad)
            ReleasePADCEntry(g_syghPad);
        g_syghPad = nullptr;
        return nullptr;
    }

    g_syghTask = HLib_EnqueueTaskWithoutParameter(CtrlTrainingHelp_SYGH_Callback, 5, 0, kSyghToken);
    if (!g_syghTask) {
        SYGU_Destroy();
        if (g_syghPad)
            ReleasePADCEntry(g_syghPad);
        g_syghPad = nullptr;
        return nullptr;
    }
    g_overlay = {false, false, kClosedX, kClosedY, 0, 0, 0, {}, {}, {}};
    return g_syghTask;
}

void CtrlTrainingHelp_SYGH_Callback(HLib::TaskData *) {
    if (!MoveScreenActive())
        return;

    if ((Shenmue_nPauseState & 2) != 0)
        return;

    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if ((pressed & kAcceptButton) != 0 || (g_overlay.visible && (pressed & kHelpButton) != 0)) {
        shenExecuteAICACommand(kToggleSoundCommand, 0, 0);
        if (g_overlay.visible && g_overlay.opening) {
            BeginClose();
        } else if (g_overlay.visible) {
            AuthorSelectedMove();
            g_overlay.opening = true;
        } else {
            BeginOpen();
        }
    }
    AdvanceOverlay();
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
                    increment = static_cast<std::int16_t>(toThreshold + (increment - toThreshold) / 10);
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
    return g_overlay;
}

} // namespace shenmue::s1
