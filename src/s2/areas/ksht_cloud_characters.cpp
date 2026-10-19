#include "s2/areas/ksht_authored.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/npc.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name cloudProps[]{SHEN_NAME(ab09), SHEN_NAME(a726), SHEN_NAME(a728)};
inline constexpr shen_name guestProps[]{SHEN_NAME(a904), SHEN_NAME(ab21), SHEN_NAME(a932), SHEN_NAME(a906), SHEN_NAME(a908), SHEN_NAME(a918), SHEN_NAME(a920),
    SHEN_NAME(a912), SHEN_NAME(a914), SHEN_NAME(a916), SHEN_NAME(a902), SHEN_NAME(a900), SHEN_NAME(a910), SHEN_NAME(a938), SHEN_NAME(a940)};
} // namespace
void PrepareKshtCloudCharacters(std::int32_t selection) {
    switch (selection) {
    case 0:
        PrepareKshtActor(KSHT_RYO);
        PrepareKshtActor(KSHT_SIN);
        break;
    case 1:
        PrepareKshtActor(SHEN_NAME(SNP_));
        PrepareKshtActor(SHEN_NAME(SNM_));
        EV_SetGlobalShadowLightingEnabled(0);
        break;
    case 2:
        PrepareKshtActor(KSHT_RYO);
        PrepareKshtActor(KSHT_SIN);
        PrepareKshtProp(SHEN_NAME(a246));
        PrepareKshtProp(SHEN_NAME(a349));
        EV_DisableCharacterDisplay(SHEN_NAME(a246));
        SetPluralControlFor(FindCharacterByScenarioName(SHEN_NAME(RCKO)), 0);
        break;
    case 3:
    case 5:
    case 6:
        for (shen_name prop : cloudProps)
            PrepareKshtProp(prop);
        for (shen_name prop : cloudProps)
            ApplyCharacterGeometryLighting(FindCharacterByScenarioName(prop), 1);
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(cloudProps[1]), 0);
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(cloudProps[2]), 0);
        PrepareKshtActor(KSHT_RYO);
        PrepareKshtActor(KSHT_SIN);
        break;
    case 4:
        EV_DisableCharacterDisplay(KSHT_SIN);
        PrepareKshtActor(SHEN_NAME(SN5_));
        PrepareKshtProp(SHEN_NAME(a345));
        break;
    case 7:
        PrepareKshtActor(SHEN_NAME(IWA_));
        PrepareKshtActor(SHEN_NAME(LRY_));
        for (shen_name prop : guestProps)
            PrepareKshtProp(prop);
        break;
    default:
        break;
    }
}
void RestoreKshtCloudCharacters(std::int32_t selection) {
    switch (selection) {
    case 0:
        RestoreKshtActor(KSHT_RYO);
        RestoreKshtActor(KSHT_SIN);
        break;
    case 1:
        RestoreKshtActor(SHEN_NAME(SNP_));
        RestoreKshtActor(SHEN_NAME(SNM_));
        EV_SetGlobalShadowLightingEnabled(1);
        break;
    case 2:
        RestoreKshtActor(KSHT_RYO);
        RestoreKshtActor(KSHT_SIN);
        RestoreKshtProp(SHEN_NAME(a246));
        RestoreKshtProp(SHEN_NAME(a349));
        EV_DisableCharacterDisplay(SHEN_NAME(a246));
        break;
    case 3:
    case 5:
    case 6:
        RestoreKshtActor(KSHT_RYO);
        RestoreKshtActor(KSHT_SIN);
        for (shen_name prop : cloudProps)
            RestoreKshtProp(prop);
        break;
    case 4:
        RestoreKshtActor(SHEN_NAME(SN5_));
        EV_ToggleCharacterFlag(SHEN_NAME(SN5_));
        EV_DisableCharacterDisplay(SHEN_NAME(SN5_));
        RestoreKshtProp(SHEN_NAME(a345));
        break;
    case 7:
        RestoreKshtActor(SHEN_NAME(IWA_));
        RestoreKshtActor(SHEN_NAME(LRY_));
        for (shen_name prop : guestProps)
            RestoreKshtProp(prop);
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
