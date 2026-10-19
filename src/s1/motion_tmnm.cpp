#include "s1/motion_internal.hpp"

namespace shenmue::s1::motion_detail {
Mt5RuntimeNode *TmnmOffsetNode(std::int32_t offset);

float SampleTmnmHalf(const std::byte *sequence, const std::byte *record, std::size_t offsetField, std::size_t staticField, std::int32_t frame,
    std::size_t frameStride = 2U, std::size_t componentOffset = 0U);

std::int32_t SampleTmnmAngle(const std::byte *sequence, const std::byte *record, std::size_t offsetField, std::size_t staticField, std::int32_t frame);

std::int32_t ApplyTmnmPose(const std::byte *sequence, Mt5RuntimeNode *node, std::int32_t frame, std::int32_t recordIndex);

TmnmWork *GetTmnmWork(Character *character);

void ApplyJhd0TmnmSequencePatch(TmnmWork &work);

void TMNM_Callback(Character *character, void *payload);
}

namespace shenmue::s1 {
void TMNM_Alloc(Character *character);

void TMNM_SetParameters(
    Character *character, std::uint32_t animationId, float startFrame, float endFrame, float currentFrame, float frameStep, std::int32_t flags);

bool TMNM_HasEnded(Character *character);

void TMNM_Reset(Character *character);
}

namespace shenmue::s1::motion_detail {

Mt5RuntimeNode *TmnmOffsetNode(std::int32_t offset) {
    return offset ? reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + offset) : nullptr;
}

float SampleTmnmHalf(const std::byte *sequence, const std::byte *record, std::size_t offsetField, std::size_t staticField, std::int32_t frame,
    std::size_t frameStride, std::size_t componentOffset) {
    const std::uint16_t offset = ReadTmnm<std::uint16_t>(record + offsetField);
    const std::byte *const value = offset ? sequence + offset + static_cast<std::size_t>(frame) * frameStride + componentOffset : record + staticField;
    return F16ToF32(ReadTmnm<std::int16_t>(value));
}

std::int32_t SampleTmnmAngle(const std::byte *sequence, const std::byte *record, std::size_t offsetField, std::size_t staticField, std::int32_t frame) {
    const std::uint16_t offset = ReadTmnm<std::uint16_t>(record + offsetField);
    const std::byte *const value = offset ? sequence + offset + static_cast<std::size_t>(frame) * 2U : record + staticField;
    return ReadTmnm<std::int16_t>(value);
}

std::int32_t ApplyTmnmPose(const std::byte *sequence, Mt5RuntimeNode *node, std::int32_t frame, std::int32_t recordIndex) {
    const auto *const header = reinterpret_cast<const TmnmSequenceHeader *>(sequence);
    if (recordIndex >= header->nodeCount)
        return recordIndex;

    const std::byte *const record = sequence + sizeof(TmnmSequenceHeader) + static_cast<std::size_t>(recordIndex) * 0x20U;
    ++recordIndex;
    node->scale = {
        SampleTmnmHalf(sequence, record, 0x00, 0x02, frame, 6U, 0U),
        SampleTmnmHalf(sequence, record, 0x00, 0x04, frame, 6U, 2U),
        SampleTmnmHalf(sequence, record, 0x00, 0x06, frame, 6U, 4U),
    };
    node->rotation = {
        SampleTmnmAngle(sequence, record, 0x08, 0x0a, frame),
        SampleTmnmAngle(sequence, record, 0x0c, 0x0e, frame),
        SampleTmnmAngle(sequence, record, 0x10, 0x12, frame),
    };
    node->position = {
        SampleTmnmHalf(sequence, record, 0x14, 0x16, frame),
        SampleTmnmHalf(sequence, record, 0x18, 0x1a, frame),
        SampleTmnmHalf(sequence, record, 0x1c, 0x1e, frame),
    };

    if (Mt5RuntimeNode *const child = TmnmOffsetNode(node->childOffset))
        recordIndex = ApplyTmnmPose(sequence, child, frame, recordIndex);
    if (Mt5RuntimeNode *const sibling = TmnmOffsetNode(node->siblingOffset))
        recordIndex = ApplyTmnmPose(sequence, sibling, frame, recordIndex);
    return recordIndex;
}

TmnmWork *GetTmnmWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(TMNM));
    return component ? reinterpret_cast<TmnmWork *>(component + 1) : nullptr;
}

void ApplyJhd0TmnmSequencePatch(TmnmWork &work) {
    if (g_areaId != SHEN_NAME(JHD0) || !work.model || !work.sequence || work.endFrame <= 0.0F) {
        return;
    }

    std::byte *const modelBase = MT5_ModelBaseForNode(work.model);
    Mt5RuntimeNode *const child = modelBase && work.model->childOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + work.model->childOffset) : nullptr;
    void *const binding = MT5_SceneBindingForNode(child, nullptr);
    if (!binding) {
        return;
    }
    const auto sceneKey = static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(binding) + 4U) % 0xffffffffULL);
    const GameNode *const sceneNode = FindSceneEntityByID(sceneKey);
    if (!sceneNode || std::memcmp(sceneNode->entityName, "MJAS3", sizeof(sceneNode->entityName)) != 0) {
        return;
    }

    std::byte *const sequence = const_cast<std::byte *>(work.sequence);
    std::byte *const adjustedStream = sequence + ReadTmnm<std::uint16_t>(sequence + 0x18);
    std::byte *const firstHeldStream = sequence + ReadTmnm<std::uint16_t>(sequence + 0x20);
    std::byte *const secondHeldStream = sequence + ReadTmnm<std::uint16_t>(sequence + 0x24);
    std::byte *const thirdHeldStream = sequence + ReadTmnm<std::uint16_t>(sequence + 0x28);
    for (std::int32_t frame = 0; static_cast<float>(frame) < work.endFrame; ++frame) {
        if (frame >= 96) {
            std::memcpy(firstHeldStream + static_cast<std::size_t>(frame) * 2U, firstHeldStream + 95U * 2U, sizeof(std::uint16_t));
            std::memcpy(secondHeldStream + static_cast<std::size_t>(frame) * 2U, secondHeldStream + 95U * 2U, sizeof(std::uint16_t));
            std::memcpy(thirdHeldStream + static_cast<std::size_t>(frame) * 2U, thirdHeldStream + 95U * 2U, sizeof(std::uint16_t));
        }
        if (frame >= 83) {
            const float blend = std::min(1.0F, static_cast<float>(frame - 83) / 13.0F);
            const auto oldValue = static_cast<float>(ReadTmnm<std::int16_t>(adjustedStream + static_cast<std::size_t>(frame) * 2U));
            const auto newValue = static_cast<std::int32_t>((1.0F - blend) * oldValue + blend * 33354.0F);
            const auto storedValue = static_cast<std::uint16_t>(newValue);
            std::memcpy(adjustedStream + static_cast<std::size_t>(frame) * 2U, &storedValue, sizeof(storedValue));
        }
    }
}

void TMNM_Callback(Character *character, void *payload) {
    auto *const work = static_cast<TmnmWork *>(payload);
    if (!character || !work || work->state == 0)
        return;
    const auto *const header = reinterpret_cast<const TmnmSequenceHeader *>(work->sequence);
    if (header->marker != 0xffffU || (header->formatFlags & 0xffdbU) != 0 || header->nodeCount < 1 || header->nodeCount > 127) {
        return;
    }

    const float frameCount = static_cast<float>(header->frameCount);
    const float endFrame = std::fmin(frameCount, std::fmax(0.0F, work->endFrame));
    const float startFrame = std::fmin(frameCount, std::fmax(0.0F, work->startFrame));
    float frame = work->frame;
    if (startFrame > frame) {
        if ((work->flags & 4U) != 0) {
            work->startFrame = startFrame;
            work->endFrame = endFrame;
            work->frame = std::fmax(startFrame, (frame - startFrame) + endFrame);
            frame = work->frame;
        } else {
            frame = startFrame;
            work->state = 2;
        }
    } else if (frame > endFrame) {
        if ((work->flags & 4U) != 0) {
            work->startFrame = startFrame;
            work->endFrame = endFrame;
            work->frame = std::fmin(endFrame, (startFrame + frame) - endFrame);
            frame = work->frame;
        } else {
            frame = endFrame;
            work->state = 2;
        }
    }
    work->frame = frame;
    work->startFrame = startFrame;
    work->endFrame = endFrame;

    ApplyTmnmPose(work->sequence, work->model, static_cast<std::int32_t>(frame), 0);
    work->frame = (work->flags & 0x20U) != 0 ? frame - work->frameStep : frame + work->frameStep;
}
}

namespace shenmue::s1 {

void TMNM_Alloc(Character *character) {
    if (!character || HLib_GetCharacterComponent(character, SHEN_NAME(TMNM))) {
        return;
    }
    auto *const work = static_cast<TmnmWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(TMNM), sizeof(TmnmWork), TMNM_Callback));
    work->state = 0;
    work->flags = 0;
    character->drawCallback = CharacterHandler_DrawSubmitAndScale;
    character->alphaDrawCallback = CharacterHandler_AlphaDraw;
}

void TMNM_SetParameters(
    Character *character, std::uint32_t animationId, float startFrame, float endFrame, float currentFrame, float frameStep, std::int32_t flags) {
    TmnmWork *const work = GetTmnmWork(character);
    if (!work)
        return;
    if (animationId != 0) {
        const auto *const sequence = reinterpret_cast<const std::byte *>(MOTN_GetDataForAnimId(static_cast<std::uint16_t>(animationId)));
        if (!sequence || !character->model)
            return;
        const auto *const header = reinterpret_cast<const TmnmSequenceHeader *>(sequence);
        work->model = static_cast<Mt5RuntimeNode *>(character->model);
        work->frame = 0.0F;
        work->startFrame = 0.0F;
        work->state = 1;
        work->sequence = sequence;
        work->endFrame = static_cast<float>(header->frameCount);
        work->frameStep = F16ToF32(header->defaultFrameStep);
        work->flags = 0;
        ApplyJhd0TmnmSequencePatch(*work);
    }
    if (startFrame >= 0.0F)
        work->startFrame = startFrame;
    if (endFrame >= 0.0F)
        work->endFrame = endFrame;
    if (currentFrame >= 0.0F)
        work->frame = currentFrame;
    if (frameStep < 32767.0F)
        work->frameStep = frameStep;
    if (flags >= 0) {
        if (flags == 0)
            work->flags = 0;
        else
            work->flags |= static_cast<std::uint16_t>(flags);
    }
}

bool TMNM_HasEnded(Character *character) {
    TmnmWork *const work = GetTmnmWork(character);
    return work && work->state == 2;
}

void TMNM_Reset(Character *character) {
    if (TmnmWork *const work = GetTmnmWork(character))
        work->state = 0;
}
}
