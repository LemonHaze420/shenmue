#include "s2/motion_node_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/motion_registry_dc.hpp"

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t SampleTimedNode(const TimedNodeMotionHeader *data, MT7Node *node, std::int32_t frame, float fraction, std::int32_t index) {
    using motion_detail::F16ToF32;
    const TimedNodeMotionRecord *record = reinterpret_cast<const TimedNodeMotionRecord *>(
        reinterpret_cast<std::uintptr_t>(data) + sizeof(TimedNodeMotionHeader) + static_cast<std::uintptr_t>(index) * sizeof(TimedNodeMotionRecord));
    if (index >= data->nodeCount)
        return index;
    ++index;
    if (record->scaleOffset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->scaleOffset) + frame * 3;
        node->scale.x = F16ToF32(samples[0]);
        node->scale.y = F16ToF32(samples[1]);
        node->scale.z = F16ToF32(samples[2]);
        if (fraction != 0.0f) {
            float deltaX = F16ToF32(samples[3]);
            deltaX -= node->scale.x;
            node->scale.x += deltaX * fraction;
            float deltaY = F16ToF32(samples[4]);
            deltaY -= node->scale.y;
            node->scale.y += deltaY * fraction;
            float deltaZ = F16ToF32(samples[5]);
            deltaZ -= node->scale.z;
            node->scale.z += deltaZ * fraction;
        }
    } else {
        node->scale.x = F16ToF32(static_cast<std::int16_t>(record->scale[0]));
        node->scale.y = F16ToF32(static_cast<std::int16_t>(record->scale[1]));
        node->scale.z = F16ToF32(static_cast<std::int16_t>(record->scale[2]));
    }
    if (record->rotation[0].offset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->rotation[0].offset);
        node->rotation.x = samples[frame];
        if (fraction != 0.0f) {
            const std::int16_t delta = static_cast<std::int16_t>(samples[frame + 1] - samples[frame]);
            const std::int32_t previous = static_cast<std::int16_t>(node->rotation.x);
            const std::int16_t change = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(delta) * fraction));
            node->rotation.x = previous + change;
        }
    } else {
        node->rotation.x = record->rotation[0].value;
    }
    if (record->rotation[1].offset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->rotation[1].offset);
        node->rotation.y = samples[frame];
        if (fraction != 0.0f) {
            const std::int16_t delta = static_cast<std::int16_t>(samples[frame + 1] - samples[frame]);
            const std::int32_t previous = static_cast<std::int16_t>(node->rotation.y);
            const std::int16_t change = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(delta) * fraction));
            node->rotation.y = previous + change;
        }
    } else {
        node->rotation.y = record->rotation[1].value;
    }
    if (record->rotation[2].offset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->rotation[2].offset);
        node->rotation.z = samples[frame];
        if (fraction != 0.0f) {
            const std::int16_t delta = static_cast<std::int16_t>(samples[frame + 1] - samples[frame]);
            const std::int32_t previous = static_cast<std::int16_t>(node->rotation.z);
            const std::int16_t change = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(delta) * fraction));
            node->rotation.z = previous + change;
        }
    } else {
        node->rotation.z = record->rotation[2].value;
    }
    if (record->position[0].offset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->position[0].offset);
        node->position.x = F16ToF32(samples[frame]);
        if (fraction != 0.0f) {
            float deltaX = F16ToF32(samples[frame + 1]);
            deltaX -= node->position.x;
            node->position.x += deltaX * fraction;
        }
    } else {
        node->position.x = F16ToF32(record->position[0].value);
    }
    if (record->position[1].offset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->position[1].offset);
        node->position.y = F16ToF32(samples[frame]);
        if (fraction != 0.0f) {
            float deltaY = F16ToF32(samples[frame + 1]);
            deltaY -= node->position.y;
            node->position.y += deltaY * fraction;
        }
    } else {
        node->position.y = F16ToF32(record->position[1].value);
    }
    if (record->position[2].offset) {
        const std::int16_t *samples = reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(data) + record->position[2].offset);
        node->position.z = F16ToF32(samples[frame]);
        if (fraction != 0.0f) {
            float deltaZ = F16ToF32(samples[frame + 1]);
            deltaZ -= node->position.z;
            node->position.z += deltaZ * fraction;
        }
    } else {
        node->position.z = F16ToF32(record->position[2].value);
    }
    if (node->child)
        index = SampleTimedNode(data, node->child, frame, fraction, index);
    if (node->sibling)
        index = SampleTimedNode(data, node->sibling, frame, fraction, index);
    return index;
}
#else
// @verified - d
std::int32_t SampleTimedNode(const TimedNodeMotionHeader *data, MT7Node *node, std::int32_t frame, float fraction, std::int32_t index) {
    if (index >= data->nodeCount)
        return index;
    const TimedNodeMotionRecord &record = reinterpret_cast<const TimedNodeMotionRecord *>(data + 1)[index++];
    const std::byte *const bytes = reinterpret_cast<const std::byte *>(data);
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        const std::uint16_t *const source =
            record.scaleOffset ? reinterpret_cast<const std::uint16_t *>(bytes + record.scaleOffset) + frame * 3 + axis : &record.scale[axis];
        float value = motion_detail::F16ToF32(static_cast<std::int16_t>(*source));
        if (record.scaleOffset && fraction != 0.0f) {
            const float delta = motion_detail::F16ToF32(static_cast<std::int16_t>(source[3])) - value;
            asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(value) : "f"(delta), "f"(fraction) : "fr0");
        }
        node->scale[axis] = value;
    }
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        const TimedNodeChannel &channel = record.rotation[axis];
        const std::int16_t *const source = channel.offset ? reinterpret_cast<const std::int16_t *>(bytes + channel.offset) + frame : &channel.value;
        std::int32_t value = *source;
        if (channel.offset && fraction != 0.0f) {
            const std::int16_t delta =
                static_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(source[1]) - static_cast<std::uint16_t>(*source)));
            const float change = static_cast<float>(delta) * fraction;
            std::int32_t truncated;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            truncated = TruncateToInt32(change);
#else
            asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(truncated) : "f"(change) : "fpul");
#endif
            value += static_cast<std::int16_t>(static_cast<std::uint16_t>(truncated));
        }
        node->rotation[axis] = value;
    }
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        const TimedNodeChannel &channel = record.position[axis];
        const std::uint16_t *const source =
            channel.offset ? reinterpret_cast<const std::uint16_t *>(bytes + channel.offset) + frame : reinterpret_cast<const std::uint16_t *>(&channel.value);
        float value = motion_detail::F16ToF32(static_cast<std::int16_t>(*source));
        if (channel.offset && fraction != 0.0f) {
            const float delta = motion_detail::F16ToF32(static_cast<std::int16_t>(source[1])) - value;
            asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(value) : "f"(delta), "f"(fraction) : "fr0");
        }
        node->position[axis] = value;
    }
    if (node->child)
        index = SampleTimedNode(data, node->child, frame, fraction, index);
    if (node->sibling)
        index = SampleTimedNode(data, node->sibling, frame, fraction, index);
    return index;
}
#endif

// @verified - d
void ClampTimedNodeMotionFrame(TimedNodeMotion *motion, const TimedNodeMotionHeader *data) {
    float frame = motion->frame;
    const float last = static_cast<float>(data->lastFrame);
    float begin = motion->begin;
    float end = motion->end;
    if (begin < 0.0f)
        begin = 0.0f;
    if (begin > last)
        begin = last;
    if (end < 0.0f)
        end = 0.0f;
    if (end > last)
        end = last;
    if (frame < begin) {
        if (motion->flags & TNMF_Loop) {
            frame = (frame - begin) + end;
            if (frame < begin)
                frame = begin;
        } else {
            frame = begin;
            motion->mode = 2;
        }
    } else if (frame > end) {
        if (motion->flags & TNMF_Loop) {
            frame = (frame + begin) - end;
            if (frame > end)
                frame = end;
        } else {
            frame = end;
            motion->mode = 2;
        }
    }
    motion->frame = frame;
    motion->begin = begin;
    motion->end = end;
}

bool TickTimedNodeMotion(TimedNodeMotion *motion) {
    if (!motion->mode)
        return false;
    const TimedNodeMotionHeader *const data = motion->data;
    if (data->version != -1 || (data->flags & 0xffdbU) || data->nodeCount <= 0 || data->nodeCount > 127)
        return false;
    ClampTimedNodeMotionFrame(motion, data);
    std::int32_t frame;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    frame = static_cast<std::int32_t>(motion->frame);
#else
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(frame) : "f"(motion->frame) : "fpul");
#endif
    SampleTimedNode(data, motion->model, frame, 0.0f, 0);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const float frameValue = motion->frame;
    if (motion->flags & TNMF_Reverse)
        motion->frame = frameValue - motion->speed;
    else
        motion->frame = frameValue + motion->speed;
#else
    if (motion->flags & TNMF_Reverse)
        motion->frame -= motion->speed;
    else
        motion->frame += motion->speed;
#endif
    if (motion->mode == 2)
        return true;
    return false;
}

// @verified - d
void TMNT_Callback(Character *character, void *data) {
    TimedNodeMotion *const motion = static_cast<TimedNodeMotion *>(data);
    if (character && motion && motion->mode)
        TickTimedNodeMotion(motion);
}

// @verified - d
void DrawTimedNodeModel(const MT7Node *node, float alpha) {
    charaApplyScale(node);
}

// @verified - d
void PrepareTimedNodeModel(const MT7Node *node, float alpha) {
    charaPrepareNodesForDraw(node, alpha);
}

// @verified - d
void AllocateTMNM(Character *character) {
    TimedNodeMotion *const motion = static_cast<TimedNodeMotion *>(AllocateModelComponent(character, TASK_TMNM, sizeof(TimedNodeMotion), TMNT_Callback));
    motion->mode = 0;
    motion->flags = 0;
    imgmAssignUpdateCallback(character, DrawTimedNodeModel);
    charaAssignPrepareForDrawCallback(character, PrepareTimedNodeModel);
}

// @verified - d
void ApplyNodeMotionFrame(const TimedNodeMotionHeader *data, MT7Node *model, float frame) {
    if (data->version != -1)
        data = reinterpret_cast<const TimedNodeMotionHeader *>(reinterpret_cast<const std::byte *>(data) + 4);
    std::int32_t index;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    index = static_cast<std::int32_t>(frame);
#else
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(index) : "f"(frame) : "fpul");
#endif
    if (index <= data->lastFrame)
        SampleTimedNode(data, model, index, 0.0f, 0);
}

void InitializePrefixedTimedNodeMotion(TimedNodeMotion *motion, MT7Node *model, const TimedNodeMotionHeader *data) {
    InitializeTimedNodeMotion(motion, model, reinterpret_cast<const TimedNodeMotionHeader *>(reinterpret_cast<const std::byte *>(data) + 4));
}

void ApplyPrefixedNodeMotionFrame(const TimedNodeMotionHeader *data, MT7Node *model, float frame) {
    ApplyNodeMotionFrame(reinterpret_cast<const TimedNodeMotionHeader *>(reinterpret_cast<const std::byte *>(data) + 4), model, frame);
}

// @verified - d
TimedNodeMotion *AllocateTimedNodeMotion() {
    TimedNodeMotion *const motion = static_cast<TimedNodeMotion *>(HLib::AllocateTaskData(sizeof(TimedNodeMotion), MEMORY_TIMED_NODE_MOTION));
    if (motion)
        motion->mode = 0;
    return motion;
}

void FreeTimedNodeMotion(TimedNodeMotion *motion) {
    HLib::FreeTaskData(motion);
}

// @verified - d
TimedNodeMotion *FindTimedNodeMotion(const Character *character) {
    return static_cast<TimedNodeMotion *>(charFindComp(character, TASK_TMNM));
}

// @verified - d
void InitializeTimedNodeMotion(TimedNodeMotion *motion, MT7Node *model, const TimedNodeMotionHeader *data) {
    if (!data)
        return;
    motion->mode = 1;
    motion->data = data;
    motion->model = model;
    motion->frame = 0.0f;
    motion->begin = 0.0f;
    motion->end = static_cast<float>(data->lastFrame);
    motion->speed = motion_detail::F16ToF32(static_cast<std::int16_t>(data->speed));
    motion->flags = 0;
}

TimedNodeMotion *CreateTimedNodeMotion() {
    return AllocateTimedNodeMotion();
}

// @verified - d
void ConfigureScenarioNodeMotion(shen_name name, std::int32_t motionId, float begin, float end, float frame, float speed, std::int32_t flags) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return;
    TimedNodeMotion *motion = FindTimedNodeMotion(character);
    if (motionId) {
        const TimedNodeMotionHeader *const data = reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(static_cast<std::uint16_t>(motionId)));
        InitializeTimedNodeMotion(motion, character->model, data);
    }
    if (!(begin < 0.0f))
        motion->begin = begin;
    if (!(end < 0.0f))
        motion->end = end;
    if (!(frame < 0.0f))
        motion->frame = frame;
    if (speed < 32767.0f)
        motion->speed = speed;
    if (flags >= 0) {
        if (!flags)
            motion->flags = flags;
        else
            motion->flags |= flags;
    }
}

// @verified - d
void CopyScenarioNodeMotionTimes(shen_name name, float *begin, float *end, float *frame, float *speed) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return;
    const TimedNodeMotion *const motion = FindTimedNodeMotion(character);
    if (begin)
        *begin = motion->begin;
    if (end)
        *end = motion->end;
    if (frame)
        *frame = motion->frame;
    if (speed)
        *speed = motion->speed;
}

// @verified - d
std::int32_t IsScenarioNodeMotionFinished(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character)
        return FindTimedNodeMotion(character)->mode == 2;
    return false;
}

// @verified - d
TimedNodeMotion *StopScenarioNodeMotion(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    TimedNodeMotion *motion = reinterpret_cast<TimedNodeMotion *>(character);
    if (motion) {
        motion = FindTimedNodeMotion(character);
        motion->mode = 0;
    }
    return motion;
}

} // namespace s2
} // namespace shenmue

#endif
