#include "s2/areas/area_ro10.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include <bit>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
void FinishVoice(AreaLoopVoiceWork &work) {
    if (work.flags & 0x80000000U)
        work.flags &= 0x7fffffffU;
    else
        HLib::DestroyCurrentTask();
}
void Ro10VoiceCallback(void *parameter) {
    AreaLoopVoiceWork *work = static_cast<AreaLoopVoiceWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!(work->flags & 2U)) {
        if (work->flags & 1U)
            dbgPrintEventMessage(work->voice);
        else
            work->conversation = EV_PlayDialogue(work->voice);
        if (work->completion)
            work->completion(0, 0, work->parent);
        work->flags |= 2U;
    }
    if (work->completion)
        work->completion(2, work->elapsed, work->parent);
    ++work->elapsed;
    if (work->duration >= 0) {
        if (static_cast<std::uint32_t>(work->duration) >= work->elapsed)
            return;
        if (work->flags & 1U)
            ctrlStopSubtitles();
        else {
            if (work->conversation && cvsHasValidConverseData(work->conversation))
                scenCvsCtrlFree(work->conversation);
            work->conversation = nullptr;
        }
        if (work->completion)
            work->completion(1, 0, work->parent);
        FinishVoice(*work);
        return;
    }
    bool done = true;
    if (!(work->flags & 1U)) {
        if (work->conversation && cvsHasValidConverseData(work->conversation))
            done = false;
        else {
            const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            const ScenarioTaskData *scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(control->scenarioTask));
            if (scenario->windowTextState.scenarioStateFlags & 0x80U)
                done = false;
            else
                work->conversation = nullptr;
        }
    }

    if (work->completion)
        work->completion(1, 0, work->parent);
    if (done)
        FinishVoice(*work);
}
struct DescriptionVariant {
    const char *first;
    const char *second;
    const char *third;
    std::int32_t secondDelay, thirdDelay;
};
struct Description {
    std::int32_t id;
    DescriptionVariant variants[3];
};
constexpr Description descriptions[]{{.id = 1, .variants = {{.first = "0110F004"}, {.first = "0110F005"}, {.first = "0110F005"}}},
    {.id = 2, .variants = {{.first = "0110F007"}, {.first = "0110F008"}, {.first = "0110F009"}}},
    {.id = 3, .variants = {{.first = "0110F010"}, {.first = "0110F011"}, {.first = "0110F012"}}},
    {.id = 4,
        .variants = {{.first = "0110F013", .second = "0110F014", .secondDelay = 95}, {.first = "0110F015", .second = "0110F016", .secondDelay = 85},
            {.first = "0110F017", .second = "0110F018", .secondDelay = 105}}},
    {.id = 10, .variants = {{.first = "0110L006"}, {.first = "0110L007", .second = "0110L008", .secondDelay = 95}, {.first = "0110L009"}}},
    {.id = 11,
        .variants = {{.first = "0110L010"}, {.first = "0110L011", .second = "0110L012", .secondDelay = 75},
            {.first = "0110L013", .second = "0110L014", .secondDelay = 100}}},
    {.id = 12,
        .variants = {{.first = "0110L015"}, {.first = "0110L016", .second = "0110L017", .secondDelay = 135},
            {.first = "0110L018", .second = "0110L019", .secondDelay = 105}}},
    {.id = 13,
        .variants = {{.first = "0110L020"}, {.first = "0110L021", .second = "0110L022", .secondDelay = 75},
            {.first = "0110L023", .second = "0110L024", .secondDelay = 135}}},
    {.id = 14,
        .variants = {{.first = "0110L025"}, {.first = "0110L026", .second = "0110L027", .secondDelay = 115},
            {.first = "0110L028", .second = "0110L029", .secondDelay = 105}}},
    {.id = 20, .variants = {{.first = "0110B006"}, {.first = "0110B007"}, {.first = "0110B008"}}},
    {.id = 21, .variants = {{.first = "0110B009"}, {.first = "0110B010", .second = "0110B011", .secondDelay = 95}, {.first = "0110B012"}}},
    {.id = 22,
        .variants = {{.first = "0110B013"}, {.first = "0110B014", .second = "0110B015", .third = "0110B016", .secondDelay = 135, .thirdDelay = 235},
            {.first = "0110B017"}}},
    {.id = 23,
        .variants = {{.first = "0110B018"}, {.first = "0110B019", .second = "0110B020", .secondDelay = 135},
            {.first = "0110B021", .second = "0110B022", .secondDelay = 105}}},
    {.id = 50, .variants = {{.first = "0110E008"}, {.first = "0110E009"}, {.first = "0110E010"}}},
    {.id = 51, .variants = {{.first = "0110E011"}, {.first = "0110E012"}, {.first = "0110E013"}}},
    {.id = 53, .variants = {{.first = "0110E018"}, {.first = "0110E019"}, {.first = "0110E020"}}},
    {.id = 54, .variants = {{.first = "0110E021"}, {.first = "0110E022"}, {.first = "0110E023"}}},
    {.id = 55, .variants = {{.first = "0110E018"}, {.first = "0110E019"}, {.first = "0110E024"}}}};
void RandomVoice(const char *first, const char *second, const char *third, std::int32_t delay, bool resume) {
    const std::int32_t choice = shenGetRandomInt32(3);
    QueueRo10Voice(choice == 0 ? first : choice == 1 ? second : third, delay, -1, resume);
}
} // namespace
void QueueRo10Voice(const char *voice, std::int32_t delay, std::int32_t duration, bool resume) {
    const std::size_t length = std::strlen(voice);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Ro10VoiceCallback, 4, 0x0b, sizeof(AreaLoopVoiceWork) + length + 1, RO10_EVENT);
    if (!task)
        return;
    AreaLoopVoiceWork *work = static_cast<AreaLoopVoiceWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->conversation = nullptr;
    work->flags = 0;
    work->duration = duration;
    work->elapsed = 0;
    work->completion = resume ? ResolveAreaLoopParent : nullptr;
    work->voice = reinterpret_cast<char *>(work + 1);
    std::memcpy(const_cast<char *>(work->voice), voice, length + 1);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    else {
        work->flags |= 0x80000000U;
        Ro10VoiceCallback(work);
        if (work->flags & 0x80000000U)
            work->flags &= 0x7fffffffU;
        else
            HLib::DestroyTask(task);
    }
}
void QueueRo10Motion(shen_name character, std::uint32_t motion, float lifetime, std::uint32_t flags) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), RO10_EVENT);
    if (!task)
        return;
    AreaLoopMotionWork *work = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->character = character;
    work->motion = motion;
    work->duration = -1;
    work->elapsed = 0;
    work->startFrame = -1;
    work->endFrame = -1;
    work->speed = 1;
    work->frameTime = std::bit_cast<std::uint32_t>(lifetime);
    work->completion = nullptr;
    work->flags = flags | 0x80000000U;
    AreaLoopMotionCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
void PlayRo10Greeting(Ro10Work &work) {
    switch (work.shop.name) {
    case SHEN_NAME(ARDX): {
        work.entryMode = 1;
        const std::int32_t choice = shenGetRandomInt32(3);
        QueueRo10Voice(choice == 0 ? "0110D005" : choice == 1 ? "0110D006" : "0110D007", 1);
        const std::int32_t delays[3]{45, 50, 70};
        RandomVoice("0110D008", "0110D009", "0110D010", delays[choice], true);
        break;
    }
    case SHEN_NAME(ARX2):
        work.entryMode = 1;
        QueueRo10Voice("0110F003", 1, -1, true);
        break;
    case SHEN_NAME(ARXX):
        work.entryMode = 0;
        RandomVoice("0110C004", "0110C005", "0110C006", 1, true);
        break;
    case SHEN_NAME(ARSF):
        work.entryMode = 0;
        QueueRo10Voice("0110B004", 1);
        QueueRo10Voice("0110B005", 40, -1, true);
        break;
    case SHEN_NAME(AKX7):
        work.entryMode = 0;
        RandomVoice("0110E004", "0110E005", "0110E006", 1, false);
        QueueRo10Voice("0110E007", 60, -1, true);
        break;
    case SHEN_NAME(QR00):
        work.entryMode = 0;
        QueueRo10Voice("0110L004", 1);
        QueueRo10Voice("0110L005", 40, -1, true);
        break;
    default:
        HLib::SetupTaskDelay(1);
        return;
    }
    QueueAreaLoopCamera(work.package, 1500, RO10_BASE, nullptr, 1);
}
void PlayRo10ItemDescription(std::int32_t dialogue) {
    QueueRo10Voice("0110A003", 1);
    if (dialogue == 30) {
        const char *first[]{"0110C007", "0110C008", "0110C009", "0110C010", "0110C011"};
        const std::int32_t delays[]{95, 135, 170, 165, 180};
        const std::int32_t choice = shenGetRandomInt32(5);
        QueueRo10Voice(first[choice], 35);
        const std::int32_t delay = delays[choice];
        const std::int32_t reply = shenGetRandomInt32(5);
        const char *second[]{"0110C012", "0110C013", "0110C014", "0110C015", "0110C017"};
        QueueRo10Voice(second[reply], delay, -1, reply != 3);
        if (reply == 3)
            QueueRo10Voice("0110C016", delay + 70, -1, true);
        return;
    }
    if (dialogue >= 40 && dialogue <= 43) {
        const char *first[]{"0110D011", "0110D012", "0110D013"};
        const std::int32_t delays[]{95, 105, 115};
        const std::int32_t choice = shenGetRandomInt32(3);
        QueueRo10Voice(first[choice], 35);
        const std::int32_t delay = delays[choice];
        const char *second[4][3]{{"0110D014", "0110D015", "0110D016"}, {"0110D017", "0110D018", "0110D019"}, {"0110D020", "0110D021", "0110D022"},
            {"0110D023", "0110D015", "0110D024"}};
        const std::int32_t reply = shenGetRandomInt32(3);
        QueueRo10Voice(second[dialogue - 40][reply], delay, dialogue == 40 && reply == 2 ? 100 : -1, true);
        return;
    }
    if (dialogue == 52) {
        QueueRo10Voice("0110E014", 35);
        const bool second = shenGetRandomInt32(2) != 0;
        QueueRo10Voice(second ? "0110E016" : "0110E015", 115);
        const std::int32_t delay = second ? 215 : 205;
        QueueRo10Voice("0110A005", delay);
        QueueRo10Voice("0110E017", delay + 50, -1, true);
        return;
    }
    for (const Description &description : descriptions)
        if (description.id == dialogue) {
            const DescriptionVariant &variant = description.variants[shenGetRandomInt32(3)];
            QueueRo10Voice(variant.first, 35, -1, !variant.second);
            if (variant.second)
                QueueRo10Voice(variant.second, variant.secondDelay, -1, !variant.third);
            if (variant.third)
                QueueRo10Voice(variant.third, variant.thirdDelay, -1, true);
            return;
        }
    HLib::SetupTaskDelay(-1);
}
void PlayRo10Selection(shen_name shop) {
    switch (shop.name) {
    case SHEN_NAME(ARDX):
        RandomVoice("0110D011", "0110D012", "0110D013", 1, false);
        break;
    case SHEN_NAME(ARX2):
        RandomVoice("0110F019", "0110F020", "0110F021", 1, false);
        break;
    case SHEN_NAME(ARXX):
        RandomVoice("0110C018", "0110C019", "0110C020", 1, false);
        break;
    case SHEN_NAME(ARSF):
        QueueRo10Voice("0110B023", 1);
        break;
    case SHEN_NAME(AKX7):
        RandomVoice("0110E025", "0110E026", "0110E027", 1, false);
        break;
    case SHEN_NAME(QR00):
        QueueRo10Voice("0110L030", 1);
        break;
    default:
        break;
    }
}
void PlayRo10InsufficientCash(shen_name shop) {
    QueueRo10Voice("0110A004", 1);
    switch (shop.name) {
    case SHEN_NAME(ARDX):
        QueueRo10Voice("0110D034", 71);
        RandomVoice("0110D035", "0110D036", "0110D037", 111, true);
        break;
    case SHEN_NAME(ARX2):
        QueueRo10Voice("0110A008", 71);
        QueueRo10Voice("0110F023", 111, -1, true);
        break;
    case SHEN_NAME(ARXX):
        QueueRo10Voice("0110C024", 61);
        RandomVoice("0110C025", "0110C026", "0110C027", 101, true);
        break;
    case SHEN_NAME(ARSF):
        QueueRo10Voice("0110B025", 61, -1, true);
        break;
    case SHEN_NAME(AKX7):
        QueueRo10Voice("0110E029", 61, -1, true);
        break;
    case SHEN_NAME(QR00):
        QueueRo10Voice("0110L032", 61, 100, true);
        break;
    default:
        break;
    }
}
void PlayRo10Payment(shen_name shop, bool after) {
    if (after) {
        switch (shop.name) {
        case SHEN_NAME(ARDX):
            RandomVoice("0110D031", "0110D032", "0110D033", 10, true);
            break;
        case SHEN_NAME(ARXX):
            QueueRo10Voice(shenGetRandomInt32(2) ? "0110C022" : "0110C021", 10, -1, true);
            break;
        case SHEN_NAME(ARX2):
            QueueRo10Voice("0110A007", 10, -1, true);
            break;
        case SHEN_NAME(ARSF):
        case SHEN_NAME(AKX7):
        case SHEN_NAME(QR00):
            HLib::SetupTaskDelay(1);
            break;
        default:
            break;
        }
    } else
        switch (shop.name) {
        case SHEN_NAME(ARDX):
            RandomVoice("0110D028", "0110D029", "0110D030", 10, true);
            break;
        case SHEN_NAME(ARXX):
            QueueRo10Voice("0110C023", 10, -1, true);
            break;
        case SHEN_NAME(ARX2):
            QueueRo10Voice("0110F022", 10, -1, true);
            break;
        case SHEN_NAME(ARSF):
            QueueRo10Voice("0110B024", 10, -1, true);
            break;
        case SHEN_NAME(AKX7):
            QueueRo10Voice("0110E028", 10, -1, true);
            break;
        case SHEN_NAME(QR00):
            QueueRo10Voice("0110L031", 10, -1, true);
            break;
        default:
            break;
        }
}
} // namespace shenmue::s2::areas
