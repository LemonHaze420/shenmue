#include "s1/task/mmtk.hpp"

#include "s1/boot.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/motion_registry.hpp"
#include "s1/mt5_types.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/render_passes.hpp"
#include "s1/subtitles.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/wind.hpp"
#include "s1/ui_runtime.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);
void MT5_PrepassNodeTreeTexturesAndStrips(void **node, std::int32_t *textureIds, std::uint32_t *depth);
void ReadSubtitleGlyphsAndSubs(std::int32_t language);
void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);
extern std::array<std::uint8_t, 128> g_shenmueFlags;

std::uint8_t g_notebookHelpOverrideActive = 0;
std::uint16_t g_notebookLastEntryPage = 0;
std::uint16_t g_notebookPersistenceWord = 0;
CVWKRuntimeSlot *g_notebookJingleSubtitle = nullptr;
std::int32_t g_hudPromptJinguruIconOverrideActive = 0;
std::array<std::byte *, 4> g_notebookMemoTextureChunks{};
std::array<std::int32_t, 4> g_notebookMemoSurfaces{};
std::int32_t g_notebookRequestedEntryPage = -1;

namespace {

constexpr std::uint16_t kCloseButtons = 0x0204U;
constexpr std::uint16_t kNotebookButton = 0x0004U;
constexpr std::uint16_t kFirstPageButton = 0x1000U;
constexpr std::uint16_t kLastPageButton = 0x2000U;
constexpr std::uint16_t kTurnBackInput = 0x8000U;
constexpr std::uint16_t kTurnForwardInput = 0x4000U;
constexpr std::uint8_t kNotebookHelpToken = 0x13;
constexpr std::uint8_t kSystemMenuHelpToken = 0x11;
constexpr std::uint8_t kPlayerDefaultControlMode = 2;
constexpr std::int32_t kGameHudElement = 65;
constexpr std::int32_t kSubtitleUiElement = 58;
constexpr std::int32_t kAuthorScripted = 9;
constexpr std::int16_t kNotebookLastPage = 101;
constexpr std::int16_t kNotebookPageCount = 111;
constexpr double kPageStackScale = 0.009;
constexpr std::int32_t kMmtkToken = static_cast<std::int32_t>(SHEN_NAME(MMTK));
constexpr std::int32_t kMemoTextureToken = static_cast<std::int32_t>(SHEN_NAME(MEMT));
constexpr std::int32_t kMemoModelToken = static_cast<std::int32_t>(SHEN_NAME(MEMO));
constexpr std::int32_t kModelImageToken = static_cast<std::int32_t>(make_shen_name('M', 'A', 'I', ' '));

#include "s1/task/mmtk_notebook_flags.inc"

HLib::Task *g_mmtkTask = nullptr;

PadCaptureEntry *g_notebookPadEntry = nullptr;
PadCaptureEntry *g_notebookPreviousPadEntry = nullptr;


std::uint8_t g_notebookManualSession = 0;
std::int32_t g_notebookScriptSession = 0;
std::uint8_t g_notebookSavedHelpToken = 0;
NotebookTaskState g_notebookState = NotebookTaskState::InactiveClosed;

std::uint8_t g_notebookOpen = 0;
std::uint8_t g_notebookDrawEnabled = 0;

std::int32_t g_notebookLinesHidden = 0;
std::int32_t g_notebookRefreshPages = 0;

UIDrawObject g_notebookDrawObject{.lightColour = {2.0f, 2.0f, 2.0f, 2.0f}};
PackModel *g_notebookModelOwner = nullptr;

Mt5RuntimeNode *g_notebookCoverNode = nullptr;
Mt5RuntimeNode *g_notebookTurningPageNode = nullptr;
Mt5RuntimeNode *g_notebookRightStackNode = nullptr;
Mt5RuntimeNode *g_notebookLeftStackNode = nullptr;
Mt5RuntimeNode *g_notebookAnimatedNode = nullptr;

Vector4 g_notebookFinalRotation{};

std::int32_t g_notebookTurnFrames = 0;
std::int32_t g_notebookTurnAngle = 0;

std::int32_t g_notebookTurnFrameCount = 0;
std::int32_t g_notebookCurlFrameLimit = 8;
std::int16_t g_notebookCurlMotionId = 0;


std::int16_t g_notebookLeftPages = 0;
std::int16_t g_notebookRightPages = 0;
std::int16_t g_notebookSpread = 0;
std::uint8_t g_notebookRepeatedInput = 0;

std::uint16_t g_notebookInput = 0;
std::uint16_t g_notebookPreviousInput = 0;

std::uint16_t g_notebookSavedUiState = 0;
UISicnPausePolicy g_notebookSavedSicnPolicy = UISicnPausePolicy::ClearOnPause;

std::int32_t g_notebookSavedRyoVisible = 0;
std::int32_t g_notebookSavedRyoFlag10 = 0;
std::int32_t g_notebookSavedFov = 0;

Vector3 g_notebookSavedEye{};
Vector3 g_notebookSavedTarget{};

float g_notebookRestX = 0.0f;
float g_notebookRestZ = 0.0f;

std::int32_t g_notebookCachedAuthorState = 0;
std::int32_t g_notebookAicaVolume = 0;

HLib::TaskData *g_notebookMemoTextureFile = nullptr;
std::int32_t g_notebookTextureCount = 0;


std::array<NotebookTextureEntry, 4> g_notebookMemoTextureEntries{};
std::array<NotebookTextureEntry, 16> g_notebookTextureEntries{};
std::array<NotebookTextureEntry, 16> g_notebookTextureEntryCopies{};
std::array<std::int32_t, 32> g_notebookStripTextures{};

MapControlData *MapControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

Character *Ryo() {
    return static_cast<Character *>(MapControl()->ryo);
}



void Notebook_SetAicaVolume(std::int32_t volume) {
    shenExecuteAICACommand(0x11a0U, 2, volume);
    shenExecuteAICACommand(0x05a0U, 2, volume);
}


void Notebook_ResetAicaVolume() {
    g_notebookAicaVolume = 0;
    Notebook_SetAicaVolume(g_notebookAicaVolume);
}


const std::uint16_t *GetNotebookLineData() {
    if (g_uiLoadedLanguage == -1)
        ReadSubtitleGlyphsAndSubs(0);
    return g_uiLoadedLanguage == 1 ? kStoryMemoFlags.data() : kLoadingFlags.data();
}


void Notebook_UpdateLastEntryPage(std::uint16_t flag) {
    g_notebookPersistenceWord = flag;
    const std::uint16_t *record = GetNotebookLineData();
    while (static_cast<std::uint8_t>(record[0]) != 0x80U) {
        for (std::size_t line = 1; line < kNotebookFlagRecordWords; ++line) {
            if (record[line] == g_notebookPersistenceWord)
                g_notebookLastEntryPage = static_cast<std::uint8_t>(record[0]);
        }
        record += kNotebookFlagRecordWords;
    }
    if (static_cast<std::int16_t>(g_notebookLastEntryPage) < 1)
        g_notebookLastEntryPage = 0;
}


std::uint8_t GetLastEntryPage() {
    if (g_notebookPersistenceWord)
        Notebook_UpdateLastEntryPage(g_notebookPersistenceWord);
    std::int32_t page = g_notebookRequestedEntryPage;
    if (page <= -1) {
        page = g_notebookLastEntryPage;
        if (static_cast<std::int16_t>(g_notebookLastEntryPage) < 1) {
            g_notebookLastEntryPage = 0;
            return 0;
        }
    } else {
        g_notebookRequestedEntryPage = -1;
    }
    return static_cast<std::uint8_t>(page);
}



std::uint32_t Notebook_CopyStripTextureIndices(std::byte *binding, std::int32_t *textures, std::uint8_t mode, std::uint32_t index) {
    std::int32_t stripOffset = 0;
    std::memcpy(&stripOffset, binding + 12, sizeof(stripOffset));
    auto *command = reinterpret_cast<std::int16_t *>(stripOffset ? g_pCurrentTaskData + stripOffset : nullptr);
    std::int32_t *texture = textures + index;
    while (true) {
        const std::int16_t opcode = command[0];
        if (static_cast<std::uint16_t>(opcode - 12) <= 9U || static_cast<std::uint16_t>(opcode - 24) <= 5U || static_cast<std::uint16_t>(opcode - 2) <= 5U) {
            command += (static_cast<std::uint16_t>(command[1]) >> 1) + 2;
            continue;
        }
        if (opcode == 9) {
            if (mode == 1)
                command[1] = static_cast<std::int16_t>(*texture);
            else if (mode == 0)
                *reinterpret_cast<std::int16_t *>(texture) = command[1];
            ++index;
            texture += 1;
            command += 2;
            continue;
        }
        if ((static_cast<std::uint16_t>(opcode - 8) & 0xfffcU) == 0) {
            command += 2;
            continue;
        }
        if (static_cast<std::uint16_t>(command[0] - 16) <= 5U) {
            command += (static_cast<std::uint16_t>(command[1]) >> 1) + 2;
            continue;
        }
        if (opcode)
            return index;
        ++command;
    }
}


std::uint32_t NotebookCopyTextures(Mt5RuntimeNode *node, std::int32_t *textures, std::uint8_t mode, std::uint32_t index) {
    std::byte *const binding = node->sceneBindingOffset ? g_pCurrentTaskData + node->sceneBindingOffset : nullptr;
    std::uint32_t next = index;
    std::uint32_t stripCount = 0;
    std::memcpy(&stripCount, binding + 8, sizeof(stripCount));
    if (stripCount)
        next = Notebook_CopyStripTextureIndices(binding, textures, mode, index);
    if (node->childOffset && g_pCurrentTaskData + node->childOffset)
        next = NotebookCopyTextures(reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + node->childOffset), textures, mode, next);
    if (node->siblingOffset && g_pCurrentTaskData + node->siblingOffset)
        return NotebookCopyTextures(reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + node->siblingOffset), textures, mode, next);
    return next;
}



PackModel *LoadForNotebook(std::byte *file, NotebookTextureEntry *textureEntries, std::int32_t *stripTextures) {
    std::int32_t textureCount = 0;
    const std::byte *textureList = nullptr;
    Mt5RuntimeNode *root = nullptr;
    std::array<std::int32_t, 640> textures{};
    std::byte *chunk = file;
    while (true) {
        std::uint32_t tag = 0;
        std::uint32_t operand = 0;
        std::memcpy(&tag, chunk, sizeof(tag));
        std::memcpy(&operand, chunk + 8, sizeof(operand));
        if (tag == SHEN_NAME(TEXD)) {
            textureCount = static_cast<std::int32_t>(operand);
        } else if (tag == SHEN_NAME(PTRL)) {
            const auto *relocation = reinterpret_cast<const std::uint32_t *>(chunk + 12);
            for (std::int32_t index = 0; index < static_cast<std::int32_t>(operand); ++index) {
                std::uint64_t field = 0;
                std::memcpy(&field, file + relocation[index], sizeof(field));
                if (field) {
                    std::uint32_t offset = 0;
                    std::memcpy(&offset, file + relocation[index], sizeof(offset));
                    const auto relocated = static_cast<std::uint32_t>(file + offset - g_pCurrentTaskData);
                    std::memcpy(file + relocation[index], &relocated, sizeof(relocated));
                }
            }
        } else if (tag == SHEN_NAME(TEXL)) {
            textureList = file + operand;
        } else if (tag == SHEN_NAME(HRCM)) {
            root = reinterpret_cast<Mt5RuntimeNode *>(file + operand);
        } else if (tag == SHEN_NAME(TEXN)) {
            for (std::int32_t index = 0; index < textureCount; ++index) {
                textures[static_cast<std::size_t>(index)] = LoadTEXN(chunk);
                std::uint32_t size = 0;
                std::memcpy(&size, chunk + 4, sizeof(size));
                if (index + 1 < textureCount)
                    chunk += size;
            }
        }
        std::memcpy(&tag, chunk, sizeof(tag));
        if (tag == SHEN_NAME(PTRL))
            break;
        std::uint32_t size = 0;
        std::memcpy(&size, chunk + 4, sizeof(size));
        chunk += size;
    }
    for (std::int32_t index = 0; index < textureCount; ++index) {
        std::int32_t offset = 0;
        std::memcpy(&offset, textureList + index * 12, sizeof(offset));
        const std::byte *const texture = offset ? g_pCurrentTaskData + offset : nullptr;
        NotebookTextureEntry &entry = textureEntries[index];
        entry.dataOffset = offset;
        std::memcpy(&entry.idLow, texture, sizeof(entry.idLow));
        std::memcpy(&entry.idHigh, texture + 4, sizeof(entry.idHigh));
    }
    if (textureCount)
        NotebookCopyTextures(root, stripTextures, 0, 0);
    auto *const model = static_cast<PackModel *>(
        HLib_AllocHeapMemBlock(static_cast<std::int64_t>(textureCount) * 4 + static_cast<std::int64_t>(sizeof(PackModel)), kModelImageToken));
    model->field_14 = 0;
    model->pakf = reinterpret_cast<PakChunkHeader *>(file);
    model->model = root;
    model->textureCount = static_cast<std::uint32_t>(textureCount);
    if (textureCount)
        std::memcpy(model + 1, textures.data(), static_cast<std::size_t>(textureCount) * sizeof(std::int32_t));
    std::uint32_t depth = 0;
    std::byte *const previousBase = MT5_SetModelBaseOverride(g_pCurrentTaskData);
    void *rootNode = model->model;
    MT5_PrepassNodeTreeTexturesAndStrips(&rootNode, textures.data(), &depth);
    MT5_SetModelBaseOverride(previousBase);
    return model;
}


std::uint32_t Notebook_LoadMemoTextures(std::byte *file, std::uint64_t size, NotebookTextureEntry *entries) {
    std::byte *const end = file + size;
    std::byte *chunk = file;
    std::uint32_t count = 0;
    if (file >= end)
        return 0;
    std::int32_t *surface = g_notebookMemoSurfaces.data();
    do {
        std::uint32_t token = 0;
        std::memcpy(&token, chunk, sizeof(token));
        if (!token)
            break;
        *surface = LoadTEXN(chunk);
        entries->dataOffset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(&g_notebookMemoSurfaces[count]) - g_pCurrentTaskData);
        ++count;
        std::memcpy(&entries->idLow, chunk + 8, sizeof(entries->idLow));
        ++surface;
        std::memcpy(&entries->idHigh, chunk + 12, sizeof(entries->idHigh));
        ++entries;
        std::uint32_t chunkSize = 0;
        std::memcpy(&chunkSize, chunk + 4, sizeof(chunkSize));
        chunk += chunkSize;
    } while (chunk < end);
    return count;
}


bool Notebook_AbortLoad() {
    for (const std::int32_t surface : g_notebookMemoSurfaces) {
        if (surface != -1)
            PVR_FreeAllocatedSurfaceByIndex(surface);
    }
    HLib_FreeTaskData(g_notebookMemoTextureFile, 0);
    shenExecuteAICACommand(0x800abU, 0, 0);
    return true;
}


void Notebook_ResolvePageTextures() {
    for (std::int32_t index = 0; index < g_notebookTextureCount; ++index) {
        const NotebookTextureEntry &entry = g_notebookTextureEntries[static_cast<std::size_t>(index)];
        g_notebookDrawObject.textureIds[static_cast<std::size_t>(index)] =
            PVR_FindOrAllocateSurface(static_cast<std::int32_t>(entry.idLow), static_cast<std::int32_t>(entry.idHigh));
    }
    NotebookCopyTextures(static_cast<Mt5RuntimeNode *>(g_notebookDrawObject.model), g_notebookStripTextures.data(), 1, 0);
}


void Notebook_LeftSide() {
    std::swap(g_notebookTextureLeft[0], g_notebookTextureLeft[2]);
    g_notebookLeftPageFlags[0] = 1;
    Notebook_BindModelPageTextures();
    g_notebookTextureEntries[7] = g_notebookTextureEntries[5];
    Notebook_ResolvePageTextures();
}


void Notebook_RightSide() {
    std::swap(g_notebookTextureLeft[1], g_notebookTextureLeft[3]);
    g_notebookLeftPageFlags[1] = 1;
    Notebook_BindModelPageTextures();
    g_notebookTextureEntries[6] = g_notebookTextureEntries[4];
    Notebook_ResolvePageTextures();
}



void Notebook_PrepareTurnForward(std::int32_t opening) {
    std::uint32_t leftTexture = 2;
    if (!opening) {
        std::swap(g_notebookTextureLeft[3], g_notebookTextureLeft[1]);
        g_notebookLeftPageFlags[3] = 1;
        Notebook_BindModelPageTextures();
        leftTexture = 0;
    }
    PrepareBothNotebookPages(g_notebookSpread, leftTexture, 3);
    g_notebookTextureEntries[4] = g_notebookTextureEntries[6];
    if ((g_notebookSpread & 1) != 0) {
        g_notebookTextureEntries[6] = g_notebookMemoTextureEntries[3];
        g_notebookTextureEntries[5] = g_notebookMemoTextureEntries[2];
    } else {
        g_notebookTextureEntries[6] = g_notebookMemoTextureEntries[1];
        g_notebookTextureEntries[5] = g_notebookMemoTextureEntries[0];
    }
    Notebook_ResolvePageTextures();
}


void ProgressPages() {
    std::swap(g_notebookTextureLeft[2], g_notebookTextureLeft[0]);
    g_notebookLeftPageFlags[2] = 1;
    Notebook_BindModelPageTextures();
    const std::int16_t spread = g_notebookSpread - 1 >= 0 ? static_cast<std::int16_t>(g_notebookSpread - 1) : 0;
    PrepareBothNotebookPages(spread, 2, 1);
    g_notebookTextureEntries[5] = g_notebookTextureEntries[7];
    if ((g_notebookSpread & 1) != 0) {
        g_notebookTextureEntries[7] = g_notebookMemoTextureEntries[0];
        g_notebookTextureEntries[4] = g_notebookMemoTextureEntries[1];
    } else {
        g_notebookTextureEntries[7] = g_notebookMemoTextureEntries[2];
        g_notebookTextureEntries[4] = g_notebookMemoTextureEntries[3];
    }
    Notebook_ResolvePageTextures();
}


void Notebook_RefreshOpenSpread() {
    const std::int8_t spread = g_notebookSpread <= 0 ? 0 : static_cast<std::int8_t>(g_notebookSpread - 1);
    if ((spread & 1) != 0) {
        g_notebookTextureEntries[7] = g_notebookMemoTextureEntries[2];
        g_notebookTextureEntries[6] = g_notebookMemoTextureEntries[3];
    } else {
        g_notebookTextureEntries[7] = g_notebookMemoTextureEntries[0];
        g_notebookTextureEntries[6] = g_notebookMemoTextureEntries[1];
    }
    Notebook_ResolvePageTextures();
}


void TurnToPage(std::int16_t page) {
    CreateNotebookTexture2DForPages();
    g_notebookFinalRotation.x = 0.0f;
    g_notebookDrawObject.position.z = g_notebookRestZ;
    g_notebookRightPages = static_cast<std::int16_t>(kNotebookPageCount - page);
    g_notebookDrawObject.position.x = g_notebookRestX;
    g_notebookLeftPages = page;
    g_notebookSpread = page;
    g_notebookState = NotebookTaskState::Opening;
    Notebook_PrepareTurnForward(1);
    ++g_notebookSpread;
    g_notebookFinalRotation.y =
        (static_cast<float>(static_cast<double>(kNotebookPageCount - g_notebookRightPages) * 2.04e-05) - g_notebookFinalRotation.x) / 22.0f;
    g_notebookCoverNode->rotation.z = 0;
    g_notebookRightStackNode->scale.x = 1.0f;
    g_notebookRightStackNode->scale.z = 1.0f;
    g_notebookRightStackNode->scale.y = static_cast<float>(static_cast<double>(g_notebookRightPages) * kPageStackScale);
    g_notebookLeftStackNode->rotation.z = 0;
    if (g_notebookLeftPages > 0) {
        g_notebookLeftStackNode->scale.x = 1.0f;
        g_notebookLeftStackNode->scale.z = 1.0f;
        g_notebookLeftStackNode->scale.y = static_cast<float>(static_cast<double>(125 - g_notebookRightPages) * kPageStackScale);
    } else {
        g_notebookLeftStackNode->scale = {};
    }
    Notebook_LeftSide();
    shenExecuteAICACommand(0xb00abU, 0, 0);
}


void UpdatePageAngles() {
    g_notebookFinalRotation.z = 0.0f;
    g_notebookTurnFrames = 0;
    g_notebookTurnAngle = 0;
    const bool forward = g_notebookState == NotebookTaskState::BeginTurnLeft;
    if (!g_notebookRepeatedInput) {
        g_notebookFinalRotation.w = 1.0f;
        g_notebookCurlMotionId = forward ? 1017 : 1018;
        g_notebookTurnFrameCount = 22;
        g_notebookCurlFrameLimit = 8;
        return;
    }
    if (g_notebookCurlFrameLimit / 2 >= 4) {
        g_notebookFinalRotation.w = 2.0f;
        g_notebookTurnFrameCount = 11;
        g_notebookCurlFrameLimit = 4;
        g_notebookCurlMotionId = forward ? 1017 : 1018;
        return;
    }
    g_notebookFinalRotation.w = 1.0f;
    g_notebookTurnFrameCount = 5;
    g_notebookCurlFrameLimit = 1;
    g_notebookCurlMotionId = forward ? 1550 : 1551;
}

float HalfToFloat(std::uint16_t half) {
    const std::uint32_t sign = static_cast<std::uint32_t>(half & 0x8000U) << 16U;
    std::uint32_t exponent = (half >> 10U) & 0x1fU;
    std::uint32_t mantissa = half & 0x3ffU;
    std::uint32_t bits = 0;
    if (exponent == 0x1fU) {
        bits = sign | 0x7f800000U | (mantissa << 13U);
    } else if (exponent) {
        bits = sign | ((exponent + 112U) << 23U) | (mantissa << 13U);
    } else if (mantissa) {
        exponent = 113;
        while ((mantissa & 0x400U) == 0) {
            mantissa <<= 1U;
            --exponent;
        }
        bits = sign | (exponent << 23U) | ((mantissa & 0x3ffU) << 13U);
    } else {
        bits = sign;
    }
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}




std::int32_t MT5_ApplyNodeTrackFrame(const std::byte *tracks, Mt5RuntimeNode *node, std::int32_t frame, float blend, std::int32_t nodeIndex) {
    std::uint32_t nodeCount = 0;
    std::memcpy(&nodeCount, tracks + 8, sizeof(nodeCount));
    if (static_cast<std::uint32_t>(nodeIndex) >= nodeCount)
        return nodeIndex;
    const std::byte *const record = tracks + 32 * nodeIndex;
    const auto word = [](const std::byte *address) {
        std::uint16_t value = 0;
        std::memcpy(&value, address, sizeof(value));
        return value;
    };
    const std::uint16_t scaleTrack = word(record + 12);
    if (scaleTrack) {
        const std::byte *const sample = tracks + scaleTrack + 6 * frame;
        const float x = HalfToFloat(word(sample));
        node->scale.x = x;
        const float y = HalfToFloat(word(sample + 2));
        node->scale.y = y;
        node->scale.z = HalfToFloat(word(sample + 4));
        if (blend != 0.0f) {
            const float z = node->scale.z;
            node->scale.x = (HalfToFloat(word(sample + 6)) - x) * blend + x;
            node->scale.y = (HalfToFloat(word(sample + 8)) - y) * blend + y;
            node->scale.z = (HalfToFloat(word(sample + 10)) - z) * blend + z;
        }
    } else {
        node->scale.x = HalfToFloat(word(record + 14));
        node->scale.y = HalfToFloat(word(record + 16));
        node->scale.z = HalfToFloat(word(record + 18));
    }
    const auto rotation = [&](std::size_t trackOffset, std::int32_t &value) {
        const std::uint16_t track = word(record + trackOffset);
        if (!track) {
            value = static_cast<std::int16_t>(word(record + trackOffset + 2));
            return;
        }
        const std::byte *const sample = tracks + track;
        const auto current = static_cast<std::int16_t>(word(sample + 2 * frame));
        value = current;
        if (blend != 0.0f) {
            const auto next = static_cast<std::int16_t>(word(sample + 2 * (frame + 1)));
            value = static_cast<std::int32_t>(static_cast<float>(current) + static_cast<float>(next - current) * blend);
        }
    };
    rotation(20, node->rotation.x);
    rotation(24, node->rotation.y);
    rotation(28, node->rotation.z);
    const auto position = [&](std::size_t trackOffset, float &value) {
        const std::uint16_t track = word(record + trackOffset);
        if (!track) {
            value = HalfToFloat(word(record + trackOffset + 2));
            return;
        }
        const std::byte *const sample = tracks + track;
        const float current = HalfToFloat(word(sample + 2 * frame));
        value = current;
        if (blend != 0.0f)
            value = (HalfToFloat(word(sample + 2 * (frame + 1))) - current) * blend + current;
    };
    position(32, node->position.x);
    position(36, node->position.y);
    position(40, node->position.z);
    std::int32_t next = nodeIndex + 1;
    if (node->childOffset && g_pCurrentTaskData + node->childOffset)
        next = MT5_ApplyNodeTrackFrame(tracks, reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + node->childOffset), frame, blend, next);
    if (node->siblingOffset && g_pCurrentTaskData + node->siblingOffset)
        return MT5_ApplyNodeTrackFrame(tracks, reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + node->siblingOffset), frame, blend, next);
    return next;
}


bool UpdatePageTurnRotation() {
    std::int16_t leftPages = g_notebookLeftPages;
    bool accumulate = true;
    if (!leftPages) {
        const auto yaw = static_cast<std::uint16_t>(g_notebookDrawObject.rotation.y);
        const std::int32_t facing = yaw > 0x8000U ? 0x10000 - yaw : yaw;
        if (g_notebookState == NotebookTaskState::TurnLeftPage) {
            g_notebookDrawObject.position.x =
                static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(0x4000 - facing))) * 0.0018 + g_notebookDrawObject.position.x);
            g_notebookDrawObject.position.z = static_cast<float>(
                static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_notebookDrawObject.rotation.y))) * -0.0018 + g_notebookDrawObject.position.z);
        } else if (g_notebookState == NotebookTaskState::TurnRightPage) {
            const double x = g_notebookDrawObject.position.x;
            g_notebookDrawObject.position.x = static_cast<float>(x - static_cast<double>(sin_fixed16(static_cast<std::int16_t>(0x4000 - facing))) * 0.0018);
            const double z = g_notebookDrawObject.position.z;
            g_notebookDrawObject.position.z =
                static_cast<float>(z - static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_notebookDrawObject.rotation.y))) * -0.0018);
        }
    } else if (leftPages > 0) {
        MotnFileNode *file = MOTN_GetFileRegistryHead();
        while (file &&
            (file->firstAnimId >= static_cast<std::uint16_t>(g_notebookCurlMotionId) ||
                static_cast<std::uint16_t>(g_notebookCurlMotionId) >= file->endAnimId)) {
            file = file->next;
        }
        const std::byte *sequence = nullptr;
        if (file) {
            const auto *const motion = static_cast<const std::byte *>(file->motionData);
            std::uint32_t tableOffset = 0;
            std::uint32_t dataOffset = 0;
            std::uint32_t entry = 0;
            std::memcpy(&tableOffset, motion, sizeof(tableOffset));
            std::memcpy(&dataOffset, motion + 8, sizeof(dataOffset));
            const std::int32_t index = static_cast<std::uint16_t>(g_notebookCurlMotionId - 1) - file->firstAnimId;
            std::memcpy(&entry, motion + tableOffset + 8 * index, sizeof(entry));
            sequence = motion + entry + dataOffset;
        }
        std::uint16_t marker = 0;
        std::memcpy(&marker, sequence + 2, sizeof(marker));
        const std::byte *const tracks = marker == 0xffffU ? sequence : sequence + 4;
        std::int16_t frames = 0;
        std::memcpy(&frames, tracks, sizeof(frames));
        if (static_cast<std::int32_t>(g_notebookFinalRotation.z) <= frames) {
            MT5_ApplyNodeTrackFrame(
                tracks, static_cast<Mt5RuntimeNode *>(g_notebookDrawObject.model), static_cast<std::int32_t>(g_notebookFinalRotation.z), 0.0f, 0);
        }
        const float frame = g_notebookFinalRotation.z + g_notebookFinalRotation.w;
        g_notebookFinalRotation.z = frame;
        if (frame >= 26.0f && g_notebookFinalRotation.w == 2.0f)
            g_notebookFinalRotation.z = 29.0f;
        else if (frame >= 29.0f)
            g_notebookFinalRotation.z = 29.0f;
        ++g_notebookTurnFrames;
        g_notebookLeftStackNode->rotation.z = 0x8000;
        g_notebookLeftStackNode->scale.y = static_cast<float>(static_cast<double>(117 - g_notebookRightPages) * kPageStackScale);
        if (g_notebookLeftPages == 1)
            g_notebookLeftStackNode->scale = {};
        g_notebookRightStackNode->scale.y = static_cast<float>(static_cast<double>(g_notebookRightPages + 2) * kPageStackScale);
        leftPages = g_notebookLeftPages;
        if (g_notebookLeftPages == 100) {
            g_notebookRightStackNode->scale = {};
            leftPages = g_notebookLeftPages;
        }
    }
    if (leftPages && g_notebookTurnFrames < g_notebookCurlFrameLimit)
        accumulate = false;
    float lift = g_notebookFinalRotation.x;
    if (accumulate) {
        lift = g_notebookFinalRotation.x + g_notebookFinalRotation.y;
        g_notebookFinalRotation.x = lift;
        g_notebookTurnAngle += 0x8000 / g_notebookTurnFrameCount;
    }
    g_notebookAnimatedNode->position.y = lift;
    if (g_notebookTurnAngle <= 0x8000)
        return false;
    g_notebookTurnFrames = 0;
    g_notebookTurnAngle = 0;
    g_notebookTurningPageNode->scale = {};
    return true;
}


void UpdateJinguruSoundAndUIState() {
    std::uint32_t pending = g_scenarioStepLut.notebookJinglePending;
    if (g_notebookHelpOverrideActive)
        pending = 0;
    g_scenarioStepLut.notebookJinglePending = pending;
    if (!g_flmwPlayerDriveActive && !ShouldRun())
        return;
    if (!SCEN_HasActiveScript() && pending && ++g_scenarioStepLut.notebookJingleDelayFrames > 3) {
        g_scenarioStepLut.notebookJinglePending = 0;
        g_notebookJingleSubtitle = ProcessSubtitleFile("JINGURU03", 0, 1, nullptr);
        g_hudPromptJinguruIconOverrideActive = 1;
        UI_SICNEnqueue(static_cast<std::uint16_t>(UI_GetPresentationSnapshot().sicnState | 0x200U), UISicnPausePolicy::KeepOnPause);
    }
}

void OpenIdleState(std::uint16_t pressed) {
    if ((pressed & kCloseButtons) != 0 && !g_uiElements[kUiGameMenu].isVisible) {
        const std::uint16_t uiState = g_notebookSavedUiState;
        g_notebookState = NotebookTaskState::Closing;
        UI_SICNSetState(uiState, g_notebookSavedSicnPolicy);
        if (g_notebookManualSession && !uiState)
            UI_SICNClearState(static_cast<std::uint16_t>(((~UI_GetPresentationSnapshot().sicnState) & 0x200U) | 0x0dffU));
        shenForceClearCurrentSubtitles(nullptr);
        Notebook_ResetAicaVolume();
        ExecuteAicaCommandsForAllBanks(0, true);
        return;
    }
    if (!g_notebookDrawObject.model)
        return;
    g_fBloomEnabled = false;
    SEQ_Cleanup();
    bool slotBusy = false;
    for (const CVWKRuntimeSlot &slot : g_pCVWK->slots)
        slotBusy = slotBusy || slot.callback;
    if (!slotBusy)
        ProcessSubtitleFile("MEMO", 0, 1, nullptr);
    if (g_notebookRefreshPages) {
        Notebook_RefreshOpenSpread();
        g_notebookRefreshPages = 0;
    }
    if ((g_notebookInput & kTurnBackInput) != 0) {
        if (!g_notebookLeftPages) {
            g_notebookState = NotebookTaskState::OpenIdle;
        } else {
            g_notebookState = NotebookTaskState::BeginTurnRight;
            g_notebookRepeatedInput = g_notebookPreviousInput == g_notebookInput;
        }
    } else if ((g_notebookInput & kTurnForwardInput) != 0) {
        if (g_notebookLeftPages == kNotebookLastPage) {
            g_notebookState = NotebookTaskState::OpenIdle;
        } else {
            g_notebookState = NotebookTaskState::BeginTurnLeft;
            g_notebookRepeatedInput = g_notebookPreviousInput == g_notebookInput;
        }
    } else if ((pressed & kFirstPageButton) != 0) {
        TurnToPage(0);
    } else if ((pressed & kLastPageButton) != 0) {
        TurnToPage(GetLastEntryPage());
    }
}



bool LoadNotebook() {
    g_hudPromptJinguruIconOverrideActive = 0;
    g_fBloomEnabled = false;
    auto *const memo = static_cast<HLib::TaskData *>(ReadFile("model/item", "MEMOTEX.SPR", kMemoTextureToken));
    if (!memo) {
        shenExecuteAICACommand(0x800abU, 0, 0);
        return false;
    }
    g_notebookMemoTextureFile = memo;
    const std::uint64_t memoSize = shenOpenFile("model/item", "MEMOTEX.SPR");
    Notebook_LoadMemoTextures(reinterpret_cast<std::byte *>(memo), memoSize, g_notebookMemoTextureEntries.data());
    bool failed = g_notebookMemoSurfaces[0] == -1;
    failed = failed || g_notebookMemoSurfaces[1] == -1;
    failed = failed || g_notebookMemoSurfaces[2] == -1;
    if (g_notebookMemoSurfaces[3] == -1 || failed)
        return !Notebook_AbortLoad();
    std::byte *memoChunk = reinterpret_cast<std::byte *>(g_notebookMemoTextureFile);
    for (std::byte *&chunk : g_notebookMemoTextureChunks) {
        std::uint32_t size = 0;
        std::memcpy(&size, memoChunk + 4, sizeof(size));
        chunk = memoChunk;
        memoChunk += size;
    }
    auto *const file = static_cast<std::byte *>(ReadFile("model/item", "MMA0201G.MT5", kMemoModelToken));
    if (!file)
        return !Notebook_AbortLoad();
    PackModel *const model = LoadForNotebook(file, g_notebookTextureEntries.data(), g_notebookStripTextures.data());
    if (!model) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
        return !Notebook_AbortLoad();
    }
    char fbxPath[260]{};
    std::snprintf(fbxPath, sizeof(fbxPath), "%s/%s/%s.fbx", "model/item", "MMA0201G.MT5", "MMA0201G");

    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), fbxPath, 0, 2);
    g_notebookDrawObject.model = model->model;
    g_notebookModelOwner = model;
    g_notebookTextureCount = static_cast<std::int32_t>(model->textureCount);
    for (std::int16_t index = 0; index < g_notebookTextureCount; ++index)
        g_notebookTextureEntryCopies[static_cast<std::size_t>(index)] = g_notebookTextureEntries[static_cast<std::size_t>(index)];

    g_previousHelpTokenId = g_currentHelpTokenId;
    const UIPresentationSnapshot presentation = UI_GetPresentationSnapshot();
    g_defaultHelpTokenId = kNotebookHelpToken;
    g_notebookSavedUiState = static_cast<std::uint16_t>(presentation.sicnState & 0xfdffU);
    g_notebookSavedSicnPolicy = presentation.sicnPausePolicy;
    g_currentHelpTokenId = kNotebookHelpToken;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    if (UI_SICNIsTaskActive())
        UI_SICNSetState(0x23, UISicnPausePolicy::KeepOnPause);
    else
        UI_SICNEnqueue(0x23, UISicnPausePolicy::KeepOnPause);
    g_notebookOpen = 1;
    ExecuteAicaCommandsForAllBanks(1, true);
    g_notebookAicaVolume = -64;
    Notebook_SetAicaVolume(g_notebookAicaVolume);
    if (Character *const ryo = Ryo()) {
        const std::uint8_t motionFlags = ryo->motionFlags;
        const std::uint32_t flags = ryo->flags.value;
        g_notebookSavedRyoVisible = (motionFlags & 4U) == 4U;
        g_notebookSavedRyoFlag10 = static_cast<std::int32_t>((flags >> 4U) & 1U);
        const bool wasVisible = (motionFlags & 4U) != 0;
        ryo->motionFlags = static_cast<std::uint8_t>(motionFlags & 0xfbU);
        if (g_bDebugLogVisibility && (flags & 0x400U) != 0 && ((ryo->motionFlags >> 2U) & 1U) != static_cast<std::uint32_t>(wasVisible))
            CHAR_LogNPCVisibility(ryo, ((ryo->motionFlags >> 2U) & 1U) != 0);
        ryo->flags.value &= ~0x10U;
    }
    g_notebookSavedFov = currentPerspectiveFov;
    CAME_SetFieldOfView(9830);
    HLib::Task *const windTask = MapControl()->windTask ? MapControl()->windTask : g_windTask;

    const std::uint32_t windDisabled = 0;
    std::memcpy(reinterpret_cast<std::byte *>(HLib_GetTaskDataForTask(windTask)) + 0x2b0, &windDisabled, sizeof(windDisabled));

    auto *const root = static_cast<Mt5RuntimeNode *>(g_notebookDrawObject.model);
    g_notebookCoverNode = root->childOffset ? reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + root->childOffset) : nullptr;
    g_notebookTurningPageNode =
        g_notebookCoverNode->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + g_notebookCoverNode->siblingOffset) : nullptr;
    g_notebookRightStackNode =
        g_notebookTurningPageNode->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + g_notebookTurningPageNode->siblingOffset) : nullptr;
    g_notebookLeftStackNode =
        g_notebookRightStackNode->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + g_notebookRightStackNode->siblingOffset) : nullptr;
    g_notebookLeftStackNode->scale.x = 0.0f;
    g_notebookAnimatedNode = g_notebookLeftStackNode;
    g_notebookLeftStackNode->scale.z = 0.0f;
    g_notebookLeftStackNode->scale.y = 0.0f;
    g_notebookTurningPageNode->scale = {};
    g_notebookRightStackNode->scale = {1.0f, 1.0f, 1.0f};

    Character *const ryo = Ryo();
    std::int32_t facing = -(ryo ? ryo->entity.node.rotation.y : 0);
    g_notebookDrawObject.scale = 1.3f;
    g_notebookSavedEye = g_CameraEyePos;
    g_notebookDrawObject.rotation.y = -facing;
    g_notebookSavedTarget = g_CameraTargetPos;
    g_notebookDrawObject.rotation.x = 12743;
    g_notebookDrawObject.rotation.z = 0;
    g_notebookTurnAngle = 0;
    g_notebookCachedAuthorState = g_nAuthorState;
    SetNewAuthorGameState(kAuthorScripted);
    Vector3 eye{};
    float eyeHeight = 0.0f;
    if (g_notebookManualSession && g_notebookCachedAuthorState == kAuthorScripted) {
        eye = g_CameraEyePos;
        Vector3 unused{};
        SRCH_UpdateCameraEyeAndTarget(eye, unused);
        eyeHeight = eye.y;
        facing = g_fCamAccumYaw;
        g_notebookDrawObject.rotation.y = -g_fCamAccumYaw;
    } else {
        if (Character *const owner = Ryo()) {
            eye = GetPositionForEntity(owner->entity.node);
        } else {
            eye.x = 0.0f;
            eye.z = 0.0f;
        }
        eyeHeight = static_cast<float>(static_cast<double>(eye.y) + 1.6);
        eye.y = eyeHeight;
    }
    TranslateCameraRelative(eye);
    g_notebookDrawObject.position.y = eyeHeight;
    const auto facingAngle = static_cast<std::uint16_t>(facing);
    const float sine = sin_fixed16(static_cast<std::int16_t>(facing));
    const std::int32_t folded = facingAngle <= 0x8000U ? facingAngle : 0x10000 - facingAngle;
    float x = static_cast<float>(static_cast<double>(sine) * 0.2 + eye.x);
    g_notebookDrawObject.position.x = x;
    float z = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(0x4000 - folded))) * -0.2 + eye.z);
    g_notebookDrawObject.position.z = z;
    if (!bLockCamera) {
        g_CameraTargetPos = {x, eyeHeight, z};
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
        z = g_notebookDrawObject.position.z;
        eyeHeight = g_notebookDrawObject.position.y;
        x = g_notebookDrawObject.position.x;
    }
    g_notebookFinalRotation.x = 0.0f;
    g_notebookRestX = x;
    g_notebookRestZ = z;
    g_notebookDrawObject.position.y = static_cast<float>(static_cast<double>(eyeHeight) + 0.005);
    TurnToPage(GetLastEntryPage());
    if (!g_notebookScriptSession && g_playerStats.pages.notebookOpenCount != 0xffffU)
        ++g_playerStats.pages.notebookOpenCount;
    g_notebookHelpOverrideActive = 1;
    return true;
}

void OpeningState() {
    g_notebookDrawEnabled = 1;
    const auto yaw = static_cast<std::uint16_t>(g_notebookDrawObject.rotation.y);
    const std::int32_t facing = yaw > 0x8000U ? 0x10000 - yaw : yaw;
    g_notebookDrawObject.position.x =
        static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(0x4000 - facing))) * 0.0018 + g_notebookDrawObject.position.x);
    const float sine = sin_fixed16(static_cast<std::int16_t>(g_notebookDrawObject.rotation.y));
    const std::int32_t angle = g_notebookTurnAngle + 1456;
    g_notebookTurnAngle = angle;
    g_notebookDrawObject.position.z = static_cast<float>(static_cast<double>(sine) * -0.0018 + g_notebookDrawObject.position.z);
    if (angle < 0x8000) {
        g_notebookCoverNode->rotation.z = angle;
        g_notebookLeftStackNode->rotation.z = g_notebookTurnAngle;
        return;
    }
    g_notebookTurnAngle = 0;
    g_notebookCoverNode->rotation.z = 0x8000;
    if (g_notebookLeftPages > 0)
        g_notebookLeftStackNode->rotation.z = 0x8000;
    g_notebookLeftPages = static_cast<std::int16_t>(g_notebookLeftPages + 1 >= kNotebookLastPage ? kNotebookLastPage : g_notebookLeftPages + 1);
    g_notebookState = NotebookTaskState::OpenIdle;
}

void BeginTurnForwardState() {
    Notebook_PrepareTurnForward(0);
    ++g_notebookSpread;
    std::uint32_t sound = 0xc00abU;
    if (g_notebookLeftPages) {
        Mt5RuntimeNode *const page = g_notebookTurningPageNode;
        g_notebookAnimatedNode = page;
        page->rotation.z = 0;
        page->scale = {1.0f, 1.0f, 1.0f};
        std::int16_t rightPages = static_cast<std::int16_t>(g_notebookRightPages - 1);
        g_notebookRightPages = rightPages;
        if (rightPages <= 1) {
            rightPages = 1;
            g_notebookRightPages = 1;
        }
        g_notebookRightStackNode->scale.y = static_cast<float>(static_cast<double>(rightPages + 2) * kPageStackScale);
        g_notebookFinalRotation.x = static_cast<float>(0.0 - static_cast<double>(g_notebookLeftPages) * 2.04e-05);
        page->position.y = g_notebookFinalRotation.x;
        UpdatePageAngles();
        g_notebookFinalRotation.y = (static_cast<float>(static_cast<double>(g_notebookLeftPages) * 3.29e-05) - g_notebookFinalRotation.x) /
            static_cast<float>(g_notebookTurnFrameCount);
        if (g_notebookLeftPages == 100)
            g_notebookRightStackNode->scale = {};
    } else {
        g_notebookAnimatedNode = g_notebookCoverNode;
        sound = 0xb00abU;
        g_notebookCoverNode->rotation.z = 0;
        g_notebookFinalRotation.x = 0.0f;
        g_notebookTurnFrameCount = 22;
    }
    shenExecuteAICACommand(sound, 0, 0);
    g_notebookState = NotebookTaskState::TurnLeftPage;
}

void BeginTurnBackState() {
    g_notebookLeftPages = static_cast<std::int16_t>(g_notebookLeftPages - 1);
    if (g_notebookLeftPages <= 0) {
        g_notebookLeftPages = 0;
        g_notebookLeftStackNode->scale = {};
    }
    --g_notebookSpread;
    ProgressPages();
    std::uint32_t sound = 0xc00abU;
    if (g_notebookLeftPages) {
        Mt5RuntimeNode *const page = g_notebookTurningPageNode;
        g_notebookAnimatedNode = page;
        page->rotation.z = 0x8000;
        page->scale = {1.0f, 1.0f, 1.0f};
        g_notebookLeftStackNode->scale.y = static_cast<float>(static_cast<double>(117 - g_notebookRightPages) * kPageStackScale);
        g_notebookFinalRotation.x = static_cast<float>(static_cast<double>(111 - g_notebookRightPages) * 3.92e-05);
        page->position.y = g_notebookFinalRotation.x;
        UpdatePageAngles();
        g_notebookFinalRotation.y = (static_cast<float>(0.0 - static_cast<double>(111 - g_notebookRightPages) * 2.04e-05) - g_notebookFinalRotation.x) /
            static_cast<float>(g_notebookTurnFrameCount);
        if (g_notebookLeftPages == 1)
            g_notebookLeftStackNode->scale = {};
    } else {
        g_notebookAnimatedNode = g_notebookCoverNode;
        sound = 0xb00abU;
        g_notebookCoverNode->rotation.z = 0x8000;
        g_notebookFinalRotation.x = 0.0f;
        g_notebookTurnFrameCount = 22;
    }
    shenExecuteAICACommand(sound, 0, 0);
    g_notebookState = NotebookTaskState::TurnRightPage;
}

void TurnForwardState() {
    if (!UpdatePageTurnRotation()) {
        g_notebookAnimatedNode->rotation.z = g_notebookTurnAngle;
        return;
    }
    g_notebookAnimatedNode->rotation.z = 0x8000;
    g_notebookLeftStackNode->scale.y = static_cast<float>(static_cast<double>(118 - g_notebookRightPages) * kPageStackScale);
    std::int16_t leftPages = g_notebookLeftPages;
    if (g_notebookLeftPages == 1) {
        g_notebookLeftStackNode->rotation.z = 0x8000;
        g_notebookLeftStackNode->scale.x = 1.0f;
        g_notebookLeftStackNode->scale.z = 1.0f;
        leftPages = g_notebookLeftPages;
    }
    g_notebookLeftPages = static_cast<std::int16_t>(leftPages + 1 >= kNotebookLastPage ? kNotebookLastPage : leftPages + 1);
    g_notebookState = NotebookTaskState::OpenIdle;
    Notebook_LeftSide();
}

void TurnBackState() {
    if (!UpdatePageTurnRotation()) {
        g_notebookAnimatedNode->rotation.z = 0x8000 - g_notebookTurnAngle;
        return;
    }
    g_notebookAnimatedNode->rotation.z = 0;
    std::int16_t rightPages = static_cast<std::int16_t>(g_notebookRightPages + 1);
    g_notebookRightPages = rightPages;
    if (rightPages >= kNotebookPageCount) {
        rightPages = kNotebookPageCount;
        g_notebookRightPages = kNotebookPageCount;
    }
    g_notebookRightStackNode->scale.y = static_cast<float>(static_cast<double>(rightPages + 2) * kPageStackScale);
    if (g_notebookLeftPages == 1) {
        g_notebookLeftStackNode->rotation.z = 0;
        g_notebookRightStackNode->scale.y = 1.0f;
    } else if (g_notebookLeftPages == 100) {
        g_notebookRightStackNode->rotation.z = 0;
        g_notebookRightStackNode->scale.x = 1.0f;
        g_notebookRightStackNode->scale.z = 1.0f;
    }
    g_notebookState = NotebookTaskState::OpenIdle;
    Notebook_RightSide();
}

void ClosingState() {
    if (Character *const ryo = Ryo()) {
        NPCLogger(ryo, 3, g_notebookSavedRyoVisible);
        if (g_notebookSavedRyoFlag10)
            ryo->flags.value |= 0x10U;
        else
            ryo->flags.value &= ~0x10U;
    }
    SetNewAuthorGameState(g_notebookCachedAuthorState);
    TranslateCameraRelative(g_notebookSavedEye);
    if (!bLockCamera) {
        g_CameraTargetPos = g_notebookSavedTarget;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    CAME_SetFieldOfView(g_notebookSavedFov);
    g_notebookState = NotebookTaskState::Teardown;
    g_notebookRefreshPages = 0;
}

void TeardownState() {
    FreePVR(g_notebookModelOwner);
    g_notebookModelOwner = nullptr;
    g_notebookDrawObject.model = nullptr;
    HLib_FreeTaskData(g_notebookMemoTextureFile, 0);
    for (const std::int32_t surface : g_notebookMemoSurfaces)
        PVR_FreeAllocatedSurfaceByIndex(surface);
    g_notebookState = NotebookTaskState::InactiveClosed;
    g_notebookOpen = 0;
    HLib::Task *const windTask = MapControl()->windTask ? MapControl()->windTask : g_windTask;
    g_defaultHelpTokenId = kSystemMenuHelpToken;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    const std::uint32_t windEnabled = 1;
    std::memcpy(reinterpret_cast<std::byte *>(HLib_GetTaskDataForTask(windTask)) + 0x2b0, &windEnabled, sizeof(windEnabled));
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_fBloomEnabled = g_currentSettings.display.bloomMode != 0;
    g_currentHelpTokenId = kSystemMenuHelpToken;
    g_notebookHelpOverrideActive = 0;
}

} // namespace


void MMTK_ResetForEnqueue() {
    g_notebookLinesHidden = 0;
    g_notebookDrawEnabled = 0;
    g_notebookState = NotebookTaskState::InactiveClosed;
    g_notebookModelOwner = nullptr;
    g_notebookDrawObject.model = nullptr;
    g_notebookCoverNode = nullptr;
    g_notebookLeftStackNode = nullptr;
    g_notebookTurningPageNode = nullptr;
    g_notebookRightStackNode = nullptr;
    g_notebookFinalRotation.z = 0.0f;
    g_notebookCurlFrameLimit = 8;
    g_notebookOpen = 0;
    g_notebookFinalRotation.x = 0.0f;
    g_notebookDrawObject.lightColour = {2.0f, 2.0f, 2.0f, 2.0f};
    g_notebookDrawObject.lightYaw = 0;
    g_notebookSpread = 0;
    g_notebookScriptSession = 0;
    g_notebookRepeatedInput = 0;
    g_notebookHelpOverrideActive = 0;
    if (g_notebookPersistenceWord)
        Notebook_UpdateLastEntryPage(g_notebookPersistenceWord);
    g_notebookRefreshPages = 0;
}

HLib::Task *MMTK_Init() {
    g_mmtkTask = HLib_EnqueueTaskWithoutParameter(MMTK_Callback, 14, 0, kMmtkToken);
    return g_mmtkTask;
}

void MMTK_Shutdown() {
    HLib::Task *const task = g_mmtkTask;
    g_mmtkTask = nullptr;
    if (task && (task->mainTaskCallbackFnPtr == MMTK_Callback || task->mainTaskCallbackFnPtr == MMTK_Callback_0))
        HLib_DestroyTask(task);
}


void MMTK_Callback(HLib::TaskData *) {
    g_notebookPadEntry = AllocateClearedPADCEntry();
    g_notebookManualSession = 0;
    if (CurrentTask) {
        CurrentTask->mainTaskCallbackFnPtr = MMTK_Callback_0;
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}


void MMTK_Callback_0(HLib::TaskData *) {
    if (g_uiElements[kUiGameMenu].isVisible)
        return;
    UpdateJinguruSoundAndUIState();
    if (g_notebookManualSession) {
        if (g_notebookManualSession == 1 && ProcessNotebookStates()) {
            g_notebookHelpOverrideActive = 0;
            if ((g_notebookPadEntry->flags & 2U) != 0)
                shenUpdateInputsInto(g_notebookPreviousPadEntry);
            g_previousHelpTokenId = g_currentHelpTokenId;
            const std::uint8_t token = static_cast<std::int8_t>(g_notebookSavedHelpToken) >= 68 ? 0 : g_notebookSavedHelpToken;
            g_notebookManualSession = 0;
            g_helpButtonId = 0;
            g_currentHelpIconState = 0;
            g_defaultHelpTokenId = token;
            g_currentHelpTokenId = token;
            return;
        }
    } else if (convCanOpen() && (static_cast<PadCaptureEntry *>(MapControl()->padCaptureEntry)->pressedPriority & kNotebookButton) != 0) {
        MMTK_BeginManualSession();
    }
    if (!g_notebookHelpOverrideActive || g_pControlPadController->controlMode != kPlayerDefaultControlMode)
        return;
    if (Character *const ryo = Ryo())
        ryo->skipRootMotionDelta = 1;
}

// The X-button open shared by MMTK_Callback_0 and PrepareNotebookBuffers' requested-page path.
void MMTK_BeginManualSession() {
    shenExecuteAICACommand(0x400abU, 0, 0);
    if ((g_notebookPadEntry->flags & 2U) == 0)
        g_notebookPreviousPadEntry = shenUpdateInputsInto(g_notebookPadEntry);
    g_notebookSavedHelpToken = g_defaultHelpTokenId;
    g_notebookManualSession = 1;
    g_notebookHelpOverrideActive = 1;
}


bool ProcessNotebookStates() {
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    g_notebookPreviousInput = g_notebookInput;
    g_notebookInput = g_inputState.slots[0].repeatedPresses;
    if ((Shenmue_nPauseState & 2) == 0) {
        switch (g_notebookState) {
        case NotebookTaskState::OpenIdle:
            OpenIdleState(pressed);
            break;
        case NotebookTaskState::BeginTurnLeft:
            BeginTurnForwardState();
            break;
        case NotebookTaskState::BeginTurnRight:
            BeginTurnBackState();
            break;
        case NotebookTaskState::TurnLeftPage:
            TurnForwardState();
            break;
        case NotebookTaskState::TurnRightPage:
            TurnBackState();
            break;
        case NotebookTaskState::Teardown:
            TeardownState();
            break;
        case NotebookTaskState::Opening:
            OpeningState();
            break;
        case NotebookTaskState::InactiveClosed:
            if (!LoadNotebook())
                return true;
            break;
        case NotebookTaskState::Closing:
            ClosingState();
            break;
        }
    }
    if (g_notebookDrawEnabled && g_notebookDrawObject.model) {
        DrawObjectForUI(&g_notebookDrawObject);
    }
    if (!g_notebookOpen)
        g_notebookDrawEnabled = 0;
    return !g_notebookOpen;
}



void PrepareBothNotebookPages(std::int16_t page, std::uint32_t leftTexture, std::uint32_t rightTexture) {
    for (std::size_t index = 0; index < g_notebookMemoTextureChunks.size(); ++index)
        g_notebookMemoTextureData[index] = g_notebookMemoTextureChunks[index] + 0x2c;
    const bool odd = (page & 1) != 0;
    std::byte *const firstChunk = odd ? g_notebookMemoTextureChunks[2] : g_notebookMemoTextureChunks[0];
    std::byte *const secondChunk = odd ? g_notebookMemoTextureChunks[3] : g_notebookMemoTextureChunks[1];
    const std::uint16_t *record = GetNotebookLineData();
    while (record[0] != 0x80U && record[0] != static_cast<std::uint16_t>(page))
        record += kNotebookFlagRecordWords;
    bool noPage = false;
    bool leftMissing = false;
    bool rightMissing = false;
    if (record[0] == 0x80U) {
        noPage = true;
    } else {
        const auto leftPage = static_cast<std::uint8_t>(record[0]);
        leftMissing = record[0] == 0x80U || leftPage != static_cast<std::uint16_t>(page);
        const auto rightPage = static_cast<std::uint8_t>(record[kNotebookFlagRecordWords]);
        rightMissing = leftPage == 0x80U || rightPage == 0x80U || rightPage != static_cast<std::uint16_t>(page);
    }
    for (std::array<char, 1280> &text : g_memoText)
        text[0] = '\0';
    const std::uint32_t leftIndex = 2U * static_cast<std::uint32_t>(page);
    const std::uint32_t leftSlot = 2U * static_cast<std::uint32_t>(page & 1);
    const auto lineShown = [](std::uint16_t flag) {
        return (static_cast<std::int16_t>(flag) < 0x2000 &&
                   (g_shenmueFlags[static_cast<std::size_t>(static_cast<std::int16_t>(flag) >> 3)] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0) ||
            !flag;
    };
    const std::uint16_t *line = record + 1;
    for (std::int16_t lineIndex = 0; lineIndex < 8; ++lineIndex, ++line) {
        char type = '0';
        if (!noPage && !leftMissing && !g_notebookLinesHidden && lineShown(line[0]))
            type = 'A';
        FormatNotebookText(leftIndex, leftTexture, leftSlot, static_cast<std::uint32_t>(lineIndex), type);
        type = '0';
        if (!noPage && !rightMissing && !g_notebookLinesHidden && lineShown(line[kNotebookFlagRecordWords]))
            type = 'A';
        FormatNotebookText(leftIndex + 1, rightTexture, leftSlot | 1U, static_cast<std::uint32_t>(lineIndex), type);
    }
    UI_SetPVRSurfaceTexture(0, g_notebookTextureLeft[leftTexture]);
    g_notebookLeftPageFlags[leftTexture] = 1;
    UI_SetPVRSurfaceTexture(0, g_notebookTextureLeft[rightTexture]);
    g_notebookLeftPageFlags[rightTexture] = 1;
    if (odd) {
        CreateNotebookTexture2D(g_notebookMemoSurfaces[2], firstChunk);
        CreateNotebookTexture2D(g_notebookMemoSurfaces[3], secondChunk);
    } else {
        CreateNotebookTexture2D(g_notebookMemoSurfaces[0], firstChunk);
        CreateNotebookTexture2D(g_notebookMemoSurfaces[1], secondChunk);
    }
}


void shenBeginNotebook() {
    if (g_notebookScriptSession)
        return;
    g_notebookSavedHelpToken = g_defaultHelpTokenId;
    g_notebookScriptSession = 1;
    g_notebookHelpOverrideActive = 1;
    UI_ActivateElement(g_uiElements[kGameHudElement]);
}


void shenEndNotebook() {
    if (!g_notebookScriptSession)
        return;
    UI_SICNSetState(g_notebookSavedUiState, g_notebookSavedSicnPolicy);
    if (g_notebookManualSession && !g_notebookSavedUiState)
        UI_SICNClearState(static_cast<std::uint16_t>(((~UI_GetPresentationSnapshot().sicnState) & 0x200U) | 0x0dffU));
    CloseNotebook();
    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t token = static_cast<std::int8_t>(g_notebookSavedHelpToken) >= 68 ? 0 : g_notebookSavedHelpToken;
    g_notebookScriptSession = 0;
    g_defaultHelpTokenId = token;
    g_currentHelpTokenId = token;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    g_notebookHelpOverrideActive = 0;
}


void CloseNotebook() {
    if (!g_notebookDrawObject.model)
        return;
    if (CVWK *const cvwk = g_pCVWK) {
        UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiElement];
        if ((g_currUIState == -1 || g_currUIState == kSubtitleUiElement) &&
            (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
            subtitles.isVisible = 0;
        }
        for (CVWKRuntimeSlot &slot : cvwk->slots) {
            if (slot.callback)
                SEQ_FreeFileHandles(slot);
        }
    }
    Notebook_ResetAicaVolume();
    ExecuteAicaCommandsForAllBanks(0, true);
    g_notebookState = NotebookTaskState::Closing;
    ProcessNotebookStates();
    if (!g_notebookManualSession)
        ProcessNotebookStates();
}

HLib::Task *MMTK_GetTask() {
    return g_mmtkTask && (g_mmtkTask->mainTaskCallbackFnPtr == MMTK_Callback || g_mmtkTask->mainTaskCallbackFnPtr == MMTK_Callback_0) &&
            !g_mmtkTask->task_data_ptr ?
        g_mmtkTask :
        nullptr;
}

PadCaptureEntry *MMTK_GetInputEntry() {
    return g_notebookPadEntry;
}

bool MMTK_CanOpenFromFreeRoam() {
    return !g_notebookManualSession && !g_uiElements[kUiGameMenu].isVisible && convCanOpen();
}

bool MMTK_IsNotebookActive() {
    return g_notebookHelpOverrideActive != 0;
}

bool MMTK_HasScriptSession() {
    return g_notebookScriptSession != 0;
}

bool MMTK_HasManualSession() {
    return g_notebookManualSession != 0;
}

NotebookTaskState MMTK_GetNotebookState() {
    return g_notebookState;
}

std::uint16_t MMTK_GetLastEntryPage() {
    return g_notebookLastEntryPage;
}

void MMTK_SetLastEntryPage(std::uint16_t page) {
    g_notebookLastEntryPage = page;
}



void MMTK_UpdateEntryForFlag(std::uint16_t flag) {
    g_notebookJingleSubtitle = nullptr;
    if (flag < 1024U && (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0)
        return;
    if (kLoadingFlags[0] == 0x80U)
        return;
    const std::uint16_t *record = kLoadingFlags.data();
    while (true) {
        bool found = false;
        for (std::size_t line = 1; line < kNotebookFlagRecordWords; ++line)
            found = found || record[line] == flag;
        if (found)
            break;
        record += kNotebookFlagRecordWords;
        if (static_cast<std::uint8_t>(record[0]) == 0x80U)
            return;
    }
    g_scenarioStepLut.notebookJinglePending = 1;
    g_scenarioStepLut.notebookJingleDelayFrames = 0;
    Notebook_UpdateLastEntryPage(flag);
}

std::uint16_t MMTK_GetPersistenceWord() {
    return g_notebookPersistenceWord;
}

void MMTK_SetPersistenceWord(std::uint16_t value) {
    g_notebookPersistenceWord = value;
}

NotebookPresentationSnapshot MMTK_GetPresentationSnapshot() {
    return {g_notebookState, static_cast<std::uint16_t>(g_notebookLeftPages), g_notebookLastEntryPage, g_playerStats.pages.notebookOpenCount, 0,
        g_notebookHelpOverrideActive};
}


void MMTK_ScriptMemoToggle(ScenarioContext *context, void *rawParams) {
    const std::uint32_t mode = *static_cast<const std::uint32_t *>(rawParams);
    if (!mode) {
        shenBeginNotebook();
    } else if (mode == 1) {
        shenEndNotebook();
    } else if (mode == 2) {
        context->stack->accumulator = g_notebookHelpOverrideActive;
    }
}

} // namespace shenmue::s1
