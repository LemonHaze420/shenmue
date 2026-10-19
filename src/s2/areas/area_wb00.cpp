#include "s2/areas/area_wb00.hpp"
#include "s2/field_collision.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/search.hpp"
#include <cstdint>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name attendant{SHEN_NAME(a092)};
std::uint32_t smokeIds[16]{};
shen_name books[5]{};
std::int8_t door96{}, door213{}, door467{};
std::int32_t initializing{};
std::uint32_t fireId{};
constexpr Vector2 rainClip[]{{342.9700927734375f, 226.9893035888672f}, {353.7340087890625f, 226.9866943359375f}, {353.706787109375f, 236.87100219726562f},
    {342.87921142578125f, 236.9113006591797f}, {126.0999984741211f, 133.89840698242188f}};
constexpr Vector2 particleClip[]{{329.4164123535156f, 238.5950927734375f}, {343.948486328125f, 238.57110595703125f}, {343.9844055175781f, 262.45001220703125f},
    {329.45709228515625f, 262.3843078613281f}, {126.19999694824219f, 136.99859619140625f}};
constexpr Vector3 smokePositions[]{
    {330.6400146484375f, 128.0500030517578f, 268.0f},
    {341.2099914550781f, 128.0500030517578f, 268.0f},
};
constexpr Vector3 bookPositions[]{
    {341.1700134277344f, 127.44999694824219f, 237.3800048828125f},
    {335.5899963378906f, 127.44999694824219f, 228.3000030517578f},
    {337.17999267578125f, 127.44999694824219f, 237.47999572753906f},
    {330.8800048828125f, 127.44999694824219f, 228.2899932861328f},
    {333.17999267578125f, 127.44999694824219f, 237.5f},
};
constexpr Vector3 firePositions[]{
    {343.5f, 129.10000610351562f, 228.3000030517578f},
    {343.5f, 129.10000610351562f, 235.5f},
    {327.1000061035156f, 128.39999389648438f, 276.0f},
    {343.70001220703125f, 128.39999389648438f, 267.8999938964844f},
    {343.70001220703125f, 128.39999389648438f, 276.0f},
    {327.1000061035156f, 128.39999389648438f, 228.3000030517578f},
    {327.1000061035156f, 128.39999389648438f, 236.39999389648438f},
    {327.1000061035156f, 128.39999389648438f, 247.1999969482422f},
    {327.1000061035156f, 128.39999389648438f, 257.1000061035156f},
    {327.1000061035156f, 128.39999389648438f, 267.8999938964844f},
};
struct BookPartTransform {
    Vector3 position;
    Vector3i rotation;
    std::uint32_t following;
};
static_assert(sizeof(BookPartTransform) == 28);
BookPartTransform bookParts[]{
    {{-1.2999999523162842f, 0.019999999552965164f, -0.5199999809265137f}, {0, 396, 0}, 0x1U},
    {{-1.0f, 0.03999999910593033f, -0.5299999713897705f}, {0, 64932, 0}, 0x1U},
    {{-0.7200000286102295f, 0.009999999776482582f, -0.550000011920929f}, {0, 479, 0}, 0x1U},
    {{-1.3700000047683716f, 0.009999999776482582f, -0.12999999523162842f}, {0, 65182, 0}, 0x1U},
    {{-1.100000023841858f, 0.009999999776482582f, -0.11999999731779099f}, {0, 604, 0}, 0x1U},
    {{-0.8100000023841858f, 0.009999999776482582f, -0.18000000715255737f}, {0, 64639, 0}, 0x1U},
    {{-1.3799999952316284f, 0.019999999552965164f, 0.1599999964237213f}, {0, 250, 0}, 0x1U},
    {{-1.100000023841858f, 0.009999999776482582f, 0.23000000417232513f}, {0, 16751, 0}, 0x1U},
    {{-0.7099999785423279f, 0.009999999776482582f, 0.20000000298023224f}, {0, 396, 0}, 0x1U},
    {{-1.3600000143051147f, 0.03999999910593033f, 0.5799999833106995f}, {0, 64932, 0}, 0x1U},
    {{-0.9900000095367432f, 0.019999999552965164f, 0.550000011920929f}, {0, 48744, 0}, 0x2U},
    {{-0.6299999952316284f, 0.019999999552965164f, 0.5699999928474426f}, {0, 65078, 0}, 0x2U},
    {{-0.3700000047683716f, 0.009999999776482582f, -0.550000011920929f}, {0, 65265, 0}, 0x2U},
    {{-0.03999999910593033f, 0.009999999776482582f, -0.550000011920929f}, {0, 65182, 0}, 0x2U},
    {{0.25f, 0.03999999910593033f, -0.5699999928474426f}, {0, 980, 0}, 0x2U},
    {{-0.38999998569488525f, 0.019999999552965164f, -0.17000000178813934f}, {0, 15186, 0}, 0x2U},
    {{0.019999999552965164f, 0.019999999552965164f, -0.15000000596046448f}, {0, 396, 0}, 0x2U},
    {{0.3400000035762787f, 0.009999999776482582f, -0.18000000715255737f}, {0, 855, 0}, 0x2U},
    {{-0.3100000023841858f, 0.009999999776482582f, 0.20999999344348907f}, {0, 50496, 0}, 0x2U},
    {{0.029999999329447746f, 0.019999999552965164f, 0.2199999988079071f}, {0, 250, 0}, 0x2U},
    {{0.28999999165534973f, 0.03999999910593033f, 0.20000000298023224f}, {0, 65328, 0}, 0x2U},
    {{-0.30000001192092896f, 0.009999999776482582f, 0.5299999713897705f}, {0, 166, 0}, 0x2U},
    {{0.03999999910593033f, 0.009999999776482582f, 0.5299999713897705f}, {0, 65182, 0}, 0x3U},
    {{0.33000001311302185f, 0.009999999776482582f, 0.5199999809265137f}, {0, 64639, 0}, 0x3U},
    {{0.6299999952316284f, 0.019999999552965164f, -0.5099999904632568f}, {0, 64952, 0}, 0x3U},
    {{0.9599999785423279f, 0.03999999910593033f, -0.6000000238418579f}, {0, 16646, 0}, 0x3U},
    {{1.2899999618530273f, 0.019999999552965164f, -0.47999998927116394f}, {0, 64994, 0}, 0x3U},
    {{0.6299999952316284f, 0.009999999776482582f, -0.10999999940395355f}, {0, 354, 0}, 0x3U},
    {{0.9300000071525574f, 0.009999999776482582f, -0.30000001192092896f}, {0, 64827, 0}, 0x3U},
    {{1.2799999713897705f, 0.009999999776482582f, -0.07000000029802322f}, {0, 1126, 0}, 0x3U},
    {{0.6200000047683716f, 0.009999999776482582f, 0.27000001072883606f}, {0, 64765, 0}, 0x3U},
    {{0.9399999976158142f, 0.03999999910593033f, 0.05000000074505806f}, {0, 64932, 0}, 0x3U},
    {{1.2599999904632568f, 0.019999999552965164f, 0.23999999463558197f}, {0, 16125, 0}, 0x3U},
    {{0.6100000143051147f, 0.019999999552965164f, 0.5799999833106995f}, {0, 813, 0}, 0x3U},
    {{0.9300000071525574f, 0.019999999552965164f, 0.46000000834465027f}, {0, 458, 0}, 0x3U},
    {{1.2799999713897705f, 0.009999999776482582f, 0.5400000214576721f}, {0, 354, 0}, 0x7fU},
};

void ClearBooks() {
    const auto flag = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Story, 11));
    for (auto &book : books) {
        if (book.name) {
            SetScenarioCharacterKillMode(book, 1);
            book = {};
        }
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 11, flag & 0xf0);
}
void PlaceBooks() {
    auto count = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Story, 11) >> 4);
    if (count > 14)
        count = 14;
    if (EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED) || EV_GetShenmueFlag(0x96)) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 11, count & 0xf0);
        count = 0;
    }
    for (auto &book : books)
        book = {};
    if (!count)
        return;
    shen_name last{};
    Vector3i rotation{0, 32768, 0};
    for (std::int32_t index = 0; index < (count + 2) / 3; ++index) {
        last = books[index] = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("putbook"), {});
        EnsureScenarioCharacterDraw(last);
        EnableScenarioCharacterDisplay(last);
        EV_SetCharaControllerFlags(last, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(last, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(last, UpdateHeight, NpcCharacterControllerState::Disabled);
        AT_SetLocalBaseChara(last, {});
        EV_AT_SetPos(last, EventTransformFlags::AllAxes, bookPositions[index]);
        EV_AT_SetAngY(last, EventTransformFlags::AllAxes, rotation, nullptr);
    }
    const std::int32_t remainder = count % 3;
    if (remainder) {
        Vector3 hidden{-1000.0f, -1000.0f, -1000.0f};
        for (std::int32_t index = 0; index < 36; ++index)
            EV_scenApplyPosOrRotToPartOf(last, static_cast<std::uint16_t>(index + 152), &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
        for (std::int32_t index = 0; index < 12 * remainder; ++index)
            EV_scenApplyPosOrRotToPartOf(last, static_cast<std::uint16_t>(index + 152), &bookParts[index].position, &bookParts[index].rotation,
                EventCharacterPartTransformMode::Absolute);
    }
}
void RelocateRyo() {
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (position.z < 277.8999938964844f && position.x > 326.29998779296875f) {
        SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(ryo), 17, 0);
        EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, Vector3{337.07000732421875f, 126.97000122070312f, 280.3699951171875f});
        EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, Vector3i{0, 12196, 0}, nullptr);
    }
}
void ApplyDoors(std::uint32_t *) {
    if (door96 != EV_GetShenmueFlag(0x96)) {
        door96 = static_cast<std::int8_t>(EV_GetShenmueFlag(0x96));
        EV_AT_SetAngY(SHEN_NAME(dr1L), EventTransformFlags::AllAxes, Vector3i{0, door96 ? 16384 : 32768, 0}, nullptr);
        EV_AT_SetAngY(SHEN_NAME(dr1R), EventTransformFlags::AllAxes, Vector3i{0, door96 ? 49152 : 32768, 0}, nullptr);
        ToggleFlagOnFieldEntity(SHEN_NAME(dor1), door96);
    }
    if (door467 != EV_GetShenmueFlag(0x467)) {
        door467 = static_cast<std::int8_t>(EV_GetShenmueFlag(0x467));
        EV_AT_SetAngY(SHEN_NAME(dr2L), EventTransformFlags::AllAxes, Vector3i{0, door467 ? 32768 : 52792, 0}, nullptr);
        EV_AT_SetAngY(SHEN_NAME(dr2R), EventTransformFlags::AllAxes, Vector3i{0, door467 ? 0 : 45511, 0}, nullptr);
        ToggleFlagOnFieldEntity(SHEN_NAME(dor2), door467);
    }
    if (door213 != EV_GetShenmueFlag(0x213)) {
        door213 = static_cast<std::int8_t>(EV_GetShenmueFlag(0x213));
        EV_AT_SetAngY(SHEN_NAME(dr3L), EventTransformFlags::AllAxes, Vector3i{0, door213 ? 546 : 16384, 0}, nullptr);
        EV_AT_SetAngY(SHEN_NAME(dr3R), EventTransformFlags::AllAxes, Vector3i{0, door213 ? 32768 : 16384, 0}, nullptr);
        ToggleFlagOnFieldEntity(SHEN_NAME(dor3), door213);
    }
}
void DisplayAttendant(std::uint32_t *args) {
    if (*args == 1)
        EnableScenarioCharacterDisplay(attendant);
    else
        DisableScenarioCharacterDisplay(attendant);
    areaFlags |= 4U;
}
void InitializeFire() {
    fireId = static_cast<std::uint32_t>(CreateFireEffect(firePositions[0], 0.30000001192092896f, 0.4000000059604645f, 0, 4));
    for (std::int32_t index = 1; index < 10; ++index)
        SetFirePosition(fireId, index, &firePositions[index]);
}
void InitializeSmoke() {
    const Vector3 volume{0.1f, 0.0f, 0.1f};
    const Vector3 velocity{0.0f, 0.01600000075995922f, 0.0f};
    const Vector3 variance{};
    for (std::int32_t index = 0; index < 2; ++index) {
        smokeIds[index] = static_cast<std::uint32_t>(CreateSmokeEffect(smokePositions[index], volume, 40, 0.05000000074505806f, {}, 0));
        SetSmokeEffectFalloff(smokeIds[index], 21);
        SetSmokeEffectFadeFactor(smokeIds[index], 110);
        SetParticleEffectColour(0, smokeIds[index], 0, 144, 144, 144);
        SetParticleEffectInitialScale(0, smokeIds[index], 0.32899999618530273f);
        SetParticleEffectSize(0, smokeIds[index], 1.559999942779541f, 1.409999966621399f);
        SetParticleEffectBirthRate(0, smokeIds[index], 0.007799999788403511f);
        SetParticleEffectVelocity(0, smokeIds[index], velocity, variance);
        SuspendParticleEffect(0, smokeIds[index]);
    }
}
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(4, 1);
        ClearBooks();
        if (EV_GetShenmueFlag(0x213))
            RelocateRyo();
        SuspendParticleEffect(4, fireId);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(4, 0);
        ResumeParticleEffect(4, fireId);
    }
    ApplyWeatherRainAndRippleClip(rainClip);
    ApplyWeatherParticleClip(particleClip);
    if (!initializing) {
        EV_LoadCutsceneFromCurrentScene("WB00");
        EV_LoadAudio("WB00_000", night ? 1 : 0);
    }
    ApplyDoors(nullptr);
}
void WB00MainLoop(void *) {
    DateTime now;
    GetCurrentDateTime(now);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (now.hour >= 19) {
        if (srchHasActiveLockonTarget() && EV_GetShenmueFlag(0x213) && position.z < 277.8999938964844f && position.x > 326.29998779296875f)
            srchRequestLockonExit();
    } else
        ApplyDoors(nullptr);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2231) && now.hour == 18 && static_cast<std::uint32_t>(now.minute - 55) <= 3U && ryoNotBusy() &&
        !EV_GetShenmueFlag(0x4b3) && position.z < 277.8999938964844f && position.x > 326.29998779296875f) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2231, 1);
        EV_PlayDialogue("1021A001");
    }
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2238) && (position.z > 277.8999938964844f || position.x < 326.29998779296875f)) {
        ClearBooks();
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2238, 0);
        ApplyDoors(nullptr);
    }
    if (ryoNotBusy()) {
        EV_AT_SetPos(attendant, EventTransformFlags::AllAxes, Vector3{352.8479919433594f, 128.02499389648438f, 228.00999450683594f});
        EV_AT_SetAngY(attendant, EventTransformFlags::AllAxes, Vector3i{}, nullptr);
    }
    if (EV_GetShenmueFlag(0x20d)) {
        EnableScenarioCharacterDisplay(SHEN_NAME(sak1));
        EnableScenarioCharacterDisplay(SHEN_NAME(sak2));
    } else {
        DisableScenarioCharacterDisplay(SHEN_NAME(sak1));
        DisableScenarioCharacterDisplay(SHEN_NAME(sak2));
    }
}
} // namespace
void SetWarehouseBooksVisible(bool visible) {
    for (const shen_name book : books) {
        if (!book.name)
            continue;
        if (visible)
            EnableScenarioCharacterDisplay(book);
        else
            DisableScenarioCharacterDisplay(book);
    }
}

void ResetWarehouseBookCharacters() {
    for (shen_name &book : books)
        book = {};
}

void AreaWB00Init() {
    for (auto &id : smokeIds)
        id = 0;
    door96 = door213 = door467 = 0;
    initializing = 0;
    fireId = 0;
}
void AreaWB00Main(std::uint32_t) {
    initializing = 1;
    InitializeFire();
    HLib::EnqueueTaskWithoutParameter(WB00MainLoop, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    door96 = door213 = door467 = -1;
    EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED);
    PlaceBooks();
    EV_FreeAuthorPak(1, "1021");
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    areaBaseCallbacks[0] = DisplayAttendant;
    areaBaseCallbacks[1] = ApplyDoors;
    InitializeSmoke();
    initializing = 0;
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 897) || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 188))
        EnableScenarioCharacterDisplay(attendant);
    else
        DisableScenarioCharacterDisplay(attendant);
    SetDisplayFlagForProp(SHEN_NAME(MA01), 0);
    if (EV_GetShenmueFlag(0x46) || EV_GetShenmueFlag(0x47) || EV_GetShenmueFlag(0x2e1) || EV_GetShenmueFlag(0x25) || EV_GetShenmueFlag(0x6f))
        SetNpcColdResetMode(0);
}
void AreaWB00Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaWB00GameplayDefinition{SHEN_NAME(WB00), 0, AreaWB00Init, AreaWB00Main, AreaWB00Cleanup};
} // namespace shenmue::s2::areas
