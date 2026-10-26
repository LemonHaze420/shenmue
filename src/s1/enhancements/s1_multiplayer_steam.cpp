#include "s1_multiplayer_steam.hpp"
#include "s1_multiplayer.hpp"

#if SHENMUE_ENHANCEMENT_STEAM_S1_MP && SHENMUE_IS_S1 && SHENMUE_IS_PC

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <array>
#include <cstring>

namespace shenmue::s1::multiplayer_steam {
namespace {
using SteamAPICall = std::uint64_t;
using SteamId = std::uint64_t;
using HSteamUser = std::int32_t;
using HSteamPipe = std::int32_t;

#pragma pack(push, 1)
struct SteamNetworkingIdentity {
    std::int32_t type;
    std::int32_t size;
    union {
        SteamId steamId;
        std::uint32_t reserved[32];
    };
};
#pragma pack(pop)
static_assert(sizeof(SteamNetworkingIdentity) == 136);

struct SteamNetworkingMessage {
    void *data;
    std::int32_t size;
    std::uint32_t connection;
    SteamNetworkingIdentity peer;
    std::int64_t connectionUserData;
    std::int64_t receivedAt;
    std::int64_t messageNumber;
    void (*freeData)(SteamNetworkingMessage *);
    void (*release)(SteamNetworkingMessage *);
    std::int32_t channel;
    std::int32_t flags;
    std::int64_t userData;
    std::uint16_t lane;
    std::uint16_t padding;
};
static_assert(offsetof(SteamNetworkingMessage, peer) == 16);
static_assert(offsetof(SteamNetworkingMessage, release) == 184);
static_assert(sizeof(SteamNetworkingMessage) == 216);

class SteamNetworkingMessages {
  public:
    virtual std::int32_t SendMessageToUser(
        const SteamNetworkingIdentity &remote, const void *data, std::uint32_t size, std::int32_t flags, std::int32_t channel) = 0;
    virtual std::int32_t ReceiveMessagesOnChannel(std::int32_t channel, SteamNetworkingMessage **messages, std::int32_t maxMessages) = 0;
    virtual bool AcceptSessionWithUser(const SteamNetworkingIdentity &remote) = 0;
    virtual bool CloseSessionWithUser(const SteamNetworkingIdentity &remote) = 0;
    virtual bool CloseChannelWithUser(const SteamNetworkingIdentity &remote, std::int32_t channel) = 0;
    virtual std::int32_t GetSessionConnectionInfo(const SteamNetworkingIdentity &remote, void *connectionInfo, void *realtimeStatus) = 0;
};

#pragma pack(push, 8)
struct SteamLobbyCreatedResult {
    std::int32_t result;
    std::int32_t padding;
    SteamId lobbyId;
};

struct SteamLobbyEnterResult {
    SteamId lobbyId;
    std::uint32_t chatPermissions;
    bool locked;
    std::array<std::uint8_t, 3> padding;
    std::uint32_t response;
    std::uint32_t trailingPadding;
};

struct SteamLobbyJoinRequested {
    SteamId lobbyId;
    SteamId friendId;
};

struct SteamNetworkingMessagesSessionRequest {
    SteamNetworkingIdentity remote;
};
#pragma pack(pop)
static_assert(sizeof(SteamLobbyCreatedResult) == 16);
static_assert(sizeof(SteamLobbyEnterResult) == 24);
static_assert(sizeof(SteamLobbyJoinRequested) == 16);
static_assert(sizeof(SteamNetworkingMessagesSessionRequest) == 136);

class SteamCallbackBase {
  public:
    virtual void Run(void *parameter) = 0;
    virtual void Run(void *parameter, bool ioFailure, SteamAPICall call) = 0;
    virtual std::int32_t GetCallbackSizeBytes() = 0;

    std::uint8_t flags = 0;
    std::array<std::uint8_t, 3> padding{};
    std::int32_t callback = 0;
};
static_assert(sizeof(SteamCallbackBase) == 16);

struct SteamApi {
    HMODULE module = nullptr;
    HSteamUser(__cdecl *getUser)() = nullptr;
    HSteamPipe(__cdecl *getPipe)() = nullptr;
    void *(__cdecl *createInterface)(const char *) = nullptr;
    void *(__cdecl *getSteamUser)(void *, HSteamUser, HSteamPipe, const char *) = nullptr;
    void *(__cdecl *getSteamFriends)(void *, HSteamUser, HSteamPipe, const char *) = nullptr;
    void *(__cdecl *getSteamUtils)(void *, HSteamPipe, const char *) = nullptr;
    void *(__cdecl *getSteamMatchmaking)(void *, HSteamUser, HSteamPipe, const char *) = nullptr;
    void *(__cdecl *getGenericInterface)(void *, HSteamUser, HSteamPipe, const char *) = nullptr;
    SteamId(__cdecl *getSteamId)(void *) = nullptr;
    std::uint32_t(__cdecl *getAppId)(void *) = nullptr;
    bool(__cdecl *isOverlayEnabled)(void *) = nullptr;
    bool(__cdecl *isCallCompleted)(void *, SteamAPICall, bool *) = nullptr;
    bool(__cdecl *getCallResult)(void *, SteamAPICall, void *, std::int32_t, std::int32_t, bool *) = nullptr;
    void(__cdecl *activateInviteDialog)(void *, SteamId) = nullptr;
    SteamAPICall(__cdecl *createLobby)(void *, std::int32_t, std::int32_t) = nullptr;
    SteamAPICall(__cdecl *joinLobby)(void *, SteamId) = nullptr;
    void(__cdecl *leaveLobby)(void *, SteamId) = nullptr;
    std::int32_t(__cdecl *getLobbyMemberCount)(void *, SteamId) = nullptr;
    SteamId(__cdecl *getLobbyMemberByIndex)(void *, SteamId, std::int32_t) = nullptr;
    const char *(__cdecl *getLobbyData)(void *, SteamId, const char *) = nullptr;
    bool(__cdecl *setLobbyData)(void *, SteamId, const char *, const char *) = nullptr;
    bool(__cdecl *setLobbyJoinable)(void *, SteamId, bool) = nullptr;
    SteamId(__cdecl *getLobbyOwner)(void *, SteamId) = nullptr;
    void(__cdecl *registerCallback)(SteamCallbackBase *, std::int32_t) = nullptr;
    void(__cdecl *unregisterCallback)(SteamCallbackBase *) = nullptr;
};

constexpr int kChannel = 0x534d;
constexpr char kProtocolKey[] = "shenmue_s1_multiplayer";
constexpr char kProtocol[] = "dobuita-4";
SteamApi g_api{};
void *g_client = nullptr, *g_user = nullptr, *g_friends = nullptr, *g_utils = nullptr, *g_matchmaking = nullptr;
SteamNetworkingMessages *g_messages = nullptr;
HSteamUser g_hUser = 0;
HSteamPipe g_hPipe = 0;
SteamId g_localId = 0, g_lobbyId = 0, g_peerId = 0, g_invitedLobby = 0;
std::uint32_t g_appId = 0;
SteamAPICall g_pendingCall = 0;
ULONGLONG g_deadline = 0;
bool g_initialized = false, g_host = false, g_connected = false, g_joining = false;
void(__cdecl *g_runCallbacks)() = nullptr;

template <typename Function> bool ResolveSteamFunction(Function &function, const char *name) {
    function = reinterpret_cast<Function>(GetProcAddress(g_api.module, name));
    return function != nullptr;
}

class InviteCallback final : public SteamCallbackBase {
    void Run(void *parameter) override {
        if (parameter && !g_lobbyId)
            g_invitedLobby = static_cast<const SteamLobbyJoinRequested *>(parameter)->lobbyId;
    }
    void Run(void *parameter, bool failed, SteamAPICall) override {
        if (!failed)
            Run(parameter);
    }
    std::int32_t GetCallbackSizeBytes() override {
        return sizeof(SteamLobbyJoinRequested);
    }
};
SteamNetworkingIdentity SteamIdentity(SteamId id) {
    SteamNetworkingIdentity identity{};
    identity.type = 16;
    identity.size = sizeof(id);
    identity.steamId = id;
    return identity;
}

class SessionCallback final : public SteamCallbackBase {
    void Run(void *parameter) override {
        if (!parameter || !g_messages || !g_peerId)
            return;
        const SteamNetworkingIdentity &remote = static_cast<const SteamNetworkingMessagesSessionRequest *>(parameter)->remote;
        if (remote.type == 16 && remote.size == sizeof(SteamId) && remote.steamId == g_peerId)
            g_messages->AcceptSessionWithUser(remote);
    }
    void Run(void *parameter, bool failed, SteamAPICall) override {
        if (!failed)
            Run(parameter);
    }
    std::int32_t GetCallbackSizeBytes() override {
        return sizeof(SteamNetworkingMessagesSessionRequest);
    }
};
InviteCallback g_inviteCallback;
SessionCallback g_sessionCallback;
} // namespace

bool InitializeBindings() {
    if (g_initialized)
        return true;
    g_api.module = GetModuleHandleW(L"steam_api64.dll");
    if (!g_api.module || !ResolveSteamFunction(g_runCallbacks, "SteamAPI_RunCallbacks"))
        return false;
    const bool resolved = ResolveSteamFunction(g_api.getUser, "SteamAPI_GetHSteamUser") && ResolveSteamFunction(g_api.getPipe, "SteamAPI_GetHSteamPipe") &&
        ResolveSteamFunction(g_api.createInterface, "SteamInternal_CreateInterface") &&
        ResolveSteamFunction(g_api.getSteamUser, "SteamAPI_ISteamClient_GetISteamUser") &&
        ResolveSteamFunction(g_api.getSteamFriends, "SteamAPI_ISteamClient_GetISteamFriends") &&
        ResolveSteamFunction(g_api.getSteamUtils, "SteamAPI_ISteamClient_GetISteamUtils") &&
        ResolveSteamFunction(g_api.getSteamMatchmaking, "SteamAPI_ISteamClient_GetISteamMatchmaking") &&
        ResolveSteamFunction(g_api.getGenericInterface, "SteamAPI_ISteamClient_GetISteamGenericInterface") &&
        ResolveSteamFunction(g_api.getSteamId, "SteamAPI_ISteamUser_GetSteamID") && ResolveSteamFunction(g_api.getAppId, "SteamAPI_ISteamUtils_GetAppID") &&
        ResolveSteamFunction(g_api.isOverlayEnabled, "SteamAPI_ISteamUtils_IsOverlayEnabled") &&
        ResolveSteamFunction(g_api.isCallCompleted, "SteamAPI_ISteamUtils_IsAPICallCompleted") &&
        ResolveSteamFunction(g_api.getCallResult, "SteamAPI_ISteamUtils_GetAPICallResult") &&
        ResolveSteamFunction(g_api.activateInviteDialog, "SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog") &&
        ResolveSteamFunction(g_api.createLobby, "SteamAPI_ISteamMatchmaking_CreateLobby") &&
        ResolveSteamFunction(g_api.joinLobby, "SteamAPI_ISteamMatchmaking_JoinLobby") &&
        ResolveSteamFunction(g_api.leaveLobby, "SteamAPI_ISteamMatchmaking_LeaveLobby") &&
        ResolveSteamFunction(g_api.getLobbyMemberCount, "SteamAPI_ISteamMatchmaking_GetNumLobbyMembers") &&
        ResolveSteamFunction(g_api.getLobbyMemberByIndex, "SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex") &&
        ResolveSteamFunction(g_api.getLobbyData, "SteamAPI_ISteamMatchmaking_GetLobbyData") &&
        ResolveSteamFunction(g_api.setLobbyData, "SteamAPI_ISteamMatchmaking_SetLobbyData") &&
        ResolveSteamFunction(g_api.setLobbyJoinable, "SteamAPI_ISteamMatchmaking_SetLobbyJoinable") &&
        ResolveSteamFunction(g_api.getLobbyOwner, "SteamAPI_ISteamMatchmaking_GetLobbyOwner") &&
        ResolveSteamFunction(g_api.registerCallback, "SteamAPI_RegisterCallback") &&
        ResolveSteamFunction(g_api.unregisterCallback, "SteamAPI_UnregisterCallback");
    if (!resolved) {
        Leave();
        return false;
    }
    g_hUser = g_api.getUser();
    g_hPipe = g_api.getPipe();
    if (!g_hUser || !g_hPipe) {
        Leave();
        return false;
    }
    g_client = g_api.createInterface("SteamClient017");
    if (!g_client) {
        Leave();
        return false;
    }
    g_user = g_api.getSteamUser(g_client, g_hUser, g_hPipe, "SteamUser019");
    g_friends = g_api.getSteamFriends(g_client, g_hUser, g_hPipe, "SteamFriends015");
    g_utils = g_api.getSteamUtils(g_client, g_hPipe, "SteamUtils009");
    g_matchmaking = g_api.getSteamMatchmaking(g_client, g_hUser, g_hPipe, "SteamMatchMaking009");
    g_messages = static_cast<SteamNetworkingMessages *>(g_api.getGenericInterface(g_client, g_hUser, g_hPipe, "SteamNetworkingMessages002"));
    if (!g_user || !g_friends || !g_utils || !g_matchmaking || !g_messages) {
        Leave();
        return false;
    }
    g_localId = g_api.getSteamId(g_user);
    g_appId = g_api.getAppId(g_utils);
    if (!g_localId || !g_appId) {
        Leave();
        return false;
    }

    if (g_appId != 758330 || !g_runCallbacks)
        return false;
    g_inviteCallback.callback = 333;
    g_sessionCallback.callback = 1251;
    g_inviteCallback.flags = g_sessionCallback.flags = 1;
    g_api.registerCallback(&g_inviteCallback, 333);
    g_api.registerCallback(&g_sessionCallback, 1251);
    g_initialized = true;

    return true;
}

bool Initialize() {
    if (InitializeBindings())
        return true;
    Release();
    return false;
}

bool BeginHost() {
    if (!Initialize())
        return false;
    if (!g_api.isOverlayEnabled(g_utils)) {
        Leave();
        return false;
    }
    if (g_host && g_lobbyId && !g_connected && !g_pendingCall) {
        g_deadline = GetTickCount64() + 120000;
        g_api.activateInviteDialog(g_friends, g_lobbyId);
        return true;
    }
    if (g_lobbyId || g_pendingCall)
        return false;
    g_host = true;
    g_joining = false;
    g_pendingCall = g_api.createLobby(g_matchmaking, 0, 2);
    g_deadline = GetTickCount64() + 15000;
    return g_pendingCall != 0;
}

void Join(std::uint64_t lobby) {
    if (!lobby || !Initialize() || g_lobbyId || g_pendingCall)
        return;
    g_host = false;
    g_joining = true;
    g_pendingCall = g_api.joinLobby(g_matchmaking, lobby);
    g_lobbyId = lobby;
    g_deadline = GetTickCount64() + 15000;
}

void Update() {
    if (!g_initialized)
        return;
    g_runCallbacks();
    if (g_invitedLobby && !g_lobbyId && !g_pendingCall) {
        const SteamId lobby = g_invitedLobby;
        g_invitedLobby = 0;
        Join(lobby);
    }
    if (g_pendingCall) {
        bool failed = false;
        if (!g_api.isCallCompleted(g_utils, g_pendingCall, &failed)) {
            if (GetTickCount64() > g_deadline)
                Leave();
            return;
        }
        if (failed) {
            Leave();
            return;
        }
        if (!g_joining) {
            SteamLobbyCreatedResult result{};
            if (!g_api.getCallResult(g_utils, g_pendingCall, &result, sizeof(result), 513, &failed) || failed || result.result != 1) {
                Leave();
                return;
            }
            g_pendingCall = 0;
            g_lobbyId = result.lobbyId;
            if (!g_api.setLobbyData(g_matchmaking, g_lobbyId, kProtocolKey, kProtocol)) {
                Leave();
                return;
            }
            g_deadline = GetTickCount64() + 120000;
            g_api.activateInviteDialog(g_friends, g_lobbyId);
        } else {
            SteamLobbyEnterResult result{};
            if (!g_api.getCallResult(g_utils, g_pendingCall, &result, sizeof(result), 504, &failed) || failed || result.response != 1 ||
                result.lobbyId != g_lobbyId) {
                Leave();
                return;
            }
            g_pendingCall = 0;
            g_joining = false;
            const char *protocol = g_api.getLobbyData(g_matchmaking, g_lobbyId, kProtocolKey);
            if (!protocol || std::strcmp(protocol, kProtocol) != 0) {
                Leave();
                return;
            }
        }
    }
    if (!g_lobbyId || g_joining)
        return;
    if (!g_connected && GetTickCount64() > g_deadline) {
        Leave();
        return;
    }
    const int count = g_api.getLobbyMemberCount(g_matchmaking, g_lobbyId);
    const SteamId owner = g_api.getLobbyOwner(g_matchmaking, g_lobbyId);
    if (count > 2 || !owner || (g_host && owner != g_localId) || (!g_host && owner == g_localId)) {
        Leave();
        return;
    }
    SteamId peer = 0;
    for (int index = 0; index < count; ++index) {
        const SteamId member = g_api.getLobbyMemberByIndex(g_matchmaking, g_lobbyId, index);
        if (member && member != g_localId)
            peer = member;
    }
    if (g_connected && peer != g_peerId) {
        Leave();
        return;
    }
    if (peer && peer != g_peerId) {
        const SteamNetworkingIdentity identity = SteamIdentity(peer);
        if (!g_messages->AcceptSessionWithUser(identity)) {
            Leave();
            return;
        }
    }
    g_peerId = peer;
    g_connected = peer != 0;
    if (g_host && g_connected)
        g_api.setLobbyJoinable(g_matchmaking, g_lobbyId, false);
}

bool Connected() {
    return g_connected;
}
bool Active() {
    return g_lobbyId || g_pendingCall;
}
std::uint64_t Lobby() {
    return g_lobbyId;
}
bool Host() {
    return g_host;
}

bool Send(const void *bytes, std::uint32_t size) {
    if (!g_connected || !g_messages)
        return false;
    const SteamNetworkingIdentity peer = SteamIdentity(g_peerId);
    return g_messages->SendMessageToUser(peer, bytes, size, 9, kChannel) == 1;
}

int Receive(void *bytes, std::uint32_t size) {
    if (!g_connected || !g_messages)
        return 0;
    SteamNetworkingMessage *message = nullptr;
    const int count = g_messages->ReceiveMessagesOnChannel(kChannel, &message, 1);
    if (count < 0)
        return -1;
    if (!count)
        return 0;
    if (!message)
        return -1;
    const bool valid = message->size == static_cast<std::int32_t>(size) && message->data && message->peer.type == 16 && message->peer.size == sizeof(SteamId) &&
        message->peer.steamId == g_peerId;
    if (valid)
        std::memcpy(bytes, message->data, size);
    message->release(message);
    return valid ? 1 : -1;
}

void Leave() {
    if (g_messages && g_peerId) {
        const SteamNetworkingIdentity peer = SteamIdentity(g_peerId);
        g_messages->CloseChannelWithUser(peer, kChannel);
        g_messages->CloseSessionWithUser(peer);
    }
    if (g_matchmaking && g_lobbyId)
        g_api.leaveLobby(g_matchmaking, g_lobbyId);
    g_lobbyId = g_peerId = g_pendingCall = g_invitedLobby = 0;
    g_connected = g_joining = g_host = false;
}

void Release() {
    Leave();
    if (g_initialized) {
        g_api.unregisterCallback(&g_sessionCallback);
        g_api.unregisterCallback(&g_inviteCallback);
        g_initialized = false;
    }
    g_api = {};
    g_client = g_user = g_friends = g_utils = g_matchmaking = nullptr;
    g_messages = nullptr;
    g_hUser = g_hPipe = 0;
    g_localId = 0;
    g_appId = 0;
    g_runCallbacks = nullptr;
}
} // namespace shenmue::s1::multiplayer_steam
#endif
