#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KRCT_TASK{SHEN_NAME(krct)}, KRCT_AUTHOR_TASK{SHEN_NAME(KRAU)};
inline constexpr shen_name KRCT_RYO{SHEN_NAME(RYO_)}, KRCT_SHENHUA{SHEN_NAME(SIN_)}, KRCT_NPC{SHEN_NAME(M01_)};
inline constexpr shen_name KRCT_LOAD_TASK{SHEN_NAME(load)}, KRCT_HIGH_MODEL_TASK{EVENT_HIGH_MODEL_TASK}, KRCT_INVALID{0xffffffffU};
using KrctSceneControl = void (*)(std::int32_t);
// The request and its four callbacks are borrowed from the ordinary KRC1 owner.
struct KrctRequest {
    std::int32_t state;
    std::uint32_t reserved04;
    KrctSceneControl scene, objects, character, lighting;
};
struct KrctGlobals {
    HLib::Task *task;
    KrctSceneControl scene;
    std::int32_t state, resumeState;
    std::int32_t authorSelection, reserved1c, reserved20, reserved24, admitted, reserved2c;
    KrctSceneControl objects, character, lighting;
    std::int32_t trigger, reserved4c;
};

struct KrctAuthorWork {
    HLib::Task *task;
    std::int32_t reserved08, selection;
    std::uint16_t state, reserved12, reserved14, reserved16;
    std::int32_t *result;
};
struct KrctAuthorLoopWork {
    KrctAuthorWork *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(KrctRequest) == 0x28 && offsetof(KrctRequest, scene) == 8);
static_assert(sizeof(KrctGlobals) == 0x50 && offsetof(KrctGlobals, objects) == 0x30 && offsetof(KrctGlobals, trigger) == 0x48);
static_assert(sizeof(KrctAuthorWork) == 0x20 && offsetof(KrctAuthorWork, state) == 0x10 && offsetof(KrctAuthorWork, result) == 0x18);
static_assert(sizeof(KrctAuthorLoopWork) == 0x10);
extern KrctGlobals krct;
extern float krctFireHeight;
extern std::int32_t krctFire, krctHandFire, krctFirePhase, krctFireTicks, krctFirePhaseStep;
extern std::int32_t krctAlternateSound;
inline constexpr shen_name krctCharacters[]{KRCT_SHENHUA, KRCT_INVALID};
inline constexpr shen_name krctAuthorNames[]{SHEN_NAME(A070), SHEN_NAME(A180), SHEN_NAME(A090), SHEN_NAME(A091), SHEN_NAME(A092), SHEN_NAME(A093),
    SHEN_NAME(A094), SHEN_NAME(A240), SHEN_NAME(A241), SHEN_NAME(A250), SHEN_NAME(A251), SHEN_NAME(A252), SHEN_NAME(A253), SHEN_NAME(A300), SHEN_NAME(A310),
    SHEN_NAME(A170)};
inline constexpr shen_name krctWindObjects[]{SHEN_NAME(a988), SHEN_NAME(a990), SHEN_NAME(a992), SHEN_NAME(a994), SHEN_NAME(aa51), SHEN_NAME(aa52),
    SHEN_NAME(aa53), SHEN_NAME(aa54), SHEN_NAME(aa55), SHEN_NAME(aa56), SHEN_NAME(aa57), SHEN_NAME(aa58), SHEN_NAME(aa59), SHEN_NAME(aa60), SHEN_NAME(aa61),
    KRCT_INVALID};
inline constexpr Vector3 krctPlayerReturn{160, 96.8f, 360.9f}, krctShenhuaReturn{157.18f, 500, 362.0358f};
inline constexpr Vector3i krctReturnAngle{0, 23665, 0};
void AreaKRCTInit();
void AreaKRCTMain(std::uintptr_t request);
void KrctMainLoop(void *data);
void QueueKrctAuthor(std::int32_t *result);
void KrctAuthorMain(void *data);
void KrctRestoreHighModels(void *data);
void KrctConfigureRestoredParty();
void KrctConfigureSeatedParty();
void KrctNullsub();
extern const ScriptAreaDefinition areaKRCTScriptDefinition;
} // namespace shenmue::s2::areas
