#pragma once
#include "s2/area_event.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_movement.hpp"
#include "s2/event_call.hpp"
#include "s2/event_queued.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name YTQB_YTQB{SHEN_NAME(YTQB)};
inline constexpr shen_name YTQB_EVNT{SHEN_NAME(EVNT)};
inline constexpr shen_name YTQB_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name YTQB_DUMR{SHEN_NAME(DUMR)};
inline constexpr shen_name YTQB_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name YTQB_SBO_{SHEN_NAME(SBO_)};
inline constexpr shen_name YTQB_YOH_{SHEN_NAME(YOH_)};
inline constexpr shen_name YTQB_SCX_{SHEN_NAME(SCX_)};
inline constexpr shen_name YTQB_SBK_{SHEN_NAME(SBK_)};
inline constexpr shen_name YTQB_VUN_{SHEN_NAME(VUN_)};
inline constexpr shen_name YTQB_IKO_{SHEN_NAME(IKO_)};
inline constexpr shen_name YTQB_KIC_{SHEN_NAME(KIC_)};
inline constexpr shen_name YTQB_RIK_{SHEN_NAME(RIK_)};
inline constexpr shen_name YTQB_THG_{SHEN_NAME(THG_)};
inline constexpr shen_name YTQB_SOY_{SHEN_NAME(SOY_)};
inline constexpr shen_name YTQB_KT2_{SHEN_NAME(KT2_)};
inline constexpr shen_name YTQB_RNT_{SHEN_NAME(RNT_)};
inline constexpr shen_name YTQB_KT3_{SHEN_NAME(KT3_)};
inline constexpr shen_name YTQB_EHY_{SHEN_NAME(EHY_)};
inline constexpr shen_name YTQB_ANC_{SHEN_NAME(ANC_)};
inline constexpr shen_name YTQB_KT5_{SHEN_NAME(KT5_)};
inline constexpr shen_name YTQB_CJU_{SHEN_NAME(CJU_)};
inline constexpr shen_name YTQB_KT7_{SHEN_NAME(KT7_)};
inline constexpr shen_name YTQB_MRC_{SHEN_NAME(MRC_)};
inline constexpr shen_name YTQB_SEE_{SHEN_NAME(SEE_)};
inline constexpr shen_name YTQB_III_{SHEN_NAME(III_)};
inline constexpr shen_name YTQB_RKN_{SHEN_NAME(RKN_)};
inline constexpr shen_name YTQB_ESY_{SHEN_NAME(ESY_)};
inline constexpr shen_name YTQB_GEN_{SHEN_NAME(GEN_)};
inline constexpr shen_name YTQB_CHO_{SHEN_NAME(CHO_)};
inline constexpr shen_name YTQB_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name YTQB_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name YTQB_CAG0{SHEN_NAME(CAG0)};
inline constexpr shen_name YTQB_KBK0{SHEN_NAME(KBK0)};
inline constexpr shen_name YTQB_SUI0{SHEN_NAME(SUI0)};
inline constexpr shen_name YTQB_SUI1{SHEN_NAME(SUI1)};
inline constexpr shen_name YTQB_SUI2{SHEN_NAME(SUI2)};
inline constexpr shen_name YTQB_YAD1{SHEN_NAME(YAD1)};
inline constexpr shen_name YTQB_YAD2{SHEN_NAME(YAD2)};
inline constexpr shen_name YTQB_YAD3{SHEN_NAME(YAD3)};
inline constexpr shen_name YTQB_AHI0{SHEN_NAME(AHI0)};
inline constexpr shen_name YTQB_AHI1{SHEN_NAME(AHI1)};
inline constexpr shen_name YTQB_NIW0{SHEN_NAME(NIW0)};
inline constexpr shen_name YTQB_NIW1{SHEN_NAME(NIW1)};
inline constexpr shen_name YTQB_NIW2{SHEN_NAME(NIW2)};
inline constexpr shen_name YTQB_BIR0{SHEN_NAME(BIR0)};
inline constexpr shen_name YTQB_BIR1{SHEN_NAME(BIR1)};
inline constexpr shen_name YTQB_BIR2{SHEN_NAME(BIR2)};
inline constexpr shen_name YTQB_0000{SHEN_NAME(0000)};
inline constexpr shen_name YTQB_0048{SHEN_NAME(0048)};
inline constexpr shen_name YTQB_l201{SHEN_NAME(l201)};
inline constexpr shen_name YTQB_l203{SHEN_NAME(l203)};
inline constexpr shen_name YTQB_l204{SHEN_NAME(l204)};
inline constexpr shen_name YTQB_l205{SHEN_NAME(l205)};
inline constexpr shen_name YTQB_ATH1{SHEN_NAME(ATH1)};
inline constexpr shen_name YTQB_ATH3{SHEN_NAME(ATH3)};
inline constexpr shen_name YTQB_ATH4{SHEN_NAME(ATH4)};
inline constexpr shen_name YTQB_ATH5{SHEN_NAME(ATH5)};
inline constexpr shen_name YTQB_STR0{SHEN_NAME(STR0)};
inline constexpr shen_name YTQB_INVALID{0xffffffffU};
inline constexpr std::uint32_t YTQB_RYOM_RYO_B_BATTLE_KAMAE_LP = 13;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_SIPAII_YARARE_W2QTE = 5674;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_DOWN_B_0 = 5708;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_DOWN_SIDE_R_M = 5709;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_DOWN_C_0 = 5710;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_KERI_ST_1QTE = 5588;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_KERI_SEIKOU_1QTE = 5658;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_KERI_SEIKOU_1QTE = 5586;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_KERI_SIPPAI_1QTE = 5660;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_KERI_SIPPAI_1QTE = 5587;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_HANGEKI_SEIKOU_1QTE = 5639;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_HANGEKI_SEIKOU_1QTE = 5571;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_1QTE = 5641;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_1QTE = 5573;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE = 5632;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_USIROKERI_SEIKOU_2QTE = 5697;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE = 5631;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE = 5698;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_KOTEKAESI_A_3QTE = 5661;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_KOTEKAESI_A_3QTE = 5593;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE = 5627;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_URAKEN_SEIKOU_5QTE = 5691;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_5QTE = 5693;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_5QTE = 5628;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_HIZA_SEIKOU_5QTE = 5694;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_HIZA_SEIKOU_5QTE = 5629;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_HIZA_SIPPAI_5QTE = 5695;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_HIZA_SIPPAI_5QTE = 5630;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_OSAE_SIPPAI_6QTE = 5666;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_6QTE = 5662;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_6QTE = 5595;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_B_6QTE = 5597;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_OSAE_SIPPAI_6QTE = 5600;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_KERI_SEIKOU_6QTE = 5663;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_A_6QTE = 5596;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_KERI_SIPPAI_6QTE = 5665;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SIPPAI_A_6QTE = 5598;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SIPPAI_B_6QTE = 5599;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_A_QTE = 5618;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SEIKOU_A_7QTE = 5683;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_7QTE = 5685;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE = 5689;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_7QTE = 5620;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE = 5624;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE = 5619;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE = 5684;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_A_7QTE = 5686;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_A_7QTE = 5621;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE = 5688;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE = 5623;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_B_7QTE = 5687;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_B_7QTE = 5622;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_KICKSABAKI_SEIKOU_A_8QTE = 5589;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_AKI_KICKSABAKI_SEIKOU_A_8QTE = 5634;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_AKI_KICKSABAKI_SIPPAI_8QTE = 5636;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_8QTE = 5642;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_KICKSABAKI_SIPPAI_8QTE = 5591;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_8QTE = 5574;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_HANGEKI_SEIKOU_8QTE = 5640;
inline constexpr std::uint32_t YTQB_LLYM_XXX_0048E_HANGEKI_SEIKOU_8QTE = 5572;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_HISSATU_TOBIGERI_SEIKOU_QTE_NEW0300 = 5647;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_HISSATU_URAKEN_SEIKOU_QTE_NEW0300 = 5648;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_BATTLE_B_0 = 5699;
inline constexpr std::uint32_t YTQB_RYOM_RYO_0048E_OTASUKE_E_0 = 5700;
struct YtqbResultParameters {
    shen_name player, firstOpponent, secondOpponent;
    std::uint32_t motionFlags;
};
static_assert(sizeof(YtqbResultParameters) == 0x10 && offsetof(YtqbResultParameters, firstOpponent) == 4);
static_assert(offsetof(YtqbResultParameters, secondOpponent) == 8 && offsetof(YtqbResultParameters, motionFlags) == 0x0c);
struct YtqbWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    std::uint64_t reserved10;
    HLib::TaskCallback continuation;
    YtqbResultParameters parameters;
    std::int32_t phase, result;
    const EventQteData *prompt;
    const EventQteData *authored;
    EventNpcReadWork *npcs;
};
static_assert(sizeof(YtqbWork) == 0x50);
static_assert(offsetof(YtqbWork, characters) == 0 && offsetof(YtqbWork, task) == 8 && offsetof(YtqbWork, reserved10) == 0x10);
static_assert(offsetof(YtqbWork, continuation) == 0x18 && offsetof(YtqbWork, parameters) == 0x20);
static_assert(offsetof(YtqbWork, phase) == 0x30 && offsetof(YtqbWork, result) == 0x34);
static_assert(offsetof(YtqbWork, prompt) == 0x38 && offsetof(YtqbWork, authored) == 0x40 && offsetof(YtqbWork, npcs) == 0x48);
inline constexpr Vector3 YTQB_WorldBase{.x = 180.60000610351562f, .y = 102.25f, .z = 198.1999969482422f};
inline constexpr Vector3 YTQB_ScxPlayer{.x = -0.6800000071525574f, .y = 0.0f, .z = -0.5299999713897705f};
inline constexpr Vector3 YTQB_SbkPlayer{.x = 2.0f, .y = 0.0f, .z = -10.5f};
inline constexpr Vector3 YTQB_VunPlayer{.x = 3.700000047683716f, .y = 0.0f, .z = -12.100000381469727f};
inline constexpr Vector3 YTQB_VunWaiting{.x = 4.650000095367432f, .y = 0.0f, .z = -12.5f};
inline constexpr Vector3 YTQB_IkoWaiting{.x = 3.140000104904175f, .y = 0.0f, .z = -13.5f};
inline constexpr Vector3 YTQB_WaterPlayer{.x = 7.0f, .y = 0.0f, .z = -16.299999237060547f};
inline constexpr Vector3 YTQB_WaterOpponent{.x = -0.15000000596046448f, .y = 0.0f, .z = -3.5999999046325684f};
inline constexpr Vector3 YTQB_Kt2Player{.x = 12.0f, .y = 0.0f, .z = -22.450000762939453f};
inline constexpr Vector3 YTQB_RntWaiting{.x = 14.5f, .y = 0.0f, .z = -25.700000762939453f};
inline constexpr Vector3 YTQB_Kt3Waiting{.x = 14.899999618530273f, .y = 0.0f, .z = -24.399999618530273f};
inline constexpr Vector3 YTQB_AncPlayer{.x = 13.420000076293945f, .y = 0.0f, .z = -30.559999465942383f};
inline constexpr Vector3 YTQB_EhyWaiting{.x = 13.100000381469727f, .y = 0.0f, .z = -35.20000076293945f};
inline constexpr Vector3 YTQB_CjuWaiting{.x = 13.600000381469727f, .y = 0.0f, .z = -28.5f};
inline constexpr Vector3 YTQB_WaterFarOpponent{.x = 0.5f, .y = 0.0f, .z = -300.6000061035156f};
inline constexpr Vector3 YTQB_YohWaiting{.x = -1.3600000143051147f, .y = 0.0f, .z = -3.7899999618530273f};
inline constexpr Vector3 YTQB_ScxWaiting{.x = 0.7360000014305115f, .y = 0.0f, .z = -3.6600000858306885f};
inline constexpr Vector3 YTQB_GrabOpponentB{.x = -0.0007900000200606883f, .y = 0.0f, .z = -2.3087899684906006f};
inline constexpr Vector3 YTQB_GrabOpponentA{.x = 0.0007900000200606883f, .y = 0.0f, .z = -2.3087899684906006f};
inline constexpr Vector3 YTQB_GrabOpponentEnd{.x = -0.009879999794065952f, .y = 0.0f, .z = -2.2938098907470703f};
inline constexpr Vector3 YTQB_Lantern{.x = 193.89999389648438f, .y = 102.25f, .z = 175.0f};
inline constexpr Vector3 YTQB_Bucket{.x = 191.39999389648438f, .y = 102.25f, .z = 167.5f};
inline constexpr Vector3 YTQB_WaterIdle{.x = 191.0500030517578f, .y = 102.27999877929688f, .z = 178.14999389648438f};
inline constexpr Vector3 YTQB_WaterReset{.x = 189.6999969482422f, .y = 101.55000305175781f, .z = 178.5f};
inline constexpr Vector3 YTQB_WaterFailure{.x = 189.10000610351562f, .y = 101.44999694824219f, .z = 178.60000610351562f};
inline constexpr Vector3 YTQB_FirstPaper{.x = -2.450000047683716f, .y = 1.5499999523162842f, .z = 0.03999999910593033f};
inline constexpr Vector3 YTQB_SecondPaper{.x = -1.8899999856948853f, .y = 1.5499999523162842f, .z = 0.03999999910593033f};
inline constexpr Vector3 YTQB_ThirdPaper{.x = -1.0800000429153442f, .y = 1.5499999523162842f, .z = 0.03999999910593033f};
inline constexpr Vector3 YTQB_FirstBird{.x = -2.049999952316284f, .y = 0.20000000298023224f, .z = -0.5299999713897705f};
inline constexpr Vector3 YTQB_SecondBird{.x = -2.450000047683716f, .y = 0.20000000298023224f, .z = -0.5299999713897705f};
inline constexpr std::int32_t YTQB_LanternParts[]{
    175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157, 166, 154, 173, 168, 153, 165, 160, 176, 171, 164, 159, 161, 156, 180, 181, 182, 177, 178, 179};
inline constexpr std::int32_t YTQB_BirdStallParts[]{172, 178, 162, 183, 158, 167, 168, 170, 171, 173, 175, 177, 180, 184, 190, 181, 161, 194, 152, 157, 153,
    160, 159, 156, 155, 154, 163, 176, 179, 169, 164, 174, 195, 193, 199, 192, 198, 191, 166, 165, 187, 186, 197, 185, 182, 196, 188, 189};
inline constexpr std::int32_t YTQB_NearStallParts[]{
    165, 164, 163, 167, 171, 170, 169, 168, 166, 172, 180, 179, 178, 177, 173, 174, 175, 176, 162, 156, 155, 154, 153, 152, 161, 160, 159, 157, 158};
inline constexpr std::int32_t YTQB_WaterParts[]{152, 158, 155, 157, 154, 153, 160, 159, 156, 161};
inline constexpr std::int32_t YTQB_FrontStallParts[]{173, 172, 171, 170, 169, 168, 167, 174, 180, 179, 178, 177, 176, 175, 166, 158, 157, 156, 155, 154, 153,
    152, 159, 165, 164, 163, 162, 161, 160, 181, 202, 201, 200, 199, 198, 197, 203, 209, 208, 207, 206, 205, 204, 196, 188, 187, 186, 185, 184, 183, 182, 189,
    195, 194, 193, 192, 191, 190};
inline constexpr std::int32_t YTQB_BucketParts[]{152, 153, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
    175, 176, 177, 178, 179, 180, 181, 182, 183, 185, 184, 186, 187, 188, 189};
inline constexpr shen_name YTQB_PlayerNames[]{YTQB_RYO_, YTQB_INVALID};
inline constexpr shen_name YTQB_FirstNames[]{YTQB_SBO_, YTQB_YOH_, YTQB_SCX_, YTQB_ESY_, YTQB_GEN_, YTQB_CHO_, YTQB_INVALID};
inline constexpr shen_name YTQB_SecondNames[]{YTQB_SBK_, YTQB_VUN_, YTQB_IKO_, YTQB_INVALID};
inline constexpr shen_name YTQB_WaterNames[]{YTQB_KIC_, YTQB_RIK_, YTQB_THG_, YTQB_SOY_, YTQB_INVALID};
inline constexpr shen_name YTQB_ThirdNames[]{YTQB_KT2_, YTQB_RNT_, YTQB_KT3_, YTQB_INVALID};
inline constexpr shen_name YTQB_LastNames[]{YTQB_EHY_, YTQB_ANC_, YTQB_KT5_, YTQB_CJU_, YTQB_KT7_, YTQB_INVALID};
inline constexpr shen_name YTQB_FinalNames[]{YTQB_MRC_, YTQB_SEE_, YTQB_III_, YTQB_RKN_, YTQB_ESY_, YTQB_GEN_, YTQB_CHO_, YTQB_SYE_, YTQB_YUA_, YTQB_INVALID};
extern std::int32_t ytqbAmbientSmoke, ytqbFinalAuthorActive;
void YtqbBeginKick(void *, std::uint32_t);
void YtqbKickSuccess(void *, std::uint32_t);
void YtqbKickFailure(void *, std::uint32_t);
void YtqbKickCounterSuccess(void *, std::uint32_t);
void YtqbKickCounterFailure(void *, std::uint32_t);
void YtqbBeginRearKick(void *, std::uint32_t);
void YtqbRearKickSuccess(void *, std::uint32_t);
void YtqbRearKickFailure(void *, std::uint32_t);
void YtqbBeginWristTwist(void *, std::uint32_t);
void YtqbWristTwistSuccess(void *, std::uint32_t);
void YtqbBeginBackfist(void *, std::uint32_t);
void YtqbBackfistSuccess(void *, std::uint32_t);
void YtqbBackfistFailure(void *, std::uint32_t);
void YtqbKneeCounterSuccess(void *, std::uint32_t);
void YtqbKneeCounterFailure(void *, std::uint32_t);
void YtqbBeginPin(void *, std::uint32_t);
void YtqbPinSuccess(void *, std::uint32_t);
void YtqbPinFailure(void *, std::uint32_t);
void YtqbPinKickSuccess(void *, std::uint32_t);
void YtqbPinKickFailure(void *, std::uint32_t);
void YtqbBeginGrabEvadeA(void *, std::uint32_t);
void YtqbGrabEvadeASuccess(void *, std::uint32_t);
void YtqbGrabEvadeAFailure(void *, std::uint32_t);
void YtqbBeginGrabEvadeB(void *, std::uint32_t);
void YtqbGrabEvadeBSuccess(void *, std::uint32_t);
void YtqbGrabEvadeBFailure(void *, std::uint32_t);
void YtqbThrowASuccess(void *, std::uint32_t);
void YtqbThrowAFailure(void *, std::uint32_t);
void YtqbThrowBSuccess(void *, std::uint32_t);
void YtqbThrowBFailure(void *, std::uint32_t);
void YtqbBeginKickDodge(void *, std::uint32_t);
void YtqbKickDodgeSuccess(void *, std::uint32_t);
void YtqbKickDodgeFailure(void *, std::uint32_t);
void YtqbKickDodgeCounterSuccess(void *, std::uint32_t);
void YtqbKickDodgeCounterFailure(void *, std::uint32_t);
void YtqbFlyingKickSuccess(void *, std::uint32_t);
void YtqbFinishingBackfistSuccess(void *, std::uint32_t);
void YtqbBeginRearKickAssist(void *, std::uint32_t);
void YtqbRearKickAssistSuccess(void *, std::uint32_t);
void YtqbRearKickAssistFailure(void *, std::uint32_t);
void YtqbBeginBackfistAssist(void *, std::uint32_t);
void YtqbBackfistAssistSuccess(void *, std::uint32_t);
void YtqbBackfistAssistFailure(void *, std::uint32_t);
inline constexpr EventQteData YTQB_Kick_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginKick,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKickSuccess,
    .resultFailure = YtqbKickFailure};
inline constexpr EventQteData YTQB_Kick_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginKick,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_KickCounter_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 19,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKickCounterSuccess,
    .resultFailure = YtqbKickCounterFailure};
inline constexpr EventQteData YTQB_KickCounter_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 19,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_RearKick_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginRearKick,
    .beginFailure = nullptr,
    .resultSuccess = YtqbRearKickSuccess,
    .resultFailure = YtqbRearKickFailure};
inline constexpr EventQteData YTQB_RearKick_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginRearKick,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_WristTwist_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginWristTwist,
    .beginFailure = nullptr,
    .resultSuccess = YtqbWristTwistSuccess,
    .resultFailure = YtqbKickFailure};
inline constexpr EventQteData YTQB_WristTwist_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginWristTwist,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_WristCounter_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 19,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKickCounterSuccess,
    .resultFailure = YtqbKickCounterFailure};
inline constexpr EventQteData YTQB_WristCounter_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 19,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_Backfist_Authored{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfist,
    .beginFailure = nullptr,
    .resultSuccess = YtqbBackfistSuccess,
    .resultFailure = YtqbBackfistFailure};
inline constexpr EventQteData YTQB_Backfist_Prompt{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfist,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_KneeCounter_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKneeCounterSuccess,
    .resultFailure = YtqbKneeCounterFailure};
inline constexpr EventQteData YTQB_KneeCounter_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_Pin_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 5,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginPin,
    .beginFailure = nullptr,
    .resultSuccess = YtqbPinSuccess,
    .resultFailure = YtqbPinFailure};
inline constexpr EventQteData YTQB_Pin_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 5,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginPin,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_PinKick_Authored{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbPinKickSuccess,
    .resultFailure = YtqbPinKickFailure};
inline constexpr EventQteData YTQB_PinKick_Prompt{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_GrabEvadeA_Authored{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 18,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = YtqbBeginGrabEvadeA,
    .resultSuccess = YtqbGrabEvadeASuccess,
    .resultFailure = YtqbGrabEvadeAFailure};
inline constexpr EventQteData YTQB_GrabEvadeA_Prompt{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 18,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = YtqbBeginGrabEvadeA,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_ThrowA_Authored{.requiredQteStatus = 64,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 15,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbThrowASuccess,
    .resultFailure = YtqbThrowAFailure};
inline constexpr EventQteData YTQB_ThrowA_Prompt{.requiredQteStatus = 64,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 15,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_GrabEvadeB_Authored{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 13,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = YtqbBeginGrabEvadeB,
    .resultSuccess = YtqbGrabEvadeBSuccess,
    .resultFailure = YtqbGrabEvadeBFailure};
inline constexpr EventQteData YTQB_GrabEvadeB_Prompt{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 13,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = YtqbBeginGrabEvadeB,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_ThrowB_Authored{.requiredQteStatus = 64,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 15,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbThrowBSuccess,
    .resultFailure = YtqbThrowBFailure};
inline constexpr EventQteData YTQB_ThrowB_Prompt{.requiredQteStatus = 64,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 15,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_KickDodge_Authored{.requiredQteStatus = 1024,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 18,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = YtqbBeginKickDodge,
    .resultSuccess = YtqbKickDodgeSuccess,
    .resultFailure = YtqbKickDodgeFailure};
inline constexpr EventQteData YTQB_KickDodge_Prompt{.requiredQteStatus = 1024,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 18,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = YtqbBeginKickDodge,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_KickDodgeCounter_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 14,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKickDodgeCounterSuccess,
    .resultFailure = YtqbKickDodgeCounterFailure};
inline constexpr EventQteData YTQB_KickDodgeCounter_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 14,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_FlyingKick_Authored{.requiredQteStatus = 1024,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfist,
    .beginFailure = nullptr,
    .resultSuccess = YtqbFlyingKickSuccess,
    .resultFailure = YtqbBackfistFailure};
inline constexpr EventQteData YTQB_FlyingKick_Prompt{.requiredQteStatus = 1024,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfist,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_FlyingKickCounter_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKneeCounterSuccess,
    .resultFailure = YtqbKneeCounterFailure};
inline constexpr EventQteData YTQB_FlyingKickCounter_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_FinishingBackfist_Authored{.requiredQteStatus = 1024,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfist,
    .beginFailure = nullptr,
    .resultSuccess = YtqbFinishingBackfistSuccess,
    .resultFailure = YtqbBackfistFailure};
inline constexpr EventQteData YTQB_FinishingBackfist_Prompt{.requiredQteStatus = 1024,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 2,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfist,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_FinishingBackfistCounter_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = YtqbKneeCounterSuccess,
    .resultFailure = YtqbKneeCounterFailure};
inline constexpr EventQteData YTQB_FinishingBackfistCounter_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 9,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = nullptr,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_RearKickAssist_Authored{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginRearKickAssist,
    .beginFailure = nullptr,
    .resultSuccess = YtqbRearKickAssistSuccess,
    .resultFailure = YtqbRearKickAssistFailure};
inline constexpr EventQteData YTQB_RearKickAssist_Prompt{.requiredQteStatus = 256,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginRearKickAssist,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
inline constexpr EventQteData YTQB_BackfistAssist_Authored{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfistAssist,
    .beginFailure = nullptr,
    .resultSuccess = YtqbBackfistAssistSuccess,
    .resultFailure = YtqbBackfistAssistFailure};
inline constexpr EventQteData YTQB_BackfistAssist_Prompt{.requiredQteStatus = 512,
    .retryReferenceFrames = 24,
    .maxTime = 16,
    .callbackTimeA = 0,
    .callbackTimeB = 0,
    .callbackTimeC = 0,
    .authoredWord = 0,
    .beginSuccess = YtqbBeginBackfistAssist,
    .beginFailure = nullptr,
    .resultSuccess = nullptr,
    .resultFailure = nullptr};
void YtqbStart(void *);
void YtqbStartIntroAuthor(void *);
void YtqbStartWaterAuthor(void *);
void YtqbWaitWaterAuthor(void *);
void YtqbWaterResult(void *);
void YtqbStartFinalAuthor(void *);
void YtqbWaitFinalAuthor(void *);
void YtqbFinalResult(void *);
void YtqbWaitResultAuthor(void *);
void YtqbFinish(void *);
void YtqbSboAttack(void *);
void YtqbYohAttack(void *);
void YtqbScxAttack(void *);
void YtqbSbkAttack(void *);
void YtqbVunAttack(void *);
void YtqbStartSecondGroup(void *);
void YtqbIkoAttack(void *);
void YtqbSoyAttack(void *);
void YtqbKicAttack(void *);
void YtqbRikThgAttack(void *);
void YtqbKt2Attack(void *);
void YtqbRntAttack(void *);
void YtqbKt3Attack(void *);
void YtqbAncAttack(void *);
void YtqbKt5Attack(void *);
void YtqbCjuAttack(void *);
void YtqbKt7Attack(void *);
void YtqbFinalEhyAttack(void *);
void YtqbWaitMotion(void *);
void YtqbNextQte(void *);
void YtqbRetry(void *);
void YtqbLoadFinalCharacters(void *);
void YtqbSetup(YtqbWork &);
void YtqbCleanup(YtqbWork &);
void YtqbExit(YtqbWork &);
void YtqbSetupLantern();
void YtqbSetupBucket();
void YtqbSetupWater();
void YtqbResetWater();
void YtqbSetupStallObjects();
std::int32_t YtqbBreakLantern();
std::int32_t YtqbBreakBucket();
std::int32_t YtqbBreakFrontStall();
std::int32_t YtqbBreakNearStall();
std::int32_t YtqbBreakBirdStall();
std::int32_t YtqbWaterSuccess();
std::int32_t YtqbWaterFailure();
void YtqbKnockdown(float, std::int32_t);
void YtqbBackKnockdown(float);
void YtqbSideKnockdown(float);
void YtqbFinalKnockdown(float);
void YtqbAmbientSmoke(void *, std::int32_t);
void YtqbSplashUpdate(void *, std::int32_t);
void YtqbBirdStallUpdate(void *, std::int32_t);
void YtqbPaperStallUpdate(void *, std::int32_t);
extern const ScriptAreaDefinition ytqbScriptDefinition;
void YtqbCamera(std::int32_t, std::uint8_t = 0, float = 0);
void YtqbPosition(shen_name, shen_name, const Vector3 &);
void YtqbAngle(shen_name, shen_name, float);
void YtqbStand(shen_name);
void YtqbPrompt(YtqbWork &, const EventQteData &, const EventQteData &);
void YtqbApplyResult(YtqbWork &);
void YtqbTransition(YtqbWork &, HLib::TaskCallback, std::int32_t, HLib::TaskCallback = nullptr);
} // namespace shenmue::s2::areas
