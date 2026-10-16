#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;

struct HatoEntity {
    std::array<std::uint8_t, 4> reserved_00{};
    Vector3 position{};
    Vector3 velocity{};
    std::uint16_t pitch = 0;
    std::int16_t yaw = 0;
    std::uint16_t roll = 0;
    std::uint16_t modelVariant = 0;
    float animationFrame = 0.0f;
    std::uint32_t state = 0;
    std::uint8_t turnTimer = 0;
    std::uint8_t avoidanceTimer = 0;
    std::array<std::uint8_t, 2> reserved_2e{};
    float flightAngle = 0.0f;
    std::uint32_t renderFlag = 0;
    float alpha = 0.0f;
};

static_assert(sizeof(HatoEntity) == 0x3c);
static_assert(offsetof(HatoEntity, position) == 0x04);
static_assert(offsetof(HatoEntity, velocity) == 0x10);
static_assert(offsetof(HatoEntity, modelVariant) == 0x22);
static_assert(offsetof(HatoEntity, animationFrame) == 0x24);
static_assert(offsetof(HatoEntity, state) == 0x28);
static_assert(offsetof(HatoEntity, flightAngle) == 0x30);
static_assert(offsetof(HatoEntity, renderFlag) == 0x34);
static_assert(offsetof(HatoEntity, alpha) == 0x38);



struct HatoTaskData {
    HLib::Task *task = nullptr;
    std::array<void *, 4> models{};
    Vector3 center{};
    float maxDistance = 5.0f;
    std::array<HatoEntity, 20> entities{};
    std::uint8_t entityCount = 0;
    std::array<std::uint8_t, 0x17> reserved_4e9{};
};

static_assert(sizeof(HatoTaskData) == 0x500);
static_assert(offsetof(HatoTaskData, task) == 0x00);
static_assert(offsetof(HatoTaskData, models) == 0x08);
static_assert(offsetof(HatoTaskData, center) == 0x28);
static_assert(offsetof(HatoTaskData, maxDistance) == 0x34);
static_assert(offsetof(HatoTaskData, entities) == 0x38);
static_assert(offsetof(HatoTaskData, entityCount) == 0x4e8);

struct HatoScriptParameters {
    std::int32_t mode;
    std::int32_t radius;
    std::int32_t totalNum;
};

static_assert(sizeof(HatoScriptParameters) == 0x0c);

bool HATO_Init();
bool HATO_Setup(const Vector3 &center, float maxDistance);
std::int32_t HATO_Enqueue(std::int32_t radius, std::int32_t totalNum);
void HATO_Callback(HLib::TaskData *taskData);
void HATO_FinishUpdate();
void HATO_Stop();
void HATO_Cleanup();

extern HatoTaskData *TempHATO;
extern std::int32_t g_bHatoEnabled;
extern std::int16_t g_hato_ticks;

HatoTaskData *HATO_GetTaskData() noexcept;
bool HATO_IsEnabled() noexcept;

// Shenmue script function 451. Modes 0..4 respectively allocate, configure,
// enqueue, stop, and release the HATO owner.
void Script_HATO_Funcs(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
