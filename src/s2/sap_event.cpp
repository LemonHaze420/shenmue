#include "s2/events/event_api.h"
#include "s2/sap_event.hpp"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
SapEventData sapEventData = {0};
const shen_name emptySapCharacters[1] = {0};
#else
#include "s2/scene_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>

namespace shenmue::s2 {
SapEventData sapEventData{};
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void CopySapString(char *destination, const char *source) {
    if (source)
        while (*source)
            *destination++ = *source++;
    *destination = 0;
}

void CopySapCharacters(shen_name *destination, const shen_name *source) {
    if (source)
        while (*source)
            *destination++ = *source++;
    *destination = 0;
}

void SelectSapAuthor(const shen_name *authors, int count, HLibTaskCallback next, unsigned int flags) {
    int author;
    EventDateTime date;
    sapEventData.nextCallback = next;
    sapEventData.authorFlags = flags;
    author = authors[FTBL.shenGetRandomInt32(count)];
    if (FTBL.EVT_GetControllerInput(0, 0) & 4) {
        FTBL.GetCurrentDateTime((DateTime *)&date);
        author = authors[date.day % count];
    }
    FTBL.DebugLogToFileRemoved("Author:%c%c%c%c\n", author, author >> 8, (short)(author >> 16), (int)author >> 24);
    if (sapEventData.authorFlags & 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (sapEventData.authorFlags & 128)
        FTBL.EV_spmBeginSampleExecute(0);
    if (sapEventData.authorFlags & 32)
        FTBL.EV_authSceneCtrl(-11);
    if (!(sapEventData.authorFlags & 64)) {
        sapEventData.savedFilmBars = FTBL.EV_authSceneCtrl(-9);
        FTBL.EV_authSceneCtrl(-8);
    }
    FTBL.EV_authSceneCtrl(author);
    FTBL.HLib_SetCurrentTaskTo(SapAuthorWaitCallback);
}

void SapAuthorWaitCallback(void *data) {
    int frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == FTBL.EV_authSceneCtrl(-5) - 31) {
        if (sapEventData.authorFlags & 2)
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (sapEventData.authorFlags & 256)
            FTBL.spmFadeOut(0);
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        int i;
        for (i = 0; sapEventData.characters[i]; ++i)
            ResetSapAuthorCharacter(sapEventData.characters[i]);
        if (sapEventData.authorFlags & 32)
            FTBL.EV_authSceneCtrl(-10);
        if (!(sapEventData.authorFlags & 64))
            FTBL.EV_authSceneCtrl(-7);
        FTBL.HLib_SetCurrentTaskTo(sapEventData.nextCallback);
    }
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void InitializeSapAuthorEvent(const char *authorBank, const char *package, const shen_name *characters, HLibTaskCallback next) {

    int i;
    sapEventData.nextCallback = next;
    sapEventData.pendingMask = 0;
    CopySapString(sapEventData.packageName, package);
    CopySapString(sapEventData.authorBank, authorBank);
    CopySapCharacters(sapEventData.characters, characters);
    FTBL.DebugLogToFileRemoved("sap:PackLoad\n");
    if (sapEventData.authorBank[0])
        FTBL.EV_FreeAuthorPak(1, sapEventData.authorBank);
    for (i = 0; sapEventData.characters[i]; ++i) {
        sapEventData.pendingMask |= 2U << i;
        FTBL.CheckWarmStartNpcRoute(sapEventData.characters[i]);
    }
    if (sapEventData.packageName[0]) {
        sapEventData.pendingMask |= 1;
        FTBL.HLib_SetCurrentTaskTo(SapPackageLoadCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(SapPackageWaitCallback);

#else
void InitializeSapAuthorEvent(const char *authorBank, std::uintptr_t, const shen_name *characters, HLib::TaskCallback next) {

    sapEventData.nextCallback = next;
    sapEventData.pendingMask = 0;
    sapEventData.packageName[0] = 0;
    char *bank = sapEventData.authorBank;
    if (authorBank)
        while (*authorBank)
            *bank++ = *authorBank++;
    *bank = 0;
    shen_name *destination = sapEventData.characters;
    if (characters)
        while (characters->name)
            *destination++ = *characters++;
    *destination = shen_name{};
    if (sapEventData.authorBank[0])
        (void)EV_FreeAuthorPak(1, sapEventData.authorBank);
    for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index) {
        sapEventData.pendingMask |= 2U << (index & 31U);
        (void)EV_CheckWarmStartNpcRoute(sapEventData.characters[index]);
    }
    std::uint64_t packageWord;
    std::memcpy(&packageWord, sapEventData.packageName, sizeof(packageWord));
    if (packageWord) {
        sapEventData.pendingMask |= 1;
        HLib::SetCurrentTaskTo(SapPackageLoadCallback);
    } else {
        HLib::SetCurrentTaskTo(SapPackageWaitCallback);
    }

#endif
}

// @verified - p
void SapPackageLoadCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)

    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (unsigned int)sapEventData.packageName, 0))
        FTBL.HLib_SetCurrentTaskTo(SapPackageWaitCallback);
    else
        FTBL.DebugLogToFileRemoved("sap:PakWait(%d)\n", g_frameCounter);

#else

    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory),
                    reinterpret_cast<std::uintptr_t>(sapEventData.packageName)))
        HLib::SetCurrentTaskTo(SapPackageWaitCallback);

#endif
}

// @verified - p
void SapPackageWaitCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)

    int i;
    if ((sapEventData.pendingMask & 1) && !FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0))
        sapEventData.pendingMask &= ~1;
    for (i = 0; sapEventData.characters[i]; ++i) {
        unsigned int base = 2, mask;
        __asm__ volatile("" : "+r"(base));
        mask = base << i;
        if ((sapEventData.pendingMask & mask) && FTBL.ForceLoadNpc(sapEventData.characters[i]))
            sapEventData.pendingMask &= ~mask;
    }
    if (!sapEventData.pendingMask) {
        for (i = 0; sapEventData.characters[i]; ++i)
            ResetSapAuthorCharacter(sapEventData.characters[i]);
        ResetSapAuthorCharacter(SAP_PLAYER);
        FTBL.HLib_SetCurrentTaskTo(sapEventData.nextCallback);
    }

#else

    if ((sapEventData.pendingMask & 1U) && !EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
        sapEventData.pendingMask &= ~1U;
    for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index) {
        const std::uint32_t mask = 2U << (index & 31U);
        if ((sapEventData.pendingMask & mask) && EV_ForceLoadNpc(sapEventData.characters[index]))
            sapEventData.pendingMask &= ~mask;
    }
    if (!sapEventData.pendingMask) {
        for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index)
            ResetSapAuthorCharacter(sapEventData.characters[index]);
        ResetSapAuthorCharacter(SAP_PLAYER);
        HLib::SetCurrentTaskTo(sapEventData.nextCallback);
    }

#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void ReleaseSapAuthorEvent(void) {
    int i;
    FTBL.DebugLogToFileRemoved("sap:PackRelease\n");
    if (sapEventData.packageName[0])
        FTBL.EV_ControlPackageResources(0, 6, sapEventData.packageName, 0, 0);
    for (i = 0; sapEventData.characters[i]; ++i) {
        FTBL.npcFree(sapEventData.characters[i]);
        FTBL.npcLockUnlock(sapEventData.characters[i]);
    }
    if (sapEventData.authorBank[0])
        FTBL.EV_FreeAuthorPak(0, sapEventData.authorBank);
}

void QueueSapSceneLoad(shen_name area, int entry, unsigned int flags) {
    HLibTask *task;
    sapEventData.authorFlags = flags;
    sapEventData.nextArea = area;
    sapEventData.nextEntry = entry;
    task = FTBL.HLib_EnqueueTaskWithoutParameter(SapSceneLoadCallback, 4, 11, SAP_TASK);
    if (sapEventData.authorFlags & 2) {
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTimedTask(task, 31);
    }
}

void SapSceneLoadCallback(void *data) {
    FTBL.EV_scenQueueLoad(g_actNumber, sapEventData.nextArea, sapEventData.nextEntry);
    FTBL.HLib_DestroyCurrentTask();
}
#endif

// @verified - p
void ResetSapAuthorCharacter(shen_name character) {

    Vector3 position = {-1000, -1000, -1000};
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
    FTBL.EV_SetCharaControllerFlags(character, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 71, 0, 0);
#else
    (void)mtwkSetRequestedRootMode(scenFindMotionwork(character), 0);
    (void)mtwkSetRegionFlag(scenFindMotionwork(character), 71, 0, MotionRegion::LowerBody);
#endif
    FTBL.EV_AT_SetPos(character, 0x38000000, &position);
}

#if defined(SHENMUE_EVENT_OVERLAY)
void RestoreSapAuthorCharacter(shen_name character) {
    FTBL.EV_SetCharaControllerFlags(character, 1, 1);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
    FTBL.EV_SetCharaControllerFlags(character, 2, 1);
    FTBL.EV_SetCharaControllerFlags(character, 3, 1);
}
#else
// @verified - p
void SapAuthorWaitCallback(void *) {
    const std::int32_t endFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 31;
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == static_cast<std::uint32_t>(endFrame)) {
        if (sapEventData.authorFlags & 2U)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (sapEventData.authorFlags & 0x100U)
            spmFadeOut();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        for (std::uint32_t index = 0; sapEventData.characters[index].name; ++index)
            ResetSapAuthorCharacter(sapEventData.characters[index]);
        if (sapEventData.authorFlags & 0x20U)
            (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (!(sapEventData.authorFlags & 0x40U))
            (void)EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        HLib::SetCurrentTaskTo(sapEventData.nextCallback);
    }
}
} // namespace shenmue::s2
#endif
