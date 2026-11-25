#pragma once

#include "s2/author_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#define AUTHOR_FILE_STORAGE SHEN_NAME4('S', 'C', 'I', 'F')
static const char AUTHOR_REPLACEMENT_WARNING[67] = "****** Warning : Already set or running authoring data[%d]. ******";
void authDestroySubtitleConverseData(AuthorController *control);
void SCMS_Enqueue(AuthorController *control);
void UnloadAuthorTableSlot(std::int32_t index);
void LoadAuthorTableSlot(std::int32_t index, const char *filename, const char *directory);
void SetAuthorTableFile(std::int32_t index, const char *filename, const char *directory);
std::int32_t probeSceneById(std::int32_t id);
bool FindAuthor(shen_name id);

} } // namespace shenmue::s2

#endif
