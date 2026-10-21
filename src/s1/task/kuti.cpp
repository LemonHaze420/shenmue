#include "s1/task/kuti.hpp"

#include "s1/face.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/motion.hpp"
#include "s1/task/char.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
namespace {

constexpr std::uint32_t kCharacterHandlerType = SHEN_NAME(CEWP);
constexpr std::size_t kLargeScratchPayload = 0x960;
constexpr std::size_t kSmallScratchPayload = 0x4c0;
constexpr std::uint16_t kNoDialogueSlot = 129;

constexpr std::int32_t ScratchToken(std::size_t bank, std::size_t pool) {
    return static_cast<std::int32_t>(make_shen_name('P', 'A', static_cast<char>('0' + bank), static_cast<char>('0' + pool)));
}

static_assert(static_cast<std::uint32_t>(ScratchToken(0, 0)) == SHEN_NAME(PA00));
static_assert(static_cast<std::uint32_t>(ScratchToken(0, 1)) == SHEN_NAME(PA01));
static_assert(static_cast<std::uint32_t>(ScratchToken(0, 2)) == SHEN_NAME(PA02));
static_assert(static_cast<std::uint32_t>(ScratchToken(2, 2)) == SHEN_NAME(PA22));

std::int32_t BlockToken(const HLib::TaskData &block) {
#if defined(SHENMUE_PLATFORM_DREAMCAST)
    return static_cast<std::int32_t>(block.taskToken.name);
#else
    return block.taskToken;
#endif
}

std::int32_t BlockStatus(const HLib::TaskData &block) {
#if defined(SHENMUE_PLATFORM_DREAMCAST)
    return static_cast<std::int32_t>(block.status.name);
#else
    return block.status;
#endif
}

bool IsOwnedScratchPool(void *pool, std::int32_t token) {
    if (!pool)
        return false;
    const auto *const block = static_cast<const HLib::TaskData *>(pool) - 1;
    return BlockStatus(*block) == HLIB_USED_TOKEN.name && BlockToken(*block) == token;
}

void ReleaseScratchPools() {
    for (std::size_t bank = 0; bank < g_kutiScratchBanks.size(); ++bank) {
        KutiScratchBank &scratchBank = g_kutiScratchBanks[bank];
        for (std::size_t pool = 0; pool < scratchBank.pools.size(); ++pool) {
            void *&allocation = scratchBank.pools[pool];
            const std::int32_t token = ScratchToken(bank, pool);
            if (IsOwnedScratchPool(allocation, token)) {
                HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(allocation), token);
            }
            allocation = nullptr;
        }
        scratchBank.vertexCount = 0;
        scratchBank.frame = 0;
        scratchBank.frameCount = 0;
        scratchBank.allocated = 0;
    }
}

bool AllocateScratchPools() {
    for (std::size_t bank = 0; bank < g_kutiScratchBanks.size(); ++bank) {
        KutiScratchBank &scratchBank = g_kutiScratchBanks[bank];

        for (std::size_t pool = 0; pool < scratchBank.pools.size(); ++pool) {
            const std::size_t payloadSize = pool == 0 ? kLargeScratchPayload : kSmallScratchPayload;
            const std::int32_t token = ScratchToken(bank, pool);
            void *const allocation = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(payloadSize), token);

            scratchBank.pools[pool] = allocation;
        }
        scratchBank.allocated = 0;
    }
    for (KutiScratchBank &scratchBank : g_kutiScratchBanks) {
        for (void *const allocation : scratchBank.pools)
            HLib_EnableTokenCheck(allocation);
    }
    return true;
}

bool IsActiveCharacter(const Character &character) {
    if (character.entity.name != kCharacterHandlerType)
        return false;
    const std::uint32_t flags = character.flags.value;
    return (flags & 1U) != 0U || ((flags & 2U) != 0U && (character.motionFlags & 1U) != 0U);
}

void ResetPublishedState() {
    g_clipMemory.fill(nullptr);
    g_pCurrentFaceActor = nullptr;
    g_pFACE = nullptr;
    g_activeDialoguePosition = {};
    g_activeDialogueSlot = kNoDialogueSlot;
    g_clipSlotHighWaterIndex = 0;
    g_clipScanIndex = 0;
}

} // namespace

std::array<KutiScratchBank, 3> g_kutiScratchBanks{};
std::array<Character *, 128> g_clipMemory{};
void *g_pCurrentFaceActor = nullptr;
void *g_pFACE = nullptr;
Vector3 g_activeDialoguePosition{};
std::uint16_t g_activeDialogueSlot = 129;
std::int16_t g_clipSlotHighWaterIndex = 0;
std::int16_t g_clipScanIndex = 0;
HLib::Task *g_kutiTask = nullptr;

void KUTI_Callback(HLib::TaskData *) {
    const std::int16_t scanLimit = g_clipSlotHighWaterIndex;
    g_clipScanIndex = 0;
    g_activeDialogueSlot = kNoDialogueSlot;
    if (scanLimit <= 0)
        return;

    while (g_clipScanIndex < scanLimit && g_clipScanIndex < static_cast<std::int16_t>(g_clipMemory.size())) {
        const std::int16_t index = g_clipScanIndex;
        Character *&character = g_clipMemory[static_cast<std::size_t>(index)];
        if (!character || !IsActiveCharacter(*character)) {
            character = nullptr;
        } else {
            FaceClipControllerState *const clip = CLIP_GetWork(character);
            if (clip && clip->activeLipCues) {
                g_activeDialogueSlot = static_cast<std::uint16_t>(index);

                const Matrix4x4 callerTransform = workTransformMatrix;
                *g_matrixStack.top.stack = callerTransform;
                ++g_matrixStack.top.info.index;
                if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity) {
                    ++g_matrixStack.top.stack;
                }

                const Matrix4x4 *transform = GetTransformForFACE_ForSpecificChara(character, 5);
                if (!transform)
                    transform = g_matrixStack.top.stack;
                workTransformMatrix = *transform;
                g_activeDialoguePosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};

                std::int32_t count = 1;
                if (g_matrixStack.top.info.index < 1)
                    count = g_matrixStack.top.info.index;
                std::int32_t stackCount = count;
                if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
                    stackCount = count + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index;
                }
                g_matrixStack.top.info.index -= count;
                g_matrixStack.top.stack -= stackCount;
                workTransformMatrix = *g_matrixStack.top.stack;
                return;
            }
        }
        g_clipScanIndex = static_cast<std::int16_t>(index + 1);
    }
}

Character **KUTI_Init() {
    g_kutiTask = HLib_EnqueueTaskWithoutParameter(KUTI_Callback, 8, 0, TASK_KUTI.name);
    g_pCurrentFaceActor = nullptr;
    g_pFACE = nullptr;
    g_activeDialogueSlot = kNoDialogueSlot;
    g_activeDialoguePosition = {};
    AllocateScratchPools();
    g_clipMemory.fill(nullptr);

    g_clipSlotHighWaterIndex = 0;

    g_clipScanIndex = 0;

    return g_clipMemory.data();
}

void KUTI_Cleanup() {
    HLib::Task *const task = g_kutiTask;
    g_kutiTask = nullptr;
    if (task && task->mainTaskCallbackFnPtr == KUTI_Callback) {
        HLib_DestroyTask(task);
    }
    ResetPublishedState();
    ReleaseScratchPools();
}

void KUTI_BeginFaceHandoff(void *faceActor, void *faceWork) {
    g_pCurrentFaceActor = faceActor;
    g_pFACE = faceWork;
}

void KUTI_EndFaceHandoff() {
    g_pFACE = nullptr;
    g_pCurrentFaceActor = nullptr;
}

std::int16_t KUTI_RegisterClipCharacter(Character *character) {
    if (!character)
        return -1;

    std::size_t index = 0;
    while (index < g_clipMemory.size() && g_clipMemory[index])
        ++index;
    if (index < g_clipMemory.size()) {
        g_clipMemory[index] = character;
        if (FaceClipControllerState *const clip = CLIP_GetWork(character))
            clip->field1a = static_cast<std::uint16_t>(index);
    }
    const std::int16_t cursor = static_cast<std::int16_t>(index);
    if (cursor > g_clipSlotHighWaterIndex)
        g_clipSlotHighWaterIndex = cursor;
    return g_clipSlotHighWaterIndex;
}

bool KUTI_GetActiveDialogueAnchor(Character *&character, Vector3 &position) {
    const std::size_t index = g_activeDialogueSlot;
    if (index >= g_clipMemory.size())
        return false;
    character = g_clipMemory[index];
    if (!character)
        return false;
    position = g_activeDialoguePosition;
    return true;
}

} // namespace shenmue::s1
