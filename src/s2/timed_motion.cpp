#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/model_dc.hpp"

namespace shenmue {
namespace s2 {

std::uint32_t *AllocateTMOT() {
    std::uint32_t *const storage = static_cast<std::uint32_t *>(HLib::AllocateTaskData(24, shen_name(SHEN_NAME(TMOT))));
    if (storage)
        *storage = 0;
    return storage;
}

struct TimedMotionNodeKeys {
    Vector3 *scale;
    std::int32_t *rotationX, *rotationY, *rotationZ;
    Vector3 *position;
};

struct TimedMotion {
    TimedMotionNodeKeys **nodes;
    float speed;
    float frameCount;
    std::uint32_t flags;
};

enum { TIMED_MOTION_LOOP = 4 };
enum { TIMED_MOTION_IDLE, TIMED_MOTION_PLAYING, TIMED_MOTION_FINISHED };

struct TimedMotionState {
    std::int32_t status;
    const TimedMotion *motion;
    MT7Node *node;
    float time;
    float speed;
};

void FreeTMOT(void *timedMotion) {
    HLib::FreeTaskData(timedMotion);
}

void InitTimedMotion(TimedMotionState *state, MT7Node *node, const TimedMotion *motion) {
    state->status = TIMED_MOTION_PLAYING;
    state->motion = motion;
    state->node = node;
    state->time = 0.0f;
    state->speed = motion->speed;
}

TimedMotionState *CreateTimedMotion() {
    return reinterpret_cast<TimedMotionState *>(AllocateTMOT());
}

int ApplyTimedMotionFrame(const TimedMotion *motion, MT7Node *node, int frame, float fraction, int nodeIndex) {
    TimedMotionNodeKeys *keys = motion->nodes[nodeIndex];
    int delta;
    nodeIndex++;
    if (keys->scale) {
        node->scale.x = keys->scale[frame].x;
        node->scale.y = keys->scale[frame].y;
        node->scale.z = keys->scale[frame].z;
        if (fraction != 0.0f) {
            node->scale.x += (keys->scale[frame + 1].x - keys->scale[frame].x) * fraction;
            node->scale.y += (keys->scale[frame + 1].y - keys->scale[frame].y) * fraction;
            node->scale.z += (keys->scale[frame + 1].z - keys->scale[frame].z) * fraction;
        }
    }
    if (keys->rotationZ) {
        node->rotation.z = keys->rotationZ[frame];
        if (fraction != 0.0f) {
            delta = (short)(keys->rotationZ[frame + 1] - keys->rotationZ[frame]);
            node->rotation.z = (short)node->rotation.z + (short)(delta * fraction);
        }
    }
    if (keys->rotationY) {
        node->rotation.y = keys->rotationY[frame];
        if (fraction != 0.0f) {
            delta = (short)(keys->rotationY[frame + 1] - keys->rotationY[frame]);
            node->rotation.y = (short)node->rotation.y + (short)(delta * fraction);
        }
    }
    if (keys->rotationX) {
        node->rotation.x = keys->rotationX[frame];
        if (fraction != 0.0f) {
            delta = (short)(keys->rotationX[frame + 1] - keys->rotationX[frame]);
            node->rotation.x = (short)node->rotation.x + (short)(delta * fraction);
        }
    }
    if (keys->position) {
        node->position.x = keys->position[frame].x;
        node->position.y = keys->position[frame].y;
        node->position.z = keys->position[frame].z;
        if (fraction != 0.0f) {
            node->position.x += (keys->position[frame + 1].x - keys->position[frame].x) * fraction;
            node->position.y += (keys->position[frame + 1].y - keys->position[frame].y) * fraction;
            node->position.z += (keys->position[frame + 1].z - keys->position[frame].z) * fraction;
        }
    }
    if (node->child)
        nodeIndex = ApplyTimedMotionFrame(motion, node->child, frame, fraction, nodeIndex);
    if (node->sibling)
        nodeIndex = ApplyTimedMotionFrame(motion, node->sibling, frame, fraction, nodeIndex);
    return nodeIndex;
}

int UpdateTimedMotion(TimedMotionState *state) {
    if (state->status == TIMED_MOTION_IDLE)
        return 0;
    float time = state->time;
    const TimedMotion *motion = state->motion;
    if (time < 0.0f) {
        if (motion->flags & TIMED_MOTION_LOOP) {
            time += motion->frameCount;
            if (time < 0.0f)
                time = motion->frameCount;
        } else {
            time = 0.0f;
            state->status = TIMED_MOTION_FINISHED;
        }
    } else if (time > motion->frameCount) {
        if (motion->flags & TIMED_MOTION_LOOP) {
            time -= motion->frameCount;
            if (time > motion->frameCount)
                time = 0.0f;
        } else {
            time = motion->frameCount;
            state->status = TIMED_MOTION_FINISHED;
        }
    }
    int frame = (int)time;
    float fraction;
    if ((int)motion->frameCount <= frame)
        fraction = 0.0f;
    else
        fraction = time - (float)frame;
    ApplyTimedMotionFrame(motion, state->node, frame, fraction, 0);
    state->time = state->speed + time;
    if (state->status == TIMED_MOTION_FINISHED)
        return 1;
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyTimedMotionAtFrame(const TimedMotion *motion, MT7Node *node, float frame) {
    ApplyTimedMotionFrame(motion, node, static_cast<int>(frame), 0.0f, 0);
}
#endif

} // namespace s2
} // namespace shenmue

#endif
