#pragma once

#include "s2/area_jump_dc.hpp"
#include "s2/area_names.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

static const AreaJumpDestinationEntry areaJumpDestinations[] = {
    {-1, {SHEN_FIELD(specialMode) 0}, 0},
    {165, {SHEN_FIELD(areaId) AREA_AR03}, 3},
    {165, {SHEN_FIELD(areaId) AREA_AR02}, 26},
    {166, {SHEN_FIELD(areaId) AREA_AR02}, 5},
    {167, {SHEN_FIELD(areaId) AREA_AK00}, 1},
    {168, {SHEN_FIELD(areaId) AREA_AB00}, 95},
    {169, {SHEN_FIELD(areaId) AREA_WB00}, 1},
    {170, {SHEN_FIELD(areaId) AREA_WT00}, 1},
    {170, {SHEN_FIELD(areaId) AREA_WT00}, 80},
    {171, {SHEN_FIELD(areaId) AREA_WTA0}, 1},
    {172, {SHEN_FIELD(areaId) AREA_WK00}, 1},
    {172, {SHEN_FIELD(areaId) AREA_WK00}, 3},
    {173, {SHEN_FIELD(areaId) AREA_WS00}, 79},
    {173, {SHEN_FIELD(areaId) AREA_WS00}, 2},
    {174, {SHEN_FIELD(areaId) AREA_WS00}, 1},
    {233, {SHEN_FIELD(specialMode) 2}, 0},
    {253, {SHEN_FIELD(specialMode) 1}, 0},
    {249, {SHEN_FIELD(areaId) AREA_Q100}, 1},
    {250, {SHEN_FIELD(areaId) AREA_Q200}, 6},
    {251, {SHEN_FIELD(areaId) AREA_QR00}, 80},
    {253, {SHEN_FIELD(specialMode) 0}, 0},
    {174, {SHEN_FIELD(areaId) AREA_WSY0}, 50},
    {255, {SHEN_FIELD(areaId) AREA_WK00}, 32},
    {256, {SHEN_FIELD(areaId) AREA_WN00}, 28},
    {233, {SHEN_FIELD(areaId) AREA_QUG0}, 0},
    {165, {SHEN_FIELD(candidateIndices){3, 2, 4, 20}}, -1},
};
static const AreaJumpScheduleEntry wsy0AreaJumpSchedule[] = {
    {{15, 1, 3, 20}, 182},
    {{15, 1, 10, 20}, 183},
    {{15, 7, 10, 20}, 184},
    {{0, 15, 16, 0}, 186},
    {{15, 6, 7, 20}, 210},
    {{0, 1, 20, 15}, 791},
    {{0, 0, 0, 0}, -1},
};
static const AreaJumpScheduleEntry ab00AreaJumpSchedule[] = {
    {{15, 4, 14, 20}, 187},
    {{2, 6, 14, 20}, 188},
    {{15, 6, 9, 20}, 189},
    {{0, 0, 0, 0}, -1},
};
static const AreaJumpScheduleEntry ar01AreaJumpSchedule[] = {{{0, 15, 16, 0}, 495}, {{0, 0, 0, 0}, -1}};
static const AreaJumpScheduleEntry qr00AreaJumpSchedule[] = {
    {{0, 15, 16, 0}, 492},
    {{0, 24, 20, 0}, 493},
    {{15, 18, 17, 20}, 494},
    {{0, 0, 0, 0}, -1},
};
static const AreaJumpScheduleEntry ak00AreaJumpSchedule[] = {{{0, 21, 20, 0}, 522}, {{0, 0, 0, 0}, -1}};
static const AreaJumpScheduleEntry wb00AreaJumpSchedule[] = {
    {{0, 15, 16, 0}, 523},
    {{0, 15, 16, 0}, 524},
    {{0, 0, 0, 0}, -1},
};
static const CtrlAreaJumpEntry areaJumps[] = {
    {AREA_WSY0, 66.5f, 121.0f, 64.5f, 120.5f, 63.5f, 121.0f, 66.5f, 122.0f, 98.199997f, shen_name(), wsy0AreaJumpSchedule},
    {AREA_AB00, 163.5f, 536.5f, 163.5f, 538.5f, 164.5f, 536.5f, 164.5f, 538.5f, 95.300003f, shen_name(), ab00AreaJumpSchedule},
    {AREA_AR01, 176.5f, 308.0f, 176.5f, 302.5f, 176.5f, 308.0f, 173.5f, 308.0f, 100.2f, shen_name(), ar01AreaJumpSchedule},
    {AREA_QR00, 109.0f, 152.5f, 107.5f, 152.5f, 109.0f, 152.5f, 109.0f, 150.5f, 92.0f, shen_name(), qr00AreaJumpSchedule},
    {AREA_AK00, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AREA_JUMP_AUTHOR, ak00AreaJumpSchedule},
    {AREA_WB00, 334.5f, 277.0f, 337.0f, 277.0f, 334.5f, 277.5f, 337.0f, 277.5f, 127.0f, shen_name(), wb00AreaJumpSchedule},
    {shen_name(), 0, 0, 0, 0, 0, 0, 0, 0, 0, shen_name(), 0},
};

} // namespace s2
} // namespace shenmue

#endif
