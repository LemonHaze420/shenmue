#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/maze.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/pause.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/world_render.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/math.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "s2/scene_assets_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/weather_data_dc.hpp"
#include "s2/weather_particles_dc.hpp"
#include "s2/file_loader_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/hlib.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/map_info.hpp"
#include "s2/memo_flags_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/player_stats_dc.hpp"
#define UI_ZERO
#define UI_ICON(name) name
#include "s2/memo.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/font.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/window.hpp"
#include "s2/money_icon.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/gauge.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/sound_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/memo_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/model_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#define UI_STRLEN ::strlen
#define UI_NOEXCEPT
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include <cstring>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/game_runtime.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/pak.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/character.hpp"
#include "s2/fish.hpp"
#include "s2/mt7.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/weather.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/navi.hpp"
#include "d3t/object_store.hpp"

#include <array>
#include <bit>
#include <cctype>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cmath>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "s2/directory_cache.hpp"
#include "s2/load_title_dc.hpp"
#include <sg_gd.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/startup_assets_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/texture_dc.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "common/math.hpp"
#include "platform/dc/screen.hpp"
#define SPRITE_NULL 0
#define SPRITE_ZERO
#define SPRITE_INIT =
#define SPRITE_ARRAY(type, name, count) type name[count]
#define SPRITE_DATA(name) (name)
#define SPRITE_MEMCPY ::memcpy
#define SPRITE_ALIGN(name) name
#define SPRITE_MODE(name) name
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cstdio>
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHENMUE_MOTION_DATA(name) name
#define SHENMUE_MOTION_NAME(value) shenmue::shen_name(value)
#include "s2/character_yaw_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/motion_task_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/model_clone_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/field_collision_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/object_light_dc.hpp"
#include "s2/map_models_dc.hpp"
#endif

namespace shenmue {
namespace dc {
namespace sdk {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMirroredModelDrawMode(std::int32_t enabled);
#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_transparency_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/prop_dc.hpp"
#include "auto_test.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/title_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cstring>
#include <vector>
#endif


namespace shenmue {
namespace s2 {

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct AuthoredUnit {
    const char *model;
    const Vector2 *const *paths;
    std::int32_t areaIndex;
};

struct MazeUnitArea {
    Vector2 quad[4];
    std::int16_t pointCount, reserved22;
    Vector2 points[3];
    std::int16_t wallCount, reserved3e;
    Vector4 walls[3];
};
typedef char MazeUnitAreaSizeCheck[sizeof(MazeUnitArea) == 0x70 ? 1 : -1];

struct EndpointNeighbor {
    std::int16_t partition, element;
};

struct EndpointLinks {
    EndpointNeighbor neighbors[2];
    std::uint16_t visited;
};

struct RouteSearch {
    std::int32_t partition, element, targetPartition, targetElement, nextPartition, nextElement;
    std::int32_t retained18, sourcePartition, sourceElement;
};
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct MazeAuthoredObject {
    shen_name name;
    const char *resource;
    MT7Model *model;
    std::uint32_t flags;
};
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct MazeRoadDecoration {
    shen_name area;
    std::int32_t marker, block, index;
    Vector3 position;
    std::int32_t yaw, kind;
};

struct MazeSearchTargets {
    shen_name area;
    SearchTarget targets[4];
};

struct MazePropState {
    std::int32_t kind;
    std::uint32_t flags, drawState;
    std::int32_t yaw;
    float x, z, height;
};

typedef char MazePropStateSizeCheck[sizeof(MazePropState) == 0x1c ? 1 : -1];
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef std::int32_t (*ItemWindowShape)(std::int32_t cell, std::int32_t columns);
PakGlobalData *RequestMapPackage(const char *directory, const char *name);
std::int32_t CountMazePointerList(const void *const *list);
std::int32_t CountMazePathPoints(const Vector2 *points);
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeUnit(MazeRoad *road);
bool InsideMazeQuad(const Vector3 *first, const Vector3 *second, const Vector3 *third, const Vector3 *fourth, const Vector3 *point);
void DrawMazeFlutterEffects(const MazeRoad *road);
std::uint32_t MazeFlutterPattern(std::uint32_t distance, std::uint32_t random);
void DrawMazeButterflyEffects(const MazeRoad *road);
void DrawMazeSkyModel();
void DrawMazeUnitAuxiliary(const MazeRoad *road);
void AssignMazeMarkerDecorationRoads();
void SetMazeSearchTargets();
void LoadMazeAuxiliaryModels();
void LoadMazeAuthoredObjectModels();
void LoadMazeTreeModels();
void DrawMazeAuxiliaryVariation(const MazeRoad *road, shen_name name);
void DrawMazeAuthoredObjects(shen_name name, const MazeRoad *road);
void GetMazeMarkerSlotPosition(std::int32_t slot, Vector3 *output);
void DrawMazeAuthoredAnimatedObject(MazeAuthoredObject *object);
void DrawMazeAuthoredHourObject(MazeAuthoredObject *object);
void DrawMazeHourObjectMeshes(MazeAuthoredObject *object);
void *AllocateMaze(std::size_t bytes, std::uint32_t name);
void DrawMazeRoad(MazeRoad *road);
void DrawMazeRoadProps(const MazeRoad *road);
void InitializeMazeHourAnimation();
void GenerateMazePropStates(const MazeRoad *road, MazePropState *output);
std::uint32_t MazePropRecordSelected(const MazePropRecord *record, const MazeRoad *road);
const MazePropRecord *SelectMazePropChoice(const MazePropRecord *record);
std::int32_t MazeRecordMatchesIndex(std::uint32_t flags, std::int32_t index);
MazePropState *MakeMazePropState(const MazePropRecord *record, const MazeRoad *road, MazePropState *output);
void DrawMazePropStates(const MazePropState *states);
float GetMazeBob();
void CopyMazeMatrixBasis(const Matrix4x4 *source);
void InitializeMazeFrameEnvironment();
std::int32_t UpdateMazeRoadTransforms(MazeRoad *roads, const Vector3 *camera, float *nearestDistance);
void ClearMazeVisibleLists();
void InsertMazeRoad(MazeRoad *road);
void InsertMazeUnit(MazeRoad *road);
void DrawMazeAuthoredFish(const MazeAuthoredObject *object, const MazeRoad *road);
void SetMazeNodeWiggle(std::uint32_t phase, MT7Node *node, std::int32_t amplitude, std::uint32_t axes);
void FindMazeHangingUnitModels();
void SampleMazeSpline(const Vector3 *points, std::int32_t count, std::int32_t start, Vector3 *output);
void BuildMazeUnit(MazeRoad *road, MazeUnit *unit);
bool MazeRoadPassesCameraAngle(const MazeRoad *road, std::int32_t unit);
std::int32_t MazeUnitHasVisiblePoint(const MazeRoad *road);
std::int32_t MazeVisibleRoadsBlock(const Vector3 *camera, const Vector3 *target);
bool MazeUnitWallsBlock(const MazeRoad *road, const Vector3 *camera, const Vector3 *target);
bool MazeSegmentsIntersect(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d);
void ApplyMazeRoadJoins(MazeRoad *road);
void FilterMazeVisibleRoads();
std::int32_t GetAuthoredWord(std::int32_t block, std::int32_t record);
std::int32_t GetAuthoredRecordStyle(std::int32_t block, std::int32_t record);
std::uint32_t SearchRoute(RouteSearch &search, std::int32_t endpoint);
std::int32_t MazeRouteElementDistance(std::int32_t first, std::int32_t second);
void ClearMazeRouteVisited();
void StoreMazePartition(std::int32_t partition, const Vector3 *points);
void CopyMazePartitionPoints(Vector3 *output, const Vector3 *points);
Vector3 *CollectMazePartitionStartUnit(MazeRoad *road, Vector3 *output);
Vector3 *AppendMazeRoadsOutsideUnits(MazeRoad *road, Vector3 *output);
std::int32_t ClassifyMazeRoadUnit(MazeRoad *road);
std::int32_t GetMazeUnitTraversalDirection(MazeRoad *road);
Vector3 *AppendMazeUnitPath(std::int32_t unitIndex, std::int32_t list, std::int32_t direction, Vector3 *output);
void CopyMazeVector3(Vector3 *output, const Vector3 *source);
void TerminateMazeRoutePoints(Vector3 *point);
MazeRoad *SeekOutsideMazeUnit(MazeRoad *road);
MazeRoad *SeekInsideMazeUnit(MazeRoad *road);
std::int32_t CountMazePartitionPoints(const Vector3 *points);
void FindNearestMazeUnitPoint(const Vector3 *point, std::int32_t unit, std::int32_t *list, std::int32_t *element);
std::int32_t GetMazeUnitListCount(std::int32_t unit);
std::int32_t MazePathCount(std::int32_t unit, std::int32_t list);
void GetMazeUnitPathPoint(std::int32_t unit, std::int32_t list, std::int32_t element, Vector3 *output);
void MazeFrameNullsub();
void InitializeMazeRenderModelNullsub();
void MarkMazeUnitModelsUsed();
void MarkMazeMediumModelsUsed();
void MarkMazePropTableModelsUsed(std::int32_t style);
std::int32_t MazeRoadStyle(std::uint32_t flags);
void LoadMazeRoadModels();
MT7Model *GetMazeRoadModel(const MazeRoad *road);
MazePropRecord *GetMazePropTable(std::int32_t style);
void SetMazeRoadCount(std::int32_t block, std::int32_t count);
MazeRoad *BuildMazeRoads(std::int32_t block, Vector3 *points, std::int32_t count);
float MeasureMazeSpline(const Vector3 *points, std::int32_t count);
std::int32_t PlaceMazeRoads(std::int32_t block, const Vector3 *points, std::int32_t count, MazeRoad *road);
Vector3 **BuildMazeUnitPaths(const Vector2 *const *paths);
void InitMazeJoinVector(Vector3 *vector);
void LoadMazeMarkers();
void LoadMazePackage();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern char mazeLoadFilename[32];
const char *mazeMapNames[] = {"qte", "kmz1", "kmz2", "kmz3", "kmz4"};
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t FindNearestMazeFirstBlockRoad(const Vector3 *position);
void FindNearestMazeRoad(const Vector3 *position, std::int32_t *block, std::int32_t *index);
std::int32_t FindMazeUnit(const Vector3 *position);
MazeUnit *GetMazeUnit(std::int32_t index);
MazeRoad *GetMazeRoad(std::int32_t block, std::int32_t index);
std::int32_t FindMazeMarkerWithinRadius(const Vector3 *position, float radius);
std::int32_t InitializeMazeObjectTransforms(MazeRoad **blocks, const Vector3 *camera, float *nearestDistance);
void InitializeMazeNavigationTrail();
bool IsPositionWithinMazeMarkerRadius(const Vector3 *position, std::int32_t marker, float radius);
void mazeSamplePartition(std::int32_t partition, std::int32_t element, Vector3 *output);
void SetMazeHourAnimationFrame(std::int32_t frame);
void InitializeMazeFishPath();
void DrawMazeFish(const Vector3 *boxMin, const Vector3 *boxMax, std::int32_t count, std::int32_t seed, const MT7Node *fish);
void mazeSetCurrentRefCount(std::uint32_t count);
std::uint32_t mazeGetCurrentRefCount();
std::uint32_t GetMazeBlockCount();
std::int32_t GetAuthoredRecordYaw(std::int32_t block, std::int32_t record);
std::int32_t mazeFindPartitionDistance(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *nextElement);
void mazeInit();
std::int32_t GetMazeRoadCountRaw(std::int32_t block);
std::int32_t GetMazeRoadStyle(const MazeRoad *road);

std::uint32_t GetCurrentItemSelectState();
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
std::uint32_t GetCurrentItemSelectState();
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
MazeUnit *MUZT_Data = 0;
std::int32_t g_mazeMode, g_mazeUnitCount = 0, g_mazePartitionCount = 0, g_mazeMarkerCount = 0;
MazeRoad *g_mazeRoadBlocks[16];
MazePartition g_mazePartitions[32];
MazeMarker *g_mazeMarkers = 0;
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const Vector2 unit2Path0[] = {
    {0.9559339880943298f, 13.85644245147705f},
    {0.17282700538635254f, 15.684398651123047f},
    {-0.0011970000341534615f, 17.251218795776367f},
    {-0.0011970000341534615f, 18.46985626220703f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit2Path1[] = {
    {1.2169690132141113f, 12.115531921386719f},
    {0.7095990180969238f, 10.068550109863281f},
    {0.31189200282096863f, 8.043073654174805f},
    {0.02264999970793724f, 6.632472991943359f},
    {-1.098162055015564f, 4.82401180267334f},
    {-1.4235730171203613f, 3.232564926147461f},
    {-1.2789520025253296f, 2.111319065093994f},
    {-0.664313018321991f, 1.243257999420166f},
    {1.9027080535888672f, -0.5290340185165405f},
    {3.5676960945129395f, -1.4694340229034424f},
    {4.688508987426758f, -1.758787989616394f},
    {8.014789581298828f, -1.975803017616272f},
    {10.473345756530762f, -1.7226179838180542f},
    {12.283597946166992f, -0.9992340207099915f},
    {14.452912330627441f, -0.16734200716018677f},
    {15.899121284484863f, 0.12201199680566788f},
    {17.417640686035156f, 0.15818099677562714f},
    {18.683074951171875f, 0.15818099677562714f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit2Path2[] = {
    {2.3296759128570557f, 12.802322387695312f},
    {3.829200029373169f, 12.644415855407715f},
    {5.635041236877441f, 12.439910888671875f},
    {6.927080154418945f, 11.885965347290039f},
    {8.1268310546875f, 11.193532943725586f},
    {9.280437469482422f, 9.947155952453613f},
    {9.8803129196167f, 8.469968795776367f},
    {9.972600936889648f, 7.038942813873291f},
    {9.14134407043457f, 5.561756134033203f},
    {7.710872173309326f, 3.992244005203247f},
    {6.463959217071533f, 2.145759105682373f},
    {6.048661231994629f, 0.25311198830604553f},
    {6.094804763793945f, -2.424290895462036f},
    {6.0022478103637695f, -3.9476420879364014f},
    {5.1716508865356445f, -4.778560161590576f},
    {3.7873239517211914f, -5.240180969238281f},
    {2.1722750663757324f, -5.147857189178467f},
    {-0.9206590056419373f, -4.224614143371582f},
    {-3.320159912109375f, -3.4398579597473145f},
    {-6.829052925109863f, -2.2858049869537354f},
    {-10.153417587280273f, -1.2240760326385498f},
    {-12.045331954956055f, -0.6239680051803589f},
    {-14.029535293579102f, -0.023861000314354897f},
    {-15.690728187561035f, 0.16078799962997437f},
    {-17.16734504699707f, 0.2992739975452423f},
    {-18.413238525390625f, 0.2992739975452423f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit2Paths[] = {unit2Path0, unit2Path1, unit2Path2, 0};

static const Vector2 unit5Path0[] = {
    {-0.01992099918425083f, 1.3089890480041504f},
    {-1.122668981552124f, 2.6485579013824463f},
    {-1.8315789699554443f, 4.697311878204346f},
    {-2.225416898727417f, 7.455248832702637f},
    {-1.8315789699554443f, 10.449580192565918f},
    {-1.122668981552124f, 12.73472785949707f},
    {-0.4137600064277649f, 15.177472114562988f},
    {-0.01992099918425083f, 18.093006134033203f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit5Path1[] = {
    {-1.122668981552124f, -0.5821679830551147f},
    {-2.382952928543091f, -1.606544017791748f},
    {-4.352146148681641f, -2.47332501411438f},
    {-6.242570877075195f, -2.5521230697631836f},
    {-8.054227828979492f, -2.079334020614624f},
    {-10.180956840515137f, -1.0549570322036743f},
    {-12.15014934539795f, -0.10937900096178055f},
    {-13.961807250976562f, -0.10937900096178055f},
    {-16.403606414794922f, -0.030579999089241028f},
    {-18.372798919677734f, 0.048218000680208206f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit5Path2[] = {
    {1.0040589570999146f, 0.048218000680208206f},
    {2.4218780994415283f, -0.6609659790992737f},
    {4.233534812927246f, -1.0549570322036743f},
    {6.517798900604248f, -1.2913520336151123f},
    {9.510972023010254f, -1.0549570322036743f},
    {11.71646785736084f, -0.6609659790992737f},
    {14.47333812713623f, -0.34577301144599915f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit5Paths[] = {unit5Path0, unit5Path1, unit5Path2, 0};

static const Vector2 unit6Path0[] = {
    {-0.5f, 1.3860000371932983f},
    {1.0f, -0.35100001096725464f},
    {1.899999976158142f, -2.109999895095825f},
    {3.200000047683716f, -4.8680009841918945f},
    {5.300000190734863f, -6.126999855041504f},
    {7.5f, -5.997000217437744f},
    {10.699999809265137f, -4.454999923706055f},
    {13.300000190734863f, -1.4160000085830688f},
    {15.800000190734863f, -0.026000000536441803f},
    {18.19999885559082f, 0.25600001215934753f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit6Path1[] = {
    {-0.800000011920929f, 3.7070000171661377f},
    {-0.20000000298023224f, 5.791999816894531f},
    {0.20000000298023224f, 8.9350004196167f},
    {0.30000001192092896f, 11.791000366210938f},
    {0.30000001192092896f, 14.358000755310059f},
    {0.30000001192092896f, 17.084001541137695f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit6Path2[] = {
    {-3.0f, 1.878000020980835f},
    {-5.0f, 1.0119999647140503f},
    {-7.5f, 0.3700000047683716f},
    {-10.59999942779541f, -0.14300000667572021f},
    {-14.200000762939453f, -0.14300000667572021f},
    {-17.19999885559082f, -0.2070000022649765f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit6Paths[] = {unit6Path0, unit6Path1, unit6Path2, 0};

static const Vector2 unit7Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit7Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit7Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit7Paths[] = {unit7Path0, unit7Path1, unit7Path2, 0};

static const Vector2 unit8Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit8Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit8Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit8Paths[] = {unit8Path0, unit8Path1, unit8Path2, 0};

static const Vector2 unit9Path0[] = {
    {-0.30000001192092896f, 14.09999942779541f},
    {-0.4000000059604645f, 16.100000381469727f},
    {-0.10000000149011612f, 18.399999618530273f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit9Path1[] = {
    {-1.899999976158142f, 12.0f},
    {-4.0f, 10.100000381469727f},
    {-6.400000095367432f, 8.699999809265137f},
    {-9.199999809265137f, 7.400000095367432f},
    {-11.0f, 6.099999904632568f},
    {-12.600000381469727f, 3.5f},
    {-14.40000057220459f, 1.2000000476837158f},
    {-16.399999618530273f, 0.20000000298023224f},
    {-18.299999237060547f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit9Path2[] = {
    {1.2999999523162842f, 12.0f},
    {3.299999952316284f, 9.800000190734863f},
    {6.199999809265137f, 7.900000095367432f},
    {7.900001049041748f, 4.699999809265137f},
    {9.600000381469727f, 2.0999999046325684f},
    {12.09999942779541f, 0.30000001192092896f},
    {15.200000762939453f, -0.30000001192092896f},
    {18.100000381469727f, -0.20000000298023224f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit9Paths[] = {unit9Path0, unit9Path1, unit9Path2, 0};

static const Vector2 unit10Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit10Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 unit10Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit10Paths[] = {unit10Path0, unit10Path1, unit10Path2, 0};

static const Vector2 unit11Path0[] = {
    {0.11362899839878082f, 17.5744686126709f},
    {0.08527199923992157f, 15.617023468017578f},
    {0.3121339976787567f, 13.914895057678223f},
    {1.1345109939575195f, 12.865249633789062f},
    {2.2688241004943848f, 11.843973159790039f},
    {3.374778985977173f, 10.822696685791016f},
    {3.830336093902588f, 9.48935604095459f},
    {4.142271995544434f, 7.3900651931762695f},
    {4.227344989776611f, 4.836874008178711f},
    {3.376599073410034f, 2.368788957595825f},
    {2.0721399784088135f, 0.4680800139904022f},
    {0.25589001178741455f, -0.8936219811439514f},
    {-1.3037899732589722f, -2.368799924850464f},
    {-2.609050989151001f, -4.780147075653076f},
    {-3.176448106765747f, -7.9858222007751465f},
    {-3.034658908843994f, -10.36880111694336f},
    {-2.2119200229644775f, -12.581570625305176f},
    {-1.134323000907898f, -14.198591232299805f},
    {-0.3686619997024536f, -15.276605606079102f},
    {0.028348000720143318f, -16.723413467407227f},
    {0.05670500174164772f, -17.602846145629883f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit11Paths[] = {unit11Path0, 0};

static const Vector2 unit12Path0[] = {
    {0.04133100062608719f, 7.938899993896484f},
    {0.08983299881219864f, 3.4265151023864746f},
    {0.08983299881219864f, -0.8432689905166626f},
    {-0.24964000284671783f, -4.773410797119141f},
    {-0.007133000064641237f, -7.781668186187744f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit12Paths[] = {unit12Path0, 0};

static const Vector2 unit13Path0[] = {
    {-0.1207439973950386f, 8.045559883117676f},
    {-0.15051999688148499f, 6.16896390914917f},
    {-0.0016070000128820539f, 4.143433094024658f},
    {0.41525301337242126f, 2.1476879119873047f},
    {0.7725620269775391f, -0.5033760070800781f},
    {0.593779981136322f, -3.0352909564971924f},
    {0.20644299685955048f, -5.179973125457764f},
    {0.05757699906826019f, -6.550185203552246f},
    {0.027800999581813812f, -8.426780700683594f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit13Paths[] = {unit13Path0, 0};

static const Vector2 unit14Path0[] = {
    {0.0f, 18.0f},
    {0.0f, 17.0f},
    {0.8999999761581421f, 15.0f},
    {2.5f, 13.0f},
    {3.799999952316284f, 11.5f},
    {5.099999904632568f, 9.0f},
    {6.5f, 6.0f},
    {7.400000095367432f, 3.5f},
    {7.400001049041748f, 2.0f},
    {6.199999809265137f, 0.5f},
    {4.300000190734863f, -1.5f},
    {3.799999952316284f, -3.0f},
    {3.4000000953674316f, -6.0f},
    {2.799999952316284f, -8.5f},
    {1.5f, -11.0f},
    {-0.20000000298023224f, -13.5f},
    {-0.699999988079071f, -15.5f},
    {-0.6000000238418579f, -17.5f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit14Paths[] = {unit14Path0, 0};

static const Vector2 unit16Path0[] = {
    {0.0f, 8.899999618530273f},
    {0.0f, 7.099999904632568f},
    {0.0f, 4.699999809265137f},
    {0.0f, 2.0f},
    {0.0f, -1.0f},
    {0.0f, -3.299999952316284f},
    {0.0f, -6.0f},
    {0.0f, -8.0f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit16Paths[] = {unit16Path0, 0};

static const Vector2 unit17Path0[] = {
    {0.10000000149011612f, 9.137001037597656f},
    {0.10000000149011612f, 7.125999927520752f},
    {-0.10000000149011612f, 5.364999771118164f},
    {-0.10000000149011612f, 3.753000020980835f},
    {0.10000000149011612f, 1.4470000267028809f},
    {0.30000001192092896f, -0.04100000113248825f},
    {0.4000000059604645f, -1.5360000133514404f},
    {0.30000001192092896f, -3.494999885559082f},
    {0.20000000298023224f, -5.197000980377197f},
    {0.10000000149011612f, -7.061999797821045f},
    {0.10000000149011612f, -8.836999893188477f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit17Paths[] = {unit17Path0, 0};

static const Vector2 unit21Path0[] = {
    {0.13924099504947662f, 7.984930992126465f},
    {0.17633099853992462f, 5.944645881652832f},
    {0.02797199971973896f, 3.5704970359802246f},
    {-0.027672000229358673f, 0.6770049929618835f},
    {-0.23171000182628632f, -2.661642074584961f},
    {-0.2874009907245636f, -5.888998985290527f},
    {-0.13904200494289398f, -8.09621524810791f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit21Paths[] = {unit21Path0, 0};

static const Vector2 unit23Path0[] = {
    {0.0f, 22.0f},
    {0.0f, 21.0f},
    {0.05650800094008446f, 18.606828689575195f},
    {0.17621000111103058f, 15.568537712097168f},
    {0.6230649948120117f, 13.185556411743164f},
    {1.2483550310134888f, 10.77278995513916f},
    {1.0994759798049927f, 7.615344047546387f},
    {0.08679000288248062f, 5.768537998199463f},
    {-1.6406739950180054f, 4.0110931396484375f},
    {-3.904664993286133f, 1.8962010145187378f},
    {-5.364343166351318f, -0.48677799105644226f},
    {-5.662099838256836f, -1.8569910526275635f},
    {-5.691934108734131f, -4.299543857574463f},
    {-5.334601879119873f, -5.610180854797363f},
    {-3.8752119541168213f, -7.9633708000183105f},
    {-1.939054012298584f, -10.882519721984863f},
    {-0.7173389792442322f, -14.218689918518066f},
    {-0.09177300333976746f, -17.614431381225586f},
    {0.02738800086081028f, -20.02718734741211f},
    {-0.0023880000226199627f, -21.96335792541504f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit23Paths[] = {unit23Path0, 0};

static const Vector2 unit27Path0[] = {
    {0.0f, -3.869999885559082f},
    {-1.600000023841858f, -3.4100000858306885f},
    {-2.700000047683716f, -2.2300000190734863f},
    {-3.3399999141693115f, -1.1100000143051147f},
    {-3.640000104904175f, 0.6800000071525574f},
    {-2.5999999046325684f, 2.609999895095825f},
    {-1.1699999570846558f, 4.070000171661377f},
    {-0.12999999523162842f, 6.150000095367432f},
    {0.03999999910593033f, 8.09000015258789f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit27Paths[] = {unit27Path0, 0};

static const Vector2 unit28Path0[] = {
    {-0.9522500038146973f, 7.930092811584473f},
    {-1.4557980298995972f, 6.362894058227539f},
    {-1.6240689754486084f, 4.068066120147705f},
    {-1.5401769876480103f, 0.7657529711723328f},
    {-1.2604069709777832f, -2.480587959289551f},
    {-0.365092009305954f, -5.587000846862793f},
    {-0.08529999852180481f, -7.126214981079102f},
    {-0.029349999502301216f, -8.273628234863281f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit28Paths[] = {unit28Path0, 0};

static const Vector2 unit29Path0[] = {
    {-5.978970050811768f, -14.110830307006836f},
    {-7.628101825714111f, -11.809833526611328f},
    {-8.539463996887207f, -9.899574279785156f},
    {-10.23199462890625f, -7.8156538009643555f},
    {-11.664944648742676f, -6.0356340408325195f},
    {-12.619705200195312f, -3.560978889465332f},
    {-13.010289192199707f, -1.042907953262329f},
    {-12.489510536193848f, 1.518576979637146f},
    {-11.231013298034668f, 4.470798015594482f},
    {-9.364890098571777f, 6.728377819061279f},
    {-6.499682903289795f, 9.593769073486328f},
    {-3.4618079662323f, 12.415743827819824f},
    {-1.2476069927215576f, 15.628458976745605f},
    {-0.5966339707374573f, 17.538719177246094f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit29Paths[] = {unit29Path0, 0};

static const Vector2 unit30Path0[] = {
    {0.16825300455093384f, 4.084362030029297f},
    {0.15165899693965912f, 5.826985836029053f},
    {0.12224899977445602f, 7.894902229309082f},
    {0.13506600260734558f, 10.042478561401367f},
    {0.0696600005030632f, 12.781872749328613f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit30Paths[] = {unit30Path0, 0};

static const Vector2 unit31Path0[] = {
    {3.4000000953674316f, -2.9570000171661377f},
    {3.299999952316284f, -0.039000000804662704f},
    {2.299999952316284f, 2.7980000972747803f},
    {0.8999999761581421f, 5.392000198364258f},
    {0.0f, 7.90500020980835f},
    {-0.6000000238418579f, 10.905000686645508f},
    {-0.800000011920929f, 13.039999961853027f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit31Paths[] = {unit31Path0, 0};

static const Vector2 unit32Path0[] = {
    {0.0f, 22.0f},
    {0.0f, 20.0f},
    {0.0f, 18.0f},
    {0.18047699332237244f, 15.699812889099121f},
    {0.9443749785423279f, 11.980733871459961f},
    {1.1480820178985596f, 9.586258888244629f},
    {0.5369629859924316f, 7.242730140686035f},
    {-1.3485020399093628f, 4.593527793884277f},
    {-3.3346381187438965f, 2.3009450435638428f},
    {-5.0661420822143555f, -0.09352999925613403f},
    {-5.6273980140686035f, -2.589898109436035f},
    {-5.525545120239258f, -4.882481098175049f},
    {-4.455692768096924f, -7.480741024017334f},
    {-2.5204830169677734f, -10.079001426696777f},
    {-0.8897690176963806f, -13.798078536987305f},
    {-0.2786499857902527f, -16.90580177307129f},
    {-0.023899000138044357f, -19.809736251831055f},
    {0.02702699974179268f, -21.898534774780273f},
    {10000.0f, 10000.0f},
};

static const Vector2 *const unit32Paths[] = {unit32Path0, 0};

static const AuthoredUnit authoredUnits[] = {
    {"", 0, 10},
    {"", 0, 10},
    {"UNTIIT2", unit2Paths, 9},
    {"", 0, 10},
    {"", 0, 10},
    {"UNTSOT2", unit5Paths, 9},
    {"UNTH3T1", unit6Paths, 9},
    {"UNTHIY1", unit7Paths, 9},
    {"UNTHIY2", unit8Paths, 9},
    {"UNTHIY3", unit9Paths, 9},
    {"UNTHIY4", unit10Paths, 9},
    {"UNTIIA1", unit11Paths, 6},
    {"UNTIIA2", unit12Paths, 4},
    {"UNTIIA3", unit13Paths, 4},
    {"UNTIIA4", unit14Paths, 8},
    {"UNTIIA5", 0, 7},
    {"UNTYI01", unit16Paths, 4},
    {"UNTY302", unit17Paths, 4},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"UNTHIA1", unit21Paths, 4},
    {"", 0, 10},
    {"UNTHOA0", unit23Paths, 6},
    {"UNTSRA1", 0, 6},
    {"UNTSRA2", 0, 6},
    {"UNTRIA1", 0, 6},
    {"UNTHIE1", unit27Paths, 0},
    {"UNTH3A3", unit28Paths, 4},
    {"UNTH3E1", unit29Paths, 3},
    {"UNTH3E2", unit30Paths, 1},
    {"UNTI3E1", unit31Paths, 2},
    {"UNTHOS0", unit32Paths, 6},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"QTE2RA1", 0, 4},
    {"QTE1OH0", 0, 4},
    {"QTE13T1", 0, 4},
    {"QTE1RJ1", 0, 4},
    {"QTE2OB0", 0, 4},
    {"QTE1OR0", 0, 4},
    {"QTE2OD0", 0, 4},
    {"QTE1IK1", 0, 4},
    {"QTE1OL0", 0, 4},
    {"QTE1OH1", 0, 4},
    {"QTE33A1", 0, 4},
    {"QTE33E1", 0, 4},
    {"QTE1IK2", 0, 4},
    {"QTE23C1", 0, 5},
    {"QTE3OC1", 0, 4},
    {"QTE3OB1", 0, 4},
    {"QTE3OD1", 0, 4},
    {"QTE3OF1", 0, 4},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"", 0, 10},
    {"end", 0, 10},
};

std::byte *mazeAuthoredMap;
std::byte *mazeUnitAreas = 0;
std::int32_t mazeRoadCounts[16];
Vector3 *mazeControlPoints[16];
Vector3 *mazeSplineBuffer = 0, *mazeRouteBuffer = 0;
float mazeFlutterCenterY = 3.7f, mazeFlutterCenterX = 4.1f;
std::int32_t mazeHourFrame = 0, mazePreviousHourFrame = 0;
float mazeHourBlend = 0.0f;
#endif

} // namespace
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t g_mazePropDrawCount = 0;
std::uint32_t g_mazeUnitWallHitCount = 0;
MazeUnit *MUZT_Data = nullptr;
#endif

namespace {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
constexpr Vector2 unit2Path0[] = {
    {0.9559339880943298f, 13.85644245147705f},
    {0.17282700538635254f, 15.684398651123047f},
    {-0.0011970000341534615f, 17.251218795776367f},
    {-0.0011970000341534615f, 18.46985626220703f},
};
constexpr Vector2 unit2Path1[] = {
    {1.2169690132141113f, 12.115531921386719f},
    {0.7095990180969238f, 10.068550109863281f},
    {0.31189200282096863f, 8.043073654174805f},
    {0.02264999970793724f, 6.632472991943359f},
    {-1.098162055015564f, 4.82401180267334f},
    {-1.4235730171203613f, 3.232564926147461f},
    {-1.2789520025253296f, 2.111319065093994f},
    {-0.664313018321991f, 1.243257999420166f},
    {1.9027080535888672f, -0.5290340185165405f},
    {3.5676960945129395f, -1.4694340229034424f},
    {4.688508987426758f, -1.758787989616394f},
    {8.014789581298828f, -1.975803017616272f},
    {10.473345756530762f, -1.7226179838180542f},
    {12.283597946166992f, -0.9992340207099915f},
    {14.452912330627441f, -0.16734200716018677f},
    {15.899121284484863f, 0.12201199680566788f},
    {17.417640686035156f, 0.15818099677562714f},
    {18.683074951171875f, 0.15818099677562714f},
};
constexpr Vector2 unit2Path2[] = {
    {2.3296759128570557f, 12.802322387695312f},
    {3.829200029373169f, 12.644415855407715f},
    {5.635041236877441f, 12.439910888671875f},
    {6.927080154418945f, 11.885965347290039f},
    {8.1268310546875f, 11.193532943725586f},
    {9.280437469482422f, 9.947155952453613f},
    {9.8803129196167f, 8.469968795776367f},
    {9.972600936889648f, 7.038942813873291f},
    {9.14134407043457f, 5.561756134033203f},
    {7.710872173309326f, 3.992244005203247f},
    {6.463959217071533f, 2.145759105682373f},
    {6.048661231994629f, 0.25311198830604553f},
    {6.094804763793945f, -2.424290895462036f},
    {6.0022478103637695f, -3.9476420879364014f},
    {5.1716508865356445f, -4.778560161590576f},
    {3.7873239517211914f, -5.240180969238281f},
    {2.1722750663757324f, -5.147857189178467f},
    {-0.9206590056419373f, -4.224614143371582f},
    {-3.320159912109375f, -3.4398579597473145f},
    {-6.829052925109863f, -2.2858049869537354f},
    {-10.153417587280273f, -1.2240760326385498f},
    {-12.045331954956055f, -0.6239680051803589f},
    {-14.029535293579102f, -0.023861000314354897f},
    {-15.690728187561035f, 0.16078799962997437f},
    {-17.16734504699707f, 0.2992739975452423f},
    {-18.413238525390625f, 0.2992739975452423f},
};
constexpr AuthoredPath unit2Paths[] = {{unit2Path0}, {unit2Path1}, {unit2Path2}};
constexpr Vector2 unit5Path0[] = {
    {-0.01992099918425083f, 1.3089890480041504f},
    {-1.122668981552124f, 2.6485579013824463f},
    {-1.8315789699554443f, 4.697311878204346f},
    {-2.225416898727417f, 7.455248832702637f},
    {-1.8315789699554443f, 10.449580192565918f},
    {-1.122668981552124f, 12.73472785949707f},
    {-0.4137600064277649f, 15.177472114562988f},
    {-0.01992099918425083f, 18.093006134033203f},
};
constexpr Vector2 unit5Path1[] = {
    {-1.122668981552124f, -0.5821679830551147f},
    {-2.382952928543091f, -1.606544017791748f},
    {-4.352146148681641f, -2.47332501411438f},
    {-6.242570877075195f, -2.5521230697631836f},
    {-8.054227828979492f, -2.079334020614624f},
    {-10.180956840515137f, -1.0549570322036743f},
    {-12.15014934539795f, -0.10937900096178055f},
    {-13.961807250976562f, -0.10937900096178055f},
    {-16.403606414794922f, -0.030579999089241028f},
    {-18.372798919677734f, 0.048218000680208206f},
};
constexpr Vector2 unit5Path2[] = {
    {1.0040589570999146f, 0.048218000680208206f},
    {2.4218780994415283f, -0.6609659790992737f},
    {4.233534812927246f, -1.0549570322036743f},
    {6.517798900604248f, -1.2913520336151123f},
    {9.510972023010254f, -1.0549570322036743f},
    {11.71646785736084f, -0.6609659790992737f},
    {14.47333812713623f, -0.34577301144599915f},
};
constexpr AuthoredPath unit5Paths[] = {{unit5Path0}, {unit5Path1}, {unit5Path2}};
constexpr Vector2 unit6Path0[] = {
    {-0.5f, 1.3860000371932983f},
    {1.0f, -0.35100001096725464f},
    {1.899999976158142f, -2.109999895095825f},
    {3.200000047683716f, -4.8680009841918945f},
    {5.300000190734863f, -6.126999855041504f},
    {7.5f, -5.997000217437744f},
    {10.699999809265137f, -4.454999923706055f},
    {13.300000190734863f, -1.4160000085830688f},
    {15.800000190734863f, -0.026000000536441803f},
    {18.19999885559082f, 0.25600001215934753f},
};
constexpr Vector2 unit6Path1[] = {
    {-0.800000011920929f, 3.7070000171661377f},
    {-0.20000000298023224f, 5.791999816894531f},
    {0.20000000298023224f, 8.9350004196167f},
    {0.30000001192092896f, 11.791000366210938f},
    {0.30000001192092896f, 14.358000755310059f},
    {0.30000001192092896f, 17.084001541137695f},
};
constexpr Vector2 unit6Path2[] = {
    {-3.0f, 1.878000020980835f},
    {-5.0f, 1.0119999647140503f},
    {-7.5f, 0.3700000047683716f},
    {-10.59999942779541f, -0.14300000667572021f},
    {-14.200000762939453f, -0.14300000667572021f},
    {-17.19999885559082f, -0.2070000022649765f},
};
constexpr AuthoredPath unit6Paths[] = {{unit6Path0}, {unit6Path1}, {unit6Path2}};
constexpr Vector2 unit7Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
};
constexpr Vector2 unit7Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
};
constexpr Vector2 unit7Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
};
constexpr AuthoredPath unit7Paths[] = {{unit7Path0}, {unit7Path1}, {unit7Path2}};
constexpr Vector2 unit8Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
};
constexpr Vector2 unit8Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
};
constexpr Vector2 unit8Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
};
constexpr AuthoredPath unit8Paths[] = {{unit8Path0}, {unit8Path1}, {unit8Path2}};
constexpr Vector2 unit9Path0[] = {
    {-0.30000001192092896f, 14.09999942779541f},
    {-0.4000000059604645f, 16.100000381469727f},
    {-0.10000000149011612f, 18.399999618530273f},
};
constexpr Vector2 unit9Path1[] = {
    {-1.899999976158142f, 12.0f},
    {-4.0f, 10.100000381469727f},
    {-6.400000095367432f, 8.699999809265137f},
    {-9.199999809265137f, 7.400000095367432f},
    {-11.0f, 6.099999904632568f},
    {-12.600000381469727f, 3.5f},
    {-14.40000057220459f, 1.2000000476837158f},
    {-16.399999618530273f, 0.20000000298023224f},
    {-18.299999237060547f, 0.0f},
};
constexpr Vector2 unit9Path2[] = {
    {1.2999999523162842f, 12.0f},
    {3.299999952316284f, 9.800000190734863f},
    {6.199999809265137f, 7.900000095367432f},
    {7.900001049041748f, 4.699999809265137f},
    {9.600000381469727f, 2.0999999046325684f},
    {12.09999942779541f, 0.30000001192092896f},
    {15.200000762939453f, -0.30000001192092896f},
    {18.100000381469727f, -0.20000000298023224f},
};
constexpr AuthoredPath unit9Paths[] = {{unit9Path0}, {unit9Path1}, {unit9Path2}};
constexpr Vector2 unit10Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
};
constexpr Vector2 unit10Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
};
constexpr Vector2 unit10Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
};
constexpr AuthoredPath unit10Paths[] = {{unit10Path0}, {unit10Path1}, {unit10Path2}};
constexpr Vector2 unit11Path0[] = {
    {0.11362899839878082f, 17.5744686126709f},
    {0.08527199923992157f, 15.617023468017578f},
    {0.3121339976787567f, 13.914895057678223f},
    {1.1345109939575195f, 12.865249633789062f},
    {2.2688241004943848f, 11.843973159790039f},
    {3.374778985977173f, 10.822696685791016f},
    {3.830336093902588f, 9.48935604095459f},
    {4.142271995544434f, 7.3900651931762695f},
    {4.227344989776611f, 4.836874008178711f},
    {3.376599073410034f, 2.368788957595825f},
    {2.0721399784088135f, 0.4680800139904022f},
    {0.25589001178741455f, -0.8936219811439514f},
    {-1.3037899732589722f, -2.368799924850464f},
    {-2.609050989151001f, -4.780147075653076f},
    {-3.176448106765747f, -7.9858222007751465f},
    {-3.034658908843994f, -10.36880111694336f},
    {-2.2119200229644775f, -12.581570625305176f},
    {-1.134323000907898f, -14.198591232299805f},
    {-0.3686619997024536f, -15.276605606079102f},
    {0.028348000720143318f, -16.723413467407227f},
    {0.05670500174164772f, -17.602846145629883f},
};
constexpr AuthoredPath unit11Paths[] = {{unit11Path0}};
constexpr Vector2 unit12Path0[] = {
    {0.04133100062608719f, 7.938899993896484f},
    {0.08983299881219864f, 3.4265151023864746f},
    {0.08983299881219864f, -0.8432689905166626f},
    {-0.24964000284671783f, -4.773410797119141f},
    {-0.007133000064641237f, -7.781668186187744f},
};
constexpr AuthoredPath unit12Paths[] = {{unit12Path0}};
constexpr Vector2 unit13Path0[] = {
    {-0.1207439973950386f, 8.045559883117676f},
    {-0.15051999688148499f, 6.16896390914917f},
    {-0.0016070000128820539f, 4.143433094024658f},
    {0.41525301337242126f, 2.1476879119873047f},
    {0.7725620269775391f, -0.5033760070800781f},
    {0.593779981136322f, -3.0352909564971924f},
    {0.20644299685955048f, -5.179973125457764f},
    {0.05757699906826019f, -6.550185203552246f},
    {0.027800999581813812f, -8.426780700683594f},
};
constexpr AuthoredPath unit13Paths[] = {{unit13Path0}};
constexpr Vector2 unit14Path0[] = {
    {0.0f, 18.0f},
    {0.0f, 17.0f},
    {0.8999999761581421f, 15.0f},
    {2.5f, 13.0f},
    {3.799999952316284f, 11.5f},
    {5.099999904632568f, 9.0f},
    {6.5f, 6.0f},
    {7.400000095367432f, 3.5f},
    {7.400001049041748f, 2.0f},
    {6.199999809265137f, 0.5f},
    {4.300000190734863f, -1.5f},
    {3.799999952316284f, -3.0f},
    {3.4000000953674316f, -6.0f},
    {2.799999952316284f, -8.5f},
    {1.5f, -11.0f},
    {-0.20000000298023224f, -13.5f},
    {-0.699999988079071f, -15.5f},
    {-0.6000000238418579f, -17.5f},
};
constexpr AuthoredPath unit14Paths[] = {{unit14Path0}};
constexpr Vector2 unit16Path0[] = {
    {0.0f, 8.899999618530273f},
    {0.0f, 7.099999904632568f},
    {0.0f, 4.699999809265137f},
    {0.0f, 2.0f},
    {0.0f, -1.0f},
    {0.0f, -3.299999952316284f},
    {0.0f, -6.0f},
    {0.0f, -8.0f},
};
constexpr AuthoredPath unit16Paths[] = {{unit16Path0}};
constexpr Vector2 unit17Path0[] = {
    {0.10000000149011612f, 9.137001037597656f},
    {0.10000000149011612f, 7.125999927520752f},
    {-0.10000000149011612f, 5.364999771118164f},
    {-0.10000000149011612f, 3.753000020980835f},
    {0.10000000149011612f, 1.4470000267028809f},
    {0.30000001192092896f, -0.04100000113248825f},
    {0.4000000059604645f, -1.5360000133514404f},
    {0.30000001192092896f, -3.494999885559082f},
    {0.20000000298023224f, -5.197000980377197f},
    {0.10000000149011612f, -7.061999797821045f},
    {0.10000000149011612f, -8.836999893188477f},
};
constexpr AuthoredPath unit17Paths[] = {{unit17Path0}};
constexpr Vector2 unit21Path0[] = {
    {0.13924099504947662f, 7.984930992126465f},
    {0.17633099853992462f, 5.944645881652832f},
    {0.02797199971973896f, 3.5704970359802246f},
    {-0.027672000229358673f, 0.6770049929618835f},
    {-0.23171000182628632f, -2.661642074584961f},
    {-0.2874009907245636f, -5.888998985290527f},
    {-0.13904200494289398f, -8.09621524810791f},
};
constexpr AuthoredPath unit21Paths[] = {{unit21Path0}};
constexpr Vector2 unit23Path0[] = {
    {0.0f, 22.0f},
    {0.0f, 21.0f},
    {0.05650800094008446f, 18.606828689575195f},
    {0.17621000111103058f, 15.568537712097168f},
    {0.6230649948120117f, 13.185556411743164f},
    {1.2483550310134888f, 10.77278995513916f},
    {1.0994759798049927f, 7.615344047546387f},
    {0.08679000288248062f, 5.768537998199463f},
    {-1.6406739950180054f, 4.0110931396484375f},
    {-3.904664993286133f, 1.8962010145187378f},
    {-5.364343166351318f, -0.48677799105644226f},
    {-5.662099838256836f, -1.8569910526275635f},
    {-5.691934108734131f, -4.299543857574463f},
    {-5.334601879119873f, -5.610180854797363f},
    {-3.8752119541168213f, -7.9633708000183105f},
    {-1.939054012298584f, -10.882519721984863f},
    {-0.7173389792442322f, -14.218689918518066f},
    {-0.09177300333976746f, -17.614431381225586f},
    {0.02738800086081028f, -20.02718734741211f},
    {-0.0023880000226199627f, -21.96335792541504f},
};
constexpr AuthoredPath unit23Paths[] = {{unit23Path0}};
constexpr Vector2 unit27Path0[] = {
    {0.0f, -3.869999885559082f},
    {-1.600000023841858f, -3.4100000858306885f},
    {-2.700000047683716f, -2.2300000190734863f},
    {-3.3399999141693115f, -1.1100000143051147f},
    {-3.640000104904175f, 0.6800000071525574f},
    {-2.5999999046325684f, 2.609999895095825f},
    {-1.1699999570846558f, 4.070000171661377f},
    {-0.12999999523162842f, 6.150000095367432f},
    {0.03999999910593033f, 8.09000015258789f},
};
constexpr AuthoredPath unit27Paths[] = {{unit27Path0}};
constexpr Vector2 unit28Path0[] = {
    {-0.9522500038146973f, 7.930092811584473f},
    {-1.4557980298995972f, 6.362894058227539f},
    {-1.6240689754486084f, 4.068066120147705f},
    {-1.5401769876480103f, 0.7657529711723328f},
    {-1.2604069709777832f, -2.480587959289551f},
    {-0.365092009305954f, -5.587000846862793f},
    {-0.08529999852180481f, -7.126214981079102f},
    {-0.029349999502301216f, -8.273628234863281f},
};
constexpr AuthoredPath unit28Paths[] = {{unit28Path0}};
constexpr Vector2 unit29Path0[] = {
    {-5.978970050811768f, -14.110830307006836f},
    {-7.628101825714111f, -11.809833526611328f},
    {-8.539463996887207f, -9.899574279785156f},
    {-10.23199462890625f, -7.8156538009643555f},
    {-11.664944648742676f, -6.0356340408325195f},
    {-12.619705200195312f, -3.560978889465332f},
    {-13.010289192199707f, -1.042907953262329f},
    {-12.489510536193848f, 1.518576979637146f},
    {-11.231013298034668f, 4.470798015594482f},
    {-9.364890098571777f, 6.728377819061279f},
    {-6.499682903289795f, 9.593769073486328f},
    {-3.4618079662323f, 12.415743827819824f},
    {-1.2476069927215576f, 15.628458976745605f},
    {-0.5966339707374573f, 17.538719177246094f},
};
constexpr AuthoredPath unit29Paths[] = {{unit29Path0}};
constexpr Vector2 unit30Path0[] = {
    {0.16825300455093384f, 4.084362030029297f},
    {0.15165899693965912f, 5.826985836029053f},
    {0.12224899977445602f, 7.894902229309082f},
    {0.13506600260734558f, 10.042478561401367f},
    {0.0696600005030632f, 12.781872749328613f},
};
constexpr AuthoredPath unit30Paths[] = {{unit30Path0}};
constexpr Vector2 unit31Path0[] = {
    {3.4000000953674316f, -2.9570000171661377f},
    {3.299999952316284f, -0.039000000804662704f},
    {2.299999952316284f, 2.7980000972747803f},
    {0.8999999761581421f, 5.392000198364258f},
    {0.0f, 7.90500020980835f},
    {-0.6000000238418579f, 10.905000686645508f},
    {-0.800000011920929f, 13.039999961853027f},
};
constexpr AuthoredPath unit31Paths[] = {{unit31Path0}};
constexpr Vector2 unit32Path0[] = {
    {0.0f, 22.0f},
    {0.0f, 20.0f},
    {0.0f, 18.0f},
    {0.18047699332237244f, 15.699812889099121f},
    {0.9443749785423279f, 11.980733871459961f},
    {1.1480820178985596f, 9.586258888244629f},
    {0.5369629859924316f, 7.242730140686035f},
    {-1.3485020399093628f, 4.593527793884277f},
    {-3.3346381187438965f, 2.3009450435638428f},
    {-5.0661420822143555f, -0.09352999925613403f},
    {-5.6273980140686035f, -2.589898109436035f},
    {-5.525545120239258f, -4.882481098175049f},
    {-4.455692768096924f, -7.480741024017334f},
    {-2.5204830169677734f, -10.079001426696777f},
    {-0.8897690176963806f, -13.798078536987305f},
    {-0.2786499857902527f, -16.90580177307129f},
    {-0.023899000138044357f, -19.809736251831055f},
    {0.02702699974179268f, -21.898534774780273f},
};
constexpr AuthoredPath unit32Paths[] = {{unit32Path0}};

constexpr AuthoredUnit authoredUnits[] = {
    {"", {}, 10},
    {"", {}, 10},
    {"UNTIIT2", unit2Paths, 9},
    {"", {}, 10},
    {"", {}, 10},
    {"UNTSOT2", unit5Paths, 9},
    {"UNTH3T1", unit6Paths, 9},
    {"UNTHIY1", unit7Paths, 9},
    {"UNTHIY2", unit8Paths, 9},
    {"UNTHIY3", unit9Paths, 9},
    {"UNTHIY4", unit10Paths, 9},
    {"UNTIIA1", unit11Paths, 6},
    {"UNTIIA2", unit12Paths, 4},
    {"UNTIIA3", unit13Paths, 4},
    {"UNTIIA4", unit14Paths, 8},
    {"UNTIIA5", {}, 7},
    {"UNTYI01", unit16Paths, 4},
    {"UNTY302", unit17Paths, 4},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"UNTHIA1", unit21Paths, 4},
    {"", {}, 10},
    {"UNTHOA0", unit23Paths, 6},
    {"UNTSRA1", {}, 6},
    {"UNTSRA2", {}, 6},
    {"UNTRIA1", {}, 6},
    {"UNTHIE1", unit27Paths, 0},
    {"UNTH3A3", unit28Paths, 4},
    {"UNTH3E1", unit29Paths, 3},
    {"UNTH3E2", unit30Paths, 1},
    {"UNTI3E1", unit31Paths, 2},
    {"UNTHOS0", unit32Paths, 6},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"QTE2RA1", {}, 4},
    {"QTE1OH0", {}, 4},
    {"QTE13T1", {}, 4},
    {"QTE1RJ1", {}, 4},
    {"QTE2OB0", {}, 4},
    {"QTE1OR0", {}, 4},
    {"QTE2OD0", {}, 4},
    {"QTE1IK1", {}, 4},
    {"QTE1OL0", {}, 4},
    {"QTE1OH1", {}, 4},
    {"QTE33A1", {}, 4},
    {"QTE33E1", {}, 4},
    {"QTE1IK2", {}, 4},
    {"QTE23C1", {}, 5},
    {"QTE3OC1", {}, 4},
    {"QTE3OB1", {}, 4},
    {"QTE3OD1", {}, 4},
    {"QTE3OF1", {}, 4},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
};

std::vector<RoadBlock> roadBlocks;
std::vector<void *> allocations;
std::int32_t unitCount = 0;
std::array<MazePartition, 32> partitions{};
std::array<std::array<EndpointLinks, 2>, 32> links{};
std::int32_t partitionCount = 0;
std::span<const std::byte> markerBytes;
std::int32_t markerCount = 0;
Vector3 *splineBuffer = nullptr;
Vector3 *routeBuffer = nullptr;
std::span<const std::byte> unitAreas;
const char *terrainPackage = nullptr;
bool ownsTerrainPackage = false;
HLib::Task *mazeTask = nullptr;
std::uint32_t mazeCurrentReferenceCount = 0;
std::uint32_t mazeFrameCount = 0;

Vector3 *fishControlPoints{};
Vector3 *fishPathSamples{};
std::int32_t fishControlCount{};
std::int32_t fishPathRingLength{};
std::int32_t fishPathCursor{};
std::uint32_t fishLastReferenceCount{};
Vector3 fishBoxMin{};
Vector3 fishBoxMax{};
std::uint32_t fishCount{};
std::int32_t fishSeed{};
const MT7Model *fishModel{};
HLib::Task *fishTask{};
float fishScale{1.0f};
bool fishEnabled{};
bool fishForceYaw{};
std::uint16_t fishForcedYaw{};

struct MazeModelResource {
    MT7Model *model{};
    MazeModelReader reader;
};
std::array<MazeModelResource, 24> roadModels;
std::array<MazeModelReader *, 24> activeRoadModels{};
std::array<MazeModelResource, std::size(authoredUnits)> unitModels;
std::array<MazeModelResource, 10> authoredObjectModels;
MazeAuthoredObject authoredObjects[] = {
    {.name = SHEN_NAME(HIA1), .resource = "KAWHIA1G", .flags = 0x90004},
        {.name = SHEN_NAME(IIT2), .resource = "KAWIIT2G", .flags = 0x90004},
            {.name = SHEN_NAME(IIA4), .resource = "TAKIIA4G", .flags = 0x50012},
                {.name = SHEN_NAME(IIA4), .resource = "TUBIIA4G", .flags = 0x150012},
                    {.name = SHEN_NAME(IIA4), .resource = "KAWIIA4G", .flags = 0x150014},
                        {.name = SHEN_NAME(HIY1), .resource = "KAWHIEKG", .flags = 0xe0008},
                            {.name = SHEN_NAME(HIY1), .resource = "IWNA2L1T", .flags = 0x8000},
                                {.name = SHEN_NAME(H3E1), .resource = "KAWH3E1G", .flags = 0xe0008},
                                    {.name = SHEN_NAME(H3E1), .resource = "IWNA2L1T", .flags = 0x8000},
                                        {.name = SHEN_NAME(HIE1), .resource = "TAKHIE1G", .flags = 0x50011}, {.flags = 0xffffffff},
};
std::array<MazeRoad *, 120> currentMazeObjects{};
// The eighth admitted unit is followed by the native zero sentinel.
std::array<MazeRoad *, 9> currentMazeUnitObjects{};
// Native SetMazeDefaults' settings block. Only SetMazeDefaults writes the road, unit and prop gates and the culling step count;
// the terrain gate and unit distance also have area setters.
float mazeUnitDistance = 60.0f;
std::int32_t mazeTerrainDrawEnabled = 1;
std::int32_t mazeRoadDrawEnabled = 1;
std::int32_t mazeUnitDrawEnabled = 1;
std::int32_t mazeRoadPropsEnabled = 1;
std::int32_t mazeCullingSteps = 13;
constexpr float mazeBob[] = {1.0f, 0.875f, 0.75f, 0.5f, 0.0f, -0.5f, -0.75f, -0.875f, -1.0f, -0.875f, -0.75f, -0.5f, 0.0f, 0.5f, 0.75f, 0.875f};
float mazeHourBlend = 0.0000099999997f;
std::int32_t mazeHourFrame = 2;
std::int32_t mazePreviousHourFrame = 2;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
DateTime mazeHourDate;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
PakGlobalData *mazePackage = 0;
HLib::Task *mazeTask = 0;
float mazeCameraLookAhead = 3.0f, mazeNearCullDistanceSq = 144.0f;
std::uint32_t mazeReferenceCount = 0;
EndpointLinks mazeLinks[32][2];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t mazeUnitWallHitCount;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 *mazeFishControlPoints, *mazeFishPathSamples = 0;
std::int32_t mazeFishControlCount = 0, mazeFishPathLength = 0, mazeFishPathCursor = 0;
std::uint32_t mazeFishLastReferenceCount = 0;
std::int32_t mazeUnitModelUsed[64], mazeMediumModelUsed[35];
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
MT7Model *g_mazeRoadModels[24], *g_mazeUnitModels[64], *g_mazeMediumModels[35], *g_mazeTerrainModel = 0;
MazePropRecord *g_mazePropTables[6];
Matrix4x4 g_mazeViewTransform;
std::int32_t g_mazeRoadDrawEnabled = 0, g_mazeUnitDrawEnabled = 0, g_mazeRoadPropsEnabled = 0, g_mazeCullingSteps = 0;
std::int32_t g_mazeWireframeEnabled = 0, g_mazeUnitWallsEnabled = 0, g_mazeModelOverride = 0, g_mazeRouteDrawEnabled = 0;
void SetNaviTaskEnabled(std::int32_t enabled);
void AddNaviTrailLocations(const Vector3 *points, std::int32_t count);
MazeRoad *g_mazeVisibleCandidates[120];
MazeRoad *g_currentMazeRoads[120], *g_currentMazeUnits[9];
MT7Model *g_mazeAuxiliaryModels[12], *g_mazeTreeModels[9];
char mazeModelFilename[32];

MazeAuthoredObject g_mazeAuthoredObjects[] = {{SHEN_NAME4('H', 'I', 'A', '1'), "KAWHIA1G", 0, 0x90004},
    {SHEN_NAME4('I', 'I', 'A', '4'), "TAKIIA4G", 0, 0x50012}, {SHEN_NAME4('I', 'I', 'A', '4'), "KAWIIA4G", 0, 0x10010},
    {SHEN_NAME4('H', 'I', 'Y', '1'), "KAWHIEKG", 0, 0xe0008}, {SHEN_NAME4('H', 'I', 'Y', '1'), "IWNA2L1T", 0, 0x8000},
    {SHEN_NAME4('H', '3', 'E', '1'), "KAWH3E1G", 0, 0xe0008}, {SHEN_NAME4('H', '3', 'E', '1'), "IWNA2L1T", 0, 0x8000},
    {SHEN_NAME4('H', 'I', 'E', '1'), "TAKHIE1G", 0, 0x50011}, {0, 0, 0, 0xffffffffU}};
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazeRoadDecoration mazeMarkerDecorations[] = {{SHEN_NAME4('K', 'M', 'Z', '2'), 0, 0, 0, {-2.1f, 0.0f, 0.0f}, 0, 7},
    {SHEN_NAME4('K', 'M', 'Z', '3'), 0, 0, 0, {-2.1f, 0.0f, 0.0f}, 0, 7}, {SHEN_NAME4('K', 'M', 'Z', '4'), 0, 0, 0, {2.1f, 0.0f, 0.0f}, 0x8000, 8},
    {0, 0, 0, 0, {0.0f, 0.0f, 0.0f}, 0, 0}};

static MazeSearchTargets mazeSearchTargets[] = {
    {SHEN_NAME4('K', 'M', 'Z', '1'),
        {{SHEN_NAME4('L', 'M', 'Z', '0'), 0, 0, {-189.669998f, 19.6499996f, -177.529999f}, 0.600000024f, static_cast<std::int16_t>(0x876a), 0x3800},
            {SHEN_NAME4('L', 'M', 'Z', '1'), 0, 0, {-190.160004f, 19.6499996f, -176.770004f}, 0.600000024f, static_cast<std::int16_t>(0xa500), 0x3800},
            {SHEN_NAME4('L', 'M', 'Z', '2'), 0, 0, {-190.710007f, 19.5300007f, -177.199997f}, 0.600000024f, static_cast<std::int16_t>(0x876a), 0x3800},
            {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}}},
    {SHEN_NAME4('K', 'M', 'Z', '2'),
        {{SHEN_NAME4('L', 'M', 'Z', '0'), 0, 0, {-502.089996f, 2.0999999f, -299.070007f}, 0.600000024f, static_cast<std::int16_t>(0xb8ac), 0x3800},
            {SHEN_NAME4('L', 'M', 'Z', '1'), 0, 0, {-502.230011f, 2.0999999f, -298.279999f}, 0.600000024f, static_cast<std::int16_t>(0xbbb3), 0x3800},
            {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}, {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}}},
    {SHEN_NAME4('K', 'M', 'Z', '3'),
        {{SHEN_NAME4('L', 'M', 'Z', '0'), 0, 0, {-398.480011f, 4.0999999f, -400.779999f}, 0.600000024f, 0x7870, 0x3800},
            {SHEN_NAME4('L', 'M', 'Z', '1'), 0, 0, {-399.290009f, 4.0999999f, -400.929993f}, 0.600000024f, 0x7870, 0x3800},
            {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}, {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}}},
    {SHEN_NAME4('K', 'M', 'Z', '4'),
        {{SHEN_NAME4('L', 'M', 'Z', '0'), 0, 0, {-400.339996f, 27.7800007f, -803.549988f}, 0.600000024f, static_cast<std::int16_t>(0x9321), 0x3800},
            {SHEN_NAME4('L', 'M', 'Z', '1'), 0, 0, {-401.170013f, 27.7800007f, -803.179993f}, 0.600000024f, static_cast<std::int16_t>(0x9321), 0x3800},
            {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}, {0, 0, 0, {0.0f, 0.0f, 0.0f}, 0.0f, 0, 0}}}};

std::int32_t mazeHangingUnitModels[2] = {0, 0};
const std::int32_t mazeMediumWiggleChildren[][2] = {{18, 1}, {19, 0}, {19, 2}, {20, 0}, {20, 1}, {22, 0}, {24, 1}, {26, 0}, {26, 1}, {0, 0}};
const std::int32_t mazeWiggle[] = {0, 16, 32, 40, 48, 52, 56, 58, 60, 58, 56, 52, 48, 40, 32, 16};
Matrix4x4 mazeFlutterTransform, mazeEffectTransform;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct MazeRoadCommandCopy {
    std::span<std::byte> bytes;
    LegacyDrawResources resources;
};
std::array<MazeRoadCommandCopy, 240> roadCommandCopies;
std::uint32_t roadCommandCopySize{};
MazeModelResource terrainModel;
Matrix4x4 mazeViewTransform{};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Vector3 mazeCameraPosition;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t mazeClearWeather, mazeHeavyWeather = 0;
std::uint32_t mazeRoadDrawCount, mazeUnitDrawCount = 0, mazePropDrawCount = 0;
static const float mazeBob[] = {1.0f, 0.875f, 0.75f, 0.5f, 0.0f, -0.5f, -0.75f, -0.875f, -1.0f, -0.875f, -0.75f, -0.5f, 0.0f, 0.5f, 0.75f, 0.875f};
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetDefaultWeatherRainParameters(float *first, float *second);
void GetDefaultWeatherDriftVariance(Vector3 *direction);
std::uint32_t GetDefaultWeatherRainColour();
float GetDefaultWeatherRippleTimeStep();
std::uint32_t GetDefaultWeatherRippleColour();
void InitWeibullResources();
void EnsureWeatherFixcRegistered();
void UnregisterActiveWeatherFixc();
void UpdateWeather();
void LoadWeatherClipChunk(const MapInfoChunk *chunk);
void shenLoadSetLensflare();

const std::int16_t memoLineFlags[146][10] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 138, 138, 138, 138},
    {138, 0, 694, 694, 694, 1426, 1426, 1427, 1427, 0},
    {0, 24, 24, 24, 0, 1, 1, 0, 1, 1},
    {0, 35, 35, 1508, 1508, 0, 1186, 1186, 0, 0},
    {2, 0, 2, 2, 2, 57, 57, 0, 59, 59},
    {1231, 0, 33, 33, 0, 33, 60, 1493, 1493, 0},
    {3, 3, 0, 3, 3, 0, 7, 7, 0, 0},
    {7, 36, 66, 72, 0, 13, 37, 38, 621, 621},
    {137, 0, 137, 137, 0, 0, 39, 39, 39, 0},
    {0, 39, 938, 938, 0, 1348, 0, 1348, 1348, 0},
    {1943, 1944, 1944, 1945, 1963, 1917, 1918, 1973, 1951, 0},
    {509, 646, 1690, 1690, 0, 0, 1848, 1848, 1847, 1847},
    {1691, 1692, 1692, 1693, 1693, 0, 1694, 1694, 1696, 1696},
    {0, 1695, 1695, 1697, 1697, 1960, 1961, 1969, 0, 0},
    {1795, 1923, 1924, 0, 0, 0, 1972, 1796, 0, 0},
    {1680, 1827, 1679, 1679, 1698, 0, 1699, 1699, 0, 0},
    {449, 0, 31, 0, 0, 0, 32, 32, 0, 0},
    {1398, 1398, 0, 377, 377, 77, 77, 0, 899, 0},
    {77, 1399, 1806, 1807, 0, 1519, 1520, 1521, 1522, 0},
    {1525, 1526, 1528, 1529, 1527, 78, 78, 0, 75, 76},
    {0, 26, 26, 26, 0, 26, 84, 82, 140, 88},
    {84, 84, 82, 82, 82, 140, 140, 88, 88, 88},
    {115, 115, 0, 115, 0, 276, 276, 0, 276, 0},
    {0, 132, 132, 132, 0, 132, 132, 132, 117, 0},
    {438, 438, 0, 438, 0, 121, 121, 121, 120, 0},
    {0, 83, 83, 83, 0, 0, 118, 118, 927, 0},
    {141, 0, 141, 141, 141, 0, -13, -14, 0, 0},
    {143, 143, 143, 0, 143, 0, 87, 87, 87, 0},
    {87, 87, 0, 87, 87, 0, 142, 142, 0, 0},
    {0, 1235, 1235, 1235, 0, 89, 89, 89, 89, 0},
    {89, 89, 0, 89, 89, 90, 90, 0, 90, 90},
    {90, 90, 0, 90, 90, 0, 147, 147, 0, 0},
    {166, 166, 0, 1681, 1681, 0, 421, 421, 421, 0},
    {1723, 1723, 1723, 131, 131, 1507, 1507, 1507, 1974, 0},
    {0, 154, 154, 154, 0, 148, 148, 0, 148, 0},
    {0, 1230, 1230, 1230, 0, 0, 1230, 1230, 1230, 0},
    {96, 96, 96, 96, 0, 96, 96, 0, 96, 96},
    {100, 100, 100, 100, 0, 0, 1166, 1166, 1166, 0},
    {1166, 1166, 1166, 1166, 0, 1166, 1166, 1166, 1166, 0},
    {1166, 1166, 1166, 1166, 1166, 1166, 1166, 0, 1166, 1166},
    {0, 157, 157, 0, 0, 157, 1414, 976, 978, 980},
    {0, 1492, 1498, 0, 0, 158, 158, 158, 158, 158},
    {103, 103, 103, 103, 103, 103, 103, 103, 103, 103},
    {0, 103, 103, 103, 0, 0, 97, 97, 0, 0},
    {97, 97, 97, 97, 0, 97, 97, 0, 97, 97},
    {1042, 1131, 1035, 967, 0, 1033, 968, 1038, 970, 1475},
    {1037, 969, 1406, 1073, 1488, 1412, 971, 1132, 1074, 1489},
    {1408, 1072, 1410, 1075, 1490, 1404, 982, 1028, 974, 1479},
    {1026, 972, 1024, 975, 1480, 0, 1030, 1030, 973, 0},
    {0, 1402, 1402, 1076, 1491, 439, 0, 439, 439, 0},
    {440, 1041, 0, 1041, 0, -8, -9, -10, -11, -12},
    {0, 844, 844, 844, 0, 105, 105, 0, 105, 0},
    {191, 191, 191, 191, 191, 0, 191, 191, 191, 0},
    {0, 191, 191, 191, 0, 191, 191, 191, 0, 0},
    {191, 191, 0, 1781, 1781, 286, 286, 0, 286, 286},
    {0, 366, 367, 367, 0, 0, 368, 368, 1424, 1424},
    {0, 287, 287, 287, 0, 287, 287, 0, 287, 0},
    {0, 290, 290, 290, 0, 1064, 1064, 1062, 1065, 178},
    {1066, 1066, 1066, 0, 1817, 0, 843, 843, 843, 0},
    {0, 843, 843, 843, 0, 843, 0, 293, 293, 293},
    {293, 0, 294, 294, 0, 298, 298, 0, 472, 472},
    {0, 300, 300, 300, 0, 300, 300, 300, 300, 0},
    {-4, -5, 0, -6, -7, 106, 106, 0, 106, 106},
    {106, 106, 0, 357, 0, 0, 2012, 710, 711, 0},
    {313, 313, 0, 313, 313, 313, 0, 313, 313, 0},
    {313, 313, 313, 313, 0, 0, 0, 0, 0, 0},
    {1648, 0, 1648, 1648, 1648, 0, 0, 0, 0, 0},
    {1933, 1968, 1919, 2016, 2015, 2001, 2002, 0, 0, 0},
    {1967, 1968, 2004, 0, 0, 1920, 1921, 2005, 0, 0},
    {1934, 1922, 0, 0, 0, 1932, 1925, 1925, 2003, 2006},
    {538, 538, 0, 538, 0, 539, 539, 0, 539, 539},
    {539, 539, 0, 539, 0, 539, 825, 826, 1914, 1926},
    {491, 491, 0, 491, 0, 556, 556, 556, 0, 556},
    {556, 556, 0, 556, 0, 0, 556, 556, 556, 0},
    {556, 556, 0, 556, 556, 556, 0, 556, 556, 0},
    {564, 564, 564, 0, 564, 564, 564, 564, 564, 0},
    {961, 0, 961, 961, 961, 498, 498, 0, 498, 498},
    {505, 505, 0, 505, 505, 1153, 1153, 0, 1154, 1154},
    {1155, 0, 1447, 1447, 0, 1156, 1156, 1156, 0, 1298},
    {0, 1574, 1574, 1574, 0, 591, 591, 0, 591, 591},
    {1714, 1714, 0, 1949, 1949, 575, 0, 575, 1927, 827},
    {546, 546, 546, 546, 0, 546, 1364, 1950, 1328, 828},
    {2007, 2007, 0, 2008, 2008, 2009, 2009, 2010, 2011, 0},
    {1870, 1870, 0, 1870, 1870, 0, 513, 513, 513, 0},
    {0, 515, 515, 515, 0, 577, 577, 577, 577, 577},
    {0, 577, 577, 577, 0, 519, 519, 0, 519, 519},
    {519, 519, 519, 0, 519, 0, 519, 830, 1928, 0},
    {521, 521, 0, 521, 0, 0, 570, 570, 570, 0},
    {522, 522, 0, 522, 522, 0, 524, 524, 524, 0},
    {524, 524, 524, 524, 524, 524, 524, 0, 524, 524},
    {524, 524, 0, 524, 524, 524, 524, 0, 524, 831},
    {839, 839, 0, 839, 0, 526, 526, 0, 526, 526},
    {1497, 1209, 1211, 1215, 1213, 504, 504, 0, 504, 504},
    {832, 832, 0, 507, 507, 0, 507, 507, 507, 0},
    {507, 507, 507, 507, 507, 0, 507, 507, 507, 0},
    {507, 507, 507, 507, 507, 507, 507, 507, 507, 0},
    {0, 507, 507, 0, 0, 932, 933, 933, 933, 933},
    {930, 931, 0, 931, 931, 934, 759, 0, 759, 759},
    {760, 761, 0, 761, 761, 762, 763, 763, 763, 763},
    {764, 427, 427, 427, 427, 1417, 1417, 0, 1417, 1417},
    {528, 528, 0, 528, 528, 528, 1995, 1996, 1996, 1996},
    {1469, 1469, 0, 1930, 1930, 1468, 1468, 0, 1929, 1929},
    {1470, 1470, 1682, 1931, 1931, 529, 529, 529, 529, 529},
    {530, 530, 0, 530, 530, 531, 0, 531, 531, 531},
    {1341, 1341, 0, 1418, 1418, 647, 647, 0, 647, 0},
    {0, 647, 647, 0, 0, 1329, 1329, 1329, 1329, 1329},
    {0, -1, -2, -3, 0, 1932, 1932, 0, 1592, 1592},
    {1444, 1444, 0, 1444, 0, 1330, 1330, 0, 1330, 0},
    {534, 534, 0, 534, 534, 648, 648, 0, 648, 648},
    {0, 1720, 1720, 0, 0, 533, 533, 533, 0, 533},
    {599, 599, 0, 599, 599, 1081, 1081, 0, 1081, 0},
    {600, 0, 600, 600, 0, 605, 605, 0, 623, 623},
    {624, 0, 626, 626, 0, 536, 536, 536, 536, 536},
    {536, 536, 536, 536, 536, 0, 548, 548, 0, 0},
    {0, 1820, 1820, 0, 0, 1822, 1822, 0, 1822, 1822},
    {1821, 1821, 0, 1821, 1821, 636, 636, 636, 636, 636},
    {0, 1437, 1437, 1437, 0, 1437, 1437, 1437, 1437, 0},
    {1437, 0, 1437, 1437, 1437, 1437, 1437, 1437, 0, 1437},
    {1437, 1437, 0, 1437, 1437, 1437, 1437, 1437, 1437, 0},
    {1437, 1437, 1437, 1437, 1437, 1437, 1437, 0, 1437, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {424, 0, 424, 424, 0, 424, 424, 424, 424, 424},
    {0, 424, 424, 0, 424, 424, 1438, 1439, 0, 0},
    {426, 426, 426, 426, 0, 426, 0, 426, 426, 426},
    {426, 426, 426, 426, 426, 1593, 1593, 0, 1593, 0},
    {1593, 1593, 1593, 1593, 1593, 1537, 1537, 0, 1537, 0},
    {1558, 1558, 0, 1558, 1558, 455, 455, 455, 455, 0},
    {0, 455, 455, 0, 0, 0, 467, 467, 467, 0},
    {0, 467, 0, 467, 467, 686, 686, 686, 686, 0},
    {552, 552, 552, 552, 552, 756, 0, 756, 756, 0},
    {1302, 1302, 1302, 1302, 1302, 0, 767, 767, 767, 0},
    {0, 1204, 1204, 1204, 0, 1204, 1204, 0, 1204, 0},
    {0, 1587, 1587, 1587, 0, 0, 768, 768, 768, 0},
    {0, 757, 757, 0, 0, 434, 0, 434, 434, 0},
    {434, 434, 0, 434, 434, 434, 434, 0, 434, 434},
    {0, 434, 434, 0, 0, 0, 936, 936, 936, 0},
    {435, 435, 435, 435, 0, 0, 435, 435, 435, 0},
    {0, 435, 435, 0, 0, 769, 769, 0, 769, 769},
    {1207, 1207, 0, 1207, 0, 0, 770, 770, 770, 0},
    {841, 841, 841, 841, 0, 841, 841, 841, 841, 841},
    {0, 436, 436, 436, 0, 436, 436, 436, 436, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const ConditionalMemoLine conditionalMemoLines[14] = {
    {SHEN_FIELD(flags){999, 999}, SHEN_FIELD(text){0, "\312\375\315\350\316\271\274\322\244\316\241\241\241\241\241\241\241\241"},
        SHEN_FIELD(ruby){0,
            "\245\333\245\246\245\351\245\244\245\352\245\347\245\267\245\343\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241"
            "\241"}},
    {SHEN_FIELD(flags){999, 999},
        SHEN_FIELD(text){"\312\375\315\350\316\271\274\322\244\316\241\241\241\241\241\241\241\241",
            "\241\241\245\304\245\261\244\362\312\247\244\303\244\277\244\254\241\241"},
        SHEN_FIELD(ruby){
            "\245\333\245\246\245\351\245\244\245\352\245\347\245\267\245\343\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241",
            0}},
    {SHEN_FIELD(flags){999, 999},
        SHEN_FIELD(text){"\241\241\245\304\245\261\244\362\301\264\311\364\312\247\244\303\244\277",
            "\241\241\302\255\244\352\244\312\244\253\244\303\244\277\241\304\241\241"},
        SHEN_FIELD(ruby){0, 0}},
    {SHEN_FIELD(flags){302, 303},
        SHEN_FIELD(text){"\303\317\277\336\244\316\276\354\275\352\244\313\241\241\241\241\241\241",
            "\245\246\245\251\245\363\244\254\241\241\241\241\241\241\241\241\241\241"},
        SHEN_FIELD(ruby){0, 0}},
    {SHEN_FIELD(flags){302, 303},
        SHEN_FIELD(text){"\241\241\274\353\270\265\303\243\244\254\244\244\244\353\241\251\241\241",
            "\241\241\275\365\244\261\244\306\244\257\244\354\244\277\244\316\244\253"},
        SHEN_FIELD(ruby){
            "\241\241\241\241\245\267\245\345\245\261\241\311\245\363\245\277\245\304\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241",
            0}},
    {SHEN_FIELD(flags){302, 303},
        SHEN_FIELD(text){"\263\253\261\277\342\373\306\254\244\316\241\241\241\241\241\241\241\241",
            "\245\354\245\363\244\317\244\311\244\263\244\300\241\252\241\241\241\241"},
        SHEN_FIELD(ruby){
            "\245\253\245\244\245\246\245\363\245\325\245\310\245\246\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241",
            0}},
    {SHEN_FIELD(flags){302, 303}, SHEN_FIELD(text){"\241\241\303\317\277\336\244\316\276\354\275\352\244\313\265\336\244\260", 0}, SHEN_FIELD(ruby){0, 0}},
    {SHEN_FIELD(flags){1184, 442}, SHEN_FIELD(text){"\267\260\241\304\244\271\244\336\244\312\244\244\241\304\241\241\241\241", 0},
        SHEN_FIELD(ruby){
            "\245\257\245\363\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241",
            0}},
    {SHEN_FIELD(flags){1184, 442},
        SHEN_FIELD(text){"\241\241\244\275\244\354\244\307\244\342\262\266\244\317\241\304\241\241",
            "\274\353\270\265\303\243\244\317\241\241\241\241\241\241\241\241\241\241"},
        SHEN_FIELD(ruby){0,
            "\245\267\245\345\245\261\241\311\245\363\245\277\245\304\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241"
            "\241"}},
    {SHEN_FIELD(flags){1184, 442},
        SHEN_FIELD(text){"\265\331\244\363\244\307\244\353\244\357\244\261\244\313\244\317\241\241",
            "\241\241\241\326\306\246\276\255\270\256\241\327\244\307\241\241\241\241"},
        SHEN_FIELD(ruby){0,
            "\241\241\241\241\241\241\241\241\244\310\244\246\244\267\244\347\244\246\244\261\244\363\241\241\241\241\241\241\241\241\241\241\241\241\241"
            "\241"}},
    {SHEN_FIELD(flags){1184, 442},
        SHEN_FIELD(text){"\241\241\241\241\241\241\244\244\244\253\244\312\244\244\244\363\244\300",
            "\241\241\317\242\315\355\244\362\244\310\244\303\244\306\244\244\244\353"},
        SHEN_FIELD(ruby){0,
            "\241\241\241\241\244\354\244\363\244\351\244\257\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241"
            "\241"}},
    {SHEN_FIELD(flags){1184, 442}, SHEN_FIELD(text){"\244\342\244\246\260\354\305\331\303\343\317\322\277\330\244\300\241\252", 0},
        SHEN_FIELD(ruby){
            "\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\245\301\245\343\245\357\245\363\245\267\241\311\245\363\241\241\241\241\241\241",
            0}},
    {SHEN_FIELD(flags){1220, 1221},
        SHEN_FIELD(text){"\273\260\305\341\276\256\317\251\244\316\241\241\241\241\241\241\241\241",
            "\271\310\306\356\263\271\244\316\241\241\241\241\241\241\241\241\241\241"},
        SHEN_FIELD(ruby){
            "\245\265\245\363\245\310\245\246\245\310\241\311\245\252\245\352\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241",
            "\245\263\245\246\245\312\245\363\245\253\241\311\245\244\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241\241"
            "\241"}},
    {SHEN_FIELD(flags){1220, 1221},
        SHEN_FIELD(text){"\241\241\241\241\262\332\272\333\313\245\305\271\244\313\271\324\244\257",
            "\241\241\241\241\314\300\265\376\314\364\312\336\244\313\271\324\244\257"},
        SHEN_FIELD(ruby){
            "\241\241\241\241\241\241\241\241\245\253\245\265\245\244\245\333\245\246\245\306\245\363\241\241\241\241\241\241\241\241\241\241\241\241\241\241",
            "\241\241\241\241\241\241\241\241\245\341\245\244\245\255\245\347\245\246\245\344\245\257\245\333\241\241\241\241\241\241\241\241\241\241\241"
            "\241"}},
};

std::uint16_t g_memoPage;
std::int16_t g_memoFlag;
std::int16_t g_memoPreviousFlag;
std::int16_t g_memoLatestPage;
std::byte *g_memoData = 0;
std::byte *g_memoMasterTable = 0;
std::byte *g_memoRubyTable = 0;
std::uint32_t g_memoRecordCount;
std::byte *g_memoShenmue1Table = 0;
std::uint32_t g_memoShenmue1RecordCount;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MazeTaskCallback(void *parameter) {
    static_cast<MazeTaskData *>(parameter)->reserved = 0;
    if (!g_worldDrawingEnabled)
        return;
    matrix_stack::GetMatrix(&g_mazeViewTransform);
    InitializeMazeFrameEnvironment();
    InitializeMazeNavigationTrail();
    if (!shenIsGamePaused())
        mazeSetCurrentRefCount(mazeGetCurrentRefCount() + 1);
    MazeFrameNullsub();
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    float nearestDistance;
    InitializeMazeObjectTransforms(g_mazeRoadBlocks, &camera, &nearestDistance);
    FilterMazeVisibleRoads();
    DrawMazeSkyModel();
    for (std::int32_t index = 0; g_currentMazeRoads[index];)
        DrawMazeRoad(g_currentMazeRoads[index++]);
    for (std::int32_t index = 0; g_currentMazeUnits[index];)
        DrawMazeUnit(g_currentMazeUnits[index++]);
    dc::sdk::SeedRandom(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister));
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeUnit(MazeRoad *road) {
    if (!MazeUnitHasVisiblePoint(road) || !MazeRoadPassesCameraAngle(road, 1))
        return;
    matrix_stack::Matrix4x4_Push();
    if (GetMazeUnitDrawEnabled()) {
        const std::uint32_t model = GetAuthoredWord(road->block, road->flags & 0xffffU) & 0xffffU;
        Matrix4x4::preTranslate(road->position.x, road->position.y + 0.02f, road->position.z);
        const std::int32_t yaw = GetAuthoredRecordYaw(road->block, road->flags & 0xffffU);
        g_mazeUnitModels[model]->root->rotation.y = 0x4000 - yaw;
        DrawCharacterModelTree(g_mazeUnitModels[model]->root);
        Matrix4x4::RotateAroundY_Wrapper(g_mazeUnitModels[model]->root->rotation.y);
        DrawMazeUnitAuxiliary(road);
    }
    PopCurrentMatrix(1);
    ++mazeUnitDrawCount;
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t FindCharacterNearestMazeFirstBlockRoad(Character *character) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    return FindNearestMazeFirstBlockRoad(&position);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t FindNearestMazeFirstBlockRoad(const Vector3 *position) {
    std::int32_t block, index;
    FindNearestMazeRoad(position, &block, &index);
    if (block == 0)
        return index;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FindNearestMazeRoad(const Vector3 *position, std::int32_t *block, std::int32_t *index) {
    float nearest = 25600.0f;
    *block = -1;
    *index = -1;
    for (std::int32_t b = 0; b < static_cast<std::int32_t>(GetMazeBlockCount()); ++b) {
        MazeRoad *road = g_mazeRoadBlocks[b];
        for (std::int32_t i = 0; road->index >= 0; ++i, ++road) {
            const float x = position->x - road->position.x, z = position->z - road->position.z;
            const float distance = x * x + z * z;
            if (nearest > distance) {
                *index = i;
                *block = b;
                nearest = distance;
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeRoadCount(std::int32_t block) {
    return GetMazeRoadCountRaw(block);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetMazeRoadPosition(std::int32_t block, std::int32_t index, Vector3 *output) {
    *output = GetMazeRoad(block, index)->position;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetMazeRoadYaw(std::int32_t block, std::int32_t index) {
    return GetMazeRoad(block, index)->yaw;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetMazeUnitCount() {
    return g_mazeUnitCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeUnitModelIndex(std::int32_t index) {
    return GetMazeUnit(index)->modelIndex;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
shen_name GetMazeUnitAuthoredName(std::int32_t index) {
    const std::int32_t model = GetMazeUnit(index)->modelIndex;
    const char *name = authoredUnits[model].model;
    return static_cast<std::uint8_t>(name[6]) << 24 | static_cast<std::uint8_t>(name[5]) << 16 | static_cast<std::uint8_t>(name[4]) << 8 |
        static_cast<std::uint8_t>(name[3]);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool HasMazeUnitAuthoredName(shen_name name) {
    const std::int32_t count = GetMazeUnitCount();
    for (std::int32_t index = 0; index < count; ++index)
        if (GetMazeUnitAuthoredName(index) == name)
            return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetMazeUnitYaw(std::int32_t index) {
    const MazeRoad *road = GetMazeUnit(index)->road;
    const std::int32_t word = GetAuthoredWord(road->block, road->flags & 0xffffU);
    return 0x4000U - GetAuthoredRecordYaw(road->block, road->flags & 0xffffU);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetMazeUnitPosition(std::int32_t index, Vector3 *output) {
    *output = GetMazeUnit(index)->road->position;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetCharacterMazeUnitIndex(Character *character) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    return FindMazeUnit(&position);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindMazeUnit(const Vector3 *position) {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(GetMazeUnitCount()); ++index) {
        const MazeUnit *const unit = GetMazeUnit(index);
        if (InsideMazeQuad(&unit->quad[0], &unit->quad[1], &unit->quad[2], &unit->quad[3], position))
            return index;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
MazeUnit *GetMazeUnit(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(GetMazeUnitCount()))
        return 0;
    return MUZT_Data + index;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
MazeRoad *GetMazeRoad(std::int32_t block, std::int32_t index) {
    MazeRoad *roads = g_mazeRoadBlocks[block];
    return roads + index;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool InsideMazeQuad(const Vector3 *first, const Vector3 *second, const Vector3 *third, const Vector3 *fourth, const Vector3 *point) {
    float reference, x, z, edgeX, edgeZ;
    x = first->x - point->x;
    z = first->z - point->z;
    edgeX = second->x - first->x;
    edgeZ = second->z - first->z;
    reference = z * edgeX - x * edgeZ;
    x = second->x - point->x;
    z = second->z - point->z;
    edgeX = third->x - second->x;
    edgeZ = third->z - second->z;
    if (0.0f > reference * (z * edgeX - x * edgeZ))
        return false;
    x = third->x - point->x;
    z = third->z - point->z;
    edgeX = fourth->x - third->x;
    edgeZ = fourth->z - third->z;
    if (0.0f > reference * (z * edgeX - x * edgeZ))
        return false;
    x = fourth->x - point->x;
    z = fourth->z - point->z;
    edgeX = first->x - fourth->x;
    edgeZ = first->z - fourth->z;
    if (0.0f > reference * (z * edgeX - x * edgeZ))
        return false;
    return true;
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MazeMarkerNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetMazeMarkerCount() {
    return g_mazeMarkerCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool mazeGetMarkerPosition(std::int32_t marker, Vector3 *output) {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(GetMazeMarkerCount()); ++index) {
        const MazeMarker *entry = g_mazeMarkers + index;
        if (entry->id == marker) {
            mazeSamplePartition(entry->partition, entry->element, output);
            return true;
        }
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetMazeMarkerRouteCoordinates(std::int32_t marker, std::int32_t *partition, std::int32_t *element) {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(GetMazeMarkerCount()); ++index) {
        const MazeMarker *entry = g_mazeMarkers + index;
        if (entry->id == marker) {
            *partition = entry->partition;
            *element = entry->element;
            return;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindCharacterMazeMarkerWithinRadius(Character *character, float radius) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    return FindMazeMarkerWithinRadius(&position, radius);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t FindMazeMarkerWithinRadius(const Vector3 *position, float radius) {
    const float squaredRadius = radius * radius;
    std::int32_t result = -1;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(GetMazeMarkerCount()); ++index) {
        Vector3 point;
        GetMazeMarkerSlotPosition(index, &point);
        const float x = position->x - point.x, z = position->z - point.z;
        if (squaredRadius > x * x + z * z)
            result = g_mazeMarkers[index].id;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsCharacterWithinMazeMarkerRadius(Character *character, std::int32_t marker, float radius) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    return IsPositionWithinMazeMarkerRadius(&position, marker, radius);
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeTreeEffects(const MazeRoad *road) {
    float centerX, centerY;
    if (!GetMazeRoadStyle(road)) {
        centerX = 5.0f;
        centerY = 5.0f;
    } else if (GetMazeRoadStyle(road) == 5) {
        centerX = 2.0f;
        centerY = 8.0f;
    } else {
        return;
    }
    DrawMazeFlutterEffects(road);
    dc::sdk::SeedRandom(road->block * 0x200 + road->index);
    if (dc::sdk::RandomFloat() > 0.5f)
        return;
    const std::int32_t count = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 1.1f + 1.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        Vector3 position;
        std::int32_t model = 2;
        const float choice = dc::sdk::RandomFloat();
        if (choice < 0.8f)
            model = 1;
        if (choice < 0.45f)
            model = 0;
        position.x = dc::sdk::RandomFloat() - 0.5f + centerX;
        position.y = dc::sdk::RandomFloat() * 2.0f - 1.0f + centerY;
        position.z = dc::sdk::RandomFloat() - 0.5f;
        if (!(dc::sdk::RandomFloat() < 0.5f))
            position.x *= -1.0f;
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        SwizzledBasis3x4IntoActiveFrom(g_mazeViewTransform.rows);
#else
        CopyMazeMatrixBasis(&g_mazeViewTransform);
#endif
        dc::sdk::DrawSingleLightMT7Mesh(g_mazeTreeModels[model]->root->child->sibling->mesh);
        if (mazeClearWeather) {
            Matrix4x4::MultiplyWithPrePassMatrix(&mazeEffectTransform);
            dc::sdk::DrawSingleLightMT7MeshAlpha(g_mazeTreeModels[model]->root->child->mesh, 0.4f);
        }
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeFlutterEffects(const MazeRoad *road) {
    if (road->cameraDistance > 256.0f || !mazeClearWeather)
        return;
    float centerX, centerY;
    if (!GetMazeRoadStyle(road)) {
        if (MazeRecordMatchesIndex(0x1000U, road->index))
            return;
        centerX = mazeFlutterCenterX;
        centerY = mazeFlutterCenterY;
    } else if (GetMazeRoadStyle(road) == 5) {
        centerX = 0.6f;
        centerY = 11.6f;
    } else {
        return;
    }
    dc::sdk::SeedRandom(road->index * 2 + road->block * 600);
    std::int32_t index, count;
    count = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 3.0f) + 3;
    for (index = 0; index < count; ++index) {
        const std::uint32_t random = dc::sdk::RandomInt();
        if (!((static_cast<std::int32_t>(mazeGetCurrentRefCount()) >> 3) * random & 0x70U))
            continue;
        const std::int32_t model = (random & 3U) + 3;
        Vector3 position;
        position.x = dc::sdk::RandomFloat() * 0.4f - 0.2f + centerX;
        position.y = dc::sdk::RandomFloat() * 1.6f - 0.8f + centerY;
        position.z = dc::sdk::RandomFloat() * 2.0f - 1.0f;
        if (dc::sdk::RandomFloat() > 0.5f)
            position.x *= -1.0f;
        const std::int32_t distance = static_cast<std::int32_t>((position.x * position.x + position.z * position.z + road->cameraDistance) * 100.0f);
        if (!MazeFlutterPattern(distance, random))
            continue;
        float alpha = 0.4f;
        if (distance & 4)
            alpha += 0.3f;
        if (distance & 2)
            alpha += 0.3f;
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        SwizzledBasis3x4IntoActiveFrom(mazeFlutterTransform.rows);
#else
        CopyMazeMatrixBasis(&mazeFlutterTransform);
#endif
        dc::sdk::DrawSingleLightMT7MeshAlpha(g_mazeTreeModels[model]->root->mesh, alpha);
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t MazeFlutterPattern(std::uint32_t distance, std::uint32_t random) {
    if (!(distance & 15U))
        return 0;
    const std::int32_t shift = (!(distance & 0xffe000U) ? distance >> 10 : static_cast<std::int32_t>(distance) >> 12) & 7;
    return (static_cast<std::int32_t>(random) >> shift) & 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeButterflyEffectsSeeded(const MazeRoad *road) {
    if (GetMazeRoadStyle(road) != 0)
        return;
    dc::sdk::SeedRandom(road->index * 3 + road->block * 0x200);
    if (!mazeHeavyWeather)
        DrawMazeButterflyEffects(road);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeButterflyEffects(const MazeRoad *road) {
    if (dc::sdk::RandomFloat() > 0.05f)
        return;
    static Vector3 path[] = {{-0.02f, 0.02f, 0.01f}, {-0.04f, 0.05f, 0.02f}, {-0.05f, 0.07f, 0.0f}, {-0.03f, 0.08f, -0.01f}, {0.03f, 0.08f, 0.01f},
        {0.05f, 0.07f, 0.0f}, {0.04f, 0.05f, -0.02f}, {0.02f, 0.02f, -0.01f}};
    const std::int32_t count = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 1.9f + 1.0f);
    Vector3 position;
    for (std::int32_t index = 0; index < count; ++index) {
        const std::uint32_t random = dc::sdk::RandomInt(), phase = random + mazeGetCurrentRefCount();
        const std::uint32_t frame = phase & 15U;
        position.x = dc::sdk::RandomFloat() * 0.8f - 0.4f + 3.0f + path[phase & 7U].x;
        const float lift = path[phase & 7U].y;
        const float y = dc::sdk::RandomFloat() * 0.8f - 0.4f + 1.5f;
        if (!(phase & 8U))
            position.y = y + lift * -1.0f;
        else
            position.y = y + lift;
        position.z = dc::sdk::RandomFloat() * 0.8f - 0.4f + path[frame & 7U].z;
        position.x = (road->index & 1) ? position.x * -1.0f : position.x;
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        MT7Node *node = g_mazeTreeModels[(random & 1U) + 7]->root->child->child;
        for (std::int32_t step = 0; step < static_cast<std::int32_t>(mazeGetCurrentRefCount() & 3U); ++step)
            node = node->sibling;
        dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeSkyModel() {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    if (GetMazeTerrainDrawEnabled())
        DrawCharacterModelTree(g_mazeTerrainModel->root);
    PopCurrentMatrix(1);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t InitializeMazeObjectTransforms(MazeRoad **blocks, const Vector3 *camera, float *nearestDistance) {
    float nearest = 1000000.0f;
    std::int32_t bestIndex = -1;
    std::int32_t bestBlock = -1;
    ClearMazeVisibleLists();
    const std::int32_t count = static_cast<std::int32_t>(GetMazeBlockCount());
    for (std::int32_t block = 0; block < count; ++block) {
        float distance;
        const std::int32_t index = UpdateMazeRoadTransforms(blocks[block], camera, &distance);
        if (index != -1 && nearest > distance) {
            nearest = distance;
            bestIndex = index;
            bestBlock = block;
        }
    }
    *nearestDistance = nearest;
    if (bestBlock == -1)
        return -1;
    return bestBlock * 0x10000 + bestIndex;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeMazeNavigationTrail() {
    Vector3 start, end;
    if (mazeGetCurrentRefCount() != 3)
        return;
    if (g_currentMap == SHEN_NAME4('K', 'M', 'Z', '2')) {
        start.x = -500.0f;
        start.z = -190.0f;
        end.x = -500.0f;
        end.z = -310.0f;
    } else if (g_currentMap == SHEN_NAME4('K', 'M', 'Z', '3')) {
        start.x = -500.0f;
        start.z = -400.0f;
        end.x = -390.0f;
        end.z = -400.0f;
    } else if (g_currentMap == SHEN_NAME4('K', 'M', 'Z', '4')) {
        start.x = -500.0f;
        start.z = -700.0f;
        end.x = -400.0f;
        end.z = -810.0f;
    } else {
        return;
    }
    Vector3 *points = static_cast<Vector3 *>(HLib::AllocateTaskData(0x60, SHEN_NAME4('M', 'Z', 'E', 'T')));
    dc::sdk::SeedRandom(g_currentMap);
    for (std::int32_t index = 0; index < 8; ++index) {
        const float x = dc::sdk::RandomFloat() * 4.0f - 2.0f, z = dc::sdk::RandomFloat() * 4.0f - 2.0f;
        const float fraction = static_cast<float>(index) * 0.125f;
        points[index].x = (end.x - start.x) * fraction + start.x + x;
        points[index].y = 1.0f;
        points[index].z = start.z + (end.z - start.z) * fraction + z;
    }
    AddNaviTrailLocations(points + 1, 7);
    HLib::FreeTaskData(points);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeMazeAuxiliaryModels() {
    LoadMazeAuxiliaryModels();
    LoadMazeAuthoredObjectModels();
    LoadMazeTreeModels();
    InitializeMazeFishPath();
    AssignMazeMarkerDecorationRoads();
    SetMazeSearchTargets();
    InitializeMazeHourAnimation();
    FindMazeHangingUnitModels();
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeUnitAuxiliary(const MazeRoad *road) {
    const shen_name name = GetMazeUnitAuthoredName(road->unit);
    if (name == SHEN_NAME4('S', 'O', 'T', '2') || name == SHEN_NAME4('H', 'I', 'Y', '4'))
        DrawMazeAuxiliaryVariation(road, name);
    else
        DrawMazeAuthoredObjects(name, road);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AssignMazeMarkerDecorationRoads() {
    for (std::int32_t entry = 0; mazeMarkerDecorations[entry].area; ++entry) {
        if (mazeMarkerDecorations[entry].area == g_currentMap) {
            Vector3 position;
            mazeGetMarkerPosition(mazeMarkerDecorations[entry].marker, &position);
            float nearest = 1000000.0f;
            std::int32_t block = 0, index = 0;
            for (std::int32_t current = 0; current < static_cast<std::int32_t>(GetMazeBlockCount()); ++current)
                for (MazeRoad *road = g_mazeRoadBlocks[current]; road->index >= 0; ++road) {
                    const float x = position.x - road->position.x, z = position.z - road->position.z;
                    const float distance = x * x + z * z;
                    if (nearest > distance) {
                        block = road->block;
                        index = road->index;
                        nearest = distance;
                    }
                }
            mazeMarkerDecorations[entry].block = block;
            mazeMarkerDecorations[entry].index = index;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMazeSearchTargets() {
    for (std::int32_t index = 0; index < 4; ++index)
        if (mazeSearchTargets[index].area == g_currentMap)
            srchSetTarget(0, mazeSearchTargets[index].targets);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeRoadDecoration(const MazeRoad *road) {
    for (std::int32_t index = 0; mazeMarkerDecorations[index].area; ++index) {
        if (g_currentMap == mazeMarkerDecorations[index].area && mazeMarkerDecorations[index].block == road->block &&
            mazeMarkerDecorations[index].index == road->index) {
            matrix_stack::Matrix4x4_Push();
            Matrix4x4::preTranslate(mazeMarkerDecorations[index].position.x, mazeMarkerDecorations[index].position.y, mazeMarkerDecorations[index].position.z);
            Matrix4x4::RotateAroundY_Wrapper(mazeMarkerDecorations[index].yaw);
            DrawCharacterModelTree(g_mazeAuxiliaryModels[mazeMarkerDecorations[index].kind]->root);
            PopCurrentMatrix(1);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMazeAuxiliaryModels() {
    static const char *auxiliaryModelNames[] = {
        "MAZHOB1G", "MAZHOB2G", "MAZHOB3G", "MAZHOP3G", "MAZHOP1G", "MAZHOP2G", "KANSOT2G", "KANSRB1G", "KANIRB1G", "ZIZHIY4G", "KOMHIY4G", "USAG200T", "end"};
    std::int32_t index;
    for (index = 0; index < 12; ++index)
        g_mazeAuxiliaryModels[index] = 0;
    for (index = 0; ::strcmp(auxiliaryModelNames[index], "end"); ++index) {
        ::sprintf(mazeModelFilename, "%s", auxiliaryModelNames[index]);
        g_mazeAuxiliaryModels[index] = LoadGameModel(mazePackage, mazeModelFilename);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMazeAuthoredObjectModels() {
    for (MazeAuthoredObject *object = g_mazeAuthoredObjects; static_cast<std::int32_t>(object->flags) >= 0; ++object) {
        if (HasMazeUnitAuthoredName(object->name)) {
            ::sprintf(mazeModelFilename, "%s", object->resource);
            object->model = LoadGameModel(mazePackage, mazeModelFilename);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMazeTreeModels() {
    static const char *treeNames[] = {"EFFHIS1G", "EFFHIM1G", "EFFHIL1G", "EFFPISSG", "EFFPIS1G", "EFFPIM1G", "EFFPIL1G", "MON02L4P", "MONY2L4P"};
    for (std::int32_t index = 0; index < 9; ++index) {
        ::sprintf(mazeModelFilename, "%s.MT7", treeNames[index]);
        g_mazeTreeModels[index] = LoadGameModel(mazePackage, mazeModelFilename);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeAuxiliaryVariation(const MazeRoad *road, shen_name name) {
    struct Placement {
        Vector3 position;
        std::int32_t yaw;
    };
    static const std::int32_t models[][2] = {{0, 2}, {1, 3}, {4, 5}, {6, 0}, {9, 0}, {10, 0}};
    static const Placement placements[] = {{{0.63f, 0.82f, -5.58f}, 13747}, {{-6.5f, 0.83f, 3.2f}, 57317}, {{-6.96f, 1.14f, 3.38f}, 2545},
        {{-0.2f, 0.0f, -6.0f}, 55252}, {{1.44f, 0.15f, -5.61f}, 64535}, {{-5.59f, 0.7f, 1.88f}, 32260}, {{0.0f, 0.0f, -3.5f}, 0}, {{0.0f, 0.0f, 0.0f}, 0},
        {{0.0f, 0.0f, 0.0f}, 0}, {{6.48f, 1.72f, 14.08f}, 0}, {{6.48f, 1.18f, 14.08f}, 0}};
    std::int32_t selected[2];
    dc::sdk::SeedRandom(road->block * 0x100 + road->unit);
    std::int32_t variation = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 3.0f);
    switch (g_currentMap) {
    case SHEN_NAME4('K', 'M', 'Z', '1'):
        if (road->unit == 1)
            variation = 3;
        break;
    case SHEN_NAME4('K', 'M', 'Z', '3'):
        if (name == SHEN_NAME4('H', 'I', 'Y', '4'))
            variation = 4;
        break;
    case SHEN_NAME4('K', 'M', 'Z', '4'):
        if (name == SHEN_NAME4('H', 'I', 'Y', '4'))
            variation = 5;
        break;
    }
    if (variation > 2) {
        selected[0] = 1;
        selected[1] = 0;
    } else {
        selected[0] = dc::sdk::RandomFloat() < 0.65f;
        selected[1] = dc::sdk::RandomFloat() < 0.65f;
    }
    for (std::int32_t index = 0; index < 2; ++index) {
        if (selected[index]) {
            const std::int32_t model = models[variation][index];
            matrix_stack::Matrix4x4_Push();
            Matrix4x4::preTranslate(placements[model].position.x, placements[model].position.y, placements[model].position.z);
            Matrix4x4::RotateAroundY_Wrapper(placements[model].yaw);
            DrawCharacterModelTree(g_mazeAuxiliaryModels[model]->root);
            PopCurrentMatrix(1);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeAuthoredObjects(shen_name name, const MazeRoad *road) {
    for (MazeAuthoredObject *object = g_mazeAuthoredObjects; static_cast<std::int32_t>(object->flags) >= 0; ++object) {
        if (object->name != name)
            continue;
        if (object->flags & 0x10000U)
            DrawMazeAuthoredAnimatedObject(object);
        if (object->flags & 0x20000U)
            DrawMazeAuthoredHourObject(object);
        if (object->flags & 0x8000U)
            DrawMazeAuthoredFish(object, road);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const char *mazeTerrainName = "SRL9";
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeMazeModels() {
    static const char *mediumNames[] = {"SRB1", "SRB2", "SRB3", "IRB1", "IRB2", "IRB3", "KRB1", "KRB2", "KRB3", "IOB0", "IOB1", "IOB2", "IRM1", "IRM2", "IRM3",
        "IRM4", "IRM5", "HOM0", "H3F1", "H3F2", "H3F3", "H3F4", "H3F5", "H3F6", "H3F7", "H3F8", "H3F9", "HOM3", "HIJ1", "HIJ2", "HIJ3", "HOJ0", "HOJ1", "H3J1",
        "H3J2", "end"};
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int32_t index;
    ::sprintf(mazeLoadFilename, "%smap", mazeMapNames[GetMazeAuxiliaryWord()]);
    void *mapData = FileLoader(mazePackage, mazeLoadFilename, PACKAGE_BINARY_EXTENSION);
    mazeAuthoredMap = static_cast<std::byte *>(mapData);
    mazeUnitAreas = static_cast<std::byte *>(FileLoader(mazePackage, "unitarea", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[0] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_mori", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[5] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_lwod", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[1] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_iwa", PACKAGE_BINARY_EXTENSION));
    LoadMazeRoadModels();
    MarkMazeMediumModelsUsed();
    for (index = 0; ::strcmp(mediumNames[index], "end"); ++index) {
        if (!mazeMediumModelUsed[index])
            g_mazeMediumModels[index] = 0;
        else {
            ::sprintf(mazeLoadFilename, "MAZ%sG", mediumNames[index]);
            g_mazeMediumModels[index] = LoadGameModel(mazePackage, mazeLoadFilename);
        }
    }
    MarkMazeUnitModelsUsed();
    for (index = 0; ::strcmp(authoredUnits[index].model, "end"); ++index) {
        if (!mazeUnitModelUsed[index])
            g_mazeUnitModels[index] = 0;
        else {
            ::sprintf(mazeLoadFilename, "%sG", authoredUnits[index].model);
            g_mazeUnitModels[index] = LoadGameModel(mazePackage, mazeLoadFilename);
        }
    }
    ::sprintf(mazeLoadFilename, "MAZ%sG", mazeTerrainName);
    g_mazeTerrainModel = LoadGameModel(mazePackage, mazeLoadFilename);
    InitializeMazeRenderModelNullsub();
#else
    char filename[32];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    ::sprintf(mazeLoadFilename, "%smap", mazeMapNames[GetMazeAuxiliaryWord()]);
    mazeAuthoredMap = static_cast<std::byte *>(FileLoader(mazePackage, mazeLoadFilename, PACKAGE_BINARY_EXTENSION));
    mazeUnitAreas = static_cast<std::byte *>(FileLoader(mazePackage, "unitarea", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[0] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_mori", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[5] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_lwod", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[1] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_iwa", PACKAGE_BINARY_EXTENSION));
#endif
    LoadMazeRoadModels();
    MarkMazeMediumModelsUsed();
    for (std::int32_t index = 0; ::strcmp(mediumNames[index], "end"); ++index) {
        if (!mazeMediumModelUsed[index]) {
            g_mazeMediumModels[index] = 0;
        } else {
            ::sprintf(filename, "MAZ%sG", mediumNames[index]);
            g_mazeMediumModels[index] = LoadGameModel(mazePackage, filename);
        }
    }
    MarkMazeUnitModelsUsed();
    for (std::int32_t index = 0; index < 62; ++index) {
        if (!mazeUnitModelUsed[index]) {
            g_mazeUnitModels[index] = 0;
        } else {
            ::sprintf(filename, "%sG", authoredUnits[index].model);
            g_mazeUnitModels[index] = LoadGameModel(mazePackage, filename);
        }
    }
    g_mazeTerrainModel = LoadGameModel(mazePackage, "MAZSRL9G");
    InitializeMazeRenderModelNullsub();
#endif
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMazeDefaults() {
    SetMazeCullingSteps(13);
    SetMazeTerrainDrawEnabled(1);
    SetMazeUnitDrawEnabled(1);
    SetMazeRoadPropsEnabled(1);
    SetMazeRoadDrawEnabled(1);
    SetMazeWireframeEnabled(0);
    SetMazeUnitWallsEnabled(0);
    SetMazeNaviEnabled(1);
    SetMazeModelOverride(0);
    SetMazeUnitDrawDistance(60.0f);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsPositionWithinMazeMarkerRadius(const Vector3 *position, std::int32_t marker, float radius) {
    const float squaredRadius = radius * radius;
    Vector3 target;
    if (!mazeGetMarkerPosition(marker, &target))
        return false;
    const float x = position->x - target.x, z = position->z - target.z;
    if (squaredRadius > x * x + z * z)
        return true;
    return false;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetMazeMarkerSlotPosition(std::int32_t slot, Vector3 *output) {
    GetMazeMarkerCount();
    const MazeMarker *entry = g_mazeMarkers + slot;
    mazeSamplePartition(entry->partition, entry->element, output);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t mazeGetNumPartitions() {
    return g_mazePartitionCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetMZNRDataForID(std::int32_t partition) {
    const std::int32_t count = mazeGetNumPartitions();
    return g_mazePartitions[partition].count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void mazeSamplePartition(std::int32_t partition, std::int32_t element, Vector3 *output) {
    const Vector3 *points;
    const std::int32_t count = GetMZNRDataForID(partition);
    points = g_mazePartitions[partition].points;
    *output = points[element];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void mazeFindClosestTarget(std::int32_t *partition, std::int32_t *element, const Vector3 *point) {
    float globalDistance = 1000000.0f, localDistance = globalDistance;
    const std::int32_t count = mazeGetNumPartitions();
    std::int32_t samples, previousElement, localElement = -1;
    const std::int32_t previousPartition = *partition;
    previousElement = *element;
    for (std::int32_t index = 0; index < count; ++index) {
        samples = GetMZNRDataForID(index);
        for (std::int32_t sample = 0; sample < samples; ++sample) {
            Vector3 position;
            mazeSamplePartition(index, sample, &position);
            const float x = position.x - point->x, z = position.z - point->z;
            const float distance = x * x + z * z;
            if (index == previousPartition && localDistance > distance) {
                localElement = sample;
                localDistance = distance;
            }
            if (globalDistance > distance) {
                globalDistance = distance;
                *partition = index;
                *element = sample;
            }
        }
    }
    if (previousPartition >= 0 && previousPartition != *partition && mazeGetNumPartitions() > previousPartition) {
        samples = GetMZNRDataForID(previousPartition);
        if (previousElement > 4 && previousElement < samples - 4 && 16.0f > localDistance) {
            *partition = previousPartition;
            *element = localElement;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t mazeFindPartitionDirection(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *nextElement) {
    const std::int32_t distance = mazeFindPartitionDistance(sourcePartition, sourceElement, targetPartition, targetElement, nextPartition, nextElement);
    if (distance > 0)
        return 1;
    if (distance < 0)
        return -1;
    return 0;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyMazeAuthoredUV(const MazeAuthoredObject *object) {
    float u, v, amount;
    u = v = amount = 0.0f;
    if (object->flags & 1U)
        amount = 0.04f;
    if (object->flags & 2U)
        amount = 0.02f;
    if (object->flags & 4U)
        amount = 0.001f;
    if (object->flags & 8U)
        amount = 0.0005f;
    if (object->flags & 0x80000U)
        u = amount;
    if (object->flags & 0x40000U)
        v = amount;
    nlSetTexanimUV(u, v);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeAuthoredAnimatedObject(MazeAuthoredObject *object) {
    matrix_stack::Matrix4x4_Push();
    MT7Node *root = object->model->root;
    ApplyMazeAuthoredUV(object);
    if (!shenIsGamePaused())
        RecursiveApplyTexAnimUVs(root);
    if (object->flags & 0x20U)
        Matrix4x4::preTranslate(0.0f, GetMazeBob() * 0.5f, 0.0f);
    if (object->flags & 0x10U)
        Matrix4x4::preTranslate(0.0f, GetMazeBob() * 0.1f, 0.0f);
    DrawCharacterModelTree(root);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeAuthoredHourObject(MazeAuthoredObject *object) {
    DateTime now;
    shenGetCurrentDateTime(now);
    if (now.hour != mazeHourDate.hour && now.minute == mazeHourDate.minute) {
        std::int32_t next = mazeHourFrame + 1;
        if (next > 4)
            next = 0;
        SetMazeHourAnimationFrame(next);
    }
    DrawMazeHourObjectMeshes(object);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeHourObjectMeshes(MazeAuthoredObject *object) {
    MT7Node *root = object->model->root;
    ApplyMazeAuthoredUV(object);
    if (!shenIsGamePaused()) {
        RecursiveApplyTexAnimUVs(root);
        mazeHourBlend -= 0.0125f;
        if (mazeHourBlend < 0.0f)
            mazeHourBlend = 0.0f;
        if (mazeHourBlend > 1.0f)
            mazeHourBlend = 1.0f;
    }
    matrix_stack::Matrix4x4_Push();
    MT7Node *selected = root->child, *previous = selected;
    for (std::int32_t index = 0; index < mazeHourFrame; ++index)
        selected = selected->sibling;
    for (std::int32_t index = 0; index < mazePreviousHourFrame; ++index)
        previous = previous->sibling;
    Matrix4x4::preTranslate(selected->position.x, selected->position.y, selected->position.z);
    dc::sdk::DrawSingleLightMT7MeshAlpha(previous->mesh, mazeHourBlend);
    dc::sdk::DrawSingleLightMT7MeshAlpha(selected->mesh, 1.0f - mazeHourBlend);
    PopCurrentMatrix(1);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ResetMazeHourAnimation(std::int32_t frame) {
    mazeHourFrame = frame;
    mazePreviousHourFrame = frame;
    mazeHourBlend = 0.0f;
    shenGetCurrentDateTime(mazeHourDate);
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void *AllocateMaze(std::size_t bytes, std::uint32_t name) {
    return HLib::AllocateTaskData(bytes, shen_name(name));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeRoad(MazeRoad *road) {
    const std::uint32_t flags = road->flags;
    if (!GetMazeRoadDrawEnabled())
        return;
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(road->position.x, road->position.y, road->position.z);
    Matrix4x4::RotateAroundY_Wrapper(road->yaw);
    ApplyMazeRoadJoins(road);
    DrawCharacterModelTree(GetMazeRoadModel(road)->root);
    if (!(flags & 0x20000000U)) {
        DrawMazeRoadProps(road);
        DrawMazeRoadDecoration(road);
    }
    PopCurrentMatrix(1);
    ++mazeRoadDrawCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeRoadProps(const MazeRoad *road) {
    if (!GetMazeRoadPropsEnabled())
        return;
    MazePropState props[32];
    GenerateMazePropStates(road, props);
    DrawMazePropStates(props);
    DrawMazeTreeEffects(road);
    DrawMazeButterflyEffectsSeeded(road);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetMazeHourAnimationFrame(std::int32_t frame) {
    mazePreviousHourFrame = mazeHourFrame;
    mazeHourFrame = frame;
    mazeHourBlend = 1.0f;
    shenGetCurrentDateTime(mazeHourDate);
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeMazeHourAnimation() {
    mazeHourBlend = 0.0f;
    mazePreviousHourFrame = 2;
    mazeHourFrame = 2;
    shenGetCurrentDateTime(mazeHourDate);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GenerateMazePropStates(const MazeRoad *road, MazePropState *output) {
    const MazePropRecord *record = GetMazePropTable(GetMazeRoadStyle(road));
    dc::sdk::SeedRandom(road->block * 0x800 + road->index);
    do {
        if (record->flags & 0x4000U) {
            ++record;
            if (MazeRecordMatchesIndex(record->flags, road->index)) {
                record = SelectMazePropChoice(record);
                output = MakeMazePropState(record, road, output);
                while (!(record->flags & 0x8000U))
                    ++record;
            }
        } else if (MazePropRecordSelected(record, road)) {
            output = MakeMazePropState(record, road, output);
        }
        ++record;
    } while (record->flags != 0xffffffffU);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t MazePropRecordSelected(const MazePropRecord *record, const MazeRoad *road) {
    if (record->flags & 0x2000U)
        return 0;
    std::uint32_t selected = record->chance > dc::sdk::RandomFloat();
    selected &= MazeRecordMatchesIndex(record->flags, road->index);
    return selected;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const MazePropRecord *SelectMazePropChoice(const MazePropRecord *record) {
    std::int32_t count = 0;
    if (record->flags & 0x2000U)
        return 0;
    const MazePropRecord *choices = record;
    for (; !(record->flags & 0x8000U); ++record)
        ++count;
    if (!count)
        return 0;
    count = static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(count));
    return choices + count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t MazeRecordMatchesIndex(std::uint32_t flags, std::int32_t index) {
    if ((flags & 0x400U) && (index & 1))
        return 0;
    if ((flags & 0x800U) && index != index / 3 * 3)
        return 0;
    if ((flags & 0x1000U) && (index & 3))
        return 0;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazePropState *MakeMazePropState(const MazePropRecord *record, const MazeRoad *, MazePropState *output) {
    if (!record)
        return output;
    const std::int32_t kind = record->kind;
    float x = dc::sdk::RandomFloat() * (record->maxX - record->minX) + record->minX;
    if (dc::sdk::RandomFloat() > 0.5f)
        x = -x;
    float z = dc::sdk::RandomFloat() * 2.0f - 1.0f;
    if (((record->flags & 1U) && x > 0.0f) || ((record->flags & 2U) && x < 0.0f))
        x = -x;
    if (record->flags & 0x40U)
        z = 0.0f;
    output->x = x;
    output->z = z;
    output->height = record->height;
    std::int32_t yaw = dc::sdk::RandomInt() * 2;
    if (record->flags & 8U)
        yaw = 0;
    if (record->flags & 0x10U)
        yaw = 0x8000;
    if (record->flags & 0x20U)
        yaw = yaw > 0x8000 ? 0x8000 : 0;
    output->yaw = yaw;
    output->drawState = record->drawState;
    output->kind = kind;
    output->flags = record->flags;
    ++output;
    if (record->flags & 4U) {
        output->drawState = record->drawState;
        output->kind = kind;
        output->x = -x;
        output->z = z;
        output->height = record->height;
        output->flags = output[-1].flags;
        output->yaw = output[-1].yaw;
        ++output;
    }
    output->kind = -1;
    return output;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazePropStates(const MazePropState *states) {
    for (std::int32_t index = 0; states[index].kind >= 0; ++index) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(states[index].x, states[index].height, states[index].z);
        g_mazeMediumModels[states[index].kind]->root->rotation.y = states[index].yaw;
        if (states[index].drawState)
            DrawCharacterModelTree(g_mazeMediumModels[states[index].kind]->root);
        PopCurrentMatrix(1);
        ++mazePropDrawCount;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float GetMazeBob() {
    float value;
    std::int32_t phase, current, previous;
    float fraction;
    phase = mazeGetCurrentRefCount();
    current = (phase & 0xf0) >> 4;
    previous = current - 1;
    fraction = static_cast<float>(phase & 15) * 0.0625f;
    value = (mazeBob[current] - mazeBob[previous & 15]) * fraction + mazeBob[previous & 15];
    return value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float GetMazeBob() {
    const std::uint32_t current = (mazeGetCurrentRefCount() & 0xf0U) >> 4, previous = (current - 1) & 15U;
    const float fraction = static_cast<float>(mazeGetCurrentRefCount() & 15U) * 0.0625f;
    return (mazeBob[current] - mazeBob[previous]) * fraction + mazeBob[previous];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CopyMazeMatrixBasis(const Matrix4x4 *source) {
    Matrix4x4 current;
    matrix_stack::GetMatrix(&current);
    for (std::int32_t row = 0; row < 3; ++row) {
        current.rows[row].x = source->rows[row].x;
        current.rows[row].y = source->rows[row].y;
        current.rows[row].z = source->rows[row].z;
        current.rows[row].w = 0.0f;
    }
    matrix_stack::LoadMatrix(&current);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeMazeFrameEnvironment() {
    matrix_stack::PushUnitMatrix();
    matrix_stack::GetMatrix(&mazeFlutterTransform);
    Matrix4x4::RotateAroundX_Wrapper(0x1200);
    Matrix4x4::RotateAroundZ_Wrapper(0x1200);
    Matrix4x4::RotateAroundY_Wrapper((mazeGetCurrentRefCount() & 0x1ffU) << 7);
    matrix_stack::GetMatrix(&mazeEffectTransform);
    PopCurrentMatrix(1);
    mazeHeavyWeather = false;
    mazeClearWeather = false;
    DateTime date;
    shenGetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 7) < 10) {
        if (GetWeatherPressure() > 0.7f)
            mazeHeavyWeather = true;
        if (GetWeatherPressure() < 0.3f && GetWeatherWindVector() < 0.5f)
            mazeClearWeather = true;
    }
    camCtrlGetNextCamPos(&mazeCameraPosition);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t UpdateMazeRoadTransforms(MazeRoad *roads, const Vector3 *camera, float *nearestDistance) {
    float nearest = 25600.0f;
    std::int32_t result = -1, index = 0;
    float farSquared = camCtrlGetFarDistance();
    farSquared *= farSquared;
    const float unitSquared = GetMazeUnitDrawDistance() * GetMazeUnitDrawDistance();
    for (MazeRoad *road = roads; road->index >= 0; ++road, ++index) {
        const float x = camera->x - road->position.x, z = camera->z - road->position.z;
        road->cameraDistance = x * x + z * z;
        if (road->flags & 0xffffU) {
            if (static_cast<std::int32_t>(road->flags) < 0 && unitSquared > road->cameraDistance)
                InsertMazeUnit(road);
        } else if (farSquared > road->cameraDistance) {
            InsertMazeRoad(road);
        }
        if (nearest > road->cameraDistance) {
            result = index;
            nearest = road->cameraDistance;
        }
    }
    *nearestDistance = nearest;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ClearMazeVisibleLists() {
    for (std::int32_t index = 0; index < 120; ++index)
        g_currentMazeRoads[index] = 0;
    for (std::int32_t index = 0; index < 8; ++index)
        g_currentMazeUnits[index] = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InsertMazeRoad(MazeRoad *road) {
    std::int32_t index;
    for (index = 118; index >= 0; --index) {
        if (g_currentMazeRoads[index] && g_currentMazeRoads[index]->cameraDistance < road->cameraDistance) {
            ++index;
            break;
        }
    }
    if (index < 0)
        index = 0;
    if (index > 118)
        return;
    for (std::int32_t copy = 118; copy > index; --copy)
        g_currentMazeRoads[copy] = g_currentMazeRoads[copy - 1];
    g_currentMazeRoads[index] = road;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InsertMazeUnit(MazeRoad *road) {
    std::int32_t index;
    for (index = 7; index >= 0; --index) {
        if (g_currentMazeUnits[index] && g_currentMazeUnits[index]->cameraDistance < road->cameraDistance) {
            ++index;
            break;
        }
    }
    if (index < 0)
        index = 0;
    if (index > 7)
        return;
    for (std::int32_t copy = 7; copy > index; --copy)
        g_currentMazeUnits[copy] = g_currentMazeUnits[copy - 1];
    g_currentMazeUnits[index] = road;
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitializeMazeFishPath() {
    static const Vector2 mazeFishPathControls[] = {{-1.91311002f, -0.219183996f}, {-1.92379701f, -0.572017014f}, {-1.91311002f, -0.924848974f},
        {-1.84898305f, -1.341833f}, {-1.560413f, -1.68397403f}, {-0.983273983f, -1.85504401f}, {-0.491636992f, -1.75881696f}, {-0.106877998f, -1.52359605f},
        {0.224443004f, -0.989000976f}, {0.427511007f, -0.219183996f}, {0.427511007f, 0.657552004f}, {-0.480949014f, 0.850005984f},
        {-0.972585976f, 0.882081985f}, {-1.06877601f, 1.48082805f}, {-0.598514974f, 1.79089296f}, {0.513013005f, 1.87642801f}, {1.76348102f, 1.61982298f},
        {1.82760799f, 0.539941013f}, {1.02602506f, 0.0481140018f}, {1.710042f, -0.775162995f}, {1.69935405f, -1.63051498f}, {1.16496599f, -1.85504401f},
        {0.491636992f, -1.92988801f}, {-0.171003997f, -1.64120698f}, {-0.395446986f, -1.13868701f}, {-0.470261991f, -0.572017014f},
        {-0.887084007f, 0.197799996f}, {-1.11152697f, 0.700318992f}, {-0.662640989f, 1.24560595f}, {-1.12221503f, 1.73743403f}, {-1.77416897f, 1.416677f},
        {-1.87035799f, 0.850005984f}, {-1.92379701f, 0.539941013f}, {-1.92379701f, 0.176415995f}, {-1.92379701f, -0.197799996f}, {-1.91311002f, -0.219183996f},
        {-1.92379701f, -0.572017014f}, {1000.0f, 1000.0f}};
    const shen_name allocationName = SHEN_NAME4('M', 'Z', 'F', 'S');
    std::int32_t count = 0;
    while (mazeFishPathControls[count].x < 999.0f)
        ++count;
    mazeFishControlCount = count;
    mazeFishControlPoints = static_cast<Vector3 *>(HLib::AllocateTaskData(count * 12, allocationName));
    for (std::int32_t index = 0; index < count; ++index) {
        mazeFishControlPoints[index].x = mazeFishPathControls[index].x;
        mazeFishControlPoints[index].y = 0.0f;
        mazeFishControlPoints[index].z = mazeFishPathControls[index].y;
    }
    Vector3 *const samples = static_cast<Vector3 *>(HLib::AllocateTaskData(count * 0x300, allocationName));
    Vector3 *cursor = samples;
    for (std::int32_t segment = 1; segment < count - 2; ++segment) {
        SampleMazeSpline(mazeFishControlPoints, mazeFishControlCount, (segment - 1) * 64, cursor);
        cursor += 64;
    }
    std::int32_t index;
    for (index = 0; index < 16; ++index) {
        cursor[index].x = (samples[0].x - cursor[-1].x) * (static_cast<float>(index + 1) / 17.0f) + cursor[-1].x;
        cursor[index].z = cursor[-1].z + (samples[0].z - cursor[-1].z) * (static_cast<float>(index + 1) / 17.0f);
    }
    cursor[index].x = 1000.0f;
    cursor[index].z = 1000.0f;
    mazeFishPathLength = (count - 3) * 64 + 15;
    mazeFishPathSamples = samples;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeAuthoredFish(const MazeAuthoredObject *object, const MazeRoad *road) {
    static const Vector3 boxes[] = {{-3.5f, -1.6f, -4.5f}, {-1.0f, -1.6f, -0.1f}, {-1.0f, -1.6f, -4.5f}, {3.8f, -1.6f, -5.5f}, {-2.8f, -1.1f, 6.33f},
        {-6.55f, -1.1f, 0.17f}, {-6.55f, -1.1f, 0.17f}, {-0.53f, -1.1f, -9.8f}};
    const std::int32_t box = object->name == SHEN_NAME4('H', 'I', 'Y', '1') ? 0 : 4;
    const MT7Node *root = object->model->root;
    const std::int32_t seed = road->block * 0x80 + road->unit;
    DrawMazeFish(boxes + box, boxes + box + 1, 9, seed, root);
    DrawMazeFish(boxes + box + 2, boxes + box + 3, 9, seed, root);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawMazeFish(const Vector3 *boxMin, const Vector3 *boxMax, std::int32_t count, std::int32_t seed, const MT7Node *fish) {
    dc::sdk::SeedRandom(seed);
    if (mazeFishLastReferenceCount != mazeGetCurrentRefCount()) {
        mazeFishLastReferenceCount = mazeGetCurrentRefCount();
        ++mazeFishPathCursor;
    }
    if (mazeFishPathCursor >= mazeFishPathLength)
        mazeFishPathCursor = 0;
    for (std::int32_t index = 0; index < count; ++index) {
        std::int32_t offset = static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(mazeFishPathLength));
        if (offset + mazeFishPathCursor >= mazeFishPathLength)
            offset -= mazeFishPathLength;
        const Vector3 *point = &mazeFishPathSamples[mazeFishPathCursor] + offset;
        const Vector3 *next = point + 1;
        if (offset + mazeFishPathCursor + 1 >= mazeFishPathLength)
            next = mazeFishPathSamples;
        const float fraction = dc::sdk::RandomFloat();
        Vector3 position;
        position.x = fraction * (boxMax->x - boxMin->x) + boxMin->x;
        position.y = fraction * (boxMax->y - boxMin->y) + boxMin->y;
        position.z = boxMin->z + fraction * (boxMax->z - boxMin->z);
        const float jitter = dc::sdk::RandomFloat() * 0.60000002f - 0.30000001f;
        const std::uint32_t yaw = dc::sdk::ArcTan2Fixed(point->x - next->x, point->z - next->z);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(position.x + point->x, position.y + jitter, position.z + point->z);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        DrawCharacterModelTree(fish);
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MazeFishNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyMazeMediumChildWiggle() {
    std::int32_t index = 0;
    const std::int32_t phase = static_cast<std::int32_t>(mazeGetCurrentRefCount()) >> 1;
    for (; mazeMediumWiggleChildren[index][0]; ++index) {
        MT7Node *node = g_mazeMediumModels[mazeMediumWiggleChildren[index][0]]->root->child;
        for (std::int32_t child = 0; child < mazeMediumWiggleChildren[index][1]; ++child)
            node = node->sibling;
        SetMazeNodeWiggle(phase + mazeMediumWiggleChildren[index][0] + index * 4, node, 1, 4);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyMazeUnitChildWiggle() {
    const std::int32_t phase = static_cast<std::int32_t>(mazeGetCurrentRefCount()) >> 1;
    for (std::int32_t index = 0; index < 2; ++index) {
        if (!mazeHangingUnitModels[index])
            continue;
        std::int32_t step = 0;
        for (MT7Node *node = g_mazeUnitModels[mazeHangingUnitModels[index]]->root->child->child; node; node = node->sibling, ++step)
            SetMazeNodeWiggle(phase + step * 2, node, 4, 6);
    }
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMazeNodeWiggle(std::uint32_t phase, MT7Node *node, std::int32_t amplitude, std::uint32_t axes) {
    std::int32_t value = amplitude * mazeWiggle[phase & 15U];
    value *= (phase & 0x10U) ? amplitude : -amplitude;
    node->rotation.x = (axes & 2U) ? value : 0;
    node->rotation.y = (axes & 4U) ? value : 0;
    node->rotation.z = (axes & 8U) ? value : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FindMazeHangingUnitModels() {
    std::int32_t model = 0;
    mazeHangingUnitModels[0] = 0;
    mazeHangingUnitModels[1] = 0;
    for (; ::strcmp(authoredUnits[model].model, "end"); ++model) {
        if (mazeUnitModelUsed[model]) {
            if (!::strcmp(authoredUnits[model].model, "UNTHOA0"))
                mazeHangingUnitModels[0] = model;
            if (!::strcmp(authoredUnits[model].model, "UNTHOS0"))
                mazeHangingUnitModels[1] = model;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMazeRoadFlags(MazeRoad *road, std::int32_t record, std::int32_t sample) {
    std::int32_t type, next, previous, metadata;
    type = GetAuthoredWord(road->block, record);
    next = GetAuthoredWord(road->block, record + 1);
    previous = GetAuthoredWord(road->block, record - 1);
    metadata = GetAuthoredRecordStyle(road->block, record);
    if (type) {
        if (!metadata) {
            const std::int32_t before = GetAuthoredRecordStyle(road->block, record - 1);
            const std::int32_t after = GetAuthoredRecordStyle(road->block, record + 1);
            if (after)
                metadata = after;
            if (before)
                metadata = before;
        }
        road->flags = record;
        if (!next)
            road->flags = sample ? 0 : 0x24000000U;
        if (!previous && !sample)
            road->flags = 0x28000000U;
    }
    if ((type & 0x80000000U) && !sample)
        road->flags |= 0x80000000U;
    if (!sample)
        road->flags |= 0x40000000U;
    road->flags |= static_cast<std::uint32_t>(metadata) << 16;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SampleMazeRoadPlacement(float sample, const Vector3 *samples, MazeRoad *road) {
    const std::int32_t index = static_cast<std::int32_t>(sample);
    const float fraction = sample - static_cast<float>(index);
    const Vector3 current = samples[index], next = samples[index + 1];
    road->position.x = current.x + (next.x - current.x) * fraction;
    road->position.y = current.y + (next.y - current.y) * fraction;
    road->position.z = current.z + (next.z - current.z) * fraction;
    road->yaw = InverseFixedAtan2(next.x - current.x, next.z - current.z);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float MazeSplineLength(std::int32_t count, const Vector3 *points) {
    float length = 0.0f;
    if (count > 0) {
        Vector3 difference = *points++;
        for (std::int32_t index = 0; index < count; ++index, ++points) {
            const Vector3 next = *points;
            difference.x -= next.x;
            difference.y -= next.y;
            difference.z -= next.z;
            length += Vector3::CalcMagnitude(difference);
            difference = next;
        }
    }
    return length;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SampleMazeSpline(const Vector3 *points, std::int32_t count, std::int32_t start, Vector3 *output) {
    std::int32_t last = count;
    last -= 3;
    last *= 64;
    std::int32_t sample = 0;
    if (start > last)
        return;
    do {
        InterpolateNpcSplineNormalized(points, count, start + sample, output);
        ++sample;
        ++output;
    } while (sample <= 64 && start + sample <= last);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildMazeRoadJoins(MazeRoad *road) {
    Vector3 leftOut, rightOut, left, right, join;
    InitMazeJoinVector(&join);
    while (road->index >= 0) {
        MazeRoad *current = road;
        MazeRoad *next = ++road;
        if (next->index == -1) {
            current->leftJoin.x = -join.x;
            current->leftJoin.y = 0.0f;
            current->leftJoin.z = -join.z;
            reinterpret_cast<std::uint32_t &>(current->leftJoin.x) |= 1U;
            current->rightJoin.x = join.x;
            current->rightJoin.y = 0.0f;
            current->rightJoin.z = -join.z;
            reinterpret_cast<std::uint32_t &>(current->rightJoin.x) |= 1U;
            return;
        }
        matrix_stack::Matrix4x4_Push();
        matrix_stack::UnitCurrentMatrix();
        Matrix4x4::preTranslate(next->position.x, next->position.y, next->position.z);
        Matrix4x4::RotateAroundY_Wrapper(next->yaw);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(-join.x, 0.0f, join.z);
        matrix_stack::GetTranslation(&left);
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(join.x, 0.0f, join.z);
        matrix_stack::GetTranslation(&right);
        PopCurrentMatrix(1);
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push();
        matrix_stack::UnitCurrentMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-current->yaw);
        Matrix4x4::preTranslate(-current->position.x, -current->position.y, -current->position.z);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(left.x, left.y, left.z);
        matrix_stack::GetTranslation(&leftOut);
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(right.x, right.y, right.z);
        matrix_stack::GetTranslation(&rightOut);
        PopCurrentMatrix(1);
        if (leftOut.z > 1.0f)
            leftOut.z = 1.0f;
        if (rightOut.z > 1.0f)
            rightOut.z = 1.0f;
        leftOut.y = next->position.y - current->position.y;
        rightOut.y = leftOut.y;
        reinterpret_cast<std::uint32_t &>(leftOut.x) |= 1U;
        reinterpret_cast<std::uint32_t &>(rightOut.x) |= 1U;
        current->leftJoin = leftOut;
        current->rightJoin = rightOut;
        PopCurrentMatrix(1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildMazeRoadEndJoins(MazeRoad *road) {
    Vector3 left, right, join, rightEnd, leftEnd, centre;
    InitMazeJoinVector(&join);
    while (road->index >= 0) {
        MazeRoad *current = road++;
        left = current->leftJoin;
        reinterpret_cast<std::uint32_t &>(left.x) &= ~1U;
        right = current->rightJoin;
        reinterpret_cast<std::uint32_t &>(right.x) &= ~1U;
        const float scale = 7.5f / join.x;
        centre.x = (right.x + left.x) * 0.5f;
        centre.z = (right.z + left.z) * 0.5f;
        rightEnd.x = scale * (right.x - centre.x) + centre.x;
        rightEnd.z = scale * (right.z - centre.z) + centre.z;
        leftEnd.x = scale * (left.x - centre.x) + centre.x;
        leftEnd.z = scale * (left.z - centre.z) + centre.z;
        matrix_stack::UnitCurrentMatrix();
        Matrix4x4::preTranslate(current->position.x, current->position.y, current->position.z);
        Matrix4x4::RotateAroundY_Wrapper(current->yaw);
        matrix_stack::Matrix4x4_Push(), Matrix4x4::preTranslate(-join.x * scale, 0.0f, join.z), matrix_stack::GetTranslation(&current->leftStart),
            PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push(), Matrix4x4::preTranslate(leftEnd.x, 0.0f, leftEnd.z), matrix_stack::GetTranslation(&current->leftEnd),
            PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push(), Matrix4x4::preTranslate(join.x * scale, 0.0f, join.z), matrix_stack::GetTranslation(&current->rightStart),
            PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push(), Matrix4x4::preTranslate(rightEnd.x, 0.0f, rightEnd.z), matrix_stack::GetTranslation(&current->rightEnd),
            PopCurrentMatrix(1);
        current->rightCentre.x = (current->rightStart.x + current->rightEnd.x) * 0.5f;
        current->rightCentre.z = (current->rightStart.z + current->rightEnd.z) * 0.5f;
        current->leftCentre.x = (current->leftStart.x + current->leftEnd.x) * 0.5f;
        current->leftCentre.z = (current->leftStart.z + current->leftEnd.z) * 0.5f;
    }
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildMazeUnits(MazeRoad *road) {
    for (; road->index >= 0; ++road) {
        if (road->flags & 0x80000000U) {
            BuildMazeUnit(road, MUZT_Data + g_mazeUnitCount);
            road->unit = g_mazeUnitCount++;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildMazeUnit(MazeRoad *road, MazeUnit *unit) {
    const std::int32_t model = GetAuthoredWord(road->block, road->flags & 0xffffU) & 0xffffU;
    const MazeUnitArea *area = reinterpret_cast<const MazeUnitArea *>(mazeUnitAreas) + authoredUnits[model].areaIndex;
    unit->wallCount = area->wallCount;
    unit->pointCount = area->pointCount;
    unit->road = road;
    unit->modelIndex = model;
    matrix_stack::Matrix4x4_Push();
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(road->position.x, road->position.y, road->position.z);
    Matrix4x4::RotateAroundY_Wrapper(0x4000 - GetAuthoredRecordYaw(road->block, road->flags & 0xffffU));
    unit->numLists = CountMazePointerList(reinterpret_cast<const void *const *>(authoredUnits[model].paths));
    unit->pointLists = BuildMazeUnitPaths(authoredUnits[model].paths);
    for (std::int32_t index = 0; index < 4; ++index) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(area->quad[index].x, 0.0f, area->quad[index].y);
        matrix_stack::GetTranslation(&unit->quad[index]);
        PopCurrentMatrix(1);
    }
    for (std::int32_t index = 0; index < unit->wallCount; ++index) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(area->walls[index].x, 0.0f, area->walls[index].y);
        matrix_stack::GetTranslation(&unit->walls[index][0]);
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(area->walls[index].z, 0.0f, area->walls[index].w);
        matrix_stack::GetTranslation(&unit->walls[index][1]);
        PopCurrentMatrix(1);
    }
    for (std::int32_t index = 0; index < unit->pointCount; ++index) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(area->points[index].x, 0.0f, area->points[index].y);
        matrix_stack::GetTranslation(&unit->points[index]);
        PopCurrentMatrix(1);
    }
    PopCurrentMatrix(1);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void mazeSetCurrentRefCount(std::uint32_t count) {
    mazeReferenceCount = count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t mazeGetCurrentRefCount() {
    return mazeReferenceCount;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool MazeRoadPassesCameraAngle(const MazeRoad *road, std::int32_t unit) {
    Vector3 difference, position, camera, forward;
    matrix_stack::Matrix4x4_Push();
    matrix_stack::UnitCurrentMatrix();
    const std::int32_t yaw = camCtrlGetNextRotY();
    camCtrlGetNextCamPos(&camera);
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::preTranslate(0.0f, 0.0f, mazeCameraLookAhead);
    matrix_stack::GetTranslation(&forward);
    difference.x = road->position.x - forward.x;
    difference.y = road->position.y - forward.y;
    difference.z = road->position.z - forward.z;
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    Matrix4x4::preTranslate(difference.x, difference.y, difference.z);
    matrix_stack::GetTranslation(&position);
    PopCurrentMatrix(1);
    std::int32_t angle;
    float distance;
    switch (unit) {
    case 1:
        distance = position.x * position.x + position.z * position.z;
        if (distance < mazeNearCullDistanceSq)
            return true;
        if (26.0f > position.z)
            return true;
        angle = dc::sdk::ArcTan2Fixed(-position.x, -position.z);
        if (static_cast<std::uint32_t>(angle - 0x4001) > 0x7ffeU)
            return true;
        return false;
    case 0:
    default:
        distance = position.x * position.x + position.z * position.z;
        if (distance < mazeNearCullDistanceSq)
            return true;
        if (position.z > 0.0f)
            return false;
        angle = dc::sdk::ArcTan2Fixed(-position.x, -position.z);
        if (angle > GetMazeCullingAngleLimit() && angle < 0x10000 - GetMazeCullingAngleLimit())
            return false;
        return true;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t FilterMazeVisibleRoadLists(MazeRoad **output, MazeRoad **candidates) {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    std::int32_t index = 8;
    {
        std::int32_t other = 0;
        do {
            output[other] = candidates[other];
        } while (++other < index);
    }
    std::int32_t visible;
    std::int32_t other;
    std::int32_t admitted = index;
    for (; candidates[index]; ++index) {
        visible = 0;
        std::int32_t blocked = 0;
        other = index - 1;
        do {
            std::int32_t hit = MazeSegmentsIntersect(camera, candidates[index]->leftCentre, candidates[other]->leftStart, candidates[other]->leftEnd);
            if (!hit)
                hit = MazeSegmentsIntersect(camera, candidates[index]->leftCentre, candidates[other]->rightStart, candidates[other]->rightEnd);
            if (hit) {
                ++blocked;
                break;
            }
        } while (--other >= 0);
        if (!blocked)
            blocked = MazeUnitWallsBlock(candidates[index], &camera, &candidates[index]->leftCentre);
        if (!blocked)
            visible = 1;
        if (!visible) {
            other = index - 1;
            do {
                std::int32_t hit = MazeSegmentsIntersect(camera, candidates[index]->rightCentre, candidates[other]->leftStart, candidates[other]->leftEnd);
                if (!hit)
                    hit = MazeSegmentsIntersect(camera, candidates[index]->rightCentre, candidates[other]->rightStart, candidates[other]->rightEnd);
                if (hit) {
                    ++blocked;
                    break;
                }
            } while (--other >= 0);
            if (blocked < 1)
                blocked += MazeUnitWallsBlock(candidates[index], &camera, &candidates[index]->rightCentre);
            if (blocked < 1)
                visible = 1;
        }
        if (!visible) {
            other = index;
            do {
                std::int32_t hit = MazeSegmentsIntersect(camera, candidates[index]->position, candidates[other]->leftStart, candidates[other]->leftEnd);
                if (!hit)
                    hit = MazeSegmentsIntersect(camera, candidates[index]->position, candidates[other]->rightStart, candidates[other]->rightEnd);
                if (hit) {
                    ++blocked;
                    break;
                }
            } while (--other >= 0);
            if (blocked < 2)
                blocked += MazeUnitWallsBlock(candidates[index], &camera, &candidates[index]->position);
            if (blocked < 2)
                visible = 1;
        }
        if (visible)
            output[admitted++] = candidates[index];
    }
    return admitted;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t MazeUnitHasVisiblePoint(const MazeRoad *road) {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    const MazeUnit *unit = MUZT_Data + road->unit;
    std::int32_t blocked = 0, index;
    for (index = 0; index < unit->pointCount; ++index)
        if (MazeVisibleRoadsBlock(&camera, &unit->points[index]))
            ++blocked;
    if (blocked < index)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t MazeVisibleRoadsBlock(const Vector3 *camera, const Vector3 *target) {
    for (std::int32_t index = 0; g_currentMazeRoads[index]; ++index) {
        const MazeRoad *road = g_currentMazeRoads[index];
        if (MazeSegmentsIntersect(*camera, *target, road->leftStart, road->leftEnd))
            return 1;
        if (MazeSegmentsIntersect(*camera, *target, road->rightStart, road->rightEnd))
            return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool MazeUnitWallsBlock(const MazeRoad *road, const Vector3 *camera, const Vector3 *target) {
    for (std::int32_t index = 0; g_currentMazeUnits[index]; ++index) {
        const MazeRoad *current = g_currentMazeUnits[index];
        const MazeUnit *unit = MUZT_Data + current->unit;
        if (road->cameraDistance > current->cameraDistance) {
            for (std::int32_t wall = 0; wall < unit->wallCount; ++wall) {
                if (MazeSegmentsIntersect(*camera, *target, unit->walls[wall][0], unit->walls[wall][1])) {
                    ++mazeUnitWallHitCount;
                    return true;
                }
            }
        }
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool MazeUnitWallsBlock(const MazeRoad *road, const Vector3 *camera, const Vector3 *target) {
    for (std::int32_t index = 0; g_currentMazeUnits[index]; ++index) {
        const MazeRoad *current = g_currentMazeUnits[index];
        if (!(road->cameraDistance > current->cameraDistance))
            continue;
        const MazeUnit *unit = MUZT_Data + current->unit;
        for (std::int32_t wall = 0; wall < unit->wallCount; ++wall) {
            const Vector3 &first = unit->walls[wall][0], &second = unit->walls[wall][1];
            const float crossFirst = (first.x - target->x) * (target->z - camera->z) - (first.z - target->z) * (target->x - camera->x);
            const float crossSecond = (second.x - target->x) * (target->z - camera->z) - (second.z - target->z) * (target->x - camera->x);
            if (!(crossFirst * crossSecond <= -0.0f))
                continue;
            const float crossTarget = (target->x - second.x) * (second.z - first.z) - (target->z - second.z) * (second.x - first.x);
            const float crossCamera = (camera->x - second.x) * (second.z - first.z) - (camera->z - second.z) * (second.x - first.x);
            if (crossTarget * crossCamera <= -0.0f)
                return true;
        }
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool MazeSegmentsIntersect(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d) {
    float deltaX = b.x - a.x;
    float deltaZ = b.z - a.z;
    float offsetX = d.x - b.x;
    float offsetZ = d.z - b.z;
    float first = deltaZ * offsetX - deltaX * offsetZ;
    offsetX = c.x - b.x;
    offsetZ = c.z - b.z;
    float second = deltaZ * offsetX - deltaX * offsetZ;
    if (first * second > -0.0f)
        return false;
    deltaX = d.x - c.x;
    deltaZ = d.z - c.z;
    offsetX = b.x - d.x;
    offsetZ = b.z - d.z;
    first = deltaZ * offsetX - deltaX * offsetZ;
    offsetX = a.x - d.x;
    offsetZ = a.z - d.z;
    second = deltaZ * offsetX - deltaX * offsetZ;
    if (first * second > -0.0f)
        return false;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool MazeSegmentsIntersect(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d) {
    const float crossC = (c.x - b.x) * (b.z - a.z) - (c.z - b.z) * (b.x - a.x);
    const float crossD = (d.x - b.x) * (b.z - a.z) - (d.z - b.z) * (b.x - a.x);
    if (crossC * crossD > -0.0f)
        return false;
    const float crossA = (a.x - d.x) * (d.z - c.z) - (a.z - d.z) * (d.x - c.x);
    const float crossB = (b.x - d.x) * (d.z - c.z) - (b.z - d.z) * (d.x - c.x);
    return crossA * crossB <= -0.0f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyMazeRoadJoins(MazeRoad *road) {
    MT7Model *model = GetMazeRoadModel(road);
    Vector3 *join = reinterpret_cast<Vector3 *>(static_cast<std::byte *>(model->root->mesh) + 0xd0);
    join->x = road->leftJoin.x;
    join->y = road->leftJoin.y;
    join->z = road->leftJoin.z;
    join = reinterpret_cast<Vector3 *>(static_cast<std::byte *>(model->root->mesh) + 0xb0);
    join->x = road->rightJoin.x;
    join->y = road->rightJoin.y;
    join->z = road->rightJoin.z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FilterMazeVisibleRoads() {
    std::int32_t index = 0, count = 0;
    matrix_stack::Matrix4x4_Push();
    matrix_stack::UnitCurrentMatrix();
    for (; index < 120; ++index) {
        MazeRoad *road = g_currentMazeRoads[index];
        if (!road)
            break;
        if (MazeRoadPassesCameraAngle(road, 0))
            g_mazeVisibleCandidates[count++] = road;
    }
    if (count > 120)
        count = 119;
    g_mazeVisibleCandidates[count] = 0;
    if (count > 8) {
        g_currentMazeRoads[FilterMazeVisibleRoadLists(g_currentMazeRoads, g_mazeVisibleCandidates)] = 0;
    } else {
        index = 0;
        do {
            g_currentMazeRoads[index] = g_mazeVisibleCandidates[index];
        } while (++index <= count);
    }
    PopCurrentMatrix(1);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetMazeBlockCount() {
    return *reinterpret_cast<std::uint32_t *>(mazeAuthoredMap) & 0xffffU;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::byte *GetAuthoredBlock(std::int32_t block) {
    std::byte *entry = mazeAuthoredMap;
    entry += 4;
    for (std::int32_t index = 0; index < block; ++index) {
        const std::uint32_t count = *reinterpret_cast<std::uint32_t *>(entry);
        entry += 4;
        entry += count * 0x38;
    }
    return entry;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeBlockPointCount(std::int32_t block) {
    return *reinterpret_cast<std::uint32_t *>(GetAuthoredBlock(block)) & 0xffffU;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetAuthoredWord(std::int32_t block, std::int32_t record) {
    std::byte *records = GetAuthoredBlock(block);
    records += 4;
    return *reinterpret_cast<std::int32_t *>(records + record * 0x38);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetAuthoredRecordStyle(std::int32_t block, std::int32_t record) {
    return *reinterpret_cast<std::int32_t *>(GetAuthoredBlock(block) + record * 0x38 + 0x20);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 *GetAuthoredRecordPoint(std::int32_t block, std::int32_t record) {
    return reinterpret_cast<Vector3 *>(GetAuthoredBlock(block) + record * 0x38 + 0x10);
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetAuthoredRecordYaw(std::int32_t block, std::int32_t record) {
    return *reinterpret_cast<std::int32_t *>(GetAuthoredBlock(block) + record * 0x38 + 0x1c);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t mazeFindPartitionDistance(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *volatile nextElement) {
    RouteSearch search;
    search.sourcePartition = sourcePartition;
    search.sourceElement = sourceElement;
    search.partition = sourcePartition;
    search.element = sourceElement;
    search.targetPartition = targetPartition;
    search.targetElement = targetElement;
    search.nextPartition = -1;
    search.nextElement = 0;
    search.retained18 = 0;
    ClearMazeRouteVisited();
    const std::uint32_t backward = SearchRoute(search, 0);
    *nextPartition = search.nextPartition;
    *nextElement = search.nextElement;
    ClearMazeRouteVisited();
    const std::uint32_t forward = SearchRoute(search, 1);
    const std::int32_t chosenPartition = search.nextPartition, chosenElement = search.nextElement;
    const std::int32_t backDistance = backward & 0xffffU, forwardDistance = forward & 0xffffU;
    if (search.sourcePartition == targetPartition) {
        if (search.sourceElement > targetElement)
            return -backDistance;
        return backDistance;
    }
    if (!backward || !forward) {
        if (!backward) {
            *nextPartition = chosenPartition;
            *nextElement = chosenElement;
            return forwardDistance;
        }
    } else if (forwardDistance < backDistance) {
        *nextPartition = chosenPartition;
        *nextElement = chosenElement;
        return forwardDistance;
    }
    return -backDistance;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t mazeFindPartitionDistance(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *nextElement) {
    RouteSearch search = {sourcePartition, sourceElement, targetPartition, targetElement, -1, 0, 0, sourcePartition, sourceElement};
    ClearMazeRouteVisited();
    const std::uint32_t backward = SearchRoute(search, 0);
    *nextPartition = search.nextPartition;
    *nextElement = search.nextElement;
    ClearMazeRouteVisited();
    const std::uint32_t forward = SearchRoute(search, 1);
    const std::int32_t backDistance = static_cast<std::uint16_t>(backward), forwardDistance = static_cast<std::uint16_t>(forward);
    if (search.sourcePartition == targetPartition)
        return search.sourceElement > targetElement ? -backDistance : backDistance;
    if (!backward || (forward && forwardDistance < backDistance)) {
        *nextPartition = search.nextPartition;
        *nextElement = search.nextElement;
        return forwardDistance;
    }
    return -backDistance;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t SearchRoute(RouteSearch &search, std::int32_t endpoint) {
    if (search.partition < 0)
        return 0;
    if (mazeLinks[search.partition][0].visited)
        return 0;
    mazeLinks[search.partition][0].visited = 1;
    if (search.partition == search.targetPartition) {
        search.nextPartition = search.partition;
        search.nextElement = search.targetElement;
        return static_cast<std::uint32_t>(MazeRouteElementDistance(search.element, search.targetElement)) | 0x10000U;
    }
    const std::int32_t partition = search.partition, element = search.element;
    {
        const EndpointLinks(*links)[2] = mazeLinks;
        search.partition = links[partition][endpoint].neighbors[1].partition;
        search.element = links[partition][endpoint].neighbors[1].element;
        const std::uint32_t result = SearchRoute(search, search.element == 0);
        search.nextPartition = search.partition;
        search.nextElement = search.element;
        search.partition = partition;
        search.element = element;
        if (result) {
            std::int32_t distance;
            if (endpoint)
                distance = GetMZNRDataForID(partition) - search.element;
            else
                distance = search.element;
            return result + distance + 1;
        }
    }
    {
        const EndpointLinks(*links)[2] = mazeLinks;
        search.partition = links[partition][endpoint].neighbors[0].partition;
        search.element = links[partition][endpoint].neighbors[0].element;
        const std::uint32_t result = SearchRoute(search, search.element == 0);
        search.nextPartition = search.partition;
        search.nextElement = search.element;
        search.partition = partition;
        search.element = element;
        if (result) {
            std::int32_t distance;
            if (endpoint)
                distance = GetMZNRDataForID(partition) - search.element;
            else
                distance = search.element;
            return result + distance + 1;
        }
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t SearchRoute(RouteSearch &search, std::int32_t endpoint) {
    if (search.partition < 0 || mazeLinks[search.partition][0].visited)
        return 0;
    mazeLinks[search.partition][0].visited = 1;
    if (search.partition == search.targetPartition) {
        search.nextPartition = search.partition;
        search.nextElement = search.targetElement;
        return static_cast<std::uint32_t>(MazeRouteElementDistance(search.element, search.targetElement)) | 0x10000U;
    }
    const std::int32_t partition = search.partition, element = search.element;
    const EndpointLinks &links = mazeLinks[partition][endpoint];
    for (std::int32_t index = 1; index >= 0; --index) {
        search.partition = links.neighbors[index].partition;
        search.element = links.neighbors[index].element;
        const std::uint32_t result = SearchRoute(search, search.element == 0);
        search.nextPartition = search.partition;
        search.nextElement = search.element;
        search.partition = partition;
        search.element = element;
        if (result) {
            const std::int32_t distance = endpoint ? GetMZNRDataForID(partition) - search.element : search.element;
            return result + static_cast<std::uint32_t>(distance) + 1U;
        }
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t MazeRouteElementDistance(std::int32_t first, std::int32_t second) {
    const std::int32_t difference = second - first;
    return difference < 0 ? -difference : difference;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ClearMazeRouteVisited() {
    for (std::int32_t partition = 0; partition < 32; ++partition) {
        mazeLinks[partition][0].visited = 0;
        mazeLinks[partition][1].visited = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazeRoad *CollectMazePartition(MazeRoad *road, Vector3 *point) {
    point = CollectMazePartitionStartUnit(road, point);
    road = SeekOutsideMazeUnit(road);
    for (;;) {
        point = AppendMazeRoadsOutsideUnits(road, point);
        road = SeekInsideMazeUnit(road);
        if (road->index < 0) {
            TerminateMazeRoutePoints(point);
            return road;
        }
        const std::int32_t unitIndex = FindMazeUnit(&road->position);
        const std::int32_t kind = ClassifyMazeRoadUnit(road);
        if (kind == 1) {
            point = AppendMazeUnitPath(unitIndex, 0, GetMazeUnitTraversalDirection(road), point);
            road = SeekOutsideMazeUnit(road);
        }
        if (kind == 0) {
            point = AppendMazeUnitPath(unitIndex, 0, -1, point);
            TerminateMazeRoutePoints(point);
            road = SeekOutsideMazeUnit(road);
            return road;
        }
        if (kind > 1) {
            std::int32_t list, element;
            FindNearestMazeUnitPoint(&road->position, unitIndex, &list, &element);
            point = AppendMazeUnitPath(unitIndex, list, -1, point);
            TerminateMazeRoutePoints(point);
            MazeRoad *outside = SeekOutsideMazeUnit(road);
            if (outside->index == -1)
                return outside;
            return road;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildMazePartitions() {
    std::int32_t count = 0;
    const std::int32_t blocks = GetMazeBlockCount();
    for (std::int32_t block = 0; block < blocks; ++block) {
        MazeRoad *road = g_mazeRoadBlocks[block];
        do {
            road = CollectMazePartition(road, mazeRouteBuffer);
            StoreMazePartition(count++, mazeRouteBuffer);
        } while (road->index != -1);
    }
    g_mazePartitionCount = count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void StoreMazePartition(std::int32_t partition, const Vector3 *points) {
    const std::int32_t count = CountMazePartitionPoints(points);
    g_mazePartitions[partition].count = count;
    g_mazePartitions[partition].points = static_cast<Vector3 *>(HLib::AllocateTaskData((count + 1) * 0xb0, SHEN_NAME4('M', 'Z', 'N', 'R')));
    CopyMazePartitionPoints(g_mazePartitions[partition].points, points);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CopyMazePartitionPoints(Vector3 *output, const Vector3 *points) {
    for (; points->y > -100.0f; ++points, ++output)
        *output = *points;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 *CollectMazePartitionStartUnit(MazeRoad *road, Vector3 *output) {
    const std::int32_t unit = FindMazeUnit(&road->position);
    if (unit >= 0) {
        std::int32_t list, element;
        FindNearestMazeUnitPoint(&SeekOutsideMazeUnit(road)->position, unit, &list, &element);
        return output = AppendMazeUnitPath(unit, list, 1, output);
    }
    return output;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 *AppendMazeRoadsOutsideUnits(MazeRoad *road, Vector3 *output) {
    for (; road->index >= 0; ++road, ++output) {
        if (FindMazeUnit(&road->position) != -1)
            return output;
        CopyMazeVector3(output, &road->position);
    }
    return output;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ClassifyMazeRoadUnit(MazeRoad *road) {
    std::int32_t result = FindMazeUnit(&road->position);
    if (result >= 0) {
        result = GetMazeUnitListCount(result);
        if (result) {
            if (result > 1)
                return result;
            if (SeekOutsideMazeUnit(road))
                return 1;
            return 0;
        }
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeUnitTraversalDirection(MazeRoad *road) {
    const std::int32_t unit = FindMazeUnit(&road->position);
    if (GetMazeUnitListCount(unit) < 0)
        return 0;
    std::int32_t list, element;
    FindNearestMazeUnitPoint(&road->position, unit, &list, &element);
    return element <= MazePathCount(unit, list) / 2 ? 1 : -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 *AppendMazeUnitPath(std::int32_t unitIndex, std::int32_t list, std::int32_t direction, Vector3 *output) {
    if (GetMazeUnitListCount(unitIndex) < 0)
        return output;
    const std::int32_t count = MazePathCount(unitIndex, list);
    std::int32_t index;
    if (direction > 0) {
        direction = 1;
        index = 0;
    } else {
        direction = -1;
        index = count - 1;
    }
    for (; index < count && index >= 0; index += direction) {
        Vector3 point;
        GetMazeUnitPathPoint(unitIndex, list, index, &point);
        if (FindMazeUnit(&point) != -1) {
            CopyMazeVector3(output, &point);
            ++output;
        }
    }
    return output;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CopyMazeVector3(Vector3 *output, const Vector3 *source) {
    *output = *source;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void TerminateMazeRoutePoints(Vector3 *point) {
    point->x = 0.0f;
    point->y = -1000.0f;
    point->z = 0.0f;
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetDefaultWeatherSpawnDrift(Vector3 *direction) {
    direction->x = 0.0f;
    direction->y = -0.28f;
    direction->z = 0.0f;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazeRoad *SeekOutsideMazeUnit(MazeRoad *road) {
    for (; road->index >= 0; ++road) {
        if (FindMazeUnit(&road->position) < 0)
            return road;
    }
    return road;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazeRoad *SeekInsideMazeUnit(MazeRoad *road) {
    for (; road->index >= 0; ++road) {
        if (FindMazeUnit(&road->position) != -1)
            return road;
    }
    return road;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t CountMazePartitionPoints(const Vector3 *points) {
    std::int32_t count = 0;
    for (; points->y > -100.0f; ++points)
        ++count;
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetMazeEndpointLinks(EndpointLinks *links) {
    links->neighbors[0].partition = -1;
    links->neighbors[1].partition = -1;
    links->neighbors[0].element = -1;
    links->neighbors[1].element = -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void BuildMazeEndpointLinks(EndpointNeighbor *links, std::int32_t partition, std::int32_t element) {
    Vector3 position, points[3];
    std::int32_t adjacentPartitions[3], adjacentElements[3];
    std::int32_t unitIndex, own = -1;
    mazeSamplePartition(partition, element, &position);
    unitIndex = FindMazeUnit(&position);
    if (unitIndex == -1)
        return;
    if (GetMazeUnitListCount(unitIndex) != 3)
        return;
    for (std::int32_t index = 0; index < 3; ++index) {
        GetMazeUnitPathPoint(unitIndex, index, 0, &points[index]);
        adjacentPartitions[index] = -1;
        mazeFindClosestTarget(&adjacentPartitions[index], &adjacentElements[index], &points[index]);
        if (partition == adjacentPartitions[index] && element == adjacentElements[index])
            own = index;
    }
    switch (own) {
    case 0:
        links[0].partition = adjacentPartitions[1];
        links[0].element = adjacentElements[1];
        links[1].partition = adjacentPartitions[2];
        links[1].element = adjacentElements[2];
        break;
    case 1:
        links[0].partition = adjacentPartitions[2];
        links[0].element = adjacentElements[2];
        links[1].partition = adjacentPartitions[0];
        links[1].element = adjacentElements[0];
        break;
    case 2:
        links[0].partition = adjacentPartitions[0];
        links[0].element = adjacentElements[0];
        links[1].partition = adjacentPartitions[1];
        links[1].element = adjacentElements[1];
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void BuildMazeEndpointLinks(std::int32_t partition, std::int32_t endpoint) {
    const std::int32_t element = endpoint ? g_mazePartitions[partition].count - 1 : 0;
    Vector3 position;
    mazeSamplePartition(partition, element, &position);
    const std::int32_t unitIndex = FindMazeUnit(&position);
    if (unitIndex == -1 || GetMazeUnitListCount(unitIndex) != 3)
        return;
    std::int32_t adjacentPartitions[3], adjacentElements[3], own = -1;
    for (std::int32_t index = 0; index < 3; ++index) {
        Vector3 point;
        GetMazeUnitPathPoint(unitIndex, index, 0, &point);
        adjacentPartitions[index] = -1;
        mazeFindClosestTarget(adjacentPartitions + index, adjacentElements + index, &point);
        if (adjacentPartitions[index] == partition && adjacentElements[index] == element)
            own = index;
    }
    if (own < 0)
        return;
    EndpointLinks *links = &mazeLinks[partition][endpoint];
    for (std::int32_t index = 0; index < 2; ++index) {
        const std::int32_t next = (own + index + 1) % 3;
        links->neighbors[index].partition = adjacentPartitions[next];
        links->neighbors[index].element = adjacentElements[next];
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitializeMazeRouteLinks() {
    std::int32_t index;
    for (index = 0; index < 32; ++index) {
        ResetMazeEndpointLinks(&mazeLinks[index][0]);
        ResetMazeEndpointLinks(&mazeLinks[index][1]);
    }
    for (index = 0; index < mazeGetNumPartitions(); ++index) {
        BuildMazeEndpointLinks(mazeLinks[index][0].neighbors, index, 0);
        BuildMazeEndpointLinks(mazeLinks[index][1].neighbors, index, GetMZNRDataForID(index) - 1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void InitializeMazeRouteLinks() {
    for (std::int32_t index = 0; index < 32; ++index) {
        ResetMazeEndpointLinks(&mazeLinks[index][0]);
        ResetMazeEndpointLinks(&mazeLinks[index][1]);
    }
    for (std::int32_t index = 0; index < g_mazePartitionCount; ++index) {
        BuildMazeEndpointLinks(index, 0);
        BuildMazeEndpointLinks(index, 1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FindCharacterNearestMazeUnitPoint(Character *character, std::int32_t unit, std::int32_t *list, std::int32_t *element) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    FindNearestMazeUnitPoint(&position, unit, list, element);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FindNearestMazeUnitPoint(const Vector3 *point, std::int32_t unit, std::int32_t *list, std::int32_t *element) {
    *list = -1;
    *element = -1;
    float nearest = 1000000000.0f;
    const std::int32_t lists = GetMazeUnitListCount(unit);
    if (lists < 0)
        return;
    for (std::int32_t index = 0; index < lists; ++index) {
        const std::int32_t count = MazePathCount(unit, index);
        for (std::int32_t i = 0; i < count; ++i) {
            Vector3 sample;
            GetMazeUnitPathPoint(unit, index, i, &sample);
            const float x = point->x - sample.x, y = point->y - sample.y, z = point->z - sample.z;
            const float distance = x * x + y * y + z * z;
            if (nearest > distance) {
                nearest = distance;
                *list = index;
                *element = i;
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeUnitListCount(std::int32_t unit) {
    const MazeUnit *entry = GetMazeUnit(unit);
    if (entry->numLists)
        return entry->numLists;
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t MazePathCount(std::int32_t unit, std::int32_t list) {
    const MazeUnit *entry = GetMazeUnit(unit);
    std::int32_t count = 0;
    for (const Vector3 *points = entry->pointLists[list]; 990.0f > points->y; ++points)
        ++count;
    if (!count)
        return -1;
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetMazeUnitPathPoint(std::int32_t unit, std::int32_t list, std::int32_t element, Vector3 *output) {
    const Vector3 *points = GetMazeUnit(unit)->pointLists[list];
    output->x = points[element].x;
    output->y = points[element].y;
    output->z = points[element].z;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool LoadMazeModel(const LoadedMapPackage &package, const char *name, MazeModelResource &resource);
bool InitializeMazeAuthoredObjects(const LoadedMapPackage &package);
shen_name MazeUnitAuthoredName(std::int32_t modelIndex);
std::int32_t AuthoredWord(const RoadBlock &block, std::int32_t record, std::size_t offset);
void ScanMazeAuthoredObjects(shen_name name, const MazeRoad &context);
std::size_t MazeRoadModelIndex(const MazeRoad &road);
std::span<const std::byte> FindTerrainEntry(const LoadedMapPackage &package, const char *name);
void *Allocate(std::size_t bytes, shen_name name);
std::array<MazeModelResource, 35> mediumModels;
std::array<MazeModelResource, 9> treeModels;
std::array<MazeModelResource, 12> auxiliaryModels;
std::array<std::span<const MazePropRecord>, 6> propTables{};
std::array<std::array<MazePropState, 32>, 120> propStates;
std::array<std::uint8_t, 7000> mediumOccupancy{};
Matrix4x4 mazeFlutterTransform;
Matrix4x4 mazeEffectTransform;
bool mazeClearWeather;
bool mazeHeavyWeather{};
constexpr std::int32_t mazeWiggle[] = {0, 16, 32, 40, 48, 52, 56, 58, 60, 58, 56, 52, 48, 40, 32, 16};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool MazeRecordMatchesIndex(std::uint32_t flags, std::int32_t index) {
    return (!(flags & 0x400U) || !(index & 1)) && (!(flags & 0x800U) || index == 3 * (index / 3)) && (!(flags & 0x1000U) || index % 4 == 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool ShouldGenerateMazeProp(const MazePropRecord &record, const MazeRoad &road) {
    if ((record.flags & 0x2000U) != 0)
        return false;
    const float chance = IncrementSharedPrngFloat();
    return record.chance > chance && MazeRecordMatchesIndex(record.flags, road.index);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
const void *FindFreeEntryInMazeObjectPoolOfKind(std::int32_t kind) {
    for (std::size_t index = 0; index < 200; ++index) {
        std::uint8_t &occupied = mediumOccupancy[index * 35 + kind];
        if (!occupied) {
            occupied = 1;
            return &occupied;
        }
    }
    char diagnostic[256];
    std::snprintf(diagnostic, sizeof(diagnostic), "Couldn't find free entry in maze object pool of kind: %i", kind);
    return nullptr;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
MazePropState *MakeMazePropState(const MazePropRecord *record, MazePropState *output) {
    if (!record)
        return output;
    float x = IncrementSharedPrngFloat() * (record->maxX - record->minX) + record->minX;
    if (IncrementSharedPrngFloat() > 0.5f)
        x = -x;
    float z = IncrementSharedPrngFloat() * 2.0f - 1.0f;
    if (((record->flags & 1U) && x > 0.0f) || ((record->flags & 2U) && x < 0.0f))
        x = -x;
    if (record->flags & 0x40U)
        z = 0.0f;
    std::int32_t yaw = 2 * IncrementSharedPrngUInt16();
    if (record->flags & 8U)
        yaw = 0;
    if (record->flags & 0x10U)
        yaw = 0x8000;
    if (record->flags & 0x20U)
        yaw = yaw > 0x8000 ? 0x8000 : 0;
    output->kind = record->kind;
    output->flags = record->flags;
    output->drawState = record->drawState;
    output->yaw = yaw;
    output->x = x;
    output->z = z;
    output->height = record->height;
    output->placement = FindFreeEntryInMazeObjectPoolOfKind(record->kind);
    ++output;
    if (record->flags & 4U) {
        output->kind = record->kind;
        output->flags = record->flags;
        output->drawState = record->drawState;
        output->yaw = yaw;
        output->x = -x;
        output->z = z;
        output->height = record->height;
        output->placement = FindFreeEntryInMazeObjectPoolOfKind(record->kind);
        ++output;
    }
    output->kind = -1;
    return output;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void GenerateMazePropStates(const MazeRoad &road, std::size_t drawIndex) {
    SetSharedPrngSeed(static_cast<std::uint32_t>(road.index) + (static_cast<std::uint32_t>(road.block) << 11));
    const auto table = propTables[MazeRoadModelIndex(road) / 4];
    MazePropState *output = propStates[drawIndex].data();
    for (std::size_t index = 0; index < table.size() && table[index].flags != 0xffffffffU; ++index) {
        const MazePropRecord &record = table[index];
        if (record.flags & 0x4000U) {
            ++index;
            if (MazeRecordMatchesIndex(table[index].flags, road.index)) {
                std::size_t count = 0;
                if (!(table[index].flags & 0xa000U)) {
                    do {
                        ++count;
                    } while (!(table[index + count].flags & 0x8000U));
                }
                const MazePropRecord *selected =
                    count ? &table[index + static_cast<std::size_t>(IncrementSharedPrngFloat() * static_cast<float>(count))] : nullptr;
                output = MakeMazePropState(selected, output);
                while (!(table[index].flags & 0x8000U))
                    ++index;
            }
        } else if (ShouldGenerateMazeProp(record, road))
            output = MakeMazePropState(&record, output);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazePropStates(const MazeRoad &road, std::size_t drawIndex) {
    for (const MazePropState &prop : propStates[drawIndex]) {
        if (prop.kind < 0)
            break;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        Matrix4x4::preTranslate(prop.x, prop.height, prop.z);
        if (prop.yaw)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(prop.yaw));
        d3t::HashModel(prop.placement, GetLegacyPerspectiveProjection());
        const bool hashed = road.alpha == 1.0f ? d3t::HashModel(prop.placement, GetLegacyPerspectiveProjection()) :
                                                 d3t::HashModelWithAlpha(prop.placement, road.alpha, GetLegacyPerspectiveProjection());
        if (!hashed) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(prop.x, prop.height, prop.z);
            MT7Model &model = *mediumModels[prop.kind].model;
            model.nodes[0].rotation.y = static_cast<float>(prop.yaw);
            if (prop.drawState) {
                if (road.alpha == 1.0f)
                    charaApplyScale(&model, model.nodes.data());
                else {
                    SetLegacyBusyPreparingDraw(true);
                    charaPrepareNodesForDraw(&model, model.nodes.data(), road.alpha);
                    SetLegacyBusyPreparingDraw(false);
                }
            }
        }
        Matrix4x4::Pop(1);
        ++g_mazePropDrawCount;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct MazeAuxiliaryDefinition {
    shen_name area;
    const char *model;
    Vector3 position;
    std::int32_t yaw;
};
constexpr MazeAuxiliaryDefinition mazeAuxiliaryDefinitions[] = {
    {.area = SHEN_NAME(KMZA), .model = "MAZHOB1G", .position = {0.63f, 0.82f, -5.58f}, .yaw = 13747},
        {.area = SHEN_NAME(KMZA), .model = "MAZHOB2G", .position = {-6.5f, 0.83f, 3.2f}, .yaw = 57317},
            {.area = SHEN_NAME(KMZA), .model = "MAZHOB3G", .position = {-6.96f, 1.14f, 3.38f}, .yaw = 2545},
                {.area = SHEN_NAME(KMZA), .model = "MAZHOP3G", .position = {-0.2f, 0.0f, -6.0f}, .yaw = 55252},
                    {.area = SHEN_NAME(KMZA), .model = "MAZHOP1G", .position = {1.44f, 0.15f, -5.61f}, .yaw = 64535},
                        {.area = SHEN_NAME(KMZA), .model = "MAZHOP2G", .position = {-5.59f, 0.7f, 1.88f}, .yaw = 32260},
                            {.area = SHEN_NAME(KMZ1), .model = "KANSOT2G", .position = {0.0f, 0.0f, -3.5f}},
                                {.area = SHEN_NAME(KMZA), .model = "KANSRB1G"},
                                    {.area = SHEN_NAME(KMZ4), .model = "KANIRB1G"},
                                        {.area = SHEN_NAME(KMZ3), .model = "ZIZHIY4G", .position = {6.48f, 1.72f, 14.08f}},
                                            {.area = SHEN_NAME(KMZ4), .model = "KOMHIY4G", .position = {6.48f, 1.18f, 14.08f}},
                                                {.area = SHEN_NAME(KMZA), .model = "IWAR001G"},
};
struct MazeRoadDecoration {
    shen_name area;
    std::int32_t marker;
    std::int32_t block;
    std::int32_t index;
    Vector3 position;
    std::int32_t yaw;
    std::int32_t kind;
};
MazeRoadDecoration mazeMarkerDecorations[] = {
    {.area = SHEN_NAME(KMZ2), .position = {-2.1f, 0.0f, 0.0f}, .kind = 7},
        {.area = SHEN_NAME(KMZ3), .position = {-2.1f, 0.0f, 0.0f}, .kind = 7},
            {.area = SHEN_NAME(KMZ4), .position = {2.1f, 0.0f, 0.0f}, .yaw = 0x8000, .kind = 8},
};
constexpr MazeRoadDecoration mazeFixedDecorations[] = {
    {.area = SHEN_NAME(KMZ4), .block = 1, .index = 88, .position = {-9.0f, 0.0f, 0.0f}, .kind = 25},
        {.area = SHEN_NAME(KMZ4), .block = 1, .index = 122, .position = {9.0f, 0.0f, 0.0f}, .yaw = 0x8000, .kind = 25},
            {.area = SHEN_NAME(KMZ4), .block = 4, .index = 3, .position = {-9.0f, 0.0f, 0.0f}, .kind = 25},
};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeEnvironmentModels(const LoadedMapPackage &package) {
    constexpr const char *tableNames[] = {"tbl_mori.BIN", "tbl_iwa.BIN", nullptr, nullptr, nullptr, "tbl_lwod.BIN"};
    std::array<bool, 35> used{};
    std::array<bool, 6> visited{};
    for (const RoadBlock &block : roadBlocks) {
        for (std::int32_t index = 0; index < block.controlCount; ++index) {
            MazeRoad authored{};
            authored.flags = static_cast<std::uint32_t>(AuthoredWord(block, index, 28)) << 16;
            const std::size_t kind = MazeRoadModelIndex(authored) / 4;
            if (visited[kind])
                continue;
            visited[kind] = true;
            if (!tableNames[kind])
                return false;
            const auto bytes = FindTerrainEntry(package, tableNames[kind]);
            if (bytes.empty())
                return false;
            propTables[kind] = {reinterpret_cast<const MazePropRecord *>(bytes.data()), bytes.size() / sizeof(MazePropRecord)};
            for (const MazePropRecord &record : propTables[kind]) {
                if (record.flags == 0xffffffffU)
                    break;
                if (!(record.flags & 0xc000U))
                    used[record.kind] = true;
            }
        }
    }
    constexpr const char *mediumNames[] = {"SRB1", "SRB2", "SRB3", "IRB1", "IRB2", "IRB3", "KRB1", "KRB2", "KRB3", "IOB0", "IOB1", "IOB2", "IRM1", "IRM2",
        "IRM3", "IRM4", "IRM5", "HOM0", "H3F1", "H3F2", "H3F3", "H3F4", "H3F5", "H3F6", "H3F7", "H3F8", "H3F9", "HOM3", "HIJ1", "HIJ2", "HIJ3", "HOJ0", "HOJ1",
        "H3J1", "H3J2"};
    for (std::size_t index = 0; index < used.size(); ++index) {
        if (!used[index])
            continue;
        char name[16];
        std::snprintf(name, sizeof(name), "MAZ%sG", mediumNames[index]);
        if (!LoadMazeModel(package, name, mediumModels[index]))
            return false;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeAuxiliaryModels(const LoadedMapPackage &package) {
    for (std::size_t index = 0; index < std::size(mazeAuxiliaryDefinitions); ++index) {
        const MazeAuxiliaryDefinition &definition = mazeAuxiliaryDefinitions[index];
        if (definition.area.name == g_currentMap.name || definition.area.name == SHEN_NAME(KMZA))
            if (!LoadMazeModel(package, definition.model, auxiliaryModels[index]))
                return false;
    }
    if (!InitializeMazeAuthoredObjects(package))
        return false;
    constexpr const char *treeNames[] = {"EFFHIS1G", "EFFHIM1G", "EFFHIL1G", "EFFPISSG", "EFFPIS1G", "EFFPIM1G", "EFFPIL1G", "MON02L4P", "MONY2L4P"};
    for (std::size_t index = 0; index < std::size(treeNames); ++index)
        if (!LoadMazeModel(package, treeNames[index], treeModels[index]))
            return false;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void BindMazeMarkerDecorations() {
    for (MazeRoadDecoration &decoration : mazeMarkerDecorations) {
        if (decoration.area.name != g_currentMap.name)
            continue;
        Vector3 position{};
        mazeGetMarkerPosition(decoration.marker, &position);
        float closest = 1000000.0f;
        for (const RoadBlock &block : roadBlocks)
            for (std::int32_t index = 0; index < block.roadCount; ++index) {
                const MazeRoad &road = block.roads[index];
                const float x = position.x - road.position.x;
                const float z = position.z - road.position.z;
                if (closest > z * z + x * x) {
                    closest = z * z + x * x;
                    decoration.block = road.block;
                    decoration.index = road.index;
                }
            }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeAuxiliaryUnit(const MazeRoad &road, shen_name name) {
    SetSharedPrngSeed(static_cast<std::uint32_t>(road.unit) + (static_cast<std::uint32_t>(road.block) << 8));
    std::int32_t variation = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 3.0f);
    if (g_currentMap.name == SHEN_NAME(KMZ1) && road.unit == 1)
        variation = 3;
    if (g_currentMap.name == SHEN_NAME(KMZ3) && name.name == SHEN_NAME(HIY4))
        variation = 4;
    if (g_currentMap.name == SHEN_NAME(KMZ4) && name.name == SHEN_NAME(HIY4))
        variation = 5;
    bool selected[2]{true, false};
    if (variation < 3) {
        selected[0] = IncrementSharedPrngFloat() < 0.65f;
        selected[1] = IncrementSharedPrngFloat() < 0.65f;
    }
    constexpr std::int32_t modelIndices[][2] = {{0, 2}, {1, 3}, {4, 5}, {6, 0}, {9, 0}, {10, 0}};
    for (std::size_t index = 0; index < 2; ++index) {
        if (!selected[index])
            continue;
        const std::size_t modelIndex = modelIndices[variation][index];
        const MazeAuxiliaryDefinition &definition = mazeAuxiliaryDefinitions[modelIndex];
        MT7Model &model = *auxiliaryModels[modelIndex].model;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        Matrix4x4::preTranslate(definition.position.x, definition.position.y, definition.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(definition.yaw));
        if (!d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection())) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(definition.position.x, definition.position.y, definition.position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(definition.yaw));
            applyPosRotScl(&model, model.nodes.data());
        }
        Matrix4x4::Pop(1);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ApplyMazeRoadDecoration(const MazeRoad &road, const MazeRoadDecoration &decoration, MT7Model &model, bool clearYaw) {
    if (decoration.area.name != g_currentMap.name || decoration.block != road.block || decoration.index != road.index || model.nodes.empty())
        return;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
    Matrix4x4::preTranslate(decoration.position.x, decoration.position.y, decoration.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(decoration.yaw));
    if (!d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection())) {
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(decoration.position.x, decoration.position.y, decoration.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(decoration.yaw));
        if (clearYaw)
            model.nodes[0].rotation.y = 0.0f;
        charaApplyScale(&model, model.nodes.data());
    }
    Matrix4x4::Pop(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SwizzledBasis3x4IntoActiveFrom(const Matrix4x4 &source) {
    for (std::size_t row = 0; row < 3; ++row) {
        workTransformMatrix.rows[row].x = source.rows[row].x;
        workTransformMatrix.rows[row].y = source.rows[row].y;
        workTransformMatrix.rows[row].z = source.rows[row].z;
        workTransformMatrix.rows[row].w = 0.0f;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeEffectMesh(MT7Model &model, std::int32_t node, const float *alpha = nullptr) {
    const MT7Node &mesh = model.nodes[node];
    if (!alpha)
        ModelFlagDispatch_SetModelFlagDataOpaque(model.source, mesh.xb01Offset, mesh.borrowedMesh ? mesh.borrowedMesh->drawResources : mesh.drawResources);
    else
        SetModelFlagDataWithAlpha(model.source, mesh.xb01Offset, mesh.borrowedMesh ? mesh.borrowedMesh->drawResources : mesh.drawResources, *alpha);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitializeMazeFrameEnvironment() {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::GetTransformMatrix(&mazeFlutterTransform);
    Matrix4x4::RotateAroundX_Wrapper(4608);
    Matrix4x4::RotateAroundZ_Wrapper(4608);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>((mazeCurrentReferenceCount & 0x1ff) << 7));
    Matrix4x4::GetTransformMatrix(&mazeEffectTransform);
    Matrix4x4::Pop(1);
    mazeClearWeather = false;
    mazeHeavyWeather = false;
    DateTime date{};
    shenGetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 7) <= 9) {
        mazeHeavyWeather = GetWeatherPressure() > 0.69999999f;
        if (GetWeatherPressure() < 0.30000001f)
            mazeClearWeather = GetWeatherWindVector() < 0.5f;
    }
    camCtrlGetNextCamPos(&mazeCameraPosition);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void FreeNecessaryMazeObjects() {
    constexpr std::int32_t children[][2] = {
        {27, 0}, {27, 1}, {27, 2}, {27, 3}, {18, 4}, {19, 1}, {19, 3}, {20, 2}, {20, 3}, {22, 4}, {24, 3}, {26, 2}, {26, 3}};
    const std::uint32_t phase = mazeCurrentReferenceCount >> 1;
    std::uint8_t offset = 0;
    for (const auto &child : children) {
        if (!mediumModels[child[0]].model) {
            offset += 4;
            continue;
        }
        MT7Model &model = *mediumModels[child[0]].model;
        if (!model.nodes.empty()) {
            std::int32_t node = model.nodes[0].firstChild;
            for (std::int32_t index = 0; index < child[1]; ++index)
                node = model.nodes[node].nextSibling;
            const std::uint8_t angle = static_cast<std::uint8_t>(phase + offset + child[0]);
            model.nodes[node].rotation = {0, ((angle & 0x10) ? 1 : -1) * mazeWiggle[angle & 15], 0};
        }
        offset += 4;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ApplyMazeUnitChildWiggle() {
    for (std::int32_t modelIndex : {23, 32}) {
        if (!unitModels[modelIndex].model)
            continue;
        MT7Model &model = *unitModels[modelIndex].model;
        if (model.nodes.empty())
            continue;
        std::int32_t node = model.nodes[0].firstChild;
        node = model.nodes[node].firstChild;
        std::uint8_t phase = static_cast<std::uint8_t>(mazeCurrentReferenceCount >> 1);
        while (node >= 0) {
            const std::int32_t angle = ((phase & 0x10) ? 3 : -3) * mazeWiggle[phase & 15];
            model.nodes[node].rotation = {angle, angle, 0};
            phase += 2;
            node = model.nodes[node].nextSibling;
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeFlutterEffects(const MazeRoad &road) {
    const std::size_t kind = MazeRoadModelIndex(road) / 4;
    if ((road.flags & 0x10000000U) || road.cameraDistance > 256.0f || !mazeClearWeather || (kind != 0 && kind != 5))
        return;
    if (kind == 0 && MazeRecordMatchesIndex(0x1000, road.index))
        return;
    const float centerX = kind == 5 ? 0.60000002f : 4.099999904632568f;
    const float centerY = kind == 5 ? 11.6f : 3.700000047683716f;
    SetSharedPrngSeed(2U * (static_cast<std::uint32_t>(road.index) + 300U * static_cast<std::uint32_t>(road.block)));
    const std::int32_t count = 3 - static_cast<std::int32_t>(IncrementSharedPrngFloat() * -3.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        const std::uint16_t random = IncrementSharedPrngUInt16();
        if (((static_cast<std::uint8_t>(random) * static_cast<std::uint8_t>(mazeCurrentReferenceCount >> 3)) & 0x70) == 0)
            continue;
        MT7Model &model = *treeModels[(random & 3) + 3].model;
        float x = IncrementSharedPrngFloat() * 0.40000001f - 0.2f + centerX;
        const float y = IncrementSharedPrngFloat() * 1.6f - 0.80000001f + centerY;
        const float z = IncrementSharedPrngFloat() * 2.0f - 1.0f;
        if (IncrementSharedPrngFloat() > 0.5f)
            x = -x;
        const std::int32_t distance = static_cast<std::int32_t>(static_cast<double>(z * z + x * x + road.cameraDistance) * 100.0);
        if (!(distance & 0xe))
            continue;
        const std::int32_t shift = distance & 0xffe000 ? distance >> 12 : distance >> 10;
        if (!((random >> (shift & 7)) & 1))
            continue;
        float alpha = distance & 4 ? 0.70000005f : 0.40000001f;
        if (distance & 2)
            alpha += 0.30000001f;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        Matrix4x4::preTranslate(x, y, z);
        if (!d3t::HashModelWithAlpha(GetMT7ReaderData(&model), alpha, GetLegacyPerspectiveProjection())) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(x, y, z);
            SwizzledBasis3x4IntoActiveFrom(mazeFlutterTransform);
            DrawMazeEffectMesh(model, 0, &alpha);
        }
        Matrix4x4::Pop(1);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeTreeEffects(const MazeRoad &road) {
    const std::size_t kind = MazeRoadModelIndex(road) / 4;
    if ((road.flags & 0x10000000U) || (kind != 0 && kind != 5))
        return;
    DrawMazeFlutterEffects(road);
    SetSharedPrngSeed(static_cast<std::uint32_t>(road.index) + (static_cast<std::uint32_t>(road.block) << 9));
    if (IncrementSharedPrngFloat() > 0.5f)
        return;
    const std::int32_t count = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 1.1f + 1.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        const float choice = IncrementSharedPrngFloat();
        const std::size_t modelIndex = choice < 0.44999999f ? 0 : choice < 0.80000001f ? 1 : 2;
        float x = IncrementSharedPrngFloat() - 0.5f + (kind == 5 ? 2.0f : 5.0f);
        const float y = IncrementSharedPrngFloat() * 2.0f - 1.0f + (kind == 5 ? 8.0f : 5.0f);
        const float z = IncrementSharedPrngFloat() - 0.5f;
        if (IncrementSharedPrngFloat() >= 0.5f)
            x = -x;
        Matrix4x4_Push();
        Matrix4x4::preTranslate(x, y, z);
        SwizzledBasis3x4IntoActiveFrom(mazeViewTransform);
        if (!GetLegacyRenderMask()) {
            MT7Model &model = *treeModels[modelIndex].model;
            const std::int32_t child = model.nodes[0].firstChild;
            DrawMazeEffectMesh(model, model.nodes[child].nextSibling);
            if (mazeClearWeather) {
                Matrix4x4::MultiplyWithPrePassMatrix(&mazeEffectTransform);
                const float alpha = 0.40000001f;
                DrawMazeEffectMesh(model, child, &alpha);
            }
        }
        Matrix4x4::Pop(1);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeButterflyEffects(const MazeRoad &road) {
    if (static_cast<double>(IncrementSharedPrngFloat()) > 0.05)
        return;
    const std::int32_t count = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 1.9f + 1.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        const std::uint8_t random = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
        const std::uint8_t phase = (random + mazeCurrentReferenceCount) & 15;
        const Vector3 &point = mazeButterflyPath[phase & 7];
        float x = IncrementSharedPrngFloat() * 0.80000001f - 0.40000001f + 3.0f + point.x;
        const float y = ((phase & 8) ? 1.0f : -1.0f) * point.y + (IncrementSharedPrngFloat() * 0.80000001f - 0.40000001f + 1.5f);
        const float z = IncrementSharedPrngFloat() * 0.80000001f - 0.40000001f + point.z;
        if (road.index & 1)
            x = -x;
        Matrix4x4_Push();
        Matrix4x4::preTranslate(x, y, z);
        MT7Model &model = *treeModels[(random & 1) + 7].model;
        std::int32_t node = model.nodes[0].firstChild;
        node = model.nodes[node].firstChild;
        for (std::uint32_t frame = 0; frame < (mazeCurrentReferenceCount & 3); ++frame)
            node = model.nodes[node].nextSibling;
        DrawMazeEffectMesh(model, node);
        Matrix4x4::Pop(1);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeRoadButterflies(const MazeRoad &road) {
    if (MazeRoadModelIndex(road) / 4 != 0)
        return;
    SetSharedPrngSeed(3U * static_cast<std::uint32_t>(road.index) + (static_cast<std::uint32_t>(road.block) << 9));
    if (!mazeHeavyWeather)
        DrawMazeButterflyEffects(road);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool CheckLineSegmentIntersection(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d) {
    const float crossC = (c.x - b.x) * (b.z - a.z) - (c.z - b.z) * (b.x - a.x);
    const float crossD = (d.x - b.x) * (b.z - a.z) - (d.z - b.z) * (b.x - a.x);
    if (crossC * crossD > -0.0f)
        return false;
    const float crossA = (a.x - d.x) * (d.z - c.z) - (a.z - d.z) * (d.x - c.x);
    const float crossB = (b.x - d.x) * (d.z - c.z) - (b.z - d.z) * (d.x - c.x);
    return crossA * crossB <= -0.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
template <std::size_t N> void InsertMazeObject(std::array<MazeRoad *, N> &list, std::size_t capacity, MazeRoad *road) {
    std::size_t begin = 0;
    std::size_t end = capacity;
    while (begin < end) {
        const std::size_t middle = (begin + end) / 2;
        if (!list[middle] || list[middle]->cameraDistance >= road->cameraDistance)
            end = middle;
        else
            begin = middle + 1;
    }
    if (begin == capacity)
        return;
    for (std::size_t index = capacity - 1; index > begin; --index)
        list[index] = list[index - 1];
    list[begin] = road;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t UpdateXFormMazeObject(MazeRoad *roads, const Vector3 &camera, float &output) {
    float nearest = 25600.0f;
    std::int32_t nearestIndex = -1;
    const float far = camCtrlGetFarDistance();
    const float farSquared = far * far;
    for (MazeRoad *road = roads; road->index >= 0; ++road) {
        const float x = camera.x - road->position.x;
        const float z = camera.z - road->position.z;
        road->cameraDistance = z * z + x * x;
        if (static_cast<std::uint16_t>(road->flags) == 0) {
            if (farSquared > road->cameraDistance) {
                InsertMazeObject(currentMazeObjects, 119, road);
                // spanY is 1.0.
                if (1.0f > road->alpha)
                    road->alphaDirection = 1;
            } else if (road->alpha > 0.0f) {
                road->alphaDirection = 2;
                InsertMazeObject(currentMazeObjects, 119, road);
            }
        } else if (static_cast<std::int32_t>(road->flags) < 0) {
            if (mazeUnitDistance * mazeUnitDistance > road->cameraDistance) {
                InsertMazeObject(currentMazeUnitObjects, 8, road);
                if (1.0f > road->alpha)
                    road->alphaDirection = 1;
            } else if (road->alpha > 0.0f) {
                road->alphaDirection = 2;
                InsertMazeObject(currentMazeUnitObjects, 8, road);
            }
        }
        if (nearest > road->cameraDistance) {
            nearest = road->cameraDistance;
            nearestIndex = road->index;
        }
    }
    output = nearest;
    return nearestIndex;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool ApplyTransformationForMazeObject(const MazeRoad &road, bool unit) {
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    const std::uint16_t yaw = camCtrlGetNextRotY();
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::preTranslate(0.0f, 0.0f, 3.0f);
    Matrix4x4 matrix{};
    Matrix4x4::GetTransformMatrix(&matrix);
    const Vector3 delta{road.position.x - matrix.rows[3].x, road.position.y - matrix.rows[3].y, road.position.z - matrix.rows[3].z};
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::preTranslate(delta.x, delta.y, delta.z);
    Matrix4x4::GetTransformMatrix(&matrix);
    Matrix4x4::Pop(1);
    const Vector3 point{matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
    const float distance = point.z * point.z + point.x * point.x;
    const std::int32_t angle = FixedAtan2(-point.x, -point.z);
    if (unit)
        return !(distance >= 144.0f && point.z >= 26.0f && static_cast<std::uint32_t>(angle - 16385) <= 0x7ffeU);
    if (distance < 144.0f)
        return true;
    if (point.z > 0.0f)
        return false;
    const std::int32_t cullingAngle = mazeCullingSteps << 10;
    return !(angle > cullingAngle && angle < 0x10000 - cullingAngle);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool CheckLineSegmentIntersection_Wrapper(const MazeRoad &road) {
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const MazeUnit &unit = MUZT_Data[road.unit];
    if (unit.pointCount <= 0)
        return false;
    std::int32_t blocked = 0;
    for (std::int32_t point = 0; point < unit.pointCount; ++point) {
        for (const MazeRoad *current : currentMazeObjects) {
            if (!current)
                break;
            if (CheckLineSegmentIntersection(camera, unit.points[point], current->rightStart, current->rightEnd) ||
                CheckLineSegmentIntersection(camera, unit.points[point], current->leftStart, current->leftEnd)) {
                ++blocked;
                break;
            }
        }
    }
    return blocked < unit.pointCount;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool GetDistanceClosest_ForMaze(const MazeRoad &road, const Vector3 &camera, const Vector3 &target) {
    for (const MazeRoad *current : currentMazeUnitObjects) {
        if (!current)
            break;
        if (!(road.cameraDistance > current->cameraDistance))
            continue;
        const MazeUnit &unit = MUZT_Data[current->unit];
        for (std::int32_t wall = 0; wall < unit.wallCount; ++wall) {
            const Vector3 &first = unit.walls[wall][0];
            const Vector3 &second = unit.walls[wall][1];
            const float crossFirst = (first.x - target.x) * (target.z - camera.z) - (first.z - target.z) * (target.x - camera.x);
            const float crossSecond = (second.x - target.x) * (target.z - camera.z) - (second.z - target.z) * (target.x - camera.x);
            if (!(crossFirst * crossSecond <= -0.0f))
                continue;
            const float crossTarget = (target.x - second.x) * (second.z - first.z) - (target.z - second.z) * (second.x - first.x);
            const float crossCamera = (camera.x - second.x) * (second.z - first.z) - (camera.z - second.z) * (second.x - first.x);
            if (crossTarget * crossCamera <= -0.0f) {
                ++g_mazeUnitWallHitCount;
                return true;
            }
        }
    }
    return false;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ApplyTransformationForCurrentMazeObjects() {
    std::array<MazeRoad *, 120> candidates{};
    std::size_t count = 0;
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    for (MazeRoad *road : currentMazeObjects) {
        if (!road)
            break;
        if (ApplyTransformationForMazeObject(*road, false))
            candidates[count++] = road;
    }
    if (count <= 8)
        currentMazeObjects = candidates;
    else {
        Vector3 camera{};
        camCtrlGetNextCamPos(&camera);
        for (std::size_t index = 0; index < 8; ++index)
            currentMazeObjects[index] = candidates[index];
        std::size_t admitted = 8;
        for (std::size_t index = 8; index < count; ++index) {
            MazeRoad &road = *candidates[index];
            auto blocked = [&](const Vector3 &target, bool includeSelf) {
                std::size_t previous = index + (includeSelf ? 1 : 0);
                while (previous) {
                    const MazeRoad &obstacle = *candidates[--previous];
                    if (CheckLineSegmentIntersection(camera, target, obstacle.leftStart, obstacle.leftEnd) ||
                        CheckLineSegmentIntersection(camera, target, obstacle.rightStart, obstacle.rightEnd))
                        return true;
                }
                return false;
            };
            std::int32_t occluded = blocked(road.leftCentre, false) || GetDistanceClosest_ForMaze(road, camera, road.leftCentre);
            if (occluded) {
                occluded += blocked(road.rightCentre, false);
                if (occluded < 1)
                    occluded += GetDistanceClosest_ForMaze(road, camera, road.rightCentre);
                if (occluded >= 1) {
                    occluded += blocked(road.position, true);
                    if (occluded < 2)
                        occluded += GetDistanceClosest_ForMaze(road, camera, road.position);
                }
            }
            if (occluded >= 2) {
                road.alphaDirection = road.alpha > 0.0f ? 2 : 0;
                if (road.alpha <= 0.0f)
                    continue;
            } else
                road.alphaDirection = road.alpha < 1.0f ? 1 : 0;
            currentMazeObjects[admitted++] = &road;
        }
        currentMazeObjects[admitted] = nullptr;
    }
    Matrix4x4::Pop(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void AdvanceMazeObjectAlpha(MazeRoad &road) {
    if (road.alphaDirection == 1)
        road.alpha += 0.1f;
    if (road.alphaDirection == 2)
        road.alpha -= 0.1f;
    if (road.alpha > 1.0f)
        road.alpha = 1.0f;
    if (road.alpha < 0.0f)
        road.alpha = 0.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint32_t MazeMeshWord(const MT7Model &model, std::uint32_t offset) {
    std::uint32_t word;
    std::memcpy(&word, model.source.data() + offset, sizeof(word));
    return word;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitMazeModels(const LoadedMapPackage &package) {
    constexpr const char *names[] = {"SRZ1", "SRZ2", "SRZ3", "SRZ4", "IRZ1", "IRZ2", "IRZ3", "IRZ4", "KRZ1", "KRZ2", "KRZ3", "KRZ4", "NULL04", "NULL05",
        "NULL06", "NULL07", "NULL00", "NULL01", "NULL02", "NULL03", "NULL00", "NULL01", "NULL02", "NULL03"};
    for (std::size_t index = 0; index < std::size(names); ++index) {
        if (std::strncmp(names[index], "NULL", 4) == 0)
            activeRoadModels[index] = activeRoadModels[names[index][5] - 96 + names[index][4]];
        else {
            char filename[16];
            std::snprintf(filename, sizeof(filename), "MAZ%sG", names[index]);
            if (!LoadMazeModel(package, filename, roadModels[index]))
                return false;
            activeRoadModels[index] = &roadModels[index].reader;
        }
        MT7Model &model = *activeRoadModels[index]->data;
        d3t::RegisterObjectPlacement(GetMT7ReaderData(&model), GetMT7ReaderData(&model), 0);
        if (!model.nodes.empty()) {
            const std::uint32_t xb = model.nodes[0].xb01Offset;
            const std::uint32_t word = MazeMeshWord(model, xb) == SHEN_NAME(XB01) ? MazeMeshWord(model, xb + 24) + 10 : 13;
            const std::uint32_t flags = 0x41002000;
            std::memcpy(const_cast<std::byte *>(model.source.data()) + xb + word * 4, &flags, 4);
        }
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeUnitModels(const LoadedMapPackage &package) {
    for (const RoadBlock &block : roadBlocks) {
        for (std::int32_t record = 0; record < block.controlCount; ++record) {
            const std::int32_t type = AuthoredWord(block, record, 0);
            if (type >= 0)
                continue;
            const std::uint8_t index = static_cast<std::uint8_t>(type);
            if (unitModels[index].reader.data)
                continue;
            char name[16];
            std::snprintf(name, sizeof(name), "%sG", authoredUnits[index].model);
            if (!LoadMazeModel(package, name, unitModels[index]))
                return false;
        }
    }
    if (!LoadMazeModel(package, "MAZSRL9G", terrainModel))
        return false;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::size_t MazeRoadModelIndex(const MazeRoad &road) {
    std::size_t kind = 0;
    switch (road.flags & 0x3c0000U) {
    case 0x40000:
        kind = 1;
        break;
    case 0xc0000:
        kind = 2;
        break;
    case 0x80000:
        kind = 3;
        break;
    case 0x100000:
        kind = 4;
        break;
    case 0x140000:
        kind = 5;
        break;
    }
    return kind * 4 + road.index % 4;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeRoadCommandCopies() {
    const MT7Model &model = *activeRoadModels[MazeRoadModelIndex(roadBlocks[0].roads[0])]->data;
    const std::uint32_t xb = model.nodes[0].xb01Offset;
    if (MazeMeshWord(model, xb) != SHEN_NAME(XB01))
        return true;
    const std::uint32_t table = MazeMeshWord(model, xb + 28) * 4 + 28;
    const std::uint32_t count = MazeMeshWord(model, xb + table + 4);
    std::uint32_t size = table + 8 * (count + 1);
    for (std::uint32_t index = 0; index < count; ++index)
        size += MazeMeshWord(model, xb + table + 12 + index * 12);
    roadCommandCopySize = size + 8;
    if (roadCommandCopySize > 0x200)
        return false;
    for (MazeRoadCommandCopy &copy : roadCommandCopies) {
        void *allocation = Allocate(0x200, SHEN_NAME(XMAZ));
        if (!allocation)
            return false;
        copy.bytes = {static_cast<std::byte *>(allocation), 0x200};
        std::memcpy(copy.bytes.data(), model.source.data() + xb, roadCommandCopySize);
        if (!PrepareLegacyDrawResources(copy.bytes, 0, copy.resources))
            return false;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void PrepareMazeRoadCommandCopy(const MazeRoad &road, std::size_t drawIndex) {
    const MT7Model &model = *activeRoadModels[MazeRoadModelIndex(road)]->data;
    const MT7Node &node = model.nodes[0];
    if (MazeMeshWord(model, node.xb01Offset) != SHEN_NAME(XB01))
        return;
    MazeRoadCommandCopy &copy = roadCommandCopies[drawIndex * 2 + (mazeFrameCount & 1)];
    d3t::RetireObjectPlacement(roadCommandCopies[drawIndex * 2].bytes.data());
    d3t::RegisterObjectPlacement(GetMT7ReaderData(&model), roadCommandCopies[drawIndex * 2].bytes.data(), 0);
    std::memcpy(copy.bytes.data(), model.source.data() + node.xb01Offset, roadCommandCopySize);
    RefreshLegacyDrawResources(copy.bytes, 0, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources, copy.resources);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ApplyMazeRoadJoins(const MazeRoad &road, std::size_t drawIndex) {
    MT7Model &model = *activeRoadModels[MazeRoadModelIndex(road)]->data;
    const std::uint32_t xb = model.nodes[0].xb01Offset;
    if (MazeMeshWord(model, xb) == SHEN_NAME(XB01)) {
        MazeRoadCommandCopy &copy = roadCommandCopies[drawIndex * 2 + (mazeFrameCount & 1)];
        std::uint32_t count;
        std::memcpy(&count, copy.bytes.data() + 28, 4);
        const std::uint32_t offset = 4 * count;
        std::memcpy(copy.bytes.data() + offset + 140, &road.leftJoin, sizeof(Vector3));
        std::memcpy(copy.bytes.data() + offset + 108, &road.rightJoin, sizeof(Vector3));
        std::uint32_t size;
        std::memcpy(&size, copy.bytes.data() + offset + 40, 4);
        UploadLegacyVertexBuffer(copy.resources, 0, {copy.bytes.data() + offset + 44, size});
    } else {
        std::memcpy(const_cast<std::byte *>(model.source.data()) + xb + 208, &road.leftJoin, sizeof(Vector3));
        std::memcpy(const_cast<std::byte *>(model.source.data()) + xb + 176, &road.rightJoin, sizeof(Vector3));
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DoMazeTransformationForObject(MazeRoad &road) {
    if (!CheckLineSegmentIntersection_Wrapper(road) || !ApplyTransformationForMazeObject(road, true))
        return;
    AdvanceMazeObjectAlpha(road);
    Matrix4x4_Push();
    if (!mazeUnitDrawEnabled) {
        Matrix4x4::Pop(1);
        return;
    }
    MT7Model &model = *unitModels[MUZT_Data[road.unit].modelIndex].reader.data;
    const RoadBlock &block = roadBlocks[road.block];
    const std::uint16_t record = static_cast<std::uint16_t>(road.flags);
    const std::int32_t yaw = 0x4000 - AuthoredWord(block, record, 24);
    model.nodes[0].rotation.y = static_cast<float>(yaw);
    Matrix4x4::preTranslate(road.position.x, road.position.y + 0.02f, road.position.z);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(road.position.x, road.position.y + 0.02f, road.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    const bool hashed = road.alpha == 1.0f ? d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection()) :
                                             d3t::HashModelWithAlpha(GetMT7ReaderData(&model), road.alpha, GetLegacyPerspectiveProjection());
    Matrix4x4::Pop(1);
    if (!hashed) {
        if (road.alpha == 1.0f)
            charaApplyScale(&model, model.nodes.data());
        else {
            SetLegacyBusyPreparingDraw(true);
            charaPrepareNodesForDraw(&model, model.nodes.data(), road.alpha);
            SetLegacyBusyPreparingDraw(false);
        }
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        const shen_name name = MazeUnitAuthoredName(MUZT_Data[road.unit].modelIndex);
        if (name.name == SHEN_NAME(SOT2) || name.name == SHEN_NAME(HIY4))
            DrawMazeAuxiliaryUnit(road, name);
        else
            ScanMazeAuthoredObjects(name, road);
    }
    Matrix4x4::Pop(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ApplyMazePositionTranslationsFromArray() {
    struct Position {
        shen_name area;
        Vector3 point;
    };
    constexpr Position positions[] = {
        {.area = SHEN_NAME(KMZ1), .point = {-357.18f, 8.0f, -138.1f}},
            {.area = SHEN_NAME(KMZ3), .point = {-91.8f, 10.0f, -499.0f}},
                {.area = SHEN_NAME(KMZ3), .point = {-112.54f, 10.0f, -471.48f}},
                    {.area = SHEN_NAME(KMZ3), .point = {-205.5f, 15.0f, -577.4f}},
                        {.area = SHEN_NAME(KMZ3), .point = {-209.97f, 15.0f, -577.42f}},
                            {.area = SHEN_NAME(KMZ3), .point = {-224.7f, 15.0f, -558.6f}},
                                {.area = SHEN_NAME(KMZ3), .point = {-373.89f, 20.0f, -551.94f}},
                                    {.area = SHEN_NAME(KMZ3), .point = {-368.53f, 19.9f, -517.66f}},
                                        {.area = SHEN_NAME(KMZ3), .point = {-506.37f, 25.0f, -470.9f}},
                                            {.area = SHEN_NAME(KMZ3), .point = {-500.91f, 25.0f, -470.98f}},
                                                {.area = SHEN_NAME(KMZ4), .point = {-194.8f, 24.0f, -762.91f}},
                                                    {.area = SHEN_NAME(KMZ4), .point = {-188.9f, 24.0f, -768.5f}},
                                                };
    const float far = camCtrlGetFarDistance();
    MT7Model &model = *auxiliaryModels[11].model;
    for (const Position &position : positions) {
        const float x = position.point.x - mazeCameraPosition.x;
        const float z = position.point.z - mazeCameraPosition.z;
        if (position.area.name != g_currentMap.name || far * far <= z * z + x * x || model.nodes.empty())
            continue;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.point.x, position.point.y, position.point.z);
        if (!d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection())) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(position.point.x, position.point.y, position.point.z);
            applyPosRotScl(&model, model.nodes.data());
        }
        Matrix4x4::Pop(1);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetTranslationToCurrent() {
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    Matrix4x4_Push();
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    if (mazeTerrainDrawEnabled && !d3t::HashModel(GetMT7ReaderData(terrainModel.model), GetLegacyPerspectiveProjection()))
        charaApplyScale(terrainModel.model, terrainModel.model->nodes.data());
    Matrix4x4::Pop(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeRoad(MazeRoad &road, std::size_t drawIndex) {
    if (!mazeRoadDrawEnabled)
        return;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
    ApplyMazeRoadJoins(road, drawIndex);
    for (const MazeRoadDecoration &decoration : mazeFixedDecorations)
        ApplyMazeRoadDecoration(road, decoration, *mediumModels[decoration.kind].model, true);
    if (!(road.flags & 0x20000000U)) {
        if (mazeRoadPropsEnabled) {
            AdvanceMazeObjectAlpha(road);
            GenerateMazePropStates(road, drawIndex);
            DrawMazePropStates(road, drawIndex);
            DrawMazeTreeEffects(road);
            DrawMazeRoadButterflies(road);
        }
        for (const MazeRoadDecoration &decoration : mazeMarkerDecorations)
            ApplyMazeRoadDecoration(road, decoration, *auxiliaryModels[decoration.kind].model, false);
    }
    MT7Model &model = *activeRoadModels[MazeRoadModelIndex(road)]->data;
    const MT7Node &root = model.nodes[0];
    if (MazeMeshWord(model, root.xb01Offset) == SHEN_NAME(XB01)) {
        MazeRoadCommandCopy &copy = roadCommandCopies[drawIndex * 2 + (mazeFrameCount & 1)];
        const LegacyNodeCommandOverride override{root.sourceOffset, copy.bytes, 0, &copy.resources};
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        const bool hashed = d3t::FindHashedObject(roadCommandCopies[drawIndex * 2].bytes.data(), &road.leftJoin, GetLegacyPerspectiveProjection());
        Matrix4x4::Pop(1);
        if (!hashed)
            DrawMT7Model(model, nullptr, LegacyModelShaderPath::Lambert, 1.0f, &override);
    } else
        charaApplyScale(&model, model.nodes.data());
    Matrix4x4::Pop(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 || !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Callback between reference updates and camera culling.

// @verified - d
void MazeFrameNullsub() {
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MemoNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetMemoLatestPage() {
    g_memoLatestPage = -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t IsItemSelectable() {
    if (GetCurrentItemSelectState() & 0x20U)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void LoadMemoData() {
    g_memoData = 0;
    g_memoMasterTable = 0;
    g_memoRubyTable = 0;
    const std::uint32_t size = shenOpenFile(Filepath_Gen(FilepathGenMode::MiscDirectory), "MEMODATA.BIN");
    std::byte *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, MEMO_WORK_STORAGE));
    if (data) {
        if (shenLoadFile(Filepath_Gen(FilepathGenMode::MiscDirectory), "MEMODATA.BIN", data, size)) {
            g_memoData = data;
            const MapInfoChunk &header = *reinterpret_cast<const MapInfoChunk *>(data);
            g_memoRecordCount = (header.nextByteOffset - 8U) / 40U;
            MapInfoChunk *chunk = reinterpret_cast<MapInfoChunk *>(data + header.nextByteOffset);
            if (chunk->name == SHEN_NAME4('R', 'U', 'B', 'I')) {
                const std::uint32_t next = chunk->nextByteOffset;
                g_memoRubyTable = reinterpret_cast<std::byte *>(chunk + 1);
                chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + next);
            }
            if (chunk->name == SHEN_NAME4('S', '1', 'M', 'M')) {
                g_memoShenmue1RecordCount = (chunk->nextByteOffset - 8U) >> 6;
                g_memoShenmue1Table = reinterpret_cast<std::byte *>(chunk + 1);
                chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + chunk->nextByteOffset);
            }
            if (chunk->name == SHEN_NAME4('M', 'S', 'T', 'R'))
                g_memoMasterTable = reinterpret_cast<std::byte *>(chunk + 1);
            HLib::SetTaskDataMotionFlag(g_memoData);
        } else
            HLib::FreeTaskData(data);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool LoadMemoData() {
    g_memoData = 0;
    g_memoMasterTable = 0;
    g_memoRubyTable = 0;
    const std::uint32_t size = shenOpenFile(Filepath_Gen(FilepathGenMode::MiscDirectory), "MEMODATA.BIN");
    std::byte *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, MEMO_WORK_STORAGE));
    if (!data)
        return false;
    if (shenLoadFile(Filepath_Gen(FilepathGenMode::MiscDirectory), "MEMODATA.BIN", data, size)) {
        g_memoData = data;
        const MapInfoChunk &header = *reinterpret_cast<const MapInfoChunk *>(data);
        g_memoRecordCount = (header.nextByteOffset - 8U) / 40U;
        MapInfoChunk *chunk = reinterpret_cast<MapInfoChunk *>(data + header.nextByteOffset);
        if (SHEN_NAME(RUBI) == chunk->name) {
            const std::uint32_t next = chunk->nextByteOffset;
            g_memoRubyTable = reinterpret_cast<std::byte *>(chunk + 1);
            chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + next);
        }
        if (SHEN_NAME(S1MM) == chunk->name) {
            g_memoShenmue1RecordCount = (chunk->nextByteOffset - 8U) >> 6;
            g_memoShenmue1Table = reinterpret_cast<std::byte *>(chunk + 1);
            chunk = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(chunk) + chunk->nextByteOffset);
        }
        if (SHEN_NAME(MSTR) == chunk->name)
            g_memoMasterTable = reinterpret_cast<std::byte *>(chunk + 1);
        HLib::SetTaskDataMotionFlag(g_memoData);
        return true;
    }
    HLib::FreeTaskData(data);
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeMemoPageState(std::int16_t page, std::int16_t side) {
    g_memoPage = page;
    g_memoFlag = side;
    g_memoPreviousFlag = -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t GetMemoPageStateValue(std::uint8_t side) {
    if (side)
        return g_memoFlag;
    return static_cast<std::int16_t>(g_memoPage);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void OpenMemoFromNotebookMenu() {
    RefreshPadTask(2, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint8_t UpdateSelectorMemo() {
    return IsItemSelectable();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMemoTextureName(std::byte *header, const char *name) {
    std::int32_t id[2];
    const char *source = name;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(::strlen(name)); ++index)
        reinterpret_cast<char *>(id)[index % 8] = *source++;
    reinterpret_cast<std::int32_t *>(header)[2] = id[0];
    reinterpret_cast<std::int32_t *>(header)[3] = id[1];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool NotifyMemoFlag(std::int32_t flag, bool selected) {
    const std::int16_t *row = memoLineFlags[0];
    std::int32_t latest = -1;
    std::int16_t first = static_cast<std::int16_t>(latest);
    const std::int32_t process = selected ? GetInternalFlag(flag) == 0 : 1;
    if (process) {
        for (std::int32_t page = 0; page < static_cast<std::int32_t>(g_memoRecordCount); ++page, row += 10) {
            for (std::int32_t line = 0; line < 10; ++line) {
                std::int32_t matches = 0;
                if (row[line] < 0) {
                    for (std::int32_t choice = 0; choice < 2; ++choice) {
                        if (static_cast<std::uint32_t>(flag) ==
                            static_cast<std::uint32_t>(conditionalMemoLines[(row[line] < 0 ? -row[line] : row[line]) - 1].flags[choice]))
                            matches = 1;
                    }
                } else if (static_cast<std::uint32_t>(flag) == static_cast<std::uint32_t>(row[line]))
                    matches = 1;
                if (matches == 1) {
                    if (first < 0)
                        first = static_cast<std::int16_t>(page);
                    else
                        latest = static_cast<std::int16_t>(page);
                }
            }
        }
    }
    if (first < 0)
        return false;
    g_memoPage = static_cast<std::uint16_t>(first);
    g_memoLatestPage = static_cast<std::int16_t>(latest);
    if (latest < 0)
        g_memoLatestPage = static_cast<std::int16_t>(first);
    g_memoFlag = static_cast<std::int16_t>(flag);
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool NotifyMemoFlag(std::int32_t flag, bool selected) {
    const std::int16_t *row = memoLineFlags[0];
    std::int16_t latest = -1;
    std::int16_t first = -1;
    const bool process = selected ? !GetInternalFlag(flag) : true;
    bool found = false;
    if (process) {
        for (std::int32_t page = 0; page < static_cast<std::int32_t>(g_memoRecordCount); ++page, row += 10) {
            for (std::int32_t line = 0; line < 10; ++line) {
                const std::int16_t candidate = row[line];
                bool matches = false;
                if (candidate < 0) {
                    for (std::int32_t choice = 0; choice < 2; ++choice) {
                        std::int32_t condition = row[line];
                        if (condition < 0)
                            condition = -condition;
                        if (static_cast<std::uint32_t>(flag) == static_cast<std::uint32_t>(conditionalMemoLines[condition - 1].flags[choice]))
                            matches = true;
                    }
                } else {
                    matches = static_cast<std::uint32_t>(flag) == static_cast<std::uint32_t>(candidate);
                }
                if (matches) {
                    if (first < 0)
                        first = static_cast<std::int16_t>(page);
                    else
                        latest = static_cast<std::int16_t>(page);
                }
            }
        }
        found = first >= 0;
    }
    if (!found)
        return false;
    g_memoLatestPage = latest < 0 ? first : latest;
    g_memoFlag = static_cast<std::int16_t>(flag);
    g_memoPage = static_cast<std::uint16_t>(first);
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AppendCentringSpaces(char *text, std::int16_t width) {
    std::int32_t count = width - static_cast<std::int32_t>(::strlen(text) >> 1);
    if (count >= 0)
        for (; count; --count)
            ::strcat(text, "\241\241");
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool SelectMemoFlagText(char *text, char *ruby, std::int32_t line) {
    if (line >= 0)
        return false;
    line = line < 0 ? -line : line;
    const ConditionalMemoLine *entry = &conditionalMemoLines[line - 1];
    for (std::int32_t choice = 0; choice < 2; ++choice) {
        if (GetInternalFlag(entry->flags[choice])) {
            if (entry->text[choice])
                ::strcpy(text, entry->text[choice]);
            if (entry->ruby[choice])
                ::strcpy(ruby, entry->ruby[choice]);
            return true;
        }
    }
    return false;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeMazeRenderModelNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeMazePostLoadNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const char *GetMazeDirectory() {
    return g_actNumber == 4 ? "/scene/04/MAZE/" : reinterpret_cast<const char *>(g_actNumber);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MarkMazeUnitModelsUsed() {
    for (std::int32_t index = 0; index < 64; ++index)
        mazeUnitModelUsed[index] = 0;
    const std::int32_t blocks = GetMazeBlockCount();
    for (std::int32_t block = 0; block < blocks; ++block) {
        const std::int32_t count = GetMazeBlockPointCount(block);
        for (std::int32_t record = 0; record < count; ++record) {
            const std::uint32_t type = GetAuthoredWord(block, record);
            if (type & 0x80000000U)
                mazeUnitModelUsed[type & 0xffU] = 1;
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MarkMazeMediumModelsUsed() {
    std::int32_t visited[6];
    for (std::int32_t index = 0; index < 6; ++index)
        visited[index] = 0;
    for (std::int32_t index = 0; index < 35; ++index)
        mazeMediumModelUsed[index] = 0;
    const std::int32_t blocks = GetMazeBlockCount();
    for (std::int32_t block = 0; block < blocks; ++block) {
        const std::int32_t count = GetMazeBlockPointCount(block);
        for (std::int32_t record = 0; record < count; ++record) {
            const std::int32_t style = MazeRoadStyle(static_cast<std::uint32_t>(GetAuthoredRecordStyle(block, record)) << 16);
            if (!visited[style]) {
                MarkMazePropTableModelsUsed(style);
                visited[style] = 1;
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MarkMazePropTableModelsUsed(std::int32_t style) {
    for (const MazePropRecord *prop = GetMazePropTable(style); prop->flags != 0xffffffffU; ++prop)
        if (!(prop->flags & 0xc000U))
            mazeMediumModelUsed[prop->kind] = 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t MazeRoadStyle(std::uint32_t flags) {
    switch (static_cast<std::int32_t>(flags & 0x3c0000U)) {
    case 0:
        return 0;
    case 0x40000:
        return 1;
    case 0xc0000:
        return 2;
    case 0x80000:
        return 3;
    case 0x100000:
        return 4;
    case 0x140000:
        return 5;
    default:
        return 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
char mazeLoadFilename[32];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMazeRoadModels() {
    static const char *roadNames[] = {"SRZ1", "SRZ2", "SRZ3", "SRZ4", "IRZ1", "IRZ2", "IRZ3", "IRZ4", "KRZ1", "KRZ2", "KRZ3", "KRZ4", "NULL04", "NULL05",
        "NULL06", "NULL07", "NULL00", "NULL01", "NULL02", "NULL03", "NULL00", "NULL01", "NULL02", "NULL03"};
    for (std::int32_t index = 0; index < 24; ++index) {
        if (!::strncmp(roadNames[index], "NULL", 4)) {
            std::int32_t source = roadNames[index][4] - '0';
            source += roadNames[index][5] - '0';
            g_mazeRoadModels[index] = g_mazeRoadModels[source];
        } else {
            ::sprintf(mazeLoadFilename, "MAZ%sG", roadNames[index]);
            g_mazeRoadModels[index] = LoadGameModel(mazePackage, mazeLoadFilename);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MT7Model *GetMazeRoadModel(const MazeRoad *road) {
    const std::int32_t style = GetMazeRoadStyle(road), variant = road->index % 4;
    return g_mazeRoadModels[style * 4 + variant];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazePropRecord *GetMazePropTable(std::int32_t style) {
    return g_mazePropTables[style];
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeMazeRuntime(std::int32_t act, shen_name area) {
    if (act != 4)
        return;
    switch (static_cast<std::int32_t>(area)) {
    case SHEN_NAME4('K', 'W', 'Q', 'A'):
        SetMazeAuxiliaryWord(0);
        break;
    case SHEN_NAME4('K', 'M', 'Z', '1'):
        SetMazeAuxiliaryWord(1);
        break;
    case SHEN_NAME4('K', 'M', 'Z', '2'):
        SetMazeAuxiliaryWord(2);
        break;
    case SHEN_NAME4('K', 'M', 'Z', '3'):
        SetMazeAuxiliaryWord(3);
        break;
    case SHEN_NAME4('K', 'M', 'Z', '4'):
        SetMazeAuxiliaryWord(4);
        break;
    default:
        return;
    }
    mazeInit();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void MazeRuntimeNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void mazeInit() {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    LoadMazePackage();
    SetMazeDefaults();
    mazeRoadDrawCount = 0;
    mazePropDrawCount = 0;
    g_mazeUnitCount = 0;
    MUZT_Data = static_cast<MazeUnit *>(HLib::AllocateTaskData(64 * 0xb0, SHEN_NAME4('M', 'Z', 'U', 'T')));
    LoadMazeMarkers();
    mazeSetCurrentRefCount(0);
    mazeSplineBuffer = static_cast<Vector3 *>(HLib::AllocateTaskData(0x30c, SHEN_NAME4('M', 'Z', 'B', 'F')));
#else
    static const char *mapNames[] = {"qte", "kmz1", "kmz2", "kmz3", "kmz4"};
    LoadMazePackage();
    SetMazeDefaults();
    g_mazeUnitCount = 0;
    MUZT_Data = static_cast<MazeUnit *>(HLib::AllocateTaskData(64 * 0xb0, SHEN_NAME4('M', 'Z', 'U', 'T')));
    LoadMazeMarkers();
    mazeSetCurrentRefCount(0);
    mazeSplineBuffer = static_cast<Vector3 *>(HLib::AllocateTaskData(0x30c, SHEN_NAME4('M', 'Z', 'B', 'F')));
    char filename[32];
    ::sprintf(filename, "%smap", mapNames[GetMazeAuxiliaryWord()]);
    mazeAuthoredMap = static_cast<std::byte *>(FileLoader(mazePackage, filename, PACKAGE_BINARY_EXTENSION));
    mazeUnitAreas = static_cast<std::byte *>(FileLoader(mazePackage, "unitarea", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[0] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_mori", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[5] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_lwod", PACKAGE_BINARY_EXTENSION));
    g_mazePropTables[1] = static_cast<MazePropRecord *>(FileLoader(mazePackage, "tbl_iwa", PACKAGE_BINARY_EXTENSION));
#endif
    InitializeMazeModels();
    const std::int32_t blocks = GetMazeBlockCount();
    for (std::int32_t block = 0; block < blocks; ++block) {
        mazeControlPoints[block] = static_cast<Vector3 *>(HLib::AllocateTaskData(GetMazeBlockPointCount(block) * 12, SHEN_NAME4('M', 'Z', 'P', 'T')));
        Vector3 *points = mazeControlPoints[block];
        for (std::int32_t index = 0; index < GetMazeBlockPointCount(block); ++index) {
            points[index] = *GetAuthoredRecordPoint(block, index);
            points[index].y += 0.005f;
        }
    }
    for (std::int32_t block = 0; block < blocks; ++block) {
        g_mazeRoadBlocks[block] = BuildMazeRoads(block, mazeControlPoints[block], GetMazeBlockPointCount(block));
        MazeRoad *road = g_mazeRoadBlocks[block];
        std::int32_t roads = 0;
        for (; road->index >= 0; ++road)
            ++roads;
        SetMazeRoadCount(block, roads);
    }
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MazeTaskCallback, 14, 2, 8, shen_name(SHEN_NAME4('M', 'A', 'Z', 'E')));
    static_cast<MazeTaskData *>(HLib::GetTaskParameter(task))->task = task;
#else
    mazeTask = HLib::EnqueueTaskWithParameter(MazeTaskCallback, 14, 2, 8, shen_name(SHEN_NAME4('M', 'A', 'Z', 'E')));
    static_cast<MazeTaskData *>(HLib::GetTaskParameter(mazeTask))->task = mazeTask;
#endif
    mazeRouteBuffer = static_cast<Vector3 *>(HLib::AllocateTaskData(0x3000, SHEN_NAME4('M', 'Z', 'R', 'T')));
    BuildMazePartitions();
    InitializeMazeRouteLinks();
    InitializeMazeAuxiliaryModels();
    InitializeMazePostLoadNullsub();
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMazeRoadCount(std::int32_t block, std::int32_t count) {
    mazeRoadCounts[block] = count;
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeRoadCountRaw(std::int32_t block) {
    return mazeRoadCounts[block];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetMazeRoadStyle(const MazeRoad *road) {
    return MazeRoadStyle(road->flags);
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
MazeRoad *BuildMazeRoads(std::int32_t block, Vector3 *points, std::int32_t count) {
    const float length = MeasureMazeSpline(points, count);
    std::int32_t samples = static_cast<std::int32_t>(length * 0.5f) + 4;
    MazeRoad *roads = static_cast<MazeRoad *>(HLib::AllocateTaskData((samples + count) * 0x84, SHEN_NAME4('M', 'A', 'Z', 'E')));
    PlaceMazeRoads(block, points, count, roads);
    BuildMazeRoadJoins(roads);
    BuildMazeRoadEndJoins(roads);
    BuildMazeUnits(roads);
    return roads;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float MeasureMazeSpline(const Vector3 *points, std::int32_t count) {
    std::int32_t limit = count - 3;
    limit *= 64;
    float length = 0.0f;
    for (std::int32_t start = 0; start < limit; start += 64) {
        SampleMazeSpline(points, count, start, mazeSplineBuffer);
        length += MazeSplineLength(64, mazeSplineBuffer);
    }
    return length;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t PlaceMazeRoads(std::int32_t block, const Vector3 *points, std::int32_t count, MazeRoad *road) {
    std::int32_t roadCount = 0;
    for (std::int32_t segment = 1; segment < count - 2; ++segment) {
        SampleMazeSpline(points, count, (segment - 1) * 64, mazeSplineBuffer);
        const float halfLength = MazeSplineLength(64, mazeSplineBuffer) * 0.5f;
        std::int32_t samples = static_cast<std::int32_t>(halfLength);
        if (halfLength - static_cast<float>(samples) > 0.5f)
            ++samples;
        if (samples < 1)
            samples = 1;
        for (std::int32_t sample = 0; sample < samples; ++sample) {
            SampleMazeRoadPlacement(static_cast<float>(sample * 64 / samples), mazeSplineBuffer, road);
            road->block = block;
            road->index = roadCount;
            road->flags = 0;
            SetMazeRoadFlags(road, segment, sample);
            ++road;
            ++roadCount;
            road->index = -1;
        }
    }
    return roadCount;
}
#endif

} // namespace
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t CountMazePointerList(const void *const *list) {
    std::int32_t count = 0;
    if (!list)
        return 0;
    while (*list++)
        ++count;
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t CountMazePathPoints(const Vector2 *points) {
    std::int32_t count = 0;
    for (; points->x < 9990.0f; ++points)
        ++count;
    return count;
}
#endif

namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 **BuildMazeUnitPaths(const Vector2 *const *paths) {
    if (!paths)
        return 0;
    const std::int32_t count = CountMazePointerList(reinterpret_cast<const void *const *>(paths));
    std::int32_t index, total = 0;
    for (index = 0; index < count; ++index)
        total += CountMazePathPoints(paths[index]);
    total += 3;
    Vector3 **lists = static_cast<Vector3 **>(HLib::AllocateTaskData(count << 2, SHEN_NAME4('M', 'Z', 'R', 'T')));
    Vector3 *point = static_cast<Vector3 *>(HLib::AllocateTaskData(total * 12, SHEN_NAME4('M', 'Z', 'R', 'T')));
    for (index = 0; index < count; ++index) {
        lists[index] = point;
        std::int32_t remaining = CountMazePathPoints(paths[index]);
        const Vector2 *source = paths[index];
        while (remaining--) {
            matrix_stack::Matrix4x4_Push();
            Matrix4x4::preTranslate(source->x, 0.0f, source->y);
            matrix_stack::GetTranslation(point);
            PopCurrentMatrix(1);
            ++source;
            ++point;
        }
        point->x = 0.0f;
        point->y = 1000.0f;
        point->z = 0.0f;
        ++point;
    }
    return lists;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitMazeJoinVector(Vector3 *vector) {
    vector->x = 5.0f;
    vector->y = 2.0f;
    vector->z = 1.0f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMazeMarkers() {
    static const char *markerNames[] = {"DMY", "kmz1", "kmz2", "kmz3", "kmz4", "end"};
    g_mazeMarkerCount = 0;
    if (::strcmp(markerNames[GetMazeAuxiliaryWord()], "DMY")) {
        ::sprintf(mazeLoadFilename, "%sev", markerNames[GetMazeAuxiliaryWord()]);
        g_mazeMarkers = static_cast<MazeMarker *>(FileLoader(mazePackage, mazeLoadFilename, PACKAGE_BINARY_EXTENSION));
        while (g_mazeMarkers[g_mazeMarkerCount].id >= 0)
            ++g_mazeMarkerCount;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMazePackage() {
    const char *directory, *area;
    switch (GetMazeAuxiliaryWord()) {
    case 1:
        directory = GetMazeDirectory();
        area = "KMZ1";
        break;
    case 2:
        directory = GetMazeDirectory();
        area = "KMZ2";
        break;
    case 3:
        directory = GetMazeDirectory();
        area = "KMZ3";
        break;
    case 4:
        directory = GetMazeDirectory();
        area = "KMZ4";
        break;
    case 0:
        directory = GetMazeDirectory();
        area = "QTE";
        break;
    default:
        return;
    }
    mazePackage = RequestMapPackage(directory, area);
    pakLoadFlush();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void RetireMazeModels() {
    for (MazeRoadCommandCopy &copy : roadCommandCopies) {
        if (!copy.bytes.empty())
            d3t::RetireObjectPlacement(copy.bytes.data());
        ReleaseLegacyDrawResources(copy.resources);
        copy.bytes = {};
    }
    if (terrainModel.model)
        d3t::RetireObjectPlacement(GetMT7ReaderData(terrainModel.model));
    terrainModel = {};
    for (std::span<MazeModelResource> resources :
        {std::span<MazeModelResource>{mediumModels}, std::span<MazeModelResource>{treeModels}, std::span<MazeModelResource>{auxiliaryModels},
            std::span<MazeModelResource>{roadModels}, std::span<MazeModelResource>{unitModels}, std::span<MazeModelResource>{authoredObjectModels}}) {
        for (MazeModelResource &resource : resources) {
            if (resource.model)
                d3t::RetireObjectPlacement(GetMT7ReaderData(resource.model));
            resource = {};
        }
    }
    propTables = {};
    propStates = {};
    mediumOccupancy = {};
    roadCommandCopySize = 0;
    activeRoadModels = {};
    for (MazeAuthoredObject &object : authoredObjects)
        object.modelReader = nullptr;
    currentMazeObjects = {};
    currentMazeUnitObjects = {};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool LoadMazeModel(const LoadedMapPackage &package, const char *name, MazeModelResource &resource) {
    PakGlobalData *const owner = FindPakByEntryName(package.name.data());
    resource.model = LoadGameModel(owner, name);
    if (!resource.model)
        return false;
    resource.reader = {.source = resource.model->source.data(), .data = resource.model};
    const MT7Node *const data = GetMT7ReaderData(resource.model);
    d3t::RegisterObjectPlacement(data, data, 0);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
shen_name MazeUnitAuthoredName(std::int32_t modelIndex) {
    const char *const name = authoredUnits[modelIndex].model;
    return shen_name{make_shen_name(name[3], name[4], name[5], name[6])};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeAuthoredObjects(const LoadedMapPackage &package) {
    for (std::size_t index = 0; index < std::size(authoredObjects) - 1; ++index) {
        MazeAuthoredObject &object = authoredObjects[index];
        if (!HasMazeUnitOfKind(object.name))
            continue;
        // LoadGameModel returns the package's existing reader for duplicate sources.
        for (std::size_t previous = 0; previous < index; ++previous)
            if (authoredObjects[previous].modelReader && std::strcmp(authoredObjects[previous].resource, object.resource) == 0)
                object.modelReader = authoredObjects[previous].modelReader;
        if (!object.modelReader) {
            if (!LoadMazeModel(package, object.resource, authoredObjectModels[index]))
                return false;
            object.modelReader = &authoredObjectModels[index].reader;
        }
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ApplyMazeAuthoredUV(MazeAuthoredObject &object) {
    float amount = 0.0f;
    if (object.flags & 1U)
        amount = 0.039999999f;
    if (object.flags & 2U)
        amount = 0.02f;
    if (object.flags & 4U)
        amount = 0.0012000001f;
    if (object.flags & 8U)
        amount = 0.00050000002f;
    if (object.flags & 0x100000U)
        amount = -amount;
    const float u = object.flags & 0x80000U ? amount : 0.0f;
    const float v = object.flags & 0x40000U ? amount : 0.0f;
    SetTextureAnimationUV(u, v);
    if ((g_loadingFlag & 2U) == 0) {
        MT7Model &model = *object.modelReader->data;
        for (MT7Node &node : model.nodes)
            if (node.xb01Offset)
                UpdateLegacyTextureUVs(model.source, node.xb01Offset,
                    node.borrowedMesh ? const_cast<LegacyDrawResources &>(node.borrowedMesh->drawResources) : node.drawResources, u, v);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeAuthoredAnimatedObject(MazeAuthoredObject &object) {
    Matrix4x4_Push();
    ApplyMazeAuthoredUV(object);
    const std::uint32_t phase = mazeCurrentReferenceCount >> 4;
    const float bob =
        (mazeBob[phase & 15] - mazeBob[(phase - 1) & 15]) * (static_cast<float>(mazeCurrentReferenceCount & 15) * 0.0625f) + mazeBob[(phase - 1) & 15];
    if (object.flags & 0x20U)
        Matrix4x4::preTranslate(0.0f, bob * 0.5f, 0.0f);
    if (object.flags & 0x10U)
        Matrix4x4::preTranslate(0.0f, bob * 0.1f, 0.0f);
    applyPosRotScl(object.modelReader->data, object.modelReader->data->nodes.data());
    Matrix4x4::Pop(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void DrawMazeAuthoredHourObject(MazeAuthoredObject &object) {
    DateTime current{};
    shenGetCurrentDateTime(current);
    if (current.hour != mazeHourDate.hour && current.minute == mazeHourDate.minute) {
        mazeHourBlend = 0.99998999f;
        mazePreviousHourFrame = mazeHourFrame;
        if (++mazeHourFrame >= 5)
            mazeHourFrame = 0;
        shenGetCurrentDateTime(mazeHourDate);
    }
    ApplyMazeAuthoredUV(object);
    if ((g_loadingFlag & 2U) == 0)
        mazeHourBlend = std::fmin(0.99998999f, std::fmax(0.0000099999997f, mazeHourBlend - 0.0125f));
    MT7Model &model = *object.modelReader->data;
    std::int32_t selected = model.nodes[0].firstChild;
    std::int32_t previous = selected;
    for (std::int32_t index = 0; index < mazeHourFrame; ++index)
        selected = model.nodes[selected].nextSibling;
    for (std::int32_t index = 0; index < mazePreviousHourFrame; ++index)
        previous = model.nodes[previous].nextSibling;
    const MT7Node &node = model.nodes[selected];
    Matrix4x4_Push();
    Matrix4x4::preTranslate(node.position.x, node.position.y + (object.name.name == SHEN_NAME(HIY1) ? -0.2f : 0.0f), node.position.z);
    ExecuteLegacyDrawCommandStream(model.source, model.nodes[previous].xb01Offset,
        model.nodes[previous].borrowedMesh ? model.nodes[previous].borrowedMesh->drawResources : model.nodes[previous].drawResources,
        LegacyModelShaderPath::Lambert, mazeHourBlend);
    ExecuteLegacyDrawCommandStream(model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources,
        LegacyModelShaderPath::Lambert, 1.0f - mazeHourBlend);
    Matrix4x4::Pop(1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ScanMazeAuthoredObjects(shen_name name, const MazeRoad &context) {
    for (MazeAuthoredObject &object : authoredObjects) {
        if (static_cast<std::int32_t>(object.flags) < 0)
            break;
        if (object.name.name != name.name)
            continue;
        if (object.flags & 0x10000U)
            DrawMazeAuthoredAnimatedObject(object);
        if (object.flags & 0x20000U)
            DrawMazeAuthoredHourObject(object);
        if (object.flags & 0x8000U)
            FISH_Draw(object, context);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ResetMazeState(bool releaseAllocations) {
    if (releaseAllocations) {
        if (mazeTask)
            HLib::DestroyTask(mazeTask);
        if (fishTask)
            HLib::DestroyTask(fishTask);
    }
    RetireMazeModels();
    fishTask = nullptr;
    fishControlPoints = nullptr;
    fishPathSamples = nullptr;
    fishModel = nullptr;
    fishEnabled = false;
    markerBytes = {};
    markerCount = 0;
    unitAreas = {};
    unitCount = 0;
    partitionCount = 0;
    partitions = {};
    links = {};
    roadBlocks.clear();
    if (releaseAllocations) {
        for (std::size_t i = allocations.size(); i > 0; --i)
            HLib::FreeTaskData(allocations[i - 1]);
        if (ownsTerrainPackage)
            UnloadMapPackage(terrainPackage);
    }
    allocations.clear();
    MUZT_Data = nullptr;
    splineBuffer = nullptr;
    routeBuffer = nullptr;
    ownsTerrainPackage = false;
    terrainPackage = nullptr;
    mazeTask = nullptr;
    mazeCurrentReferenceCount = 0;
    mazeFrameCount = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateMazeTask(void *taskData) {
    MazeTaskData *const data = static_cast<MazeTaskData *>(taskData);
    if (data)
        data->reserved = 0;
    if (!IsWorldDrawingEnabled())
        return;
    Matrix4x4::GetTransformMatrix(&mazeViewTransform);
    InitializeMazeFrameEnvironment();
    FreeNecessaryMazeObjects();
    ApplyMazeUnitChildWiggle();
    ApplyMazePositionTranslationsFromArray();
    if ((g_loadingFlag & 2U) == 0)
        ++mazeCurrentReferenceCount;
    MazeFrameNullsub();
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    float closest;
    InitializeMazeObjectTransforms(&camera, &closest);
    ApplyTransformationForCurrentMazeObjects();
    SetTranslationToCurrent();
    mediumOccupancy.fill(0);
    std::size_t drawIndex = 0;
    for (MazeRoad *road : currentMazeObjects) {
        if (!road)
            break;
        PrepareMazeRoadCommandCopy(*road, drawIndex);
        DrawMazeRoad(*road, drawIndex++);
    }
    for (MazeRoad *road : currentMazeUnitObjects) {
        if (!road)
            break;
        DoMazeTransformationForObject(*road);
    }
    SetSharedPrngSeed(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta));
    ++mazeFrameCount;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void MazeTaskCallback(void *taskData) {
    HLib::ExecuteCallbackNow(UpdateMazeTask, taskData);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void *Allocate(std::size_t bytes, shen_name name) {
    void *data = HLib::AllocateTaskData(bytes, name);
    if (data)
        allocations.push_back(data);
    return data;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
template <class T> T ReadValue(std::span<const std::byte> bytes, std::size_t offset) {
    T result;
    std::memcpy(&result, bytes.data() + offset, sizeof(result));
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::span<const std::byte> FindTerrainEntry(const LoadedMapPackage &package, const char *name) {
    std::span<const std::byte> bytes = package.pks.Find(name);
    if (bytes.empty())
        bytes = package.pkf.Find(name);
    return bytes;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t AuthoredWord(const RoadBlock &block, std::int32_t record, std::size_t offset) {
    return ReadValue<std::int32_t>(block.authored, 56 * static_cast<std::size_t>(record) + offset);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Vector3 CurrentTranslation() {
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    return {matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Vector3 TranslatePoint(float x, float y, float z) {
    Matrix4x4_Push();
    Matrix4x4::preTranslate(x, y, z);
    const Vector3 result = CurrentTranslation();
    Matrix4x4::Pop();
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SampleControlSpline(const RoadBlock &block, std::int32_t sample, Vector3 &output) {
    const float parameter = static_cast<float>(sample) * 0.015625f;
    std::int32_t index = static_cast<std::int32_t>(parameter);
    float t = parameter - static_cast<float>(index);
    if (parameter >= static_cast<float>(block.controlCount - 3)) {
        index = block.controlCount - 4;
        t = 1.0f;
    }
    const Vector3 *points = block.controlPoints + index;
    const Matrix4x4 coordinates{{
        {points[0].x, points[1].x, points[2].x, points[3].x},
        {points[0].y, points[1].y, points[2].y, points[3].y},
        {points[0].z, points[1].z, points[2].z, points[3].z},
        {1.0f, 1.0f, 1.0f, 1.0f},
    }};
    constexpr Matrix4x4 basis{{
        {-0.1666666716337204f, 0.5f, -0.5f, 0.1666666716337204f},
        {0.5f, -1.0f, 0.0f, 0.6666666865348816f},
        {-0.5f, 0.5f, 0.5f, 0.1666666716337204f},
        {0.1666666716337204f, 0.0f, 0.0f, 0.0f},
    }};
    const float square = t * t;
    const float cube = square * t;
    const Matrix4x4 powers{{
        {cube * 6.0f, cube * 6.0f, cube, 0.0f},
        {0.0f, square + square, square, 0.0f},
        {0.0f, 0.0f, t, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    Matrix4x4_Push(&powers);
    Matrix4x4::MultiplyWithPrePassMatrix(&basis);
    Matrix4x4::MultiplyWithPrePassMatrix(&coordinates);
    Matrix4x4 result;
    Matrix4x4::GetTransformMatrix(&result);
    Matrix4x4::Pop(0);
    output = {result.rows[0].z + result.rows[0].w, result.rows[1].z + result.rows[1].w, result.rows[2].z + result.rows[2].w};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
float SampleSegment(const RoadBlock &block, std::int32_t segment) {
    for (std::int32_t i = 0; i <= 64; ++i)
        SampleControlSpline(block, segment * 64 + i, splineBuffer[i]);
    float length = 0.0f;
    for (std::int32_t i = 1; i <= 64; ++i) {
        const float x = splineBuffer[i - 1].x - splineBuffer[i].x;
        const float y = splineBuffer[i - 1].y - splineBuffer[i].y;
        const float z = splineBuffer[i - 1].z - splineBuffer[i].z;
        length += std::sqrt((x * x + y * y) + z * z);
    }
    return length;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetRoadFlags(MazeRoad &road, const RoadBlock &block, std::int32_t record, std::int32_t sample) {
    const std::int32_t type = AuthoredWord(block, record, 0);
    const std::int32_t next = AuthoredWord(block, record + 1, 0);
    const std::int32_t previous = AuthoredWord(block, record - 1, 0);
    std::int32_t metadata = AuthoredWord(block, record, 28);
    road.flags = 0;
    if (type) {
        if (!metadata) {
            const std::int32_t previousMetadata = AuthoredWord(block, record - 1, 28);
            const std::int32_t nextMetadata = AuthoredWord(block, record + 1, 28);
            if (nextMetadata)
                metadata = nextMetadata;
            if (previousMetadata)
                metadata = previousMetadata;
        }

        road.flags = static_cast<std::uint32_t>(record);
        if (!next)
            road.flags = sample == 0 ? 0x24000000U : 0;
        if (!previous && sample == 0)
            road.flags = 0x28000000U;
    }
    if (sample == 0)
        road.flags |= type < 0 ? 0xc0000000U : 0x40000000U;
    road.flags |= static_cast<std::uint32_t>(metadata) << 16;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void BuildRoadJoins(RoadBlock &block) {
    for (std::int32_t i = 0; i < block.roadCount; ++i) {
        MazeRoad &road = block.roads[i];
        if (i + 1 == block.roadCount) {
            road.leftJoin = {-5.0f, 0.0f, -1.0f};
            road.rightJoin = {5.0f, 0.0f, -1.0f};
        } else {
            const MazeRoad &next = block.roads[i + 1];
            Matrix4x4_Push();
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::preTranslate(next.position.x, next.position.y, next.position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(next.yaw));
            const Vector3 left = TranslatePoint(-5.0f, 0.0f, 1.0f);
            const Vector3 right = TranslatePoint(5.0f, 0.0f, 1.0f);
            Matrix4x4::Pop();

            Matrix4x4_Push();
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-road.yaw));
            Matrix4x4::preTranslate(-road.position.x, -road.position.y, -road.position.z);
            road.leftJoin = TranslatePoint(left.x, left.y, left.z);
            road.rightJoin = TranslatePoint(right.x, right.y, right.z);
            Matrix4x4::Pop();

            if (road.leftJoin.z > 1.0f)
                road.leftJoin.z = 1.0f;
            if (road.rightJoin.z > 1.0f)
                road.rightJoin.z = 1.0f;
            road.leftJoin.y = next.position.y - road.position.y;
            road.rightJoin.y = road.leftJoin.y;
        }
        road.leftJoin.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.leftJoin.x) | 1U);
        road.rightJoin.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.rightJoin.x) | 1U);
    }
    Matrix4x4_Push();
    for (std::int32_t i = 0; i < block.roadCount; ++i) {
        MazeRoad &road = block.roads[i];
        const float leftX = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.leftJoin.x) & ~1U);
        const float rightX = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.rightJoin.x) & ~1U);
        const float centreX = (leftX + rightX) * 0.5f;
        const float centreZ = (road.leftJoin.z + road.rightJoin.z) * 0.5f;
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        road.leftStart = TranslatePoint(-7.5f, 0.0f, 1.0f);
        road.leftEnd = TranslatePoint((leftX - centreX) * 1.5f + centreX, 0.0f, (road.leftJoin.z - centreZ) * 1.5f + centreZ);
        road.rightStart = TranslatePoint(7.5f, 0.0f, 1.0f);
        road.rightEnd = TranslatePoint((rightX - centreX) * 1.5f + centreX, 0.0f, (road.rightJoin.z - centreZ) * 1.5f + centreZ);
        road.leftCentre.x = (road.leftEnd.x + road.leftStart.x) * 0.5f;
        road.leftCentre.z = (road.leftEnd.z + road.leftStart.z) * 0.5f;
        road.rightCentre.x = (road.rightEnd.x + road.rightStart.x) * 0.5f;
        road.rightCentre.z = (road.rightEnd.z + road.rightStart.z) * 0.5f;
    }
    Matrix4x4::Pop();
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool BuildRoads(RoadBlock &block, std::int32_t blockIndex) {
    float length = 0.0f;
    for (std::int32_t i = 0; i < block.controlCount - 3; ++i)
        length += SampleSegment(block, i);
    const std::int32_t capacity = block.controlCount - static_cast<std::int32_t>(length * -0.5f) + 4;
    block.roads = static_cast<MazeRoad *>(Allocate(sizeof(MazeRoad) * capacity, SHEN_NAME(MAZE)));
    if (!block.roads)
        return false;
    for (std::int32_t segment = 0; segment < block.controlCount - 3; ++segment) {
        const float halfLength = SampleSegment(block, segment) * 0.5f;
        std::int32_t samples = static_cast<std::int32_t>(halfLength);
        if (halfLength - static_cast<float>(samples) > 0.5f)
            ++samples;
        if (samples < 1)
            samples = 1;
        if (samples >= capacity - block.roadCount)
            return false;
        for (std::int32_t i = 0; i < samples; ++i) {
            MazeRoad &road = block.roads[block.roadCount];
            road = {};
            const std::int32_t sample = (i * 64) / samples;
            const Vector3 &first = splineBuffer[sample];
            const Vector3 &second = splineBuffer[sample + 1];
            road.position = first;
            road.yaw = static_cast<std::int16_t>(FixedAtan2(first.x - second.x, first.z - second.z));
            road.block = blockIndex;
            road.index = block.roadCount++;
            SetRoadFlags(road, block, segment + 1, i);
        }
    }
    block.roads[block.roadCount] = {};
    block.roads[block.roadCount].index = -1;
    BuildRoadJoins(block);
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
Vector3 **AllocateMazeUnitPaths(std::span<const AuthoredPath> paths) {
    if (paths.empty())
        return nullptr;
    std::size_t totalPoints = 0;
    for (const AuthoredPath &path : paths)
        totalPoints += path.points.size();
    Vector3 **const lists = static_cast<Vector3 **>(Allocate(paths.size() * sizeof(Vector3 *), MEMORY_MAZE_ROUTES));
    Vector3 *cursor = static_cast<Vector3 *>(Allocate((totalPoints + 3) * sizeof(Vector3), MEMORY_MAZE_ROUTES));
    if (!lists || !cursor)
        return nullptr;
    for (std::size_t index = 0; index < paths.size(); ++index) {
        lists[index] = cursor;
        for (const Vector2 &point : paths[index].points)
            *cursor++ = TranslatePoint(point.x, 0.0f, point.y);
        *cursor++ = {0.0f, 1000.0f, 0.0f};
    }
    return lists;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool BuildUnits(RoadBlock &block) {
    for (std::int32_t i = 0; i < block.roadCount; ++i) {
        MazeRoad &road = block.roads[i];
        if (!(road.flags & 0x80000000U))
            continue;

        if (unitCount == static_cast<std::int32_t>(MAZE_UNIT_CAPACITY)) {
            return false;
        }

        const std::uint16_t record = static_cast<std::uint16_t>(road.flags);
        if (record >= block.controlCount) {
            return false;
        }

        const std::uint8_t model = static_cast<std::uint8_t>(AuthoredWord(block, record, 0));
        if (model >= std::size(authoredUnits)) {
            return false;
        }

        const AuthoredUnit &authored = authoredUnits[model];
        const std::size_t area = static_cast<std::size_t>(authored.areaIndex) * 112;
        if (area + 112 > unitAreas.size()) {
            return false;
        }

        MazeUnit &unit = MUZT_Data[unitCount];
        unit.road = &road;
        unit.modelIndex = model;
        unit.pointCount = ReadValue<std::int16_t>(unitAreas, area + 32);
        unit.wallCount = ReadValue<std::int16_t>(unitAreas, area + 60);
        if (unit.pointCount < 0 || unit.pointCount > 3 || unit.wallCount < 0 || unit.wallCount > 3) {
            return false;
        }
        unit.numLists = static_cast<std::int32_t>(authored.paths.size());
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(0x4000 - AuthoredWord(block, record, 24)));
        unit.pointLists = AllocateMazeUnitPaths(authored.paths);
        if (unit.numLists && !unit.pointLists) {
            Matrix4x4::Pop(1);
            return false;
        }
        for (std::size_t j = 0; j < 4; ++j) {
            const Vector2 point = ReadValue<Vector2>(unitAreas, area + j * 8);
            unit.quad[j] = TranslatePoint(point.x, 0.0f, point.y);
        }
        for (std::int32_t j = 0; j < unit.wallCount; ++j) {
            const Vector4 wall = ReadValue<Vector4>(unitAreas, area + 64 + j * 16);
            unit.walls[j][0] = TranslatePoint(wall.x, 0.0f, wall.y);
            unit.walls[j][1] = TranslatePoint(wall.z, 0.0f, wall.w);
        }
        for (std::int32_t j = 0; j < unit.pointCount; ++j) {
            const Vector2 point = ReadValue<Vector2>(unitAreas, area + 36 + j * 8);
            unit.points[j] = TranslatePoint(point.x, 0.0f, point.y);
        }
        Matrix4x4::Pop();
        road.unit = unitCount++;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InsideQuad(const MazeUnit &unit, const Vector3 &point) {
    const Vector3 *quad = unit.quad;
    const float first = (quad[1].x - quad[0].x) * (quad[0].z - point.z) - (quad[0].x - point.x) * (quad[1].z - quad[0].z);
    for (std::size_t i = 1; i < 4; ++i) {
        const Vector3 &a = quad[i];
        const Vector3 &b = quad[(i + 1) % 4];
        const float side = (b.x - a.x) * (a.z - point.z) - (b.z - a.z) * (a.x - point.x);
        if (!(side * first >= 0.0f))
            return false;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t FindUnit(const Vector3 &point) {
    for (std::int32_t i = 0; i < unitCount; ++i)
        if (InsideQuad(MUZT_Data[i], point))
            return i;
    return -1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t PathCount(const Vector3 *points) {
    std::int32_t count = 0;
    while (points[count].y < 990.0f)
        ++count;
    return count == 0 ? -1 : count;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void FindNearestUnitPoint(const Vector3 &point, const MazeUnit &unit, std::int32_t &list, std::int32_t &element) {
    float minimum = 1000000000.0f;
    list = -1;
    element = -1;
    for (std::int32_t i = 0; i < unit.numLists; ++i) {
        const Vector3 *points = unit.pointLists[i];
        const std::int32_t count = PathCount(points);
        for (std::int32_t j = 0; j < count; ++j) {
            const float x = point.x - points[j].x;
            const float y = point.y - points[j].y;
            const float z = point.z - points[j].z;
            const float distance = (y * y + x * x) + z * z;
            if (minimum > distance) {
                minimum = distance;
                list = i;
                element = j;
            }
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
MazeRoad *SeekOutsideUnit(MazeRoad *road) {
    while (road->index >= 0 && FindUnit(road->position) >= 0)
        ++road;
    return road;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
MazeRoad *SeekInsideUnit(MazeRoad *road) {
    while (road->index >= 0 && FindUnit(road->position) < 0)
        ++road;
    return road;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool AppendPoint(const Vector3 &point, std::int32_t &count) {
    if (count >= 0x3000 / static_cast<std::int32_t>(sizeof(Vector3)) - 1)
        return false;
    routeBuffer[count++] = point;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool AppendOutsideRoads(MazeRoad *road, std::int32_t &count) {
    while (road->index >= 0 && FindUnit(road->position) < 0) {
        if (!AppendPoint(road->position, count))
            return false;
        ++road;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool AppendUnitPath(const MazeUnit &unit, std::int32_t list, std::int32_t direction, std::int32_t &count) {
    if (unit.numLists <= 0)
        return true;
    if (list < 0 || list >= unit.numLists)
        return false;
    const Vector3 *points = unit.pointLists[list];
    const std::int32_t pointCount = PathCount(points);
    const std::int32_t step = direction > 0 ? 1 : -1;
    for (std::int32_t i = direction > 0 ? 0 : pointCount - 1; i >= 0 && i < pointCount; i += step) {
        if (FindUnit(points[i]) >= 0 && !AppendPoint(points[i], count))
            return false;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool CollectPartition(MazeRoad *&road, std::int32_t &count) {
    count = 0;
    const std::int32_t initialUnit = FindUnit(road->position);
    if (initialUnit >= 0) {
        std::int32_t list;
        std::int32_t element;
        FindNearestUnitPoint(SeekOutsideUnit(road)->position, MUZT_Data[initialUnit], list, element);
        if (!AppendUnitPath(MUZT_Data[initialUnit], list, 1, count))
            return false;
    }
    road = SeekOutsideUnit(road);
    if (!AppendOutsideRoads(road, count))
        return false;
    road = SeekInsideUnit(road);
    while (road->index >= 0) {
        const std::int32_t unitIndex = FindUnit(road->position);
        if (unitIndex >= 0) {
            const MazeUnit &unit = MUZT_Data[unitIndex];
            std::int32_t list;
            std::int32_t element;
            FindNearestUnitPoint(road->position, unit, list, element);
            if (unit.numLists > 1) {
                if (!AppendUnitPath(unit, list, -1, count))
                    return false;
                MazeRoad *outside = SeekOutsideUnit(road);
                if (outside->index == -1)
                    road = outside;
                routeBuffer[count] = {0.0f, -1000.0f, 0.0f};
                return true;
            }
            std::int32_t direction = 0;
            if (unit.numLists > 0)
                direction = element > PathCount(unit.pointLists[list]) / 2 ? -1 : 1;
            if (!AppendUnitPath(unit, 0, direction, count))
                return false;
            road = SeekOutsideUnit(road);
        }
        if (!AppendOutsideRoads(road, count))
            return false;
        road = SeekInsideUnit(road);
    }
    routeBuffer[count] = {0.0f, -1000.0f, 0.0f};
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool BuildPartitions() {
    for (RoadBlock &block : roadBlocks) {
        MazeRoad *road = block.roads;
        do {
            if (partitionCount == static_cast<std::int32_t>(partitions.size()))
                return false;
            std::int32_t count;
            if (!CollectPartition(road, count))
                return false;
            MazePartition &partition = partitions[partitionCount++];
            partition.count = count;
            partition.points = static_cast<Vector3 *>(Allocate(192 * static_cast<std::size_t>(count + 1), SHEN_NAME(MZNR)));
            if (!partition.points)
                return false;
            for (std::int32_t i = 0; i < count; ++i)
                partition.points[i] = routeBuffer[i];
        } while (road->index != -1);
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void BuildEndpointLinks(std::int32_t partition, std::int32_t endpoint) {
    const std::int32_t element = endpoint == 0 ? 0 : partitions[partition].count - 1;
    if (element < 0)
        return;
    const std::int32_t unitIndex = FindUnit(partitions[partition].points[element]);
    if (unitIndex < 0 || MUZT_Data[unitIndex].numLists != 3)
        return;
    const MazeUnit &unit = MUZT_Data[unitIndex];
    std::int32_t adjacentPartition[3];
    std::int32_t adjacentElement[3]{};
    std::int32_t ownList = -1;
    for (std::int32_t i = 0; i < 3; ++i) {
        adjacentPartition[i] = -1;
        mazeFindClosestTarget(&adjacentPartition[i], &adjacentElement[i], unit.pointLists[i]);
        if (adjacentPartition[i] == partition && adjacentElement[i] == element)
            ownList = i;
    }
    if (ownList < 0)
        return;
    EndpointLinks &result = links[partition][endpoint];
    for (std::int32_t i = 0; i < 2; ++i) {
        const std::int32_t next = (ownList + i + 1) % 3;
        result.neighbors[i].partition = static_cast<std::int16_t>(adjacentPartition[next]);
        result.neighbors[i].element = static_cast<std::int16_t>(adjacentElement[next]);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct RouteSearch {
    std::int32_t partition;
    std::int32_t element;
    std::int32_t targetPartition;
    std::int32_t targetElement;
    std::int32_t nextPartition;
    std::int32_t nextElement;
};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t SearchRoute(RouteSearch &search, std::int32_t endpoint) {
    if (search.partition < 0 || search.partition >= partitionCount || links[search.partition][0].visited)
        return 0;
    links[search.partition][0].visited = true;
    if (search.partition == search.targetPartition) {
        search.nextPartition = search.targetPartition;
        search.nextElement = search.targetElement;
        const std::int32_t difference = search.targetElement - search.element;
        return (difference < 0 ? -difference : difference) | 0x10000;
    }
    const std::int32_t partition = search.partition;
    const std::int32_t element = search.element;
    const EndpointLinks &neighbors = links[partition][endpoint];
    for (std::int32_t i = 1; i >= 0; --i) {
        search.partition = neighbors.neighbors[i].partition;
        search.element = neighbors.neighbors[i].element;
        const std::int32_t result = SearchRoute(search, search.element == 0);
        search.nextPartition = search.partition;
        search.nextElement = search.element;
        search.partition = partition;
        search.element = element;
        if (result)
            return (endpoint ? partitions[partition].count - element : element) + result + 1;
    }
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void MarkRoadEnds(shen_name area) {
    for (RoadBlock &block : roadBlocks) {
        for (std::int32_t i = 1; i < block.roadCount; ++i) {
            if (!(block.roads[i].flags & 0xffffU))
                continue;
            for (std::int32_t j = i; j < block.roadCount && j < i + 5; ++j)
                block.roads[j].flags |= 0x10000000U;
        }

        for (std::int32_t i = block.roadCount - 1; i > 0 && i >= block.roadCount - 5; --i)
            block.roads[i].flags |= 0x10000000U;

        for (std::int32_t i = block.roadCount - 2; i > 0; --i) {
            if (!(block.roads[i].flags & 0xffffU))
                continue;
            for (std::int32_t j = i; j > 0 && j > i - 5; --j)
                block.roads[j].flags |= 0x10000000U;
        }
    }

    if (area.name != SHEN_NAME(KMZ3) || partitionCount <= 13)
        return;
    for (std::int32_t i = 0; i < partitions[13].count; ++i) {
        const Vector3 &point = partitions[13].points[i];
        float minimum = 25600.0f;
        MazeRoad *nearest = nullptr;
        for (RoadBlock &block : roadBlocks) {
            for (std::int32_t j = 0; j < block.roadCount; ++j) {
                const float x = point.x - block.roads[j].position.x;
                const float z = point.z - block.roads[j].position.z;
                const float distance = z * z + x * x;
                if (minimum > distance) {
                    minimum = distance;
                    nearest = &block.roads[j];
                }
            }
        }
        if (nearest)
            nearest->flags |= 0x10000000U;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeAssets(const LoadedMapPackage &package, std::int32_t mode) {
    constexpr const char *mapNames[] = {"qte", "kmz1", "kmz2", "kmz3", "kmz4"};
    char filename[16];
    std::snprintf(filename, sizeof(filename), "%smap.BIN", mapNames[mode]);
    const std::span<const std::byte> map = FindTerrainEntry(package, filename);
    unitAreas = FindTerrainEntry(package, "unitarea.BIN");
    if (map.size() < 4 || unitAreas.empty()) {
        return false;
    }
    if (mode) {
        std::snprintf(filename, sizeof(filename), "%sev.BIN", mapNames[mode]);
        markerBytes = FindTerrainEntry(package, filename);
        if (markerBytes.size() < sizeof(Marker)) {
            return false;
        }
        std::size_t offset = 0;
        while (offset + sizeof(Marker) <= markerBytes.size() && ReadValue<std::int32_t>(markerBytes, offset) >= 0) {
            ++markerCount;
            offset += sizeof(Marker);
        }
        if (offset + sizeof(Marker) > markerBytes.size()) {
            return false;
        }
    }
    MUZT_Data = static_cast<MazeUnit *>(Allocate(sizeof(MazeUnit) * MAZE_UNIT_CAPACITY, MEMORY_MAZE_UNITS));
    splineBuffer = static_cast<Vector3 *>(Allocate(0x30c, SHEN_NAME(MZBF)));
    if (!MUZT_Data || !splineBuffer) {
        return false;
    }
    const std::uint16_t blockCount = ReadValue<std::uint16_t>(map, 0);
    if (blockCount == 0)
        return false;
    roadBlocks.resize(blockCount);
    std::size_t offset = 4;
    for (RoadBlock &block : roadBlocks) {
        if (offset + 4 > map.size())
            return false;
        const std::uint32_t count = ReadValue<std::uint32_t>(map, offset);
        if (count < 4 || count > 0xffffU || count > (map.size() - offset - 4) / 56)
            return false;
        block.controlCount = static_cast<std::int32_t>(count);
        block.authored = map.subspan(offset + 4, count * 56);
        offset += 4 + count * 56;
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeGeometry() {
    for (RoadBlock &block : roadBlocks) {
        const std::size_t count = static_cast<std::size_t>(block.controlCount);
        block.controlPoints = static_cast<Vector3 *>(Allocate(sizeof(Vector3) * count, SHEN_NAME(MZPT)));
        if (!block.controlPoints)
            return false;
        for (std::size_t i = 0; i < count; ++i) {
            block.controlPoints[i] = ReadValue<Vector3>(block.authored, i * 56 + 12);
            block.controlPoints[i].y += 0.005f;
        }
    }
    for (std::size_t i = 0; i < roadBlocks.size(); ++i) {
        if (!BuildRoads(roadBlocks[i], static_cast<std::int32_t>(i)) || !BuildUnits(roadBlocks[i])) {
            return false;
        }
    }
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeRoutes(shen_name area) {
    routeBuffer = static_cast<Vector3 *>(Allocate(0x3000, SHEN_NAME(MZRT)));
    if (!routeBuffer || !BuildPartitions()) {
        return false;
    }
    for (std::int32_t i = 0; i < partitionCount; ++i) {
        BuildEndpointLinks(i, 0);
        BuildEndpointLinks(i, 1);
    }
    MarkRoadEnds(area);
    return true;
}
#endif

} // namespace
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetMazeDefaults() {
    mazeCullingSteps = 13;
    mazeTerrainDrawEnabled = 1;
    mazeUnitDrawEnabled = 1;
    mazeRoadPropsEnabled = 1;
    mazeRoadDrawEnabled = 1;
    SetNaviTaskEnabled(true);
    mazeUnitDistance = 60.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
PakGlobalData *SetupRendererAndForceLoadPak(std::int32_t mode) {
    if (mode < 0 || mode > 4)
        return nullptr;
    const char *const folder = g_actNumber != 4 ? "/viewer/maze/" : "/scene/04/MAZE/";
    PakGlobalData *const package = LoadMapPackage(folder, mazeTerrainPackages[mode]);
    pakLoadFlush();
    return package;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool InitializeMazeRuntime(std::int32_t act, shen_name area) {
    if (act != 4)
        return false;
    std::int32_t mode;
    switch (area.name) {
    case SHEN_NAME(KMZ1):
        mode = 1;
        break;
    case SHEN_NAME(KMZ2):
        mode = 2;
        break;
    case SHEN_NAME(KMZ3):
        mode = 3;
        break;
    case SHEN_NAME(KMZ4):
        mode = 4;
        break;
    case SHEN_NAME(KWQA):
        mode = 0;
        break;
    default:
        return false;
    }

    // Parent module teardown calls RetireMazeRuntime before freeing use-spaces and package buffers.
    ResetMazeState(false);
    g_mazePropDrawCount = 0;
    terrainPackage = mazeTerrainPackages[mode];
    ownsTerrainPackage = FindPakByEntryName(terrainPackage) == nullptr;
    if (!SetupRendererAndForceLoadPak(mode)) {
        ResetMazeState(true);
        return false;
    }
    const LoadedMapPackage *package = nullptr;
    for (const LoadedMapPackage &loaded : GetLoadedMapPackages()) {
        std::size_t index = 0;
        while (terrainPackage[index] && std::toupper(static_cast<unsigned char>(loaded.name[index])) == terrainPackage[index])
            ++index;
        if (!terrainPackage[index] && !loaded.name[index]) {
            package = &loaded;
            break;
        }
    }
    SetMazeDefaults();
    if (!package || !InitializeMazeAssets(*package, mode) || !InitMazeModels(*package) || !InitializeMazeEnvironmentModels(*package) ||
        !InitializeMazeUnitModels(*package) || !InitializeMazeGeometry()) {
        ResetMazeState(true);
        return false;
    }

    mazeTask = HLib::EnqueueTaskWithParameter(MazeTaskCallback, 13, 2, sizeof(MazeTaskData), TASK_MAZE);
    MazeTaskData *const data = mazeTask ? static_cast<MazeTaskData *>(HLib::GetTaskParameter(mazeTask)) : nullptr;
    if (!data) {
        if (mazeTask) {
            HLib::DestroyTask(mazeTask);
            mazeTask = nullptr;
        }
        ResetMazeState(true);
        return false;
    }
    data->reserved = 0;
    data->task = mazeTask;
    if (!InitializeMazeRoutes(area) || !InitializeMazeAuxiliaryModels(*package)) {
        ResetMazeState(true);
        return false;
    }
    MAZ_fishCtrl();
    BindMazeMarkerDecorations();
    mazeHourBlend = 0.0000099999997f;
    mazeHourFrame = 2;
    mazePreviousHourFrame = 2;
    shenGetCurrentDateTime(mazeHourDate);
    if (!InitializeMazeRoadCommandCopies()) {
        ResetMazeState(true);
        return false;
    }

    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void RetireMazeRuntime() {
    ResetMazeState(true);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::span<MazeRoad *const> GetCurrentMazeObjects() {
    std::size_t count = 0;
    while (count < currentMazeObjects.size() && currentMazeObjects[count])
        ++count;
    return {currentMazeObjects.data(), count};
}

std::span<MazeRoad *const> GetCurrentMazeUnitObjects() {
    std::size_t count = 0;
    while (count < currentMazeUnitObjects.size() && currentMazeUnitObjects[count])
        ++count;
    return {currentMazeUnitObjects.data(), count};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t InitializeMazeObjectTransforms(const Vector3 *camera, float *nearestDistance) {
    currentMazeObjects = {};
    currentMazeUnitObjects = {};
    std::int32_t nearest = -1;
    float distance = 1000000.0f;
    for (std::size_t block = 0; block < roadBlocks.size(); ++block) {
        float blockDistance;
        const std::int32_t index = UpdateXFormMazeObject(roadBlocks[block].roads, *camera, blockDistance);
        if (index != -1 && distance > blockDistance) {
            distance = blockDistance;
            nearest = static_cast<std::int32_t>((block << 16) + index);
        }
    }
    *nearestDistance = distance;
    return nearest;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::span<const MazeUnit> GetMazeUnits() {
    return {MUZT_Data, static_cast<std::size_t>(unitCount)};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::span<const MazeRoad> GetMazeRoads(std::int32_t block) {
    if (block < 0 || static_cast<std::size_t>(block) >= roadBlocks.size())
        return {};
    return {roadBlocks[block].roads, static_cast<std::size_t>(roadBlocks[block].roadCount)};
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
MazeRoad *GetMazeRoad(std::int32_t block, std::int32_t index) {
    return roadBlocks[block].roads + index;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetMazeRoadPosition(std::int32_t block, std::int32_t index, Vector3 *output) {
    *output = roadBlocks[block].roads[index].position;
    return std::bit_cast<std::uint32_t>(output->z);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetMazeRoadYaw(std::int32_t block, std::int32_t index) {
    return static_cast<std::uint32_t>(roadBlocks[block].roads[index].yaw);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetMazeUnitCount() {
    return static_cast<std::uint32_t>(unitCount);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetMazeMarkerCount() {
    return static_cast<std::uint32_t>(markerCount);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void FindNearestMazeRoad(const Vector3 *position, std::int32_t *block, std::int32_t *index) {
    *block = -1;
    *index = -1;
    float nearest = 25600.0f;
    for (std::size_t b = 0; b < roadBlocks.size(); ++b) {
        const RoadBlock &roads = roadBlocks[b];
        for (std::int32_t i = 0; i < roads.roadCount; ++i) {
            const Vector3 &sample = roads.roads[i].position;
            const float x = position->x - sample.x, z = position->z - sample.z;
            const float distance = z * z + x * x;
            if (nearest > distance) {
                nearest = distance;
                *block = static_cast<std::int32_t>(b);
                *index = i;
            }
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t FindNearestMazeFirstBlockRoad(const Vector3 *position) {
    std::int32_t block, index;
    FindNearestMazeRoad(position, &block, &index);
    return block == 0 ? static_cast<std::uint32_t>(index) : 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t FindCharacterNearestMazeFirstBlockRoad(Character *character) {
    const Vector3 position = GetCharacterWorldPosition(character);
    return FindNearestMazeFirstBlockRoad(&position);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
shen_name GetMazeUnitAuthoredName(std::int32_t index) {
    const MazeUnit *const unit = index < 0 || index >= unitCount ? nullptr : MUZT_Data + index;
    return MazeUnitAuthoredName(unit->modelIndex);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool HasMazeUnitOfKind(shen_name name) {
    for (std::int32_t index = 0; index < unitCount; ++index)
        if (MazeUnitAuthoredName(MUZT_Data[index].modelIndex).name == name.name)
            return true;
    return false;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetMazeUnitDrawDistance(float distance) {
    mazeUnitDistance = distance;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetMazeTerrainDrawEnabled(std::int32_t enabled) {
    mazeTerrainDrawEnabled = enabled;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t mazeGetNumPartitions() {
    return partitionCount;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetMZNRDataForID(std::int32_t partition) {
    return partitions[partition].count;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void mazeFindClosestTarget(std::int32_t *partition, std::int32_t *element, const Vector3 *point) {
    const std::int32_t previousPartition = *partition;
    const std::int32_t previousElement = *element;
    float globalDistance = 1000000.0f;
    float localDistance = 1000000.0f;
    std::int32_t localElement = -1;
    for (std::int32_t i = 0; i < partitionCount; ++i) {
        for (std::int32_t j = 0; j < partitions[i].count; ++j) {
            const Vector3 &sample = partitions[i].points[j];
            const float x = sample.x - point->x;
            const float z = sample.z - point->z;
            const float distance = z * z + x * x;
            if (i == previousPartition && localDistance > distance) {
                localDistance = distance;
                localElement = j;
            }
            if (globalDistance > distance) {
                globalDistance = distance;
                *partition = i;
                *element = j;
            }
        }
    }
    if (previousPartition >= 0 && previousPartition != *partition && previousPartition < partitionCount && previousElement > 4 &&
        previousElement < partitions[previousPartition].count - 4 && localDistance < 16.0f) {
        *partition = previousPartition;
        *element = localElement;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void mazeSamplePartition(std::int32_t partition, std::int32_t element, Vector3 *output) {
    *output = partitions[partition].points[element];
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t mazeFindPartitionDirection(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *nextElement) {
    RouteSearch search{sourcePartition, sourceElement, targetPartition, targetElement, -1, 0};
    for (std::array<EndpointLinks, 2> &partition : links)
        for (EndpointLinks &endpoint : partition)
            endpoint.visited = false;

    const std::int32_t backward = SearchRoute(search, 0);
    *nextPartition = search.nextPartition;
    *nextElement = search.nextElement;
    for (std::array<EndpointLinks, 2> &partition : links)
        for (EndpointLinks &endpoint : partition)
            endpoint.visited = false;

    const std::int32_t forward = SearchRoute(search, 1);
    std::int32_t distance = static_cast<std::uint16_t>(backward);
    if (sourcePartition == targetPartition) {
        if (sourceElement > targetElement)
            distance = -distance;
    } else if (backward && (!forward || static_cast<std::uint16_t>(backward) <= static_cast<std::uint16_t>(forward))) {
        distance = -distance;
    } else {
        *nextPartition = search.nextPartition;
        *nextElement = search.nextElement;
        distance = static_cast<std::uint16_t>(forward);
    }
    return distance > 0 ? 1 : distance < 0 ? -1 : 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool mazeGetMarkerPosition(std::int32_t marker, Vector3 *output) {
    for (std::int32_t i = 0; i < markerCount; ++i) {
        const Marker entry = ReadValue<Marker>(markerBytes, static_cast<std::size_t>(i) * sizeof(Marker));
        if (entry.id != marker)
            continue;
        *output = partitions[entry.partition].points[entry.element];
        return true;
    }
    return false;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void GetMazeMarkerRouteCoordinates(std::int32_t marker, std::int32_t *partition, std::int32_t *element) {
    for (std::int32_t index = 0; index < markerCount; ++index) {
        const Marker entry = ReadValue<Marker>(markerBytes, static_cast<std::size_t>(index) * sizeof(Marker));
        if (entry.id != marker)
            continue;
        *partition = entry.partition;
        *element = entry.element;
        return;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int32_t FindMazeMarkerWithinRadius(const Vector3 *position, float radius) {
    std::int32_t result = -1;
    const float squaredRadius = radius * radius;
    for (std::int32_t index = 0; index < markerCount; ++index) {
        const Marker marker = ReadValue<Marker>(markerBytes, static_cast<std::size_t>(index) * sizeof(Marker));
        const Vector3 &point = partitions[marker.partition].points[marker.element];
        const float x = position->x - point.x, z = position->z - point.z;
        if (squaredRadius > z * z + x * x)
            result = marker.id;
    }
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int32_t FindCharacterMazeMarkerWithinRadius(Character *character, float radius) {
    const Vector3 position = GetCharacterWorldPosition(character);
    return FindMazeMarkerWithinRadius(&position, radius);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool IsCharacterWithinMazeMarkerRadius(Character *character, std::int32_t marker, float radius) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    Vector3 target;
    if (!mazeGetMarkerPosition(marker, &target))
        return false;
    const float dx = position.x - target.x;
    const float dz = position.z - target.z;
    return radius * radius > dz * dz + dx * dx;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool IsPositionWithinMazeMarkerRadius(const Vector3 *position, std::int32_t marker, float radius) {
    Vector3 target;
    if (!mazeGetMarkerPosition(marker, &target))
        return false;
    const float dx = position->x - target.x;
    const float dz = position->z - target.z;
    return radius * radius > dz * dz + dx * dx;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void GetMazeUnitPosition(std::int32_t index, Vector3 *output) {
    const MazeUnit *const unit = index < 0 || index >= unitCount ? nullptr : MUZT_Data + index;
    *output = unit->road->position;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t GetMazeUnitYaw(std::int32_t index) {
    const MazeUnit *const unit = index < 0 || index >= unitCount ? nullptr : MUZT_Data + index;
    const MazeRoad *const road = unit->road;
    const RoadBlock &block = roadBlocks[road->block];
    const std::uint16_t record = static_cast<std::uint16_t>(road->flags);
    (void)AuthoredWord(block, record, 0);
    return 0x4000U - static_cast<std::uint32_t>(AuthoredWord(block, record, 24));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint64_t GetCharacterMazeUnitIndex(Character *character) {
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    return static_cast<std::uint32_t>(FindUnit(position));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::uint32_t mazeGetCurrentRefCount() {
    return mazeCurrentReferenceCount;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void mazeSetCurrentRefCount(std::uint32_t count) {
    mazeCurrentReferenceCount = count;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
HLib::Task *MAZ_fishCtrl() {
    std::int32_t count = 0;
    while (mazeFishControlPath[count].x < 999.0f)
        ++count;
    fishControlCount = count;
    fishControlPoints = static_cast<Vector3 *>(Allocate(12ULL * count, MEMORY_MAZE_FISH));
    for (std::int32_t index = 0; index < count; ++index)
        fishControlPoints[index] = {mazeFishControlPath[index].x, 0.0f, mazeFishControlPath[index].y};
    fishPathSamples = static_cast<Vector3 *>(Allocate(768ULL * count, MEMORY_MAZE_FISH));
    const RoadBlock spline{.controlPoints = fishControlPoints, .controlCount = fishControlCount};
    Vector3 *cursor = fishPathSamples;
    for (std::int32_t segment = 0; segment < count - 3; ++segment) {
        for (std::int32_t index = 0; index <= 64; ++index)
            SampleControlSpline(spline, segment * 64 + index, cursor[index]);
        cursor += 64;
    }
    const Vector3 &last = cursor[-1];
    for (std::int32_t index = 0; index < 16; ++index) {
        const float fraction = static_cast<float>(index + 1) / 17.0f;
        cursor[index].x = (fishPathSamples[0].x - last.x) * fraction + last.x;
        cursor[index].z = (fishPathSamples[0].z - last.z) * fraction + last.z;
    }
    cursor[16].x = 1000.0f;
    cursor[16].z = 1000.0f;
    fishPathRingLength = count * 64 - 177;
    fishScale = 1.0f;
    fishForceYaw = false;
    fishForcedYaw = 0;
    HLib::Task *task = nullptr;
    if (g_currentMap.name == AREA_MAZE_FISH.name) {
        task = HLib::EnqueueTaskWithParameter(MAZ_fishCtrl_Callback, 13, 5, 0, TASK_FISH);
        fishTask = task;
    }
    fishEnabled = false;
    return task;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void MAZ_fishCtrl_Callback(void *) {
    if (fishEnabled)
        MAZ_fishCtrlUpdate(&fishBoxMin, &fishBoxMax, fishCount, fishSeed, fishModel);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void MAZ_fishCtrlUpdate(const Vector3 *boxMin, const Vector3 *boxMax, std::uint32_t count, std::int32_t seed, const MT7Model *fish) {
    if (!IsWorldDrawingEnabled())
        return;
    SetSharedPrngSeed(static_cast<std::uint32_t>(seed));
    std::int32_t cursor = fishPathCursor;
    if (fishLastReferenceCount != mazeGetCurrentRefCount()) {
        fishLastReferenceCount = mazeGetCurrentRefCount();
        ++cursor;
    }
    if (cursor >= fishPathRingLength)
        cursor = 0;
    fishPathCursor = cursor;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(count); ++index) {
        std::int32_t offset = static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(fishPathRingLength));
        if (fishPathCursor + offset >= fishPathRingLength)
            offset -= fishPathRingLength;
        const Vector3 &point = fishPathSamples[fishPathCursor + offset];
        const Vector3 &next = fishPathCursor + offset + 1 >= fishPathRingLength ? fishPathSamples[0] : fishPathSamples[fishPathCursor + offset + 1];
        const float intensity = IncrementSharedPrngFloat();
        const float x = (boxMax->x - boxMin->x) * intensity + boxMin->x;
        const float y = (boxMax->y - boxMin->y) * intensity + boxMin->y;
        const float z = (boxMax->z - boxMin->z) * intensity + boxMin->z;
        const float jitter = IncrementSharedPrngFloat() * 0.60000002f - 0.30000001f;
        const std::uint16_t yaw = fishForceYaw ? fishForcedYaw : FixedAtan2(point.x - next.x, point.z - next.z);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(x + point.x, y + jitter, z + point.z);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::Scale(fishScale, fishScale, fishScale);
        charaApplyScale(fish, fish ? fish->nodes.data() : nullptr);
        Matrix4x4::Pop(1);
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void SetFish(const Vector3 *boxMin, const Vector3 *boxMax, std::int32_t count, std::int32_t seed, Character *fish) {
    if (!IsWorldDrawingEnabled())
        return;
    fishBoxMin = *boxMin;
    fishBoxMax = *boxMax;
    fishCount = static_cast<std::uint32_t>(count);
    fishSeed = seed;
    fishModel = static_cast<const MT7Model *>(fish->model);
    fishEnabled = true;
    fishForceYaw = true;
    fishForcedYaw = static_cast<std::uint16_t>(FixedAtan2(boxMin->x - boxMax->x, boxMin->z - boxMax->z));
    fishScale = 3.0f;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int64_t InitializeMazeNavigationTrail() {
    float startZ;
    float endX;
    float endZ;
    switch (g_currentMap.name) {
    case SHEN_NAME(KMZ2):
        startZ = -190.0f;
        endX = -500.0f;
        endZ = -310.0f;
        break;
    case SHEN_NAME(KMZ3):
        startZ = -400.0f;
        endX = -390.0f;
        endZ = -400.0f;
        break;
    case SHEN_NAME(KMZ4):
        startZ = -700.0f;
        endX = -400.0f;
        endZ = -810.0f;
        break;
    default:
        return static_cast<std::int64_t>(g_currentMap.name);
    }
    auto *points = static_cast<Vector3 *>(HLib::AllocateTaskData(96, SHEN_NAME(MZET)));
    SetSharedPrngSeed(g_currentMap.name);
    const float spanX = endX - -500.0f;
    const float spanZ = endZ - startZ;
    for (std::int32_t index = 0; index < 8; ++index) {
        const float jitterX = IncrementSharedPrngFloat() * 4.0f - 2.0f;
        const float jitterZ = IncrementSharedPrngFloat() * 4.0f - 2.0f;
        const float fraction = static_cast<float>(index) * 0.125f;
        points[index].y = 1.0f;
        points[index].x = (spanX * fraction - 500.0f) + jitterX;
        points[index].z = (spanZ * fraction + startZ) + jitterZ;
    }
    AddNaviTrailLocations({points + 1, 7});
    return HLib::FreeTaskData(points);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void ResetMazeHourAnimation(std::int32_t frame) {
    mazeHourFrame = frame;
    mazePreviousHourFrame = frame;
    mazeHourBlend = 0.0000099999997f;
    shenGetCurrentDateTime(mazeHourDate);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Preserve the preceding authored frame while starting a blend.
void SetMazeHourAnimationFrame(std::int32_t frame) {
    mazePreviousHourFrame = mazeHourFrame;
    mazeHourFrame = frame;
    mazeHourBlend = 0.99998999f;
    shenGetCurrentDateTime(mazeHourDate);
}
#endif


} // namespace s2
} // namespace shenmue
