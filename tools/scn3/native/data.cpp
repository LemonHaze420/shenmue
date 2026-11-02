#include "scn3/native/data.hpp"
#include "s1/task/scen/internal.hpp"
#include "s1/face.hpp"
#include "s1/date_time.hpp"
#include "scn3/native/fields.hpp"

#include <cstddef>
#include <deque>

namespace shenmue::scenario {
namespace {

#define SCENARIO_FIELD(record, member) scenarioNativeField<decltype(s1::record::member)>(#member, offsetof(s1::record, member))
#define SCENARIO_OFFSET(record, member, pointee) scenarioNativeOffset<decltype(s1::record::member)>(#member, offsetof(s1::record, member), pointee)

constexpr ScenarioDataField initFields[] = {SCENARIO_OFFSET(DoorSystemInitParams, doorEntriesOff, "DSWKDoorEntry"),
    SCENARIO_OFFSET(DoorSystemInitParams, doorIdLookupChainsOff, "u32"), SCENARIO_OFFSET(DoorSystemInitParams, resourceNameTableOff, "DoorResourceDescriptor"),
    SCENARIO_OFFSET(DoorSystemInitParams, mapObjectRecordsOff, "MapObjectRecord"), SCENARIO_OFFSET(DoorSystemInitParams, systemStateOff, "DoorSlotState"),
    SCENARIO_OFFSET(DoorSystemInitParams, slotStatesOff, "i32"), SCENARIO_OFFSET(DoorSystemInitParams, candidateDoorEntryIndicesOff, "i32"),
    SCENARIO_OFFSET(DoorSystemInitParams, ownerActorArrayOff, "u32"), SCENARIO_OFFSET(DoorSystemInitParams, progressArrayOff, "i32"),
    SCENARIO_FIELD(DoorSystemInitParams, actorLookupKey), SCENARIO_FIELD(DoorSystemInitParams, maxCandidates),
    SCENARIO_OFFSET(DoorSystemInitParams, auxTable0Off, "DoorMotionSelectorBlock"),
    SCENARIO_OFFSET(DoorSystemInitParams, auxTable1Off, "ScnDoorMotionDescriptor"), SCENARIO_OFFSET(DoorSystemInitParams, doorActorHandlesOff, "u32")};

constexpr ScenarioDataField entryFields[] = {SCENARIO_FIELD(DSWKDoorEntry, slot), SCENARIO_FIELD(DSWKDoorEntry, flags),
    SCENARIO_FIELD(DSWKDoorEntry, ctrl_mask_pair), SCENARIO_FIELD(DSWKDoorEntry, coarse_metric), SCENARIO_FIELD(DSWKDoorEntry, selection_metric),
    SCENARIO_FIELD(DSWKDoorEntry, view_ang_delta), SCENARIO_FIELD(DSWKDoorEntry, group), SCENARIO_FIELD(DSWKDoorEntry, global1),
    SCENARIO_FIELD(DSWKDoorEntry, global2), SCENARIO_FIELD(DSWKDoorEntry, global3), SCENARIO_FIELD(DSWKDoorEntry, sndCommandId),
    SCENARIO_FIELD(DSWKDoorEntry, global5), SCENARIO_FIELD(DSWKDoorEntry, global6)};

constexpr ScenarioDataField slotFields[] = {SCENARIO_FIELD(DoorSlotState, mode), SCENARIO_FIELD(DoorSlotState, id), SCENARIO_FIELD(DoorSlotState, entry_idx)};

constexpr ScenarioDataField resourceFields[] = {SCENARIO_OFFSET(DoorResourceDescriptor, modelNameOff, "cstr"),
    SCENARIO_FIELD(DoorResourceDescriptor, behaviorId), SCENARIO_FIELD(DoorResourceDescriptor, openSfxId), SCENARIO_FIELD(DoorResourceDescriptor, closeSfxId)};

constexpr ScenarioDataField mapObjectFields[] = {SCENARIO_FIELD(MapObjectRecord, kind), SCENARIO_FIELD(MapObjectRecord, resourceIndex),
    SCENARIO_FIELD(MapObjectRecord, scale), SCENARIO_FIELD(MapObjectRecord, pos), SCENARIO_FIELD(MapObjectRecord, yawFixed16)};

constexpr ScenarioDataField selectorFields[] = {scenarioNativeBytes<decltype(s1::DoorMotionSelectorBlock::motion_desc_index)>(
    "motion_desc_index", offsetof(s1::DoorMotionSelectorBlock, motion_desc_index))};

constexpr ScenarioDataField motionFields[] = {SCENARIO_FIELD(ScnDoorMotionDescriptor, motion_id), SCENARIO_FIELD(ScnDoorMotionDescriptor, frame_open_sfx_begin),
    SCENARIO_FIELD(ScnDoorMotionDescriptor, frame_phase1_end), SCENARIO_FIELD(ScnDoorMotionDescriptor, frame_close_sfx_begin),
    SCENARIO_FIELD(ScnDoorMotionDescriptor, frame_motion_end), SCENARIO_FIELD(ScnDoorMotionDescriptor, approach_off_x),
    SCENARIO_FIELD(ScnDoorMotionDescriptor, approach_off_z), SCENARIO_FIELD(ScnDoorMotionDescriptor, npc_tail_end_frame),
    SCENARIO_FIELD(ScnDoorMotionDescriptor, npc_tail_delta_per_frame)};

#undef SCENARIO_FIELD
#undef SCENARIO_OFFSET

static_assert(sizeof(s1::DoorSystemInitParams) == 0x38 && sizeof(s1::DSWKDoorEntry) == 0x34 && sizeof(s1::DoorSlotState) == 0x0C);
static_assert(sizeof(s1::DoorResourceDescriptor) == 0x10 && sizeof(s1::MapObjectRecord) == 0x24);
static_assert(sizeof(s1::DoorMotionSelectorBlock) == 0x28 && sizeof(s1::ScnDoorMotionDescriptor) == 0x24);

constexpr ScenarioDataField vector3Fields[] = {
    scenarioNativeField<decltype(Vector3::x)>("x", offsetof(Vector3, x)),
    scenarioNativeField<decltype(Vector3::y)>("y", offsetof(Vector3, y)),
    scenarioNativeField<decltype(Vector3::z)>("z", offsetof(Vector3, z)),
};
constexpr ScenarioDataField vector3iFields[] = {
    scenarioNativeField<decltype(Vector3i::x)>("x", offsetof(Vector3i, x)),
    scenarioNativeField<decltype(Vector3i::y)>("y", offsetof(Vector3i, y)),
    scenarioNativeField<decltype(Vector3i::z)>("z", offsetof(Vector3i, z)),
};
constexpr ScenarioDataField vector4Fields[] = {
    scenarioNativeField<decltype(Vector4::x)>("x", offsetof(Vector4, x)),
    scenarioNativeField<decltype(Vector4::y)>("y", offsetof(Vector4, y)),
    scenarioNativeField<decltype(Vector4::z)>("z", offsetof(Vector4, z)),
    scenarioNativeField<decltype(Vector4::w)>("w", offsetof(Vector4, w)),
};
constexpr ScenarioDataField lipCueFields[] = {
    scenarioNativeField<decltype(s1::FaceLipCue::viseme)>("viseme", offsetof(s1::FaceLipCue, viseme)),
    scenarioNativeField<decltype(s1::FaceLipCue::field02)>("field02", offsetof(s1::FaceLipCue, field02)),
    scenarioNativeField<decltype(s1::FaceLipCue::duration)>("duration", offsetof(s1::FaceLipCue, duration)),
    scenarioNativeField<decltype(s1::FaceLipCue::field06)>("field06", offsetof(s1::FaceLipCue, field06)),
};
constexpr ScenarioDataField dateTimeFields[] = {
    scenarioNativeField<decltype(s1::GameDateTime::year)>("year", offsetof(s1::GameDateTime, year)),
    scenarioNativeField<decltype(s1::GameDateTime::month)>("month", offsetof(s1::GameDateTime, month)),
    scenarioNativeField<decltype(s1::GameDateTime::day)>("day", offsetof(s1::GameDateTime, day)),
    scenarioNativeField<decltype(s1::GameDateTime::dayOfWeek)>("dayOfWeek", offsetof(s1::GameDateTime, dayOfWeek)),
    scenarioNativeField<decltype(s1::GameDateTime::hours)>("hours", offsetof(s1::GameDateTime, hours)),
    scenarioNativeField<decltype(s1::GameDateTime::minutes)>("minutes", offsetof(s1::GameDateTime, minutes)),
    scenarioNativeField<decltype(s1::GameDateTime::seconds)>("seconds", offsetof(s1::GameDateTime, seconds)),
    scenarioNativeField<decltype(s1::GameDateTime::reserved)>("reserved", offsetof(s1::GameDateTime, reserved)),
};
constexpr ScenarioDataField fogColourFields[] = {
    scenarioNativeField<decltype(s1::scen_detail::FogColourData::red)>("red", offsetof(s1::scen_detail::FogColourData, red)),
    scenarioNativeField<decltype(s1::scen_detail::FogColourData::green)>("green", offsetof(s1::scen_detail::FogColourData, green)),
    scenarioNativeField<decltype(s1::scen_detail::FogColourData::blue)>("blue", offsetof(s1::scen_detail::FogColourData, blue)),
    scenarioNativeField<decltype(s1::scen_detail::FogColourData::colour)>("colour", offsetof(s1::scen_detail::FogColourData, colour)),
};

constexpr ScenarioDataSchema schemas[] = {
    {.name = "DoorSystemInitParams", .size = sizeof(s1::DoorSystemInitParams), .fields = initFields, .alignment = alignof(s1::DoorSystemInitParams)},
    {.name = "DSWKDoorEntry", .size = sizeof(s1::DSWKDoorEntry), .fields = entryFields, .alignment = alignof(s1::DSWKDoorEntry)},
    {.name = "DoorSlotState", .size = sizeof(s1::DoorSlotState), .fields = slotFields, .alignment = alignof(s1::DoorSlotState)},
    {.name = "DoorResourceDescriptor", .size = sizeof(s1::DoorResourceDescriptor), .fields = resourceFields, .alignment = alignof(s1::DoorResourceDescriptor)},
    {.name = "MapObjectRecord", .size = sizeof(s1::MapObjectRecord), .fields = mapObjectFields, .alignment = alignof(s1::MapObjectRecord)},
    {.name = "DoorMotionSelectorBlock",
        .size = sizeof(s1::DoorMotionSelectorBlock),
        .fields = selectorFields,
        .alignment = alignof(s1::DoorMotionSelectorBlock)},
    {.name = "ScnDoorMotionDescriptor", .size = sizeof(s1::ScnDoorMotionDescriptor), .fields = motionFields, .alignment = alignof(s1::ScnDoorMotionDescriptor)},
    {.name = "Vector3", .size = sizeof(Vector3), .fields = vector3Fields, .alignment = alignof(Vector3)},
    {.name = "Vector3i", .size = sizeof(Vector3i), .fields = vector3iFields, .alignment = alignof(Vector3i)},
    {.name = "Vector4", .size = sizeof(Vector4), .fields = vector4Fields, .alignment = alignof(Vector4)},
    {.name = "FaceLipCue", .size = sizeof(s1::FaceLipCue), .fields = lipCueFields, .alignment = alignof(s1::FaceLipCue)},
    {.name = "GameDateTime", .size = sizeof(s1::GameDateTime), .fields = dateTimeFields, .alignment = alignof(s1::GameDateTime)},
    {.name = "FogColourData", .size = sizeof(s1::scen_detail::FogColourData), .fields = fogColourFields, .alignment = alignof(s1::scen_detail::FogColourData)}};

static_assert(validScenarioSchemas(schemas), "Scenario schemas must cover the actual native record fields exactly");

} // namespace

std::span<const ScenarioDataSchema> scenarioDataSchemas() {
    return schemas;
}

namespace {

struct ScriptSchema {
    std::string name;
    std::vector<ScenarioScriptField> owned;
    std::vector<ScenarioDataField> fields;
    ScenarioDataSchema schema;
};

std::deque<ScriptSchema> &scriptSchemas() {
    static std::deque<ScriptSchema> definitions;
    return definitions;
}

} // namespace

void registerScenarioScriptSchema(const std::string &name, std::uint32_t size, std::uint32_t alignment, const std::vector<ScenarioScriptField> &fields) {
    ScriptSchema &definition = scriptSchemas().emplace_back(ScriptSchema{.name = name, .owned = fields});
    for (const ScenarioScriptField &field : definition.owned)
        definition.fields.push_back(ScenarioDataField{.name = field.name.c_str(),
            .offset = field.offset,
            .elementCount = field.elementCount,
            .type = field.type,
            .pointeeType = field.pointeeType.empty() ? nullptr : field.pointeeType.c_str()});
    definition.schema = ScenarioDataSchema{.name = definition.name.c_str(), .size = size, .fields = definition.fields, .alignment = alignment};
}

const ScenarioDataSchema *findScenarioDataSchema(std::string_view name) {
    for (const ScenarioDataSchema &schema : schemas) {
        if (name == schema.name)
            return &schema;
    }
    for (size_t index = scriptSchemas().size(); index > 0; --index)
        if (name == scriptSchemas()[index - 1].name)
            return &scriptSchemas()[index - 1].schema;
    return nullptr;
}

std::uint32_t scenarioDataTypeSize(ScenarioDataType type) {
    switch (type) {
    case ScenarioDataType::U8:
    case ScenarioDataType::I8:
        return 1;
    case ScenarioDataType::U16:
    case ScenarioDataType::I16:
        return 2;
    default:
        return 4;
    }
}

std::optional<std::uint32_t> scenarioDataConstantValue(std::string_view name) {
#define SCENARIO_CONSTANT(symbol)                                                                                                                              \
    if (name == #symbol)                                                                                                                                       \
        return static_cast<std::uint32_t>(s1::symbol);
    SCENARIO_CONSTANT(DSWK_REQ_FREE)
    SCENARIO_CONSTANT(DSWK_REQ_NOT_READY)
    SCENARIO_CONSTANT(DSWK_REQ_FAILED)
    SCENARIO_CONSTANT(DSWK_REQ_COMPLETE)
    SCENARIO_CONSTANT(DSWK_REQ_ACTIVE)
    SCENARIO_CONSTANT(DSWK_REQ_PENDING)
    SCENARIO_CONSTANT(DSWK_TASK_IDLE)
    SCENARIO_CONSTANT(DSWK_TASK_PRIME_CANDIDATES)
    SCENARIO_CONSTANT(DSWK_TASK_ACTIVE_UPDATE)
    SCENARIO_CONSTANT(DSWK_4)
    SCENARIO_CONSTANT(DSWK_8)
    SCENARIO_CONSTANT(DSWK_FRONT)
    SCENARIO_CONSTANT(DSWK_20)
    SCENARIO_CONSTANT(DSWK_100)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_NONE)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_HANDLE_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_HANDLE_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_HANDLE_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_HANDLE_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_HANDLE_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_HANDLE_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_D_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_D_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_LEAF_A_ONLY)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_LEAF_B_ONLY)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_AUTOMATIC)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_AUTOMATIC_ALT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_PAIR_SELECTABLE_SINGLE_LEAF)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_PAIR_SINGLE_LEAF_ALT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_SELECTABLE)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_SPECIAL)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_AUTOMATIC)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_LEAF_A_ONLY)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_LEAF_B_ONLY)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_SELECTABLE)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_LEAF_A_ONLY)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_LEAF_B_ONLY)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_SELECTABLE_SINGLE_LEAF)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_4000)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_C000)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_4000)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_C000)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SWING_PAIR_BOTH_LEAVES_SIMULTANEOUS)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SPECIAL_SUBTYPE_1D)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_ALT_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_ALT_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_ALT_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_ALT_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_ALT_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_ALT_LEFT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_AP_RIGHT)
    SCENARIO_CONSTANT(DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_AP_LEFT)
#undef SCENARIO_CONSTANT
    return std::nullopt;
}

const char *scenarioPointeeCType(const char *pointee) {
    if (!pointee)
        return "void";
    const std::string_view name = pointee;
    if (name == "u8")
        return "uint8_t";
    if (name == "i8")
        return "int8_t";
    if (name == "u16")
        return "uint16_t";
    if (name == "i16")
        return "int16_t";
    if (name == "u32")
        return "uint32_t";
    if (name == "i32")
        return "int32_t";
    if (name == "f32")
        return "float";
    if (name == "cstr")
        return "char";
    return pointee;
}

} // namespace shenmue::scenario
