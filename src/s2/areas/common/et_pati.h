#ifndef SHENMUE_S2_AREAS_COMMON_ET_PATI_H
#define SHENMUE_S2_AREAS_COMMON_ET_PATI_H
 
#include "s2/events/event_dc.h"

#define PATI_TABLE_CAPACITY 150
#define PATI_BOARD_CAPACITY 300

typedef struct PatiTableEntry {
    char area[5];
    char tableName[5];
    char reserved0a[2];
    Vector3 position;
    Vector3i rotation;
    char character[5];
    char npc[5];
    char surface[5];
    char reserved33;
    float scaleX, scaleY;
    char gameOptions[5];
    char gameType[5];
    char stake[10];
    char payout[10];
    char selectedBoard[6];
    char reserved60[4];
    int firstBoard;
    int boardCount;
} PatiTableEntry;
typedef struct PatiBoardEntry {
    char character[5];
    char reserved05[3];
    float positionX;
    char size[2];
    char model[9];
    char labelModel[9];
    char prizes[11];
    char prizeModes[7];
    char prizeItems[7];
    char reserved39[3];
} PatiBoardEntry;
typedef struct PatiTableData {
    PatiTableEntry tables[PATI_TABLE_CAPACITY];
    PatiBoardEntry boards[PATI_BOARD_CAPACITY];
} PatiTableData;

#define PATI_NAIL_CAPACITY 512
typedef struct PatiNailNode {
    int reserved00;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    int reserved28[2];
} PatiNailNode;
typedef struct PatiNailData {
    char header[48];
    PatiNailNode nails[PATI_NAIL_CAPACITY];
    shen_name character;
} PatiNailData;
typedef struct PatiPlacementNode {
    short type, flags;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    int mesh, child, sibling, parent;
    void *transform;
    int reserved3c;
} PatiPlacementNode;

typedef struct PatiGameType {
    char name[5];
    unsigned char mode;
    unsigned short reserved06;
    int countA, countB, countC;
    char model[9];
} PatiGameType;
typedef struct PatiGameOption {
    char name[5];
    unsigned char mode;
    unsigned short reserved06;
    float scale;
} PatiGameOption;
typedef struct PatiPrize {
    char code;
    char reserved01[3];
    int item;
    char model[9];
    unsigned char quantity;
} PatiPrize;

extern const PatiGameType patiGameTypes[];
extern const PatiGameOption patiGameOptions[];
extern const PatiPrize patiPrizes[];
extern const float patiPrizeX[][10];
extern const float patiPrizeSeparatorX[][10];
extern const float patiPocketPositions[][2];

void InitializePatiNails(PatiNailData *data);
int ParsePatiNailFile(PatiNailData *data, const char *resource);
void InitializePatiPlacementNode(PatiPlacementNode *node);
float PatiAtof(const char *text);
void PATI_TBL_Parser(PatiTableData *destination, const char *name);

#endif
