#pragma once
#include "s2/area_event.hpp"
#include "s2/bridge_bending.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name HASI_RYO{SHEN_NAME(RYO_)}, HASI_CAMERA{SHEN_NAME(dcam)}, HASI_RETURN_AREA{SHEN_NAME(QD01)};
inline constexpr shen_name HASI_STAGE_1{SHEN_NAME(1BAS)};
inline constexpr shen_name HASI_STAGE_2{SHEN_NAME(2BAS)};
inline constexpr shen_name HASI_STAGE_3{SHEN_NAME(3BAS)};
inline constexpr shen_name HASI_STAGE_4{SHEN_NAME(4BAS)};
inline constexpr shen_name HASI_STAGE_5{SHEN_NAME(5BAS)};
inline constexpr shen_name HASI_STAGE_6{SHEN_NAME(6BAS)};
inline constexpr shen_name HASI_STAGE_7{SHEN_NAME(7BAS)};
inline constexpr shen_name HASI_STAGE_8{SHEN_NAME(8BAS)};
inline constexpr shen_name HASI_STAGE_9{SHEN_NAME(9BAS)};
inline constexpr shen_name HASI_AUTHOR_0{SHEN_NAME(AUH0)};
inline constexpr shen_name HASI_AUTHOR_1{SHEN_NAME(AUH1)};
inline constexpr shen_name HASI_AUTHOR_2{SHEN_NAME(AUH2)};
inline constexpr shen_name HASI_AUTHOR_3{SHEN_NAME(AUH3)};
inline constexpr shen_name HASI_AUTHOR_4{SHEN_NAME(AUH4)};
inline constexpr shen_name HASI_AUTHOR_5{SHEN_NAME(AUH5)};
inline constexpr shen_name HASI_AUTHOR_6{SHEN_NAME(AUH6)};
inline constexpr shen_name HASI_AUTHOR_7{SHEN_NAME(AUH7)};
inline constexpr shen_name HASI_AUTHOR_8{SHEN_NAME(AUH8)};
inline constexpr shen_name HASI_AUTHOR_9{SHEN_NAME(AUH9)};
inline constexpr shen_name HASI_AUTHOR_A{SHEN_NAME(AUHa)};
inline constexpr shen_name HASI_AUTHOR_B{SHEN_NAME(AUHb)};
inline constexpr shen_name HASI_AUTHORS[]{HASI_AUTHOR_0, HASI_AUTHOR_1, HASI_AUTHOR_2, HASI_AUTHOR_3, HASI_AUTHOR_4, HASI_AUTHOR_5, HASI_AUTHOR_6,
    HASI_AUTHOR_7, HASI_AUTHOR_8, HASI_AUTHOR_9, HASI_AUTHOR_A, HASI_AUTHOR_B};
inline constexpr const char *HASI_LEAN_VOICES[]{"0289A007", "0289A008", "0289A009", "0289A010"}; // @text 0289A008 EN: " " | JA: "うわっ…" ; 0289A010 EN: " " | JA: "わあっ！"
inline constexpr const char *HASI_RECOVER_VOICES[]{"0289A011", "0289A012", "0289A013", "0289A014"}; // @text EN: " " | JA: "おっと！"
inline constexpr std::uint32_t HASI_MOTION_IDLE = 0xf003, HASI_MOTION_START = 0x1cd8, HASI_MOTION_WALK = 0x1cdb, HASI_MOTION_EXIT = 0x1cdf;
struct HasiStageRestoreDefinition {
    shen_name stage, first, second;
    float scale;
};
static_assert(sizeof(HasiStageRestoreDefinition) == 0x10);
inline constexpr HasiStageRestoreDefinition HASI_RESTORE_STAGES[]{{.stage = HASI_STAGE_1, .first = SHEN_NAME(hs1a), .second = SHEN_NAME(hs1b), .scale = 2.0f},
    {.stage = HASI_STAGE_2, .first = SHEN_NAME(hs2a), .second = SHEN_NAME(hs2b), .scale = 2.0f},
    {.stage = HASI_STAGE_3, .first = SHEN_NAME(hs3a), .second = SHEN_NAME(hs3b), .scale = 1.8f},
    {.stage = HASI_STAGE_4, .first = SHEN_NAME(hs4a), .second = SHEN_NAME(hs4b), .scale = 1.8f},
    {.stage = HASI_STAGE_5, .first = SHEN_NAME(hs5a), .second = SHEN_NAME(hs5b), .scale = 1.5f},
    {.stage = HASI_STAGE_6, .first = SHEN_NAME(hs6a), .second = SHEN_NAME(hs6b), .scale = 1.5f},
    {.stage = HASI_STAGE_7, .first = SHEN_NAME(hs7a), .second = SHEN_NAME(hs7b), .scale = 2.0f},
    {.stage = HASI_STAGE_8, .first = SHEN_NAME(hs8a), .second = SHEN_NAME(hs8b), .scale = 1.8f},
    {.stage = HASI_STAGE_9, .first = SHEN_NAME(hs9a), .second = SHEN_NAME(hs9b), .scale = 2.0f}};
struct HasiBridgeAttachment {
    shen_name character;
    std::int32_t delay;
};
static_assert(sizeof(HasiBridgeAttachment) == 8);
struct HasiStageDefinition {
    shen_name character;
    std::int32_t steps[10];
    std::int32_t reversed;
    float scale;
    std::int32_t qteTime;
    shen_name author;
    std::int32_t camera;
};
static_assert(sizeof(HasiStageDefinition) == 0x40);
static_assert(offsetof(HasiStageDefinition, reversed) == 0x2c);
static_assert(offsetof(HasiStageDefinition, author) == 0x38);
inline constexpr BridgeQteAction HASI_ACTIONS[]{{.motion = 7401, .recoverMotion = 7374, .fallMotion = 7380, .qte = 4, .camera = 2000, .fallCamera = 1200},
    {.motion = 7402, .recoverMotion = 7376, .fallMotion = 7381, .qte = 64, .camera = 3000, .fallCamera = 1100},
    {.motion = 7401, .recoverMotion = 7374, .fallMotion = 7380, .qte = 16, .camera = 2000, .fallCamera = 1200},
    {.motion = 7402, .recoverMotion = 7376, .fallMotion = 7381, .qte = 1, .camera = 3000, .fallCamera = 1100}};
inline constexpr HasiStageDefinition HASI_STAGES[10][2]{
    {{.character = HASI_STAGE_1, .steps = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .reversed = 0, .scale = 1.0f, .qteTime = 20, .author = shen_name{}, .camera = 0},
        {.character = HASI_STAGE_1, .steps = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .reversed = 0, .scale = 1.0f, .qteTime = 20, .author = shen_name{}, .camera = 0}},
    {{.character = HASI_STAGE_1, .steps = {4, 4, 1, 4, 0, 4, 1, 4, 5, 5}, .reversed = 0, .scale = 2.0f, .qteTime = 20, .author = HASI_AUTHOR_A, .camera = 1000},
        {.character = HASI_STAGE_1,
            .steps = {4, 4, 1, 4, 1, 4, 0, 4, 5, 5},
            .reversed = 1,
            .scale = 2.0f,
            .qteTime = 20,
            .author = HASI_AUTHOR_4,
            .camera = 1000}},
    {{.character = HASI_STAGE_2, .steps = {4, 4, 0, 4, 0, 4, 1, 4, 5, 5}, .reversed = 0, .scale = 2.0f, .qteTime = 20, .author = HASI_AUTHOR_9, .camera = 1000},
        {.character = HASI_STAGE_2,
            .steps = {4, 4, 0, 4, 1, 4, 0, 4, 5, 5},
            .reversed = 1,
            .scale = 2.0f,
            .qteTime = 20,
            .author = HASI_AUTHOR_3,
            .camera = 1000}},
    {{.character = HASI_STAGE_3,
         .steps = {4, 4, 0, 4, 1, 0, 2, 4, 5, 5},
         .reversed = 0,
         .scale = 1.7999999523162842f,
         .qteTime = 16,
         .author = HASI_AUTHOR_5,
         .camera = 1010},
        {.character = HASI_STAGE_3,
            .steps = {4, 4, 2, 4, 1, 0, 1, 4, 5, 5},
            .reversed = 1,
            .scale = 1.7999999523162842f,
            .qteTime = 16,
            .author = HASI_AUTHOR_9,
            .camera = 1010}},
    {{.character = HASI_STAGE_4,
         .steps = {4, 4, 1, 1, 3, 4, 0, 4, 5, 5},
         .reversed = 0,
         .scale = 1.7999999523162842f,
         .qteTime = 16,
         .author = HASI_AUTHOR_0,
         .camera = 1010},
        {.character = HASI_STAGE_4,
            .steps = {4, 4, 1, 4, 0, 1, 3, 4, 5, 5},
            .reversed = 1,
            .scale = 1.7999999523162842f,
            .qteTime = 16,
            .author = HASI_AUTHOR_A,
            .camera = 1010}},
    {{.character = HASI_STAGE_5,
         .steps = {4, 4, 2, 0, 3, 1, 0, 4, 5, 5},
         .reversed = 0,
         .scale = 1.7999999523162842f,
         .qteTime = 12,
         .author = HASI_AUTHOR_7,
         .camera = 1020},
        {.character = HASI_STAGE_5,
            .steps = {4, 4, 0, 2, 1, 4, 3, 0, 5, 5},
            .reversed = 1,
            .scale = 1.7999999523162842f,
            .qteTime = 12,
            .author = HASI_AUTHOR_9,
            .camera = 1020}},
    {{.character = HASI_STAGE_6,
         .steps = {0, 4, 1, 0, 0, 4, 2, 3, 5, 5},
         .reversed = 0,
         .scale = 1.7999999523162842f,
         .qteTime = 12,
         .author = HASI_AUTHOR_B,
         .camera = 1020},
        {.character = HASI_STAGE_6,
            .steps = {4, 4, 0, 4, 1, 3, 1, 2, 5, 5},
            .reversed = 1,
            .scale = 1.7999999523162842f,
            .qteTime = 12,
            .author = HASI_AUTHOR_A,
            .camera = 1020}},
    {{.character = HASI_STAGE_7, .steps = {4, 4, 1, 4, 3, 0, 1, 4, 5, 5}, .reversed = 0, .scale = 2.0f, .qteTime = 16, .author = HASI_AUTHOR_A, .camera = 1010},
        {.character = HASI_STAGE_7,
            .steps = {4, 4, 0, 4, 0, 1, 3, 1, 5, 5},
            .reversed = 1,
            .scale = 2.0f,
            .qteTime = 16,
            .author = HASI_AUTHOR_6,
            .camera = 1010}},
    {{.character = HASI_STAGE_8,
         .steps = {4, 4, 0, 1, 2, 0, 1, 4, 5, 5},
         .reversed = 0,
         .scale = 1.7999999523162842f,
         .qteTime = 16,
         .author = HASI_AUTHOR_9,
         .camera = 1010},
        {.character = HASI_STAGE_8,
            .steps = {4, 4, 1, 0, 2, 4, 1, 1, 5, 5},
            .reversed = 1,
            .scale = 1.7999999523162842f,
            .qteTime = 16,
            .author = HASI_AUTHOR_2,
            .camera = 1010}},
    {{.character = HASI_STAGE_9, .steps = {4, 4, 0, 4, 1, 4, 1, 4, 5, 5}, .reversed = 0, .scale = 1.0f, .qteTime = 20, .author = HASI_AUTHOR_1, .camera = 1000},
        {.character = HASI_STAGE_9,
            .steps = {4, 4, 1, 4, 0, 4, 1, 4, 5, 5},
            .reversed = 1,
            .scale = 1.0f,
            .qteTime = 20,
            .author = HASI_AUTHOR_A,
            .camera = 1000}}};
struct HasiEventWork {
    HLib::Task *task;
    std::int32_t entry, state;
    const HasiStageDefinition *definition;
    std::uint16_t qteTime, reserved1a;
    std::int32_t sequence, step;
    std::uint32_t action;
    Vector3 translation;
    Vector3i rotation;
    shen_name author;
    std::uint32_t reserved44;
    const BridgeQteAction *actions;
    Vector3 previousPosition;
    std::uint32_t reserved5c;
};
static_assert(sizeof(HasiEventWork) == 0x60);
static_assert(offsetof(HasiEventWork, definition) == 0x10);
static_assert(offsetof(HasiEventWork, translation) == 0x28);
static_assert(offsetof(HasiEventWork, rotation) == 0x34);
static_assert(offsetof(HasiEventWork, author) == 0x40);
static_assert(offsetof(HasiEventWork, actions) == 0x48);
static_assert(offsetof(HasiEventWork, previousPosition) == 0x50);
void HasiInit();
void HasiMain(std::uintptr_t argument);
extern const ScriptAreaDefinition hasiScriptDefinition;
} // namespace shenmue::s2
