#include "s2/areas/wbbk_models.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"

namespace shenmue::s2::areas {
BookModelState bookModelState;
void CreateBookStackModels() {
    for (std::int32_t stack = 0; bookStackPlacements[stack].position.y > 0; ++stack) {
        const BookStackPlacement &placement = bookStackPlacements[stack];
        const shen_name actor = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("bkDyn2"), shen_name{});
        bookModelState.stacks[stack + 1] = actor;
        EV_AT_SetPos(actor, EventTransformFlags::AllAxes, placement.position);
        EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, {.x = placement.yaw, .y = 0, .z = 16384});
        EV_EnableCharacterDisplay(actor);
        EV_EnsureCharacterDraw(actor);
        std::uint8_t shuffled[20];
        for (std::int32_t i = 0; i < 20; ++i)
            shuffled[i] = static_cast<std::uint8_t>(i);
        for (std::int32_t i = 2; i < 20; ++i) {
            const std::int32_t selected = shenGetRandomInt32(18) + 2;
            const std::uint8_t temporary = shuffled[selected];
            shuffled[selected] = shuffled[i];
            shuffled[i] = temporary;
        }
        float centre = 0;
        for (std::int32_t i = 0; i < 20; ++i) {
            const std::int32_t part = shuffled[i];
            const BookHeldPlacement &held = bookHeldPlacements[part];
            const float width = bookModelTypes[held.type].width;
            Vector3 position{.x = width * .5f + centre, .y = held.position.z, .z = held.position.x};
            centre += width;
            Vector3i rotation{.x = 0, .y = shenGetRandomInt32(2184) - 1092, .z = 49152};
            EV_scenApplyPosOrRotToPartOf(
                actor, static_cast<std::uint16_t>(part == 0 ? 152 : part + 153), &position, &rotation, EventCharacterPartTransformMode::Absolute);
            bookModelState.types[stack][i] = static_cast<std::uint8_t>(held.type);
        }
    }
}
void CreatePutBookModels() {
    const Vector3 zero{.x = 0, .y = 0, .z = 0};
    const Vector3i rotation{.x = 0, .y = 0, .z = 0};
    for (const shen_name *named = bookPutCharacters; named->name; ++named) {
        const shen_name actor = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("putbook"), shen_name{});
        CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("putbook"), *named);
        EV_AT_SetPos(actor, EventTransformFlags::AllAxes, zero);
        EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, rotation);
    }
}
void UpdateBookStackDisplay() {
    const shen_name stack = bookModelState.stacks[bookModelState.currentStack + 1];
    if (bookModelState.displayMode == 0) {
        EV_DisableCharacterDisplay(BOOK_DYNAMIC);
        EV_ToggleCharacterFlag(BOOK_DYNAMIC);
        EV_EnableCharacterDisplay(stack);
        EV_EnsureCharacterDraw(stack);
        for (std::uint32_t i = 0; i < 20; ++i) {
            const shen_name actor{BOOK_FIRST.name + i};
            EV_DisableCharacterDisplay(actor);
            EV_ToggleCharacterFlag(actor);
        }
    } else if (bookModelState.displayMode == 1) {
        EV_EnableCharacterDisplay(BOOK_DYNAMIC);
        EV_EnsureCharacterDraw(BOOK_DYNAMIC);
        EV_DisableCharacterDisplay(stack);
        EV_ToggleCharacterFlag(stack);
        for (std::uint32_t i = 0; i < 20; ++i) {
            const shen_name actor{BOOK_FIRST.name + i};
            EV_DisableCharacterDisplay(actor);
            EV_ToggleCharacterFlag(actor);
        }
    } else {
        EV_DisableCharacterDisplay(BOOK_DYNAMIC);
        EV_ToggleCharacterFlag(BOOK_DYNAMIC);
        EV_DisableCharacterDisplay(stack);
        EV_ToggleCharacterFlag(stack);
        for (std::uint32_t i = 0; i < 20; ++i) {
            const shen_name actor{BOOK_FIRST.name + i};
            EV_EnableCharacterDisplay(actor);
            EV_EnsureCharacterDraw(actor);
        }
    }
    float centre = 0;
    for (std::int32_t i = 0; i < 20; ++i) {
        Vector3 position{.x = 0, .y = 0, .z = 0};
        if (i > 0) {
            const std::uint8_t previous = bookModelState.types[bookModelState.currentStack][i - 1];
            const std::uint8_t current = bookModelState.types[bookModelState.currentStack][i];
            position.x = (bookModelTypes[previous].width + bookModelTypes[current].width) * .5f + centre;
            centre = position.x;
            position.z = (bookHeldPlacements[i].position.x + bookHeldPlacements[i - 1].position.x) * .5f;
            position.y = (bookHeldPlacements[i].position.z + bookHeldPlacements[i - 1].position.z) * .5f;
        }
        const shen_name actor{BOOK_FIRST.name + static_cast<std::uint32_t>(i)};
        EV_AT_SetPos(actor, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
    }
    if (bookModelState.displayMode == 1) {
        for (std::int32_t i = 0; i < 20; ++i) {
            Vector3 position;
            Vector3i rotation;
            dynaReadEvaluate(i, 2 * bookModelState.animationFrame + 1, &position.x, &rotation, reinterpret_cast<const std::uint8_t *>("DYNB"));
            position.x *= 10;
            position.y *= 10;
            position.z *= 10;
            if (bookModelState.mirror < 0)
                position.x *= -1;
            EV_scenApplyPosOrRotToPartOf(
                BOOK_DYNAMIC, static_cast<std::uint16_t>(i == 1 ? 172 : i + 152), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        }
    }
}
void AttachBookStackToPlayer() {
    AT_SetLocalBaseChara(BOOK_BASE, BOOK_RYO);
    EV_AT_SetPos(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
    EV_AT_SetAngY(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 49152, .y = 16384, .z = 0});
    float accumulated = 0;
    for (std::int32_t i = 0; i < 20; ++i) {
        const Vector3 &held = bookHeldPlacements[i].position;
        const Vector3 position{.x = held.x, .y = accumulated + held.z, .z = held.y};
        accumulated += position.y;
        const shen_name actor{BOOK_FIRST.name + static_cast<std::uint32_t>(i)};
        EV_AT_SetPos(actor, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
        AT_SetLocalBaseChara(actor, BOOK_BASE);
    }
}
void CreatePlacedBookStack(float x, float z) {
    const shen_name actor = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("bkDyn2"), shen_name{});
    bookModelState.placed[bookModelState.currentBook] = actor;
    bookModelState.created[bookModelState.createdCount++] = actor;
    EV_EnsureCharacterDraw(actor);
    EV_EnableCharacterDisplay(actor);
    AT_SetLocalBaseChara(actor, shen_name{});
    EV_SetCharaControllerFlags(actor, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    EV_AT_SetPos(actor, EventTransformFlags::AllAxes, {.x = x, .y = 127.5f, .z = z});
    EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
}
void UpdatePlacedBookStack(std::int32_t index, std::int32_t mode) {
    const shen_name actor = bookModelState.placed[index];
    Vector3 hidden{.x = -1000, .y = -1000, .z = -1000};
    Vector3i rotation;
    // Absolute mode only reads its arguments; the shared API uses writable pointers for Get mode.
    if (mode == 0) {
        for (std::uint16_t part = 13; part <= 20; ++part)
            EV_scenApplyPosOrRotToPartOf(actor, part + 152, &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
    }
    if (mode == 1) {
        for (std::uint16_t part = 7; part <= 20; ++part)
            EV_scenApplyPosOrRotToPartOf(actor, part + 152, &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
        const BookPutGroup &group = bookPutGroups[index];
        const shen_name put = bookPutCharacters[group.group];
        if (!group.count) {
            rotation = {.x = 0, .y = 32768, .z = 0};
            EV_AT_SetPos(put, EventTransformFlags::AllAxes, bookPutPositions[group.group]);
            EV_AT_SetAngY(put, EventTransformFlags::AllAxes, rotation);
            EV_EnsureCharacterDraw(put);
            EV_EnableCharacterDisplay(put);
        }
        for (std::uint16_t i = 0; i < 36; ++i)
            EV_scenApplyPosOrRotToPartOf(put, i + 152, &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
        for (std::int32_t i = 0; i < 12 * group.count + 6; ++i)
            EV_scenApplyPosOrRotToPartOf(put, static_cast<std::uint16_t>(i + 152), const_cast<Vector3 *>(&bookPutPartPlacements[i].position),
                const_cast<Vector3i *>(&bookPutPartPlacements[i].rotation), EventCharacterPartTransformMode::Absolute);
    } else if (mode == 2) {
        const BookPutGroup &group = bookPutGroups[index];
        const shen_name put = bookPutCharacters[group.group];
        EV_ToggleCharacterFlag(actor);
        EV_DisableCharacterDisplay(actor);
        for (std::uint16_t i = 0; i < 36; ++i)
            EV_scenApplyPosOrRotToPartOf(put, i + 152, &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
        for (std::int32_t i = 0; i < 12 * (group.count + 1); ++i)
            EV_scenApplyPosOrRotToPartOf(put, static_cast<std::uint16_t>(i + 152), const_cast<Vector3 *>(&bookPutPartPlacements[i].position),
                const_cast<Vector3i *>(&bookPutPartPlacements[i].rotation), EventCharacterPartTransformMode::Absolute);
    }
}
} // namespace shenmue::s2::areas
