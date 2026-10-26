#include "s1_multiplayer.hpp"

#if SHENMUE_ENHANCEMENT_STEAM_S1_MP && SHENMUE_IS_S1 && SHENMUE_IS_PC

#include "s1_multiplayer_fight.hpp"
#include "s1_multiplayer_steam.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/char.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/vfcl.hpp"
#include "s1/scene.hpp"
#include "s1/window.hpp"
#include "s1/task/scen.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstring>

namespace shenmue::s1 {
S1MultiplayerRole g_s1MultiplayerRole = S1MultiplayerRole::None;

namespace {
bool g_localFightCreated = false;
bool g_peerFightCreated = false;
bool g_sessionFailed = false;
bool g_sessionAccepted = false;
bool g_phoneRequested = false;
bool g_phoneClosed = false;
bool g_transportActive = false;
bool g_admitting = false;
bool g_acceptSent = false;
bool g_needTravel = false;
bool g_travelIssued = false;
bool g_localReady = false;
bool g_peerReady = false;
bool g_profileSent = false;
bool g_profileReceived = false;
bool g_profileApplied = false;
bool g_profileAcknowledged = false;
bool g_beginFight = false;
ULONGLONG g_sessionDeadline = 0;
S1MultiplayerCombatProfile g_remoteProfile{};
std::uint32_t g_inputFrame = 0;
InputSlot g_physicalInput{};
InputSlot g_savedSecondaryInput{};
bool g_secondaryInputCaptured = false;
struct RemoteInput {
    std::uint32_t frame;
    std::uint32_t checksum;
    InputSlot input;
};
RemoteInput g_remoteInputs[8]{};

bool SendSessionMessage(
    S1MultiplayerMessageKind kind, std::uint32_t frame = 0, const InputSlot *input = nullptr, const S1MultiplayerCombatProfile *profile = nullptr) {
    S1MultiplayerMessage message{.magic = kS1MultiplayerMagic,
        .version = kS1MultiplayerVersion,
        .kind = kind,
        .role = g_s1MultiplayerRole,
        .scene = kS1MultiplayerScene,
        .area = kS1MultiplayerArea.name,
        .entry = kS1MultiplayerEntry,
        .frame = frame};
    if (input) {
        message.input = *input;
        message.checksum = S1Multiplayer_FightChecksum();
    }
    if (profile)
        message.profile = *profile;
    message.session = multiplayer_steam::Lobby();
    return multiplayer_steam::Send(&message, sizeof(message));
}

int ReadSessionMessage(S1MultiplayerMessage &message) {
    const int result = multiplayer_steam::Receive(&message, sizeof(message));
    if (result <= 0)
        return result;
    if (message.magic != kS1MultiplayerMagic || message.version != kS1MultiplayerVersion ||
        message.role != (g_s1MultiplayerRole == S1MultiplayerRole::Host ? S1MultiplayerRole::Guest : S1MultiplayerRole::Host) ||
        message.scene != kS1MultiplayerScene || message.area != kS1MultiplayerArea.name || message.entry != kS1MultiplayerEntry ||
        message.session != multiplayer_steam::Lobby()) {
        return -1;
    }
    return 1;
}

void AcceptSession() {
    g_sessionAccepted = true;
    g_admitting = false;
    g_sessionFailed = g_localFightCreated = g_peerFightCreated = false;
    g_needTravel = true;
    g_travelIssued = g_localReady = g_peerReady = g_profileSent = g_profileReceived = g_profileApplied = false;
    g_profileAcknowledged = g_beginFight = false;
    g_sessionDeadline = GetTickCount64() + 120000;
    g_inputFrame = 0;
    for (RemoteInput &input : g_remoteInputs)
        input = {};
}

bool FreeMapReady() {
    const MapControlData *control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const Character *ryo = control ? static_cast<const Character *>(control->ryo) : nullptr;
    return control && !(control->flags & 1U) && ryo && ryo->model && control->scenarioTask && g_mdlmTask && !g_bIsFighting && !g_pFightStage &&
        g_pControlPadController && g_pControlPadController->blockedCanControl == 1 && !SCEN_HasActiveScript() && !CDTT_GetState().task;
}
} // namespace

bool S1Multiplayer_Initialize(std::uint64_t invitedLobby) {
    if (!multiplayer_steam::Initialize())
        return false;
    if (invitedLobby)
        multiplayer_steam::Join(invitedLobby);
    return !invitedLobby || multiplayer_steam::Active();
}

void S1Multiplayer_Release() {
    S1Multiplayer_EndFight();
    S1Multiplayer_CloseSession();
    multiplayer_steam::Release();
}

static bool PollSession(bool fightCreated) {
    if ((!g_sessionAccepted && !g_admitting) || g_sessionFailed)
        return !g_sessionFailed;
    if (fightCreated && !g_localFightCreated) {
        g_localFightCreated = true;
        if (!SendSessionMessage(S1MultiplayerMessageKind::FightCreated))
            g_sessionFailed = true;
    }
    for (unsigned count = 0; count < 32 && !g_sessionFailed; ++count) {
        S1MultiplayerMessage message{};
        const int result = ReadSessionMessage(message);
        if (!result)
            break;
        if (result < 0 || message.kind == S1MultiplayerMessageKind::Leave) {
            g_sessionFailed = true;
        } else if (g_admitting) {
            const bool host = g_s1MultiplayerRole == S1MultiplayerRole::Host;
            if (host && !g_acceptSent && message.kind == S1MultiplayerMessageKind::Hello) {
                g_acceptSent = SendSessionMessage(S1MultiplayerMessageKind::Accept);
                g_sessionFailed = !g_acceptSent;
            } else if (host && g_acceptSent && message.kind == S1MultiplayerMessageKind::Acknowledge) {
                AcceptSession();
            } else if (!host && message.kind == S1MultiplayerMessageKind::Accept) {
                if (SendSessionMessage(S1MultiplayerMessageKind::Acknowledge))
                    AcceptSession();
                else
                    g_sessionFailed = true;
            } else
                g_sessionFailed = true;
        } else if (message.kind == S1MultiplayerMessageKind::Ready && !g_peerReady) {
            g_peerReady = true;
        } else if (message.kind == S1MultiplayerMessageKind::Profile && g_s1MultiplayerRole == S1MultiplayerRole::Guest && !g_profileReceived) {
            g_remoteProfile = message.profile;
            g_profileReceived = true;
        } else if (message.kind == S1MultiplayerMessageKind::ProfileAccepted && g_s1MultiplayerRole == S1MultiplayerRole::Host && g_profileSent &&
            !g_profileAcknowledged) {
            g_profileAcknowledged = true;
        } else if (message.kind == S1MultiplayerMessageKind::Begin && g_s1MultiplayerRole == S1MultiplayerRole::Guest && g_profileApplied && !g_beginFight) {
            g_beginFight = true;
        } else if (message.kind == S1MultiplayerMessageKind::FightCreated && g_beginFight && !g_peerFightCreated) {
            g_peerFightCreated = true;
        } else if (message.kind == S1MultiplayerMessageKind::Input && g_beginFight && message.frame > g_inputFrame && message.frame <= g_inputFrame + 8 &&
            !g_remoteInputs[message.frame % 8].frame) {
            g_remoteInputs[message.frame % 8] = {.frame = message.frame, .checksum = message.checksum, .input = message.input};
        } else
            g_sessionFailed = true;
    }
    return !g_sessionFailed;
}

void S1Multiplayer_PumpTransport() {
    multiplayer_steam::Update();
    if (!g_transportActive && multiplayer_steam::Connected() && !S1Multiplayer_OwnsFight()) {
        g_transportActive = g_admitting = true;
        g_acceptSent = g_sessionFailed = false;
        g_sessionDeadline = GetTickCount64() + 120000;
        g_s1MultiplayerRole = multiplayer_steam::Host() ? S1MultiplayerRole::Host : S1MultiplayerRole::Guest;
        if (g_s1MultiplayerRole == S1MultiplayerRole::Guest && !SendSessionMessage(S1MultiplayerMessageKind::Hello))
            g_sessionFailed = true;
    }
    if (g_transportActive && !multiplayer_steam::Connected())
        g_sessionFailed = true;
    PollSession(false);
    if ((g_admitting || (g_sessionAccepted && !g_localFightCreated)) && GetTickCount64() >= g_sessionDeadline)
        g_sessionFailed = true;
}

bool S1Multiplayer_OnDial(const char *digits) {
    if (!digits || std::strcmp(digits, "420") != 0)
        return false;
    if (!g_sessionAccepted && !g_admitting && !g_phoneRequested && !S1Multiplayer_OwnsFight()) {
        g_phoneRequested = true;
        g_phoneClosed = false;
    }
    return true;
}

void S1Multiplayer_OnPhoneClosed() {
    if (g_phoneRequested)
        g_phoneClosed = true;
}

void S1Multiplayer_ExecuteTask(HLib::Task *task) {
    if (g_sessionAccepted && g_needTravel && !g_travelIssued && !g_mapControlTask && task == g_pModuleTask && g_isOnMainMenu) {
        const ModuleTaskData *module = reinterpret_cast<const ModuleTaskData *>(task->task_data_ptr);
        if (module && module->currentModule == SystemModule::Display) {
            LoadIntro();
            g_sceneId = kS1MultiplayerScene;
            g_areaId = kS1MultiplayerArea.name;
            g_entryPoint = kS1MultiplayerEntry;
            g_travelIssued = true;

            return;
        }
    }
    const HLib::TaskCallback callback = task->mainTaskCallbackFnPtr;
    const bool fightTask =
        S1Multiplayer_OwnsFight() && (callback == vfm_Callback_FightTaskEntry_Callback || callback == vfg_Callback_GameInfoEntry || callback == VFCL_Callback);
    S1MultiplayerFightRandomScope randomScope(fightTask);
    callback(task->task_data_ptr);
}

void S1Multiplayer_ExecuteCharacterComponent(Character *character, CharacterComponentNode *component) {
    const bool fightMotion = component->componentId == kMultiplayerMotionToken.name && S1Multiplayer_IsFightCharacter(character);
    S1MultiplayerFightRandomScope randomScope(fightMotion);
    if (fightMotion)
        MOTM_GetWork(character)->motion.poseEvaluationStage = 0;
    component->updateCallback(character, component + 1);
}

void S1Multiplayer_BeginInputFrame() {
    if (g_sessionAccepted && g_inputFrame && g_s1MultiplayerRole == S1MultiplayerRole::Guest)
        g_inputState.slots[0] = g_physicalInput;
}

void S1Multiplayer_Update() {
    S1Multiplayer_UpdateFight();
    if (!g_sessionAccepted)
        S1Multiplayer_RestoreCombatProfile();
    if (g_phoneRequested && g_phoneClosed && FreeMapReady()) {
        g_phoneRequested = g_phoneClosed = false;
        multiplayer_steam::BeginHost();
    }
    if (g_sessionFailed || (g_localFightCreated && !S1Multiplayer_OwnsFight())) {
        S1Multiplayer_EndFight();
        S1Multiplayer_CloseSession();
        return;
    }
    if (!g_sessionAccepted)
        return;
    if (!g_localReady) {
        if (!FreeMapReady())
            return;
        if (g_needTravel && !g_travelIssued) {
            LoadGameModule(SystemModule::Map, kS1MultiplayerScene, kS1MultiplayerArea.name, kS1MultiplayerEntry);
            g_travelIssued = true;
            return;
        }
        if (g_sceneId != kS1MultiplayerScene || g_areaId != kS1MultiplayerArea || g_entryPoint != kS1MultiplayerEntry)
            return;
        g_localReady = SendSessionMessage(S1MultiplayerMessageKind::Ready);
        if (!g_localReady)
            g_sessionFailed = true;
        return;
    }
    const bool host = g_s1MultiplayerRole == S1MultiplayerRole::Host;
    if (g_peerReady && !g_beginFight) {
        if (host && !g_profileSent) {
            const S1MultiplayerCombatProfile profile = S1Multiplayer_GetCombatProfile();
            g_profileSent = SendSessionMessage(S1MultiplayerMessageKind::Profile, 0, nullptr, &profile);
            g_sessionFailed = !g_profileSent;
        } else if (!host && g_profileReceived && !g_profileApplied) {
            g_profileApplied = S1Multiplayer_ApplyCombatProfile(g_remoteProfile);
            if (!g_profileApplied || !SendSessionMessage(S1MultiplayerMessageKind::ProfileAccepted))
                g_sessionFailed = true;
        } else if (host && g_profileAcknowledged) {
            g_beginFight = SendSessionMessage(S1MultiplayerMessageKind::Begin);
            g_sessionFailed = !g_beginFight;
        }
    }
    if (!g_beginFight || g_sessionFailed)
        return;
    if (!g_localFightCreated && S1Multiplayer_CanStartFight()) {
        if (!S1Multiplayer_CreateFight()) {
            g_sessionFailed = true;
            return;
        }
        PollSession(true);
    }
    if (!g_localFightCreated)
        return;
    const InputSlot physicalInput = g_inputState.slots[0];
    g_physicalInput = physicalInput;
    if (!g_secondaryInputCaptured) {
        g_savedSecondaryInput = g_inputState.slots[1];
        g_secondaryInputCaptured = true;
    }
    const std::uint32_t frame = g_inputFrame + 1;
    if (!SendSessionMessage(S1MultiplayerMessageKind::Input, frame, &physicalInput))
        g_sessionFailed = true;
    const ULONGLONG deadline = GetTickCount64() + (frame == 1 ? 120000 : 10000);
    while (!g_sessionFailed && (!g_peerFightCreated || g_remoteInputs[frame % 8].frame != frame)) {
        MSG message{};
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        S1Multiplayer_PumpTransport();
        if (g_shouldQuit)
            g_sessionFailed = true;
        if (GetTickCount64() >= deadline)
            g_sessionFailed = true;
        if (!g_sessionFailed)
            Sleep(1);
    }
    if (g_sessionFailed) {
        S1Multiplayer_EndFight();
        return;
    }
    if (g_remoteInputs[frame % 8].checksum != S1Multiplayer_FightChecksum()) {
        g_sessionFailed = true;
        S1Multiplayer_EndFight();
        return;
    }
    const InputSlot remoteInput = g_remoteInputs[frame % 8].input;
    g_inputState.slots[0] = host ? physicalInput : remoteInput;
    g_inputState.slots[1] = host ? remoteInput : physicalInput;
    g_remoteInputs[frame % 8] = {};
    g_inputFrame = frame;
}

void S1Multiplayer_CloseSession() {
    if (g_transportActive)
        SendSessionMessage(S1MultiplayerMessageKind::Leave);
    if (g_transportActive || multiplayer_steam::Active())
        multiplayer_steam::Leave();
    if (g_sessionAccepted && g_inputFrame && g_s1MultiplayerRole == S1MultiplayerRole::Guest)
        g_inputState.slots[0] = g_physicalInput;
    if (g_secondaryInputCaptured)
        g_inputState.slots[1] = g_savedSecondaryInput;
    g_secondaryInputCaptured = false;
    g_sessionAccepted = g_transportActive = g_admitting = g_sessionFailed = false;
    g_localFightCreated = g_peerFightCreated = false;
    g_phoneRequested = g_phoneClosed = false;
    g_s1MultiplayerRole = S1MultiplayerRole::None;
    S1Multiplayer_RestoreCombatProfile();
}
} // namespace shenmue::s1

#endif
