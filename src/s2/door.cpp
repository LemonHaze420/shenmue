#include "s2/door.hpp"

#include "s2/door_area_tables.hpp"
#include "s2/area_event.hpp"
#include "s2/conversation.hpp"
#include "s2/area_jump.hpp"
#include "s2/load_title.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "logging.hpp"
#include "s2/archive_data.hpp"
#include "s2/flag.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/lighting.hpp"
#include "s2/sound_program.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/world_render.hpp"
#include "s2/player.hpp"
#include "s2/magic_room.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <limits>
#include <span>
#include <vector>

namespace shenmue::s2 {

Vector3i g_doorIngressRotation{};
shen_name g_doorQueuedAuthor{};

namespace {

constexpr float kDoorInteractionRadiusSquared = 2.5600002f;
constexpr float kAutomaticDoorRadiusSquared = 0.8f;
constexpr float kAutomaticDoorOpenRadiusSquared = 1.9320999f;
constexpr Matrix4x4 kDoorIdentity{{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
}};
constexpr float kTau = 6.2831853071795864769f;

struct RigidMotionTrack {
    std::uint16_t scaleOffset{};
    Vector3 scale{1.0f, 1.0f, 1.0f};
    std::array<std::uint16_t, 3> rotationOffsets{};
    Vector3 rotation{};
    std::array<std::uint16_t, 3> translationOffsets{};
    Vector3 translation{};
};

struct RigidMotion {
    std::uint16_t id{};
    std::int16_t durationFrames{};
    float authoredRate{};
    std::span<const std::byte> source;
    std::vector<RigidMotionTrack> tracks;
};

struct AnimatedDoorTarget {
    LoadedMapNodeBinding binding{};
    Vector3 initialPosition{};
    Vector3i initialRotation{};
    Vector3 initialScale{};
};

struct DoorEntry {
    DoorRuntimeEntryState state{};
    LoadedMapNodeBinding primaryBinding{};
    LoadedMapNodeBinding secondaryBinding{};
    DoorTaskEntry *taskEntry{};
    MT7Node motionNode{};
    MT7Node *originalPrimaryChild{};
    std::uint32_t originalPrimaryMesh{};
    std::byte *originalPrimaryRuntimeMesh{};
    MT7Node *numberRoot{};
    MT7Node *originalNumberChild{};
    std::array<MT7Node, 4> numberNodes{};
    std::array<LegacyNodeCommandOverride, 4> numberCommands{};
    std::vector<AnimatedDoorTarget> animationTargets;
};

struct DoorRuntimeOwner {
    shen_name temporaryAuthor{};
    HLib::Task *task{};
    shen_name previousAutomaticDoor{};
    float previousAutomaticLocalZ{};
    std::vector<DoorEntry> entries;
    std::vector<std::size_t> sortedEntries;
    std::vector<RigidMotion> rigidMotions;
    std::array<Character *, 2> collisionCharacters{};
    std::int32_t ingressDoorId{};
    std::int32_t ingressSide{};
    std::int32_t ingressLeafSide{};
    Vector3 ingressPosition{};
    Vector3 ingressOrigin{};
    std::vector<std::uint16_t> missingMotionIds;
    Vector3 cameraStartPosition{};
    Vector3 cameraStartTarget{};
    Vector3 cameraEndPosition{};
    Vector3 cameraEndTarget{};
    CameraState previousCameraState{CameraState::Init};
    std::int32_t cameraFrame{};
    std::int32_t cameraDuration{};
    bool cameraActive{};
    bool entriesActive{};
    DoorAreaCleanupCallback cleanupCallback{};
    DoorAreaCleanupCallback savedCleanupCallback{};
    std::int32_t cachedSceneId{};
    std::int32_t currentStreamId{};
    DoorTaskEntry *taskEntries{};
    DoorTaskEntry *sortedHead{};
    DoorTaskEntry *sortedTail{};
    std::int32_t authorPhase{};
    shen_name authorDoor{};
    std::int32_t destinationPhase{};
    shen_name destinationDoor{};
    shen_name destinationArea{};
    std::int32_t destinationEntry{};
    std::int32_t crossingControlAcquired{};
    std::int32_t savedAreaJump{};
    float crossingLocalZ{};
    CameraState authorCameraState{};
    Vector3 authorCameraPosition{};
};

DoorRuntimeOwner doorRuntime;
shen_name doorSearchRegion{};

std::array<MapEventCallbackEntry, 32> doorMapEvents{};

// @verified - p
void DoorMapEventNullCallback(std::uintptr_t) {
}

const DoorAreaAdmission *FindDoorAreaTable() {
    const std::int32_t act = g_actNumber == 2 ? 1 : g_actNumber;
    for (const DoorAreaAdmissionSelector &selector : doorAreaAdmissionSelectors) {
        if (selector.act == act && selector.area.name == g_currentMap.name)
            return doorAreaAdmissions.data() + selector.first;
    }
    return nullptr;
}

shen_name FindDoorByNumber(std::int8_t number) {
    const std::uint32_t suffix =
        static_cast<std::uint32_t>((number % 10) * 0x1000000) + static_cast<std::uint32_t>((number / 10) * 0x10000) + make_shen_name('\0', '\0', '0', '0');
    for (const DoorEntry &entry : doorRuntime.entries) {
        if ((entry.state.primary.name & 0xffff0000U) == suffix)
            return entry.state.primary;
    }
    return {};
}

void SetDoorAuthorSecondaryNodes(shen_name author, std::int32_t visible) {
    const DoorAreaAdmission *table = FindDoorAreaTable();
    if (!table)
        return;
    while (table->area.name != author.name) {
        if (table->event[2] == 0xffffffffU)
            return;
        ++table;
    }
    for (unsigned index = 0; index < 2; ++index) {
        const shen_name subject = FindDoorByNumber(static_cast<std::int8_t>(table->admission >> (8U * (index + 1))));
        for (DoorEntry &entry : doorRuntime.entries) {
            if (entry.state.primary.name != subject.name)
                continue;
            MT7Node *const node = entry.taskEntry->secondary;
            if (node) {
                node->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | ((visible == 0 ? 1U : 0U) << 16U));
                node->visible = visible != 0;
            }
            break;
        }
    }
}
bool DoorAdmissionFileExists(const char *path) {
    std::byte unused{};
    return ArchiveData_GetFileSize(path) != 0 || ArchiveData_Read(path, 0, &unused, 0);
}

void AdmitDoorAreaTable() {
    const DoorAreaAdmission *const table = FindDoorAreaTable();
    if (!table)
        return;
    std::size_t eventCount = 0;
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    for (const DoorAreaAdmission *row = table; row->event[2] != 0xffffffffU; ++row) {
        if (row->event[0] || row->event[1]) {
            doorMapEvents[eventCount++] = {DoorMapEventNullCallback, static_cast<std::int32_t>(row->event[2]), row->event[3],
                static_cast<std::uintptr_t>(row->area.name) | (static_cast<std::uintptr_t>(row->reserved14) << 32U)};
        }
        const std::int8_t entryId = static_cast<std::int8_t>(row->admission);
        if (!entryId)
            continue;
        const std::int32_t stream = static_cast<std::int8_t>(row->admission >> 24U);
        char path[128]{};
        bool authorPackage = false;
        if (entryId > 0) {
            std::snprintf(path, sizeof(path), "/%s/%.4s.PKF", directory, row->area.characters);
            authorPackage = DoorAdmissionFileExists(path);
            if (!authorPackage) {
                std::snprintf(path, sizeof(path), "/scene/%02d/%.4s/mapinfo.bin", g_actNumber, row->area.characters);
                if (!DoorAdmissionFileExists(path))
                    continue;
            }
        }
        for (unsigned index = 0; index < 2; ++index) {
            const shen_name subject = FindDoorByNumber(static_cast<std::int8_t>(row->admission >> (8U * (index + 1))));
            if (!subject.name)
                continue;
            for (DoorEntry &entry : doorRuntime.entries) {
                if (entry.state.primary.name != subject.name)
                    continue;
                if (authorPackage)
                    entry.taskEntry->authorPackage = row->area;
                else {
                    entry.taskEntry->destination = row->area;
                    entry.taskEntry->destinationEntry = entryId < 0 ? -entryId : entryId;
                }
                entry.taskEntry->stream = stream;
                break;
            }
        }
    }
    const DoorAreaAdmission *sentinel = table;
    while (sentinel->event[2] != 0xffffffffU)
        ++sentinel;
    doorMapEvents[eventCount] = {
        nullptr, -1, sentinel->event[3], static_cast<std::uintptr_t>(sentinel->area.name) | (static_cast<std::uintptr_t>(sentinel->reserved14) << 32U)};
    if (eventCount)
        SetMapEventCallbackTable(doorMapEvents.data());
}



std::array<char, 16> cachedDoorCutsceneId{};
std::array<char, 16> cachedDoorSoundBankId{};
std::int32_t cachedDoorSoundSceneIndex{};
std::uint32_t doorLightingReaderMask{};
MT7Model doorNumberModel;
void *doorNumberAllocation{};
std::array<const MT7Node *, 10> doorNumberTemplates{};

DoorEntry *FindDoorEntry(std::uint32_t doorId) {
    if (!doorRuntime.entriesActive)
        return nullptr;

    for (DoorEntry &entry : doorRuntime.entries) {
        if (entry.state.primary.name == doorId)
            return &entry;
    }
    return nullptr;
}

Vector3 DoorLocalPosition(const DoorEntry &entry, const Vector3 &position) {
    const Matrix4x4 &transform = entry.taskEntry->worldTransform;
    const float dx = position.x - transform.rows[3].x;
    const float dy = position.y - transform.rows[3].y;
    const float dz = position.z - transform.rows[3].z;
    return {
        dy * transform.rows[0].y + dx * transform.rows[0].x + dz * transform.rows[0].z,
        dy * transform.rows[1].y + dx * transform.rows[1].x + dz * transform.rows[1].z,
        dy * transform.rows[2].y + dx * transform.rows[2].x + dz * transform.rows[2].z,
    };
}

Vector3i ExtractDoorRotation(const Matrix4x4 &transform) {
    Vector3i rotation{};
    const float sine = -transform.rows[0].z;
    if (sine - 1.0f < -0.0000076295109f) {
        if (1.0f - transform.rows[0].z > 0.0000076295109f) {
            rotation.x = FixedAtan2(transform.rows[1].z, transform.rows[2].z);
            rotation.y = FixedAtan2(sine, std::sqrt(1.0f - sine * sine));
            rotation.z = FixedAtan2(transform.rows[0].y, transform.rows[0].x);
        } else {
            rotation.x = FixedAtan2(-transform.rows[1].x, transform.rows[1].y);
            rotation.y = 0xc000;
        }
    } else {
        rotation.x = FixedAtan2(transform.rows[1].x, transform.rows[1].y);
        rotation.y = 0x4000;
    }

    if (rotation.x >= 0x8000 && rotation.z >= 0x8000) {
        rotation.x -= 0x8000;
        rotation.z -= 0x8000;
        rotation.y = 0x8000 - rotation.y;
    }
    return rotation;
}

#include "s2/door_metadata.inl"



// @verified - p
const DoorAreaMetadata *GetDataForCurrentMapAndScene(shen_name region) {
    std::span<const DoorAreaMetadataMap> maps{};
    if (g_actNumber <= 0)
        return nullptr;
    if (g_actNumber <= 2)
        maps = DOOR_METADATA_ACTS_1_2;
    else if (g_actNumber == 3)
        maps = DOOR_METADATA_ACT_3;
    else if (g_actNumber == 4)
        maps = DOOR_METADATA_ACT_4;
    else
        return nullptr;
    const DoorAreaMetadata *metadata = nullptr;
    for (const DoorAreaMetadataMap &map : maps)
        if (map.map.name == g_currentMap.name)
            metadata = map.entries;
    if (!metadata)
        return nullptr;
    const std::int32_t tens = static_cast<std::uint8_t>(region.name >> 16U);
    const std::int32_t door = static_cast<std::uint8_t>(region.name >> 24U) + 2 * (tens + 4 * (tens - 66));
    for (; metadata->door != 0; ++metadata)
        if (metadata->door == door)
            return metadata;
    return nullptr;
}

template <typename Value> bool ReadValue(const std::byte *data, std::size_t size, std::size_t offset, Value &value) {
    if (!data || offset > size || sizeof(Value) > size - offset)
        return false;
    std::memcpy(&value, data + offset, sizeof(Value));
    return true;
}

template <typename Value> bool ReadValue(std::span<const std::byte> data, std::size_t offset, Value &value) {
    return ReadValue(data.data(), data.size(), offset, value);
}

float DecodeS2Float(std::uint16_t raw) {
    if (raw == 0)
        return 0.0f;
    const std::uint32_t bits = (static_cast<std::uint32_t>(raw & 0x8000U) << 16U) | (static_cast<std::uint32_t>((raw & 0x7fffU) + 0x1c000U) << 13U);
    return std::bit_cast<float>(bits);
}

bool CurveRangeValid(std::span<const std::byte> source, std::uint16_t relative, std::size_t bytesPerFrame, std::int16_t durationFrames) {
    if (relative == 0)
        return true;

    const std::size_t frameCount = static_cast<std::size_t>(durationFrames) + 1;
    if (frameCount > (std::numeric_limits<std::size_t>::max)() / bytesPerFrame) {
        return false;
    }
    const std::size_t curveOffset = relative;
    const std::size_t curveSize = frameCount * bytesPerFrame;
    return curveOffset <= source.size() && curveSize <= source.size() - curveOffset;
}

bool ParseRigidMotion(std::uint16_t runtimeId, RigidMotion &motion) {
    const std::span<const std::byte> source = FindLoadedMotionData(runtimeId);
    std::int16_t durationFrames{};
    std::uint16_t marker{};
    std::uint16_t encodedRate{};
    std::uint16_t flags{};
    std::uint16_t transformCount{};
    if (!ReadValue(source, 0, durationFrames) || !ReadValue(source, 2, marker) || !ReadValue(source, 4, encodedRate) || !ReadValue(source, 6, flags) ||
        !ReadValue(source, 8, transformCount) || durationFrames < 0 || marker != 0xffffU || (flags & 0xffdbU) != 0 || transformCount == 0 ||
        transformCount > 0x7fU || 12U + static_cast<std::size_t>(transformCount) * 32U > source.size()) {
        return false;
    }

    motion = {};
    motion.id = runtimeId;
    motion.durationFrames = durationFrames;
    motion.authoredRate = DecodeS2Float(encodedRate);
    motion.source = source;
    motion.tracks.resize(transformCount);
    for (std::size_t index = 0; index < motion.tracks.size(); ++index) {
        const std::size_t at = 12 + index * 32;
        RigidMotionTrack &track = motion.tracks[index];
        std::array<std::uint16_t, 3> encodedScale{};
        std::array<std::int16_t, 3> rotation{};
        std::array<std::uint16_t, 3> encodedTranslation{};
        if (!ReadValue(source, at, track.scaleOffset)) {
            return false;
        }

        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (!ReadValue(source, at + 2 + axis * 2, encodedScale[axis]) || !ReadValue(source, at + 8 + axis * 4, track.rotationOffsets[axis]) ||
                !ReadValue(source, at + 10 + axis * 4, rotation[axis]) || !ReadValue(source, at + 20 + axis * 4, track.translationOffsets[axis]) ||
                !ReadValue(source, at + 22 + axis * 4, encodedTranslation[axis])) {
                return false;
            }

            track.scale[axis] = DecodeS2Float(encodedScale[axis]);
            track.rotation[axis] = static_cast<float>(rotation[axis]);
            track.translation[axis] = DecodeS2Float(encodedTranslation[axis]);
            if (!CurveRangeValid(source, track.rotationOffsets[axis], 2, durationFrames) ||
                !CurveRangeValid(source, track.translationOffsets[axis], 2, durationFrames)) {
                return false;
            }
        }
        if (!CurveRangeValid(source, track.scaleOffset, 6, durationFrames)) {
            return false;
        }
    }
    return true;
}

const RigidMotion *LoadRigidMotion(std::uint16_t runtimeId) {
    for (const RigidMotion &motion : doorRuntime.rigidMotions) {
        if (motion.id == runtimeId)
            return &motion;
    }
    if (std::find(doorRuntime.missingMotionIds.begin(), doorRuntime.missingMotionIds.end(), runtimeId) != doorRuntime.missingMotionIds.end()) {
        return nullptr;
    }
    RigidMotion motion{};
    if (!ParseRigidMotion(runtimeId, motion)) {
        doorRuntime.missingMotionIds.push_back(runtimeId);
        return nullptr;
    }
    doorRuntime.rigidMotions.push_back(std::move(motion));
    return &doorRuntime.rigidMotions.back();
}

template <typename Decode> bool SampleWordCurve(const RigidMotion &motion, std::uint16_t relative, float frame, Decode decode, float &value) {
    if (relative == 0)
        return false;

    const float clamped = std::clamp(frame, 0.0f, static_cast<float>(motion.durationFrames));
    const auto whole = static_cast<std::size_t>(std::floor(clamped));
    const float alpha = clamped < motion.durationFrames ? clamped - static_cast<float>(whole) : 0.0f;
    const std::size_t at = relative + whole * 2;
    std::uint16_t current{};
    if (!ReadValue(motion.source, at, current))
        return false;

    value = decode(current);
    if (alpha == 0.0f)
        return true;

    std::uint16_t next{};
    if (!ReadValue(motion.source, at + 2, next))
        return false;

    value += (decode(next) - value) * alpha;
    return true;
}

void SampleRotationCurve(const RigidMotion &motion, std::uint16_t relative, float frame, float &value) {
    if (relative == 0)
        return;

    const float clamped = std::clamp(frame, 0.0f, static_cast<float>(motion.durationFrames));
    const std::size_t whole = static_cast<std::size_t>(clamped);
    const float alpha = clamped < motion.durationFrames ? clamped - static_cast<float>(whole) : 0.0f;
    const std::size_t at = relative + whole * 2;
    std::uint16_t current{};
    if (!ReadValue(motion.source, at, current))
        return;

    std::int32_t angle = static_cast<std::int16_t>(current);
    if (alpha != 0.0f) {
        std::uint16_t next{};
        if (!ReadValue(motion.source, at + 2, next))
            return;

        const std::int16_t difference = static_cast<std::int16_t>(next - current);
        angle += static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(difference) * alpha));
    }
    value = static_cast<float>(angle);
}

bool SampleRigidMotion(const RigidMotion &motion, float frame, std::size_t trackIndex, Vector3 &translation, Vector3 &rotation, Vector3 &scale) {
    if (trackIndex >= motion.tracks.size())
        return false;
    const RigidMotionTrack &track = motion.tracks[trackIndex];
    translation = track.translation;
    rotation = track.rotation;
    scale = track.scale;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        (void)SampleWordCurve(
            motion, track.translationOffsets[axis], frame, [](std::uint16_t value) { return DecodeS2Float(value); }, translation[axis]);

        SampleRotationCurve(motion, track.rotationOffsets[axis], frame, rotation[axis]);
    }
    if (track.scaleOffset != 0) {
        const float clamped = std::clamp(frame, 0.0f, static_cast<float>(motion.durationFrames));
        const auto whole = static_cast<std::size_t>(std::floor(clamped));
        const float alpha = clamped < motion.durationFrames ? clamped - static_cast<float>(whole) : 0.0f;
        const std::size_t at = track.scaleOffset + whole * 6;
        for (std::size_t axis = 0; axis < 3; ++axis) {
            std::uint16_t current{};
            if (!ReadValue(motion.source, at + axis * 2, current)) {
                return false;
            }

            scale[axis] = DecodeS2Float(current);
            if (alpha != 0.0f) {
                std::uint16_t next{};
                if (!ReadValue(motion.source, at + 6 + axis * 2, next)) {
                    return false;
                }
                scale[axis] += (DecodeS2Float(next) - scale[axis]) * alpha;
            }
        }
    }
    return true;
}

void ApplyRigidMotion(DoorEntry &entry, const RigidMotion &motion, float frame) {
    const std::size_t count = std::min(
        entry.animationTargets.size(), motion.tracks.size());
    for (std::size_t index = 0; index < count; ++index) {
        AnimatedDoorTarget &target = entry.animationTargets[index];
        if (!target.binding.node)
            continue;

        Vector3 translation{}, rotation{}, scale{};
        if (!SampleRigidMotion(motion, frame, index, translation, rotation, scale))
            continue;

        if (index == 0) {
            entry.taskEntry->motionNode.position = translation;
            entry.taskEntry->motionNode.rotation = {
                static_cast<std::int32_t>(rotation.x), static_cast<std::int32_t>(rotation.y), static_cast<std::int32_t>(rotation.z)};
            entry.taskEntry->motionNode.scale = scale;
            entry.motionNode.position = translation;
            entry.motionNode.rotation = entry.taskEntry->motionNode.rotation;
            entry.motionNode.scale = scale;
        } else {
            target.binding.node->position = translation;
            target.binding.node->rotation = {
                static_cast<std::int32_t>(rotation.x), static_cast<std::int32_t>(rotation.y), static_cast<std::int32_t>(rotation.z)};
            target.binding.node->scale = scale;
        }
    }
}

DoorLeafState GetLeafState(const DoorEntry &entry) {
    return static_cast<DoorLeafState>(entry.taskEntry->flags & 3U);
}

void SetLeafState(DoorEntry &entry, DoorLeafState state) {
    entry.taskEntry->flags = static_cast<std::uint16_t>((entry.taskEntry->flags & ~3U) | static_cast<std::uint16_t>(state));
}

void PublishDoorBlocker(DoorEntry &entry) {
    if (entry.state.family == 5 || entry.state.family == 7 || entry.taskEntry->blockingRadius <= 0.0f || !entry.taskEntry->claimant.name ||
        !FindCharacterByScenarioName(entry.taskEntry->claimant)) {
        return;
    }

    const std::int16_t yaw = static_cast<std::int16_t>(entry.taskEntry->motionNode.rotation.y);
    entry.taskEntry->blockingRadius = std::max(std::fabs(sin_fixed16(yaw) * 0.89999998f), 0.1f);
    for (std::size_t index = 0; index < doorRuntime.collisionCharacters.size() && index < doorRuntime.sortedEntries.size(); ++index) {
        if (&entry != &doorRuntime.entries[doorRuntime.sortedEntries[index]])
            continue;
        Character *const character = doorRuntime.collisionCharacters[index];
        CharacterControlModel *const control = FindCharacterControlModel(character);
        if (!control)
            return;

        shenApplyTranslationToAll(&character->entity.node, entry.state.interactionPoint);
        control->flags = 0x000840d9U | CharacterControlPreserveCollisionRadius;
        control->collisionCenterRadius = entry.taskEntry->blockingRadius;
        return;
    }
}

extern std::int32_t drEntryState;
std::uint8_t RegionFamilyCode(shen_name region);
void drUpdateDoorMotionSound(DoorEntry &entry);

bool AdvanceDoorMotion(DoorEntry &entry) {
    if (entry.taskEntry->motionType == 0)
        return false;

    const RigidMotion *motion = nullptr;
    for (const RigidMotion &candidate : doorRuntime.rigidMotions) {
        if (candidate.source.data() == entry.taskEntry->motionDefinition) {
            motion = &candidate;
            break;
        }
    }
    if (!motion)
        return false;

    // TMNM samples the clamped current frame before advancing and retires only
    // on the next out-of-range update.
    const float previousFrame = entry.taskEntry->motionFrame;
    const float duration = static_cast<float>(motion->durationFrames);
    const float firstFrame = std::fmin(duration, std::fmax(0.0f, entry.taskEntry->motionStartFrame));
    const float lastFrame = std::fmin(duration, std::fmax(0.0f, entry.taskEntry->motionEndFrame));
    float sampledFrame = previousFrame;
    if (firstFrame > sampledFrame) {
        if ((entry.taskEntry->motionChannelState & 4U) != 0)
            sampledFrame = std::fmax(firstFrame, sampledFrame - firstFrame + lastFrame);
        else {
            sampledFrame = firstFrame;
            entry.taskEntry->motionType = 2;
        }
    } else if (sampledFrame > lastFrame) {
        if ((entry.taskEntry->motionChannelState & 4U) != 0)
            sampledFrame = std::fmin(lastFrame, firstFrame + sampledFrame - lastFrame);
        else {
            sampledFrame = lastFrame;
            entry.taskEntry->motionType = 2;
        }
    }
    entry.taskEntry->motionStartFrame = firstFrame;
    entry.taskEntry->motionEndFrame = lastFrame;
    ApplyRigidMotion(entry, *motion, sampledFrame);
    const float tick = HLib::GetScaledDeltaTick() * entry.taskEntry->motionSpeed;
    entry.taskEntry->motionFrame = sampledFrame + ((entry.taskEntry->motionChannelState & 0x20U) != 0 ? -tick : tick);
    const bool completed = entry.taskEntry->motionType == 2;

    if (entry.state.family == 7) {
        const bool openingAllowed = GetDoorOpeningState(entry.state.primary) != 0;
        const DoorLeafState transition = GetLeafState(entry);

        if (transition == DoorLeafState::ToB && entry.taskEntry->motionEndFrame - entry.taskEntry->motionFrame < 25.0f &&
            entry.taskEntry->motionEndFrame - previousFrame >= 25.0f) {
            bool disableCollision = openingAllowed;
            if (!disableCollision && !doorRuntime.sortedEntries.empty() && &entry == &doorRuntime.entries[doorRuntime.sortedEntries.front()] &&
                g_playerRuntime.character) {
                const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(entry.state.primary);
                const std::uint8_t sideFlags = metadata ? metadata->flags & 3U : 0;
                Vector3 playerPosition{};
                (void)plymUpdatePlayerSearchLocation(&playerPosition);
                const float localZ = DoorLocalPosition(entry, playerPosition).z;
                disableCollision = sideFlags ? localZ < 0.0f : localZ > 0.0f;
            }
            if (disableCollision)
                (void)ToggleFlagOnFieldEntity(entry.state.primary, false);
        }
        if (transition == DoorLeafState::ToA && entry.taskEntry->motionFrame > 25.0f && previousFrame <= 25.0f) {
            (void)ToggleFlagOnFieldEntity(entry.state.primary, true);
        }
    }

    PublishDoorBlocker(entry);
    if (!completed) {
        if (doorRuntime.destinationPhase == 0 || RegionFamilyCode(doorRuntime.destinationDoor) != '7')
            drUpdateDoorMotionSound(entry);
        return false;
    }

    const DoorLeafState transition = GetLeafState(entry);
    SetLeafState(entry, entry.state.family == 7 && transition == DoorLeafState::ToB ? DoorLeafState::StableB : DoorLeafState::StableA);
    entry.taskEntry->claimant = {};
    entry.taskEntry->blockingRadius = 0.0f;
    entry.state.active = false;
    return true;
}

bool StartDoorMotion(DoorEntry &entry, std::uint16_t motionId, DoorLeafState transition, float startFrame = 0.0f, float speedScale = 1.0f) {
    const RigidMotion *const motion = LoadRigidMotion(motionId);
    if (!motion || entry.animationTargets.empty())
        return false;
    SetLeafState(entry, transition);
    entry.taskEntry->animationId = motionId;
    entry.taskEntry->motionType = 1;
    entry.taskEntry->motionDefinition = motion->source.data();
    entry.taskEntry->motionTarget = &entry.taskEntry->motionNode;
    entry.taskEntry->motionStartFrame = 0.0f;
    entry.taskEntry->motionEndFrame = static_cast<float>(motion->durationFrames);
    entry.taskEntry->motionChannelState = 0;
    entry.taskEntry->motionSpeed = speedScale * motion->authoredRate;
    entry.taskEntry->motionFrame = startFrame;
    entry.state.active = true;
    (void)AdvanceDoorMotion(entry);
    return true;
}

std::uint8_t DoorFamily(shen_name name) {
    const std::uint8_t first = static_cast<std::uint8_t>(name.name);
    const std::uint8_t second = static_cast<std::uint8_t>(name.name >> 8U);
    if (first != 'R' || second < '1' || second > '7')
        return 0;
    return static_cast<std::uint8_t>(second - '0');
}

std::int32_t DoorEventId(shen_name name) {
    const std::uint8_t tens = static_cast<std::uint8_t>(name.name >> 16U);
    const std::uint8_t units = static_cast<std::uint8_t>(name.name >> 24U);
    if (tens < '0' || tens > '9' || units < '0' || units > '9')
        return -1;
    return static_cast<std::int32_t>((tens - '0') * 10 + units - '0');
}

void SetDoorViewSpaceEuler(const Vector3i &rotation);

// @verified - p
MT7Node *FindDoorPropNode(MT7Node *node, std::uint32_t name) {
    if (!node)
        return nullptr;
    if (IsMDPXorMDP7(node) == name)
        return node;
    if (MT7Node *const child = FindDoorPropNode(node->runtimeChild, name))
        return child;
    return FindDoorPropNode(node->runtimeSibling, name);
}

bool FindDoorPropBinding(shen_name name, LoadedMapNodeBinding &binding) {
    binding = {};
    MT7Node *const node = FindDoorPropNode(GetPropRoot(), name.name);
    if (!node)
        return false;
    const std::span<const PropReaderSlot, 17> slots = GetPropReaderSlots();
    for (std::size_t slotIndex = 1; slotIndex < slots.size(); ++slotIndex) {
        MT7Model *const model = slots[slotIndex].reader;
        if (!model)
            continue;
        for (std::size_t nodeIndex = 0; nodeIndex < model->nodes.size(); ++nodeIndex) {
            if (&model->nodes[nodeIndex] != node)
                continue;
            binding = {model, node, slotIndex, nodeIndex};
            return true;
        }
    }
    return false;
}

Vector3 TransformPoint(const Matrix4x4 &transform, const Vector3 &point) {
    return {
        transform.rows[3].x + transform.rows[0].x * point.x + transform.rows[1].x * point.y + transform.rows[2].x * point.z,
        transform.rows[3].y + transform.rows[0].y * point.x + transform.rows[1].y * point.y + transform.rows[2].y * point.z,
        transform.rows[3].z + transform.rows[0].z * point.x + transform.rows[1].z * point.y + transform.rows[2].z * point.z,
    };
}

void AddAnimationTarget(DoorEntry &entry, const LoadedMapNodeBinding &binding) {
    if (!binding.node)
        return;

    entry.animationTargets.push_back({binding, binding.node->position, binding.node->rotation, binding.node->scale});
}

DoorEntry BuildDoorEntry(shen_name primaryName, shen_name secondaryName, DoorTaskEntry *taskEntry) {
    DoorEntry entry{};
    entry.taskEntry = taskEntry;
    entry.state.primary = primaryName;
    entry.state.secondary = secondaryName;
    entry.state.family = DoorFamily(primaryName);
    entry.state.eventId = DoorEventId(primaryName);
    taskEntry->flags = 0x10U;
    if (!FindDoorPropBinding(primaryName, entry.primaryBinding))
        return entry;
    entry.state.primaryResolved = true;
    MT7Node *const primary = entry.primaryBinding.node;
    taskEntry->primary = primary;
    if (secondaryName.name == 0 && primary->runtimeParent) {
        const std::uint32_t parentName = static_cast<std::uint32_t>(IsMDPXorMDP7(primary->runtimeParent));
        if (parentName == kDoorSecondaryPrefix.name)
            secondaryName = shen_name{parentName};
    }
    if (taskEntry->sourcePair)
        const_cast<shen_name *>(taskEntry->sourcePair)[1] = secondaryName;
    entry.state.secondary = secondaryName;
    (void)FindDoorPropBinding(secondaryName, entry.secondaryBinding);
    entry.state.secondaryResolved = entry.secondaryBinding.node != nullptr;
    taskEntry->secondary = entry.secondaryBinding.node;
    if (primary->sourceOffset <= entry.primaryBinding.model->source.size() &&
        sizeof(MT7NodeSource) <= entry.primaryBinding.model->source.size() - primary->sourceOffset) {
        std::memcpy(&taskEntry->motionNode, entry.primaryBinding.model->source.data() + primary->sourceOffset, sizeof(MT7NodeSource));
    }
    taskEntry->motionNode.id = static_cast<std::int32_t>((static_cast<std::uint32_t>(primary->id) & 0xffff0000U) | static_cast<std::uint16_t>(-1337));
    taskEntry->motionNode.position = {};
    taskEntry->motionNode.rotation = {};
    taskEntry->motionNode.scale = primary->scale;
    taskEntry->motionNode.siblingOffset = 0;
    entry.originalPrimaryChild = primary->runtimeChild;
    entry.originalPrimaryMesh = primary->xb01Offset;
    entry.originalPrimaryRuntimeMesh = primary->runtimeMesh;
    entry.motionNode.sourceOffset = primary->sourceOffset;
    entry.motionNode.id = taskEntry->motionNode.id;
    entry.motionNode.parent = static_cast<std::int32_t>(entry.primaryBinding.nodeIndex);
    entry.motionNode.scale = primary->scale;
    entry.motionNode.xb01Offset = primary->xb01Offset;
    entry.motionNode.meshType = primary->meshType;
    entry.motionNode.borrowedMesh = primary->borrowedMesh ? primary->borrowedMesh : primary;
    entry.motionNode.commandOverride = primary->commandOverride;
    entry.motionNode.runtimeMesh = primary->runtimeMesh;
    entry.motionNode.runtimeChild = primary->runtimeChild;
    entry.motionNode.runtimeParent = primary;
    entry.motionNode.visible = primary->visible;
    Matrix4x4_Push(&transformMatrix_prePass);
    if (MT7Node *const secondary = entry.secondaryBinding.node) {
        Matrix4x4::preTranslate(secondary->position.x, secondary->position.y, secondary->position.z);
        SetDoorViewSpaceEuler(secondary->rotation);
    }
    const Vector3 secondaryPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::preTranslate(primary->position.x, primary->position.y, primary->position.z);
    SetDoorViewSpaceEuler(primary->rotation);
    Matrix4x4::GetTransformMatrix(&taskEntry->worldTransform);
    Matrix4x4::Pop(1);
    entry.taskEntry->origin = {taskEntry->worldTransform.rows[3].x, taskEntry->worldTransform.rows[3].y, taskEntry->worldTransform.rows[3].z};
    entry.state.yawRadians = std::atan2(taskEntry->worldTransform.rows[0].z, taskEntry->worldTransform.rows[0].x);
    const float interactionOffset = entry.state.family >= 1 && entry.state.family <= 4 ? 0.45f : 0.0f;
    entry.state.interactionPoint = TransformPoint(taskEntry->worldTransform, {interactionOffset, 0.0f, 0.0f});
    if (DoorLocalPosition(entry, secondaryPosition).z >= 0.0f)
        taskEntry->flags |= 8U;

    AddAnimationTarget(entry, entry.primaryBinding);
    if (entry.state.family >= 5 && entry.state.family <= 7) {
        std::size_t directChildren = 0;
        std::int32_t skippedParent = -1;
        const std::int32_t primaryIndex = static_cast<std::int32_t>(entry.primaryBinding.nodeIndex);
        for (std::size_t index = entry.primaryBinding.nodeIndex + 1; index < entry.primaryBinding.model->nodes.size(); ++index) {
            MT7Node &node = entry.primaryBinding.model->nodes[index];
            if (node.parent < primaryIndex)
                break;

            if (node.parent == primaryIndex) {
                skippedParent = directChildren++ < 2 ? static_cast<std::int32_t>(index) : -1;
            } else if (skippedParent >= 0) {
                continue;
            }
            AddAnimationTarget(entry, {entry.primaryBinding.model, &node, entry.primaryBinding.slotIndex, index});
        }
    }
    return entry;
}

float DoorLocalZ(const DoorEntry &entry, const Vector3 &position) {
    const float dx = position.x - entry.taskEntry->origin.x;
    const float dz = position.z - entry.taskEntry->origin.z;
    return -dx * std::sin(entry.state.yawRadians) + dz * std::cos(entry.state.yawRadians);
}

// @verified - p
std::uint64_t SetDoorNodeHierarchyHidden(MT7Node *node, std::uint32_t hidden) {
    for (; node; node = node->runtimeSibling) {
        node->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | ((hidden & 0xffffU) << 16U));
        node->visible = (hidden & 0xffffU) == 0;
        if (node->runtimeChild)
            SetDoorNodeHierarchyHidden(node->runtimeChild, hidden);
    }
    return 0;
}

// @verified - p
void SetDoorEntrySortKey(float key, DoorTaskEntry *entry) {
    DoorTaskEntry *const previous = entry->previous;
    DoorTaskEntry *cursor = entry;
    DoorTaskEntry *previousCursor = entry;
    entry->sortKey = key;
    if (previous) {
        if (previous->sortKey <= key) {
            if (entry->next) {
                if (key <= entry->next->sortKey)
                    goto sorted;
                DoorTaskEntry *next = entry->next;
                do {
                    if (next->sortKey >= key)
                        break;
                    cursor = next;
                    next = next->next;
                } while (next);
            } else {
                do {
                    cursor = previousCursor->previous;
                    if (key >= cursor->sortKey)
                        break;
                    previousCursor = cursor;
                } while (cursor->previous);
            }
        } else {
            do {
                cursor = previousCursor->previous;
                if (key >= cursor->sortKey)
                    break;
                previousCursor = cursor;
            } while (cursor->previous);
        }
    } else {
        DoorTaskEntry *next = entry->next;
        if (!next)
            goto sorted;
        do {
            if (next->sortKey >= key)
                break;
            cursor = next;
            next = next->next;
        } while (next);
    }
    if (cursor != entry && cursor->next != entry) {
        if (previous)
            previous->next = entry->next;
        if (entry->next)
            entry->next->previous = entry->previous;
        entry->next = cursor->next;
        entry->previous = cursor;
        if (entry->next)
            entry->next->previous = entry;
        if (entry->previous)
            entry->previous->next = entry;
    }
sorted:
    if (!entry->previous)
        doorRuntime.sortedHead = entry;
    if (!entry->next)
        doorRuntime.sortedTail = entry;
}

void RefreshDoorDistances(const Vector3 &playerPosition) {
    for (DoorEntry &entry : doorRuntime.entries) {
        if ((entry.taskEntry->flags & 0x10U) == 0) {
            SetDoorEntrySortKey(1000.0f, entry.taskEntry);
            continue;
        }
        if (entry.taskEntry->flags & 4U)
            continue;
        const float dx = entry.taskEntry->origin.x - playerPosition.x;
        const float dy = entry.taskEntry->origin.y - playerPosition.y;
        const float dz = entry.taskEntry->origin.z - playerPosition.z;
        SetDoorEntrySortKey((dy * dy + dx * dx) + dz * dz, entry.taskEntry);
    }
    std::size_t index = 0;
    for (DoorTaskEntry *entry = doorRuntime.sortedHead; entry; entry = entry->next)
        doorRuntime.sortedEntries[index++] = static_cast<std::size_t>(entry - doorRuntime.taskEntries);
}

void RestoreDoorTargets() {
    for (DoorEntry &entry : doorRuntime.entries) {
        for (AnimatedDoorTarget &target : entry.animationTargets) {
            if (!target.binding.node)
                continue;
            target.binding.node->position = target.initialPosition;
            target.binding.node->rotation = target.initialRotation;
            target.binding.node->scale = target.initialScale;
        }
        if (MT7Node *const primary = entry.primaryBinding.node) {
            primary->xb01Offset = entry.originalPrimaryMesh;
            primary->runtimeMesh = entry.originalPrimaryRuntimeMesh;
            primary->runtimeChild = entry.originalPrimaryChild;
            if (entry.originalPrimaryChild)
                entry.originalPrimaryChild->runtimeParent = primary;
        }
    }
}


// @verified - p
std::uint16_t npcGetRemappedAnimId(std::uint32_t motion) {
    switch (motion) {
    case 61452:
        return 61641;
    case 61454:
        return 61615;
    case 61455:
        return 61616;
    case 61456:
        return 61642;
    case 61468:
        return 61625;
    case 61471:
        return 61626;
    case 61520:
        return 61631;
    case 61521:
        return 61611;
    case 61522:
        return 61612;
    case 61523:
        return 61632;
    case 61524:
        return 61621;
    case 61525:
        return 61622;
    case 61531:
        return 61633;
    case 61532:
        return 61634;
    case 61553:
        return 61635;
    case 61554:
        return 61613;
    case 61555:
        return 61614;
    case 61556:
        return 61636;
    case 61557:
        return 61623;
    case 61558:
        return 61624;
    case 61563:
        return 61637;
    case 61564:
        return 61638;
    case 61569:
        return 61639;
    case 61570:
        return 61640;
    case 61591:
        return 61643;
    case 61592:
        return 61617;
    case 61593:
        return 61618;
    case 61594:
        return 61644;
    case 61595:
        return 61627;
    case 61596:
        return 61628;
    case 61601:
        return 61681;
    case 61602:
        return 61682;
    case 61666:
        return 61645;
    case 61667:
        return 61619;
    case 61668:
        return 61620;
    case 61669:
        return 61646;
    case 61671:
        return 61629;
    case 61672:
        return 61630;
    default:
        return 61610;
    }
}



std::int32_t drEntryState{};
std::int32_t drEntryKind{};
std::int32_t drEntryWaitFrames{};
std::int32_t drEntryDialogue{};
std::uint64_t drEntryNpcLoadState{};
std::uint32_t drEntryMotion{};
std::int32_t drEntryCameraContext{};
ConversationBuffer *drEntryConversation{};


shen_name drAnswerNpc{};
Vector3 drAnswerPosition{};
Vector3i drAnswerRotation{};
std::uint32_t drAnswerMotion{};
float drAnswerFrame{};
std::uint32_t drAnswerDoorMotion{};
std::int32_t drAnswerSide{};
std::int32_t drAnswerDoorSounded{};


constexpr std::array<std::uint16_t, 6> DOOR_ANSWER_MOTIONS{0xf013, 0xf00d, 0xf011, 0xf012, 0xf01d, 0xf01e};

std::uint8_t RegionFamilyCode(shen_name region) {
    return static_cast<std::uint8_t>(region.name >> 8U);
}


DoorTaskEntry *drFindTaskEntry(shen_name region) {
    DoorEntry *const entry = FindDoorEntry(region.name);
    return entry ? entry->taskEntry : nullptr;
}


// @verified - p
float mtwkGetAnimLoopStartFrm(std::uint32_t motion) {
    switch (motion) {
    case 0xf00c:
    case 0xf00e:
        return 10.0f;
    case 0xf00f:
        return 6.0f;
    case 0xf010:
        return 16.0f;
    case 0xf01c:
    case 0xf01f:
        return 15.0f;
    default:
        return 0.0f;
    }
}


// @verified - p
std::uint32_t drSetAnswerMotion(std::uint32_t motion) {
    std::int32_t side = 0;
    float frame = 100.0f;
    std::uint32_t doorMotion = 61712;
    switch (motion) {
    case 61520:
        frame = 85.0f;
        doorMotion = 61696;
        side = 2;
        break;
    case 61521:
        frame = 61.0f;
        doorMotion = 61697;
        side = 3;
        break;
    case 61522:
        frame = 56.0f;
        doorMotion = 61698;
        side = 4;
        break;
    case 61523:
        frame = 89.0f;
        doorMotion = 61699;
        side = 1;
        break;
    case 61524:
        frame = 44.0f;
        doorMotion = 61700;
        side = 5;
        break;
    case 61525:
        frame = 46.0f;
        doorMotion = 61701;
        side = 6;
        break;
    case 61531:
    case 61563:
        frame = 66.0f;
        doorMotion = 61708;
        side = 2;
        break;
    case 61532:
    case 61564:
        frame = 60.0f;
        doorMotion = 61709;
        side = 1;
        break;
    case 61553:
        frame = 90.0f;
        doorMotion = 61702;
        side = 2;
        break;
    case 61554:
        frame = 90.0f;
        doorMotion = 61703;
        side = 3;
        break;
    case 61555:
        frame = 84.0f;
        doorMotion = 61704;
        side = 4;
        break;
    case 61556:
        frame = 125.0f;
        doorMotion = 61705;
        side = 1;
        break;
    case 61557:
        frame = 72.0f;
        doorMotion = 61706;
        side = 5;
        break;
    case 61558:
        frame = 70.0f;
        doorMotion = 61707;
        side = 6;
        break;
    case 61569:
        frame = 80.0f;
        doorMotion = 61710;
        side = 2;
        break;
    case 61570:
        frame = 95.0f;
        doorMotion = 61711;
        side = 1;
        break;
    case 61591:
        side = 2;
        break;
    case 61592:
        frame = 58.0f;
        doorMotion = 61713;
        side = 3;
        break;
    case 61593:
        frame = 50.0f;
        doorMotion = 61714;
        side = 4;
        break;
    case 61594:
        frame = 136.0f;
        doorMotion = 61715;
        side = 1;
        break;
    case 61595:
        frame = 42.0f;
        doorMotion = 61716;
        side = 5;
        break;
    case 61596:
        frame = 52.0f;
        doorMotion = 61717;
        side = 6;
        break;
    case 61601:
        frame = 93.0f;
        doorMotion = 61718;
        side = 2;
        break;
    case 61602:
        frame = 86.0f;
        doorMotion = 61719;
        side = 1;
        break;
    case 61666:
        frame = 93.0f;
        doorMotion = 61720;
        side = 2;
        break;
    case 61667:
        frame = 51.0f;
        doorMotion = 61721;
        side = 3;
        break;
    case 61668:
        frame = 52.0f;
        doorMotion = 61722;
        side = 4;
        break;
    case 61669:
        frame = 79.0f;
        doorMotion = 61723;
        side = 1;
        break;
    case 61671:
        frame = 51.0f;
        doorMotion = 61724;
        side = 5;
        break;
    case 61672:
        frame = 53.0f;
        doorMotion = 61725;
        side = 6;
        break;
    default:
        break;
    }
    drAnswerFrame = frame;
    drAnswerDoorMotion = doorMotion;
    drAnswerSide = side;
    return doorMotion;
}




std::int32_t DoorMdpSoundWord(const LoadedMapNodeBinding &binding, std::size_t offset) {
    if (!binding.model || !binding.node)
        return 0;
    const std::span<const std::byte> source = binding.model->source;
    const std::size_t record = binding.node->sourceOffset;
    if (record + 0x54 > source.size())
        return 0;
    std::uint32_t type{};
    std::int32_t size{};
    std::int32_t word{};
    std::memcpy(&type, source.data() + record + 0x40, sizeof(type));
    std::memcpy(&size, source.data() + record + 0x44, sizeof(size));
    std::memcpy(&word, source.data() + record + offset, sizeof(word));
    return (type == SHEN_NAME(MDP7) || type == SHEN_NAME(MDPX)) && size > 12 ? word : 0;
}

// @verified - p
void drUpdateDoorMotionSound(DoorEntry &entry) {
    DoorTaskEntry *const task = entry.taskEntry;
    if (task->flags & 0x20U)
        return;
    float begin;
    float end;
    if ((task->motionChannelState & 0x20U) != 0) {
        begin = task->motionSpeed + task->motionFrame;
        end = task->motionSpeed + begin;
    } else {
        end = task->motionFrame - task->motionSpeed;
        begin = end - task->motionSpeed;
    }
    std::int32_t volume;
    std::uint32_t cue;
    float context;
    if (const std::span<const std::byte> command = FindLoadedMotionTimedCommand(task->animationId, 4, begin, end); !command.empty()) {
        volume = static_cast<std::int8_t>(command[3]);
        std::memcpy(&cue, command.data() + 4, sizeof(cue));
        context = static_cast<float>(std::to_integer<std::uint8_t>(command[0]));
    } else {
        const std::span<const std::byte> variants = FindLoadedMotionTimedCommand(task->animationId, 49, begin, end);
        if (variants.empty())
            return;
        volume = static_cast<std::int8_t>(variants[3]);
        context = IncrementSharedPrngFloat() * 3.0f;
        const std::int32_t pick = static_cast<std::int32_t>(context);
        std::memcpy(&cue, variants.data() + 4 + 4 * pick, sizeof(cue));
    }
    if (!cue)
        return;
    std::uint32_t material = static_cast<std::uint32_t>(DoorMdpSoundWord(entry.primaryBinding, 0x4c));
    if ((material & 0xf0U) == 0) {
        const std::uint8_t family = RegionFamilyCode(*task->sourcePair);
        const std::uint32_t digit = family >= '1' && family <= '7' ? family - '0' : 0;
        material |= digit << 4U;
    }
    const std::uint32_t environment = static_cast<std::uint32_t>(DoorMdpSoundWord(entry.primaryBinding, 0x50));
    const std::uint32_t shape = ((environment & 0xfU) | ((static_cast<std::uint32_t>(volume) & 0x7fU) << 5U)) << 19U;
    const std::uint32_t flags = material | (cue & 0x87ffffU) | shape;
    if (volume >= 0 || task->claimant == kDoorPlayerName) {

        (void)Loop3DSoundEffect(flags, context);
    } else {
        const Vector3 position{task->worldTransform.rows[3].x, task->worldTransform.rows[3].y, task->worldTransform.rows[3].z};
        (void)Execute3DSoundCommand(flags, position, 2.0f, 30.0f, position.z);
    }
}



void drPlayDoorSound(DoorEntry &entry) {
    DoorTaskEntry *const task = entry.taskEntry;
    if (task->flags & 0x20U)
        return;
    const float end = task->motionEndFrame;
    std::int32_t volume = 0;
    std::uint32_t cue = 0;
    if (const std::span<const std::byte> command = FindLoadedMotionTimedCommand(task->animationId, 4, 0.0f, end); !command.empty()) {
        volume = static_cast<std::int8_t>(command[3]);
        std::memcpy(&cue, command.data() + 4, sizeof(cue));
    } else {
        const std::span<const std::byte> variants = FindLoadedMotionTimedCommand(task->animationId, '1', 0.0f, end);
        if (variants.empty())
            return;
        volume = static_cast<std::int8_t>(variants[3]);
        const auto pick = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 3.0f);
        std::memcpy(&cue, variants.data() + 4 + 4 * pick, sizeof(cue));
    }
    if (!cue)
        return;
    std::uint32_t material = static_cast<std::uint32_t>(DoorMdpSoundWord(entry.primaryBinding, 0x4c));
    if ((material & 0xf0U) == 0) {
        const std::uint8_t family = RegionFamilyCode(*task->sourcePair);
        const std::uint32_t digit = family >= '1' && family <= '7' ? family - '0' : 0;
        material |= digit << 4U;
    }
    const std::uint32_t environment = static_cast<std::uint32_t>(DoorMdpSoundWord(entry.primaryBinding, 0x50));
    const std::uint32_t shape = ((environment & 0xfU) | ((static_cast<std::uint32_t>(volume) & 0x7fU) << 5U)) << 19U;
    (void)Loop3DSoundEffect(material | (cue & 0x87feffU) | shape | 0x100U);
}



std::int64_t drRunEntryScripts(std::int32_t bits) {
    if (bits == 0)
        return 0;
    for (std::int32_t bit = 0; bit < 5; ++bit) {
        if ((bits & (1 << bit)) && shenSetPlayerDataFields(static_cast<std::int32_t>(doorSearchRegion.name), static_cast<std::int8_t>(bit + 1))) {
            ResetDoorInteraction();
            return 1;
        }
    }
    return 0;
}



std::int32_t drGetEntryKind(shen_name region) {
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(region);
    if (!metadata)
        return 0;
    const std::int32_t hours = GetDoorOpeningState(region);
    if (hours == 0 || hours == 3) {
        std::int32_t count = 0;
        for (const std::uint8_t dialogue : metadata->dialogues)
            if (dialogue)
                ++count;
        std::int32_t pick = 0;
        if (count)
            pick = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 1000.0f) % count;
        drEntryDialogue = metadata->dialogues[static_cast<std::size_t>(pick)];
        return metadata->answer <= 3 ? 16 + metadata->answer : 0;
    }
    for (const DoorFlagRule *rule = metadata->rules; rule && rule->flag; ++rule) {
        if (!EV_GetShenmueFlag(rule->flag))
            continue;
        drEntryDialogue = rule->dialogue;
        if (rule->answer <= 3)
            return 16 + rule->answer;
    }
    return metadata->knock == 1 ? 1 : 0;
}



std::int64_t manage_alpha(std::int32_t nextState, std::int32_t scriptBits) {
    using enum CameraState;
    Character *const ryo = ctrlGetRootNode();
    if (drEntryState != 0) {
        if (drEntryState == 1 && plymGetSpecialFlag() == 1) {
            const float remaining = 1.0f - charaGetAlphaDraw(ryo);
            if ((remaining >= 0.0f ? remaining : -remaining) <= 0.00000011920929f) {
                shenToggleAreaFlags(0);
                toggleCCMflagsByAmt(ryo, CharacterControlStairMode);
                ccmSetFlagsFor(ryo, CharacterControlFineRaycast);
                drEntryState = nextState;
                return drRunEntryScripts(scriptBits);
            }
        }
        return 0;
    }
    (void)SetPlayerControlEnabled(0);
    drEntryCameraContext = camCtrlGetContextVal();
    camCtrlSetContext(0, 0);
    shenToggleAreaFlags(1);
    camCtrlSetState(Init);
    ++drEntryState;
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(doorSearchRegion);
    const std::int32_t side = metadata ? metadata->flags & 3 : 0;
    DoorEntry *const entry = FindDoorEntry(doorSearchRegion.name);
    if (!entry)
        return 1;
    Vector3 location{};
    plymUpdatePlayerSearchLocation(&location);
    location = DoorLocalPosition(*entry, location);
    if (side != 2) {
        if (location.z < 0.0f && side != 0)
            drEntryKind = 2;
        if (location.z > 0.0f && side == 0)
            drEntryKind = 2;
    }
    return 0;
}




std::int64_t drWalkThroughDoor(std::int32_t nextState, std::int32_t scriptBits) {
    using enum CameraState;
    Character *const ryo = ctrlGetRootNode();
    if (drEntryState == 16) {
        drEntryNpcLoadState = GetNpcNewLoadState();
        EV_NPC_NewLoadSw(0);
        if (DoorTaskEntry *const task = drFindTaskEntry(doorSearchRegion))
            (void)npcReleaseStreamsNearDoor(&task->origin, 1.5f);
        Vector3 location{};
        Vector3 target{};
        plymUpdatePlayerSearchLocation(&location);
        (void)ComputeDoorIngress(static_cast<std::int32_t>(doorSearchRegion.name), &location, &target);
        drEntryMotion = GetNpcDoorAnimation(static_cast<std::int32_t>(doorSearchRegion.name), ryo, &location);
        Matrix4x4_PushActiveTransform();
        if (DoorTaskEntry *const task = drFindTaskEntry(doorSearchRegion))
            Matrix4x4::SetActiveTransform(&task->worldTransform);
        const Vector3 doorPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        if (RegionFamilyCode(doorSearchRegion) == '1') {
            if (drEntryMotion == 0xf010)
                Matrix4x4::preTranslate(0.47499999f, 0.0f, -0.5f);
            else
                Matrix4x4::preTranslate(0.55000001f, 0.0f, 0.5f);
            target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        }
        Matrix4x4::Pop(1);
        SelectMagicRoomAtPosition(&doorPosition);
        const auto yaw = static_cast<std::int16_t>(g_doorIngressRotation.y);
        (void)camCtrlSwitchToEventCam(30, &target, yaw);
        (void)RequestMotionPositionTransition(
            FindMotionWork(ryo), &target, static_cast<std::uint16_t>(yaw), drEntryMotion, mtwkGetAnimLoopStartFrm(drEntryMotion));
        drEntryWaitFrames = 0;
        ResetScenarioFreeTalkResumeFlags();
        ++drEntryState;
    } else if (drEntryState == 18) {
        const auto frames = static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(drEntryMotion)));
        const float frame = GetMotionFrame(FindMotionWork(ryo), MotionRegion::LowerBody);
        if ((mtwkGetAnimLoopStartFrm(drEntryMotion) + frames) * 0.5f <= frame) {
            DoorTaskEntry *const task = drFindTaskEntry(doorSearchRegion);
            const auto count = static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(drEntryMotion)));
            task->motionScale = task->motionScale - 2.0f / (count - mtwkGetAnimLoopStartFrm(drEntryMotion));
            if (SetMotionRegionFlag(FindMotionWork(ryo), 65, 1, MotionRegion::LowerBody) != 0) {
                drFindTaskEntry(doorSearchRegion)->motionScale = 1.0f;
                toggleCCMflagsByAmt(ryo, CharacterControlFineRaycast);
                ccmSetFlagsFor(ryo, CharacterControlStairMode);
                (void)SetPlayerControlEnabled(1);
                doorSearchRegion = {};
                drEntryState = nextState;
                camCtrlSetState(doorRuntime.previousCameraState);
                if (scriptBits == 32)
                    LeaveMagicRoom();
                EV_NPC_NewLoadSw(static_cast<std::uint32_t>(drEntryNpcLoadState));
                return drRunEntryScripts(scriptBits);
            }
        }
        camCtrlApplyNextAndTargetPos();
        return 0;
    } else if (drEntryState == 19) {
        ccmSetFlagsFor(ryo, CharacterControlEvaluateMotion);
        charaToggleFlag(ryo, 0);
        drEntryState = nextState;
        doorSearchRegion = {};
        return 1;
    } else if (drEntryState != 17) {
        return 0;
    }
    if (DoorTaskEntry *const task = drFindTaskEntry(doorSearchRegion))
        (void)npcReleaseStreamsNearDoor(&task->origin, 1.5f);
    if (mtwkSetExtraFlag(FindMotionWork(ryo), 0x3a, 1) != 0) {
        if (drEntryWaitFrames++ > 120) {
            if (drEntryKind != 2)
                LeaveMagicRoom();
            camCtrlSetState(Init);
            ResetDoorInteraction();
            EV_NPC_NewLoadSw(1);
            (void)SetPlayerControlEnabled(1);
            return 0;
        }
    } else {
        const float startFrame = mtwkGetAnimLoopStartFrm(drEntryMotion) + 1.0f;
        (void)BeginDoorAnimation(
            static_cast<std::int32_t>(doorSearchRegion.name), GetCharacterScenarioName(ryo), static_cast<std::int32_t>(drEntryMotion), startFrame, 1.0f);
        EV_NPC_NewLoadSw(static_cast<std::uint32_t>(drEntryNpcLoadState));
        if (drRunEntryScripts(scriptBits) != 0)
            return 1;
        EV_NPC_NewLoadSw(0);
        if (doorTriggerTransition(doorSearchRegion, GetCharacterScenarioName(ryo)) != 0) {
            camCtrlSetState(doorRuntime.previousCameraState);
            drEntryState += 2;
        } else {
            camCtrlSetState(doorRuntime.previousCameraState);
            Vector3 location{};
            Vector3 target{};
            plymUpdatePlayerSearchLocation(&location);
            Matrix4x4_PushActiveTransform();
            Matrix4x4::preTranslate(location.x, location.y, location.z);
            SetDoorViewSpaceEuler({0, playerGetRotY(), 0});
            const auto motion = static_cast<std::uint16_t>(drEntryMotion);
            (void)SampleNativeMotionRoot(motion, GetMotionRigBoneId(FindMotionWork(ryo)), -1.0f, location);
            (void)SampleNativeMotionRoot(motion, GetMotionRigBoneId(FindMotionWork(ryo)), mtwkGetAnimLoopStartFrm(drEntryMotion) + 1.0f, target);
            Matrix4x4::preTranslate(location.x - target.x, 0.0f, location.z - target.z);
            target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4::Pop(1);
            const auto yaw = static_cast<std::int16_t>(playerGetRotY());
            const auto remaining = static_cast<float>(GetMotionFrameCount(motion)) - mtwkGetAnimLoopStartFrm(drEntryMotion);
            (void)camCtrlSwitchToEventCam(static_cast<std::int32_t>(remaining), &target, yaw);
            ++drEntryState;
        }
    }
    camCtrlApplyNextAndTargetPos();
    return 0;
}




std::int64_t drTryDoor(std::int32_t nextState, std::int32_t scriptBits) {
    using enum CameraState;
    Character *const ryo = ctrlGetRootNode();
    switch (drEntryState) {
    case 32: {
        Vector3 location{};
        plymUpdatePlayerSearchLocation(&location);
        DoorEntry *const entry = FindDoorEntry(doorSearchRegion.name);
        if (!entry)
            return 1;
        const Matrix4x4 &transform = entry->taskEntry->worldTransform;
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&transform);
        location = DoorLocalPosition(*entry, location);
        doorRuntime.ingressDoorId = static_cast<std::int32_t>(doorSearchRegion.name);
        g_doorIngressRotation = ExtractDoorRotation(transform);
        doorRuntime.ingressSide = 0;
        if (location.z >= 0.0f) {
            doorRuntime.ingressLeafSide = (entry->taskEntry->flags & 8U) == 0;
            Matrix4x4::preTranslate(0.5f, 0.0f, 0.75f);
        } else {
            doorRuntime.ingressLeafSide = (entry->taskEntry->flags & 8U) != 0;
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.75f);
            g_doorIngressRotation.y += 0x8000;
        }
        location = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        drEntryMotion = 0xf00a;
        const auto yaw = static_cast<std::int16_t>(g_doorIngressRotation.y);
        (void)RequestMotionPositionTransition(FindMotionWork(ryo), &location, static_cast<std::uint16_t>(yaw), 0xf00a, 11.0f);
        drEntryWaitFrames = 0;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(location.x, location.y, location.z);
        SetDoorViewSpaceEuler({0, yaw, 0});
        (void)SampleNativeMotionRoot(static_cast<std::uint16_t>(drEntryMotion), GetMotionRigBoneId(FindMotionWork(ryo)), -1.0f, location);
        Matrix4x4::preTranslate(location.x, 0.0f, location.z);
        location = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        (void)camCtrlSwitchToEventCam(30, &location, yaw);
        ++drEntryState;
        break;
    }
    case 33:
        if (mtwkSetExtraFlag(FindMotionWork(ryo), 0x3a, 1) == 0) {
            if (nextState == -1 || nextState == 80) {
                (void)doorPlayRigidMotionRange(doorSearchRegion, 0xf0fc, 12.0f, 1.0f, 45.0f);
                ++drEntryState;
            } else {
                (void)doorPlayRigidMotionRange(doorSearchRegion, 0xf0fc, 12.0f, 1.0f, -1.0f);
                drEntryState += 2;
            }
            if (SetMotionRegionFlag(FindMotionWork(ryo), 65, 1, MotionRegion::LowerBody) != 0) {
                drEntryState = nextState;
                camCtrlSetState(doorRuntime.previousCameraState);
                return drRunEntryScripts(scriptBits);
            }
            break;
        }
        if (drEntryWaitFrames++ > 120) {
            camCtrlSetState(Init);
            ResetDoorInteraction();
            EV_NPC_NewLoadSw(1);
            (void)SetPlayerControlEnabled(1);
            return 0;
        }
        break;
    case 34:
        if (SetMotionRegionFlag(FindMotionWork(ryo), 65, 1, MotionRegion::LowerBody) != 0) {
            drEntryState = nextState;
            camCtrlSetState(doorRuntime.previousCameraState);
            return drRunEntryScripts(scriptBits);
        }
        break;
    case 35:
        if (GetMotionFrame(FindMotionWork(ryo), MotionRegion::LowerBody) >= 46.0f) {
            drEntryState = nextState;
            camCtrlSetState(doorRuntime.previousCameraState);
            return drRunEntryScripts(scriptBits);
        }
        break;
    default:
        return 0;
    }
    camCtrlApplyNextAndTargetPos();
    return 0;
}



std::int64_t drKnockOnDoor(std::int32_t nextState, std::int32_t variant, std::int32_t scriptBits) {
    using enum CameraState;
    Character *const ryo = ctrlGetRootNode();
    if (drEntryState == 48) {
        drEntryNpcLoadState = GetNpcNewLoadState();
        EV_NPC_NewLoadSw(0);
        if (DoorTaskEntry *const task = drFindTaskEntry(doorSearchRegion))
            (void)npcReleaseStreamsNearDoor(&task->origin, 1.5f);
        Vector3 location{};
        plymUpdatePlayerSearchLocation(&location);
        DoorEntry *const entry = FindDoorEntry(doorSearchRegion.name);
        if (!entry)
            return 1;
        const Matrix4x4 &transform = entry->taskEntry->worldTransform;
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&transform);
        location = DoorLocalPosition(*entry, location);
        doorRuntime.ingressDoorId = static_cast<std::int32_t>(doorSearchRegion.name);
        g_doorIngressRotation = ExtractDoorRotation(transform);
        doorRuntime.ingressSide = 0;
        const std::int32_t pair = 2 * (RegionFamilyCode(doorSearchRegion) - '1');
        if (location.z >= 0.0f) {
            drEntryMotion = pair + 1 >= 0 && pair + 1 < 6 ? DOOR_ANSWER_MOTIONS[static_cast<std::size_t>(pair + 1)] : 0;
            doorRuntime.ingressLeafSide = (entry->taskEntry->flags & 8U) == 0;
            Matrix4x4::preTranslate(0.5f, 0.0f, 0.75f);
        } else {
            drEntryMotion = pair >= 0 && pair < 6 ? DOOR_ANSWER_MOTIONS[static_cast<std::size_t>(pair)] : 0;
            doorRuntime.ingressLeafSide = (entry->taskEntry->flags & 8U) != 0;
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.75f);
            g_doorIngressRotation.y += 0x8000;
        }
        location = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        const auto yaw = static_cast<std::int16_t>(g_doorIngressRotation.y);
        (void)RequestMotionPositionTransition(FindMotionWork(ryo), &location, static_cast<std::uint16_t>(yaw), drEntryMotion, variant ? 17.0f : 75.0f);
        drEntryWaitFrames = 0;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(location.x, location.y, location.z);
        SetDoorViewSpaceEuler({0, yaw, 0});
        (void)SampleNativeMotionRoot(static_cast<std::uint16_t>(drEntryMotion), GetMotionRigBoneId(FindMotionWork(ryo)), -1.0f, location);
        Matrix4x4::preTranslate(location.x, 0.0f, location.z);
        location = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        (void)camCtrlSwitchToEventCam(30, &location, yaw);
        ++drEntryState;
    } else if (drEntryState != 49) {
        return 0;
    }
    if (DoorTaskEntry *const task = drFindTaskEntry(doorSearchRegion))
        (void)npcReleaseStreamsNearDoor(&task->origin, 1.5f);
    if (mtwkSetExtraFlag(FindMotionWork(ryo), 0x3a, 1) != 0) {
        if (drEntryWaitFrames++ > 120) {
            camCtrlSetState(Init);
            ResetDoorInteraction();
            EV_NPC_NewLoadSw(1);
            (void)SetPlayerControlEnabled(1);
            return 0;
        }
        camCtrlApplyNextAndTargetPos();
        return 0;
    }
    std::uint16_t leafMotion = 0;
    switch (drEntryMotion) {
    case 0xf00d:
        leafMotion = 0xf0fd;
        break;
    case 0xf011:
        leafMotion = 0xf0f8;
        break;
    case 0xf012:
        leafMotion = 0xf0f9;
        break;
    case 0xf013:
        leafMotion = 0xf0fe;
        break;
    case 0xf01d:
        leafMotion = 0xf0fa;
        break;
    case 0xf01e:
        leafMotion = 0xf0fb;
        break;
    default:
        break;
    }
    (void)doorPlayRigidMotion(doorSearchRegion, leafMotion, variant ? 18.0f : 76.0f, 1.0f);
    if (SetMotionRegionFlag(FindMotionWork(ryo), 65, 1, MotionRegion::LowerBody) == 0) {
        camCtrlApplyNextAndTargetPos();
        return 0;
    }
    EV_NPC_NewLoadSw(static_cast<std::uint32_t>(drEntryNpcLoadState));
    drEntryState = nextState;
    camCtrlSetState(doorRuntime.previousCameraState);
    return drRunEntryScripts(scriptBits);
}


std::int64_t drPlayAnswerDialogue(std::int32_t nextState) {
    switch (drEntryState) {
    case 64: {
        char code[16];
        std::snprintf(code, sizeof(code), "100%1dA%03d", g_actNumber, drEntryDialogue);
        drEntryConversation = EV_PlayDialogue(code);
        ++drEntryState;
        return 0;
    }
    case 65:
        if (cvsHasValidConverseData(drEntryConversation))
            return 0;
        ++drEntryState;
        return 0;
    case 66:
        drEntryState = nextState;
        return 1;
    default:
        return 0;
    }
}


std::int64_t drFinishEntry(std::int32_t nextState) {
    if (drEntryState == -2) {
        camCtrlSetContext(drEntryCameraContext != 0, 0);
        doorSearchRegion = {};
        drEntryState = nextState;
        return 1;
    }
    if (drEntryState == -1) {
        Character *const ryo = ctrlGetRootNode();
        toggleCCMflagsByAmt(ryo, CharacterControlFineRaycast);
        ccmSetFlagsFor(ryo, CharacterControlStairMode);
        (void)SetPlayerControlEnabled(1);
        (void)ReleaseDoor(static_cast<std::int32_t>(doorSearchRegion.name), GetCharacterScenarioName(ryo));
        --drEntryState;
    }
    return 0;
}



std::int64_t drOccupantAnswers(std::int32_t nextState) {
    Character *const ryo = ctrlGetRootNode();
    switch (drEntryState) {
    case 80: {
        DoorEntry *const entry = FindDoorEntry(doorSearchRegion.name);
        const DoorAreaMetadata *const metadata = entry ? GetDataForCurrentMapAndScene(doorSearchRegion) : nullptr;
        if (metadata && (metadata->flags & 4U))
            drAnswerNpc = npcFindAreaPointOccupant(doorSearchRegion);
        if (!metadata || !(metadata->flags & 4U) || drAnswerNpc.name == 0) {
            drEntryState = nextState;
            return 1;
        }
        drEntryDialogue = 0;
        Vector3 location{};
        plymUpdatePlayerSearchLocation(&location);
        const Matrix4x4 &transform = entry->taskEntry->worldTransform;
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&transform);
        const Vector3 doorPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        location = DoorLocalPosition(*entry, location);
        doorRuntime.ingressDoorId = static_cast<std::int32_t>(doorSearchRegion.name);
        g_doorIngressRotation = ExtractDoorRotation(transform);
        const std::uint8_t family = RegionFamilyCode(doorSearchRegion);
        if ((family == '1' && location.z > 0.0f) || family == '2') {
            if (!(location.z < 0.0f)) {
                Matrix4x4::preTranslate(0.80000001f, 0.0f, 1.0f);
            } else {
                Matrix4x4::preTranslate(0.80000001f, 0.0f, -1.0f);
                g_doorIngressRotation.y += 0x8000;
            }
            location = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            (void)RequestMotionPositionTransition(FindMotionWork(ryo), &location, static_cast<std::uint16_t>(g_doorIngressRotation.y), 61442, 0.0f);
        } else {
            (void)mtwkRequestMotion(FindMotionWork(ryo), 0xf002, MotionRegion::LowerBody);
        }
        Matrix4x4::Pop(1);
        SelectMagicRoomAtPosition(&doorPosition);
        (void)EV_CheckWarmStartNpcRoute(drAnswerNpc);
        (void)npcSetDoorAnswerMode(drAnswerNpc, 1);
        if (EV_ForceLoadNpc(drAnswerNpc) == 0) {
            EV_NPC_NewLoadSw(0);
            (void)EV_ResetFreeTalkFlags();
        }
        plymUpdatePlayerSearchLocation(&location);
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&transform);
        location = DoorLocalPosition(*entry, location);
        drAnswerRotation = ExtractDoorRotation(transform);
        if (!(location.z < 0.0f)) {
            Matrix4x4::preTranslate(0.80000001f, 0.0f, -0.85000002f);
            drAnswerRotation.y += 0x8000;
        } else {
            Matrix4x4::preTranslate(0.80000001f, 0.0f, 0.85000002f);
        }
        location = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        (void)ComputeDoorIngress(static_cast<std::int32_t>(doorSearchRegion.name), &location, &drAnswerPosition);
        ++drEntryState;
        return 0;
    }
    case 81: {
        if (EV_ForceLoadNpc(drAnswerNpc) == 0)
            return 0;
        Character *const npc = FindCharacterByScenarioName(drAnswerNpc);
        MotionWork *const work = FindMotionWork(npc);
        toggleCCMflagsByAmt(npc, CharacterControlStairMode);
        ccmSetFlagsFor(npc, CharacterControlFineRaycast);
        ccmSetFlagsFor(npc, CharacterControlEvaluateMotion);
        (void)SetMotionRegionFlag(work, 71, 0, MotionRegion::LowerBody);
        shenApplyTranslationToAll(&npc->entity.node, drAnswerPosition);
        SetCharacterWorldRotation(npc, drAnswerRotation);
        drAnswerMotion = GetNpcDoorAnimation(static_cast<std::int32_t>(doorSearchRegion.name), npc, &drAnswerPosition);
        (void)mtwkRequestMotion(work, static_cast<std::uint16_t>(drAnswerMotion), MotionRegion::LowerBody);
        (void)drSetAnswerMotion(drAnswerMotion);
        SetMotionSpeed(work, 0.89999998f, MotionRegion::LowerBody);
        SetMotionLayerEndFrame(work, drAnswerFrame, MotionRegion::LowerBody);
        (void)doorLoopRigidMotion(doorSearchRegion, npcGetRemappedAnimId(drAnswerMotion), 0.0f, 0.89999998f);
        ++drEntryState;
        return 0;
    }
    case 82:
        ++drEntryState;
        [[fallthrough]];
    case 83: {
        Character *const npc = FindCharacterByScenarioName(drAnswerNpc);
        MotionWork *const work = FindMotionWork(npc);
        if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody) == 0)
            return 0;
        (void)StartScenarioFreeTalkRecord(drAnswerNpc, -1);
        Vector3 position{};
        (void)GetMotionBonePosition(work, MB_Head, &position);
        (void)UpdateMotionWorkWithPosition(FindMotionWork(ryo), &position, 0);
        (void)GetMotionBonePosition(FindMotionWork(ryo), MB_Head, &position);
        (void)UpdateMotionWorkWithPosition(work, &position, 0);
        (void)mtwkRequestMotion(work, static_cast<std::uint16_t>(drAnswerDoorMotion), MotionRegion::LowerBody);
        (void)doorLoopRigidMotion(doorSearchRegion, npcGetRemappedAnimId(drAnswerMotion), drAnswerFrame, 0.0f);
        ++drEntryState;
        return 0;
    }
    case 84:
        if (!IsScenarioBusy()) {
            (void)StopScenarioFreeTalkRecord();
            MotionWork *const work = FindMotionWork(FindCharacterByScenarioName(drAnswerNpc));
            (void)RequestMotionPositionTransition(work, &drAnswerPosition, static_cast<std::uint16_t>(drAnswerRotation.y), drAnswerMotion, drAnswerFrame);
            (void)UpdateMotionWorkWithPosition(work, nullptr, 0);
            ++drEntryState;
        }
        return 0;
    case 85: {
        MotionWork *const work = FindMotionWork(FindCharacterByScenarioName(drAnswerNpc));
        if (mtwkSetExtraFlag(work, 0x3a, 1) != 0)
            return 0;
        SetMotionSpeed(work, 0.0f, MotionRegion::LowerBody);
        drAnswerDoorSounded = 0;
        (void)doorLoopRigidMotion(doorSearchRegion, npcGetRemappedAnimId(drAnswerMotion), drAnswerFrame - 1.0f, -1.0f);
        ++drEntryState;
        [[fallthrough]];
    }
    case 86: {
        Character *const npc = FindCharacterByScenarioName(drAnswerNpc);
        MotionWork *const work = FindMotionWork(npc);
        const float frame = GetMotionFrame(work, MotionRegion::LowerBody) - 1.0f;
        if (drAnswerDoorSounded == 0) {
            DoorEntry *const entry = FindDoorEntry(doorSearchRegion.name);
            const auto yaw = static_cast<std::int16_t>(entry->taskEntry->motionNode.rotation.y);
            if ((yaw < 0 ? -yaw : yaw) < 64) {
                drPlayDoorSound(*entry);
                drAnswerDoorSounded = 1;
            }
        }
        if (frame < 1.0f) {
            ++drEntryState;
            return 0;
        }
        Vector3 position{};
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(drAnswerPosition.x, drAnswerPosition.y, drAnswerPosition.z);
        SetDoorViewSpaceEuler(drAnswerRotation);
        (void)SampleNativeMotionRoot(static_cast<std::uint16_t>(drAnswerMotion), GetMotionRigBoneId(work), frame - 1.0f, position);
        Matrix4x4::preTranslate(position.x, 0.0f, position.z);
        position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        (void)SetMotionRegionFlag(work, 71, 0, MotionRegion::LowerBody);
        shenApplyTranslationToAll(&npc->entity.node, position);
        (void)mtwkRequestMotion(work, static_cast<std::uint16_t>(drAnswerMotion), MotionRegion::LowerBody);
        SetMotionRegionEvalTime(work, frame, MotionRegion::LowerBody);
        return 0;
    }
    case 87:
        if ((drFindTaskEntry(doorSearchRegion)->flags & 3U) == 0) {
            LeaveMagicRoom();
            Character *const npc = FindCharacterByScenarioName(drAnswerNpc);
            ccmSetFlagsFor(npc, CharacterControlStairMode);
            toggleCCMflagsByAmt(npc, CharacterControlFineRaycast);
            toggleCCMflagsByAmt(npc, CharacterControlEvaluateMotion);
            (void)npcSetDoorAnswerMode(drAnswerNpc, 0);
            EV_NPC_NewLoadSw(1);
            (void)EV_npcFree(drAnswerNpc);
            (void)EV_npcLockUnlock(drAnswerNpc);
            drEntryState = nextState;
            drAnswerDoorMotion = 0;
            drAnswerFrame = 0.0f;
            drAnswerSide = 0;
            drAnswerNpc = {};
            drAnswerMotion = 0;
        }
        return 0;
    default:
        return 0;
    }
}


void drUpdateEntry() {
    CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
    std::int64_t result = 0;
    switch (drEntryKind) {
    case 0:
        if (manage_alpha(16, 7) == 0)
            (void)drWalkThroughDoor(0, 8);
        return;
    case 1:
        if (manage_alpha(32, 1) == 0 && drTryDoor(16, 6) == 0)
            (void)drWalkThroughDoor(0, 8);
        return;
    case 2:
        if (manage_alpha(16, 16) == 0)
            (void)drWalkThroughDoor(0, 32);
        return;
    case 16:
        if (manage_alpha(48, 1) != 0)
            return;
        result = drKnockOnDoor(drEntryDialogue ? 64 : -1, 1, 6);
        break;
    case 17:
        if (manage_alpha(32, 1) != 0 || drTryDoor(80, 6) != 0)
            return;
        result = drOccupantAnswers(drEntryDialogue ? 64 : -1);
        break;
    case 18:
        if (manage_alpha(48, 1) != 0)
            return;
        result = drKnockOnDoor(drEntryDialogue ? 64 : -1, 0, 6);
        break;
    case 19:
        result = manage_alpha(64, 0);
        break;
    default:
        return;
    }
    if (result != 0)
        return;
    (void)drPlayAnswerDialogue(-1);
    (void)drFinishEntry(0);
}

struct AutomaticDoorProximity {
    DoorEntry *entry;
    std::uint8_t sideFlags;
    bool openingAllowed;
    bool characterNear;
};

void CheckAutomaticDoorProximity(Character *character, void *context) {
    AutomaticDoorProximity &proximity = *static_cast<AutomaticDoorProximity *>(context);
    DoorEntry &entry = *proximity.entry;
    const Vector3 position = GetCharacterWorldPosition(character);
    const float dx = position.x - entry.state.interactionPoint.x;
    const float dy = position.y - entry.state.interactionPoint.y;
    const float dz = position.z - entry.state.interactionPoint.z;
    if (dy * dy + dx * dx + dz * dz < kAutomaticDoorOpenRadiusSquared) {
        if (character == g_playerRuntime.character && !proximity.openingAllowed) {
            const float localZ = DoorLocalPosition(entry, position).z;
            if (proximity.sideFlags ? localZ > 0.0f : !(localZ >= 0.0f))
                return;
        }
        proximity.characterNear = true;
    }
}

void UpdateAutomaticDoor(DoorEntry &entry) {
    if (entry.state.family != 7 || !entry.state.primaryResolved || entry.animationTargets.size() < 3) {
        return;
    }
    if (GetLeafState(entry) == DoorLeafState::ToB || GetLeafState(entry) == DoorLeafState::ToA)
        (void)AdvanceDoorMotion(entry);
    const bool openingAllowed = GetDoorOpeningState(entry.state.primary) != 0;
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(entry.state.primary);
    AutomaticDoorProximity proximity{
        .entry = &entry,
        .sideFlags = static_cast<std::uint8_t>(metadata ? metadata->flags & 3U : 0),
        .openingAllowed = openingAllowed,
        .characterNear = false,
    };
    ForEachLiveCharacterInMapOrder(CheckAutomaticDoorProximity, &proximity);
    const bool wantsOpen = proximity.characterNear;
    const DoorLeafState leafState = GetLeafState(entry);
    if (wantsOpen && leafState == DoorLeafState::StableA) {

        (void)doorPlayRigidMotion(entry.state.primary, 0xf0aaU, 0.0f, 1.0f);

    } else if (!wantsOpen && leafState == DoorLeafState::StableB) {

        (void)doorPlayRigidMotion(entry.state.primary, 0xf0a9U, 0.0f, 1.0f);

    } else if (wantsOpen && leafState == DoorLeafState::ToA) {
        const float start = entry.taskEntry->motionEndFrame - entry.taskEntry->motionFrame;
        SetLeafState(entry, DoorLeafState::StableA);
        (void)doorPlayRigidMotion(entry.state.primary, 0xf0aaU, start, 1.0f);
    }

    entry.state.active = entry.taskEntry->motionType == 1;
}

void UpdateAutomaticDoorCrossing(DoorEntry &entry, const Vector3 &playerPosition) {
    if (entry.state.family != 7 || !entry.state.primaryResolved)
        return;
    const float localZ = DoorLocalZ(entry, playerPosition);
    const bool crossed = doorRuntime.previousAutomaticDoor.name == entry.state.primary.name &&
        ((doorRuntime.previousAutomaticLocalZ < 0.0f && localZ > 0.0f) || (doorRuntime.previousAutomaticLocalZ > 0.0f && localZ < 0.0f));
    if (crossed && entry.taskEntry->sortKey < kAutomaticDoorRadiusSquared && (doorRuntime.authorPhase == 0 || doorRuntime.authorPhase == 38) &&
        doorRuntime.destinationPhase == 0) {
        (void)doorTriggerTransition(entry.state.primary, kDoorTaskToken);
        doorRuntime.crossingControlAcquired = 0;
        doorRuntime.savedAreaJump = static_cast<std::int32_t>(EV_CanDoAreaJump());
        if (ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            doorRuntime.crossingControlAcquired = 1;
        } else
            EV_SetCanDoAreaJumpTo(0);
        doorRuntime.crossingLocalZ = localZ;
    }
    doorRuntime.previousAutomaticDoor = entry.state.primary;
    doorRuntime.previousAutomaticLocalZ = localZ;
}

bool ReadPlayerPosition(Vector3 &position) {
    if (!g_playerRuntime.character)
        return false;

    (void)plymUpdatePlayerSearchLocation(&position);
    return true;
}

void CacheNativeSoundName(std::array<char, 16> &destination, const char *source) {
    for (std::size_t index = 0; index < destination.size(); ++index) {
        destination[index] = source[index];
        if (source[index] == '\0')
            return;
    }
}

void SetDoorViewSpaceEuler(const Vector3i &rotation) {
    const std::int16_t x = static_cast<std::int16_t>(rotation.x);
    const std::int16_t y = static_cast<std::int16_t>(rotation.y);
    const std::int16_t z = static_cast<std::int16_t>(rotation.z);
    if (x == 0 && z == 0) {
        if (y != 0)
            Matrix4x4::RotateAroundY(sin_fixed16(y), cos_fixed16(y));
        return;
    }
    const float sx = sin_fixed16(x), sy = sin_fixed16(y), sz = sin_fixed16(z);
    const float cx = cos_fixed16(x), cy = cos_fixed16(y), cz = cos_fixed16(z);
    const Matrix4x4 transform{{
        {cz * cy, cy * sz, -sy, 0.0f},
        {(sy * sx) * cz - cx * sz, (sy * sx) * sz + cz * cx, cy * sx, 0.0f},
        {(cx * sy) * cz + sz * sx, (cx * sy) * sz - cz * sx, cy * cx, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    Matrix4x4::Multiply(&transform);
}

// @verified - p
std::uint64_t GetDoorRoomNumber(shen_name name) {
    const std::int32_t index = static_cast<std::uint8_t>(name.name >> 24U) - 529 + 10 * static_cast<std::uint8_t>(name.name >> 16U);
    switch (g_currentMap.name & 0xffffU) {
    case kDoorQaAreaPrefix.name:
        if (index < 32)
            return static_cast<std::uint32_t>(kDoorRoomNumbers[index]);
        break;
    case kDoorQbAreaPrefix.name:
        if (index < 5)
            return static_cast<std::uint32_t>(kDoorRoomNumbers[index + 32]);
        break;
    case kDoorQcAreaPrefix.name:
        if (index < 7)
            return static_cast<std::uint32_t>(kDoorRoomNumbers[index + 40]);
        break;
    case kDoorQdAreaPrefix.name:
        if (index < 7)
            return static_cast<std::uint32_t>(kDoorRoomNumbers[index + 48]);
        break;
    case kDoorQeAreaPrefix.name:
        if (index < 21)
            return static_cast<std::uint32_t>(kDoorRoomNumbers[index + 56]);
        break;
    }
    return static_cast<std::uint32_t>(index + 1);
}

void ReleaseDoorNumberModel() {
    for (DoorEntry &entry : doorRuntime.entries) {
        if (entry.numberRoot)
            entry.numberRoot->runtimeChild = entry.originalNumberChild;
    }
    doorNumberTemplates.fill(nullptr);
    ReleaseModelFormat7(doorNumberModel);
    HLib::FreeTaskData(doorNumberAllocation);
    doorNumberAllocation = nullptr;
}

void LoadDoorNumberModel() {
    constexpr const char *path = "misc/QRRNWA0G.MT7";
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (size == 0) {
        return;
    }
    doorNumberAllocation = HLib::AllocateTaskData(size, kDoorNumberStorageToken);
    if (!doorNumberAllocation || !ArchiveData_Read(path, 0, doorNumberAllocation, size) ||
        !ReadModelFormat7({static_cast<const std::byte *>(doorNumberAllocation), static_cast<std::size_t>(size)}, path, doorNumberModel, 0)) {
        return;
    }
    BindMT7WritableRuntimeMeshes(doorNumberModel, {static_cast<std::byte *>(doorNumberAllocation), static_cast<std::size_t>(size)});
    const MT7Node *node = doorNumberModel.nodes.empty() ? nullptr : doorNumberModel.nodes.front().runtimeChild;
    for (std::size_t index = 0; index < doorNumberTemplates.size(); ++index) {
        doorNumberTemplates[index] = node;
        if (node)
            node = node->runtimeSibling;
    }
}

void UpdateDoorNumberNodes(DoorEntry &entry) {
    MT7Node *const root = FindDoorPropNode(entry.primaryBinding.node, (entry.state.primary.name & 0xffffff00U) | 'Q');
    if (!root)
        return;
    if (!entry.numberRoot) {
        entry.numberRoot = root;
        entry.originalNumberChild = root->runtimeChild;
    }
    root->runtimeChild = entry.numberNodes.data();
    std::int32_t stream = doorRuntime.currentStreamId;
    if (stream <= 0)
        stream = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(stream) + 1U);
    const std::int32_t room = static_cast<std::int32_t>(GetDoorRoomNumber(entry.state.primary));
    const std::int32_t digits[4]{stream / 10, stream % 10, room / 10, room % 10};
    float centering = 0.0f;
    for (std::size_t index = 0; index < entry.numberNodes.size(); ++index) {
        MT7Node &node = entry.numberNodes[index];
        const MT7Node *const source = doorNumberTemplates[digits[index]];
        if (!source)
            continue;
        node.id = source->id;
        node.sourceOffset = source->sourceOffset;
        node.position = source->position;
        node.rotation = source->rotation;
        node.scale = source->scale;
        node.meshType = 0;
        node.modelMarker = 0;
        node.xb01Offset = source->xb01Offset;
        node.runtimeMesh = source->runtimeMesh;
        node.borrowedMesh = source->borrowedMesh ? source->borrowedMesh : source;
        node.transformOverride = source->transformOverride;
        node.taskData = source->taskData;
        node.runtimeChild = nullptr;
        node.runtimeParent = root;
        node.runtimeSibling = index + 1 < entry.numberNodes.size() ? &entry.numberNodes[index + 1] : nullptr;
        node.position.x += static_cast<float>(index) * 0.079999998f - centering;
        node.visible = (entry.taskEntry->flags & 0x10U) != 0;
        entry.numberCommands[index] = {node.sourceOffset, doorNumberModel.source, source->xb01Offset, &node.borrowedMesh->drawResources};
        node.commandOverride = &entry.numberCommands[index];
        if (index == 0 && stream / 10 == 0) {
            node.xb01Offset = 0;
            node.runtimeMesh = nullptr;
            node.commandOverride = nullptr;
            centering = 0.039999999f;
        }
    }
}



void RecursiveSetDoorViewSpace(MT7Node *node) {
    while (node) {
        const bool hasSibling = node->runtimeSibling != nullptr;
        if (hasSibling)
            Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        SetDoorViewSpaceEuler(node->rotation);
        DoorEntry *const entry = FindDoorEntry(static_cast<std::uint32_t>(IsMDPXorMDP7(node)));
        if (entry) {
            Matrix4x4::GetTransformMatrix(&entry->taskEntry->worldTransform);
            entry->state.yawRadians = std::atan2(workTransformMatrix.rows[0].z, workTransformMatrix.rows[0].x);
            Matrix4x4_Push();
            const std::uint8_t family = static_cast<std::uint8_t>(entry->state.primary.name >> 8U);
            if (static_cast<std::uint8_t>(family - '5') > 2U)
                Matrix4x4::preTranslate(0.44999999f, 0.0f, 0.0f);
            entry->taskEntry->origin = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            entry->state.interactionPoint = entry->taskEntry->origin;
            Matrix4x4::Pop(1);
            UpdateDoorNumberNodes(*entry);
        }
        RecursiveSetDoorViewSpace(node->runtimeChild);
        if (!hasSibling)
            return;
        Matrix4x4::Pop(1);
        node = node->runtimeSibling;
    }
}

// @verified - p
void ApplyDoorSceneStateToSiblings(MT7Node *node, std::int32_t sceneId) {
    while (node) {
        const std::uint8_t modelMarker = static_cast<std::uint8_t>(IsMDPXorMDP7(node));
        if (modelMarker == 72U)
            node->visible = static_cast<std::uint16_t>(sceneId) == 0;
        else if (modelMarker == 78U)
            node->visible = sceneId != 0;

        ApplyDoorSceneStateToSiblings(node->runtimeChild, sceneId);
        node = node->runtimeSibling;
    }
}

// @verified - p
void RefreshDoorSceneStateForCurrentStep() {
    if ((g_currentMap.name & 0xffffU) != kDoorQeAreaPrefix.name)
        return;
    DoorEntry *const entry = FindDoorEntry(kDoorQeStepSubject.name);
    if (!entry || !entry->primaryBinding.node)
        return;
    ApplyDoorSceneStateToSiblings(entry->primaryBinding.node, std::bit_cast<std::int32_t>(GetCurrentStep()) < 340000 ? 0 : 1);
}

void ApplyDoorSceneState(std::int32_t sceneId) {
    if (!doorRuntime.entriesActive)
        return;
    for (DoorEntry &entry : doorRuntime.entries) {
        if (entry.primaryBinding.node)
            ApplyDoorSceneStateToSiblings(entry.primaryBinding.node, sceneId);
    }
    RefreshDoorSceneStateForCurrentStep();
}

void DoorSceneCleanup(std::int32_t suppressAudio) {
    std::array<char, 16> sceneId{};
    std::memcpy(sceneId.data(), &doorRuntime.temporaryAuthor, sizeof(doorRuntime.temporaryAuthor));
    if (EV_LoadCutsceneFromCurrentScene(sceneId.data()) != 0) {
        std::array<char, 9> soundBank{};
        std::memcpy(soundBank.data(), sceneId.data(), 4);
        std::memcpy(soundBank.data() + 4, "000_", 4);
        if (suppressAudio == 0)
            (void)EV_LoadAudio(soundBank.data(), 0);
    }

    SetWeatherInstruction(0x780U, false);
    SetWeatherInstruction(0x0cU, false);
    LGHT_Parser_Internal(doorRuntime.temporaryAuthor.name);
    const std::uint32_t changedReaders = doorLightingReaderMask ^ static_cast<std::uint32_t>(GetMapModelReaderMask());
    for (DoorEntry &entry : doorRuntime.entries) {
        entry.taskEntry->motionNode.rotation.y = 0;
        entry.motionNode.rotation.y = 0;
    }

    for (std::int32_t index = 0; index < 32; ++index) {
        if ((changedReaders & (1U << index)) != 0) {
            (void)SetMapModelGridCullingEnabled(0, index);
            SetMapLightingPlacementEnabled(1, index);
            AssignLightProgram(static_cast<std::uint32_t>(index));
        } else {
            EV_AssignLightParams(1, index);
            (void)SetMapModelGridCullingEnabled(1, index);
            SetMapLightingPlacementEnabled(0, index);
        }
    }
    SetupLightConfig();
    camCtrlSetNearDistance(20.0f);
    camCtrlSetNpcRadius(20.0f);
}

void RestoreDoorMapReadersAndCamera() {
    for (std::int32_t index = 0; index < 32; ++index) {
        if ((doorLightingReaderMask & (1U << index)) == 0)
            continue;
        (void)SetMapModelGridCullingEnabled(1, index);
        SetMapLightingPlacementEnabled(1, index);
    }
    const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
}

void ReleaseDoorAuthorPackage() {
    char package[8]{};
    std::memcpy(package, &doorRuntime.temporaryAuthor, sizeof(shen_name));
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
}

std::int64_t RestoreDoorAuthor(std::int32_t restoreMedia) {
    if (doorRuntime.authorPhase != 38)
        return 0;
    spmFadeOut();
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    ReleaseDoorAuthorPackage();
    if (restoreMedia)
        authLoadCurrentQueued();
    else {
        SetMapModelDataDefaultVertexShader();
        SetWeatherInstruction(0x780U, true);
        SetWeatherInstruction(0x0cU, true);
        doorRuntime.cleanupCallback = doorRuntime.savedCleanupCallback;
        if (doorRuntime.savedCleanupCallback)
            doorRuntime.savedCleanupCallback(doorRuntime.cachedSceneId);
        SetQronMapLightingEnabled(1);
    }
    EV_NPC_NewLoadSw(1);
    SetDoorAuthorSecondaryNodes(doorRuntime.temporaryAuthor, 1);
    SetWeatherLightManager(1);
    SyncWindWithWDATWeatherState();
    RestoreDoorMapReadersAndCamera();
    for (DoorEntry &entry : doorRuntime.entries) {
        if (entry.taskEntry->authorPackage.name != doorRuntime.temporaryAuthor.name)
            continue;
        if (entry.taskEntry->callback)
            entry.taskEntry->callback(doorRuntime.temporaryAuthor.name);
        break;
    }
    EV_spmBeginSampleExecute();
    if (flagRelated(g_currentMap))
        shenUpdatePlayerDataFlags(1);
    doorRuntime.authorPhase = 0;
    doorRuntime.temporaryAuthor = {};
    return 1;
}

std::int64_t LoadDoorAuthor(shen_name author, std::int32_t withAudio) {
    if (!author.name)
        return RestoreDoorAuthor(withAudio);
    doorRuntime.temporaryAuthor = author;
    doorRuntime.authorPhase = 38;
    doorLightingReaderMask = static_cast<std::uint32_t>(GetMapModelReaderMask());
    EV_NPC_NewLoadSw(0);
    if (withAudio)
        spmFadeOut();
    EV_ResetFreeTalkFlags();
    char package[8]{};
    std::memcpy(package, &author, sizeof(author));
    if (FindPakByEntryName(package))
        return 0;
    LoadMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), package);
    LoadPak();
    SetupDoorAreaCallbacks();
    DoorSceneCleanup(withAudio ? 0 : 1);
    EV_NPC_NewLoadSw(1);
    SetDoorAuthorSecondaryNodes(author, 0);
    SetWeatherLightManager(0);
    SetNewWindPhaseAndSpeed(0, 0.0f);
    if (flagRelated(author)) {
        shenUpdatePlayerDataFlags(1);
        shenUpdateFlagEvaluation(1);
    }
    if (withAudio)
        EV_spmBeginSampleExecute();
    return 1;
}

void CompleteDoorPlayerPlacement(DoorEntry &entry) {
    Character *const player = g_playerRuntime.character;
    MotionWork *const motion = FindMotionWork(player);
    SetPlayerControlEnabled(0);
    Vector3 position = GetCharacterWorldPosition(player);
    if (entry.state.family != 7) {
        const std::uint16_t animation = GetMotionAnimationId(motion, MotionRegion::LowerBody);
        float current{}, maximum{};
        CopyMotionRegionTimes(motion, nullptr, &current, &maximum, nullptr, MotionRegion::LowerBody);
        (void)plymFinishRegionInteraction();
        Vector3 first{}, last{};
        SampleNativeMotionRoot(animation, GetMotionRigBoneId(motion), current, first);
        SampleNativeMotionRoot(animation, GetMotionRigBoneId(motion), maximum, last);
        Vector3i rotation{};
        GetScenarioCharacterEulerAngles(kDoorPlayerName, &rotation, EventTransformFlags::World);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        SetDoorViewSpaceEuler(rotation);
        Matrix4x4::preTranslate(last.x - first.x, 0.0f, last.z - first.z);
        position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        position = DoorLocalPosition(entry, position);
        Matrix4x4::SetActiveTransform(&entry.taskEntry->worldTransform);
        position.z = position.z >= 0.0f ? 1.4f : -1.4f;
        position = TransformPoint(workTransformMatrix, position);
        Matrix4x4::Pop(1);
    } else {
        position = DoorLocalPosition(entry, position);
        position.z = doorRuntime.crossingLocalZ >= 0.0f ? 1.5f : -1.5f;
        if (position.x < -0.69999999f)
            position.x = -0.69999999f;
        else if (position.x > 0.69999999f)
            position.x = 0.69999999f;
        position = TransformPoint(entry.taskEntry->worldTransform, position);
    }
    const float height = position.y;
    position.y += 0.5f;
    position.y = shenFindIntersectionWithScene(position, position.y);
    if (std::fabs(position.y - height) > 0.5f)
        position.y = height;
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    shenApplyTranslationToAll(&player->entity.node, position);
    mtwkRequestMotion(motion, 0xf002U, MotionRegion::LowerBody);
}

void FinishDoorAuthorLeaf() {
    DoorEntry *const entry = FindDoorEntry(doorRuntime.authorDoor.name);
    if (!entry)
        return;
    if (entry->taskEntry->callback)
        entry->taskEntry->callback(doorRuntime.authorDoor.name);
    charaToggleFlag(g_playerRuntime.character, 1);
    entry->taskEntry->flags |= 0x20U;
    if (entry->state.family == 7) {
        SetLeafState(*entry, DoorLeafState::StableB);
        StartDoorMotion(*entry, 0xf0a9U, DoorLeafState::ToA);
        while (entry->taskEntry->motionType == 1)
            AdvanceDoorMotion(*entry);
    } else if (GetLeafState(*entry) == DoorLeafState::ToB) {
        while (entry->taskEntry->motionType == 1)
            AdvanceDoorMotion(*entry);
    }
    entry->taskEntry->flags &= ~0x20U;
    CompleteDoorPlayerPlacement(*entry);
}

void RestoreDoorPlayerControl() {
    CharacterControlModel *const control = FindCharacterControlModel(g_playerRuntime.character);
    if (control) {
        control->flags |= CharacterControlFineRaycast;
        control->flags &= ~CharacterControlStairMode;
        control->flags |= CharacterControlEvaluateMotion;
    }
    SetPlayerControlEnabled(1);
}

bool AcquireAutomaticDoorControl(shen_name subject) {
    if (static_cast<std::uint8_t>(subject.name >> 8U) != '7' || doorRuntime.crossingControlAcquired != 0)
        return true;
    if (!ryoNotBusy())
        return false;
    SetPlayerControlEnabled(0);
    doorRuntime.crossingControlAcquired = 1;
    EV_SetCanDoAreaJumpTo(doorRuntime.savedAreaJump);
    return true;
}

void UpdateDoorAuthorController() {
    using enum CameraState;
    switch (doorRuntime.authorPhase) {
    case 0:
    case 38:
        return;
    case 1:
    case 39:
        if (!AcquireAutomaticDoorControl(doorRuntime.authorDoor))
            return;
        doorRuntime.authorCameraState = camCtrlGetState();
        if (doorRuntime.authorCameraState == LockedOn)
            doorRuntime.authorCameraState = Init;
        camCtrlGetNextCamPos(&doorRuntime.authorCameraPosition);
        camCtrlSetState(Event);
        camCtrlSetNextPosition(doorRuntime.authorCameraPosition);
        if (doorRuntime.authorPhase == 1)
            doorLightingReaderMask = static_cast<std::uint32_t>(GetMapModelReaderMask());
        StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 20, 0, 255);
        if (static_cast<std::uint8_t>(doorRuntime.authorDoor.name >> 8U) == '7') {
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(1);
        }
        EV_NPC_NewLoadSw(0);
        spmFadeOut();
        ++doorRuntime.authorPhase;
        return;
    case 2:
    case 40:
        if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
            return;
        EV_ResetFreeTalkFlags();
        ++doorRuntime.authorPhase;
        return;
    case 3:
        Queue_ST_LoadTitleFadeIn(doorRuntime.temporaryAuthor);
        ++doorRuntime.authorPhase;
        return;
    case 34:
    case 72:
        FinishDoorAuthorLeaf();
        ++doorRuntime.authorPhase;
        return;
    case 35: {
        if (PakLoadWait())
            return;
        char package[8]{};
        std::memcpy(package, &doorRuntime.temporaryAuthor, sizeof(shen_name));
        if (FindPakByEntryName(package) || !LoadMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), package))
            return;
        LoadPak();
        SetupDoorAreaCallbacks();
        EV_NPC_NewLoadSw(1);
        SetDoorAuthorSecondaryNodes(doorRuntime.temporaryAuthor, 0);
        SetWeatherLightManager(0);
        SetNewWindPhaseAndSpeed(0, 0.0f);
        camCtrlSetState(doorRuntime.authorCameraState);
        camCtrlSetFovTransitionTo(true);
        RestoreDoorPlayerControl();
        ST_LoadTitleFade();
        if (flagRelated(doorRuntime.temporaryAuthor)) {
            shenUpdatePlayerDataFlags(1);
            shenUpdateFlagEvaluation(1);
        }
        const bool suppressAudio = (GetLoadingPhaseFlags() & 4U) != 0;
        if (!suppressAudio)
            EV_spmBeginSampleExecute();
        DoorSceneCleanup(suppressAudio ? 1 : 0);
        if ((GetLoadingPhaseFlags() & 1U) != 0)
            doorRuntime.authorPhase = 38;
        else
            ++doorRuntime.authorPhase;
        return;
    }
    case 36:
    case 74:
        StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 20, 255, 0);
        ++doorRuntime.authorPhase;
        return;
    case 37:
        if (!DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
            ++doorRuntime.authorPhase;
        return;
    case 41:
        doorRuntime.authorPhase = 72;
        return;
    case 73:
        ReleaseDoorAuthorPackage();
        authLoadCurrentQueued();
        EV_NPC_NewLoadSw(1);
        SetDoorAuthorSecondaryNodes(doorRuntime.temporaryAuthor, 1);
        SetWeatherLightManager(1);
        SyncWindWithWDATWeatherState();
        camCtrlSetState(doorRuntime.authorCameraState);
        camCtrlSetFovTransitionTo(true);
        RestoreDoorMapReadersAndCamera();
        RestoreDoorPlayerControl();
        EV_spmBeginSampleExecute();
        if (flagRelated(g_currentMap))
            shenUpdatePlayerDataFlags(1);
        ++doorRuntime.authorPhase;
        return;
    case 75:
        if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
            return;
        doorRuntime.authorPhase = 0;
        doorRuntime.temporaryAuthor = {};
        doorRuntime.authorDoor = {};
        return;
    default:
        ++doorRuntime.authorPhase;
        return;
    }
}

void UpdateDoorDestinationController() {
    using enum CameraState;
    switch (doorRuntime.destinationPhase) {
    case 1: {
        if (!AcquireAutomaticDoorControl(doorRuntime.destinationDoor))
            return;
        Vector3 position{};
        camCtrlGetNextCamPos(&position);
        camCtrlSetState(Event);
        camCtrlSetNextPosition(position);
        spmFadeOut();
        StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 30, 0, 255);
        EV_NPC_NewLoadSw(0);
        if (static_cast<std::uint8_t>(doorRuntime.destinationDoor.name >> 8U) == '7')
            shenToggleAreaFlags(2);
        ++doorRuntime.destinationPhase;
        return;
    }
    case 2:
        if (!DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
            ++doorRuntime.destinationPhase;
        return;
    case 3:
        shenQueueChangeModuleAndScene(SystemModule::Game, g_actNumber, doorRuntime.destinationArea, doorRuntime.destinationEntry);
        return;
    default:
        return;
    }
}

void DoorTask_Callback(void *);
void PublishDoorMotionNodes(void *) {
    const std::int32_t savedVertexColourType = GetLegacyVertexColourType();
    SetLegacyVertexColourType(0);
    for (DoorEntry &entry : doorRuntime.entries) {
        MT7Node *const primary = entry.primaryBinding.node;
        if (!primary)
            continue;
        if (entry.taskEntry->motionScale >= 1.0f) {
            primary->runtimeChild = &entry.motionNode;
        } else {
            MT7Node *const sibling = entry.motionNode.runtimeSibling;
            entry.motionNode.runtimeSibling = nullptr;
            primary->runtimeChild = nullptr;
            Matrix4x4_Push();
            Matrix4x4::MultiplyWithPrePassMatrix(&entry.taskEntry->worldTransform);
            DrawMT7NodeHierarchy(*entry.primaryBinding.model, entry.motionNode, LegacyModelShaderPath::Lambert, entry.taskEntry->motionScale);
            Matrix4x4::Pop(1);
            entry.motionNode.runtimeSibling = sibling;
        }
    }
    SetLegacyVertexColourType(savedVertexColourType);
}

void DoorAdmission_Callback(void *) {
    AdmitDoorAreaTable();
    HLib::SetCurrentTaskTo(DoorTask_Callback);
    if (g_doorQueuedAuthor.name) {
        LoadDoorAuthor(g_doorQueuedAuthor, 1);
        g_doorQueuedAuthor = {};
    }
}

void DoorTask_Callback(void *) {
    if (!doorRuntime.entriesActive || doorRuntime.entries.empty())
        return;

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);

    if (EV_GetShenmueFlag(0xff) != 0 && doorRuntime.cachedSceneId == 0) {
        doorRuntime.cachedSceneId = 1;
        ApplyDoorSceneState(1);
    }

    std::int32_t streamId = static_cast<std::int32_t>(GetQronNextStream());
    if (streamId == -1000)
        streamId = 1;
    if (streamId != doorRuntime.currentStreamId) {
        doorRuntime.currentStreamId = streamId;
        TransformPropObject();
        RefreshDoorSceneStateForCurrentStep();
    }

    Vector3 playerPosition{};
    if (!ReadPlayerPosition(playerPosition))
        return;

    RefreshDoorDistances(playerPosition);
    for (DoorEntry &entry : doorRuntime.entries) {
        UpdateAutomaticDoor(entry);
        if (entry.state.family != 7 && (GetLeafState(entry) == DoorLeafState::ToB || GetLeafState(entry) == DoorLeafState::ToA)) {
            (void)AdvanceDoorMotion(entry);
        }
    }

    if (!doorRuntime.sortedEntries.empty()) {
        UpdateAutomaticDoorCrossing(doorRuntime.entries[doorRuntime.sortedEntries.front()], playerPosition);
    }

    if (doorSearchRegion.name != 0) {
        drUpdateEntry();
    }
    UpdateDoorAuthorController();
    UpdateDoorDestinationController();
    HLib::ExecuteCallbackNow(PublishDoorMotionNodes, nullptr);
}

} // namespace
// @verified - p
void TransformPropObject() {
    Matrix4x4_Push(&transformMatrix_prePass);
    Vector3 position{};
    GetPropVector4(&position);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    RecursiveSetDoorViewSpace(GetPropRoot());
    Matrix4x4::Pop(1);
}

bool camCtrlSwitchToEventCam(std::int32_t frames, const Vector3 *target, std::int16_t yaw) {
    using enum CameraState;
    if (!g_cameraControl || !camCtrlComputeOrbitCameraFromTarget(target, yaw, &doorRuntime.cameraEndPosition, &doorRuntime.cameraEndTarget)) {
        return false;
    }
    doorRuntime.cameraDuration = frames;
    doorRuntime.cameraFrame = 0;
    camCtrlGetNextCamPos(&doorRuntime.cameraStartPosition);
    doorRuntime.cameraStartTarget = g_cameraControl->targetPosition;
    doorRuntime.previousCameraState = g_cameraControl->state == LockedOn ? Init : g_cameraControl->state;
    doorRuntime.cameraActive = true;
    camCtrlSetState(Event);
    return true;
}

void camCtrlApplyNextAndTargetPos() {
    if (!doorRuntime.cameraActive || !g_cameraControl)
        return;
    const float progress = std::fmin(1.0f, std::fmax(0.0f, static_cast<float>(++doorRuntime.cameraFrame) / static_cast<float>(doorRuntime.cameraDuration)));
    for (std::size_t axis = 0; axis < 3; ++axis) {
        g_cameraControl->previousPosition[axis] = doorRuntime.cameraStartPosition[axis] * (1.0f - progress) + doorRuntime.cameraEndPosition[axis] * progress;
        g_cameraControl->targetPosition[axis] = doorRuntime.cameraStartTarget[axis] * (1.0f - progress) + doorRuntime.cameraEndTarget[axis] * progress;
    }
}

// doorFindEntryTransform: the door-task transform registered under a search-region key.
// @verified - p
const Matrix4x4 *doorFindEntryTransform(std::uint32_t doorId) {
    const DoorEntry *const entry = FindDoorEntry(doorId);
    return entry ? &entry->taskEntry->worldTransform : nullptr;
}

bool IsDoorInteractionActive() {
    return doorSearchRegion.name != 0;
}

shen_name doorGetSearchRegion() {
    return doorSearchRegion;
}

// @verified - p
std::int32_t drGetAnswerSide(shen_name *region) {
    if (region)
        *region = drAnswerSide ? doorSearchRegion : shen_name{};
    return drAnswerSide;
}



shen_name ComputeRegionKeyForPos(const Vector3 *position) {
    const DoorTaskEntry *const head = doorRuntime.sortedHead;
    if (!position || !doorRuntime.task || !head || head->sortKey > kDoorInteractionRadiusSquared)
        return {};
    if (static_cast<std::uint8_t>(head->sourcePair->name >> 8U) == '7')
        return {};
    return *head->sourcePair;
}




std::int64_t drBeginEntry(shen_name searchRegion) {
    drEntryState = 0;
    if (ryoNotBusy() && shenSetPlayerDataFields(static_cast<std::int32_t>(searchRegion.name), 0) == 0) {
        doorSearchRegion = searchRegion;
        drEntryKind = drGetEntryKind(searchRegion);
        return 1;
    }
    const shen_name ryo = GetCharacterScenarioName(ctrlGetRootNode());
    if (DoorEntry *const entry = FindDoorEntry(searchRegion.name); entry && (entry->taskEntry->flags & 3U) == 0) {
        if (!entry->taskEntry->claimant.name || entry->taskEntry->claimant.name == ryo.name)
            entry->taskEntry->claimant = {};
    }
    (void)plymFinishRegionInteraction();
    return 0;
}


bool drIsDoorButtonAvailable(shen_name searchRegion) {
    const DoorEntry *const entry = FindDoorEntry(searchRegion.name);
    if (!entry)
        return false;
    const std::uint32_t state = entry->taskEntry->flags & 3U;
    return state == 2 || (state == 0 && !entry->taskEntry->claimant.name);
}


shen_name drGetCurrentAuthorId() {
    if (!doorRuntime.taskEntries)
        return {};
    return g_doorQueuedAuthor.name != 0 ? g_doorQueuedAuthor : doorRuntime.temporaryAuthor;
}



void ResetDoorInteraction() {
    Character *const ryo = ctrlGetRootNode();
    charaToggleFlag(ryo, 1);
    toggleCCMflagsByAmt(ryo, CharacterControlEvaluateMotion);
    toggleCCMflagsByAmt(ryo, CharacterControlFineRaycast);
    ccmSetFlagsFor(ryo, CharacterControlStairMode);
    const shen_name name = GetCharacterScenarioName(ryo);
    if (DoorEntry *const entry = FindDoorEntry(doorSearchRegion.name); entry && (entry->taskEntry->flags & 3U) == 0) {
        if (!entry->taskEntry->claimant.name || entry->taskEntry->claimant.name == name.name)
            entry->taskEntry->claimant = {};
    }
    drEntryState = 0;
    doorSearchRegion = {};
}



// @verified - p
std::int32_t GetDoorOpeningState(shen_name doorId) {
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(doorId);
    if (!metadata)
        return 2;

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    const std::uint32_t time = static_cast<std::uint32_t>(dateTime.minute + 100 * dateTime.hour);
    if (metadata->opens == metadata->closes)
        return 3;

    const bool overnight = metadata->opens > metadata->closes;
    const std::uint32_t lower = overnight ? 0U : static_cast<std::uint32_t>(metadata->opens);
    const std::uint32_t upper = overnight ? 2400U : static_cast<std::uint32_t>(metadata->closes);
    return (static_cast<std::uint32_t>(metadata->opens) < time && time < upper) || (lower < time && time < static_cast<std::uint32_t>(metadata->closes));
}

std::int32_t GetSelectedNpcDoorId() {
    return doorRuntime.ingressDoorId;
}

const Vector3 &GetDoorIngressOrigin() {
    return doorRuntime.ingressOrigin;
}

// @verified - p
void UnlinkDoorEntry(shen_name doorId) {
    if (!doorRuntime.entriesActive || !doorRuntime.taskEntries)
        return;
    DoorEntry *const entry = FindDoorEntry(doorId.name);
    if (!entry)
        return;
    DoorTaskEntry &data = *entry->taskEntry;
    data.flags |= 4U;
    if (data.previous)
        data.previous->next = data.next;
    else
        doorRuntime.sortedHead = data.next;
    if (data.next)
        data.next->previous = data.previous;
    else
        doorRuntime.sortedTail = data.previous;
    doorRuntime.sortedEntries.resize(doorRuntime.entries.size());
    std::size_t count = 0;
    for (DoorTaskEntry *current = doorRuntime.sortedHead; current; current = current->next)
        doorRuntime.sortedEntries[count++] = static_cast<std::size_t>(current - doorRuntime.taskEntries);
    doorRuntime.sortedEntries.resize(count);
}
bool HasDoorEntry(shen_name doorId) {
    return FindDoorEntry(doorId.name) != nullptr;
}

bool IsDoorAvailableForNpcRoute(shen_name doorId, const Vector3 &position) {
    const DoorEntry *const entry = FindDoorEntry(doorId.name);
    if (!entry)
        return false;

    const float x = position.x - entry->taskEntry->origin.x;
    const float z = position.z - entry->taskEntry->origin.z;
    const DoorLeafState state = GetLeafState(*entry);
    return x * x + z * z <= 400.0f && (state == DoorLeafState::StableB || (state == DoorLeafState::StableA && !entry->taskEntry->claimant.name));
}

bool ClaimDoor(shen_name doorId, shen_name scenarioName) {
    DoorEntry *const entry = FindDoorEntry(doorId.name);
    if (!entry || !entry->state.primaryResolved || !doorRuntime.task)
        return false;
    const bool automatic = static_cast<std::uint8_t>(doorId.name >> 8U) == '7';
    if (automatic && scenarioName.name == kDoorTaskToken.name) {
        entry->taskEntry->claimant = scenarioName;
        return true;
    }
    const DoorLeafState state = GetLeafState(*entry);
    if (automatic || state == DoorLeafState::StableA) {
        if (scenarioName.name != kDoorTaskToken.name && FindCharacterByScenarioName(scenarioName) != g_playerRuntime.character &&
            !doorRuntime.sortedEntries.empty() && entry == &doorRuntime.entries[doorRuntime.sortedEntries.front()] &&
            entry->taskEntry->sortKey < kDoorInteractionRadiusSquared) {
            return false;
        }
    }

    if (state == DoorLeafState::StableB)
        return true;
    if (state != DoorLeafState::StableA)
        return false;

    if (scenarioName.name != kDoorTaskToken.name && entry->taskEntry->claimant.name && scenarioName.name != entry->taskEntry->claimant.name) {
        return false;
    }
    entry->taskEntry->claimant = scenarioName;
    return true;
}

bool ComputeDoorIngress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition) {
    doorRuntime.ingressDoorId = 0;
    DoorEntry *const entry = FindDoorEntry(static_cast<std::uint32_t>(doorId));
    if (!entry || !entry->state.primaryResolved || !currentPosition || !targetPosition)
        return false;
    *targetPosition = DoorLocalPosition(*entry, *currentPosition);
    doorRuntime.ingressDoorId = doorId;
    doorRuntime.ingressOrigin = entry->state.interactionPoint;
    if (!(targetPosition->x * targetPosition->x + targetPosition->z * targetPosition->z <= 400.0f))
        return false;

    g_doorIngressRotation = ExtractDoorRotation(entry->taskEntry->worldTransform);
    std::uint32_t leafSide = entry->taskEntry->flags >> 3U;
    const bool negativeSide = targetPosition->z < 0.0f;
    if (!negativeSide)
        leafSide = ~leafSide;
    doorRuntime.ingressLeafSide = static_cast<std::int32_t>(leafSide & 1U);
    Vector3 localTarget{};
    switch (static_cast<std::uint8_t>(static_cast<std::uint32_t>(doorId) >> 8U) - '0') {
    case 1:
        localTarget = negativeSide ? Vector3{0.5f, 0.0f, -0.55000001f} : Vector3{0.55000001f, 0.0f, 0.80000001f};
        break;
    case 2:
    case 3:
    case 4:
    case 6:
        localTarget = {0.5f, 0.0f, negativeSide ? -0.55000001f : 0.55000001f};
        break;
    case 5:
    case 7:
        localTarget = {std::clamp(targetPosition->x, -0.44999999f, 0.44999999f), 0.0f, negativeSide ? -0.44999999f : 0.44999999f};
        break;
    default:
        *targetPosition = TransformPoint(entry->taskEntry->worldTransform, localTarget);
        doorRuntime.ingressPosition = *targetPosition;
        return true;
    }
    doorRuntime.ingressSide = negativeSide ? 1 : 0;
    if (negativeSide)
        g_doorIngressRotation.y += 0x8000;
    *targetPosition = TransformPoint(entry->taskEntry->worldTransform, localTarget);
    doorRuntime.ingressPosition = *targetPosition;
    return true;
}

bool ComputeDoorEgress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition) {

    doorRuntime.ingressDoorId = 0;
    DoorEntry *const entry = FindDoorEntry(static_cast<std::uint32_t>(doorId));
    if (!entry || !entry->state.primaryResolved || !currentPosition || !targetPosition)
        return false;
    doorRuntime.ingressDoorId = doorId;
    *targetPosition = DoorLocalPosition(*entry, *currentPosition);
    g_doorIngressRotation = ExtractDoorRotation(entry->taskEntry->worldTransform);
    const bool positiveSide = targetPosition->z > 0.0f;
    Vector3 localTarget{};
    switch (static_cast<std::uint8_t>(static_cast<std::uint32_t>(doorId) >> 8U)) {
    case '1':
        localTarget = positiveSide ? Vector3{0.5f, 0.0f, -0.55000001f} : Vector3{0.55000001f, 0.0f, 0.80000001f};
        break;
    case '2':
    case '3':
    case '4':
    case '6':
        localTarget = {0.5f, 0.0f, positiveSide ? -0.55000001f : 0.55000001f};
        break;
    case '5':
    case '7':
        localTarget = {targetPosition->x, 0.0f, positiveSide ? -0.55000001f : 0.55000001f};
        if (static_cast<double>(localTarget.x) > 0.45)
            localTarget.x = 0.44999999f;
        else if (static_cast<double>(localTarget.x) < -0.45)
            localTarget.x = -0.44999999f;
        break;
    default:
        *targetPosition = TransformPoint(entry->taskEntry->worldTransform, localTarget);
        doorRuntime.ingressPosition = *targetPosition;
        return true;
    }
    doorRuntime.ingressSide = positiveSide ? 0 : 1;
    if (positiveSide)
        g_doorIngressRotation.y += 0x8000;
    *targetPosition = TransformPoint(entry->taskEntry->worldTransform, localTarget);
    doorRuntime.ingressPosition = *targetPosition;
    return true;
}

std::uint32_t GetNpcDoorAnimation(std::int32_t doorId, Character *character, const Vector3 *position) {
    if (!character || !position)
        return 0;

    const std::int32_t bone = GetCharacterBoneId(character);
    if (bone < 0)
        return 0;

    Vector3 target{};
    (void)ComputeDoorIngress(doorId, position, &target);
    if (!doorRuntime.ingressDoorId)
        return 0;

    const bool negativeSide = doorRuntime.ingressSide != 0;
    const std::uint8_t family = static_cast<std::uint8_t>(static_cast<std::uint32_t>(doorId) >> 8U);

    switch (family) {
    case '1':
        switch (bone) {
        case 0:
        case 1:
        case 13:
        case 14:
            return negativeSide ? 61523 : 61520;
        case 2:
            return negativeSide ? 61570 : 61569;
        case 3:
        case 4:
            return negativeSide ? 61602 : 61601;
        case 5:
            return negativeSide ? 61669 : 61666;
        case 6:
            return negativeSide ? 61556 : 61553;
        case 8:
            return negativeSide ? 61456 : 61452;
        case 9:
            return negativeSide ? 61564 : 61563;
        case 10:
        case 11:
            return negativeSide ? 61532 : 61531;
        default:
            return negativeSide ? 61594 : 61591;
        }
    case '2':
    case '4':
    case '6':
        switch (bone) {
        case 0:
        case 1:
        case 2:
        case 13:
        case 14:
            return negativeSide ? 61521 : 61522;
        case 3:
        case 4:
        case 5:
            return negativeSide ? 61667 : 61668;
        case 6:
            return negativeSide ? 61554 : 61555;
        case 8:
            return negativeSide ? 61454 : 61455;
        default:
            return negativeSide ? 61592 : 61593;
        }
    case '3':
        switch (bone) {
        case 0:
        case 1:
        case 2:
        case 13:
        case 14:
            return negativeSide ? 61524 : 61525;
        case 3:
        case 4:
        case 5:
            return negativeSide ? 61671 : 61672;
        case 6:
            return negativeSide ? 61557 : 61558;
        case 8:
            return negativeSide ? 61468 : 61471;
        default:
            return negativeSide ? 61595 : 61596;
        }
    case '5':
    case '7':
        switch (bone) {
        case 0:
        case 1:
        case 2:
        case 13:
        case 14:
            return 61503;
        case 3:
        case 4:
        case 5:
            return 61650;
        case 6:
            return 61537;
        case 7:
        case 9:
        case 10:
        case 11:
        case 12:
            return 61575;
        case 8:
            return 61445;
        default:
            return 61572;
        }
    default:
        return 0;
    }
}

std::int64_t doorPlayRigidMotionRange(shen_name doorId, std::uint16_t motionId, float startFrame, float speed, float endFrame) {
    DoorEntry *const entry = FindDoorEntry(doorId.name);
    if (!entry)
        return 0;
    DoorTaskEntry *const data = entry->taskEntry;
    data->animationId = motionId;
    const RigidMotion *const motion = LoadRigidMotion(motionId);
    if (motion) {
        data->motionType = 1;
        data->motionDefinition = motion->source.data();
        data->motionTarget = &data->motionNode;
        data->motionFrame = 0.0f;
        data->motionStartFrame = 0.0f;
        data->motionEndFrame = static_cast<float>(motion->durationFrames);
        data->motionSpeed = motion->authoredRate;
        data->motionChannelState = 0;
    }
    data->motionSpeed *= speed;
    data->motionFrame = startFrame;
    if (endFrame > 0.0f)
        data->motionEndFrame = endFrame;
    const DoorLeafState state = GetLeafState(*entry);
    if (state == DoorLeafState::StableA)
        SetLeafState(*entry, DoorLeafState::ToB);
    else if (state == DoorLeafState::StableB)
        SetLeafState(*entry, DoorLeafState::ToA);
    AdvanceDoorMotion(*entry);
    return 1;
}

// @verified - p
std::int64_t doorPlayRigidMotion(shen_name doorId, std::uint16_t motionId, float startFrame, float speed) {
    DoorEntry *const entry = FindDoorEntry(doorId.name);
    if (!entry)
        return 0;
    if (entry->state.family == 7 || GetLeafState(*entry) == DoorLeafState::StableA)
        entry->taskEntry->claimant = kDoorTaskToken;
    else if (GetLeafState(*entry) != DoorLeafState::StableB)
        return 0;
    return doorPlayRigidMotionRange(doorId, motionId, startFrame, speed, -1.0f);
}

// @verified - p
std::int64_t doorLoopRigidMotion(shen_name doorId, std::uint16_t motionId, float startFrame, float speed) {
    DoorEntry *const entry = FindDoorEntry(doorId.name);
    if (entry && (entry->state.family == 7 || GetLeafState(*entry) == DoorLeafState::StableA))
        entry->taskEntry->claimant = kDoorTaskToken;
    return doorPlayRigidMotionRange(doorId, motionId, startFrame, speed, -1.0f);
}

bool BeginDoorAnimation(std::int32_t doorId, shen_name scenarioName, std::int32_t animationId) {
    return BeginDoorAnimation(doorId, scenarioName, animationId, 0.0f, 1.0f);
}

bool BeginDoorAnimation(std::int32_t doorId, shen_name scenarioName, std::int32_t animationId, float startFrame, float speedScale) {
    DoorEntry *const entry = FindDoorEntry(static_cast<std::uint32_t>(doorId));
    if (!entry || !entry->state.primaryResolved || !doorRuntime.task)
        return false;
    if (static_cast<std::uint8_t>(static_cast<std::uint32_t>(doorId) >> 8U) == '7')
        return true;
    if (entry->taskEntry->claimant.name != scenarioName.name)
        return false;
    const std::uint16_t leafMotion = npcGetRemappedAnimId(static_cast<std::uint32_t>(animationId));
    const RigidMotion *const motion = LoadRigidMotion(leafMotion);
    if (!motion || entry->animationTargets.empty() || !doorRuntime.collisionCharacters[0] || !doorRuntime.collisionCharacters[1]) {
        return false;
    }
    const DoorLeafState state = GetLeafState(*entry);
    const DoorLeafState transition = state == DoorLeafState::StableA ? DoorLeafState::ToB : state == DoorLeafState::StableB ? DoorLeafState::ToA : state;
    entry->taskEntry->blockingRadius = 0.40000001f;
    return StartDoorMotion(*entry, leafMotion, transition, startFrame, speedScale);
}

bool ReleaseDoor(std::int32_t doorId, shen_name scenarioName) {
    DoorEntry *const entry = FindDoorEntry(static_cast<std::uint32_t>(doorId));
    if (!entry || GetLeafState(*entry) != DoorLeafState::StableA)
        return false;

    if (entry->taskEntry->claimant.name && entry->taskEntry->claimant.name != scenarioName.name)
        return false;

    entry->taskEntry->claimant = {};
    return true;
}

void InitializeDoorRuntime(const MapInfoChunk *doorChunk) {
    if (g_actNumber >= 1 && g_actNumber <= 4) {
        char package[5]{'1', '0', '0', static_cast<char>('0' + g_actNumber), '\0'};
        EV_FreeAuthorPak(1, package);
    }
    const std::int32_t cachedSceneId = doorRuntime.cachedSceneId;
    ReleaseDoorNumberModel();
    doorRuntime = {};
    doorRuntime.cachedSceneId = cachedSceneId;
    doorRuntime.currentStreamId = 1;
    doorSearchRegion = {};
    if (!doorChunk || doorChunk->nextByteOffset < 0x18)
        return;
    if (!GetPropRoot())
        return;

    std::int32_t entryCount{};
    const auto *const bytes = reinterpret_cast<const std::byte *>(doorChunk);
    if (!ReadValue(bytes, doorChunk->nextByteOffset, 0x14, entryCount) || entryCount <= 0 ||
        static_cast<std::uint64_t>(entryCount) * 8U > doorChunk->nextByteOffset - 0x18U) {
        return;
    }
    doorRuntime.task = HLib::EnqueueTaskWithParameter(DoorAdmission_Callback, 8, 2, 0, kDoorTaskToken);
    doorRuntime.taskEntries =
        static_cast<DoorTaskEntry *>(HLib::AllocateTaskData(sizeof(DoorTaskEntry) * static_cast<std::size_t>(entryCount), kDoorTaskToken));
    if (!doorRuntime.taskEntries)
        return;
    std::memset(doorRuntime.taskEntries, 0, sizeof(DoorTaskEntry) * static_cast<std::size_t>(entryCount));

    doorRuntime.entries.reserve(static_cast<std::size_t>(entryCount));
    LoadDoorNumberModel();
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    doorRuntime.cachedSceneId = EV_GetShenmueFlag(0xff) != 0;
    std::size_t primaryResolved = 0;
    std::size_t pairedResolved = 0;
    bool hasAutomaticDoor = false;
    for (std::int32_t index = 0; index < entryCount; ++index) {
        shen_name primary{};
        shen_name secondary{};
        const std::size_t pairOffset = 0x18 + static_cast<std::size_t>(index) * 8;
        std::memcpy(&primary, bytes + pairOffset, sizeof(primary));
        std::memcpy(&secondary, bytes + pairOffset + 4, sizeof(secondary));
        DoorTaskEntry *const taskEntry = doorRuntime.taskEntries + index;
        taskEntry->sourcePair = reinterpret_cast<const shen_name *>(bytes + pairOffset);
        taskEntry->motionScale = 1.0f;
        taskEntry->previous = index == 0 ? nullptr : taskEntry - 1;
        taskEntry->next = index + 1 == entryCount ? nullptr : taskEntry + 1;
        DoorEntry entry = BuildDoorEntry(primary, secondary, taskEntry);
        if (entry.primaryBinding.node)
            ApplyDoorSceneStateToSiblings(entry.primaryBinding.node, doorRuntime.cachedSceneId);
        primaryResolved += entry.state.primaryResolved;
        pairedResolved += entry.state.primaryResolved && entry.state.secondaryResolved;
        hasAutomaticDoor |= entry.state.family == 7 && entry.animationTargets.size() >= 3;
        doorRuntime.entries.push_back(std::move(entry));
    }
    // Publish only after final storage has been allocated: scene/projected
    // consumers borrow both the MAP nodes and these DOOR-owned motion children.
    for (DoorEntry &entry : doorRuntime.entries) {
        if (MT7Node *const primary = entry.primaryBinding.node) {
            primary->xb01Offset = 0;
            primary->runtimeMesh = nullptr;
            primary->runtimeChild = &entry.motionNode;
            if (entry.motionNode.runtimeChild)
                entry.motionNode.runtimeChild->runtimeParent = &entry.motionNode;
        }
    }
    doorRuntime.sortedEntries.resize(doorRuntime.entries.size());
    for (std::size_t index = 0; index < doorRuntime.sortedEntries.size(); ++index) {
        doorRuntime.sortedEntries[index] = index;
    }
    doorRuntime.entriesActive = !doorRuntime.entries.empty();
    doorRuntime.sortedHead = doorRuntime.taskEntries;
    doorRuntime.sortedTail = doorRuntime.taskEntries + entryCount - 1;
    if (g_currentMap == kDoorWs00Map)
        for (const shen_name door : kDoorWs00ExcludedEntries)
            UnlinkDoorEntry(door);
    if (hasAutomaticDoor) {
        (void)LoadRigidMotion(0xf0aaU);
        (void)LoadRigidMotion(0xf0a9U);
    }

    TransformPropObject();
    for (std::size_t index = 0; index < std::size(kDoorCollisionNames); ++index) {
        HLib::Task *handlerTask{};
        Character *const character = AddHighModelCharacter(kDoorCollisionNames[index], handlerTask);
        if (!character || !AttachCharacterControlModel(character)) {
            FreeCharacter(character);
            continue;
        }
        doorRuntime.collisionCharacters[index] = character;
        FindCharacterControlModel(character)->flags = 0;
        shenApplyTranslationToAll(&character->entity.node, {});
    }
}

void ResetDoorRuntime() {
    using enum CameraState;
    if (doorRuntime.cameraActive)
        camCtrlSetState(Init);
    if (IsDoorInteractionActive())
        ResetDoorInteraction();
    if (doorRuntime.task) {
        HLib::DestroyTask(doorRuntime.task);
        doorRuntime.task = nullptr;
    }
    for (Character *character : doorRuntime.collisionCharacters)
        FreeCharacter(character);
    RestoreDoorTargets();
    if (doorRuntime.taskEntries)
        HLib::FreeTaskData(doorRuntime.taskEntries);
    // These fields match the long-lived dr_runtime_globals_t owner; map
    // teardown only releases the task, characters, targets, and entries.
    const shen_name temporaryAuthor = doorRuntime.temporaryAuthor;
    const DoorAreaCleanupCallback cleanupCallback = doorRuntime.cleanupCallback;
    const DoorAreaCleanupCallback savedCleanupCallback = doorRuntime.savedCleanupCallback;
    ReleaseDoorNumberModel();
    const std::int32_t cachedSceneId = doorRuntime.cachedSceneId;
    const std::int32_t currentStreamId = doorRuntime.currentStreamId;
    doorRuntime = {};
    doorRuntime.temporaryAuthor = temporaryAuthor;
    doorRuntime.cleanupCallback = cleanupCallback;
    doorRuntime.savedCleanupCallback = savedCleanupCallback;
    doorRuntime.cachedSceneId = cachedSceneId;
    doorRuntime.currentStreamId = currentStreamId;
}

void SetDoorAreaCleanupCallback(DoorAreaCleanupCallback callback) {
    doorRuntime.cleanupCallback = callback;
}

std::int64_t SetupDoorAreaCallbacks() {
    doorRuntime.savedCleanupCallback = doorRuntime.cleanupCallback;
    doorRuntime.cleanupCallback = DoorSceneCleanup;
    CacheNativeSoundName(cachedDoorCutsceneId, GetCurrentBgm());
    CacheNativeSoundName(cachedDoorSoundBankId, shenGetCurrentSbt());
    cachedDoorSoundSceneIndex = static_cast<std::int32_t>(GetSelectedSoundSceneIndex());
    return SetQronMapLightingEnabled(0) ? 1 : 0;
}

void ExecuteDoorAreaCleanup(std::int32_t sceneId) {
    const DoorAreaCleanupCallback callback = doorRuntime.cleanupCallback;
    if (callback)
        callback(sceneId);
}

// @verified - p
DoorAreaCleanupCallback ExecuteCleanupFuncForAreaWithCachedScene() {
    const DoorAreaCleanupCallback callback = doorRuntime.cleanupCallback;
    if (callback)
        callback(doorRuntime.cachedSceneId);
    return callback;
}

// @verified - p
std::int64_t authLoadCurrentQueued() {
    (void)EV_LoadCutsceneFromCurrentScene(cachedDoorCutsceneId.data());
    (void)EV_LoadAudio(cachedDoorSoundBankId.data(), cachedDoorSoundSceneIndex);
    SetMapModelDataDefaultVertexShader();
    SetWeatherInstruction(0x780U, true);
    SetWeatherInstruction(0x0cU, true);
    doorRuntime.cleanupCallback = doorRuntime.savedCleanupCallback;
    if (doorRuntime.savedCleanupCallback)
        doorRuntime.savedCleanupCallback(doorRuntime.cachedSceneId);
    return SetQronMapLightingEnabled(1) ? 1 : 0;
}

// @verified - p
void doorQueueAuthorForMap(shen_name author) {
    g_doorQueuedAuthor = author;
}

void SetTemporaryDoorAuthor(shen_name author) {
    if (author.name != 0)
        doorLightingReaderMask = static_cast<std::uint32_t>(GetMapModelReaderMask());
    doorRuntime.temporaryAuthor = author;
}

std::int64_t doorLoadAuthorWithAudio(shen_name author) {
    return LoadDoorAuthor(author, 1);
}

std::int64_t doorLoadAuthorSilently(shen_name author) {
    return LoadDoorAuthor(author, 0);
}

// @verified - p
std::int64_t doorTriggerTransition(shen_name subject, shen_name character) {
    DoorEntry *const entry = FindDoorEntry(subject.name);
    if (!entry)
        return 0;
    Character *const owner = FindCharacterByScenarioName(character);
    if (owner != g_playerRuntime.character && character.name != kDoorTaskToken.name)
        return 0;
    DoorTaskEntry *const data = entry->taskEntry;
    if (data->stream != doorRuntime.currentStreamId && data->stream != 0)
        return 0;
    if (data->authorPackage.name) {
        ++doorRuntime.authorPhase;
        doorRuntime.temporaryAuthor = data->authorPackage;
        doorRuntime.authorDoor = subject;
        return 1;
    }
    if (!data->destination.name) {
        if (data->callback)
            data->callback(subject.name);
        return 0;
    }
    constexpr shen_name collectionExit{SHEN_NAME(QCCE)};
    if (data->destination.name == collectionExit.name) {
        if (doorRuntime.currentStreamId == 3)
            data->destinationEntry = 1;
        else if (doorRuntime.currentStreamId == 4)
            data->destinationEntry = 2;
    }
    ++doorRuntime.destinationPhase;
    doorRuntime.destinationEntry = data->destinationEntry;
    doorRuntime.destinationArea = data->destination;
    doorRuntime.destinationDoor = subject;
    return 1;
}

// @verified - p
std::int64_t SetDoorSubjectCallback(shen_name subject, DoorSubjectCallback callback) {
    DoorEntry *const entry = FindDoorEntry(subject.name);
    if (!entry)
        return 0;
    entry->taskEntry->callback = callback;
    return 1;
}

// @verified - p
std::uint64_t SetDoorSubjectEnabled(shen_name subject, std::int32_t enabled) {
    const bool directModelOnly = static_cast<std::uint8_t>(subject.name >> 8U) == '0';
    MT7Node *node = nullptr;
    if (!directModelOnly && doorRuntime.entriesActive) {
        for (DoorEntry &entry : doorRuntime.entries) {
            if (entry.state.primary.name != subject.name)
                continue;
            if (enabled)
                entry.taskEntry->flags |= 0x10U;
            else
                entry.taskEntry->flags &= ~0x10U;
            node = entry.primaryBinding.node;
            break;
        }
    }
    if (!node)
        node = FindDoorPropNode(GetPropRoot(), subject.name);
    if (!node)
        return 0;
    const std::uint32_t hidden = enabled == 0 ? 1U : 0U;
    node->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | (hidden << 16U));
    node->visible = enabled != 0;
    if (!directModelOnly && node->runtimeChild)
        SetDoorNodeHierarchyHidden(node->runtimeChild, hidden);
    return 1;
}

// @verified - p
shen_name GetCurrentDoorAuthor() {
    if (!doorRuntime.entriesActive)
        return {};
    return g_doorQueuedAuthor.name ? g_doorQueuedAuthor : doorRuntime.temporaryAuthor;
}

bool HasDoorAuthorEntries() {
    return doorRuntime.entriesActive;
}

std::size_t GetDoorRuntimeEntryCount() {
    return doorRuntime.entries.size();
}

bool GetDoorRuntimeEntry(std::size_t index, DoorRuntimeEntryState &state) {
    if (index >= doorRuntime.entries.size())
        return false;
    state = doorRuntime.entries[index].state;
    const DoorTaskEntry *const data = doorRuntime.entries[index].taskEntry;
    state.flags = data->flags;
    state.activeMotionId = data->animationId;
    state.origin = data->origin;
    state.distanceSquared = data->sortKey;
    state.motionFrame = data->motionFrame;
    state.claimant = data->claimant;
    state.motionSpeed = data->motionSpeed;
    state.blockingRadius = data->blockingRadius;
    state.active = data->motionType == 1;
    return true;
}

// @verified - p: SetQeDoorSceneState.
std::int64_t SetQeDoorSceneState(std::int32_t mode) {
    if (mode != 1)
        return 0;
    DoorEntry *const entry = FindDoorEntry(kDoorQeStepSubject.name);
    if (!entry)
        return 0;
    if (entry->taskEntry->primary)
        ApplyDoorSceneStateToSiblings(entry->taskEntry->primary, 1);
    return 1;
}

} // namespace shenmue::s2
