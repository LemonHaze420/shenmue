#include "s2/areas/area_wb01.hpp"
#include "s2/field_collision.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
std::uint32_t smokeIds[16]{};
ConversationBuffer *closingDialogue{};
constexpr Vector3 smokePositions[]{
    {336.6806945800781f, 128.16099548339844f, 250.6540069580078f},
    {335.7546081542969f, 129.37100219726562f, 245.04330444335938f},
    {337.7090148925781f, 129.46099853515625f, 245.03529357910156f},
    {335.1780090332031f, 129.4709930419922f, 242.3419952392578f},
    {338.235595703125f, 129.4709930419922f, 242.21009826660156f},
    {336.72760009765625f, 129.26600646972656f, 245.03280639648438f},
    {336.71820068359375f, 129.51100158691406f, 244.4698944091797f},
    {337.51971435546875f, 129.4709930419922f, 242.2375030517578f},
    {337.2933044433594f, 129.62600708007812f, 238.87210083007812f},
    {336.6243896484375f, 129.4011993408203f, 242.2886962890625f},
    {330.544189453125f, 129.44349670410156f, 243.28919982910156f},
    {342.7651062011719f, 129.47210693359375f, 242.7613067626953f},
    {331.84210205078125f, 129.61599731445312f, 238.2852020263672f},
    {341.62579345703125f, 129.38710021972656f, 238.26730346679688f},
    {335.9136962890625f, 129.39810180664062f, 242.23390197753906f},
};
struct WindowPlacement {
    std::uint8_t x;
    std::uint8_t z;
    std::uint16_t padding02;
    std::int32_t yaw;
    std::int32_t kind;
    shen_name character;
    std::int32_t flag;
};
static_assert(sizeof(WindowPlacement) == 20);
static_assert(offsetof(WindowPlacement, character) == 12);
constexpr WindowPlacement windowPlacements[]{
    {7, 2, 0, 0, 4, SHEN_NAME(WL00), 1600},
    {7, 3, 0, 0, 4, SHEN_NAME(WL01), 1601},
    {7, 9, 0, 0, 4, SHEN_NAME(WL02), 1602},
    {7, 10, 0, 0, 4, SHEN_NAME(WL03), 1603},
    {7, 11, 0, 0, 4, SHEN_NAME(WL04), 1604},
    {7, 14, 0, 0, 4, SHEN_NAME(WL06), 1606},
    {7, 15, 0, 0, 4, SHEN_NAME(WL07), 1607},
    {7, 16, 0, 0, 4, SHEN_NAME(WL08), 1608},
    {7, 17, 0, 0, 4, SHEN_NAME(WL09), 1609},
    {7, 19, 0, 0, 4, SHEN_NAME(WL10), 1610},
    {7, 20, 0, 0, 4, SHEN_NAME(WL11), 1611},
    {7, 21, 0, 0, 4, SHEN_NAME(WL12), 1612},
    {7, 22, 0, 0, 4, SHEN_NAME(WL13), 1613},
    {7, 23, 0, 0, 4, SHEN_NAME(WL14), 1614},
    {7, 24, 0, 0, 4, SHEN_NAME(WL15), 1615},
    {22, 1, 0, 32768, 5, SHEN_NAME(WL16), 1616},
    {22, 9, 0, 32768, 5, SHEN_NAME(WL17), 1617},
    {22, 10, 0, 32768, 5, SHEN_NAME(WL18), 1618},
    {22, 11, 0, 32768, 5, SHEN_NAME(WL19), 1619},
    {22, 14, 0, 32768, 5, SHEN_NAME(WL21), 1621},
    {22, 15, 0, 32768, 5, SHEN_NAME(WL22), 1622},
    {22, 16, 0, 32768, 5, SHEN_NAME(WL23), 1623},
    {22, 17, 0, 32768, 5, SHEN_NAME(WL24), 1624},
    {22, 19, 0, 32768, 5, SHEN_NAME(WL25), 1625},
    {22, 20, 0, 32768, 5, SHEN_NAME(WL26), 1626},
    {22, 21, 0, 32768, 5, SHEN_NAME(WL27), 1627},
    {22, 22, 0, 32768, 5, SHEN_NAME(WL28), 1628},
    {22, 23, 0, 32768, 5, SHEN_NAME(WL29), 1629},
    {22, 24, 0, 32768, 5, SHEN_NAME(WL30), 1630},
    {8, 25, 0, 49152, 6, SHEN_NAME(WL31), 1631},
    {9, 25, 0, 49152, 6, SHEN_NAME(WL32), 1632},
    {10, 25, 0, 49152, 6, SHEN_NAME(WL33), 1633},
    {11, 25, 0, 49152, 6, SHEN_NAME(WL34), 1634},
    {19, 25, 0, 49152, 6, SHEN_NAME(WL38), 1635},
    {20, 25, 0, 49152, 6, SHEN_NAME(WL39), 1636},
    {21, 25, 0, 49152, 6, SHEN_NAME(WL40), 1637},
    {1, 14, 0, 49152, 11, SHEN_NAME(CHR1), 1150},
    {1, 14, 0, 49152, 10, SHEN_NAME(CHR6), 1149},
    {1, 1, 0, 49152, 8, SHEN_NAME(CHR7), 1638},
    {1, 1, 0, 49152, 9, SHEN_NAME(CHR8), 1148},
    {},
};
void PlaceWindow(std::int32_t index) {
    const WindowPlacement &window = windowPlacements[index];
    if (window.x == 1)
        return;
    Vector3 position{
        (static_cast<float>(window.x) * 0.9f + 323.0f) + 0.45f,
        (GetRandomNumberInRange(0.5f) + (window.z < 13 ? 129.7f : 128.8f)) - 0.3f,
        (static_cast<float>(window.z) * 0.9f + 237.60001f) + 0.45f,
    };
    Vector3i rotation{window.yaw, 0, 49152};
    Vector3 scale;
    scale.x = GetRandomNumberInRange(0.5f) + 0.8f;
    scale.y = GetRandomNumberInRange(0.5f) + 0.8f;
    scale.z = GetRandomNumberInRange(0.5f) + 0.8f;
    if (window.yaw == 0) {
        position.x = GetRandomNumberInRange(0.001f) + 329.53f;
        rotation.y = shenGetRandomInt32(65536);
    } else if (window.yaw == 32768) {
        position.x = GetRandomNumberInRange(0.001f) + 343.92001f;
        rotation.y = shenGetRandomInt32(65536);
    } else if (window.yaw == 49152) {
        position.z = GetRandomNumberInRange(0.001f) + 260.66f;
        rotation.z = shenGetRandomInt32(65536);
    }
    EV_AT_SetPos(window.character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(window.character, EventTransformFlags::AllAxes, rotation, nullptr);
    SetMotionworkFlagForCharaById(window.character, &scale);
}
void SetWindows() {
    for (std::int32_t index = 0; windowPlacements[index].x; ++index) {
        const WindowPlacement &window = windowPlacements[index];
        const std::int8_t enabled = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, window.flag));
        if (window.x == 1) {
            if (enabled) {
                SetScenarioCharacterTransparency(window.character, 0.5f);
                ToggleScenarioCharacterFlag(window.character);
                EnableScenarioCharacterDisplay(window.character);
            } else {
                ToggleScenarioCharacterFlag(window.character);
                DisableScenarioCharacterDisplay(window.character);
            }
        } else {
            PlaceWindow(index);
        }
    }
}
void InitializeSmoke() {
    const Vector3 volume{0.1f, 0.0f, 0.1f};
    smokeIds[0] = static_cast<std::uint32_t>(CreateSmokeEffect(smokePositions[0], volume, 70, 0.08f, shen_name{}, 0));
    SetSmokeEffectFalloff(smokeIds[0], 21);
    SetSmokeEffectFadeFactor(smokeIds[0], 110);
    SetParticleEffectColour(0, smokeIds[0], 0, 144, 144, 144);
    SetParticleEffectInitialScale(0, smokeIds[0], 0.329f);
    SetParticleEffectSize(0, smokeIds[0], 1.56f, 1.41f);
    SetParticleEffectBirthRate(0, smokeIds[0], 0.0078f);
    const Vector3 velocity{0.0f, 0.016f, 0.0f};
    const Vector3 variance{};
    SetParticleEffectVelocity(0, smokeIds[0], velocity, variance);
    SuspendParticleEffect(0, smokeIds[0]);
    const Vector3 smallVelocity{0.0f, 0.0117647061124444f, 0.0f};
    const Vector3 smallVariance{0.0f, 0.005098039284348488f, 0.0f};
    for (std::int32_t index = 1; index != 15; ++index) {
        smokeIds[index] = static_cast<std::uint32_t>(CreateSmokeEffect(smokePositions[index], Vector3{}, 40, 0.06f, shen_name{}, 0));
        SetSmokeEffectFalloff(smokeIds[index], 13);
        SetSmokeEffectFadeFactor(smokeIds[index], 188);
        SetParticleEffectColour(0, smokeIds[index], 0, 191, 194, 170);
        SetParticleEffectInitialScale(0, smokeIds[index], 0.027450980618596077f);
        SetParticleEffectSize(0, smokeIds[index], 0.6666666865348816f, 0.3921568691730499f);
        SetParticleEffectBirthRate(0, smokeIds[index], 0.0235294122248888f);
        SetParticleEffectVelocity(0, smokeIds[index], smallVelocity, smallVariance);
        SuspendParticleEffect(0, smokeIds[index]);
    }
}
void WB01ClosingExit(void *) {
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(WB00), 5);
}
void WB01ClosingWait(void *) {
    if (SetPlayerGameFlags(PlayerGameFlagMode::QueryAreaControl, nullptr) && !cvsHasValidConverseData(closingDialogue)) {
        SetPlayerGameFlags(PlayerGameFlagMode::DisableAreaControl, nullptr);
        HLib::SetCurrentTaskTo(WB01ClosingExit);
        HLib::SetupTaskDelay(60);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
    }
}
void WB01MainLoop(void *) {
    if (EV_GetShenmueFlag(0x213) && !EV_GetShenmueFlag(0x4b3)) {
        if (srchHasActiveLockonTarget())
            srchRequestLockonExit();
        if (ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            SetPlayerGameFlags(PlayerGameFlagMode::EnableAreaControl, nullptr);
            spmFadeOut();
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2231)) {
                closingDialogue = EV_PlayDialogue("1021A001");
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2231, 1);
            }
            HLib::SetCurrentTaskTo(WB01ClosingWait);
        }
    }
}
} // namespace
void AreaWB01Init() {
    for (std::uint32_t &id : smokeIds)
        id = 0;
    closingDialogue = nullptr;
}
void AreaWB01Main(std::uint32_t startParameters) {
    SetMapControlStatusFlag(0, false);
    EV_FreeAuthorPak(1, "1021");
    SetWeatherUpdateEnabledTo(0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    SetScenarioCharacterDrawControl(ryo, 2, 0);
    HLib::EnqueueTaskWithoutParameter(WB01MainLoop, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    InitializeSmoke();
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1639, static_cast<std::int32_t>(startParameters));
    ToggleFlagOnFieldEntity(SHEN_NAME(dor2), 0);
    SetScenarioCharacterRadius(SHEN_NAME(GLD2), 3.0f);
    SetScenarioCharacterRadius(SHEN_NAME(GRD2), 3.0f);
    SetWindows();
}
void AreaWB01Cleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaWB01GameplayDefinition{SHEN_NAME(WB01), 0, AreaWB01Init, AreaWB01Main, AreaWB01Cleanup};
} // namespace shenmue::s2::areas
