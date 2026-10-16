#include "s2/areas/area_ro10.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include <cmath>
namespace shenmue::s2::areas {
float Ro10FlushSmall(float value) {
    return std::fabs(value) < .00000011920929f ? 0.0f : value;
}
bool GetRo10ItemTransform(const Ro10Shop &shop, std::int32_t column, std::int32_t row, Vector3 *position, Vector3i *rotation, Vector3 *scale, bool grip) {
    while (column < 0)
        column += shop.columns;
    while (column >= shop.columns)
        column -= shop.columns;
    while (row < 0)
        row += shop.rows;
    while (row >= shop.rows)
        row -= shop.rows;
    Vector3 translated{Ro10FlushSmall(shop.columnSpacing * static_cast<float>(column)), 0, shop.rowSpacing * static_cast<float>(row)};
    ApplyScenarioTranslationAndRotation(&shop.position, &shop.rotation, &translated);
    if (position)
        *position = translated;
    const std::int32_t style = shop.grips[column + row * shop.columns];
    if (rotation) {
        *rotation = Vector3i{shop.rotation.x + 49152, shop.rotation.y, shop.rotation.z};
        if (grip && style > 0) {
            const Vector3i corrections[3]{{0, 0, 0}, {0, 0, 16384}, {16384, 0, 0}};
            rotation->x += corrections[style - 1].x;
            rotation->y += corrections[style - 1].y;
            rotation->z += corrections[style - 1].z;
        }
    }
    if (scale && style > 0) {
        const float value = style == 3 ? 1.0f : 1.5f;
        *scale = Vector3{value, value, value};
    }
    return true;
}
void CreateRo10PriceBoards(Ro10Work &work) {
    for (std::int32_t index = 0; index < 16; ++index) {
        const std::int32_t price = work.definition->prices[index];
        if (price <= 0) {
            work.boards[index] = shen_name{0};
            continue;
        }
        const shen_name board = CreateEventCharacter(CharacterCreationTable0, reinterpret_cast<const std::byte *>("SHITAJI"), shen_name{0});
        work.boards[index] = board;
        shen_name digits[3];
        if (board.name) {
            char numbers[3]{static_cast<char>(price % 10 + '0'), static_cast<char>((price / 10) % 10 + '0'), static_cast<char>((price / 100) % 10 + '0')};
            if (numbers[2] == '0') {
                numbers[2] = 0;
                if (numbers[1] == '0')
                    numbers[1] = 0;
            }
            for (std::int32_t digit = 0; digit < 3; ++digit) {
                if (!numbers[digit]) {
                    digits[digit] = shen_name{0};
                    continue;
                }
                const char model[5]{'N', 'U', 'M', numbers[digit], 0};
                digits[digit] = CreateEventCharacter(CharacterCreationTable0, reinterpret_cast<const std::byte *>(model), shen_name{0});
                if (!digits[digit].name)
                    continue;
                AT_SetLocalBaseChara(digits[digit], board);
                const Vector3 position{Ro10FlushSmall(Ro10FlushSmall(static_cast<float>(digit - 1) * -.02f) + 0.0f), .001f, .0020000001f};
                const Vector3i rotation{50972, 0, 0};
                const Vector3 scale{.25f, .25f, .25f};
                EV_AT_SetPos(digits[digit], EventTransformFlags::AllAxes, position);
                EV_AT_SetAngY(digits[digit], EventTransformFlags::AllAxes, rotation);
                SetMotionworkFlagForCharaById(digits[digit], &scale);
            }
        } else {
            digits[0] = shen_name{0};
            digits[1] = shen_name{0};
            digits[2] = shen_name{0};
        }
        work.hundreds[index] = digits[2];
        work.tens[index] = digits[1];
        work.ones[index] = digits[0];
    }
}
void RefreshRo10Items(Ro10Work &work) {
    const Ro10Shop &shop = *work.definition;
    for (std::int32_t index = 0; index < 16; ++index) {
        const shen_name item = shop.objects[index];
        if (!item.name || shop.prices[index] <= 0)
            continue;
        Vector3 base, itemPosition, scale;
        Vector3i baseRotation, itemRotation;
        GetRo10ItemTransform(shop, index & 3, index >> 2, &base, &baseRotation, nullptr, false);
        GetRo10ItemTransform(shop, index & 3, index >> 2, &itemPosition, &itemRotation, &scale, true);
        if (work.purchased[index] & 1U) {
            EV_DisableCharacterDisplay(item);
            if (work.boards[index].name)
                EV_DisableCharacterDisplay(work.boards[index]);
            if (work.hundreds[index].name)
                EV_DisableCharacterDisplay(work.hundreds[index]);
            if (work.tens[index].name)
                EV_DisableCharacterDisplay(work.tens[index]);
            if (work.ones[index].name)
                EV_DisableCharacterDisplay(work.ones[index]);
        } else {
            EV_AT_SetPos(item, EventTransformFlags::World | EventTransformFlags::AllAxes, itemPosition);
            EV_AT_SetAngY(item, EventTransformFlags::World | EventTransformFlags::AllAxes, itemRotation);
            if (!(work.purchased[index] & 8U)) {
                SetMotionworkFlagForCharaById(item, &scale);
                work.purchased[index] |= 8U;
            }
            EV_EnableCharacterDisplay(item);
        }

        if (work.boards[index].name && work.ones[index].name) {
            Vector3 boardPosition{0, -.07f, .0099999998f};
            ApplyScenarioTranslationAndRotation(&base, &baseRotation, &boardPosition);
            const Vector3i rotation{baseRotation.x - 44055, baseRotation.y, baseRotation.z};
            EV_AT_SetPos(work.boards[index], EventTransformFlags::World | EventTransformFlags::AllAxes, boardPosition);
            EV_AT_SetAngY(work.boards[index], EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            EV_EnableCharacterDisplay(work.boards[index]);
            if (work.hundreds[index].name)
                EV_EnableCharacterDisplay(work.hundreds[index]);
            if (work.tens[index].name)
                EV_EnableCharacterDisplay(work.tens[index]);
            EV_EnableCharacterDisplay(work.ones[index]);
        }
    }
}
void UpdateRo10ItemCamera(const Ro10Shop &shop, std::int32_t column, std::int32_t row, float distance, std::int32_t frames) {
    Vector3 target;
    Vector3i rotation;
    GetRo10ItemTransform(shop, column, row, &target, &rotation, nullptr, false);
    rotation.x += 5461;
    Vector3 position{0, 0, distance};
    ApplyScenarioTranslationAndRotation(&target, &rotation, &position);
    if (frames > 1) {
        Vector3 previousPosition, previousTarget;
        camCtrlGetNextCamPos(&previousPosition);
        camCtrlGetTargetPosition(&previousTarget);
        const float inverse = Ro10FlushSmall(1.0f / static_cast<float>(frames));
        const float blend = Ro10FlushSmall(1.0f - inverse);
        position.x = Ro10FlushSmall(position.x + Ro10FlushSmall(Ro10FlushSmall(previousPosition.x - position.x) * blend));
        position.y = Ro10FlushSmall(position.y + Ro10FlushSmall(Ro10FlushSmall(previousPosition.y - position.y) * blend));
        position.z = Ro10FlushSmall(position.z + Ro10FlushSmall(Ro10FlushSmall(previousPosition.z - position.z) * blend));
        target.x = Ro10FlushSmall(target.x + Ro10FlushSmall(Ro10FlushSmall(previousTarget.x - target.x) * blend));
        target.y = Ro10FlushSmall(target.y + Ro10FlushSmall(Ro10FlushSmall(previousTarget.y - target.y) * blend));
        target.z = Ro10FlushSmall(target.z + Ro10FlushSmall(Ro10FlushSmall(previousTarget.z - target.z) * blend));
    }
    camCtrlSetState(CameraState::Callback);
    camCtrlSetNextPosition(position);
    camCtrlSetTargetPosition(target);
    camCtrlSetFov(7281);
}
} // namespace shenmue::s2::areas
