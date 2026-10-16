#pragma once
#include "s2/area_event.hpp"
#include "s2/bridge_bending.hpp"
#include <cstddef>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
inline constexpr shen_name QDH_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name QDH_REN{SHEN_NAME(REN_)};
inline constexpr shen_name QDH_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name QDH_CAMERA{SHEN_NAME(dcam)};
inline constexpr shen_name QDH_BACKGROUND{SHEN_NAME(a942)};
inline constexpr shen_name QDH_STAGE_1{SHEN_NAME(1BAS)};
inline constexpr shen_name QDH_STAGE_2{SHEN_NAME(2BAS)};
inline constexpr shen_name QDH_BRIDGE_1{SHEN_NAME(BRG1)};
inline constexpr shen_name QDH_BRIDGE_2{SHEN_NAME(BRG2)};
inline constexpr shen_name QDH_AUTHOR_KYA0{SHEN_NAME(KYA0)};
inline constexpr shen_name QDH_AUTHOR_KYA1{SHEN_NAME(KYA1)};
inline constexpr shen_name QDH_AUTHOR_KYA2{SHEN_NAME(KYA2)};
inline constexpr shen_name QDH_AUTHOR_NOA0{SHEN_NAME(NOA0)};
inline constexpr shen_name QDH_AUTHOR_NOA1{SHEN_NAME(NOA1)};
inline constexpr shen_name QDH_CHARACTERS[]{QDH_REN, QDH_TGY, shen_name{0xffffffffU}};
inline constexpr const char *QDH_LEAN_VOICES[]{"0289A007", "0289A008", "0289A009", "0289A010"}; // @text 0289A008 EN: " " | JA: "うわっ…" ; 0289A010 EN: " " | JA: "わあっ！"
inline constexpr const char *QDH_RECOVER_VOICES[]{"0289A011", "0289A012", "0289A013", "0289A014"}; // @text EN: " " | JA: "おっと！"
inline constexpr std::uint32_t QDH_MOTION_IDLE = 0xf003, QDH_MOTION_START = 0x1cd8, QDH_MOTION_WALK = 0x1cdb;
inline constexpr std::uint32_t QDH_MOTION_LEAN_RIGHT = 0x1ccc, QDH_MOTION_RECOVER_RIGHT = 0x1cd0;
inline constexpr std::uint32_t QDH_MOTION_EXIT = 0x1cdf, QDH_MOTION_CALL = 0x1ce5;


struct QdhStageDefinition {
    shen_name character;
    std::int32_t steps[12];
    std::int32_t reversed;
    float scale;
    std::int32_t qteTime;
    std::int32_t reserved40;
    std::int32_t camera;
};
static_assert(sizeof(QdhStageDefinition) == 72 && offsetof(QdhStageDefinition, camera) == 0x44);
inline constexpr QdhStageDefinition QDH_STAGES[3][2]{
    {{.character = QDH_STAGE_1, .steps = {}, .reversed = 0, .scale = 1, .qteTime = 20, .reserved40 = 0, .camera = 0},
        {.character = QDH_STAGE_1, .steps = {}, .reversed = 0, .scale = 1, .qteTime = 20, .reserved40 = 0, .camera = 0}},
    {{.character = QDH_STAGE_1, .steps = {1, 2, 0, 4, 2, 2, 1, 4, 3, 4, 5, 5}, .reversed = 0, .scale = 2, .qteTime = 12, .reserved40 = 0, .camera = 1010},
        {.character = QDH_STAGE_1, .steps = {1, 2, 0, 4, 2, 2, 1, 4, 3, 4, 5, 5}, .reversed = 1, .scale = 2, .qteTime = 12, .reserved40 = 0, .camera = 1010}},
    {{.character = QDH_STAGE_2, .steps = {1, 4, 3, 4, 2, 0, 1, 4, 5, 5, 5, 5}, .reversed = 0, .scale = 2, .qteTime = 16, .reserved40 = 0, .camera = 1010},
        {.character = QDH_STAGE_2, .steps = {1, 4, 3, 4, 2, 0, 1, 4, 5, 5, 5, 5}, .reversed = 1, .scale = 2, .qteTime = 16, .reserved40 = 0, .camera = 1010}}};
inline constexpr BridgeQteAction QDH_ACTIONS[]{{.motion = 7401, .recoverMotion = 7374, .fallMotion = 7380, .qte = 4, .camera = 2000, .fallCamera = 1200},
    {.motion = 7402, .recoverMotion = 7376, .fallMotion = 7381, .qte = 64, .camera = 3000, .fallCamera = 1100},
    {.motion = 7401, .recoverMotion = 7374, .fallMotion = 7380, .qte = 16, .camera = 2000, .fallCamera = 1200},
    {.motion = 7402, .recoverMotion = 7376, .fallMotion = 7381, .qte = 1, .camera = 3000, .fallCamera = 1100}};
struct QdhBridgeAttachment {
    shen_name character;
    std::int32_t delay;
};
static_assert(sizeof(QdhBridgeAttachment) == 8);
struct QdhEventWork {
    HLib::Task *task;
    std::int32_t entry;
    std::int32_t state;
    const QdhStageDefinition *definition;
    std::int32_t qteTime;
    std::int32_t sequence;
    std::int32_t step;
    std::uint32_t action;
    std::int32_t authorMode;
    std::int32_t reserved2c;
    const BridgeQteAction *actions;
    std::int32_t failureLevel;
    float speed;
    Vector3 previousPosition;
    std::int32_t waitCount;
};
static_assert(sizeof(QdhEventWork) == 80 && offsetof(QdhEventWork, actions) == 0x30 && offsetof(QdhEventWork, waitCount) == 0x4c);
bool WaitQdhAuthor(QdhEventWork *work);
void Qdh1Init();
void Qdh1Main(std::uintptr_t argument);
void Qdh2Init();
void Qdh2Main(std::uintptr_t argument);
extern const ScriptAreaDefinition qdh1ScriptDefinition;
extern const ScriptAreaDefinition qdh2ScriptDefinition;
} // namespace shenmue::s2
