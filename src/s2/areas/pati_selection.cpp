#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/conversation.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/pati_tables.hpp"
#include "s2/scene_data.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
namespace shenmue::s2 {
std::int32_t g_patiOddsVisible;
namespace {
char patiOddsText[50];
}
// @verified - p
void ShowPatiBoardOdds(PatiEventWork *work) {
    std::int32_t tableIndex = 0, boardIndex = 0;
    bool found = false;
    for (; work->tables.tables[tableIndex].area.name; ++tableIndex) {
        const PatiTableEntry &table = work->tables.tables[tableIndex];
        if (std::strcmp(work->bgm, reinterpret_cast<const char *>(&table.area)))
            continue;
        boardIndex = static_cast<std::int32_t>(table.firstBoard);
        const std::int32_t end = boardIndex + table.boardCount;
        while (boardIndex < end && std::strcmp(work->selectedName, work->tables.boards[boardIndex].character))
            ++boardIndex;
        if (boardIndex < end) {
            found = true;
            break;
        }
    }
    if (!found)
        return;
    const PatiTableEntry &table = work->tables.tables[tableIndex];
    const PatiGameOption *option = PATI_GAME_OPTIONS;
    while (option->name[0] && std::strcmp(option->name, table.gameOptions))
        ++option;
    if (!option->name[0])
        return;
    const PatiPayoutMode *payout = PATI_PAYOUT_MODES;
    while (payout->name[0] && std::strcmp(payout->name, table.payout))
        ++payout;
    if (!payout->name[0])
        return;
    const float stake = static_cast<float>(std::atof(table.stake));
    const std::string_view betLabel = GetLocalizedSceneText("UIGNRLBET0"); // @text EN: "Bet" | JA: " "
    if (option->mode == 7)
        std::snprintf(patiOddsText, sizeof(patiOddsText), "%.*s: $%i ", static_cast<int>(betLabel.size()), betLabel.data(), static_cast<int>(stake));
    else {
        char odds[24];
        if (payout->mode >= 21 && payout->mode <= 24) {
            float first = 0.0f, second = 0.0f, third = 0.0f;
            if ((payout->mode == 21 || payout->mode == 22) && stake > 0.0f) {
                first = (payout->mode == 21 ? 5.0f : 10.0f) / stake;
                second = (payout->mode == 21 ? 10.0f : 20.0f) / stake;
                third = (payout->mode == 21 ? 20.0f : 40.0f) / stake;
            }
            std::snprintf(odds, 20, "x%.1f/%i/%i", first, static_cast<int>(second), static_cast<int>(third));
        } else {
            const std::int32_t amount = static_cast<std::int32_t>(std::atof(table.payout));
            const std::int32_t ratio = amount > 0 && stake > 0.0f ? amount / static_cast<std::int32_t>(stake) : 0;
            std::snprintf(odds, 20, "x%i", ratio);
        }
        const std::string_view oddsLabel = GetLocalizedSceneText("UIGNRLODDS"); // @text EN: "Odds" | JA: " "
        std::snprintf(patiOddsText, sizeof(patiOddsText), "%.*s: $%i %.*s: %s", static_cast<int>(betLabel.size()), betLabel.data(), static_cast<int>(stake),
            static_cast<int>(oddsLabel.size()), oddsLabel.data(), odds);
    }
    g_patiOddsVisible = 1;
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    shenPrepareDialogue(control->scenarioTask, 1, patiOddsText, 2);
}
// @verified - p
std::int32_t SelectPatiBoardInput(PatiEventWork *work) {
    const std::int16_t input =
        g_patiCameraMoving || g_patiSelectionAccepted || g_patiInspectRequested ? 0 : static_cast<std::int16_t>(GetCurrentControlInput(INPUT_REPEAT));
    GetCurrentControlInput(INPUT_HELD);
    const std::int32_t previous = work->selectedBoard;
    std::int32_t left = previous, right = previous;
    float leftDistance = 100000000.0f, rightDistance = 100000000.0f;
    const PatiBoardChoice &selected = work->choices[previous];
    const Vector3 origin = selected.position;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(selected.rotation.y))));
    const float sine = sin_fixed16(static_cast<std::int16_t>(selected.rotation.y)) * -1.0f;
    for (std::int32_t index = 0; work->choices[index].character.name; ++index) {
        if (index == previous)
            continue;
        const Vector3 &position = work->choices[index].position;
        const float distance = GetSquaredDistanceXZ(&origin, &position);
        const float side = ((position.x - origin.x) * cosine + (position.y - origin.y) * 0.0f) + (position.z - origin.z) * sine;
        if (side <= 0.0f) {
            if (leftDistance > distance) {
                leftDistance = distance;
                left = index;
            }
        } else if (rightDistance > distance) {
            rightDistance = distance;
            right = index;
        }
    }
    if (!(input & 0x3000) && (input & 0xc000)) {
        if (input < 0)
            work->selectedBoard = right;
        else if (input & 0x4000)
            work->selectedBoard = left;
        if (work->selectedBoard != previous)
            g_patiCameraStep = 8;
    }
    const PatiBoardChoice &choice = work->choices[work->selectedBoard];
    g_patiBoardRotation = choice.rotation;
    g_patiBoardPosition = choice.position;
    g_patiBoardSize = choice.size;
    SetupPatiBoardSize(0);
    if (g_patiCameraStep == 4) {
        if (ResetSceneTextLanguage() > 1)
            ShowPatiBoardOdds(work);
        AttachPatiNails(work, choice.character, choice.nailFile);
        shen_name next;
        std::memcpy(&next, work->nextBoard, 4);
        SetPatiHelpTopic(work->option == 1 && choice.character.name == next.name ? 2 : choice.option, static_cast<std::int8_t>(choice.type));
    }
    std::memcpy(work->selectedName, &choice.character, 4);
    work->selectedName[4] = 0;
    if (GetBattleDataConfig(2, PATI_CANCEL)) {
        if (g_patiOddsVisible) {
            g_patiOddsVisible = 0;
            MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            scenDestroySubtitles(control->scenarioTask);
        }
        return -1;
    }
    if (GetBattleDataConfig(2, PATI_ACCEPT))
        g_patiSelectionAccepted = 1;
    else if (!g_patiSelectionAccepted) {
        GetBattleDataConfig(0, PATI_ACCEPT);
        GetBattleDataConfig(0, PATI_CANCEL);
    }
    if (g_patiCameraStep <= -15) {
        if (!g_patiCameraMoving && g_patiSelectionAccepted) {
            g_patiSelectionAccepted = 0;
            g_patiInspectRequested = 0;
            return 1;
        }
    } else
        --g_patiCameraStep;
    return 0;
}
} // namespace shenmue::s2
