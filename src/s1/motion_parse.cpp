#include "s1/motion_internal.hpp"
#include <shendk/files/animation/motn_reader.h>

namespace shenmue::s1::motion_detail {
void ReadTriplet(
    MotnParseCursor &cursor, std::uint16_t instruction, std::uint16_t xMask, std::uint16_t yMask, std::uint16_t zMask, MotnKeyframeTriplet &output);

std::uint32_t ReadU32(const std::byte *bytes);

const std::byte *MotionDataForId(const MotnFileNode &node, std::uint16_t animationId);

const MotnFileNode *MotionFileForId(std::uint16_t animationId);

const std::byte *FindAnimationExtraRecord(std::uint16_t animationId, std::uint8_t tag);
}

namespace shenmue::s1 {
const std::byte *MOTN_GetReactionDataForAnimId(std::uint16_t animationId);

const MotnHandCommandEvent *MOTN_FindHandCommandEvent(
    std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive, std::uint32_t occurrenceOneBased);

const MotnSoundCommandEvent *MOTN_FindSoundCommandEvent(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive);

const MotnFigpControlEvent *MOTN_FindFigpControlEvent(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive);

bool MOTN_FindFrameMarker(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive, std::uint16_t &frame);
}

namespace shenmue::s1::motion_detail {

void ReadKeyframeData(MotnParseCursor &cursor, MotnKeyframeChannel &output) {
    const std::uint16_t encodedSize = cursor.block2Words ? *cursor.block2Words++ : *cursor.block2Bytes++;
    output = {};
    output.present = static_cast<std::uint16_t>(encodedSize + 2U);
    output.block3Data = cursor.block3Words ? static_cast<const void *>(cursor.block3Words) : static_cast<const void *>(cursor.block3Bytes);
    output.sizeData = cursor.block3Words;
    output.block4Data = cursor.block4;
    output.block5Begin = cursor.block5;

    const std::uint32_t descriptorCount = (static_cast<std::uint32_t>(output.present) + 3U) >> 2U;
    for (std::uint32_t index = 0; index < descriptorCount; ++index) {
        cursor.block5 += kPackedValueCounts[*cursor.block4++];
    }
    output.block5End = cursor.block5;
    if (cursor.block3Words)
        cursor.block3Words += encodedSize;
    else
        cursor.block3Bytes += encodedSize;
}

void ReadTriplet(
    MotnParseCursor &cursor, std::uint16_t instruction, std::uint16_t xMask, std::uint16_t yMask, std::uint16_t zMask, MotnKeyframeTriplet &output) {
    if (instruction & xMask)
        ReadKeyframeData(cursor, output.x);
    else
        output.x.present = 0;
    if (instruction & yMask)
        ReadKeyframeData(cursor, output.y);
    else
        output.y.present = 0;
    if (instruction & zMask)
        ReadKeyframeData(cursor, output.z);
    else
        output.z.present = 0;
}

std::uint32_t ReadU32(const std::byte *bytes) {
    std::uint32_t value{};
    std::memcpy(&value, bytes, sizeof(value));
    return value;
}

const std::byte *MotionDataForId(const MotnFileNode &node, std::uint16_t animationId) {
#if 0
    if (!node.motionData || animationId <= node.firstAnimId || animationId >= node.endAnimId) {
        return nullptr;
    }
    const auto *const base = static_cast<const std::byte *>(node.motionData);
    const std::size_t size = static_cast<std::size_t>(node.motionDataSize);
    if (size < 3U * sizeof(std::uint32_t))
        return nullptr;
    const std::uint32_t sequenceTableOffset = ReadU32(base);
    const std::uint32_t sequenceDataOffset = ReadU32(base + 2U * sizeof(std::uint32_t));
    const std::uint32_t sequenceIndex = static_cast<std::uint32_t>(animationId - node.firstAnimId - 1U);
    const std::uint64_t tableEntryOffset = static_cast<std::uint64_t>(sequenceTableOffset) + static_cast<std::uint64_t>(sequenceIndex) * 8U;
    if (tableEntryOffset + sizeof(std::uint32_t) > size)
        return nullptr;
    const std::uint32_t relativeAnimationOffset = ReadU32(base + tableEntryOffset);
    const std::uint64_t animationOffset = static_cast<std::uint64_t>(sequenceDataOffset) + relativeAnimationOffset;
    if (animationOffset + sizeof(MotnAnimationHeader) > size) {
        return nullptr;
    }
    return base + animationOffset;
#else
    if (!node.motionData || animationId <= node.firstAnimId || animationId >= node.endAnimId)
        return nullptr;
    const shendk::Bytes bytes{static_cast<const std::byte *>(node.motionData), static_cast<std::size_t>(node.motionDataSize)};
    shendk::MotnBankHeader header;
    shendk::Bytes payload;
    const std::uint32_t index = static_cast<std::uint32_t>(animationId - node.firstAnimId - 1U);
    if (!shendk::ReadMotnBankHeader(bytes, header) || !shendk::ReadMotnPayload(bytes, header, index, false, payload) ||
        payload.size() < sizeof(MotnAnimationHeader))
        return nullptr;
    return payload.data();
#endif
}

const MotnFileNode *MotionFileForId(std::uint16_t animationId) {
    for (MotnFileNode *node = MOTN_GetFileRegistryHead(); node; node = node->next) {
        if (MotionDataForId(*node, animationId))
            return node;
    }
    return nullptr;
}

bool MotionHasRootEndMarker(std::uint16_t animationId) {
    for (const std::uint16_t basePose : kBasePoseAnimationIds)
        if (animationId == basePose)
            return false;
    const MotnFileNode *const node = MotionFileForId(animationId);
    if (!node)
        return false;
    const auto *const bytes = static_cast<const std::byte *>(node->motionData);
    const std::uint32_t tableOffset = ReadU32(bytes);
    const std::uint32_t index = static_cast<std::uint32_t>(animationId - node->firstAnimId - 1U);
    const std::uint32_t markerOffset = ReadU32(bytes + tableOffset + index * 8U + 4U);
    if (!markerOffset || markerOffset + 12U >= static_cast<std::uint32_t>(node->motionDataSize))
        return false;
    std::size_t cursor = static_cast<std::size_t>(markerOffset) + 12U;
    for (std::size_t guard = 0; guard < 128U && cursor < static_cast<std::size_t>(node->motionDataSize); ++guard) {
        const auto cue = static_cast<std::uint8_t>(bytes[cursor]);
        if (cue == 24U)
            return true;
        if ((cue & 0xf7U) == 0U || cue >= kMarkerRecordSizes.size() || kMarkerRecordSizes[cue] == 0U)
            return false;
        cursor += kMarkerRecordSizes[cue];
    }
    return false;
}

const std::byte *FindAnimationExtraRecord(std::uint16_t animationId, std::uint8_t tag) {
    const MotnFileNode *const node = MotionFileForId(animationId);
    if (!node)
        return nullptr;
    const auto *const bytes = static_cast<const std::byte *>(node->motionData);
    const std::size_t size = static_cast<std::size_t>(node->motionDataSize);
    const std::uint32_t tableOffset = ReadU32(bytes);
    const std::uint32_t index = static_cast<std::uint32_t>(animationId - node->firstAnimId - 1U);
    const std::uint64_t entryOffset = static_cast<std::uint64_t>(tableOffset) + index * 8U + 4U;
    if (entryOffset + sizeof(std::uint32_t) > size)
        return nullptr;
    const std::uint32_t markerOffset = ReadU32(bytes + entryOffset);
    if (!markerOffset || static_cast<std::uint64_t>(markerOffset) + 16U > size) {
        return nullptr;
    }
    const std::byte *record = bytes + markerOffset + 12U;
    const std::byte *const end = bytes + size;
    while (record < end) {
        const std::uint8_t type = static_cast<std::uint8_t>(*record);
        if (type == 0U || type >= kMarkerRecordSizes.size())
            return nullptr;
        const std::uint8_t recordSize = kMarkerRecordSizes[type];
        if (recordSize == 0U || static_cast<std::size_t>(end - record) < recordSize) {
            return nullptr;
        }
        if (type == tag)
            return record;
        record += recordSize;
    }
    return nullptr;
}

bool FindAnimationExtraWord(std::uint16_t animationId, std::uint8_t tag, std::uint16_t &value) {
    const std::byte *const record = FindAnimationExtraRecord(animationId, tag);
    if (!record)
        return false;
    const std::uint8_t recordSize = kMarkerRecordSizes[tag];
    if (recordSize < 4U)
        return false;
    std::memcpy(&value, record + 2U, sizeof(value));
    return true;
}
}

namespace shenmue::s1 {

const std::uint8_t *shenLookupAnimationIndex(std::uint16_t animationId, std::uint8_t cueType) {
    return reinterpret_cast<const std::uint8_t *>(FindAnimationExtraRecord(animationId, cueType));
}

const MotnAnimationHeader *MOTN_GetDataForAnimId(std::uint16_t animationId) {
    const MotnFileNode *const node = MotionFileForId(animationId);
    const std::byte *const data = node ? MotionDataForId(*node, animationId) : nullptr;
    return reinterpret_cast<const MotnAnimationHeader *>(data);
}

const std::byte *MOTN_GetReactionDataForAnimId(std::uint16_t animationId) {
#if 0
    const MotnFileNode *const node = MotionFileForId(animationId);
    if (!node || !node->motionData)
        return nullptr;

    const auto *const base = static_cast<const std::byte *>(node->motionData);
    const std::size_t size = static_cast<std::size_t>(node->motionDataSize);
    if (size < sizeof(std::uint32_t))
        return nullptr;

    const std::uint32_t tableOffset = ReadU32(base);
    const std::uint32_t index = static_cast<std::uint32_t>(animationId - node->firstAnimId - 1U);
    const std::uint64_t entryOffset = static_cast<std::uint64_t>(tableOffset) + static_cast<std::uint64_t>(index) * 8U + 4U;
    if (entryOffset + sizeof(std::uint32_t) > size)
        return nullptr;

    const std::uint32_t reactionOffset = ReadU32(base + entryOffset);
    if (static_cast<std::uint64_t>(reactionOffset) + sizeof(MotnAnimationHeader) > size) {
        return nullptr;
    }
    return base + reactionOffset;
#else
    const MotnFileNode *const node = MotionFileForId(animationId);
    if (!node || !node->motionData)
        return nullptr;
    const shendk::Bytes bytes{static_cast<const std::byte *>(node->motionData), static_cast<std::size_t>(node->motionDataSize)};
    shendk::MotnBankHeader header;
    shendk::Bytes payload;
    const std::uint32_t index = static_cast<std::uint32_t>(animationId - node->firstAnimId - 1U);
    if (!shendk::ReadMotnBankHeader(bytes, header) || !shendk::ReadMotnPayload(bytes, header, index, true, payload))
        return nullptr;
    return payload.empty() ? bytes.data() : payload.data();
#endif
}

std::uint32_t MOTN_GetAnimationFlag(std::uint16_t animationId) {
    const MotnAnimationHeader *const animation = MOTN_GetDataForAnimId(animationId);
    return animation ? animation->sourceRig & 0x8000U : 0U;
}
}

namespace shenmue::s1::motion_detail {
const std::byte *MOTN_FindFrameEvent(
    std::uint16_t animationId, std::uint8_t eventType, float firstFrameInclusive, float lastFrameExclusive, std::uint32_t occurrenceOneBased) {
    if (occurrenceOneBased == 0)
        return nullptr;
    for (MotnFileNode *node = MOTN_GetFileRegistryHead(); node; node = node->next) {
        if (!node->motionData || animationId <= node->firstAnimId || animationId >= node->endAnimId) {
            continue;
        }
        const auto *const base = static_cast<const std::byte *>(node->motionData);
        const std::size_t size = static_cast<std::size_t>(node->motionDataSize);
        if (size < sizeof(std::uint32_t))
            return nullptr;
        const std::uint32_t tableOffset = ReadU32(base);
        const std::uint32_t index = static_cast<std::uint32_t>(animationId - node->firstAnimId - 1U);
        const std::uint64_t entryOffset = static_cast<std::uint64_t>(tableOffset) + index * 8U + 4U;
        if (entryOffset + sizeof(std::uint32_t) > size)
            return nullptr;
        const std::uint32_t markerOffset = ReadU32(base + entryOffset);
        if (!markerOffset || static_cast<std::uint64_t>(markerOffset) + 13U > size) {
            return nullptr;
        }

        const std::byte *marker = base + markerOffset + 12U;
        const std::byte *const end = base + size;
        while (marker < end) {
            const std::uint8_t type = static_cast<std::uint8_t>(*marker);
            if (type == 8U)
                break;
            if (type == 0U || type >= kMarkerRecordSizes.size()) {
                return nullptr;
            }
            const std::uint8_t recordSize = kMarkerRecordSizes[type];
            if (recordSize == 0U || static_cast<std::size_t>(end - marker) < recordSize) {
                return nullptr;
            }
            marker += recordSize;
        }
        if (marker >= end || static_cast<std::size_t>(end - marker) < 7U) {
            return nullptr;
        }

        const std::byte *event = marker + 4U;
        std::uint32_t match = 0;
        while (static_cast<std::size_t>(end - event) >= 4U) {
            const std::uint8_t type = static_cast<std::uint8_t>(event[2]);
            if (type == 0U)
                return nullptr;
            if (type >= kFrameEventRecordSizes.size())
                return nullptr;
            const std::uint8_t recordSize = kFrameEventRecordSizes[type];
            if (recordSize < 4U || static_cast<std::size_t>(end - event) < recordSize) {
                return nullptr;
            }
            std::uint16_t frame{};
            std::memcpy(&frame, event, sizeof(frame));
            const float frameValue = static_cast<float>(frame);
            if (type == eventType && frameValue >= firstFrameInclusive && frameValue < lastFrameExclusive && ++match == occurrenceOneBased) {
                return event;
            }
            event += recordSize;
        }
        return nullptr;
    }
    return nullptr;
}
}

namespace shenmue::s1 {

const MotnHandCommandEvent *MOTN_FindHandCommandEvent(
    std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive, std::uint32_t occurrenceOneBased) {
    return reinterpret_cast<const MotnHandCommandEvent *>(MOTN_FindFrameEvent(animationId, 0x12U, firstFrameInclusive, lastFrameExclusive, occurrenceOneBased));
}

const MotnSoundCommandEvent *MOTN_FindSoundCommandEvent(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive) {
    return reinterpret_cast<const MotnSoundCommandEvent *>(MOTN_FindFrameEvent(animationId, 4U, firstFrameInclusive, lastFrameExclusive, 1U));
}

const MotnFigpControlEvent *MOTN_FindFigpControlEvent(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive) {
    return reinterpret_cast<const MotnFigpControlEvent *>(MOTN_FindFrameEvent(animationId, 0x18U, firstFrameInclusive, lastFrameExclusive, 1U));
}

bool MOTN_FindFrameMarker(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive, std::uint16_t &frame) {
    const std::byte *const event = MOTN_FindFrameEvent(animationId, 1U, firstFrameInclusive, lastFrameExclusive, 1U);
    if (!event)
        return false;
    std::memcpy(&frame, event, sizeof(frame));
    return true;
}

bool MOTN_LoadAnimation(MotionInformationWork &motion, MotionNodeRuntime *nodes, std::uint16_t animationId, std::uint16_t targetRigIndex,
    MotnKeyframeTriplet *&keyStorage, MotionNodeRuntime *pairedNodes) {
    const MotnAnimationHeader *const animation = MOTN_GetDataForAnimId(animationId);
    if (!animation || !nodes || !keyStorage)
        return false;
    const bool templateOnly = (animation->sourceRig & 0x8000U) != 0;
    const std::uint16_t sourceRigIndex = templateOnly ? static_cast<std::uint16_t>(animation->sourceRig & 0x7fffU) : animation->sourceRig;
    const MotionRigType *const sourceRig = MOTN_GetRigType(sourceRigIndex);
    const MotionRigType *const channelRig = templateOnly ? MOTN_GetRigType(static_cast<std::uint8_t>(animation->sourceRig)) : sourceRig;
    const MotionRigType *const targetRig = MOTN_GetRigType(targetRigIndex);
    if (!sourceRig || !targetRig || sourceRig->nodes[0].position.y == 0.0f)
        return false;

    const float timeScale = targetRig->nodes[0].position.y / sourceRig->nodes[0].position.y;
    MOTN_InitRigNodes(motion, nodes, targetRigIndex, timeScale, pairedNodes);




    if (templateOnly) {
        std::array<std::uint8_t, 80> channelMap{};
        const auto *const bytes = reinterpret_cast<const std::uint8_t *>(animation);
        std::uint8_t nextChannel = 3;
        constexpr std::uint64_t kSingleChannelNodes = 0x20102010400ULL;
        for (std::size_t index = 0; index < channelRig->nodeCount && index < channelMap.size(); ++index) {
            std::uint16_t maskWord{};
            std::memcpy(&maskWord, bytes + 6 + (index >> 4U) * 2U, sizeof(maskWord));
            if ((maskWord & (1U << (index & 15U))) == 0)
                continue;
            channelMap[index] = nextChannel;
            const std::uint8_t nodeId = channelRig->nodes[index].nodeId;
            nextChannel = static_cast<std::uint8_t>(nextChannel + (nodeId <= 41U && ((kSingleChannelNodes >> nodeId) & 1U) ? 1U : 3U));
        }
        for (MotionNodeRuntime *node = nodes; node->nodeId != 0xff; ++node) {
            node->reserved07 = 0;
            node->bindPositionPacked[0] = 0;
            for (std::size_t sourceIndex = 0; sourceIndex < channelRig->nodeCount; ++sourceIndex) {
                if (channelRig->nodes[sourceIndex].nodeId != node->nodeId)
                    continue;
                node->reserved07 = channelMap[sourceIndex];
                node->bindPositionPacked[0] = channelMap[sourceIndex];
                break;
            }
        }
        for (std::size_t index = 0; index < targetRig->nodeCount; ++index) {
            MotionNodeRuntime &node = nodes[index];
            if (!node.channelSet || node.channelSet >= targetRig->nodeCount)
                continue;
            MotionNodeRuntime &paired = nodes[node.channelSet];
            paired.bindPositionPacked[0] = node.reserved07;
            node.bindPositionPacked[0] = paired.reserved07;
        }
        return true;
    }

    const auto *const bytes = reinterpret_cast<const std::uint8_t *>(animation);
    MotnParseCursor cursor{};
    cursor.block1 = reinterpret_cast<const std::uint16_t *>(animation + 1);
    if ((animation->attribute & 0x8000U) != 0)
        cursor.block2Words = reinterpret_cast<const std::uint16_t *>(bytes + animation->block2Offset);
    else
        cursor.block2Bytes = bytes + animation->block2Offset;
    if ((animation->attribute & 0x7fffU) > 255U)
        cursor.block3Words = reinterpret_cast<const std::uint16_t *>(bytes + animation->block3Offset);
    else
        cursor.block3Bytes = bytes + animation->block3Offset;
    cursor.block4 = bytes + animation->block4Offset;
    cursor.block5 = reinterpret_cast<const std::uint16_t *>(bytes + animation->block5Offset);

    for (std::size_t sourceIndex = 0; sourceIndex < sourceRig->nodeCount; ++sourceIndex) {
        const std::uint16_t instruction = *cursor.block1;
        if ((instruction >> 9U) != sourceIndex)
            continue;
        ++cursor.block1;

        MotionNodeRuntime *const target = FindRuntimeNode(nodes, sourceRig->nodes[sourceIndex].nodeId);
        if (target) {
            if ((instruction & 0x01c0U) != 0) {
                target->translationKeys = &keyStorage->x;
                ReadTriplet(cursor, instruction, 0x0100U, 0x0080U, 0x0040U, *keyStorage++);
            }
            if ((instruction & 0x0038U) != 0) {
                target->rotationKeys = &keyStorage->x;
                ReadTriplet(cursor, instruction, 0x0020U, 0x0010U, 0x0008U, *keyStorage++);
            }
        } else {
            MotnKeyframeChannel discarded{};
            constexpr std::array<std::uint16_t, 6> masks{0x0100U, 0x0080U, 0x0040U, 0x0020U, 0x0010U, 0x0008U};
            for (const std::uint16_t mask : masks)
                if (instruction & mask)
                    ReadKeyframeData(cursor, discarded);
        }
    }
    return true;
}

// MOTN_IncrementDataOffsets: exchanges each row's FCV channels with its mirror partner's.
void MOTN_IncrementDataOffsets(MotionNodeRuntime *rows) {
    for (MotionNodeRuntime *row = rows; row->nodeId != 0xff; ++row) {
        if (!row->channelSet)
            continue;
        MotionNodeRuntime &partner = rows[row->channelSet];
        MotnKeyframeChannel *const translation = row->translationKeys;
        if (translation || partner.translationKeys) {
            row->translationKeys = partner.translationKeys;
            partner.translationKeys = translation;
        }
        MotnKeyframeChannel *const rotation = row->rotationKeys;
        if (rotation || partner.rotationKeys) {
            row->rotationKeys = partner.rotationKeys;
            partner.rotationKeys = rotation;
        }
    }
}

// doMotionUpdate: loads the lookup motion into the transition rows, paired to the rows ApplyBasePose is about to make active.
std::int32_t MOTN_LoadTransitionRows(MotionInformationWork &motion, std::uint16_t animationId) {
    std::uint16_t lookupAnimId = animationId;
    const MotnAnimationHeader *animation = MOTN_GetDataForAnimId(animationId);
    if (!animation) {
        lookupAnimId = 1;
        animation = MOTN_GetDataForAnimId(1);
    }
    const std::uint32_t flags = motion.motionFlags;
    motion.lookupAnimId = lookupAnimId;
    const bool mirrored =
        (flags & 0x1000000U) != 0 ? (flags & 0x80U) != 0 : ((flags >> 7U) & 1U) != (shenLookupAnimationIndex(lookupAnimId, 19) != nullptr ? 1U : 0U);
    motion.motionFlags = mirrored ? flags | 0x80000U : flags & ~0x80000U;
    if (!animation || (animation->sourceRig & 0x8000U) != 0)
        return 2;
    if (!motion.transitionNodes) {
        AllocateMotionNodes(reinterpret_cast<MotionNodeRuntime *&>(motion.transitionNodes), motion.transitionNodesCookie, nullptr);
        static_cast<MotionNodeRuntime *>(motion.transitionNodes)->nodeId = 0xff;
    }
    auto *const rows = static_cast<MotionNodeRuntime *>(motion.transitionNodes);
    auto *keyStorage = reinterpret_cast<MotnKeyframeTriplet *>(motion.transitionNodesCookie);
    MOTN_LoadAnimation(motion, rows, lookupAnimId, static_cast<std::uint16_t>(motion.basePoseTemplateIndex), keyStorage,
        static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    if ((motion.motionFlags & 0x80000U) != 0)
        MOTN_IncrementDataOffsets(rows);
    return 0;
}
}
