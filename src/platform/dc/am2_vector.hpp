#pragma once

#include "common/math.hpp"

#if SHENMUE_IS_DC

// AM2 hand-written vector and matrix helpers (src/platform/dc/am2_vector.S), with C linkage as GameJam's game code calls
// them. Vector arguments point at three floats; the XMTRX helpers act on the SH-4 back-bank matrix.
extern "C" {
float am2Vector3DistanceSquared(const shenmue::Vector3 *first, const shenmue::Vector3 *second);
float am2NegativeSineFixed(std::int32_t angle);
float am2NegativeCosineFixed(std::int32_t angle);
void am2StoreXmtrx(shenmue::Matrix4x4 *matrix);
void am2LoadXmtrx(const shenmue::Matrix4x4 *matrix);
void am2LoadXmtrxPairs(const shenmue::Matrix4x4 *matrix);
void hmtStoreXmtrx(shenmue::Matrix4x4 *matrix);
void hmtLoadXmtrxPairs(const shenmue::Matrix4x4 *matrix);
std::uint32_t am2ReadUnalignedLe32(const void *base, std::int32_t end);
void am2ScaleXmtrxRows(const shenmue::Vector3 *scale);
void am2Vector3AddInPlace(shenmue::Vector3 *value, const shenmue::Vector3 *addend);
void am2Vector3Add(shenmue::Vector3 *output, const shenmue::Vector3 *first, const shenmue::Vector3 *second);
void am2Vector3Combine(const shenmue::Vector3 *first, const shenmue::Vector3 *second, float firstScale, float secondScale, shenmue::Vector3 *output);
float am2Vector3Dot(const shenmue::Vector3 *first, const shenmue::Vector3 *second);
float am2Vector3Distance(const shenmue::Vector3 *first, const shenmue::Vector3 *second);
void am2Vector3Cross(const shenmue::Vector3 *first, const shenmue::Vector3 *second, shenmue::Vector3 *output);
float am2Vector3CrossLength(const shenmue::Vector3 *first, const shenmue::Vector3 *second);
void am2Vector3Scale(shenmue::Vector3 *output, const shenmue::Vector3 *value, float scale);
float am2Vector3Length(const shenmue::Vector3 *value);
float am2Vector3LengthSquared(const shenmue::Vector3 *value);
void am2Vector3SubtractInPlace(shenmue::Vector3 *value, const shenmue::Vector3 *subtrahend);
void am2Vector3Subtract(shenmue::Vector3 *output, const shenmue::Vector3 *first, const shenmue::Vector3 *second);
float am2Vector3NormalizeInPlace(shenmue::Vector3 *value);
float am2Vector3Normalize(const shenmue::Vector3 *value, shenmue::Vector3 *output);
void am2Vector3NormalizeFast(shenmue::Vector3 *value);
// Multiplies XMTRX by the projection along direction onto the plane given as a point followed by its normal, then
// translates by direction.
void am2MultiplyPlaneProjection(const shenmue::Vector3 *plane, const shenmue::Vector3 *direction);
}

#endif
