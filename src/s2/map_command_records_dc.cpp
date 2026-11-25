#include "s2/map_draw_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
void DrawMapCommandRecords(MapModelClipRecord *const *records, std::int32_t count) {
    do {
        const MapModelClipRecord *record = *records++;
        const MT7Node *node = reinterpret_cast<const MT7Node *>(record->object);
        matrix_stack::LoadMatrix(&mapCommandMatrix);
        if (node->mesh) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            if (mapObjectVisibilityTests[record->commandType](record->centre, record->radius)) {
                dc::sdk::SetModelScale(record->modelScale);
                g_mapMeshDraw(node->mesh);
            }
        }
    } while (--count);
    matrix_stack::LoadMatrix(&mapCommandMatrix);
}

// @verified - d
void DrawMapDepthCommandRecords(MapModelClipRecord *const *records, std::int32_t count) {
    do {
        const MapModelClipRecord *record = *records++;
        const MT7Node *node = reinterpret_cast<const MT7Node *>(record->object);
        matrix_stack::LoadMatrix(&mapCommandMatrix);
        if (node->mesh) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            if (mapObjectVisibilityTests[record->commandType](record->centre, record->radius) &&
                (record->commandType == 4 || camCtrlIsModelDepthVisible(record->centre, *node, record->radius))) {
                dc::sdk::SetModelScale(record->modelScale);
                g_mapMeshDraw(node->mesh);
            }
        }
    } while (--count);
    matrix_stack::LoadMatrix(&mapCommandMatrix);
}

// @verified - d
void DrawMapScreenCommandRecords(MapModelClipRecord *const *records, std::int32_t count) {
    do {
        const MapModelClipRecord *record = *records++;
        const MT7Node *node = reinterpret_cast<const MT7Node *>(record->object);
        matrix_stack::LoadMatrix(&mapCommandMatrix);
        if (node->mesh) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            if (camCtrlIsScreenSpaceVisible(record->centre, record->radius)) {
                dc::sdk::SetModelScale(record->modelScale);
                g_mapMeshDraw(node->mesh);
            }
        }
    } while (--count);
    matrix_stack::LoadMatrix(&mapCommandMatrix);
}

// @verified - d
void DrawMapAlphaCommandRecords(MapModelClipRecord *const *records, std::int32_t count, float alpha) {
    do {
        const MapModelClipRecord *record = *records++;
        const MT7Node *node = reinterpret_cast<const MT7Node *>(record->object);
        matrix_stack::LoadMatrix(&mapAlphaCommandMatrix);
        if (node->mesh) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            if (mapObjectVisibilityTests[record->commandType](record->centre, record->radius) &&
                (record->commandType == 4 || camCtrlIsModelDepthVisible(record->centre, *node, record->radius))) {
                dc::sdk::SetModelScale(record->modelScale);
                g_mapMeshAlphaDraw(node->mesh, alpha);
            }
        }
    } while (--count);
    matrix_stack::LoadMatrix(&mapAlphaCommandMatrix);
}

// @verified - d
void DrawMapScreenAlphaCommandRecords(MapModelClipRecord *const *records, std::int32_t count, float alpha) {
    do {
        const MapModelClipRecord *record = *records++;
        const MT7Node *node = reinterpret_cast<const MT7Node *>(record->object);
        matrix_stack::LoadMatrix(&mapAlphaCommandMatrix);
        if (node->mesh) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            if (camCtrlIsScreenSpaceVisible(record->centre, record->radius)) {
                dc::sdk::SetModelScale(record->modelScale);
                g_mapMeshAlphaDraw(node->mesh, alpha);
            }
        }
    } while (--count);
    matrix_stack::LoadMatrix(&mapAlphaCommandMatrix);
}

} // namespace s2
} // namespace shenmue

#endif
