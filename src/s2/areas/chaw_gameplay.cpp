#include "s2/areas/area_chaw.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/motion_work.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2 {
Vector3 chawPreviousHand;
std::int32_t chawLectureErrors, chawSerifDelay;
namespace {
bool MatchesSign(const ChawEventWork *work, const std::int32_t *sign) {
    for (std::int32_t index = 0; index < 4; ++index)
        if (work->cupRows[index] != sign[index])
            return false;
    return true;
}
std::int32_t CurrentSecond() {
    DateTime date;
    GetCurrentDateTime(date);
    return date.second + 60 * (date.minute + 60 * date.hour);
}
} // namespace
Vector3 ChawGetHandTarget(std::int32_t cell) {
    const float x = cell < 12 ? CHAW_HAND_X[cell] : std::bit_cast<float>(static_cast<std::uint32_t>(CHAW_PLACEMENT_FLAGS[cell - 12]));
    const float z = cell < 12 ? CHAW_HAND_Z[cell] : CHAW_HAND_BLEND[cell - 12];
    return {x, 0.9f, z};
}
std::int32_t ChawGetHandYaw(std::int32_t cell) {
    return cell < 12 ? CHAW_HAND_YAW[cell] : CHAW_SUCCESS_SIGN[cell - 12];
}
void ChawUpdateHand(ChawEventWork *work) {
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(CHAW_HAND, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(CHAW_HAND, &rotation, EventTransformFlags::Local);
    if (work->handMovement) {
        position.x = (work->handTarget.x - chawPreviousHand.x) * CHAW_HAND_BLEND[work->ticks] + chawPreviousHand.x;
        position.z = (work->handTarget.z - chawPreviousHand.z) * CHAW_HAND_BLEND[work->ticks] + chawPreviousHand.z;
        if (work->ticks >= 12)
            work->handMovement = 0;
    } else {
        position.x = work->handTarget.x;
        position.z = work->handTarget.z;
    }
    position.y = 0.9f;
    EV_AT_SetPos(CHAW_HAND, EventTransformFlags::AllAxes, position);
    rotation.y = static_cast<std::uint16_t>(rotation.y);
    const std::int32_t difference = work->handYaw - rotation.y;
    if (static_cast<std::uint32_t>(static_cast<std::uint16_t>(difference)) - 18U > 0xffdbU)
        rotation.y = work->handYaw;
    else
        rotation.y +=
            static_cast<std::int32_t>(difference > 0 ? static_cast<double>(difference) * 0.25 : static_cast<double>(rotation.y - work->handYaw) * -0.25);
    rotation.x = static_cast<std::uint16_t>(
        static_cast<std::int32_t>((static_cast<double>(sin_fixed16(static_cast<std::int16_t>(work->ticks << 9))) + 270) * 8192 / 45));
    EV_AT_SetAngY(CHAW_HAND, EventTransformFlags::AllAxes, rotation);
}
void ChawSelectCup(ChawEventWork *work, std::int32_t step) {
    const std::int32_t cup = work->selectedCup + step;
    if (static_cast<std::uint32_t>(cup) <= 3) {
        work->selectedCup = cup;
        work->carriedCup = cup;
        work->handMovement = 1;
        work->row = work->cupRows[cup];
        work->handTarget = ChawGetHandTarget(work->row * 4 + cup);
        work->ticks = 0;
        work->handYaw = ChawGetHandYaw(work->row * 4 + cup);
        EV_GetCharaPosition(CHAW_HAND, CHARACTER_TRANSLATION_ROOT, &chawPreviousHand, EventTransformFlags::Local);
    }
}
void ChawMoveCup(ChawEventWork *work, std::int32_t step) {
    const std::uint32_t row = static_cast<std::uint32_t>(work->row + step);
    if (row <= 2) {
        work->row = static_cast<std::int32_t>(row);
        work->handMovement = 2;
        work->cupMoved = 1;
        const std::int32_t cell = work->selectedCup + 4 * work->row;
        work->handTarget = ChawGetHandTarget(cell);
        work->ticks = 0;
        work->handYaw = ChawGetHandYaw(cell);
        EV_GetCharaPosition(CHAW_HAND, CHARACTER_TRANSLATION_ROOT, &chawPreviousHand, EventTransformFlags::Local);
    }
}
void ChawAttachCup(ChawEventWork *work, std::int32_t cup) {
    Vector3 position;
    EV_GetCharaPosition(CHAW_HAND, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x += (static_cast<float>(cup) * 0.1f - 0.15000001f) - CHAW_HAND_X[work->selectedCup];
    position.y += -0.14999998f;
    position.z += 0.30000001f - CHAW_HAND_Z[work->selectedCup];
    EV_AT_SetPos(work->cups[cup], EventTransformFlags::AllAxes, position);
}
void ChawPlaceCup(ChawEventWork *work, std::int32_t cup) {
    const std::int32_t row = work->cupRows[cup];
    const float x = row == 3 ? std::bit_cast<float>(AREA_ENTITY_POSITION_IMAGE.terminator.name) : AREA_ENTITY_POSITION_IMAGE.positions[row].x;
    const float z = row == 3 ? std::bit_cast<float>(AREA_ENTITY_POSITION_IMAGE.adjacentFlagIds[1]) : AREA_ENTITY_POSITION_IMAGE.positions[row].z;
    const Vector3 position{static_cast<float>(cup) * 0.1f + x, 0.75f, z};
    EV_AT_SetPos(work->cups[cup], EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(work->cups[cup], EventTransformFlags::AllAxes, Vector3i{0, 40960, 0});
}
void ChawSetupChoice(std::int16_t seconds) {
    std::int16_t choices[4]{-1, 149, 152, 34};
    SetupControlSpeakAndUI(choices, 30 * seconds, 152, 2);
}
void ChawStoreSign(ChawEventWork *work) {
    if (work->entry == 21) {
        for (std::int16_t index = 16; index <= 31; ++index)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, index, 85);
    } else {
        const std::uint8_t sign = static_cast<std::uint8_t>(work->cupRows[0] | (4 * (work->cupRows[1] | (4 * (work->cupRows[2] | (4 * work->cupRows[3]))))));
        if (work->entry >= 16)
            work->entry -= 16;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, static_cast<std::int16_t>(work->entry + 16), sign);
    }
}
std::int32_t ChawGetProgression(ChawEventWork *work) {
    if (!MatchesSign(work, CHAW_SUCCESS_SIGN)) {
        work->wrongSign = 1;
        return 1;
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, CHAW_PLACEMENT_FLAGS[work->entry], 1);
    std::int16_t recorded = 0;
    switch (work->entry) {
    case 0:
        if (EV_GetShenmueFlag(0xe6))
            return 6;
        recorded = 2211;
        break;
    case 1:
        if (EV_GetShenmueFlag(0xee))
            return 7;
        recorded = 2212;
        break;
    case 2: {
        if (!EV_GetShenmueFlag(0x24b))
            return 1;
        DateTime date;
        GetCurrentDateTime(date);
        return (date.minute + 100 * date.hour >= 1930) + 2;
    }
    case 3:
        if (EV_GetShenmueFlag(0x302))
            return 6;
        recorded = 2213;
        break;
    case 7:
        if (EV_GetShenmueFlag(0xee))
            return 7;
        recorded = 2217;
        break;
    case 9:
        if (EV_GetShenmueFlag(0x2b3))
            return 6;
        recorded = 2210;
        break;
    case 10:
        if (EV_GetShenmueFlag(0x4f))
            return 8;
        recorded = 2216;
        break;
    case 11:
        if (EV_GetShenmueFlag(0xe6))
            return 6;
        recorded = 2214;
        break;
    case 12:
        if (EV_GetShenmueFlag(0xee))
            return 7;
        recorded = 2215;
        break;
    case 15:
        if (EV_GetShenmueFlag(0x43d))
            return 9;
        recorded = 2218;
        break;
    case 16:
        return EV_GetShenmueFlag(0x2c1) ? 10 : 1;
    default:
        return 1;
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, recorded, 1);
    return 1;
}
void ChawQueueSerif(const char *first, const char *second, const char *third, const char *nextFirst, const char *nextSecond, const char *nextThird) {
    if (!nextFirst && !nextSecond && nextThird) {
        nextFirst = nextThird;
        nextSecond = nextThird;
        nextThird = nullptr;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ChawSerifCallback, 3, 11, sizeof(ChawSerifWork), CHAW_VOICE_TASK);
    ChawSerifWork *const work = static_cast<ChawSerifWork *>(HLib::GetTaskParameter(task));
    chawSerifDelay = 7;
    work->choices[0] = first;
    work->choices[1] = second;
    work->choices[2] = third;
    work->choices[3] = nextFirst;
    work->choices[4] = nextSecond;
    work->choices[5] = nullptr;
    work->choices[6] = nextThird;
    work->choices[7] = nullptr;
    work->choices[8] = nullptr;
    work->phase = 0;
}
void ChawSerifCallback(void *data) {
    ChawSerifWork *const work = static_cast<ChawSerifWork *>(data);
    if (chawSerifDelay > 1) {
        --chawSerifDelay;
        return;
    }
    chawSerifDelay = 0;
    if (!cvsHasValidConverseData(nullptr)) {
        const char *const *const choices = work->choices + 3 * work->phase;
        const std::int32_t count = choices[2] ? 3 : choices[1] ? 2 : choices[0] ? 1 : 0;
        if (count)
            EV_PlayDialogue(choices[shenGetRandomInt32(count)]);
        if (++work->phase > 2)
            HLib::DestroyCurrentTask();
    }
}
bool ChawReminderDue(ChawEventWork *work) {
    const std::uint32_t second = static_cast<std::uint32_t>(CurrentSecond());
    if (static_cast<std::uint32_t>(work->lastReminderSecond + 100) > second)
        return false;
    work->lastReminderSecond = static_cast<std::int32_t>(second);
    return true;
}
void ChawAdvanceLecture(ChawEventWork *work, std::int32_t phase) {
    const char *line = nullptr;
    switch (work->lecturePhase) {
    case 1:
        line = "0038B021";
        break;
    case 2:
        line = "0038B027";
        break;
    case 4:
        line = "0038B036";
        break;
    case 6:
        line = "0038B039";
        break;
    case 8:
        line = "0038B044";
        break;
    default:
        break;
    }
    if (line)
        EV_PlayDialogue(line);
    work->lecturePhase = phase;
    work->reminders = 0;
    work->lastReminderSecond = CurrentSecond();
}
bool ChawCheckLectureSelection(ChawEventWork *work, std::int32_t phase) {
    const bool first = work->lecturePhase == 2;
    const std::int32_t selected = first ? 0 : 2;
    const char *last = first ? "0038B026" : "0038B038";
    const char *middle = first ? "0038B025" : "0038B037";
    const char *extra = "0038B035";
    bool wrong = false;
    if (work->cupMoved) {
        work->lastReminderSecond = CurrentSecond();
        work->error = !MatchesSign(work, CHAW_LECTURE_SIGNS[first ? 0 : 1]);
        if (work->error) {
            if (first)
                wrong = true;
            else if (!chawLectureErrors) {
                wrong = true;
                chawLectureErrors = 1;
            } else if (chawLectureErrors == 1) {
                chawLectureErrors = 2;
                wrong = true;
            } else if (chawLectureErrors == 2)
                chawLectureErrors = 1;
        } else
            chawLectureErrors = 0;
        if (wrong) {
            if (++work->reminders == 3) {
                work->reminders = 0;
                work->cupMoved = 0;
            } else {
                last = nullptr;
                middle = nullptr;
                work->cupMoved = 0;
            }
            work->error = 1;
            ChawQueueSerif("0038B030", "0038B031", "0038B032", middle, last, extra);
            return false;
        }
    } else {
        if (work->selectedCup == selected && MatchesSign(work, CHAW_LECTURE_SIGNS[first ? 0 : 1])) {
            ChawAdvanceLecture(work, phase);
            work->enabled = 1;
            chawLectureErrors = 0;
            return false;
        }
        if (ChawReminderDue(work) && !work->error) {
            ChawQueueSerif("0038B022", "0038B023", "0038B024", middle, last, nullptr);
            return false;
        }
    }
    return true;
}
bool ChawCheckLectureMovement(ChawEventWork *work, std::int32_t phase) {
    const bool first = work->lecturePhase == 4;
    const std::int32_t selected = first ? 0 : 2;
    work->enabled = work->selectedCup != selected;
    const char *firstLine = "0038B030";
    const char *middle = nullptr;
    const char *last = nullptr;
    if (work->cupMoved) {
        work->lastReminderSecond = CurrentSecond();
        if (MatchesSign(work, CHAW_LECTURE_SIGNS[first ? 1 : 2])) {
            ChawAdvanceLecture(work, phase);
            work->enabled = 1;
            return false;
        }
        if (!work->error) {
            work->error = !MatchesSign(work, CHAW_LECTURE_SIGNS[first ? 1 : 2]);
            if (!work->error || work->row == 1)
                return true;
            if (work->selectedCup == selected) {
                middle = first ? "0038B033" : "0038B042";
                last = first ? "0038B034" : "0038B043";
            }
            if (++work->reminders == 3)
                work->reminders = 0;
            work->error = 1;
            ChawQueueSerif(firstLine, "0038B031", "0038B032", middle, last, nullptr);
        } else
            work->error = 0;
    } else if (ChawReminderDue(work)) {
        middle = first ? "0038B028" : "0038B040";
        last = first ? "0038B029" : "0038B041";
        ChawQueueSerif("0038B022", "0038B023", "0038B024", middle, last, nullptr);
    }
    return true;
}
} // namespace shenmue::s2
