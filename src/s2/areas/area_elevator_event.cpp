#include "s2/areas/area_elevator_event.hpp"
#include "s2/areas/area_elevator_event_data.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/elevator.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
namespace elevator_event {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name ren{SHEN_NAME(REN_)};
constexpr shen_name cabin{0x64766c65U};
constexpr shen_name panel{0x70766c65U};
constexpr shen_name cursor{0x79766c65U};
constexpr std::uint32_t localXYZ = 0x38000000U;
constexpr std::uint32_t recursiveXYZ = 0x78000000U;
// Each overlay has its own 0x118-byte owner and constant tables.
ElevatorEventState elvtState{};
ElevatorEventState elv2State{};

template <bool Alternate> ElevatorEventState &State() {
    if constexpr (Alternate)
        return elv2State;
    else
        return elvtState;
}
template <bool Alternate> const char *Area() {
    return Alternate ? "ELV2" : "ELVT";
}
template <bool Alternate> std::array<std::int32_t, 58> &PressedHand() {
    if constexpr (Alternate)
        return elv2pressedHand;
    else
        return elvtpressedHand;
}
template <bool Alternate> std::array<std::int32_t, 58> &RestHand() {
    if constexpr (Alternate)
        return elv2restHand;
    else
        return elvtrestHand;
}
template <bool Alternate> const Vector3i &UpRotation() {
    if constexpr (Alternate)
        return elv2upRotation;
    else
        return elvtupRotation;
}
template <bool Alternate> const Vector3i &FloorRotation() {
    if constexpr (Alternate)
        return elv2floorRotation;
    else
        return elvtfloorRotation;
}

template <bool Alternate> void InitCallback(void *);
template <bool Alternate> void EventCallback(void *);
template <bool Alternate> void SceneChangeFirst(void *);
template <bool Alternate> void SceneChangeSecond(void *);

void SetPart(shen_name character, std::uint16_t part, bool shown) {
    Vector3 position = shown ? Vector3{} : Vector3{0.0f, 8000.0f, 8000.0f};
    EV_scenApplyPosOrRotToPartOf(character, part, &position, nullptr, EventCharacterPartTransformMode::Absolute);
}
void SetLocalPose(shen_name character, const Vector3 &position, const Vector3i &rotation) {
    EV_AT_SetPos(character, localXYZ, position);
    EV_AT_SetAngY(character, localXYZ, rotation);
}
void SetGravity(shen_name character, NpcCharacterControllerState enabled) {
    EV_SetCharaControllerFlags(character, 2, enabled);
    EV_SetCharaControllerFlags(character, 3, enabled);
}
void PlayVoiceCallback(void *data) {
    EV_PlayDialogue(static_cast<const char *>(data));
    HLib::DestroyCurrentTask();
}
void QueueVoice(const char *voice, std::int32_t delay) {
    if (!delay) {
        EV_PlayDialogue(voice);
        return;
    }
    auto *task = HLib::EnqueueTaskWithParameter(PlayVoiceCallback, 4, 0x0b, 32, AREA_EVENT_TASK);
    std::strcpy(static_cast<char *>(HLib::GetTaskParameter(task)), voice);
    HLib::SetupTimedTask(task, delay);
}
template <bool Alternate> void PoseHand(bool pressed, std::int16_t ticks) {
    auto &command = pressed ? PressedHand<Alternate>() : RestHand<Alternate>();
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(cursor), 0, reinterpret_cast<std::byte *>(command.data()), ticks);
}
template <bool Alternate> bool RenPresent() {
    return (State<Alternate>().flags & 1) && DoesScenarioCharacterExist(ren);
}

template <bool Alternate> void Cleanup() {
    for (const char *bank : {"0354", "0355", "0304", "0298"})
        EV_FreeAuthorPak(0, bank);
    scenFreeEvent(Area<Alternate>());
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(Area<Alternate>()), 0);
    SetPlayerControlEnabled(1);
    LoadCutsceneAndSoundProgramForArea();
    EV_NPC_NewLoadSw(1);
    EV_EventSetUpHelpIcon_0(0, 0);
    FinishLoading();
}
template <bool Alternate> void ReleaseActors() {
    auto &state = State<Alternate>();
    SetGravity(ryo, NpcCharacterControllerState::Enabled);
    AT_SetLocalBaseChara(ryo, EVENT_CHARACTER_INVALID);
    if (RenPresent<Alternate>()) {
        AT_SetLocalBaseChara(ren, EVENT_CHARACTER_INVALID);
        SetGravity(ren, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(ren, true);
        EV_npcLockUnlock(ren);
        EV_npcFree(ren);
    }
    for (auto &button : state.buttons) {
        AT_SetLocalBaseChara(button.character, EVENT_CHARACTER_INVALID);
        SetScenarioCharacterKillMode(button.character, 1);
    }
    AT_SetLocalBaseChara(panel, EVENT_CHARACTER_INVALID);
    AT_SetLocalBaseChara(cursor, EVENT_CHARACTER_INVALID);
    SetHighModelXYZ(ctrlGetCharaByName(cursor), 0, 0, 0);
    hndlSetFlags(ctrlGetCharaByName(cursor));
}
template <bool Alternate> void UpdateCurrentDigits() {
    auto &state = State<Alternate>();
    const auto floor = GetCurrentElevatorFloor(state.elevator);
    const auto magnitude = floor < 0 ? -floor : floor;
    auto tens = magnitude / 10;
    auto ones = magnitude % 10;
    if (floor < 1) {
        tens = 5;
        ones = ones == 10 ? 0 : ones + 1;
    }
    if (!ones)
        ones = 10;
    SetPart(panel, 251, false);
    bool special = false;
    for (const auto &row : state.buttons)
        if (row.floor == floor && (row.flags & 4))
            special = true;
    SetPart(panel, 249, special);
    SetPart(panel, 250, special);
    for (std::uint16_t part = 153; part <= 162; ++part)
        SetPart(panel, part, !special && part == 152 + ones);
    for (std::uint16_t part = 152, index = 0; part <= 207; part += 11, ++index)
        SetPart(panel, part, !special && index == tens);
}
template <bool Alternate> void UpdateButtonDigits() {
    for (const auto &row : State<Alternate>().buttons) {
        SetPluralControlFor(ctrlGetCharaByName(row.character), 0);
        if (!(row.flags & 1))
            SetPart(row.character, 234, false);
        SetPart(row.character, 231, (row.flags & 8) != 0);
        SetPart(row.character, 232, (row.flags & 8) == 0);
        SetPart(row.character, 233, false);
        auto magnitude = row.floor < 0 ? -row.floor : row.floor;
        auto ones = magnitude % 10;
        auto tens = magnitude / 10;
        bool single = false;
        if (row.floor == -100) {
            ones = 10;
            single = true;
        } else if (row.floor == -101) {
            ones = 11;
            single = true;
        } else if (row.flags & 4) {
            ones = 0;
            single = true;
        } else if (row.floor >= 1)
            single = tens == 0;
        else {
            tens = 5;
            ones = ones == 10 ? 0 : ones + 1;
        }
        for (std::uint16_t part = 152; part <= 162; ++part)
            SetPart(row.character, part, single && part == 152 + ones);
        for (std::uint16_t part = 242; part <= 251; ++part)
            SetPart(row.character, part, !single && part == 242 + ones);
        for (std::uint16_t part = 163, index = 1; part <= 207; part += 11, ++index)
            SetPart(row.character, part, !single && index == tens);
    }
}
Vector3 DirectionPosition(std::int32_t direction) {
    if (direction == 1)
        return {std::bit_cast<float>(0x3f847ae1U), std::bit_cast<float>(0x3f90e560U), 0.70999998f};
    return {std::bit_cast<float>(0x3f845a1dU), std::bit_cast<float>(0x3f910625U), 0.71899998f};
}
Vector3i DirectionRotation(std::int32_t direction) {
    return {direction == 1 ? 4551 : 2002, 3640, 0};
}
template <bool Alternate> void SetupDirectionCursor() {
    auto &state = State<Alternate>();
    state.direction = state.sourceFloor == state.upperFloor ? 2 : 1;
    EnableScenarioCharacterDisplay(cursor);
    AT_SetLocalBaseChara(cursor, cabin);
    SetLocalPose(cursor, DirectionPosition(state.direction), DirectionRotation(state.direction));
    PoseHand<Alternate>(false, 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(cursor), 0, 21, &UpRotation<Alternate>());
    state.timeout = 1800;
}
template <bool Alternate> void BlendDirectionCursor() {
    Vector3 position{};
    Vector3i rotation{};
    AT_SetLocalBaseChara(cursor, cabin);
    EV_GetCharaPosition(cursor, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(cursor, &rotation, EventTransformFlags::Local);
    const auto target = DirectionPosition(State<Alternate>().direction);
    const auto targetRotation = DirectionRotation(State<Alternate>().direction);
    const std::int32_t ticks = State<Alternate>().ticks;
    position.x += (target.x - position.x) / ticks;
    position.y += (target.y - position.y) / ticks;
    position.z += (target.z - position.z) / ticks;
    rotation.x += (targetRotation.x - rotation.x) / ticks;
    rotation.y += (targetRotation.y - rotation.y) / ticks;
    rotation.z += (targetRotation.z - rotation.z) / ticks;
    SetLocalPose(cursor, position, rotation);
}
template <bool Alternate> std::int32_t ReadDirection() {
    ElevatorEventState &state = State<Alternate>();
    CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
    if (state.timeout-- < 1)
        return 3;
    if (state.ticks >= 1) {
        GetBattleDataConfig(0, SHEN_NAME(BTEV));
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        BlendDirectionCursor<Alternate>();
        --state.ticks;
        return 0;
    }
    GetBattleDataConfig(0, SHEN_NAME(BTEV));
    if (GetBattleDataConfig(2, SHEN_NAME(BTEV)) && (((state.flags & 2) && state.direction == 1) || ((state.flags & 4) && state.direction == 2)))
        return state.direction;
    GetBattleDataConfig(0, SHEN_NAME(BTCC));
    if (GetBattleDataConfig(2, SHEN_NAME(BTCC)))
        return 3;
    if (GetCurrentControlInput(1) & 0x1000) {
        state.direction = 1;
        state.ticks = 10;
    }
    if (GetCurrentControlInput(1) & 0x2000) {
        state.direction = 2;
        state.ticks = 10;
    }
    return 0;
}
template <bool Alternate> void SetupFloorCursor() {
    ElevatorEventState &state = State<Alternate>();
    for (std::int32_t index = 0; index < 10; ++index)
        if (state.buttons[index].floor == state.sourceFloor)
            state.selection = index;
    EnableScenarioCharacterDisplay(cursor);
    AT_SetLocalBaseChara(cursor, state.buttons[state.selection].character);
    SetLocalPose(cursor, {0.129f, 0.118f, 0.565f}, {0, 5461, 0});
    PoseHand<Alternate>(false, 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(cursor), 0, 21, &FloorRotation<Alternate>());
    state.timeout = 1800;
}
template <bool Alternate> void BlendFloorCursor() {
    Vector3 position{};
    ElevatorEventState &state = State<Alternate>();
    AT_SetLocalBaseChara(cursor, state.buttons[state.selection].character);
    EV_GetCharaPosition(cursor, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x += (0.129f - position.x) / state.ticks;
    position.y += (0.118f - position.y) / state.ticks;
    position.z += (0.565f - position.z) / state.ticks;
    EV_AT_SetPos(cursor, localXYZ, position);
}
template <bool Alternate> bool ReadFloor() {
    ElevatorEventState &state = State<Alternate>();
    CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
    if (state.timeout-- < 1) {
        state.destination = -1000;
        return true;
    }
    if (state.ticks >= 1) {
        GetBattleDataConfig(0, SHEN_NAME(BTEV));
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        BlendFloorCursor<Alternate>();
        --state.ticks;
        return false;
    }
    GetBattleDataConfig(0, SHEN_NAME(BTEV));
    if (GetBattleDataConfig(2, SHEN_NAME(BTEV))) {
        state.destination = state.buttons[state.selection].floor;
        return true;
    }
    GetBattleDataConfig(0, SHEN_NAME(BTCC));
    if (GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
        state.destination = -1000;
        return true;
    }
    if ((GetCurrentControlInput(2) & 0x1000) && (state.selection + 1) % 5) {
        ++state.selection;
        state.ticks = 10;
    }
    if ((GetCurrentControlInput(2) & 0x2000) && state.selection % 5) {
        --state.selection;
        state.ticks = 10;
    }
    if ((GetCurrentControlInput(2) & 0x8000) && state.selection < 5) {
        state.selection += 5;
        state.ticks = 10;
    }
    if ((GetCurrentControlInput(2) & 0x4000) && state.selection >= 5) {
        state.selection -= 5;
        state.ticks = 10;
    }
    return false;
}
template <bool Alternate> void MoveCursor(float dz) {
    Vector3 position{};
    EV_GetCharaPosition(cursor, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.z += dz;
    EV_AT_SetPos(cursor, localXYZ, position);
}
template <bool Alternate> void SetRanges(float ryoStart, float ryoEnd, float renStart, float renEnd) {
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    SetMotionLayerFrameRange(scenFindMotionwork(ryo), ryoStart, ryoStart, ryoEnd, MotionRegion::LowerBody);
    if (RenPresent<Alternate>()) {
        SetMotionRegionFlag(scenFindMotionwork(ren), 71, 0, MotionRegion::LowerBody);
        SetMotionLayerFrameRange(scenFindMotionwork(ren), renStart, renStart, renEnd, MotionRegion::LowerBody);
    }
}
template <bool Alternate> void EventCallback(void *) {
    ElevatorEventState &state = State<Alternate>();
    switch (state.phase) {
    case 0: {
        const auto result = ReadDirection<Alternate>();
        if (result == 1 || result == 2) {
            PoseHand<Alternate>(true, 10);
            state.ticks = 10;
            state.phase = 1;
            nullsub();
        } else if (result == 3) {
            SetLocalPose(ryo, {0.5f, 0, 1}, {});
            SetClothFlag(ctrlGetCharaByName(ryo), 16, 1);
            state.flags |= 0x40;
            state.phase = 16;
        }
        return;
    }
    case 1:
        MoveCursor<Alternate>(-0.0020000001f);
        if (--state.ticks < 1) {
            Loop3DSoundEffect(0x4b5);
            if (!(state.flags & 0x20))
                SetElevatorIndicatorFlag(state.elevator, state.direction == 2, 1);
            state.ticks = 10;
            state.phase = 2;
        }
        return;
    case 2:
        if (--state.ticks < 1) {
            if (state.flags & 0x20) {
                DisableScenarioCharacterDisplay(cursor);
                state.phase = 17;
            } else if constexpr (Alternate) {
                spmFadeOut();
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                HLib::SetupTaskDelay(30);
                state.phase = 3;
            } else {
                DisableScenarioCharacterDisplay(cursor);
                EVS_LoopCamera(1010, cabin, cabin);
                if (state.flags & 0x10)
                    Loop3DSoundEffect(0xb40);
                state.phase = 3;
            }
        }
        return;
    case 3:
        SetElevatorIndicatorFlag(state.elevator, 1, 0);
        SetElevatorIndicatorFlag(state.elevator, 0, 0);
        if constexpr (Alternate) {
            DisableScenarioCharacterDisplay(cursor);
            if (EV_GetShenmueFlag(0x164)) {
                LoadCutsceneAndSoundProgramForArea();
                LoadAreaFunc_Wrapper_0("BTEL.EV3", 0);
                ExecuteCurrAreaMainFunc("BTEL.EV3", 0);
                EV_stepSetFlags(0, 0x164);
                ReleaseActors<Alternate>();
                scenFreeEvent("ELV2");
                EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ELV2"), 0);
                camCtrlSetState(CameraState::Init);
                EV_EventSetUpHelpIcon_0(0, 0);
                FinishLoading();
                HLib::DestroyCurrentTask();
            }
        } else {
            if (!RequestElevatorFloor(state.elevator, static_cast<std::int16_t>(state.sourceFloor), state.direction))
                return;
            mtwkRequestMotion(scenFindMotionwork(ryo), 0x2683, MotionRegion::LowerBody);
            SetMotionLayerFrameRange(scenFindMotionwork(ryo), 0, 0, 160, MotionRegion::LowerBody);
            if (RenPresent<Alternate>()) {
                mtwkRequestMotion(scenFindMotionwork(ren), 0x2681, MotionRegion::LowerBody);
                SetMotionLayerFrameRange(scenFindMotionwork(ren), 45, 45, 200, MotionRegion::LowerBody);
            }
            UpdateCurrentDigits<Alternate>();
            state.phase = 4;
            return;
        }
        [[fallthrough]];
    case 4:
        if (GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody) > 80) {
            EVS_LoopCamera(1020, cabin, cabin);
            state.phase = 5;
        }
        return;
    case 5: {
        const float frame = GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody);
        if (Alternate ? !(frame <= 155.0f) : frame > 155.0f) {
            SetRanges<Alternate>(300, 301, 200, 200);
            SetupFloorCursor<Alternate>();
            EVS_LoopCamera(1031, cursor, cursor);
            state.phase = 6;
            FinishLoading();
        }
        return;
    }
    case 6:
        if (ReadFloor<Alternate>()) {
            if (state.destination == -1000) {
                RequestElevatorFloor(state.elevator, static_cast<std::int16_t>(state.sourceFloor), 0);
                if constexpr (Alternate)
                    DisableScenarioCharacterDisplay(cursor);
                state.phase = 12;
            } else {
                PoseHand<Alternate>(true, 10);
                state.ticks = 10;
                state.phase = 7;
            }
            nullsub();
        }
        return;
    case 7:
        MoveCursor<Alternate>(-0.0020000001f);
        if (--state.ticks < 1) {
            PoseHand<Alternate>(false, 10);
            const ElevatorEventButton &button = state.buttons[state.selection];
            if (button.flags & 2) {
                Loop3DSoundEffect(0x4b5);
                SetPart(button.character, 231, false);
                SetPart(button.character, 232, false);
                SetPart(button.character, 233, true);
            } else
                Loop3DSoundEffect(0x9b4);
            state.ticks = 10;
            state.phase = 8;
        }
        return;
    case 8:
        MoveCursor<Alternate>(0.0020000001f);
        if (--state.ticks < 1) {
            if (state.buttons[state.selection].flags & 2) {
                spmFadeOutBank(2, 60);
                state.phase = 9;
            } else {
                state.phase = 6;
                FinishLoading();
            }
        }
        return;
    case 9:
        if (BeginElevatorTravel(0, state.elevator, ryo, state.sourceFloor, state.destination)) {
            DisableScenarioCharacterDisplay(cursor);
            const ElevatorEventButton &button = state.buttons[state.selection];
            if (!button.area.name || button.area.name == g_currentMap.name) {
                SetQronFloor(state.destination);
                Vector3 position{};
                EV_GetCharaPosition(cabin, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
                position.y = GetQronTemporaryFloorHeight();
                EV_AT_SetPos(cabin, recursiveXYZ, position);
                spmFadeInBank(2, 60);
            } else {
                HLib::EnqueueTaskWithoutParameter(SceneChangeFirst<Alternate>, 4, 0x0b, ELEVATOR_EVENT_TASK.name);
                state.flags |= 8;
            }
            EVS_LoopCamera(1041, cabin, cabin);
            state.phase = 10;
        }
        return;
    case 10:
        UpdateCurrentDigits<Alternate>();
        if constexpr (!Alternate)
            if (g_inputBuffer[0].pressedPriority & 0x200)
                SetCurrentElevatorFloor(state.elevator, static_cast<std::int16_t>(state.destination));
        if (ArriveAndOpenElevator(0, state.elevator))
            state.phase = 12 - ((state.flags & 8) != 0);
        return;
    case 12:
        if constexpr (!Alternate)
            if (state.destination == -1000)
                DisableScenarioCharacterDisplay(cursor);
        SetRanges<Alternate>(520, 620, 540, 630);
        EVS_LoopCamera(1055, cabin, cabin);
        state.phase = 13;
        return;
    case 13:
        if (GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody) > 560) {
            CloseAndResumeElevator(state.elevator);
            state.phase = 14;
        }
        return;
    case 14:
        if (GetMotionFrame(scenFindMotionwork(ryo), MotionRegion::LowerBody) > 600)
            state.phase = 15;
        return;
    case 15:
        if (CloseAndResumeElevator(state.elevator)) {
            Vector3i rotation{};
            GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::MotionYaw);
            EV_AT_SetAngY(ryo, localXYZ, rotation);
            mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
            state.flags &= ~0x40U;
            state.phase = 16;
        }
        return;
    case 16:
        ReleaseActors<Alternate>();
        camCtrlSetFovTransitionTo((state.flags & 0x40) != 0);
        camCtrlSetState(CameraState::Init);
        Cleanup<Alternate>();
        HLib::DestroyCurrentTask();
        return;
    case 17: {
        EVS_LoopCamera(2000, cabin, cabin);
        std::int32_t delay;
        if (EV_GetShenmueFlag(0x30a)) {
            EV_stepSetFlags(0, 0x30a);
            QueueVoice("0304A001", 0);
            QueueVoice("0304A039", 85);
            delay = 175;
        } else if (EV_GetShenmueFlag(0x3f6)) {
            QueueVoice("0304A040", 0);
            QueueVoice("0304A041", 75);
            delay = 165;
        } else if (EV_GetShenmueFlag(0x421)) {
            QueueVoice("0304A001", 0);
            QueueVoice("0304A028", 85);
            delay = 175;
        } else {
            QueueVoice("0304A044", 0);
            delay = 155;
        }
        state.flags |= 0x40;
        HLib::SetupTaskDelay(delay);
        state.phase = 16;
        return;
    }
    case 18:
        QueueVoice("0354A001", 0);
        QueueVoice("0354A002", 75);
        state.flags |= 0x40;
        HLib::SetupTaskDelay(150);
        state.phase = 16;
        return;
    case 19: {
        QueueVoice("0355A001", 0);
        const bool first = shenGetRandomInt32(2) == 1;
        QueueVoice(first ? "0355A002" : "0355A003", 80);
        state.flags |= 0x40;
        HLib::SetupTaskDelay(first ? 145 : 135);
        state.phase = 16;
        return;
    }
    case 20: {
        const auto choice = shenGetRandomInt32(3) - 1;
        QueueVoice(choice == 0 ? "0298A035" : choice == 1 ? "0298A036" : "0298A037", 0);
        state.flags |= 0x40;
        HLib::SetupTaskDelay(choice == 0 || choice == 1 ? 70 : 90);
        state.phase = 16;
        return;
    }
    default:
        return;
    }
}
template <bool Alternate> void InitCallback(void *) {
    auto &state = State<Alternate>();
    if ((state.flags & 1) && !EV_ForceLoadNpc(ren))
        return;
    SetGravity(ryo, NpcCharacterControllerState::Disabled);
    AT_SetLocalBaseChara(ryo, cabin);
    SetLocalPose(ryo, {1, 0, 1}, {});
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0x2684, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(ryo), 1, MotionRegion::LowerBody);
    SetClothFlag(ctrlGetCharaByName(ryo), 16, 1);
    if (RenPresent<Alternate>()) {
        EV_SetCharacterAlphaEnabled(ren, false);
        AT_SetLocalBaseChara(ren, cabin);
        SetGravity(ren, NpcCharacterControllerState::Disabled);
        SetLocalPose(ren, {-0.1f, 0, 1.3f}, {});
        SetMotionRegionFlag(scenFindMotionwork(ren), 71, 0, MotionRegion::LowerBody);
        mtwkRequestMotion(scenFindMotionwork(ren), 0x2682, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(ren), 1, MotionRegion::LowerBody);
        SetClothFlag(ctrlGetCharaByName(ren), 16, 1);
    }
    SetLowQualityHands(ctrlGetCharaByName(cursor));
    SetHighModelXYZ(ctrlGetCharaByName(cursor), 1, 0, 0);
    Vector4 cursorLight{1, 0.3f, 0.3f, 0.3f};
    SetScenarioReflectorBoardValues(cursor, &cursorLight);
    AT_SetLocalBaseChara(panel, cabin);
    SetLocalPose(panel, {1.1f, 0.9f, -0.12f}, {0, 32768, 0});
    EnableScenarioCharacterDisplay(panel);
    Vector4 panelLight{0.1f, 0.1f, 0.1f, 0.1f};
    SetScenarioReflectorBoardValues(panel, &panelLight);
    for (std::int32_t index = 0; index < 10; ++index) {
        auto &row = state.buttons[index];
        row.character = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("ELVTTBBG"), {});
        AT_SetLocalBaseChara(row.character, cabin);
        SetLocalPose(row.character, {1.14f - (index / 5) * 0.090000004f, 1.23f + (index % 5) * 0.090000004f, -0.13f}, {0, 32768, 0});
        EnableScenarioCharacterDisplay(row.character);
    }
    EVS_LoopCamera(1000, cabin, cabin);
    UpdateCurrentDigits<Alternate>();
    UpdateButtonDigits<Alternate>();
    SetupDirectionCursor<Alternate>();
    state.phase = 0;
    if (state.flags & 6) {
        FinishLoading();
        if (g_currentMap == AREA_QF39 && state.elevator.name == SHEN_NAME(R598)) {
            DisableScenarioCharacterDisplay(cursor);
            state.phase = 19;
        } else if (g_currentMap.name == SHEN_NAME(QKBT) && state.elevator.name == SHEN_NAME(R597)) {
            DisableScenarioCharacterDisplay(cursor);
            state.phase = 18;
        }
    } else {
        DisableScenarioCharacterDisplay(cursor);
        state.phase = 20;
    }
    HLib::SetCurrentTaskTo(EventCallback<Alternate>);
}
template <bool Alternate> const ElevatorEventFloorTable *SelectTable() {
    switch (g_currentMap.name) {
    case SHEN_NAME(QA00):
    case SHEN_NAME(QA11):
    case SHEN_NAME(QAE1):
    case SHEN_NAME(QAE6):
    case SHEN_NAME(QAW1):
    case SHEN_NAME(QAW6):
        if constexpr (Alternate)
            return elv2QA;
        else
            return elvtQA;
    case SHEN_NAME(QC00):
    case SHEN_NAME(QC01):
    case SHEN_NAME(QC06):
        if constexpr (Alternate)
            return elv2QC;
        else
            return elvtQC;
    case SHEN_NAME(QD00):
    case SHEN_NAME(QD01):
        if constexpr (Alternate)
            return elv2QD;
        else
            return elvtQD;
    case SHEN_NAME(QE00):
    case SHEN_NAME(QE01):
    case SHEN_NAME(QE06):
        if constexpr (Alternate)
            return elv2QE;
        else
            return elvtQE;
    case SHEN_NAME(QF00):
    case SHEN_NAME(QF01):
    case SHEN_NAME(QF02):
    case SHEN_NAME(QF06):
    case SHEN_NAME(QF39):
    case SHEN_NAME(QUG0):
    case SHEN_NAME(QKBT):
        if constexpr (Alternate)
            return elv2QF;
        else
            return elvtQF;
    case SHEN_NAME(QJBT):
        if constexpr (Alternate)
            return elv2QJ;
        else
            return elvtQJ;
    default:
        return nullptr;
    }
}
template <bool Alternate> void Main(shen_name elevator) {
    SetPlayerControlEnabled(0);
    nullsub();
    EV_EventSetUpHelpIcon_0(85, 0);
    EV_SetNewSceneIndex();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(Area<Alternate>()), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(Area<Alternate>()), 0);
    EV_LoadCutsceneFromCurrentScene("0298E000");
    for (const char *bank : {"0354", "0355", "0304", "0298"})
        EV_FreeAuthorPak(1, bank);
    Vector3 position{};
    std::uint32_t yaw{};
    const auto status = GetElevatorTransform(elevator, position, yaw) - 1;
    if (!status) {
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        Cleanup<Alternate>();
        return;
    }
    HLib::EnqueueTaskWithoutParameter(InitCallback<Alternate>, 4, 0x0b, ELEVATOR_EVENT_TASK.name);
    auto &state = State<Alternate>();
    state = {};
    state.position = position;
    state.rotation.y = std::bit_cast<std::int32_t>(yaw);
    state.elevator = elevator;
    state.sourceFloor = GetQronNextStream();
    if (status == 1)
        state.flags |= 0x20;
    if (EV_GetShenmueFlag(0x190))
        state.flags |= 1;
    for (auto *table = SelectTable<Alternate>(); table->elevator.name; ++table)
        if (table->elevator.name == elevator.name)
            state.floorTable = table;
    for (std::size_t index = 0; index < 10; ++index) {
        const auto &source = state.floorTable->destinations[index];
        auto &row = state.buttons[index];
        row.floor = source.floor;
        row.flags = source.flags;
        row.area = source.area;
        row.entry = source.entry;
        if (IsElevatorFloorValid(elevator, static_cast<std::int16_t>(row.floor)) && !(row.flags & 1) && row.floor != -100 && row.floor != -101) {
            row.flags |= 8;
            if (row.floor != state.sourceFloor)
                row.flags |= 2;
        }
    }
    state.upperFloor = -15;
    state.lowerFloor = 48;
    for (const auto &row : state.buttons)
        if (row.flags & 8) {
            if (row.floor > state.upperFloor)
                state.upperFloor = row.floor;
            if (row.floor < state.lowerFloor)
                state.lowerFloor = row.floor;
        }
    if (IsElevatorFloorValid(elevator, static_cast<std::int16_t>(state.sourceFloor))) {
        if (state.sourceFloor != state.upperFloor)
            state.flags |= 2;
        if (state.sourceFloor != state.lowerFloor)
            state.flags |= 4;
    }
    if (GetCurrentElevatorFloor(elevator) != state.sourceFloor)
        state.flags |= 0x10;
    EV_AT_SetPos(cabin, recursiveXYZ, state.position);
    EV_AT_SetAngY(cabin, recursiveXYZ, state.rotation);
    if (state.flags & 1)
        EV_CheckWarmStartNpcRoute(ren);
}
template <bool Alternate> void SceneChangeFirst(void *) {
    const auto &state = State<Alternate>();
    if (GetCurrentElevatorFloor(state.elevator) != state.destination)
        return;
    HLib::SetCurrentTaskTo(SceneChangeSecond<Alternate>);
    EVS_LoopCamera(1042, cabin, cabin);
    HLib::SetupTaskDelay(60);
}
template <bool Alternate> void SceneChangeSecond(void *) {
    const ElevatorEventState &state = State<Alternate>();
    const ElevatorEventButton &destination = state.buttons[state.selection];
    Vector3 position{0, 0, 1};
    ApplyScenarioTranslationAndRotation(&state.position, &state.rotation, &position);
    QueueQronElevatorSceneChange(destination.area, destination.entry, static_cast<std::int16_t>(state.destination), position, state.rotation.y + 32768);
    if (destination.area.name == SHEN_NAME(QKBT))
        for (const std::int32_t item : {34, 29, 33, 32, 30, 31})
            if (GetItemState(item))
                RemoveInventoryItem(item, 1);
    HLib::DestroyCurrentTask();
}
} // namespace elevator_event
void ELV2_init_func() {
    elevator_event::elv2State = {};
}
void ELV2_main_func(std::uintptr_t elevator) {
    elevator_event::Main<true>(shen_name{static_cast<std::uint32_t>(elevator)});
}
void ELVT_init_func() {
    elevator_event::elvtState = {};
}
void ELVT_main_func(std::uintptr_t elevator) {
    elevator_event::Main<false>(shen_name{static_cast<std::uint32_t>(elevator)});
}
const ScriptAreaDefinition areaELV2ScriptDefinition{.areaId = SHEN_NAME(ELV2), .field04 = 0, .init = ELV2_init_func, .main = ELV2_main_func};
const ScriptAreaDefinition areaELVTScriptDefinition{.areaId = SHEN_NAME(ELVT), .field04 = 0, .init = ELVT_init_func, .main = ELVT_main_func};
} // namespace shenmue::s2
