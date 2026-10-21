#include "s1/task/ewzo.hpp"
#include "s1/font.hpp"
#include "s1/initialization.hpp"
#include "s1/boot.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/subtitles.hpp"
#include "s1/player_stats.hpp"
#include "s1/steam.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

HLib::Task *g_pMovePopupTask = nullptr;
std::uint8_t g_pendingMovePopupAnimSetId = 0;
std::uint16_t g_pendingMovePopupMoveId = 0;
std::array<std::int16_t, 3> g_movePopupSurfaceIds{-1, -1, -1};
std::array<char, 256> g_movePopupTrainingText{};
std::array<char, 256> g_movePopupMoveNameText{};
std::array<char, 256> g_movePopupResultText{};
extern void *g_pFont;
extern std::int32_t g_busy_reset;
std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);

namespace {

constexpr std::int32_t makeToken(char a, char b, char c, char d) noexcept {
    return static_cast<std::int32_t>(static_cast<std::uint8_t>(a)) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(b)) << 8) |
        (static_cast<std::int32_t>(static_cast<std::uint8_t>(c)) << 16) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(d)) << 24);
}

template <std::size_t Size> constexpr SubtitleStringId makeStringId(const char (&text)[Size]) noexcept {
    SubtitleStringId result{};
    for (std::size_t index = 0; index + 1 < Size && index < sizeof(SubtitleStringId); ++index) {
        const std::uint32_t value = static_cast<std::uint32_t>(static_cast<std::uint8_t>(text[index])) << static_cast<std::uint32_t>((index & 3U) * 8U);
        if (index < 4) {
            result.word0 |= value;
        } else if (index < 8) {
            result.word1 |= value;
        } else {
            result.word2 |= value;
        }
    }
    return result;
}

constexpr std::int16_t kInvalidSurface = -1;
constexpr std::int32_t kEwzoSurfaceWidth = 256;
constexpr std::int32_t kEwzoSurfaceHeight = 16;
constexpr std::array<std::int32_t, 3> kEwzoTextureIds{makeToken('E', 'W', 'Z', '0'), makeToken('E', 'W', 'Z', '1'), makeToken('E', 'W', 'Z', '2')};
constexpr std::int32_t kEwzoTextureBank = makeToken('W', 'A', 'Z', 'A');

constexpr std::array<SubtitleStringId, 3> kTrainingHeadingIds{{
    makeStringId("UIWAZAHAN1"), // @text EN: "Hand move" | JA: "手技"
    makeStringId("UIWAZALEG1"), // @text EN: "Leg move" | JA: "足技"
    makeStringId("UIWAZATHR1"), // @text EN: "Throw move" | JA: "投技"
}};



constexpr std::array<SubtitleStringId, 19> kHandMoveIds{{
    makeStringId("UIMOVETGKN"), // @text EN: "Tiger Knuckle" | JA: "虎菱"
    makeStringId("UIMOVEELSL"), // @text EN: "Elbow Slam" | JA: "小車打ち"
    makeStringId("UIMOVETWKN"), // @text EN: "Twist Knuckle" | JA: "捻り菱"
    makeStringId("UIMOVEELAS"), // @text EN: "Elbow Assault" | JA: "裡門頂肘"
    makeStringId("UIMOVEUPKN"), // @text EN: "Upper Knuckle" | JA: "昇り菱"
    makeStringId("UIMOVESLST"), // @text EN: "Sleeve Strike" | JA: "袖突き"
    makeStringId("UIMOVERATH"), // @text EN: "Rain Thrust" | JA: "村雨突き"
    makeStringId("UIMOVEBIWH"), // @text EN: "Big Wheel" | JA: "大車"
    makeStringId("UIMOVETWHA"), // @text EN: "Twin Hand Waves" | JA: "双手波"
    makeStringId("UIMOVEBAWI"), // @text EN: "Backfist Willow" | JA: "裏柳"
    makeStringId("UIMOVEAVLA"), // @text EN: "Avalanche Lance" | JA: "崩山槍"
    makeStringId("UIMOVEKAMI"), // @text EN: "Katana Mist Slash" | JA: "太刀霞"
    makeStringId("UIMOVEMIFL"), // @text EN: "Mistral Flash" | JA: "薙風"
    makeStringId("UIMOVEPITB"), // @text EN: "Pit Blow" | JA: "水月突き"
    makeStringId("UIMOVEDBLB"), // @text EN: "Double Blow" | JA: "重ね当て"
    makeStringId("UIMOVESWAL"), // @text EN: "Swallow Flip" | JA: "燕旋擺柳"
    makeStringId("UIMOVERISF"), // @text EN: "Rising Flash" | JA: "裂晃閃"
    makeStringId("UIMOVETWIB"), // @text EN: "Twin Blades" | JA: "双刃"
    makeStringId("UIMOVESTBA"), // @text EN: "Stab Armor" | JA: "鎧通"
}};

constexpr std::array<SubtitleStringId, 18> kLegMoveIds{{
    makeStringId("UIMOVECRKI"), // @text EN: "Crescent Kick" | JA: "三日月蹴り"
    makeStringId("UIMOVETRKI"), // @text EN: "Trample Kick" | JA: "踏み蹴り"
    makeStringId("UIMOVESIRE"), // @text EN: "Side Reaper Kick" | JA: "払い薙"
    makeStringId("UIMOVEAGCA"), // @text EN: "Against Cascade" | JA: "瀧昇り"
    makeStringId("UIMOVESUSL"), // @text EN: "Surplice Slash" | JA: "袈裟斬り"
    makeStringId("UIMOVETHKI"), // @text EN: "Thunder Kick" | JA: "雷蹴り"
    makeStringId("UIMOVEHOAG"), // @text EN: "Hold Against Leg" | JA: "出足止め"
    makeStringId("UIMOVEBRTI"), // @text EN: "Brutal Tiger" | JA: "虎乱"
    makeStringId("UIMOVEDAMO"), // @text EN: "Dark Moon" | JA: "新月"
    makeStringId("UIMOVECYKI"), // @text EN: "Cyclone Kick" | JA: "疾風"
    makeStringId("UIMOVEWIND"), // @text EN: "Windmill" | JA: "風車"
    makeStringId("UIMOVESWDI"), // @text EN: "Swallow Dive" | JA: "燕旋降脚"
    makeStringId("UIMOVETOKI"), // @text EN: "Tornado Kick" | JA: "竜巻キック"
    makeStringId("UIMOVENOSK"), // @text EN: "Nothing Skill" | JA: "未修得"
    makeStringId("UIMOVECRCY"), // @text EN: "Crawl Cyclone" | JA: "旋風"
    makeStringId("UIMOVEMUDS"), // @text EN: "Mud Spider" | JA: "土蜘蛛"
    makeStringId("UIMOVETWSW"), // @text EN: "Twin Swallow Leap" | JA: "飛燕連刈"
    makeStringId("UIMOVESHRE"), // @text EN: "Shadow Reaper" | JA: "影狩り"
}};

constexpr std::array<SubtitleStringId, 33> kThrowMoveIds{{
    makeStringId("UIMOVEOVER"), // @text EN: "Overthrow" | JA: "背負い投げ"
    makeStringId("UIMOVESWTH"), // @text EN: "Sweep Throw" | JA: "払い腰"
    makeStringId("UIMOVEVOTH"), // @text EN: "Vortex Throw" | JA: "巴投げ"
    makeStringId("UIMOVEMIRE"), // @text EN: "Mist Reaper" | JA: "霞刈り"
    makeStringId("UIMOVEDEDR"), // @text EN: "Demon Drop" | JA: "羅刹落とし"
    makeStringId("UIMOVESHBU"), // @text EN: "Shoulder Buster" | JA: "巻き固め"
    makeStringId("UIMOVETEDR"), // @text EN: "Tengu Drop" | JA: "天狗勝"
    makeStringId("UIMOVEDAHA"), // @text EN: "Darkside Hazuki" | JA: "裏芭月"
    makeStringId("UIMOVEBATW"), // @text EN: "Back Twist Drop" | JA: "裏落とし"
    makeStringId("UIMOVESHTE"), // @text EN: "Shadow Step" | JA: "影身"
    makeStringId("UIMOVEARBR"), // @text EN: "Arm Break Fire" | JA: "朧無双"
    makeStringId("UIMOVETIST"), // @text EN: "Tiger Storm" | JA: "虎身崩し"
    makeStringId("UIMOVESHBL"), // @text EN: "Shadow Blade" | JA: "影刀"
    makeStringId("UIMOVECRCH"), // @text EN: "Cross Charge" | JA: "残月"
    makeStringId("UIMOVETGKN"), // @text EN: "Tiger Knuckle" | JA: "虎菱"
    makeStringId("UIMOVEELSL"), // @text EN: "Elbow Slam" | JA: "小車打ち"
    makeStringId("UIMOVETWKN"), // @text EN: "Twist Knuckle" | JA: "捻り菱"
    makeStringId("UIMOVEELAS"), // @text EN: "Elbow Assault" | JA: "裡門頂肘"
    makeStringId("UIMOVEUPKN"), // @text EN: "Upper Knuckle" | JA: "昇り菱"
    makeStringId("UIMOVESLST"), // @text EN: "Sleeve Strike" | JA: "袖突き"
    makeStringId("UIMOVERATH"), // @text EN: "Rain Thrust" | JA: "村雨突き"
    makeStringId("UIMOVEBIWH"), // @text EN: "Big Wheel" | JA: "大車"
    makeStringId("UIMOVETWHA"), // @text EN: "Twin Hand Waves" | JA: "双手波"
    makeStringId("UIMOVEBAWI"), // @text EN: "Backfist Willow" | JA: "裏柳"
    makeStringId("UIMOVEAVLA"), // @text EN: "Avalanche Lance" | JA: "崩山槍"
    makeStringId("UIMOVEKAMI"), // @text EN: "Katana Mist Slash" | JA: "太刀霞"
    makeStringId("UIMOVEMIFL"), // @text EN: "Mistral Flash" | JA: "薙風"
    makeStringId("UIMOVEPITB"), // @text EN: "Pit Blow" | JA: "水月突き"
    makeStringId("UIMOVEDBLB"), // @text EN: "Double Blow" | JA: "重ね当て"
    makeStringId("UIMOVESWAL"), // @text EN: "Swallow Flip" | JA: "燕旋擺柳"
    makeStringId("UIMOVERISF"), // @text EN: "Rising Flash" | JA: "裂晃閃"
    makeStringId("UIMOVETWIB"), // @text EN: "Twin Blades" | JA: "双刃"
    makeStringId("UIMOVESTBA"), // @text EN: "Stab Armor" | JA: "鎧通"
}};

constexpr SubtitleStringId kMoveLearnedId = makeStringId("UIWAZALEAR"); // @text EN: "has been learned." | JA: "を修得しました。"
constexpr SubtitleStringId kMoveMasteredId = makeStringId("UIWAZAMAST"); // @text EN: "has been mastered." | JA: "を修得しました。"

constexpr std::array<std::uint8_t, 8> kGlyphBitMasks{1, 2, 4, 8, 16, 32, 64, 128};
constexpr std::array<std::uint8_t, 91> kGlyphStamp{0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1, 1, 2, 4, 8, 8, 16, 16, 16, 8,
    8, 4, 2, 1, 1, 2, 4, 8, 16, 32, 128, 64, 16, 8, 4, 2, 1, 1, 2, 4, 8, 8, 16, 16, 64, 64, 8, 4, 2, 1, 1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1, 0, 1, 2, 2, 2, 2,
    2, 2, 2, 4, 2, 1, 0};

struct FontStorage {
    const std::uint8_t *kana;
    const std::uint8_t *kanji;
    const std::uint8_t *ryou;
};

std::int8_t g_movePopupSfxTimer = -1;
std::uint8_t g_movePopupMode = 0;
std::uint8_t g_movePopupAnimSetId = 0;
std::uint16_t g_movePopupMoveId = 0;
std::uint16_t g_movePopupSavedSicnState = 0;
EwzoUiHandler g_ewzoUiHandler = nullptr;

const std::uint8_t *glyphSource(const FontStorage &font, std::uint16_t glyphCode, bool usePrimaryKanji) {
    if ((glyphCode & 0x8080U) == 0x8080U || (glyphCode & 0xff00U) == 0x0100U) {
        return font.kana ? font.kana + 30U * static_cast<std::uint8_t>(glyphCode) : nullptr;
    }

    const std::int32_t glyphIndex = 94 * static_cast<std::uint8_t>(glyphCode >> 8) + static_cast<std::uint8_t>(glyphCode) - 3135;
    const std::uint8_t *const base = usePrimaryKanji || !font.ryou ? font.kanji : font.ryou;
    return base ? base + 45 * glyphIndex : nullptr;
}

std::uint32_t expandPaletteColour(std::uint16_t colour) {
    std::uint32_t expanded = colour >> 12;
    expanded = (expanded << 8) + ((colour >> 8) & 0x0fU);
    expanded = (expanded << 8) + ((colour >> 4) & 0x0fU);
    expanded = (expanded << 8) + (colour & 0x0fU);
    return expanded * 17U;
}

const SubtitleStringId *trainingHeadingId(std::uint16_t animationSetId) {
    if (animationSetId == 0 || animationSetId > kTrainingHeadingIds.size()) {
        return nullptr;
    }
    return &kTrainingHeadingIds[animationSetId - 1];
}

const SubtitleStringId *trainingMoveId(std::uint16_t animationSetId, std::uint16_t moveId) {
    switch (animationSetId) {
    case 1:
        return moveId < kHandMoveIds.size() ? &kHandMoveIds[moveId] : nullptr;
    case 2:
        return moveId < kLegMoveIds.size() ? &kLegMoveIds[moveId] : nullptr;
    case 3:
        return moveId < kThrowMoveIds.size() ? &kThrowMoveIds[moveId] : nullptr;
    default:
        return nullptr;
    }
}

const char *localizedText(const SubtitleStringId *id) {
    return id ? ReadStringFromStringTableById(*id) : "";
}

template <std::size_t Capacity> void copySecure(std::array<char, Capacity> &destination, const char *source) {
    destination.fill(0);
    if (!source)
        return;
    const std::size_t length = std::strlen(source);
    if (length >= Capacity)
        return;
    std::memcpy(destination.data(), source, length + 1);
}

void rasterizeEwzoSentinel(const FontStorage &font, std::uint32_t *pixels) {
    constexpr std::uint16_t kGlyphCode = 0x012e;
    constexpr std::array<std::uint16_t, 4> kPalette{0, 0xffff, 0xb000, 0xb000};
    const std::uint8_t *const source = glyphSource(font, kGlyphCode, true);
    if (!source)
        return;

    std::array<std::uint8_t, 25 * 16> coverage{};
    for (std::int32_t sourcePixel = 0; sourcePixel < 240; ++sourcePixel) {
        if ((source[sourcePixel >> 3] & kGlyphBitMasks[sourcePixel & 7]) == 0) {
            continue;
        }
        const std::int32_t x = sourcePixel & 0x0f;
        const std::int32_t y = sourcePixel >> 4;
        coverage[25 * y + x] |= 2;
        coverage[25 * y + x + 1] |= 1;
        coverage[25 * (y + 1) + x] |= 1;
        coverage[25 * (y + 1) + x + 1] |= 1;
    }

    constexpr std::int32_t startX = (kEwzoSurfaceWidth - 12) / 2;
    for (std::int32_t sourceY = 0; sourceY < kEwzoSurfaceHeight; ++sourceY) {
        std::uint32_t *const destination = pixels + (kEwzoSurfaceHeight - sourceY - 1) * kEwzoSurfaceWidth + startX;
        const std::uint8_t *const sourceRow = coverage.data() + 25 * sourceY;
        for (std::int32_t x = 0; x < 13; ++x) {
            destination[x] = expandPaletteColour(kPalette[sourceRow[x]]);
        }
    }
}

std::int16_t allocateEwzoTextSurface(const FontStorage &font, const char *text, std::size_t slot) {
    if (slot >= g_movePopupSurfaceIds.size())
        return kInvalidSurface;




    static_cast<void>(text);
    constexpr std::size_t kPixelBytes = kEwzoSurfaceWidth * kEwzoSurfaceHeight * sizeof(std::uint32_t);
    auto *const pixels = static_cast<std::uint32_t *>(HLib_AllocHeapMemBlock(kPixelBytes, makeToken('E', 'W', 'Z', 'P')));
    if (!pixels)
        return kInvalidSurface;
    std::memset(pixels, 0, kPixelBytes);
    rasterizeEwzoSentinel(font, pixels);

    std::int16_t surface = kInvalidSurface;
    if (g_pFirstTaskData) {
        const std::intptr_t arenaOffset = reinterpret_cast<std::intptr_t>(pixels) - reinterpret_cast<std::intptr_t>(g_pFirstTaskData) - 0x40;
        if (arenaOffset >= std::numeric_limits<std::int32_t>::min() && arenaOffset <= std::numeric_limits<std::int32_t>::max() && arenaOffset != 0) {
            const std::int32_t allocated = PVR_FindOrAllocateSurface(kEwzoTextureIds[slot], kEwzoTextureBank);
            if (allocated >= 0) {
                const TextureUploadDescription upload{static_cast<std::int16_t>(kEwzoSurfaceWidth), static_cast<std::int16_t>(kEwzoSurfaceHeight), 0x8089,
                    static_cast<std::int32_t>(arenaOffset)};
                if (d3t_UploadTexture2DToGPU(allocated, &upload, pixels) >= 0) {
                    surface = static_cast<std::int16_t>(allocated);
                } else {
                    PVR_FreeAllocatedSurfaceByIndex(allocated);
                }
            }
        }
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(pixels), 0);
    return surface;
}

void authorEwzoTextResources(std::uint8_t mode, std::uint16_t animationSetId, std::uint16_t moveId) {
    auto *const font = static_cast<FontStorage *>(g_pFont);
    if (!font)
        return;

    LoadFont("WAZA.FON");

    const SubtitleStringId *const heading = trainingHeadingId(animationSetId);
    const char *text = localizedText(heading);
    g_movePopupSurfaceIds[0] = allocateEwzoTextSurface(*font, text, 0);
    copySecure(g_movePopupTrainingText, localizedText(heading));

    const SubtitleStringId *const move = trainingMoveId(animationSetId, moveId);
    std::array<char, 32> formattedMove{};
    copySecure(formattedMove, localizedText(move));
    g_movePopupSurfaceIds[1] = allocateEwzoTextSurface(*font, formattedMove.data(), 1);
    copySecure(g_movePopupMoveNameText, localizedText(move));

    const SubtitleStringId *const result = mode == 2 ? &kMoveLearnedId : &kMoveMasteredId;
    text = localizedText(result);
    g_movePopupSurfaceIds[2] = allocateEwzoTextSurface(*font, text, 2);
    copySecure(g_movePopupResultText, localizedText(result));

    LoadFont("DC_KANJI.FON");
}

void releaseEwzoTextResources() {
    for (std::int16_t surface : g_movePopupSurfaceIds) {
        if (surface != kInvalidSurface) {
            PVR_FreeAllocatedSurfaceByIndex(surface);
        }
    }
}

bool destroyUiElement(std::size_t index) {
    UIBaseRuntime &element = g_uiElements[index];
    if (!element.isVisible)
        return true;
    if (element.destroy && element.destroy() != 1 && g_busy_reset != 1) {
        return false;
    }
    element.isVisible = 0;
    return true;
}

EwzoUiBinding currentBinding() {
    return EwzoUiBinding{g_movePopupMode, g_movePopupAnimSetId, g_movePopupMoveId, &g_movePopupSurfaceIds, &g_movePopupTrainingText, &g_movePopupMoveNameText,
        &g_movePopupResultText};
}

void clearPopupState() {
    g_movePopupMode = 0;
    g_movePopupAnimSetId = 0;
    g_movePopupMoveId = 0;
    g_movePopupSfxTimer = -1;
}
std::array<EwzoUiEvent, 8> g_ewzoValidationEvents{};
std::array<EwzoUiBinding, 8> g_ewzoValidationBindings{};
std::size_t g_ewzoValidationEventCount = 0;
std::int64_t EWZO_ValidationUiHandler(EwzoUiEvent event, const EwzoUiBinding &binding) {
    if (g_ewzoValidationEventCount < g_ewzoValidationEvents.size()) {
        g_ewzoValidationEvents[g_ewzoValidationEventCount] = event;
        g_ewzoValidationBindings[g_ewzoValidationEventCount] = binding;
        ++g_ewzoValidationEventCount;
    }
    return 1;
}

} // namespace

void EWZO_SetUiHandler(EwzoUiHandler handler) {
    g_ewzoUiHandler = handler;
}

EwzoUiBinding EWZO_GetUiBinding() {
    return currentBinding();
}

void EnqueueEWZO(std::uint8_t mode, std::uint16_t animationSetId, std::uint16_t moveId) {
    if ((mode & 0x80U) != 0) {
        animationSetId = g_pendingMovePopupAnimSetId;
        moveId = g_pendingMovePopupMoveId;
        g_pendingMovePopupAnimSetId = 0;
        g_pendingMovePopupMoveId = 0;
        mode &= 0x7fU;
    }

    if ((mode & 1U) != 0) {
        if (!g_pMovePopupTask)
            return;

        releaseEwzoTextResources();
        const EwzoUiBinding binding = currentBinding();
        if (g_ewzoUiHandler) {
            g_ewzoUiHandler(EwzoUiEvent::Close, binding);
        }
        if (g_currUIState == -1 || g_currUIState == kEwzoUiElementIndex) {
            static_cast<void>(destroyUiElement(kEwzoUiElementIndex));
        }
        HLib_DestroyTask(g_pMovePopupTask);
        g_pMovePopupTask = nullptr;
        UI_SICNSetState(g_movePopupSavedSicnState, UISicnPausePolicy::KeepOnPause);
        return;
    }

    SteamStatDefinition &stat = g_steamTemporaryStats[Stat_Moves_Learned];
    if (stat.type == SS_Set)
        stat.value = 1;
    else if (stat.type == SS_Add)
        stat.value = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(stat.value) + 1U);

    UpdateSteamStat(Stat_Moves_Learned);
    UpdateSteamAchievement(Stat_Moves_Learned);
    if (g_pMovePopupTask || animationSetId == 0)
        return;

    d3t::ActivateElementByIdx(kEwzoUiElementIndex);
    g_movePopupMode = mode;
    g_movePopupAnimSetId = static_cast<std::uint8_t>(animationSetId);
    g_movePopupMoveId = moveId;
    g_movePopupSavedSicnState = UI_GetPresentationSnapshot().sicnState;
    if (g_ewzoUiHandler) {
        g_ewzoUiHandler(EwzoUiEvent::Activate, currentBinding());
    }

    g_pMovePopupTask = HLib_EnqueueTaskWithoutParameter(EWZO_Callback, 15, 0, makeToken('E', 'W', 'Z', 'O'));
    authorEwzoTextResources(mode, animationSetId, moveId);
    if (g_ewzoUiHandler) {
        g_ewzoUiHandler(EwzoUiEvent::BindLabels, currentBinding());
    }

    if (mode == 2) {
        shenExecuteAICACommand(0x0aabU, 0, 0);
        g_movePopupSfxTimer = -1;
    } else {
        shenExecuteAICACommand(0x208abU, 0, 0);
        g_movePopupSfxTimer = 18;
    }
}

void EWZO_Callback(HLib::TaskData *) {
    if (g_movePopupSfxTimer < 0)
        return;

    --g_movePopupSfxTimer;
    if (g_movePopupSfxTimer == 13) {
        shenExecuteAICACommand(0x208abU, 0, 0);
    }
    if (g_movePopupSfxTimer < 0) {
        shenExecuteAICACommand(0x208abU, 0, 0);
    }
}

void EWZO_Destroy() {
    releaseEwzoTextResources();
    if (g_ewzoUiHandler) {
        g_ewzoUiHandler(EwzoUiEvent::Close, currentBinding());
    }
    if (g_currUIState == -1 || g_currUIState == kEwzoUiElementIndex) {
        UIBaseRuntime &element = g_uiElements[kEwzoUiElementIndex];
        element.isVisible = 0;
    }
    if (g_pMovePopupTask) {
        HLib_DestroyTask(g_pMovePopupTask);
        g_pMovePopupTask = nullptr;
    }
    clearPopupState();
}

} // namespace shenmue::s1
