#include "s2/areas/area_pati.hpp"
#include "s2/pati_tables.hpp"
#include "s2/event_runtime.hpp"
#include "s2/character.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
const PatiPrize &FindPrize(char code) {
    const PatiPrize *prize = PATI_PRIZES;
    while (prize->code && prize->code != code)
        ++prize;
    return *prize;
}
} // namespace
// @verified - p
void ConfigurePatiBallBoard(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    PATI_TBL_Parser(&b.tables);
    char area[5], table[5], board[5];
    std::memcpy(area, &b.game->area, 4);
    area[4] = 0;
    std::memcpy(table, &b.game->table, 4);
    table[4] = 0;
    std::memcpy(board, &b.game->board, 4);
    board[4] = 0;
    std::int32_t index = 0;
    while (b.tables.tables[index].area.name) {
        const PatiTableEntry &entry = b.tables.tables[index];
        if (!std::strcmp(area, reinterpret_cast<const char *>(&entry.area)) && !std::strcmp(table, entry.tableName))
            break;
        ++index;
    }
    const PatiTableEntry &place = b.tables.tables[index];
    std::int32_t boardIndex = place.firstBoard;
    while (boardIndex < static_cast<std::int32_t>(place.firstBoard) + place.boardCount && std::strcmp(board, b.tables.boards[boardIndex].character))
        ++boardIndex;
    PatiBoardEntry &entry = b.tables.boards[boardIndex];
    const PatiGameOption *option = PATI_GAME_OPTIONS;
    while (option->name[0] && std::strcmp(option->name, place.gameOptions))
        ++option;
    b.option = option->mode;
    b.scale = option->scale;
    const PatiGameType *type = PATI_GAME_TYPES;
    while (type->name[0] && std::strcmp(type->name, place.gameType))
        ++type;
    b.gameType = type->mode;
    b.ballLimit = static_cast<std::uint16_t>(type->countA);
    b.winLimit = static_cast<std::uint16_t>(type->countB);
    b.missLimit = static_cast<std::uint16_t>(type->countC);
    b.ballModel = type->model;
    switch (entry.size[0]) {
    case 'M':
        b.top = 1.09f;
        b.bottom = 0.0049999999f;
        b.left = -0.30000001f;
        b.right = 0.30000001f;
        b.boardSize = 2;
        break;
    case 'S':
        b.top = 0.88999999f;
        b.bottom = 0.0049999999f;
        b.left = -0.30000001f;
        b.right = 0.30000001f;
        b.boardSize = 1;
        break;
    case 'L':
    case 'X':
        b.top = 1.89f;
        b.bottom = 0.0049999999f;
        b.left = -0.30000001f;
        b.right = 0.30000001f;
        b.boardSize = b.scale == 1.0f ? 3 : 4;
        break;
    default:
        break;
    }
    b.nailFile = entry.model;
    b.labelModel = entry.labelModel;
    b.pocketCount = static_cast<std::int16_t>(std::strlen(entry.prizes));
    b.centerModes = entry.prizeModes;
    b.radius = (b.scale * 0.014f) * b.radiusScale;
    b.prizes = entry.prizes;
    b.centerPrizes = entry.prizeItems;
    for (PatiNailNode &nail : b.nails->nails)
        nail.scale = {b.scale * 1.5f, b.scale * 1.5f, b.scale};
    b.top = b.top * b.scale;
    b.bottom = b.bottom * b.scale;
    b.left = b.left * b.scale;
    b.right = b.right * b.scale;
    b.top = b.top - b.radius;
    b.bottom = b.bottom + b.radius;
    b.left = b.left + b.radius;
    b.right = b.right - b.radius;
    b.dividerHeight = b.scale * 0.075000003f;
    b.dividerWidth = b.scale * 0.0049999999f;
    for (std::int32_t divider = 0; divider + 1 < b.pocketCount; ++divider)
        b.dividers[divider] = {PATI_PRIZE_SEPARATOR_X[10 * b.pocketCount - 40 + divider] * b.scale, b.scale * 0.079999998f, 0.0f};
}
// @verified - p
void InitializePatiBallNails(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    ApplyPatiWeather(ball);
    b.radiusScale = 1.0f;
    ConfigurePatiBallBoard(ball);
    for (PatiNailNode &nail : b.nails->nails) {
        nail.position = {0.0f, -1.0f, 0.0f};
        nail.rotation = {};
        nail.scale = {1.0f, 1.0f, 1.0f};
    }
    ParsePatiNailFile(b.nails, b.nailFile);
    for (PatiNailNode &nail : b.nails->nails) {
        nail.scale = {b.scale * 1.5f, b.scale * 1.5f, b.scale};
        nail.position.x = b.scale * nail.position.x;
        nail.position.y = b.scale * nail.position.y;
        nail.position.z = b.scale * nail.position.z;
    }
    b.nails->character = PATI_PIN;
    EV_DisableCharacterDisplay(PATI_PIN);
}
// @verified - p
void ConfigurePatiPrizes(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    std::uint32_t wins = 0, deaths = 0;
    for (std::int32_t index = 0; index < b.pocketCount; ++index) {
        char &code = b.prizes[index];
        if (static_cast<std::uint8_t>(code - '6') <= 2)
            code = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1721) == 1 ? '6' : EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1722) == 1 ? '7' : '8';
        const PatiPrize &prize = FindPrize(code);
        if (prize.quantity) {
            if (b.option == 4 || b.option == 5 || b.option == 7)
                wins |= 1U << index;
        } else if (b.option != 7) {
            if (prize.item == 2)
                deaths |= 1U << index;
            else if (prize.item == 1 && !(b.option >= 4 && b.option <= 5))
                wins |= 1U << index;
        }
    }
    for (PatiCenterPocket &pocket : b.centerPockets) {
        pocket.type = 0;
        pocket.reserved14 = 0;
        pocket.reserved18 = 0;
    }
    if (b.option == 8) {
        for (std::uint32_t index = 0; index < 6; ++index) {
            if (b.centerModes[index] == ' ')
                continue;
            PatiCenterPocket &pocket = b.centerPockets[index];
            const PatiPrize &prize = FindPrize(b.centerPrizes[index]);
            if (!prize.quantity) {
                if (prize.item == 2)
                    deaths |= 1U << (index + 10);
                else if (prize.item == 1)
                    wins |= 1U << (index + 10);
            }
            pocket.x = PATI_PRIZE_X[index * 2] * b.scale;
            pocket.y = PATI_PRIZE_X[index * 2 + 1] * b.scale;
            switch (b.centerModes[index]) {
            case 'A':
                pocket.type = 1;
                pocket.halfWidth = 0.029999999f;
                break;
            case 'B':
                pocket.type = 2;
                pocket.halfWidth = 0.050000001f;
                break;
            case 'C':
                pocket.type = 3;
                pocket.halfWidth = 0.075000003f;
                break;
            default:
                continue;
            }
            pocket.height = 0.075000003f;
        }
    }
    b.winMask = static_cast<std::uint16_t>(wins);
    b.deathMask = static_cast<std::uint16_t>(deaths);
}
} // namespace shenmue::s2
