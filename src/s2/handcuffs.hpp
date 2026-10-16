#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct Character;
struct MotionWork;

inline constexpr shen_name TASK_CUFF{SHEN_NAME(CUFF)};
inline constexpr shen_name MODEL_HANDCUFF_HIGH{SHEN_NAME(TEJ1)};
inline constexpr shen_name MODEL_HANDCUFF_LOW{SHEN_NAME(TEJ0)};
inline constexpr std::array<const char *, 2> HANDCUFF_MODEL_PATHS{"misc/TEJOB01G.MT7", "misc/TEJOB00G.MT7"};
inline constexpr Vector3 HANDCUFF_CHAIN_FORCE{0.0f, 100.0f, 0.0f};

#pragma pack(push, 4)
struct HandcuffConstraint {
    HandcuffConstraint *peer;
    Character *character;
    std::int32_t bone;
    float maximumDistance;
    std::uint32_t flags;
    std::uint32_t reserved1c;
    MT7Model *model;
};
struct HandcuffsTaskData {
    MT7Model *highModel;
    MT7Node *highChainLink;
    MT7Model *lowModel;
    MT7Node *lowChainLink;
    std::uint32_t active;
    std::int32_t chainCount;
    HandcuffConstraint first;
    HandcuffConstraint second;
    Vector3 attachmentOffset;
    std::byte reserved84[0x0c];
};
#pragma pack(pop)
static_assert(sizeof(HandcuffConstraint) == 0x28);
static_assert(offsetof(HandcuffConstraint, character) == 0x08);
static_assert(offsetof(HandcuffConstraint, bone) == 0x10);
static_assert(offsetof(HandcuffConstraint, maximumDistance) == 0x14);
static_assert(offsetof(HandcuffConstraint, flags) == 0x18);
static_assert(offsetof(HandcuffConstraint, model) == 0x20);
static_assert(sizeof(HandcuffsTaskData) == 0x90);
static_assert(offsetof(HandcuffsTaskData, active) == 0x20);
static_assert(offsetof(HandcuffsTaskData, first) == 0x28);
static_assert(offsetof(HandcuffsTaskData, second) == 0x50);
static_assert(offsetof(HandcuffsTaskData, attachmentOffset) == 0x78);

struct HandcuffsModelResource {
    void *allocation{};
    MT7Model model;
};

extern HLib::Task *g_Handcuffs_Task;
void Handcuffs_Enqueue();
void Handcuffs_Callback(void *taskData);
void Handcuffs_Render(void *taskData);
std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float maximumDistance);
void Handcuffs_ConstrainWrist(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *parameter);
void Handcuffs_BuildChainTransform(const Vector3 &first, const Vector3 &second, Matrix4x4 &transform);
void cuffsSetFlags();
// The map owner calls this after its preceding task/heap retirement, not as an active-cuff detach operation.
void resetHandcuffTask();
void ReleaseHandcuffsModels();

} // namespace shenmue::s2
