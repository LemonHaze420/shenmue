#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0529_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0529_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0529_a287{SHEN_NAME(a287)};
inline constexpr shen_name AREA_0529_a288{SHEN_NAME(a288)};
inline constexpr shen_name AREA_0529_a636{SHEN_NAME(a636)};
inline constexpr shen_name AREA_0529_CHARACTERS[]{AREA_0529_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0529_MOTION[] = "0529A.MOT", AREA_0529_CONVERSATION[] = "0529";
struct AuthorViewerFrameTaskParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(AuthorViewerFrameTaskParameter) == 16);
static_assert(offsetof(AuthorViewerFrameTaskParameter, currentFrame) == 8);
void Initialize0529();
void Start0529(std::uintptr_t argument);
void Update0529(void *taskData);
void Cleanup0529(AuthorViewerTaskParameter *parent);
std::int64_t Queue0529Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0529Author(AuthorViewerFrameTaskParameter *child);
void Update0529AuthorTask(void *taskData);
void Prepare0529Characters();
void Restore0529Characters();
void Setup0529FireEffects();
extern const ScriptAreaDefinition area0529ScriptDefinition;

} // namespace shenmue::s2::areas
