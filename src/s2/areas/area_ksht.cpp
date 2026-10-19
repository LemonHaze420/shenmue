#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/search.hpp"
namespace shenmue::s2::areas {
std::int32_t kshtSelection, kshtPhase, kshtDelay, kshtAuthSelection, kshtRabbitDropCount;
std::int32_t kshtFreeTalkSelection, kshtNextPhase, kshtFreeTalkIndicator;
HLib::Task *kshtTask;
Vector3 kshtSavedPosition;
Vector3i kshtSavedRotation;
std::uint32_t kshtBreathEffects[2], kshtLeafEffect;
extern std::uint64_t kshtAuthorMapLighting;
void ResetKshtSubtitles();
void StartKshtSubtitles();
namespace {
constexpr HLib::TaskCallback conversationCallbacks[]{StartKshtZodiacConversation, KshtWaitConversationMotion, StartKshtWeatherConversation,
    KshtWaitConversationMotion, StartKshtNamesConversation, KshtWaitConversationMotion, StartKshtBadFeelingConversation, KshtWaitConversationMotion,
    StartKshtDreamConversation, KshtWaitConversationMotion, StartKshtDestinyConversation, KshtWaitConversationMotion, KshtPrepareShortConversation,
    KshtWaitShortConversation, KshtPrepareHelp, KshtWaitHelp, KshtPrepareInstructions, KshtWaitInstructions, KshtPrepareFinalConversation,
    KshtWaitInstructions};
void SetupConversation(void *data) {
    conversationCallbacks[2 * kshtSelection](data);
    HLib::SetCurrentTaskTo(conversationCallbacks[2 * kshtSelection + 1]);
}
void QueueRoot(HLib::TaskCallback callback) {
    kshtTask = HLib::EnqueueTaskWithoutParameter(callback, 4, 0x0b, static_cast<std::int32_t>(KSHT_TASK.name));
}
void StartAuthor(std::int32_t selection, HLib::TaskCallback callback, bool disallowSkip) {
    kshtAuthSelection = selection;
    EV_ResetFreeTalkFlags();
    EV_NPC_NewLoadSw(0);
    if (disallowSkip)
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    QueueKshtAuthor(&kshtAuthSelection);
    QueueRoot(callback);
}
void InitializeKsht() {
    kshtRabbitDropCount = -1;
    kshtAuthorMapLighting = 0;
    kshtPhase = 0;
    kshtNextPhase = 0;
    kshtFreeTalkSelection = 0;
    kshtFreeTalkIndicator = 0;
    kshtSelection = 0;
    kshtDelay = 0;
    kshtTask = nullptr;
    kshtSavedPosition = {0, 0, 0};
    kshtSavedRotation = {0, 0, 0};
    kshtAuthSelection = 0;
    ResetKshtSubtitles();
    kshtLeafEffect = 0xffffffffU;
    kshtBreathEffects[0] = 0;
    kshtBreathEffects[1] = 0;
}
void StartKsht(std::uintptr_t argument) {
    const std::int32_t state = static_cast<std::int32_t>(argument);
    DateTime date;
    GetCurrentDateTime(date);
    camCtrlCacheTransform();
    if (state >= 5100 && state <= 5107) {
        EV_PTYCUtils(KSHT_SIN, 3, 1);
        GetCurrentDateTime(date);
        date.hour = 17;
        date.minute = 12;
        SetCurrentDateTimeTo(date);
        SetRenderClockAndCompassTo(1);
        kshtSelection = state;
        kshtFreeTalkSelection = state - 5100;
        kshtPhase = 7;
        EV_npcLockUnlock(SHEN_NAME(M01_));
        QueueKshtAuthor(&kshtFreeTalkSelection);
        QueueRoot(KshtFreeTalkCallback);
        return;
    }
    if (state == 5120 || state == 5121) {
        SetRenderClockAndCompassTo(1);
        StartAuthor(state - 5112, KshtWaitDrawingCallback, false);
        return;
    }
    if (state >= 5210 && state <= 5215) {
        if (state == 5212 && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 936)) {
            EV_EnsureCharacterDraw(SHEN_NAME(usag));
            EV_EnableCharacterDisplay(SHEN_NAME(usag));
        }
        SetRenderClockAndCompassTo(1);
        kshtSelection = state;
        kshtAuthSelection = state - 5200;
        srchSetLookMode();
        SetPlayerControlEnabled(0);
        EV_ResetFreeTalkFlags();
        EV_NPC_NewLoadSw(0);
        QueueKshtAuthor(&kshtAuthSelection);
        QueueRoot(KshtWaitAuthorCallback);
        return;
    }
    if (state == 5280 || state == 5281) {
        SetRenderClockAndCompassTo(1);
        kshtSelection = state;
        StartAuthor(state - 5264, KshtWaitAuthorCallback, true);
        return;
    }
    if (state == 5282) {
        EV_PTYCUtils(KSHT_SIN, 3, 1);
        SetRenderClockAndCompassTo(1);
        kshtAuthSelection = 18;
        EV_EnableCharacterDisplay(SHEN_NAME(a349));
        EV_ResetFreeTalkFlags();
        EV_NPC_NewLoadSw(0);
        QueueKshtAuthor(&kshtAuthSelection);
        QueueRoot(KshtWaitReturnCallback);
        return;
    }
    if (state == 5370 || state == 5371) {
        SetRenderClockAndCompassTo(1);
        EV_DisableCharacterDisplay(SHEN_NAME(usag));
        EV_ToggleCharacterFlag(SHEN_NAME(usag));
        kshtSelection = state;
        kshtAuthSelection = state - 5351;
        EV_ResetFreeTalkFlags();
        EV_NPC_NewLoadSw(0);
        QueueKshtAuthor(&kshtAuthSelection);
        kshtAuthorMapLighting = 0x100000001ULL;
        ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
        return;
    }
    if (state == 5450 || state == 5451) {
        if (state == 5450 && date.hour == 18 && date.minute >= 30) {
            kshtRabbitDropCount = 0;
            EV_EnsureCharacterDraw(SHEN_NAME(usag));
            EV_EnableCharacterDisplay(SHEN_NAME(usag));
        }
        SetRenderClockAndCompassTo(1);
        if (kshtRabbitDropCount >= 0 && state == 5451 && ++kshtRabbitDropCount >= 2) {
            const shen_name rabbit{SHEN_NAME(usag)};
            EV_AT_SetPos(rabbit, EventTransformFlags::AllAxes, {143.85851f, 100.8371f, 215.11729f});
            EV_AT_SetAngY(rabbit, EventTransformFlags::AllAxes, {0, 0, 46785});
            ConfigureScenarioNodeMotion(rabbit, 40964, 0, 32767, 0, 1, 0);
            ConfigureScenarioNodeMotion(rabbit, 0, -1, -1, -1, 32767, 4);
        }
        StartAuthor(state - 5427, KshtWaitAuthorCallback, true);
        return;
    }
    if (state == -1) {
        StartKshtSubtitles();
        return;
    }
    SetRenderClockAndCompassTo(1);
    SetPlayerControlEnabled(0);
    kshtSelection = state;
    EV_FreeAuthorPak(1, "0521");
    QueueRoot(SetupConversation);
    HLib::SetupTimedTask(kshtTask, 0);
    LoadKshtCharacters(kshtCompanionList, kshtTask);
}
} // namespace
const ScriptAreaDefinition kshtScriptDefinition{.areaId = SHEN_NAME(KSHT), .field04 = 0, .init = InitializeKsht, .main = StartKsht};
} // namespace shenmue::s2::areas
