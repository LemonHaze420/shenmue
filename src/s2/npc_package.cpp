#include "s2/npc_package_internal.hpp"
#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace shenmue::s2 {
using namespace npc_package_detail;

Vector3 npcPlayerPosition{};
float npcPlayerRadius{};
float npcHourWeatherForecast{};
std::int32_t g_usingCustomAuthorCharacterList{};
std::int32_t npcPoolTarget{34};
namespace npc_package_detail {

constexpr std::uint32_t afsSignature = make_shen_name('A', 'F', 'S', '\0');
constexpr std::size_t npcPackageCapacity = 64;

static_assert(sizeof(NpcNoSpawnArea) == 0x14);

static_assert(sizeof(AfsEntry) == 8);

std::array<LoadedNpcPackage, npcPackageCapacity> loadedNpcPackages{};
std::size_t loadedNpcPackageCount{};
std::uint32_t npcNewLoadState{};
NpcRegistryTask npcRegistryTask{};
std::span<NpcRouteLiveEntry> npcRouteEntries;
std::span<NpcRouteLiveEntry> npcEntryStorage;
std::span<std::uint8_t> npcSlotFlags;
std::span<std::byte> npcBinaryStorage;
bool npcSceneCheckEnabled{};
std::size_t npcAuthoredEntryCount{};
constexpr std::array<std::byte, 0x38> npcScriptRecord{
    std::byte{'M'}, std::byte{'O'}, std::byte{'V'}, std::byte{'E'}, std::byte{0}, std::byte{20}, std::byte{1}, std::byte{1}};

// @verified - p
NpcRouteLiveEntry *FindActiveNpcEntry(shen_name name) {
    if (!npcIsNpcTaskActive())
        return nullptr;
    for (NpcRouteLiveEntry &entry : npcRouteEntries)
        if (entry.name.name == name.name)
            return &entry;
    return nullptr;
}
float npcCrowdDensityScale{};
float npcEndOfDayWeatherForecast{};
std::uint32_t npcWaitFrames{};
bool npcReloadPassStarted{};
std::array<char, 64> npcScheduleFilename{};
bool npcWaitBypassActive{};
bool npcWaitSawLoadFailure{};
std::int32_t npcCurrentTimeSimState{};
std::int32_t npcIsTimeAdvancing{};
std::int32_t npcTimeAdvanceBreak{};
std::uint32_t npcSimDeltaTime{};
std::uint32_t npcPresentedTime{};
std::int32_t npcLatestTime{};
DateTime npcPausedDateTime{};
NpcRouteLiveEntry *npcCrowdHead{};
NpcRouteLiveEntry *npcCrowdBookmark{};
Vector3 npcPlayerCenter{};
float npcPlayerDisplacement{};
bool npcPlayerPresent{};
std::uint32_t npcRouteBuildCount{};
std::uint32_t npcFrameCount{};
std::uint8_t npcCrowdPhase{};
std::int32_t npcActiveStreamCost{};
std::int32_t npcPoolMinimum{-1};
float npcStreamThrottle{};
float npcStreamThrottleWithBias{4.5f};
std::uint32_t npcColdResetMode{1};
NpcRouteResetGroupHeader npcEmptyRouteResetGroup{};
NpcRouteResetGroupHeader *npcRouteResetGroup{&npcEmptyRouteResetGroup};
std::array<NpcTransitionRuntimeSlot, 4> npcTransitionRuntimeSlots{};

std::array<TimedNodeMotion, 4> npcTransitionMotionCaches{};
bool npcDrawActive{};
shen_name npcInterpolatingCharacter{};
shen_name npcLoadedArea{};
const shen_name *areaNpcLoadCharacters{};
HLib::Task *areaNpcLoadStartupTask{};
std::int32_t areaNpcLoadPhase{};
std::int32_t areaNpcLoadCharactersVisible{1};
std::int32_t areaNpcLoadLowQualityHands{1};


const shen_name *genericNpcLoadCharacters{};
HLib::Task *genericNpcLoadStartupTask{};
std::int32_t genericNpcLoadPhase{};
std::int32_t genericNpcLoadCharactersVisible{1};
std::int32_t genericNpcLoadLowQualityHands{1};

NpcScheduleStorage npcSchedule;
std::array<shen_name, 2> npcScheduleSeedNames{};
DateTime npcCurrentDateTime{};
std::uint32_t npcCurrentSeconds{};
std::uint32_t npcCondition2StartSeconds{};
std::uint32_t npcCondition2OutsideFrames{};
const std::byte *pendingNpcScheduleSeeds{};
bool hasPendingNpcScheduleSeeds{};
NpcRouteLiveEntry *npcScriptRouteOwner{};
NpcRouteStateContext *npcActiveRouteContext{};
NpcRouteStateContext npcCachedAreaRouteContext{};
shen_name npcCachedAreaRouteCharacter{};
std::uint32_t npcCachedAreaRouteId{};
std::array<NpcMotionClip, 8> npcMotionClips{};
bool npcMotionClipsLoaded{};
bool npcUsePositionCullCallback{};
std::array<std::int32_t, 32> npcMotionBankHandles{};
std::size_t npcMotionBankHandleCount{};
bool npcMotionBanksLoaded{};
std::array<std::int32_t, 5> npcMotionPropHandles{};
std::size_t npcMotionPropHandleCount{};
static_assert(sizeof(NpcTimedMotionSlot) == 0x58);
static_assert(offsetof(NpcTimedMotionSlot, animation) == 0x18);
static_assert(offsetof(NpcTimedMotionSlot, offset) == 0x48);
std::array<NpcTimedMotionSlot, 2> npcTimedMotionSlots{};
std::int32_t npcTimedMotionCount{};

std::vector<NpcRouteRuntimeRecord> npcRouteRuntimeRecords;

constexpr std::array<std::uint8_t, 59> npcOpcodeSizeDwords{
    1,
    2,
    2,
    2,
    3,
    2,
    2,
    5,
    6,
    4,
    3,
    5,
    3,
    2,
    2,
    3,
    7,
    2,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    6,
    2,
    3,
    2,
    4,
    5,
    3,
    2,
    2,
    5,
    5,
    2,
    3,
    4,
    3,
    9,
    2,
    4,
    5,
    3,
    3,
    2,
    2,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
    3,
    2,
};
std::int32_t npcPositionQueryMode;
std::byte *npcHumanIndex;
std::int32_t npcHumanCount;
NpcHumanPakSlot *npcHumanSlots;

PakAfsPartition *npcHumansAfs;
std::int32_t npcLastHumanLoadResult;
std::int32_t npcHumansInUse;
bool npcHasCachedPak;
std::uint64_t npcLargestSceneBlock;
std::uint64_t npcLargestDynamicBlock;

bool Contains(std::span<const std::byte> bytes, std::uint64_t offset, std::uint64_t size) {
    return offset <= bytes.size() && size <= bytes.size() - offset;
}

bool AllocateNpcTimedMotion(NpcRouteLiveEntry &entry) {
    if (npcTimedMotionCount == 2)
        return false;
    for (NpcTimedMotionSlot &slot : npcTimedMotionSlots) {
        if (slot.occupied == 1)
            continue;
        ++npcTimedMotionCount;
        slot.occupied = 1;
        slot.character = nullptr;
        entry.timedMotionHandle = &slot;
        return true;
    }
    return false;
}

void ReleaseNpcTimedMotion(NpcRouteLiveEntry &entry) {
    auto *const slot = static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle);
    if (!slot)
        return;
    if (slot->character)
        slot->character->renderFlags &= ~0x04U;
    slot->character = nullptr;
    entry.flags &= ~0x400U;
    --npcTimedMotionCount;
    slot->occupied = 0;
    entry.timedMotionHandle = nullptr;
}
constexpr std::size_t npcAttachFlagsOffset = 0x310;
constexpr std::size_t npcCurrentMotionOffset = 0x346;
constexpr std::size_t npcClipOverrideOffset = 0x34c;
constexpr std::size_t npcLocalBranchCountOffset = 0x34e;
constexpr std::size_t npcMotionSpeedOffset = 0x380;
constexpr shen_name ryoName{SHEN_NAME(RYO_)};

static_assert(sizeof(NpcLocalBranchTrigger) == 12);
static_assert(sizeof(NpcGlobalBranchTrigger) == 0x10);
static_assert(offsetof(NpcGlobalBranchTrigger, packedCondition) == 0x04);
static_assert(offsetof(NpcGlobalBranchTrigger, commandOffset) == 0x0e);
std::array<NpcGlobalBranchTrigger, 16> npcGlobalBranchTriggers{};
static_assert(npcWorldPositionOffset + sizeof(Vector3) <= sizeof(NpcRouteLiveEntry));

NpcRouteRuntimeRecord *GetNpcRuntimeRecord(NpcRouteLiveEntry &entry) {
    if (npcRouteEntries.empty() || npcRouteRuntimeRecords.size() != npcRouteEntries.size()) {
        return nullptr;
    }
    const std::uintptr_t first = reinterpret_cast<std::uintptr_t>(npcRouteEntries.data());
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(&entry);
    if (address < first)
        return nullptr;
    const std::uintptr_t delta = address - first;
    if (delta % sizeof(NpcRouteLiveEntry) != 0)
        return nullptr;
    const std::size_t index = delta / sizeof(NpcRouteLiveEntry);
    return index < npcRouteRuntimeRecords.size() ? &npcRouteRuntimeRecords[index] : nullptr;
}

bool NpcRecordContains(const NpcRouteRuntimeRecord &runtime, const std::byte *cursor, std::size_t size) {
    if (!runtime.begin || !runtime.end || !cursor)
        return false;
    const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(runtime.begin);
    const std::uintptr_t end = reinterpret_cast<std::uintptr_t>(runtime.end);
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(cursor);
    return begin <= address && address <= end && size <= end - address;
}

float ReadF32(const std::byte *source) {
    float value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

} // namespace

namespace npc_package_detail {
static_assert(sizeof(NpcAvoidanceCandidate) == 0x40);
std::array<NpcAvoidanceCandidate, 5> npcAvoidanceCandidates{};
NpcAvoidanceCandidate *npcAvoidanceFirst{};
NpcAvoidanceCandidate *npcAvoidanceLast{};
float npcAvoidanceLow{};
float npcAvoidanceHigh{};
float npcAvoidanceLateral{};
float npcAvoidanceRadius{};
float npcAvoidanceDistance{};

bool ReadArchiveFile(const char *path, std::vector<std::byte> &bytes) {
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (!size || size > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
        return false;
    }
    bytes.resize(static_cast<std::size_t>(size));
    return ArchiveData_Read(path, 0, bytes.data(), bytes.size());
}

bool ReadCString(std::span<const std::byte> bytes, std::uint32_t offset, std::string &value) {
    value.clear();
    if (!offset || !Contains(bytes, offset, 1))
        return false;
    for (std::size_t cursor = offset; cursor < bytes.size(); ++cursor) {
        const char character = static_cast<char>(bytes[cursor]);
        if (!character)
            return true;
        value.push_back(character);
    }
    return false;
}

} // namespace

namespace npc_package_detail {

bool ReadArchiveRange(const char *path, const AfsEntry &entry, std::vector<std::byte> &bytes) {
    if (!entry.size || entry.size > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
        return false;
    }
    bytes.resize(entry.size);
    return ArchiveData_Read(path, entry.offset, bytes.data(), bytes.size());
}

bool ExtensionEquals(const PakContainerEntry &entry, const char *extension) {
    for (std::size_t index = 0; index < entry.extension.size(); ++index) {
        const auto left = static_cast<unsigned char>(entry.extension[index]);
        const auto right = static_cast<unsigned char>(extension[index]);
        if (std::toupper(left) != std::toupper(right))
            return false;
    }
    return true;
}

std::span<const std::byte> EntryBytes(const PakContainerView &container, const PakContainerEntry &entry) {
    const std::uint64_t offset = static_cast<std::uint64_t>(container.ipacOffset) + entry.relativeOffset;
    if (offset > container.bytes.size() || entry.size > container.bytes.size() - offset) {
        return {};
    }
    return container.bytes.subspan(static_cast<std::size_t>(offset), entry.size);
}

void CopyEntryName(const PakContainerEntry &entry, std::array<char, 14> &output) {
    std::size_t cursor = 0;
    for (const char value : entry.stem) {
        if (!value)
            break;
        output[cursor++] = value;
    }
    if (cursor + 1 < output.size())
        output[cursor++] = '.';
    for (const char value : entry.extension) {
        if (!value || cursor + 1 >= output.size())
            break;
        output[cursor++] = value;
    }
    output[cursor] = '\0';
}

} // namespace



void npcInitData() {
    npcLoadedArea = {};
    npcWaitBypassActive = false;
    npcWaitSawLoadFailure = false;
    npcWaitFrames = 0;
    npcRegistryTask.current = nullptr;
    npcRegistryTask.pending = nullptr;
    npcNewLoadState = 0;
    npcHumansInUse = 0;
    npcHumanIndex = nullptr;
    npcHumanSlots = nullptr;
    npcHumanCount = 0;
    npcBinaryStorage = {};
    npcEntryStorage = {};
    npcSlotFlags = {};
    AllocateNPCH();
    npcInitDataInternal();
    npcScheduleFilename = {};
    ResetNpcScheduleSeeds();
}

// @verified - p
std::int64_t InitializeNpcFlags() {
    npcColdResetMode = 1;
    npcAuthoredEntryCount = 0;
    npcSchedule.areaPointCount = 0;
    npcSchedule.routeNodeCount = 0;
    npcLoadedArea = {};
    npcWaitBypassActive = false;
    npcWaitSawLoadFailure = false;
    npcWaitFrames = 0;
    npcRegistryTask.current = nullptr;
    npcRegistryTask.pending = nullptr;
    npcHasCachedPak = false;
    npcNewLoadState = 0;
    npcCurrentTimeSimState = 0;
    npcHumansInUse = 0;
    npcScheduleFilename[0] = 0;
    ResetNpcScheduleSeeds();
    return 0;
}

std::span<NpcRouteLiveEntry> CreateNpcRouteLiveEntries(std::span<const NpcRouteRecordSource> records) {
    for (auto &trigger : npcGlobalBranchTriggers)
        trigger.packedCondition = 0xffffffffU;
    UnpublishNpcRouteLiveEntries();
    const std::size_t oldCount = std::min(npcRouteEntries.size(), npcRouteRuntimeRecords.size());
    for (std::size_t index = 0; index < oldCount; ++index) {
        ReleaseNpcTimedMotion(npcRouteEntries[index]);
        ReleaseNpcRouteStream(npcRouteEntries[index], npcRouteRuntimeRecords[index], true);
        npcRouteEntries[index].previous = nullptr;
        npcRouteEntries[index].next = nullptr;
    }
    npcRouteEntries = {};
    npcAuthoredEntryCount = records.size();
    const std::size_t count = records.size() + npcScriptEntryCount;
    if (count > npcEntryStorage.size())
        return {};
    npcRouteEntries = npcEntryStorage.first(count);
    std::uninitialized_value_construct_n(npcRouteEntries.data(), count);
    std::fill_n(npcSlotFlags.data(), count, std::uint8_t{});
    npcRouteRuntimeRecords.assign(count, {});
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        const bool authored = index < records.size();
        entry.name = authored ? records[index].name : SHEN_NAME(MOVE);
        entry.recordBase = authored ? records[index].recordBase : npcScriptRecord.data();
        entry.humanPackageIndex = authored ? records[index].humanPackageIndex : static_cast<std::int16_t>(index);
        entry.previous = index == 0 ? nullptr : &npcRouteEntries[index - 1];
        entry.next = index + 1 == npcRouteEntries.size() ? nullptr : &npcRouteEntries[index + 1];
        if (index < npcSchedule.records.size()) {
            const ParsedNpcRecord &parsed = npcSchedule.records[index];
            const std::byte *const begin = npcSchedule.bytes.data() + parsed.offset;
            if (begin == entry.recordBase) {
                npcRouteRuntimeRecords[index] = {begin, npcSchedule.bytes.data() + parsed.endOffset, parsed.branchCount};
            }
        }
        InitializeNpcInterpreterState(entry, static_cast<std::int32_t>(index));
        entry.poolIndex = static_cast<std::int16_t>(index);
        if (!authored) {
            entry.flags = NpcRouteAbstractActive;
            entry.liveState = 1;
            entry.streamSortKey = 1000.0f;
            entry.currentCommand = entry.branchCommandStart = nullptr;
        }
        entry.stateContext.owner = &entry;
        entry.stateContext.motionWork = entry.motionWork;
        // npcInitStateCtxCore bypasses the route-build quota until
        // the initialized context reaches its ordinary route state.
        entry.stateContext.flags = 0x100U;
        entry.stateContext.stateTable =
            npcSchedule.bytes.empty() ? nullptr : reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
        YieldNpcLoadingPresentation();
    }
    npcCrowdHead = npcCrowdBookmark = npcRouteEntries.data();
    npcCrowdPhase = 0;
    npcFrameCount = 0;
    npcRouteBuildCount = 0;
    npcReloadPassStarted = false;
    npcColdResetMode = 1;
    npcWaitFrames = 0;
    npcStreamThrottle = 0.0f;
    npcActiveStreamCost = 0;
    npcPositionQueryMode = 0;
    if (npcRegistryTask.current) {
        PublishNpcRouteLiveEntries(npcRouteEntries);
    }
    return npcRouteEntries;
}

std::span<NpcRouteLiveEntry> GetNpcRouteLiveEntries() {
    return npcRouteEntries;
}

void FreeNpcRouteLiveEntries() {
    // Invalidate readers before retiring Character/MTWK and package owners.
    UnpublishNpcRouteLiveEntries();
    ReleaseNpcTransitionHumanPackages();
    npcDrawActive = false;
    const std::size_t count = std::min(npcRouteEntries.size(), npcRouteRuntimeRecords.size());
    for (std::size_t index = 0; index < count; ++index) {
        ReleaseNpcTimedMotion(npcRouteEntries[index]);
        ReleaseNpcRouteStream(npcRouteEntries[index], npcRouteRuntimeRecords[index], true);
        ResetNpcAreaRouteState(npcRouteEntries[index]);
        npcRouteEntries[index].previous = nullptr;
        npcRouteEntries[index].next = nullptr;
    }
    ResetNpcAreaRouteResources();
    std::destroy_n(npcRouteEntries.data(), npcRouteEntries.size());
    npcRouteEntries = {};
    npcRouteRuntimeRecords.clear();
    npcSchedule = {};
    npcRouteResetGroup = &npcEmptyRouteResetGroup;
    npcWaitFrames = 0;
    npcAuthoredEntryCount = 0;
    npcCrowdHead = npcCrowdBookmark = nullptr;
    npcActiveStreamCost = 0;
    npcPlayerPresent = false;
}

std::int64_t RegisterNpcFlags(shen_name character, std::uint32_t flags, std::int32_t operation) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (operation == 0)
            entry.flags &= ~flags;
        else if (operation == 1)
            entry.flags |= flags;
        else if (operation == 2 && !(entry.flags & flags))
            return 0;
        return 1;
    }
    return 0;
}

namespace npc_package_detail {
static_assert(sizeof(ConditionalNpcLoadWork) == 0x30);
static_assert(offsetof(ConditionalNpcLoadWork, admitted) == 0x0c);
std::array<ConditionalNpcLoadSlot, 8> conditionalNpcLoadSlots{};
std::int32_t conditionalNpcLoadsReady{};
}

const LoadedNpcPackage *LoadNpcPackage(shen_name name, const char *scene) {
    for (LoadedNpcPackage &package : loadedNpcPackages) {
        if (package.references && package.name.name == name.name) {
            if (package.references != (std::numeric_limits<std::size_t>::max)()) {
                ++package.references;
            }
            return &package;
        }
    }
    if (!scene || loadedNpcPackageCount >= loadedNpcPackages.size()) {
        return nullptr;
    }
    const auto freeSlot =
        std::find_if(loadedNpcPackages.begin(), loadedNpcPackages.end(), [](const LoadedNpcPackage &package) { return package.references == 0; });
    if (freeSlot == loadedNpcPackages.end())
        return nullptr;

    std::array<char, 64> idxPath{};
    std::array<char, 64> afsPath{};
    std::snprintf(idxPath.data(), idxPath.size(), "scene/%s/npc/humans.idx", scene);
    std::snprintf(afsPath.data(), afsPath.size(), "scene/%s/npc/humans.afs", scene);

    const std::uint64_t idxSize = ArchiveData_GetFileSize(idxPath.data());
    if (idxSize < 4 || idxSize > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
        return nullptr;
    }
    std::vector<std::byte> indexBytes(static_cast<std::size_t>(idxSize));
    if (!ArchiveData_Read(idxPath.data(), 0, indexBytes.data(), indexBytes.size())) {
        return nullptr;
    }
    const std::uint16_t count = ReadU16(indexBytes.data());
    if (!count || 4ULL + static_cast<std::uint64_t>(count) * 4 > indexBytes.size()) {
        return nullptr;
    }
    std::int32_t packageIndex = -1;
    for (std::uint16_t index = 0; index < count; ++index) {
        if (ReadU32(indexBytes.data() + 4 + index * 4) == name.name) {
            packageIndex = index;
            break;
        }
    }
    if (packageIndex < 0)
        return nullptr;

    const std::uint64_t afsSize = ArchiveData_GetFileSize(afsPath.data());
    const std::size_t directorySize = 8 + static_cast<std::size_t>(count) * 16;
    if (afsSize < directorySize)
        return nullptr;
    std::vector<std::byte> directory(directorySize);
    if (!ArchiveData_Read(afsPath.data(), 0, directory.data(), directory.size()) || ReadU32(directory.data()) != afsSignature ||
        ReadU32(directory.data() + 4) != static_cast<std::uint32_t>(count) * 2) {
        return nullptr;
    }
    AfsEntry pkf{}, pks{};
    std::memcpy(&pkf, directory.data() + 8 + packageIndex * 16, sizeof(pkf));
    std::memcpy(&pks, directory.data() + 16 + packageIndex * 16, sizeof(pks));
    if (static_cast<std::uint64_t>(pkf.offset) + pkf.size > afsSize || static_cast<std::uint64_t>(pks.offset) + pks.size > afsSize) {
        return nullptr;
    }

    std::vector<std::byte> packedPkf;
    std::vector<std::byte> packedPks;
    if (!ReadArchiveRange(afsPath.data(), pkf, packedPkf) || !ReadArchiveRange(afsPath.data(), pks, packedPks)) {
        return nullptr;
    }
    std::array<char, 5> packageName{};
    std::memcpy(packageName.data(), &name.name, 4);
    const LoadedMapPackage *const pak = LoadPakPackageFromMemory(packageName.data(), packedPkf, packedPks);
    if (!pak)
        return nullptr;

    LoadedNpcPackage result{};
    result.name = name;
    result.pak = pak;
    result.references = 1;
    for (const PakContainerEntry &entry : pak->pks.entries) {
        if (result.model.empty() && ExtensionEquals(entry, "CHRM")) {
            result.model = EntryBytes(pak->pks, entry);
            CopyEntryName(entry, result.modelName);
        } else if (result.behavior.empty() && ExtensionEquals(entry, "BIN\0")) {
            result.behavior = EntryBytes(pak->pks, entry);
        } else if (result.characterTable.empty() && ExtensionEquals(entry, "CHRT")) {
            result.characterTable = EntryBytes(pak->pks, entry);
        }
    }
    if (result.characterTable.empty()) {
        for (const PakContainerEntry &entry : pak->pkf.entries) {
            if (ExtensionEquals(entry, "CHRT")) {
                result.characterTable = EntryBytes(pak->pkf, entry);
                break;
            }
        }
    }
    if (result.model.empty()) {
        UnloadMapPackage(packageName.data());
        return nullptr;
    }
    *freeSlot = result;
    ++loadedNpcPackageCount;
    return &*freeSlot;
}

void ReleaseNpcPackage(const LoadedNpcPackage *package) {
    if (!package)
        return;
    const auto found = std::find_if(loadedNpcPackages.begin(), loadedNpcPackages.end(),
        [package](const LoadedNpcPackage &candidate) { return &candidate == package && candidate.references != 0; });
    if (found == loadedNpcPackages.end())
        return;
    if (found->references > 1) {
        --found->references;
        return;
    }

    std::array<char, 5> packageName{};
    std::memcpy(packageName.data(), &found->name.name, 4);
    *found = {};
    --loadedNpcPackageCount;
    UnloadMapPackage(packageName.data());

    const std::span<const LoadedMapPackage> mapPackages = GetLoadedMapPackages();
    for (LoadedNpcPackage &candidate : loadedNpcPackages) {
        if (!candidate.references)
            continue;
        candidate.pak = nullptr;
        for (const LoadedMapPackage &mapPackage : mapPackages) {
            if (std::strncmp(mapPackage.name.data(), reinterpret_cast<const char *>(&candidate.name.name), 4) == 0) {
                candidate.pak = &mapPackage;
                break;
            }
        }
    }
}

void FreeNpcPackages() {
    // Route entries retire their Character/MTWK owners before any package
    // allocation backing their model spans is released.
    ReleaseNpcMapResources();
    for (LoadedNpcPackage &package : loadedNpcPackages) {
        if (!package.references)
            continue;
        std::array<char, 5> packageName{};
        std::memcpy(packageName.data(), &package.name.name, 4);
        package = {};
        UnloadMapPackage(packageName.data());
    }
    loadedNpcPackageCount = 0;
}

// @verified - p
std::int64_t EV_NPC_NewLoadSw(std::uint32_t state) {
    const bool starting = state == 1 && npcNewLoadState == 0;
    npcNewLoadState = state;
    if (starting) {
        npcWaitBypassActive = true;
        npcWaitSawLoadFailure = false;
        npcWaitFrames = 0;
    }
    return state;
}

} // namespace shenmue::s2
