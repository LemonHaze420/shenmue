#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {




struct MhndHand {
    MT7Node *node;                   // +0x00, non-null while an interpolation runs
    std::int32_t side;
    std::int32_t shape;
    std::int32_t frames;
    std::array<Vector3i, 4> current;
    std::array<Vector3i, 4> target;
    std::array<Vector3i, 4> step;    // +0x74
    // Palm-to-second-tip distance, first/second/third proximal chains, second/third distal chains.
    std::array<float, 6> measurements;
};
struct MhndPoseScript;
using MhndPoseScriptCallback = void (*)(Character *, MhndPoseScript *, MhndHand *);
// State 14 fetches the next 8-byte-aligned command, 6 waits `wait` frames, 7 waits for the hand
// interpolation to finish, 13 runs `callback` each frame and 0 ends the script.
struct MhndPoseScript {
    std::int64_t state;
    std::int64_t mode;
    std::int64_t wait;
    const std::byte *cursor;
    const std::byte *loopStart;
    const std::byte *loopBack;
    std::int32_t loopCount;
    MhndPoseScriptCallback callback;
    std::uint64_t field40;
};
struct MhndComponent {
    std::array<MhndHand, 2> hands;
    std::array<MhndPoseScript, 2> scripts;
};
static_assert(sizeof(MhndHand) == 0xc0);
static_assert(offsetof(MhndHand, side) == 0x08);
static_assert(offsetof(MhndHand, shape) == 0x0c);
static_assert(offsetof(MhndHand, frames) == 0x10);
static_assert(offsetof(MhndHand, current) == 0x14);
static_assert(offsetof(MhndHand, target) == 0x44);
static_assert(offsetof(MhndHand, step) == 0x74);
static_assert(offsetof(MhndHand, measurements) == 0xa4);
static_assert(sizeof(MhndPoseScript) == 0x48);
static_assert(offsetof(MhndPoseScript, state) == 0x00);
static_assert(offsetof(MhndPoseScript, mode) == 0x08);
static_assert(offsetof(MhndPoseScript, wait) == 0x10);
static_assert(offsetof(MhndPoseScript, cursor) == 0x18);
static_assert(offsetof(MhndPoseScript, loopStart) == 0x20);
static_assert(offsetof(MhndPoseScript, loopBack) == 0x28);
static_assert(offsetof(MhndPoseScript, loopCount) == 0x30);
static_assert(offsetof(MhndPoseScript, callback) == 0x38);
static_assert(offsetof(MhndPoseScript, field40) == 0x40);
static_assert(sizeof(MhndComponent) == 0x210);
static_assert(offsetof(MhndComponent, scripts) == 0x180);
void mhndInit(MhndComponent *component);
void ResetHandMeasurementBindings(Character *character);

void SetupHandPoseScript(Character *character);
// FTBL344: installs the idle-pose callback on both sides, returning that callback, not MHND.
// An existing disabled MHND is not reactivated at this boundary.
MhndPoseScriptCallback SetupHandIdlePoseCallback(Character *character);
// FTBL345: preserves the zero-extended EAX shape/error word, including modes below zero.
std::uint64_t GetHandShapeFor(Character *character, std::int32_t mode);



void hmdlSetFlagsFor(Character *character, std::int32_t mode, std::int32_t shape, std::int32_t frames);
void hmdlSetPoseFor(Character *character, std::int32_t mode, const Vector3i *pose, std::int32_t frames);


void mhndSetupHuman(Character *character);

} // namespace shenmue::s2
