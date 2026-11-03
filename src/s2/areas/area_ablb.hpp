#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"

#define ABLB_NEXT_AREA SHEN_NAME4('0', '0', '5', '5')
#define ABLB_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define ABLB_H01 SHEN_NAME4('H', '0', '1', '_')
#define ABLB_H02 SHEN_NAME4('H', '0', '2', '_')
#define ABLB_H03 SHEN_NAME4('H', '0', '3', '_')
#define ABLB_H04 SHEN_NAME4('H', '0', '4', '_')
#define ABLB_H05 SHEN_NAME4('H', '0', '5', '_')
#define ABLB_H06 SHEN_NAME4('H', '0', '6', '_')
#define ABLB_HVA SHEN_NAME4('H', 'V', 'A', '_')
#define ABLB_TGI SHEN_NAME4('T', 'G', 'I', '_')
#define ABLB_SNC SHEN_NAME4('S', 'N', 'C', '_')
#define ABLB_SAM SHEN_NAME4('S', 'A', 'M', '_')
#define ABLB_LLY SHEN_NAME4('L', 'L', 'Y', '_')
#define ABLB_CLJ SHEN_NAME4('C', 'L', 'J', '_')
#define ABLB_ENF SHEN_NAME4('E', 'N', 'F', '_')
#define ABLB_TYZ SHEN_NAME4('T', 'Y', 'Z', '_')
#define ABLB_A073 SHEN_NAME4('a', '0', '7', '3')
#define ABLB_A417 SHEN_NAME4('a', '4', '1', '7')
#define ABLB_AA50 SHEN_NAME4('a', 'a', '5', '0')
#define ABLB_A014 SHEN_NAME4('a', '0', '1', '4')
#define ABLB_DEAD SHEN_NAME4('D', 'E', 'A', 'D')
#define ABLB_LIGHT_ALLEY SHEN_NAME4('l', '0', '0', '1')
#define ABLB_LIGHT_FIGHT SHEN_NAME4('l', '1', '0', '1')
#define ABLB_LIGHT_OUTRO SHEN_NAME4('l', '2', '0', '1')
#define ABLB_LIGHT_OUTRO_OVERLAY SHEN_NAME4('l', '2', '0', '2')
#define ABLB_DEBUG_ID 3
#define ABLB_OBJECT_COUNT 13
typedef struct AblbWork {
    HLibTask *task;
    int field04, field08, field0c;
    char reserved10[0x24];
    unsigned int phase;
    int field38, field3c;
    char reserved40[0x3c];
    int npcBatchCount;
    EventReadCharactersWork *characters;
    int fadeOutFrames, fadeInFrames;
    char reserved8c[4];
    unsigned char reserved90, delay, entry;
    unsigned char reserved93;
    int elapsed;
    int colours[4][6];
    signed char fadeOutSeconds, fadeInSeconds;
    char reservedfa[2];
    unsigned int skyColour, cloudColour;
    float colourBlend[16];
    unsigned int outroFrames;
} AblbWork;
typedef struct AblbAuthorWork {
    HLibTask *task;
    int reserved04;
    short author, phase;
} AblbAuthorWork;
typedef struct AblbAuthorLoopWork {
    AblbAuthorWork *parent;
    short frame;
} AblbAuthorLoopWork;
extern const shen_name ablbCharacters[];
extern const char ablbPackage[], ablbFirstCutscene[];
void ABLB_main_func(void *argument);
void QueueAuthor(unsigned char author);
void AuthorCallback(void *data);
void QueueAuthorLoop(AblbAuthorWork *work);
void SelectAuthorPackage(int author);
void AuthorLoopCallback(void *data);
void PrepareAuthor(void);
void FinishAuthor(AblbAuthorWork *work);
void HideAuthoredCharacters(void);
void ShowAuthoredCharacters(void);
void HideAuthoredCharacter(shen_name character);
void ShowAuthoredCharacter(shen_name character);
void HideDecoration(shen_name character);
void ShowDecoration(shen_name character);
void AlleyBattleCallback(void *data);
void SetupBattle(AblbWork *work);
void BeginFight(AblbWork *work);
void AddGeneratorPoints(void);
void AblbDebugExitCallback(void *data);
void ExitAlleyBattle(AblbWork *work);
void SaveWeatherColours(AblbWork *work);
void BlendWeatherColours(AblbWork *work);
void SetBattleCharacterVisible(shen_name character, int visible);
void OutroVoices(void);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct AlleyBattleWork {
    HLib::Task *task;
    std::uint64_t field08;
    std::int32_t field10;
    std::byte reserved14[0x24];
    std::uint32_t phase;
    std::int32_t field3c, field40;
    std::byte reserved44[0x3c];
    std::int32_t npcBatchCount;
    std::uint32_t reserved84;
    EventReadCharactersWork *characters;
    std::int32_t fadeOutFrames, fadeInFrames;
    std::byte reserved98[8];
    std::uint8_t reservedA0, delay, entry, reservedA3;
    std::uint32_t elapsed;
    std::int32_t colours[4][6];
    std::int8_t fadeOutSeconds, fadeInSeconds;
    std::byte reserved10a[2];
    std::uint32_t skyColour, cloudColour;
    float colourBlend[16];
    std::uint32_t outroFrames;
};
static_assert(sizeof(AlleyBattleWork) == 0x158);
static_assert(offsetof(AlleyBattleWork, phase) == 0x38);
static_assert(offsetof(AlleyBattleWork, characters) == 0x88);
static_assert(offsetof(AlleyBattleWork, colours) == 0xa8);
static_assert(offsetof(AlleyBattleWork, colourBlend) == 0x114);
static_assert(offsetof(AlleyBattleWork, outroFrames) == 0x154);
void ABLB_init_func();
void ABLB_main_func(std::uintptr_t);
void AlleyBattleCallback(void *);
extern const ScriptAreaDefinition areaABLBScriptDefinition;
} // namespace shenmue::s2
#endif
