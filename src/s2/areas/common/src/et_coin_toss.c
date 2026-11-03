#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_coin_toss.h"

static shen_name coinTossState = 0;
static short coinTossChoices[] = {-1, 231, 232, -1};
static int coinTossTimer;
static int coinTossResult;
static shen_name coinTossRyoBase;
static shen_name coinTossRenBase;

void CoinTossInit(const Vector3 *position, Vector3i *rotation)
{
    coinTossState = COIN_TOSS_INIT;
    FTBL.CreateEventCharacter(3, 0, COIN_TOSS_BASE);
    FTBL.EV_AT_SetPos(COIN_TOSS_BASE, 0x78000000U, position);
    FTBL.EV_AT_SetAngY_0(COIN_TOSS_BASE, 0x38000000U, rotation, 0);
    FTBL.EV_ToggleCharacterFlag(COIN_TOSS_BASE);
    coinTossRyoBase = FTBL.SCEN_FindChar_Wrapper(COIN_TOSS_RYO);
    coinTossRenBase = FTBL.SCEN_FindChar_Wrapper(COIN_TOSS_REN);
    FTBL.AT_SetLocalBaseChara(COIN_TOSS_RYO, COIN_TOSS_BASE);
    FTBL.AT_SetLocalBaseChara(COIN_TOSS_REN, COIN_TOSS_BASE);
    FTBL.EV_FreeAuthorPak(1, "0280");
}

int CoinTossUpdate(void)
{
    if (coinTossTimer > 0)
        coinTossTimer--;
    switch (coinTossState) {
    case COIN_TOSS_IDLE:
        return 0;
    case COIN_TOSS_INIT:
        CoinTossStart();
        return 0;
    case COIN_TOSS_START:
        CoinTossSelect();
        return 0;
    case COIN_TOSS_SELECT:
        if (!coinTossTimer)
            CoinTossMenu();
        return 0;
    case COIN_TOSS_CHOICE:
        if (!FTBL.GetShouldRenderIcons(2))
            CoinTossChoose();
        return 0;
    case COIN_TOSS_AFTER:
        CoinTossAfter();
        return 0;
    case COIN_TOSS_NONE:
        CoinTossNone();
        return 0;
    case COIN_TOSS_END:
        if (!coinTossTimer) {
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            coinTossState = COIN_TOSS_FADE;
        }
        break;
    case COIN_TOSS_FADE:
        if (FTBL.filmCheckFadeTrackColour())
            return 0;
        CoinTossFinish();
        coinTossState = COIN_TOSS_IDLE;
        return 1;
    default:
        FTBL.EventAssertMessageNullsub(__FILE__, 171, "CoinToss : Undefined mode2\n");
        break;
    }
    return 0;
}

void CoinTossStart(void)
{
    EVT_AT_SetPos(COIN_TOSS_RYO, COIN_TOSS_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(COIN_TOSS_RYO, COIN_TOSS_BASE, 0.0f, 0);
    EVT_AT_SetPos(COIN_TOSS_REN, COIN_TOSS_BASE, -0.85f, 0.0f, -0.5f, 0);
    EVT_AT_SetAngY(COIN_TOSS_REN, COIN_TOSS_BASE, 45.0f, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(COIN_TOSS_RYO), 1);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(COIN_TOSS_REN), 1);
    FTBL.EVS_LoopCamera(0, COIN_TOSS_BASE, COIN_TOSS_BASE);
    coinTossState = COIN_TOSS_START;
}

void CoinTossSelect(void)
{
    EVT_AT_SetPoint(COIN_TOSS_RYO, COIN_TOSS_REN, 5, 5, 1, 0, 300, 0);
    EVT_AT_SetMot(COIN_TOSS_RYO, 0xf003, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(COIN_TOSS_REN, 8838, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetCam(1000, COIN_TOSS_BASE, COIN_TOSS_BASE, 0, 0, 0);
    QueueEventMessage("●青あそこか…", 30, 60);
    EVT_AT_SetCam(1001, COIN_TOSS_BASE, COIN_TOSS_BASE, 0, 0, 90);
    QueueEventMessage("●青誰がやる？", 90, 60);
    EVT_AT_SetPoint(COIN_TOSS_REN, COIN_TOSS_RYO, 5, 5, 1, 180, 120, 0);
    EVT_AT_SetVoice("0280B001", 180);
    EVT_AT_SetVoice("0280B002", 198);
    EVT_AT_SetVoice("0280B003", 312);
    coinTossTimer = 382;
    coinTossState = COIN_TOSS_SELECT;
}

void CoinTossMenu(void)
{
    FTBL.SetupControlSpeakAndUI(coinTossChoices, 600, 0xffff, 2);
    coinTossState = COIN_TOSS_CHOICE;
}

void CoinTossChoose(void)
{
    switch (FTBL.shenGetCtrlSpeakIconFlag()) {
    case 231:
        coinTossResult = 1;
        coinTossState = COIN_TOSS_AFTER;
        break;
    case 232:
        coinTossResult = 0;
        coinTossState = COIN_TOSS_AFTER;
        break;
    default:
        coinTossState = COIN_TOSS_NONE;
        break;
    }
}

void CoinTossAfter(void)
{
    unsigned int flags = coinTossResult ? 0x20000000U : 0;
    int delay;
    EVT_AT_SetPos(COIN_TOSS_RYO, COIN_TOSS_BASE, 0.6f, 0.0f, 0.2f, 120);
    EVT_AT_SetAngY(COIN_TOSS_RYO, COIN_TOSS_BASE, 45.0f, 120);
    EVT_AT_SetPos(COIN_TOSS_REN, COIN_TOSS_BASE, -0.5f, 0.0f, -0.5f, 120);
    EVT_AT_SetAngY(COIN_TOSS_REN, COIN_TOSS_BASE, 270.0f, 120);
    EVT_AT_SetMot(COIN_TOSS_REN, 8837, 0.0f, 1.0f, flags | 0x40000000U, 120);
    delay = EVT_AT_SetMot(COIN_TOSS_REN, 8835, 0.0f, 1.0f, flags, 190);
    EVT_AT_SetMot(COIN_TOSS_REN, 8833, 70.0f, 1.0f, 0, delay);
    EVT_AT_SetCam(1002, COIN_TOSS_BASE, COIN_TOSS_BASE, 0, 0, 0);
    EVT_AT_SetCam(1003, COIN_TOSS_BASE, COIN_TOSS_BASE, 0, 0, 120);
    if (coinTossResult) {
        EVT_AT_SetVoice("0280A002", 0);
        EVT_AT_SetVoice("0280B004", 18);
        EVT_AT_SetVoice("0280B006", delay + 20);
    } else {
        EVT_AT_SetVoice("0280A003", 0);
        EVT_AT_SetVoice("0280B004", 18);
        EVT_AT_SetVoice("0280B005", delay + 20);
    }
    coinTossState = COIN_TOSS_END;
    coinTossTimer = delay + 160;
}

void CoinTossNone(void)
{
    EVT_AT_SetPos(COIN_TOSS_REN, COIN_TOSS_BASE, -0.8f, 0.0f, -0.5f, 0);
    EVT_AT_SetAngY(COIN_TOSS_REN, COIN_TOSS_BASE, 270.0f, 0);
    EVT_AT_SetMot(COIN_TOSS_REN, 8842, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetCam(1004, COIN_TOSS_BASE, COIN_TOSS_BASE, 0, 0, 0);
    QueueEventMessage("何ボサッとしてやがんだ", 0, 60);
    QueueEventMessage("やらねえんなら＆お前が行け", 60, 120);
    coinTossState = COIN_TOSS_END;
    coinTossTimer = 180;
}

void CoinTossFinish(void)
{
    FTBL.AT_SetLocalBaseChara(COIN_TOSS_RYO, coinTossRyoBase);
    FTBL.AT_SetLocalBaseChara(COIN_TOSS_REN, coinTossRenBase);
    FTBL.EV_FreeAuthorPak(0, "0280");
}
#endif
