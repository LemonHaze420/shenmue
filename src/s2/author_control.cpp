#include "s2/author_control.hpp"

#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hlib.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <cstring>

namespace shenmue::s2 {
namespace {

struct SubcontrollerSlot {
    HLib::Task *task;
    shen_name subject;
    std::uint32_t reserved;
};

struct EyesController {
    std::int32_t mode;
    std::uint32_t reserved;
    const std::byte *record;
};

struct PartsController {
    std::int32_t mode;
    std::uint32_t reserved;
    const std::byte *record;
    Vector3 position;
    Vector3i rotation;
    Vector3 positionStep;
    Vector3i rotationStep;
};

static_assert(sizeof(SubcontrollerSlot) == 0x10);
static_assert(sizeof(EyesController) == 0x10);
static_assert(offsetof(EyesController, record) == 8);
static_assert(sizeof(PartsController) == 0x40);
static_assert(offsetof(PartsController, record) == 8);
static_assert(offsetof(PartsController, position) == 0x10);
static_assert(offsetof(PartsController, rotation) == 0x1c);
static_assert(offsetof(PartsController, positionStep) == 0x28);
static_assert(offsetof(PartsController, rotationStep) == 0x34);

std::array<SubcontrollerSlot, 32> eyesControllers{};
std::array<SubcontrollerSlot, 32> partsControllers{};
constexpr std::array<std::uint16_t, 8> eyeBoneIds{1, 5, 3, 21, 14, 31, 26, 0};
constexpr std::array<std::uint16_t, 104> partNodeIds{0xffbf, 0xffbe,
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
    162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
    172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
    202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
    212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
    222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
    232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
    150, 151};

template <typename T> T ReadRecord(const std::byte *record, std::size_t offset) {
    T value;
    std::memcpy(&value, record + offset, sizeof(value));
    return value;
}

SubcontrollerSlot *FindSlot(std::array<SubcontrollerSlot, 32> &slots, shen_name subject) {
    SubcontrollerSlot *existing = nullptr;
    SubcontrollerSlot *available = nullptr;
    for (SubcontrollerSlot &slot : slots) {
        if (slot.task && slot.subject.name == subject.name)
            existing = &slot;
        else if (!slot.task && !available)
            available = &slot;
    }
    return existing ? existing : available;
}

void ReleaseTask(HLib::Task *task) {
    if (!task)
        return;
    if (task->taskData) {
        HLib::FreeHeapSpace(task->taskData, static_cast<std::int32_t>(SHEN_NAME(TASK)));
        task->taskData = nullptr;
    }
    HLib::DestroyTask(task);
}

void RetireCurrent(std::array<SubcontrollerSlot, 32> &slots, shen_name subject) {
    for (SubcontrollerSlot &slot : slots) {
        if (slot.subject.name != subject.name)
            continue;
        slot = {};
        break;
    }
    HLib::FreeCurrentTaskData();
    HLib::DestroyCurrentTask();
}

void *CreateController(SubcontrollerSlot &slot, shen_name subject, HLib::Task *parent, HLib::TaskCallback callback, shen_name token, std::size_t size) {
    HLib::Task *const task = HLib::QueueTaskAfterCurrentTask(parent, callback, 3, 0x0b, static_cast<std::int32_t>(token.name));
    if (!task)
        return nullptr;
    void *const payload = HLib::AllocateTaskData(size, SHEN_NAME(TASK));
    if (!payload) {
        HLib::DestroyTask(task);
        return nullptr;
    }
    HLib::AssignDynamicAlignmentForTaskData(payload);
    HLib::AssignTaskDataToTask(task, payload);
    slot.task = task;
    slot.subject = subject;
    return payload;
}

// @verified - p
void EyesCallback(void *payload) {
    auto &state = *static_cast<EyesController *>(payload);
    const shen_name subject = ReadRecord<shen_name>(state.record, 8);
    const AuthorController *const author = Author_GetController();
    if (!author || !author->loaded) {
        RetireCurrent(eyesControllers, subject);
        return;
    }
    Character *const character = scenFindChara(author->task, subject);
    if (!character)
        return;
    const std::int16_t flags = ReadRecord<std::int16_t>(state.record, 0x12);
    if (state.mode != 0) {
        if (state.mode == 1)
            SetEyeToChara(character, nullptr, flags, 0);
        else if (state.mode != 2)
            return;
        RetireCurrent(eyesControllers, subject);
        return;
    }

    Vector3 target = ReadRecord<Vector3>(state.record, 0x14);
    const shen_name reference = ReadRecord<shen_name>(state.record, 0x0c);
    if (reference.name) {
        const auto index = ReadRecord<std::int16_t>(state.record, 0x10);
        if (index < 0 || static_cast<std::size_t>(index) >= eyeBoneIds.size())
            return;
        // mtwkGetTranslationFor: the solved rig bone, else the node translation; zero for a missing character.
        Character *const referenced = scenFindChara(author->task, reference);
        Vector3 position{};
        if (referenced)
            GetCharacterMotionWorldPosition(referenced, eyeBoneIds[static_cast<std::size_t>(index)], &position);
        target.x += position.x;
        target.y += position.y;
        target.z += position.z;
    } else if (const Matrix4x4 *const placement = Author_GetPlacementMatrix()) {
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(placement);
        Matrix4x4::TransformPoint(target, target);
        Matrix4x4::Pop(1);
    }
    SetEyeToChara(character, &target, flags, 2);
    if (flags > 0)
        state.mode = -1;
}

std::int32_t AddRotation(std::int32_t angle, std::int32_t step) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(angle) + static_cast<std::uint32_t>(step));
}

// @verified - p
void PartsCallback(void *payload) {
    auto &state = *static_cast<PartsController *>(payload);
    const shen_name subject = ReadRecord<shen_name>(state.record, 0x0c);
    const AuthorController *const author = Author_GetController();
    if (!author || !author->loaded) {
        RetireCurrent(partsControllers, subject);
        return;
    }
    Character *const character = scenFindChara(author->task, subject);
    if (!character)
        return;
    if (state.mode != 0) {
        if (state.mode != 1)
            return;
        RetireCurrent(partsControllers, subject);
        return;
    }

    const auto mode = ReadRecord<std::uint32_t>(state.record, 8);
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    if (model && !model->nodes.empty()) {
        for (std::size_t selector = 0; selector < 4; ++selector) {
            const auto index = ReadRecord<std::int8_t>(state.record, 0x10 + selector);
            if (index < 0 || static_cast<std::size_t>(index) >= partNodeIds.size())
                continue;
            MT7Node *const node = GetChildObjectByIndex(*model, &model->nodes.front(), partNodeIds[static_cast<std::size_t>(index)]);
            if (!node)
                continue;
            if (mode == 0) {
                node->position = ReadRecord<Vector3>(state.record, 0x20);
                const Vector3i rotation = ReadRecord<Vector3i>(state.record, 0x14);
                node->rotation = rotation;
            } else if (mode == 1 || mode == 2 || mode == 4) {
                state.position.x += state.positionStep.x;
                state.position.y += state.positionStep.y;
                state.position.z += state.positionStep.z;
                state.rotation.x = AddRotation(state.rotation.x, state.rotationStep.x);
                state.rotation.y = AddRotation(state.rotation.y, state.rotationStep.y);
                state.rotation.z = AddRotation(state.rotation.z, state.rotationStep.z);
                node->position.x += state.position.x;
                node->position.y += state.position.y;
                node->position.z += state.position.z;
                node->rotation.x = AddRotation(node->rotation.x, state.rotation.x);
                node->rotation.y = AddRotation(node->rotation.y, state.rotation.y);
                node->rotation.z = AddRotation(node->rotation.z, state.rotation.z);
            }
        }
    }
    if (mode != 2 && mode != 4)
        RetireCurrent(partsControllers, subject);
}

} // namespace

// @verified - p
bool ApplyAuthoredEyesRecord(std::span<const std::byte> record, HLib::Task *parent) {
    if (record.size() < 44 || ReadRecord<std::uint16_t>(record.data(), 4) != 4)
        return false;
    const shen_name subject = ReadRecord<shen_name>(record.data(), 8);
    if (!scenFindChara(parent, subject))
        return false;
    SubcontrollerSlot *const slot = FindSlot(eyesControllers, subject);
    if (!slot)
        return false;
    const bool receive = ReadRecord<shen_name>(record.data(), 0x0c).name == SHEN_NAME(RECV);
    EyesController *state = slot->task ? static_cast<EyesController *>(HLib::GetTaskParameter(slot->task)) : nullptr;
    if (!state && !receive) {
        state = static_cast<EyesController *>(CreateController(*slot, subject, parent, EyesCallback, SHEN_NAME(SCEY), sizeof(EyesController)));
    }
    if (!state)
        return false;
    state->mode = receive ? 1 : 0;
    if (!receive)
        state->record = record.data();
    return true;
}

// @verified - p
bool ApplyAuthoredPartsRecord(std::span<const std::byte> record, HLib::Task *parent) {
    if (record.size() < 44 || ReadRecord<std::uint16_t>(record.data(), 4) != 15)
        return false;
    const shen_name subject = ReadRecord<shen_name>(record.data(), 0x0c);
    if (!scenFindChara(parent, subject))
        return false;
    SubcontrollerSlot *const slot = FindSlot(partsControllers, subject);
    if (!slot)
        return false;
    const auto mode = ReadRecord<std::uint32_t>(record.data(), 8);
    PartsController *state = slot->task ? static_cast<PartsController *>(HLib::GetTaskParameter(slot->task)) : nullptr;
    if (!state && mode != 3) {
        state = static_cast<PartsController *>(CreateController(*slot, subject, parent, PartsCallback, SHEN_NAME(SCPT), sizeof(PartsController)));
    }
    if (!state)
        return false;
    state->mode = mode == 3 ? 1 : 0;
    if (mode == 3)
        return true;
    state->record = record.data();
    if (mode == 4) {
        state->positionStep = ReadRecord<Vector3>(record.data(), 0x20);
        state->rotationStep = ReadRecord<Vector3i>(record.data(), 0x14);
    } else {
        state->position = ReadRecord<Vector3>(record.data(), 0x20);
        state->rotation = ReadRecord<Vector3i>(record.data(), 0x14);
        state->positionStep = {};
        state->rotationStep = {};
    }
    return true;
}

void FreeAuthoredSubcontrollers() {
    for (SubcontrollerSlot &slot : eyesControllers) {
        ReleaseTask(slot.task);
        slot = {};
    }
    for (SubcontrollerSlot &slot : partsControllers) {
        ReleaseTask(slot.task);
        slot = {};
    }
}

} // namespace shenmue::s2
