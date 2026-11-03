#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"

#define AREA_0276_FIRE_PROP SHEN_NAME4('a', '4', '1', '2')
#define AREA_0276_TRANSPARENT_PROP SHEN_NAME4('a', 'a', '8', '9')
#define AREA_0276_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0276_SRY SHEN_NAME4('S', 'R', 'Y', '_')
#define AREA_0276_WON SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0276_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0276_SGT SHEN_NAME4('S', 'G', 'T', '_')
#define AREA_0276_CHO SHEN_NAME4('C', 'H', 'O', '_')
#define AREA_0276_LIGHT SHEN_NAME4('l', '1', '0', '1')
#define AREA_0276_RETURN_FLAGGED SHEN_NAME4('0', '3', '6', '2')
#define AREA_0276_RETURN_CLEAR SHEN_NAME4('0', '2', '7', '8')
#define AREA_0276_A209 SHEN_NAME4('a', '2', '0', '9')
#define AREA_0276_A207 SHEN_NAME4('a', '2', '0', '7')
#define AREA_0276_A208 SHEN_NAME4('a', '2', '0', '8')
#define AREA_0276_A326 SHEN_NAME4('a', '3', '2', '6')
#define AREA_0276_A270 SHEN_NAME4('a', '2', '7', '0')
#define AREA_0276_A325 SHEN_NAME4('a', '3', '2', '5')
#define AREA_0276_A342 SHEN_NAME4('a', '3', '4', '2')
#define AREA_0276_A343 SHEN_NAME4('a', '3', '4', '3')
#define AREA_0276_AB19 SHEN_NAME4('a', 'b', '1', '9')
#define AREA_0276_AB18 SHEN_NAME4('a', 'b', '1', '8')
#define AREA_0276_AA23 SHEN_NAME4('a', 'a', '2', '3')
#define AREA_0276_A300 SHEN_NAME4('a', '3', '0', '0')
#define AREA_0276_A067 SHEN_NAME4('a', '0', '6', '7')
#define AREA_0276_A393 SHEN_NAME4('a', '3', '9', '3')
#define AREA_0276_A394 SHEN_NAME4('a', '3', '9', '4')
#define AREA_0276_A395 SHEN_NAME4('a', '3', '9', '5')
typedef struct Area0276Parameter {
    HLibTask *self;
    short selection, state, loadState, characterIndex;
} Area0276Parameter;
typedef struct Area0276AuthorParameter {
    Area0276Parameter *parent;
    short frame;
} Area0276AuthorParameter;
extern shen_name area0276Characters[];
void Start0276(void);
void Update0276(void *data);
void Load0276Characters(void *data);
void Release0276Characters(void *data);
void Queue0276Author(Area0276Parameter *parent);
void Update0276FirstAuthor(void *data);
void Update0276SecondAuthor(void *data);
void Setup0276Environment(void);
void Cleanup0276(Area0276Parameter *parent);
void Prepare0276Characters(void);
void Restore0276Characters(void);
void Prepare0276FirstScene(void);
void Restore0276FirstScene(void);
void Hide0276IndoorProps(void);
void Show0276IndoorProps(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0276_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0276_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0276_FIRE_PROP{SHEN_NAME(a412)}, AREA_0276_TRANSPARENT_PROP{SHEN_NAME(aa89)};
inline constexpr shen_name AREA_0276_REN{SHEN_NAME(REN_)}, AREA_0276_SRY{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_0276_SGT{SHEN_NAME(SGT_)}, AREA_0276_CHO{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0276_LIGHT{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0276_RETURN_FLAGGED{SHEN_NAME(0362)};
inline constexpr shen_name AREA_0276_RETURN_CLEAR{SHEN_NAME(0278)};
inline constexpr shen_name AREA_0276_CHARACTERS[]{
    SHEN_NAME(SRY_), SHEN_NAME(WON_), SHEN_NAME(REN_), SHEN_NAME(JOP_), SHEN_NAME(CHO_), SHEN_NAME(SGT_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0276_INITIAL_PROPS[]{SHEN_NAME(a209), SHEN_NAME(a207), SHEN_NAME(a208), SHEN_NAME(a326), SHEN_NAME(a270), SHEN_NAME(a325)};
inline constexpr shen_name AREA_0276_INDOOR_PROPS[]{SHEN_NAME(a342), SHEN_NAME(a343), SHEN_NAME(ab19), SHEN_NAME(ab18), SHEN_NAME(aa23)};
inline constexpr shen_name AREA_0276_FIRST_PROPS[]{
    SHEN_NAME(a300), SHEN_NAME(a067), SHEN_NAME(a393), SHEN_NAME(a394), SHEN_NAME(a395), SHEN_NAME(a412), SHEN_NAME(aa89)};
struct Area0276Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::int16_t loadState;
    std::int16_t characterIndex;
};
struct Area0276AuthorParameter {
    std::int16_t frame;
    std::byte reserved02[6];
    Area0276Parameter *parent;
};
static_assert(sizeof(Area0276Parameter) == 16 && sizeof(Area0276AuthorParameter) == 16);
static_assert(offsetof(Area0276Parameter, selection) == 8 && offsetof(Area0276Parameter, state) == 10);
static_assert(offsetof(Area0276AuthorParameter, frame) == 0 && offsetof(Area0276AuthorParameter, parent) == 8);
void Initialize0276();
void Start0276(std::uintptr_t argument);
void Load0276Characters(void *data);
void Update0276(void *data);
void Update0276FirstAuthor(void *data);
void Update0276SecondAuthor(void *data);
void Setup0276Environment();
void Prepare0276Characters();
void Restore0276Characters();
void Prepare0276FirstScene();
void Restore0276FirstScene();
void Queue0276Author(Area0276Parameter *parent);
void Cleanup0276(Area0276Parameter *parent);
extern const ScriptAreaDefinition area0276ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
