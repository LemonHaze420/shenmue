#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"

#define AREA_0505_SIN_ SHEN_NAME4('S', 'I', 'N', '_')
#define AREA_0505_BN0_ SHEN_NAME4('B', 'N', '0', '_')
#define AREA_0505_RETURN SHEN_NAME4('K', 'R', 'M', '3')
typedef struct Area0505AuthorParameter {
    AreaAuthorTaskParameter *parent;
    short currentFrame;
} Area0505AuthorParameter;
extern const shen_name area0505Characters[];
void Start0505(void);
void Update0505(void *taskData);
void Queue0505Author(AreaAuthorTaskParameter *parent);
void Update0505Author(void *taskData);
void Setup0505Waves(void);
void Cleanup0505(AreaAuthorTaskParameter *parent);
void Prepare0505Characters(void);
void Restore0505Characters(void);
void PrepareAuthorCharacterDisplay(shen_name character);
void EnableCharDisplayAndSetMotionworkData(shen_name character);
void Update0505Waves(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0505_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0505_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0505_BN0_{SHEN_NAME(BN0_)};
inline constexpr shen_name AREA_0505_CHARACTERS[]{AREA_0505_SIN_, AREA_0505_BN0_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0505_MOTION[] = "0505A.MOT", AREA_0505_CONVERSATION[] = "0505";
inline constexpr shen_name AREA_0505_RETURN{SHEN_NAME(KRM3)};
struct Area0505AuthorParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0505AuthorParameter) == 16);
void Setup0505Waves();
void Update0505Waves(void *);
void Initialize0505();
void Start0505(std::uintptr_t argument);
void Update0505(void *taskData);
void Cleanup0505(AuthorViewerTaskParameter *parent);
std::int64_t Update0505Author(Area0505AuthorParameter *child);
void Update0505AuthorTask(void *taskData);
void Prepare0505Characters();
extern const ScriptAreaDefinition area0505ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
