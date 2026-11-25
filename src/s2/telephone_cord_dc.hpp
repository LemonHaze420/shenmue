#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"

namespace shenmue {
namespace s2 {

#define TELEPHONE_CORD_TASK shen_name(SHEN_NAME4('T', 'E', 'L', 'C'))
#define TELEPHONE_CORD_RECEIVER shen_name(0x33353061U)
#define TELEPHONE_CORD_BASE shen_name(0x32353061U)

struct TelephoneCordGeometry {
    void *mesh;
    float segmentLength;
    std::int32_t segmentCount;
    float velocity;
    Vector3 receiverAttachment, baseAttachment, anchor, force;
    std::int32_t arc;
    float radius, height;
};
struct TelephoneCordTaskData {
    Character *receiver, *base;
    void *allocation;
    MT7Node *root;
    TelephoneCordGeometry geometry;
};
typedef char TelephoneCordGeometrySizeCheck[sizeof(TelephoneCordGeometry) == 0x4c ? 1 : -1];
typedef char TelephoneCordForceOffsetCheck[offsetof(TelephoneCordGeometry, force) == 0x34 ? 1 : -1];
typedef char TelephoneCordArcOffsetCheck[offsetof(TelephoneCordGeometry, arc) == 0x40 ? 1 : -1];
typedef char TelephoneCordTaskSizeCheck[sizeof(TelephoneCordTaskData) == 0x5c ? 1 : -1];
typedef char TelephoneCordGeometryOffsetCheck[offsetof(TelephoneCordTaskData, geometry) == 0x10 ? 1 : -1];

extern HLib::Task *g_telephoneCordTask;
void InitializeTelephoneCordGeometry(TelephoneCordGeometry *geometry, void *mesh, float length, std::int32_t count);
void SetTelephoneCordAttachments(TelephoneCordGeometry *geometry, const Vector3 *receiver, const Vector3 *base);
void DrawTelephoneCordGeometry(TelephoneCordGeometry *geometry);
void TelephoneCordDrawCallback(void *taskData);
void TelephoneCordCallback(void *taskData);
void ResetTelephoneCordTask();
void EnqueueTelephoneCord();
void CleanupTelephoneCord();
void telCodeInit();

} // namespace s2
} // namespace shenmue

#endif
