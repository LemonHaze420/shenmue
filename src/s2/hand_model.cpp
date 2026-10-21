#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"

namespace shenmue::s2 {
using namespace model_component_detail;

namespace model_component_detail {

PresentationState *FindPresentation(const Character *character) {
    return presentationStates.contains(character) ? presentationStates.at(character).get() : nullptr;
}

template <class T> bool ReadHandValue(const HandModelResource &resource, std::size_t offset, T &value) {
    if (!resource.reader || offset > resource.reader->source.size() || sizeof(T) > resource.reader->source.size() - offset)
        return false;
    std::memcpy(&value, resource.reader->source.data() + offset, sizeof(T));
    return true;
}

bool ReadHandJointTree(HandModelResource &resource, std::size_t &cursor, std::size_t end, std::int32_t parent) {
    if (resource.joints.size() >= 72 || cursor > end || end - cursor < 96)
        return false;
    std::int32_t children{};
    HandPoseJoint joint{};
    joint.parent = parent;
    if (!ReadHandValue(resource, cursor, children) || children < 0 || children > 72 || !ReadHandValue(resource, cursor + 4, joint.id) || joint.id >= 72 ||
        !ReadHandValue(resource, cursor + 8, joint.position) || !ReadHandValue(resource, cursor + 20, joint.rotation) ||
        !ReadHandValue(resource, cursor + 32, joint.inverseBind))
        return false;
    for (const auto &previous : resource.joints)
        if (previous.id == joint.id)
            return false;
    joint.relativeParent = parent;
    while (joint.relativeParent >= 0 && resource.joints[static_cast<std::size_t>(joint.relativeParent)].id > 19)
        joint.relativeParent = resource.joints[static_cast<std::size_t>(joint.relativeParent)].relativeParent;
    const auto index = static_cast<std::int32_t>(resource.joints.size());
    resource.joints.push_back(joint);
    cursor += 96;
    for (std::int32_t child = 0; child < children; ++child)
        if (!ReadHandJointTree(resource, cursor, end, index))
            return false;
    return true;
}

// AllocateHandModels/BindHandPoseResource. MDH7 and MDHX use a 96-byte preorder joint
// stream, then 48-byte weighted vertices and an offset-linked XB01 remap.
bool BindHandPoseResource(HandModelResource &resource, std::uint32_t side) {
    if (!resource.reader)
        return false;
    const std::size_t sourceSize = resource.reader->source.size();
    for (std::size_t index = 0; index < resource.reader->nodes.size(); ++index) {
        MT7Node &node = resource.reader->nodes[index];
        if ((static_cast<std::uint32_t>(node.id) & 0xffffU) != ((side == 1 || side == 3) ? 69U : 68U))
            continue;
        const std::size_t base = static_cast<std::size_t>(node.sourceOffset) + 64U;
        std::uint32_t signature{}, vertexOffset{}, count{};
        if (!ReadHandValue(resource, base, signature) || (signature != 0x3748444dU && signature != 0x5848444dU))
            continue;
        if (!ReadHandValue(resource, base + 4, vertexOffset) || vertexOffset > sourceSize - base)
            return false;
        std::size_t cursor = base + 8;
        const std::size_t vertexHeader = base + vertexOffset;

        if (!ReadHandJointTree(resource, cursor, sourceSize, -1) || !ReadHandValue(resource, vertexHeader + 12, count) || count > sourceSize / 48U)
            return false;
        for (HandPoseJoint &joint : resource.joints) {
            if (joint.id > 19)
                continue;
            for (MT7Node &renderNode : resource.reader->nodes) {
                if ((static_cast<std::uint32_t>(renderNode.id) & 0xffffU) != joint.id)
                    continue;
                std::int32_t ancestor = renderNode.parent;
                while (ancestor >= 0 && ancestor != static_cast<std::int32_t>(index))
                    ancestor = resource.reader->nodes[static_cast<std::size_t>(ancestor)].parent;
                if (ancestor < 0)
                    continue;
                joint.renderNode = &renderNode;
                break;
            }
        }
        resource.poseVertices.resize(count);
        cursor = vertexHeader + 16;
        for (auto &vertex : resource.poseVertices) {
            if (!ReadHandValue(resource, cursor, vertex.position) || !ReadHandValue(resource, cursor + 12, vertex.normal))
                return false;
            cursor += 48;
        }
        std::uint32_t groups{};
        if (!ReadHandValue(resource, cursor + 8, groups) || groups > sourceSize / 8U)
            return false;
        cursor += 12;
        for (std::uint32_t group = 0; group < groups; ++group) {
            std::uint32_t joint{}, influenceCount{};
            if (!ReadHandValue(resource, cursor, joint) || joint >= 72 || !ReadHandValue(resource, cursor + 4, influenceCount) ||
                influenceCount > sourceSize / 8U)
                return false;
            const bool found = std::any_of(resource.joints.begin(), resource.joints.end(), [joint](const HandPoseJoint &value) { return value.id == joint; });
            if (!found)
                return false;
            cursor += 8;
            for (std::uint32_t entry = 0; entry < influenceCount; ++entry) {
                HandPoseInfluence influence{joint, 0, 0};
                if (!ReadHandValue(resource, cursor, influence.vertex) || !ReadHandValue(resource, cursor + 4, influence.weight) || influence.vertex >= count)
                    return false;
                resource.influences.push_back(influence);
                cursor += 8;
            }
        }
        std::uint32_t remapCount{}, vertex{};
        if (!ReadHandValue(resource, cursor + 4, resource.normalizedNormalCount) || resource.normalizedNormalCount > count ||
            !ReadHandValue(resource, cursor + 16, remapCount) || !ReadHandValue(resource, cursor + 20, vertex) || remapCount > sourceSize / 8U)
            return false;

        if (resource.normalizedNormalCount && !(resource.normalizedNormalCount & 3U))
            ++resource.normalizedNormalCount;
        if (resource.normalizedNormalCount > count)
            return false;
        if (!ReadHandValue(resource, cursor + 8, resource.markedVertexCount) || resource.markedVertexCount > count - resource.normalizedNormalCount)
            return false;
        resource.markedVertexCount += resource.normalizedNormalCount;
        cursor += 24;
        for (std::uint32_t entry = 0; entry < remapCount; ++entry) {
            MT7VertexMapping mapping{vertex, 0};
            if (vertex >= count || !ReadHandValue(resource, cursor, mapping.destinationByteOffset) || !ReadHandValue(resource, cursor + 4, vertex))
                return false;
            resource.mappings.push_back(mapping);
            cursor += 8;
        }
        resource.poseNode = index;
        resource.poseDirty = true;
        node.rotation = {};
        return true;
    }
    return true;
}

Vector3 TransformHandVector(const Vector3 &value, const Matrix4x4 &matrix, bool point) {
    Vector3 result{};
    for (std::size_t axis = 0; axis < 3; ++axis)
        result[axis] = value.x * matrix.rows[0][axis] + value.y * matrix.rows[1][axis] + value.z * matrix.rows[2][axis] + (point ? matrix.rows[3][axis] : 0.0f);
    return result;
}

} // namespace model_component_detail

HandModelResource *GetHandModelResource(const HandParams &hand) {
    PresentationState *const state = FindPresentation(hand.owner);
    if (!state || hand.selector < 1 || hand.selector > 4)
        return nullptr;
    for (HandModelResource *resource = state->handResources[(hand.selector - 1) & 1]; resource; resource = resource->next)
        if (resource->hand == &hand)
            return resource;
    return nullptr;
}

void LoadHandModels() {
    for (std::size_t side = 0; side < 2; ++side) {
        void *allocation =
            LoadLegacySceneFile("scene/01/model/hand", side ? "RYO_TR.MT7" : "RYO_TL.MT7", DEFAULT_HAND_MODEL_MEMORY, &defaultHandFileSizes[side]);
        defaultHandFiles[side] = allocation;
    }
}

void InitializeHandWork() {
    g_temporaryHNDL = g_temporaryHNDR = nullptr;
    handPoseMatrices = static_cast<Matrix4x4 *>(HLib::AllocateStaticOrDynamicBlock(0x1200, HAND_MATRIX_MEMORY));
    handJointMatrices = static_cast<Matrix4x4 *>(HLib::AllocateStaticOrDynamicBlock(0x1200, HAND_MATRIX_MEMORY));
    handRelativeMatrices = static_cast<Matrix4x4 *>(HLib::AllocateStaticOrDynamicBlock(0x1200, HAND_MATRIX_MEMORY));

    Matrix4x4::SetPrePassToTempTransform();
    for (std::size_t index = 0; index < 71; ++index) {
        if (handJointMatrices)
            Matrix4x4::GetTransformMatrix(handJointMatrices + index);
        if (handRelativeMatrices)
            Matrix4x4::GetTransformMatrix(handRelativeMatrices + index);
    }
    Matrix4x4::Pop(1);
    handPoseBanks = {};
    handCalculationWork = HLib::AllocateStaticOrDynamicBlock(0x18000, HAND_CALCULATION_MEMORY);
    handProbeWork = HLib::AllocateStaticOrDynamicBlock(0xa0, HAND_CALCULATION_MEMORY);
}

void *LoadHandPoseBank(std::int32_t act, shen_name area, const char *filename) {
    if (!handPoseMatrices || !handCalculationWork)
        return nullptr;
    PakGlobalData *package = GetNextReadyModelPackage(nullptr);
    void *data = nullptr;
    while (package) {
        data = FileLoader(package, filename, HAND_BANK_EXTENSION);
        if (data)
            break;
        package = GetNextReadyModelPackage(package);
    }
    if (!data) {
        char directory[64];
        std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", act, area.characters[0], area.characters[1], area.characters[2], area.characters[3]);
        if (!(data = LoadLegacySceneFile(directory, filename, HAND_BANK_MEMORY, nullptr)))
            return nullptr;
    }
    HandPoseBank *tail = &handPoseBanks;
    while (tail->next)
        tail = tail->next;
    HandPoseBank *const bank = static_cast<HandPoseBank *>(HLib::AllocateStaticOrDynamicBlock(sizeof(HandPoseBank), HAND_BANK_MEMORY));
    tail->next = bank;
    if (!bank)
        return nullptr;
    std::uint32_t range;
    std::memcpy(&range, data, sizeof(range));
    *bank = {.offsets = static_cast<std::int32_t *>(data),
        .first = static_cast<std::int16_t>(static_cast<std::int32_t>(range) / 65536),
        .last = static_cast<std::int16_t>(range),
        .reserved0c = 0,
        .next = nullptr,
        .previous = tail,
        .package = package};
    const std::int32_t count = bank->last - bank->first + 1;
    if (!(static_cast<std::uint32_t>(bank->offsets[1]) & 0xff000000U)) {
        for (std::int32_t index = 1; index <= count; ++index) {
            std::byte *const pose = static_cast<std::byte *>(data) + bank->offsets[index];
            bank->offsets[index] = HLib::GetDynamicHeapOffset(pose);
            std::int32_t jointCount;
            std::memcpy(&jointCount, pose + 36, sizeof(jointCount));
            std::int32_t *const channels = reinterpret_cast<std::int32_t *>(pose + 40);
            for (std::int32_t channel = 0; channel < 3 * jointCount; ++channel) {
                if (channels[2 * channel] > 0)
                    channels[2 * channel + 1] = HLib::GetDynamicHeapOffset(reinterpret_cast<std::byte *>(channels) + channels[2 * channel + 1]);
            }
        }
    }
    return data;
}

// @verified - p
std::uint64_t ReleaseHandPoseBank(void *data) {
    if (!data)
        return 1;
    HandPoseBank *bank = handPoseBanks.next;
    while (bank && bank->offsets != data)
        bank = bank->next;
    if (!bank)
        return 0;
    if (!bank->package)
        HLib::FreeTaskData(bank->offsets);
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    HLib::FreeTaskData(bank);
    return 1;
}

namespace model_component_detail {

std::int16_t InterpolateHandPoseChannel(std::int32_t count, const HandCubicKeyframe *keys, std::int32_t frame) {
    std::int32_t upper = count, lower = 0;
    do {
        const std::int32_t middle = (lower + upper) / 2;
        if (keys[middle].frame < frame)
            lower = middle;
        else
            upper = middle;
    } while (upper > lower + 1);
    float angle;
    if (!upper)
        angle = keys[0].degrees;
    else if (upper == count || keys[upper - 1].frame == frame)
        angle = keys[upper - 1].degrees;
    else {
        const HandCubicKeyframe &first = keys[upper - 1], &last = keys[upper];
        const float t = static_cast<float>(frame - first.frame) / static_cast<float>(last.frame - first.frame);
        const float square = t * t, cube = square * t;
        angle = ((cube - (square + square) + t) * first.outgoing + (cube - square) * last.incoming) * static_cast<float>(last.frame - first.frame) +
            ((cube + cube - square * 3.0f + 1.0f) * first.degrees + (square * 3.0f - (cube + cube)) * last.degrees);
    }
    return static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(angle) * 65536.0 / 360.0));
}

std::int16_t ParseHandPoseJoint(HandParams &hand, const std::byte *&joint, const std::byte *&channels, std::int32_t frame) {
    const auto &record = *reinterpret_cast<const HandPoseModelJoint *>(joint);
    std::int16_t result = 0;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        std::int16_t value = static_cast<std::int16_t>(record.rotation[axis]);
        if (channels) {
            std::int32_t channel[2];
            std::memcpy(channel, channels, sizeof(channel));
            channels += sizeof(channel);
            if (channel[0] > 0)
                value = InterpolateHandPoseChannel(
                    channel[0], static_cast<const HandCubicKeyframe *>(HLib::GetPointerFromDynamicHeapOffsetBase(channel[1])), frame);
        }
        result = static_cast<std::int16_t>(static_cast<std::uint16_t>(value) - static_cast<std::uint16_t>(record.rotation[axis]));
        hand.targetJointAngles[record.id][axis] = result;
    }
    joint += sizeof(record);
    for (std::int32_t child = 0; child < record.childCount; ++child)
        result = ParseHandPoseJoint(hand, joint, channels, frame);
    return result;
}

std::int16_t ParseHandPoseFrame(HandParams &hand, std::int32_t frame) {
    const std::byte *joint = hand.poseModelData + 8;
    const std::byte *channels = hand.poseFrames;
    return ParseHandPoseJoint(hand, joint, channels, frame);
}

} // namespace model_component_detail

std::int16_t ConfigureHandPoseSequence(
    HandParams &hand, std::int16_t pose, std::int16_t first, std::int16_t last, std::int16_t start, std::uint8_t flags, float step, std::int16_t ticks) {
    hand.poseFlags = flags & static_cast<std::uint8_t>(~HandPoseInactive);
    HandPoseBank *bank = handPoseBanks.next;
    while (bank && (pose < bank->first || pose > bank->last))
        bank = bank->next;
    hand.poseData = bank ? static_cast<const std::byte *>(HLib::GetPointerFromDynamicHeapOffsetBase(bank->offsets[pose - bank->first + 1])) : nullptr;
    if (bank) {
        std::int32_t frameCount;
        std::memcpy(&frameCount, hand.poseData + 32, sizeof(frameCount));
        hand.poseFrameCount = frameCount / 2;
        std::memcpy(&hand.poseJointCount, hand.poseData + 36, sizeof(hand.poseJointCount));
        hand.poseFrames = hand.poseData + 40;
    } else {
        hand.poseFrameCount = 0;
        hand.poseJointCount = 0;
        hand.poseFrames = nullptr;
    }
    hand.firstPoseFrame = static_cast<std::int16_t>(first - 1);
    if (hand.firstPoseFrame < 0)
        hand.firstPoseFrame = 0;
    else if (hand.firstPoseFrame >= hand.poseFrameCount)
        hand.firstPoseFrame = static_cast<std::int16_t>(hand.poseFrameCount - 1);
    hand.lastPoseFrame = static_cast<std::int16_t>(last - 1);
    if (hand.lastPoseFrame < 0 || hand.lastPoseFrame >= hand.poseFrameCount)
        hand.lastPoseFrame = static_cast<std::int16_t>(hand.poseFrameCount - 1);
    if (hand.firstPoseFrame > hand.lastPoseFrame)
        hand.firstPoseFrame = hand.lastPoseFrame = 0;
    hand.currentPoseFrame = static_cast<std::int16_t>(start - 1);
    if (hand.currentPoseFrame < hand.firstPoseFrame || hand.currentPoseFrame > hand.lastPoseFrame)
        hand.currentPoseFrame = hand.poseFlags & HandPoseReverse ? hand.lastPoseFrame : hand.firstPoseFrame;
    hand.previousPoseFrame = hand.currentPoseFrame;
    hand.poseFrame = static_cast<float>(hand.currentPoseFrame);
    SetHandPoseStep(hand, step);
    hand.poseMode = 0;
    hand.poseTicks = ticks;
    std::int16_t result = ticks;
    if (ticks > 0) {
        hand.poseMode = 2;
        result = ParseHandPoseFrame(hand, 2 * hand.currentPoseFrame);
    }
    hand.secondaryPoseWork = nullptr;
    return result;
}

namespace model_component_detail {

// @verified - p
MT7Model *QueryHandModelCacheNullsub(const char *) {
    return nullptr;
}

std::uint32_t ReadHandMeshWord(std::span<const std::byte> bytes, std::size_t offset) {
    std::uint32_t value = 0;
    if (offset <= bytes.size() && bytes.size() - offset >= 4)
        std::memcpy(&value, bytes.data() + offset, 4);
    return value;
}

std::uint64_t GetNumHandModelChunks(std::span<const std::byte> bytes) {
    if (bytes.size() < 28)
        return 0;
    if (ReadHandMeshWord(bytes, 0) == SHEN_NAME(XB01)) {
        const std::uint64_t table = 4ULL * ReadHandMeshWord(bytes, 28);
        if (table + 40 > bytes.size())
            return 0;
        const std::uint32_t count = ReadHandMeshWord(bytes, static_cast<std::size_t>(table + 32));
        if (count > (bytes.size() - table - 40) / 12)
            return 0;
        std::uint64_t size = table + 28 + 8ULL * count + 8;
        for (std::uint32_t index = 0; index < count; ++index)
            size += ReadHandMeshWord(bytes, static_cast<std::size_t>(table + 40 + 12ULL * index));
        return (size + 8) >> 2;
    }
    if (static_cast<std::int32_t>(ReadHandMeshWord(bytes, 0)) < 0 || !ReadHandMeshWord(bytes, 24))
        return 0;
    std::uint64_t cursor = 24;
    do {
        if (cursor + 88 > bytes.size())
            return 0;
        cursor += ReadHandMeshWord(bytes, static_cast<std::size_t>(cursor + 84)) + 88ULL;
    } while (ReadHandMeshWord(bytes, static_cast<std::size_t>(cursor)));
    return (cursor + 8) >> 3;
}

// ReadModelFormat7's reader for a file-loaded hand. The hand's reader and file fields own it until ReleaseHandReader.
MT7Model *ReadOwnedHandModel(const char *directory, const char *filename, shen_name token, std::byte *&file) {
    std::size_t size = file ? HLib::GetTaskDataSize(file) : 0;
    if (!file) {
        std::uint32_t rawSize = 0;
        file = static_cast<std::byte *>(LoadLegacySceneFile(directory, filename, token, &rawSize));
        if (!file)
            return nullptr;
        size = rawSize;
    }
    char path[264];
    std::snprintf(path, sizeof(path), "%s/%s", directory, filename);
    MT7Model *const model = new (std::nothrow) MT7Model{};
    if (!model)
        return nullptr;
    if (!ReadModelFormat7({file, size}, path, *model)) {
        ReleaseModelFormat7(*model);
        delete model;
        return nullptr;
    }
    BindMT7WritableRuntimeMeshes(*model, {file, size});
    return model;
}

// mtMarkFree for a file-loaded hand reader: the parsed model, then its file and the reader itself.
void ReleaseHandReader(MT7Model *reader, std::byte *file) {
    ReleaseModelFormat7(*reader);
    delete reader;
    HLib::FreeTaskData(file);
}

std::uint8_t AllocateHandModels(Character *character, HandParams &hand) {
    HandModelResource *const resource = GetHandModelResource(hand);
    if (!resource)
        return 2;
    PakGlobalData *package = GetPakTaskDataField() ? GetPakLoadTaskDataPointer() : nullptr;
    std::uint8_t result = 2;
    if (!hand.primaryNode && !hand.secondaryNode) {
        hand.meshWordCount = 0;
        hand.meshBackup = nullptr;
        hand.meshCopy = nullptr;
        if (hand.selector == 3 || hand.selector == 4) {
            resource->reader = static_cast<MT7Model *>(character->model);
        } else {
            char directory[32], filename[16];
            std::snprintf(directory, sizeof(directory), "scene/%02d/model/hand", g_actNumber);
            std::strcpy(filename, hand.filename);
            MT7Model *reader = QueryHandModelCacheNullsub(filename);
            if (!reader) {
                package = GetNextReadyModelPackage(nullptr);
                while (package) {
                    if (FileLoader(package, filename, SHEN_NAME(CHRM)))
                        reader = LoadGameModel(package, filename);
                    if (reader)
                        break;
                    package = GetNextReadyModelPackage(package);
                }
            }
            if (!reader) {
                if (hand.externalDrawControl) {
                    const std::size_t side = (hand.selector - 1) & 1U;
                    if (defaultHandFiles[side]) {
                        hand.primaryFile = static_cast<std::byte *>(HLib::AllocateTaskData(defaultHandFileSizes[side], DEFAULT_HAND_MODEL_MEMORY));
                        if (hand.primaryFile)
                            std::memcpy(hand.primaryFile, defaultHandFiles[side], defaultHandFileSizes[side]);
                    }
                }
                reader = ReadOwnedHandModel(directory, filename, DEFAULT_HAND_MODEL_MEMORY, hand.primaryFile);
            } else {
                hand.primaryFile = nullptr;
            }
            hand.primaryReader = reader;
            resource->reader = reader;
            if (hand.secondaryEnabled) {
                filename[4] = 'X';
                MT7Model *secondary = QueryHandModelCacheNullsub(filename);
                if (!secondary && package)
                    secondary = LoadGameModel(package, filename);
                if (secondary)
                    hand.secondaryFile = nullptr;
                else
                    secondary = ReadOwnedHandModel(directory, filename, HAND_MODEL_MEMORY, hand.secondaryFile);
                hand.secondaryReader = secondary;
                hand.secondaryNode = secondary && !secondary->nodes.empty() ? &secondary->nodes[0] : nullptr;
            }
        }
        MT7Model *const reader = resource->reader;
        if (reader && !reader->nodes.empty())
            hand.primaryNode = GetChildObjectByIndex(*reader, &reader->nodes[0], hand.selector == 1 || hand.selector == 3 ? 69 : 68);
        if (hand.primaryNode && hand.primaryNode->xb01Offset && reader) {
            const std::size_t offset = hand.primaryNode->xb01Offset;
            const std::uint64_t words = GetNumHandModelChunks(reader->source.subspan(offset));
            if (words <= 0x7fffU) {
                hand.meshWordCount = static_cast<std::int16_t>(words);
                hand.meshBackup = reinterpret_cast<const std::uint32_t *>(reader->source.data() + offset);
                hand.meshCopy = static_cast<std::uint32_t *>(HLib::AllocateTaskData(4ULL * words, HAND_MESH_MEMORY));
                if (hand.meshBackup && hand.meshCopy)
                    result = 1;
            }
        }
        if (reader && hand.primaryNode) {
            BindHandPoseResource(*resource, hand.selector);
            const std::size_t offset = hand.primaryNode->sourceOffset + 64;
            const std::uint32_t signature = ReadHandMeshWord(reader->source, offset);
            if (signature == SHEN_NAME(MDH7) || signature == SHEN_NAME(MDHX))
                hand.poseModelData = reader->source.data() + offset;
        }
    }
    if (hand.meshWordCount > 0 && hand.meshBackup && hand.meshCopy) {
        const std::size_t size = 4U * static_cast<std::uint16_t>(hand.meshWordCount);


        const std::span<const std::byte> source = resource->reader->source;
        const std::size_t available = static_cast<std::size_t>(source.data() + source.size() - reinterpret_cast<const std::byte *>(hand.meshBackup));
        std::memcpy(hand.meshCopy, hand.meshBackup, size < available ? size : available);
        if (size > available)
            std::memset(reinterpret_cast<std::byte *>(hand.meshCopy) + available, 0, size - available);
        const std::span<const std::byte> commands{reinterpret_cast<const std::byte *>(hand.meshCopy), size};

        const LegacyDrawResources &sourceResources =
            hand.primaryNode->borrowedMesh ? hand.primaryNode->borrowedMesh->drawResources : hand.primaryNode->drawResources;
        if (!resource->meshResources.prepared) {
            PrepareLegacyDrawResources(commands, 0, resource->meshResources);
            resource->meshResources.textureBindings = sourceResources.textureBindings;
        } else {
            RefreshLegacyDrawResources(commands, 0, sourceResources, resource->meshResources);
        }
        resource->meshCommand = {.nodeSourceOffset = hand.primaryNode->sourceOffset, .bytes = commands, .xb01Offset = 0, .resources = &resource->meshResources};
    }
    if (hand.primaryNode || hand.secondaryNode || hand.externalDrawControl) {
        if (hand.regionState)
            hand.previousRegionState = hand.regionState;
        hand.regionState = hand.previousRegionState = 0;
        hand.modelAlpha = 0.0f;
    }
    if (hand.primaryNode)
        hand.primaryNode->rotation = {};
    hand.previousRegionState = 0;
    hand.modelAlpha = 0.0f;
    return result;
}

std::uint8_t RetireHandModels(Character *character, HandParams &hand) {
    if (!character)
        return 0;
    if (hand.primaryNode || hand.secondaryNode || hand.externalDrawControl) {
        if (hand.regionState)
            hand.previousRegionState = hand.regionState;
        hand.regionState = hand.previousRegionState = 0;
        hand.modelAlpha = 0.0f;
    }
    if (hand.primaryNode)
        hand.primaryNode->rotation = {};
    hand.previousRegionState = 0;
    hand.modelAlpha = 0.0f;
    if (!std::strncmp(hand.filename, "RYO_T", 5))
        return 1;
    std::uint8_t result = 2;
    HandModelResource *const resource = GetHandModelResource(hand);
    if (hand.primaryNode) {
        result = 1;
        if (hand.meshCopy && hand.meshBackup) {
            if (resource)
                ReleaseLegacyDrawResources(resource->meshResources);
            hand.primaryNode->commandOverride = nullptr;
            HLib::FreeTaskData(hand.meshCopy);
            hand.meshCopy = nullptr;
            hand.meshBackup = nullptr;
        }
        if (static_cast<std::uint8_t>(hand.selector - 1) <= 1U && hand.primaryFile && hand.primaryReader)
            ReleaseHandReader(hand.primaryReader, hand.primaryFile);
    }
    hand.primaryNode = nullptr;
    hand.primaryReader = nullptr;
    hand.primaryFile = nullptr;
    if (hand.secondaryNode && hand.secondaryFile && hand.secondaryReader)
        ReleaseHandReader(hand.secondaryReader, hand.secondaryFile);
    hand.secondaryNode = nullptr;
    hand.secondaryReader = nullptr;
    hand.secondaryFile = nullptr;
    if (resource) {
        resource->reader = nullptr;
        resource->joints.clear();
        resource->poseVertices.clear();
        resource->influences.clear();
        resource->mappings.clear();
    }
    return result;
}

} // namespace model_component_detail

// @verified - p
std::uint64_t SetLowQualityHands(Character *character) {
    std::uint8_t result = 0;
    HandParams *const left = static_cast<HandParams *>(FindModelComponent(character, kLeftHandToken));
    if (left)
        result = AllocateHandModels(character, *left);
    HandParams *const right = static_cast<HandParams *>(FindModelComponent(character, kRightHandToken));
    return right ? AllocateHandModels(character, *right) : result;
}

// @verified - p
std::uint8_t hndlSetFlags(Character *character) {
    std::uint8_t result = 0;
    HandParams *const left = static_cast<HandParams *>(FindModelComponent(character, kLeftHandToken));
    if (left)
        result = RetireHandModels(character, *left);
    HandParams *const right = static_cast<HandParams *>(FindModelComponent(character, kRightHandToken));
    return right ? RetireHandModels(character, *right) : result;
}

// @verified - p
CharacterCoreCallback assignBlurFigurePoseCallback(Character *character, CharacterCoreCallback callback) {
    ReflectorBoardParams *const reflector = FindReflectorBoard(character);
    if (!reflector)
        return nullptr;
    CharacterCoreCallback const previous = reflector->coreAssignment;
    reflector->coreAssignment = callback;
    return previous;
}

// @verified - p
bool MatchAreaNamePattern(shen_name area, shen_name pattern) {
    std::uint32_t mask = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8)
        if (((pattern.name >> shift) & 0xffU) != static_cast<std::uint32_t>('?'))
            mask |= 0xffU << shift;
    return (area.name & mask) == (pattern.name & mask);
}

namespace model_component_detail {

void StageHandJoint(
    HandParams &hand, const HandPoseModelJoint *&cursor, const std::int32_t *&channels, HandModelResource *resource, std::int32_t frame, bool publishMatrices) {
    const HandPoseModelJoint &joint = *cursor++;
    const bool positiveXY = hand.selector == 2 || hand.selector == 4;
    std::uint16_t rotation[3];
    for (std::size_t axis = 0; axis < 3; ++axis) {
        std::uint16_t value = static_cast<std::uint16_t>(joint.rotation[axis]);
        if (joint.id != 71) {
            if (hand.poseMode) {
                value += axis < 2 && !positiveXY ? -hand.currentJointAngles[joint.id][axis] : hand.currentJointAngles[joint.id][axis];
            } else {
                if (channels) {
                    if (axis < 2 && !positiveXY)
                        value = -value;
                    if (channels[0] > 0)
                        value = InterpolateHandPoseChannel(
                            channels[0], static_cast<const HandCubicKeyframe *>(HLib::GetPointerFromDynamicHeapOffsetBase(channels[1])), frame);
                    channels += 2;
                    if (axis < 2 && !positiveXY)
                        value = -value;
                }
                const auto base = static_cast<std::uint16_t>(joint.rotation[axis]);
                hand.currentJointAngles[joint.id][axis] = static_cast<std::int16_t>(axis < 2 && !positiveXY ? base - value : value - base);
            }
        }
        rotation[axis] = value;
    }
    Matrix4x4::preTranslate(joint.position.x, joint.position.y, joint.position.z);
    Matrix4x4::RotateAroundZ_Wrapper(rotation[2]);
    Matrix4x4::RotateAroundY_Wrapper(rotation[1]);
    Matrix4x4::RotateAroundX_Wrapper(rotation[0]);
    if (joint.id == 71) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-hand.wristAngles[0]));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-hand.wristAngles[1]));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-hand.wristAngles[2]));
    }
    hand.jointPositions[joint.id] = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    if (publishMatrices) {
        if (handJointMatrices)
            handJointMatrices[joint.id] = workTransformMatrix;
        if (resource)
            resource->jointMatrices[joint.id] = workTransformMatrix;
        const Matrix4x4 absolute = workTransformMatrix;
        Matrix4x4::MultiplyWithPrePassMatrix(&joint.inverseBind);
        if (handPoseMatrices)
            handPoseMatrices[joint.id] = workTransformMatrix;
        if (resource)
            resource->skinMatrices[joint.id] = workTransformMatrix;
        Matrix4x4::SetActiveTransform(&absolute);
    }
    for (std::int32_t child = 0; child < joint.childCount; ++child) {
        Matrix4x4_Push();
        StageHandJoint(hand, cursor, channels, resource, frame, publishMatrices);
        Matrix4x4::Pop(1);
    }
}

RuntimeHandStaging BuildHandPoseCpuStaging(HandParams &hand) {
    std::int32_t descriptorOffset, count;
    std::memcpy(&descriptorOffset, hand.poseModelData + 4, sizeof(descriptorOffset));

    auto *const descriptor = const_cast<std::byte *>(hand.poseModelData) + descriptorOffset;
    std::memcpy(&count, descriptor + 12, sizeof(count));
    auto *const vertices = reinterpret_cast<RuntimeHandVertex *>(descriptor + 16);
    RuntimeHandStaging result{vertices, nullptr, 0};
    Matrix4x4_Push();
    for (std::int32_t index = 0; index < count; ++index) {
        vertices[index].deformedPosition = {};
        vertices[index].deformedNormal = {};
    }
    if (count > 0) {
        auto *const end = reinterpret_cast<std::int32_t *>(vertices + count);
        const std::int32_t groups = end[2];
        auto *cursor = end + 3;
        for (std::int32_t group = 0; group < groups; ++group) {
            const std::int32_t joint = cursor[0], influences = cursor[1];
            cursor += 2;
            if (influences > 0)
                Matrix4x4::SetActiveTransform(handPoseMatrices + joint);
            for (std::int32_t influence = 0; influence < influences; ++influence) {
                auto &vertex = vertices[cursor[0]];
                float weight;
                std::memcpy(&weight, cursor + 1, sizeof(weight));
                const Vector3 position = TransformHandVector(vertex.position, workTransformMatrix, true);
                const Vector3 normal = TransformHandVector(vertex.normal, workTransformMatrix, false);
                for (std::size_t axis = 0; axis < 3; ++axis) {
                    vertex.deformedPosition[axis] += position[axis] * weight;
                    vertex.deformedNormal[axis] += normal[axis] * weight;
                }
                cursor += 2;
            }
        }
        if (groups > 0) {
            std::int32_t normalized = cursor[1];
            if (normalized > 0 && !(normalized & 3))
                ++normalized;
            for (std::int32_t index = 0; index < normalized; ++index) {
                Vector3 &normal = vertices[index].deformedNormal;
                const float square = (normal.x * normal.x + normal.y * normal.y) + normal.z * normal.z;
                const float reciprocal = static_cast<float>(1.0 / (index + 1 == normalized ? square : std::sqrt(square)));
                normal.x *= reciprocal;
                normal.y *= reciprocal;
                normal.z *= reciprocal;
            }
            for (std::int32_t index = 0; index < normalized + cursor[2]; ++index) {
                std::uint32_t bits;
                std::memcpy(&bits, &vertices[index].deformedPosition.x, sizeof(bits));
                bits |= 1U;
                std::memcpy(&vertices[index].deformedPosition.x, &bits, sizeof(bits));
            }
            result.mappingCount = cursor[4];
            result.mappings = cursor + 5;
        }
    }
    Matrix4x4::Pop(1);
    return result;
}

} // namespace model_component_detail

void StageHandPose(HandParams &hand, std::int32_t frame) {
    if (!hand.poseModelData)
        return;
    HandModelResource *const owned = GetHandModelResource(hand);
    const auto *cursor = reinterpret_cast<const HandPoseModelJoint *>(hand.poseModelData + 8);
    const auto *channels = reinterpret_cast<const std::int32_t *>(hand.poseFrames);
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    StageHandJoint(hand, cursor, channels, owned, frame, true);
    Matrix4x4::Pop(1);
    const RuntimeHandStaging staging = BuildHandPoseCpuStaging(hand);
    if (!owned) {
        if (staging.mappingCount > 0) {
            auto *const mapped = static_cast<std::byte *>(CopyForDirectX(hand.meshCopy, 0, 0, 0, 1));
            std::byte *const destination = IsModelXB01(hand.meshCopy) ? mapped : reinterpret_cast<std::byte *>(hand.meshCopy);
            auto *cursor = staging.mappings;
            for (std::int32_t index = 0; index < staging.mappingCount; ++index) {
                const auto &vertex = staging.vertices[cursor[0]];
                std::memcpy(destination + cursor[1], &vertex.deformedPosition, 24);
                cursor += 2;
            }
            CopyBufferToDirectX_Wrapper(hand.meshCopy, 0);
        }
        return;
    }
    HandModelResource &resource = *owned;
    for (std::size_t index = 0; index < resource.poseVertices.size(); ++index) {
        resource.poseVertices[index].deformedPosition = staging.vertices[index].deformedPosition;
        resource.poseVertices[index].deformedNormal = staging.vertices[index].deformedNormal;
    }
    MT7Node &node = resource.reader->nodes[resource.poseNode];
    const std::uint64_t required = static_cast<std::uint64_t>(node.meshVertexStride) * node.meshVertexCount;
    for (const LegacyVertexBufferBinding &binding : resource.meshResources.vertexBuffers) {
        const auto size = d3t::GetSizeForModelBuffer(binding.handle);
        if (size < 24 || required > static_cast<std::uint32_t>(size))
            continue;
        auto *destination = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, binding.handle, 2, 0));
        if (!destination)
            continue;
        for (const auto &mapping : resource.mappings) {
            if (mapping.destinationByteOffset > static_cast<std::uint32_t>(size) - 24U)
                continue;
            const auto &vertex = resource.poseVertices[mapping.sourceVertex];
            std::memcpy(destination + mapping.destinationByteOffset, &vertex.deformedPosition, sizeof(Vector3));
            std::memcpy(destination + mapping.destinationByteOffset + 12, &vertex.deformedNormal, sizeof(Vector3));
        }
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, binding.handle);
        break;
    }
    resource.poseDirty = false;
}

namespace model_component_detail {


bool CalculateHandScaleRatios(HandParams &hand) {
    for (float &value : hand.retainedUnitValues)
        value = 1.0f;
    const auto *const measurements = static_cast<const MhndComponent *>(FindModelComponent(hand.owner, kHandPoseToken));
    Character *const ryo = FindCharacterByScenarioName(SHEN_NAME(RYO_));
    const auto *const reference = ryo ? static_cast<const MhndComponent *>(FindModelComponent(ryo, kHandPoseToken)) : nullptr;
    if (!measurements || !reference)
        return false;
    const std::size_t side = hand.selector == 2 || hand.selector == 4 ? 0 : 1;
    for (std::size_t index = 0; index < 6; ++index) {
        const float value = measurements->hands[side].measurements[index];
        const float base = reference->hands[side].measurements[index];
        if (value > 0.0f && base > 0.0f)
            hand.retainedUnitValues[index] = value / base;
    }
    return true;
}

std::int32_t HandAttachmentKind(std::uint8_t selector) {
    constexpr std::int32_t kinds[]{0, -66, -65, 69, 68};
    return selector < std::size(kinds) ? kinds[selector] : 0;
}



void FinalizeHandMatrices(const d3t::HandMatrixAttachment &hand) {
    if (!hand.enabled || !hand.ready || !hand.node)
        return;
    for (std::uint32_t index = 0; index < hand.matrixCount; ++index) {
        if (index > 20 || !hand.cacheHandle)
            continue;
        std::uint32_t hash;

        std::memcpy(&hash, reinterpret_cast<const std::byte *>(&hand) + 40 + index * 4, sizeof(hash));
        if (!hash)
            continue;
        const Matrix4x4 &source = hand.matrices[hand.matrixIndices[index]];
        Matrix4x4 transposed;
        for (std::size_t row = 0; row < 4; ++row)
            for (std::size_t column = 0; column < 4; ++column)
                transposed.rows[row][column] = source.rows[column][row];
        d3t::SetModelCacheTransform((static_cast<std::uint64_t>(hash) << 32) | hand.cacheHandle, transposed);
    }
}

void FinalizeMatchingHandAttachments(d3t::ModelRuntimeState &state, std::int32_t kind, std::int32_t childrenOnly) {
    for (const auto &attachment : state.hands)
        if (attachment.node && attachment.nodeKind == kind && (!childrenOnly || attachment.hasChildren))
            FinalizeHandMatrices(attachment);
}

void GenerateHandJointRelativeMatrices(
    const HandPoseModelJoint *&cursor, const Matrix4x4 *parent, const float *scales, std::int32_t bodyKind, HandModelResource *resource) {
    const auto &joint = *cursor++;
    const Matrix4x4 *const absolute = handJointMatrices + joint.id;
    if (joint.id <= 19) {
        Matrix4x4_Push(absolute);
        Matrix4x4_Push(parent);
        Matrix4x4 inverse;
        if (Matrix4x4::invertMatrix(&workTransformMatrix, &inverse))
            Matrix4x4::SetActiveTransform(&inverse);
        Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.top.stack - 1);
        constexpr std::int32_t scalePairs[15][2]{
            {29, 1}, {30, 1}, {31, 1}, {45, 3}, {46, 5}, {47, 5},
            {41, 3}, {42, 5}, {43, 5}, {37, 3}, {38, 5}, {39, 5},
            {33, 2}, {34, 4}, {35, 4}};
        float scale = scales[0];
        if (bodyKind > 5)
            for (const auto &pair : scalePairs)
                if (pair[0] == joint.id) {
                    scale = scales[pair[1]];
                    break;
                }
        if (scale <= 0.0f)
            scale = 1.0f;
        workTransformMatrix.rows[3].x *= scale;
        workTransformMatrix.rows[3].y *= scale;
        workTransformMatrix.rows[3].z *= scale;
        Matrix4x4::GetTransformMatrix(handRelativeMatrices + joint.id);
        if (resource)
            resource->relativeMatrices[joint.id] = workTransformMatrix;
        Matrix4x4::Pop(2);
        parent = absolute;
    }
    for (std::int32_t child = 0; child < joint.childCount; ++child)
        GenerateHandJointRelativeMatrices(cursor, parent, scales, bodyKind, resource);
}


void GenerateHandRelativeMatrices(HandParams &hand, const Matrix4x4 *parent, std::int32_t bodyKind) {
    if (!hand.poseModelData)
        return;
    if (!hand.calculationEnabled)
        hand.calculationEnabled = CalculateHandScaleRatios(hand);
    if (bodyKind < 0)
        bodyKind = static_cast<std::int32_t>(GetModelBodyKind(hand.owner->model));
    constexpr float unitScale[6]{1, 1, 1, 1, 1, 1};
    const float *const scales = hand.calculationEnabled ? hand.retainedUnitValues : unitScale;
    const auto *cursor = reinterpret_cast<const HandPoseModelJoint *>(hand.poseModelData + 8);
    GenerateHandJointRelativeMatrices(cursor, parent, scales, bodyKind, GetHandModelResource(hand));
}

void RotateHandPrimaryRelativeMatrix(HandParams &hand) {
    Matrix4x4_PushActiveTransform();
    const Vector3i rotation = hand.primaryNode ? hand.primaryNode->rotation : Vector3i{};
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4_Push(handRelativeMatrices);
    Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.top.stack - 1);
    Matrix4x4::GetTransformMatrix(handRelativeMatrices);
    if (auto *const resource = GetHandModelResource(hand))
        resource->relativeMatrices[0] = workTransformMatrix;
    Matrix4x4::Pop(2);
}

void ApplyHandNodeTransform(const MT7Node &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
    Matrix4x4::Scale(node.scale.x, node.scale.y, node.scale.z);
}


void ApplyHandBodyTransform(Character &character, const HandParams &hand) {
    if (hand.selector == 1 || hand.selector == 2) {
        Matrix4x4::SetActiveTransform(charaGetBoneTransform(FindMotionWork(&character), hand.selector == 1 ? 21 : 14));
        if (hand.primaryNode)
            ApplyHandNodeTransform(*hand.primaryNode);
        return;
    }
    Matrix4x4::SetPrePassToTempTransform();
    const Vector3 position = GetCharacterWorldPosition(&character);
    const Vector3i rotation = GetCharacterWorldRotation(&character);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    const auto *const model = static_cast<const MT7Model *>(character.model);
    if (!model || model->nodes.empty())
        return;
    const std::uint16_t target = hand.selector == 3 ? 0xffbeU : 0xffbfU;
    for (const MT7Node *node = model->nodes.data(); node;) {
        ApplyHandNodeTransform(*node);
        if (static_cast<std::uint16_t>(node->id) == target)
            break;
        node = node->firstChild < 0 ? nullptr : &model->nodes[static_cast<std::size_t>(node->firstChild)];
    }
}


void ComputeHandTransformedProbes(const HandParams &hand) {
    Vector3 world[72];
    for (std::size_t joint = 0; joint < 72; ++joint)
        Matrix4x4::TransformPoint(hand.jointPositions[joint], world[joint]);
    constexpr struct {
        std::uint8_t first, second;
        float firstWeight, secondWeight, radius;
    } probes[10]{{69, 70, 0.29660001f, 0.70340002f, 0.0057000001f}, {69, 70, 1.0f, 0.0f, 0.0106f}, {59, 60, 0.25529999f, 0.74470001f, 0.0093f},
        {59, 60, 1.0f, 0.0f, 0.0114f}, {45, 46, 0.28549999f, 0.71450001f, 0.0091000004f}, {45, 46, 1.0f, 0.0f, 0.0121f}, {37, 38, 0.0f, 1.0f, 0.0097000003f},
        {37, 38, 0.80000001f, 0.19999999f, 0.0106f}, {29, 30, 0.3073f, 0.69270003f, 0.0098999999f}, {29, 30, 0.84560001f, 0.15439999f, 0.011f}};
    HandGripProbe *const output = static_cast<HandGripProbe *>(handProbeWork);
    if (!output)
        return;
    for (std::size_t index = 0; index < 10; ++index) {
        const auto &probe = probes[index];
        const Vector3 &first = world[probe.first];
        const Vector3 &second = world[probe.second];
        output[index] = {{first.x * probe.firstWeight + second.x * probe.secondWeight, first.y * probe.firstWeight + second.y * probe.secondWeight,
                             first.z * probe.firstWeight + second.z * probe.secondWeight},
            probe.radius};
    }
}

// Adapt HNDP_1 to the existing legacy absolute-override contract. Authored
// non-hand parents and live body overrides retain their original ownership.
void ApplyHandRenderParent(const MT7Model &model, std::size_t index) {
    const MT7Node &node = model.nodes[index];
    if (const Matrix4x4 *const transform = GetMT7NodeTransformOverride(node)) {
        Matrix4x4::SetActiveTransform(transform);
    } else {
        if (node.parent >= 0)
            ApplyHandRenderParent(model, static_cast<std::size_t>(node.parent));
        ApplyHandNodeTransform(node);
    }
}

void ComposeHandRenderMatrices(HandModelResource &resource) {
    if (resource.joints.empty())
        return;
    Matrix4x4_Push();
    const Matrix4x4 identity{{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
                             {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};
    Matrix4x4::SetActiveTransform(&identity);
    ApplyHandRenderParent(*resource.reader, resource.poseNode);
    Matrix4x4 parent;
    Matrix4x4::GetTransformMatrix(&parent);
    for (const HandPoseJoint &joint : resource.joints) {
        if (joint.id > 19)
            continue;
        const Matrix4x4 *const inherited =
            joint.relativeParent < 0 ? &parent : &resource.renderMatrices[resource.joints[static_cast<std::size_t>(joint.relativeParent)].id];
        Matrix4x4::SetActiveTransform(inherited);
        Matrix4x4::MultiplyWithPrePassMatrix(&resource.relativeMatrices[joint.id]);
        Matrix4x4::GetTransformMatrix(&resource.renderMatrices[joint.id]);
    }
    Matrix4x4::Pop(1);
}

void BindHandRenderMatrices(HandModelResource &resource, bool bind) {
    if (bind)
        ComposeHandRenderMatrices(resource);
    for (HandPoseJoint &joint : resource.joints) {
        if (joint.id > 19 || !joint.renderNode)
            continue;
        if (bind) {
            joint.previousRenderTransform = GetMT7NodeTransformOverride(*joint.renderNode);
            SetMT7NodeTransformOverride(*joint.renderNode, &resource.renderMatrices[joint.id]);
        } else {
            SetMT7NodeTransformOverride(*joint.renderNode, joint.previousRenderTransform);
        }
    }
}

void ReleaseHandModel(HandModelResource &resource) {
    resource.reader = nullptr;
    resource.joints.clear();
    resource.poseVertices.clear();
    resource.influences.clear();
    resource.mappings.clear();
}

std::string DeriveHandFilename(const char *modelImage, bool left) {
    std::string_view image = modelImage ? modelImage : "";
    const std::size_t slash = image.find_last_of("/\\");
    if (slash != std::string_view::npos)
        image.remove_prefix(slash + 1);
    const std::size_t prefixLength = std::min<std::size_t>(3, image.size());
    std::string filename(image.substr(0, prefixLength));
    filename += left ? "_TL.MT7" : "_TR.MT7";
    return filename;
}

void CharacterHandDrawClosure(Character *character);

void CharacterHandDirectCore(Character *character);


HandParams *ResolveCharacterHandForDraw(std::uint16_t nodeId) {
    if (nodeId == 0xffbeU)
        return g_temporaryHNDL;
    return nodeId == 0xffbfU ? g_temporaryHNDR : nullptr;
}

// @verified - p
void CharacterHandDrawClosure(Character *character) {
    g_temporaryHNDL = static_cast<HandParams *>(FindModelComponent(character, kLeftHandToken));
    g_temporaryHNDR = static_cast<HandParams *>(FindModelComponent(character, kRightHandToken));
    HandParams *const hand = g_temporaryHNDR ? g_temporaryHNDR : g_temporaryHNDL;
    if (character->model && hand && hand->previousCore)
        hand->previousCore(character);
    g_temporaryHNDL = g_temporaryHNDR = nullptr;
}

void CharacterHandDirectCore(Character *character) {
    HandParams *hand = static_cast<HandParams *>(FindModelComponent(character, kLeftHandToken));
    if (!hand)
        hand = static_cast<HandParams *>(FindModelComponent(character, kRightHandToken));
    if (!hand)
        return;
    HandModelResource *const resource = GetHandModelResource(*hand);
    const bool posed = resource && (hand->regionState || hand->previousRegionState);
    const bool legacyPose = posed && !d3t::GetModelRuntimeState(GetMT7ReaderData(static_cast<const MT7Model *>(character->model)));
    if (hand->primaryNode)
        hand->primaryNode->commandOverride = legacyPose ? &resource->meshCommand : nullptr;
    if (legacyPose)
        BindHandRenderMatrices(*resource, true);
    if (hand->previousCore)
        hand->previousCore(character);
    if (legacyPose)
        BindHandRenderMatrices(*resource, false);
}


// @verified - p
void UpdateHand(Character *character, void *payload) {
    HandParams &hand = *static_cast<HandParams *>(payload);
    if (hand.immediatePoseResult == 0) {
        MT7Model *const source = GetImageModuleSource(character);
        if (source && !source->nodes.empty() && d3t::IsObjectModelRuntime(source->nodes.data()))
            hand.poseRefreshRequested = 1;
    }
    if ((hand.regionState <= 0 && hand.previousRegionState <= 0) || !hand.poseModelData) {
        hand.poseFlags |= 8U;
        return;
    }
    hand.previousPoseFrame = hand.currentPoseFrame;
    bool wristChanged = false;
    if (hand.primaryNode) {
        const std::int16_t *const angles = g_characterHandWristAngles;
        const std::size_t wrist = hand.selector == 1 || hand.selector == 3 ? 2 : 0;
        const std::int16_t current[3] = {static_cast<std::int16_t>(hand.primaryNode->rotation.x),
            static_cast<std::int16_t>(static_cast<std::int16_t>(hand.primaryNode->rotation.y) + angles[wrist]),
            static_cast<std::int16_t>(static_cast<std::int16_t>(hand.primaryNode->rotation.z) + angles[wrist + 1])};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            wristChanged = wristChanged || hand.wristAngles[axis] != current[axis];
            hand.wristAngles[axis] = current[axis];
        }
    }
    if (gripDisabled) {
        hand.poseFlags |= 8U;
        return;
    }
    if (wristChanged || !hand.poseMode || hand.poseTicks >= 1 || hand.poseRefreshRequested) {
        hand.poseRefreshRequested = 0;
        StageHandPose(hand, hand.currentPoseFrame * 2);
        hand.immediatePoseResult = static_cast<std::uint8_t>(StageImmediateHandPose(&hand, handJointMatrices));
        Matrix4x4_Push();
        ApplyHandBodyTransform(*character, hand);
        ComputeHandTransformedProbes(hand);
        Matrix4x4::Pop(1);
        if (hand.secondaryPoseWork && hand.poseMode)
            UpdateHandGripBounds(hand);
    }
    hand.poseFlags &= ~8U;
    if (!hand.poseMode) {
        if (!(hand.poseFlags & 4U)) {
            hand.poseFrame = hand.poseFlags & 2U ? hand.poseFrame - hand.poseStep : hand.poseStep + hand.poseFrame;
            const double frame = static_cast<double>(hand.poseFrame) + 0.5;
            const std::int32_t rounded =
                frame >= -2147483648.0 && frame < 2147483648.0 ? static_cast<std::int32_t>(frame) : std::numeric_limits<std::int32_t>::min();
            hand.currentPoseFrame = static_cast<std::int16_t>(rounded);
            if (hand.poseFlags & 1U) {
                const std::int16_t duration = static_cast<std::int16_t>(hand.lastPoseFrame - hand.firstPoseFrame + 1);
                while (hand.currentPoseFrame < hand.firstPoseFrame)
                    hand.currentPoseFrame = static_cast<std::int16_t>(hand.currentPoseFrame + duration);
                while (hand.currentPoseFrame > hand.lastPoseFrame)
                    hand.currentPoseFrame = static_cast<std::int16_t>(hand.currentPoseFrame - duration);
            } else {
                if (hand.currentPoseFrame < hand.firstPoseFrame)
                    hand.currentPoseFrame = hand.firstPoseFrame;
                if (hand.currentPoseFrame > hand.lastPoseFrame)
                    hand.currentPoseFrame = hand.lastPoseFrame;
            }
            if (hand.currentPoseFrame != static_cast<std::int16_t>(rounded))
                hand.poseFrame = static_cast<float>(hand.currentPoseFrame);
        }
        if (hand.poseFlags & 2U) {
            if (hand.currentPoseFrame <= hand.firstPoseFrame || hand.currentPoseFrame > hand.previousPoseFrame)
                hand.poseFlags |= 8U;
        } else if (hand.currentPoseFrame >= hand.lastPoseFrame || hand.currentPoseFrame < hand.previousPoseFrame) {
            hand.poseFlags |= 8U;
        }
    } else {
        if (hand.poseTicks > 0)
            AdvanceGripAngles(hand, hand.secondaryPoseWork != nullptr, 0);
        if (hand.poseMode && hand.poseTicks < 1)
            hand.poseFlags |= 8U;
    }
}



void CleanupHand(Character *character, void *payload) {
    if (!character)
        return;
    HandParams &hand = *static_cast<HandParams *>(payload);
    HandModelResource *const resource = GetHandModelResource(hand);
    if (auto *const state = d3t::GetModelRuntimeState(GetMT7ReaderData(hand.owner ? static_cast<const MT7Model *>(hand.owner->model) : nullptr))) {
        // Remove the borrowed matrix owner before either hand resource is retired.
        state->hands[0] = state->hands[1] = {};
        state->nodeCount = state->nodeTableAvailable = state->nodesAssigned = state->recordCount = 0;
        for (auto &node : state->nodes)
            node = {};
    }
    if (resource)
        ReleaseLegacyDrawResources(resource->meshResources);
    if (hand.primaryNode && hand.meshCopy && hand.meshBackup) {
        HLib::FreeTaskData(hand.meshCopy);
        hand.primaryNode->commandOverride = nullptr;
        hand.meshBackup = nullptr;
        hand.meshCopy = nullptr;
    }
    if (hand.selector == 1 || hand.selector == 2) {

        if (hand.primaryFile && hand.primaryReader)
            ReleaseHandReader(hand.primaryReader, hand.primaryFile);
        if (hand.secondaryFile && hand.secondaryReader)
            ReleaseHandReader(hand.secondaryReader, hand.secondaryFile);
    }
    hand.primaryNode = nullptr;
    hand.primaryReader = nullptr;
    hand.primaryFile = nullptr;
    hand.secondaryNode = nullptr;
    hand.secondaryReader = nullptr;
    hand.secondaryFile = nullptr;
    hand.owner = nullptr;
    if (hand.extendedPoseWork) {
        HLib::FreeTaskData(hand.extendedPoseWork);
        hand.extendedPoseWork = nullptr;
    }
    hand.poseModelData = nullptr;
    if (resource) {
        ReleaseHandModel(*resource);
        if (resource->state) {
            HandModelResource **link = &resource->state->handResources[(hand.selector - 1) & 1];
            while (*link && *link != resource)
                link = &(*link)->next;
            if (*link)
                *link = resource->next;
        }
        delete resource;
    }
}

} // namespace model_component_detail

void StageHandGripProbes(HandParams &hand) {
    hand.previousPoseFrame = hand.currentPoseFrame;
    const HandPoseModelJoint *cursor = reinterpret_cast<const HandPoseModelJoint *>(hand.poseModelData + 8);
    const std::int32_t *channels = reinterpret_cast<const std::int32_t *>(hand.poseFrames);
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    StageHandJoint(hand, cursor, channels, nullptr, 2 * hand.currentPoseFrame, false);
    Matrix4x4::Pop(1);
    ComputeHandTransformedProbes(hand);
}

std::uint64_t PublishHandModelAttachment(HandParams &hand, Character *character, MT7Model *model, std::int32_t childrenOnly, std::uint32_t flags) {
    UpdateModelFrame(static_cast<MT7Model *>(character->model), character, 0, flags | 4U);
    auto *const state = d3t::GetModelRuntimeState(GetMT7ReaderData(model));
    if (!state || !state->nodeTableAvailable || !state->nodesAssigned)
        return 0;
    constexpr std::int32_t kinds[]{-66, -65, 69, 68};
    const auto selector = static_cast<std::int8_t>(hand.selector);
    const auto kind = selector >= 1 && selector <= 4 ? kinds[selector - 1] : 0;
    FinalizeMatchingHandAttachments(*state, kind, childrenOnly);
    return 1;
}

std::uint64_t StageImmediateHandPose(HandParams *hand, const Matrix4x4 *parent) {
    if (!hand || !hand->owner || !hand->poseModelData)
        return 0;
    GenerateHandRelativeMatrices(*hand, parent, static_cast<std::int32_t>(GetModelBodyKind(hand->owner->model)));
    if (static_cast<std::uint8_t>(hand->selector - 3) > 1U)
        RotateHandPrimaryRelativeMatrix(*hand);
    if (!GetImageModuleSource(hand->owner))
        return 0;
    return PublishHandModelAttachment(*hand, hand->owner, static_cast<MT7Model *>(hand->owner->model), 0, 8);
}

std::uint64_t StageImmediateHandPose(HandParams *hand) {
    return StageImmediateHandPose(hand, handJointMatrices);
}

namespace model_component_detail {

// @verified - p
int HandDrawNullsub() {
    return 0;
}

// @verified - p
bool DrawPrimaryHandModel(HandParams &hand, float alpha) {
    if (!hand.primaryNode)
        return false;
    HandModelResource *const resource = GetHandModelResource(hand);
    if (resource)
        hand.primaryNode->commandOverride = &resource->meshCommand;
    MT7Model *const reader = hand.primaryReader;
    Matrix4x4_Push(nullptr);
    HandDrawNullsub();
    if (static_cast<double>(alpha) <= 0.999999) {
        if (static_cast<double>(alpha) > 0.000001) {
            if (resource)
                BindHandRenderMatrices(*resource, true);
            PrepareProceduralObjects(*reader, &reader->nodes.front(), alpha);
            if (resource)
                BindHandRenderMatrices(*resource, false);
        }
    } else {
        if (resource)
            BindHandRenderMatrices(*resource, true);
        ConstructMatricesForObjectRecursive(*reader, &reader->nodes.front());
        if (resource)
            BindHandRenderMatrices(*resource, false);
    }
    Matrix4x4::Pop(1);
    HandDrawNullsub();
    return true;
}

// @verified - p
bool DrawSecondaryHandModel(HandParams &hand, float alpha) {
    if (!hand.secondaryNode)
        return false;
    Matrix4x4_Push(nullptr);
    HandDrawNullsub();
    if (static_cast<double>(alpha) <= 0.999999) {
        if (static_cast<double>(alpha) > 0.000001)
            PrepareProceduralObjects(*hand.secondaryReader, hand.secondaryNode, alpha);
    } else {
        ConstructMatricesForObjectRecursive(*hand.secondaryReader, hand.secondaryNode);
    }
    Matrix4x4::Pop(1);
    HandDrawNullsub();
    return true;
}

} // namespace model_component_detail

std::uint32_t DrawCharacterHandReplacement(std::uint16_t nodeId, float *originalAlphaScale, float alpha, const MT7Node *originalNode) {
    *originalAlphaScale = 1.0f;
    HandParams *const hand = ResolveCharacterHandForDraw(nodeId);
    float replacementAlpha = 0.0f;
    std::uint32_t draws = 0;
    if (hand) {
        const std::int8_t state = AdvanceHandDrawState(*hand);
        replacementAlpha = hand->modelAlpha;
        if (state > 0) {
            const bool admitted = DrawPrimaryHandModel(*hand, replacementAlpha * alpha);
            if (!admitted)
                replacementAlpha = 0.0f;
            else
                ++draws;
        }
        if (state < 0) {
            const bool admitted = DrawSecondaryHandModel(*hand, replacementAlpha * alpha);
            if (!admitted)
                replacementAlpha = 0.0f;
            else
                ++draws;
        }
    }
    *originalAlphaScale = 1.0f - replacementAlpha;
    // The typed body-hand callback applies this after the caller's node transform; matrix-frame draws bypass it.
    if (originalNode && static_cast<double>(replacementAlpha) < 0.00001) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(originalNode->rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(originalNode->rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(originalNode->rotation.x));
    }
    return draws;
}

} // namespace shenmue::s2
