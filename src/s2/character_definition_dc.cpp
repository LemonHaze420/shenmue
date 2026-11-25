#include "s2/character_table_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include <cstring>
#include "s2/map_info.hpp"
#include "shenmue.h"
#include "s2/character_pose_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include <cstdio>
#include "s2/pak_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/math.hpp"
#include "s2/event_matrix_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/map_load_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/map_vertex_cache_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/prop_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/map_draw_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/mt7.hpp"
#include <cmath>
#include "s2/model_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/font.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/title_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/window.hpp"
#include "s2/area_event.hpp"
#include "s2/scenario_control_dc.hpp"
#include "common/math.hpp"
#include "common/native_sqrt.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/ui_state_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#endif
#include "s2/model_image_component_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"

namespace shenmue {
namespace s2 {

const std::uint32_t *ParseCharacterTableRecord(CharacterTableRecord *record, const char *const *tokens, const std::uint32_t *source);

std::int32_t CompareCharacterTableTokens(const char *left, const char *right);
const std::uint32_t *ApplyCharacterTableNodeRecord(CharacterNode *node, std::uint32_t *fields, const std::uint32_t *source);
void ReadMapBlockSize(const MapInfoChunk *chunk);
void LoadPROPs(std::int32_t act, shen_name area);
void ReadMapDisabledLayers(const MapInfoChunk *chunk);
void LoadMap(std::int32_t act, shen_name area, MapInfoChunk *mapInfo);
CharacterNode *GetCharacterControlSlot(std::int32_t index);
Character *AddCharacterHandler(std::int32_t type);
HLib::Task *GetCharacterHandlerTask(Character *character);
void SetDirtAngle(std::int32_t angle);
std::int32_t CompareCharacterTableTokens(const char *left, const char *right);
const std::uint32_t *ApplyCharacterTableNodeRecord(CharacterNode *node, std::uint32_t *fields, const std::uint32_t *source);
MapInfoChunk *GetCurrentDynamicMap();
FileState *g_mapInfoFile;
void ReadMapDisabledLayers(const MapInfoChunk *chunk);
void LoadMap(std::int32_t act, shen_name area, MapInfoChunk *mapInfo);
void SetDirtAngle(std::int32_t angle);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool ApplyCharacterEntryOverride(CharacterEntryOverride *state, const CharacterTableArgument *arguments, std::int32_t count, std::int32_t entry) {
#else
// @verified - d
bool ApplyCharacterEntryOverride(CharacterEntryOverride *state, const CharacterTableArgument *arguments, std::int32_t entry) {
#endif
    if (static_cast<float>(entry) != arguments[0].value.floating)
        return false;
    const std::uint32_t *cursor = arguments[1].source + 1;
    const std::uint32_t *const end = cursor + arguments[1].value.bits;
    while (cursor < end) {
        const std::uint32_t *next = ApplyCharacterTableNodeRecord(&state->node, &state->fields, cursor);
        if (cursor == next) {
            CharacterTableRecord record;
            next = ParseCharacterTableRecord(&record, characterEntryTokens, cursor);
            switch (record.tokenIndex) {
            case 0:
                state->flags = record;
                break;
            case 1:
                state->image = record;
                break;
            case 2:
                state->group = record;
                break;
            }
        }
        cursor = next;
    }
    return true;
}

// @verified - d
void ApplyCharacterTableFlags(Character *character, const CharacterTableArgument *arguments, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        if (!CompareCharacterTableTokens(arguments[index].value.string, "DISP")) {
            EV_SetCharDisplay(character, CVM_DisplayEnabled, 1);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "KILL")) {
            MarkCharacterForCleanup(character, 1);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "MAPEV")) {
            ParseMapEv(character, 1);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "NOMAPEV")) {
            SetFlagForToken(character, CHARACTER_MAP_EVENT, 0);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "NOCLIPPER")) {
            SetFlagForToken(character, CHARACTER_CLIPPER, 0);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "NOIMGMODULE")) {
            SetFlagForToken(character, CHARACTER_IMAGE, 0);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "SLEEP")) {
            SetCharacterActive(character, 0);
            HLib::SetupTimedTask(GetCharacterHandlerTask(character), 0);
        } else if (!CompareCharacterTableTokens(arguments[index].value.string, "NOTMIRRORED")) {
            SetMirrored(character, 0);
        }
    }
}

// @verified - d
void ApplyMapEventCharacterFlag(Character *character, const CharacterTableArgument *arguments, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        if (!CompareCharacterTableTokens(arguments[index].value.string, "MAPEV")) {
            AllocateMapEventComponent(character);
            ParseMapEv(character, 1);
        }
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static inline void InitializeCharacterTableNode(CharacterNode *node) {
    node->parent = 0;
    node->position.x = node->position.y = node->position.z = 0.0f;
    node->rotation.x = node->rotation.y = node->rotation.z = 0;
    node->layer = 0;
    node->collision = 0;
}

static inline void CopyCharacterNodePosition(CharacterNode *node, const CharacterNode *source) {
    Vector3 position(source->position);
    node->position = position;
}

static inline void CopyCharacterNodeRotation(CharacterNode *node, const CharacterNode *source) {
    Vector3i rotation;
    rotation.x = source->rotation.x;
    rotation.y = source->rotation.y;
    rotation.z = source->rotation.z;
    node->rotation.x = rotation.x;
    node->rotation.y = rotation.y;
    node->rotation.z = rotation.z;
}

static inline void CopyCharacterNodeLayer(CharacterNode *node, const CharacterNode *source) {
    node->layer = source->layer;
}

static inline void CopyCharacterNodeParent(CharacterNode *node, const CharacterNode *source) {
    node->parent = source->parent;
}

static inline void CopyCharacterNodeCollision(CharacterNode *node, const CharacterNode *source) {
    node->collision = source->collision;
}

// @verified - d
void ApplyCharacterDefinition(Character *character, const std::uint32_t *source, std::int32_t size, std::int32_t entry, shen_name allocationName) {
    CharacterTableRecord record;
    CharacterEntryOverride override;
    CharacterEntryOverride base;
    CharacterTableRecord *const components = reinterpret_cast<CharacterTableRecord *>(allocateInGlobalHeap(54 * sizeof(CharacterTableRecord)));
    for (std::int32_t index = 0; index < 54; ++index)
        components[index].source = 0;
    base.fields = 0;
    base.flags.source = 0;
    InitializeCharacterTableNode(&base.node);
    override.fields = 0;
    override.flags.source = 0;
    override.image.source = 0;
    override.group.source = 0;
    InitializeCharacterTableNode(&override.node);
    const std::uint32_t *cursor = source;
    const std::uint32_t *const end = cursor + size;
    while (cursor < end) {
        const std::uint32_t *next = ApplyCharacterTableNodeRecord(&base.node, &base.fields, cursor);
        if (cursor == next) {
            next = ParseCharacterTableRecord(&record, characterComponentTokens, cursor);
            if (record.tokenIndex == CC_Entry)
                ApplyCharacterEntryOverride(&override, record.arguments, record.count, entry);
            else if (static_cast<std::uint32_t>(record.tokenIndex) < 54)
                components[record.tokenIndex] = record;
        }
        cursor = next;
    }
    CharacterNode *const node = GetCharacterNode(character);
    const CharacterEntryOverride *const state = &base;
    *node = state->node;
    {
        const CharacterEntryOverride *const entryOverride = &override;
        if (entryOverride->flags.source)
            components[CC_Flags] = entryOverride->flags;
    }
    {
        const CharacterEntryOverride *const entryOverride = &override;
        if (entryOverride->image.source)
            components[CC_Group] = entryOverride->image;
    }
    {
        const CharacterEntryOverride *const entryOverride = &override;
        if (entryOverride->group.source)
            components[CC_CommonMotionWork] = entryOverride->group;
    }
    if (override.fields & CN_Position)
        CopyCharacterNodePosition(node, &override.node);
    if (override.fields & CN_Angle)
        CopyCharacterNodeRotation(node, &override.node);
    if (override.fields & CN_Layer)
        CopyCharacterNodeLayer(node, &override.node);
    if (override.fields & CN_Local)
        CopyCharacterNodeParent(node, &override.node);
    if (override.fields & CN_Collision)
        CopyCharacterNodeCollision(node, &override.node);
    const CharacterTableRecord &image = components[CC_Image];
    if (image.source) {
        SetupImageModuleHandlers(character);
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        AssignCharacterTableImage(character, control->modelManagerTask, components[CC_Image].arguments, image.count);
        IMGM_AssignMTWK(character);
    }
    if (components[CC_MotOnly].source) {
        InitializeDefaultCharacterShadow(character);
        mtwkCreateForChara(character, components[CC_CommonMotionWork].source != 0);
    } else {
        std::int32_t mode;
        InitializeDefaultCharacterShadow(character);
        charaSetDisplayEnabledFlag(character, 1);
        if (components[CC_ShadowOff].source)
            charaSetDisplayEnabledFlag(character, 0);
        if (components[CC_Shadow].source)
            scenCreateModel(character, components[CC_Shadow].arguments, components[CC_Shadow].count);
        if (!components[CC_Human].source && !components[CC_Hand].source && !components[CC_CutMvShadow].source && character->model)
            SetupModelForImageModule(character->model);
        if (components[CC_Human].source) {
            if (components[CC_Player].source)
                AttachPlayerModel(character);
            mtwkCreateForChara(character, components[CC_CommonMotionWork].source != 0);
            InitializeHumanCharacterNullsub(character);
            if (components[CC_Party].source)
                AttachPartyCompanion(character);
            if (components[CC_Fight].source) {
                mtSetupModelFigurePose(character);
                ApplyFightCharacterDefinition(character, components[CC_Fight].arguments, components[CC_Fight].count);
            }
            if (components[CC_Handcuffs].source)
                Handcuffs_Enqueue();
            if (components[CC_Step].source) {
                mode = 0;
                if (components[CC_Step].count == 2)
                    mode = static_cast<std::int32_t>(components[CC_Step].arguments[1].value.floating);
                AttachFootstepComponent(character, components[CC_Step].arguments[0].value.string, mode);
            }
            if (components[CC_Bone].source)
                InitRigType(character, components[CC_Bone].arguments[0].value.string);
            AllocateCharacterHandPose(character);
            if (components[CC_RefBoard].source)
                AttachReflectorBoardComponent(character, 1);
            else
                AttachReflectorBoardComponent(character, 2);
            if (components[CC_Player].source) {
                SetCharacterMotionEnabled(character, 1);
                imgmAssignUpdateCallback(character, mtUpdateTransparencyCallback);
                SetPluralControlFor(character, 0);
            } else {
                AttachCharacterAlphaComponent(character);
                MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                if (scenGetCharaName(control->scenarioTask, character) == CHARACTER_SHENHUA)
                    SetPluralControlFor(character, 0);
            }
            AllocateScenarioFreeTalkCharacter(character);
            character->flags |= CF_NpcLoggerTarget;
        } else if (components[CC_Object].source) {
            AllocateFIXO(character);
            if (components[CC_Trans].source)
                imgmAssignUpdateCallback(character, charaPrepareNodesForDraw);
            if (components[CC_ForceTrans].source)
                imgmAssignUpdateCallback(character, DrawPreparedVertexColours);
            if (components[CC_ProjectionTrans].source)
                AttachCharacterAlphaComponent(character);
            if (components[CC_TailLamp].source)
                AttachTailLampComponent(character);
            if (components[CC_TinyGame].source)
                AttachGameDisplayComponent(character);
            if (components[CC_RefBoard].source)
                AttachReflectorBoardComponent(character, 6);
            if (components[CC_ObjBlur].source)
                AttachObjectBlur(character, &components[CC_ObjBlur]);
            AttachFlagWindComponent(character);
        }
        BindCharacterFacialModel(character);
        AttachFacialController(character);
        mode = 0;
        if (components[CC_Cloth].source && components[CC_Cloth].count > 0) {
            mode = static_cast<std::uint8_t>(static_cast<std::int32_t>(components[CC_Cloth].arguments[0].value.floating));
            if (!CompareCharacterTableTokens(components[CC_Cloth].arguments[0].value.string, "OFF"))
                mode = -1;
        }
        if (mode >= 0)
            mtAssignClothFlags(character, static_cast<std::uint8_t>(mode));
        mtAssignOsage(character);
        if (components[CC_Clock].source) {
            if (components[CC_Clock].count > 0)
                SetCharacterClockModeNullsub(character, static_cast<std::int16_t>(static_cast<std::int32_t>(components[CC_Clock].arguments[0].value.floating)));
            else
                SetCharacterClockModeNullsub(character, 0);
        }
        if (!components[CC_ColiOff].source) {
            AllocateCCM(character);
            ApplyCharacterTableCollisionFlags(character, &components[CC_Coli]);
        }
        bool defaultHands = false;
        if (components[CC_Human].source && components[CC_Player].source && !components[CC_Hand].source) {
            char leftHand[12], rightHand[12];
            char *const left = leftHand;
            std::memcpy(left, "RYO_TL.MT7", 11);
            char *const right = rightHand;
            std::memcpy(right, "RYO_TR.MT7", 11);
            AttachCharacterHandComponents(character, left, right, 0);
            SetLowQualityHands(character);
            const shen_name map = g_currentMap;
            if (g_actNumber == 3 && static_cast<std::int8_t>(map) != 'Q') {
                SetCharacterHandRegionEnabled(AllocateHNDL(character), 1, 0, 1);
                SetCharacterHandRegionEnabled(AllocateHNDR(character), 1, 0, 1);
            }
            defaultHands = true;
        }
        if (!defaultHands && components[CC_Hand].source) {
            const std::int32_t flags = components[CC_Hand].count == 3 ? static_cast<std::int32_t>(components[CC_Hand].arguments[2].value.floating) : 0;
            AttachCharacterHandComponents(character, components[CC_Hand].arguments[0].value.string, components[CC_Hand].arguments[1].value.string, flags);
            if (components[CC_Human].source && components[CC_Player].source) {
                SetLowQualityHands(character);
                if (g_actNumber == 3) {
                    SetCharacterHandRegionEnabled(AllocateHNDL(character), 1, 0, 1);
                    SetCharacterHandRegionEnabled(AllocateHNDR(character), 1, 0, 1);
                }
            }
        }
        if (components[CC_Range].source)
            SetCharacterRadius(character, components[CC_Range].arguments[0].value.floating);
        if (components[CC_TinyMot].source)
            AllocateTMNM(character);
    }
    if (components[CC_MotClip].source) {
        if (!CompareCharacterTableTokens(components[CC_MotClip].arguments[0].value.string, "NONE"))
            SetCharacterMotionClipMode(character, MCM_None);
        else if (!CompareCharacterTableTokens(components[CC_MotClip].arguments[0].value.string, "LOW"))
            SetCharacterMotionClipMode(character, MCM_Low);
        else if (!CompareCharacterTableTokens(components[CC_MotClip].arguments[0].value.string, "HIGH"))
            SetCharacterMotionClipMode(character, MCM_High);
        else
            SetCharacterMotionClipMode(character, MCM_Default);
    }
    if (components[CC_Knife].source)
        AllocateKnifeComponent(character);
    if (components[CC_Player].source) {
        AllocateMapEventComponent(character);
        ParseMapEv(character, 1);
    } else if (components[CC_Flags].source) {
        ApplyMapEventCharacterFlag(character, components[CC_Flags].arguments, components[CC_Flags].count);
    }
    if (components[CC_Flags].source)
        ApplyCharacterTableFlags(character, components[CC_Flags].arguments, components[CC_Flags].count);
    const CharacterTableRecord &group = components[CC_Group];
    if (group.source) {
        switch (group.arguments[0].type) {
        case CharacterTableArgumentType_Integer:
            SetCharacterModelName(character, shen_name(components[CC_Group].arguments[0].value.bits));
            break;
        case CharacterTableArgumentType_String: {
            union {
                std::uint32_t name;
                char characters[4];
            } name;
            const char *const text = components[CC_Group].arguments[0].value.string;
            name.characters[0] = text[0];
            name.characters[1] = text[1];
            name.characters[2] = text[2];
            name.characters[3] = text[3];
            SetCharacterModelName(character, shen_name(name.name));
            break;
        }
        }
    } else {
        SetCharacterModelName(character, allocationName);
    }
    SetTaskHeapData(components);
}
#else
// @verified - d
void ApplyCharacterDefinition(Character *character, const std::uint32_t *source, std::int32_t size, std::int32_t entry, shen_name allocationName) {
    CharacterTableRecord *const components = reinterpret_cast<CharacterTableRecord *>(allocateInGlobalHeap(54 * sizeof(CharacterTableRecord)));
    for (std::int32_t index = 0; index < 54; ++index)
        components[index].source = 0;
    CharacterNode node = {};
    std::uint32_t nodeFields = 0;
    CharacterEntryOverride override;
    override.fields = 0;
    override.flags.source = 0;
    override.image.source = 0;
    override.group.source = 0;
    override.node = __typeof__(override.node)();
    const std::uint32_t *const end = source + size;
    while (source < end) {
        const std::uint32_t *next = ApplyCharacterTableNodeRecord(&node, &nodeFields, source);
        if (source == next) {
            CharacterTableRecord record;
            next = ParseCharacterTableRecord(&record, characterComponentTokens, source);
            if (record.tokenIndex == CC_Entry)
                ApplyCharacterEntryOverride(&override, record.arguments, entry);
            else if (static_cast<std::uint32_t>(record.tokenIndex) < 54)
                components[record.tokenIndex] = record;
        }
        source = next;
    }
    character->node = node;
    if (override.flags.source)
        components[CC_Flags] = override.flags;
    if (override.image.source)
        components[CC_Image] = override.image;
    if (override.group.source)
        components[CC_Group] = override.group;
    if (override.fields & CN_Position)
        character->node.position = override.node.position;
    if (override.fields & CN_Angle)
        character->node.rotation = override.node.rotation;
    if (override.fields & CN_Layer)
        character->node.layer = override.node.layer;
    if (override.fields & CN_Local)
        character->node.parent = override.node.parent;
    if (override.fields & CN_Collision)
        character->node.collision = override.node.collision;
    const CharacterTableRecord &image = components[CC_Image];
    if (image.source) {
        SetupImageModuleHandlers(character);
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        AssignCharacterTableImage(character, control->modelManagerTask, image.arguments, image.count);
        IMGM_AssignMTWK(character);
    }
    if (components[CC_MotOnly].source) {
        InitializeDefaultCharacterShadow(character);
        mtwkCreateForChara(character, components[CC_CommonMotionWork].source != 0);
    } else {
        InitializeDefaultCharacterShadow(character);
        charaSetDisplayEnabledFlag(character, 1);
        if (components[CC_ShadowOff].source)
            charaSetDisplayEnabledFlag(character, 0);
        if (components[CC_Shadow].source)
            scenCreateModel(character, components[CC_Shadow].arguments, components[CC_Shadow].count);
        const bool human = components[CC_Human].source != 0;
        const bool player = components[CC_Player].source != 0;
        if (!human && !components[CC_Hand].source && !components[CC_CutMvShadow].source && character->model)
            SetupModelForImageModule(character->model);
        if (human) {
            if (player)
                AttachPlayerModel(character);
            mtwkCreateForChara(character, components[CC_CommonMotionWork].source != 0);
            InitializeHumanCharacterNullsub(character);
            if (components[CC_Party].source)
                AttachPartyCompanion(character);
            if (components[CC_Fight].source) {
                mtSetupModelFigurePose(character);
                ApplyFightCharacterDefinition(character, components[CC_Fight].arguments, components[CC_Fight].count);
            }
            if (components[CC_Handcuffs].source)
                Handcuffs_Enqueue();
            if (components[CC_Step].source) {
                std::int32_t mode = 0;
                if (components[CC_Step].count == 2)
                    mode = static_cast<std::int32_t>(components[CC_Step].arguments[1].value.floating);
                AttachFootstepComponent(character, components[CC_Step].arguments[0].value.string, mode);
            }
            if (components[CC_Bone].source)
                InitRigType(character, components[CC_Bone].arguments[0].value.string);
            AllocateCharacterHandPose(character);
            AttachReflectorBoardComponent(character, components[CC_RefBoard].source ? 1 : 2);
            if (player) {
                SetCharacterMotionEnabled(character, 1);
                imgmAssignUpdateCallback(character, mtUpdateTransparencyCallback);
                SetPluralControlFor(character, 0);
            } else {
                AttachCharacterAlphaComponent(character);
                MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                if (scenGetCharaName(control->scenarioTask, character) == CHARACTER_SHENHUA)
                    SetPluralControlFor(character, 0);
            }
            AllocateScenarioFreeTalkCharacter(character);
            character->flags |= CF_NpcLoggerTarget;
        } else if (components[CC_Object].source) {
            AllocateFIXO(character);
            if (components[CC_Trans].source)
                imgmAssignUpdateCallback(character, charaPrepareNodesForDraw);
            if (components[CC_ForceTrans].source)
                imgmAssignUpdateCallback(character, DrawPreparedVertexColours);
            if (components[CC_ProjectionTrans].source)
                AttachCharacterAlphaComponent(character);
            if (components[CC_TailLamp].source)
                AttachTailLampComponent(character);
            if (components[CC_TinyGame].source)
                AttachGameDisplayComponent(character);
            if (components[CC_RefBoard].source)
                AttachReflectorBoardComponent(character, 6);
            if (components[CC_ObjBlur].source)
                AttachObjectBlur(character, &components[CC_ObjBlur]);
            AttachFlagWindComponent(character);
        }
        BindCharacterFacialModel(character);
        AttachFacialController(character);
        std::int32_t clothMode = 0;
        if (components[CC_Cloth].source && components[CC_Cloth].count > 0) {
            const float value = components[CC_Cloth].arguments[0].value.floating;
            std::int32_t integer;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            integer = TruncateToInt32(value);
#else
            asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(integer) : "f"(value) : "fpul");
#endif
            clothMode = static_cast<std::uint8_t>(integer);
            if (!CompareCharacterTableTokens(components[CC_Cloth].arguments[0].value.string, "OFF"))
                clothMode = -1;
        }
        if (clothMode >= 0)
            mtAssignClothFlags(character, static_cast<std::uint8_t>(clothMode));
        mtAssignOsage(character);
        if (components[CC_Clock].source) {
            std::int16_t mode = 0;
            if (components[CC_Clock].count > 0)
                mode = static_cast<std::int16_t>(static_cast<std::int32_t>(components[CC_Clock].arguments[0].value.floating));
            SetCharacterClockModeNullsub(character, mode);
        }
        if (!components[CC_ColiOff].source) {
            AllocateCCM(character);
            ApplyCharacterTableCollisionFlags(character, &components[CC_Coli]);
        }
        bool defaultHands = false;
        if (human && player && !components[CC_Hand].source) {
            char leftHand[12], rightHand[12];
            std::memcpy(leftHand, "RYO_TL.MT7", 11);
            std::memcpy(rightHand, "RYO_TR.MT7", 11);
            AttachCharacterHandComponents(character, leftHand, rightHand, 0);
            SetLowQualityHands(character);
            if (g_actNumber == 3 && static_cast<std::int8_t>(g_currentMap) != 'Q') {
                SetCharacterHandRegionEnabled(AllocateHNDL(character), 1, 0, 1);
                SetCharacterHandRegionEnabled(AllocateHNDR(character), 1, 0, 1);
            }
            defaultHands = true;
        }
        if (!defaultHands && components[CC_Hand].source) {
            const CharacterTableRecord &hand = components[CC_Hand];
            const std::int32_t flags = hand.count == 3 ? static_cast<std::int32_t>(hand.arguments[2].value.floating) : 0;
            AttachCharacterHandComponents(character, hand.arguments[0].value.string, hand.arguments[1].value.string, flags);
            if (human && player) {
                SetLowQualityHands(character);
                if (g_actNumber == 3) {
                    SetCharacterHandRegionEnabled(AllocateHNDL(character), 1, 0, 1);
                    SetCharacterHandRegionEnabled(AllocateHNDR(character), 1, 0, 1);
                }
            }
        }
        if (components[CC_Range].source)
            SetCharacterRadius(character, components[CC_Range].arguments[0].value.floating);
        if (components[CC_TinyMot].source)
            AllocateTMNM(character);
    }
    if (components[CC_MotClip].source) {
        const char *const mode = components[CC_MotClip].arguments[0].value.string;
        SetCharacterMotionClipMode(character,
            !CompareCharacterTableTokens(mode, "NONE")     ? MCM_None :
                !CompareCharacterTableTokens(mode, "LOW")  ? MCM_Low :
                !CompareCharacterTableTokens(mode, "HIGH") ? MCM_High :
                                                             MCM_Default);
    }
    if (components[CC_Knife].source)
        AllocateKnifeComponent(character);
    if (components[CC_Player].source) {
        AllocateMapEventComponent(character);
        ParseMapEv(character, 1);
    } else if (components[CC_Flags].source) {
        ApplyMapEventCharacterFlag(character, components[CC_Flags].arguments, components[CC_Flags].count);
    }
    if (components[CC_Flags].source)
        ApplyCharacterTableFlags(character, components[CC_Flags].arguments, components[CC_Flags].count);
    const CharacterTableRecord &group = components[CC_Group];
    if (!group.source) {
        SetCharacterModelName(character, allocationName);
    } else if (group.arguments[0].type == CharacterTableArgumentType_Integer) {
        SetCharacterModelName(character, shen_name(group.arguments[0].value.bits));
    } else if (group.arguments[0].type == CharacterTableArgumentType_String) {
        std::uint32_t name;
        std::memcpy(&name, group.arguments[0].value.string, sizeof(name));
        SetCharacterModelName(character, shen_name(name));
    }
    SetTaskHeapData(components);
}
#endif

// @verified - d
void InitializeScenarioCharacter(const CharacterTableArgument *arguments, std::int32_t count, std::int32_t entry, shen_name allocationName) {
    bool named = false;
    std::int32_t type = 2;
    Character *character = 0;
    shen_name name;
    for (std::int32_t index = 0; index < count; ++index) {
        switch (arguments->type) {
        case CharacterTableArgumentType_Integer:
            named = true;
            name = shen_name(arguments->value.bits);
            break;
        case CharacterTableArgumentType_Float:
            type = static_cast<std::int32_t>(arguments->value.floating);
            break;
        case CharacterTableArgumentType_String:
            if (!CompareCharacterTableTokens(arguments->value.string, "HIGH"))
                type = 1;
            else if (!CompareCharacterTableTokens(arguments->value.string, "LOW"))
                type = 3;
            break;
        case CharacterTableArgumentType_Records:
            if (named) {
                const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                character = scenFindChara(control->scenarioTask, name);
            }
            if (!character) {
                character = AddCharacterHandler(type);
                if (named) {
                    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                    CreateScenarioCharacterId(control->scenarioTask, name, character);
                }
            }
            ApplyCharacterDefinition(character, arguments->source + 1, arguments->value.integer, entry, allocationName);
            break;
        }
        ++arguments;
    }
    if (!character) {
        character = AddCharacterHandler(type);
        if (named) {
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            CreateScenarioCharacterId(control->scenarioTask, name, character);
        }
    }
}

// @verified - d
void ApplyCharacterTableControl(const CharacterTableArgument *arguments) {
    CharacterNode *const node = GetCharacterControlSlot(static_cast<std::int32_t>(arguments[0].value.floating));
    const std::uint32_t *cursor = arguments[1].source + 1;
    const std::uint32_t *const end = cursor + arguments[1].value.bits;
    std::uint32_t fields;
    while (cursor < end)
        cursor = ApplyCharacterTableNodeRecord(node, &fields, cursor);
}

// @verified - d
void LoadCharacters(const void *data, std::int32_t entry, shen_name allocationName) {
    const std::uint32_t *const table = static_cast<const std::uint32_t *>(data);
    const std::uint32_t *const first = table + 2;
    const std::uint32_t *const end = reinterpret_cast<const std::uint32_t *>(static_cast<const std::byte *>(data) + table[1]);
    CharacterTableRecord record;
    const std::uint32_t *cursor;
    if (!GetPakLoadParams()) {
        BeginModelImageFileReads();
        for (cursor = first; cursor < end;) {
            cursor = ParseCharacterTableRecord(&record, characterTableTokens, cursor);
            switch (static_cast<CharacterTableCommand>(record.tokenIndex)) {
            case CharacterTableCommand_Character:
                break;
            case CharacterTableCommand_DefImage: {
                const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                AssignModelImageDefinition(control->modelManagerTask, record.arguments, record.count, entry);
                break;
            }
            case CharacterTableCommand_LocalPos:
                break;
            case CharacterTableCommand_PreLoad:
                break;
            }
        }
        FinishModelImageFileReads();
    }
    for (cursor = first; cursor < end;) {
        cursor = ParseCharacterTableRecord(&record, characterTableTokens, cursor);
        switch (static_cast<CharacterTableCommand>(record.tokenIndex)) {
        case CharacterTableCommand_Character:
            InitializeScenarioCharacter(record.arguments, record.count, entry, allocationName);
            break;
        case CharacterTableCommand_DefImage:
            if (GetPakLoadParams()) {
                const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                AssignModelImageDefinition(control->modelManagerTask, record.arguments, record.count, entry);
            }
            break;
        case CharacterTableCommand_LocalPos:
            reinterpret_cast<void (*)(const CharacterTableArgument *, std::int32_t)>(ApplyCharacterTableControl)(record.arguments, record.count);
            break;
        case CharacterTableCommand_PreLoad:
            break;
        }
    }
}

// @verified - d
void *SceneMapinfoReader(std::int32_t actNumber, shen_name areaName) {
    char directory[64];
    FileState **file;
    do {
        const std::int32_t name = static_cast<std::int32_t>(areaName);
        std::sprintf(directory, "scene/%02d/%c%c%c%c", actNumber, static_cast<std::int8_t>(name),
            static_cast<std::int8_t>(static_cast<std::uint32_t>(name) >> 8), static_cast<std::int8_t>((name << 8) >> 24), static_cast<std::int8_t>(name >> 24));
        FileState *const nextFile = Archive_GetNextUnusedFileState();
        file = &g_mapInfoFile;
        *file = nextFile;
        OpenFile(*file, directory, "MAPINFO.BIN");
    } while (0);
    const std::uint32_t size = GetFileSize(*file);
    void *const memory = HLib::AllocateTaskData(size, MAP_INFO_STORAGE);
    CopyFileToHLibMemory(*file, memory, size);
    return memory;
}

// @verified - d
void MAPINFORead(void *mapInfo) {
    FileStatAndProcessIfExists(g_mapInfoFile);
    CloseFile(g_mapInfoFile);
    FindNativeChunk("END", static_cast<MapInfoChunk *>(mapInfo))->nextByteOffset = 0;
}

// @verified - d
void Mapinfo_MAP_Reader(std::int32_t act, shen_name area, void *mapInfo) {
    LoadPROPs(act, area);
    MapInfoChunk *const first = static_cast<MapInfoChunk *>(mapInfo);
    ReadMapBlockSize(FindNativeChunk("BLKS", first));
    LoadMap(act, area, first);
    ReadMapDisabledLayers(FindNativeChunk("MAPD", first));
    InitializeAirPresentation(mapInfo);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void EventBase_Wrapper(void *mapInfo) {
    MapInfoChunk *chunk = FindNativeChunk("DIRT", static_cast<MapInfoChunk *>(mapInfo));
    if (chunk) {
        const std::int32_t degrees = *reinterpret_cast<const std::int32_t *>(chunk + 1);
        const std::int32_t scaled = static_cast<std::int32_t>(static_cast<std::uint32_t>(degrees) * 8192U);
        SetDirtAngle(static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(scaled) / 45.0f)));
    }
    SetNewScenarioTask(reinterpret_cast<const std::byte *>(FindNativeChunk("SCNF", static_cast<MapInfoChunk *>(mapInfo))));
    QueueAreaEventEntry();
    chunk = FindNativeChunk("REGD", static_cast<MapInfoChunk *>(mapInfo));
    if (chunk) {
        const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(mapInfo) - 8U;
        HLib::ShrinkTaskData(mapInfo, reinterpret_cast<std::uintptr_t>(chunk) - base);
        do {
            chunk->name = SHEN_NAME(END);
        } while (0);
        chunk->nextByteOffset = 0;
    }
}
#endif

// @verified - d
const std::uint32_t *ParseCharacterTableRecord(CharacterTableRecord *record, const char *const *tokens, const std::uint32_t *source) {
    record->source = source;
    const char *const token = reinterpret_cast<const char *>(source) + *source;
    ++source;
    std::uint32_t argumentMask = *source++;
    std::int32_t index = 0;
    while (*tokens && CompareCharacterTableTokens(token, *tokens)) {
        ++tokens;
        ++index;
    }
    record->tokenIndex = index;
    CharacterTableArgument *argument = record->arguments;
    index = 0;
    for (; argumentMask; argumentMask >>= 3) {
        argument->source = source;
        switch (argumentMask & 7U) {
        case 1:
            argument->type = CharacterTableArgumentType_Float;
            argument->value.floating = *reinterpret_cast<const float *>(source++);
            break;
        case 2:
            argument->type = CharacterTableArgumentType_Integer;
            argument->value.bits = *source++;
            break;
        case 3:
            argument->type = CharacterTableArgumentType_String;
            argument->value.string = reinterpret_cast<const char *>(source);
            argument->value.string += *source;
            ++source;
            break;
        case 4:
            argument->type = CharacterTableArgumentType_Records;
            argument->value.bits = *source++;
            source += argument->value.bits;
            break;
        }
        ++argument;
        ++index;
    }
    record->count = index;
    return source;
}

MapInfoChunk *GetCurrentEventCameraChunk() {
    return FindNativeChunk("ECAM", GetCurrentMapInfo());
}

// @verified - d
const std::byte *dynaRead(const std::uint8_t name[4]) {
    const std::byte *const package = FindPackageDynamicMap(name);
    if (package)
        return package;
    MapInfoChunk *first = GetCurrentDynamicMap();
    ++first;
    return reinterpret_cast<const std::byte *>(FindNativeChunk(reinterpret_cast<const char *>(name), first));
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MapInfoChunk *GetCurrentSceneExtensionChunk() {
    return FindNativeChunk("SCEX", GetCurrentMapInfo());
}

MapInfoChunk *GetCurrentCollisionChunk() {
    return FindNativeChunk("COLS", GetCurrentMapInfo());
}

float RoundToTenUnitStep(float value, float unit) {
    volatile float step = unit * 10.0f;
    if (!(value > 0.0f))
        value -= unit * 5.0f;
    else
        value += unit * 5.0f;
    return dc::sdk::TruncateFloat(value / step) * step;
}
#endif

// @verified - d
void InverseTransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation) {
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(transform);
    matrix_stack::InvertMatrix(SHENMUE_CHARACTER_POSE_NULL);
    Matrix4x4::preTranslate(position->x, position->y, position->z);
    Matrix4x4::SetViewMatrixFromEuler(rotation->x, rotation->y, rotation->z);
    Matrix4x4 composed;
    matrix_stack::GetMatrix(&composed);
    matrix_stack::GetTranslation(position);
    PopCurrentMatrix(1);
    ExtractMatrixEulerAnglesFixed(&composed, rotation);
}

// @verified - d
std::int32_t CalculateCircleIntersections(const Vector2 *first, const Vector2 *second, Vector2 *intersections, float firstRadius, float secondRadius) {
    Vector3 local;
    Vector3 point;
    Vector3 delta;
    delta.x = second->x - first->x;
    delta.y = 0.0f;
    delta.z = second->y - first->y;
    float distance;
    distance = NativeSqrt(delta.x * delta.x + delta.z * delta.z);
    if (distance == 0.0f)
        return 0;
    float along = distance + (firstRadius * firstRadius - secondRadius * secondRadius) / distance;
    along *= 0.5f;
    float heightSquared = firstRadius;
    heightSquared *= firstRadius;
    heightSquared -= along * along;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(first->x, 0.0f, first->y);
    matrix_stack::RotateYSinCos(-delta.z / distance, delta.x / distance);
    std::int32_t count;
    if (heightSquared > 0.0f) {
        count = 2;
        local.x = along;
        local.y = 0.0f;
        local.z = NativeSqrt(heightSquared);
        Matrix4x4::TransformPoint(local, point);
        intersections[0].x = point.x;
        intersections[0].y = point.z;
        local.z = -local.z;
        Matrix4x4::TransformPoint(local, point);
        intersections[1].x = point.x;
        intersections[1].y = point.z;
    } else if (heightSquared < 0.0f) {
        count = 0;
    } else {
        count = 1;
        local.x = along;
        local.y = 0.0f;
        local.z = 0.0f;
        Matrix4x4::TransformPoint(local, point);
        intersections[0].x = point.x;
        intersections[0].y = point.z;
    }
    PopCurrentMatrix(1);
    return count;
}

} // namespace s2
} // namespace shenmue

#endif
