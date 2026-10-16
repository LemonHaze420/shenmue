#include "s2/areas/pati_event.hpp"
#include "s2/character.hpp"
#include "s2/face.hpp"
#include "s2/motion_work.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/pati_tables.hpp"
namespace shenmue::s2 {
// @verified - p
void PatiGreeting(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;
    std::int16_t v7;
    signed int *v8;
    PatiGameWork *v9;
    std::int16_t v10;
    signed int *v11;
    PatiGameWork *v12;
    std::int16_t v13;
    signed int *v14;
    PatiGameWork *v15;
    std::int16_t v16;
    signed int *v17;
    PatiGameWork *v18;
    std::int16_t v19;
    signed int *v20;
    PatiGameWork *v21;
    std::int16_t v22;
    signed int *v23;
    PatiGameWork *v24;
    std::int64_t v25;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    shenGetRandomInt32(2);
    QueuePatiVoice(&ctx->game, "A002", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->speak, 0.0, 1.0, ctx->delay);
    switch (ctx->option) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 8:
        switch (ctx->type) {
        case 1:
            RandomInt32 = shenGetRandomInt32(2);
            p_start_frm = &ctx->delay;
            p_gameData = &ctx->game;
            if (RandomInt32 == 1)
                QueuePatiVoice(p_gameData, "B003", p_start_frm);
            else
                QueuePatiVoice(p_gameData, "B002", p_start_frm);
            break;
        case 2:
            v7 = shenGetRandomInt32(2);
            v8 = &ctx->delay;
            v9 = &ctx->game;
            if (v7 == 1)
                QueuePatiVoice(v9, "B006", v8);
            else
                QueuePatiVoice(v9, "B005", v8);
            break;
        case 3:
        case 4:
            v16 = shenGetRandomInt32(2);
            v17 = &ctx->delay;
            v18 = &ctx->game;
            if (v16 == 1)
                QueuePatiVoice(v18, "B015", v17);
            else
                QueuePatiVoice(v18, "B014", v17);
            break;
        case 5:
            v10 = shenGetRandomInt32(2);
            v11 = &ctx->delay;
            v12 = &ctx->game;
            if (v10 == 1)
                QueuePatiVoice(v12, "B009", v11);
            else
                QueuePatiVoice(v12, "B008", v11);
            break;
        case 6:
            v13 = shenGetRandomInt32(2);
            v14 = &ctx->delay;
            v15 = &ctx->game;
            if (v13 == 1)
                QueuePatiVoice(v15, "B012", v14);
            else
                QueuePatiVoice(v15, "B011", v14);
            break;
        default:
            v25 = 923;
            goto LABEL_25;
        }
        break;
    case 4:
    case 5:
        v22 = shenGetRandomInt32(2);
        v23 = &ctx->delay;
        v24 = &ctx->game;
        if (v22 == 1)
            QueuePatiVoice(v24, "D003", v23);
        else
            QueuePatiVoice(v24, "D002", v23);
        break;
    case 7:
        v19 = shenGetRandomInt32(2);
        v20 = &ctx->delay;
        v21 = &ctx->game;
        if (v19 == 1)
            QueuePatiVoice(v21, "C003", v20);
        else
            QueuePatiVoice(v21, "C002", v20);
        break;
    default:
        v25 = 952;
    LABEL_25:
        PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", v25, (char *)"No Serif Data\n");
        break;
    }
    if ((std::uint8_t)(ctx->option - 1) <= 1u)
        QueuePatiVoice(&ctx->game, "E004", &ctx->delay);
}

// @verified - p
void PatiExplainRules(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::uint8_t v4;
    std::int64_t v5;
    const char *v6;
    const char *v7;
    std::int16_t v8;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;
    const char *v11;
    std::int16_t RandomInt32;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A003", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    v4 = ctx->option;
    if (v4 >= 4u) {
        if (v4 <= 5u) {
            if (ctx->type == 1) {
                v6 = "D004";
            LABEL_10:
                QueuePatiVoice(&ctx->game, v6, &ctx->delay);
                QueuePatiVoice(&ctx->game, "B017", &ctx->delay);
            } else {
                v5 = 995;
            LABEL_17:
                PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", v5, (char *)"No Serif Data\n");
            }
            goto LABEL_18;
        }
        if (v4 == 7) {
            QueuePatiVoice(&ctx->game, "C004", &ctx->delay);
            QueuePatiVoice(&ctx->game, "C005", &ctx->delay);
            goto LABEL_18;
        }
    }
    switch (ctx->type) {
    case 1:
        v6 = "B016";
        goto LABEL_10;
    case 2:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B019", &ctx->delay);
        break;
    case 3:
        v7 = "B018";
        goto LABEL_15;
    case 4:
        v7 = "B023";
    LABEL_15:
        QueuePatiVoice(&ctx->game, v7, &ctx->delay);
        QueuePatiVoice(&ctx->game, "B022", &ctx->delay);
        break;
    case 5:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B020", &ctx->delay);
        break;
    case 6:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B021", &ctx->delay);
        break;
    default:
        v5 = 1074;
        goto LABEL_17;
    }
LABEL_18:
    switch (ctx->option) {
    case 0:
    case 6:
        goto LABEL_19;
    case 1:
    case 2:
        QueuePatiVoice(&ctx->game, "E021", &ctx->delay);
        QueuePatiVoice(&ctx->game, "E022", &ctx->delay);
        QueuePatiVoice(&ctx->game, "E023", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B029", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B030", &ctx->delay);
        goto LABEL_19;
    case 4:
    case 5:
        QueuePatiVoice(&ctx->game, "D009", &ctx->delay);
        QueuePatiVoice(&ctx->game, "D010", &ctx->delay);
        QueuePatiVoice(&ctx->game, "D011", &ctx->delay);
        break;
    case 7:
        break;
    case 8:
        QueuePatiVoice(&ctx->game, "B031", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B032", &ctx->delay);
    LABEL_19:
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    default:
        PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1124, (char *)"No Serif Data\n");
        break;
    }
    if (ctx->option == 7) {
        RandomInt32 = shenGetRandomInt32(2);
        v11 = "C007";
        p_start_frm = &ctx->delay;
        if (RandomInt32 == 1)
            v11 = "C008";
        goto LABEL_33;
    }
    v8 = shenGetRandomInt32(3);
    p_start_frm = &ctx->delay;
    if (v8 == 1) {
        v11 = "B027";
    LABEL_33:
        p_gameData = &ctx->game;
        goto LABEL_34;
    }
    p_gameData = &ctx->game;
    if (v8 == 2)
        v11 = "B028";
    else
        v11 = "B026";
LABEL_34:
    QueuePatiVoice(p_gameData, v11, p_start_frm);
}

// @verified - p
void PatiExplainMuseumBoard(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    char v4;
    std::int64_t v5;
    char v6;
    const char *v7;
    std::int16_t RandomInt32;
    PatiGameWork *v9;
    const char *v10;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A025", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    v4 = ctx->option;
    if (((v4 - 4) & 0xFC) != 0 || v4 == 6) {
        switch (ctx->type) {
        case 1:
            QueuePatiVoice(&ctx->game, "B097", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B016", &ctx->delay);
            break;
        case 2:
            QueuePatiVoice(&ctx->game, "B098", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B019", &ctx->delay);
            break;
        case 3:
            QueuePatiVoice(&ctx->game, "B101", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B022", &ctx->delay);
            break;
        case 4:
            QueuePatiVoice(&ctx->game, "B102", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B103", &ctx->delay);
            break;
        case 5:
            QueuePatiVoice(&ctx->game, "B099", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B020", &ctx->delay);
            break;
        case 6:
            QueuePatiVoice(&ctx->game, "B100", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B021", &ctx->delay);
            break;
        default:
            v5 = 1247;
            goto LABEL_11;
        }
    } else {
        v5 = 1169;
    LABEL_11:
        PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", v5, (char *)"No Serif Data\n");
    }
    v6 = ctx->option;
    if (!v6)
        goto LABEL_20;
    if (v6 == 3) {
        v7 = "B030";
    LABEL_19:
        QueuePatiVoice(&ctx->game, v7, &ctx->delay);
        goto LABEL_20;
    }
    if (v6 != 6) {
        if (v6 != 8) {
            PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1281, (char *)"No Serif Data\n");
            goto LABEL_21;
        }
        QueuePatiVoice(&ctx->game, "B104", &ctx->delay);
        v7 = "B105";
        goto LABEL_19;
    }
LABEL_20:
    QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
LABEL_21:
    RandomInt32 = shenGetRandomInt32(3);
    if (RandomInt32 == 1) {
        v10 = "B027";
        v9 = &ctx->game;
    } else {
        v9 = &ctx->game;
        if (RandomInt32 == 2)
            v10 = "B028";
        else
            v10 = "B026";
    }
    QueuePatiVoice(v9, v10, &ctx->delay);
}

// @verified - p
void PatiCancel(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *p_start_frm;
    std::int16_t RandomInt32;
    const char *v6;
    std::int16_t v7;
    const char *v8;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    p_start_frm = &ctx->delay;
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v6 = "A004";
    if (RandomInt32 == 1)
        v6 = "A005";
    QueuePatiVoice(&ctx->game, v6, &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, *p_start_frm);
    v7 = shenGetRandomInt32(2);
    v8 = "B034";
    if (v7 == 1)
        v8 = "B035";
    QueuePatiVoice(&ctx->game, v8, p_start_frm);
}

// @verified - p
void PatiAcceptPlay(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "A006";
    if (RandomInt32 == 1)
        v5 = "A008";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiChooseBoard(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    QueuePatiVoice(&ctx->game, "B037", &ctx->delay);
}

// @verified - p
void PatiSelectionReminder(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    PatiGameWork *p_gameData;
    signed int start_frame;

    start_frame = 0;
    g_patiCameraTime = 99999;
    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 2, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(3);
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "B039", &start_frame);
    } else {
        p_gameData = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(p_gameData, "B040", &start_frame);
        else
            QueuePatiVoice(p_gameData, "B038", &start_frame);
    }
}

// @verified - p
void PatiBoardSelected(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(0);
    v5 = "A009";
    if (RandomInt32 == 1)
        v5 = "A012";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiInsufficientFunds(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    p_start_frm = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "B045", p_start_frm);
    } else {
        p_gameData = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(p_gameData, "B046", p_start_frm);
        else
            QueuePatiVoice(p_gameData, "B044", p_start_frm);
    }
}

// @verified - p
void PatiLowerStake(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *v4;
    std::int16_t RandomInt32;
    PatiGameWork *v6;
    const char *v7;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    v4 = &ctx->delay;
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v6 = &ctx->game;
    v7 = "B048";
    if (RandomInt32 == 1)
        v7 = "B049";
    QueuePatiVoice(v6, v7, v4);
    QueuePatiVoice(v6, "B050", v4);
}

// @verified - p
void PatiWin(PatiEventWork *ctx) {
    Character *Chara;
    signed int *p_start_frm;
    int Type;
    Character *v5;
    std::int16_t RandomInt32;
    signed int *v7;
    PatiGameWork *v8;
    const char *v9;
    PatiGameWork *v10;
    PatiGameWork *p_gameData;
    int v12;
    shen_name v13;
    Character *v14;
    std::int16_t v15;
    signed int *v16;
    Character *v17;
    std::int16_t v18;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    p_start_frm = &ctx->delay;
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    Type = GetPatiPayout(ctx);
    PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1452, (char *)"(%i,", Type + ctx->payout - ctx->totalBet);
    PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1453, (char *)"%i)", 5 * ctx->bet);
    if ((int)(GetPatiPayout(ctx) + ctx->payout - ctx->totalBet) >= 5 * ctx->bet) {
        v5 = FindCharacterByScenarioName(ctx->dealer);
        SetHighModelAndFaceFlagsForChara(v5, 10, 10.0, 1.0);
        PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1456, (char *)" Now win, and total win over 5x");
        RandomInt32 = shenGetRandomInt32(3);
        v7 = &ctx->delay;
        if (RandomInt32 == 1) {
            p_gameData = &ctx->game;
            QueuePatiVoice(p_gameData, "B053", v7);
            v9 = "B057";
            v10 = p_gameData;
        } else {
            v8 = &ctx->game;
            if (RandomInt32 == 2)
                QueuePatiVoice(v8, "B056", v7);
            else
                QueuePatiVoice(v8, "B052", v7);
            v9 = "B057";
            v10 = v8;
        }
    LABEL_18:
        v16 = p_start_frm;
        goto LABEL_19;
    }
    v12 = GetPatiPayout(ctx);
    v13 = ctx->dealer;
    if (ctx->payout - ctx->totalBet + v12 < 0) {
        v17 = FindCharacterByScenarioName(v13);
        SetHighModelAndFaceFlagsForChara(v17, 2, 10.0, 1.0);
        PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1494, (char *)"Now win, but  total lose");
        v18 = shenGetRandomInt32(2);
        v10 = &ctx->game;
        v9 = "B066";
        if (v18 == 1)
            v9 = "B067";
        goto LABEL_18;
    }
    v14 = FindCharacterByScenarioName(v13);
    SetHighModelAndFaceFlagsForChara(v14, 3, 10.0, 1.0);
    PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1474, (char *)" Now win, and total win less 5x");
    v15 = shenGetRandomInt32(4);
    v16 = &ctx->delay;
    if (v15 == 1) {
        v10 = &ctx->game;
        v9 = "B059";
    } else if (v15 == 2) {
        v10 = &ctx->game;
        v9 = "B063";
    } else {
        v10 = &ctx->game;
        if (v15 == 3)
            v9 = "B064";
        else
            v9 = "B058";
    }
LABEL_19:
    QueuePatiVoice(v10, v9, v16);
}

// @verified - p
void PatiFirstDoubleWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *p_start_frm;
    std::int16_t RandomInt32;
    signed int *v6;
    PatiGameWork *p_gameData;
    PatiGameWork *v8;
    const char *v9;
    std::int16_t v10;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    p_start_frm = &ctx->delay;
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(6);
    switch (RandomInt32) {
    case 1:
        v9 = "B059";
        goto LABEL_12;
    case 2:
        v9 = "E044";
        goto LABEL_12;
    case 3:
        v9 = "E045";
    LABEL_12:
        v6 = &ctx->delay;
        goto LABEL_13;
    }
    v6 = &ctx->delay;
    if (RandomInt32 != 4) {
        p_gameData = &ctx->game;
        v8 = p_gameData;
        if (RandomInt32 == 5)
            v9 = "B064";
        else
            v9 = "B058";
        goto LABEL_14;
    }
    v9 = "B063";
LABEL_13:
    p_gameData = &ctx->game;
    v8 = p_gameData;
LABEL_14:
    QueuePatiVoice(v8, v9, v6);
    v10 = shenGetRandomInt32(3);
    if (v10 == 1) {
        QueuePatiVoice(p_gameData, "E050", p_start_frm);
    } else if (v10 == 2) {
        QueuePatiVoice(p_gameData, "E051", p_start_frm);
    } else {
        QueuePatiVoice(p_gameData, "E049", p_start_frm);
    }
}

// @verified - p
void PatiDoubleWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;
    const char *v7;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(6);
    if (RandomInt32 == 1) {
        v7 = "E053";
        goto LABEL_12;
    }
    if (RandomInt32 == 2) {
        v7 = "B059";
    LABEL_12:
        p_start_frm = &ctx->delay;
        goto LABEL_13;
    }
    p_start_frm = &ctx->delay;
    if (RandomInt32 == 3) {
        v7 = "E044";
        goto LABEL_13;
    }
    if (RandomInt32 == 4) {
        v7 = "B064";
    LABEL_13:
        QueuePatiVoice(&ctx->game, v7, p_start_frm);
        return;
    }
    p_gameData = &ctx->game;
    if (RandomInt32 == 5)
        QueuePatiVoice(p_gameData, "E054", p_start_frm);
    else
        QueuePatiVoice(p_gameData, "E052", p_start_frm);
}

// @verified - p
void PatiFinalDoubleWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *v4;
    PatiGameWork *v5;
    std::int16_t RandomInt32;
    const char *v7;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 10, 10.0, 1.0);
    v4 = &ctx->delay;
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    v5 = &ctx->game;
    QueuePatiVoice(v5, "E072", v4);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, *v4);
    RandomInt32 = shenGetRandomInt32(2);
    v7 = "A014";
    if (RandomInt32 == 1)
        v7 = "A015";
    QueuePatiVoice(v5, v7, v4);
}

// @verified - p
void PatiFirstChallengeWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;
    const char *v7;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(4);
    v5 = &ctx->delay;
    if (RandomInt32 == 1) {
        v7 = "B059";
        goto LABEL_8;
    }
    if (RandomInt32 == 2) {
        v7 = "B063";
    LABEL_8:
        QueuePatiVoice(&ctx->game, v7, v5);
        return;
    }
    v6 = &ctx->game;
    if (RandomInt32 == 3)
        QueuePatiVoice(v6, "B064", v5);
    else
        QueuePatiVoice(v6, "B058", v5);
}

// @verified - p
void PatiChallengeWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    v5 = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "D042", v5);
    } else {
        v6 = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(v6, "D046", v5);
        else
            QueuePatiVoice(v6, "D041", v5);
    }
}

// @verified - p
void PatiFinalChallengeWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *v4;
    std::int16_t RandomInt32;
    signed int *v6;
    PatiGameWork *v7;
    PatiGameWork *v8;
    const char *v9;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 4, 10.0, 1.0);
    v4 = &ctx->delay;
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    v6 = &ctx->delay;
    if (RandomInt32 == 1) {
        v7 = &ctx->game;
        v9 = "D055";
        v8 = v7;
    } else {
        v7 = &ctx->game;
        v8 = v7;
        if (RandomInt32 == 2)
            v9 = "D056";
        else
            v9 = "D052";
    }
    QueuePatiVoice(v8, v9, v6);
    QueuePatiVoice(v7, "D058", v4);
}

// @verified - p
void PatiLose(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    int v4;
    std::int16_t v5;
    PatiGameWork *v6;
    signed int *v7;
    std::int16_t v8;
    PatiGameWork *v9;
    signed int *v10;
    std::int16_t RandomInt32;
    const char *v12;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    v4 = ctx->totalBet - ctx->payout;
    if (v4 < 5 * ctx->bet) {
        if (v4 < 0) {
            PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1712, (char *)"Now lose, but total win");
            RandomInt32 = shenGetRandomInt32(2);
            v12 = "B070";
            if (RandomInt32 == 1)
                v12 = "B071";
            QueuePatiVoice(&ctx->game, v12, &ctx->delay);
        } else {
            PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1699, (char *)" Now lose, and total lose less 5x");
            v8 = shenGetRandomInt32(2);
            v9 = &ctx->game;
            v10 = &ctx->delay;
            if (v8 == 1)
                QueuePatiVoice(v9, "B079", v10);
            else
                QueuePatiVoice(v9, "B076", v10);
        }
    } else {
        PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 1687, (char *)"Now lose, and total lose over 5x");
        v5 = shenGetRandomInt32(2);
        v6 = &ctx->game;
        v7 = &ctx->delay;
        if (v5 == 1)
            QueuePatiVoice(v6, "B074", v7);
        else
            QueuePatiVoice(v6, "B073", v7);
    }
}

// @verified - p
void PatiDoubleLose(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    PatiGameWork *p_gameData;
    const char *v10;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 4, 15.0, 1.0);
    EVT_AT_SetFaceLooks(ctx->dealer, 3, 15, 1.0f, 20);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(4);
    if (RandomInt32 == 1) {
        v10 = "B079";
        goto LABEL_8;
    }
    if (RandomInt32 == 2) {
        v10 = "E059";
    LABEL_8:
        p_gameData = &ctx->game;
        goto LABEL_9;
    }
    p_gameData = &ctx->game;
    if (RandomInt32 == 3)
        v10 = "E060";
    else
        v10 = "E055";
LABEL_9:
    QueuePatiVoice(p_gameData, v10, &ctx->delay);
}

// @verified - p
void PatiFirstChallengeLose(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    p_start_frm = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "B074", p_start_frm);
    } else {
        p_gameData = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(p_gameData, "D040", p_start_frm);
        else
            QueuePatiVoice(p_gameData, "B073", p_start_frm);
    }
}

// @verified - p
void PatiChallengeLose(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    v5 = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "D048", v5);
    } else {
        v6 = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(v6, "D049", v5);
        else
            QueuePatiVoice(v6, "D047", v5);
    }
}

// @verified - p
void PatiFinalChallengeLose(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    v5 = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "D060", v5);
    } else {
        v6 = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(v6, "D063", v5);
        else
            QueuePatiVoice(v6, "D059", v5);
    }
}

// @verified - p
void PatiOfferReplay(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "B080";
    if (RandomInt32 == 1)
        v5 = "B082";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiOfferDouble(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "E067";
    if (RandomInt32 == 1)
        v5 = "E068";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiAcceptContinue(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "A019";
    if (RandomInt32 == 1)
        v5 = "A021";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiDeclineContinue(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "A022";
    if (RandomInt32 == 1)
        v5 = "A023";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiOfferChallenge(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(3);
    v5 = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "D065", v5);
    } else {
        v6 = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(v6, "D066", v5);
        else
            QueuePatiVoice(v6, "D064", v5);
    }
}

// @verified - p
void PatiFinalChallengeRound(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    QueuePatiVoice(&ctx->game, "D069", &ctx->delay);
    QueuePatiVoice(&ctx->game, "D070", &ctx->delay);
}

// @verified - p
void PatiAfterDouble(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    QueuePatiVoice(&ctx->game, "E065", &ctx->delay);
}

// @verified - p
void PatiAfterChallenge(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;
    const char *v7;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    v5 = &ctx->delay;
    if (RandomInt32 == 1) {
        v7 = "D072";
    } else {
        v6 = &ctx->game;
        if (RandomInt32 != 2) {
            QueuePatiVoice(v6, "D071", v5);
            return;
        }
        QueuePatiVoice(v6, "D073", v5);
        v5 = &ctx->delay;
        v7 = "D074";
    }
    QueuePatiVoice(&ctx->game, v7, v5);
}

// @verified - p
void PatiPrizeExhausted(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 12, 10.0, 1.0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(3);
    p_start_frm = &ctx->delay;
    if (RandomInt32 == 1) {
        QueuePatiVoice(&ctx->game, "D076", p_start_frm);
    } else {
        p_gameData = &ctx->game;
        if (RandomInt32 == 2)
            QueuePatiVoice(p_gameData, "D077", p_start_frm);
        else
            QueuePatiVoice(p_gameData, "D075", p_start_frm);
    }
}

// @verified - p
void PatiRejectSameDoubleBoard(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    PatiGameWork *p_gameData;
    const char *v7;
    std::int16_t v8;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(2);
    p_start_frm = &ctx->delay;
    p_gameData = &ctx->game;
    v7 = "D078";
    if (RandomInt32 == 1)
        v7 = "D079";
    QueuePatiVoice(p_gameData, v7, p_start_frm);
    v8 = shenGetRandomInt32(3);
    if (v8 == 1) {
        QueuePatiVoice(p_gameData, "E081", p_start_frm);
    } else if (v8 == 2) {
        QueuePatiVoice(p_gameData, "E084", p_start_frm);
    } else {
        QueuePatiVoice(p_gameData, "D081", p_start_frm);
    }
}

// @verified - p
void PatiRejectOtherChallengeBoard(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *v5;
    PatiGameWork *v6;
    const char *v7;
    std::int16_t v8;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = &ctx->delay;
    v6 = &ctx->game;
    v7 = "D078";
    if (RandomInt32 == 1)
        v7 = "D079";
    QueuePatiVoice(v6, v7, v5);
    v8 = shenGetRandomInt32(3);
    if (v8 == 1) {
        QueuePatiVoice(v6, "D082", v5);
    } else if (v8 == 2) {
        QueuePatiVoice(v6, "D083", v5);
    } else {
        QueuePatiVoice(v6, "D081", v5);
    }
}

// @verified - p
void PatiStuck(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 7, 10.0, 1.0);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->concern, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "B084";
    if (RandomInt32 == 1)
        v5 = "B085";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
    QueuePatiVoice(&ctx->game, "B086", &ctx->delay);
    QueuePatiVoice(&ctx->game, "B087", &ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->speak, 0.0, 1.0, ctx->delay);
}

// @verified - p
void PatiClosing(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *p_start_frm;
    std::int16_t RandomInt32;
    PatiGameWork *p_gameData;
    const char *v7;
    std::int16_t v8;
    const char *v9;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 7, 10.0, 1.0);
    p_start_frm = &ctx->delay;
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    p_gameData = &ctx->game;
    v7 = "B092";
    if (RandomInt32 == 1)
        v7 = "B093";
    QueuePatiVoice(p_gameData, v7, p_start_frm);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, *p_start_frm);
    v8 = shenGetRandomInt32(2);
    v9 = "A014";
    if (v8 == 1)
        v9 = "A015";
    QueuePatiVoice(p_gameData, v9, p_start_frm);
}

// @verified - p
void PatiSelectGameCount(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *v4;
    std::int16_t RandomInt32;
    PatiGameWork *v6;
    const char *v7;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    v4 = &ctx->delay;
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v6 = &ctx->game;
    v7 = "C027";
    if (RandomInt32 == 1)
        v7 = "C028";
    QueuePatiVoice(v6, v7, v4);
    QueuePatiVoice(v6, "C031", v4);
}

// @verified - p
void PatiPrizeResult(PatiEventWork *ctx) {
    Character *ryo;
    Character *npc;
    std::int16_t v4;
    PatiGameWork *v5;
    signed int *v6;
    std::int16_t v7;
    PatiGameWork *v8;
    signed int *v9;
    std::int16_t RandomInt32;
    PatiGameWork *p_gameData;
    signed int *p_start_frm;

    ryo = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(ryo, 0, 10.0, 1.0);

    npc = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(npc, 3, 10.0, 1.0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch (PATI_PRIZES[ctx->game.prize].quantity) {
    case 1:
        RandomInt32 = shenGetRandomInt32(4);
        p_gameData = &ctx->game;
        p_start_frm = &ctx->delay;
        if (RandomInt32 == 1) {
            QueuePatiVoice(p_gameData, "C038", p_start_frm);
        } else {
            QueuePatiVoice(p_gameData, "C036", p_start_frm);
            QueuePatiVoice(&ctx->game, "C037", &ctx->delay);
        }
        break;
    case 2:
        v7 = shenGetRandomInt32(2);
        v8 = &ctx->game;
        v9 = &ctx->delay;
        if (v7 == 1)
            QueuePatiVoice(v8, "C034", v9);
        else
            QueuePatiVoice(v8, "C032", v9);
        break;
    case 3:
        v4 = shenGetRandomInt32(2);
        v5 = &ctx->game;
        v6 = &ctx->delay;
        if (v4 == 1)
            QueuePatiVoice(v5, "C043", v6);
        else
            QueuePatiVoice(v5, "C036", v6);
        break;
    }
}

// @verified - p
void PatiMissingDealer(PatiEventWork *ctx) {
    Character *Chara;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A017", &ctx->delay);
}

// @verified - p
void PatiDealerVersusWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 3, 10.0, 1.0);
    SetPatiCamera((g_patiDealerCamera + 2), ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, 6055, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "F014";
    if (RandomInt32 == 1)
        v5 = "F015";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiDealerVersusLose(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 2, 10.0, 1.0);
    SetPatiCamera((g_patiDealerCamera + 2), ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, 6045, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "F016";
    if (RandomInt32 == 1)
        v5 = "F017";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiPlayerTurn(PatiEventWork *ctx) {
    Character *Chara;
    std::int16_t RandomInt32;
    const char *v4;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v4 = "A055";
    if (RandomInt32 == 1)
        v4 = "A056";
    QueuePatiVoice(&ctx->game, v4, &ctx->delay);
}

// @verified - p
void PatiPlayerVersusWin(PatiEventWork *ctx) {
    Character *Chara;
    std::int16_t RandomInt32;
    const char *v4;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 3, 10.0, 0.5);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6044, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v4 = "A057";
    if (RandomInt32 == 1)
        v4 = "A058";
    QueuePatiVoice(&ctx->game, v4, &ctx->delay);
}

// @verified - p
void PatiPlayerVersusLose(PatiEventWork *ctx) {
    Character *Chara;
    std::int16_t RandomInt32;
    const char *voiceCode;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 2, 10.0, 0.5);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6039, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    voiceCode = "A059";
    if (RandomInt32 == 1)
        voiceCode = "A060";
    QueuePatiVoice(&ctx->game, voiceCode, &ctx->delay);
}

// @verified - p
void PatiNextVersusRound(PatiEventWork *ctx) {
    Character *Chara;
    std::int16_t RandomInt32;
    const char *v4;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v4 = "A061";
    if (RandomInt32 == 1)
        v4 = "A062";
    QueuePatiVoice(&ctx->game, v4, &ctx->delay);
}

// @verified - p
void PatiVersusSuddenDeath(PatiEventWork *ctx) {
    Character *Chara;
    signed int *v3;
    std::int16_t RandomInt32;
    PatiGameWork *v5;
    const char *v6;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = &ctx->delay;
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = &ctx->game;
    v6 = "A063";
    if (RandomInt32 == 1)
        v6 = "A064";
    QueuePatiVoice(v5, v6, v3);
    QueuePatiVoice(v5, "A065", v3);
}

// @verified - p
void PatiVersusWin(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    signed int *v4;
    signed int name;
    std::int16_t RandomInt32;
    const char *v7;
    std::int16_t v8;
    const char *v9;
    int v10;
    signed int v11;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 3, 10.0, 0.5);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 1, 10.0, 0.5);
    v4 = &ctx->delay;
    name = ctx->delay;
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, name);
    SetPatiMotion(PATI_PLAYER, 6044, 0.0, 1.0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6052, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v7 = "A066";
    if (RandomInt32 == 1)
        v7 = "A067";
    QueuePatiVoice(&ctx->game, v7, &ctx->delay);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, *v4);
    v8 = shenGetRandomInt32(2);
    v9 = "F018";
    if (v8 == 1)
        v9 = "F019";
    QueuePatiVoice(&ctx->game, v9, &ctx->delay);
    v10 = *v4 - name;
    v11 = name + 300;
    if (v10 < 300)
        v11 = *v4;
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, *v4);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, v11);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, v11);
}

// @verified - p
void PatiVersusLose(PatiEventWork *ctx) {
    signed int *v1;
    std::int32_t v2;
    Character *Chara;
    Character *v5;
    std::int16_t RandomInt32;
    const char *v7;
    std::int16_t v8;
    const char *v9;
    int v10;
    signed int v11;

    v1 = &ctx->delay;
    v2 = ctx->delay;
    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v5 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v5, 3, 10.0, 1.0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, *v1);
    SetPatiMotion(PATI_PLAYER, 6039, 0.0, 1.0, *v1);
    SetPatiMotion(ctx->dealer, 6054, 0.0, 1.0, *v1);
    RandomInt32 = shenGetRandomInt32(2);
    v7 = "A067";
    if (RandomInt32 == 1)
        v7 = "A068";
    QueuePatiVoice(&ctx->game, v7, v1);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, *v1);
    v8 = shenGetRandomInt32(2);
    v9 = "F020";
    if (v8 == 1)
        v9 = "F021";
    QueuePatiVoice(&ctx->game, v9, v1);
    v10 = *v1 - v2;
    v11 = v2 + 170;
    if (v10 < 170)
        v11 = *v1;
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, v11);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, v11);
}

// @verified - p
void PatiMuseumIntro(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;
    std::int16_t v6;
    const char *v7;

    PatiDebugLog((char *)"PATI\\ePATI_et_patinko.c", 2416, (char *)"ctx : SA_MuseumIntro\n");
    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A001", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->speak, 0.0, 1.0, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "B002";
    if (RandomInt32 == 1)
        v5 = "B003";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
    v6 = shenGetRandomInt32(3);
    if (v6 == 1) {
        v7 = "B095";
    } else if (v6 == 2) {
        v7 = "B096";
    } else {
        v7 = "B094";
    }
    QueuePatiVoice(&ctx->game, v7, &ctx->delay);
}

// @verified - p
void PatiMuseumBoardSelected(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    signed int *v6;
    PatiGameWork *p_gameData;
    PatiGameWork *v8;
    const char *v9;
    std::int16_t v10;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(5);
    p_start_frm = &ctx->delay;
    v6 = &ctx->delay;
    switch (RandomInt32) {
    case 1:
        v9 = "A027";
        break;
    case 2:
        v9 = "A028";
        break;
    case 3:
        v9 = "A029";
        break;
    default:
        p_gameData = &ctx->game;
        v8 = p_gameData;
        if (RandomInt32 == 4)
            v9 = "A030";
        else
            v9 = "A026";
        goto LABEL_11;
    }
    p_gameData = &ctx->game;
    v8 = p_gameData;
LABEL_11:
    QueuePatiVoice(v8, v9, v6);
    v10 = shenGetRandomInt32(3);
    if (v10 == 1) {
        QueuePatiVoice(p_gameData, "B037", p_start_frm);
    } else if (v10 == 2) {
        QueuePatiVoice(p_gameData, "B107", p_start_frm);
    } else {
        QueuePatiVoice(p_gameData, "B106", p_start_frm);
    }
}

// @verified - p
void PatiMuseumPlay(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    const char *v5;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(2);
    v5 = "A006";
    if (RandomInt32 == 1)
        v5 = "A008";
    QueuePatiVoice(&ctx->game, v5, &ctx->delay);
}

// @verified - p
void PatiMuseumReselect(PatiEventWork *ctx) {
    Character *Chara;
    Character *v3;
    std::int16_t RandomInt32;
    signed int *p_start_frm;
    signed int *v6;
    PatiGameWork *p_gameData;
    PatiGameWork *v8;
    const char *v9;

    Chara = FindCharacterByScenarioName(PATI_PLAYER);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 10.0, 1.0);
    v3 = FindCharacterByScenarioName(ctx->dealer);
    SetHighModelAndFaceFlagsForChara(v3, 0, 10.0, 1.0);
    RandomInt32 = shenGetRandomInt32(4);
    p_start_frm = &ctx->delay;
    v6 = &ctx->delay;
    if (RandomInt32 == 1) {
        v9 = "A032";
        goto LABEL_8;
    }
    if (RandomInt32 == 2) {
        v9 = "A033";
    LABEL_8:
        p_gameData = &ctx->game;
        v8 = p_gameData;
        goto LABEL_9;
    }
    p_gameData = &ctx->game;
    v8 = p_gameData;
    if (RandomInt32 == 3)
        v9 = "A034";
    else
        v9 = "A031";
LABEL_9:
    QueuePatiVoice(v8, v9, v6);
    QueuePatiVoice(p_gameData, "B050", p_start_frm);
}

// @verified - p
void PatiVersusStuck(PatiEventWork *ctx) {
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A073", &ctx->delay);
}

// @verified - p
void PatiVersusClosing(PatiEventWork *ctx) {
    std::int16_t RandomInt32;
    const char *v3;

    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = shenGetRandomInt32(2);
    v3 = "A069";
    if (RandomInt32 == 1)
        v3 = "A071";
    QueuePatiVoice(&ctx->game, v3, &ctx->delay);
    QueuePatiVoice(&ctx->game, "A070", &ctx->delay);
}
} // namespace shenmue::s2
