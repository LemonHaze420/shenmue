#ifndef SHENMUE_S2_AREAS_AREA_LOOP_DC_H
#define SHENMUE_S2_AREAS_AREA_LOOP_DC_H
#include "s2/events/event_dc.h"

typedef int (*AreaLoopCompletion)(int phase, int value, HLibTask *parent);

typedef struct AreaLoopVoiceWork {
    HLibTask *parent;
    void *conversation;
    unsigned int flags;
    int duration, elapsed;
    AreaLoopCompletion completion;
    char *voice;
} AreaLoopVoiceWork;

typedef struct AreaLoopCameraWork {
    HLibTask *parent;
    unsigned int flags;
    int camera;
    shen_name character;
    AreaLoopCompletion completion;
    char *area;
} AreaLoopCameraWork;

typedef struct AreaLoopMotionWork {
    HLibTask *parent;
    shen_name character;
    unsigned int flags, motion;
    int duration, elapsed;
    float startFrame, endFrame, speed, frameTime;
    AreaLoopCompletion completion;
} AreaLoopMotionWork;

typedef struct AreaLoopPositionWork {
    HLibTask *parent;
    shen_name character;
    unsigned int flags;
    Vector3 position;
    Vector3i rotation;
    AreaLoopCompletion completion;
} AreaLoopPositionWork;

typedef struct AreaLoopAttachmentWork {
    HLibTask *parent;
    int elapsed;
    shen_name character;
    int bone;
    shen_name target;
    unsigned int flags;
    Vector3 position;
    Vector3i rotation;
    int duration;
    AreaLoopCompletion completion;
} AreaLoopAttachmentWork;

typedef struct AreaLoopNpcWork {
    HLibTask *parent;
    int previousLoadState, remaining;
    shen_name *characters;
    AreaLoopCompletion completion;
} AreaLoopNpcWork;

typedef struct AreaLoopAuthorWork {
    HLibTask *parent;
    unsigned int flags;
    char *package;
    shen_name author;
#if defined(SHENMUE_AREA_LOOP_AUTHOR_COUNTER)
    int frame;
#endif
    int endFrame;
    AreaLoopCompletion completion;
} AreaLoopAuthorWork;

typedef struct AreaLoopLookAtWork {
    HLibTask *parent;
    unsigned int flags;
    shen_name character, target;
    int bone;
    Vector3 position;
    int elapsed, duration;
    AreaLoopCompletion completion;
} AreaLoopLookAtWork;

typedef struct AreaLoopPackageWork {
    HLibTask *parent;
    unsigned int flags;
    char *package, *folder;
    AreaLoopCompletion completion;
} AreaLoopPackageWork;

void AreaLoopVoiceCallback(void *data);
void AreaLoopCameraCallback(void *data);
void AreaLoopMotionCallback(void *data);
void AreaLoopPositionCallback(void *data);
void AreaLoopAttachmentCallback(void *data);
void AreaLoopNpcCallback(void *data);
void AreaLoopAuthorCallback(void *data);
void AreaLoopLookAtCallback(void *data);
void AreaLoopPackageCallback(void *data);
void AreaLoopSoundCallback(void *data);

void QueueAreaLoopVoice(const char *voice, int dialogue, int delay, int duration, AreaLoopCompletion completion);
void QueueAreaLoopCamera(const char *area, int camera, shen_name character, int delay, AreaLoopCompletion completion);
void QueueAreaLoopMotion(shen_name character, unsigned int motion, int delay, int duration, float startFrame, float endFrame, float speed,
                         float frameTime, AreaLoopCompletion completion, unsigned int flags);
void QueueAreaLoopPosition(shen_name character, int delay, int local, const Vector3 *position, const Vector3i *rotation, AreaLoopCompletion completion);
void QueueAreaLoopAttachment(shen_name character, int bone, shen_name target, int delay, int duration, const Vector3 *position,
                             const Vector3i *rotation, AreaLoopCompletion completion);
void SetAreaLoopCharacterControl(shen_name character, int scripted);
int QueueAreaLoopNpcs(const shen_name *characters, AreaLoopCompletion completion);
void RestoreAreaLoopCharacters(const shen_name *characters, const shen_name *retained);
int QueueAreaLoopAuthor(const char *package, shen_name author, int delay, int endFrame, AreaLoopCompletion completion);
void QueueAreaLoopLookAt(shen_name character, shen_name target, int bone, const Vector3 *position, int delay, int duration,
                         AreaLoopCompletion completion);
int QueueAreaLoopPackage(const char *folder, const char *package, AreaLoopCompletion completion);
void QueueAreaLoopSound(unsigned int sound, int delay);
int ResolveAreaLoopParent(int phase, int value, HLibTask *parent);
extern const int areaLoopHandPoses[52][57];

#endif
