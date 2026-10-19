#include "s2/areas/area_WB88.hpp"
#include "s2/pak.hpp"
#include "s2/areas/area_wb00.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, rude{SHEN_NAME(RUDE)}, dummy{SHEN_NAME(DUMY)}, duck{SHEN_NAME(DUCK)}, kun{SHEN_NAME(KUN_)}, invalid{0xffffffffU};
constexpr std::uint32_t local = 0x38000000U, recursive = 0x40000000U;
constexpr shen_name leafNames[]{SHEN_NAME(HAP0), SHEN_NAME(HAP1), SHEN_NAME(HAP2), SHEN_NAME(HAP3), SHEN_NAME(HAP4), SHEN_NAME(HAP5), SHEN_NAME(HAP6),
    SHEN_NAME(HAP7), SHEN_NAME(HAP8), SHEN_NAME(HAP9), SHEN_NAME(HAPA), SHEN_NAME(HAPB), SHEN_NAME(HAPC), SHEN_NAME(HAPD), SHEN_NAME(HAPE), SHEN_NAME(HAPF),
    SHEN_NAME(HAPG), SHEN_NAME(HAPH), SHEN_NAME(HAPI), SHEN_NAME(HAPJ)};
constexpr shen_name characters[]{SHEN_NAME(SYE_), SHEN_NAME(SRY_), kun, invalid};
struct LeafWork {
    Vector3 position, direction;
    Vector3i rotation;
    std::int32_t phase, step, accumulated, limit, reserved34, inside, state, delay, blur;
    float alpha;
};
static_assert(sizeof(LeafWork) == 0x4c);
static_assert(offsetof(LeafWork, state) == 0x3c);
struct CatchWork {
    HLib::Task *self;
    std::byte reserved08[12];
    LeafWork leaves[20];
    std::int32_t reserved604, selected, round, caughtChain, duckTokens, duckCaught, alphaMode;
    HLib::Task *moveTask;
    Vector3 delta;
    std::uint32_t reserved634;
    std::byte reserved638[8];
    std::int32_t approach, entry, replay;
    float caughtHeight;
    std::int32_t scene, authorFlags, currentAuthorFrame;
    std::uint32_t authorEffectHandle;
    HLib::TaskCallback returnCallback;
    std::int32_t showHelp, remainingFrames;
};
static_assert(sizeof(CatchWork) == 0x670);
static_assert(offsetof(CatchWork, leaves) == 0x14);
static_assert(offsetof(CatchWork, moveTask) == 0x620);
static_assert(offsetof(CatchWork, returnCallback) == 0x660);
static_assert(offsetof(CatchWork, entry) == 0x644 && offsetof(CatchWork, scene) == 0x650);
static_assert(offsetof(CatchWork, currentAuthorFrame) == 0x658 && offsetof(CatchWork, authorEffectHandle) == 0x65c);
static_assert(offsetof(CatchWork, remainingFrames) == 0x66c);
CatchWork work{};
std::int32_t noVisible{}, misses{}, visibleState{}, zoomPercent{}, zoomDelay{}, previousZoom{}, caughtCount{};
Vector3 cameraEye{}, cameraCentre{}, cameraOffset{}, cameraSide{}, cameraOtherSide{};
std::int32_t cameraFov{}, cameraZoom{}, cameraHold{}, lastLookX{}, lastLookY{};
std::int16_t savedWeather{};
Vector2 duckFall{};
constexpr Vector3 leafOrigin{0, 2.5f, 2.4f};
constexpr Vector3i leafRotation{0, 32768, 16384};
struct Attachment {
    Vector3 position;
    Vector3i rotation;
};
constexpr Attachment attachments[20]{{{0.1599999964237213f, 0.00800000037997961f, 0.0f}, {16384, 49152, 49152}},
    {{0.18000000715255737f, 0.00800000037997961f, 0.0f}, {16384, 49152, 45511}}, {{0.14000000059604645f, 0.00800000037997961f, 0.0f}, {16384, 49152, 52792}},
    {{0.17000000178813934f, 0.00800000037997961f, 0.0f}, {16384, 49152, 47331}}, {{0.15000000596046448f, 0.00800000037997961f, 0.0f}, {16384, 49152, 50972}},
    {{0.16500000655651093f, 0.00800000037997961f, 0.0f}, {16384, 49152, 48241}}, {{0.1550000011920929f, 0.00800000037997961f, 0.0f}, {16384, 49152, 50062}},
    {{0.17499999701976776f, 0.00800000037997961f, 0.0f}, {16384, 49152, 46421}}, {{0.14499999582767487f, 0.00800000037997961f, 0.0f}, {16384, 49152, 51882}},
    {{0.1850000023841858f, 0.00800000037997961f, 0.0f}, {16384, 49152, 44600}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}},
    {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}},
    {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}, {{0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
struct ReplayShot {
    std::uint32_t animation;
    std::int32_t next, camera;
    float begin, end;
};
constexpr ReplayShot shots[]{{4101, 1, 1020, 70, 100}, {4101, 0, 1021, 70, 145}, {4101, 1, 1020, 70, 100}, {4101, 0, 1021, 70, 145}, {4101, 0, 1024, 70, 230},
    {4101, 0, 0, 0, 0}, {4101, 0, 1024, 90, 230}};
std::uint32_t readyHand[57]{0x0U, 0x0U, 0xd280U, 0x0U, 0x0U, 0xefa4U, 0x0U, 0x0U, 0x2d5U, 0x0U, 0x0U, 0xfa4fU, 0x0U, 0x0U, 0x16cU, 0x0U, 0xfc73U, 0xfc73U, 0x0U,
    0x0U, 0x0U, 0x0U, 0x0U, 0xfa4fU, 0x0U, 0x0U, 0x16cU, 0x0U, 0xfddeU, 0xfc73U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xd280U, 0x0U, 0xb6U, 0xb555U, 0x0U, 0x0U,
    0xe000U, 0x0U, 0x12U, 0x0U, 0x886U, 0x16cU, 0xcd82U, 0x0U, 0xfd29U, 0xba4fU, 0x2d5U, 0xfbbdU, 0xdbbcU, 0x2d5U, 0x5bU, 0xfe94U};
std::uint32_t relaxedHand[57]{0x0U, 0x0U, 0xffffd556U, 0x0U, 0x0U, 0xffffd6c2U, 0x0U, 0x5b0U, 0x0U, 0x0U, 0x0U, 0xffffb99aU, 0x0U, 0x0U, 0xffffbd28U, 0x0U,
    0x0U, 0xffffb82eU, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xffffd05cU, 0x0U, 0x0U, 0xffffb8e4U, 0x0U, 0x0U, 0xffffbe94U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xffffd1c8U,
    0x0U, 0x0U, 0xffffb82eU, 0x0U, 0x0U, 0xffffc4fbU, 0x0U, 0x0U, 0x0U, 0x0U, 0x38eU, 0xffffd112U, 0x0U, 0xfffff778U, 0xffffbe94U, 0x0U, 0xfffff778U,
    0xffffd8e4U, 0x0U, 0x0U, 0x0U};

void MainInit(void *);
void MainInit2(void *);
void MainLoop(void *);
void Exit(void *);
void Exit2(void *);
void PreExit(void *);
void PreExit2(void *);
void CatchLoop(void *);
void MissLoop(void *);
void Replay(void *);
void AuthorMain(void *);
void Auto1(void *);
void Auto2(void *);
void Auto3(void *);
void Auto4(void *);
void Auto5A(void *);
void Auto5(void *);
void Auto6(void *);
void Auto7(void *);
void AuthorCheck(void *);
void Author0(void *);
void Author1(void *);
void Author2(void *);
void Author3(void *);
void Author4(void *);
void StepNullsub() {
}
void Fade(std::int32_t frames, bool toBlack) {
    SetScreenFilmFade(frames, toBlack ? 0 : 255, 0, 0, 0, toBlack ? 255 : 0, 0, 0, 0);
}
void Continue(HLib::TaskCallback callback, std::int32_t delay = 0) {
    HLib::SetCurrentTaskTo(callback);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void Sound(std::uint32_t sound, std::int32_t delay = 0) {
    EVT_AT_SetSound(sound, delay);
}
void Voice(const char *voice, std::int32_t delay) {
    EVT_AT_SetVoice(voice, 1, delay);
}
void Camera(std::int32_t id, std::int32_t delay = 0) {
    EVT_AT_SetCam(id, dummy, dummy, 0, 0, delay);
}
MotionWork *Motion(shen_name name = ryo) {
    return scenFindMotionwork(name);
}
void ConfigureMotion(std::uint16_t animation) {
    mtwkRequestMotion(Motion(), animation, MotionRegion::LowerBody);
    mtwkSetExtraFlag(Motion(), 13, 0);
    mtwkSetExtraFlag(Motion(), 17, 0);
    SetMotionRegionFlag(Motion(), 71, 0, MotionRegion::LowerBody);
}
void SetupLeaf(std::int32_t index) {
    EV_AT_SetPos(leafNames[index], local, leafOrigin);
    EV_AT_SetAngY(leafNames[index], local, leafRotation);
    EV_DisableCharacterDisplay(leafNames[index]);
    auto &leaf = work.leaves[index];
    leaf.position = {leafOrigin.x + GetRandomNumberInRange(1), leafOrigin.y, leafOrigin.z};
    leaf.direction = {};
    leaf.rotation = leafRotation;
    leaf.phase = 0;
    leaf.step = static_cast<std::int32_t>(shenGetRandomInt32(512));
    if (leaf.step < 256)
        leaf.step += 256;
    leaf.limit = static_cast<std::int32_t>(shenGetRandomInt32(15)) << 12;
    leaf.accumulated = 0;
    leaf.reserved34 = leaf.inside = leaf.state = leaf.blur = 0;
    leaf.delay = 30 * static_cast<std::int32_t>(shenGetRandomInt32(10));
    leaf.alpha = 1;
    ResetCharaFixo(FindCharacterByScenarioName(leafNames[index]));
}
void SetupAllLeaves() {
    for (std::int32_t i = 20; i--;)
        SetupLeaf(i);
}
void SetupRyo() {
    AT_SetLocalBaseChara(ryo, dummy);
    EV_EnableCharacterDisplay(ryo);
    EV_SetCharaControllerFlags(ryo, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(Motion(), 6, 2);
    EV_SetCharacterAlphaEnabled(ryo, false);
    (void)GetCharacterMotionCompatibilityResult(FindCharacterByScenarioName(ryo));
    (void)HasCharacterMotionWork(FindCharacterByScenarioName(ryo));
    EV_AT_SetPos(ryo, local, Vector3{0, 0, 3});
    EV_AT_SetAngY(ryo, local, Vector3i{});
    SetFaceFlagsForChara(FindCharacterByScenarioName(ryo), 1);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ryo), 0, reinterpret_cast<std::byte *>(readyHand), 0);
}
void ConfigureAuthorSubjects() {
    AT_SetLocalBaseChara(ryo, invalid);
    EV_DisableCharacterDisplay(rude);
    constexpr shen_name subjects[]{ryo, SHEN_NAME(SYE_), SHEN_NAME(SRY_), SHEN_NAME(a115), SHEN_NAME(a054), SHEN_NAME(a055), SHEN_NAME(a092), SHEN_NAME(a536),
        SHEN_NAME(a538), SHEN_NAME(a057), SHEN_NAME(a404), SHEN_NAME(a405), SHEN_NAME(a403), SHEN_NAME(a406)};
    for (std::size_t i = 0; i < std::size(subjects); ++i) {
        EV_SetCharaControllerFlags(subjects[i], UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(subjects[i], UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        if (i < 3)
            mtwkSetExtraFlag(Motion(subjects[i]), 6, 2);
        EV_SetCharacterAlphaEnabled(subjects[i], false);
    }
    EV_DisableCharacterDisplay(kun);
}
void RestoreSubject(shen_name character) {
    EV_DisableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    mtwkSetExtraFlag(Motion(character), 6, 0);
    EV_SetCharacterAlphaEnabled(character, true);
}
void RestoreAuthorSubjects(bool includeFourth) {
    RestoreSubject(ryo);
    RestoreSubject(SHEN_NAME(SYE_));
    RestoreSubject(SHEN_NAME(SRY_));
    constexpr shen_name subjects[]{SHEN_NAME(a115), SHEN_NAME(a054), SHEN_NAME(a055), SHEN_NAME(a092), SHEN_NAME(a536), SHEN_NAME(a538), SHEN_NAME(a057)};
    for (shen_name character : subjects) {
        if (character.name == SHEN_NAME(a092) && !includeFourth)
            continue;
        EV_DisableCharacterDisplay(character);
        EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(character, true);
    }
    EV_EnableCharacterDisplay(kun);
}
void CameraUpdate(CameraRuntime *) {
    std::int32_t x, y;
    if (cameraHold > 0) {
        x = lastLookX;
        y = lastLookY;
        cameraHold = std::max(cameraHold - 1, 0);
    } else {
        x = static_cast<std::int32_t>(GetCurrentControlInput(5)) - 128;
        y = static_cast<std::int32_t>(GetCurrentControlInput(6)) - 128;
        lastLookX = x;
        lastLookY = y;
    }
    const auto &side = TruncateToInt32((((10922 - cameraFov) / 182.04445f) / 49.998779f) * 100) >= 70 ? cameraOtherSide : cameraSide;
    Vector3 position{side.x * static_cast<float>(x) * .0078125f + cameraCentre.x + cameraOffset.x,
        cameraCentre.y + cameraOffset.y - static_cast<float>(y) * .4f * .0078125f, side.z * static_cast<float>(x) * .0078125f + cameraCentre.z};
    Vector3 previous;
    camCtrlGetTargetPosition(&previous);
    position = {((position.x - previous.x) / 28) + previous.x, ((position.y - previous.y) / 28) + previous.y, ((position.z - previous.z) / 28) + previous.z};
    camCtrlSetTargetPosition(position);
    if (cameraZoom == 1) {
        if (cameraFov > 1820)
            cameraFov -= 91;
        else {
            cameraFov = 1820;
            cameraZoom = 0;
        }
    } else if (cameraZoom == 2) {
        if (cameraFov < 10922)
            cameraFov += 273;
        else {
            cameraFov = 10922;
            cameraZoom = 0;
        }
    }
    camCtrlSetFov(static_cast<std::int16_t>(cameraFov));
}
void InitializeCamera(CameraRuntime *camera) {
    camCtrlGetNextCamPos(&cameraEye);
    camCtrlGetTargetPosition(&cameraCentre);
    Vector3 difference{cameraCentre.x - cameraEye.x, cameraCentre.y - cameraEye.y, cameraCentre.z - cameraEye.z};
    const float length = std::sqrt((difference.y * difference.y + difference.x * difference.x) + difference.z * difference.z);
    if (length > 0) {
        difference.x *= 1 / length;
        difference.y *= 1 / length;
        difference.z *= 1 / length;
    }
    cameraCentre = {cameraEye.x + difference.x, cameraEye.y + difference.y, cameraEye.z + difference.z};
    camCtrlSetTargetPosition(cameraCentre);
    cameraFov = 10922;
    cameraOtherSide = cameraSide = {difference.z * -1.4f, difference.y, difference.x * 1.4f};
    cameraOffset = {};
    lastLookX = lastLookY = cameraHold = cameraZoom = 0;
    camCtrlSetUpdateCallback(CameraUpdate);
    CameraUpdate(camera);
}
void MoveLeaves(void *) {
    for (std::int32_t i = 20; i--;) {
        auto &leaf = work.leaves[i];
        const shen_name name = leafNames[i];
        if (leaf.state == 3) {
            Vector3 position, point;
            Vector3i rotation;
            EV_GetCharaPosition(rude, CHARACTER_TRANSLATION_ROOT, &position, recursive);
            GetScenarioCharacterEulerAngles(rude, &rotation, recursive);
            EV_scenApplyPosOrRotToPartOf(rude, 152, &point, nullptr, EventCharacterPartTransformMode::Get);
            ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
            EV_AT_SetPos(name, local | recursive, point);
            EV_AT_SetAngY(name, local | recursive, rotation);
        } else if (leaf.state == 2) {
            Vector3i target, rotation;
            GetScenarioCharacterEulerAngles(rude, &target, EventTransformFlags::Local);
            GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::Local);
            if (work.approach) {
                auto blend = [&](std::int32_t current, std::int32_t to) {
                    float angle = (static_cast<std::int32_t>(static_cast<std::uint16_t>(current)) - static_cast<std::int32_t>(static_cast<std::uint16_t>(to))) /
                        182.04445f;
                    if (angle > 180)
                        angle = 360 - angle;
                    return current + TruncateToInt32(((angle / static_cast<float>(work.approach)) * 8192) / 45);
                };
                rotation = {blend(rotation.x, target.x), blend(rotation.y, target.y), blend(rotation.z, target.z)};
            } else
                rotation = target;
            EV_AT_SetAngY(name, local, rotation);
        } else if (leaf.state == 1) {
            Vector3 direction{1, 0, 0};
            TransformScenarioDirection(nullptr, &leaf.rotation, &direction);
            leaf.direction = direction;
            const float cross = (direction.x * 0 - direction.y) + direction.z * 0;
            leaf.position.y -= std::fabs(cross * .009f) + .003f;
            const float sine = sin_fixed16(static_cast<std::int16_t>(leaf.phase));
            leaf.phase += leaf.step;
            leaf.accumulated += leaf.step;
            leaf.position.x += sine * .007f;
            EV_AT_SetPos(name, local, leaf.position);
            leaf.rotation.x += 2 * leaf.step;
            leaf.rotation.y += 2 * leaf.step;
            leaf.rotation.z += 2 * leaf.step;
            EV_AT_SetAngY(name, local, leaf.rotation);
            if (work.alphaMode == 1)
                leaf.alpha -= .0083f;
            else if (work.alphaMode == 2)
                leaf.alpha -= .016f;
            if (leaf.alpha < 0)
                leaf.alpha = 0;
            modelSetCharaAlpha(FindCharacterByScenarioName(name), leaf.alpha);
            if (leaf.position.y < .5f)
                SetupLeaf(i);
            if (leaf.accumulated >= leaf.limit) {
                leaf.accumulated = 0;
                leaf.step = static_cast<std::int32_t>(shenGetRandomInt32(512));
                if (leaf.step < 256)
                    leaf.step += 256;
                leaf.limit = static_cast<std::int32_t>(shenGetRandomInt32(15)) << 12;
            }
        } else if (leaf.state == 0) {
            leaf.direction = {};
            --leaf.delay;
            if (name.name == SHEN_NAME(HAPJ)) {
                if (work.duckTokens) {
                    leaf.state = 1;
                    EV_EnableCharacterDisplay(name);
                    Sound(189);
                    work.duckTokens = std::max(work.duckTokens - 1, 0);
                    Sound(3102);
                    Sound(3102, 10);
                } else
                    SetupLeaf(i);
            } else if (leaf.delay <= 0) {
                leaf.state = 1;
                EV_EnableCharacterDisplay(name);
                Sound(189);
            }
        }
    }
}
std::int32_t FindHit(const Vector3 &eye) {
    std::int32_t count = 0;
    float firstDistance = 0;
    for (std::int32_t i = 20; i--;)
        if (work.leaves[i].inside) {
            Vector3 point;
            EV_GetCharaPosition(leafNames[i], CHARACTER_TRANSLATION_ROOT, &point, recursive);
            const float distance =
                std::sqrt((eye.x - point.x) * (eye.x - point.x) + (eye.y - point.y) * (eye.y - point.y) + (eye.z - point.z) * (eye.z - point.z));
            if (count) {
                if (firstDistance > distance)
                    work.selected = i;
            } else {
                firstDistance = distance;
                work.selected = i;
            }
            ++count;
        }
    return count;
}
void InspectLeaves() {
    const auto old = visibleState;
    visibleState = 0;
    for (std::int32_t i = 20; i--;) {
        Vector3 point;
        EV_GetCharaPosition(leafNames[i], CHARACTER_TRANSLATION_ROOT, &point, EventTransformFlags::Local);
        const float factor = std::min(100.0f, 101 - static_cast<float>(zoomPercent) * 1.4f) / 100;
        const float x = 640.0f / 3.2f * factor, y = 480.0f / 3.2f * factor;
        EV_GetCharaPosition(leafNames[i], CHARACTER_TRANSLATION_ROOT, &point, recursive);
        Vector2 screen;
        ProjectWorldToScreen(&point, &screen);
        work.leaves[i].inside = screen.x >= x && screen.x <= 639 - x && screen.y >= y && screen.y <= 479 - y;
        if (work.leaves[i].inside)
            visibleState = 1;
    }
    if (visibleState == 1) {
        noVisible = 0;
        if (old)
            visibleState = old;
    } else if (!visibleState) {
        Sound(959);
        Sound(961);
        noVisible = 1;
    }
}
void UpdateBlur() {
    for (std::int32_t i = 20; i--;) {
        auto &leaf = work.leaves[i];
        if (leaf.state == 1 && zoomPercent >= 90 && visibleState) {
            CheckHelpIcon(4, 0, CheckHelpIconOp::FrameOverlay);
            figpAssignModeFor(leafNames[i], 1);
            figpSetFlagsBlendForBlur(leafNames[i], 10, 1);
            if (!leaf.blur) {
                Sound(2419);
                leaf.blur = 1;
            }
        } else {
            figpSetFlagModesFor(leafNames[i], 2, 1);
            leaf.blur = 0;
        }
    }
}
void UpdateZoom() {
    std::int32_t delay = zoomDelay;
    if (noVisible == 1) {
        if (previousZoom != 2 && zoomPercent) {
            cameraHold = 10;
            cameraZoom = 2;
            delay = 30;
        }
    } else if (!delay) {
        const auto x = GetCurrentControlInput(5), y = GetCurrentControlInput(6);
        if (x != 128 && y != 128) {
            if (zoomPercent != 100) {
                cameraZoom = 1;
                cameraHold = 0;
            }
            if (zoomPercent >= 90 && visibleState == 1)
                visibleState = 2;
        }
        if (previousZoom == 1) {
            Vector3 eye;
            camCtrlGetTargetPosition(&eye);
            if (FindHit(eye)) {
                if (misses) {
                    Vector3 point;
                    EV_GetCharaPosition(leafNames[work.selected], CHARACTER_TRANSLATION_ROOT, &point, recursive);
                    const float inverse = 1 / static_cast<float>(6 - misses);
                    cameraOffset.x = (point.x - eye.x) * inverse;
                    cameraOffset.y = (point.y - eye.y) * inverse;
                } else
                    cameraOffset.x = cameraOffset.y = 0;
            }
        }
    }
    zoomDelay = std::max(delay - 1, 0);
}
void AimHand(bool hit, Vector3 &handPosition) {
    Vector3 eye, origin, target, endpoint;
    camCtrlGetNextCamPos(&origin);
    camCtrlGetTargetPosition(&eye);
    std::int32_t yaw = static_cast<std::int16_t>(atan2_fixed16(-(eye.x - origin.x), -(eye.z - origin.z)));
    if (hit)
        EV_GetCharaPosition(leafNames[work.selected], CHARACTER_TRANSLATION_ROOT, &target, recursive);
    else {
        const auto percent = TruncateToInt32((((10922 - cameraFov) / 182.04445f) / 49.998779f) * 100);
        target = eye;
        const float offset = static_cast<float>(100 - percent) / 500 + .12f;
        target.x -= cos_fixed16(static_cast<std::uint16_t>(GetCurrentControlInput(5) << 7)) * offset;
        target.y += cos_fixed16(static_cast<std::uint16_t>(GetCurrentControlInput(6) << 7)) * offset;
    }
    work.caughtHeight = target.y;
    Vector2 screen;
    ProjectWorldToScreen(&target, &screen);
    const float vertical = 240 - screen.y;
    if (static_cast<float>(yaw) / 182.04445f >= 90)
        yaw = 0;
    Vector3i rotation{TruncateToInt32(((vertical / 7) * 8192) / 45),
        TruncateToInt32((((static_cast<float>(yaw) / 182.04445f) / 1.5f) + (20 + (640 - screen.x) * .0625f)) * 8192 / 45), 0};
    EV_AT_SetAngY(rude, local | recursive, rotation);
    EV_scenApplyPosOrRotToPartOf(rude, 152, &endpoint, nullptr, EventCharacterPartTransformMode::Get);
    ApplyScenarioTranslationAndRotation(&handPosition, &rotation, &endpoint);
    work.delta = {target.x - endpoint.x, target.y - endpoint.y, target.z - endpoint.z};
    const float divisor = work.approach ? static_cast<float>(work.approach) : 1;
    handPosition = {handPosition.x + work.delta.x / divisor, handPosition.y + work.delta.y / divisor, handPosition.z + work.delta.z / divisor};
    EV_AT_SetPos(rude, local | recursive, handPosition);
}
void AttachCaughtLeaves() {
    std::int32_t attached = 0;
    for (std::int32_t i = 20; i--;)
        if (work.leaves[i].state == 3) {
            AttachScenarioCharacter(leafNames[i], ryo, 14, &attachments[attached].position, &attachments[attached].rotation);
            work.leaves[i].state = -1;
            ++attached;
            EV_debug_log("WB88\\eWB88_et_WB88_move.c", 309, "Mode4 - %d!!\n", i);
        }
}
void GoodVoice(std::int32_t replay) {
    if (!(replay & 1))
        return;
    if (EV_GetShenmueFlag(0xa3) && work.duckCaught)
        Voice("0037A044", 12); // @text EN: "Damn=@" | JA: "あっ…"
    else {
        constexpr const char *voices[]{"0037A041", "0037A042", "0037A043"}; // @text 0037A041 EN: "Good!" | JA: "よし！" ; 0037A042 EN: "I did it!" | JA: "やった！" ; 0037A043 EN: "I got it." | JA: "とれた！"
        Voice(voices[shenGetRandomInt32(3)], 12);
        if (EV_GetShenmueFlag(0x2c6)) {
            if (!work.round)
                Voice(shenGetRandomInt32(2) ? "0037A048" : "0037A047", 70); // @text 0037A048 EN: "Two more leaves." | JA: "あと２枚だ。" ; 0037A047 EN: "Two more times." | JA: "あと２回か。"
            else if (work.round == 1)
                Voice(shenGetRandomInt32(2) ? "0037A050" : "0037A049", 70); // @text 0037A050 EN: "If I can catch just one more=@" | JA: "もう１枚とれば…" ; 0037A049 EN: "One more time." | JA: "あと１回だ。"
        }
    }
}
void BadVoice() {
    const auto first = shenGetRandomInt32(3);
    Voice(first == 0 ? "0037A045" : first == 1 ? "0037A046" : "0037A044", 12); // @text 0037A045 EN: "No=@" | JA: "くそっ…" ; 0037A046 EN: "No good." | JA: "ダメだ…" ; 0037A044 EN: "Damn=@" | JA: "あっ…"
    if (EV_GetShenmueFlag(0x2c6) && work.round == 2) {
        const auto second = shenGetRandomInt32(2);
        Voice(!first || second ? "0037A052" : "0037A051", 70); // @text 0037A052 EN: "Have to start all over." | JA: "１からやり直しか…" ; 0037A051 EN: "Damn.\nThat was supposed to be the last one." | JA: "くそ…あと１回だったのに。"
    } else {
        constexpr const char *voices[]{"0037A053", "0037A054", "0037A055"}; // @text 0037A053 EN: "Have to concentrate on the leaves." | JA: "葉っぱから目をそらしたら\nダメだな…" ; 0037A054 EN: "Only go for a single leaf at a time." | JA: "１枚の葉っぱだけを、\n追わないとな…" ; 0037A055 EN: "The timing for catching the leaf\nis important=@" | JA: "葉っぱをつかむ\nタイミングさえわかれば…"
        Voice(voices[shenGetRandomInt32(3)], 70);
    }
}
std::int32_t PlayReplay(std::int32_t index) {
    const ReplayShot &shot = shots[index];
    Camera(shot.camera + (index >= 4 ? static_cast<std::int32_t>(shenGetRandomInt32(3)) : 0));
    LoadBlurModelForCharacter(ryo, nullptr, 0);
    EVT_AT_DispBlur(ryo, 4, 7, 3, index >= 6 ? 5 : index >= 4 ? 15 : 27, 0);
    if (index >= 4)
        Sound(962, 10);
    else {
        const auto length = TruncateToInt32(shot.end - shot.begin);
        std::int32_t duration = length;
        if (index & 1) {
            duration = work.entry == 2 ? length + 31 : (!work.round && EV_GetShenmueFlag(0x2f)) || work.round == 2 ? length + 34 : length + 61;
        }
        EVT_AT_SetPoint(ryo, ryo, 14, 5, 0, 0, duration, 0x20000000U);
        Sound(555);
        for (std::int32_t i = caughtCount; i--;)
            Sound(554, 2 * i + 6);
    }
    EV_EnableCharacterDisplay(ryo);
    mtwkRequestMotion(Motion(), static_cast<std::uint16_t>(shot.animation), MotionRegion::LowerBody);
    SetMotionRegionEvalTime(Motion(), shot.begin, MotionRegion::LowerBody);
    if (index >= 4) {
        Queue_EVT_AT_CtrlHandUp_1(ryo, 0, reinterpret_cast<std::byte *>(neutralHandPoseCommand), 40, 70);
        Queue_EVT_AT_CtrlHandUp_1(ryo, 0, reinterpret_cast<std::byte *>(relaxedHand), 20, 85);
        BadVoice();
    } else {
        SetMotionLayerEndFrame(Motion(), shot.end, MotionRegion::LowerBody);
        GoodVoice(index);
    }
    mtwkSetExtraFlag(Motion(), 17, 0);
    mtwkSetExtraFlag(Motion(), 13, 0);
    SetMotionRegionFlag(Motion(), 71, 0, MotionRegion::LowerBody);
    EV_DisableCharacterDisplay(rude);
    return TruncateToInt32(shot.end - shot.begin);
}
void StartReplay(std::int32_t index) {
    SetupRyo();
    if (index)
        work.alphaMode = 2;
    else {
        AttachCaughtLeaves();
        work.alphaMode = 1;
    }
    Sound(959);
    Sound(961);
    (void)GetCurrentControlInput(6);
    Vector3 point;
    EV_GetCharaPosition(leafNames[work.selected], CHARACTER_TRANSLATION_ROOT, &point, recursive);
    work.replay = index;
    StepNullsub();
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    Continue(Replay);
    HLib::SetupTaskDelay(PlayReplay(index));
}
void CatchLoop(void *) {
    work.leaves[work.selected].inside = 0;
    Vector3 position;
    EV_GetCharaPosition(rude, CHARACTER_TRANSLATION_ROOT, &position, recursive);
    AimHand(true, position);
    if (--work.approach < 0) {
        figpSetFlagModesFor(leafNames[work.selected], 2, 1);
        work.leaves[work.selected].state = 3;
        if (leafNames[work.selected].name == SHEN_NAME(HAPJ))
            work.duckCaught = 1;
        camCtrlGetTargetPosition(&position);
        const auto count = EV_GetShenmueFlag(0xa3) ? FindHit(position) : 0;
        Sound(554);
        if (count) {
            Vector3 endpoint, next;
            EV_GetCharaPosition(rude, CHARACTER_TRANSLATION_ROOT, &position, recursive);
            EV_scenApplyPosOrRotToPartOf(rude, 152, &endpoint, nullptr, EventCharacterPartTransformMode::Get);
            const Vector3i rotation{};
            ApplyScenarioTranslationAndRotation(&position, &rotation, &endpoint);
            EV_GetCharaPosition(leafNames[work.selected], CHARACTER_TRANSLATION_ROOT, &next, recursive);
            ++caughtCount;
            work.delta = {next.x - endpoint.x, next.y - endpoint.y, next.z - endpoint.z};
            work.approach = 3;
            EV_debug_log("WB88\\eWB88_et_WB88.c", 825, "Hit%d!!\n", caughtCount + 1);
            if (caughtCount >= 2 && ++work.caughtChain >= 2 && EV_GetShenmueFlag(0x464)) {
                work.duckTokens += 2;
                Sound(1109, 60);
                EV_debug_log("WB88\\eWB88_et_WB88.c", 832, "がぁがぁモードＯＮ %d\n", work.duckTokens);
            }
        } else
            Continue([](void *) { StartReplay(0); }, 1);
    }
}
void MissLoop(void *) {
    Vector3 position;
    EV_GetCharaPosition(rude, CHARACTER_TRANSLATION_ROOT, &position, recursive);
    AimHand(false, position);
    if (--work.approach < 0)
        Continue([](void *) { StartReplay(6); }, 1);
}
void StartCatch() {
    cameraZoom = cameraHold = 0;
    EVS_LoopCamera(1017, shen_name{}, shen_name{});
    EV_EnableCharacterDisplay(rude);
    Vector3 eye, origin;
    camCtrlGetTargetPosition(&eye);
    camCtrlGetNextCamPos(&origin);
    const auto hit = FindHit(eye);
    (void)atan2_fixed16(-(eye.x - origin.x), -(eye.z - origin.z));
    if ((zoomPercent >= 90 && visibleState && hit) || g_forcePlayerGodmode) {
        misses = std::max(misses - 1, 0);
        Vector3 position{origin.x + .12f, eye.y - .11f, 234.39999f};
        work.leaves[work.selected].state = 2;
        work.approach = 5;
        caughtCount = 1;
        EV_AT_SetPos(rude, local | recursive, position);
        AimHand(true, position);
        Continue(CatchLoop);
        Sound(555);
        LoadBlurModelForCharacter(rude, nullptr, 0);
        figpAssignModeFor(rude, 1);
        figpSetFlagsBlendForBlur(rude, 5, 2);
        EV_debug_log("WB88\\eWB88_et_WB88.c", 708, "Hit!!\n");
    } else {
        misses = std::min(misses + 1, 5);
        if (zoomPercent < 55) {
            StartReplay(4);
            EV_debug_log("WB88\\eWB88_et_WB88.c", 734, "------NG2\n");
        } else {
            work.approach = 5;
            caughtCount = 0;
            Vector3 position{origin.x + .12f, eye.y - .11f, 234.39999f};
            work.caughtChain = 0;
            EV_AT_SetPos(rude, local | recursive, position);
            AimHand(false, position);
            Continue(MissLoop);
            Sound(962);
            EV_debug_log("WB88\\eWB88_et_WB88.c", 728, "------NG1\n");
        }
    }
}
void MainLoop(void *) {
    SetXInputAnalogProcessing(1);
    (void)GetCurrentControlInput(5);
    (void)GetCurrentControlInput(6);
    --work.remainingFrames;
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour >= 19)
        work.remainingFrames = 0;
    if (zoomPercent <= 20 && work.remainingFrames <= 0) {
        Fade(30, true);
        Continue(Exit, 30);
        return;
    }
    InspectLeaves();
    UpdateBlur();
    previousZoom = cameraZoom;
    zoomPercent = TruncateToInt32((((10922 - cameraFov) / 182.04445f) / 49.998779f) * 100);
    UpdateZoom();
    if (GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
        Fade(30, true);
        Continue(PreExit, 30);
    } else {
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        if (GetBattleDataConfig(2, SHEN_NAME(BTBA)) || static_cast<std::int8_t>(g_inputBuffer[0].held) < 0)
            StartCatch();
        else
            GetBattleDataConfig(0, SHEN_NAME(BTBA));
    }
}
void MainInit2(void *) {
    FinishLoading();
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    work.alphaMode = 0;
    HLib::ResolveDeferredCallback(work.moveTask);
    EV_LoadAudio(shenGetCurrentSbt(), GetSelectedSoundSceneIndex());
    camCtrlSetUpdateCallback(InitializeCamera);
    camCtrlSetState(CameraState::Callback);
    Continue(MainLoop);
}
void MainInit(void *) {
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour >= 19) {
        ProcessAicaCommand(0xaa0, 30);
        Continue(Exit, 30);
        return;
    }
    SetupRyo();
    EV_DisableCharacterDisplay(ryo);
    EV_AT_SetPos(ryo, local, Vector3{0, 0, 3});
    EV_AT_SetAngY(ryo, local, Vector3i{});
    EV_DisableCharacterDisplay(rude);
    EV_AT_SetPos(rude, local, Vector3{.4f, 1.2f, 3.2f});
    EV_AT_SetAngY(rude, local, Vector3i{});
    SetupAllLeaves();
    zoomDelay = noVisible = visibleState = 0;
    EVS_LoopCamera(1000, shen_name{}, shen_name{});
    Continue(MainInit2);
    Fade(30, false);
    if (work.showHelp) {
        auto *task = HLib::EnqueueTaskWithoutParameterWrapper(
            [](void *) {
                SetupCurrentHelpIcon();
                HLib::DestroyCurrentTask();
            },
            0xc, static_cast<std::int32_t>(SHEN_NAME(EVNT)), nullptr);
        HLib::SetupTimedTask(task, 30);
        work.showHelp = 0;
    }
}
void SkipOpening(void *) {
    SetupRyo();
    EV_AT_SetPos(ryo, local, Vector3{0, 0, 3});
    EV_AT_SetAngY(ryo, local, Vector3i{});
    mtwkSetExtraFlag(Motion(), 17, 0);
    mtwkSetExtraFlag(Motion(), 6, 2);
    mtwkRequestMotion(Motion(), 4107, MotionRegion::LowerBody);
    mtwkSetExtraFlag(Motion(), 13, 0);
    SetMotionRegionFlag(Motion(), 71, 0, MotionRegion::LowerBody);
    EV_DisableCharacterDisplay(rude);
    EVS_LoopCamera(1018, shen_name{}, shen_name{});
    Fade(30, false);
    if (EV_GetShenmueFlag(0xa3) && EV_GetShenmueFlag(0x464))
        Sound(3104, 180);
    Continue(MainInit, 300);
}
void Opening(void *) {
    EV_DisableCharacterDisplay(kun);
    if (work.entry)
        SkipOpening(nullptr);
    else {
        Continue(AuthorMain);
        work.authorFlags = 1;
        work.scene = 0;
        work.returnCallback = MainInit;
    }
    work.showHelp = 0;
}
void PreExit(void *) {
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour >= 19) {
        Continue(Exit);
        return;
    }
    Fade(30, false);
    std::int16_t choices[]{-1, 152, 34, -1};
    SetupControlSpeakAndUI(choices, 10800, -1, 2);
    Camera(1200);
    camCtrlDisableFilmBars();
    Continue(PreExit2, 30);
}
void PreExit2(void *) {
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        if (shenGetCtrlSpeakIconFlag() == 152)
            Continue(MainInit);
        else {
            Continue(Exit);
            ProcessAicaCommand(0xaa0, 30);
        }
        Fade(30, true);
        HLib::SetupTaskDelay(30);
    }
}
void Exit(void *) {
    EV_debug_log("WB88\\eWB88_et_WB88.c", 929, "EXIT come on!!\n");
    EV_DisableCharacterDisplay(rude);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_EventSetUpHelpIcon_0(0, 0);
    FinishLoading();
    for (std::int32_t i = 20; i--;)
        AT_SetLocalBaseChara(leafNames[i], invalid);
    AT_SetLocalBaseChara(ryo, invalid);
    EV_SetCharaControllerFlags(ryo, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    mtwkSetExtraFlag(Motion(), 6, 0);
    EV_SetCharacterAlphaEnabled(ryo, true);
    (void)HasCharacterMotionWork(FindCharacterByScenarioName(ryo));
    UpdateMotionWorkWithPosition(Motion(), nullptr, 0);
    EV_EnableCharacterDisplay(ryo);
    (void)HasCharacterMotionWork(FindCharacterByScenarioName(ryo));
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ryo), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 1, 0);
    mtwkRequestMotion(Motion(), 61443, MotionRegion::LowerBody);
    mtwkSetExtraFlag(Motion(), 13, 0);
    SetMotionRegionFlag(Motion(), 71, 2, MotionRegion::LowerBody);
    EV_FreeAuthorPak(0, "0037");
    LoadCutsceneAndSoundProgramForArea();
    EV_LoadAudio(shenGetCurrentSbt(), GetSelectedSoundSceneIndex());
    HLib::DestroyTaskInternal(work.moveTask);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WB88"), 0);
    for (const shen_name character : characters) {
        if (character.name == invalid.name)
            break;
        if (FindCharacterByScenarioName(character)) {
            EV_npcLockUnlock(character);
            EV_npcFree(character);
        }
    }
    EV_NPC_NewLoadSw(1);
    Continue(Exit2);
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(static_cast<CameraUtilityMode>(0), 0);
    camCtrlSetState(CameraState::Init);
    Fade(30, false);
    HLib::SetupTaskDelay(30);
}
void Exit2(void *) {
    SetPlayerControlEnabled(1);
    EV_debug_log("WB88\\eWB88_et_WB88.c", 983, "EXITEXIT\n");
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
}
void AuthorCheck(void *) {
    if (work.round == 1 && EV_GetShenmueFlag(0x2f)) {
        work.round = 0;
        EV_stepSetFlags(0, 0x2f);
        Continue(AuthorMain);
        work.authorFlags = 1;
        work.returnCallback = MainInit;
        work.scene = 4;
        return;
    }
    if (work.round == 1 || work.round == 2) {
        Continue(MainInit);
        Fade(30, true);
        HLib::SetupTaskDelay(30);
        return;
    }
    Continue(AuthorMain);
    work.returnCallback = Exit;
    work.authorFlags = 1;
    work.scene = 1;
    if (work.entry == 0 || work.entry == 1) {
        if (!work.entry)
            EV_stepSetFlags(0, 0xa2);
        EV_stepSetFlags(0, 0x2c6);
        EV_stepSetFlags(0, 0x23f);
    }
}
void Replay(void *) {
    if (shots[work.replay].next) {
        ++work.replay;
        HLib::SetupTaskDelay(PlayReplay(work.replay));
        Sound(963, 55);
        return;
    }
    figpSetFlagModesFor(ryo, 2, 1);
    bool fade = true;
    if (work.replay >= 4) {
        work.round = 0;
        Continue(MainInit);
    } else if (work.entry >= 2) {
        Continue(EV_GetShenmueFlag(0xa3) && work.duckCaught ? Auto1 : MainInit);
    } else {
        ++work.round;
        Continue(AuthorCheck);
        fade = EV_GetShenmueFlag(0x2f) != 0;
    }
    if (fade)
        Fade(30, true);
    HLib::SetupTaskDelay(30);
}
void BeginAuthor() {
    constexpr HLib::TaskCallback callbacks[]{Author0, Author1, Author2, Author3, Author4};
    HLib::EnqueueTaskWithoutParameter(callbacks[work.scene], 4, 0xb, 0x41706168);
    work.currentAuthorFrame = 0;
    work.authorEffectHandle = 0;
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    if (work.scene <= 3)
        Fade(work.scene == 0 || work.scene == 2 ? 30 : 0, false);
    else if (work.scene == 4)
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    constexpr shen_name scenes[]{SHEN_NAME(AT00), SHEN_NAME(AT01), SHEN_NAME(AT02), SHEN_NAME(AT03), SHEN_NAME(AT04)};
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(scenes[work.scene].name));
    work.authorFlags = 2;
    HLib::SetupTimedTask(work.moveTask, 0);
    SetupAllLeaves();
}
void ReturnAuthor() {
    HLib::SetCurrentTaskTo(work.returnCallback);
    if (work.scene == 2) {
        EventSetPosition position{0, ryo, invalid, Vector3{340.79999f, 127, 231.8f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, ryo, invalid, Vector3i{0, 16384, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        ConfigureMotion(61573);
    } else if (work.scene == 3) {
        SetPlayerControlEnabled(1);
        camCtrlSetState(CameraState::Init);
        Fade(30, false);
    }
}
void AuthorMain(void *) {
    if (work.authorFlags == 1)
        BeginAuthor();
    else if (work.authorFlags == 3)
        ReturnAuthor();
}
void Author0(void *) {
    work.currentAuthorFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!work.currentAuthorFrame) {
        ConfigureAuthorSubjects();
        InitializeWarehouseFallingLeaves(&work.authorEffectHandle);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        FreeParticleEffect(3, work.authorEffectHandle);
        RestoreAuthorSubjects(false);
        work.authorFlags = 3;
        Fade(0, true);
        HLib::DestroyCurrentTask();
    }
}
void Author1(void *) {
    work.currentAuthorFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!work.currentAuthorFrame) {
        InitializeWarehouseFallingLeaves(&work.authorEffectHandle);
        ConfigureAuthorSubjects();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        FreeParticleEffect(3, work.authorEffectHandle);
        RestoreAuthorSubjects(false);
        work.authorFlags = 1;
        work.scene = 2;
        HLib::SetupTimedTask(work.self, 60);
        HLib::DestroyCurrentTask();
    }
    if (GetCurrentAuthorId().name == SHEN_NAME(AT01)) {
        const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 200)
            snapSetDesc(0);
    }
}
void RestoreSunnyWeather() {
    if (savedWeather == 2) {
        savedWeather = 0;
        ResetWeatherOverrides();
        FreeWAIR();
        EV_UpdateWeather();
    }
}
void Author2(void *) {
    work.currentAuthorFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!work.currentAuthorFrame) {
        ConfigureAuthorSubjects();
        for (std::int32_t index = 0; index < 8; ++index)
            EV_AssignLightParams(0, index);
        EV_AssignLightParams(0, 20);
    } else if (work.currentAuthorFrame == 1762) {
        SetIsSkySetup(0);
        SetWarehouseBooksVisible(false);
        savedWeather = static_cast<std::int16_t>(GetCurrentWeather());
        if (savedWeather == 2)
            UpdateWeatherForMinute(1, 0, 0, 0);
    } else if (work.currentAuthorFrame == 1928) {
        SetIsSkySetup(1);
        SetWarehouseBooksVisible(true);
        RestoreSunnyWeather();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetIsSkySetup(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        RestoreSunnyWeather();
        RestoreAuthorSubjects(true);
        work.authorFlags = 3;
        HLib::SetupTimedTask(work.self, 130);
        AddInventoryItem(14, 1, 1);
        Fade(0, true);
        HLib::DestroyCurrentTask();
    }
}
void Author3(void *) {
    work.currentAuthorFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!work.currentAuthorFrame) {
        ConfigureAuthorSubjects();
        InitializeWarehouseFallingLeaves(&work.authorEffectHandle);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        FreeParticleEffect(3, work.authorEffectHandle);
        RestoreAuthorSubjects(false);
        work.authorFlags = 3;
        HLib::SetupTimedTask(work.self, 30);
        Fade(30, true);
        HLib::DestroyCurrentTask();
    }
}
void Author4(void *) {
    work.currentAuthorFrame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!work.currentAuthorFrame) {
        InitializeWarehouseFallingLeaves(&work.authorEffectHandle);
        ConfigureAuthorSubjects();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        FreeParticleEffect(3, work.authorEffectHandle);
        RestoreAuthorSubjects(false);
        work.authorFlags = 3;
        HLib::SetupTimedTask(work.self, 30);
        HLib::DestroyCurrentTask();
    }
    if (GetCurrentAuthorId().name == SHEN_NAME(AT04)) {
        const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 120)
            snapSetDesc(0);
    }
}
void Auto1(void *) {
    Fade(30, false);
    EV_EnsureCharacterDraw(duck);
    EV_EnableCharacterDisplay(duck);
    ConfigureScenarioNodeMotion(duck, 7426, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(duck, 0, -1, -1, -1, 32767, 4);
    Camera(1100);
    camCtrlDisableFilmBars();
    Camera(1110, 120);
    Camera(1120, 420);
    Voice("0037A056", 60); // @text EN: "This is=@" | JA: "…んっ？　これは？"
    Sound(1109, 300);
    Sound(3104, 360);
    Voice("0037A057", 330); // @text EN: "That is=@" | JA: "あれは…"
    EventSetPoint point{ryo, ryo, 14, 5, 0, 270, 0x20000000U, 0, 0, 0};
    EVT_AT_SetPoint_Callback(&point);
    EVT_AT_SetPoint(ryo, duck, -1, 1, 0, 300, 120, 0x20000000U);
    EventChoiceIcon choice{1, {-1, 461, 24, -1}, 10800, 65535, 2, 420};
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_KsIcon_Callback, 6, 0xb, sizeof(choice), WB88_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<EventChoiceIcon *>(HLib::GetTaskParameter(task)) = choice;
    Continue(Auto2, 420);
}
void Auto2(void *) {
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        Continue(shenGetCtrlSpeakIconFlag() == 461 ? Auto3 : Exit);
        Fade(30, true);
        HLib::SetupTaskDelay(30);
    }
    Vector3 position;
    EV_GetCharaPosition(duck, CHARACTER_TRANSLATION_ROOT, &position, recursive);
    UpdateMotionWorkWithPosition(Motion(), &position, 1280);
    UpdateMotionWorkWithPosition(Motion(), nullptr, 3);
}
void Auto3(void *) {
    UpdateMotionWorkWithPosition(Motion(), nullptr, 0);
    SetupAllLeaves();
    Camera(1130);
    camCtrlDisableFilmBars();
    EV_AT_SetPos(ryo, local, Vector3{.1244f, 0, 2.8900001f});
    mtwkSetExtraFlag(Motion(), 17, 0);
    SetMotionRegionFlag(Motion(), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(Motion(), 13, 0);
    EVT_AT_SetMot(ryo, 306, 0, 1, 0, 0);
    (void)GetMotionFrameCount(306);
    Voice("0037A038", 30); // @text EN: "I don't have a choice." | JA: "やるしかないか。"
    Fade(30, false);
    Continue(Auto4, 60);
}
void Auto4(void *) {
    EVT_AT_DispBlur(ryo, 1, 7, 3, 70, 20);
    QueueCameraShake(.5f, .2f, .5f, 17);
    QueueCameraShake(.3f, .3f, .4f, 44);
    QueueCameraShake(.5f, .5f, .4f, 58);
    Sound(1109, 17);
    Sound(1109, 45);
    Sound(1109, 59);
    Sound(1821, 10);
    Sound(1821, 37);
    Sound(1821, 50);
    Camera(1150);
    camCtrlDisableFilmBars();
    Camera(1151, 165);
    Voice("0037A045", 130); // @text EN: "No=@" | JA: "くそっ…"
    EVT_AT_SetMot(ryo, 155, 0, 1, 0, 0);
    const auto frames = static_cast<std::int32_t>(GetMotionFrameCount(155));
    Queue_EVT_AT_CtrlHandUp_1(ryo, 0, reinterpret_cast<std::byte *>(relaxedHand), 18, 15);
    Queue_EVT_AT_CtrlHandUp_1(ryo, 1, reinterpret_cast<std::byte *>(relaxedHand), 18, 15);
    EventSetPoint point{ryo, ryo, -1, 5, 0, 0, 0x10000000U, 0, 0, 0};
    EVT_AT_SetPoint_Callback(&point);
    EVT_AT_SetPoint(ryo, duck, -1, 5, 0, 120, frames + 407, 0x40000000U);
    Continue(Auto5A, frames + 120);
}
void Auto5A(void *) {
    Sound(3099);
    Voice("0037A044", 20); // @text EN: "Damn=@" | JA: "あっ…"
    Continue(Auto5, 40);
}
void Auto5(void *) {
    EVT_AT_SetMot(ryo, 306, 0, 1, 0, 0);
    (void)GetMotionFrameCount(306);
    EV_AT_SetPos(duck, local, Vector3{331.8402099609375f, 130, 232.832f});
    EV_AT_SetAngY(duck, local, Vector3i{4551, 58345, 3640});
    ConfigureScenarioNodeMotion(duck, 7427, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(duck, 0, -1, -1, -1, 32767, 4);
    duckFall = {.01f, 0};
    Sound(3101, 3);
    Sound(3104);
    Camera(1160);
    camCtrlDisableFilmBars();
    Continue(Auto6);
}
void Auto6(void *) {
    Vector3 position;
    EV_GetCharaPosition(duck, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y -= duckFall.y + duckFall.x;
    duckFall.y += duckFall.x;
    if (position.y <= 127.13f) {
        Sound(3100);
        position.y = 127.13f;
        Continue(Auto7);
        EV_AT_SetAngY(duck, local, Vector3i{0, 1001, 0});
        ConfigureScenarioNodeMotion(duck, 7425, 0, 32767, 0, 1, 0);
        ConfigureScenarioNodeMotion(duck, 0, -1, -1, -1, 32767, 4);
        EV_stepSetFlags(0, 0x464);
        Camera(1140);
        camCtrlDisableFilmBars();
        Voice("0037A042", 210); // @text EN: "I did it!" | JA: "やった！"
        HLib::SetupTaskDelay(330);
    }
    EV_AT_SetPos(duck, local, position);
}
void Auto7(void *) {
    Continue(Exit);
    InitiateCleanupForSceneChara(duck);
    AddInventoryItem(80, 1, 1);
    Fade(30, true);
    HLib::SetupTaskDelay(60);
    ProcessAicaCommand(0xaa0, 30);
}
void Init() {
    std::memset(&work, 0, sizeof(work));
    noVisible = misses = visibleState = zoomPercent = zoomDelay = previousZoom = 0;
    cameraEye = cameraCentre = cameraOffset = cameraSide = cameraOtherSide = {};
    cameraFov = cameraZoom = cameraHold = lastLookX = lastLookY = 0;
    savedWeather = 0;
    duckFall = {};
}
void Main(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    EV_debug_log("WB88\\eWB88_et_WB88.c", 150, "WB88-EVENT\n");
    work.self = HLib::EnqueueTaskWithoutParameter(Opening, 4, 0xb, 1299210600);
    work.moveTask = HLib::EnqueueTaskWithoutParameterWrapper(MoveLeaves, 0xc, static_cast<std::int32_t>(SHEN_NAME(EVNT)), nullptr);
    HLib::SetupTimedTask(work.moveTask, 0);
    work.entry = *reinterpret_cast<const std::int32_t *>(argument);
    work.selected = work.round = work.caughtChain = work.duckTokens = work.duckCaught = work.alphaMode = 0;
    work.remainingFrames = 10800;
    EV_EventSetUpHelpIcon_0(75, 0);
    for (std::int32_t i = 20; i--;) {
        AT_SetLocalBaseChara(leafNames[i], dummy);
        LoadBlurModelForCharacter(leafNames[i], nullptr, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("WB88"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("WB88"), 0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, "0037");
    EV_LoadCutsceneFromCurrentScene("0037A000");
    EV_LoadAudio("0037A000", 0);
    Queue_EVT_UTL_LoadNPCChara_3(characters, work.self);
    StepNullsub();
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
}
} // namespace
const ScriptAreaDefinition areaWB88ScriptDefinition{.areaId = SHEN_NAME(WB88), .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2::areas
