#include "s2/scene_data.hpp"

#include "s2/map_control.hpp"
#define SHENMUE_SCENE_DIRECTORY "SCENE"

#include <array>
#include <cstdio>
#define SHENMUE_PATH_DATA directory.data()
#define SHENMUE_PATH_CAPACITY directory.size()
#define SHENMUE_PATH_SNPRINTF std::snprintf
#define SHENMUE_PATH_NULL nullptr

namespace shenmue { namespace s2 {


// @verified - d
const char *Filepath_Gen(FilepathGenMode mode) {
    static std::array<char, 0x40> directory{};
    const char *area = reinterpret_cast<const char *>(&g_currentMap.name);
    switch (mode) {
    case FilepathGenMode::SceneDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, SHENMUE_SCENE_DIRECTORY "/%02d/%.4s", g_actNumber, area);
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::MiscDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, "MISC");
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::SoundDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, SHENMUE_SCENE_DIRECTORY "/%02d/SOUND", g_actNumber);
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::StreamDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, SHENMUE_SCENE_DIRECTORY "/%02d/STREAM", g_actNumber);
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::SpriteDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, "SPRITE");
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::ItemDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, "MODEL/ITEM");
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::CharaDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, SHENMUE_SCENE_DIRECTORY "/%02d/MODEL/CHARA", g_actNumber);
        return SHENMUE_PATH_DATA;
    case FilepathGenMode::PackDirectory:
        SHENMUE_PATH_SNPRINTF(SHENMUE_PATH_DATA, SHENMUE_PATH_CAPACITY, SHENMUE_SCENE_DIRECTORY "/%02d/PACK", g_actNumber);
        return SHENMUE_PATH_DATA;
    }
    return SHENMUE_PATH_NULL;
}

} } // namespace shenmue::s2

#undef SHENMUE_SCENE_DIRECTORY

#undef SHENMUE_PATH_DATA
#undef SHENMUE_PATH_CAPACITY
#undef SHENMUE_PATH_SNPRINTF
#undef SHENMUE_PATH_NULL
