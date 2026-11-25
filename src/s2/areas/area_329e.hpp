#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_329E SHEN_NAME4('3', '2', '9', 'E')
#define AREA_329E_TASK SHEN_NAME4('E', 'V', 'N', 'T')
#define AREA_329E_AUTHOR SHEN_NAME4('0', '3', '2', '9')
#define AREA_329E_RETURN_MAP SHEN_NAME4('Q', 'R', '0', '0')
#define AREA_329E_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_329E_GAI SHEN_NAME4('G', 'A', 'I', '_')
#define AREA_329E_PROP0 SHEN_NAME4('a', '1', '0', '2')
#define AREA_329E_PROP1 SHEN_NAME4('a', '1', '0', '3')
#define AREA_329E_NPC_AREA SHEN_NAME4('A', 'N', 'P', '_')
typedef struct Area329EParameter {
    HLibTask *self;
    short selection, state;
} Area329EParameter;
typedef struct Area329EAuthorParameter {
    Area329EParameter *parent;
    short frame;
} Area329EAuthorParameter;
extern shen_name AREA_329E_CHARACTERS[];
void Start329E(void);
void Update329E(void *);
void Queue329EAuthor(Area329EParameter *);
void Update329EAuthor(void *);
void Setup329EScene(void);
void Cleanup329E(Area329EParameter *);
void Prepare329ECharacters(void);
void Restore329ECharacters(void);
void Prepare329ECharacter(shen_name);
void Restore329ECharacter(shen_name);
void Hide329EProp(shen_name);
void Show329EProp(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_329E_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_329E_AUTHOR{SHEN_NAME(0329)};
inline constexpr shen_name AREA_329E_RETURN_MAP{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_329E_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_329E_GAI{SHEN_NAME(GAI_)};
inline constexpr shen_name AREA_329E_PROP0{SHEN_NAME(a102)};
inline constexpr shen_name AREA_329E_PROP1{SHEN_NAME(a103)};
inline constexpr shen_name AREA_329E_NPC_AREA{SHEN_NAME(ANP_)};
inline constexpr shen_name AREA_329E_CHARACTERS[]{AREA_329E_GAI, shen_name{0xffffffffU}};
struct Area329EParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
struct Area329EAuthorParameter {
    Area329EParameter *parent;
    std::int32_t frame;
    std::byte reserved0c[4];
};
static_assert(sizeof(Area329EParameter) == 16 && offsetof(Area329EParameter, state) == 10);
static_assert(sizeof(Area329EAuthorParameter) == 16);
void Initialize329E();
void Start329E(std::uintptr_t argument);
void Update329E(void *parameter);
void Cleanup329E(Area329EParameter *parent);
std::int64_t Update329EAuthor(Area329EAuthorParameter *child);
void Update329EAuthorTask(void *parameter);
void Prepare329ECharacters();
void Restore329ECharacters();
extern const ScriptAreaDefinition area329EScriptDefinition;
} // namespace shenmue::s2::areas

#endif
