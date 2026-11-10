#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/pati_tables.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2 {
// @verified - p
void SetPatiHelpTopic(std::uint8_t option, std::int8_t type) {
    const PatiHelpTopic *topic = PATI_HELP_TOPICS;
    while (topic->topic != -1 && (topic->option != option || static_cast<std::int8_t>(topic->type) != type))
        ++topic;
    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(topic->topic == -1 ? 119 : topic->topic), 0);
}
// @verified - p
void ConfigurePatiEventBoard(PatiEventWork *work, const char *name) {
    char area[5], search[5];
    std::memcpy(area, &g_currentMap, 4);
    area[4] = 0;
    std::memcpy(search, name, 4);
    search[4] = 0;
    if (search[0] == 'b')
        search[0] = 'B';
    std::int32_t tableIndex = 0, boardIndex = 0;
    for (; work->tables.tables[tableIndex].area.name; ++tableIndex) {
        const PatiTableEntry &table = work->tables.tables[tableIndex];
        if (std::strcmp(area, reinterpret_cast<const char *>(&table.area)))
            continue;
        boardIndex = static_cast<std::int32_t>(table.firstBoard);
        const std::int32_t end = boardIndex + table.boardCount;
        while (boardIndex < end && std::strcmp(search, work->tables.boards[boardIndex].character))
            ++boardIndex;
        if (boardIndex < end)
            break;
    }
    std::memcpy(work->board, name, 4);
    work->board[4] = 0;
    const PatiTableEntry &table = work->tables.tables[tableIndex];
    const PatiBoardEntry &board = work->tables.boards[boardIndex];
    const shen_name previous = work->dealer;
    std::memcpy(&work->dealer, table.npc, 4);
    if (!work->dealer.name || work->dealer.name == 0x20202020U)
        work->dealer = previous;
    work->game.area = table.area;
    std::memcpy(&work->game.table, table.tableName, 4);
    std::memcpy(&work->game.board, board.character, 4);
    const PatiGameOption *option = PATI_GAME_OPTIONS;
    while (option->name[0] && std::strcmp(option->name, table.gameOptions))
        ++option;
    work->option = option->mode;
    work->scale = option->scale;
    const PatiGameType *type = PATI_GAME_TYPES;
    while (type->name[0] && std::strcmp(type->name, table.gameType))
        ++type;
    work->type = type->mode;
    const PatiWagerMode *wager = PATI_WAGER_MODES;
    while (wager->name[0] && std::strcmp(wager->name, table.stake))
        ++wager;
    work->wagerMode = wager->mode;
    const PatiPayoutMode *payout = PATI_PAYOUT_MODES;
    while (payout->name[0] && std::strcmp(payout->name, table.payout))
        ++payout;
    work->payoutMode = payout->mode;
    if (work->option == 1 || static_cast<std::uint8_t>(work->option - 4) <= 1) {
        for (std::int32_t index = 0; index < 7; ++index) {
            work->nextBoard[index] = board.prizeModes[index];
            if (!work->nextBoard[index])
                break;
        }
        work->nextBoard[7] = 0;
        for (std::int32_t index = 0; index < 7; ++index) {
            work->excludedBoard[index] = table.selectedBoard[index];
            if (!work->excludedBoard[index])
                break;
        }
        work->excludedBoard[7] = 0;
    }
    SelectPatiBoard(work, work->board);
    AttachPatiNails(work, work->game.board, board.model);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    SetPatiHelpTopic(work->option == 1 && !std::strcmp(work->nextBoard, board.character) ? 2 : work->option, static_cast<std::int8_t>(work->type));
}
// @verified - p
void FindPatiDealerBoards(PatiEventWork *work, const char *dealer) {
    g_patiMuseumMode = 0;
    g_patiSingleMode = 0;
    g_patiMinimumBet = 100000;
    Vector3 player;
    EV_GetCharaPosition(PATI_PLAYER, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    char area[5], group[5];
    std::memcpy(area, &g_currentMap, 4);
    area[4] = 0;
    std::memcpy(group, dealer, 4);
    group[4] = 0;
    PatiDebugLog("PATI\\ePATI_et_patinko.c", 4888, "Search DM=%s:", area);
    PatiDebugLog("PATI\\ePATI_et_patinko.c", 4889, "%s\n", group);
    std::int32_t count = 0, nearest = 9999;
    float nearestDistance = 100000000.0f;
    const char *selectedOption = nullptr;
    const char *selectedType = nullptr;
    for (std::int32_t tableIndex = 0; work->tables.tables[tableIndex].area.name; ++tableIndex) {
        const PatiTableEntry &table = work->tables.tables[tableIndex];
        if (std::strcmp(area, reinterpret_cast<const char *>(&table.area)) || std::strcmp(group, table.npc))
            continue;
        const std::int32_t end = static_cast<std::int32_t>(table.firstBoard) + table.boardCount;
        for (std::int32_t boardIndex = static_cast<std::int32_t>(table.firstBoard); boardIndex < end; ++boardIndex) {
            const PatiBoardEntry &board = work->tables.boards[boardIndex];
            PatiBoardChoice &choice = work->choices[count];
            std::memcpy(&choice.character, board.character, 4);
            EV_GetCharaPosition(choice.character, CHARACTER_TRANSLATION_ROOT, &choice.position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(choice.character, &choice.rotation, EventTransformFlags::World);
            if (choice.rotation.z >= 0x8000) {
                choice.rotation.x += 32768;
                choice.rotation.z -= 32768;
                choice.rotation.y =
                    static_cast<std::uint16_t>(static_cast<std::int32_t>((540.0 - static_cast<float>(choice.rotation.y) / 182.04445f) * 8192.0 / 45.0));
            }
            choice.size = board.size[0];
            choice.nailFile = board.model;
            const float distance = GetSquaredDistanceXZ(&player, &choice.position);
            const PatiGameType *type = PATI_GAME_TYPES;
            while (type->name[0] && std::strcmp(type->name, table.gameType))
                ++type;
            if (!type->name[0])
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 4916, "Error:Undefined Game Type.\n");
            choice.type = type->mode;
            const PatiGameOption *option = PATI_GAME_OPTIONS;
            while (option->name[0] && std::strcmp(option->name, table.gameOptions))
                ++option;
            if (!option->name[0])
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 4925, "Error:Undefined Game Option.\n");
            choice.option = option->mode;
            const PatiWagerMode *wager = PATI_WAGER_MODES;
            while (wager->name[0] && std::strcmp(wager->name, table.stake))
                ++wager;
            if (!wager->name[0])
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 4936, "Error:Undefined Bet Type.\n");
            work->wagerMode = wager->mode;
            SetPatiBet(work);
            if (g_patiMinimumBet > work->bet)
                g_patiMinimumBet = work->bet;
            const bool doubleBoard = !std::strcmp(table.gameOptions, "DBUP") && !std::strcmp(board.character, table.selectedBoard);
            const bool challengeBoard =
                (!std::strcmp(table.gameOptions, "CAR3") || !std::strcmp(table.gameOptions, "CAR5")) && std::strcmp(board.character, table.selectedBoard);
            if (selectedOption && std::strcmp(selectedOption, table.gameOptions))
                g_patiMuseumMode = 1;
            if (!doubleBoard && !challengeBoard) {
                if (nearestDistance > distance) {
                    nearestDistance = distance;
                    nearest = count;
                }
                if (selectedType && std::strcmp(selectedType, table.gameType))
                    g_patiMuseumMode = 1;
                if (nearest == count || !selectedOption)
                    selectedOption = table.gameOptions;
                if (nearest == count || !selectedType)
                    selectedType = table.gameType;
            }
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4977, "bd:%s ", board.character);
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4978, "(% f,", choice.position.x);
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4979, "% f,", choice.position.y);
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4980, "% f)-", choice.position.z);
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4981, "[% f,", static_cast<float>(choice.rotation.x) / 182.04445f);
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4982, "% f,", static_cast<float>(choice.rotation.y) / 182.04445f);
            PatiDebugLog("PATI\\ePATI_et_patinko.c", 4983, "% f]\n", static_cast<float>(choice.rotation.z) / 182.04445f);
            if (++count >= 20)
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 4986, "Too much boards controled by domoto [%s]\n", group);
        }
    }
    if (!count)
        PatiDebugLog("PATI\\ePATI_et_patinko.c", 4996, "No boards controled by domoto [%s]\n", group);
    if (count == 1)
        g_patiSingleMode = 1;
    work->choices[count].character = {};
    PatiDebugLog("PATI\\ePATI_et_patinko.c", 5004, "Museum Mode: ");
    if (g_patiMuseumMode)
        PatiDebugLog("PATI\\ePATI_et_patinko.c", 5006, "Enable\n");
    else
        PatiDebugLog("PATI\\ePATI_et_patinko.c", 5008, "Disable\n");
    PatiDebugLog("PATI\\ePATI_et_patinko.c", 5010, "Single Mode: ");
    if (g_patiSingleMode)
        PatiDebugLog("PATI\\ePATI_et_patinko.c", 5012, "Enable\n");
    else
        PatiDebugLog("PATI\\ePATI_et_patinko.c", 5014, "Disable\n");
    work->selectedBoard = nearest;
    AttachPatiNails(work, work->choices[nearest].character, work->choices[nearest].nailFile);
    g_patiCameraStep = 8;
    g_patiSelectionAccepted = 0;
    g_patiInspectRequested = 0;
    g_patiCameraMoving = 1;
    std::memcpy(work->selectedName, &work->choices[work->selectedBoard].character, 4);
    work->selectedName[4] = 0;
}
// @verified - p
shen_name FindNearestPatiDealer() {
    constexpr shen_name dealers[]{SHEN_NAME(MST1),
        SHEN_NAME(MST2), SHEN_NAME(MST3), SHEN_NAME(MST4), SHEN_NAME(MST5), SHEN_NAME(MST6), SHEN_NAME(MST7), SHEN_NAME(MST8), SHEN_NAME(MST9)};
    Vector3 player;
    EV_GetCharaPosition(PATI_PLAYER, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    std::int32_t selected = -1;
    float minimum = 1000000.0f;
    for (std::int32_t index = 0; index < 9; ++index) {
        if (!DoesScenarioCharacterExist(dealers[index]))
            continue;
        Vector3 position;
        EV_GetCharaPosition(dealers[index], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const float distance = GetSquaredDistanceXZ(&player, &position);
        for (std::int32_t phase = 0; phase < 3; ++phase) {
            if (phase == 2) {
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 5828, "%f\n", distance);
            } else {
                const Vector3 &logged = phase == 0 ? player : position;
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 5822 + phase * 3, "(%f,", logged.x);
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 5823 + phase * 3, "%f,", logged.y);
                PatiDebugLog("PATI\\ePATI_et_patinko.c", 5824 + phase * 3, "%f)-", logged.z);
            }
            volatile std::int64_t wait = 1000000;
            while (wait)
                wait = wait - 1;
        }
        if (minimum > distance) {
            minimum = distance;
            selected = index;
        }
    }
    return selected >= 0 ? dealers[selected] : shen_name{};
}
// @verified - p
shen_name FindPatiDealer(PatiEventWork *work) {
    if (!work->dealer.name || work->dealer.name == 0x20202020U)
        return FindNearestPatiDealer();
    return DoesScenarioCharacterExist(work->dealer) ? work->dealer : shen_name{};
}
} // namespace shenmue::s2
