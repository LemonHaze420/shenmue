#include "s2/character_table_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "platform/dc/simm.hpp"

namespace shenmue {
namespace s2 {

const std::uint32_t *ParseCharacterTableRecord(CharacterTableRecord *record, const char *const *tokens, const std::uint32_t *source);

void ApplyCharacterTableControl(const CharacterTableArgument *arguments);

// @verified - d
std::int32_t CompareCharacterTableTokens(const char *left, const char *right) {
    for (;;) {
        const std::int8_t a = static_cast<std::int8_t>(SIMM_UppercaseAscii(static_cast<std::int8_t>(*left++)));
        const std::int8_t b = static_cast<std::int8_t>(SIMM_UppercaseAscii(static_cast<std::int8_t>(*right++)));
        if (a > b)
            return 1;
        if (a < b)
            return -1;
        if (!a)
            return 0;
    }
}

// @verified - d
const std::uint32_t *ApplyCharacterTableNodeRecord(CharacterNode *node, std::uint32_t *fields, const std::uint32_t *source) {
    CharacterTableRecord record;
    const std::uint32_t *const next = ParseCharacterTableRecord(&record, characterNodeTokens, source);
    switch (static_cast<CharacterNodeCommand>(record.tokenIndex)) {
    case CharacterNodeCommand_Position:
        node->position.x = record.arguments[0].value.floating;
        node->position.y = record.arguments[1].value.floating;
        node->position.z = record.arguments[2].value.floating;
        *fields |= CN_Position;
        break;
    case CharacterNodeCommand_Angle:
        if (record.count == 1) {
            node->rotation.y = static_cast<std::int32_t>(record.arguments[0].value.floating * 8192.0f / 45.0f) & 0xffff;
        } else {
            node->rotation.x = static_cast<std::int32_t>(record.arguments[0].value.floating * 8192.0f / 45.0f) & 0xffff;
            node->rotation.y = static_cast<std::int32_t>(record.arguments[1].value.floating * 8192.0f / 45.0f) & 0xffff;
            node->rotation.z = static_cast<std::int32_t>(record.arguments[2].value.floating * 8192.0f / 45.0f) & 0xffff;
        }
        *fields |= CN_Angle;
        break;
    case CharacterNodeCommand_Layer:
        node->layer = static_cast<std::int32_t>(record.arguments[0].value.floating);
        *fields |= CN_Layer;
        break;
    case CharacterNodeCommand_Local:
        if (record.arguments[0].type == CharacterTableArgumentType_Float) {
            node->parent = GetCharacterControlSlot(static_cast<std::int32_t>(record.arguments[0].value.floating));
        } else {
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            node->parent = GetCharacterNode(scenFindChara(control->scenarioTask, shen_name(record.arguments[0].value.bits)));
        }
        *fields |= CN_Local;
        break;
    case CharacterNodeCommand_HasColi:
        if (record.arguments[0].type == CharacterTableArgumentType_String)
            node->collision = 0;
        else
            node->collision = 0;
        *fields |= CN_Collision;
        break;
    default:
        return source;
    }
    return next;
}

} // namespace s2
} // namespace shenmue

#endif
