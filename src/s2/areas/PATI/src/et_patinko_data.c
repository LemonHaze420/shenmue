#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_pati.hpp"
#include "s2/event_queued_dc.h"
int strcmp(const char *,const char *);
unsigned int strlen(const char *);
void ApplyPatiWeather(PatiBallWork *ball);
void InitializePatiBallNails(PatiBallWork *ball);
void ConfigurePatiBallBoard(PatiBallWork *ball, const char *tableName);
void ConfigurePatiPrizes(PatiBallWork *ball);
void PatiBallCleanup(PatiBallWork *ball);

float patiBoardPrizeX[][10] = {
    {-0.225f, -0.075f, 0.075f, 0.225f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.24f, -0.12f, 0.0f, 0.12f, 0.24f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.25f, -0.15f, -0.05f, 0.05f, 0.15f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.257f, -0.172f, -0.086f, 0.0f, 0.086f, 0.172f, 0.257f, 0.0f, 0.0f, 0.0f},
    {-0.2625f, -0.1875f, -0.1125f, -0.0375f, 0.0375f, 0.1125f, 0.1875f, 0.2625f, 0.0f, 0.0f},
    {-0.264f, -0.198f, -0.132f, -0.066f, 0.0f, 0.066f, 0.132f, 0.198f, 0.264f, 0.0f},
    {-0.27f, -0.21f, -0.1515f, -0.09f, -0.03f, 0.03f, 0.09f, 0.15f, 0.21f, 0.27f},
};
float patiBoardPocketPositions[][2] = {
    {-0.15f, 1.0f},
    {0.0f, 1.0f},
    {0.15f, 1.0f},
    {-0.15f, 0.5f},
    {0.0f, 0.5f},
    {0.15f, 0.5f},
};
float patiBoardSeparatorX[][10] = {
    {-0.1525f, -0.0025f, 0.14750001f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.1825f, -0.0625f, 0.057499997f, 0.17750001f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.2025f, -0.1025f, -0.0025f, 0.097500004f, 0.1975f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.2165f, -0.13149999f, -0.045500003f, 0.0405f, 0.1265f, 0.2115f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.22749999f, -0.1525f, -0.0775f, -0.0025f, 0.072500005f, 0.14750001f, 0.2225f, 0.0f, 0.0f, 0.0f},
    {-0.2335f, -0.1675f, -0.1015f, -0.0355f, 0.0305f, 0.0965f, 0.16250001f, 0.22850001f, 0.0f, 0.0f},
    {-0.24249999f, -0.1825f, -0.122499995f, -0.0625f, -0.0025f, 0.057499997f, 0.1175f, 0.17750001f, 0.2375f, 0.0f},
};

void ApplyPatiWeather(PatiBallWork *ball) {
    EventDateTime time;
    float rate = 1, humidity = 1, temperature = 1;
    float hour, cosine;
    unsigned short angle;
    switch (FTBL.GetCurrentWeather()) {
    case 0:
    default:
        FTBL.DebugLogToFileRemoved("Wether:Sunny(Default)\n");
        break;
    case 1:
        FTBL.DebugLogToFileRemoved("Wether:Cloudy\n");
        humidity *= 0.99000001f;
        break;
    case 2:
        FTBL.DebugLogToFileRemoved("Wether:Rain\n");
        humidity *= 0.98000002f;
        break;
    }
    FTBL.GetCurrentDateTime((DateTime *)&time);
    hour = (float)time.hour + (float)time.minute / 60 + (float)time.second / 3600;
    angle = (unsigned short)(int)((hour - 13) / 24 * 360 * 8192 / 45);
    cosine = FTBL.CalculateFixedCosine(angle);
    temperature *= 1 - (cosine + 1) * 0.5f * 0.0099999998f;
    rate = humidity * temperature;
    FTBL.DebugLogToFileRemoved("Rate:%f ", rate);
    FTBL.DebugLogToFileRemoved("(Humidity:%f,", humidity);
    FTBL.DebugLogToFileRemoved("Temperature:%f)\n", temperature);
    ball->tangentialRetention = rate * 0.95999998f;
    ball->wallRestitution = rate * 0.40000001f;
    ball->nailRestitution = rate * 0.43000001f;
    ball->surfaceFriction = rate * 0.15000001f;
}

void InitializePatiBallNails(PatiBallWork *ball) {
    int index;
    ApplyPatiWeather(ball);
    ball->radiusScale = 1;
    ConfigurePatiBallBoard(ball, "PATI_TBL");
    InitializePatiNails(ball->nails);
    ParsePatiNailFile(ball->nails, ball->nailFile);
    for (index = 0; index < 512; ++index) {
        ball->nails->nails[index].scale.x = ball->scale * 1.5f;
        ball->nails->nails[index].scale.y = ball->scale * 1.5f;
        ball->nails->nails[index].scale.z = ball->scale;
        ball->nails->nails[index].position.x *= ball->scale;
        ball->nails->nails[index].position.y *= ball->scale;
        ball->nails->nails[index].position.z *= ball->scale;
    }
    ball->nails->character = SHEN_NAME4('P','I','N','_');
    FTBL.EV_DisableCharacterDisplay(ball->nails->character);
}

void ConfigurePatiBallBoard(PatiBallWork *ball, const char *tableName) {
    char area[5], table[5], board[5];
    int index, tableIndex = 0, boardIndex;
    PATI_TBL_Parser(&ball->tables, tableName);
    for (index = 0; index < 4; ++index)
        area[index] = ball->game->area >> (index * 8);
    area[index] = 0;
    for (index = 0; index < 4; ++index)
        table[index] = ball->game->table >> (index * 8);
    table[index] = 0;
    for (index = 0; index < 4; ++index)
        board[index] = ball->game->board >> (index * 8);
    board[index] = 0;
    while (ball->tables.tables[tableIndex].area[0]) {
        if (!strcmp(area, ball->tables.tables[tableIndex].area) && !strcmp(table, ball->tables.tables[tableIndex].tableName))
            break;
        ++tableIndex;
    }
    if (!ball->tables.tables[tableIndex].area[0]) {
        FTBL.DebugLogToFileRemoved("%s:no place:%s,%s\n", tableName, area, table);
        FTBL.EventAssertNullsub(__FILE__, 248);
    }
    boardIndex = ball->tables.tables[tableIndex].firstBoard;
    while (boardIndex < ball->tables.tables[tableIndex].firstBoard + ball->tables.tables[tableIndex].boardCount) {
        if (!strcmp(board, ball->tables.boards[boardIndex].character))
            break;
        ++boardIndex;
    }
    if (boardIndex == ball->tables.tables[tableIndex].firstBoard + ball->tables.tables[tableIndex].boardCount) {
        FTBL.DebugLogToFileRemoved("%s:no board:%s,%s,%s\n", tableName, area, table, board);
        FTBL.EventAssertNullsub(__FILE__, 258);
    }
    index = 0;
    while (patiGameOptions[index].name[0] && strcmp(patiGameOptions[index].name, ball->tables.tables[tableIndex].gameOptions))
        ++index;
    ball->option = patiGameOptions[index].mode;
    ball->scale = patiGameOptions[index].scale;
    index = 0;
    while (patiGameTypes[index].name[0] && strcmp(patiGameTypes[index].name, ball->tables.tables[tableIndex].gameType))
        ++index;
    ball->gameType = (signed char)patiGameTypes[index].mode;
    ball->ballLimit = patiGameTypes[index].countA;
    ball->winLimit = patiGameTypes[index].countB;
    ball->missLimit = patiGameTypes[index].countC;
    ball->ballModel = patiGameTypes[index].model;
    switch (ball->tables.boards[boardIndex].size[0]) {
    case 'S':
        ball->top = 0.88999999f;
        ball->bottom = 0.0049999999f;
        ball->left = -0.30000001f;
        ball->right = 0.30000001f;
        ball->boardSize = 1;
        break;
    case 'M':
        ball->top = 1.09f;
        ball->bottom = 0.0049999999f;
        ball->left = -0.30000001f;
        ball->right = 0.30000001f;
        ball->boardSize = 2;
        break;
    case 'L':
    case 'X':
        ball->top = 1.89f;
        ball->bottom = 0.0049999999f;
        ball->left = -0.30000001f;
        ball->right = 0.30000001f;
        ball->boardSize = 3;
        if (ball->scale != 1)
            ball->boardSize = 4;
        break;
    default:
        FTBL.DebugLogToFileRemoved("%s:size error:%s,%s,%s,%s\n", tableName, area, table, board, ball->tables.boards[boardIndex].size);
        FTBL.EventAssertNullsub(__FILE__, 307);
        break;
    }
    ball->nailFile = ball->tables.boards[boardIndex].model;
    ball->labelModel = ball->tables.boards[boardIndex].labelModel;
    ball->pocketCount = strlen(ball->tables.boards[boardIndex].prizes);
    ball->prizes = ball->tables.boards[boardIndex].prizes;
    ball->centerModes = ball->tables.boards[boardIndex].prizeModes;
    ball->centerPrizes = ball->tables.boards[boardIndex].prizeItems;
    ball->radius = ball->scale * 0.014f * ball->radiusScale;
    for (index = 0; index < 512; ++index) {
        ball->nails->nails[index].scale.x = ball->scale * 1.5f;
        ball->nails->nails[index].scale.y = ball->scale * 1.5f;
        ball->nails->nails[index].scale.z = ball->scale;
    }
    ball->top *= ball->scale;
    ball->bottom *= ball->scale;
    ball->left *= ball->scale;
    ball->right *= ball->scale;
    ball->top -= ball->radius;
    ball->bottom += ball->radius;
    ball->left += ball->radius;
    ball->right -= ball->radius;
    ball->dividerWidth = ball->scale * 0.0049999999f;
    ball->dividerHeight = ball->scale * 0.075000003f;
    for (index = 0; index + 1 < ball->pocketCount; ++index) {
        ball->dividers[index].x = patiBoardSeparatorX[ball->pocketCount - 4][index] * ball->scale;
        ball->dividers[index].y = ball->scale * 0.079999998f;
        ball->dividers[index].z = 0;
    }
    FTBL.DebugLogToFileRemoved("%s,%s,%s,%s,%s,%s\n", area, table, board, ball->tables.tables[tableIndex].gameOptions,
        ball->tables.tables[tableIndex].gameType, ball->nailFile);
}

void ConfigurePatiPrizes(PatiBallWork *ball) {
    Vector3 origin = {0, 0, 0}, front = {0, 0, 0}, back = {0, 0, 0}, scale;
    unsigned int wins, deaths;
    int index, prize, centers;
    scale.x = scale.y = scale.z = ball->scale;
    wins = deaths = 0;
    for (index = 0; index < ball->pocketCount; ++index) {
        if ((unsigned char)(ball->prizes[index] - '6') <= 2) {
            if (FTBL.EV_GetPlayerFlag(11, 1721) == 1)
                ball->prizes[index] = '6';
            else if (FTBL.EV_GetPlayerFlag(11, 1722) == 1)
                ball->prizes[index] = '7';
            else
                ball->prizes[index] = '8';
        }
        prize = 0;
        while (patiPrizes[prize].code && patiPrizes[prize].code != ball->prizes[index])
            ++prize;
        if (!patiPrizes[prize].quantity) {
            if (ball->option == 7) {
                FTBL.DebugLogToFileRemoved("Standard-icon found in keihin board.\n");
                FTBL.EventAssertNullsub(__FILE__, 493);
            } else {
                switch ((unsigned int)patiPrizes[prize].item) {
                case 0:
                default:
                    break;
                case 1:
                    if (ball->option == 7 || ball->option == 4 || ball->option == 5) {
                        FTBL.DebugLogToFileRemoved("Atari-icon found in keihin board.\n");
                        FTBL.EventAssertNullsub(__FILE__, 505);
                    } else
                        wins |= 1U << index;
                    break;
                case 2:
                    deaths |= 1U << index;
                    break;
                }
            }
        } else if (ball->option == 7 || ball->option == 4 || ball->option == 5)
            wins |= 1U << index;
        else {
            FTBL.DebugLogToFileRemoved("Keihin-icon found in no keihin board.\n");
            FTBL.EventAssertNullsub(__FILE__, 524);
        }
    }
    for (index = 0; index < 6; ++index) {
        ball->centerPockets[index].reserved14 = 0;
        ball->centerPockets[index].reserved18 = 0;
        ball->centerPockets[index].type = 0;
    }
    if (ball->option == 8) {
        centers = 0;
        for (index = 0; index < 6; ++index) {
            if (ball->centerModes[index] == ' ') {
                if (ball->centerPrizes[index] != ' ') {
                    FTBL.DebugLogToFileRemoved("Illegal mark at no center pocket position.\n");
                    FTBL.EventAssertNullsub(__FILE__, 559);
                }
                continue;
            }
            ++centers;
            prize = 0;
            while (patiPrizes[prize].code && patiPrizes[prize].code != ball->centerPrizes[index])
                ++prize;
            if (!patiPrizes[prize].quantity) {
                switch ((unsigned int)patiPrizes[prize].item) {
                case 0:
                default:
                    break;
                case 1:
                    wins |= 1U << (index + 10);
                    break;
                case 2:
                    deaths |= 1U << (index + 10);
                    break;
                }
            } else {
                FTBL.DebugLogToFileRemoved("Keihin-icon found in no keihin board.\n");
                FTBL.DebugLogToFileRemoved("%c,%i,%i\n", ball->centerPrizes[index], index, patiPrizes[prize].quantity);
                FTBL.DebugLogToFileRemoved("%08x,%s\n", ball->centerPrizes, ball->centerPrizes);
                FTBL.EventAssertNullsub(__FILE__, 587);
            }
            ball->centerPockets[index].x = back.x = front.x = patiBoardPocketPositions[index][0] * ball->scale;
            ball->centerPockets[index].y = back.y = front.y = patiBoardPocketPositions[index][1] * ball->scale;
            front.z = 0.0049999999f;
            switch (ball->centerModes[index]) {
            case 'A':
                ball->centerPockets[index].type = 1;
                ball->centerPockets[index].halfWidth = 0.029999999f;
                ball->centerPockets[index].height = 0.075000003f;
                break;
            case 'B':
                ball->centerPockets[index].type = 2;
                ball->centerPockets[index].halfWidth = 0.050000001f;
                ball->centerPockets[index].height = 0.075000003f;
                break;
            case 'C':
                ball->centerPockets[index].type = 3;
                ball->centerPockets[index].halfWidth = 0.075000003f;
                ball->centerPockets[index].height = 0.075000003f;
                break;
            default:
                FTBL.DebugLogToFileRemoved("Illegal center pocket type. [%i] =", index);
                FTBL.DebugLogToFileRemoved("$c", ball->centerModes[index]);
                FTBL.DebugLogToFileRemoved("(0x$02X)\n", ball->centerModes[index]);
                FTBL.EventAssertNullsub(__FILE__, 629);
                break;
            }
        }
        if (!centers) {
            FTBL.DebugLogToFileRemoved("no center pocket found in center pocket board.\n");
            FTBL.EventAssertNullsub(__FILE__, 640);
        }
    }
    if (!wins) {
        FTBL.DebugLogToFileRemoved("no bingo found.\n");
        FTBL.EventAssertNullsub(__FILE__, 645);
    }
    if (ball->option == 3 && !deaths) {
        FTBL.DebugLogToFileRemoved("no death found in sudden-death board.\n");
        FTBL.EventAssertNullsub(__FILE__, 649);
    }
    if (ball->option != 3 && deaths) {
        FTBL.DebugLogToFileRemoved("death found in no sudden-death board.\n");
        FTBL.EventAssertNullsub(__FILE__, 653);
    }
    ball->winMask = wins;
    ball->deathMask = deaths;
}

void PatiBallCleanup(PatiBallWork *ball) {
}

#endif
