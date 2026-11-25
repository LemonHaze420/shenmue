#include "s2/scene_data.hpp"

#if SHENMUE_IS_DC
#include "s2/scene_state_dc.hpp"
#define SHENMUE_SCENE_DIRECTORY "/SCENE"
#else
#include "s2/map_control.hpp"
#define SHENMUE_SCENE_DIRECTORY "SCENE"
#endif

#if SHENMUE_S2_GCC295
#include <stdio.h>
#include <string.h>
#define SHENMUE_PATH_DATA directory
#define SHENMUE_PATH_CAPACITY sizeof(directory)
#define SHENMUE_PATH_SNPRINTF(buffer, capacity, args...) ::sprintf(buffer, args)
#define SHENMUE_PATH_NULL 0
#else
#include <array>
#include <cstdio>
#define SHENMUE_PATH_DATA directory.data()
#define SHENMUE_PATH_CAPACITY directory.size()
#define SHENMUE_PATH_SNPRINTF std::snprintf
#define SHENMUE_PATH_NULL nullptr
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC
std::int32_t g_actNumber __attribute__((section(".data.dc_scene_act"))) = 0;
#if SHENMUE_S2_GCC295
shen_name g_currentMap __attribute__((section(".data.dc_scene_area")));
#else
shen_name g_currentMap __attribute__((section(".data.dc_scene_area"))){};
#endif
std::int32_t g_entryPoint __attribute__((section(".data.dc_scene_entry"))) = 0;
#endif

// @verified - d
#if SHENMUE_S2_GCC295
const char *Filepath_Gen(FilepathGenMode::Type mode) {
#else
const char *Filepath_Gen(FilepathGenMode mode) {
#endif
#if SHENMUE_S2_GCC295
    static char directory[0x40] = {};
    shen_name area[2];
    switch (mode) {
    case FilepathGenMode::SceneDirectory:
        area[0] = g_currentMap;
        area[1] = 0;
        ::sprintf(directory, SHENMUE_SCENE_DIRECTORY "/%02d/%s", g_actNumber, reinterpret_cast<char *>(area));
        break;
    case FilepathGenMode::MiscDirectory:
        ::strcpy(directory, "MISC");
        break;
    case FilepathGenMode::SoundDirectory:
        ::sprintf(directory, SHENMUE_SCENE_DIRECTORY "/%02d/SOUND", g_actNumber);
        break;
    case FilepathGenMode::StreamDirectory:
        ::sprintf(directory, SHENMUE_SCENE_DIRECTORY "/%02d/STREAM", g_actNumber);
        break;
    case FilepathGenMode::SpriteDirectory:
        ::strcpy(directory, "SPRITE");
        break;
    case FilepathGenMode::ItemDirectory:
        ::strcpy(directory, "MODEL/ITEM");
        break;
    case FilepathGenMode::CharaDirectory:
        ::sprintf(directory, SHENMUE_SCENE_DIRECTORY "/%02d/MODEL/CHARA", g_actNumber);
        break;
    case FilepathGenMode::PackDirectory:
        ::sprintf(directory, SHENMUE_SCENE_DIRECTORY "/%02d/PACK", g_actNumber);
        break;
    default:
        return 0;
    }
    return directory;
#else
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
#endif
}

} // namespace s2
} // namespace shenmue

#undef SHENMUE_SCENE_DIRECTORY

#undef SHENMUE_PATH_DATA
#undef SHENMUE_PATH_CAPACITY
#undef SHENMUE_PATH_SNPRINTF
#undef SHENMUE_PATH_NULL
