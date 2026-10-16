#pragma once

#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace shenmue::s2 {

inline constexpr std::uint8_t motnTimedCommandWidths[] = {0, 4, 4, 16, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 4, 8, 8, 8, 16, 12, 28, 4, 8, 8, 4, 8,
    8, 8, 8, 8, 12, 8, 12, 8, 8, 4, 8, 8, 4, 4, 4, 4, 16, 8, 12, 8, 4, 8, 4, 4};

struct MT7Model;
struct Character;
struct MotionWork;
float DecodeMotnFloat(std::uint16_t raw);
inline constexpr shen_name TASK_TMNM{SHEN_NAME(TMNM)};
struct TimedNodeMotion {
    std::uint32_t mode{};
    std::uint32_t padding04{};
    const std::byte *data{};
    MT7Model *model{};
    float frame{};
    float speed{};
    float begin{};
    float end{};
    std::uint16_t flags{};
    std::byte padding2a[6]{};
};
static_assert(sizeof(TimedNodeMotion) == 0x30);
static_assert(offsetof(TimedNodeMotion, data) == 0x08);
static_assert(offsetof(TimedNodeMotion, model) == 0x10);
static_assert(offsetof(TimedNodeMotion, frame) == 0x18);
static_assert(offsetof(TimedNodeMotion, speed) == 0x1c);
static_assert(offsetof(TimedNodeMotion, begin) == 0x20);
static_assert(offsetof(TimedNodeMotion, end) == 0x24);
static_assert(offsetof(TimedNodeMotion, flags) == 0x28);


bool AllocateTMNM(Character *character);
void TMNT_Callback(Character *character, void *payload);
void InitializeTimedNodeMotion(TimedNodeMotion &motion, MT7Model *model, std::uint16_t id);
void ClampTimedNodeMotionFrame(TimedNodeMotion &motion, const std::byte *data);
bool TickTimedNodeMotion(TimedNodeMotion &motion);


void ApplyNodeMotionFrame(MT7Model &model, std::span<const std::byte> data, float frame);
std::uintptr_t ConfigureScenarioNodeMotion(shen_name character, std::int32_t motionId, float begin, float end, float frame, float speed, std::int32_t flags);
bool IsScenarioNodeMotionFinished(shen_name character);
TimedNodeMotion *StopScenarioNodeMotion(shen_name character);
void CopyScenarioNodeMotionTimes(shen_name character, float *begin, float *end, float *frame, float *speed);


struct MotionRegistryEntry {
    std::int32_t motionId; // +0x00 bank handle
    std::uint32_t reserved04;
    std::byte *data;           // +0x08 owned MOTI allocation
    std::uint16_t firstMotion; // +0x10 exclusive lower bound
    std::uint16_t lastMotion;  // +0x12 exclusive upper bound
    std::uint32_t reserved14;
    MotionRegistryEntry *previous;
    MotionRegistryEntry *next;
    std::uint32_t motionCount;
    std::uint32_t reserved2c;
};

struct MotnKey {
    std::int16_t frame{};
    float value{};
    float leftSlope{};
    float rightSlope{};
};

struct MotnScalarChannel {
    std::vector<MotnKey> keys;
};


struct MotnFootstepCueCommand {
    std::uint16_t frame{};
    std::int8_t material{};
    std::uint32_t cue{};
};



struct MotnRandomFootstepCueCommand {
    std::uint16_t frame{};
    std::int8_t material{};
    std::array<std::uint32_t, 3> cues{};
};



struct MotnPlayerMotionFlagCommand {
    std::uint16_t frame{};
    std::uint32_t flags{};
};

static_assert(sizeof(MotnFootstepCueCommand) == 8);
static_assert(offsetof(MotnFootstepCueCommand, cue) == 4);
static_assert(sizeof(MotnRandomFootstepCueCommand) == 16);
static_assert(offsetof(MotnRandomFootstepCueCommand, cues) == 4);
static_assert(sizeof(MotnPlayerMotionFlagCommand) == 8);
static_assert(offsetof(MotnPlayerMotionFlagCommand, flags) == 4);

struct MotnSequence {
    std::string name;
    std::uint32_t index{};
    std::uint16_t animationFlags{};
    std::uint16_t frameCount{};
    bool allowsFrameOverflow{};
    bool hasLoopCommand{};
    bool hasLeadFootCommand{};
    std::vector<MotnFootstepCueCommand> footstepCues;
    std::vector<MotnRandomFootstepCueCommand> randomFootstepCues;
    std::vector<MotnPlayerMotionFlagCommand> playerMotionFlagCommands;
    std::vector<MotnScalarChannel> scalars;

    const std::byte *armTrack{};
    const std::byte *regionTrack{};
};

struct MotnFile {
    std::uint16_t animationIdBase{};
    std::uint16_t animationIdEnd{};
    std::vector<MotnSequence> sequences;
};

bool ReadMotnFile(std::span<const std::byte> bytes, MotnFile &file);
const MotnSequence *FindMotnSequence(const MotnFile &file, std::uint32_t runtimeId);
const MotnSequence *FindLoadedMotionSequence(std::uint32_t runtimeId);


std::span<const std::byte> FindLoadedMotionData(std::uint32_t runtimeId);
std::uint32_t GetMotionFrameCount(std::uint16_t animation);


std::span<const std::byte> FindLoadedMotionMetadata(std::uint32_t runtimeId);
// motnFindSecondaryMotionEntry: borrowed startup command, stopping at opcodes 0/8.
std::span<const std::byte> FindLoadedMotionSecondaryEntry(std::uint32_t runtimeId, std::uint8_t opcode);
std::span<const std::byte> FindLoadedMotionTimedCommand(
    std::uint32_t runtimeId, std::uint8_t opcode, float beginFrame, float endFrame, std::uint32_t match = 1);

inline constexpr shen_name MOTION_SNAPSHOT_EMPTY_CHARACTER{SHEN_NAME(____)};
std::int32_t CaptureMotionSnapshot(shen_name character);
bool RestoreMotionSnapshot(shen_name character, std::uint32_t slot);
bool RestoreMotionSnapshotPlacement(shen_name character, std::uint32_t slot);
bool ReleaseMotionSnapshot(std::uint32_t slot);
void RegisterAuthorMotionBanks(std::span<const MotnFile> banks);
bool IsRetiredMotionId(std::uint16_t motionId);


std::int32_t ReadMotiFromDirectory(std::int32_t act, shen_name map, const char *filename);
std::int32_t LoadMOTNPCTable(const char *directory, const char *filename);

std::int32_t LoadMoti(std::span<const std::byte> bytes);
bool DeleteBorrowedMotion(std::int32_t handle);
// DeleteMotion: unlink, retire the ID range, and release its allocation.
bool DeleteMotion(std::int32_t handle);
// MotionPropFBX: links an in-memory MOTP bank (borrowed, never freed here); returns its handle or -1.
std::int32_t MotionPropFBX(std::span<const std::byte> motp);
// LoadMotionProps: six linked MPT slots, including the empty permanent head.
std::int32_t LoadMotionProps(const char *directory, const char *filename, shen_name allocationName = SHEN_NAME(NPCp));
bool DeleteMotionProps(std::int32_t handle);
// MOTPParser: unlinks a package-owned MOTP bank (MotionPropFBX handle) without freeing it.
bool MOTPParser(std::int32_t handle);
std::int32_t ReadMotionPropsFromDirectory(std::int32_t act, shen_name map, const char *filename);
// Borrowed MOTN descriptors are registered/retired at layer promotion.
void RegisterMotionPropTrack(const void *track, std::uint32_t region);
void RetireMotionPropTrack(const void *track, std::uint32_t region);
void ReleaseMotionPropTracks(const MotionWork &work);
void DrawMotionParts(Character &character, MotionWork &work, bool forceDisable);
void QueueMotionRetirementTask(std::uint8_t nextFunctionIndex = 13);
void FreeMotionRetirementTask();
float SampleMotnScalar(const MotnScalarChannel &channel, float time);
void SampleMotnScalarPVA(
    const MotnScalarChannel &channel, float time, float &value, float &incomingHalfTangent, float &outgoingHalfTangent, bool linear = false);

static_assert(sizeof(MotionRegistryEntry) == 0x30);
static_assert(offsetof(MotionRegistryEntry, data) == 0x08);
static_assert(offsetof(MotionRegistryEntry, previous) == 0x18);
static_assert(offsetof(MotionRegistryEntry, motionCount) == 0x28);

std::byte *AllocateAndLoadMOTFiles();


std::int64_t AllocateMotionBanks();
void AllocateARMM();

extern std::array<MotionRegistryEntry, 32> g_motionRegistry;
extern MotionRegistryEntry *g_motionData;
extern MotionRegistryEntry *g_battleMotionData;
extern std::byte *g_fallbackMotionTable;
extern std::array<std::int32_t *, 4> g_fallbackMotionSections;
extern std::int32_t g_motionBankCount;
extern std::uint32_t g_totalMotionCount;
extern std::byte *g_armMotionMemory;
extern std::byte *g_unfoSpace;

} // namespace shenmue::s2
