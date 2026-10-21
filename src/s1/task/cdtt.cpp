#include "s1/task/cdtt.hpp"

#include "s1/boot.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/simple_shader.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/time.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"

#include <array>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

void *LoadGzippedSpriteIntoHeapMem(const char *filename, std::int32_t allocationToken, std::uint32_t *decompressedSize);

CDTTMode g_cdttMode = CDTTMode::None;
const std::byte *g_cdttLoadScreenChunk = nullptr;
HLib::Task *g_cdttTask = nullptr;
std::int16_t g_cdttAlpha = -1;
std::int32_t g_cdttLoadNumCount = 0;
std::uint8_t g_cdttIntroDateTimeSet = 0;

CDTTState g_cdttState{};

namespace {

constexpr std::int32_t make_shen_name(char a, char b, char c, char d) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::uint8_t>(a)) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) << 8) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(c)) << 16) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d)) << 24));
}

constexpr std::int32_t kCDTTTaskToken = SHEN_NAME(CDTT);
constexpr std::int32_t kTexnToken = SHEN_NAME(TEXN);

// @verified - p
void CDTT_DrawLoadScreen(HLib::TaskData *) {
}
std::int32_t g_cdttHeaderSurface = -1;
std::int32_t g_cdttSpecialDateSurface = -1;

std::array<std::int32_t, 30> g_cdttLoadNumSurfaces = [] {
    std::array<std::int32_t, 30> surfaces{};
    surfaces.fill(-1);
    return surfaces;
}();

constexpr std::size_t kDebugMapIdIndex = kMapIdList.size() - 1;


constexpr std::array<const char *, 4> kSpecialDateArchives{"LD_0101.GZ", "LD_1224.GZ", "LD_1225.GZ", "LD_1231.GZ"};


std::int32_t g_cdttViewLoadIndex = -1;
std::int32_t g_cdttViewSpecialDateIndex = 0;
const char *g_cdttViewFilename = nullptr;

bool IsSkipMode(CDTTMode mode) {
    return mode == CDTTMode::SkipLoadScreenRender || mode == CDTTMode::SkipLoadScreenRenderAlt;
}


bool AreaEquals(const char *text, std::uint32_t name) {
    for (std::uint32_t index = 0; index < 4; ++index) {
        char left = text[index];
        char right = static_cast<char>(name >> (index * 8U));
        if (left >= 'a' && left <= 'z')
            left = static_cast<char>(left - ' ');
        if (right >= 'a' && right <= 'z')
            right = static_cast<char>(right - ' ');
        if (left != right)
            return false;
    }
    return text[4] == '\0';
}

// The TEXN chunks of a sprite archive into consecutive surface slots; returns how many loaded.
std::int32_t LoadArchiveSurfaces(const char *filename, std::int32_t token, std::int32_t *surfaces, std::size_t capacity) {
    std::uint32_t size = 0;
    auto *const bytes = static_cast<std::byte *>(LoadGzippedSpriteIntoHeapMem(filename, token, &size));
    if (!bytes)
        return 0;
    std::size_t offset = 0;
    std::size_t count = 0;
    while (offset + 8 <= size && count < capacity) {
        std::int32_t chunkToken = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&chunkToken, bytes + offset, sizeof(chunkToken));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (chunkToken != kTexnToken || chunkSize < 8 || chunkSize > size - offset)
            break;
        surfaces[count++] = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(bytes), 0);
    return static_cast<std::int32_t>(count);
}

} // namespace

const CDTTState &CDTT_GetState() {
    const LoadScreenPresentation &screen = g_loadScreenPresentation;
    g_cdttState.task = g_cdttTask;
    g_cdttState.mode = g_cdttMode;
    g_cdttState.alpha = g_cdttAlpha;
    g_cdttState.fadingOut = g_cdttTask && g_cdttTask->mainTaskCallbackFnPtr == CDTT_FreeCallback;
    g_cdttState.presentationAuthored = static_cast<std::uint8_t>(screen.imageSet != 0);
    g_cdttState.renderSuppressed = static_cast<std::uint8_t>(g_isRenderSuppressed != 0);
    g_cdttState.surfaces.primaryHeader = g_cdttHeaderSurface;
    g_cdttState.surfaces.specialDateHeader = g_cdttSpecialDateSurface;
    for (std::size_t index = 0; index < g_cdttState.surfaces.digitSurfaces.size(); ++index)
        g_cdttState.surfaces.digitSurfaces[index] = g_cdttLoadNumSurfaces[index];
    g_cdttState.surfaces.digitSurfaceCount = static_cast<std::uint8_t>(g_cdttLoadNumCount);

    constexpr std::array<std::size_t, 9> kWindowForWidget{0, 1, 2, 3, 5, 4, 6, 7, 8};
    for (std::size_t index = 0; index < g_cdttState.widgets.size(); ++index) {
        const std::size_t window = kWindowForWidget[index];
        CDTTWidgetState &widget = g_cdttState.widgets[index];
        widget.id = static_cast<CDTTWidgetId>(index);
        widget.authoredOrder = static_cast<std::uint8_t>(index);
        widget.authored = static_cast<std::uint8_t>(screen.widgetBindings[window] >= 0);
        widget.visible = static_cast<std::uint8_t>(screen.windows[window] && screen.windows[window]->alpha > 0.0f);
    }
    UICdttPresentationText &text = g_cdttState.text;
    std::snprintf(text.date, sizeof(text.date), "%s", screen.date.data());
    std::snprintf(text.time, sizeof(text.time), "%s", screen.time.data());
    std::snprintf(text.location, sizeof(text.location), "%s", screen.area.data());
    std::snprintf(text.subLocation, sizeof(text.subLocation), "%s", screen.subArea.data());
    std::snprintf(text.headerPath, sizeof(text.headerPath), "%s", g_cdttViewFilename ? g_cdttViewFilename : "");
    text.locationIndex = static_cast<std::int16_t>(g_cdttViewLoadIndex);
    text.specialDateIndex = static_cast<std::uint8_t>(g_cdttViewSpecialDateIndex);
    text.loadingTextVisible = static_cast<std::uint8_t>(screen.windows[8] && screen.windows[8]->alpha > 0.0f);
    return g_cdttState;
}


void CDTT_SetMode(CDTTMode mode) {
    if (g_cdttMode != CDTTMode::TitleOrIntroDateSetup) {
        g_cdttMode = mode;
        if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(g_cdttMode) - 4U) > 1U)
            return;
    }
    g_displayActNumber = 0;
}


void CDTT_SetPresentationPayload(const UICdttPresentationPayload &payload) {
    std::int32_t specialDateIndex = g_cdttMode == CDTTMode::SpecialDateLoadScreen ? payload.specialDateIndex : 0;
    if (g_cdttMode == CDTTMode::SpecialDateLoadScreen && !specialDateIndex) {
        if (payload.month == 1 && payload.day == 1)
            specialDateIndex = 1;
        else if (payload.month == 12 && payload.day == 24)
            specialDateIndex = 2;
        else if (payload.month == 12 && payload.day == 25)
            specialDateIndex = 3;
        else if (payload.month == 12 && payload.day == 31)
            specialDateIndex = 4;
    }
    const std::int32_t loadIndex = payload.locationIndex;
    const char *filename = nullptr;
    if (specialDateIndex)
        filename = kSpecialDateArchives[static_cast<std::size_t>(specialDateIndex - 1)];
    else if (loadIndex >= 0 && static_cast<std::size_t>(loadIndex) < kDebugMapIdIndex)
        filename = kMapIdList[static_cast<std::size_t>(loadIndex)].filename;
    g_cdttViewLoadIndex = loadIndex;
    g_cdttViewSpecialDateIndex = specialDateIndex;
    g_cdttViewFilename = filename;
    g_loadScreenPresentation.imageSet = 0;
    LoadLoadingScreenImage(static_cast<std::int32_t>(payload.areaId), loadIndex, specialDateIndex, payload.year, payload.month, payload.day, payload.dayOfWeek,
        payload.hours, payload.minutes, payload.hours >= 12, filename, g_cdttMode);
}



void IncrementCurrentDateTime_Wrapper() {
    IncrementCurrentDateTime(g_currentDateTime, {});
    if (g_sceneId == 80 || static_cast<std::uint8_t>(static_cast<std::uint8_t>(g_cdttMode) - 4U) > 1U)
        return;
    if (g_sceneId == 1 && AreaEquals("OP00", g_areaId)) {
        GameDateTime intro{.year = 86, .month = 11, .day = 29, .dayOfWeek = 0, .hours = 16, .minutes = 0, .seconds = 0, .reserved = 0};
        SetNewDateTime(intro);
        g_cdttIntroDateTimeSet = 1;
        return;
    }
    if (g_cdttMode != CDTTMode::GameplayDateSetup)
        return;
    if (static_cast<std::int8_t>(g_currentDateTime.year) < 86 || static_cast<std::int8_t>(g_currentDateTime.month) < 12 ||
        static_cast<std::int8_t>(g_currentDateTime.day) < 3) {
        g_currentDateTime.year = 86;
        g_currentDateTime.month = 12;
        g_currentDateTime.day = 3;
    }
    g_currentDateTime.hours = 8;
    g_currentDateTime.minutes = 30;
    g_currentDateTime.seconds = 0;
}





void RenderLoadingScreen(std::int32_t name) {
    g_postFxState.enabled = 0;
    IncrementCurrentDateTime_Wrapper();
    g_cdttLoadNumCount = 0;
    const MapControlData *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control || !control->mapInfo)
        return;
    g_cdttLoadScreenChunk = MAPINFO_FindChunkIn(control->mapInfo, g_mapInfoSize, SHEN_NAME(LSCN));
    if (static_cast<std::uint8_t>(g_cdttMode) < 9U) {
        if (IsSkipMode(g_cdttMode))
            return;
    } else {
        g_cdttMode = CDTTMode::None;
    }

    const GameDateTime now = g_currentDateTime;
    std::int32_t specialDateIndex = 0;
    if (g_cdttMode == CDTTMode::SpecialDateLoadScreen) {
        if (now.month == 1) {
            if (now.day == 1)
                specialDateIndex = 1;
        } else if (now.month == 12) {
            if (now.day == 24)
                specialDateIndex = 2;
            else if (now.day == 25)
                specialDateIndex = 3;
            else if (now.day == 31)
                specialDateIndex = 4;
        }
    }
    if (!name)
        name = static_cast<std::int32_t>(g_areaId);
    const std::uint32_t area = static_cast<std::uint32_t>(name);

    const MapLoadingScreenEntry *list = nullptr;
    std::int32_t loadIndex = 0;
    bool found = false;
    if (g_sceneId && g_cdttMode != CDTTMode::SpecialDateLoadScreen) {
        list = g_sceneId == 80 ? &kMapIdList[kDebugMapIdIndex] : kMapIdList.data();
        for (const MapLoadingScreenEntry *entry = list; !AreaEquals(entry->areaId, SHEN_NAME(9999)); ++entry) {
            if (AreaEquals(entry->areaId, area)) {
                found = true;
                break;
            }
            ++loadIndex;
        }
    }
    bool listed = found;
    if (g_cdttIntroDateTimeSet) {
        found = true;
        loadIndex = 84;
        listed = true;
    }
    g_cdttIntroDateTimeSet = 0;
    if (AreaEquals("JOMO", area)) {
        if (g_entryPoint - 3U > 1U) {
            if (g_entryPoint == 1)
                loadIndex = 77;
        } else if (static_cast<std::uint8_t>(static_cast<std::uint8_t>(g_cdttMode) - 4U) <= 1U) {
            loadIndex = 77;
        } else {
            loadIndex = g_displayActNumber ? 77 : 54;
        }
    }
    g_displayActNumber = 0;
    if (AreaEquals("JD00", area) && g_entryPoint == 20)
        loadIndex = 70;

    if (found && list && list[loadIndex].filename)
        LoadArchiveSurfaces(list[loadIndex].filename, SHEN_NAME(LDAR), &g_cdttHeaderSurface, 1);
    if (specialDateIndex)
        LoadArchiveSurfaces(kSpecialDateArchives[static_cast<std::size_t>(specialDateIndex - 1)], SHEN_NAME(LDGJ), &g_cdttSpecialDateSurface, 1);
    g_cdttLoadNumCount = LoadArchiveSurfaces("LOADNUM.GZ", SHEN_NAME(LDTD), g_cdttLoadNumSurfaces.data(), g_cdttLoadNumSurfaces.size());
    g_isRenderSuppressed = 1;
    FlushRenderQueue();

    const char *filename = nullptr;
    if (listed && list)
        filename = list[loadIndex].filename;
    else if (specialDateIndex)
        filename = kSpecialDateArchives[static_cast<std::size_t>(specialDateIndex - 1)];
    g_cdttViewLoadIndex = loadIndex;
    g_cdttViewSpecialDateIndex = specialDateIndex;
    g_cdttViewFilename = filename;
    LoadLoadingScreenImage(
        name, loadIndex, specialDateIndex, now.year, now.month, now.day, now.dayOfWeek, now.hours, now.minutes, now.hours >= 12, filename, g_cdttMode);
}


bool CDTT_PrepareLoadingScreen(std::uint32_t areaId) {
    RenderLoadingScreen(static_cast<std::int32_t>(areaId));
    return g_cdttLoadNumCount != 0;
}





HLib::Task *CDTT_Init() {
    if (!g_cdttLoadNumCount && !IsSkipMode(g_cdttMode))
        return g_cdttTask;
    if (g_cdttTask) {
        RenderScreen_Wrapper(nullptr, 0);
        Shenmue_DispatchRenderPipeline();
        g_cdttTask->initCallbackFnPtr = nullptr;
        g_cdttTask->mainTaskCallbackFnPtr = CDTT_FreeCallback;
        g_cdttTask->autoInitTaskCallbackFnPtr = nullptr;
        return g_cdttTask;
    }
    HLib::TaskCallback callback = CDTT_Callback;
    if (!g_cdttLoadScreenChunk) {
        RenderScreen_Wrapper(nullptr, 0);
        Shenmue_DispatchRenderPipeline();
        callback = CDTT_FreeCallback;
    }
    g_cdttTask = HLib_EnqueueTaskWithoutParameter(callback, 4, 0, kCDTTTaskToken);
    g_cdttAlpha = 255;
    return g_cdttTask;
}


void CDTT_Callback(HLib::TaskData *) {
    if ((Shenmue_nPauseState & 0x102) == 0) {
        g_cdttAlpha = static_cast<std::int16_t>(g_cdttAlpha + 8);
        if (g_cdttAlpha > 255)
            g_cdttAlpha = 255;
    }
    CDTT_DrawLoadScreen(nullptr);
}



void CDTT_FreeCallback(HLib::TaskData *) {
    g_postFxState.enabled = 1;
    std::int16_t alpha = g_cdttAlpha;
    if ((Shenmue_nPauseState & 0x102) == 0) {
        alpha = static_cast<std::int16_t>(g_cdttAlpha - 8);
        g_cdttAlpha = alpha;
    }
    if (alpha >= 0) {
        CDTT_DrawLoadScreen(nullptr);
        return;
    }
    if (g_cdttHeaderSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(g_cdttHeaderSurface);
    g_cdttHeaderSurface = -1;
    if (g_cdttSpecialDateSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(g_cdttSpecialDateSurface);
    g_cdttSpecialDateSurface = -1;
    for (std::int32_t index = 0; index < g_cdttLoadNumCount; ++index) {
        if (g_cdttLoadNumSurfaces[static_cast<std::size_t>(index)] != -1)
            PVR_FreeAllocatedSurfaceByIndex(g_cdttLoadNumSurfaces[static_cast<std::size_t>(index)]);
        g_cdttLoadNumSurfaces[static_cast<std::size_t>(index)] = -1;
    }
    g_cdttLoadNumCount = 0;
    g_cdttMode = CDTTMode::None;
    HLib_DestroyTask(g_cdttTask);
    g_cdttTask = nullptr;
}


void CDTT_BeginFadeOut() {
    if (g_cdttTask)
        CDTT_Init();
}


void CDTT_ResetPublishedState() {
    g_cdttHeaderSurface = -1;
    g_cdttSpecialDateSurface = -1;
    g_cdttLoadNumSurfaces.fill(-1);
    g_cdttTask = nullptr;
}


void CDTT_Clear() {
    if (g_cdttTask && g_cdttTask->mainTaskCallbackFnPtr && g_cdttTask->taskToken == kCDTTTaskToken)
        HLib_DestroyTask(g_cdttTask);
    g_cdttTask = nullptr;
    g_cdttAlpha = -1;
    if (g_cdttHeaderSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(g_cdttHeaderSurface);
    if (g_cdttSpecialDateSurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(g_cdttSpecialDateSurface);
    for (std::int32_t index = 0; index < g_cdttLoadNumCount; ++index) {
        if (g_cdttLoadNumSurfaces[static_cast<std::size_t>(index)] != -1)
            PVR_FreeAllocatedSurfaceByIndex(g_cdttLoadNumSurfaces[static_cast<std::size_t>(index)]);
    }
    CDTT_ResetPublishedState();
    g_cdttLoadNumCount = 0;
    g_cdttMode = CDTTMode::None;
    g_cdttLoadScreenChunk = nullptr;
    g_cdttViewLoadIndex = -1;
    g_cdttViewSpecialDateIndex = 0;
    g_cdttViewFilename = nullptr;
    LoadScreenPresentation &screen = g_loadScreenPresentation;
    for (const std::int32_t binding : screen.widgetBindings) {
        if (binding >= 0 && binding < static_cast<std::int32_t>(g_uiWidgetBindings.size()))
            g_uiWidgetBindings[static_cast<std::size_t>(binding)].animating = 0;
    }
    for (d3t::UIWindow *const window : screen.windows) {
        if (window)
            window->alpha = 0.0f;
    }
    screen.imageSet = 0;
    LoadScreen.isVisible = 0;
    g_isRenderSuppressed = 0;
}


bool CDTT_ApplyLoadScreenUtility(const CDTTLoadScreenParameters &params) {
    if (params.mode == 0)
        RenderLoadingScreen(0);
    else if (params.mode == 1)
        CDTT_Init();
    else if (params.mode == 2)
        RenderLoadingScreen(params.setting);
    return true;
}

} // namespace shenmue::s1
