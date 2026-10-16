#include "s2/areas/wbbk_qte.hpp"
#include "s2/areas/wbbk_carry.hpp"
#include "s2/areas/wbbk_models.hpp"
#include "s2/motion_work.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/pause.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
char bookQteCommands[48];
std::int32_t bookQteCommandIndex, bookQteTimingIndex;
float bookQteTimings[9];
namespace {
std::int16_t qteMaximumFrames;
constexpr float bounds[5][2][2]{
    {{1, 1}, {2, 3}},
    {{1, 1}, {2, 4}},
    {{1, 1}, {2, 4}},
    {{1, 2}, {2, 3.5f}},
    {{1, 2.5f}, {3, 4}},
};
constexpr const char *patterns[5][2][4]{
    {{"LLRRD,E,", "", "", ""}, {"", "LR,LRDD,E,", "LR,LR,LRDD,E,", ""}},
    {{"LRD,E,", "", "", ""}, {"", "LR,LRDD,E,", "LR,LRD,LRDD,E,", "LR,LR,LRD,LRDD,E,"}},
    {{"LRD,E,", "", "", ""}, {"", "LR,LRDD,E,", "LR,LRD,LRDD,E,", "LR,LR,LRD,LRDD,E,"}},
    {{"LRU,E,", "LRD,LRU,E,", "", ""}, {"", "LRD,LRUU,E,", "LRD,LRD,LRUU,E,", "LRD,LRD,LRD,LRUU,E,"}},
    {{"LRD,E,", "LRU,UD,E,", "LRD,LRU,UD,E,", ""}, {"", "", "LRD,LRD,LRUD,E,", "LRD,LRD,LRUD,LRUD,E,"}},
};
constexpr std::int8_t intervals[14]{2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4};
} // namespace
std::uint32_t SelectBookQteCommands(std::int32_t type, std::int32_t pattern) {
    char *output = bookQteCommands;
    const char *source = patterns[bookModelState.currentBook / 3][type][pattern - 1];
    std::uint32_t count = 0xffffffffU;
    char command;
    do {
        std::int32_t length = 0;
        while (source[length] != ',')
            ++length;
        ++count;
        command = source[shenGetRandomInt32(length)];
        *output++ = command;
        source += length + 1;
    } while (command != 'E');
    output[-1] = 0;
    bookQteCommandIndex = 0;
    return count;
}
std::uint32_t RandomizeBookQteCommands(std::int32_t type) {
    const float first = bounds[bookModelState.currentBook / 3][type][0];
    const float scaled = ((bounds[bookModelState.currentBook / 3][type][1] - first) * .25f) * bookCarryMotion.difficulty;
    const float random = GetRandomNumberInRange(1);
    return SelectBookQteCommands(type, static_cast<std::int32_t>(random + (scaled + first)));
}
void InitializeBookCarryQte() {
    bookQteTimingIndex = 0;
    const float interval = bookCarryThresholds[bookModelState.currentBook] / static_cast<float>(intervals[bookModelState.currentBook]);
    for (std::int32_t index = 0; index < 9; ++index)
        bookQteTimings[index] = interval * static_cast<float>(index + 1);
}
std::uint32_t AdvanceBookQteCommand() {
    const char command = bookQteCommands[bookQteCommandIndex++];
    std::uint16_t button;
    switch (command) {
    case 'L':
        button = 64;
        break;
    case 'R':
        button = 4;
        break;
    case 'U':
    case 'D':
        button = 16;
        break;
    default:
        return 0;
    }
    SetShowQteFlag(button, qteMaximumFrames, 0);
    EVT_SetQteButtonState(57344);
    return button;
}
void StartBookCarryQte(std::int32_t type) {
    bookCarryPhase = 3;
    bookModelState.displayMode = 0;
    bookModelState.animationFrame = 0;
    Vector3 target;
    EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &target, EventTransformFlags::Local);
    target.y += 1.5f;
    UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_SAI), &target, 1);
    qteMaximumFrames = type == 2 ? 18 : 20;
    RandomizeBookQteCommands(type == 2 ? 1 : 0);
    AdvanceBookQteCommand();
}
std::int32_t CheckBookCarryQte() {
    if (bookCarryPhase != 3)
        return 0;
    const std::uint16_t status = static_cast<std::uint16_t>(EV_GetCurrentQteStatus());
    if (status == 0xffff)
        return 0;
    FinishLoading();
    if ((status & 0xf000) != 0x2000) {
        bookCarryPhase = 5;
        bookModelState.displayMode = 0;
        bookModelState.mirror = -1;
        return 2;
    }
    if ((bookCarryDistance + 40 > bookCarryThresholds[bookModelState.currentBook] && bookCarryVariant == 3) || !AdvanceBookQteCommand()) {
        bookCarryMode = 2;
        bookCarryPhase = 0;
        bookModelState.displayMode = 0;
        bookModelState.animationFrame = 0;
        return 1;
    }
    return 0;
}
} // namespace shenmue::s2::areas
