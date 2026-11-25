#include "s2/conversation_playback_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "s2/stream_audio_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/conversation_fade_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/startup_assets_dc.hpp"
#include "platform/dc/aica.hpp"
#include "platform/dc/audio.hpp"
#include <cstdio>
#include <cstring>
extern "C" void ADXT_DestroyAll();
extern "C" void ADXF_CloseAll();
#include "s2/face_controller_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/area_names.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/pak_dc.hpp"

namespace shenmue {
namespace s2 {

namespace {

void StreamingAudioErrorNullsub(void *, char *);
void SceneSoundInitializationNullsub();

} // namespace

void *AllocateAudioPartitionInfo(std::int32_t entryCount);
void ResetAudioPartitions();
AudioPartition *MountAudioPartition(const char *directory, const char *filename, std::int32_t entries, void *providedInfo);

void StopConversationStream(ConversationStream &stream);
void MountSceneConversationIndex();
std::uint32_t *LoadSceneAudioIndex(const char *name, std::int32_t scene, std::int32_t allocateFromBack);
SceneAudioBank g_sceneMusicBanks[2] = {};
void *g_sceneMusicPartitionInfo[2] = {};
AudioPartition g_audioPartitions[16];
std::int32_t g_audioPartitionCount = 0;

void InitializeStreamingAudio() {
    SOND_ConfigureStreamSlots(1, 1, 0);
    dc::sdk::BindAudioStreams();
    ADXT_Init();
    ADXF_Init();
    ADXT_EntryErrFunc(StreamingAudioErrorNullsub, 0);
    g_audioPartitionCount = 0;
    ResetAudioPartitions();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    MountSceneConversationIndex();
#else
    SceneSoundInitializationNullsub();
#endif
}

// @verified - d
void FinishStreamingAudio() {
    ADXT_DestroyAll();
    ADXF_CloseAll();
    ADXT_Finish();
}

void CloseAllAudioFiles() {
    ADXF_CloseAll();
}

// @verified - d
void AllocateConversationStreamBuffers() {
    g_conversationVoiceMemory = HLib::AllocateTaskData(0xd904, CONVERSATION_VOICE_STORAGE);
    g_sharedConversationStream = 0;
}

// @verified - d
bool AllocateConversationVoice(ConversationStream &stream, std::int32_t output) {
    const std::int32_t state = g_sharedConversationStream ? ADXT_GetStat(static_cast<ADXT>(g_sharedConversationStream)) : 0;
    if (output == 2 || state != 0) {
        std::int32_t bytes = 0xd904;
        if (output == 2)
            bytes = 0x1a9c4;
        stream.compressedBuffer = HLib::AllocateTaskData(bytes, CONVERSATION_VOICE_STORAGE);
        if (!stream.compressedBuffer) {
            stream.handle = 0;
            return false;
        }
        stream.handle = ADXT_Create(2, stream.compressedBuffer, bytes);
    } else {
        stream.compressedBuffer = g_conversationVoiceMemory;
        stream.handle = ADXT_Create(1, stream.compressedBuffer, 0xd904);
        g_sharedConversationStream = stream.handle;
    }
    if (!stream.handle)
        return false;
    return true;
}

// @verified - d
void InitializeSceneSoundBanks() {
    const std::int32_t act = g_loadedActNumber;
    if (!act)
        return;
    char name[12];
    std::sprintf(name, "BGM0%d", static_cast<int>(act));
    std::strcpy(g_sceneMusicBanks[0].name, name);
    g_sceneMusicBanks[0].index = LoadSceneAudioIndex(name, act, 0);
    g_sceneMusicPartitionInfo[0] = AllocateAudioPartitionInfo(GetConversationIndexEntryCount(g_sceneMusicBanks[0].index));
    std::sprintf(name, "FREE0%d", static_cast<int>(act));
    std::strcpy(g_sceneMusicBanks[1].name, name);
    g_sceneMusicBanks[1].index = LoadSceneAudioIndex(name, act, 0);
    g_sceneMusicPartitionInfo[1] = AllocateAudioPartitionInfo(GetConversationIndexEntryCount(g_sceneMusicBanks[1].index));
}

namespace {

// @verified - d
void StreamingAudioErrorNullsub(void *, char *) {
}

// @verified - d
void SceneSoundInitializationNullsub() {
}

} // namespace

void ReleaseConversationVoice(ConversationStream &stream) {
    ADXT_Destroy(static_cast<ADXT>(stream.handle));
    if (stream.compressedBuffer != g_conversationVoiceMemory)
        HLib::FreeTaskData(stream.compressedBuffer);
}

// @verified - d
bool StartConversationStream(ConversationStream &stream, std::int32_t partition, std::uint32_t entry, std::int32_t output) {
    StopConversationStream(stream);
    if (!AllocateConversationVoice(stream, output))
        return false;
    stream.volume = -1;
    stream.baseAttenuation = -1;
    stream.frameScale = 256;
    stream.cameraCode = 0;
    stream.paused = 0;
    stream.remainingFrames = 600;
    ADXT_StartAfs(static_cast<ADXT>(stream.handle), partition, entry);
    return true;
}

// @verified - d
std::int32_t IsConversationStreamActive(ConversationStream &stream) {
    if (!stream.handle)
        return false;
    const std::int32_t state = ADXT_GetStat(static_cast<ADXT>(stream.handle));
    if (state == 4) {
        stream.remainingFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(stream.remainingFrames) - 1U);
        if (stream.remainingFrames < 1)
            return false;
    }
    return static_cast<std::uint32_t>(state) - 1U < 4U;
}

// @verified - d
void SetConversationStreamPauseState(ConversationStream *stream, std::int32_t paused) {
    if (stream->handle) {
        ADXT_Pause(static_cast<ADXT>(stream->handle), paused);
        stream->paused = paused;
    }
}

void SetConversationStreamBaseAttenuation(ConversationStream &stream, std::int32_t attenuation) {
    stream.baseAttenuation = attenuation;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetConversationStreamPitch(ConversationStream &stream, std::int32_t pitch) {
    stream.frameScale = static_cast<std::uint32_t>(pitch);
}
#endif

// @verified - d
void SetConversationStreamCameraCode(ConversationStream &stream, std::int32_t cameraCode) {
    stream.cameraCode = cameraCode;
}

// @verified - d
void UpdateConversationStream(ConversationStream &stream) {
    if (!stream.handle)
        return;
    const std::uint32_t state = static_cast<std::uint32_t>(ADXT_GetStat(static_cast<ADXT>(stream.handle)));
    if (state - 2U > 2U)
        return;
    if (stream.volume != -1) {
        ADXT_SetOutVol(static_cast<ADXT>(stream.handle), stream.volume);
        stream.volume = -1;
    }
    if (stream.baseAttenuation != -1) {
        ADXT_SetOutPan(static_cast<ADXT>(stream.handle), 0, stream.baseAttenuation);
        stream.baseAttenuation = -1;
    }
    if (state == 2)
        return;
    if (stream.frameScale != 256) {
        ADXT_SetTranspose(static_cast<ADXT>(stream.handle), static_cast<std::int32_t>(stream.frameScale), 0);
        stream.frameScale = 256;
    }
    if (stream.cameraCode) {
        ADXT_SetFx(static_cast<ADXT>(stream.handle), 0, CONVERSATION_CAMERA_REVERB_ATTENUATION[stream.cameraCode]);
        stream.cameraCode = 0;
    }
}

// @verified - d
void *ReadConversationPacket(AudioPartition *partition, std::uint32_t entry, std::int32_t sector, ConversationPacketBuffer *packet) {
    ADXF const handle = ADXF_OpenAfs(partition->id, entry);
    ADXF_Seek(handle, sector, 0);
    ADXF_ReadNw32(handle, 1, packet);
    return handle;
}

// @verified - d
void ReleaseConversationPacket(void *request) {
    ADXF const handle = static_cast<ADXF>(request);
    if (handle) {
        if (handle->stm)
            ADXF_Stop(handle);
        ADXF_Close(handle);
    }
}

// @verified - d
std::int32_t IsConversationPacketReadPending(void *request) {
    if (!request)
        return false;
    const std::int32_t state = ADXF_GetStat(static_cast<ADXF>(request));
    return state != 3 && state != 0 && state != 4;
}

void ReserveAudioPartitionSlot() {
    ++g_audioPartitionCount;
}

// @verified - d
void *AllocateAudioPartitionInfo(std::int32_t entryCount) {
    return HLib::AllocateTaskData(((static_cast<std::uint32_t>(entryCount) + 0x20U) >> 1) << 2, AUDIO_PARTITION_STORAGE);
}

// @verified - d
void ResetAudioPartitions() {
    for (std::int32_t index = g_audioPartitionCount; index < 16; ++index)
        g_audioPartitions[index].id = -1;
}

AudioPartition *MountAudioPartitionDefault(const char *directory, const char *filename, std::int32_t entries) {
    return MountAudioPartition(directory, filename, entries, 0);
}

AudioPartition *MountAudioPartition(const char *directory, const char *filename, std::int32_t entries, void *providedInfo) {
    std::int32_t index = 0;
    while (index < 16 && g_audioPartitions[index].id != -1)
        ++index;
    if (index == 16)
        return 0;
    AudioPartition &partition = g_audioPartitions[index];
    partition.id = index;
    partition.handle = providedInfo ? providedInfo : AllocateAudioPartitionInfo(entries);
    SelectDiscDirectory(directory);
    const std::int32_t result = ADXF_LoadPartition(index, const_cast<char *>(filename), partition.handle, entries);
    if (result)
        return 0;
    ProcessDiscFileReads();
    return &partition;
}

} // namespace s2
} // namespace shenmue

#endif
