#include "s2/areas/area_elevator.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include <bit>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
AreaElevatorBinding areaElevatorsQA00[]{{1, {}, {269.1000061035156f, 100.0f, 195.3000030517578f}, {0, 32768, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {301.5f, 100.0f, 203.39999389648438f}, {0, 16384, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {287.1000061035156f, 100.0f, 123.30000305175781f}, {0, 32768, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {301.5f, 101.80000305175781f, 133.1999969482422f}, {}, {65535, 65535, 65535, 65535}}, {}};
AreaElevatorBinding areaElevatorsQD00[]{{1, {}, {162.89999389648438f, 100.0f, 73.80000305175781f}, {0, 49152, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {213.3000030517578f, 100.0f, 65.69999694824219f}, {0, 32768, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {164.6999969482422f, 100.0f, 118.4800033569336f}, {}, {65535, 65499, 65535, 65535}}, {}};
AreaElevatorBinding areaElevatorsQF00[]{{1, {}, {188.10000610351562f, 100.0f, 139.5f}, {0, 32768, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {224.10000610351562f, 100.0f, 186.3000030517578f}, {}, {65535, 65535, 65535, 65535}},
    {1, {}, {264.6000061035156f, 100.0f, 167.39999389648438f}, {0, 16384, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {263.70001220703125f, 100.0f, 140.39999389648438f}, {0, 16384, 0}, {65535, 65535, 65535, 65535}}, {}};
AreaElevatorBinding areaElevatorsQC00[]{{1, {}, {180.89999389648438f, 100.0f, 181.8000030517578f}, {0, 16384, 0}, {65535, 65535, 65535, 65535}},
    {1, {}, {164.6999969482422f, 100.0f, 122.5199966430664f}, {0, 32768, 0}, {65535, 65499, 65535, 65535}}, {}};
AreaElevatorBinding areaElevatorsQE00[]{{1, {}, {254.6999969482422f, 100.0f, 97.19999694824219f}, {}, {65535, 65535, 65535, 65535}},
    {1, {}, {272.70001220703125f, 100.0f, 81.0f}, {}, {65535, 65535, 65535, 65535}},
    {1, {}, {306.8999938964844f, 100.0f, 108.9000015258789f}, {}, {65535, 65535, 65535, 65535}}, {}};
AreaElevatorBinding areaElevatorsQJBT[]{{1, {}, {279.8999938964844f, 100.0f, 243.89999389648438f}, {}, {65535, 65535, 65535, 65535}}, {}};
AreaElevatorBinding *g_areaElevatorBindings{};

void ElevatorStuff() {
    if (g_currentMap.name == SHEN_NAME(QA00))
        g_areaElevatorBindings = areaElevatorsQA00;
    else if (g_currentMap.name == SHEN_NAME(QC00))
        g_areaElevatorBindings = areaElevatorsQC00;
    else if (g_currentMap.name == SHEN_NAME(QD00))
        g_areaElevatorBindings = areaElevatorsQD00;
    else if (g_currentMap == AREA_QE00)
        g_areaElevatorBindings = areaElevatorsQE00;
    else if (g_currentMap.name == SHEN_NAME(QF00) || g_currentMap.name == SHEN_NAME(QF02))
        g_areaElevatorBindings = areaElevatorsQF00;
    else if (g_currentMap == AREA_QJBT)
        g_areaElevatorBindings = areaElevatorsQJBT;
    else {
        g_areaElevatorBindings = nullptr;
        return;
    }
    for (auto *row = g_areaElevatorBindings; row->active; ++row) {
        for (auto &character : row->characters) {
            character = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("XX515NTG"), {});
            if (character.name)
                EnableScenarioCharacterDisplay(character);
        }
    }
}

Vector3 *UpdateAreaElevatorBindings(const Vector3 *floorPosition, std::uint32_t floor) {
    Vector3 *result = reinterpret_cast<Vector3 *>(static_cast<std::uintptr_t>(floor));
    if (!g_areaElevatorBindings)
        return result;
    const std::int32_t firstFloor = std::bit_cast<std::int32_t>(floor + 15U);
    const std::int32_t secondFloor = std::bit_cast<std::int32_t>(floor + 14U);
    for (auto *row = g_areaElevatorBindings; row->active; ++row) {
        result = &row->position;
        Vector3 positions[]{{0.8f, 4.2f, 0.0f}, {-0.8f, 4.2f, 0.0f}, {0.8f, 0.6f, 0.0f}, {-0.8f, 0.6f, 0.0f}};
        Vector3i rotations[]{{0, 32768, 0}, {32768, 0, 0}, {0, 32768, 0}, {32768, 0, 0}};
        Vector3i partRotations[]{{0, 0, 0}, {32768, 32768, 0}, {0, 0, 0}, {32768, 32768, 0}};
        std::int32_t glyphOffsets[]{2, 1, 1, 0};
        for (std::int32_t index = 0; index != 4; ++index) {
            const shen_name character = row->characters[index];
            if (!character.name)
                continue;
            ApplyScenarioTranslationAndRotation(&row->position, &row->rotation, &positions[index]);
            positions[index].y += floorPosition->y;
            EV_AT_SetPos(character, EventTransformFlags::AllAxes, positions[index]);
            rotations[index].x += row->rotation.x;
            rotations[index].y += row->rotation.y + row->rotation.z;
            EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotations[index]);
            std::int32_t selected = std::bit_cast<std::int32_t>(floor + static_cast<std::uint32_t>(glyphOffsets[index]) + 151U);
            if (selected > 192)
                selected = 192;
            else if (selected <= 152) {
                switch (selected) {
                case 148:
                    selected = 198;
                    break;
                case 149:
                    selected = 197;
                    break;
                case 150:
                    selected = 196;
                    break;
                case 151:
                    selected = 195;
                    break;
                case 152:
                    selected = 194;
                    break;
                default:
                    selected = 152;
                    break;
                }
            }
            Vector3 shown{}, hidden{0.0f, -1000.0f, 0.0f};
            for (std::int32_t part = 152; part <= 198; ++part) {
                if (part == 193)
                    continue;
                const bool selectedPart = part == selected;
                EV_scenApplyPosOrRotToPartOf(character, part, selectedPart ? &shown : &hidden, selectedPart ? &partRotations[index] : nullptr,
                    EventCharacterPartTransformMode::Absolute);
            }
            SetPluralControlFor(ctrlGetCharaByName(character), 1);
            const std::int32_t displayFloor = index < 2 ? firstFloor : secondFloor;
            const std::uint32_t mask = row->floorMasks[displayFloor / 16];
            if ((mask >> (static_cast<std::uint32_t>(displayFloor % 16) & 31U)) & 1U)
                EnableScenarioCharacterDisplay(character);
            else
                DisableScenarioCharacterDisplay(character);
        }
    }
    return result;
}
} // namespace shenmue::s2
