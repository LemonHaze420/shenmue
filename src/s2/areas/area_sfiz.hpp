#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name SFIZ_AREA{SHEN_NAME(SFIZ)}, SFIZ_RYO{SHEN_NAME(RYO_)}, SFIZ_IZUMI{SHEN_NAME(XGH_)}, SFIZ_EILEEN{SHEN_NAME(EDL_)};
inline constexpr shen_name SFIZ_BASE{SHEN_NAME(BASE)}, SFIZ_A015{SHEN_NAME(a015)}, SFIZ_A016{SHEN_NAME(a016)}, SFIZ_BOOKIE{SHEN_NAME(COH_)};
inline constexpr shen_name SFIZ_SF9{SHEN_NAME(SF9_)}, SFIZ_SFA{SHEN_NAME(SFA_)}, SFIZ_CAMERA_BUTTON{SHEN_NAME(BTCC)};
inline constexpr shen_name SFIZ_IZUMI_START{SHEN_NAME(IZST)}, SFIZ_EILEEN_START{SHEN_NAME(EDST)};
inline constexpr shen_name SFIZ_IZUMI_WIN{SHEN_NAME(IZWN)}, SFIZ_EILEEN_WIN{SHEN_NAME(EDWN)}, SFIZ_IZUMI_QTE{SHEN_NAME(ZETU)};
inline constexpr shen_name SFIZ_CHARACTERS[]{SFIZ_RYO, SFIZ_IZUMI, SFIZ_EILEEN, SFIZ_SF9, SFIZ_SFA, SFIZ_BOOKIE, EVENT_CHARACTER_INVALID};
inline constexpr shen_name SFIZ_LIT_MODELS[]{SHEN_NAME(DRT0), SHEN_NAME(DRT1), SHEN_NAME(DRT2), SHEN_NAME(TITL)};
inline constexpr shen_name SFIZ_REFBOARD_CHARACTERS[]{SHEN_NAME(ST01), SHEN_NAME(ST03), SHEN_NAME(ST05), SHEN_NAME(ST07), SHEN_NAME(ST09), SHEN_NAME(ST11),
    SHEN_NAME(ST13), SHEN_NAME(ST15), SHEN_NAME(ST17), SHEN_NAME(ST19), SHEN_NAME(ST02), SHEN_NAME(ST04), SHEN_NAME(ST06), SHEN_NAME(ST08), SHEN_NAME(ST10),
    SHEN_NAME(ST12), SHEN_NAME(ST14), SHEN_NAME(ST16), SHEN_NAME(ST18), SHEN_NAME(ST20)};
inline constexpr Vector4 SFIZ_REFBOARD_COLOUR{1.0f, 0.800000011920929f, 0.800000011920929f, 0.800000011920929f};
inline constexpr Vector3 SFIZ_BATTLE_CAMERA{207.1999969482422f, 109.30000305175781f, 92.0f};
inline constexpr Vector3 SFIZ_BATTLE_TARGET{207.1999969482422f, 108.0999984741211f, 87.5f};
inline constexpr Vector3 SFIZ_EILEEN_DEFEAT_POSITION{206.89999389648438f, 107.0f, 86.4000015258789f};
inline constexpr Vector3 SFIZ_IZUMI_DEFEAT_POSITION{206.89999389648438f, 107.0999984741211f, 86.4000015258789f};
inline constexpr Vector3 SFIZ_OPPONENT_POSITION{205.8000030517578f, 107.0999984741211f, 87.80000305175781f};
inline constexpr Vector3 SFIZ_PLAYER_POSITION{208.5f, 107.0999984741211f, 87.80000305175781f};
inline constexpr Vector3 SFIZ_EXIT_POSITION{203.8000030517578f, 103.94999694824219f, 89.5f};
inline constexpr Vector3 SFIZ_BOOKIE_POSITION{208.5f, 107.0999984741211f, 92.9000015258789f};
inline constexpr Vector3 SFIZ_CONVERSATION_PLAYER_POSITION{207.6999969482422f, 107.0999984741211f, 94.19999694824219f};
inline constexpr Vector3 SFIZ_LEAF_POSITION{207.39999389648438f, 108.80000305175781f, 92.9000015258789f};
inline constexpr Vector3 SFIZ_LEAF_VOLUME{2.5f, 0.019999999552965164f, 2.5f};
inline constexpr Vector3 SFIZ_LEAF_VELOCITY{0.009999999776482582f, 0.014999999664723873f, 0.009999999776482582f};
inline constexpr Vector3 SFIZ_LEAF_VARIANCE{0.009999999776482582f, 0.004999999888241291f, 0.009999999776482582f};
inline constexpr const char *SFIZ_IZUMI_DEFEAT_VOICES[]{"C004", "C005", "C006", "C007", "C008", "C009", "C010"};
inline constexpr const char *SFIZ_EILEEN_DEFEAT_VOICES[]{"B025", "B026", "B027", "B028", "B029", "B030", "B031", "B032"};
enum class IzumiEventState : std::uint16_t {
    Load,
    Offer,
    InsufficientCash,
    Decline,
    AcceptAuthoredFight,
    PayDemonstration,
    Demonstration,
    Fight,
    StartAuthor,
    VictoryAuthor,
    VictoryReply,
    Defeat,
    DefeatReply,
    End
};
struct IzumiEventWork {
    HLib::Task *task;
    shen_name opponent;
    IzumiEventState state;
    std::int16_t frame;
    EventReadCharactersWork *characters;
};
static_assert(sizeof(IzumiEventWork) == 0x18);
static_assert(offsetof(IzumiEventWork, opponent) == 8 && offsetof(IzumiEventWork, state) == 12);
static_assert(offsetof(IzumiEventWork, frame) == 14 && offsetof(IzumiEventWork, characters) == 16);
extern const ScriptAreaDefinition areaSfizScriptDefinition;
} // namespace shenmue::s2
