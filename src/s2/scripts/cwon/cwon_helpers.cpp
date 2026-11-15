#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/scripts/cwon/area_cwon.hpp"

HLibTask *CwonQueueFadeWait(HLibTask *parent) {
    HLibTask *task = 0;
    HLibTask **work = CwonCreateTask(&task, (HLibTaskCallback)CwonFadeWaitCallback, 4);
    if (work) {
        *work = parent;
        FTBL.HLib_SetupTimedTask(parent, 0);
    }
    return task;
}

void CwonFadeWaitCallback(HLibTask **work) {
    if (work) {
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.HLib_ResolveDeferredCallback(*work);
            FTBL.HLib_DestroyCurrentTask();
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

HLibTask *CwonQueueMotionWait(HLibTask *parent, shen_name character, float frame) {
    HLibTask *task = 0;
    CwonMotionWaitWork *work = CwonCreateTask(&task, (HLibTaskCallback)CwonMotionWaitCallback, sizeof(*work));
    if (work) {
        work->frame = frame;
        work->character = character;
        work->parent = parent;
        FTBL.HLib_SetupTimedTask(parent, 0);
    }
    return task;
}

void CwonMotionWaitCallback(CwonMotionWaitWork *work) {
    int done = 0;
    if (work) {
        if (work->frame != 0) {
            if (work->frame <= FTBL.GetMotionFrame(FTBL.scenFindMotionwork(work->character), 0))
                done = 1;
        } else
            done = FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->character), 65, 1, 0);
        if (done) {
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyCurrentTask();
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

HLibTask *CwonQueueConversationWait(HLibTask *parent, void *conversation) {
    HLibTask *task = 0;
    CwonConversationWaitWork *work = CwonCreateTask(&task, (HLibTaskCallback)CwonConversationWaitCallback, sizeof(*work));
    if (work) {
        work->ticks = 100;
        work->conversation = conversation;
        work->parent = parent;
        FTBL.HLib_SetupTimedTask(parent, 0);
    }
    return task;
}

void CwonConversationWaitCallback(CwonConversationWaitWork *work) {
    if (work) {
        if (!FTBL.cvsHasValidConverseData(work->conversation)) {
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyCurrentTask();
        } else if (--work->ticks < 1) {
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyCurrentTask();
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

HLibTask *CwonQueueQteWait(HLibTask *parent, short button, short frames, unsigned short *result) {
    HLibTask *task = 0;
    CwonQteWaitWork *work = CwonCreateTask(&task, (HLibTaskCallback)CwonQteWaitCallback, sizeof(*work));
    if (work) {
        work->button = button;
        work->frames = frames;
        work->result = result;
        work->parent = parent;
        FTBL.SetShowQteFlagForButtonType((unsigned short)button, frames, 0);
        FTBL.HLib_SetupTimedTask(parent, 0);
    }
    return task;
}

void CwonQteWaitCallback(CwonQteWaitWork *work) {
    if (work) {
        if (!FTBL.GetShouldRenderIcons(3)) {
            *work->result = FTBL.GetCurrentQteStatus();
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyCurrentTask();
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void CwonFightWaitCallback(CwonFightWaitWork *work) {
    if (work && !FTBL.IsFightActive()) {
        *work->result = FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(work->character));
        FTBL.HLib_SetupTimedTask(work->parent, 30);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void *CwonCreateTask(HLibTask **task, HLibTaskCallback callback, unsigned size) {
    void *work = 0;
    if (size) {
        *task = FTBL.HLib_EnqueueTaskWithParameter(callback, 4, 11, size, TASK_EVNT);
        if (*task)
            work = FTBL.HLib_GetTaskParameter(*task);
    } else
        *task = FTBL.HLib_EnqueueTaskWithoutParameter(callback, 4, 11, TASK_EVNT);
    return work;
}

void CwonChangeAndWaitMotion(HLibTaskCallback callback, HLibTask *parent, shen_name character, float frame) {
    if (callback)
        FTBL.HLib_SetCurrentTaskTo(callback);
    CwonQueueMotionWait(parent, character, frame);
    return;
}

HLibTask *CwonQueueMotionOffset(shen_name character, const Vector3 *position, float frame) {
    HLibTask *task = 0;
    CwonOffsetMotionWork *work = CwonCreateTask(&task, (HLibTaskCallback)CwonMotionOffsetCallback, sizeof(*work));
    if (work) {
        work->frame = frame;
        work->position.x = position->x;
        work->position.y = position->y;
        work->position.z = position->z;
        work->character = character;
    }
    return task;
}

void CwonMotionOffsetCallback(CwonOffsetMotionWork *work) {
    int done = 0;
    if (work) {
        if (work->frame != 0) {
            if (work->frame <= FTBL.GetMotionFrame(FTBL.scenFindMotionwork(work->character), 0))
                done = 1;
        } else
            done = FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->character), 65, 1, 0);
        if (done)
            FTBL.HLib_DestroyCurrentTask();
        else
            FTBL.SetCharaTargetTransformOffset(work->character, &work->position, 1);
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void CwonResetCamera(void) {
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    return;
}

void CwonGetLocalTransform(shen_name name, Vector3 *position, Vector3i *rotation) {
    if (position)
        FTBL.GetCharaPosition(name, -1, position, 0);
    if (rotation)
        FTBL.GetScenarioCharacterEulerAngles(name, rotation, 0);
    return;
}

void CwonGetWorldTransform(shen_name name, Vector3 *position, Vector3i *rotation) {
    if (position)
        FTBL.GetCharaPosition(name, -1, position, 0x40000000);
    if (rotation)
        FTBL.GetScenarioCharacterEulerAngles(name, rotation, 0x40000000);
    return;
}

void CwonSetPositionMasked(shen_name name, unsigned flags, const Vector3 *value) {
    Vector3 position;
    if (!(flags & 64))
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 0, 0);
    if (flags & 128)
        FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
    else
        FTBL.GetCharaPosition(name, -1, &position, 0);
    if (flags & 8)
        position.x += value->x;
    else if (flags & 1)
        position.x = value->x;
    if (flags & 16)
        position.y += value->y;
    else if (flags & 2)
        position.y = value->y;
    if (flags & 32)
        position.z += value->z;
    else if (flags & 4)
        position.z = value->z;
    if (flags & 128)
        FTBL.EV_AT_SetPos(name, 0x78000000, &position);
    else
        FTBL.EV_AT_SetPos(name, 0x38000000, &position);
    return;
}

void CwonSetRotationMasked(shen_name name, unsigned flags, const Vector3i *value) {
    Vector3i rotation;
    if (!(flags & 64))
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 0, 0);
    if (flags & 128)
        FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0x40000000);
    else
        FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0);
    if (flags & 1)
        rotation.x = value->x;
    else if (flags & 8)
        rotation.x += value->x;
    if (flags & 2)
        rotation.y = value->y;
    else if (flags & 16)
        rotation.y += value->y;
    if (flags & 4)
        rotation.z = value->z;
    else if (flags & 32)
        rotation.z += value->z;
    if (flags & 128)
        FTBL.EV_AT_SetAngY_0(name, 0x78000000, &rotation, 0);
    FTBL.EV_AT_SetAngY_0(name, 0x38000000, &rotation, 0);
    return;
}

void CwonSetYaw(shen_name name, int yaw) {
    Vector3i rotation;
    rotation.z = 0;
    rotation.x = 0;
    rotation.y = yaw;
    CwonSetRotationMasked(name, 7, &rotation);
    return;
}

void CwonAddYaw(shen_name name, int yaw) {
    Vector3i rotation;
    rotation.y = yaw;
    CwonSetRotationMasked(name, 16, &rotation);
    return;
}

void CwonHideAtOrigin(shen_name name, int mode) {
    if (FTBL.DoesScenarioCharacterExist(name)) {
        Vector3 position;
        position.x = position.y = position.z = 0;
        CwonSetPositionMasked(name, 7, &position);
        CwonHideCharacter(name);
    }
    return;
}

void CwonMoveBackward(shen_name name, float distance) {
    Vector3 offset;
    Vector3i rotation;
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0);
    offset.x = -(distance * FTBL.CalculateFixedSine(rotation.y));
    offset.z = -(distance * FTBL.CalculateFixedCosine(rotation.y));
    CwonSetPositionMasked(name, 40, &offset);
    return;
}

void CwonMoveBackwardAtYaw(shen_name name, int yaw, float distance) {
    Vector3 offset;
    offset.x = -(distance * FTBL.CalculateFixedSine(yaw));
    offset.z = -(distance * FTBL.CalculateFixedCosine(yaw));
    CwonSetPositionMasked(name, 40, &offset);
    return;
}

int CwonAdvanceRoute(const CwonRouteEntry *route, int index) {
    const CwonRouteEntry *entry;
    if (index < 0)
        return index;
    entry = &route[index];
    switch (entry->kind) {
    case 0:
        if (CwonTurnTowardYaw(entry->character, entry->yaw, entry->step, entry->direction))
            index++;
        break;
    case 1:
        if (CwonPositionThreshold(entry->character, entry->threshold, entry->axis, entry->direction))
            index++;
        break;
    case 2:
        index++;
        break;
    default:
        index = -1;
        break;
    }
    if (route[index].kind == 3)
        index = -1;
    return index;
}

int CwonPositionThreshold(shen_name name, float threshold, int isZ, int reversed) {
    Vector3 position;
    float value;
    int done = 0;
    FTBL.GetCharaPosition(name, -1, &position, 0);
    value = position.x;
    if (isZ)
        value = position.z;
    if (reversed) {
        if (value < threshold)
            done = 1;
    } else if (threshold < value)
        done = 1;
    return done;
}

int CwonTurnTowardYaw(shen_name name, int target, int step, int reversed) {
    Vector3i rotation;
    int done = 0;
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0);
    if (!reversed) {
        if ((int)((target - rotation.y) & 65535U) <= step) {
            rotation.y = target;
            done = 1;
        } else
            rotation.y += step;
    } else {
        if ((int)((rotation.y - target) & 65535U) <= step) {
            rotation.y = target;
            done = 1;
        } else
            rotation.y -= step;
    }
    CwonSetRotationMasked(name, 2, &rotation);
    return done;
}

void CwonHideCharacter(shen_name name) {
    FTBL.EV_ToggleCharacterFlag(name);
    FTBL.EV_DisableCharacterDisplay(name);
    return;
}

void CwonShowCharacter(shen_name name) {
    FTBL.EV_EnsureCharacterDraw(name);
    FTBL.EV_EnableCharacterDisplay(name);
    return;
}

void CwonEnablePositionedCharacter(shen_name name, float x, float y, float z, float ax, float ay, float az) {
    Vector3 position;
    Vector3i rotation;
    position.x = x;
    position.y = y;
    position.z = z;
    rotation.x = ((int)(ax * 8192.0f / 45.0f) & 65535);
    rotation.y = ((int)(ay * 8192.0f / 45.0f) & 65535);
    rotation.z = ((int)(az * 8192.0f / 45.0f) & 65535);
    CwonSetPositionMasked(name, 7, &position);
    CwonSetRotationMasked(name, 7, &rotation);
    FTBL.EV_EnableCharacterDisplay(name);
    return;
}

void CwonRestoreGravity(shen_name name) {
    register char *control __asm__("r1");
    FTBL.EV_SetCharaControllerFlags(name, 2, 1);
    FTBL.EV_SetCharaControllerFlags(name, 3, 1);
    control = (char *)&FTBL + 252;
    SHENMUE_EVENT_OPAQUE(control);
    ((EventFunctionTable *)(control - 252))->EV_SetCharaControllerFlags(name, 1, 1);
}

void CwonDisableGravity(shen_name name) {
    register char *control __asm__("r1");
    FTBL.EV_SetCharaControllerFlags(name, 2, 0);
    FTBL.EV_SetCharaControllerFlags(name, 3, 0);
    control = (char *)&FTBL + 252;
    SHENMUE_EVENT_OPAQUE(control);
    ((EventFunctionTable *)(control - 252))->EV_SetCharaControllerFlags(name, 1, 0);
}

float CwonBallisticHeight(float time, float height) {
    return height - .00544f * time * time;
}

unsigned CwonCreateDust(const Vector3 *position, const Vector3 *volume, const Vector3 *velocity) {
    Vector3 acceleration;
    unsigned effect;
    acceleration.x = .026274512f;
    acceleration.y = acceleration.z = 0;
    effect = FTBL.CreateSmokeEffect(position, volume, 50, .08f, 0, 0);
    FTBL.SetSmokeEffectFadeFactor(effect, 27);
    FTBL.SetSmokeEffectFalloff(effect, 158);
    FTBL.SetParticleEffectColour(0, effect, 0, 106, 104, 101);
    FTBL.SetParticleEffectInitialScale(0, effect, .21568628f);
    FTBL.SetParticleEffectSize(0, effect, 2.3137255f, 2.8235295f);
    FTBL.SetParticleEffectBirthRate(0, effect, .03019608f);
    FTBL.SetParticleEffectVelocity(0, effect, velocity, &acceleration);
    FTBL.ResumeParticleEffect(0, effect);
    return effect;
}

void CwonNullsub(void) {
    return;
}

void CwonPrepareMotionCharacters(const shen_name *names) {
    float colour[4] = {1, .3f, .3f, .3f};
    for (; *names; names++) {
        CwonShowCharacter(*names);
        CwonDisableGravity(*names);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(*names), 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(*names), 71, 0, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(*names), 69, 2, 0);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(*names), 0);
        FTBL.EV_SetCharacterAlphaEnabled(*names, 0);
    }
    return;
}

int CwonStartAuthor(int index, HLibTask *parent, int frame, const shen_name *motion, const shen_name *gravity) {
    static const shen_name authors[29] = {SHEN_NAME4('A', 'T', '0', '0'), SHEN_NAME4('A', 'T', '0', '1'), SHEN_NAME4('A', 'T', '0', '2'),
        SHEN_NAME4('A', 'T', '0', '3'), SHEN_NAME4('A', 'T', '0', '4'), SHEN_NAME4('A', 'T', '0', '5'), SHEN_NAME4('A', 'T', '0', '6'),
        SHEN_NAME4('A', 'T', '0', '7'), SHEN_NAME4('A', 'T', '0', '8'), SHEN_NAME4('A', 'T', '0', '9'), SHEN_NAME4('A', 'T', '1', '0'),
        SHEN_NAME4('A', 'T', '1', '1'), SHEN_NAME4('A', 'T', '1', '2'), SHEN_NAME4('A', 'T', '1', '3'), SHEN_NAME4('A', 'T', '1', '4'),
        SHEN_NAME4('A', 'T', '1', '5'), SHEN_NAME4('A', 'T', '1', '6'), SHEN_NAME4('A', 'T', '1', '7'), SHEN_NAME4('A', 'T', '1', '8'),
        SHEN_NAME4('A', 'T', '1', '9'), SHEN_NAME4('A', 'T', '2', '0'), SHEN_NAME4('A', 'T', '2', '1'), SHEN_NAME4('A', 'T', '2', '2'),
        SHEN_NAME4('A', 'T', '2', '3'), SHEN_NAME4('A', 'T', '2', '4'), SHEN_NAME4('A', 'T', '2', '5'), SHEN_NAME4('A', 'T', '2', '6'),
        SHEN_NAME4('A', 'T', '2', '7'), SHEN_NAME4('A', 'T', '2', '8')};
    HLibTask *task = 0;
    CwonAuthorWaitWork *work;
    if (index != -1) {
        if (FTBL.EV_authSceneCtrl(-1))
            FTBL.EV_authSceneCtrl(-4);
        FTBL.EV_authSceneCtrl(-14);
        FTBL.EV_authSceneCtrl(authors[index]);
    }
    work = CwonCreateTask(&task, (HLibTaskCallback)CwonAuthorWaitCallback, sizeof(*work));
    if (work) {
        if (motion)
            CwonPrepareMotionCharacters(motion);
        SHENMUE_EVENT_BOUNDARY();
        if (gravity)
            CwonPrepareGravityCharacters(gravity);
        work->frame = FTBL.EV_authSceneCtrl(-5) < frame ? FTBL.EV_authSceneCtrl(-5) : frame;
        work->parent = parent;
        work->motionCharacters = motion;
        work->gravityCharacters = gravity;
        if (parent)
            FTBL.HLib_SetupTimedTask(parent, 0);
    } else
        FTBL.DebugLogToFileRemoved("Auth Wait Error\n");
    return FTBL.EV_authSceneCtrl(-5);
}

void CwonAuthorWaitCallback(CwonAuthorWaitWork *work) {
    int finished = 0, early = 0;
    if (FTBL.EV_authSceneCtrl(-1)) {
        if (work->frame) {
            if (FTBL.EV_authSceneCtrl(-6) >= work->frame) {
                finished = 1;
                early = 1;
            }
        }
    } else
        finished = 1;
    if (finished) {
        if (work->parent)
            FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyCurrentTask();
        if (!early) {
            if (work->motionCharacters)
                CwonRestoreMotionCharacters(work->motionCharacters);
            if (work->gravityCharacters)
                CwonRestoreGravityCharacters(work->gravityCharacters);
        }
    }
    return;
}

void CwonPrepareGravityCharacters(const shen_name *names) {
    for (; *names; names++) {
        CwonShowCharacter(*names);
        CwonDisableGravity(*names);
    }
    return;
}

void CwonRestoreMotionCharacters(const shen_name *names) {
    for (; *names; names++) {
        CwonRestoreGravity(*names);
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(*names), 1);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(*names), 3);
        FTBL.SetRefBoardParamsForCharaById(*names, 1);
    }
    return;
}

void CwonRestoreGravityCharacters(const shen_name *names) {
    for (; *names; names++)
        CwonRestoreGravity(*names);
    return;
}

HLibTask *CwonQueueLook(shen_name character, shen_name target, int bone, const Vector3 *position, unsigned operation, unsigned flags, int duration, int delay) {
    HLibTask *task = 0;
    CwonLookWork *work = CwonCreateTask(&task, (HLibTaskCallback)CwonLookCallback, sizeof(*work));
    if (work) {
        work->character = character;
        work->target = target;
        work->bone = bone;
        work->operation = operation;
        work->flags = flags;
        work->duration = duration;
        work->state = 1;
        if (position)
            work->position = *position;
        else
            work->position = (Vector3){0, 0, 0};
        if (delay)
            FTBL.HLib_SetupTimedTask(task, delay);
        else
            CwonLookCallback(work);
    }
    return task;
}

void CwonLookCallback(CwonLookWork *work) {
    Vector3 position;
    if (work) {
        if (!work->state) {
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->character), 0, 0);
            FTBL.HLib_DestroyCurrentTask();
        } else {
            work->state = 0;
            if (work->target) {
                if (FTBL.DoesScenarioCharacterExist(work->target)) {
                    if (work->bone)
                        FTBL.GetCharaPosition(work->target, work->bone, &position, 0);
                    else
                        CwonGetLocalTransform(work->target, &position, 0);
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->character), &position, work->operation + (work->flags << 8));
                }
            } else
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->character), &work->position, work->operation + (work->flags << 8));
            FTBL.HLib_SetupTaskDelay(work->duration);
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

HLibTask *CwonQueueLoadingTitle(shen_name title, int delay) {
    HLibTask *task = 0;
    FTBL.Queue_ST_LoadTitleFadeIn(title);
    if (delay) {
        CwonCreateTask(&task, CwonTitleFadeCallback, 0);
        FTBL.HLib_SetupTimedTask(task, delay);
    }
    return task;
}

void CwonTitleFadeCallback(void *data) {
    FTBL.ST_LoadTitleFade();
    FTBL.HLib_DestroyCurrentTask();
    return;
}

#else
#include "s2/character.hpp"
#include "s2/scripts/cwon/cwon_helpers.hpp"
#include "s2/author.hpp"
#include "s2/debug.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/load_title.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/ui.hpp"
#include "s2/screen_film.hpp"
#include <utility>
namespace shenmue::s2 {
void CwonSetPositionMasked(shen_name name, std::uint8_t flags, const Vector3 &value) {
    if (!(flags & 0x40))
        mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    Vector3 position;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, (flags & 0x80) ? EventTransformFlags::World : EventTransformFlags::Local);
    if (flags & 8)
        position.x += value.x;
    else if (flags & 1)
        position.x = value.x;
    if (flags & 16)
        position.y += value.y;
    else if (flags & 2)
        position.y = value.y;
    if (flags & 32)
        position.z += value.z;
    else if (flags & 4)
        position.z = value.z;
    EV_AT_SetPos(name, (flags & 0x80) ? EventTransformFlags::World | EventTransformFlags::AllAxes : EventTransformFlags::AllAxes, position);
}
void CwonSetRotationMasked(shen_name name, std::uint8_t flags, const Vector3i &value) {
    if (!(flags & 0x40))
        mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(name, &rotation, (flags & 0x80) ? EventTransformFlags::World : EventTransformFlags::Local);
    if (flags & 1)
        rotation.x = value.x;
    else if (flags & 8)
        rotation.x += value.x;
    if (flags & 2)
        rotation.y = value.y;
    else if (flags & 16)
        rotation.y += value.y;
    if (flags & 4)
        rotation.z = value.z;
    else if (flags & 32)
        rotation.z += value.z;
    if (flags & 0x80)
        EV_AT_SetAngY(name, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, rotation);
}
void CwonRotateDoorCallback(void *) {
    const shen_name door{SHEN_NAME(DDOR)};
    mtwkSetRegionFlag(scenFindMotionwork(door), 71, 0, MotionRegion::LowerBody);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(door, &rotation, EventTransformFlags::Local);
    rotation.y -= 910;
    EV_AT_SetAngY(door, EventTransformFlags::AllAxes, rotation);
}
void CwonHideAtOrigin(shen_name name) {
    if (DoesScenarioCharacterExist(name)) {
        mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
        Vector3 position;
        EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        position = {};
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
        EV_ToggleCharacterFlag(name);
        EV_DisableCharacterDisplay(name);
    }
}
void CwonMoveBackward(shen_name name, float distance) {
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::Local);
    const float x = -(sin_fixed16(static_cast<std::int16_t>(rotation.y)) * distance), z = -(cos_fixed16(static_cast<std::uint16_t>(rotation.y)) * distance);
    mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    Vector3 position;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x += x;
    position.z += z;
    EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
}
std::int32_t CwonPositionThreshold(shen_name name, float threshold, bool isZ, bool reversed) {
    Vector3 position;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    const float value = isZ ? position.z : position.x;
    return reversed ? threshold > value : value > threshold;
}
std::int32_t CwonTurnTowardYaw(shen_name name, std::int32_t target, std::int32_t step, bool reverse) {
    Vector3i previous;
    GetScenarioCharacterEulerAngles(name, &previous, EventTransformFlags::Local);
    const auto yaw = static_cast<std::uint16_t>(previous.y);
    const bool reached = reverse ? (static_cast<std::int32_t>(static_cast<std::uint16_t>(yaw - target)) <= step) :
                                   (static_cast<std::int32_t>(static_cast<std::uint16_t>(target - yaw)) <= step);
    const std::int32_t next = reached ? target : (reverse ? previous.y - step : previous.y + step);
    mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::Local);
    rotation.y = next;
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, rotation);
    return reached;
}
void CwonSetPositionAnglesDegrees(shen_name name, const Vector3 &position, const Vector3 &angles) {
    mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    Vector3 previous;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &previous, EventTransformFlags::Local);
    EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
    mtwkSetRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::Local);
    rotation = {static_cast<std::uint16_t>(static_cast<std::int32_t>(angles.x * 8192.0f / 45.0f)),
        static_cast<std::uint16_t>(static_cast<std::int32_t>(angles.y * 8192.0f / 45.0f)),
        static_cast<std::uint16_t>(static_cast<std::int32_t>(angles.z * 8192.0f / 45.0f))};
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, rotation);
    EV_EnableCharacterDisplay(name);
}
void CwonDisableGravity(shen_name name) {
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
}
void CwonRestoreGravity(shen_name name) {
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
}
std::uint32_t CwonCreateDust(const Vector3 &position, const Vector3 &volume, const Vector3 &velocity) {
    const auto smoke = static_cast<std::uint32_t>(CreateSmokeEffect(position, volume, 50, 0.07999999821186066f, shen_name{}, 0));
    SetSmokeEffectFadeFactor(smoke, 27);
    SetSmokeEffectFalloff(smoke, 158);
    SetParticleEffectColour(0, smoke, 0, 106, 104, 101);
    SetParticleEffectInitialScale(0, smoke, 0.21568627655506134f);
    SetParticleEffectSize(0, smoke, 2.313725471496582f, 2.313725471496582f);
    SetParticleEffectBirthRate(0, smoke, 0.03019607998430729f);
    SetParticleEffectVelocity(0, smoke, velocity, {0.026274511590600014f, 0, 0});
    ResumeParticleEffect(0, smoke);
    return smoke;
}
void CwonResetSmoke(CwonEventWork *work) {
    if (work->smoke != -1) {
        SuspendParticleEffect(0, static_cast<std::uint32_t>(work->smoke));
        FreeParticleEffect(0, static_cast<std::uint32_t>(work->smoke));
        work->smoke = -1;
    }
}
void CwonPrepareMotionCharacters(const shen_name *names) {
    for (; names->name; ++names) {
        EV_EnsureCharacterDraw(*names);
        EV_EnableCharacterDisplay(*names);
        CwonDisableGravity(*names);
        auto *motion = scenFindMotionwork(*names);
        mtwkSetRequestedRootMode(motion, 0);
        mtwkSetRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
        mtwkSetRegionFlag(motion, 69, 2, MotionRegion::LowerBody);
        mtwkSetEnabled(motion, 0);
        EV_SetCharacterAlphaEnabled(*names, false);
    }
}
void CwonRestoreMotionCharacters(const shen_name *names) {
    for (; names->name; ++names) {
        mtwkSetEnabled(scenFindMotionwork(*names), 1);
        mtwkSetRequestedRootMode(scenFindMotionwork(*names), 3);
        EV_SetRefBoardParamsFor(*names, 1);
    }
}
std::int32_t CwonStartAuthor(std::int32_t index, HLib::Task *parent, std::int32_t frame, const shen_name *motion, const shen_name *gravity) {
    if (index != -1) {
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(CWON_AUTHORS[index].name));
    }
    auto *task = HLib::EnqueueTaskWithParameter(CwonAuthorWaitCallback, 4, 11, sizeof(CwonAuthorWaitWork), CWON_TASK);
    auto *work = task ? static_cast<CwonAuthorWaitWork *>(HLib::GetTaskParameter(task)) : nullptr;
    if (work) {
        if (motion)
            CwonPrepareMotionCharacters(motion);
        if (gravity)
            for (const shen_name *name = gravity; name->name; ++name) {
                EV_EnsureCharacterDraw(*name);
                EV_EnableCharacterDisplay(*name);
                CwonDisableGravity(*name);
            }
        if (EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) < frame)
            frame = EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining);
        work->frame = frame;
        work->parent = parent;
        work->motionCharacters = motion;
        work->gravityCharacters = gravity;
        if (parent)
            HLib::SetupTimedTask(parent, 0);
    } else
        EV_debug_log("CWON\\eCWON_et_mnevent.c", 924, "Auth Wait Error\n");
    return EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining);
}
void CwonAuthorWaitCallback(void *data) {
    auto *work = static_cast<CwonAuthorWaitWork *>(data);
    const bool loaded = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0;
    const bool frameReached = loaded && work->frame != 0 && EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) >= work->frame;
    if (!loaded || frameReached) {
        if (work->parent)
            HLib::ResolveDeferredCallback(work->parent);
        HLib::DestroyCurrentTask();
        if (!frameReached) {
            if (work->motionCharacters)
                CwonRestoreMotionCharacters(work->motionCharacters);
            if (work->gravityCharacters)
                for (const shen_name *name = work->gravityCharacters; name->name; ++name)
                    CwonRestoreGravity(*name);
        }
    }
}
void CwonLookCallback(void *data) {
    auto *work = static_cast<CwonLookWork *>(data);
    if (!work || work->state == 0) {
        if (work)
            mtwkUpdateWithPosition(scenFindMotionwork(work->character), nullptr, 0);
        HLib::DestroyCurrentTask();
        return;
    }
    work->state = 0;
    const std::uint32_t operation = work->operation + (work->flags << 8);
    if (!work->target.name)
        mtwkUpdateWithPosition(scenFindMotionwork(work->character), &work->position, operation);
    else if (DoesScenarioCharacterExist(work->target)) {
        Vector3 position;
        EV_GetCharaPosition(
            work->target, work->bone ? static_cast<std::uint32_t>(work->bone) : CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        mtwkUpdateWithPosition(scenFindMotionwork(work->character), &position, operation);
    }
    HLib::SetupTaskDelay(work->duration);
}
HLib::Task *CwonQueueLook(
    shen_name character, shen_name target, std::int32_t, std::int32_t, std::int32_t, std::uint32_t flags, std::int32_t duration, std::int32_t delay) {
    auto *task = HLib::EnqueueTaskWithParameter(CwonLookCallback, 4, 11, sizeof(CwonLookWork), CWON_TASK);
    auto *work = task ? static_cast<CwonLookWork *>(HLib::GetTaskParameter(task)) : nullptr;
    if (work) {
        work->flags = flags;
        work->duration = duration;
        work->character = character;
        work->target = target;
        work->bone = 5;
        work->operation = 1;
        work->state = 1;
        work->position = {};
        if (delay)
            HLib::SetupTimedTask(task, delay);
        else
            CwonLookCallback(work);
    }
    return task;
}
std::int32_t CwonPollQte(CwonEventWork *work, std::int32_t requested, HLib::TaskCallback pass, HLib::TaskCallback fail) {
    std::uint16_t result = 0;
    if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
        result = static_cast<std::uint16_t>(EV_GetCurrentQteStatus());
        if (result) {
            work->authorState = 0;
            if ((result & 0xfff) == requested) {
                result = 1;
                if (pass) {
                    HLib::SetCurrentTaskTo(pass);
                    pass(work);
                }
            } else {
                ++work->failures;
                result = 0xffff;
                if (fail) {
                    HLib::SetCurrentTaskTo(fail);
                    fail(work);
                }
            }
        }
    }
    return result;
}
HLib::Task *CwonQueueMotionWait(HLib::Task *parent, shen_name character, float frame) {
    auto *task = HLib::EnqueueTaskWithParameter(CwonMotionWaitCallback, 4, 11, sizeof(CwonMotionWaitWork), CWON_TASK);
    auto *work = task ? static_cast<CwonMotionWaitWork *>(HLib::GetTaskParameter(task)) : nullptr;
    HLib::SetupTimedTask(task, 1);
    if (work) {
        work->frame = frame;
        work->character = character;
        work->parent = parent;
        HLib::SetupTimedTask(parent, 0);
    }
    return task;
}
void CwonMotionWaitCallback(void *data) {
    auto *work = static_cast<CwonMotionWaitWork *>(data);
    if (work) {
        auto *motion = scenFindMotionwork(work->character);
        if (work->frame == 0) {
            if (!mtwkSetRegionFlag(motion, 65, 1, MotionRegion::LowerBody))
                return;
        } else if (mtwkGetFrame(motion, MotionRegion::LowerBody) < work->frame)
            return;
        HLib::ResolveDeferredCallback(work->parent);
    }
    HLib::DestroyCurrentTask();
}
namespace {
void TitleFadeCallback(void *) {
    ST_LoadTitleFade();
    HLib::DestroyCurrentTask();
}
} // namespace
HLib::Task *CwonQueueLoadingTitle(shen_name title, std::int32_t delay) {
    Queue_ST_LoadTitleFadeIn(title);
    auto *task = HLib::EnqueueTaskWithoutParameter(TitleFadeCallback, 4, 11, static_cast<std::int32_t>(CWON_TASK.name));
    HLib::SetupTimedTask(task, delay);
    return task;
}
namespace {
void FadeWaitCallback(void *data) {
    if (data) {
        if (filmCheckFadeTrackColour())
            return;
        HLib::ResolveDeferredCallback(*static_cast<HLib::Task **>(data));
    }
    HLib::DestroyCurrentTask();
}
} // namespace
HLib::Task *CwonQueueFadeWait(HLib::Task *parent) {
    auto *task = HLib::EnqueueTaskWithParameter(FadeWaitCallback, 4, 11, sizeof(HLib::Task *), CWON_TASK);
    auto *work = task ? static_cast<HLib::Task **>(HLib::GetTaskParameter(task)) : nullptr;
    if (work) {
        *work = parent;
        HLib::SetupTimedTask(parent, 0);
    }
    return task;
}
std::uint32_t CwonQueueTemporary(CwonEventWork *root, HLib::TaskCallback callback, std::int32_t delay) {
    std::uint32_t slot = 0;
    while (slot < 10 && root->temporaryTasks[slot])
        ++slot;
    if (slot == 10) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2512, "Tmp Taskwork error\n");
        return 0xffffffffU;
    }
    root->temporaryTasks[slot] = HLib::EnqueueTaskWithParameter(callback, 4, 11, sizeof(CwonTemporaryWork), CWON_TASK);
    auto *task = root->temporaryTasks[slot];
    auto *work = task ? static_cast<CwonTemporaryWork *>(HLib::GetTaskParameter(task)) : nullptr;
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2512, "Tmp Taskwork error\n");
        return 0xffffffffU;
    }
    work->root = root;
    work->slot = slot;
    work->ticks = 0;
    if (delay)
        HLib::SetupTimedTask(task, delay);
    EV_debug_log(
        "CWON\\eCWON_et_ChaseWon.c", 2507, "Make TmpTaskNo = %d addr:%x\n", slot, static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(callback)));
    return slot;
}
std::int64_t CwonDestroyTemporary(CwonEventWork *work, std::int32_t slot) {
    if (slot > 9 || !work->temporaryTasks[slot])
        return 0xffffffffLL;
    EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2532, "Destry TmpTaskNo = %d\n", slot);
    HLib::DestroyTaskInternal(work->temporaryTasks[slot]);
    work->temporaryTasks[slot] = nullptr;
    return 0;
}
void CwonDummyControlCallback(void *) {
    for (const auto names : {std::pair{CWON_RYO, CWON_DUMMY}, std::pair{CWON_WON, CWON_SECOND_DUMMY}})
        if (DoesScenarioCharacterExist(names.first)) {
            if (DoesScenarioCharacterExist(names.second)) {
                EventSetPosition position{0, names.second, names.first, {}, 0};
                EVT_AT_SetPosRot(&position);
                EventSetRawAngle angle{0, names.second, names.first, {}, 0};
                EVT_AT_SetAng_Callback(&angle);
            } else
                HLib::SetupTaskDelay(0);
        }
}
namespace {
void AdvanceBird(shen_name name, float speed, float height) {
    CwonDisableGravity(name);
    CwonMoveBackward(name, speed);
    EventSetPosition position{0, name, name, {0, height, 0}, 0};
    EVT_AT_SetPosRot(&position);
}
void UpdatePlayerDust(CwonTemporaryWork *work, float offset, std::int32_t line) {
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", line, "WkTask Error\n");
        return;
    }
    Vector3 position;
    EV_GetCharaPosition(CWON_RYO, 31, &position, EventTransformFlags::Local);
    position.x += offset;
    const Vector3 volume{.005f, 0, .005f};
    if (work->ticks == 0) {
        work->ticks = 1;
        if (work->root->smoke == -1)
            work->root->smoke = static_cast<std::int32_t>(CwonCreateDust(position, volume, {}));
    } else if (work->root->smoke == -1)
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
    else
        SetParticleEffectPosition(0, static_cast<std::uint32_t>(work->root->smoke), &position, &volume);
}
} // namespace
void CwonBirdCallback(void *data) {
    auto *work = static_cast<CwonTemporaryWork *>(data);
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2638, "WkTask Error\n");
        return;
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
        return;
    }
    if (work->ticks++ == 336) {
        work->ticks = 0;
        CwonSetPositionAnglesDegrees(SHEN_NAME(BIR0), {219.3f, 99.3f, 397.65f}, {});
        EV_EnsureCharacterDraw(SHEN_NAME(BIR0));
        CwonSetPositionAnglesDegrees(SHEN_NAME(BIR1), {219.5f, 99.3f, 397.65f}, {0, 165, 0});
        EV_EnsureCharacterDraw(SHEN_NAME(BIR1));
        CwonSetPositionAnglesDegrees(SHEN_NAME(BIR2), {219.1f, 99.3f, 397.65f}, {0, 200, 0});
        EV_EnsureCharacterDraw(SHEN_NAME(BIR2));
        CwonSetPositionAnglesDegrees(SHEN_NAME(BIR3), {219.3f, 99.3f, 397.44998f}, {0, 100, 0});
        EV_EnsureCharacterDraw(SHEN_NAME(BIR3));
        HLib::SetCurrentTaskTo(CwonBirdFlightCallback);
    }
}
void CwonBirdFlightCallback(void *data) {
    auto *work = static_cast<CwonTemporaryWork *>(data);
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2673, "WkTask Error\n");
        return;
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) && work->ticks < 100) {
        work->ticks += 3;
        AdvanceBird(SHEN_NAME(BIR0), .05f, .07f);
        AdvanceBird(SHEN_NAME(BIR1), .045f, work->ticks <= 15 ? 0 : .08f);
        AdvanceBird(SHEN_NAME(BIR2), .039999999f, work->ticks <= 15 ? 0 : .07f);
        AdvanceBird(SHEN_NAME(BIR3), .035f, work->ticks <= 20 ? 0 : .09f);
    } else {
        for (shen_name name : {SHEN_NAME(BIR0), SHEN_NAME(BIR1), SHEN_NAME(BIR2), SHEN_NAME(BIR3)}) {
            EV_DisableCharacterDisplay(name);
            EV_ToggleCharacterFlag(name);
        }
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
    }
}
void CwonPipeCallback(void *data) {
    auto *work = static_cast<CwonTemporaryWork *>(data);
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2754, "WkTask Error\n");
        return;
    }
    const std::int32_t tick = ++work->ticks;
    if (tick >= 200) {
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
        return;
    }
    constexpr std::uint8_t pipe[]{'P', 'I', 'P', 'E'};
    const auto frame = static_cast<std::uint32_t>(cwonPipeStep) * static_cast<std::uint32_t>(tick);
    for (std::int32_t track = 0; track < 12; ++track) {
        Vector3 position;
        Vector3i rotation;
        dynaReadEvaluate(track, static_cast<std::int32_t>(frame), &position.x, &rotation, pipe);
        position.x *= 10;
        position.y *= 10;
        position.z *= 10;
        EV_scenApplyPosOrRotToPartOf(SHEN_NAME(PIP1), static_cast<std::uint16_t>(152 + track), &position, &rotation, EventCharacterPartTransformMode::Absolute);
    }
}
void CwonPlayerDustCallback(void *data) {
    UpdatePlayerDust(static_cast<CwonTemporaryWork *>(data), 0, 2781);
}
void CwonPlayerOffsetDustCallback(void *data) {
    UpdatePlayerDust(static_cast<CwonTemporaryWork *>(data), -.5f, 2809);
}
} // namespace shenmue::s2

#endif
