#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_pati.h"

#define PATI_NAIL_MAGIC SHEN_NAME4('N', 'A', 'I', 'L')

double atof(const char *text);
unsigned int strlen(const char *text);

const PatiGameType patiGameTypes[] = {
    {"IPPA", 1, 0, 1, 1, 0, "PCKTM01G"},
    {"MAR1", 2, 0, 3, 1, 2, "PCKTM02G"},
    {"PAR3", 3, 0, 3, 3, 0, "PCKTM03G"},
    {"PAR4", 4, 0, 4, 4, 0, "PCKTM04G"},
    {"TAK2", 5, 0, 3, 2, 1, "PCKTM01G"},
    {"COUN", 6, 0, 3, 3, 2, "PCKTM02G"},
    {"VERS", 7, 0, 3, 3, 3, "PCKTM01G"},
    {"VSJB", 8, 0, 3, 3, 3, "PCKTM01G"},
    {"", 1, 0, 1, 1, 0, "PCKTM01G"},
};
const PatiGameOption patiGameOptions[] = {
    {"NORM", 0, 0, 1.0f},
    {"DBUP", 1, 0, 1.0f},
    {"DBSP", 2, 0, 1.0f},
    {"DETH", 3, 0, 1.0f},
    {"KEIH", 7, 0, 1.0f},
    {"CAR3", 4, 0, 1.0f},
    {"CENT", 8, 0, 1.0f},
    {"BIG0", 6, 0, 8.0f},
    {"", 0, 0, 1.0f},
};
const PatiPrize patiPrizes[] = {
    {32, {0}, 0, "PCKAM00G", 0},
    {42, {0}, 1, "PCKAM01G", 0},
    {38, {0}, 2, "PCKAM02G", 0},
    {65, {0}, 4000, "PCKCMVAG", 1},
    {66, {0}, 4002, "PCKCMVBG", 1},
    {67, {0}, 4004, "PCKCMVCG", 1},
    {68, {0}, 4006, "PCKCMVDG", 1},
    {69, {0}, 4008, "PCKCMVEG", 1},
    {70, {0}, 4010, "PCKCMVFG", 1},
    {71, {0}, 4012, "PCKCMVGG", 1},
    {72, {0}, 4014, "PCKCMVHG", 1},
    {73, {0}, 4016, "PCKCMVIG", 2},
    {74, {0}, 4017, "PCKCMVJG", 2},
    {75, {0}, 4024, "PCKCMKAG", 1},
    {76, {0}, 4026, "PCKCMKBG", 1},
    {77, {0}, 4028, "PCKCMKCG", 1},
    {78, {0}, 4030, "PCKCMKDG", 1},
    {79, {0}, 4032, "PCKCMKEG", 1},
    {80, {0}, 4034, "PCKCMKFG", 1},
    {81, {0}, 4036, "PCKCMKGG", 1},
    {82, {0}, 4038, "PCKCMKHG", 1},
    {83, {0}, 4040, "PCKCMKIG", 1},
    {84, {0}, 4042, "PCKCMKJG", 1},
    {85, {0}, 4044, "PCKCMKKG", 2},
    {86, {0}, 4045, "PCKCMKLG", 2},
    {87, {0}, 4065, "PCKCMGAG", 2},
    {88, {0}, 4066, "PCKCMGBG", 2},
    {89, {0}, 4068, "PCKCMGCG", 3},
    {90, {0}, 4069, "PCKCMGDG", 3},
    {48, {0}, 4070, "PCKCMGEG", 3},
    {49, {0}, 4071, "PCKCMGFG", 1},
    {50, {0}, 4150, "PCKCMGGG", 1},
    {51, {0}, 4165, "PCKCMOAG", 3},
    {52, {0}, 4166, "PCKCMOBG", 3},
    {53, {0}, 4167, "PCKCMOCG", 3},
    {54, {0}, 3021, "PCKCMMKG", 2},
    {55, {0}, 3022, "PCKCMMKG", 2},
    {56, {0}, 4017, "PCKCMVJG", 2},
    {0, {0}, 0, "PCKAM00G", 0},
};
const float patiPrizeX[][10] = {
    {-0.225f, -0.075f, 0.075f, 0.225f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.24f, -0.12f, 0.0f, 0.12f, 0.24f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.25f, -0.15f, -0.05f, 0.05f, 0.15f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.257f, -0.172f, -0.086f, 0.0f, 0.086f, 0.172f, 0.257f, 0.0f, 0.0f, 0.0f},
    {-0.2625f, -0.1875f, -0.1125f, -0.0375f, 0.0375f, 0.1125f, 0.1875f, 0.2625f, 0.0f, 0.0f},
    {-0.264f, -0.198f, -0.132f, -0.066f, 0.0f, 0.066f, 0.132f, 0.198f, 0.264f, 0.0f},
    {-0.27f, -0.21f, -0.1515f, -0.09f, -0.03f, 0.03f, 0.09f, 0.15f, 0.21f, 0.27f},
};
const float patiPrizeSeparatorX[][10] = {
    {-0.1525f, -0.0025f, 0.14750001f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.1825f, -0.0625f, 0.057499997f, 0.17750001f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.2025f, -0.1025f, -0.0025f, 0.097500004f, 0.1975f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.2165f, -0.13149999f, -0.045500003f, 0.0405f, 0.1265f, 0.2115f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.22749999f, -0.1525f, -0.0775f, -0.0025f, 0.072500005f, 0.14750001f, 0.2225f, 0.0f, 0.0f, 0.0f},
    {-0.2335f, -0.1675f, -0.1015f, -0.0355f, 0.0305f, 0.0965f, 0.16250001f, 0.22850001f, 0.0f, 0.0f},
    {-0.24249999f, -0.1825f, -0.122499995f, -0.0625f, -0.0025f, 0.057499997f, 0.1175f, 0.17750001f, 0.2375f, 0.0f},
};
const float patiPocketPositions[][2] = {
    {-0.15f, 1.0f},
    {0.0f, 1.0f},
    {0.15f, 1.0f},
    {-0.15f, 0.5f},
    {0.0f, 0.5f},
    {0.15f, 0.5f},
};

void InitializePatiNails(PatiNailData *data)
{
    int i;
    for (i = 0; i < PATI_NAIL_CAPACITY; i++) {
        data->nails[i].position.x = 0.0f;
        data->nails[i].position.y = -1.0f;
        data->nails[i].position.z = 0.0f;
        data->nails[i].rotation.x = 0;
        data->nails[i].rotation.y = 0;
        data->nails[i].rotation.z = 0;
        data->nails[i].scale.x = 1.0f;
        data->nails[i].scale.y = 1.0f;
        data->nails[i].scale.z = 1.0f;
    }
}

int ParsePatiNailFile(PatiNailData *data, const char *resource)
{
    int *destination;
    int *source;
    int words, i;
    short x, y;
    do {
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ControlPackageResources(2, 2, "PATI", 0, 0);
        if (strlen(resource) < 8) {
            source = (int *)FTBL.FindChunkInCurrentMapInfo(resource);
            words = *source++ / 4;
        } else {
            source = (int *)FTBL.LoadCurrentPakBinary(resource);
            words = FTBL.GetCurrentPakBinarySize(resource) / 4;
        }
        destination = (int *)data;
    } while (0);
    if (!source) {
        FTBL.DebugLogToFileRemoved("Can't get PATI pak\n");
        FTBL.EventAssertNullsub(__FILE__, 302);
    }
    if (*source != PATI_NAIL_MAGIC) {
        FTBL.DebugLogToFileRemoved("OLD FORMAT [%8x]\n", *source);
        for (i = 0; i < words; i++)
            *destination++ = *source++;
        return words;
    }
    FTBL.DebugLogToFileRemoved("NEW FORMAT [%8x]\n", *source++);
    for (i = 0; i < words - 1; i++) {
        x = ((short *)source)[0];
        y = ((short *)source)[1];
        source++;
        data->nails[i].position.x = x * 0.0005f;
        data->nails[i].position.y = y * 0.0005f;
    }
    return (words - 1) * sizeof(PatiNailNode) + 52;
}

void InitializePatiPlacementNode(PatiPlacementNode *node)
{
    node->type = 152;
    node->flags = 0;
    node->position.x = node->position.y = node->position.z = 0.0f;
    node->rotation.x = node->rotation.y = node->rotation.z = 0;
    node->scale.x = node->scale.y = node->scale.z = 1.0f;
    node->mesh = 0;
    node->parent = 0;
    node->sibling = 0;
    node->child = 0;
    node->transform = 0;
    node->reserved3c = 0;
}

float PatiAtof(const char *text)
{
    return atof(text);
}

#define PATI_COPY(field, count) \
    for (i = 0; i < (count); i++) \
        (field)[i] = *cursor++; \
    (field)[i] = 0
#define PATI_EXPECT(separator, line) \
    if (*cursor++ != (separator)) \
        FTBL.EventAssertNullsub(__FILE__, line)
#define PATI_TEXT(field, maximum, separator) \
    i = 0; \
    if (*cursor != (separator)) \
        do { \
            (field)[i++] = *cursor++; \
            if (i > (maximum)) \
                break; \
        } while (*cursor != (separator)); \
    (field)[i] = 0
#define PATI_NUMBER(line) \
    PATI_TEXT(text, 254, ','); \
    PATI_EXPECT(',', line)

void PATI_TBL_Parser(PatiTableData *destination, const char *name)
{
    char area[5], tableName[5], boardName[5], text[256];
    const char *cursor, *end;
    unsigned int i, tableCount, boardCount;
    tableCount = 0;
    boardCount = 0;
    cursor = (const char *)FTBL.LoadCurrentPakBinary(name);
    end = cursor + FTBL.GetCurrentPakBinarySize(name);
    while (cursor < end) {
        switch (*cursor++) {
        case '[':
            PATI_COPY(area, 4);
            PATI_EXPECT(']', 389);
            break;
        case '(':
            PATI_COPY(tableName, 4);
            for (i = 0; i < 5; i++)
                destination->tables[tableCount].area[i] = area[i];
            for (i = 0; i < 5; i++)
                destination->tables[tableCount].tableName[i] = tableName[i];
            PATI_EXPECT(',', 404);
            PATI_NUMBER(415);
            destination->tables[tableCount].position.x = PatiAtof(text);
            PATI_NUMBER(427);
            destination->tables[tableCount].position.y = PatiAtof(text);
            PATI_NUMBER(439);
            destination->tables[tableCount].position.z = PatiAtof(text);
            PATI_NUMBER(451);
            destination->tables[tableCount].rotation.x = (unsigned short)(PatiAtof(text) * 8192.0f / 45.0f);
            PATI_NUMBER(463);
            destination->tables[tableCount].rotation.y = (unsigned short)(PatiAtof(text) * 8192.0f / 45.0f);
            PATI_NUMBER(475);
            destination->tables[tableCount].rotation.z = (unsigned short)(PatiAtof(text) * 8192.0f / 45.0f);
            PATI_COPY(destination->tables[tableCount].character, 4);
            PATI_EXPECT(',', 489);
            PATI_COPY(destination->tables[tableCount].npc, 4);
            PATI_EXPECT(',', 502);
            PATI_COPY(destination->tables[tableCount].surface, 4);
            PATI_EXPECT(',', 515);
            PATI_NUMBER(526);
            destination->tables[tableCount].scaleX = PatiAtof(text);
            PATI_NUMBER(538);
            destination->tables[tableCount].scaleY = PatiAtof(text);
            PATI_COPY(destination->tables[tableCount].gameOptions, 4);
            PATI_EXPECT(',', 552);
            PATI_COPY(destination->tables[tableCount].gameType, 4);
            PATI_EXPECT(',', 565);
            PATI_TEXT(destination->tables[tableCount].stake, 8, ',');
            PATI_EXPECT(',', 578);
            PATI_TEXT(destination->tables[tableCount].payout, 8, ',');
            PATI_EXPECT(',', 591);
            PATI_TEXT(destination->tables[tableCount].selectedBoard, 4, ')');
            PATI_EXPECT(')', 604);
            destination->tables[tableCount].firstBoard = boardCount;
            destination->tables[tableCount].boardCount = 0;
            if (++tableCount > 149)
                FTBL.EventAssertNullsub(__FILE__, 616);
            break;
        case '{':
            if (!tableCount)
                FTBL.EventAssertNullsub(__FILE__, 623);
            PATI_COPY(boardName, 4);
            for (i = 0; i < 5; i++)
                destination->boards[boardCount].character[i] = boardName[i];
            PATI_EXPECT(',', 637);
            PATI_NUMBER(648);
            destination->boards[boardCount].positionX = PatiAtof(text);
            PATI_COPY(destination->boards[boardCount].size, 1);
            PATI_EXPECT(',', 664);
            PATI_COPY(destination->boards[boardCount].model, 8);
            PATI_EXPECT(',', 679);
            PATI_TEXT(destination->boards[boardCount].labelModel, 7, ',');
            PATI_EXPECT(',', 694);
            PATI_TEXT(destination->boards[boardCount].prizes, 9, ',');
            PATI_EXPECT(',', 709);
            PATI_TEXT(destination->boards[boardCount].prizeModes, 5, ',');
            PATI_EXPECT(',', 724);
            PATI_TEXT(destination->boards[boardCount].prizeItems, 5, '}');
            PATI_EXPECT('}', 739);
            if (++boardCount > 299)
                FTBL.EventAssertNullsub(__FILE__, 752);
            destination->tables[tableCount - 1].boardCount++;
            break;
        }
    }
    destination->tables[tableCount].area[0] = 0;
    destination->boards[boardCount].model[0] = 0;
}
#endif
