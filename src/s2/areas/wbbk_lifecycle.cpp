#include "s2/areas/area_wbbk.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/inventory.hpp"
#include "s2/model_component.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/step_event.hpp"
namespace shenmue::s2::areas {
BookEventPhase bookEventPhase;
std::int8_t bookEventState, bookCompletion;
std::int32_t bookStoryVariant, bookIntroWait, bookEntrySelection;
Vector3 bookCompanionTarget, bookCarryAnchor;
float bookCarryAnchorBlend;
SearchTarget bookSearchTargets[15]{
    {.name = SHEN_NAME(EV0A),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 350.100006f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0B),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 349.519989f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0C),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 348.920013f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0D),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 348.320007f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0E),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 347.670013f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0F),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 345.450012f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0G),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 344.779999f, .y = 128.199997f, .z = 230.289993f},
        .lockonAngle = .5f,
        .yaw = -32768,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0H),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 350.100006f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0I),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 349.519989f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0J),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 348.920013f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0K),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 348.320007f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0L),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 347.670013f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0M),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 345.450012f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = SHEN_NAME(EV0N),
        .flags = 0,
        .field08 = 0,
        .position = {.x = 344.779999f, .y = 128.199997f, .z = 233.360001f},
        .lockonAngle = .5f,
        .yaw = 0,
        .yawLimit = 13653},
    {.name = {}, .flags = 0, .field08 = 0, .position = {.x = -1, .y = -1, .z = -1}, .lockonAngle = 0, .yaw = 0, .yawLimit = 0},
};
void ChangeBookEventState(std::int8_t state) {
    bookEventState = state;
    bookEventPhase = {.phase = 0, .frame = 0};
    if (g_forcePlayerGodmode)
        bookEventState = 1;
}
bool IsBookTimeExpired() {
    DateTime time;
    GetCurrentDateTime(time);
    if (time.minute + 60 * time.hour <= 719)
        return false;
    SetPlayerControlEnabled(0);
    ChangeBookEventState(7);
    return true;
}
void CleanupBookActors() {
    EV_FreeAuthorPak(0, "0035");
    EV_NPC_NewLoadSw(0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(BOOK_SAI), FacialNodeOriginal);
    const shen_name knk{SHEN_NAME(KNK_)}, kun{SHEN_NAME(KUN_)};
    EV_npcFree(knk);
    EV_npcFree(BOOK_SAI);
    EV_npcFree(kun);
    EV_npcLockUnlock(knk);
    EV_npcLockUnlock(BOOK_SAI);
    EV_npcLockUnlock(kun);
    EV_ResetFreeTalkFlags();
    npcAdvanceTime(-1, 0);
    srchClearTarget(2);
}
void RecordBookCount(std::int32_t books, std::int32_t failures) {
    const std::int32_t flags = EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 11);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 11, (16 * books) | (failures + (flags & 15)));
}
void CompleteBookWork() {
    if (bookModelState.currentBook < 10) {
        EV_stepSetFlags(1, 0x29);
        return;
    }
    const std::int32_t progress = (EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 10) & 0xf0) + 16;
    EV_stepSetFlags(0, 0x29);
    constexpr std::uint32_t items[]{3017, 3020, 3018, 3019, 3014, 0};
    std::int32_t index = 0;
    while (GetItemState(items[index]))
        if (!items[++index])
            return;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2044, 1);
    const std::int32_t flags = EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 10);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 10, progress | (flags & 15));
}
} // namespace shenmue::s2::areas
