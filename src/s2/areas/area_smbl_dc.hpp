#ifndef SHENMUE_S2_AREA_SMBL_DC_HPP
#define SHENMUE_S2_AREA_SMBL_DC_HPP

#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
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
    HLibTask *task;
    int field04;
    shen_name board, dealer;
    unsigned short activeBall, state, pollState, reserved16;
    unsigned int settledFrames, field1c, field20;
    DropBall balls[3];
    unsigned short nailCount;
    unsigned char settled, wins;
    Vector3 boardPosition;
    Vector3i boardRotation;
    int boardType, hasDuck;
    shen_name pockets[80];
    unsigned char reserved284[0x34];
} DropBallWork;
typedef struct SmallGameWork {
    HLibTask *task;
    int field04;
    unsigned short state, reserved0a;
    int field0c;
    unsigned int frames, wait;
    unsigned char flag, busy;
    unsigned short reserved1a;
    DropBallWork ball;
} SmallGameWork;
void QueueSmallGame(void);
int PollSmallBall(DropBallWork *work);
void InitDropBall(DropBallWork *work, HLibTask *task);
void CreateDropBalls(DropBallWork *work);
void CreateSmallNails(DropBallWork *work);
void DropBallCallback(DropBallWork *work);
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
void FollowDropBall(DropBallWork *work);
void CopyDropVector(Vector3 *target, const Vector3 *source);
void SetDropVector(Vector3 *target, float x, float y, float z);
void GetSmallNailPoint(DropBallWork *work, Vector3 *point, int board, unsigned int index);
void InitializeSmallGame(int unused, SmallGameWork *work);
void InitSmallGame(SmallGameWork *work, HLibTask *task);
void SmallGameCallback(void *data);
void FadeSmallGame(void);
void ExitSmallGame(void);
int IsNearSmallGame(void);
int ReadSmallGameInput(SmallGameWork *work);
void QueueSmallGameIntro(void);
int IsLeavingSmallGame(void);

#endif
