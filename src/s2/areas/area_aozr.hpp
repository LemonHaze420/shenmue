#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_debug_dc.h"
#include "s2/event_queued_dc.h"

#define AOZR_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AOZR_MAGIC SHEN_NAME4('A', 'O', 'Z', 'R')
#define AOZR_BTIL SHEN_NAME4('B', 'T', 'I', 'L')
#define AOZR_BTEV SHEN_NAME4('B', 'T', 'E', 'V')
#define AOZR_BTCC SHEN_NAME4('B', 'T', 'C', 'C')
#define AOZR_INVALID 0xffffffffU
#define AOZR_DEBUG_ID 3
#define AOZR_SCRIPT_WAITING 0x80000001U
#define AOZR_SNAP(value) if (__builtin_fabs(value) < 1.1920929e-7f) value = 0.0f
#define AOZR_SELECTED_CELL(work) ((const AozrCell *)((const char *)(work)->board + (work)->board->cellsOffset) + ((work)->x + (work)->y * (work)->board->columns))

enum {
    AOZR_SCRIPT_RET,
    AOZR_SCRIPT_SETRESULT,
    AOZR_SCRIPT_WAIT,
    AOZR_SCRIPT_SERIF,
    AOZR_SCRIPT_MOTION,
    AOZR_SCRIPT_POSANG,
    AOZR_SCRIPT_CAMERA,
    AOZR_SCRIPT_SELECT
};

typedef struct AozrScriptWork {
    char name[16];
    int *script;
    int pc;
    unsigned int flags;
    int frame;
    int wait;
    HLibTask *parent;
    int *result;
} AozrScriptWork;

typedef struct AozrSerif {
    int flags;
    short frames;
    char text[2];
} AozrSerif;

typedef struct AozrSerifWork {
    void *conversation;
    short frames;
    short voice;
} AozrSerifWork;

typedef struct AozrMotion {
    shen_name name;
    float start, end, speed;
    short motion;
    unsigned short flags;
} AozrMotion;

typedef struct AozrPosAng {
    shen_name name;
    float x, z, angle;
    int flags;
} AozrPosAng;

typedef struct AozrCamera {
    shen_name target;
    short camera;
} AozrCamera;

typedef struct AozrSelect {
    short timeout;
    unsigned short mode;
    unsigned short choices[4];
    int fallback;
    int targets[4];
} AozrSelect;

typedef struct AozrBoard {
    shen_name magic;
    char reserved04[8];
    char audio[12];
    int audioVariant;
    char authorBank[12];
    shen_name seller;
    float spacingX, spacingY;
    Vector3 origin, angles;
    unsigned int reserved4c;
    short transitionFrames[4];
    Vector3 cameraOffsets[4];
    short initialX, initialY, columns, rows;
    int cellsOffset, startScript, buyScript;
} AozrBoard;

typedef struct AozrCell {
    shen_name character;
    int price;
    short item, variant;
    int explanation, reserved10;
} AozrCell;

typedef struct AozrShopWork {
    HLibTask *self;
    HLibTask *parent;
    char package[8];
    shen_name seller;
    unsigned int flags;
    int x, y, camera, transition, elapsed;
    const AozrBoard *board;
    float zoom;
    short bufferedInput;
    int soundIndex;
    char savedAudio[4];
} AozrShopWork;

extern const char aozrBootError[];

void RunAozrEvent(int argument);
int AozrStartScript(const char *name, int *script, int *result, unsigned int flags);
void AozrScriptCallback(void *data);
int AozrStartSerif(AozrSerif *serif);
void AozrSerifCallback(void *data);
void AozrPlayMotion(AozrMotion *motion);
void AozrSetPosAng(AozrPosAng *posAng);
void AozrSetCamera(AozrCamera *camera, AozrScriptWork *work);
int AozrStartSelect(AozrSelect *select);
int AozrPollSelect(AozrSelect *select);
void AozrDoneCallback(void *data);
void AozrDebugCallback(void *data);
void AozrStartPackageCallback(void *data);
void AozrWaitPackageCallback(void *data);
void AozrGetNpcCallback(void *data);
void AozrInitializeCallback(void *data);
void AozrStartSaleCallback(void *data);
void AozrWaitStartCallback(void *data);
int AozrShowBoard(AozrShopWork *work);
int AozrHideBoard(AozrShopWork *work);
void AozrLockOnCallback(void *data);
void AozrAskBuyCallback(void *data);
void AozrWaitBuyCallback(void *data);
void AozrDeliverCallback(void *data);
shen_name AozrBoardCell(const AozrBoard *board, Vector3 *position, Vector3i *rotation, int x, int y);
void AozrPrepareSeller(shen_name name);
void AozrRestoreCharacter(shen_name name);
void AozrUpdateCamera(AozrShopWork *work);
void AozrBeepCallback(void *data);
int AozrExplainSelection(AozrShopWork *work);
void AozrExplainCallback(void *data);
int AozrMoveUp(AozrShopWork *work);
int AozrMoveDown(AozrShopWork *work);
int AozrMoveLeft(AozrShopWork *work);
int AozrMoveRight(AozrShopWork *work);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaAOZRInit();
void AreaAOZRMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaAOZRScriptDefinition;
} // namespace shenmue::s2::areas
#endif
