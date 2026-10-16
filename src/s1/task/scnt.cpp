#include "s1/task/scnt.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/face.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/motion.hpp"

#include "s1/subtitles.hpp"
#include "s1/task/scen.hpp"
#include "s1/screen_space.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/wind.hpp"
#include "s1/matrix_stack.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
namespace shenmue::s1 {
extern std::int32_t g_bGlobalFlag;

std::int8_t g_freeTalkBlendFrames = -1;
std::uint8_t g_freeTalkLetterExchangeFrameCounter = 0;
std::uint8_t g_freetalk_timer_frames = 0;
std::uint8_t g_freeTalkCurrentProgress = 0;
std::uint32_t g_freeTalkPartnerCrossedHemisphere = 0;
std::uint8_t g_has_free_talk_time_limit = 0;
std::uint8_t g_freetalk_timer_complete_flag = 0;
std::uint8_t g_freeTalkMotionResetCleanupFrameCounter = 0;
std::uint32_t g_freeTalkCurrentCharaId = 0;
std::uint32_t SecondChara = 0;
std::uint8_t g_freeTalkCurrentCharaIsContextOperand = 0;
std::array<std::uint32_t, 8> g_talk_name_tbl{};
std::array<std::uint32_t, 8> g_freeTalkConstraintCharaByTalkSlot{};
std::uint8_t g_freeTalkParticipantMobjRestorePending = 0;
std::uint8_t g_freeTalkRyoConstrained = 0;
std::array<std::uint8_t, 10> g_freeTalkFaceBlendUpdatePending{};
Vector3 g_freeTalkLookTargetPos{};
const Vector3 *g_freeTalkTargetPoint = nullptr;
std::uint8_t g_freeTalkMotionResetPending = 0;
std::uint32_t g_pendingFreeTalkMotionResetCharaId = 0;
std::int32_t g_FreeTalk_Status = 0;
std::int32_t g_pendingFreeTalkCommandCode = 0;
std::uint8_t g_freeTalkDelayedFlagSetPending = 0;
std::uint8_t g_freetalk_flag_0 = 0;
std::array<std::uint32_t, 8> g_prevFreeTalkPartner{};
std::int32_t g_tmp_chara_rot_y = 0;
std::uint8_t g_freeTalkFacePairStarted = 0;
float g_freeTalkFaceBaselineHandY = 0.0F;
float g_freeTalkFaceBaselineUpperY = 0.0F;
std::uint8_t g_freeTalkFaceBaselinesCaptured = 0;
std::uint8_t g_freeTalkConstraintsDisabled = 0;
std::uint8_t bRyoFacingDirectionFlag = 0;
std::uint8_t g_freeTalkMotionResetCleanupPending = 0;
std::uint8_t g_faceFlag = 0;
std::uint8_t g_freeTalkFaceBlendSuppressLook = 0;
std::array<std::uint32_t, 8> g_freeTalkMotionIdBySlot{};
std::array<std::int32_t, 8> g_freeTalkRelativePosModeBySlot{};
std::array<std::int32_t, 8> g_freeTalkFacingBySlot{};
std::array<std::uint8_t, 8> g_freeTalkParticipantRestorePending{};
std::array<std::uint8_t, 8> g_freeTalkConstraintCompletionPending{};
std::uint32_t g_freeTalkPairHeadSource = 0;
std::uint32_t g_freeTalkPairLookActor = 0;
std::array<std::uint32_t, 10> g_freeTalkPairHeadSources{};
std::array<std::uint32_t, 10> g_freeTalkPairLookActors{};
std::array<std::uint8_t, 10> g_freeTalkPairSetupPending{};
std::array<std::int32_t, 10> g_freeTalkPairFacing{};
std::array<std::uint8_t, 10> g_freeTalkFaceBlendFrame{};
Vector3 g_freeTalkFaceBlendPoint{};
std::int32_t g_freeTalkFaceBlendMotionGate = 1;
std::int32_t g_freeTalkRyoFacingYaw = 0;
std::int32_t g_freeTalkPendingAnimState = 0;

namespace {

std::uint32_t g_scnfSubtitleFrames = 0;

constexpr std::int32_t kFreeTalkNone = -1;
constexpr std::int32_t kFreeTalkStatusCommand = 109;



HLib::Task *g_freetalkTask = nullptr;

constexpr std::size_t kWindowPointerOffset = 0x98;
constexpr std::size_t kDialogueWorkOffset = 0xa0;
constexpr std::size_t kDialogueSourceOffset = 0xec;

static_assert(sizeof(ScenarioTaskData) == 0x2b8);
static_assert(offsetof(ScenarioTaskData, reserved_098) == kWindowPointerOffset);
static_assert(kDialogueWorkOffset + sizeof(ScntDialogueWork) == kDialogueSourceOffset);

template <typename T> T &ScenarioField(ScenarioTaskData &data, std::size_t offset) {
    return *reinterpret_cast<T *>(reinterpret_cast<std::byte *>(&data) + offset);
}

ScenarioTaskData *CurrentScenarioData(ScenarioContext *context) {
    if (!context || !context->task || !context->task->task || !context->task->task->task_data_ptr) {
        return nullptr;
    }
    return reinterpret_cast<ScenarioTaskData *>(context->task->task->task_data_ptr);
}

HLib::Task *CurrentScenarioTask() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    const auto *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
    return control->scenarioTask;
}

bool IsShiftJisLeadByte(std::uint8_t value) {
    return (value >= 0x81U && value <= 0x9fU) || (value >= 0xe0U && value <= 0xfcU);
}

bool IsShiftJisTrailByte(std::uint8_t value) {
    return (value >= 0x40U && value <= 0x7eU) || (value >= 0x80U && value <= 0xfcU);
}

std::int32_t GetAspectRatio() {
    const DisplayModeInfo *const resolution = GetScreenResolution();
    float scale = g_currentSettings.display.resolutionScale;
    const float scaledHeight = (scale * 0.6F + 0.4F) * static_cast<float>(resolution->height);
    if (g_getAspectRatioModeFunction) {
        if (g_getAspectRatioModeFunction())
            return static_cast<std::int32_t>(scaledHeight * 1.33333F);
        scale = g_currentSettings.display.resolutionScale;
    }
    return static_cast<std::int32_t>((scale * 0.6F + 0.4F) * static_cast<float>(resolution->width));
}

void ResetDialogueWindow(WindEntry &window);

WindEntry *InitializeWindowBorders(HLib::Task *windTask) {
    if (!windTask)
        windTask = g_windTask;
    WindTaskData *const wind = windTask ? reinterpret_cast<WindTaskData *>(windTask->task_data_ptr) : nullptr;
    if (!wind)
        return nullptr;
    WindEntry *window = wind->entries;
    std::size_t index = 0;
    while (index < 5 && window->flags != 0) {
        ++index;
        ++window;
    }
    if (index == 5)
        return nullptr;
    window->flags = 1;
    window->rowScrollOrigin = 0;
    window->contentColumns = 1;
    window->contentRows = 1;
    window->cellWidthPixels = 46;
    window->cellHeightPixels = 2;
    const float aspectWidth = static_cast<float>(GetAspectRatio());
    const float screenHeight = static_cast<float>(GetScreenResolution()->height);
    const float scaledHeight = static_cast<float>(static_cast<double>(screenHeight) / 1.25);
    window->field40 = 0;
    window->field42 = static_cast<std::int16_t>(static_cast<std::int32_t>(scaledHeight));
    const std::int16_t horizontalOffset = static_cast<std::int16_t>(static_cast<std::int32_t>((aspectWidth - 598.0F) * 0.5F));
    window->field24 = std::bit_cast<std::int32_t>(static_cast<float>(horizontalOffset));
    window->field28 = std::bit_cast<std::int32_t>(1.0F);
    window->drawWidth = static_cast<std::int16_t>(static_cast<std::int32_t>(aspectWidth));
    window->drawHeight = static_cast<std::int16_t>(static_cast<std::int32_t>(screenHeight - scaledHeight));
    const std::int32_t contentWidth = window->cellWidthPixels * (window->contentColumns + 12);
    std::int32_t textureWidth = 8;
    while (textureWidth < contentWidth)
        textureWidth *= 2;
    window->texturePitchBytes = static_cast<std::int16_t>(textureWidth * 4);
    window->textureHeight = 16;
    window->uScale = static_cast<float>(contentWidth) / static_cast<float>(textureWidth);
    window->vScale = 0.9375F;
    window->sourceWidth = static_cast<std::int16_t>(contentWidth);
    window->sourceHeight = 30;
    const std::int32_t cellBytes = window->cellWidthPixels * window->cellHeightPixels;
    const std::int64_t allocationBytes = 8 + 0x5a40 + 2 * cellBytes + window->cellHeightPixels;
    std::uint8_t *const heap = static_cast<std::uint8_t *>(HLib_AllocHeapMemBlock(allocationBytes, SHEN_NAME(WNDB)));
    window->heapBase = heap;
    window->surfaceHandleSlot = reinterpret_cast<std::uint8_t **>(heap);
    window->workBuffer = heap + 8;
    window->callbackContext = nullptr;
    window->callback = nullptr;
    window->waitFlag = 0;
    window->cellBufferA = window->workBuffer + 0x5a40;
    window->cellBufferB = window->cellBufferA + cellBytes;
    window->rowDirtyFlags = window->cellBufferB + cellBytes;
    *window->surfaceHandleSlot = reinterpret_cast<std::uint8_t *>(SurfaceAllocation(1024, 64));
    SurfaceAllocation_Wrapper(reinterpret_cast<ShenmueSurfaceOwner *>(window));
    window->rowScrollOrigin = 0;
    for (std::int32_t row = 0; row < window->cellHeightPixels; ++row)
        window->rowDirtyFlags[row] = 0;
    ResetDialogueWindow(*window);
    return window;
}

void ResetDialogueWindow(WindEntry &window) {
    const std::size_t cellBytes = static_cast<std::size_t>(window.cellWidthPixels * window.cellHeightPixels);
    std::memset(window.cellBufferA, 0x80, cellBytes);
    std::memset(window.cellBufferB, 0x20, cellBytes);
    window.packedCursor = 0;
    for (std::int32_t row = 0; row < window.cellHeightPixels; ++row)
        window.rowDirtyFlags[row] |= 8U;
    std::memset(window.workBuffer, 0, 0x5a40);
    if (window.callback)
        window.callback(&window, 4, window.callbackContext);
    window.rowEndColumns[0] = 0;
    window.rowEndColumns[1] = 0;
    if ((window.flags & 4U) != 0 && window.callback)
        window.callback(&window, 6, window.callbackContext);
    window.flags &= ~0x0cU;
}

bool AdvanceDialogueToken(ScntDialogueTokenReader &reader) {
    for (;;) {
        const std::uint8_t first = *reader.cursor;
        reader.token = first;
        if (first == 0) {
            reader.tokenKind = 0;
            return false;
        }
        if (reader.pendingLead != 0) {
            if (first == 0x7fU) {
                reader.pendingLead = 0;
                ++reader.cursor;
                continue;
            }
            if (first < 0xfdU) {
                reader.token |= reader.pendingLead;
                ++reader.cursor;
                reader.tokenKind = 2;
                return true;
            }
        }
        if (reader.encodingMode == 0 && first >= 0xfdU) {
            reader.pendingLead = static_cast<std::uint16_t>((first - 124U) << 8U);
            ++reader.cursor;
            continue;
        }
        if (reader.isLeadByte(first) && reader.isTrailByte(reader.cursor[1])) {
            reader.token = static_cast<std::uint16_t>((static_cast<std::uint16_t>(first) << 8U) | reader.cursor[1]);
            reader.cursor += 2;
            reader.tokenKind = 2;
            return true;
        }
        ++reader.cursor;
        reader.tokenKind = first == '\n' ? 0U : 1U;
        return true;
    }
}

std::uint16_t DecodeShiftJisGlyph(std::uint16_t token) {
    const std::uint8_t lead = static_cast<std::uint8_t>(token >> 8U);
    const std::uint8_t trail = static_cast<std::uint8_t>(token);
    std::uint8_t row = static_cast<std::uint8_t>(2U * (lead < 0xe0U ? lead : lead - 0x40U) + 0x1fU);
    std::uint8_t column = static_cast<std::uint8_t>((trail >= 0x80U ? trail - 1U : trail) - 0x1fU);
    if (column >= 0x7fU) {
        column = static_cast<std::uint8_t>(column - 0x5eU);
        ++row;
    }
    return static_cast<std::uint16_t>((static_cast<std::uint16_t>(row) << 8U) | column);
}

std::uint16_t DecodeDialogueGlyph(const ScntDialogueTokenReader &reader) {
    return reader.encodingMode != 0 ? static_cast<std::uint16_t>(reader.token & 0x7f7fU) : DecodeShiftJisGlyph(reader.token);
}

constexpr std::array<std::array<std::uint16_t, 6>, 7> kValidDialogueTokenBuckets{{
    {{0x2139, 0x2465, 0x2529, 0x2576, 0, 0}},
    {{0x2427, 0x2443, 0x2523, 0, 0, 0}},
    {{0x2421, 0x2467, 0x2563, 0x2126, 0, 0}},
    {{0x2429, 0x2525, 0x213c, 0, 0, 0}},
    {{0x2423, 0x2565, 0x2144, 0x213d, 0, 0}},
    {{0x2122, 0x2463, 0x2527, 0x2543, 0x2129, 0}},
    {{0x2123, 0x2425, 0x2521, 0x2567, 0x212a, 0}},
}};

bool IsValidDialogueToken(std::uint16_t glyph) {
    const auto &bucket = kValidDialogueTokenBuckets[glyph % kValidDialogueTokenBuckets.size()];
    for (const std::uint16_t candidate : bucket) {
        if (candidate == 0)
            return false;
        if (candidate == glyph)
            return true;
    }
    return false;
}

bool AdvanceWindowRow(WindEntry &window) {
    const std::int16_t column = static_cast<std::int16_t>(window.packedCursor);
    std::int16_t row = static_cast<std::int16_t>(window.packedCursor >> 16U);
    window.rowEndColumns[row] = static_cast<std::uint8_t>(column);
    ++row;
    if (row > window.cellHeightPixels)
        row = window.cellHeightPixels;
    window.packedCursor = static_cast<std::uint32_t>(static_cast<std::uint16_t>(row)) << 16U;
    if (window.callback)
        window.callback(&window, 3, window.callbackContext);
    return row < window.cellHeightPixels;
}

std::uint32_t WriteWindowCell(WindEntry &window, std::uint8_t attribute, std::uint8_t glyph) {
    if ((window.flags & 0x0cU) != 0)
        return 0x8001U;
    std::int16_t column = static_cast<std::int16_t>(window.packedCursor);
    std::int16_t row = static_cast<std::int16_t>(window.packedCursor >> 16U);
    if (row >= window.cellHeightPixels)
        return 0x8000U;
    while ((attribute & 1U) != 0 && column > window.cellWidthPixels - 2) {
        if (!AdvanceWindowRow(window))
            return 0x8000U;
        column = 0;
        row = static_cast<std::int16_t>(window.packedCursor >> 16U);
    }
    const std::int32_t index = row * window.cellWidthPixels + column;
    window.cellBufferA[index] = attribute | 0x80U;
    window.cellBufferB[index] = glyph;
    std::int32_t dirtyRow = window.rowScrollOrigin + row;
    if (dirtyRow >= window.cellHeightPixels)
        dirtyRow -= window.cellHeightPixels;
    window.rowDirtyFlags[dirtyRow] = static_cast<std::uint8_t>((window.rowDirtyFlags[dirtyRow] & ~8U) | 2U);
    ++column;
    window.packedCursor = (static_cast<std::uint32_t>(static_cast<std::uint16_t>(row)) << 16U) | static_cast<std::uint16_t>(column);
    if (column >= window.cellWidthPixels)
        AdvanceWindowRow(window);
    return 0;
}

constexpr std::array<std::uint8_t, 86> kJisPunctuationToKana{{0x21, 0x20, 0x2a, 0x21, 0x48, 0x22, 0x49, 0x22, 0x74, 0x23, 0x70, 0x24, 0x73, 0x25, 0x75, 0x26,
    0x47, 0x27, 0x4a, 0x28, 0x4b, 0x29, 0x76, 0x2a, 0x5c, 0x2b, 0x24, 0x2c, 0x5d, 0x2d, 0x3d, 0x2d, 0x25, 0x2e, 0x3f, 0x2f, 0x27, 0x3a, 0x28, 0x3b, 0x63, 0x3c,
    0x61, 0x3d, 0x64, 0x3e, 0x29, 0x3f, 0x77, 0x40, 0x4e, 0x5b, 0x6f, 0x5c, 0x4f, 0x5d, 0x30, 0x5e, 0x32, 0x5f, 0x46, 0x60, 0x50, 0x7b, 0x43, 0x7c, 0x51, 0x7d,
    0x31, 0x7e, 0x23, 0xa1, 0x56, 0xa2, 0x57, 0xa3, 0x22, 0xa4, 0x26, 0xa5, 0x3c, 0xb0, 0x2b, 0xde, 0x2c, 0xdf}};
constexpr std::array<std::uint8_t, 86> kJisKatakanaToKana{{0xa7, 0xb1, 0xa8, 0xb2, 0xa9, 0xb3, 0xaa, 0xb4, 0xab, 0xb5, 0xb6, 0x81, 0xb7, 0x82, 0xb8, 0x83, 0xb9,
    0x84, 0xba, 0x85, 0xbb, 0x86, 0xbc, 0x87, 0xbd, 0x88, 0xbe, 0x89, 0xbf, 0x8a, 0xc0, 0x8b, 0xc1, 0x8c, 0xaf, 0xc2, 0x8d, 0xc3, 0x8e, 0xc4, 0x8f, 0xc5, 0xc6,
    0xc7, 0xc8, 0xc9, 0xca, 0x90, 0x91, 0xcb, 0x92, 0x93, 0xcc, 0x94, 0x95, 0xcd, 0x96, 0x97, 0xce, 0x98, 0x99, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xac, 0xd4, 0xad,
    0xd5, 0xae, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0x6e, 0xdc, 0x70, 0x71, 0xa6, 0xdd, 0x9a, 0x9b, 0x9c}};

std::uint16_t ConvertJisGlyphToKana(std::uint16_t token) {
    if (token < 0x100U)
        return token;
    const std::uint16_t glyph = token & 0x7f7fU;
    if (static_cast<std::uint16_t>(glyph - 0x2330U) <= 9U || static_cast<std::uint16_t>(glyph - 0x2341U) <= 25U ||
        static_cast<std::uint16_t>(glyph - 0x2361U) <= 25U)
        return static_cast<std::uint16_t>(glyph - 0x2300U);
    if (static_cast<std::uint16_t>(glyph - 0x2521U) <= 85U) {
        const std::uint8_t kana = kJisKatakanaToKana[glyph - 0x2521U];
        return static_cast<std::uint16_t>(kana | ((((kana - 0x6eU) & 0xfcU) != 0 || kana == 0x6fU) ? 0x8080U : 0xa580U));
    }
    for (std::size_t index = 0; index < kJisPunctuationToKana.size(); index += 2) {
        if (glyph == (kJisPunctuationToKana[index] | 0x2100U))
            return kJisPunctuationToKana[index + 1];
    }
    return glyph | 0x8080U;
}

std::uint32_t WriteDialogueGlyph(WindEntry &window, const ScntDialogueTokenReader &reader) {
    std::uint16_t glyph = reader.token;
    std::int32_t encodingMode = reader.encodingMode;
    if (encodingMode == 2) {
        glyph = ConvertJisGlyphToKana(glyph);
        encodingMode = 1;
    }
    if (glyph < 0x100U) {
        if (glyph == '\n') {
            if ((window.packedCursor >> 16U) == 0 || static_cast<std::uint16_t>(window.packedCursor) != 0)
                AdvanceWindowRow(window);
            return 1;
        }
        return WriteWindowCell(window, 0, static_cast<std::uint8_t>(glyph));
    }
    glyph = encodingMode == 1 ? static_cast<std::uint16_t>(glyph & 0x7f7fU) : DecodeShiftJisGlyph(glyph);
    const std::uint32_t result = WriteWindowCell(window, 1, static_cast<std::uint8_t>(glyph >> 8U));
    return result == 0 ? WriteWindowCell(window, 2, static_cast<std::uint8_t>(glyph)) : result;
}

void ConsumeDialogueTokens(ScntDialogueWork &dialogue, WindEntry &window) {
    ScntDialogueTokenReader &reader = dialogue.reader.tokenReader;
    std::uint16_t escape = 0;
    do {
        bool writeGlyph = true;
        if (reader.token != 0) {
            const std::int32_t column = static_cast<std::int16_t>(window.packedCursor);
            const std::int32_t wrapColumn = dialogue.reader.columns - 2;
            const std::uint16_t glyph = DecodeDialogueGlyph(reader);
            if (column > wrapColumn || (column == wrapColumn && !(reader.tokenKind == 2 && IsValidDialogueToken(glyph))))
                AdvanceWindowRow(window);
            if (reader.tokenKind == 2) {
                if (glyph == 0x2175U) {
                    if ((window.packedCursor >> 16U) == 0 || static_cast<std::uint16_t>(window.packedCursor) != 0)
                        AdvanceWindowRow(window);
                    writeGlyph = false;
                } else if (glyph == 0x2156U || glyph == 0x2157U) {
                    writeGlyph = false;
                } else if (glyph == 0x217cU) {
                    writeGlyph = escape == 0x217cU;
                    escape = writeGlyph ? 0 : glyph;
                } else if (glyph == 0x4044U || glyph == 0x4056U) {
                    const bool escaped = escape == 0x217cU;
                    escape = 0;
                    if (escaped) {
                        window.waitFlag = glyph == 0x4044U ? 1 : 0;
                        if (g_applicationSettings.savedDialogueMode <= 1) {
                            dialogue.reader.flags |= 1U;
                            return;
                        }
                        writeGlyph = false;
                    }
                } else {
                    writeGlyph = escape != 0x217cU;
                    escape = 0;
                }
            }
        }
        if (writeGlyph && (WriteDialogueGlyph(window, reader) & 0x8000U) != 0)
            return;
    } while (AdvanceDialogueToken(reader));
    dialogue.reader.flags |= 1U;
}

void ProcessDialogueRelated(WindEntry *window, std::int64_t reason, void *context) {
    ScntDialogueWork &dialogue = *static_cast<ScntDialogueWork *>(context);
    if (reason != 0)
        dialogue.mode = static_cast<std::int32_t>(reason);
    if (!dialogue.active || (dialogue.reader.flags & 0x80U) == 0)
        return;
    if (reason == 7) {
        if ((dialogue.reader.flags & 1U) != 0) {
            dialogue.reader.flags |= 2U;
        } else {
            ResetDialogueWindow(*window);
            ConsumeDialogueTokens(dialogue, *window);
            window->flags |= 0x24U;
        }
    } else if (reason == 6) {
        if ((dialogue.reader.flags & 8U) != 0) {
            window->flags |= 0x30U;
            dialogue.reader.flags ^= 8U;
        } else {
            dialogue.reader.flags |= 4U;
        }
    } else if (reason == 0) {
        const std::int32_t previousDelay = dialogue.surfaceAllocationDelay;
        if (previousDelay > 0 && !dialogue.surfaceAllocated) {
            dialogue.surfaceAllocationDelay = previousDelay + 1;
            if (previousDelay > 4) {
                SurfaceAllocation_Wrapper(reinterpret_cast<ShenmueSurfaceOwner *>(window));
                dialogue.surfaceAllocationDelay = 0;
                dialogue.surfaceAllocated = 1;
            }
        }
    }
}

bool HasBlockingVoiceOwner() {
    if (!g_pCVWK)
        return false;
    for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (slot.callback && slot.stateOrOwner >= 0)
            return true;
    }
    return false;
}

constexpr std::uint32_t kFreeTalkNoParticipant = 0xffffffffU;
constexpr std::uint32_t kRyoName = SHEN_NAME(AKIR);
constexpr std::uint32_t kMtacName = SHEN_NAME(MTAC);
constexpr std::uint8_t kHeadBaseNode = 4;
constexpr std::uint8_t kLeftKneeAuxNode = 9;
constexpr std::uint8_t kLeftFootEndNode = 12;
constexpr std::uint8_t kChestUpperAuxNode = 20;
constexpr std::uint8_t kRightHandEndNode = 35;
constexpr std::uint16_t kKeepLetterAnimation = 35;
constexpr std::uint16_t kSendLetterAnimation = 830;

Character *FindFreeTalkCharacter(std::uint32_t name) {
    HLib::Task *const scenarioTask = CurrentScenarioTask();
    return scenarioTask ? CHID_Find(scenarioTask, name) : nullptr;
}

Vector3 SubtractVector(const Vector3 &lhs, const Vector3 &rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

float VectorLength(const Vector3 &value) {
    return std::sqrt((value.y * value.y + value.x * value.x) + value.z * value.z);
}

Vector3 WorldToFreeTalkYawFrame(const Vector3 &origin, std::uint16_t yaw, const Vector3 &point) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), sin_fixed16(fixed16_cosine_angle(yaw)));
    Matrix4x4 inverse = workTransformMatrix;
    Matrix4x4::InvertInPlace(&inverse);
    Matrix4x4::Pop();
    Matrix4x4::SetActiveTransformToPrePass();
    workTransformMatrix = inverse;
    Vector3 result{};
    Matrix4x4::TransformPoint(point, result);
    Matrix4x4::Pop();
    return result;
}


std::uint16_t FreeTalkAcos16(float value) {
    if (!(std::fabs(value) <= 1.0F))
        return value > 0.0F ? 0U : 0x8000U;
    return static_cast<std::uint16_t>(16384 - asin_fixed16(value));
}

Vector3 FreeTalkNodePosition(Character *character, std::uint8_t nodeId) {
    Vector3 position{};
    mtwkGetCharaBonePosition(character, nodeId, &position);
    return position;
}

void ClearCharaFaceLookAt(std::uint32_t name) {
    Character *const character = FindFreeTalkCharacter(name);
    if (!character)
        return;
    if (MotionManagerWork *const manager = MOTM_GetWork(character))
        manager->motion.motionControlLock = 0;
    SetCharaLookAtPosition(character, -13, nullptr, 0);
    constexpr Vector3 kZero{};
    FACE_SetLookTarget(character, 0, &kZero, 8);
}

void RestoreTaggedPayloads(std::uint32_t name) {
    CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(name);
    if (!npc)
        return;
    for (CycleManager::TaggedPayload *payload = npc->cycle->taggedPayloadList; payload; payload = payload->next) {
        if (payload->mode <= 3U && payload->retainAfterFade) {
            payload->retainAfterFade = 0;
            payload->lifetime = 2;
        }
    }
}

void SuspendTaggedPayloads(CycleManager::NpcRecord &npc) {
    for (CycleManager::TaggedPayload *payload = npc.cycle->taggedPayloadList; payload; payload = payload->next) {
        if (payload->mode <= 3U && !payload->retainAfterFade) {
            payload->retainAfterFade = 1;
            payload->lifetime = 7683;
        }
    }
}

void ResetFreeTalkMotionState() {
    g_freetalk_timer_complete_flag = 0;
    g_freeTalkConstraintsDisabled = 0;
    g_freeTalkCurrentCharaIsContextOperand = 0;
    SCEN_ClearFreeTalkRuntimeFlag();
    g_freeTalkRyoConstrained = 0;
    g_freeTalkMotionResetPending = 0;
    bRyoFacingDirectionFlag = 0;
    g_freeTalkMotionResetCleanupPending = 0;
    g_has_free_talk_time_limit = 0;
    g_freetalk_flag_0 = 0;
    g_freeTalkDelayedFlagSetPending = 0;
    g_faceFlag = 0;
    g_freeTalkFacePairStarted = 0;
    g_freeTalkFaceBaselinesCaptured = 0;
    g_freeTalkFaceBlendSuppressLook = 0;
    g_freeTalkFaceBaselineUpperY = 0.0F;
    g_freeTalkFaceBaselineHandY = 0.0F;
    g_tmp_chara_rot_y = 0;
    g_prevFreeTalkPartner[0] = 0;
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        g_talk_name_tbl[index] = kFreeTalkNoParticipant;
        g_freeTalkMotionIdBySlot[index] = 0xffffffffU;
        g_freeTalkRelativePosModeBySlot[index] = 0;
        g_freeTalkFacingBySlot[index] = 0;
        g_freeTalkPairFacing[index] = 0;
        g_freeTalkParticipantRestorePending[index] = 0;
        g_freeTalkConstraintCompletionPending[index] = 0;
        g_freeTalkFaceBlendUpdatePending[index] = 0;
        g_freeTalkPairSetupPending[index] = 0;
    }
}

void CleanupFreeTalkOwnedState() {
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        const std::uint32_t name = g_talk_name_tbl[index];
        if (name == kFreeTalkNoParticipant || name == 0)
            continue;
        ClearCharaFaceLookAt(name);
        if (Character *const character = FindFreeTalkCharacter(name))
            charaCancelMoveRequest(character, 0);
        if (g_freeTalkParticipantMobjRestorePending || g_freeTalkConstraintCompletionPending[index]) {
            RestoreTaggedPayloads(name);
        }
    }
    ClearCharaFaceLookAt(kRyoName);
    if (Character *const ryo = FindFreeTalkCharacter(kRyoName))
        charaCancelMoveRequest(ryo, 0);
    if (g_pendingFreeTalkMotionResetCharaId) {
        if (MotionManagerWork *const manager = MOTM_GetWork(FindFreeTalkCharacter(g_pendingFreeTalkMotionResetCharaId))) {
            MOTN_ClearSecondaryMotion(manager->motion);
        }
    }
}

void ResetFreeTalkState() {
    CleanupFreeTalkOwnedState();
    ResetFreeTalkMotionState();
}

bool UpdateLetterExchange() {
    if (g_freeTalkDelayedFlagSetPending) {
        if (++g_freeTalkLetterExchangeFrameCounter != 45)
            return false;
        g_freeTalkDelayedFlagSetPending = 0;
        g_freeTalkLetterExchangeFrameCounter = 0;
        Character *const ryo = FindFreeTalkCharacter(kRyoName);
        Character *const letter = FindFreeTalkCharacter(SHEN_NAME(SMAL));
        if (letter && ryo) {
            SetCharacterVisibility(letter, true);
            letter->motionFlags |= 4U;
            constexpr Vector3 kLetterOffset{0.15F, 0.0F, -0.15F};
            constexpr Vector3i kLetterRotation{0xE38F, 0x9C72, 0xEE39};
            fixoSetCharaPos(letter, ryo, 18, &kLetterOffset, &kLetterRotation);
        }
    }
    if (g_freetalk_flag_0) {
        if (++g_freeTalkLetterExchangeFrameCounter != 40)
            return false;
        g_freetalk_flag_0 = 0;
        g_freeTalkLetterExchangeFrameCounter = 0;
        if (Character *const letter = FindFreeTalkCharacter(SHEN_NAME(SMAL))) {
            SetCharacterVisibility(letter, false);
            letter->motionFlags &= static_cast<std::uint8_t>(~4U);
        }
    }
    return true;
}


bool QueueFreeTalkAcknowledgement(const FreeTalkEvent &event, std::int32_t command) {
    const std::int32_t outbound = AcquireScenarioRelationQueue(SHEN_NAME(SCNF));
    if (outbound < 0)
        return false;
    const std::array<std::uint32_t, 2> acknowledgement{event.characterName, static_cast<std::uint32_t>(command)};
    return EnqueueScenarioRelationData(static_cast<std::uint32_t>(outbound), acknowledgement.data(), sizeof(acknowledgement)) == 0;
}

void CaptureFreeTalkParticipants(std::uint32_t characterName) {
    SCEN_CopyFreeTalkParticipants(characterName, g_talk_name_tbl.data(), g_talk_name_tbl.size());
}

void CaptureFreeTalkBaselines() {
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        Character *const character = FindFreeTalkCharacter(g_talk_name_tbl[index]);
        MotionManagerWork *const manager = MOTM_GetWork(character);
        if (!character || !manager)
            continue;
        g_freeTalkMotionIdBySlot[index] = manager->motion.currentAnimId;
        g_freeTalkFacingBySlot[index] = character->entity.node.rotation.y;
        g_freeTalkRelativePosModeBySlot[index] = static_cast<std::int32_t>(static_cast<std::uint16_t>(manager->motion.extractedYaw) |
            (static_cast<std::uint32_t>(static_cast<std::uint16_t>(manager->motion.extractedYawAux)) << 16U));
    }

    if (Character *const current = FindFreeTalkCharacter(g_freeTalkCurrentCharaId)) {
        g_freeTalkLookTargetPos = FreeTalkNodePosition(current, kHeadBaseNode);
        g_freeTalkFaceBaselineUpperY = FreeTalkNodePosition(current, kChestUpperAuxNode).y;
        g_freeTalkFaceBaselineHandY = FreeTalkNodePosition(current, kRightHandEndNode).y;
    }
    if (Character *const ryo = FindFreeTalkCharacter(kRyoName))
        g_freeTalkFaceBlendPoint = FreeTalkNodePosition(ryo, kHeadBaseNode);
}

void SubmitFreeTalkConstraint(Character *character, const Vector3 &position, std::int32_t facing, std::uint32_t animationId) {
    if (character)
        freeTalkConv_SetMoveAndFacingConstraint(*character, position, facing, static_cast<std::int32_t>(animationId | 0x140000U), 0.0F, 1.0F, 0.0F, false);
}

void ConstrainRyoForFreeTalk(const FreeTalkEvent &event) {
    Character *const ryo = FindFreeTalkCharacter(kRyoName);
    if (!ryo)
        return;
    g_freeTalkRyoConstrained = 1;
    if (!g_freeTalkCurrentCharaIsContextOperand) {
        const Vector3 position = ryo->entity.node.position;
        freeTalkConv_SetMoveAndFacingConstraint(*ryo, position, charaGetWorldYaw(ryo), 0xf0387, 0.0F, 1.0F, 0.0F, false);
    }
    QueueFreeTalkAcknowledgement(event, -1);
}

void BeginFreeTalkMotionReset(std::uint32_t name) {
    Character *const character = FindFreeTalkCharacter(name);
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    mhndApplyCharaHandPose(character, g_freeTalkPendingAnimState, 2, 0);
    if (manager) {
        MotionInformationWork &motion = manager->motion;
        motion.motionControlLock = 0;
        motion.secondaryBlendFrames = 15;
        motion.queuedSecondaryStartFrame = 0.0F;
        motion.queuedSecondaryEndFrame = 15.0F;
        if (!motion.secondaryBlendCountdown) {
            motion.secondaryBlendFramesPrevious = 15;
            motion.secondaryBlendCountdown = static_cast<std::uint8_t>(static_cast<std::int32_t>(motion.secondaryBlendFrameScale * 15.0F));
        }
    }
    g_freeTalkMotionResetCleanupPending = 1;
    g_pendingFreeTalkMotionResetCharaId = name;
    g_freeTalkMotionResetPending = 0;
}

void UpdateFreeTalkConstraints(std::uint32_t name) {
    if (g_freeTalkConstraintsDisabled || name == kMtacName)
        return;

    if (g_freeTalkMotionResetPending) {
        g_freeTalkMotionResetPending = 0;
        BeginFreeTalkMotionReset(name);
    }

    ClearCharaFaceLookAt(name);
    ClearCharaFaceLookAt(kRyoName);
    g_freeTalkFacePairStarted = 0;
    Character *const character = FindFreeTalkCharacter(name);
    if (!character)
        return;

    Vector3 position = character->entity.node.position;
    position.x = static_cast<float>(static_cast<double>(position.x) - 0.001);
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        if (g_talk_name_tbl[index] != name)
            continue;

        if (!g_freeTalkParticipantRestorePending[index])
            return;

        g_freeTalkConstraintCharaByTalkSlot[index] = name;
        g_freeTalkConstraintCompletionPending[index] = 1;
        SubmitFreeTalkConstraint(character, position, g_freeTalkFacingBySlot[index], g_freeTalkMotionIdBySlot[index]);
        g_freeTalkParticipantRestorePending[index] = 0;
    }
}

void AimRyoAtCharacterHead(std::uint32_t targetName) {
    if (g_freeTalkMotionResetPending)
        return;
    Character *const target = FindFreeTalkCharacter(targetName);
    Character *const ryo = FindFreeTalkCharacter(kRyoName);
    if (!target || !ryo)
        return;
    const Vector3 head = FreeTalkNodePosition(target, kHeadBaseNode);
    if (MotionManagerWork *const manager = MOTM_GetWork(ryo))
        manager->motion.motionControlLock = 0;
    SetCharaLookAtPosition(ryo, 13, &head, 2);
    FACE_SetLookTarget(ryo, 2, &head, 8);
}

void UpdatePairLookAtTargets() {
    Character *const headSource = FindFreeTalkCharacter(g_freeTalkPairHeadSource);
    Character *const lookActor = FindFreeTalkCharacter(g_freeTalkPairLookActor);
    if (!headSource || !lookActor || g_freeTalkPairLookActor == kRyoName) {
        return;
    }
    const Vector3 head = FreeTalkNodePosition(headSource, kHeadBaseNode);
    if (MotionManagerWork *const manager = MOTM_GetWork(lookActor))
        manager->motion.motionControlLock = 0;
    SetCharaLookAtPosition(lookActor, 13, &head, 0);
    AimRyoAtCharacterHead(g_freeTalkPairLookActor);
}

void ComputePairFacing(std::uint32_t characterName, std::uint32_t otherName, std::size_t pairIndex) {
    if (characterName == kRyoName || pairIndex >= g_freeTalkPairFacing.size()) {
        return;
    }
    Character *const other = FindFreeTalkCharacter(otherName);
    if (!other)
        return;
    Character *const character = FindFreeTalkCharacter(characterName);
    if (!character)
        return;
    if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
        manager->motion.motionControlLock = 0;
        manager->motion.activeBasePoseAnimIndex = 0;
    }

    std::int32_t angle = 0;
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        if (g_talk_name_tbl[index] == characterName)
            angle = g_freeTalkRelativePosModeBySlot[index];
    }

    const Vector3 relative = WorldToFreeTalkYawFrame(character->entity.node.position, static_cast<std::uint16_t>(angle), other->entity.node.position);
    const float length = VectorLength(relative);
    const float cosine = ((relative.y * 0.0F + relative.x * 0.0F) + relative.z) / length;
    std::uint32_t facing = static_cast<std::uint32_t>(angle);
    if (!(cosine >= 1.0F || cosine <= 0.0F)) {
        const std::uint16_t offset = FreeTalkAcos16(1.0F - cosine);
        if (relative.x > 0.0F)
            facing -= offset;
        else
            facing += offset;
    }
    g_freeTalkPairFacing[pairIndex] = static_cast<std::int32_t>(facing);
}

void SetupFreeTalkFacePair(std::size_t index) {
    if (index >= g_freeTalkPairHeadSources.size())
        return;
    const std::uint32_t lookName = g_freeTalkPairLookActors[index];
    Character *const lookActor = FindFreeTalkCharacter(lookName);
    if (!lookActor)
        return;
    CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(lookName);
    std::int32_t type = 0;
    if (npc && npc->programList == CycleProgramList_SimSet) {
        if (npc->cycle->currentOpcode == CycleOpcode_InitTimedPath)
            type = g_bGlobalFlag ? 1 : -1;
        else if (npc->cycle->currentOpcode == CycleOpcode_StartLinkedCycle)
            type = npc->cycle->moveObjSubOpcode == CycleOpcode_WalkPath3D ? 1 : -2;
        else
            type = npc->cycle->currentOpcode == CycleOpcode_DriveForkliftPath2D ? -3 : 1;
    }

    if (!npc || (type != 1 && type != -2)) {
        constexpr Vector3 kZero{};
        FACE_SetLookTarget(lookActor, 0, &kZero, 1);
        g_freeTalkPairSetupPending[index] = 0;
        bRyoFacingDirectionFlag = 1;
        g_freeTalkFaceBlendUpdatePending[index] = 1;
        g_freeTalkFaceBlendFrame[index] = 0;
        return;
    }
    if (!npc->cycle->xformDirty || charaHasBlockingTPMRequest(lookActor))
        return;

    if (!g_freeTalkFaceBaselinesCaptured) {
        CaptureFreeTalkBaselines();
        g_freeTalkFaceBaselinesCaptured = 1;
    }
    if (!g_faceFlag && !g_freeTalkFacePairStarted) {
        g_faceFlag = 1;
        g_freeTalkFacePairStarted = 1;
        SecondChara = lookName;
    }
    g_freeTalkFaceBlendUpdatePending[index] = 1;
    ComputePairFacing(lookName, g_freeTalkPairHeadSources[index], index);

    std::size_t talkSlot = g_talk_name_tbl.size();
    for (std::size_t candidate = 0; candidate < g_talk_name_tbl.size(); ++candidate) {
        if (g_talk_name_tbl[candidate] == lookName) {
            talkSlot = candidate;
            break;
        }
    }
    const std::uint32_t activeAnimation =
        npc->cycle->currentOpcode == CycleOpcode_DriveForkliftPath2D ? static_cast<std::uint16_t>(npc->cycle->activeAnimId) : npc->cycle->selectedMotionToken;
    const std::uint32_t expectedAnimation = talkSlot < g_freeTalkMotionIdBySlot.size() ? g_freeTalkMotionIdBySlot[talkSlot] : 0xffffffffU;
    if (activeAnimation == 0xffffffffU || activeAnimation == expectedAnimation) {
        constexpr Vector3 kZero{};
        FACE_SetLookTarget(lookActor, 0, &kZero, 1);
        g_freeTalkPairSetupPending[index] = 0;
        return;
    }

    const std::uint16_t opcode = static_cast<std::uint16_t>(npc->cycle->currentOpcode);
    constexpr std::uint64_t kResumePositionOpcodes = 0x94108c0504000010ULL;
    const bool useResumePosition = (opcode <= 60U && ((kResumePositionOpcodes >> opcode) & 1U) != 0) ||
        (opcode == CycleOpcode_StartLinkedCycle && static_cast<std::uint32_t>(npc->cycle->moveObjPhase) - CycleMoveObjPhase_ProcessSubcommands <= 2U);
    Vector3 position = useResumePosition ? npc->cycle->xmptResumePos : lookActor->entity.node.position;
    position.x = static_cast<float>(static_cast<double>(position.x) + 0.001);
    if (type == -2)
        g_tmp_chara_rot_y = lookActor->entity.node.rotation.y;
    const std::uint32_t animation = talkSlot < g_freeTalkMotionIdBySlot.size() ? g_freeTalkMotionIdBySlot[talkSlot] : 0xffffffffU;
    SubmitFreeTalkConstraint(lookActor, position, g_freeTalkPairFacing[index], animation);
    if (type == -2) {
        g_freeTalkParticipantMobjRestorePending = 1;
        SuspendTaggedPayloads(*npc);
    }

    Character *const headSource = FindFreeTalkCharacter(g_freeTalkPairHeadSources[index]);
    if (headSource) {
        const Vector3 head = FreeTalkNodePosition(headSource, kHeadBaseNode);
        if (MotionManagerWork *const manager = MOTM_GetWork(lookActor)) {
            manager->motion.motionControlLock = 0;
        }
        SetCharaLookAtPosition(lookActor, 13, &head, 1);
        FACE_SetLookTarget(lookActor, 1, &head, 8);
    }
    if (talkSlot < g_freeTalkParticipantRestorePending.size())
        g_freeTalkParticipantRestorePending[talkSlot] = 1;
    g_freeTalkPairSetupPending[index] = 0;
}

void UpdateFaceBlendSlot(std::size_t index) {
    if (index >= g_freeTalkFaceBlendFrame.size())
        return;
    std::uint8_t &frame = g_freeTalkFaceBlendFrame[index];
    if (frame == 20) {
        g_freeTalkFaceBlendUpdatePending[index] = 0;
        frame = 0;
        if (!g_freeTalkFaceBlendSuppressLook)
            return;
        Character *const headSource = FindFreeTalkCharacter(g_freeTalkPairHeadSources[index]);
        if (!headSource)
            return;
        const Vector3 head = FreeTalkNodePosition(headSource, kHeadBaseNode);
        Character *const lookActor = FindFreeTalkCharacter(g_freeTalkPairLookActors[index]);
        if (lookActor)
            SetCharaLookAtPosition(lookActor, 13, &head, 1);
        return;
    }
    ++frame;
    Character *const headSource = FindFreeTalkCharacter(g_freeTalkPairHeadSources[index]);
    if (!headSource)
        return;
    const Vector3 head = FreeTalkNodePosition(headSource, kHeadBaseNode);
    const float amount = static_cast<float>(static_cast<std::int8_t>(frame)) / 20.0F;
    Vector3 &point = g_freeTalkFaceBlendPoint;
    point.x -= (point.x - head.x) * amount;
    point.y -= (point.y - head.y) * amount;
    point.z -= (point.z - head.z) * amount;
    Character *const lookActor = FindFreeTalkCharacter(g_freeTalkPairLookActors[index]);
    if (!lookActor)
        return;
    if (MotionManagerWork *const manager = MOTM_GetWork(lookActor))
        manager->motion.motionControlLock = 0;
    if (!g_freeTalkFaceBlendSuppressLook)
        SetCharaLookAtPosition(lookActor, 13, &point, 1);
}

void ReorientRyoTowardTarget(const Vector3 &target, bool crossedHemisphere) {
    Character *const ryo = FindFreeTalkCharacter(kRyoName);
    if (!ryo)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(ryo);
    if (manager)
        manager->motion.motionControlLock = 0;
    Vector3 position = GetPositionForEntity(ryo->entity.node);
    const std::int32_t currentYaw = ryo->entity.node.rotation.y;
    g_freeTalkRyoFacingYaw = currentYaw;
    const Vector3 localTarget = WorldToFreeTalkYawFrame(position, static_cast<std::uint16_t>(currentYaw), target);
    const float cosine = ((localTarget.y * 0.0F + localTarget.x * 0.0F) - localTarget.z) / VectorLength(localTarget);
    if (crossedHemisphere)
        g_has_free_talk_time_limit = 0;
    const std::int32_t offset = FreeTalkAcos16(cosine);
    const std::uint32_t adjustment = static_cast<std::uint32_t>(crossedHemisphere ? offset / 2 : offset);
    const std::uint32_t facing =
        localTarget.x > 0.0F ? static_cast<std::uint32_t>(currentYaw) - adjustment : static_cast<std::uint32_t>(currentYaw) + adjustment;
    SetCharaLookAtPosition(ryo, 13, &target, 2);
    FACE_SetLookTarget(ryo, 2, &target, 8);
    position.x = static_cast<float>(static_cast<double>(position.x) + 0.001);
    if (manager)
        SubmitFreeTalkConstraint(ryo, position, static_cast<std::int32_t>(facing), manager->motion.currentAnimId);
}

void UpdateFaceTargetBlend() {
    if (g_freeTalkBlendFrames == -1) {
        if (g_freeTalkFaceBlendMotionGate != 0) {
            if (Character *const ryo = FindFreeTalkCharacter(kRyoName)) {
                MotionManagerWork *const manager = MOTM_GetWork(ryo);
                if (!manager)
                    return;
                if ((manager->motion.rootAngleFlags & 0x40U) != 0)
                    g_freeTalkFaceBlendMotionGate = 0;
            }
        }
        std::int8_t blendFrames = 60;
        if (CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(SecondChara)) {
            const float threshold = npc->cycle->moveConfig->anchorGroupCode == 4U || npc->cycle->moveConfig->anchorGroupCode == 8U ? 0.5F : 0.7F;
            if (threshold > g_freeTalkFaceBaselineUpperY - g_freeTalkFaceBaselineHandY)
                g_freeTalkFaceBlendSuppressLook = 1;
            else
                blendFrames = 20;
        }
        g_freeTalkBlendFrames = blendFrames;
    }
    if (g_prevFreeTalkPartner[0] && g_prevFreeTalkPartner[0] != SecondChara) {
        Character *const ryo = FindFreeTalkCharacter(kRyoName);
        Character *const current = FindFreeTalkCharacter(SecondChara);
        Character *const previous = FindFreeTalkCharacter(g_prevFreeTalkPartner[0]);
        if (ryo && current && previous) {
            const std::uint16_t yaw = static_cast<std::uint16_t>(ryo->entity.node.rotation.y);
            const Vector3 currentLocal = WorldToFreeTalkYawFrame(ryo->entity.node.position, yaw, current->entity.node.position);
            const Vector3 previousLocal = WorldToFreeTalkYawFrame(ryo->entity.node.position, yaw, previous->entity.node.position);
            if (previousLocal.z * currentLocal.z >= 0.0F) {
                g_freeTalkPartnerCrossedHemisphere = 0;
                g_prevFreeTalkPartner[0] = 0;
            } else {
                g_freeTalkPartnerCrossedHemisphere = 1;
            }
        }
    }
    if (static_cast<std::int8_t>(g_freeTalkCurrentProgress) == g_freeTalkBlendFrames) {
        g_faceFlag = 0;
        g_freeTalkCurrentProgress = 0;
        g_freeTalkBlendFrames = -1;
        g_prevFreeTalkPartner[0] = SecondChara;
        return;
    }
    if (g_freeTalkPartnerCrossedHemisphere) {
        if (Character *const target = FindFreeTalkCharacter(SecondChara)) {
            const Vector3 head = FreeTalkNodePosition(target, kHeadBaseNode);
            ReorientRyoTowardTarget(head, true);
            g_faceFlag = 0;
            g_freeTalkCurrentProgress = 0;
            g_freeTalkPartnerCrossedHemisphere = 0;
            return;
        }
    }
    ++g_freeTalkCurrentProgress;
    Character *const target = FindFreeTalkCharacter(SecondChara);
    if (!target)
        return;
    const Vector3 head = FreeTalkNodePosition(target, kHeadBaseNode);
    const float amount = static_cast<float>(static_cast<std::int8_t>(g_freeTalkCurrentProgress)) / static_cast<float>(g_freeTalkBlendFrames);
    Vector3 &point = g_freeTalkLookTargetPos;
    point.x -= (point.x - head.x) * amount;
    point.y -= (point.y - head.y) * amount;
    point.z -= (point.z - head.z) * amount;
    Character *const ryo = FindFreeTalkCharacter(kRyoName);
    if (!ryo)
        return;
    if (MotionManagerWork *const manager = MOTM_GetWork(ryo))
        manager->motion.motionControlLock = 0;
    SetCharaLookAtPosition(ryo, 13, &point, 1);
    FACE_SetLookTarget(ryo, 2, &point, 1);
}

void EvaluateTimeLimitAndFaceTarget(std::int32_t command, std::uint32_t characterName) {
    Character *const character = FindFreeTalkCharacter(characterName);
    if (!character)
        return;
    const std::uint32_t locationIndex = static_cast<std::uint32_t>(command) - (command >= 120 ? 120U : 20U);
    const Vector3 *const target = SCEN_GetFreeTalkLocation(characterName, locationIndex);
    if (!target)
        return;
    std::int32_t relativeMode = static_cast<std::uint16_t>(command);
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        if (g_talk_name_tbl[index] == characterName)
            relativeMode = g_freeTalkRelativePosModeBySlot[index];
    }
    g_freeTalkTargetPoint = target;
    Character *const ryo = FindFreeTalkCharacter(kRyoName);
    if (!ryo)
        return;
    const Vector3 ryoPosition = GetPositionForEntity(ryo->entity.node);
    const Vector3 characterPosition = GetPositionForEntity(character->entity.node);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    std::uint16_t extractedYaw = 0;
    if (manager) {
        extractedYaw = static_cast<std::uint16_t>(manager->motion.extractedYaw);
        manager->motion.motionControlLock = 0;
    }
    const Vector3 targetLocal = WorldToFreeTalkYawFrame(characterPosition, extractedYaw, *target);
    const Vector3 foot = FreeTalkNodePosition(character, kLeftFootEndNode);
    const Vector3 knee = FreeTalkNodePosition(character, kLeftKneeAuxNode);
    const Vector3 ryoLocal = WorldToFreeTalkYawFrame(character->entity.node.position, static_cast<std::uint16_t>(relativeMode), ryo->entity.node.position);
    const double proximity = static_cast<double>(VectorLength(SubtractVector(foot, knee))) + 0.7;
    const float distance = VectorLength(SubtractVector(ryoPosition, characterPosition));
    if (proximity > static_cast<double>(distance)) {
        const float dot =
            ((ryoLocal.y * targetLocal.y + ryoLocal.x * targetLocal.x) + ryoLocal.z * targetLocal.z) / (VectorLength(targetLocal) * VectorLength(ryoLocal));
        if (static_cast<double>(dot) > 0.7 && dot < 1.0F) {
            g_has_free_talk_time_limit = 1;
            return;
        }
    }
    FACE_SetLookTarget(character, 2, target, 8);
    SetCharaLookAtPosition(character, 13, target, 0);
    g_has_free_talk_time_limit = 1;
    g_freeTalkMotionResetPending = 1;
    if (command < 120 && manager) {
        g_freeTalkPendingAnimState = motCalculateAnimationToPlay(character, target, false) != 0;
        manager->motion.motionControlLock = 1;
        mhndApplyCharaHandPose(character, g_freeTalkPendingAnimState, 4, 10);
    }
}

void RestoreParticipantsAndEchoMinus8(std::uint32_t name) {
    for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
        if (g_talk_name_tbl[index] == name && g_freeTalkParticipantRestorePending[index]) {
            if (Character *const character = FindFreeTalkCharacter(name)) {
                Vector3 position = character->entity.node.position;
                position.x = static_cast<float>(static_cast<double>(position.x) - 0.001);
                SubmitFreeTalkConstraint(character, position, g_freeTalkFacingBySlot[index], g_freeTalkMotionIdBySlot[index]);
            }
        }
    }

    if (g_freeTalkParticipantMobjRestorePending) {
        RestoreTaggedPayloads(name);
        g_freeTalkParticipantMobjRestorePending = 0;
    }
    if (!g_freeTalkCurrentCharaIsContextOperand) {
        for (std::size_t index = 0; index < g_talk_name_tbl.size(); ++index) {
            if (g_talk_name_tbl[index] != kFreeTalkNoParticipant) {
                ClearCharaFaceLookAt(g_talk_name_tbl[index]);
            }
        }
    }
    ResetFreeTalkMotionState();
    QueueFreeTalkAcknowledgement(FreeTalkEvent{name, -8}, -8);
}

void HandleFaceRecenterCommand(std::uint32_t targetName) {
    Character *const ryo = FindFreeTalkCharacter(kRyoName);
    ClearCharaFaceLookAt(kRyoName);
    if (!ryo)
        return;
    MotionManagerWork *const ryoManager = MOTM_GetWork(ryo);
    if (ryoManager) {
        Vector3 position = ryo->entity.node.position;
        position.x = static_cast<float>(static_cast<double>(position.x) + 0.001);
        SubmitFreeTalkConstraint(ryo, position, g_freeTalkRyoFacingYaw, ryoManager->motion.currentAnimId);
    }
    BeginFreeTalkMotionReset(targetName);
    AimRyoAtCharacterHead(targetName);
    if (ryoManager)
        ryoManager->motion.motionControlLock = 0;

    Character *const target = FindFreeTalkCharacter(targetName);
    if (target) {
        const Vector3 ryoHead = FreeTalkNodePosition(ryo, kHeadBaseNode);
        if (MotionManagerWork *const manager = MOTM_GetWork(target))
            manager->motion.motionControlLock = 0;
        SetCharaLookAtPosition(target, 13, &ryoHead, 2);
        FACE_SetLookTarget(target, 2, &ryoHead, 8);
    }
    g_freetalk_timer_complete_flag = 0;
}

void ryoGiveOrTakeItem(bool give) {
    if (MotionManagerWork *const manager = MOTM_GetWork(FindFreeTalkCharacter(kRyoName)))
        manager->motion.pendingAnimId = give ? kSendLetterAnimation : kKeepLetterAnimation;
}

std::uint32_t TalkNameAt(std::size_t index) {
    return index < g_talk_name_tbl.size() ? g_talk_name_tbl[index] : kFreeTalkNoParticipant;
}

void ProcessFreeTalkEvent(const FreeTalkEvent &event) {
    CaptureFreeTalkParticipants(event.characterName);
    g_freeTalkCurrentCharaId = event.characterName;
    g_freeTalkCurrentCharaIsContextOperand = SCEN_IsFreeTalkCharacterContextOwned(event.characterName);
    switch (event.command) {
    case -8:
        g_FreeTalk_Status = 103;
        talkRestoreParticipantsAndEchoFNCSMinus8(event.characterName);
        break;
    case -7:
    case 20:
        g_FreeTalk_Status = 108;
        break;
    case -6:
        g_FreeTalk_Status = 107;
        break;
    case -5:
        g_FreeTalk_Status = 106;
        break;
    case -4:
        g_FreeTalk_Status = 105;
        break;
    case -3:
        g_FreeTalk_Status = 104;
        break;
    case -2:
        g_freeTalkCurrentCharaId = kFreeTalkNoParticipant;
        g_FreeTalk_Status = 102;
        bRyoFacingDirectionFlag = 0;
        if (g_freeTalkMotionResetPending)
            BeginFreeTalkMotionReset(event.characterName);
        QueueFreeTalkAcknowledgement(event, -2);
        break;
    case -1:
        g_freeTalkConstraintsDisabled = g_freeTalkConstraintsDisabled || SCEN_AreFreeTalkConstraintsDisabled();
        g_FreeTalk_Status = 101;
        ConstrainRyoForFreeTalk(event);
        break;
    default:
        g_FreeTalk_Status = kFreeTalkStatusCommand;
        g_pendingFreeTalkCommandCode = event.command;
        break;
    }

    if (event.command >= 40 && event.command <= 49) {
        const std::size_t index = static_cast<std::size_t>(event.command - 40);
        std::size_t actorB = index / 3 + 1;
        std::size_t actorA = index % 3 + 1;
        if (actorB != index % 3)
            actorA = index % 3;
        if (event.command == 42) {
            actorB = 1;
            actorA = 2;
        }
        g_freeTalkPairHeadSources[index] = TalkNameAt(actorA);
        g_freeTalkPairLookActors[index] = TalkNameAt(actorB);
        g_freeTalkPairHeadSource = g_freeTalkPairHeadSources[index];
        g_freeTalkPairLookActor = g_freeTalkPairLookActors[index];
        g_freeTalkPairSetupPending[index] = 1;
    }
    if (event.command >= 50 && event.command <= 59) {
        const std::size_t index = static_cast<std::size_t>(event.command - 49);
        const std::uint32_t name = TalkNameAt(index);
        if (name != kFreeTalkNoParticipant)
            Freetalk_Motion_UpdateConstraints(name);
    }
    if (event.command >= 140 && event.command <= 149) {
        const std::size_t index = static_cast<std::size_t>(event.command - 139);
        g_faceFlag = 1;
        SecondChara = TalkNameAt(index);
    }
    if ((event.command >= 20 && event.command <= 29) || (event.command >= 120 && event.command <= 129)) {
        FreeTalk_EvaluateTimeLimitAndUpdateFaceTarget(event.command, event.characterName);
    }
    if (event.command == 10) {
        g_freeTalkDelayedFlagSetPending = 1;
        ryoGiveOrTakeItem(true);
    } else if (event.command == 15) {
        g_freetalk_flag_0 = 1;
        ryoGiveOrTakeItem(false);
    } else if (event.command == 13) {
        HandleFaceRecenterCommand(event.characterName);
    }

    QueueFreeTalkAcknowledgement(event, -10);
    g_FreeTalk_Status = kFreeTalkNone;
}

void ProcessFreeTalkFrame(HLib::TaskData *) {
    if (bRyoFacingDirectionFlag)
        UpdatePairLookAtTargets();
    for (std::size_t index = 0; index < g_freeTalkConstraintCompletionPending.size(); ++index) {
        if (!g_freeTalkConstraintCompletionPending[index])
            continue;
        Character *const character = FindFreeTalkCharacter(g_freeTalkConstraintCharaByTalkSlot[index]);
        if (!character || charaHasBlockingTPMRequest(character))
            continue;
        if (g_freeTalkParticipantMobjRestorePending) {
            RestoreTaggedPayloads(g_freeTalkConstraintCharaByTalkSlot[index]);
            g_freeTalkParticipantMobjRestorePending = 0;
        }
        g_freeTalkConstraintCompletionPending[index] = 0;
    }
    if (g_has_free_talk_time_limit && ++g_freetalk_timer_frames == 20) {
        g_has_free_talk_time_limit = 0;
        g_freetalk_timer_frames = 0;
        g_freetalk_timer_complete_flag = 1;
        ReorientRyoTowardTarget(*g_freeTalkTargetPoint, false);
    }
    if (g_faceFlag)
        UpdateFaceTargetBlend();
    for (std::size_t index = 0; index < g_freeTalkFaceBlendUpdatePending.size(); ++index) {
        if (g_freeTalkFaceBlendUpdatePending[index])
            UpdateFaceBlendSlot(index);
    }

    if (!UpdateLetterExchange())
        return;
    if (g_freeTalkMotionResetCleanupPending) {
        if (++g_freeTalkMotionResetCleanupFrameCounter != 30)
            return;
        g_freeTalkMotionResetCleanupPending = 0;
        g_freeTalkMotionResetCleanupFrameCounter = 0;
        if (MotionManagerWork *const manager = MOTM_GetWork(FindFreeTalkCharacter(g_pendingFreeTalkMotionResetCharaId))) {
            MOTN_ClearSecondaryMotion(manager->motion);
        }
        g_freeTalkMotionResetPending = 0;
    }
    for (std::size_t index = 0; index < g_freeTalkPairSetupPending.size(); ++index) {
        if (g_freeTalkPairSetupPending[index])
            SetupFreeTalkFacePair(index);
    }


    const std::int32_t inbound = AcquireScenarioRelationQueue(SHEN_NAME(SCNA));
    std::array<std::uint32_t, 2> relation{};
    if (inbound < 0 || DequeueScenarioRelationData(static_cast<std::uint32_t>(inbound), relation.data(), sizeof(relation)) < 1)
        return;
    ProcessFreeTalkEvent(FreeTalkEvent{relation[0], static_cast<std::int32_t>(relation[1])});
}

bool IsFreetalkTask(const HLib::Task *task) {
    return task && (task->mainTaskCallbackFnPtr == Freetalk_Motion_Main || task->initCallbackFnPtr == Freetalk_Motion_Main);
}

} // namespace
void SCNT_EnsureFreeTalkRuntime() {
    if (g_freeTalkInitialized != 0)
        return;
    ResetFreeTalkMotionState();
    SCEN_ActivateFreeTalkRuntime();
    g_freetalkTask = HLib_EnqueueTaskWithoutParameter(Freetalk_Motion_Main, 8, 0, static_cast<std::int32_t>(SHEN_NAME(SCNT)));
}

void Freetalk_Motion_Main(HLib::TaskData *taskData) {
    ProcessFreeTalkFrame(taskData);
}

void Freetalk_Motion_UpdateConstraints(std::uint32_t characterName) {
    UpdateFreeTalkConstraints(characterName);
}

void talkRestoreParticipantsAndEchoFNCSMinus8(std::uint32_t characterName) {
    RestoreParticipantsAndEchoMinus8(characterName);
}

void FreeTalk_EvaluateTimeLimitAndUpdateFaceTarget(std::int32_t command, std::uint32_t characterName) {
    EvaluateTimeLimitAndFaceTarget(command, characterName);
}

void talkClearCharaFaceLookAt(std::uint32_t characterName) {
    ClearCharaFaceLookAt(characterName);
}

void SCNT_ShowSubtitleText(HLib::Task *scenarioTask, const char *text, std::uint32_t speakerClass) {
    ScntDialogueWork *const dialogue = scenarioTask && scenarioTask->task_data_ptr ?
        &ScenarioField<ScntDialogueWork>(*reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr), kDialogueWorkOffset) :
        nullptr;

    if (dialogue && g_applicationSettings.savedDialogueMode <= 1 && (dialogue->reader.flags & 0x40U) == 0 && speakerClass != 2 && speakerClass != 3) {
        dialogue->reader.flags = 0x10U;
        return;
    }
    if (dialogue)
        dialogue->reader.flags = 2;

    constexpr std::size_t kSubtitleUiIndex = 0x3a;
    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiIndex];
    if (g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kSubtitleUiIndex)) {
        if (!subtitles.isVisible) {
            if (subtitles.render) {
                subtitles.render();
                if (subtitles.tick) {
                    g_waitingForNextTick = 0;
                    subtitles.tick();
                    g_waitingForNextTick = 1;
                }
            } else if (subtitles.initialize) {
                subtitles.initialize();
                if (subtitles.postInit) {
                    g_waitingForNextTick = 0;
                    subtitles.postInit();
                    g_waitingForNextTick = 1;
                }
            }
            subtitles.isVisible = 1;
        }
        subtitles.isHandledExternally = 0;
    }

    ProcessSubtitleText(text);
    ApplySubtitlePresentationClass(speakerClass);
}

void SCNT_ClearSubtitleText() {
    ProcessSubtitleText(nullptr);
}

bool SCNT_IsTimeLimitComplete() {
    return g_freetalk_timer_complete_flag;
}

void SCNT_ResetRuntime() {
    SCNT_ClearSubtitleText();
    ResetFreeTalkState();
    if (IsFreetalkTask(g_freetalkTask))
        HLib_DestroyTask(g_freetalkTask);
    g_freetalkTask = nullptr;
    g_freeTalkBlendFrames = -1;
    g_freeTalkLetterExchangeFrameCounter = 0;
    g_freetalk_timer_frames = 0;
    g_freeTalkCurrentProgress = 0;
    g_freeTalkPartnerCrossedHemisphere = 0;
    g_has_free_talk_time_limit = 0;
    g_freetalk_timer_complete_flag = 0;
    g_freeTalkMotionResetCleanupFrameCounter = 0;
    g_freeTalkCurrentCharaId = 0;
    SecondChara = 0;
    g_freeTalkCurrentCharaIsContextOperand = 0;
    g_talk_name_tbl = {};
    g_freeTalkConstraintCharaByTalkSlot = {};
    g_freeTalkParticipantMobjRestorePending = 0;
    g_freeTalkRyoConstrained = 0;
    g_freeTalkFaceBlendUpdatePending = {};
    g_freeTalkLookTargetPos = {};
    g_freeTalkTargetPoint = nullptr;
    g_freeTalkMotionResetPending = 0;
    g_pendingFreeTalkMotionResetCharaId = 0;
    g_FreeTalk_Status = 0;
    g_pendingFreeTalkCommandCode = 0;
    g_freeTalkDelayedFlagSetPending = 0;
    g_freetalk_flag_0 = 0;
    g_prevFreeTalkPartner = {};
    g_tmp_chara_rot_y = 0;
    g_freeTalkFacePairStarted = 0;
    g_freeTalkFaceBaselineHandY = 0.0F;
    g_freeTalkFaceBaselineUpperY = 0.0F;
    g_freeTalkFaceBaselinesCaptured = 0;
    g_freeTalkConstraintsDisabled = 0;
    bRyoFacingDirectionFlag = 0;
    g_freeTalkMotionResetCleanupPending = 0;
    g_faceFlag = 0;
    g_freeTalkFaceBlendSuppressLook = 0;
    g_freeTalkMotionIdBySlot = {};
    g_freeTalkRelativePosModeBySlot = {};
    g_freeTalkFacingBySlot = {};
    g_freeTalkParticipantRestorePending = {};
    g_freeTalkConstraintCompletionPending = {};
    g_freeTalkPairHeadSource = 0;
    g_freeTalkPairLookActor = 0;
    g_freeTalkPairHeadSources = {};
    g_freeTalkPairLookActors = {};
    g_freeTalkPairSetupPending = {};
    g_freeTalkPairFacing = {};
    g_freeTalkFaceBlendFrame = {};
    g_freeTalkFaceBlendPoint = {};
    g_freeTalkFaceBlendMotionGate = 1;
    g_freeTalkRyoFacingYaw = 0;
    g_freeTalkPendingAnimState = 0;
    g_scnfSubtitleFrames = 0;
}
void SCNT_ResetConversationState() {
    ResetFreeTalkMotionState();
}
HLib::Task *SCNT_GetTask() {
    return IsFreetalkTask(g_freetalkTask) ? g_freetalkTask : nullptr;
}

bool SCNT_IsFreeTalkPairSetupPending(std::size_t index) {
    return index < g_freeTalkPairSetupPending.size() && g_freeTalkPairSetupPending[index] != 0;
}

std::int32_t SCNT_GetAndResetFreeTalkCommand() {
    const std::int32_t command = g_pendingFreeTalkCommandCode;
    g_pendingFreeTalkCommandCode = kFreeTalkNone;
    return command;
}

std::int32_t SCNT_GetFreeTalkStatus() {
    return g_FreeTalk_Status;
}

std::uint32_t SCNT_GetFreeTalkCurrentCharacter() {
    return g_freeTalkCurrentCharaId;
}

std::int64_t SCNT_ShowSCNFSubtitleText(ScenarioContext *context, void *rawParams) {
    if (!rawParams)
        return 0;
    std::int32_t offset = 0;
    std::memcpy(&offset, rawParams, sizeof(offset));
    const char *const text = offset && g_pCurrentTaskData ? reinterpret_cast<const char *>(g_pCurrentTaskData + offset) : nullptr;

    SCNT_ShowSubtitleText(context && context->task ? context->task->task : nullptr, text);
    g_scnfSubtitleFrames = 60;
    return 0;
}

std::int64_t SCNT_WaitSCNFSubtitleText(ScenarioContext *context, void *) {
    const std::uint32_t previous = g_scnfSubtitleFrames--;
    if (previous != 0)
        return 0;
    g_scnfSubtitleFrames = 60;

    ScenarioTaskData *const data = CurrentScenarioData(context);
    WindEntry *const window = data ? ScenarioField<WindEntry *>(*data, kWindowPointerOffset) : nullptr;
    if (!data || !window)
        return 0;
    ScntDialogueWork &dialogue = ScenarioField<ScntDialogueWork>(*data, kDialogueWorkOffset);
    std::uint32_t &flags = dialogue.reader.flags;
    if ((flags & 0x12U) != 0) {
        constexpr std::size_t kSubtitleUiIndex = 0x3a;
        UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiIndex];
        if ((g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kSubtitleUiIndex)) &&
            (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
            subtitles.isVisible = 0;
        }
        return 0x10000;
    }
    if ((flags & 4U) != 0) {
        window->flags |= 0x30U;
        flags ^= 4U;
    } else {
        flags |= 8U;
    }
    return 0;
}

void Script_GetTaskData_0(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return;
    ScenarioTaskData *const data = CurrentScenarioData(context);
    WindEntry *const window = data ? ScenarioField<WindEntry *>(*data, kWindowPointerOffset) : nullptr;
    if (!data || !window) {
        context->stack->accumulator = 0;
        return;
    }

    ScntDialogueWork &dialogue = ScenarioField<ScntDialogueWork>(*data, kDialogueWorkOffset);
    std::uint32_t &flags = dialogue.reader.flags;
    if ((flags & 0x12U) != 0) {
        constexpr std::size_t kSubtitleUiIndex = 0x3a;
        UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiIndex];
        if ((g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kSubtitleUiIndex)) &&
            (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
            subtitles.isVisible = 0;
        }
        context->stack->accumulator = 0;
        return;
    }
    if ((flags & 4U) != 0) {
        window->flags |= 0x30U;
        flags ^= 4U;
    } else {
        flags |= 8U;
    }
    context->stack->accumulator = 0xffffffffU;
}

void Script_ProcessDialogueData(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    ScenarioTaskData *const data = CurrentScenarioData(context);
    WindEntry *const window = data ? ScenarioField<WindEntry *>(*data, kWindowPointerOffset) : nullptr;
    if (!data || !window)
        return;

    const std::int32_t mode = *static_cast<const std::int32_t *>(rawParams);
    ScntDialogueWork &dialogue = ScenarioField<ScntDialogueWork>(*data, kDialogueWorkOffset);
    std::uint32_t &flags = dialogue.reader.flags;
    std::int32_t &active = dialogue.active;

    if (mode == 1) {
        const bool readerFinished = (flags & 1U) != 0;
        active = 0;
        if (readerFinished)
            window->flags &= ~0x20U;
        return;
    }
    if (mode == 2) {
        context->stack->accumulator = static_cast<std::uint32_t>(active);
        return;
    }
    if (mode != 0)
        return;
    active = 1;

    const char *const source = reinterpret_cast<const char *>(reinterpret_cast<std::byte *>(data) + kDialogueSourceOffset);
    if (*source == '\0')
        return;

    if ((flags & 2U) != 0) {
        window->flags |= 0x24U;
        return;
    }
    ProcessDialogueRelated(window, (flags & 8U) != 0 ? 6 : 7, &dialogue);
}

void SCNT_InitializeScenarioDialogue(ScenarioTaskData &data, HLib::Task *windTask) {
    WindEntry *const window = InitializeWindowBorders(windTask);
    ScenarioField<WindEntry *>(data, kWindowPointerOffset) = window;
    if (!window)
        return;
    ScntDialogueWork &dialogue = ScenarioField<ScntDialogueWork>(data, kDialogueWorkOffset);
    dialogue.reader.columns = 46;
    dialogue.reader.config = {2, 0, 0, 0};
    dialogue.reader.flags = 2;
    dialogue.surfaceAllocationDelay = 0;
    dialogue.surfaceAllocated = 1;
    dialogue.active = 1;
    dialogue.mode = 0;
    reinterpret_cast<std::byte *>(&data)[kDialogueSourceOffset] = std::byte{};
    window->callback = ProcessDialogueRelated;
    window->callbackContext = &dialogue;
    ProcessDialogueRelated(window, 5, &dialogue);
}

void SCNT_Enqueue(ScenarioContext *, void *) {
    ResetFreeTalkMotionState();
    SCEN_CreateCharactersForSequence();
    g_freetalkTask = HLib_EnqueueTaskWithoutParameter(Freetalk_Motion_Main, 8, 0, static_cast<std::int32_t>(SHEN_NAME(SCNT)));
}

} // namespace shenmue::s1
