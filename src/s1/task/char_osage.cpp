#include "s1/task/char.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"
#include "s1/face.hpp"
#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/effe.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"

#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/motion.hpp"
#include "s1/motion_registry.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/mbst.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/cloth_presets.hpp"
#include "s1/collision.hpp"
#include "s1/scene.hpp"
#include "s1/shadow_volume.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/input.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/wind.hpp"

#include "s1/timing.hpp"
#include <bit>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>
#include <limits>

#include "s1/task/char_components_internal.hpp"

namespace shenmue::s1 {

std::int32_t g_OSAG_Val_0[6]{};
std::int32_t g_OSAG_Val_1[2]{};
float g_OSAG_Val_2 = 0.075f;
float g_OSAG_Val = 0.0f;
namespace {

static_assert(sizeof(OsageWork) == 0x210);
static_assert(offsetof(OsageWork, character) == 0x08);
static_assert(offsetof(OsageWork, faceData) == 0x30);
static_assert(offsetof(OsageWork, deltaOverride) == 0x200);
static_assert(offsetof(OsageWork, distanceLimit) == 0x204);
static_assert(offsetof(OsageWork, renderChain) == 0x208);
static_assert(sizeof(OsageSegmentRuntime) == 0x180);
static_assert(offsetof(OsageSegmentRuntime, transform) == 0x1c);
static_assert(offsetof(OsageSegmentRuntime, worldEndpoint) == 0x74);
static_assert(offsetof(OsageSegmentRuntime, parent) == 0x110);
static_assert(offsetof(OsageSegmentRuntime, next) == 0x120);
bool g_osageState121SharedValid = false;
float g_osageState121PhaseX = 0.0F;
float g_osageState121PhaseY = 0.0F;
float g_osageState121TiltX = 20.0F;
float g_osageState121TiltZ = -30.0F;
bool g_osageState121RemainingFrame = false;

constexpr std::array<OsageCollisionProfile, 29> kOsageCollisionProfiles{{OsageCollisionProfile{{{3, 0, 13, 10}, {5, 0, 9, 8}, {2, 0, 8, 7}, {0, 0, -3, 7},
                                                                             {22, -1, -8, 10}, {22, -1, 8, 10}, {15, -3, 3, 12}, {15, -3, -3, 12},
                                                                             {-1, 0, -6, 8}, {0, 0, 5, 10}, {-1, 0, 6, 8}, {0, 0, -5, 10}, {4, 0, 0, 6},
                                                                             {24, 0, 0, 5}, {4, 0, 0, 6}, {24, 0, 0, 5}, {0, 0, 0, 5}, {30, 0, 0, 5},
                                                                             {0, 0, 0, 5}, {30, 0, 0, 5}, {-8, 3, -2, 10}, {-8, 3, 2, 10}, {8, 2, 4, 11},
                                                                             {8, 2, -4, 11}, {-3, 2, 0, 10}, {32, 1, 1, 8}, {-3, 2, 0, 10}, {32, 1, -1, 8}}},
    OsageCollisionProfile{{{3, 0, 16, 10}, {6, 0, 11, 7}, {2, 0, 12, 10}, {0, 0, -3, 8}, {24, 0, -11, 10}, {24, 0, 11, 10}, {3, -4, 4, 10}, {3, -4, -4, 10},
        {1, 0, -7, 9}, {6, 0, 4, 9}, {1, 0, 7, 9}, {6, 0, -4, 9}, {2, 0, 0, 8}, {22, 0, -1, 7}, {2, 0, 0, 8}, {22, 0, 1, 7}, {0, 0, 0, 7}, {26, -2, 0, 12},
        {0, 0, 0, 7}, {26, -2, 0, 12}, {1, 4, -5, 9}, {1, 4, 5, 9}, {13, 1, 4, 16}, {13, 1, -4, 16}, {-1, 1, 1, 16}, {30, -6, -1, 18}, {-1, 1, -1, 16},
        {30, -6, 1, 18}}},
    OsageCollisionProfile{{{1, 0, 17, 10}, {5, 0, 9, 6}, {2, 0, 11, 7}, {-1, 0, -4, 8}, {22, -1, -10, 11}, {22, -1, 10, 11}, {15, -3, 3, 12}, {15, -3, -3, 12},
        {0, 0, -6, 9}, {1, 0, 5, 10}, {0, 0, 6, 9}, {1, 0, -5, 10}, {3, 0, 1, 8}, {24, 0, 0, 6}, {3, 0, -1, 8}, {24, 0, 0, 6}, {0, 0, 0, 6}, {32, 0, 0, 5},
        {0, 0, 0, 6}, {32, 0, 0, 5}, {-8, 3, -2, 10}, {-8, 3, 2, 10}, {8, 2, 4, 11}, {8, 2, -4, 11}, {-3, 2, 0, 10}, {32, 1, 1, 8}, {-3, 2, 0, 10},
        {32, 1, -1, 8}}},
    OsageCollisionProfile{{{3, 0, 15, 13}, {9, 0, 7, 7}, {3, 0, 10, 8}, {0, 0, -4, 9}, {28, 1, -10, 14}, {28, 1, 10, 14}, {0, -2, 7, 14}, {0, -2, -7, 14},
        {-1, -1, -11, 12}, {4, 2, 5, 14}, {-1, -1, 11, 12}, {4, 2, -5, 14}, {2, 0, 1, 9}, {27, 1, 0, 8}, {2, 0, -1, 9}, {27, 1, 0, 8}, {0, 1, 0, 8},
        {31, 0, 1, 6}, {0, 1, 0, 8}, {31, 0, -1, 6}, {0, 2, -7, 14}, {0, 2, 7, 14}, {18, 0, 8, 15}, {18, 0, -8, 15}, {0, 0, 1, 15}, {40, -5, 0, 12},
        {0, 0, -1, 15}, {40, -5, 0, 12}}},
    OsageCollisionProfile{{{1, 0, 10, 13}, {5, 0, 2, 11}, {1, 0, 3, 12}, {0, 0, -19, 18}, {38, 2, -14, 24}, {38, 2, 14, 24}, {0, -1, 6, 38}, {0, -1, -6, 38},
        {3, 2, -14, 16}, {2, 6, 8, 22}, {3, 2, 14, 16}, {2, 6, -8, 22}, {4, 0, 0, 14}, {30, 1, -1, 11}, {4, 0, 0, 14}, {30, 1, 1, 11}, {4, -1, -1, 11},
        {38, 0, 3, 8}, {4, -1, 1, 11}, {38, 0, -3, 8}, {0, 2, -6, 38}, {0, 2, 6, 38}, {6, 4, 8, 38}, {6, 4, -8, 38}, {6, 0, 0, 22}, {46, 0, 0, 20},
        {6, 0, 0, 22}, {46, 0, 0, 20}}},
    OsageCollisionProfile{{{1, 0, 9, 9}, {4, 0, 4, 7}, {0, 0, 4, 7}, {0, 0, -8, 8}, {35, 0, -3, 8}, {35, 0, 3, 8}, {16, -3, 4, 11}, {16, -3, -4, 11},
        {-1, 1, -3, 10}, {0, -1, 6, 11}, {-1, -1, 3, 10}, {0, -1, -6, 11}, {2, 0, 1, 6}, {22, 0, 1, 6}, {2, 0, -1, 6}, {22, 0, -1, 6}, {0, 0, 0, 6},
        {29, 0, 0, 6}, {0, 0, 0, 6}, {29, 0, 0, 6}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
        {0, 0, 0, 1}}},
    OsageCollisionProfile{{{1, 0, 12, 11}, {4, 0, 4, 8}, {1, 0, 8, 8}, {1, 0, -7, 9}, {25, 0, -12, 14}, {25, 0, 12, 14}, {-1, 1, 7, 11}, {-1, -1, -7, 11},
        {-1, 0, -15, 10}, {4, 1, 5, 12}, {-1, 0, 15, 10}, {4, 1, -5, 12}, {2, 0, 1, 9}, {24, 0, 0, 9}, {2, 0, -1, 9}, {24, 0, 0, 9}, {0, 0, 0, 9},
        {32, -2, 0, 10}, {0, 0, 0, 9}, {32, -2, 0, 10}, {-1, 0, -6, 12}, {-1, 0, 6, 12}, {20, -2, 4, 18}, {20, -2, -4, 18}, {0, 1, 1, 14}, {34, -1, 2, 13},
        {0, 1, -1, 14}, {34, -1, -2, 13}}},
    OsageCollisionProfile{{{1, 0, 16, 10}, {4, 0, 10, 7}, {0, 0, 8, 6}, {0, 0, 0, 8}, {20, 1, -10, 10}, {20, 1, 10, 10}, {4, -1, 2, 11}, {4, -1, -2, 11},
        {0, 1, -8, 8}, {2, 1, 2, 10}, {0, 1, 8, 8}, {2, 1, -2, 10}, {3, -1, 3, 7}, {20, 0, 0, 7}, {3, -1, -3, 7}, {20, 0, 0, 7}, {0, 0, 0, 7}, {24, 0, 0, 6},
        {0, 0, 0, 7}, {24, 0, 0, 6}, {0, 1, -2, 11}, {0, 1, 2, 11}, {13, 1, 6, 11}, {13, 1, -6, 11}, {0, 1, 0, 12}, {26, 0, -2, 12}, {0, 1, 0, 12},
        {26, 0, 2, 12}}},
    OsageCollisionProfile{{{1, 0, 9, 10}, {4, 0, 4, 8}, {0, 0, 4, 7}, {0, 0, -8, 7}, {22, -1, -7, 10}, {22, -1, 7, 10}, {16, -4, 4, 10}, {16, -4, -4, 10},
        {-1, 0, -7, 8}, {0, 0, 6, 10}, {-1, 0, 7, 8}, {0, 0, -6, 10}, {2, 0, 1, 6}, {18, 0, 1, 6}, {2, 0, -1, 6}, {18, 0, -1, 6}, {0, 0, 0, 6}, {29, 0, 0, 6},
        {0, 0, 0, 6}, {29, 0, 0, 6}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}}},
    OsageCollisionProfile{{{4, 0, 14, 11}, {9, 0, 7, 6}, {2, 0, 8, 7}, {1, 0, -4, 8}, {23, -1, -8, 12}, {23, -1, 8, 12}, {1, -2, 7, 11}, {1, -2, -7, 11},
        {-2, 0, -7, 9}, {-2, 0, 6, 12}, {-2, 0, 7, 9}, {-2, 0, -6, 12}, {3, 0, 2, 7}, {24, 0, 1, 7}, {3, 0, -2, 7}, {24, 0, -1, 7}, {0, 0, 0, 6}, {27, 0, 0, 5},
        {0, 0, 0, 6}, {27, 0, 0, 5}, {0, 0, -4, 1}, {0, 0, 4, 1}, {4, 0, 4, 1}, {4, 0, -4, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}}},
    OsageCollisionProfile{{{1, 0, 14, 12}, {5, 0, 10, 8}, {1, 0, 9, 9}, {0, 0, -8, 12}, {23, -1, -12, 16}, {23, -1, 12, 16}, {-1, 1, 8, 15}, {-1, 1, -8, 15},
        {0, 0, -14, 11}, {2, 1, 4, 14}, {0, 0, 14, 11}, {2, 1, -4, 14}, {0, 0, 2, 10}, {24, 1, -1, 9}, {0, 0, -2, 10}, {24, 1, 1, 9}, {0, 1, -2, 9},
        {34, 1, 0, 7}, {0, 1, 2, 9}, {34, 1, 0, 7}, {-1, -2, -8, 15}, {-1, -2, 8, 15}, {20, -1, 7, 16}, {20, -1, -7, 16}, {0, 0, 1, 14}, {34, -2, 0, 10},
        {0, 1, -1, 14}, {34, -2, 0, 10}}},
    OsageCollisionProfile{{{1, 0, 13, 9}, {5, 0, 6, 5}, {2, 0, 9, 7}, {0, 0, -4, 7}, {21, 1, -11, 10}, {21, 1, 11, 10}, {0, -2, 6, 11}, {0, -2, -6, 11},
        {-1, 1, -8, 8}, {0, 1, 4, 9}, {-1, 1, 8, 8}, {0, 1, -4, 9}, {2, -1, 2, 6}, {20, 0, 0, 7}, {2, -1, -2, 6}, {20, 0, 0, 7}, {0, 0, 0, 7}, {26, 0, 0, 5},
        {0, 0, 0, 7}, {26, 0, 0, 5}, {0, 0, -4, 1}, {0, 0, 4, 1}, {4, 0, 4, 1}, {4, 0, -4, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}}},
    OsageCollisionProfile{
        {{1, 0, 17, 10}, {5, 0, 9, 6}, {2, 0, 11, 7}, {-1, 0, -4, 8}, {22, -1, -8, 10}, {22, -1, 8, 10}, {15, -3, 3, 12}, {15, -3, -3, 12}, {0, 0, -5, 8},
            {1, 0, 5, 10}, {0, 0, 5, 8}, {1, 0, -5, 10}, {3, 0, 1, 7}, {24, 0, 0, 6}, {3, 0, -1, 7}, {24, 0, 0, 6}, {0, 0, 0, 6}, {32, 0, 0, 5}, {0, 0, 0, 6},
            {32, 0, 0, 5}, {-8, 3, -2, 10}, {-8, 3, 2, 10}, {8, 2, 4, 11}, {8, 2, -4, 11}, {-3, 2, 0, 10}, {32, 1, 1, 8}, {-3, 2, 0, 10}, {32, 1, -1, 8}}},
    OsageCollisionProfile{
        {{3, 0, 13, 11}, {8, 0, 4, 6}, {2, 0, 8, 6}, {1, 0, -1, 6}, {25, -1, -13, 12}, {25, -1, 13, 12}, {1, -1, 6, 12}, {1, -1, -6, 12}, {-1, 0, -12, 11},
            {6, 0, 4, 12}, {-1, 0, 12, 11}, {6, 0, -4, 12}, {1, 1, 3, 8}, {24, 0, 0, 7}, {1, 1, -3, 8}, {24, 0, 0, 7}, {0, 0, 0, 7}, {33, 0, 0, 6},
            {0, 0, 0, 7}, {33, 0, 0, 6}, {0, 0, -4, 1}, {0, 0, 4, 1}, {4, 0, 4, 1}, {4, 0, -4, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}}},
    OsageCollisionProfile{
        {{2, 0, 13, 12}, {6, 0, 6, 8}, {1, 0, 8, 9}, {2, 0, -6, 10}, {26, 0, -14, 12}, {26, 0, 14, 12}, {0, -3, 5, 14}, {0, -3, -5, 14}, {0, 0, -12, 10},
            {4, 0, 4, 13}, {0, 0, 12, 10}, {4, 0, -4, 13}, {2, 0, 2, 9}, {24, 0, 0, 8}, {2, 0, -2, 9}, {24, 0, 0, 8}, {0, 0, 0, 8}, {32, 0, 0, 6}, {0, 0, 0, 8},
            {32, 0, 0, 6}, {0, 3, -7, 14}, {0, 3, 7, 14}, {20, 2, 9, 16}, {20, 2, -9, 16}, {0, 2, 0, 16}, {36, 0, -4, 16}, {0, 2, 0, 16}, {36, 0, 4, 16}}},
    OsageCollisionProfile{{{7, 0, 16, 12}, {11, 0, 8, 8}, {4, 0, 7, 7}, {-4, 0, -3, 8}, {27, 3, -10, 14}, {27, 3, 10, 14}, {0, 8, 2, 15}, {0, 8, -2, 15},
        {3, 1, -14, 10}, {1, 5, 3, 16}, {3, 1, 14, 10}, {1, 5, -3, 16}, {1, 0, 1, 9}, {28, 0, 0, 5}, {1, 0, -1, 9}, {28, 0, 0, 5}, {3, -1, 0, 5}, {33, 1, 2, 6},
        {3, -1, 0, 5}, {33, 1, -2, 6}, {0, -8, -3, 14}, {0, -8, 3, 14}, {17, -4, 6, 14}, {17, -4, -6, 14}, {0, -1, 1, 12}, {38, -1, 0, 8}, {0, -1, -1, 12},
        {38, -1, 0, 8}}},
    OsageCollisionProfile{{{2, 0, 11, 8}, {6, 0, 4, 5}, {2, 0, 8, 8}, {1, 0, -4, 6}, {16, 0, -7, 8}, {16, 0, 7, 8}, {1, -3, 6, 9}, {1, -3, -6, 9},
        {0, 0, -7, 7}, {1, 0, 2, 8}, {0, 0, 7, 7}, {1, 0, -2, 8}, {0, 0, 1, 6}, {15, 0, 0, 6}, {0, 0, -1, 6}, {15, 0, 0, 6}, {0, 0, 0, 6}, {18, 0, 0, 5},
        {0, 0, 0, 6}, {18, 0, 0, 5}, {0, 0, -4, 1}, {0, 0, 4, 1}, {4, 0, 4, 1}, {4, 0, -4, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}}},
    OsageCollisionProfile{
        {{3, 0, 11, 10}, {7, 0, 3, 6}, {2, 0, 4, 8}, {2, 0, -6, 10}, {26, 0, -14, 12}, {26, 0, 14, 12}, {0, -3, 5, 14}, {0, -3, -5, 14}, {0, 0, -12, 11},
            {1, 0, 4, 14}, {0, 0, 12, 11}, {1, 0, -4, 14}, {2, 0, 2, 9}, {24, 0, 0, 8}, {2, 0, -2, 9}, {24, 0, 0, 8}, {0, 0, 0, 8}, {32, 0, 0, 6}, {0, 0, 0, 8},
            {32, 0, 0, 6}, {0, 3, -7, 14}, {0, 3, 7, 14}, {20, 2, 9, 16}, {20, 2, -9, 16}, {0, 2, 0, 16}, {36, 0, -4, 16}, {0, 2, 0, 16}, {36, 0, 4, 16}}},
    OsageCollisionProfile{
        {{3, 0, 13, 10}, {5, 0, 9, 8}, {2, 0, 8, 7}, {0, 0, -3, 7}, {22, -1, -8, 12}, {22, -1, 8, 12}, {0, 2, 2, 19}, {0, 2, -2, 19}, {-1, 0, -6, 8},
            {0, 0, 5, 10}, {-1, 0, 6, 8}, {0, 0, -5, 10}, {4, 0, 0, 6}, {24, 0, 0, 9}, {4, 0, 0, 6}, {24, 0, 0, 9}, {0, 0, 0, 9}, {30, 0, 0, 9}, {0, 0, 0, 9},
            {30, 0, 0, 9}, {0, -2, -2, 20}, {0, -2, 2, 20}, {25, 2, 4, 20}, {25, 2, -4, 20}, {0, 0, 5, 20}, {40, -3, 2, 22}, {0, 0, -5, 20}, {40, -3, -2, 22}}},
    OsageCollisionProfile{{{1, 0, 14, 11}, {4, 0, 7, 8}, {0, 0, 8, 9}, {1, 0, -8, 12}, {25, 0, -10, 15}, {25, 0, 10, 15}, {0, -3, 4, 18}, {0, -3, -4, 18},
        {-1, 0, -12, 11}, {0, 1, 4, 16}, {-1, 0, 12, 11}, {0, 1, -4, 16}, {1, 0, 1, 9}, {24, -1, -3, 8}, {1, 0, -1, 9}, {24, -1, 3, 8}, {0, -1, -3, 8},
        {30, 0, 0, 8}, {0, -1, 3, 8}, {30, 0, 0, 8}, {0, 3, -4, 18}, {0, 3, 4, 18}, {18, 2, 6, 17}, {18, 2, -6, 17}, {0, 2, 2, 17}, {44, -1, -2, 16},
        {0, 2, -2, 17}, {44, -1, 2, 16}}},
    OsageCollisionProfile{{{9, 0, 9, 9}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
        {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
        {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}}},
    OsageCollisionProfile{{{0, 0, 17, 11}, {5, 0, 11, 7}, {2, 0, 11, 8}, {-1, 0, -3, 10}, {26, 0, -10, 11}, {26, 0, 10, 11}, {0, -3, 4, 10}, {0, -3, -4, 10},
        {1, 1, -9, 7}, {6, 1, 5, 9}, {1, 1, 9, 7}, {6, 1, -5, 9}, {4, -1, 2, 7}, {26, 0, 0, 6}, {4, -1, -2, 7}, {26, 0, 0, 6}, {0, 0, 0, 6}, {26, 0, 0, 4},
        {0, 0, 0, 6}, {26, 0, 0, 4}, {0, 3, -4, 10}, {0, 3, 4, 10}, {22, 3, 3, 14}, {22, 3, -3, 14}, {0, 0, 0, 1}, {20, 0, 0, 1}, {0, 0, 0, 1}, {20, 0, 0, 1}}},
    OsageCollisionProfile{{{6, 0, 5, 9}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
        {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
        {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}}},
    OsageCollisionProfile{{{2, 0, 11, 10}, {6, 0, 4, 5}, {2, 0, 8, 8}, {1, 0, -4, 6}, {16, 0, -7, 8}, {16, 0, 7, 8}, {1, -3, 6, 9}, {1, -3, -6, 9},
        {0, 0, -7, 7}, {1, 0, 2, 8}, {0, 0, 7, 7}, {1, 0, -2, 8}, {0, 0, 1, 6}, {15, 0, 0, 6}, {0, 0, -1, 6}, {15, 0, 0, 6}, {0, 0, 0, 6}, {18, 0, 0, 5},
        {0, 0, 0, 6}, {18, 0, 0, 5}, {0, 0, -4, 1}, {0, 0, 4, 1}, {4, 0, 4, 1}, {4, 0, -4, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}, {0, 0, 0, 1}, {4, 0, 0, 1}}},
    OsageCollisionProfile{{{1, 0, 16, 11}, {4, 0, 9, 8}, {1, 0, 8, 10}, {1, 0, -4, 10}, {25, 0, -12, 13}, {25, 0, 12, 13}, {8, -2, 7, 12}, {8, -2, -7, 12},
        {0, 0, -14, 10}, {5, 1, 4, 13}, {0, 0, 14, 10}, {5, 1, -4, 13}, {2, 0, 1, 9}, {20, 0, 1, 8}, {2, 0, -1, 9}, {20, 0, -1, 8}, {0, 0, 0, 7}, {35, 0, 0, 6},
        {0, 0, 0, 7}, {35, 0, 0, 6}, {-2, 2, -5, 11}, {-2, 2, 5, 11}, {16, 0, 6, 12}, {16, 0, -6, 12}, {0, 0, 1, 11}, {34, 1, 0, 10}, {0, 0, -1, 11},
        {34, 1, 0, 10}}},
    OsageCollisionProfile{
        {{1, 0, 12, 11}, {4, 0, 4, 9}, {2, 0, 6, 9}, {2, 0, -5, 8}, {23, 0, -8, 14}, {23, 0, 8, 14}, {0, -3, 5, 16}, {0, -3, -5, 16}, {-3, 0, -7, 1},
            {-2, 1, 5, 1}, {-3, 0, 7, 1}, {-2, 1, -5, 1}, {3, 0, 2, 9}, {25, 1, 0, 7}, {3, 0, -2, 9}, {25, 1, 0, 7}, {0, 1, 0, 8}, {30, 0, 1, 6}, {0, 1, 0, 8},
            {30, 0, -1, 6}, {0, 3, -8, 16}, {0, 3, 5, 16}, {18, 0, 6, 16}, {18, 0, -6, 16}, {0, 1, -6, 14}, {34, -2, 0, 11}, {0, 1, 10, 14}, {34, -2, 0, 11}}},
    OsageCollisionProfile{{{1, 0, 14, 9}, {5, 0, 8, 6}, {1, 0, 9, 6}, {-1, 0, -3, 7}, {22, 1, -8, 10}, {22, 1, 8, 10}, {0, -1, 3, 10}, {0, -1, -3, 10},
        {-1, 0, -6, 8}, {0, 1, 4, 10}, {-1, 0, 6, 8}, {0, 1, -4, 10}, {5, 0, 1, 7}, {26, 0, 0, 6}, {5, 0, -1, 7}, {26, 0, 0, 6}, {0, 0, 0, 6}, {30, 0, 0, 4},
        {0, 0, 0, 6}, {30, 0, 0, 4}, {0, 0, -10, 1}, {0, 0, 10, 1}, {10, 0, 10, 1}, {10, 0, -10, 1}, {0, 0, 0, 1}, {20, 0, 0, 1}, {0, 0, 0, 1}, {20, 0, 0, 1}}},
    OsageCollisionProfile{
        {{4, 0, 13, 11}, {8, 0, 5, 7}, {3, 0, 10, 9}, {1, 0, -4, 8}, {25, -1, -10, 13}, {25, -1, 10, 13}, {0, -1, 7, 14}, {0, -1, -7, 14}, {0, -2, -9, 11},
            {2, 1, 5, 13}, {0, -2, 9, 11}, {2, 1, -5, 13}, {3, 1, 1, 9}, {27, 0, 0, 8}, {3, 1, -1, 9}, {27, 0, 0, 8}, {0, 0, 0, 8}, {31, 0, 0, 6}, {0, 0, 0, 8},
            {31, 0, 0, 6}, {0, 1, -7, 14}, {0, 1, 7, 14}, {18, 0, 8, 15}, {18, 0, -8, 15}, {0, 0, 1, 15}, {40, 0, 0, 11}, {0, 0, -1, 15}, {40, 0, 0, 11}}},
    OsageCollisionProfile{{{0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {40, -20, -10, 25}, {40, -20, 10, 25}, {10, -15, 10, 25}, {10, -15, -10, 25},
        {0, -5, -10, 13}, {0, -5, 5, 13}, {0, -5, 10, 13}, {0, -5, -5, 13}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1},
        {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}}}}};

const OsageCollisionProfile &OsageCollisionProfileForImage(std::uint32_t imageName) {
    if (imageName == OsageImageCode('L', 'J', 'O', 'J'))
        return kOsageCollisionProfiles[12];
    if (imageName == OsageImageCode('M', 'J', 'O', 'J'))
        return kOsageCollisionProfiles[2];

    switch (imageName & 0x00ffffffU) {
    case OsageImageCode('E', 'Y', 'S'):
    case OsageImageCode('E', 'M', 'M'):
    case OsageImageCode('J', 'Y', 'S'):
    case OsageImageCode('A', 'S', 'H'):
    case OsageImageCode('D', 'F', 'R'):
    case OsageImageCode('B', 'S', 'H'):
    case OsageImageCode('M', 'Z', 'N'):
    case OsageImageCode('N', 'G', 'H'):
    case OsageImageCode('O', 'R', 'F'):
    case OsageImageCode('P', 'Y', 'S'):
    case OsageImageCode('T', 'A', 'N'):
    case OsageImageCode('U', 'E', 'S'):
        return kOsageCollisionProfiles[0];
    case OsageImageCode('J', 'H', 'C'):
        return kOsageCollisionProfiles[1];
    case OsageImageCode('Z', 'A', 'K'):
        return kOsageCollisionProfiles[3];
    case OsageImageCode('G', 'O', 'T'):
        return kOsageCollisionProfiles[4];
    case OsageImageCode('X', 'P', 'H'):
    case OsageImageCode('Y', 'P', 'H'):
        return kOsageCollisionProfiles[5];
    case OsageImageCode('K', 'O', 'K'):
        return kOsageCollisionProfiles[6];
    case OsageImageCode('A', 'K', 'J'):
    case OsageImageCode('B', 'K', 'J'):
        return kOsageCollisionProfiles[7];
    case OsageImageCode('E', 'P', 'H'):
    case OsageImageCode('D', 'H', 'S'):
    case OsageImageCode('N', 'I', 'S'):
    case OsageImageCode('R', 'G', 'H'):
    case OsageImageCode('R', 'G', 'M'):
    case OsageImageCode('Y', 'H', 'S'):
    case OsageImageCode('X', 'I', 'S'):
        return kOsageCollisionProfiles[8];
    case OsageImageCode('H', 'I', 'M'):
        return kOsageCollisionProfiles[9];
    case OsageImageCode('B', 'K', 'G'):
        return kOsageCollisionProfiles[11];
    case OsageImageCode('O', 'A', 'K'):
    case OsageImageCode('S', 'H', 'T'):
    case OsageImageCode('Y', 'H', 'K'):
        return kOsageCollisionProfiles[12];
    case OsageImageCode('A', 'A', 'Y'):
    case OsageImageCode('S', 'C', 'A'):
        return kOsageCollisionProfiles[13];
    case OsageImageCode('K', 'U', 'F'):
    case OsageImageCode('D', 'U', 'F'):
    case OsageImageCode('B', 'U', 'F'):
    case OsageImageCode('C', 'U', 'F'):
        return kOsageCollisionProfiles[14];
    case OsageImageCode('C', 'G', 'H'):
        return kOsageCollisionProfiles[15];
    case OsageImageCode('A', 'I', 'S'):
        return kOsageCollisionProfiles[16];
    case OsageImageCode('E', 'M', 'A'):
        return kOsageCollisionProfiles[17];
    case OsageImageCode('H', 'I', 'S'):
    case OsageImageCode('G', 'M', 'A'):
        return kOsageCollisionProfiles[17];
    case OsageImageCode('M', 'H', 'H'):
    case OsageImageCode('N', 'H', 'H'):
    case OsageImageCode('T', 'H', 'H'):
    case OsageImageCode('Y', 'H', 'H'):
        return kOsageCollisionProfiles[18];
    case OsageImageCode('A', 'W', 'I'):
    case OsageImageCode('O', 'W', 'I'):
        return kOsageCollisionProfiles[19];
    case OsageImageCode('B', 'A', 'Y'):
        return kOsageCollisionProfiles[20];
    case OsageImageCode('I', 'Y', 'Y'):
        return kOsageCollisionProfiles[21];
    case OsageImageCode('U', 'Y', 'S'):
        return kOsageCollisionProfiles[22];
    case OsageImageCode('2', '6', 'X'):
        return kOsageCollisionProfiles[23];
    case OsageImageCode('N', 'E', 'R'):
        return kOsageCollisionProfiles[24];
    case OsageImageCode('E', 'Z', 'Z'):
        return kOsageCollisionProfiles[25];
    case OsageImageCode('A', 'W', 'T'):
        return kOsageCollisionProfiles[26];
    case OsageImageCode('S', 'O', 'T'):
    case OsageImageCode('Z', 'C', 'M'):
        return kOsageCollisionProfiles[27];
    case OsageImageCode('Y', 'I', 'S'):
        return kOsageCollisionProfiles[28];
    default:
        return kOsageCollisionProfiles[10];
    }
}

bool IsOsageNodeKind(std::uint16_t nodeKind) {
    return std::find(kOsageNodeKinds.begin(), kOsageNodeKinds.end() - 1, static_cast<std::int16_t>(nodeKind)) != kOsageNodeKinds.end() - 1;
}

Mt5RuntimeNode *OsageOffsetNode(Mt5RuntimeNode *node, std::int32_t offset) {
    if (!node || !offset)
        return nullptr;
    std::byte *const base = MT5_ModelBaseForNode(node);
    return base ? reinterpret_cast<Mt5RuntimeNode *>(base + offset) : nullptr;
}
} // namespace

Vector3 OsageMatrixPosition(const Matrix4x4 &matrix) {
    return {matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
}
namespace {



void ApplyOsageNodeLocalTransform(const Mt5RuntimeNode &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
}

float AdvanceEnvironmentRandomUnit();

OsageSegmentRuntime *AllocateOsageSegment(OsageWork &osage, Mt5RuntimeNode &node) {
    auto *const segment = static_cast<OsageSegmentRuntime *>(HLib_AllocHeapMemBlock(sizeof(OsageSegmentRuntime), SHEN_NAME(OSAG)));
    if (!segment || !g_pCurrentTaskData) {
        if (segment)
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(segment), 0);
        return nullptr;
    }
    std::memset(segment, 0, sizeof(*segment));
    segment->tag = SHEN_NAME(OSAG);
    segment->owner = &osage;
    segment->node = &node;
    segment->nodeKind = -1;
    const Vector3 position = OsageMatrixPosition(workTransformMatrix);
    if (position.z > 0.0F)
        segment->linkFlags |= 0x40U;
    if (position.x > 0.0F)
        segment->linkFlags |= 0x20U;
    segment->updateFlags = 0x80U;
    segment->transform = workTransformMatrix;
    segment->angleX = AdvanceEnvironmentRandomUnit() * 360.0F;
    segment->angleZ = AdvanceEnvironmentRandomUnit() * 360.0F;
    segment->next = reinterpret_cast<OsageSegmentRuntime *>(static_cast<std::uintptr_t>(osage.renderChain));
    osage.renderChain = static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(segment));

    HMDL_SetTemporaryTaskRelativeOffsets(&node, true);
    if (!HMDL_SetCycleDataForNode(&node, segment)) {
        HMDL_SetTemporaryTaskRelativeOffsets(&node, false);
        osage.renderChain = static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(segment->next));
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(segment), 0);
        return nullptr;
    }

    std::byte *const modelBase = MT5_ModelBaseForNode(&node);
    if (node.parentOffset && modelBase) {
        auto *const parentNode = reinterpret_cast<Mt5RuntimeNode *>(modelBase + node.parentOffset);
        auto *const parent = static_cast<OsageSegmentRuntime *>(HMDL_CycleDataForNode(parentNode));
        if (parent && parent->tag == SHEN_NAME(OSAG) && parent->node && parent->node->nodeKind == node.nodeKind) {
            segment->parent = parent;
            segment->linkFlags = static_cast<std::uint8_t>(parent->linkFlags + 1U);
            parent->child = segment;
        }
    }

    if (node.nodeKind == 131 && node.sceneBindingOffset && modelBase) {
        const std::byte *const binding = modelBase + node.sceneBindingOffset;
        std::int32_t recordsOffset = 0;
        std::int32_t recordCount = 0;
        std::memcpy(&recordsOffset, binding + 4, sizeof(recordsOffset));
        std::memcpy(&recordCount, binding + 8, sizeof(recordCount));
        if (recordsOffset && recordCount > 0 && static_cast<std::uint64_t>(recordCount) <= std::numeric_limits<std::size_t>::max() / 24U) {
            const std::size_t bytes = static_cast<std::size_t>(recordCount) * 24U;
            segment->auxiliaryData = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(bytes), SHEN_NAME(OSAG));
            if (segment->auxiliaryData) {
                std::memcpy(segment->auxiliaryData, modelBase + recordsOffset, bytes);
            }
        }
    }
    return segment;
}

// The bind-pose walk that gives every OSAGE node kind its segment. Each node applies its translation and its euler rotation in
// one step, without the node scale.
// @verified - p
void hmdlApplyOsage(OsageWork &osage, Mt5RuntimeNode *node) {
    while (node) {
        const bool hasSibling = node->siblingOffset != 0;
        if (hasSibling)
            PushCharacterMatrix();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        if (IsOsageNodeKind(node->nodeKind))
            AllocateOsageSegment(osage, *node);
        if (Mt5RuntimeNode *const child = OsageOffsetNode(node, node->childOffset)) {
            hmdlApplyOsage(osage, child);
        }
        if (!hasSibling)
            break;
        PopCharacterMatrix();
        node = OsageOffsetNode(node, node->siblingOffset);
    }
}
} // namespace

OsageWork *FindOsageWork(Character *character) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, SHEN_NAME(OSAG)) : nullptr;
    return component ? reinterpret_cast<OsageWork *>(component + 1) : nullptr;
}
namespace {



void OSAGE_Cleanup(Character *, void *payload) {
    auto *const osage = static_cast<OsageWork *>(payload);
    if (!osage)
        return;

    auto *segment = reinterpret_cast<OsageSegmentRuntime *>(static_cast<std::uintptr_t>(osage->renderChain));
    while (segment) {
        OsageSegmentRuntime *const next = segment->next;
        if (segment->node) {
            if (HMDL_TransformForNode(segment->node, segment->node->transformOffset) == &segment->transform) {
                segment->node->transformOffset = 0;
            }
            if (HMDL_CycleDataForNode(segment->node) == segment)
                HMDL_SetCycleDataForNode(segment->node, nullptr);
            HMDL_SetTemporaryTaskRelativeOffsets(segment->node, false);

            if (segment->auxiliaryData && segment->node->sceneBindingOffset) {
                std::byte *const modelBase = MT5_ModelBaseForNode(segment->node);
                if (modelBase) {
                    const std::byte *const binding = modelBase + segment->node->sceneBindingOffset;
                    std::int32_t recordsOffset = 0;
                    std::int32_t recordCount = 0;
                    std::memcpy(&recordsOffset, binding + 4, sizeof(recordsOffset));
                    std::memcpy(&recordCount, binding + 8, sizeof(recordCount));
                    if (recordsOffset && recordCount > 0 && static_cast<std::uint64_t>(recordCount) <= std::numeric_limits<std::size_t>::max() / 24U) {
                        std::memcpy(modelBase + recordsOffset, segment->auxiliaryData, static_cast<std::size_t>(recordCount) * 24U);
                    }
                }
            }
        }
        if (segment->auxiliaryData) {
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(segment->auxiliaryData), 0);
        }
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(segment), 0);
        segment = next;
    }
    osage->renderChain = 0;
}


void OSAGE_NoOp(Character *, void *) {
}

constexpr std::array<OsageReferenceAngleConfig, 5> kOsageReferenceAngleConfigs{{
    {47, {1.0F, 0.0F, 0.0F}, {1.0F, 0.0F, -1.0F}, {1.0F, 0.0F, 1.0F}},
    {2, {0.0F, -1.0F, 0.0F}, {-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}},
    {20, {0.0F, 1.0F, 0.0F}, {1.0F, 1.0F, 0.0F}, {-1.0F, 1.0F, 0.0F}},
    {10, {1.0F, 0.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F}},
    {16, {1.0F, 0.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F}},
}};

Vector3 TransformOsagePoint(const Matrix4x4 &transform, const Vector3 &point) {
    const Matrix4x4 saved = workTransformMatrix;
    workTransformMatrix = transform;
    Matrix4x4::preTranslate(point.x, point.y, point.z);
    const Vector3 result = OsageMatrixPosition(workTransformMatrix);
    workTransformMatrix = saved;
    return result;
}

void UpdateOsageReferenceAngles(OsageWork &osage, MotionInformationWork *motion) {
    MotionNodeRuntime *const nodes = motion ? static_cast<MotionNodeRuntime *>(motion->previousNodes) : nullptr;
    for (std::size_t index = 0; index < kOsageReferenceAngleConfigs.size(); ++index) {
        const OsageReferenceAngleConfig &config = kOsageReferenceAngleConfigs[index];
        const MotionNodeRuntime *const node = FindMotionNode(nodes, config.nodeId);
        const Matrix4x4 &transform = node && node->transform ? *node->transform : workTransformMatrix;
        const Vector3 origin = OsageMatrixPosition(transform);
        Vector3 endpoint = TransformOsagePoint(transform, config.baseOffset);
        const float vertical = endpoint.y - origin.y;
        if (vertical > 0.5F) {
            endpoint = TransformOsagePoint(transform, config.positiveOffset);
        } else if (vertical < -0.5F) {
            endpoint = TransformOsagePoint(transform, config.negativeOffset);
        }
        osage.referenceAngles[index] = Shenmue_Atan2Fixed(-(endpoint.x - origin.x), -(endpoint.z - origin.z));
    }
}

std::array<Vector4, 28> g_osageTargetReferencePoints{};
bool g_osageTargetReferencePointsValid = false;
float g_osageGroundHeight = -100000.0F;

void BuildOsageCollisionPoints(const OsageCollisionProfile &profile, MotionInformationWork &motion, const Matrix4x4 &fallback, Vector4 *output) {
    MotionNodeRuntime *const nodes = static_cast<MotionNodeRuntime *>(motion.previousNodes);
    for (std::size_t index = 0; index < profile.size(); ++index) {
        const MotionNodeRuntime *const node = FindMotionNode(nodes, kOsageCollisionNodeIds[index]);
        const Matrix4x4 &transform = node && node->transform ? *node->transform : fallback;
        const OsageProfilePoint &point = profile[index];
        const Vector3 position = TransformOsagePoint(transform,
            {static_cast<float>(static_cast<double>(point.x) * 0.01), static_cast<float>(static_cast<double>(point.y) * 0.01),
                static_cast<float>(static_cast<double>(point.z) * 0.01)});
        output[index] = {position.x, position.y, position.z, static_cast<float>(static_cast<double>(point.radius) * 0.01)};
    }
}

void UpdateOsageCollisionPoints(OsageWork &osage, MotionInformationWork &motion) {
    const Matrix4x4 fallback = workTransformMatrix;
    if (const auto *const profile = static_cast<const OsageCollisionProfile *>(osage.faceData)) {
        BuildOsageCollisionPoints(*profile, motion, fallback, osage.referencePoints);
    }

    g_osageTargetReferencePointsValid = false;
    if (!osage.targetCharacter || !osage.targetFaceData)
        return;
    MotionManagerWork *const targetManager = MOTM_GetWork(osage.targetCharacter);
    if (!targetManager)
        return;
    BuildOsageCollisionPoints(
        *static_cast<const OsageCollisionProfile *>(osage.targetFaceData), targetManager->motion, fallback, g_osageTargetReferencePoints.data());
    g_osageTargetReferencePointsValid = true;
}

OsageSegmentRuntime *FindOsageSegment(OsageWork &osage, const Mt5RuntimeNode *node) {
    for (auto *segment = reinterpret_cast<OsageSegmentRuntime *>(static_cast<std::uintptr_t>(osage.renderChain)); segment; segment = segment->next) {
        if (segment->node == node)
            return segment;
    }
    return nullptr;
}

std::int32_t OsageTransformOffset(const OsageSegmentRuntime &segment) {
    if (!g_pCurrentTaskData)
        return 0;
    const std::ptrdiff_t offset = reinterpret_cast<const std::byte *>(&segment.transform) - g_pCurrentTaskData;
    if (offset < std::numeric_limits<std::int32_t>::min() || offset > std::numeric_limits<std::int32_t>::max()) {
        return 0;
    }
    return static_cast<std::int32_t>(offset);
}

void BindOsageSegment(OsageSegmentRuntime &segment) {
    if (!segment.node)
        return;
    const std::int32_t offset = OsageTransformOffset(segment);
    if (!offset)
        return;
    HMDL_SetTemporaryTaskRelativeOffsets(segment.node, true);
    segment.node->transformOffset = offset;
}

Vector3 OsagePreviousEndpoint(const OsageSegmentRuntime &segment) {
    Vector3 result{};
    std::memcpy(&result.x, &segment.previousEndpointXY, sizeof(segment.previousEndpointXY));
    result.z = segment.previousEndpointZ;
    return result;
}

void SetOsagePreviousEndpoint(OsageSegmentRuntime &segment, const Vector3 &point) {
    std::memcpy(&segment.previousEndpointXY, &point.x, sizeof(segment.previousEndpointXY));
    segment.previousEndpointZ = point.z;
}

float OsageFrameDelta(float delta) {
    return g_bEnableFrameTimeSmoothing ? delta * 30.0F * fFrameTimeMs : delta;
}



float CalculateOsageMotionMagnitude(OsageSegmentRuntime &segment, float inverseDelta) {
    float magnitude = 0.0F;
    if ((segment.updateFlags & 0xf0U) != 0U) {
        segment.state = 6;
        segment.angles = {};
    } else {
        const Vector3 previous = OsagePreviousEndpoint(segment);
        const float dx = segment.localEndpoint.x - previous.x;
        const float dy = segment.localEndpoint.y - previous.y;
        const float dz = segment.localEndpoint.z - previous.z;
        const float sine = sin_fixed16(static_cast<std::int16_t>(segment.baseRotation));
        const std::uint16_t rotation = static_cast<std::uint16_t>(segment.baseRotation);
        const std::int16_t folded = rotation <= 0x8000U ? static_cast<std::int16_t>(rotation) : static_cast<std::int16_t>(-rotation);
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        segment.angles.z += dy;
        segment.angles.w += cosine * dz + sine * dx;
        segment.angles.y += cosine * dx - sine * dz;
        if (segment.state <= 1U) {
            segment.state = 6;
            const float x = segment.angles.z * 5.0F;
            const float y = segment.angles.y * 5.0F;
            const float z = segment.angles.w * 5.0F;
            segment.angles.x = std::sqrt(x * x + y * y + z * z);
            segment.angles.y = 0.0F;
            segment.angles.z = 0.0F;
            segment.angles.w = 0.0F;
        } else {
            --segment.state;
        }
        magnitude = inverseDelta * segment.angles.x;
    }
    SetOsagePreviousEndpoint(segment, segment.localEndpoint);
    return magnitude;
}

void UpdateOsageWindPhases(OsageSegmentRuntime &segment, float adjustSpeedX, float adjustSpeedY, float targetPhaseX, float targetPhaseY, float frameDelta) {
    if (segment.windPhaseX <= targetPhaseX) {
        segment.windPhaseX += frameDelta * adjustSpeedX;
        if (segment.windPhaseX > targetPhaseX)
            segment.windPhaseX = targetPhaseX;
    } else {
        segment.windPhaseX -= frameDelta * adjustSpeedX;
        if (segment.windPhaseX < 0.0F)
            segment.windPhaseX = 0.0F;
    }
    if (segment.windPhaseY <= targetPhaseY) {
        segment.windPhaseY += frameDelta * adjustSpeedY;
        if (segment.windPhaseY > targetPhaseY)
            segment.windPhaseY = targetPhaseY;
    } else {
        segment.windPhaseY -= frameDelta * adjustSpeedY;
        if (segment.windPhaseY < 0.0F)
            segment.windPhaseY = 0.0F;
    }
}

float AdvanceEnvironmentRandomUnit() {
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    return static_cast<float>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125F;
}

float AdvanceOsageRandomOffset() {
    return AdvanceEnvironmentRandomUnit() * 5.0F - 2.5F;
}

void ApplyOsageTilt(OsageSegmentRuntime &segment, float deltaX, float deltaZ, float frameDelta, float randomX, float randomZ) {
    segment.angleX += (randomX + deltaX) * frameDelta;
    if (segment.angleX >= 360.0F)
        segment.angleX -= 360.0F;
    else if (segment.angleX < 0.0F)
        segment.angleX += 360.0F;
    segment.angleZ += (randomZ + deltaZ) * frameDelta;
    if (segment.angleZ >= 360.0F)
        segment.angleZ -= 360.0F;
    else if (segment.angleZ < 0.0F)
        segment.angleZ += 360.0F;
}

float OsageWindMagnitude() {
    const float windX = sin_fixed16(static_cast<std::int16_t>(g_WINDAngle)) * g_fWINDIntensity;
    const std::uint16_t angle = static_cast<std::uint16_t>(g_WINDAngle);
    const std::int16_t folded = angle <= 0x8000U ? static_cast<std::int16_t>(angle) : static_cast<std::int16_t>(-angle);
    const float windZ = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * g_fWINDIntensity;
    return std::sqrt(windX * windX + windZ * windZ);
}

std::uint32_t OsageBaseRotation(const OsageSegmentRuntime &segment, std::uint16_t nodeKind) {
    if ((segment.linkFlags & 0x1fU) != 0U)
        return segment.parent->baseRotation;
    const OsageWork &osage = *segment.owner;
    switch (nodeKind) {
    case 120:
        return osage.referenceAngles[0] + 0x8000U;
    case 121:
    case 124:
    case 126:
        return osage.referenceAngles[0];
    case 122:
        return osage.referenceAngles[2] + 0x8000U;
    case 123:
    case 125:
        return osage.referenceAngles[1] + 0x8000U;
    case 127:
    case 128:
    case 130:
        return osage.referenceAngles[2];
    default:
        return osage.referenceAngles[1];
    }
}

Vector3 OsageEndpointFromTransform(const OsageSegmentRuntime &segment, float amount = 1.0F) {
    const Mt5RuntimeNode *const endpoint =
        segment.node ? (segment.node->childOffset ? OsageOffsetNode(segment.node, segment.node->childOffset) : segment.node) : nullptr;
    if (!endpoint)
        return segment.localEndpoint;
    return TransformOsagePoint(segment.transform, {endpoint->position.x * amount, endpoint->position.y * amount, endpoint->position.z * amount});
}

void ApplyOsageFinalTransform(OsageSegmentRuntime &segment) {
    Matrix4x4::SetViewMatrixFromEuler(segment.rotY, segment.rotX, segment.rotZ);
    segment.transform = workTransformMatrix;
}
void BuildOsageSegmentTransform(OsageSegmentRuntime &segment) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(segment.localEndpoint.x, segment.localEndpoint.y, segment.localEndpoint.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(segment.rotY));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(segment.rotX));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(segment.rotZ));
    segment.transform = workTransformMatrix;
}

void InitializeOsageSegment(OsageSegmentRuntime &segment) {
    Mt5RuntimeNode &node = *segment.node;
    segment.nodeKind = static_cast<std::int16_t>(node.nodeKind);
    segment.baseRotation = OsageBaseRotation(segment, node.nodeKind);
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    segment.localEndpoint = OsageMatrixPosition(workTransformMatrix);
    SetOsagePreviousEndpoint(segment, segment.localEndpoint);
    segment.rotX = node.rotation.x;
    segment.rotY = node.rotation.y;
    segment.rotZ = node.rotation.z;
    ApplyOsageFinalTransform(segment);
    segment.worldEndpoint = OsageEndpointFromTransform(segment);
}


void ResetOsageSegment(OsageSegmentRuntime &segment) {
    Mt5RuntimeNode &node = *segment.node;
    segment.baseRotation = (segment.linkFlags & 0x1fU) != 0U ? segment.parent->baseRotation : segment.owner->referenceAngles[1];
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    segment.localEndpoint = OsageMatrixPosition(workTransformMatrix);
    segment.rotX = node.rotation.x;
    segment.rotY = node.rotation.y;
    segment.rotZ = node.rotation.z;
    segment.updateFlags |= 0x80U;
    ApplyOsageFinalTransform(segment);
}

void AlignOsageVector(const Vector3 &origin, const Vector3 &target) {
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    const float x = target.x - origin.x;
    const float y = target.y - origin.y;
    const float z = target.z - origin.z;
    const float distanceXY = std::sqrt(x * x + y * y);
    const float distanceYZ = std::sqrt(y * y + z * z);
    const float distance = std::sqrt(x * x + y * y + z * z);
    if (distanceXY <= distanceYZ) {
        if (static_cast<double>(distanceXY) > 0.00001)
            Matrix4x4::RotateAroundZ(x / distanceXY, -y / distanceXY);
        Matrix4x4::RotateAroundX(-z / distance, distanceXY / distance);
    } else {
        if (static_cast<double>(distanceYZ) > 0.00001)
            Matrix4x4::RotateAroundX(-z / distanceYZ, -y / distanceYZ);
        Matrix4x4::RotateAroundZ(x / distance, distanceYZ / distance);
    }
}

void SetOsageVectorLength(const Vector3 &origin, Vector3 &value, float length) {
    const float x = value.x - origin.x;
    const float y = value.y - origin.y;
    const float z = value.z - origin.z;
    float magnitude = std::sqrt(x * x + y * y + z * z);
    if (0.0001 > static_cast<double>(magnitude))
        magnitude = 0.0001F;
    const float scale = length / magnitude;
    value = {scale * x + origin.x, scale * y + origin.y, scale * z + origin.z};
}

bool ConstrainOsageVector(const Vector4 *points, Vector3 &value, std::size_t pointCount, float tolerance) {
    Vector4 closest = points[0];
    for (std::size_t index = 1; index < pointCount; ++index) {
        const Vector4 &base = points[index];
        const float x = closest.x - base.x;
        const float y = closest.y - base.y;
        const float z = closest.z - base.z;
        const float radius = closest.w - base.w;
        float denominator = x * x + y * y + z * z;
        if (0.0000001 > static_cast<double>(denominator))
            denominator = 0.0000001F;
        float amount = ((value.x - base.x) * x + (value.y - base.y) * y + (value.z - base.z) * z) / denominator;
        if (pointCount <= 2 || static_cast<double>(amount) > 0.6)
            amount = static_cast<float>(static_cast<double>(amount) - 0.1);
        else if (0.4 > static_cast<double>(amount))
            amount = static_cast<float>(static_cast<double>(amount) + 0.1);
        amount = std::min(1.0F, std::max(0.0F, amount));
        closest = {amount * x + base.x, amount * y + base.y, amount * z + base.z, amount * radius + base.w};
    }

    const float x = value.x - closest.x;
    const float y = value.y - closest.y;
    const float z = value.z - closest.z;
    float distance = std::sqrt(x * x + y * y + z * z);
    const float radius = tolerance + closest.w;
    if (!(radius > distance))
        return false;
    if (0.0001 > static_cast<double>(distance))
        distance = 0.0001F;
    const float scale = radius / distance;
    value = {scale * x + closest.x, scale * y + closest.y, scale * z + closest.z};
    return true;
}

bool ConstrainOsageRanges(const OsageWork &osage, Vector3 &point, const OsageConstraintRange *ranges, std::size_t rangeCount, float tolerance) {
    for (std::size_t index = 0; index < rangeCount; ++index) {
        const OsageConstraintRange &range = ranges[index];
        if (ConstrainOsageVector(osage.referencePoints + range.first, point, range.count, tolerance)) {
            return true;
        }
    }
    return false;
}

std::int32_t OsageDegreesToFixed(float degrees) {
    return static_cast<std::int32_t>(degrees * 8192.0F / 45.0F);
}

// The oscillation of a phase angle in degrees: sin(angle) scaled by its wind phase.
float OsageOscillation(float angle, float phase) {
    return sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(angle * 8192.0F / 45.0F))) * phase;
}

// The 122/123 wind-phase rule: a phase below its target jumps to it, one above decays toward zero by `decay` per frame.
void DecayOsageWindPhases(OsageSegmentRuntime &segment, float targetX, float targetY, float decay, float frameDelta) {
    if (segment.windPhaseX <= targetX) {
        segment.windPhaseX = targetX;
    } else {
        segment.windPhaseX -= decay * frameDelta;
        if (segment.windPhaseX < 0.0F)
            segment.windPhaseX = 0.0F;
    }
    if (segment.windPhaseY <= targetY) {
        segment.windPhaseY = targetY;
    } else {
        segment.windPhaseY -= decay * frameDelta;
        if (segment.windPhaseY < 0.0F)
            segment.windPhaseY = 0.0F;
    }
}

// A degree value as the zero-extended 16-bit angle the states add to node rotations.
std::int32_t OsageFixedAngle(float degrees) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0F / 45.0F));
}

Vector3 EvaluateOsagePitchEndpoint(OsageSegmentRuntime &segment, float pitch, float amount = 1.0F) {
    segment.rotX = static_cast<std::uint16_t>(OsageDegreesToFixed(pitch));
    BuildOsageSegmentTransform(segment);
    return OsageEndpointFromTransform(segment, amount);
}

float ResolveOsagePitchDownward(
    OsageSegmentRuntime &segment, float pitch, const OsageConstraintRange *ranges, std::size_t rangeCount, float tolerance, float maximum = 0.0F) {
    auto collides = [&](Vector3 point) { return ConstrainOsageRanges(*segment.owner, point, ranges, rangeCount, tolerance); };

    Vector3 endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
    if (!collides(endpoint))
        return pitch;
    segment.updateFlags |= 1U;

    do {
        endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
        if (!collides(endpoint))
            break;
        pitch -= 10.0F;
    } while (pitch >= -120.0F);

    do {
        endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
        if (collides(endpoint))
            break;
        pitch += 1.0F;
    } while (pitch <= maximum);

    do {
        endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
        if (!collides(endpoint))
            break;
        pitch = static_cast<float>(static_cast<double>(pitch) - 0.1);
    } while (pitch >= -120.0F);
    return pitch;
}

float ResolveOsagePitchUpward(
    OsageSegmentRuntime &segment, float pitch, float maximum, const OsageConstraintRange *ranges, std::size_t rangeCount, float tolerance) {
    auto collides = [&](Vector3 point) { return ConstrainOsageRanges(*segment.owner, point, ranges, rangeCount, tolerance); };

    Vector3 endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
    if (!collides(endpoint))
        return pitch;
    segment.updateFlags |= 1U;

    do {
        endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
        if (!collides(endpoint))
            break;
        pitch += 10.0F;
    } while (pitch <= maximum);

    do {
        endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
        if (collides(endpoint))
            break;
        pitch -= 1.0F;
    } while (pitch >= -120.0F);

    do {
        endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
        if (!collides(endpoint))
            break;
        pitch = static_cast<float>(static_cast<double>(pitch) + 0.1);
    } while (pitch <= maximum);
    return pitch;
}

bool ConstrainOsageAgainstBody(const Vector4 *points, const Vector3 &origin, Vector3 &value, float length, float tolerance) {
    struct ConstraintRange {
        std::uint8_t first;
        std::uint8_t count;
    };
    constexpr std::array<ConstraintRange, 12> ranges{{
        {0, 2},
        {2, 2},
        {4, 4},
        {8, 2},
        {10, 2},
        {12, 2},
        {14, 2},
        {16, 2},
        {18, 2},
        {20, 4},
        {24, 2},
        {26, 2},
    }};
    bool adjusted = false;
    for (const ConstraintRange range : ranges) {
        adjusted |= ConstrainOsageVector(points + range.first, value, range.count, tolerance);
        SetOsageVectorLength(origin, value, length);
    }
    return adjusted;
}

OsageHairTuning HairTuningForSegment(const OsageSegmentRuntime &segment) {
    const auto imageIs = [&](char a, char b, char c) { return (segment.owner->imageName & 0x00ffffffU) == OsageImageCode(a, b, c); };
    const auto imageIs4 = [&](char a, char b, char c, char d) { return segment.owner->imageName == OsageImageCode(a, b, c, d); };
    const std::uint8_t chainIndex = static_cast<std::uint8_t>(segment.linkFlags & 0x1fU);

    if (imageIs4('M', 'J', 'O', 'J'))
        return {1.2F, 0.025F, 0.0055F, 0.85F, 0};
    if (imageIs('Y', 'P', 'H'))
        return {1.6F, 0.001F, 0.015F, 0.75F, 0};
    if (imageIs('X', 'P', 'H')) {
        const float length = (segment.linkFlags & 0x40U) ? 1.5F : 10.0F;
        return {length, 0.015F, 0.025F, 0.65F, 2};
    }
    if (imageIs('R', 'G', 'H') || imageIs('R', 'G', 'M')) {
        float length = 1.1F;
        if (chainIndex <= 1)
            length = 1.5F;
        else if (chainIndex <= 3)
            length = 1.7F;
        else if (chainIndex == 4)
            length = 1.3F;
        return {length, 0.01F, 0.01F, 0.8F, 0};
    }
    if (imageIs('M', 'H', 'H') || imageIs('N', 'H', 'H') || imageIs('T', 'H', 'H') || imageIs('Y', 'H', 'H')) {
        return {1.2F, 0.015F, 0.03F, 0.7F, 0};
    }
    if (imageIs4('L', 'J', 'O', 'J') || imageIs('O', 'A', 'K') || imageIs('S', 'H', 'T') || imageIs('B', 'K', 'G')) {
        return {1.2F, 0.001F, 0.025F, 0.9F, 0};
    }
    if (imageIs('N', 'I', 'S') || imageIs('H', 'I', 'S') || imageIs('X', 'I', 'S') || imageIs('Y', 'I', 'S')) {
        float length = 3.5F;
        if (chainIndex == 1)
            length = 2.4F;
        else if (chainIndex == 2)
            length = 1.3F;
        else if (chainIndex > 2)
            length = 1.7F;
        return {length, 0.005F, 0.013F, 0.8F, 0};
    }
    if (imageIs('O', 'R', 'F') || imageIs('E', 'M', 'M')) {
        return {2.5F, 0.025F, 0.015F, 0.8F, 0};
    }
    if (imageIs('P', 'Y', 'S') || imageIs('E', 'Y', 'S') || imageIs('J', 'Y', 'S')) {
        return {
            1.0F,
            0.002F,
            0.015F,
            0.85F,
            static_cast<std::uint8_t>(chainIndex ? 0 : 2),
        };
    }
    if (imageIs('2', '6', 'X')) {
        return {
            1.0F,
            chainIndex ? 0.005F : 0.01F,
            0.02F,
            0.7F,
            0,
        };
    }
    if (imageIs('A', 'I', 'S') || imageIs('A', 'W', 'T') || imageIs('H', 'I', 'M')) {
        return {1.0F, 0.01F, 0.02F, 0.7F, 0};
    }
    if (imageIs('B', 'S', 'H') || imageIs('Y', 'H', 'K')) {
        return {1.5F, 0.01F, 0.025F, 0.75F, 0};
    }
    if (imageIs('E', 'M', 'A') || imageIs('G', 'M', 'A')) {
        return {
            (segment.linkFlags & 1U) ? 0.75F : 1.25F,
            0.015F,
            0.015F,
            0.75F,
            0,
        };
    }
    if (imageIs('E', 'P', 'H'))
        return {1.1F, 0.01F, 0.02F, 0.7F, 0};
    if (imageIs('S', 'C', 'A') || imageIs('I', 'Y', 'Y')) {
        const bool hasChild = segment.node && segment.node->childOffset;
        return hasChild ? OsageHairTuning{1.1F, 0.01F, 0.015F, 0.85F, 0} : OsageHairTuning{0.5F, 0.005F, 0.015F, 0.85F, 0};
    }
    if (imageIs('J', 'H', 'C')) {
        return chainIndex ? OsageHairTuning{1.0F, 0.005F, 0.025F, 0.8F, 0} : OsageHairTuning{0.75F, 0.01F, 0.025F, 0.8F, 0};
    }
    if (imageIs('N', 'E', 'R')) {
        return chainIndex ? OsageHairTuning{1.1F, 0.002F, 0.02F, 0.75F, 0} : OsageHairTuning{1.0F, 0.01F, 0.02F, 0.75F, 1};
    }
    if (imageIs('K', 'O', 'K')) {
        OsageHairTuning tuning{1.5F, 0.01F, 0.025F, 0.6F, 0};
        if (chainIndex == 0) {
            tuning.lengthScale = 0.8F;
            tuning.collisionTolerance = 0.015F;
            tuning.alternateMode = 2;
        } else if (chainIndex == 1) {
            tuning.lengthScale = 1.0F;
            tuning.alternateMode = 2;
        } else if (chainIndex == 2) {
            tuning.lengthScale = 1.2F;
            tuning.gravity = 0.035F;
        } else if (chainIndex == 3) {
            tuning.gravity = 0.03F;
        }
        if (chainIndex && segment.parent && segment.parent->angles.x > 4.0F) {
            tuning.gravity = 0.7F;
        }
        return tuning;
    }
    return {1.0F, 0.001F, 0.01F, 0.8F, 0};
}

bool UsesHairMotionOffset(std::uint32_t imageName) {
    const std::uint32_t image = imageName & 0x00ffffffU;
    return image == OsageImageCode('R', 'G', 'M') || image == OsageImageCode('H', 'I', 'S') || image == OsageImageCode('N', 'I', 'S') ||
        image == OsageImageCode('R', 'G', 'H') || image == OsageImageCode('X', 'I', 'S') || image == OsageImageCode('Y', 'I', 'S');
}

Vector3 UpdateHairMotionOffset(OsageSegmentRuntime &segment, Vector3 anchor) {
    const float amount = UsesHairMotionOffset(segment.owner->imageName) ? g_OSAG_Val_2 : -1.0F;
    if (!(static_cast<double>(amount) > 0.000001))
        return anchor;

    auto *const history = reinterpret_cast<float *>(&segment.oscillatedAngleX);
    for (std::int32_t index = 17; index >= 2; --index)
        history[index] = history[index - 2];

    if ((segment.linkFlags & 0x1fU) != 0U) {
        const auto *const parentHistory = reinterpret_cast<const float *>(&segment.parent->oscillatedAngleX);
        segment.oscillatedAngleX = parentHistory[6];
        segment.currentAngleZ = parentHistory[7];
    } else {
        const auto advanceAxis = [](float &position, float &velocity, float limit, double ratio) {
            float acceleration = static_cast<float>(static_cast<double>(limit) * ratio);
            if (velocity < 0.0F)
                acceleration = -acceleration;
            position = velocity + acceleration + position;
            const auto negligible = [](float value) { return static_cast<double>(value) < 0.000001 && static_cast<double>(value) > -0.000001; };
            if (position > limit) {
                velocity = -std::fabs(velocity);
                if (negligible(velocity))
                    velocity = -acceleration;
            } else if (-limit > position) {
                velocity = std::fabs(acceleration) * AdvanceEnvironmentRandomUnit();
                if (negligible(velocity))
                    velocity = acceleration;
            }
        };
        advanceAxis(segment.oscillatedAngleX, history[18], static_cast<float>(static_cast<double>(amount) * 0.25), 0.066666);
        advanceAxis(segment.currentAngleZ, history[19], static_cast<float>(static_cast<double>(amount) * 0.33333), 0.1);
    }

    const std::int16_t negativeBase = static_cast<std::int16_t>(-static_cast<std::int32_t>(segment.baseRotation));
    const float sine = sin_fixed16(negativeBase);
    const float cosine = cos_fixed16(static_cast<std::uint16_t>(negativeBase));
    anchor.x += segment.oscillatedAngleX * cosine + sine * amount;
    anchor.y += segment.currentAngleZ;
    anchor.z += segment.oscillatedAngleX * sine - cosine * amount;
    return anchor;
}

void CoupleHairEndpointToParent(const Vector3 &anchor, Vector3 &candidate, const Vector3 &parentAnchor, float length) {
    const Vector3 direction{candidate.x - anchor.x, candidate.y - anchor.y, candidate.z - anchor.z};
    const float projection =
        ((parentAnchor.x - anchor.x) * direction.x + (parentAnchor.y - anchor.y) * direction.y + (parentAnchor.z - anchor.z) * direction.z) /
        (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (!(static_cast<double>(projection) > 0.2))
        return;
    candidate = {projection * direction.x + anchor.x, projection * direction.y + anchor.y, projection * direction.z + anchor.z};
    const Vector3 parentOffset{anchor.x - parentAnchor.x, anchor.y - parentAnchor.y, anchor.z - parentAnchor.z};
    const float parentDistance = std::sqrt(parentOffset.x * parentOffset.x + parentOffset.y * parentOffset.y + parentOffset.z * parentOffset.z);
    SetOsageVectorLength(parentAnchor, candidate, parentDistance);
    SetOsageVectorLength(anchor, candidate, length);
}

void ApplyOsageHairState(OsageSegmentRuntime &segment, const Vector3 &oldAnchor, const Vector3 &oldEndpoint, const Vector3 &previousEndpoint, float frameDelta,
    float inverseDelta, float randomX, float randomZ) {
    Mt5RuntimeNode &node = *segment.node;
    const std::uint32_t image = segment.owner->imageName & 0x00ffffffU;
    const bool chained = (segment.linkFlags & 0x1fU) != 0U;
    if (image == OsageImageCode('K', 'O', 'K') && chained)
        segment.angles.x = segment.parent->angles.x;
    const OsageHairTuning tuning = HairTuningForSegment(segment);
    segment.baseRotation = OsageBaseRotation(segment, node.nodeKind);
    if (!chained &&
        (image == OsageImageCode('M', 'H', 'H') || image == OsageImageCode('N', 'H', 'H') || image == OsageImageCode('T', 'H', 'H') ||
            image == OsageImageCode('Y', 'H', 'H'))) {
        segment.baseRotation = segment.owner->referenceAngles[(segment.linkFlags & 0x20U) ? 4 : 3] + 0x8000U;
    }
    const Vector3 anchor = segment.localEndpoint;
    const Vector3 physicsAnchor = UpdateHairMotionOffset(segment, anchor);

    if (tuning.alternateMode == 1) {
        segment.rotX = node.rotation.x;
        segment.rotY = node.rotation.y;
        segment.rotZ = node.rotation.z;
        ApplyOsageFinalTransform(segment);
        return;
    }
    if (tuning.alternateMode == 2) {
        CalculateOsageMotionMagnitude(segment, inverseDelta);
        const float swingTarget = static_cast<float>(static_cast<double>(segment.angles.x) * 12.0);
        UpdateOsageWindPhases(segment, 5.0F, 0.0F, 50.0F < swingTarget ? 50.0F : swingTarget, 0.0F, frameDelta);
        ApplyOsageTilt(segment, 20.0F, 0.0F, frameDelta, randomX, randomZ);
        const float swing = sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(segment.angleX * 8192.0F / 45.0F)));
        const float swingBend = static_cast<float>((static_cast<double>(swing) * 0.1 + 1.0) * static_cast<double>(segment.windPhaseX));
        float bend = 0.0F > swingBend ? 0.0F : swingBend;
        segment.rotX = static_cast<std::uint16_t>(OsageDegreesToFixed(bend));
        segment.rotY = static_cast<std::int32_t>(segment.baseRotation);
        segment.rotZ = 0;
        const auto collidesAt = [&](float angle) {
            Vector3 endpoint = EvaluateOsagePitchEndpoint(segment, angle, tuning.lengthScale);
            return ConstrainOsageVector(segment.owner->referencePoints + 4, endpoint, 4, tuning.collisionTolerance);
        };
        if (!collidesAt(bend))
            return;
        bool raising = true;
        do {
            if (!collidesAt(bend))
                break;
            bend = static_cast<float>(static_cast<double>(bend) + 10.0);
            if (bend > 120.0F) {
                bend = 120.0F;
                raising = false;
            }
            segment.updateFlags |= 1U;
        } while (raising);
        for (;;) {
            if (collidesAt(bend)) {
                segment.updateFlags |= 1U;
                break;
            }
            bend = static_cast<float>(static_cast<double>(bend) - 1.0);
            if (0.0F > bend) {
                bend = 0.0F;
                break;
            }
        }
        do {
            if (!collidesAt(bend))
                break;
            bend = static_cast<float>(static_cast<double>(bend) + 0.1);
        } while (!(bend > 120.0F));
        return;
    }

    Vector3 candidate{};
    if ((segment.updateFlags & 0xf0U) != 0U) {
        candidate = {
            (anchor.x - oldAnchor.x) + oldEndpoint.x,
            static_cast<float>(static_cast<double>(anchor.y - oldAnchor.y) - 0.1) + oldEndpoint.y,
            (anchor.z - oldAnchor.z) + oldEndpoint.z,
        };
    } else {
        candidate = {
            (oldEndpoint.x - previousEndpoint.x) * tuning.damping + oldEndpoint.x,
            ((oldEndpoint.y - previousEndpoint.y) - frameDelta * tuning.gravity) + oldEndpoint.y,
            (oldEndpoint.z - previousEndpoint.z) * tuning.damping + oldEndpoint.z,
        };
    }
    SetOsagePreviousEndpoint(segment, oldEndpoint);
    float groundHeight = g_osageGroundHeight;
    if (-10000.0F > groundHeight && !UsesHairMotionOffset(segment.owner->imageName)) {
        groundHeight = mapUpdateIntersectionStateAtLocalPoint(candidate);
        g_osageGroundHeight = groundHeight;
    }
    if (groundHeight > candidate.y) {
        segment.updateFlags |= 1U;
        candidate.y = groundHeight;
    }

    Mt5RuntimeNode *const child = OsageOffsetNode(&node, node.childOffset);
    const Vector3 segmentVector = child ? child->position : node.position;
    const float length =
        tuning.lengthScale * std::sqrt(segmentVector.x * segmentVector.x + segmentVector.y * segmentVector.y + segmentVector.z * segmentVector.z);
    const float dx = candidate.x - physicsAnchor.x;
    const float dy = candidate.y - physicsAnchor.y;
    const float dz = candidate.z - physicsAnchor.z;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (0.0001 > static_cast<double>(distance))
        distance = 0.0001F;
    const float scale = length / distance;
    candidate = {scale * dx + physicsAnchor.x, scale * dy + physicsAnchor.y, scale * dz + physicsAnchor.z};

    if (chained)
        CoupleHairEndpointToParent(anchor, candidate, segment.parent->localEndpoint, length);
    if (image == OsageImageCode('N', 'E', 'R') && segment.child && (segment.updateFlags & 0xf0U) == 0U) {
        const Vector3 childDelta{
            segment.child->worldEndpoint.x - candidate.x,
            segment.child->worldEndpoint.y - candidate.y,
            segment.child->worldEndpoint.z - candidate.z,
        };
        const float projection = ((anchor.x - candidate.x) * childDelta.x + (anchor.y - candidate.y) * childDelta.y + (anchor.z - candidate.z) * childDelta.z) /
            (childDelta.x * childDelta.x + childDelta.y * childDelta.y + childDelta.z * childDelta.z);
        if (static_cast<double>(projection) > -0.6) {
            candidate = {
                static_cast<float>(static_cast<double>(childDelta.x) * 0.2 + static_cast<double>(candidate.x)),
                static_cast<float>(static_cast<double>(childDelta.y) * 0.2 + static_cast<double>(candidate.y)),
                static_cast<float>(static_cast<double>(childDelta.z) * 0.2 + static_cast<double>(candidate.z)),
            };
            SetOsageVectorLength(anchor, candidate, length);
        }
    }

    bool adjusted = ConstrainOsageAgainstBody(segment.owner->referencePoints, anchor, candidate, length, tuning.collisionTolerance);
    if (g_osageTargetReferencePointsValid) {
        adjusted |= ConstrainOsageVector(g_osageTargetReferencePoints.data() + 16, candidate, 2, tuning.collisionTolerance);
        SetOsageVectorLength(anchor, candidate, length);
        adjusted |= ConstrainOsageVector(g_osageTargetReferencePoints.data() + 18, candidate, 2, tuning.collisionTolerance);
        SetOsageVectorLength(anchor, candidate, length);
    }
    if (adjusted)
        segment.updateFlags |= 1U;

    segment.worldEndpoint = candidate;
    workTransformMatrix = transformMatrix_prePass;
    AlignOsageVector(anchor, candidate);
    const std::uint16_t yaw = static_cast<std::uint16_t>(segment.baseRotation);
    Matrix4x4::RotateAroundY(

        sin_fixed16(static_cast<std::int16_t>(yaw)), cos_fixed16(yaw));
    segment.transform = workTransformMatrix;
}
void UpdateOsageAuxiliaryRecords(OsageSegmentRuntime &segment) {
    if (!segment.auxiliaryData || !segment.node || !segment.node->sceneBindingOffset) {
        return;
    }
    std::byte *const modelBase = MT5_ModelBaseForNode(segment.node);
    if (!modelBase)
        return;
    std::byte *const binding = modelBase + segment.node->sceneBindingOffset;
    std::int32_t recordsOffset = 0;
    std::int32_t recordCount = 0;
    std::memcpy(&recordsOffset, binding + 4, sizeof(recordsOffset));
    std::memcpy(&recordCount, binding + 8, sizeof(recordCount));
    if (!recordsOffset || recordCount <= 0)
        return;

    Matrix4x4 inverse = segment.transform;
    Matrix4x4::InvertInPlace(&inverse);
    const Vector4 &reference = segment.owner->referencePoints[0];
    const Vector3 anchor = TransformPoint(inverse, {reference.x, reference.y, reference.z});
    const float radius = reference.w;
    auto *const source = modelBase + recordsOffset;
    const auto *const original = static_cast<const std::byte *>(segment.auxiliaryData);
    for (std::int32_t index = 0; index < recordCount; ++index) {
        Vector3 value{};
        std::memcpy(&value, original + static_cast<std::size_t>(index) * 24U, sizeof(value));
        const float dx = value.x - anchor.x;
        const float dy = value.y - anchor.y;
        const float dz = value.z - anchor.z;
        float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (radius > distance) {
            distance = std::max(distance, 0.0001F);
            const float scale = radius / distance;
            value = {
                anchor.x + dx * scale,
                anchor.y + dy * scale,
                anchor.z + dz * scale,
            };
        }
        std::memcpy(source + static_cast<std::size_t>(index) * 24U, &value, sizeof(value));
    }
}

void ApplyOsageAnimatedBounce(OsageSegmentRuntime &segment, float frameDelta, float inverseDelta) {
    auto *const history = reinterpret_cast<float *>(&segment.oscillatedAngleX);
    float projectedVelocity = 0.0F;
    if ((segment.updateFlags & 0xf0U) != 0U) {
        segment.state = 6;
        segment.angles.x = 0.0F;
        segment.angles.y = 0.0F;
    } else {
        const std::uint16_t yaw = static_cast<std::uint16_t>(segment.baseRotation);
        const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
        const float cosine = cos_fixed16(yaw);
        const Vector3 previous = OsagePreviousEndpoint(segment);
        segment.angles.y += cosine * (segment.localEndpoint.x - previous.x) - sine * (segment.localEndpoint.z - previous.z);
        if (static_cast<std::int32_t>(--segment.state) <= 0) {
            segment.state = 6;
            segment.angles.x = segment.angles.y * 5.0F;
            segment.angles.y = 0.0F;
        }
        projectedVelocity = inverseDelta * segment.angles.x;
    }
    SetOsagePreviousEndpoint(segment, segment.localEndpoint);

    const float y = segment.localEndpoint.y;
    if (history[2] > y) {
        history[2] = y;
        history[4] = 0.0F;
        history[5] = 1.0F;
    }
    if (history[6] < y) {
        history[6] = y;
        history[7] = 0.0F;
        history[9] = 1.0F;
    }
    history[4] += frameDelta;
    history[7] += frameDelta;
    float direction = 1.0F;
    if (history[4] >= 4.0F && history[5] > 0.0F) {
        history[10] = history[6] - history[2];
        history[6] = -10000.0F;
        segment.angleX = 180.0F;
        history[5] = -1.0F;
        direction = -1.0F;
    }
    if (history[7] >= 4.0F && history[9] > 0.0F) {
        history[10] = history[6] - history[2];
        history[2] = 10000.0F;
        segment.angleX = 0.0F;
        history[9] = -1.0F;
    } else if (direction >= 0.0F) {
        direction = 0.0F;
    }
    if (direction != 0.0F) {
        history[12] = std::fabs(history[7] - history[4]);
        if (history[10] >= 0.0F && static_cast<double>(history[10]) < 0.3 && history[12] >= 3.0F && history[12] < 20.0F) {
            history[14] = history[10];
            history[16] = history[12];
        } else {
            history[14] = 0.0F;
        }
    }

    const std::uint32_t image = segment.owner->imageName & 0x00ffffffU;
    float response = 0.0F;
    float maximum = 0.0F;
    if (image == OsageImageCode('J', 'H', 'C')) {
        response = 0.13F;
        maximum = 0.005F;
    } else if (image == OsageImageCode('E', 'Y', 'S') || image == OsageImageCode('J', 'Y', 'S') || image == OsageImageCode('P', 'Y', 'S')) {
        response = 0.05F;
        maximum = 0.005F;
    }
    const auto approach = [frameDelta](float current, float target) {
        if (current < target) {
            const float raised = static_cast<float>(static_cast<double>(frameDelta) * 0.001 + static_cast<double>(current));
            return raised <= target ? raised : target;
        }
        if (current > target) {
            const float lowered = static_cast<float>(static_cast<double>(current) - static_cast<double>(frameDelta) * 0.0005);
            return target <= lowered ? lowered : target;
        }
        return current;
    };
    float scaledDrop = response * history[14];
    if (static_cast<double>(scaledDrop) < 0.0005)
        scaledDrop = 0.0F;
    const float target = std::fminf(maximum, scaledDrop);
    segment.windPhaseX = approach(segment.windPhaseX, target);

    float horizontal = target > 0.0F ? std::fabs(projectedVelocity) : target;
    horizontal *= response;
    if (static_cast<double>(horizontal) < 0.0005)
        horizontal = 0.0F;
    segment.windPhaseY = approach(segment.windPhaseY, std::fminf(maximum, horizontal));

    float period = history[16] + history[16];
    if (period < 6.0F)
        period = 12.0F;
    period = std::fminf(40.0F, period);
    const float phaseDelta = static_cast<float>(static_cast<double>(frameDelta) * 360.0 / static_cast<double>(period));
    segment.angleX += phaseDelta;
    if (segment.angleX >= 360.0F)
        segment.angleX -= 360.0F;
    segment.angleZ += phaseDelta;
    if (segment.angleZ >= 360.0F)
        segment.angleZ -= 360.0F;
    segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
    segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);

    segment.rotX = segment.node->rotation.x;
    segment.rotY = segment.node->rotation.y;
    segment.rotZ = segment.node->rotation.z;
    if ((segment.linkFlags & 0x1fU) == 0U) {
        Matrix4x4::preTranslate(segment.oscillatedAngleX, 0.0F, segment.currentAngleZ);
    }
}




void OsageHandRootRotation(const OsageSegmentRuntime &segment, std::size_t side, Vector3i &rotation) {
    rotation.x = 0;
    rotation.y = 0;
    Character *const character = segment.owner ? segment.owner->character : nullptr;
    if (!character || !character->model)
        return;
    if (const HandModelWork *const hand = HND_GetWork(character, side == 0U)) {
        if (hand->playback.status > 0 && hand->primaryRoot) {
            rotation = hand->primaryRoot->rotation;
            return;
        }
        if (hand->playback.status != 0)
            return;
    }
    if (const Mt5RuntimeNode *const node = shenGetNodeByChildIdx(static_cast<Mt5RuntimeNode *>(character->model), side ? -65 : -66))
        rotation = node->rotation;
}

void ApplyRotationalOsageState(OsageSegmentRuntime &segment, float frameDelta, float inverseDelta, float randomX, float randomZ) {
    Mt5RuntimeNode &node = *segment.node;
    const std::uint16_t previousBaseRotation = static_cast<std::uint16_t>(segment.baseRotation);
    segment.baseRotation = OsageBaseRotation(segment, node.nodeKind);
    const Vector3 oldAnchor = segment.localEndpoint;
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    segment.localEndpoint = OsageMatrixPosition(workTransformMatrix);
    const Matrix4x4 nodeTransform = workTransformMatrix;
    const Vector3 oldEndpoint = segment.worldEndpoint;
    const float windMagnitude = OsageWindMagnitude();
    const bool chained = (segment.linkFlags & 0x1fU) != 0U;
    const auto imageIs = [&](char a, char b, char c) { return segment.owner->imageName == OsageImageCode(a, b, c); };

    switch (node.nodeKind) {
    case 120:
        ApplyOsageHairState(segment, oldAnchor, oldEndpoint, OsagePreviousEndpoint(segment), frameDelta, inverseDelta, randomX, randomZ);
        return;
    case 121: {
        auto *const history = reinterpret_cast<float *>(&segment.oscillatedAngleX);
        for (std::int32_t index = 17; index >= 2; --index)
            history[index] = history[index - 2];

        const std::uint32_t image = segment.owner->imageName;
        float tiltX = 0.0F;
        float tiltZ = 0.0F;
        if (g_osageState121SharedValid) {
            segment.windPhaseX = g_osageState121PhaseX;
            segment.windPhaseY = g_osageState121PhaseY;
            tiltX = g_osageState121TiltX;
            tiltZ = g_osageState121TiltZ;
        } else {
            float reference = -1.0F;
            if (UsesHairMotionOffset(image))
                reference = g_OSAG_Val_2;
            else if (image == OsageImageCode('M', 'Z', 'N') || image == OsageImageCode('N', 'G', 'H'))
                reference = std::fabs(g_OSAG_Val);
            float wind = windMagnitude;
            if (static_cast<double>(reference) > 0.000001)
                wind = reference * 30.0F;
            const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
            tiltX = 20.0F;
            tiltZ = -30.0F;
            float targetX = 0.0F;
            float targetY = 0.0F;
            const auto clampedWind = [&](float limit) { return std::fminf(limit, std::fmaxf(wind, motion)); };
            if (image == OsageImageCode('T', 'H', 'H') || image == OsageImageCode('S', 'I', 'K') || image == OsageImageCode('Y', 'H', 'S') ||
                image == OsageImageCode('Y', 'H', 'H') || image == OsageImageCode('N', 'H', 'H') || image == OsageImageCode('M', 'H', 'H') ||
                image == OsageImageCode('H', 'P', 'H') || image == OsageImageCode('I', 'P', 'H') || image == OsageImageCode('J', 'P', 'H') ||
                image == OsageImageCode('D', 'H', 'S') || image == OsageImageCode('A', 'S', 'H') || image == OsageImageCode('B', 'S', 'H')) {
                targetX = std::fminf(0.5F, std::fmaxf(wind, motion) * 0.25F);
                targetY = targetX;
            } else if (image == OsageImageCode('S', 'G', 'H') || image == OsageImageCode('R', 'G', 'H') || image == OsageImageCode('R', 'G', 'M') ||
                image == OsageImageCode('U', 'E', 'S') || image == OsageImageCode('Y', 'G', 'H') || image == OsageImageCode('Y', 'H', 'K') ||
                image == OsageImageCode('M', 'Z', 'N') || image == OsageImageCode('F', 'M', 'J') || image == OsageImageCode('D', 'F', 'R')) {
                targetX = std::fmaxf(wind, motion);
                if (static_cast<double>(targetX) > 1.2)
                    targetX = 1.2F;
                targetY = targetX;
            } else if (image == OsageImageCode('N', 'R', 'K') || image == OsageImageCode('C', 'G', 'H')) {
                tiltX = 16.0F;
                tiltZ = -24.0F;
                targetX = clampedWind(1.0F);
                targetY = targetX;
            } else if (image == OsageImageCode('N', 'G', 'H')) {
                tiltX = 16.0F;
                tiltZ = -24.0F;
                targetX = clampedWind(0.5F);
                targetY = targetX;
            } else if (image == OsageImageCode('Z', 'C', 'M') || image == OsageImageCode('L', 'J', 'O', 'J') || image == OsageImageCode('M', 'J', 'O', 'J') ||
                image == OsageImageCode('I', 'Y', 'Y') || image == OsageImageCode('A', 'D', 'Y')) {
                targetX = clampedWind(1.5F);
                targetY = targetX;
            } else if (image == OsageImageCode('T', 'A', 'N')) {
                targetX = static_cast<float>(static_cast<double>(std::fmaxf(wind, motion)) * 0.2);
                if (static_cast<double>(targetX) > 0.4)
                    targetX = 0.4F;
                tiltX = 8.0F;
                tiltZ = -12.0F;
                targetY = targetX;
            } else if (image == OsageImageCode('T', 'A', 'T')) {
                wind = static_cast<float>(static_cast<double>(wind) * 0.33);
                targetY = std::fmaxf(wind, motion);
            } else if (image == OsageImageCode('O', 'N', 'O')) {
                wind = static_cast<float>(static_cast<double>(wind) * 0.1);
                targetY = std::fmaxf(wind, static_cast<float>(static_cast<double>(motion) * 0.2));
                if (static_cast<double>(targetY) > 0.1)
                    targetY = 0.1F;
                tiltX = 10.0F;
                tiltZ = -15.0F;
            } else if (image == OsageImageCode('K', 'O', 'K')) {
            } else if (image == OsageImageCode('A', 'A', 'Y')) {
                targetY = std::fmaxf(wind, motion + motion);
                targetX = targetY + targetY;
            } else {
                targetX = std::fmaxf(wind, motion + motion);
                targetY = targetX;
            }
            if (image == OsageImageCode('M', 'Z', 'N') && std::fabs(static_cast<double>(g_OSAG_Val)) > 0.000001) {
                tiltX = 16.0F;
                tiltZ = -24.0F;
                targetY = std::fminf(5.0F, wind);
            }

            UpdateOsageWindPhases(segment, 0.2F, 0.2F, targetX, targetY, frameDelta);
            g_osageState121PhaseX = segment.windPhaseX;
            g_osageState121PhaseY = segment.windPhaseY;
            g_osageState121TiltX = tiltX;
            g_osageState121TiltZ = tiltZ;
            g_osageState121SharedValid = true;
        }

        const std::uint8_t chainIndex = static_cast<std::uint8_t>(segment.linkFlags & 0x1fU);
        if (image == OsageImageCode('2', '6', 'X') && chainIndex == 0) {
            segment.windPhaseX = 0.0F;
            segment.windPhaseY = 0.0F;
        }
        if (image == OsageImageCode('D', 'F', 'R')) {
            if (chainIndex == 0) {
                segment.windPhaseX = 0.0F;
                segment.windPhaseY = 0.0F;
            } else if (chainIndex == 2) {
                segment.windPhaseX = static_cast<float>(static_cast<double>(segment.windPhaseX) * 1.2);
                segment.windPhaseY = static_cast<float>(static_cast<double>(segment.windPhaseY) * 1.2);
            }
        }
        ApplyOsageTilt(segment, tiltX, tiltZ, frameDelta, randomX, randomZ);
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        if (image == OsageImageCode('C', 'G', 'H') && chainIndex != 0) {
            const auto *const parentHistory = reinterpret_cast<const float *>(&segment.parent->oscillatedAngleX);
            segment.oscillatedAngleX = parentHistory[2];
            segment.currentAngleZ = parentHistory[3];
        }

        segment.rotX = node.rotation.x + OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = node.rotation.y;
        if (image == OsageImageCode('M', 'Z', 'N') || image == OsageImageCode('N', 'G', 'H')) {
            segment.rotZ = node.rotation.z;
            float lateral = segment.currentAngleZ;
            float remaining = g_OSAG_Val;
            std::int32_t *component = &segment.rotZ;
            const std::int32_t *source = &node.rotation.z;
            if (chainIndex != 0) {
                if (g_osageState121RemainingFrame)
                    remaining = 0.0F;
            } else {
                lateral = -lateral;
                component = &segment.rotY;
                source = &node.rotation.y;
                remaining = -g_OSAG_Val;
                g_osageState121RemainingFrame = false;
                if ((static_cast<double>(g_OSAG_Val) > 0.000001 && static_cast<double>(node.position.y) < -0.05) ||
                    (static_cast<double>(g_OSAG_Val) < -0.000001 && static_cast<double>(node.position.y) > 0.05)) {
                    g_osageState121RemainingFrame = true;
                    remaining = 0.0F;
                }
            }
            *component = OsageFixedAngle(lateral) + *source;
            if (static_cast<double>(remaining) > 0.000001 || static_cast<double>(remaining) < -0.000001)
                *component += static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(remaining) * 30.0 * 8192.0 / 45.0));
        } else {
            segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
            if (image == OsageImageCode('A', 'A', 'Y') && chainIndex == 0) {
                const std::int32_t difference = static_cast<std::int16_t>(segment.owner->referenceAngles[1] - segment.owner->referenceAngles[0]);
                if (difference > 0)
                    segment.rotX += difference / -2;
            }
        }
        break;
    }
    case 122: {
        auto *const history = reinterpret_cast<float *>(&segment.oscillatedAngleX);
        for (std::int32_t index = 17; index >= 2; --index) {
            history[index] = history[index - 2];
        }
        if (chained) {
            const auto *const parentHistory = reinterpret_cast<const float *>(&segment.parent->oscillatedAngleX);
            if (imageIs('K', 'U', 'F') || imageIs('D', 'U', 'F') || imageIs('C', 'U', 'F') || imageIs('A', 'K', 'J') || imageIs('A', 'W', 'I') ||
                imageIs('B', 'K', 'J') || imageIs('B', 'U', 'F') || imageIs('O', 'W', 'I') || imageIs('C', 'G', 'H')) {
                segment.oscillatedAngleX = parentHistory[6];
                segment.currentAngleZ = parentHistory[5];
            } else if (imageIs('G', 'O', 'T')) {
                segment.oscillatedAngleX = parentHistory[4];
                segment.currentAngleZ = parentHistory[3];
            } else {
                segment.oscillatedAngleX = parentHistory[10];
                segment.currentAngleZ = parentHistory[11];
            }
        } else {
            const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
            const bool cgh = imageIs('C', 'G', 'H');
            const float targetX = std::fminf(cgh ? 30.0F : 20.0F, motion * (cgh ? 12.0F : 4.0F));
            const float targetY = std::fminf(cgh ? 30.0F : 20.0F, motion * (cgh ? 9.0F : 3.0F));
            const float decay = cgh ? 0.2F : 0.3F;
            DecayOsageWindPhases(segment, targetX, targetY, decay, frameDelta);
            ApplyOsageTilt(segment, 15.0F, -20.0F, frameDelta, randomX, randomZ);
            segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
            if (segment.oscillatedAngleX > 0.0F) {
                segment.oscillatedAngleX = -segment.oscillatedAngleX;
            }
            segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        }
        segment.rotX = OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = static_cast<std::int32_t>(segment.baseRotation);
        segment.rotZ = OsageFixedAngle(segment.currentAngleZ);
        constexpr std::array<OsageConstraintRange, 3> ranges{{{20, 4}, {24, 2}, {26, 2}}};
        ResolveOsagePitchDownward(segment, segment.oscillatedAngleX, ranges.data(), ranges.size(), 0.01F);
        return;
    }
    case 123: {
        auto *const history = reinterpret_cast<float *>(&segment.oscillatedAngleX);
        for (std::int32_t index = 17; index >= 2; --index) {
            history[index] = history[index - 2];
        }
        if (chained) {
            const auto *const parentHistory = reinterpret_cast<const float *>(&segment.parent->oscillatedAngleX);
            segment.oscillatedAngleX = parentHistory[10];
            segment.currentAngleZ = parentHistory[11];
        } else {
            const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
            DecayOsageWindPhases(segment, std::fminf(20.0F, motion * 4.0F), std::fminf(20.0F, motion * 3.0F), 0.3F, frameDelta);
            ApplyOsageTilt(segment, 15.0F, -20.0F, frameDelta, randomX, randomZ);
            segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
            if (segment.oscillatedAngleX > 0.0F) {
                segment.oscillatedAngleX = -segment.oscillatedAngleX;
            }
            segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        }

        float maximumPitch = 0.0F;
        if (segment.owner->imageName == OsageImageCode('E', 'Z', 'Z')) {
            if (segment.owner->referencePoints[25].y > segment.owner->referencePoints[24].y) {
                std::int32_t offset = (segment.linkFlags & 0x1fU) == 1U ? 7281 : 5461;
                if ((segment.linkFlags & 0x20U) != 0U)
                    offset = -offset;
                segment.baseRotation = static_cast<std::uint32_t>(static_cast<std::int32_t>(segment.baseRotation) + offset);
            }
        } else if (segment.owner->imageName == OsageImageCode('S', 'O', 'T') && (segment.linkFlags & 0x1fU) < 2U) {
            maximumPitch = 90.0F;
        }

        std::int32_t baseDelta = static_cast<std::uint16_t>(segment.baseRotation) - previousBaseRotation;
        if (static_cast<std::uint32_t>(baseDelta + 182) > 364U) {
            baseDelta /= 10;
        } else if (baseDelta > 18) {
            baseDelta = 18;
        } else if (baseDelta < -18) {
            baseDelta = -18;
        }
        segment.baseRotation = static_cast<std::uint32_t>(previousBaseRotation + baseDelta);
        segment.rotX = OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = static_cast<std::int32_t>(segment.baseRotation);
        segment.rotZ = OsageFixedAngle(segment.currentAngleZ);

        constexpr std::array<OsageConstraintRange, 2> upperRanges{{{16, 2}, {18, 2}}};
        float pitch = ResolveOsagePitchUpward(segment, segment.oscillatedAngleX, maximumPitch, upperRanges.data(), upperRanges.size(), 0.015F);

        constexpr std::array<OsageConstraintRange, 6> bodyRanges{{
            {4, 4},
            {8, 2},
            {10, 2},
            {20, 4},
            {24, 2},
            {26, 2},
        }};
        if (maximumPitch > 0.0F) {

            Vector3 endpoint = EvaluateOsagePitchEndpoint(segment, pitch);
            while (!ConstrainOsageRanges(*segment.owner, endpoint, bodyRanges.data(), bodyRanges.size(), 0.015F)) {
                pitch = static_cast<float>(static_cast<double>(pitch) + 10.0);
                if (pitch > maximumPitch)
                    break;
            }
        }
        ResolveOsagePitchDownward(segment, pitch, bodyRanges.data(), bodyRanges.size(), 0.015F, maximumPitch);
        return;
    }
    case 124: {
        auto *const history = reinterpret_cast<float *>(&segment.oscillatedAngleX);
        for (std::int32_t index = 17; index >= 2; --index)
            history[index] = history[index - 2];
        if (chained) {
            const auto *const parentHistory = reinterpret_cast<const float *>(&segment.parent->oscillatedAngleX);
            segment.oscillatedAngleX = parentHistory[12];
            segment.currentAngleZ = parentHistory[13];
            if (segment.owner->imageName == OsageImageCode('N', 'E', 'R')) {
                segment.oscillatedAngleX = static_cast<float>(static_cast<double>(segment.oscillatedAngleX) * 1.2);
                segment.currentAngleZ = static_cast<float>(static_cast<double>(segment.currentAngleZ) * 1.2);
            }
        } else {
            const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
            float targetX = motion + windMagnitude;
            float targetY = targetX;
            if (segment.owner->imageName == OsageImageCode('N', 'E', 'R')) {
                const double target = std::fmaxf(1.5F, windMagnitude) + motion;
                targetX = static_cast<float>(target * 0.8);
                targetY = static_cast<float>(target * 1.2);
            }
            UpdateOsageWindPhases(segment, 0.2F, 0.2F, targetX, targetY, frameDelta);
            ApplyOsageTilt(segment, 10.0F, -15.0F, frameDelta, randomX, randomZ);
            segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
            segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        }
        segment.rotX = node.rotation.x + OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = node.rotation.y;
        segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
        break;
    }
    case 126: {
        const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
        const double wind = windMagnitude + windMagnitude;
        const float target = static_cast<float>(static_cast<double>(motion) * (chained ? 12.0 : 5.0) + wind);
        UpdateOsageWindPhases(segment, 0.2F, 0.2F, target, target, frameDelta);
        if (chained) {
            segment.angleX = segment.parent->angleX;
            segment.angleZ = segment.parent->angleZ;
        } else {
            ApplyOsageTilt(segment, 20.0F, -30.0F, frameDelta, randomX, randomZ);
        }
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        if (chained) {
            segment.rotX = node.rotation.x + OsageFixedAngle(segment.oscillatedAngleX);
            segment.rotY = node.rotation.y;
            segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
        } else {
            segment.rotX = OsageFixedAngle(segment.oscillatedAngleX);
            segment.rotY = static_cast<std::int32_t>(segment.baseRotation);
            segment.rotZ = OsageFixedAngle(segment.currentAngleZ);
            BuildOsageSegmentTransform(segment);
            return;
        }
        break;
    }
    case 125: {
        const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
        const std::uint8_t chainIndex = segment.linkFlags & 0x1fU;
        const std::uint8_t dynamicRoot = segment.owner->imageName == OsageImageCode('N', 'E', 'R') ? 1U : 0U;
        const bool dynamic = chainIndex == dynamicRoot || chainIndex == dynamicRoot + 1U;
        if (!dynamic) {
            if (node.childOffset && OsageOffsetNode(&node, node.childOffset)) {
                if (chainIndex) {
                    segment.angleX = segment.parent->angleX;
                    segment.angleZ = segment.parent->angleZ;
                }
                segment.rotX = node.rotation.x;
                segment.rotY = node.rotation.y;
                segment.rotZ = node.rotation.z;
                ApplyOsageFinalTransform(segment);
                return;
            }
            UpdateOsageWindPhases(segment, 0.2F, 0.2F, motion * 8.0F, motion * 8.0F, frameDelta);
            if (segment.parent) {
                segment.angleX = segment.parent->angleX;
                segment.angleZ = segment.parent->angleZ;
            }
            segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
            segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
            segment.rotX = node.rotation.x + OsageFixedAngle(segment.oscillatedAngleX);
            segment.rotY = node.rotation.y;
            segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
            ApplyOsageFinalTransform(segment);
            return;
        }

        UpdateOsageWindPhases(segment, 0.2F, 0.2F, motion * 6.0F, motion * 4.0F, frameDelta);
        if (chainIndex == dynamicRoot) {
            ApplyOsageTilt(segment, 20.0F, -30.0F, frameDelta, randomX, randomZ);
        } else {
            segment.angleX = segment.parent->angleX;
            segment.angleZ = segment.parent->angleZ;
        }
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        if (segment.oscillatedAngleX > 0.0F) {
            segment.oscillatedAngleX = -segment.oscillatedAngleX;
        }
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        segment.rotX = OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = static_cast<std::int32_t>(segment.baseRotation);
        segment.rotZ = OsageFixedAngle(segment.currentAngleZ);
        constexpr std::array<OsageConstraintRange, 1> ranges{{{4, 4}}};
        ResolveOsagePitchDownward(segment, segment.oscillatedAngleX, ranges.data(), ranges.size(), 0.01F);
        return;
    }
    case 127: {
        const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
        UpdateOsageWindPhases(segment, 0.2F, 0.2F, motion + motion, motion * 4.0F, frameDelta);
        ApplyOsageTilt(segment, 10.0F, -15.0F, frameDelta, randomX, randomZ);
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        constexpr std::array<OsageConstraintRange, 1> ranges{{{26, 2}}};
        bool collides = false;
        do {
            workTransformMatrix = nodeTransform;
            segment.rotX = node.rotation.x;
            segment.rotY = node.rotation.y + OsageFixedAngle(segment.oscillatedAngleX);
            segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
            ApplyOsageFinalTransform(segment);
            Vector3 endpoint = OsageEndpointFromTransform(segment);
            collides = ConstrainOsageRanges(*segment.owner, endpoint, ranges.data(), ranges.size(), 0.002F);
            if (!collides)
                break;
            segment.oscillatedAngleX -= 3.0F;
            if (segment.oscillatedAngleX < -90.0F)
                collides = false;
            segment.updateFlags |= 1U;
        } while (collides);
        return;
    }
    case 128: {
        const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
        UpdateOsageWindPhases(segment, 0.2F, 0.2F, motion + motion, motion * 4.0F, frameDelta);
        ApplyOsageTilt(segment, 10.0F, -15.0F, frameDelta, randomX, randomZ);
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        segment.rotX = node.rotation.x + OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = node.rotation.y;
        segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
        break;
    }
    case 130: {
        const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);
        const float wind = windMagnitude + windMagnitude;
        UpdateOsageWindPhases(segment, 0.2F, 0.2F, wind + (motion + motion), wind + motion * 4.0F, frameDelta);
        ApplyOsageTilt(segment, 10.0F, -15.0F, frameDelta, randomX, randomZ);
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        segment.rotX = node.rotation.x + OsageFixedAngle(segment.oscillatedAngleX);
        segment.rotY = node.rotation.y;
        segment.rotZ = node.rotation.z + OsageFixedAngle(segment.currentAngleZ);
        break;
    }
    case 129: {
        const bool usesExternal = UsesHairMotionOffset(segment.owner->imageName);
        const float reference = usesExternal ? g_OSAG_Val_2 : -1.0F;
        float wind = windMagnitude;
        if (static_cast<double>(reference) > 0.000001)
            wind = reference * 30.0F;
        const float motion = CalculateOsageMotionMagnitude(segment, inverseDelta);

        auto *const values = reinterpret_cast<float *>(&segment.oscillatedAngleX);
        float &offsetX = values[2];
        float &offsetZ = values[3];
        float &scale = values[4];
        float &scaleProduct = values[5];
        float &yawOffset = values[6];
        scale = 1.0F;
        offsetX = 0.0F;
        offsetZ = 0.0F;
        yawOffset = 0.0F;

        const auto negligible = [](float value) { return !(static_cast<double>(value) > 0.000001) && !(static_cast<double>(value) < -0.000001); };
        float targetX = 0.0F;
        float targetY = 0.0F;
        if (usesExternal) {
            offsetX = -1.5F;
            offsetZ = -1.5F;
            targetX = std::fminf(1.5F, (motion + motion) + wind);
            targetY = std::fminf(1.5F, motion * 3.0F + wind);
        } else if (segment.owner->imageName == OsageImageCode('K', 'O', 'K')) {
            const std::size_t side = (segment.linkFlags & 0x20U) != 0U ? 1U : 0U;
            const float controlScale = std::bit_cast<float>(g_OSAG_Val_1[side]);
            const float controlX = std::bit_cast<float>(g_OSAG_Val_0[side * 3U]);
            const float controlZ = std::bit_cast<float>(g_OSAG_Val_0[side * 3U + 1U]);
            const bool controlled = static_cast<double>(controlScale) > 0.000001 || !negligible(controlX) || !negligible(controlZ);
            if (chained) {
                const auto *const parentValues = reinterpret_cast<const float *>(&segment.parent->oscillatedAngleX);
                if (!controlled) {
                    offsetX = static_cast<float>(static_cast<double>(parentValues[2]) * 1.1);
                    offsetZ = static_cast<float>(static_cast<double>(parentValues[3]) * 1.1);
                }
                const float parentScale = parentValues[5];
                scale = static_cast<float>((static_cast<double>(parentScale) * 0.8 + 0.2) / static_cast<double>(parentScale));
                scaleProduct = scale * parentScale;
            } else {
                if (static_cast<double>(wind) > 0.1)
                    wind = 0.1F;
                targetX = std::fminf(3.0F, motion + wind);
                targetY = std::fminf(3.0F, (motion + motion) + wind);
                if (controlled) {
                    offsetX = controlX;
                    offsetZ = controlZ;
                    scale = static_cast<double>(controlScale) > 0.000001 ? controlScale : 1.0F;
                } else {

                    Vector3i handRotation{};
                    OsageHandRootRotation(segment, side, handRotation);
                    const Matrix4x4 saved = workTransformMatrix;
                    workTransformMatrix = transformMatrix_prePass;
                    Matrix4x4::SetViewMatrixFromEuler(static_cast<std::int16_t>(handRotation.y), 0, static_cast<std::int16_t>(handRotation.z));
                    Matrix4x4::preTranslate(-1.0F, 0.0F, 0.0F);
                    const Vector3 direction = OsageMatrixPosition(workTransformMatrix);
                    workTransformMatrix = saved;
                    offsetX = static_cast<float>(static_cast<double>(-direction.y) * 4.5);
                    offsetZ = static_cast<float>(static_cast<double>(-direction.z) * 4.5);
                    scale = static_cast<float>(1.0 - static_cast<double>(std::sqrt(direction.z * direction.z + direction.y * direction.y)) * 0.35);
                }
                scaleProduct = scale;
                yawOffset = std::bit_cast<float>(g_OSAG_Val_0[side * 3U + 2U]);
            }
        } else {
            targetX = (motion + motion) + wind;
            targetY = motion * 4.0F + wind;
        }

        UpdateOsageWindPhases(segment, 0.2F, 0.2F, targetX, targetY, frameDelta);
        ApplyOsageTilt(segment, 10.0F, -15.0F, frameDelta, randomX, randomZ);
        segment.oscillatedAngleX = OsageOscillation(segment.angleX, segment.windPhaseX);
        segment.currentAngleZ = OsageOscillation(segment.angleZ, segment.windPhaseY);
        segment.rotX = OsageFixedAngle(segment.oscillatedAngleX + offsetX);
        segment.rotY = OsageFixedAngle(yawOffset);
        segment.rotZ = OsageFixedAngle(offsetZ + segment.currentAngleZ);

        Matrix4x4::SetViewMatrixFromEuler(node.rotation.y, node.rotation.x, node.rotation.z);
        const auto rotateFixed = [](std::int32_t angle, auto &&rotate) {
            const auto fixed = static_cast<std::uint16_t>(angle);
            rotate(sin_fixed16(static_cast<std::int16_t>(fixed)), cos_fixed16(fixed));
        };
        rotateFixed(segment.rotZ, Matrix4x4::RotateAroundZ);
        rotateFixed(segment.rotX, Matrix4x4::RotateAroundX);
        rotateFixed(segment.rotY, Matrix4x4::RotateAroundY);
        Matrix4x4::Scale(1.0F, scale, 1.0F);
        segment.transform = workTransformMatrix;
        return;
    }
    case 131:
        segment.rotX = node.rotation.x;
        segment.rotY = node.rotation.y;
        segment.rotZ = node.rotation.z;
        break;
    case 141:
        ApplyOsageAnimatedBounce(segment, frameDelta, inverseDelta);
        break;
    default:
        ResetOsageSegment(segment);
        return;
    }
    ApplyOsageFinalTransform(segment);
    if (node.nodeKind == 131)
        UpdateOsageAuxiliaryRecords(segment);
}

void UpdateOsageNodeTree(OsageWork &osage, Mt5RuntimeNode *node, float cameraDistanceSquared, float frameDelta, float inverseDelta) {
    while (node) {
        PushCharacterMatrix();
        OsageSegmentRuntime *const segment = FindOsageSegment(osage, node);
        if (segment) {
            if (segment->nodeKind < 0) {
                InitializeOsageSegment(*segment);
            } else if ((Shenmue_nPauseState & 2) == 0) {
                segment->updateFlags = static_cast<std::uint8_t>((segment->updateFlags & 0xf0U) | (2U * (segment->updateFlags & 7U)));
                const float randomX = AdvanceOsageRandomOffset();
                const float randomZ = AdvanceOsageRandomOffset();
                const bool inRange = node->nodeKind == 120 ?
                    cameraDistanceSquared < osage.distanceLimit :
                    cameraDistanceSquared < ((node->nodeKind == 123 || node->nodeKind == 129 || node->nodeKind == 130) ? 36.0F : 4.0F);
                const bool supported = (node->nodeKind >= 120 && node->nodeKind <= 131) || node->nodeKind == 141;
                if (inRange && supported) {
                    ApplyRotationalOsageState(*segment, frameDelta, inverseDelta, randomX, randomZ);
                    if ((segment->updateFlags & 0xf0U) != 0U)
                        segment->updateFlags = static_cast<std::uint8_t>(segment->updateFlags + 0x10U);
                } else {
                    ResetOsageSegment(*segment);
                }
            }
            workTransformMatrix = segment->transform;
            BindOsageSegment(*segment);
        } else if (node->transformOffset) {
            if (const Matrix4x4 *const transform = HMDL_TransformForNode(node, node->transformOffset)) {
                workTransformMatrix = *transform;
            } else {
                ApplyOsageNodeLocalTransform(*node);
                node->transformOffset = 0;
            }
        } else {
            ApplyOsageNodeLocalTransform(*node);
        }
        if (Mt5RuntimeNode *const child = OsageOffsetNode(node, node->childOffset)) {
            UpdateOsageNodeTree(osage, child, cameraDistanceSquared, frameDelta, inverseDelta);
        }
        PopCharacterMatrix();
        node = OsageOffsetNode(node, node->siblingOffset);
    }
}
} // namespace

void OSAGE_Update(Character *character) {
    OsageWork *const osage = FindOsageWork(character);
    if (!osage || !osage->imageName || !g_pCurrentTaskData || !character->model)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    MotionInformationWork &motion = manager->motion;
    PushCharacterMatrix();
    UpdateOsageReferenceAngles(*osage, &motion);
    UpdateOsageCollisionPoints(*osage, motion);

    SelectCharacterMapQuery(character);

    float cameraDistanceSquared = 10000.0F;
    const bool cewpFlag = character->entity.name == SHEN_NAME(CEWP) &&
        ((character->flags.value & 1U) != 0U || ((character->flags.value & 2U) != 0U && (character->motionFlags & 1U) != 0U));
    if (g_pSCNC && g_pSCNC->header.active) {
        cameraDistanceSquared = 0.0F;
    } else if (cewpFlag && (character->motionFlags & 0x3fU) == 0x3fU && character->renderOpacity >= 0.01F) {
        const Vector3 position = GetPositionForEntity(character->entity.node);
        const float dx = g_CameraEyePos.x - position.x;
        const float dz = g_CameraEyePos.z - position.z;
        cameraDistanceSquared = dx * dx + dz * dz;
    }

    float rawDelta = osage->deltaOverride;
    if (rawDelta <= 0.000001F)
        rawDelta = motion.playbackRate;
    if (rawDelta <= 0.000001F)
        rawDelta = 1.0F;
    const float frameDelta = OsageFrameDelta(rawDelta);
    g_osageGroundHeight = -100000.0F;
    workTransformMatrix = transformMatrix_prePass;
    g_osageState121SharedValid = false;
    const Vector3 position = GetPositionForEntity(character->entity.node);
    const Vector3i rotation = GetEulerAnglesForEntity(character->entity.node);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    UpdateOsageNodeTree(*osage, static_cast<Mt5RuntimeNode *>(character->model), cameraDistanceSquared, frameDelta, 1.0F / rawDelta);
    PopCharacterMatrix();
}
namespace {

bool HasOsageNode(Mt5RuntimeNode *model) {
    if (!model)
        return false;
    for (const std::int16_t kind : kOsageNodeKinds) {
        if (kind < 0)
            break;
        if (shenGetNodeByChildIdx(model, kind))
            return true;
    }
    return false;
}

std::uint32_t NormalizeOsageImageName(std::uint32_t imageName) {
    if (imageName != OsageImageCode('L', 'J', 'O', 'J') && imageName != OsageImageCode('M', 'J', 'O', 'J')) {
        imageName &= 0x00ffffffU;
    }
    return imageName;
}

void ResetOsageImageControls(std::uint32_t imageName) {
    const std::uint32_t image = imageName & 0x00ffffffU;
    if (image == OsageImageCode('M', 'Z', 'N') || image == OsageImageCode('N', 'G', 'H')) {
        g_OSAG_Val = 0.0F;
    }
    if (UsesHairMotionOffset(image))
        g_OSAG_Val_2 = 0.0F;
    if (image == OsageImageCode('K', 'O', 'K')) {
        std::fill(std::begin(g_OSAG_Val_0), std::end(g_OSAG_Val_0), 0);
        std::fill(std::begin(g_OSAG_Val_1), std::end(g_OSAG_Val_1), 0);
    }
}

void InitializeOsageWork(Character *character, OsageWork &osage, Mt5RuntimeNode *model) {
    osage.character = character;
    osage.targetCharacter = nullptr;
    osage.faceData = nullptr;
    osage.targetFaceData = nullptr;
    osage.deltaOverride = 0.0F;
    osage.renderChain = 0;

    if (shenGetNodeByChildIdx(model, 120)) {
        osage.distanceLimit = 144.0F;
    } else if (shenGetNodeByChildIdx(model, 123) || shenGetNodeByChildIdx(model, 129) || shenGetNodeByChildIdx(model, 130)) {
        osage.distanceLimit = 36.0F;
    } else {
        osage.distanceLimit = 4.0F;
    }

    osage.imageName = NormalizeOsageImageName(IMGM_GetImageName(character));
    if (!osage.imageName)
        return;
    osage.faceData = const_cast<OsageCollisionProfile *>(&OsageCollisionProfileForImage(osage.imageName));
    const std::uint32_t image = osage.imageName & 0x00ffffffU;
    if (image == OsageImageCode('M', 'H', 'H') || image == OsageImageCode('N', 'H', 'H') || image == OsageImageCode('T', 'H', 'H') ||
        image == OsageImageCode('Y', 'H', 'H')) {
        osage.distanceLimit = 2500.0F;
    }
    ResetOsageImageControls(osage.imageName);

    PushCharacterMatrix();
    workTransformMatrix = transformMatrix_prePass;
    hmdlApplyOsage(osage, model);
    PopCharacterMatrix();
}
} // namespace

void OSAGE_Alloc(Character *character) {
    if (!character || !character->model || HLib_GetCharacterComponent(character, SHEN_NAME(OSAG))) {
        return;
    }

    Mt5RuntimeNode *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!HasOsageNode(model))
        return;
    OsageWork *const osage = static_cast<OsageWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(OSAG), sizeof(OsageWork), OSAGE_NoOp));
    if (!osage)
        return;
    HLib_GetCharacterComponent(character, SHEN_NAME(OSAG))->destroyCallback = OSAGE_Cleanup;
    InitializeOsageWork(character, *osage, model);
}

void OSAGE_Refresh(Character *character) {
    if (!character)
        return;
    OsageWork *osage = FindOsageWork(character);
    if (osage) {
        OSAGE_Cleanup(character, osage);
        osage->imageName = 0;
        osage->faceData = nullptr;
        osage->deltaOverride = 0.0F;
    }
    if (!character->model)
        return;
    Mt5RuntimeNode *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!HasOsageNode(model))
        return;
    if (!osage) {
        osage = static_cast<OsageWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(OSAG), sizeof(OsageWork), OSAGE_NoOp));
        if (!osage)
            return;
        HLib_GetCharacterComponent(character, SHEN_NAME(OSAG))->destroyCallback = OSAGE_Cleanup;
    }
    InitializeOsageWork(character, *osage, model);
}

void OSAGE_Initialize(Character *character) {
    OsageWork *const osage = FindOsageWork(character);
    for (auto *segment = osage ? reinterpret_cast<OsageSegmentRuntime *>(static_cast<std::uintptr_t>(osage->renderChain)) : nullptr; segment;
         segment = segment->next) {
        segment->updateFlags = static_cast<std::uint8_t>((segment->updateFlags & 0x0fU) | 0x80U);
    }
    CLMD_SetFlags(character, 0x10U, 1);
}

void Script_osageSetTarget(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params || !params[0])
        return;
    Character *const owner = ScenarioCharacter(context, params[0]);
    CharacterComponentNode *const component = owner ? HLib_GetCharacterComponent(owner, SHEN_NAME(OSAG)) : nullptr;
    auto *const osage = component ? reinterpret_cast<OsageWork *>(component + 1) : nullptr;
    if (!osage)
        return;
    osage->targetFaceData = nullptr;
    Character *const target = ScenarioCharacter(context, params[1]);
    osage->targetCharacter = target;
    if (target) {
        osage->targetFaceData = const_cast<OsageCollisionProfile *>(&OsageCollisionProfileForImage(IMGM_GetImageName(target)));
    }
}
} // namespace shenmue::s1
