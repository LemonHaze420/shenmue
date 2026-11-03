#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0273.hpp"

#define AREA_0362_KUN SHEN_NAME4('K', 'U', 'N', '_')
#define AREA_0362_PROP_A047 SHEN_NAME4('a', '0', '4', '7')
#define AREA_0362_PROP_AA76 SHEN_NAME4('a', 'a', '7', '6')
#define AREA_0362_LIGHT SHEN_NAME4('l', '0', '0', '1')
#define AREA_0362_DESTINATION SHEN_NAME4('0', '2', '7', '8')
extern const shen_name area0362Characters[];
typedef struct Area0362AuthorParameter {
    Area0273Parameter *parent;
    short frame;
} Area0362AuthorParameter;
void Start0362(void);
void Update0362(void *taskData);
void Queue0362Author(Area0273Parameter *parent);
void Update0362Author(void *taskData);
void Setup0362(void);
void Cleanup0362(Area0273Parameter *parent);
void Prepare0362Characters(void);
void Restore0362Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/area_0273.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0362_KUN{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0362_PROP_A047{SHEN_NAME(a047)}, AREA_0362_PROP_AA76{SHEN_NAME(aa76)}, AREA_0362_LIGHT{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0362_DESTINATION{SHEN_NAME(0278)};
inline constexpr shen_name AREA_0362_CHARACTERS[]{AREA_0362_KUN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0362_CONVERSATION[] = "0362";
struct Area0362AuthorParameter {
    Area0273Parameter *parent;
    std::uint16_t frame;
};
static_assert(sizeof(Area0362AuthorParameter) == 0x10 && offsetof(Area0362AuthorParameter, frame) == 8);
void Initialize0362();
void Start0362(std::uintptr_t argument);
void Update0362(void *taskData);
void Setup0362();
void Cleanup0362(Area0273Parameter *parent);
std::int64_t Update0362Author(Area0362AuthorParameter *child);
void Update0362AuthorTask(void *taskData);
void Prepare0362Characters();
void Restore0362Characters();
extern const ScriptAreaDefinition area0362ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
