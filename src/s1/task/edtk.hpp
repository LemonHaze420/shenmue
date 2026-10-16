#pragma once

#include "s1/hlib.hpp"
#include "s1/task/scen.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum class EdtkUtilityMode : std::uint32_t {
    ResetState = 0,
    Enqueue = 1,
    Destroy = 2,
    SetBackdropPosition = 3,
};

struct EdtkUtilityParameters {
    EdtkUtilityMode mode;
    float width;
    float height;
};



struct EdtkSequenceEntry {
    float triggerTime;
    const char *subtitleId;
};

static_assert(sizeof(EdtkUtilityParameters) == 0x0c);
static_assert(offsetof(EdtkUtilityParameters, width) == 0x04);
static_assert(offsetof(EdtkUtilityParameters, height) == 0x08);
static_assert(sizeof(EdtkSequenceEntry) == 0x10);

inline constexpr std::array<EdtkSequenceEntry, 11> g_edtkSeqData{{
    {96.800003f, "ENDSQ001"},
    {101.955002f, "ENDSQ002"},
    {107.553001f, "ENDSQ003"},
    {115.501999f, "ENDSQ004"},
    {121.524002f, "ENDSQ005"},
    {124.766006f, "ENDSQ006"},
    {128.233002f, "ENDSQ007"},
    {135.990005f, "ENDSQ008"},
    {138.973999f, "ENDSQ009"},
    {141.912994f, "ENDSQ010"},
    {146.701996f, nullptr},
}};



extern HLib::Task *g_pEDTK_Task;
extern std::array<std::int32_t, 3> g_pEDTK_PVRs;
extern float g_endingBackdropScrollX;
extern float g_endingBackdropScrollY;
extern std::int64_t g_pEDTK_StartTime;
extern std::int32_t g_edtkSeqIdx;

void EnqueueEDTK();
void EDTK_Callback(HLib::TaskData *taskData);
void Script_EDTKUtils(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
