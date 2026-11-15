#ifndef SHENMUE_S2_AREA_DRBL_DC_HPP
#define SHENMUE_S2_AREA_DRBL_DC_HPP

#include "s2/events/event_dc.h"
#include "s2/event_library_tail_dc.h"
typedef struct DropBall {
    shen_name character;
    Vector3 position, previous, velocity, previousVelocity, segment;
    float lowHeight, highHeight;
    unsigned char quietX, quietY;
    unsigned short reserved;
    int angle, scored;
} DropBall;
typedef struct DropBallWork {
    HLibTask *parent, *task;
    int field08;
    shen_name board;
    int field10;
    short mode;
    unsigned short pocketCount, activeBall;
    unsigned short state;
    unsigned int settledFrames;
    int field20, field24;
    DropBall balls[3];
    unsigned char *wins;
    int sound;
    char soundFrames;
    unsigned char padding12d[3];
    Vector3 boardPosition;
    Vector3i boardRotation;
    int reserved148;
    shen_name pockets[8];
    int field16c;
    int field170;
    int field174;
    int field178;
    int field17c;
    float top, bottom, left, right, scale;
    int reserved194;
} DropBallWork;
void QueueDropBall(DropBallWork *argument);
void InitDropBall(DropBallWork *work, HLibTask *task);
void CreateDropBalls(DropBallWork *work);
void CreateDropPockets(DropBallWork *work);
void DropBallCallback(void *data);
void ShowDropPlayChoice(DropBallWork *work);
void ReadDropPlayChoice(DropBallWork *work);
void ReadDropQuitChoice(DropBallWork *work);
void FreeDropBall(DropBallWork *work);
void HandleDropBallInput(DropBallWork *work);
void AdvanceDropBall(DropBallWork *work);
void IntegrateDropBall(DropBallWork *work);
void BounceDropFloor(DropBallWork *work);
void BounceDropWall(DropBallWork *work);
void FindDropNails(DropBallWork *work, unsigned char mode, unsigned int previous);
int ProbeDropBall(DropBallWork *work, Vector3 point, unsigned char mode);
void BounceDropNail(DropBallWork *work, int nail, float restitution, Vector3 point, unsigned char mode);
void FindDropDividers(DropBallWork *work);
void CheckDropDivider(DropBallWork *work, Vector3 point);
int ProbeDropLine(Vector3 first, Vector3 second, Vector3 third, Vector3 fourth);
void BounceDropLine(DropBallWork *work, Vector3 first, Vector3 second);
void SettleDropBall(DropBallWork *work);
void ScoreDropBall(DropBallWork *work);
void PositionDropBall(DropBallWork *work);
void PlayDropSound(DropBallWork *work, short material);
void FollowDropBall(DropBallWork *work);
void CopyDropVector(Vector3 *target, const Vector3 *source);
void SetDropVector(Vector3 *target, float x, float y, float z);

#endif
