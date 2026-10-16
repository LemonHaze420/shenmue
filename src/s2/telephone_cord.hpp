#pragma once

#include "s2/character.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TELEPHONE_CORD_TASK{SHEN_NAME(TELC)};
inline constexpr shen_name TELEPHONE_CORD_RECEIVER{0x33353061U};
inline constexpr shen_name TELEPHONE_CORD_BASE{0x32353061U};
inline constexpr Vector3 TELEPHONE_CORD_RECEIVER_ATTACHMENT{-0.085f, 0.07f, -0.09f};
inline constexpr Vector3 TELEPHONE_CORD_BASE_ATTACHMENT{-0.095f, -0.01f, 0.01f};
inline constexpr char TELEPHONE_CORD_MODEL[] = "DENR003G.MT7";

struct TelephoneCordGeometry {
    const MT7Node *mesh;        // +0x00 XB01 owner adaptation
    float segmentLength;
    std::int32_t segmentCount;
    float velocity;
    Vector3 receiverAttachment;
    Vector3 baseAttachment;
    Vector3 anchor;
    Vector3 force;
    std::int32_t arc;
    float radius;
    float height;
};
static_assert(sizeof(TelephoneCordGeometry) == 0x50);
static_assert(offsetof(TelephoneCordGeometry, receiverAttachment) == 0x14);
static_assert(offsetof(TelephoneCordGeometry, arc) == 0x44);

struct TelephoneCordTaskData {
    Character *receiver;
    Character *base;
    void *allocation;
    MT7Model *reader;
    TelephoneCordGeometry geometry;
};
static_assert(sizeof(TelephoneCordTaskData) == 0x70);
static_assert(offsetof(TelephoneCordTaskData, geometry) == 0x20);

extern HLib::Task *g_telephoneCordTask;
void InitializeTelephoneCordGeometry(TelephoneCordGeometry *geometry, const MT7Node *mesh, float segmentLength, std::int32_t segmentCount);
void SetTelephoneCordAttachments(TelephoneCordGeometry *geometry, const Vector3 *receiver, const Vector3 *base);
void DrawTelephoneCordGeometry(TelephoneCordGeometry *geometry);
void TelephoneCordDrawCallback(void *taskData);
void TelephoneCordCallback(void *taskData);
void EnqueueTelephoneCord();
void CleanupTelephoneCord();
void ReleaseTelephoneCordHostResources();

void telCodeInit();

} // namespace shenmue::s2
