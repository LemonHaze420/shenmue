#include "s2/system_modules_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/frame_dc.hpp"
#include "s2/arcade_modules_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "s2/pause.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/initialization_dc.hpp"
#include "s2/input_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "platform/dc/aica.hpp"
#include "platform/dc/aica_stream.hpp"
#include "platform/dc/ninja.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/event_overlay.hpp"
#include "platform/dc/am2_vector.hpp"

#include <sg_gd.h>

#include <cstring>

#if SHENMUE_S2_GCC295
#define SHENMUE_SYSTEM_MODULE(name) SystemModule_##name
#define SHENMUE_SYSTEM_MODULE_NULL 0
#else
#define SHENMUE_SYSTEM_MODULE(name) SystemModule::name
#define SHENMUE_SYSTEM_MODULE_NULL nullptr
#endif
#include "s1/arcade_scores.hpp"
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif

namespace shenmue {
namespace s2 {

bool ChangeSystemModule(SystemModule module, std::int32_t entry);
std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);

void ResetHangOnRankings();
void ResetSpaceHarrierRankings();
void HangOnVblankNullsub();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void HarrierVblankNullsub();
#endif
void ResetOutRunRankings();
void ResetAfterBurnerRankings();
void AfterBurnerVblankNullsub();
void ChangeModuleTask_Init(void *);
void ChangeModuleTask_Init(void *);
std::int32_t g_gamePauseState = 0;
HLib::Task *systemModuleTask;
SystemModuleTaskData systemModuleTaskData;
std::byte systemModuleWorkStorage[0x100];
NativeArcadeSession g_arcadeSession;
NativeHangOnBuffers g_hangOnModuleBuffers = {};
std::int32_t GetSystemAudioOutputMode();
std::int32_t ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
void shenQueueChangeModuleAndScene(SystemModule module, std::int32_t act, shen_name area, std::int32_t entry);
void PrepareArcadeModule(SystemModuleTaskData *module);
void AddFrameCount(std::int32_t count);
std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);
extern "C" void njExitSystem();
extern "C" void njSetWaitVSyncFunction(void (*callback)(), void *parameter);
extern "C" void syIntEnableInt(std::uint32_t mask);
extern "C" void syCachePurgeAll();
extern "C" void ConfigureArcadeCpuCache(std::uint32_t control) asm("_shenmue_dc_configure_cpu_cache");
void ReloadModelResourcesNullsub();

extern "C" {
std::int32_t kmStartVertexStrip(KMVERTEXBUFFDESC *buffer);
std::int32_t kmSetVertex(KMVERTEXBUFFDESC *buffer, const void *vertices, std::int32_t type, std::int32_t bytes);
std::int32_t kmSetPaletteMode(std::uint32_t mode);
std::int32_t kmSetPaletteData(const std::uint32_t *colours);
std::int32_t kmSetPixelClipping(std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom);
std::int32_t kmSetEORCallback(void (*callback)(void *), void *parameter);
std::int32_t kmRenderTexture(KMSURFACEDESC *surface);
std::int32_t kmSetGlobalClipping(std::int32_t width, std::int32_t height);
std::int32_t kmChangeDisplayFilterMode(std::uint32_t format, std::uint32_t dither, std::uint32_t antiAlias);
std::int32_t kmSetDisplayMode(std::uint32_t mode, std::int32_t format, std::uint32_t dither, std::uint32_t antiAlias);
std::int32_t kmSetSystemConfiguration(void *configuration);
std::int32_t kmSetUserClipping(KMVERTEXBUFFDESC *buffer, std::int32_t type, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom);
std::int32_t kmSetAutoSortMode(std::int32_t enabled);
std::int32_t kmSetHSyncCallback(void (*callback)(void *), void *parameter);
std::int32_t kmSetHSyncLine(std::int32_t line);
std::int32_t kmGetCurrentScanline(std::int32_t *line);
std::int32_t kmSetEndOfVertexCallback(void (*callback)(void *), void *parameter);
std::int32_t kmResetRenderer();
void njTexMemCopy1(void *destination, const void *source, std::int32_t bytes);
std::int32_t syCblCheck();
extern std::uint8_t kmiDeviceExtension[];
}

// @verified - d
void PrepareArcadeModule(SystemModuleTaskData *module) {
    const shen_name area = g_currentMap;
    ReloadModelResourcesNullsub();
    if (static_cast<std::int32_t>(module->previousModule) == 7)
        ReleaseWeatherTextureSurfaces();
    g_currentMap = shen_name(SHEN_NAME(TITL));
    InitializeSprites(1, 0);
    g_currentMap = area;
}

// @verified - d
NativeArcadeSession *GetArcadeSessionContext() {
    return &g_arcadeSession;
}

// @verified - d
void EncodeArcadeRankingScore(std::int32_t score, std::uint8_t *digits) {
    const std::int32_t weights[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
    for (std::int32_t index = 0; index < 4; ++index) {
        std::int32_t count = -1;
        while (score >= 0) {
            score -= weights[index * 2];
            ++count;
        }
        score += weights[index * 2];
        const std::uint8_t upper = static_cast<std::uint8_t>(count << 4);
        count = -1;
        while (score >= 0) {
            score -= weights[index * 2 + 1];
            ++count;
        }
        score += weights[index * 2 + 1];
        digits[index] = static_cast<std::uint8_t>(upper | count);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {

// @verified - d
void OutRunVblankNullsub() {
}

// @verified - d
void OutRunInterruptNullsub() {
    std::int8_t *const outRunCounter = reinterpret_cast<std::int8_t *>(0xac4b0861);
    if (*outRunCounter <= 0)
        *outRunCounter = 16;
}

void BindArcadeHostImports(bool outRun) {
    typedef std::int32_t (*UploadBank)(const std::uint32_t *, std::int32_t);
    const std::uintptr_t imports[kArcadeHostImportCount] = {reinterpret_cast<std::uintptr_t>(dc::sdk::kmRender),
        reinterpret_cast<std::uintptr_t>(dc::sdk::kmFlipFrameBuffer), reinterpret_cast<std::uintptr_t>(&dc::sdk::g_renderSubmissionActive),
        reinterpret_cast<std::uintptr_t>(kmProcessVertexRenderState), reinterpret_cast<std::uintptr_t>(kmSetVertexRenderState),
        reinterpret_cast<std::uintptr_t>(&g_ninjaCurrentContext), reinterpret_cast<std::uintptr_t>(kmStartVertexStrip),
        reinterpret_cast<std::uintptr_t>(kmSetVertex), reinterpret_cast<std::uintptr_t>(kmSetPaletteMode), reinterpret_cast<std::uintptr_t>(kmSetPaletteData),
        reinterpret_cast<std::uintptr_t>(SnapshotPeripheralInputs), reinterpret_cast<std::uintptr_t>(g_peripheralInputSnapshots),
        reinterpret_cast<std::uintptr_t>(kmSetPixelClipping), reinterpret_cast<std::uintptr_t>(kmSetEORCallback),
        reinterpret_cast<std::uintptr_t>(kmRenderTexture), reinterpret_cast<std::uintptr_t>(kmSetGlobalClipping),
        reinterpret_cast<std::uintptr_t>(kmChangeDisplayFilterMode), reinterpret_cast<std::uintptr_t>(kmSetDisplayMode),
        reinterpret_cast<std::uintptr_t>(dc::sdk::syCblCheckCable), reinterpret_cast<std::uintptr_t>(dc::sdk::InitializeSystem),
        reinterpret_cast<std::uintptr_t>(dc::sdk::g_systemConfiguration), reinterpret_cast<std::uintptr_t>(kmSetSystemConfiguration),
        reinterpret_cast<std::uintptr_t>(kmSetUserClipping), reinterpret_cast<std::uintptr_t>(kmSetAutoSortMode),
        reinterpret_cast<std::uintptr_t>(kmSetHSyncCallback), reinterpret_cast<std::uintptr_t>(kmSetHSyncLine),
        reinterpret_cast<std::uintptr_t>(kmGetCurrentScanline), 0, 0, 0, 0, 0, reinterpret_cast<std::uintptr_t>(static_cast<UploadBank>(SOND_UploadBank)),
        reinterpret_cast<std::uintptr_t>(SOND_ExecuteRawAicaCommand), reinterpret_cast<std::uintptr_t>(nlsndExecuteAicaCommand),
        reinterpret_cast<std::uintptr_t>(SOND_ConfigureStreamSlots), reinterpret_cast<std::uintptr_t>(nlsndStreamOpen),
        reinterpret_cast<std::uintptr_t>(nlsndStreamClose), reinterpret_cast<std::uintptr_t>(nlsndStreamPlay),
        reinterpret_cast<std::uintptr_t>(nlsndStreamGetPlayPosition), reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationInit),
        reinterpret_cast<std::uintptr_t>(nlsndStreamGetBuffer), reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetBitsPerSample),
        reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetChannels), reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetEffects),
        reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetDirectLevel), reinterpret_cast<std::uintptr_t>(nlsndStreamConfigure),
        reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetPitch), reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetPan),
        reinterpret_cast<std::uintptr_t>(nlsndStreamConfigurationSetVolume), 0, reinterpret_cast<std::uintptr_t>(kmSetEndOfVertexCallback),
        reinterpret_cast<std::uintptr_t>(kmResetRenderer), reinterpret_cast<std::uintptr_t>(dc::sdk::SubmitFrame), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        reinterpret_cast<std::uintptr_t>(gdFsGetToc), reinterpret_cast<std::uintptr_t>(gdFsClose), reinterpret_cast<std::uintptr_t>(gdFsRead),
        reinterpret_cast<std::uintptr_t>(gdFsSeek), reinterpret_cast<std::uintptr_t>(gdFsReqRd32), reinterpret_cast<std::uintptr_t>(gdFsGetStat),
        reinterpret_cast<std::uintptr_t>(gdFsGetErrStat), reinterpret_cast<std::uintptr_t>(gdFsDaGetInfo), reinterpret_cast<std::uintptr_t>(gdFsIsTrayOpen),
        reinterpret_cast<std::uintptr_t>(gdFsDaPlaySct)};
    std::uintptr_t *const destination = reinterpret_cast<std::uintptr_t *>(kArcadeExecutionRegion);
    std::memcpy(destination, imports, sizeof(imports));
    if (outRun) {
        destination[27] = reinterpret_cast<std::uintptr_t>(kmSetEndOfVertexCallback);
        destination[31] = reinterpret_cast<std::uintptr_t>(kmiDeviceExtension);
        destination[50] = reinterpret_cast<std::uintptr_t>(SOND_ReleaseBank);
        destination[51] = reinterpret_cast<std::uintptr_t>(nlsndExecuteEnvironmentAicaCommand);
    }
}

} // namespace

void GameOutRunModuleCallback(void *taskData) {
    SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    std::int32_t launchEmulator = 1;
    ExecuteAicaCommand(0x100a0, 0, 0);
    NativeArcadeWork work;
    NativeArcadeWork *const arcadeWork = &work;
    NativeArcadeRanking *const rankings = work.rankings;
    std::memset(&work, 0, sizeof(work));
    NativeArcadeSession *const session = GetArcadeSessionContext();
    session->result = -1;
    session->mode[0] = 2;
    session->mode[1] = 0;
    session->field0c = 0;
    session->audioBootrom = GetSystemAudioOutputMode();
    session->scoreBcd = 0;
    session->flags[0] = 0;
    session->flags[1] = 0;
    std::int32_t previousCredits;
    std::int32_t statisticsMode;
    if (static_cast<std::int32_t>(module->previousModule) == 7) {
        if (session->entryId == -1) {
            njTexMemCopy1(rankings, kOutRunModuleDefaultRankings, 70);
            statisticsMode = 2;
        } else {
            statisticsMode = 1;
        }
    } else {
        session->entryId = -1;
        statisticsMode = 0;
    }
    previousCredits = session->entryId;
    if (launchEmulator) {
        PrepareArcadeModule(module);
        HLib::ValidateOrResetHeap();
        void *const arena = reinterpret_cast<void *>(kArcadeExecutionRegion);
        if (HLib::AllocateHeapSpace(arena, 0x760000, shen_name(SHEN_NAME(HARR)))) {
            std::memset(arena, 0, 0x800000);
            dc::sdk::SubmitFrame();
            SnapshotPeripheralInputs();
            dc::sdk::RegisterWeatherTaskCallback(OutRunVblankNullsub);
            SetFrameInterruptCallback(OutRunInterruptNullsub);
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            for (std::int32_t frame = 0; frame < 10; ++frame)
                dc::sdk::SubmitFrame();
            syIntEnableInt(14);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            kmResetRenderer();
            syIntEnableInt(15);
            switch (static_cast<std::uint32_t>(syCblCheck())) {
            case 1:
                kmSetDisplayMode(1, 0, 0, 0);
                break;
            case 2:
                kmSetDisplayMode(2, 0, 0, 0);
                break;
            case 0:
            default:
                kmSetDisplayMode(0, 0, 0, 0);
                break;
            }
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            dc::sdk::SubmitFrame();
            shenLoadFile("outrun", "ichidata.dc", reinterpret_cast<void *>(0xa492c000), 0xffffffffU);
            shenLoadFile("outrun", "fixdata.dc", reinterpret_cast<void *>(0xa494c000), 0xffffffffU);
            shenLoadFile("outrun", "bgdata.dc", reinterpret_cast<void *>(0xa4954000), 0xffffffffU);
            shenLoadFile("outrun", "cgdata.dc", reinterpret_cast<void *>(0x0c500000), 0xffffffffU);
            shenLoadFile("outrun", "outrun.bin", reinterpret_cast<void *>(kArcadeProgramRegion), 0xffffffffU);
            shenLoadFile("outrun", "aicadrv.bin", reinterpret_cast<void *>(0x0ca80000), 0xffffffffU);
            shenLoadFile("outrun", "se.bin", reinterpret_cast<void *>(0x0ca90000), 0xffffffffU);
            shenLoadFile("outrun", "passing.bin", reinterpret_cast<void *>(0x0c900000), 0xffffffffU);
            shenLoadFile("outrun", "sprash.bin", reinterpret_cast<void *>(0x0c980000), 0xffffffffU);
            shenLoadFile("outrun", "magical.bin", reinterpret_cast<void *>(0x0c9f0000), 0xffffffffU);
            shenLoadFile("outrun", "lastwave.bin", reinterpret_cast<void *>(0x0cb30000), 0xffffffffU);
            BindArcadeHostImports(true);
            SOND_FinishAicaDriver();
            SOND_InitializeAicaDriver(reinterpret_cast<const std::uint32_t *>(0x0ca80000), 0);
            SOND_UploadBank(reinterpret_cast<const std::uint32_t *>(0x0ca90000), 0);
            SOND_UploadBank(reinterpret_cast<const std::uint32_t *>(0x0cb30000), 1);
            SOND_UploadBank(reinterpret_cast<const std::uint32_t *>(0x0c900000), 2);
            bool saveRankings;
            if (static_cast<std::int32_t>(module->previousModule) == 7 && session->entryId == -1) {
                saveRankings = false;
            } else {
                for (std::int32_t index = 0; index < 20; ++index) {
                    EncodeArcadeRankingScore(static_cast<std::int32_t>(g_outRunRankings[index].score), rankings[index].scoreBcd);
                    const std::uint8_t *const initials = reinterpret_cast<const std::uint8_t *>(&g_outRunRankings[index].initials);
                    rankings[index].initials[0] = initials[0];
                    rankings[index].initials[1] = initials[1];
                    rankings[index].initials[2] = initials[2];
                    rankings[index].initials[3] = initials[3];
                }
                saveRankings = true;
            }
            arcadeWork->credits = session->entryId;
            typedef void (*ArcadeEntry)(NativeArcadeWork *, NativeArcadeSession *);
            reinterpret_cast<ArcadeEntry>(kArcadeProgramRegion)(&work, session);
            session->entryId = previousCredits - arcadeWork->creditsUsed;
            std::uint32_t score = DecodeArcadeRankingScore(arcadeWork->lastScoreBcd);
            if (saveRankings) {
                for (std::int32_t index = 0; index < 20; ++index) {
                    g_outRunRankings[index].score = DecodeArcadeRankingScore(rankings[index].scoreBcd);
                    std::uint8_t *const initials = reinterpret_cast<std::uint8_t *>(&g_outRunRankings[index].initials);
                    initials[0] = rankings[index].initials[0];
                    initials[1] = rankings[index].initials[1];
                    initials[2] = rankings[index].initials[2];
                    initials[3] = rankings[index].initials[3];
                }
            }
            if (statisticsMode) {
                const std::uint32_t finalScore = DecodeArcadeRankingScore(reinterpret_cast<const std::uint8_t *>(&session->scoreBcd));
                if (score < finalScore)
                    score = finalScore;
                if (g_arcadeStatistics[statisticsMode - 1].highScore < score)
                    g_arcadeStatistics[statisticsMode - 1].highScore = score;
                g_arcadeStatistics[statisticsMode - 1].games += arcadeWork->creditsUsed;
                g_arcadeStatistics[statisticsMode - 1].time += session->flags[1] >> 1;
                AddFrameCount(static_cast<std::int32_t>(session->flags[1] >> 1));
            }
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            syIntEnableInt(14);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            kmResetRenderer();
            syIntEnableInt(15);
            switch (static_cast<std::uint32_t>(syCblCheck())) {
            case 1:
                kmSetDisplayMode(1, 0, 1, 0);
                break;
            case 2:
                kmSetDisplayMode(0x3a, 0, 0, 0);
                kmSetGlobalClipping(0x14, 0xf);
                break;
            case 0:
            default:
                kmSetDisplayMode(0x38, 0, 1, 0);
                kmSetGlobalClipping(0x14, 0xf);
                break;
            }
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            HLib::ValidateOrResetHeap();
        }
        dc::sdk::SubmitFrame();
        Initialization();
    }
    ChangeSystemModule(module->previousModule, 0);
}

void GameSpaceHarrierModuleCallback(void *taskData) {
    SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    std::int32_t launchEmulator = 1;
    ExecuteAicaCommand(0x100a0, 0, 0);
    NativeArcadeWork work;
    NativeArcadeWork *const arcadeWork = &work;
    NativeArcadeRanking *const rankings = work.rankings;
    std::memset(&work, 0, sizeof(work));
    NativeArcadeSession *const session = GetArcadeSessionContext();
    session->result = -1;
    session->mode[0] = 2;
    session->mode[1] = 0;
    session->field0c = 0;
    session->audioBootrom = GetSystemAudioOutputMode();
    session->scoreBcd = 0;
    session->flags[0] = 0;
    session->flags[1] = 0;
    std::int32_t statisticsMode;
    if (static_cast<std::int32_t>(module->previousModule) == 7) {
        if (session->entryId == -1) {
            njTexMemCopy1(rankings, kSpaceHarrierModuleDefaultRankings, 70);
            statisticsMode = 2;
        } else {
            statisticsMode = 1;
        }
    } else {
        session->entryId = -1;
        statisticsMode = 0;
    }
    const std::int32_t previousCredits = session->entryId;
    if (launchEmulator) {
        PrepareArcadeModule(module);
        HLib::ValidateOrResetHeap();
        void *const arena = reinterpret_cast<void *>(kArcadeExecutionRegion);
        if (HLib::AllocateHeapSpace(arena, 0x760000, shen_name(SHEN_NAME(HARR)))) {
            std::memset(arena, 0, 0x800000);
            bool saveRankings;
            if (static_cast<std::int32_t>(module->previousModule) == 7 && session->entryId == -1) {
                saveRankings = false;
            } else {
                for (std::int32_t index = 0; index < 20; ++index) {
                    EncodeArcadeRankingScore(static_cast<std::int32_t>(g_harrierRankings[index].score), rankings[index].scoreBcd);
                    const std::uint8_t *const initials = reinterpret_cast<const std::uint8_t *>(&g_harrierRankings[index].initials);
                    rankings[index].initials[0] = initials[0];
                    rankings[index].initials[1] = initials[1];
                    rankings[index].initials[2] = initials[2];
                    rankings[index].initials[3] = initials[3];
                }
                saveRankings = true;
            }
            arcadeWork->credits = session->entryId;
            dc::sdk::SubmitFrame();
            SnapshotPeripheralInputs();
            dc::sdk::RegisterWeatherTaskCallback(HarrierVblankNullsub);
            SetFrameInterruptCallback(HarrierVblankNullsub);
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            for (std::int32_t frame = 0; frame < 10; ++frame)
                dc::sdk::SubmitFrame();
            syIntEnableInt(14);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            kmResetRenderer();
            syIntEnableInt(15);
            switch (static_cast<std::uint32_t>(syCblCheck())) {
            case 1:
                kmSetDisplayMode(1, 0, 0, 0);
                break;
            case 2:
                kmSetDisplayMode(2, 0, 0, 0);
                break;
            case 0:
            default:
                kmSetDisplayMode(0, 0, 0, 0);
                break;
            }
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            dc::sdk::SubmitFrame();
            shenLoadFile("harrier", "HARRIER.PKS", reinterpret_cast<void *>(0x0c508000), 0xffffffffU);
            const std::uint8_t *const archive = reinterpret_cast<const std::uint8_t *>(0x0c508010);
            const NativeArcadeArchiveEntry *entry =
                reinterpret_cast<const NativeArcadeArchiveEntry *>(archive + reinterpret_cast<const NativeArcadeArchiveHeader *>(archive)->entriesOffset);
            const std::uint8_t *const compressed = archive + entry->dataOffset;
            const std::uint32_t bootBytes = am2ReadUnalignedLe32(compressed, static_cast<std::int32_t>(entry->compressedBytes));
            DecompressMemoryFile(compressed, reinterpret_cast<void *>(0x0c800000), bootBytes);
            syIntEnableInt(15);
            CopyEventOverlayWords(reinterpret_cast<void *>(0xa452c000), reinterpret_cast<const void *>(0x0c800000), bootBytes);
            syIntEnableInt(0);
            ++entry;
            DecompressMemoryFile(archive + entry->dataOffset, reinterpret_cast<void *>(kArcadeProgramRegion), 0x80000);
            ++entry;
            DecompressMemoryFile(archive + entry->dataOffset, reinterpret_cast<void *>(0x0c810000), 0x10000);
            DecompressMemoryFile(archive + entry[1].dataOffset, reinterpret_cast<void *>(0x0c820000), 0x200000);
            SOND_InitializeAicaDriver(reinterpret_cast<const std::uint32_t *>(0x0c810000), 0);
            SOND_UploadBank(reinterpret_cast<const std::uint32_t *>(0x0c820000), 0);
            BindArcadeHostImports(false);
            typedef void (*ArcadeEntry)(NativeArcadeWork *, NativeArcadeSession *);
            reinterpret_cast<ArcadeEntry>(kArcadeProgramRegion)(&work, session);
            session->entryId = previousCredits - arcadeWork->creditsUsed;
            std::uint32_t score = DecodeArcadeRankingScore(arcadeWork->lastScoreBcd);
            if (saveRankings) {
                for (std::int32_t index = 0; index < 20; ++index) {
                    g_harrierRankings[index].score = DecodeArcadeRankingScore(rankings[index].scoreBcd);
                    std::uint8_t *const initials = reinterpret_cast<std::uint8_t *>(&g_harrierRankings[index].initials);
                    initials[0] = rankings[index].initials[0];
                    initials[1] = rankings[index].initials[1];
                    initials[2] = rankings[index].initials[2];
                    initials[3] = rankings[index].initials[3];
                }
            }
            if (statisticsMode) {
                const std::uint32_t finalScore = DecodeArcadeRankingScore(reinterpret_cast<const std::uint8_t *>(&session->scoreBcd));
                if (score < finalScore)
                    score = finalScore;
                if (g_arcadeStatistics[statisticsMode - 1].highScore < score)
                    g_arcadeStatistics[statisticsMode - 1].highScore = score;
                g_arcadeStatistics[statisticsMode - 1].games += arcadeWork->creditsUsed;
                g_arcadeStatistics[statisticsMode - 1].time += session->flags[1] >> 1;
                AddFrameCount(static_cast<std::int32_t>(session->flags[1] >> 1));
            }
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            syIntEnableInt(14);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            kmResetRenderer();
            syIntEnableInt(15);
            switch (static_cast<std::uint32_t>(syCblCheck())) {
            case 1:
                kmSetDisplayMode(1, 0, 1, 0);
                break;
            case 2:
                kmSetDisplayMode(0x3a, 0, 0, 0);
                kmSetGlobalClipping(0x14, 0xf);
                break;
            case 0:
            default:
                kmSetDisplayMode(0x38, 0, 1, 0);
                kmSetGlobalClipping(0x14, 0xf);
                break;
            }
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            HLib::ValidateOrResetHeap();
        }
        dc::sdk::SubmitFrame();
        Initialization();
    }
    ChangeSystemModule(module->previousModule, 0);
}

namespace {
inline void SetArcadeInterruptMask(std::uint32_t level) {
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status = (status & ~0xf0U) | level;
    asm volatile("ldc %0,sr" : : "r"(status));
}

struct HangOnRankingTable {
    ArcadeRankingRecord records[20];
};
} // namespace

void GameHangOnModuleCallback(void *taskData) {
    SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    std::int32_t launchEmulator = 1;
    std::uint32_t configuration[0x44 / 4];
    std::uint32_t shared[0x4b4 / 4];
    std::memset(shared, 0, sizeof(shared));
    {
        std::size_t size = sizeof(configuration);
        std::memset(configuration, 0, size);
    }
    NativeArcadeSession *const session = GetArcadeSessionContext();
    session->result = -1;
    session->mode[0] = 2;
    session->mode[1] = 0;
    session->field0c = 0;
    session->audioBootrom = GetSystemAudioOutputMode();
    session->scoreBcd = 0;
    session->flags[0] = 0;
    session->flags[1] = 0;
    std::int32_t previousEntry;
    std::int32_t statisticsMode;
    if (static_cast<std::int32_t>(module->previousModule) == 7) {
        if (session->entryId == -1) {
            shared[0] = launchEmulator;
            statisticsMode = 2;
        } else {
            statisticsMode = 1;
        }
    } else {
        session->entryId = -1;
        statisticsMode = 0;
    }
    previousEntry = session->entryId;
    if (launchEmulator) {
        PrepareArcadeModule(module);
        HLib::ValidateOrResetHeap();
        void *const arena = reinterpret_cast<void *>(kHangOnExecutionRegion);
        if (HLib::AllocateHeapSpace(arena, 0x760000, ARCADE_HANGON_STORAGE)) {
            std::memset(arena, 0, 0x800000);
            std::int32_t frame = 0;
            dc::sdk::SubmitFrame();
            SnapshotPeripheralInputs();
            SetFrameInterruptCallback(HangOnVblankNullsub);
            dc::sdk::RegisterWeatherTaskCallback(HangOnVblankNullsub);
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            for (; frame < 10; ++frame)
                dc::sdk::SubmitFrame();
            SetArcadeInterruptMask(0xe0);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            SetArcadeInterruptMask(0);
            kmResetRenderer();
            SetArcadeInterruptMask(0xf0);
            SetEventCacheControl(0x105);
            SetArcadeInterruptMask(0);
            dc::sdk::SubmitFrame();
            shenLoadFile("hangon", "hangon.emu", arena, 0xffffffffU);
            shenLoadFile("hangon", "HANGON.BIN", reinterpret_cast<void *>(kHangOnProgramRegion), 0xffffffffU);
            bool saveRankings;
            if (static_cast<std::int32_t>(module->previousModule) != 7 || session->entryId != -1) {
                const ArcadeRankingRecord *record = &g_hangOnRankings[0];
                for (std::int32_t index = 0; index < 20; ++index, ++record) {
                    std::uint8_t *const entry = reinterpret_cast<std::uint8_t *>(shared) + (index * 12 + 4);
                    EncodeArcadeRankingScore(static_cast<std::int32_t>(record->score), entry);
                    const std::uint8_t *const initials = reinterpret_cast<const std::uint8_t *>(&record->initials);
                    entry[4] = initials[0];
                    entry[5] = initials[1];
                    entry[6] = initials[2];
                    entry[7] = initials[3];
                    *reinterpret_cast<std::uint32_t *>(entry + 8) = record->extra;
                }
                saveRankings = true;
            } else {
                saveRankings = false;
            }
            g_hangOnModuleBuffers.configuration = configuration;
            g_hangOnModuleBuffers.shared = shared;
            typedef void (*HangOnEntry)(NativeHangOnBuffers *, NativeArcadeSession *);
            reinterpret_cast<HangOnEntry>(kHangOnExecutionRegion + 10)(&g_hangOnModuleBuffers, session);
            const std::uint32_t score = session->scoreBcd;
            session->scoreBcd = (score << 24) | (((score >> 8) & 0xff) << 16) | ((score & 0xff0000U) >> 8) | (score >> 24);
            const std::uint32_t decodedScore = DecodeArcadeRankingScore(reinterpret_cast<const std::uint8_t *>(&session->scoreBcd));
            if (saveRankings) {
                *reinterpret_cast<HangOnRankingTable *>(&g_hangOnRankings[0]) = *reinterpret_cast<const HangOnRankingTable *>(shared + 1);
                for (std::int32_t index = 0; index < 20; ++index)
                    g_hangOnRankings[index].score = DecodeArcadeRankingScore(reinterpret_cast<std::uint8_t *>(shared) + (index * 12 + 4));
            }
            if (statisticsMode) {
                if (decodedScore > g_arcadeStatistics[statisticsMode + 1].highScore)
                    g_arcadeStatistics[statisticsMode + 1].highScore = decodedScore;
                previousEntry -= session->entryId;
                g_arcadeStatistics[statisticsMode + 1].games += previousEntry;
                g_arcadeStatistics[statisticsMode + 1].time += session->flags[1] >> 1;
                AddFrameCount(static_cast<std::int32_t>(session->flags[1] >> 1));
            }
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            SetArcadeInterruptMask(0xe0);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            SetArcadeInterruptMask(0);
            kmResetRenderer();
            SetArcadeInterruptMask(0xf0);
            SetEventCacheControl(0x105);
            SetArcadeInterruptMask(0);
            dc::sdk::SubmitFrame();
            HLib::ValidateOrResetHeap();
        }
        dc::sdk::SubmitFrame();
        Initialization();
    }
    ChangeSystemModule(module->previousModule, 0);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t emuSetNextState(std::int32_t state, std::int32_t argument) {
    const SystemModule modules[4] = {
        SHENMUE_SYSTEM_MODULE(HangOn), SHENMUE_SYSTEM_MODULE(SpaceHarrier), SHENMUE_SYSTEM_MODULE(HangOn), SHENMUE_SYSTEM_MODULE(SpaceHarrier)};
    std::int32_t result = 0;
    switch (state) {
    case 0:
    case 1:
    case 2:
    case 3:
        shenQueueChangeModuleAndScene(modules[state], g_actNumber, g_currentMap, argument);
        break;
    case 10:
        g_arcadeSession.entryId = argument;
        result = argument;
        break;
    case 11:
        result = g_arcadeSession.entryId;
        break;
    case 20:
        result = static_cast<std::int32_t>(DecodeArcadeRankingScore(reinterpret_cast<const std::uint8_t *>(&g_arcadeSession.scoreBcd)));
        break;
    case 21:
        result = static_cast<std::int32_t>(g_arcadeSession.flags[0]);
        break;
    }
    return result;
}
#else
// @verified - d
std::int32_t emuSetNextState(std::int32_t state, std::int32_t argument) {
    const SystemModule modules[4] = {
        SHENMUE_SYSTEM_MODULE(HangOn), SHENMUE_SYSTEM_MODULE(SpaceHarrier), SHENMUE_SYSTEM_MODULE(HangOn), SHENMUE_SYSTEM_MODULE(SpaceHarrier)};
    switch (state) {
    case 0:
    case 1:
    case 2:
    case 3:
        shenQueueChangeModuleAndScene(modules[state], g_actNumber, g_currentMap, argument);
        return 0;
    case 10:
        g_arcadeSession.entryId = argument;
        return argument;
    case 11:
        return g_arcadeSession.entryId;
    case 20:
        return static_cast<std::int32_t>(DecodeArcadeRankingScore(reinterpret_cast<const std::uint8_t *>(&g_arcadeSession.scoreBcd)));
    case 21:
        return static_cast<std::int32_t>(g_arcadeSession.flags[0]);
    default:
        return 0;
    }
}
#endif

void GameAfterBurnerModuleCallback(void *taskData) {
    SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    ExecuteAicaCommand(0x100a0, 0, 0);
    std::int32_t launchEmulator = 0;
    NativeArcadeWork work;
    NativeArcadeWork *const arcadeWork = &work;
    NativeArcadeRanking *const rankings = work.rankings;
    std::memset(&work, 0, sizeof(work));
    NativeArcadeSession *const session = GetArcadeSessionContext();
    session->result = -1;
    session->mode[0] = 2;
    session->mode[1] = 0;
    session->field0c = 0;
    session->audioBootrom = GetSystemAudioOutputMode();
    session->scoreBcd = 0;
    session->flags[0] = 0;
    session->flags[1] = 0;
    std::int32_t previousCredits;
    std::int32_t statisticsMode;
    if (static_cast<std::int32_t>(module->previousModule) == 7) {
        if (session->entryId == -1) {
            njTexMemCopy1(rankings, kAfterBurnerModuleDefaultRankings, 70);
            statisticsMode = 2;
        } else {
            statisticsMode = 1;
        }
    } else {
        session->entryId = -1;
        statisticsMode = 0;
    }
    previousCredits = session->entryId;
    if (launchEmulator) {
        PrepareArcadeModule(module);
        HLib::ValidateOrResetHeap();
        void *const arena = reinterpret_cast<void *>(kArcadeExecutionRegion);
        if (HLib::AllocateHeapSpace(arena, 0x760000, shen_name(SHEN_NAME(HARR)))) {
            std::memset(arena, 0, 0x800000);
            dc::sdk::SubmitFrame();
            SnapshotPeripheralInputs();
            dc::sdk::RegisterWeatherTaskCallback(AfterBurnerVblankNullsub);
            SetFrameInterruptCallback(AfterBurnerVblankNullsub);
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            for (std::int32_t frame = 0; frame < 10; ++frame)
                dc::sdk::SubmitFrame();
            syIntEnableInt(14);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            kmResetRenderer();
            syIntEnableInt(15);
            switch (static_cast<std::uint32_t>(syCblCheck())) {
            case 1:
                kmSetDisplayMode(1, 0, 0, 0);
                break;
            case 2:
                kmSetDisplayMode(2, 0, 0, 0);
                break;
            case 0:
            default:
                kmSetDisplayMode(0, 0, 0, 0);
                break;
            }
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            dc::sdk::SubmitFrame();
            shenLoadFile("outrun", "ICHIDATA.DC", reinterpret_cast<void *>(0xa492c000), 0xffffffffU);
            shenLoadFile("outrun", "FIXDATA.DC", reinterpret_cast<void *>(0xa494c000), 0xffffffffU);
            shenLoadFile("outrun", "BGDATA.DC", reinterpret_cast<void *>(0xa4954000), 0xffffffffU);
            shenLoadFile("outrun", "CGDATA.DC", reinterpret_cast<void *>(0x0c500000), 0xffffffffU);
            shenLoadFile("outrun", "MAIN.BIN", reinterpret_cast<void *>(kArcadeProgramRegion), 0xffffffffU);
            BindArcadeHostImports(false);
            bool saveRankings;
            if (static_cast<std::int32_t>(module->previousModule) == 7 && session->entryId == -1) {
                saveRankings = false;
            } else {
                for (std::int32_t index = 0; index < 20; ++index) {
                    EncodeArcadeRankingScore(static_cast<std::int32_t>(g_afterBurnerRankings[index].score), rankings[index].scoreBcd);
                    const std::uint8_t *const initials = reinterpret_cast<const std::uint8_t *>(&g_afterBurnerRankings[index].initials);
                    rankings[index].initials[0] = initials[0];
                    rankings[index].initials[1] = initials[1];
                    rankings[index].initials[2] = initials[2];
                    rankings[index].initials[3] = initials[3];
                }
                saveRankings = true;
            }
            arcadeWork->credits = session->entryId;
            typedef void (*ArcadeEntry)(NativeArcadeWork *, NativeArcadeSession *);
            reinterpret_cast<ArcadeEntry>(kArcadeProgramRegion)(&work, session);
            session->entryId = previousCredits - arcadeWork->creditsUsed;
            std::uint32_t score = DecodeArcadeRankingScore(arcadeWork->lastScoreBcd);
            if (saveRankings) {
                for (std::int32_t index = 0; index < 20; ++index) {
                    g_afterBurnerRankings[index].score = DecodeArcadeRankingScore(rankings[index].scoreBcd);
                    std::uint8_t *const initials = reinterpret_cast<std::uint8_t *>(&g_afterBurnerRankings[index].initials);
                    initials[0] = rankings[index].initials[0];
                    initials[1] = rankings[index].initials[1];
                    initials[2] = rankings[index].initials[2];
                    initials[3] = rankings[index].initials[3];
                }
            }
            if (statisticsMode) {
                const std::uint32_t finalScore = DecodeArcadeRankingScore(reinterpret_cast<const std::uint8_t *>(&session->scoreBcd));
                if (score < finalScore)
                    score = finalScore;
                if (g_arcadeStatistics[statisticsMode - 1].highScore < score)
                    g_arcadeStatistics[statisticsMode - 1].highScore = score;
                g_arcadeStatistics[statisticsMode - 1].games += arcadeWork->creditsUsed;
                g_arcadeStatistics[statisticsMode - 1].time += session->flags[1] >> 1;
                AddFrameCount(static_cast<std::int32_t>(session->flags[1] >> 1));
            }
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            dc::sdk::SubmitFrame();
            syIntEnableInt(14);
            FlushEventOperandCache();
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            kmResetRenderer();
            syIntEnableInt(15);
            switch (static_cast<std::uint32_t>(syCblCheck())) {
            case 1:
                kmSetDisplayMode(1, 0, 1, 0);
                break;
            case 2:
                kmSetDisplayMode(0x3a, 0, 0, 0);
                kmSetGlobalClipping(0x14, 0xf);
                break;
            case 0:
            default:
                kmSetDisplayMode(0x38, 0, 1, 0);
                kmSetGlobalClipping(0x14, 0xf);
                break;
            }
            SetEventCacheControl(0x105);
            syIntEnableInt(0);
            HLib::ValidateOrResetHeap();
        }
        dc::sdk::SubmitFrame();
        Initialization();
    }
    ChangeSystemModule(module->previousModule, 0);
}
#endif

namespace {
extern "C" void njSetBackColor(std::uint32_t first, std::uint32_t second, std::uint32_t third);

// @verified - d
void ModuleNoOp(void *) {
}

// @verified - d
void SampleModuleNoOp(void *) {
}

// @verified - d
void Module24NoOp(void *) {
}

// @verified - d
void DiagnosticBackgroundNoOp(void *) {
}

// @verified - d
void DiagnosticBackgroundModule(void *) {
    njSetBackColor(0xff00af00, 0xffaf0000, 0xff0000af);
    HLib::SetCurrentTaskTo(DiagnosticBackgroundNoOp);
}

// @verified - d
void ChangeToTitleControlTask(void *) {
    ChangeSystemModule(SHENMUE_SYSTEM_MODULE(Title), 0);
}

// @verified - d
void ChangeModuleTask_Hot(void *taskData) {
    InitializeShenmue();
    const SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    if (module->previousModule == SHENMUE_SYSTEM_MODULE(Cold))
        ChangeSystemModule(SHENMUE_SYSTEM_MODULE(Logo), 0);
    else
        ChangeSystemModule(SHENMUE_SYSTEM_MODULE(Title), 0);
}

// @verified - d
void ChangeModuleTask_Callback(void *taskData) {
    HLib::FreeUnusedHeapMem();
    ClearSTCH();
    const SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    HLib::SetCurrentTaskTo(systemModuleDefinitions[static_cast<std::size_t>(module->currentModule)].callback);
}
} // namespace

#if SHENMUE_S2_GCC295
const SystemModuleDefinition systemModuleDefinitions[SystemModule_Count] = {
#else
const std::array<SystemModuleDefinition, static_cast<std::size_t>(SystemModule::Count)> systemModuleDefinitions {{
#endif
    {COLB_Module, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {ChangeModuleTask_Init, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {ChangeToTitleControlTask, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {ChangeModuleTask_Hot, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {TitleModule, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {DiagnosticBackgroundModule, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {ModuleNoOp, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {GameSys, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {SampleModuleNoOp, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {GameOutRunModuleCallback, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {GameAfterBurnerModuleCallback, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {GameHangOnModuleCallback, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {GameSpaceHarrierModuleCallback, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {StaffRoll, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {reinterpret_cast<HLib::TaskCallback>(kExternalModuleExecutionRegion), SHENMUE_SYSTEM_MODULE_NULL,
        reinterpret_cast<void *>(kExternalModuleExecutionRegion)},
    {Module24NoOp, SHENMUE_SYSTEM_MODULE_NULL, reinterpret_cast<void *>(kModuleExecutionRegion)},
#if SHENMUE_S2_GCC295
};
#else
}};

#endif
void ResetHangOnRankings();
void ResetSpaceHarrierRankings();
void ResetOutRunRankings();
void ResetAfterBurnerRankings();

// @verified - d
void EventSystemAlternateNullsub() {
}

// @verified - d
bool ExecuteSystemModule(SystemModule module, std::int32_t entry) {
    bool result;
    if (static_cast<std::uint32_t>(module) >= (sizeof(systemModuleDefinitions) / sizeof(systemModuleDefinitions[0]))) {
        result = false;
    } else {
        systemModuleTask = HLib::EnqueueTaskWithoutParameter(systemModuleDefinitions[module].callback, 0, 2, static_cast<std::int32_t>(TASK_MODU));
        systemModuleTaskData.work = systemModuleWorkStorage;
        HLib::AssignTaskDataToTask(systemModuleTask, &systemModuleTaskData);
        systemModuleTaskData.currentModule = module;
        systemModuleTaskData.currentEntry = entry;
        result = true;
    }
    return result;
}

// @verified - d
bool ChangeSystemModule(SystemModule module, std::int32_t entry) {
    bool result;
    if (static_cast<std::uint32_t>(module) >= (sizeof(systemModuleDefinitions) / sizeof(systemModuleDefinitions[0]))) {
        result = false;
    } else {
        HLib::UnpauseTaskEnqueue();
        HLib::AssignCallbackForTask(systemModuleTask, ChangeModuleTask_Callback);
        HLib::SetupTimedTask(systemModuleTask, 1);
        g_frameRateVblankInterval = g_savedFrameRateVblankInterval;
        SystemModuleTaskData *const data = static_cast<SystemModuleTaskData *>(HLib::GetTaskParameter(systemModuleTask));
        data->previousModule = data->currentModule;
        data->previousEntry = data->currentEntry;
        data->currentModule = module;
        data->currentEntry = entry;
        HLib::DestroyTasksFromIndex(1);
        g_loadingFlag = 0;
        result = true;
    }
    return result;
}

// @verified - d
void EventSystemNullsub() {
}

// @verified - d
void EventRuntimeNullsub() {
}

// @verified - d
void ResetArcadeRankings() {
    ResetHangOnRankings();
    ResetSpaceHarrierRankings();
    ResetOutRunRankings();
    ResetAfterBurnerRankings();
}

// @verified - d
std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits) {
    const std::int32_t weights[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
    std::uint32_t score = 0;
    for (std::int32_t index = 0; index < 4; ++index) {
        score += (digits[index] >> 4) * weights[index * 2];
        score += (digits[index] & 15) * weights[index * 2 + 1];
    }
    return score;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void ConfigureCpuCache(std::uint32_t control) asm("_shenmue_dc_configure_cpu_cache");
extern "C" void njSetBorderColor(std::uint32_t colour);
namespace HLib { void DisableMemoryMonitor(); }

static std::uint32_t MovieFileSize(const char *directory, const char *filename) {
    DiscFileLocation *const location = FindDiscFileLocation(directory, filename);
    const std::uint32_t size = GetDiscFileLocationSize(location);
    ReleaseDiscFileLocation(location);
    return size;
}

static void ResetMovieInterrupts() {
    typedef void (*CacheConfigurator)(std::uint32_t);
    const CacheConfigurator configure = reinterpret_cast<CacheConfigurator>(reinterpret_cast<std::uintptr_t>(&ConfigureCpuCache) | 0xa0000000U);
    configure(0);
    *reinterpret_cast<volatile std::uint32_t *>(0xa05f6914) = 0;
    *reinterpret_cast<volatile std::uint32_t *>(0xa05f6910) = 0;
    *reinterpret_cast<volatile std::uint32_t *>(0xa05f6900) = 0x1fffffU;
    *reinterpret_cast<volatile std::uint32_t *>(0xa05f6908) = 0xffffffffU;
}

void ChangeToMovieTask(std::int32_t movie) {
    *reinterpret_cast<std::uint32_t **>(0x8c0201f8) = &g_nativePlayerStats.gameConfigurationFlags;
    HLib::FreeUnusedHeapMem();
    HLib::DisableMemoryMonitor();
    const std::uint32_t soundMode = GetSystemAudioOutputMode();
    const char *const directory = "/scene/04/movie";
    const std::uint32_t movieSize = MovieFileSize(directory, "movie.bin");
    HLib::AllocateHeapSpace(reinterpret_cast<void *>(0x8ca00000), movieSize, SHEN_NAME4('M', 'O', 'V', 'I'));
    shenLoadFile(directory, "movie.bin", reinterpret_cast<void *>(0x8ca00000), movieSize);
    const std::uint32_t monitorSize = MovieFileSize(directory, "MON.BIN");
    void *const monitor = HLib::AllocateTaskData(monitorSize, SHEN_NAME4('M', 'O', 'V', 'I'));
    void *const monitorSource = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(monitor) | 0x80000000U);
    shenLoadFile(directory, "MON.BIN", monitorSource, monitorSize);
    dc::sdk::njSetBackColor(0, 0, 0);
    njSetBorderColor(0);
    *reinterpret_cast<volatile std::uint32_t *>(0xa05f80e8) |= 8;
    ResetMovieInterrupts();
    std::memcpy(reinterpret_cast<void *>(0x8c010000), monitorSource, monitorSize);
    volatile std::uint32_t *const entry = reinterpret_cast<volatile std::uint32_t *>(0xac020000);
    entry[0] = 0x400bd002U;
    entry[1] = entry[2] = 0x00090009U;
    entry[3] = 0x8ca00000U;
    entry[4] = 0;
    typedef void (*CacheConfigurator)(std::uint32_t);
    reinterpret_cast<CacheConfigurator>(reinterpret_cast<std::uintptr_t>(&ConfigureCpuCache) | 0xa0000000U)(0);
    *reinterpret_cast<volatile std::uint32_t *>(0xac02011c) = (soundMode << 4) | static_cast<std::uint32_t>(movie) | 0x12345600U;
    reinterpret_cast<void (*)()>(0x8c010000)();
}

#endif

} // namespace s2
} // namespace shenmue

#endif
