#include "s2/areas/area_ytqb.hpp"
#include "s2/event_gameplay_queued.hpp"
#include <bit>
namespace shenmue::s2::areas {
void YtqbBeginKick(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.11149f : 0.11149f;
    EVT_AT_SetPos(p.firstOpponent, p.player, {.x = lateral, .y = 0.0f, .z = -2.52247f}, delay);
    EVT_AT_SetAngY(p.firstOpponent, p.player, 180.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_ST_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.191f : -0.191f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -1.4969f, 180.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 17);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_KERI_SEIKOU_1QTE, 8.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 0xCu, 1u, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x28Du, delay + 13);
    EVT_AT_SetSound(0x282u, delay + 58);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_SEIKOU_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", 1, delay + 25);
}
void YtqbKickFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.0096899997f : 0.0096899997f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -1.1624399f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_KERI_SIPPAI_1QTE, 0.0f, 1.0f, p.motionFlags, delay + 5);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA008", 1, delay + 8);
    EVT_AT_SetSound(0x28Cu, delay + 8);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_SIPPAI_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB009", 1, delay);
}
void YtqbKickCounterSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.02815f : 0.02815f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.87369001f, 180.0f, delay);
    EVT_AT_SetSound(0x28Bu, delay + 13);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SEIKOU_1QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 8u, 0xCu, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA001", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 34);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SEIKOU_1QTE, 31.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 16);
}
void YtqbKickCounterFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.16294f : 0.16294f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.71368998f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_1QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA012", 1, delay + 7);
    EVT_AT_SetSound(0x28Au, delay + 3);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_1QTE, 34.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB013", 1, delay);
}
void YtqbBeginRearKick(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.15888999f : -0.15888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.22727f, 0.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbRearKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.13082001f : -0.13082001f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 2.1758299f, 0.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 11);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SEIKOU_2QTE, 15.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 0xCu, 1u, 4, delay + 10);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 34);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE, 35.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB028", 1, delay + 16);
}
void YtqbRearKickFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.12888999f : -0.12888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.0372699f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", 1, delay + 20);
    EVT_AT_SetSound(0x28Du, delay + 10);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB001", 1, delay);
}
void YtqbBeginWristTwist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.61176997f : 0.61176997f;
    EVT_AT_SetPos(p.firstOpponent, p.player, {.x = lateral, .y = 0.0f, .z = -3.44645f}, delay);
    EVT_AT_SetAngY(p.firstOpponent, p.player, 180.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_ST_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbWristTwistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.17725f : 0.17725f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.1332099f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_KOTEKAESI_A_3QTE, 10.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 8u, 0xCu, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x28Du, delay + 6);
    EVT_AT_SetSound(0x282u, delay + 56);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KOTEKAESI_A_3QTE, 10.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
}
void YtqbBeginBackfist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = 1.4450901f, .y = 0.0f, .z = -1.41609f}, delay);
        yaw = 120.0f;
    } else {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = -1.4450901f, .y = 0.0f, .z = -1.41609f}, delay);
        yaw = -120.0f;
    }
    EVT_AT_SetAngY(p.firstOpponent, p.player, yaw, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbBackfistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = 120.0f;
        secondLateral = 1.4450901f;
    } else {
        lateral = -120.0f;
        secondLateral = -1.4450901f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -2.41609f, lateral, delay);
    EVT_AT_SetSound(0x6B7u, delay + 16);
    EVT_AT_SetSound(0x28Bu, delay + 31);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SEIKOU_5QTE, 6.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 8u, 0xCu, 1u, 6, delay + 15);
    EVT_AT_SetVoice("S2BVA005", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 47);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 40.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB022", 1, delay + 37);
}
void YtqbBackfistFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        secondLateral = 120.0f;
        lateral = 1.08961f;
    } else {
        secondLateral = -120.0f;
        lateral = -1.08961f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -1.7349401f, secondLateral, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_5QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA010", 1, delay + 25);
    EVT_AT_SetSound(0x28Cu, delay + 15);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_5QTE, 40.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbKneeCounterSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = 120.0f;
        secondLateral = 0.57709998f;
    } else {
        lateral = -120.0f;
        secondLateral = -0.57709998f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -1.22592f, lateral, delay);
    EVT_AT_SetSound(649u, delay + 25);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_HIZA_SEIKOU_5QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 12u, 1u, 6, delay + 10);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 70);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_HIZA_SEIKOU_5QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB030", 1, delay + 30);
}
void YtqbKneeCounterFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        secondLateral = 120.0f;
        lateral = 0.54641998f;
    } else {
        secondLateral = -120.0f;
        lateral = -0.54641998f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.94591999f, secondLateral, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_HIZA_SIPPAI_5QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA011", 1, delay + 23);
    EVT_AT_SetSound(0x28Cu, delay + 23);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_HIZA_SIPPAI_5QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB007", 1, delay); // @text EN: "Take this!" | JA: "くらえっ！"
}
void YtqbBeginPin(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = -1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, 246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = 2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = 90.0f;
    } else {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = 1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, -246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = -2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = -90.0f;
    }
    EVT_AT_SetAngY(p.secondOpponent, p.player, yaw, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SIPPAI_6QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbPinSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral, firstYaw, secondYaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        firstYaw = 270.0f;
        lateral = -2.2121899f;
    } else {
        firstYaw = -270.0f;
        lateral = 2.2121899f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.37863001f, firstYaw, delay);
    if ((p.motionFlags & 0x20000000) != 0) {
        secondYaw = -90.0f;
        secondLateral = -2.98405f;
    } else {
        secondYaw = 90.0f;
        secondLateral = 2.98405f;
    }
    EVT_AT_SetPosAng(p.secondOpponent, p.player, secondLateral, -0.31086001f, secondYaw, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_6QTE, 10.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x28Du, delay + 5);
    EVT_AT_SetSound(0x602u, delay + 10);
    EVT_AT_SetSound(0x6E7u, delay + 30);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_6QTE, 26.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetMot(p.secondOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_B_6QTE, 0.0f, 1.0f, p.motionFlags, delay + 7);
    EVT_AT_SetVoice("S2BVB021", 1, delay + 20);
}
void YtqbPinFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = -1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, 246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = 2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = 90.0f;
    } else {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = 1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, -246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = -2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = -90.0f;
    }
    EVT_AT_SetAngY(p.secondOpponent, p.player, yaw, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SIPPAI_6QTE, 35.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA009", 1, delay + 26);
    EVT_AT_SetSound(0x28Cu, delay + 20);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SIPPAI_6QTE, 6.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB009", 1, delay);
}
void YtqbPinKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral, firstYaw, secondYaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        firstYaw = 270.0f;
        lateral = -0.29901001f;
    } else {
        firstYaw = -270.0f;
        lateral = 0.29901001f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.25656f, firstYaw, delay);
    if ((p.motionFlags & 0x20000000) != 0) {
        secondYaw = -90.0f;
        secondLateral = -2.98405f;
    } else {
        secondYaw = 90.0f;
        secondLateral = 2.98405f;
    }
    EVT_AT_SetPosAng(p.secondOpponent, p.player, secondLateral, -0.31086001f, secondYaw, delay);
    EVT_AT_SetSound(0x289u, delay + 14);
    EVT_AT_SetSound(0x28Bu, delay + 36);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_KERI_SEIKOU_6QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 0xCu, 1u, 6, delay + 10);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 76);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_A_6QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 20);
    EVT_AT_SetSound(0x282u, delay + 33);
    EVT_AT_SetMot(p.secondOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_B_6QTE, 25.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB020", 1, delay + 40);
}
void YtqbPinKickFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral, firstYaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        firstYaw = 270.0f;
        lateral = -0.50663f;
    } else {
        firstYaw = -270.0f;
        lateral = 0.50663f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.024639999f, firstYaw, delay);
    if ((p.motionFlags & 0x20000000) != 0)
        secondLateral = -1.85807f;
    else
        secondLateral = 1.85807f;
    EVT_AT_SetPosAng(p.secondOpponent, p.player, secondLateral, 0.77458f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_KERI_SIPPAI_6QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA007", 1, delay + 28);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SIPPAI_A_6QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetSound(0x28Au, delay + 21);
    EVT_AT_SetMot(p.secondOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SIPPAI_B_6QTE, 22.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB013", 1, delay);
}
void YtqbBeginGrabEvadeA(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_A_QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbGrabEvadeASuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.0098799998f : -0.0098799998f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.2938099f, 180.0f, delay);
    EVT_AT_SetSound(0x28Bu, delay + 20);
    EVT_AT_SetSound(0x6B7u, delay + 8);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SEIKOU_A_7QTE, 10.0f, 1.0f, (p.motionFlags ^ 0x20000000), delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 6, delay + 17);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 42);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_A_QTE, 12.0f, 1.0f, (p.motionFlags ^ 0x20000000 | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB026", 1, delay + 30);
}
void YtqbGrabEvadeAFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.17863999f : 0.17863999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.2607501f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetVoice("S2BVB004", 1, delay);
}
void YtqbBeginGrabEvadeB(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbGrabEvadeBSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.00079000002f : 0.00079000002f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.30879f, 180.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 27);
    EVT_AT_SetSound(0x6B7u, delay + 12);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 4, delay + 25);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 41);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE, 16.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", 1, delay + 30);
}
void YtqbGrabEvadeBFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.17863999f : 0.17863999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.1607499f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetVoice("S2BVB006", 1, delay);
}
void YtqbThrowASuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.0557f : 0.0557f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.77547997f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_A_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 60);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_A_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 50);
}
void YtqbThrowAFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.25895f : 0.25895f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.87206f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA012", 1, delay + 15);
    EVT_AT_SetSound(0x28Au, delay + 12);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbThrowBSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.0046600001f : 0.0046600001f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.69023001f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 71);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_B_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB018", 1, delay + 60);
}
void YtqbThrowBFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.25895f : 0.25895f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.87206f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA009", 1, delay + 15);
    EVT_AT_SetSound(0x28Au, delay + 12);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB003", 1, delay);
}
void YtqbBeginKickDodge(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KICKSABAKI_SEIKOU_A_8QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbKickDodgeSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.0049399999f : -0.0049399999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.75157f, 180.0f, delay);
    EVT_AT_SetSound(0x28Bu, delay + 22);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_AKI_KICKSABAKI_SEIKOU_A_8QTE, 10.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA004", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 38);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KICKSABAKI_SEIKOU_A_8QTE, 10.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 32);
}
void YtqbKickDodgeFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.0058300002f : -0.0058300002f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.2780299f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_AKI_KICKSABAKI_SIPPAI_8QTE, 14.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_8QTE, 0.0f, 1.0f, p.motionFlags, delay + 2);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA008", 1, delay + 12);
    EVT_AT_SetSound(0x28Au, delay + 2);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KICKSABAKI_SIPPAI_8QTE, 14.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_8QTE, 0.0f, 1.0f, p.motionFlags, delay + 2);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbKickDodgeCounterSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.15020999f : -0.15020999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.90296f, 180.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 18);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SEIKOU_8QTE, 30.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA005", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 36);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SEIKOU_8QTE, 30.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB018", 1, delay + 28);
}
void YtqbKickDodgeCounterFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.15020999f : -0.15020999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.88231999f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_8QTE, 43.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA010", 1, delay + 28);
    EVT_AT_SetSound(0x28Cu, delay + 18);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_8QTE, 40.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB002", 1, delay);
}
void YtqbFlyingKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = -270.0f;
        secondLateral = 2.2537f;
    } else {
        lateral = 270.0f;
        secondLateral = -2.2537f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -0.79279f, lateral, delay);
    EVT_AT_SetSound(0x289u, delay + 22);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HISSATU_TOBIGERI_SEIKOU_QTE_NEW0300, 0.0f, 1.0f, (p.motionFlags ^ 0x20000000), delay);
    EVT_AT_DispBlur(p.player, 1u, 6u, 1u, 50, delay);
    EVT_AT_SetVoice("S2BVA004", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 47);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 47.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB022", 1, delay + 30);
}
void YtqbFinishingBackfistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = 90.0f;
        secondLateral = 2.36129f;
    } else {
        lateral = -90.0f;
        secondLateral = -2.36129f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -1.06471f, lateral, delay);
    EVT_AT_SetSound(0x28Bu, delay + 18);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HISSATU_URAKEN_SEIKOU_QTE_NEW0300, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 2u, 0xCu, 1u, 10, delay + 10);
    EVT_AT_SetVoice("S2BVA004", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 40);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 52.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 20);
}
void YtqbBeginRearKickAssist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.00978f : 0.00978f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 2.6208401f, 0.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbRearKickAssistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    EVT_AT_SetAngY(p.player, p.player, 200.0f, delay);
    if ((p.motionFlags & 0x20000000) != 0)
        lateral = 0.00978f;
    else
        lateral = -0.00978f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.6208401f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_BATTLE_B_0, 138.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetVoice("S2BVA001", 1, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE, 29.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetSound(0x289u, delay + 15);
    EVT_AT_SetSound(0x282u, delay + 40);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", 1, delay + 20);
}
void YtqbRearKickAssistFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.12888999f : -0.12888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.0372699f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", 1, delay + 20);
    EVT_AT_SetSound(0x28Du, delay + 10);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbBeginBackfistAssist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetAngY(p.player, p.player, 270.0f, delay);
        EVT_AT_SetPosAng(p.firstOpponent, p.player, 1.14463f, 2.0968699f, 0.0f, delay);
        yaw = -270.0f;
    } else {
        EVT_AT_SetAngY(p.player, p.player, -270.0f, delay);
        EVT_AT_SetPosAng(p.firstOpponent, p.player, -1.14463f, 2.0968699f, 0.0f, delay);
        yaw = 270.0f;
    }
    EVT_AT_SetAngY(p.player, p.player, yaw, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbBackfistAssistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetAngY(p.player, p.player, 270.0f, delay);
        lateral = 1.14463f;
    } else {
        EVT_AT_SetAngY(p.player, p.player, -270.0f, delay);
        lateral = -1.14463f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 2.0968699f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OTASUKE_E_0, 60.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 49.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetSound(0x28Cu, delay + 18);
    EVT_AT_SetSound(0x282u, delay + 40);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 20);
}
void YtqbBackfistAssistFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.12888999f : -0.12888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.19727f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", 1, delay + 20);
    EVT_AT_SetSound(0x28Du, delay + 10);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB004", 1, delay);
}
} // namespace shenmue::s2::areas
