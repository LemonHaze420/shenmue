#include "s1/task/scen.hpp"
#include "s1/task/vfma.hpp"
#include "s1/input.hpp"
#include "s1/motion.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/time.hpp"
#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/lighting.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/btpk.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/vfcl.hpp"
#include "s1/task/vf_core.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/tmex.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>

#include "s1/task/vf_internal.hpp"

namespace shenmue::s1 {
using namespace vf_detail;

namespace vf_detail {


// @verified - p
std::uint8_t ResetAndSelectCameraPhaseChoice(VFActor &actor, std::uint8_t phase, std::int16_t waitFrames) {
    VFSlotRuntimeState *const slot = actor.slot;
    std::uint32_t updateCamera = slot->updateCamera;
    if ((updateCamera & 1U) == 0) {
        return static_cast<std::uint8_t>(updateCamera);
    }

    slot->updateCamera = updateCamera & ~1U;
    slot->cameraScriptWait = 0;
    slot->hpPhase = phase;
    slot->fielda8 = static_cast<std::uint16_t>(waitFrames);
    slot->cameraScriptStep = 0;
    slot->fieldb7 = 0;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    slot->field50 = static_cast<std::uint8_t>(g_EnvPrngSeed >> 16U);
    slot->byte54 = 0x7fU;
    slot->byte55 = 0x7fU;

    const std::uint8_t *row = static_cast<const std::uint8_t *>(actor.cameraPhaseTable);
    while (row[0] != 0x7fU && row[0] != phase)
        row += 17;
    if (row[0] == 0x7fU)
        return 0x7fU;

    std::uint8_t choiceCount = 0;
    for (std::uint8_t index = 0; index < 4U; ++index)
        choiceCount += row[1U + 4U * index] != 0x7fU;
    if (choiceCount == 0)
        return 0;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const std::size_t choice = ((g_EnvPrngSeed >> 16U) & 0xffU) % choiceCount;
    const std::uint8_t *const selected = row + 1U + 4U * choice;
    slot->byte54 = selected[0];
    slot->word56 = static_cast<std::uint16_t>((slot->word56 & 0x00ffU) | (static_cast<std::uint16_t>(selected[1]) << 8U));
    slot->byte55 = selected[2];

    slot->reserved58[0] = static_cast<std::byte>(selected[3]);
    return selected[3];
}


// @verified - p
std::uint16_t RotateVFCameraSector(const VFActor &actor, std::uint16_t value) {
    if (actor.slotIndex != static_cast<std::uint8_t>(g_pFightMain->playerSlot)) {
        return value;
    }
    const std::uint16_t sector = value & 0xf0U;
    if (sector == 0 || sector == 0xf0U)
        return value;
    const auto accumulatedYaw = static_cast<std::int16_t>(g_fCamAccumYaw);
    const auto cameraYaw = static_cast<std::int16_t>(accumulatedYaw + static_cast<std::int16_t>(actor.facingYaw) + 4096);
    const std::uint8_t cameraSector = static_cast<std::uint8_t>((cameraYaw >> 13) & 7);
    std::uint8_t sectorIndex = 0;
    while (g_vfCameraSectors[sectorIndex] != sector) {
        if (++sectorIndex >= 8) {
            sectorIndex = 0xffU;
            break;
        }
    }
    return static_cast<std::uint16_t>((value & 0xff0fU) | g_vfCameraSectors[(cameraSector + sectorIndex) & 7U]);
}


// @verified - p
bool UpdateVFCameraScript(VFActor &actor) {
    VFSlotRuntimeState *const slot = g_pFightMain->slotRuntime[static_cast<std::int8_t>(actor.slotIndex)];
    std::uint16_t wait = slot->cameraScriptWait;
    if (wait != 0) {
        if (wait != 1) {
            actor.cameraControlFlags = slot->savedCameraControlFlags;
            slot->cameraScriptWait = static_cast<std::uint16_t>(wait - 1U);
            return false;
        }
        const std::uint8_t previous = slot->cameraScriptStep++;
        if (previous == 10) {
            slot->cameraScriptWait = 0;
            return true;
        }
    }

    // Flat storage preserves step ten's overlap with the hold bytes and the following row.
    static constexpr std::uint8_t kNativeFallback[] = {
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        5,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x48,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        2,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x88,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        2,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x28,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        2,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x18,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        2,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x13,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x13,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x13,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x07,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0f,
        0x00,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x04,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x04,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x01,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0xf0,
        0xf1,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        3,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0xf0,
        0xf2,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        3,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0xf0,
        0xf3,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        3,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0xf0,
        0xfa,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        3,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x11,
        0x00,
        0x02,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x2c,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x2c,
        0x00,
        0x01,
        0x7f,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,

        0x04,
    };
    const std::uint8_t *table = static_cast<const std::uint8_t *>(actor.cameraScriptTable);
    std::size_t rowIndex = slot->hpPhase;
    if (slot->hpPhase >= 200U) {
        rowIndex -= 200U;
        const std::size_t lastRead = rowIndex * sizeof(VFCameraScriptRow) + 10U + slot->cameraScriptStep;
        if (lastRead >= std::size(kNativeFallback))
            return true;
        table = kNativeFallback;
    }
    const std::size_t stepOffset = rowIndex * sizeof(VFCameraScriptRow) + slot->cameraScriptStep;
    const std::uint8_t sector = table[stepOffset];
    if (sector == 0x7fU)
        return true;
    actor.cameraControlFlags = RotateVFCameraSector(actor, sector);
    slot->savedCameraControlFlags = actor.cameraControlFlags;
    slot->cameraScriptWait = table[stepOffset + 10U];
    return false;
}


// @verified - p
std::uint8_t AdvanceVFCameraPhaseChain(VFActor &actor) {
    VFSlotRuntimeState *const slot = actor.slot;
    if (slot->fielda8 != 0) {
        --slot->fielda8;
        return static_cast<std::uint8_t>(slot->fielda8);
    }
    if (!UpdateVFCameraScript(actor))
        return 0;
    slot->updateCamera &= ~0x2000U;
    const std::uint8_t chain = slot->fieldb7;
    const std::uint8_t *const bytes = reinterpret_cast<const std::uint8_t *>(slot);
    const std::uint8_t next = bytes[offsetof(VFSlotRuntimeState, byte54) + chain];
    auto reset = [&] {
        slot->cameraScriptWait = 0;
        slot->fielda8 = 0;
        slot->fieldb7 = 0;
        slot->hpPhase = 0;
        slot->cameraScriptStep = 0;
        slot->byte54 = 0x7fU;
        slot->byte55 = 0x7fU;
        slot->updateCamera = (slot->updateCamera & 0xffffdffeU) | 1U;
    };
    if (next == 0x7fU) {
        reset();
        return static_cast<std::uint8_t>(slot->updateCamera);
    }
    const std::uint8_t wait = bytes[offsetof(VFSlotRuntimeState, word56) + 1U + chain];
    ++slot->fieldb7;
    slot->cameraScriptWait = 0;
    if ((static_cast<std::uint8_t>(slot->reserved51[0]) & 7U) == 0) {
        reset();
        return ResetAndSelectCameraPhaseChoice(actor, next, wait);
    }
    slot->hpPhase = next;
    slot->fielda8 = wait;
    slot->cameraScriptStep = 0;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    slot->field50 = static_cast<std::uint8_t>(g_EnvPrngSeed >> 16U);
    return slot->field50;
}
std::uint8_t NextVFScaledRandomIndex(std::uint8_t count) {
    if (count == 0)
        return 0;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const std::uint32_t high = (g_EnvPrngSeed >> 16U) & 0x7fffU;
    std::uint32_t index = static_cast<std::uint32_t>(static_cast<float>(high) * (1.0f / 32768.0f) * static_cast<float>(count));
    if (index >= count)
        index = count - 1U;
    return static_cast<std::uint8_t>(index);
}

bool vfStageCheckPointInQuadrantXZ(float x, float z, std::int8_t quadrant) {
    if (quadrant == -1)
        return false;
    const VFAreaMask &area = g_pFightStage->areaMasks[quadrant];
    if (area.active != 1)
        return false;
    if (area.shapeMode == 0) {
        const float x0 = std::min(area.vertices[0].x, area.vertices[1].x);
        const float x1 = std::max(area.vertices[0].x, area.vertices[1].x);
        const float z0 = std::min(area.vertices[0].y, area.vertices[1].y);
        const float z1 = std::max(area.vertices[0].y, area.vertices[1].y);
        return x >= x0 && x <= x1 && z >= z0 && z <= z1;
    }
    float intersections[4]{};
    std::uint8_t count = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        const Vector2 &current = area.vertices[index];
        const Vector2 &next = area.vertices[(index + 1U) & 3U];
        bool addEdge = current.y == z || next.y == z;
        if (!addEdge) {
            bool skipEdge = current.y > z;
            if (z > next.y)
                skipEdge = current.y <= z;
            addEdge = !skipEdge;
        }
        if (addEdge) {
            intersections[count++] = ((next.x - current.x) * (z - current.y)) / (next.y - current.y) + current.x;
        }
    }
    if (count != 2)
        return false;
    bool outside = intersections[0] > x;
    if (x > intersections[1]) {
        outside = intersections[0] <= x;
    }
    return !outside;
}

std::uint8_t vfStageResolveQuadrantForPointXZ(float x, float z, std::uint8_t quadrant) {
    const VFFightStage &stage = *g_pFightStage;
    if (quadrant != 0xffU) {
        const auto &area = stage.areaMasks[quadrant];
        if (vfStageCheckPointInQuadrantXZ(x, z, static_cast<std::int8_t>(quadrant)))
            return quadrant;
        for (const std::int8_t neighbor : area.neighbors) {
            if (neighbor != -1 && vfStageCheckPointInQuadrantXZ(x, z, neighbor))
                return static_cast<std::uint8_t>(neighbor);
        }
    }
    for (std::uint8_t index = 0; index < stage.areaMaskCount; ++index) {
        if (vfStageCheckPointInQuadrantXZ(x, z, static_cast<std::int8_t>(index)))
            return index;
    }
    return 0xffU;
}


// @verified - p
bool VFAreaCanReachQuadrant(std::int8_t facingQuadrant, std::int8_t quadrant) {
    if (facingQuadrant == -1 || quadrant == -1)
        return false;
    const VFAreaMask &area = g_pFightStage->areaMasks[facingQuadrant];
    const std::uint32_t mask = quadrant < 32 ? area.quadrantMaskLow : area.quadrantMaskHigh;
    return ((mask >> (quadrant & 31)) & 1U) != 0;
}

Vector2 VFAreaCenter(const VFAreaMask &area) {
    return {(area.vertices[0].x + area.vertices[1].x) * 0.5f, (area.vertices[0].y + area.vertices[1].y) * 0.5f};
}

std::int8_t g_vfFacingQuadrantScratch[256]{};


// @verified - p
std::uint8_t BuildVFFacingQuadrantPath(VFActor &actor, std::int8_t start, std::int8_t target) {
    const Vector2 targetCenter = VFAreaCenter(g_pFightStage->areaMasks[target]);
    std::int16_t last = 0;
    std::int8_t current = start;
    std::uint8_t direction = 0;
    std::uint8_t selected = 0;
    bool blocked = false;
    while (true) {
        g_vfFacingQuadrantScratch[last] = current;
        g_vfFacingQuadrantScratch[last + 128] = current;
        const Vector2 center = VFAreaCenter(g_pFightStage->areaMasks[current]);
        if (current == target)
            break;
        if (VFAreaCanReachQuadrant(current, target)) {
            g_vfFacingQuadrantScratch[++last] = target;
            break;
        }
        if (std::fabs(targetCenter.y - center.y) <= std::fabs(targetCenter.x - center.x))
            direction = targetCenter.x > center.x ? 1U : 3U;
        else
            direction = targetCenter.y > center.y ? 0U : 2U;
        const std::int8_t next = g_pFightStage->areaMasks[current].neighbors[direction];
        if (next == -1) {
            blocked = true;
            break;
        }
        current = next;
        if (++last >= 128)
            break;
    }

    if (blocked && current != -1) {
        const std::int8_t blockedQuadrant = current;
        std::int8_t sideCurrent[2]{current, current};
        std::uint8_t sideDirection[2]{static_cast<std::uint8_t>((direction - 1U) & 3U), static_cast<std::uint8_t>((direction + 1U) & 3U)};
        bool ended[2]{false, false};
        float bestDistance[2]{999.0f, 999.0f};
        std::int8_t bestQuadrant[2]{-1, -1};
        bool reached = false;
        last = static_cast<std::uint8_t>(last + 1);
        while (last < 128) {
            for (std::uint8_t side = 0; side < 2; ++side) {
                if (ended[side])
                    continue;
                const Vector2 center = VFAreaCenter(g_pFightStage->areaMasks[sideCurrent[side]]);
                const float dx = center.x - targetCenter.x;
                const float dz = center.y - targetCenter.y;
                const float distance = std::sqrt(dx * dx + dz * dz);
                if (bestDistance[side] > distance) {
                    bestDistance[side] = distance;
                    bestQuadrant[side] = sideCurrent[side];
                }
                const int turn = side == 0 ? 1 : -1;
                std::uint8_t nextDirection = sideDirection[side];
                const VFAreaMask &area = g_pFightStage->areaMasks[sideCurrent[side]];
                if (area.neighbors[(nextDirection + turn) & 3] != -1) {
                    nextDirection = static_cast<std::uint8_t>((nextDirection + turn) & 3);
                } else if (area.neighbors[nextDirection] == -1) {
                    nextDirection = static_cast<std::uint8_t>((nextDirection - turn) & 3);
                    if (area.neighbors[nextDirection] == -1)
                        nextDirection = static_cast<std::uint8_t>((nextDirection - turn) & 3);
                }
                sideDirection[side] = nextDirection;
                const std::int8_t next = area.neighbors[nextDirection];
                g_vfFacingQuadrantScratch[side * 128 + last] = next;
                if (next == target || VFAreaCanReachQuadrant(next, target)) {
                    if (next != target)
                        g_vfFacingQuadrantScratch[side * 128 + ++last] = target;
                    selected = side;
                    reached = true;
                    break;
                }
                if (next == blockedQuadrant) {
                    ended[side] = true;
                    break;
                }
                sideCurrent[side] = next;
            }
            if (reached || (ended[0] && ended[1]))
                break;
            ++last;
        }
        if (!reached) {
            selected = bestDistance[0] <= bestDistance[1] ? 0U : 1U;
            const std::int8_t best = bestQuadrant[selected];
            for (last = 0; last < 128; ++last) {
                if (g_vfFacingQuadrantScratch[last] == best) {
                    selected = 0;
                    break;
                }
                if (g_vfFacingQuadrantScratch[last + 128] == best) {
                    selected = 1;
                    break;
                }
            }
        }
    }

    const std::uint8_t length = static_cast<std::uint8_t>(last + 1);
    std::int8_t *const path = &g_vfFacingQuadrantScratch[selected * 128];
    path[length] = -1;
    std::int8_t compressed[184];
    compressed[0] = path[0];
    std::int8_t rawIndex = 0;
    for (std::int16_t candidate = length - 1; candidate > 0; --candidate) {
        if (path[candidate] == compressed[0])
            rawIndex = static_cast<std::int8_t>(candidate);
    }
    for (std::int16_t output = 0; output < static_cast<std::int8_t>(length); ++output) {
        std::int16_t candidate = length - 1;
        while (candidate > rawIndex) {
            if (VFAreaCanReachQuadrant(compressed[output], path[candidate])) {
                --candidate;
                break;
            }
            --candidate;
        }
        rawIndex = static_cast<std::int8_t>(candidate + 1);
        compressed[output + 1] = path[rawIndex];
        if (compressed[output + 1] == -1)
            break;
    }
    std::uint8_t copied = 0;
    if (compressed[0] != -1) {
        do {
            if (copied >= 64)
                break;
            actor.facingQuadrantPath[copied] = compressed[copied];
            ++copied;
        } while (compressed[copied] != -1);
    }
    if (copied == 64)
        actor.facingQuadrantStep = -1;
    else
        actor.facingQuadrantPath[copied] = -1;
    return copied;
}



std::uint8_t vfMoveCameraPhase(std::uint8_t move) {
    static constexpr std::array<std::uint8_t, 32> kSet1{
        0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 213, 1, 15, 31, 32, 33, 36, 0, 7, 8, 9, 10, 11, 12, 13, 19, 20, 214, 215, 9};
    static constexpr std::array<std::uint8_t, 32> kSet2{
        7, 8, 9, 10, 11, 12, 13, 19, 20, 214, 215, 9, 30, 30, 34, 35, 37, 216, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0};
    static constexpr std::array<std::uint8_t, 16> kSet3{21, 22, 23, 24, 25, 26, 27, 21, 21, 28, 38, 39, 29, 40, 0, 0};
    if (move < 0x20U)
        return kSet1[move];
    if (move < 0x40U)
        return kSet2[move - 0x20U];
    if (move < 0x50U)
        return kSet3[move - 0x40U];
    return 0;
}

Vector2 VFFacingQuadrantBoundaryTarget(const VFAreaMask &current, const VFAreaMask &next, std::uint8_t direction) {
    if (current.shapeMode == 1) {
        switch (direction) {
        case 0:
            return {(next.vertices[2].x + next.vertices[3].x) * 0.5f, (next.vertices[2].y + next.vertices[3].y) * 0.5f};
        case 1:
            return {(next.vertices[1].x + next.vertices[2].x) * 0.5f, (next.vertices[1].y + next.vertices[2].y) * 0.5f};
        case 2:
            return {(next.vertices[0].x + next.vertices[1].x) * 0.5f, (next.vertices[0].y + next.vertices[1].y) * 0.5f};
        case 3:
            return {(next.vertices[0].x + next.vertices[3].x) * 0.5f, (next.vertices[0].y + next.vertices[3].y) * 0.5f};
        default:
            return {((next.vertices[3].x + next.vertices[1].x) * 0.5f + (next.vertices[2].x + next.vertices[0].x) * 0.5f) * 0.5f,
                ((next.vertices[3].y + next.vertices[1].y) * 0.5f + (next.vertices[2].y + next.vertices[0].y) * 0.5f) * 0.5f};
        }
    }
    const float currentWidth = std::fabs(current.vertices[1].x - current.vertices[0].x);
    const float nextWidth = std::fabs(next.vertices[1].x - next.vertices[0].x);
    const float currentHeight = std::fabs(current.vertices[1].y - current.vertices[0].y);
    const float nextHeight = std::fabs(next.vertices[1].y - next.vertices[0].y);
    const VFAreaMask &horizontal = currentWidth <= nextWidth ? current : next;
    const VFAreaMask &vertical = currentHeight <= nextHeight ? current : next;
    switch (direction) {
    case 0:
        return {(horizontal.vertices[0].x + horizontal.vertices[1].x) * 0.5f, current.vertices[0].y};
    case 1:
        return {current.vertices[0].x, (vertical.vertices[0].y + vertical.vertices[1].y) * 0.5f};
    case 2:
        return {(horizontal.vertices[0].x + horizontal.vertices[1].x) * 0.5f, current.vertices[1].y};
    case 3:
        return {current.vertices[1].x, (vertical.vertices[0].y + vertical.vertices[1].y) * 0.5f};
    default:
        return VFAreaCenter(next);
    }
}
static_assert(sizeof(VFStyleTuningBE) == 0x1ce);
static_assert(offsetof(VFStyleTuningBE, reaction_prog_preset_blob) == 0x7c);
static_assert(offsetof(VFStyleTuningBE, react_state02_end_pool_near) == 0x73);

constexpr std::uint8_t kVFMainFallbackPhases[] = {201, 202, 201, 202};
constexpr std::uint8_t kVFMainFarContactPhases[] = {204, 204, 201, 202};
constexpr std::uint8_t kVFMainClearancePhases[] = {17, 20, 21, 18, 22, 23};
constexpr std::uint8_t kVFMainContactPhases[] = {204, 204, 201, 202, 28, 28, 26, 27};
constexpr std::uint16_t kVFMainCameraSectors[] = {32, 160, 128, 144, 16, 80, 64, 96};
constexpr Vector2 kVFMainQuadrantOffsets[] = {{-5.0f, 0.0f}, {-3.1600001f, 3.1600001f}, {0.0f, 5.0f}, {3.1600001f, 3.1600001f}, {5.0f, 0.0f},
    {3.1600001f, -3.1600001f}, {0.0f, -5.0f}, {-3.1600001f, -3.1600001f}};

bool vfPhaseGatePasses(const VFSlotRuntimeState *slot, std::uint8_t value) {
    return value != 0 && value >= slot->field50;
}
void vfResetCameraScriptState(VFSlotRuntimeState &slot) {
    slot.cameraScriptWait = 0;
    slot.fielda8 = 0;
    slot.fieldb7 = 0;
    slot.hpPhase = 0;
    slot.cameraScriptStep = 0;
    slot.byte54 = 0x7f;
    slot.byte55 = 0x7f;
    slot.updateCamera = (slot.updateCamera & 0xffffdffeU) | 1U;
}
// @verified - p
std::uint8_t vfCallbackNoOp(VFActor *, VFSlotRuntimeState *) {
    return 0;
}

// @verified - p
std::uint8_t vfCallbackReject(VFActor *, VFSlotRuntimeState *) {
    return 0xffU;
}

std::uint8_t vfRefreshRandomPhaseRoll(VFActor *, VFSlotRuntimeState *slot_runtime) {
    if (!slot_runtime)
        return 0;
    if ((slot_runtime->previousUpdateIndex & 0xF) == 0) {
        const std::uint8_t seed = static_cast<std::uint8_t>(NextVFFrameEventRandomWord());
        ++slot_runtime->fieldb6;
        slot_runtime->field50 = seed;
        return seed;
    }
    return slot_runtime->field50;
}

std::uint8_t vfEvalRandomPhaseGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    if ((self->groundContactFlags & 1) != 0)
        return 0xffU;
    const std::uint32_t blocked = 0x1000 | 0x4000 | 0x8000 | 0x10000 | 0x80000 | 0x10 | 0x40000;
    if ((self->hitStateFlags2 & blocked) != 0 || (target->hitStateFlags2 & blocked) != 0)
        return 0xffU;
    if (self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] > 3.0f)
        return 0xffU;
    std::uint8_t phase = (target->hitStateFlags2 & 0x8) != 0 ? slot_runtime->phaseConfig[1] : slot_runtime->phaseConfig[0];
    if (self->fightStyleIndex == 0x1D && target->contactPoints[self->game ? static_cast<std::uint8_t>(self->game->playerSlot) & 7u : 0] < 4.0f)
        phase >>= 2;
    return vfPhaseGatePasses(slot_runtime, phase) ? 1 : 0xffU;
}

std::uint8_t vfSelectRandomCameraPhaseChoice(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !style || !slot_runtime)
        return 0;
    if ((static_cast<std::uint8_t>(slot_runtime->reserved51[0]) & 0x70) != 0) {
        const auto &pools = (self->hitStateFlags2 & 0x8) != 0 ? style->camera_phase_pool_3c : style->camera_phase_pool_24;
        std::uint8_t slice = 0;
        switch (self->fightStyleIndex) {
        case 0x6:
        case 0x14:
        case 0x1D:
            slice = 1;
            break;
        case 0x1B:
            if (slot_runtime->currentHpRegenerationPhase == 2u)
                slice = 1;
            else if (slot_runtime->currentHpRegenerationPhase == 3u)
                slice = 2;
            break;
        case 0x1C: {
            const std::int8_t hpPhase = static_cast<std::int8_t>(slot_runtime->currentHpRegenerationPhase);
            if (hpPhase == 1)
                slice = 1;
            else if (hpPhase > 1)
                slice = 2;
            break;
        }
        default:
            break;
        }
        return ResetAndSelectCameraPhaseChoice(*self, pools[slice][NextVFScaledRandomIndex(8)], 0);
    }
    return ResetAndSelectCameraPhaseChoice(*self, NextVFScaledRandomIndex(style->camera_phase_choice_count), 0);
}

std::uint8_t vfEvalFollowupHitWindow(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    const float previous = self->fielda88;
    self->fielda88 = 0.0f;
    if ((self->hitStateFlags2 & (0x1000 | 0x4000 | 0x8000 | 0x10000 | 0x10)) != 0 || target->pairContactPriority ||
        ((self->hitStateFlags & 0x8000000) != 0 && (self->hitFlags & 0x4) != 0))
        return 0xffU;
    const float distance = self->contactPoints[self->targetSlotIndex];
    const float assist = slot_runtime->assistWindowCurrent;
    if (previous == 0.0f ? static_cast<double>(assist) + 0.5 > distance : static_cast<double>(assist) - 0.3 > distance)
        return 0xffU;
    if (self->fightStyleIndex == 0x1D && target->contactPoints[0] > 4.0f)
        return 0xffU;
    if (self->followupState == 0x0) {
        self->followupState = ((self->groundContactFlags & 1) != 0 || distance > static_cast<double>(assist) + 2.0) ? 0x2U : 0x1;
        if (style && (style->style_flags & 2) != 0)
            self->followupState = 0x1;
        const std::uint64_t forcedPrimary = 0x0200000000201000ull;
        if (static_cast<std::uint16_t>(self->fightStyleIndex) <= 57u && ((forcedPrimary >> static_cast<std::uint16_t>(self->fightStyleIndex)) & 1u) != 0)
            self->followupState = 0x1;
    }
    self->fielda88 = assist;
    return 2;
}

std::uint8_t vfSetFollowupHitFor(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto slotIndex = static_cast<std::int8_t>(slot_runtime->slotIndex);
    VFActor *const target = self->game->actors[slotIndex];
    self->cameraControlFlags = 0x2000U;
    if (self->followupState == 1 && (self->contactPoints[slotIndex] > 5.0f || (target->hitStateFlags2 & 0x20000000U) != 0)) {
        self->followupState = 2;
        if ((static_cast<const VFStyleTuningBE *>(self->styleTuning)->style_flags & 2) != 0)
            self->followupState = 1;
        constexpr std::uint64_t kForcedPrimaryStyles = 0x0200000000201000ULL;
        if (self->fightStyleIndex <= 57U && ((kForcedPrimaryStyles >> self->fightStyleIndex) & 1U) != 0)
            self->followupState = 1;
    }
    self->facingQuadrantStep = -1;
    self->forcedTurnState = 0;
    self->followTargetPosition.x = target->position.x;
    self->followTargetPosition.z = target->position.z;
    return 0;
}

std::uint8_t vfEvalState03GroundedNoPairContact(VFActor *self, VFSlotRuntimeState *) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !target || (self->groundContactFlags & 1) == 0)
        return 0xffU;
    if ((self->hitStateFlags2 & (0x1000 | 0x4000 | 0x8000 | 0x10000 | 0x10)) != 0)
        return 0xffU;
    return target->pairContactPriority ? 0xffU : 3;
}

std::uint8_t vfResetAndSelectRandomCameraPhaseFromMovePool54OrFallback(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !slot_runtime || !style)
        return 0;
    if (self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] < 2.0f && slot_runtime->phaseConfig[0] != 0 &&
        slot_runtime->phaseConfig[0] >= static_cast<std::uint8_t>(slot_runtime->reserved51[0]))
        return ResetAndSelectCameraPhaseChoice(*self, style->phase_select_pool_54[NextVFScaledRandomIndex(4)], 0);
    return ResetAndSelectCameraPhaseChoice(*self, kVFMainFallbackPhases[NextVFScaledRandomIndex(4)], 0);
}

std::uint8_t vfEvalState04PhaseCfg10Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime)
        return 0xffU;
    if (slot_runtime->byteb9 || slot_runtime->fielda2 > 90u)
        return 4;
    VFActor *target = (self ? self->targetActor : nullptr);
    if (slot_runtime->fielda2 && target && !target->pairContactPriority && vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[10]))
        return 4;
    return 0xffU;
}

std::uint8_t vfSetCamCtrlFlagsByFrameParity(VFActor *self, VFSlotRuntimeState *) {
    if (!self || !self->game)
        return 0;
    self->cameraControlFlags = static_cast<std::uint16_t>((self->game->frameCount & 2u) == 0 ? 0x10u : 0x20u);
    return static_cast<std::uint8_t>(self->cameraControlFlags);
}

std::uint8_t vfCheckGuardStateFor(VFActor *self, VFSlotRuntimeState *slot_runtime);

std::uint8_t vfEvalState05GuardOrMoveParamGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    VFActor *target = self ? self->targetActor : nullptr;
    if (!self || !slot_runtime || !target || !style)
        return 0xffU;
    const std::int8_t targetSlot = self->targetSlotIndex;
    if (targetSlot < 0 || static_cast<std::size_t>(targetSlot) >= std::size(self->contactPoints))
        return 0xffU;
    if ((target->hitStateFlags2 & (0x100 | 0x4)) != 0x100 || (self->hitStateFlags2 & 0x1000) != 0 || (slot_runtime->updateCamera & 0x2000) != 0 ||
        self->contactPoints[static_cast<std::size_t>(targetSlot)] > 2.5f)
        return 0xffU;
    if ((style->style_flags & 2) == 0 && vfCheckGuardStateFor(self, slot_runtime) == 0xffU)
        return 0xffU;
    return 5;
}

std::uint8_t vfCheckGuardStateFor(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = self ? self->targetActor : nullptr;
    if (!target || !slot_runtime)
        return 0xffU;

    std::uint8_t phase = 0;
    switch (static_cast<std::uint8_t>(target->guardState)) {
    case 0x0:
    case 0x3:
        phase = slot_runtime->phaseConfig[3];
        break;
    case 0x1:
        phase = slot_runtime->phaseConfig[4];
        break;
    case 0x2:
    case 0x5:
    case 0x6:
        phase = slot_runtime->phaseConfig[5];
        break;
    default:
        return 0xffU;
    }
    return vfPhaseGatePasses(slot_runtime, phase) ? 5 : 0xffU;
}

std::uint8_t vfResetCameraStateAndSetCamCtrlByOpponentState(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !target || !slot_runtime)
        return 0;
    vfResetCameraScriptState(*slot_runtime);
    self->cameraControlFlags = (target->counterReactionFlags & 2u) != 0 ? 0x200u : 0x100u;
    return static_cast<std::uint8_t>(self->cameraControlFlags);
}

std::uint8_t vfEvalState06AirbornePairContactGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    if ((self->contactDecisionMask & 0x1U) == 0 || (self->hitStateFlags2 & 0x1D110u) != 0 || (target->hitStateFlags2 & 0x1d118U) != 0 ||
        ReadPackedU16(&slot_runtime->reserved9d[1]) != 0)
        return 0xffU;
    if (slot_runtime->fieldb2 > 0x80u && slot_runtime->phaseConfig[12])
        return 6;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[12]) ? 6 : 0xffU;
}

std::uint8_t vfResetAndSelectRandomCameraPhaseFromMovePool58Or5C(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !style)
        return 0;
    const std::uint8_t *pool = (self->fightStyleIndex == 0x1B && slot_runtime && static_cast<std::int8_t>(slot_runtime->currentHpRegenerationPhase) >= 2) ?
        style->phase_select_pool_5c :
        style->phase_select_pool_58;
    return ResetAndSelectCameraPhaseChoice(*self, pool[NextVFScaledRandomIndex(4)], slot_runtime ? slot_runtime->phaseArgumentInitial : 0);
}

std::uint8_t vfEvalState11ContactDecisionBit8Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime || (self->contactDecisionMask & 8u) == 0)
        return 0xffU;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[12]) ? 11 : 0xffU;
}

std::uint8_t vfResetAndSelectRandomCameraPhaseFromStylePool60(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !style)
        return 0;
    return ResetAndSelectCameraPhaseChoice(
        *self, style->phase_select_pool_60[NextVFScaledRandomIndex(2)], slot_runtime ? slot_runtime->phaseArgumentInitial : 0);
}

// @verified - p
std::uint8_t vfSelectRandomCameraPhaseFromMovePool62(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !style)
        return 0;
    return ResetAndSelectCameraPhaseChoice(
        *self, style->phase_select_pool_62[NextVFScaledRandomIndex(2)], slot_runtime ? slot_runtime->phaseArgumentInitial : 0);
}

// @verified - p
std::uint8_t vfEvalState12LowTargetHpAirContact(VFActor *self, VFSlotRuntimeState *) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !target)
        return 0xffU;
    if (target->currentHp > target->maximumHp / 4 || self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] > 1.25f)
        return 0xffU;
    if ((self->hitStateFlags2 & 0x1C000u) != 0 || (target->hitStateFlags2 & 0x1D010u) != 0)
        return 0xffU;
    if ((target->hitStateFlags2 & 0x102u) != 0x102u)
        return 0xffU;
    return (self->groundContactFlags & 1) != 0 ? 0xffU : 12;
}

// @verified - p
std::uint8_t vfSelectRandomAttackWindowEndPhaseChoice(VFActor *self, VFSlotRuntimeState *) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !style || !target)
        return 0;
    if (static_cast<std::int16_t>(target->attackWindowEndFrame) - static_cast<std::int16_t>(target->currentAnimationEndFrame) >= 5)
        return 0;

    (void)NextVFFrameEventRandomWord();
    return ResetAndSelectCameraPhaseChoice(*self, style->phase_select_pool_6e, 0);
}

std::uint8_t vfEvalState13AssistAngleGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !target || !slot_runtime)
        return 0xffU;
    if ((target->motionFlags & 0x4) == 0 &&
        (((target->hitFlags & 0x100) == 0) || (target->hitStateFlags2 & (0x1000 | 0x4000 | 0x8000 | 0x10000 | 0x10 | 0x4)) != 0x4))
        return 0xffU;
    const std::uint8_t actIdx = static_cast<std::uint8_t>(self->targetSlotIndex) & 7u;
    if (self->contactPoints[actIdx] > 2.25f)
        return 0xffU;
    if (slot_runtime->facingAngleDifferences[actIdx] > static_cast<std::uint16_t>(slot_runtime->assistAngleLimit)) {
        return 0xffU;
    }
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[7]) ? 13 : 0xffU;
}

std::uint8_t vfSelectRandomReactState02EndPhaseChoice(VFActor *self, VFSlotRuntimeState *) {
    const auto *style = static_cast<const VFStyleTuningBE *>(self->styleTuning);
    const VFActor *const target = self->game->actors[self->targetSlotIndex];
    if (static_cast<std::int16_t>(target->reactionState02EndFrame) - static_cast<std::int16_t>(target->currentAnimationEndFrame) < 4)
        return 0;
    const std::uint8_t *pool = self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] < 2.2f ? style->react_state02_end_pool_near :
                                                                                                                   style->react_state02_end_pool_far;
    return ResetAndSelectCameraPhaseChoice(*self, pool[NextVFScaledRandomIndex(2)], 0);
}

// @verified - p
std::uint8_t vfEvalState01GroundedTargetAssistGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    if ((target->groundContactFlags & 1u) == 0)
        return 0xffU;
    constexpr std::uint32_t blockers = 0x1000 | 0x4000 | 0x8000 | 0x10000 | 0x10;
    if (((self->hitStateFlags2 | target->hitStateFlags2) & blockers) != 0)
        return 0xffU;
    if (self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] > 2.0f)
        return 0xffU;

    std::uint16_t phase = static_cast<std::uint16_t>(slot_runtime->phaseConfig[0]) + static_cast<std::uint16_t>(slot_runtime->phaseConfig[7]);
    if (phase > 0xFFu)
        phase = 0xFFu;
    if (!phase || static_cast<std::uint8_t>(phase) < slot_runtime->field50)
        return 0xffU;
    return 1;
}

std::uint8_t vfEvalState15AirbornePairContactThreshold(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if ((self->hitStateFlags2 & 0x80000U) == 0 || !self->hitSourceActor || !self->hitSourceActor->pairContactPriority)
        return 0xffU;
    if ((static_cast<const VFStyleTuningBE *>(self->styleTuning)->style_flags & 2) != 0 || slot_runtime->downedFrames >= 0xa0U ||
        slot_runtime->repeatedRankCount >= 3U || vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[13]))
        return 15;
    return 0xffU;
}

std::uint8_t vfResetCameraStateAndSetCamCtrlFlag4(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (slot_runtime)
        vfResetCameraScriptState(*slot_runtime);
    if (self) {
        const std::uint16_t activeTimer = slot_runtime ? ReadPackedU16(&slot_runtime->byteaa) : 0;
        self->cameraControlFlags = (activeTimer == 1u || activeTimer == 2u) ? 4u : 0u;
    }
    return self ? static_cast<std::uint8_t>(self->cameraControlFlags) : 0;
}

std::uint8_t vfCheckConfirmInputForFrontContact(VFActor *self, VFSlotRuntimeState *) {
    if ((g_inputState.slots[0].pressedPriority & g_vfConfirmButtonMask) == 0 || self->targetActor->fightStyleIndex == 21U)
        return 0xffU;
    if ((self->contactDecisionMask & 1U) != 0)
        return 16;
    return (self->hitStateFlags2 & 0x1d000U) == 0 ? 16 : 0xffU;
}

std::uint8_t vfSelectCameraPhaseFromAnimSetOrRandomFallback(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (slot_runtime->fielda8)
        vfResetCameraScriptState(*self->slot);
    const auto selectedMove = static_cast<std::uint8_t>(g_playerStats.currentMove >> 8);
    std::uint8_t phase = 0xffU;
    if (selectedMove != 0xffU) {
        if (static_cast<std::uint8_t>(selectedMove - 64) <= 15U && static_cast<double>(self->contactPoints[self->targetSlotIndex]) > 3.7)
            return ResetAndSelectCameraPhaseChoice(*self, 21, 0);
        phase = vfMoveCameraPhase(selectedMove);
    } else if (static_cast<std::uint8_t>(g_playerStats.currentMove) != 0xffU) {
        phase = vfMoveCameraPhase(static_cast<std::uint8_t>(g_playerStats.currentMove));
    }
    if (phase != 0xffU)
        return ResetAndSelectCameraPhaseChoice(*self, phase, 0);
    static constexpr std::uint8_t front[] = {24, 25, 26, 23};
    static constexpr std::uint8_t ordinary[] = {17, 18, 15, 3};
    const auto choice = NextVFFrameEventRandomWord() & 3U;
    phase = (self->contactDecisionMask & 1U) != 0 ? front[choice] : ordinary[choice];
    if ((self->contactDecisionMask & 1U) == 0 && (self->groundContactFlags & 1) != 0)
        phase = 7;
    return ResetAndSelectCameraPhaseChoice(*self, phase, 0);
}

// @verified - p
std::uint8_t vfEvalState17Slot174FrontContactGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    if (slot_runtime->hpPhaseIncrement)
        return 0xffU;
    if ((slot_runtime->currentHpRegenerationPhase & 0xFDu) == 0 &&
        (((self->contactDecisionMask & 0x1U) == 0) || (self->hitStateFlags2 & 0x1D110u) != 0 || (target->hitStateFlags2 & 0x1D118u) != 0)) {
        return 0xffU;
    }
    return 17;
}

// @verified - p
std::uint8_t vfHandleHpRegenPhasePostUpdate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime)
        return 0;
    switch (slot_runtime->currentHpRegenerationPhase) {
    case 0:
    case 2:
        return ResetAndSelectCameraPhaseChoice(*self, 209, 0);
    case 1:
        (void)vfAssignVoiceFor(*self->game, 4);
        ++slot_runtime->currentHpRegenerationPhase;
        slot_runtime->hpPhaseIncrement = static_cast<std::uint16_t>(self->currentHp + 150);
        return static_cast<std::uint8_t>(slot_runtime->hpPhaseIncrement);
    case 3:
        (void)vfAssignVoiceFor(*self->game, 9);
        ++slot_runtime->currentHpRegenerationPhase;
        slot_runtime->hpPhaseIncrement = static_cast<std::uint16_t>(self->currentHp + 150);
        if (self->game)
            self->game->extraValue |= 1u;
        return 0;
    default:
        return 0;
    }
}

std::uint8_t vfEvalState05GuardWindowGateA(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = self ? self->targetActor : nullptr;
    if (!self || !slot_runtime || !target || !self->game)
        return 0xffU;
    const std::int8_t targetSlot = self->targetSlotIndex;
    if (targetSlot < 0 || static_cast<std::size_t>(targetSlot) >= std::size(self->contactPoints) ||
        static_cast<std::size_t>(targetSlot) >= std::size(self->game->actors))
        return 0xffU;
    const std::size_t targetIndex = static_cast<std::size_t>(targetSlot);
    if ((target->hitStateFlags2 & (0x100 | 0x4)) != 0x100 || (self->hitStateFlags2 & 0x1000) != 0 || (slot_runtime->updateCamera & 0x2000u) != 0 ||
        self->contactPoints[targetIndex] > 2.5f) {
        return 0xffU;
    }

    const bool sameAnimBypass = (slot_runtime->updateCamera & 0x400u) != 0 &&
        (self->game->actors[targetIndex]->currentReactionAnimation & 0x0fffU) == slot_runtime->repeatedReactionAnimation;
    if (!sameAnimBypass && vfCheckGuardStateFor(self, slot_runtime) == 0xffU)
        return 0xffU;
    return 5;
}

// @verified - p
std::uint8_t vfEvalState05GuardWindowGateB(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = self ? self->targetActor : nullptr;
    if (!self || !slot_runtime || !target)
        return 0xffU;
    const std::int8_t targetSlot = self->targetSlotIndex;
    if (targetSlot < 0 || static_cast<std::size_t>(targetSlot) >= std::size(self->contactPoints))
        return 0xffU;
    if ((target->hitStateFlags2 & (0x100 | 0x4)) != 0x100 || (self->hitStateFlags2 & 0x1000) != 0 || (slot_runtime->updateCamera & 0x2000u) != 0 ||
        self->contactPoints[static_cast<std::size_t>(targetSlot)] > 2.5f) {
        return 0xffU;
    }
    if (target->guardStrength < 0x20u && vfCheckGuardStateFor(self, slot_runtime) == 0xffU) {
        return 0xffU;
    }
    return 5;
}

// @verified - p
std::uint8_t vfTriggerFollowupState2OnSfxContact(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime || !self->game)
        return 0xffU;
    const float previous = self->fielda88;
    self->fielda88 = 0.0f;
    const float threshold = self->game->soundContactThreshold;
    const float distance = self->contactPoints[slot_runtime->slotIndex];
    if (previous == 0.0f ? static_cast<double>(threshold) + 0.5 > distance : static_cast<double>(previous) - 0.3 > distance)
        return 0xffU;
    self->fielda88 = threshold;
    self->followupState = 0x2U;
    return 2;
}

// @verified - p
std::uint8_t vfEvalState21ContactDecision6000000Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime || (self->contactDecisionMask & 0x06000000U) == 0)
        return 0xffU;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[12]) ? 21 : 0xffU;
}

// @verified - p
bool vfContactActionAttackWindowGate(VFActor *self, VFSlotRuntimeState *slot_runtime, std::uint32_t mask) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target || (self->contactActionMask & mask) == 0)
        return false;
    if (target->attackWindowEndFrame > 4u &&
        static_cast<std::int16_t>(target->attackWindowEndFrame) - static_cast<std::int16_t>(target->currentAnimationEndFrame) > 4)
        return false;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[8]);
}

// @verified - p
std::uint8_t vfEvalState22ContactAction2400Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    return vfContactActionAttackWindowGate(self, slot_runtime, 0x2400) ? 22 : 0xffU;
}
// @verified - p
std::uint8_t vfEvalState22ContactAction400Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    return vfContactActionAttackWindowGate(self, slot_runtime, 0x400) ? 22 : 0xffU;
}
// @verified - p
std::uint8_t vfEvalState23ContactAction5000Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    return vfContactActionAttackWindowGate(self, slot_runtime, 0x5000) ? 23 : 0xffU;
}
// @verified - p
std::uint8_t vfEvalState24ContactAction8800Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    return vfContactActionAttackWindowGate(self, slot_runtime, 0x8800) ? 24 : 0xffU;
}

// @verified - p
std::uint8_t vfSelectCameraPhase205IfMoveFlag100Clear(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (self && slot_runtime && (self->moveFlags & 0x100) == 0) {
        const std::uint8_t result = ResetAndSelectCameraPhaseChoice(*self, 205, 0);
        slot_runtime->updateCamera |= 0x2000u;
        return result;
    }
    return 0;
}

// @verified - p
std::uint8_t vfSelectCameraPhase206IfMoveFlag100Clear(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (self && slot_runtime && (self->moveFlags & 0x100) == 0) {
        const std::uint8_t result = ResetAndSelectCameraPhaseChoice(*self, 206, 0);
        slot_runtime->updateCamera |= 0x2000u;
        return result;
    }
    return 0;
}

// @verified - p
std::uint8_t vfSelectCameraPhase207IfMoveFlagSet(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (self && slot_runtime && (self->moveFlags & 0x100) != 0) {
        const std::uint8_t result = ResetAndSelectCameraPhaseChoice(*self, 207, 0);
        slot_runtime->updateCamera |= 0x2000u;
        return result;
    }
    return 0;
}

// @verified - p
std::uint8_t vfEvalState25ContactDecisionBit4Gate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime || (self->contactDecisionMask & 0x4U) == 0)
        return 0xffU;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[12]) ? 25 : 0xffU;
}

// @verified - p
std::uint8_t vfResetAndSelectRandomCameraPhaseFromMovePool5C(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !style || !slot_runtime)
        return 0;
    return ResetAndSelectCameraPhaseChoice(*self, style->phase_select_pool_5c[NextVFScaledRandomIndex(4)], slot_runtime->phaseArgumentInitial);
}

// @verified - p
std::uint8_t vfEvalState26CloseAttackContactGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    const std::uint32_t targetFlags = target->hitStateFlags2;
    if ((targetFlags & 0x10000100u) == 0 || (targetFlags & 0x4) != 0 || (self->hitStateFlags2 & 0x1000) != 0 ||
        ((targetFlags & 0x100) == 0 && (target->motionFlags & 0x40000u) == 0))
        return 0xffU;
    if (self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] > 2.0f)
        return 0xffU;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[6]) ? 26 : 0xffU;
}

// @verified - p
std::uint8_t vfResetCameraStateAndSetWallSideCamCtrl(VFActor *self, VFSlotRuntimeState *) {
    if (!self || !self->slot)
        return 0;
    VFActor *target = (self ? self->targetActor : nullptr);
    vfResetCameraScriptState(*self->slot);
    if ((self->game && (self->game->frameCount & 1u) == 0) || !target) {
        self->cameraControlFlags = 0;
        return 0;
    }
    std::uint16_t flags = 0x88u;
    if (static_cast<std::int16_t>(target->hitDirectionAngle) >= 0)
        flags = self->rightClearance > 3.0f ? 0x88u : 0x48u;
    else
        flags = self->leftClearance <= 3.0f ? 0x88u : 0x48u;
    self->cameraControlFlags =
        static_cast<std::uint16_t>(static_cast<std::int16_t>(target->reactionState02EndFrame - target->currentAnimationEndFrame) >= 4 ? flags : 0x18u);
    return static_cast<std::uint8_t>(self->cameraControlFlags);
}

std::uint8_t vfEvalState27FarContactNoPairGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !self->game || !target)
        return 0xffU;
    if (self->game->battleType == 0x5 && self->slotIndex < 6 && self->game->enemyRanks[self->slotIndex] == 1)
        return 0xffU;
    if ((target->hitStateFlags2 & 0x100) != 0 && target->currentAnimationEndFrame < target->reactionMidFrame)
        return 0xffU;
    if (((self->groundContactFlags | target->groundContactFlags) & 1) != 0 || (target->hitStateFlags2 & (0x4000 | 0x8000 | 0x10000)) != 0)
        return 0xffU;
    const double distance = self->contactPoints[self->targetSlotIndex];
    return distance > static_cast<double>(slot_runtime->assistWindowCurrent) - 1.0 ? 0xffU : 27;
}

std::uint8_t vfSelectCameraPhase204OrRandom201202(VFActor *self, VFSlotRuntimeState *) {
    if (!self)
        return 0;
    if (self->fightStyleIndex == 0x36)
        return ResetAndSelectCameraPhaseChoice(*self, 204, 0);
    return ResetAndSelectCameraPhaseChoice(*self, kVFMainFarContactPhases[NextVFScaledRandomIndex(4)], 0);
}

std::uint8_t vfEvalState28FollowupClearanceGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target)
        return 0xffU;
    if (self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u] <= 2.5f) {
        const std::int16_t anim = static_cast<std::int16_t>(target->currentReactionAnimation & 0x0FFFu);
        if (anim == 1472 || anim == 895 || (self->leftClearance < 0.5f && self->rightClearance < 0.5f))
            return 0xffU;
        if (vfEvalState26CloseAttackContactGate(self, slot_runtime) == 0xffU)
            return 0xffU;
        if (static_cast<std::int16_t>(target->attackWindowEndFrame) - static_cast<std::int16_t>(target->currentAnimationEndFrame) < 4)
            return 0xffU;
    } else {
        if ((target->hitStateFlags2 & 0x4000) == 0 || !vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[6]))
            return 0xffU;
    }
    return 28;
}

std::uint8_t vfSelectCameraPhaseFromContactActionAndSide(VFActor *self, VFSlotRuntimeState *) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !target)
        return 0;
    const std::uint8_t actIdx = static_cast<std::uint8_t>(self->targetSlotIndex) & 7u;
    const float contactPoint = self->contactPoints[actIdx];
    std::uint8_t idx = 6;
    if (contactPoint <= 2.3f) {
        if ((target->contactActionMask & 0x2400) != 0)
            idx = 4;
        else if ((target->counterReactionFlags & 2) != 0)
            idx = 6;
        else if (self->backwardClearance <= 3.0f || ((g_EnvPrngSeed = LcgStep(g_EnvPrngSeed)) & 0x30000u) != 0)
            idx = 2;
        else
            idx = 0;
    }
    if (static_cast<std::int16_t>(target->hitDirectionAngle) <= 0) {
        if (self->rightClearance >= 0.5f)
            ++idx;
    } else if (self->leftClearance < 0.5f) {
        ++idx;
    }
    return ResetAndSelectCameraPhaseChoice(*self, kVFMainContactPhases[idx & 7u], 0);
}

// @verified - p
std::uint8_t vfEvalState29GroundedNoPairContact(VFActor *self, VFSlotRuntimeState *) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !target || (self->groundContactFlags & 1) == 0 || (self->hitStateFlags2 & 0x1D010u) != 0)
        return 0xffU;
    return target->pairContactPriority ? 0xffU : 29;
}

// @verified - p
std::uint8_t vfSelectRandomCameraPhaseFromFallbackPool4(VFActor *self, VFSlotRuntimeState *) {
    return self ? ResetAndSelectCameraPhaseChoice(*self, kVFMainFallbackPhases[NextVFScaledRandomIndex(4)], 0) : 0;
}

std::uint8_t vfEvalState30GroundedClearanceGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime)
        return 0xffU;
    if (ReadPackedU16(&slot_runtime->reserved9d[1]) != 0)
        return 0xffU;
    if ((self->contactDecisionMask & 0x1U) == 0)
        return 0xffU;
    if (self->forwardClearance < 2.5f && self->backwardClearance < 2.5f)
        return 0xffU;
    if (!(static_cast<double>(self->forwardClearance) > 1.7) || !(static_cast<double>(self->backwardClearance) > 0.2))
        return 30;
    return vfEvalState06AirbornePairContactGate(self, slot_runtime) != 0xffU ? 30 : 0xffU;
}

std::uint8_t vfSelectRandomCameraPhaseByClearance(VFActor *self, VFSlotRuntimeState *) {
    if (!self)
        return 0;
    std::uint8_t base = 0;
    if (self->backwardClearance <= 2.5f) {
        if (self->forwardClearance < 2.5f)
            return ResetAndSelectCameraPhaseChoice(*self, 15, 0);
    } else if (self->forwardClearance < 1.7f) {
        base = 3;
    } else if (((g_EnvPrngSeed = LcgStep(g_EnvPrngSeed)) & 0x10000u) != 0) {
        base = 3;
    }
    return ResetAndSelectCameraPhaseChoice(*self, kVFMainClearancePhases[base + NextVFScaledRandomIndex(3)], 0);
}

std::uint8_t vfEvalState31UpdateCamContactGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = (self ? self->targetActor : nullptr);
    if (!self || !slot_runtime || !target || (slot_runtime->updateCamera & 1u) == 0)
        return 0xffU;
    const std::uint8_t actIdx = static_cast<std::uint8_t>(self->targetSlotIndex) & 7u;
    const std::int16_t angle = static_cast<std::int16_t>(slot_runtime->facingAngleDifferences[actIdx]);
    if (static_cast<std::uint16_t>(angle) < static_cast<std::uint16_t>(slot_runtime->word22) || angle > 24576 || target->pairContactPriority ||
        (self->hitStateFlags2 & 0xDC000u) != 0 || ((target->hitStateFlags2 & 0x100) != 0 && self->contactPoints[actIdx] < 2.7f))
        return 0xffU;
    return 31;
}

std::uint8_t vfResetCameraStateAndSetCamCtrlFlag20(VFActor *self, VFSlotRuntimeState *) {
    if (!self || !self->slot)
        return 0;
    vfResetCameraScriptState(*self->slot);
    self->cameraControlFlags = 0x20u;
    return 0x20u;
}

// @verified - p
std::uint8_t vfEvalState32ProbeDistanceGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime || (self->hitStateFlags2 & 0x20000000) == 0)
        return 0xffU;
    if (static_cast<double>(self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u]) > static_cast<double>(self->fielda88) + 0.2)
        return 0xffU;
    return vfPhaseGatePasses(slot_runtime, slot_runtime->phaseConfig[0]) ? 32 : 0xffU;
}

// @verified - p
std::uint8_t vfResetAndSelectRandomCameraPhaseFromStylePool6A(VFActor *self, VFSlotRuntimeState *) {
    const auto *style = self ? static_cast<const VFStyleTuningBE *>(self->styleTuning) : nullptr;
    if (!self || !style || !self->slot)
        return 0;
    vfResetCameraScriptState(*self->slot);
    return ResetAndSelectCameraPhaseChoice(*self, style->phase_select_pool_6a[NextVFScaledRandomIndex(4)], 0);
}

std::uint8_t vfEvalState05MoveFlag40000GuardGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    VFActor *target = self ? self->targetActor : nullptr;
    if (!self || !target || !slot_runtime)
        return 0xffU;
    const std::int8_t targetSlot = self->targetSlotIndex;
    if (targetSlot < 0 || static_cast<std::size_t>(targetSlot) >= std::size(self->contactPoints))
        return 0xffU;
    if ((self->hitStateFlags2 & 0x40000) != 0 || (target->hitStateFlags2 & 0x100) == 0 || (self->motionFlags & 0x2000000) != 0 ||
        (slot_runtime->updateCamera & 0x2000u) != 0 || static_cast<std::uint16_t>(target->currentAnimationEndFrame + 2u) < target->attackWindowEndFrame)
        return 0xffU;
    return self->contactPoints[static_cast<std::size_t>(targetSlot)] <= 2.5f ? 5 : 0xffU;
}

std::uint8_t vfValidateFacingQuadrantForContextAction(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto saved = self->facingQuadrantStep;
    self->facingQuadrantStep = -1;
    const auto *target = self->game->actors[slot_runtime->slotIndex];
    if (target->pairContactPriority || (self->hitStateFlags2 & 0x1d010U) != 0 || self->cachedQuadrant == 0xffU || target->cachedQuadrant == 0xffU ||
        VFAreaCanReachQuadrant(static_cast<std::int8_t>(self->cachedQuadrant), static_cast<std::int8_t>(target->cachedQuadrant)) ||
        self->contactPoints[slot_runtime->slotIndex] <= 1.5f ||
        (self->fightStyleIndex == 29U && target->contactPoints[static_cast<std::uint8_t>(self->game->playerSlot)] > 4.0f))
        return 0xffU;
    self->facingQuadrantStep = saved;
    return 44;
}

std::uint8_t vfUpdateFollowupHitTargetPosition(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *const style = static_cast<const VFStyleTuningBE *>(self->styleTuning);
    const auto *const target = self->game->actors[slot_runtime->slotIndex];
    self->followupState = (style->style_flags & 2U) != 0 ? 1U : 2U;
    self->cameraControlFlags = 0x2000U;
    if (self->facingQuadrantStep == -1) {
        (void)BuildVFFacingQuadrantPath(*self, static_cast<std::int8_t>(self->cachedQuadrant), static_cast<std::int8_t>(target->cachedQuadrant));
        self->facingQuadrantStep = 0;
    }
    const auto step = self->facingQuadrantStep;
    const auto current = self->facingQuadrantPath[static_cast<std::uint8_t>(step)];
    const float dx = self->contactCurrentPosition.x - self->followTargetPosition.x;
    const float dz = self->contactCurrentPosition.z - self->followTargetPosition.z;
    if (current != static_cast<std::int8_t>(self->cachedQuadrant) && std::sqrt(dx * dx + dz * dz) >= 0.8f)
        return static_cast<std::uint8_t>(step);
    const auto nextStep = static_cast<std::uint8_t>(step + 1);
    if (nextStep >= std::size(self->facingQuadrantPath) || self->facingQuadrantPath[nextStep] == -1) {
        self->facingQuadrantStep = -1;
        return nextStep;
    }
    self->forcedTurnState = 0;
    self->facingQuadrantStep = static_cast<std::int8_t>(nextStep);
    const auto next = self->facingQuadrantPath[nextStep];
    if (current < 0 || next < 0 || current >= g_pFightStage->areaMaskCount || next >= g_pFightStage->areaMaskCount) {
        self->facingQuadrantStep = -1;
        self->cameraControlFlags = 0;
        return 0;
    }
    const auto &currentArea = g_pFightStage->areaMasks[static_cast<std::uint8_t>(current)];
    std::uint8_t edge = 4;
    for (std::uint8_t i = 0; i < 4; ++i)
        if (currentArea.neighbors[i] == next) {
            edge = i;
            break;
        }
    const auto point = VFFacingQuadrantBoundaryTarget(currentArea, g_pFightStage->areaMasks[static_cast<std::uint8_t>(next)], edge);
    self->followTargetPosition.x = point.x;
    self->followTargetPosition.z = point.y;
    return 0;
}

// @verified - p
std::uint8_t vfEvalState47SfxFollowupGate(VFActor *self, VFSlotRuntimeState *) {
    VFMAData &game = *self->game;
    const std::int8_t playerSlot = game.playerSlot;
    const std::uint8_t primaryTargetSlot = game.primaryTargetSlot;
    if (game.soundFlag != 0) {
        if (game.soundFlag != 1 || (game.soundSlots[self->slotIndex] & 1) == 0)
            return 0xffU;
        if (playerSlot == -1 || primaryTargetSlot != 0xffU || (self->motionFlags & 0x800000U) != 0)
            vfResetSfxTimers(game);
        return 47;
    }
    const VFFightStage &stage = *g_pFightStage;
    if (playerSlot == -1 || primaryTargetSlot != 0xffU || stage.aliveByTeam[1] < 3 || 5.0f > self->contactPoints[playerSlot] ||
        (self->motionFlags & 0x800000U) != 0)
        return 0xffU;
    if (stage.useCenterBias) {
        const VFActor *player = game.actors[playerSlot];
        const float cx = stage.battleCenter.x, cz = stage.battleCenter.z;
        const float selfDistance = (self->position.x - cx) * (self->position.x - cx) + (self->position.z - cz) * (self->position.z - cz);
        const float playerDistance = (player->position.x - cx) * (player->position.x - cx) + (player->position.z - cz) * (player->position.z - cz);
        if (selfDistance > playerDistance)
            return 0xffU;
    }
    game.soundFlag = 1;
    game.soundFrames = 0;
    return 47;
}

std::uint8_t vfUpdateFollowupProbePointAndCheckQuadrant(VFActor *self, VFSlotRuntimeState *slot_runtime);


void vfSetAllSfxSlotsTo(VFMAData &game, std::uint8_t state) {
    for (std::size_t i = 0; i < std::size(game.soundSlots); ++i) {
        if ((game.soundSlots[i] & 2) != 0)
            game.soundSlotState[i] = state;
    }
}

void vfResetSfxTimers(VFMAData &game) {
    game.soundResetTimer = 0;
    game.soundFlag = 0;
    game.soundFrames = 0;
    game.soundTimer = 0;
    for (std::size_t i = 0; i < std::size(game.soundSlots); ++i) {
        game.soundSlotOrder[i] = 0;
        game.soundSlots[i] = 0;
        game.soundSlotState[i] = 0;
        if (game.characters[i] && game.actors[i] && game.actors[i]->slot)
            game.actors[i]->slot->slotIndex = 0xFFu;
    }
}

// @verified - p
std::uint8_t vfUpdateSfxFollowupSequence(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !self->game || !slot_runtime)
        return 0;
    VFMAData &game = *self->game;
    const std::uint8_t groupIdx = self->slotIndex;
    const std::uint8_t playerSlot = static_cast<std::uint8_t>(game.playerSlot);
    self->cameraControlFlags = 0;

    if (game.soundFrames == 0) {
        if (groupIdx >= std::size(game.soundSlots))
            return 0;
        game.actorIndex = groupIdx;
        game.soundContactThreshold = 6.0f;
        game.soundSlots[groupIdx] |= 1;
        game.soundSlotOrder[groupIdx] = 0;
        std::uint8_t order = 1;
        for (std::uint8_t i = 0; i < std::size(game.actors); ++i) {
            if (i == groupIdx || !game.characters[i] || game.slotTeamId[i] != 1u)
                continue;
            game.soundSlotOrder[i] = order++;
            game.soundSlots[i] |= 2;
            game.soundSlotState[i] = 2;
        }
        ++game.soundFrames;
        return groupIdx;
    }

    if (game.soundFrames == 1) {
        std::uint8_t actorsOutsideThreshold = 0;
        for (std::uint8_t i = 0; i < std::size(game.actors); ++i) {
            VFActor *actor = game.actors[i];
            if (!game.characters[i] || !actor || (game.soundSlots[i] & 2) == 0)
                continue;
            if (game.soundContactThreshold <= actor->contactPoints[groupIdx])
                ++actorsOutsideThreshold;
            else
                game.soundSlotState[i] = 1;
        }
        if (actorsOutsideThreshold == 0) {
            vfSetAllSfxSlotsTo(game, 2);
            game.actorIndex = groupIdx;
            game.soundContactThreshold = 2.0f;
            ++game.soundFrames;
        }
        return 0;
    }

    if (game.soundFrames == 2) {
        if (game.playerSlot < 0 || playerSlot >= std::size(game.actors)) {
            vfResetSfxTimers(game);
            return 0;
        }
        slot_runtime->slotIndex = playerSlot;
        if (vfValidateFacingQuadrantForContextAction(self, slot_runtime) != 0xffU)
            return vfUpdateFollowupHitTargetPosition(self, slot_runtime);
        if (vfEvalFollowupHitWindow(self, slot_runtime) != 0xffU)
            return vfSetFollowupHitFor(self, slot_runtime);

        VFActor *player = game.actors[playerSlot];
        if (!player || player->forwardClearance < 3.0f || player->backwardClearance < 3.0f || player->leftClearance < 3.0f || player->rightClearance < 3.0f) {
            vfResetSfxTimers(game);
            return 0;
        }

        vfSetAllSfxSlotsTo(game, 3);
        game.actorIndex = playerSlot;
        game.soundContactThreshold = 3.5f;
        game.soundTimer = 120;
        game.soundResetTimer = 90;
        ++game.soundFrames;
        return 0;
    }

    if (game.soundFrames == 3) {
        (void)vfUpdateFollowupProbePointAndCheckQuadrant(self, slot_runtime);
        if (groupIdx < std::size(game.soundSlots) && (game.soundSlots[groupIdx] & 0x10) == 0 && game.soundTimer) {
            if (game.soundResetTimer) {
                if (game.soundResetTimer == 1)
                    vfSetAllSfxSlotsTo(game, 4);
            } else {
                vfSetAllSfxSlotsTo(game, 3);
                game.soundResetTimer = 90;
            }
            return groupIdx;
        }
        vfResetSfxTimers(game);
    }
    return 0;
}

// @verified - p
std::uint8_t vfEvalState48CloseContactPointGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    return self->contactPoints[slot_runtime->slotIndex & 7u] > 5.0f ? 0xffU : 48;
}

// @verified - p
std::uint8_t vfUpdateFollowupProbePointAndCheckQuadrant(VFActor *self, VFSlotRuntimeState *) {
    if (!self || !self->game)
        return 0;
    VFMAData *game = self->game;
    VFActor *cameraActor = game->actorIndex < std::size(game->actors) ? game->actors[game->actorIndex] : nullptr;
    if (!cameraActor || self->slotIndex >= std::size(cameraActor->contactAngles))
        return 0;

    const std::int16_t contactAngle = cameraActor->contactAngles[self->slotIndex];
    const std::uint8_t angleIndex = static_cast<std::uint8_t>((static_cast<std::uint8_t>(static_cast<std::int16_t>(contactAngle) >> 13) + 1u) & 7u);
    self->followTargetPosition.x = cameraActor->contactCurrentPosition.x + kVFMainQuadrantOffsets[angleIndex].x;
    self->followTargetPosition.z = cameraActor->contactCurrentPosition.z + kVFMainQuadrantOffsets[angleIndex].y;

    bool insideArea = false;
    const VFFightStage *stage = g_pFightStage;
    if (stage) {
        for (std::uint8_t quadrant = 0; quadrant < stage->areaMaskCount; ++quadrant) {
            if (vfStageCheckPointInQuadrantXZ(self->followTargetPosition.x, self->followTargetPosition.z, static_cast<std::int8_t>(quadrant))) {
                insideArea = true;
                break;
            }
        }
    }

    if (!insideArea) {
        if (self->slotIndex < std::size(game->soundSlots))
            game->soundSlots[self->slotIndex] |= 0x10;
        return self->slotIndex;
    }
    self->followupState = 0x2;
    self->cameraControlFlags = 0x2000U;
    return 0;
}

// @verified - p
std::uint8_t vfReturnState49(VFActor *, VFSlotRuntimeState *) {
    return 49;
}

// @verified - p
std::uint8_t VFActor_UpdateCameraAndContactPosition(VFActor *self, VFSlotRuntimeState *) {
    if (!self || !self->game)
        return 0;
    VFActor *target = self->game->actorIndex < std::size(self->game->actors) ? self->game->actors[self->game->actorIndex] : nullptr;
    if (!target)
        return 0;
    self->cameraControlFlags = 0x2002u;
    self->followTargetPosition.x = target->contactCurrentPosition.x;
    self->followTargetPosition.z = target->contactCurrentPosition.z;
    return static_cast<std::uint8_t>(self->cameraControlFlags);
}

// @verified - p
std::uint8_t vfFindStageHitObjectForGuardAngleResponse(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    auto &game = *self->game;
    if (game.guardEffectTimer < 450U || slot_runtime->currentStageHitResponseFrames)
        return 0xffU;
    const float point = self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex)];
    if (point < 4.0f || point > 12.0f)
        return 0xffU;
    for (const auto *object = g_vfObjectRoot; object; object = object->next)
        if ((object->stateFlags & 1U) != 0 && object->character == self->character)
            return 0xffU;
    std::int8_t selected = -1;
    for (const auto *object = g_vfObjectRoot; object; object = object->next) {
        if ((object->flags & 0xb00000U) != 0xb00000U || object->state || (object->stateFlags & 1U) != 0)
            continue;
        if (object->object->state == 2) {
            const std::uint16_t angle =
                Shenmue_Atan2Fixed(self->contactCurrentPosition.x - object->position.x, self->contactCurrentPosition.z - object->position.z);
            const std::int32_t difference = object->rotation.y - angle;
            const auto yaw = static_cast<std::int16_t>(difference >= 0 ? difference : -difference);
            if (static_cast<std::uint16_t>(yaw - 10241) <= 12286U)
                continue;
        }
        bool used = false;
        for (std::uint8_t i = 0; i < kBattleSlots; ++i)
            if (game.characters[i] && i != self->slotIndex && game.actors[i]->stageHitObjectIndex == object->objectIndex) {
                used = true;
                break;
            }
        if (used)
            continue;
        const float dx = self->contactCurrentPosition.x - object->position.x;
        const float dz = self->contactCurrentPosition.z - object->position.z;
        const float distance = std::sqrt(dx * dx + dz * dz);
        if (distance >= 4.0f || point <= distance)
            continue;
        if (self->cachedQuadrant != 0xffU) {
            std::uint8_t quadrant = 0;
            while (quadrant < g_pFightStage->areaMaskCount &&
                !vfStageCheckPointInQuadrantXZ(object->position.x, object->position.z, static_cast<std::int8_t>(quadrant)))
                ++quadrant;
            if (quadrant == g_pFightStage->areaMaskCount ||
                !VFAreaCanReachQuadrant(static_cast<std::int8_t>(self->cachedQuadrant), static_cast<std::int8_t>(quadrant)))
                continue;
        }
        selected = static_cast<std::int8_t>(object->objectIndex);
    }
    if (selected == -1)
        return 0xffU;
    if ((self->motionFlags & 0x800000U) == 0)
        self->stageHitObjectIndex = selected;
    return 50;
}

// @verified - p
std::uint8_t vfAssignFollowTargetPosForFollowup(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *const object = g_vfStageObjs[static_cast<std::uint8_t>(self->stageHitObjectIndex)];
    const std::uint16_t yaw = Shenmue_Atan2Fixed(self->contactCurrentPosition.x - object->position.x, self->contactCurrentPosition.z - object->position.z);
    const auto relative = static_cast<std::int16_t>(0x4000 - self->temporaryFacingAngle + yaw);
    const std::uint16_t frames = slot_runtime->byteaa | (static_cast<std::uint16_t>(slot_runtime->unknownab) << 8);
    if (frames <= 300U) {
        if (((object->environmentFlags >> self->slotIndex) & 1) == 0 || relative < 0) {
            self->followTargetPosition.x = object->position.x;
            self->followTargetPosition.z = object->position.z;
            self->cameraControlFlags = 0x2000U;
            self->followupState = 2;
            return 0;
        }
        return ResetAndSelectCameraPhaseChoice(*self, 210, 0);
    }
    slot_runtime->currentStageHitResponseFrames = 300;
    return 44;
}
// @verified - p
std::uint8_t vfEvalState51HitState800000Gate(VFActor *self, VFSlotRuntimeState *) {
    return self && (self->motionFlags & 0x800000) != 0 ? 51 : 0xffU;
}
// @verified - p
std::uint8_t vfUpdateCameraRelatedBasedOnContactAngles(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime)
        return 0;
    const std::int8_t targetSlot = self->targetSlotIndex;
    if (targetSlot < 0 || static_cast<std::size_t>(targetSlot) >= std::size(self->contactPoints))
        return 0;

    const std::size_t targetIndex = static_cast<std::size_t>(targetSlot);
    const std::int16_t relativeAngle = static_cast<std::int16_t>(self->temporaryFacingAngle - static_cast<std::uint16_t>(self->contactAngles[targetIndex]));
    const float distance = self->contactPoints[targetIndex];
    if (distance < 4.0f || distance > 15.0f || !self->game || self->game->guardEffectTimer < 300u) {
        return ResetAndSelectCameraPhaseChoice(*self, 211, 0);
    }

    if (distance < 7.0f || static_cast<std::uint16_t>(slot_runtime->currentStageHitResponseFrames) > 150u) {
        (void)ResetAndSelectCameraPhaseChoice(*self, 212, 0);
        self->cameraControlFlags = 0;
        return 0;
    }
    if (relativeAngle < -1024) {
        self->cameraControlFlags = 0x80;
        return static_cast<std::uint8_t>(0x80);
    }
    if (relativeAngle > 1024) {
        self->cameraControlFlags = 0x40;
        return static_cast<std::uint8_t>(0x40);
    }
    return 0;
}

// @verified - p
std::uint8_t vfEvalMultiEnemyAirAssistWindow(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto *target = self->targetActor;
    auto &game = *self->game;
    if (game.battleType == 5U && game.enemyRanks[self->slotIndex] == 1)
        return 0xffU;
    if ((self->groundContactFlags & 1) != 0 || target->pairContactPriority || (self->hitStateFlags2 & 0x2001d010U) != 0 ||
        (self->motionFlags & 0x08000000U) != 0 || g_pFightStage->aliveByTeam[game.battleMeta.groups[self->slotIndex]->teamId] < 2U ||
        (target->hitStateFlags2 & 0x20000000U) != 0)
        return 0xffU;
    const double distance = self->contactPoints[self->targetSlotIndex];
    const double assist = slot_runtime->assistWindowCurrent;
    if (distance > assist - 0.2)
        return 0xffU;
    return assist - 0.8 > distance ? 0xffU : 53;
}

// @verified - p
std::uint8_t vfSetCamCtrlFlagsFromSfxSlotOrder(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    const auto order = self->game->soundSlotOrder[self->slotIndex];

    auto &side = reinterpret_cast<std::uint8_t *>(self->game)[0x2a3U + order];
    const auto *target = self->targetActor;
    if ((slot_runtime->byteaa & 0x1fU) == 0) {
        side = (NextVFFrameEventRandomWord() & 1U) != 0 ? 1U : 2U;
        if ((target->motionFlags & 0x800U) != 0)
            side =
                static_cast<std::uint8_t>((static_cast<std::int16_t>(target->facingAngle) - static_cast<std::int16_t>(target->contactAngles[order]) >= 0) + 1);
    }
    self->cameraControlFlags = side == 2U ? 0x1040U : 0x1080U;
    return static_cast<std::uint8_t>(self->cameraControlFlags);
}

// @verified - p
std::uint8_t vfCheckEnemyAttackPressureGate(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    auto &game = *self->game;
    if (game.battleType == 5U) {
        if (game.remainingEnemies < 20U)
            return 0xffU;
        if (game.primaryTargetSlot != 0xffU && (game.battleMeta.waves[self->slotIndex]->selectFlags & 0x80U) != 0)
            return 0;
        if (game.enemyRanks[self->slotIndex] == 1)
            return 0xffU;
    }
    for (std::uint8_t i = 0; i < kBattleSlots; ++i)
        if (game.characters[i] && game.slotTeamId[i] == game.slotTeamId[self->slotIndex] && i != self->slotIndex && (game.actors[i]->hitFlags & 0x100U) != 0)
            return 0;
    if (game.playerSlot != -1 && self->slotIndex != game.actors[static_cast<std::uint8_t>(game.playerSlot)]->pairedFighterId)
        slot_runtime->phaseConfig[0] >>= 2;
    return 0xffU;
}

// @verified - p
std::uint8_t vfEvalState56PlayerContactPointGate(VFActor *self, VFSlotRuntimeState *) {
    if (!self || !self->game || self->game->playerSlot == -1)
        return 0xffU;
    return 6.0f > self->contactPoints[static_cast<std::uint8_t>(self->game->playerSlot) & 7u] ? 0xffU : 56;
}

// @verified - p
std::uint8_t VFActor_SetActorTableIndexTemporarily(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !self->game || !slot_runtime)
        return 0;
    const std::uint8_t saved = slot_runtime->slotIndex;
    slot_runtime->slotIndex = static_cast<std::uint8_t>(self->game->playerSlot);
    const std::uint8_t result = vfUpdateFollowupHitTargetPosition(self, slot_runtime);
    slot_runtime->slotIndex = saved;
    return result;
}

// @verified - p
std::uint8_t vfEvalState57ContactVsSlotThreshold(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime)
        return 0xffU;
    const float distance = self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u];
    if (distance > 4.0f)
        return 0xffU;
    return static_cast<double>(slot_runtime->assistWindowCurrent) - 1.0 > static_cast<double>(distance) ? 0xffU : 57;
}

// @verified - p
std::uint8_t Script_vfUpdateCameraCtrlFlags(VFActor *self, VFSlotRuntimeState *slot_runtime) {
    if (!self || !slot_runtime)
        return 0;
    if ((slot_runtime->previousUpdateIndex & 0x20u) != 0) {
        self->cameraControlFlags = 0;
        return 0;
    }
    if ((slot_runtime->previousUpdateIndex & 0x3Fu) == 0) {
        std::uint8_t randomSector = 0;
        const float contactPoint = self->contactPoints[static_cast<std::uint8_t>(self->targetSlotIndex) & 7u];
        if (contactPoint <= 3.0f)
            randomSector = !(static_cast<double>(slot_runtime->assistWindowCurrent) - 0.7 > static_cast<double>(contactPoint)) ?
                static_cast<std::uint8_t>(((g_EnvPrngSeed = LcgStep(g_EnvPrngSeed)) >> 16) & 7u) :
                4;
        slot_runtime->byteb8 = randomSector;
    }
    self->cameraControlFlags = kVFMainCameraSectors[slot_runtime->byteb8 & 7u];
    return static_cast<std::uint8_t>(self->cameraControlFlags);
}
constexpr VFMainCallbackPair kVFMainCallbacks[] = {
    {vfCallbackNoOp, vfRefreshRandomPhaseRoll},
    {vfEvalRandomPhaseGate, vfSelectRandomCameraPhaseChoice},
    {vfEvalFollowupHitWindow, vfSetFollowupHitFor},
    {vfEvalState03GroundedNoPairContact, vfResetAndSelectRandomCameraPhaseFromMovePool54OrFallback},
    {vfEvalState04PhaseCfg10Gate, vfSetCamCtrlFlagsByFrameParity},
    {vfEvalState05GuardOrMoveParamGate, vfResetCameraStateAndSetCamCtrlByOpponentState},
    {vfEvalState06AirbornePairContactGate, vfResetAndSelectRandomCameraPhaseFromMovePool58Or5C},
    {vfCallbackReject, vfCallbackNoOp},
    {vfCallbackReject, vfCallbackNoOp},
    {vfCallbackReject, vfCallbackNoOp},
    {vfCallbackReject, vfCallbackNoOp},
    {vfEvalState11ContactDecisionBit8Gate, vfResetAndSelectRandomCameraPhaseFromStylePool60},
    {vfEvalState12LowTargetHpAirContact, vfSelectRandomAttackWindowEndPhaseChoice},
    {vfEvalState13AssistAngleGate, vfSelectRandomReactState02EndPhaseChoice},
    {vfEvalState01GroundedTargetAssistGate, nullptr},
    {vfEvalState15AirbornePairContactThreshold, vfResetCameraStateAndSetCamCtrlFlag4},
    {vfCheckConfirmInputForFrontContact, vfSelectCameraPhaseFromAnimSetOrRandomFallback},
    {vfEvalState17Slot174FrontContactGate, vfHandleHpRegenPhasePostUpdate},
    {vfEvalState05GuardWindowGateA, nullptr},
    {vfEvalState05GuardWindowGateB, nullptr},
    {vfTriggerFollowupState2OnSfxContact, nullptr},
    {vfEvalState21ContactDecision6000000Gate, vfSelectRandomCameraPhaseFromMovePool62},
    {vfEvalState22ContactAction2400Gate, vfSelectCameraPhase205IfMoveFlag100Clear},
    {vfEvalState23ContactAction5000Gate, vfSelectCameraPhase206IfMoveFlag100Clear},
    {vfEvalState24ContactAction8800Gate, vfSelectCameraPhase207IfMoveFlagSet},
    {vfEvalState25ContactDecisionBit4Gate, vfResetAndSelectRandomCameraPhaseFromMovePool5C},
    {vfEvalState26CloseAttackContactGate, vfResetCameraStateAndSetWallSideCamCtrl},
    {vfEvalState27FarContactNoPairGate, vfSelectCameraPhase204OrRandom201202},
    {vfEvalState28FollowupClearanceGate, vfSelectCameraPhaseFromContactActionAndSide},
    {vfEvalState29GroundedNoPairContact, vfSelectRandomCameraPhaseFromFallbackPool4},
    {vfEvalState30GroundedClearanceGate, vfSelectRandomCameraPhaseByClearance},
    {vfEvalState31UpdateCamContactGate, vfResetCameraStateAndSetCamCtrlFlag20},
    {vfEvalState32ProbeDistanceGate, vfResetAndSelectRandomCameraPhaseFromStylePool6A},
    {nullptr, nullptr},
    {nullptr, nullptr},
    {vfEvalState22ContactAction400Gate, nullptr},
    {nullptr, nullptr},
    {nullptr, nullptr},
    {nullptr, nullptr},
    {nullptr, nullptr},
    {vfCallbackReject, vfCallbackNoOp},
    {vfCallbackReject, vfCallbackNoOp},
    {vfEvalState05MoveFlag40000GuardGate, nullptr},
    {nullptr, nullptr},
    {vfValidateFacingQuadrantForContextAction, vfUpdateFollowupHitTargetPosition},
    {nullptr, nullptr},
    {vfCallbackReject, vfCallbackNoOp},
    {vfEvalState47SfxFollowupGate, vfUpdateSfxFollowupSequence},
    {vfEvalState48CloseContactPointGate, vfUpdateFollowupProbePointAndCheckQuadrant},
    {vfReturnState49, VFActor_UpdateCameraAndContactPosition},
    {vfFindStageHitObjectForGuardAngleResponse, vfAssignFollowTargetPosForFollowup},
    {vfEvalState51HitState800000Gate, vfUpdateCameraRelatedBasedOnContactAngles},
    {vfCallbackReject, vfCallbackNoOp},
    {vfEvalMultiEnemyAirAssistWindow, vfSetCamCtrlFlagsFromSfxSlotOrder},
    {vfCallbackReject, nullptr},
    {vfCheckEnemyAttackPressureGate, nullptr},
    {vfEvalState56PlayerContactPointGate, VFActor_SetActorTableIndexTemporarily},
    {vfEvalState57ContactVsSlotThreshold, Script_vfUpdateCameraCtrlFlags},
};
} // namespace vf_detail

} // namespace shenmue::s1
