#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "common/types.hpp"

#define SHENMUE_AUDIO_NULL 0
#define SHENMUE_AUDIO_NOEXCEPT

namespace shenmue { namespace s2 {
using ::shenmue::shen_name;
#define VOICE_INDEX_STORAGE shen_name(SHEN_NAME4('C', 'V', 'I', 'X'))
#define AUDIO_PARTITION_STORAGE shen_name(SHEN_NAME4('A', 'D', 'F', 'S'))

struct SceneAudioBank {
    void *stream;
    std::uint32_t *index;
    char name[24];
};
struct AudioPartition {
    std::int32_t id;
    void *handle;
};
typedef char sizeofSceneAudioBankCheck[sizeof(SceneAudioBank) == 0x20 ? 1 : -1];
typedef char offsetofSceneAudioBankindexCheck[offsetof(SceneAudioBank, index) == 4 ? 1 : -1];
typedef char offsetofSceneAudioBanknameCheck[offsetof(SceneAudioBank, name) == 8 ? 1 : -1];
typedef char sizeofAudioPartitionCheck[sizeof(AudioPartition) == 8 ? 1 : -1];

extern SceneAudioBank g_sceneMusicBanks[2];
extern void *g_sceneMusicPartitionInfo[2];
extern AudioPartition g_audioPartitions[16];
extern std::int32_t g_audioPartitionCount;
void BuildSceneAudioPath(const char *name, const char *extension, char *directory, char *filename, std::int32_t scene);
std::uint32_t *LoadSceneAudioIndex(const char *name, std::int32_t scene, std::int32_t allocateFromBack);
void *AllocateAudioPartitionInfo(std::int32_t entryCount);
void InitializeSceneSoundBanks();
void ResetAudioPartitions();
AudioPartition *MountAudioPartition(const char *directory, const char *filename, std::int32_t entries, void *providedInfo = SHENMUE_AUDIO_NULL);
void ReleaseAudioPartition(AudioPartition *partition);
void InitializeStreamingAudio();
void FinishStreamingAudio();
} } // namespace shenmue::s2
#undef SHENMUE_AUDIO_NULL
#undef SHENMUE_AUDIO_NOEXCEPT
#endif
