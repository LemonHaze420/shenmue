#include "s2/darts_conversation.hpp"
#include "s2/areas/area_dart.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/face.hpp"
#include "s2/motion_work.hpp"

namespace shenmue::s2 {

std::int32_t dartsPlayerWon{};

ConversationBuffer *PlayDartsOpeningResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsReadyResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsExitResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsContinueResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsLastThrowResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsZeroScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsLowScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsLowerMiddleScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsMiddleScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsUpperMiddleScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsBullseyeResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsHighScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsEndThrowsResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsRoundResultResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsBonusResultResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsFinalResultResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsGameOverResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsReplayResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsStopResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsDeclineOpponentResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsWonBetResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsOpponentPaymentResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsLostBetResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsOpponentReplayResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsOpponentBonusReplayResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsOpponentFinalResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsThrowStartResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsThrowResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsRoundOverResponse(std::int32_t mode, bool playerTurn, shen_name opponent);

ConversationBuffer *PlayDartsOpeningResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v1;
    random = GetRandomNumberInRange(3.0);
    v1 = "0113A001";
    if (random >= 1.5)
        v1 = "0113A002";
    return EV_PlayDialogue(v1);
}

ConversationBuffer *PlayDartsReadyResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v2;
    random = GetRandomNumberInRange(3.0);
    if (random < 1.0)
        return EV_PlayDialogue("0113A003");
    v2 = "0113A004";
    if (random >= 2.0)
        v2 = "0113A005";
    return EV_PlayDialogue(v2);
}

ConversationBuffer *PlayDartsExitResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v2;
    random = GetRandomNumberInRange(3.0);
    if (random < 1.0)
        return EV_PlayDialogue("0113A006");
    v2 = "0113A007";
    if (random >= 2.0)
        v2 = "0113A008";
    return EV_PlayDialogue(v2);
}

ConversationBuffer *PlayDartsContinueResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v2;
    random = GetRandomNumberInRange(3.0);
    if (random < 1.0)
        return EV_PlayDialogue("0113A009");
    v2 = "0113A010";
    if (random >= 2.0)
        v2 = "0113A011";
    return EV_PlayDialogue(v2);
}

ConversationBuffer *PlayDartsLastThrowResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *voiceCode;
    random = GetRandomNumberInRange(3.0);
    if (playerTurn == 0)
        goto LABEL_5;
    if (random < 1.0) {
        return EV_PlayDialogue("0113A032");
    }
    voiceCode = "0113A034";
    if (random < 2.0)
    LABEL_5:
        voiceCode = "0113A033";
    return EV_PlayDialogue(voiceCode);
}

ConversationBuffer *PlayDartsZeroScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v8;
    random = GetRandomNumberInRange(3.0);
    if (mode == 0) {
        if (random >= 1.0)
            return EV_PlayDialogue("0113A017");
        else
            return EV_PlayDialogue("0113A016");
    }
    if (playerTurn != 0) {
        if (opponent.name != areas::DART_FCJ.name) {
            if (opponent.name != areas::DART_HLN.name) {
                if (opponent.name == areas::DART_JON.name) {
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137B052");
                        else
                            return EV_PlayDialogue("0137B039");
                    } else {
                        return EV_PlayDialogue("0137B038");
                    }
                } else if (opponent.name == areas::DART_HCY.name) {
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137F046");
                        else
                            return EV_PlayDialogue("0137F045");
                    } else {
                        return EV_PlayDialogue("0137F044");
                    }
                } else if (random >= 0.5) {
                    if (random >= 1.0) {
                        if (random >= 1.5 && (random < 2.0 || random >= 2.5))
                            return EV_PlayDialogue("0137E037");
                        else
                            return EV_PlayDialogue("0137E036");
                    } else {
                        return EV_PlayDialogue("0137E035");
                    }
                } else {
                    return EV_PlayDialogue("0137E034");
                }
            }
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137C039");
                else
                    return EV_PlayDialogue("0137C038");
            }
            return EV_PlayDialogue("0137C023");
        }
        if (random < 1.0 || random < 2.0)
            return EV_PlayDialogue("0137D042");
        else
            return EV_PlayDialogue("0137D043");
    } else {
        if (opponent.name != areas::DART_FCJ.name) {
            if (opponent.name != areas::DART_HLN.name) {
                if (opponent.name == areas::DART_JON.name) {
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137B031");
                        else
                            return EV_PlayDialogue("0137B030");
                    } else {
                        return EV_PlayDialogue("0137B029");
                    }
                } else if (opponent.name == areas::DART_HCY.name) {
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137F030");
                        else
                            return EV_PlayDialogue("0137F029");
                    } else {
                        return EV_PlayDialogue("0137F028");
                    }
                } else if (random >= 0.5) {
                    if (random >= 1.0) {
                        if (random >= 2.0) {
                            if (random >= 2.5)
                                return EV_PlayDialogue("0137E025");
                            else
                                return EV_PlayDialogue("0137E027");
                        } else {
                            return EV_PlayDialogue("0137E026");
                        }
                    } else {
                        return EV_PlayDialogue("0137E024");
                    }
                } else {
                    return EV_PlayDialogue("0137E023");
                }
            }
            if (random < 1.0)
                return EV_PlayDialogue("0137C024");
            if (random < 2.0)
                return EV_PlayDialogue("0137C025");
            return EV_PlayDialogue("0137C023");
        }
        if (random >= 1.0) {
            v8 = "0137D027";
            if (random >= 2.0)
                v8 = "0137D028";
            return EV_PlayDialogue(v8);
        } else {
            return EV_PlayDialogue("0137D026");
        }
    }
}

ConversationBuffer *PlayDartsLowScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v8;
    random = GetRandomNumberInRange(3.0);
    if (mode == 0)
        return EV_PlayDialogue("0113A013");
    if (playerTurn != 0) {
        switch (opponent.name) {
        case areas::DART_FCJ.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137D040");
                else
                    return EV_PlayDialogue("0137D039");
            } else {
                return EV_PlayDialogue("0137D038");
            }
        case areas::DART_HLN.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137C037");
                else
                    return EV_PlayDialogue("0137C036");
            } else {
                return EV_PlayDialogue("0137C035");
            }
        case areas::DART_JON.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137B037");
                else
                    return EV_PlayDialogue("0137B036");
            } else {
                return EV_PlayDialogue("0137B035");
            }
        case areas::DART_HCY.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137F043");
                else
                    return EV_PlayDialogue("0137F042");
            } else {
                return EV_PlayDialogue("0137F041");
            }
        default:
            if (random >= 0.5) {
                if (random >= 1.0) {
                    if (random >= 1.5 && (random < 2.0 || random >= 2.5))
                        return EV_PlayDialogue("0137E037");
                    else
                        return EV_PlayDialogue("0137E036");
                } else {
                    return EV_PlayDialogue("0137E035");
                }
            } else {
                return EV_PlayDialogue("0137E034");
            }
        }
    } else {
        switch (opponent.name) {
        case areas::DART_FCJ.name:
            if (random >= 1.0) {
                v8 = "0137D024";
                if (random >= 2.0)
                    v8 = "0137D025";
                return EV_PlayDialogue(v8);
            } else {
                return EV_PlayDialogue("0137D023");
            }
        case areas::DART_HLN.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137C022");
                else
                    return EV_PlayDialogue("0137C021");
            } else {
                return EV_PlayDialogue("0137C020");
            }
        case areas::DART_JON.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137B028");
                else
                    return EV_PlayDialogue("0137B051");
            } else {
                return EV_PlayDialogue("0137B050");
            }
        case areas::DART_HCY.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    return EV_PlayDialogue("0137F027");
                else
                    return EV_PlayDialogue("0137F026");
            } else {
                return EV_PlayDialogue("0137F025");
            }
        default:
            if (random >= 0.5) {
                if (random >= 1.0) {
                    if (random >= 2.0) {
                        if (random >= 2.5)
                            return EV_PlayDialogue("0137E025");
                        else
                            return EV_PlayDialogue("0137E027");
                    } else {
                        return EV_PlayDialogue("0137E026");
                    }
                } else {
                    return EV_PlayDialogue("0137E024");
                }
            } else {
                return EV_PlayDialogue("0137E023");
            }
        }
    }
}

ConversationBuffer *PlayDartsLowerMiddleScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    return PlayDartsMiddleScoreResponse(mode, playerTurn, opponent);
}

ConversationBuffer *PlayDartsMiddleScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v8;
    random = GetRandomNumberInRange(3.0);
    if (mode == 0) {
        if (random >= 1.5)
            return EV_PlayDialogue("0113A019");
        else
            return EV_PlayDialogue("0113A018");
    }
    if (playerTurn != 0) {
        if (opponent.name == areas::DART_FCJ.name) {
            if (random < 1.0)
                return EV_PlayDialogue("0137D035");
            if (random >= 2.0)
                return EV_PlayDialogue("0137D037");
            return EV_PlayDialogue("0137D036");
        }
        if (opponent.name == areas::DART_HLN.name) {
            if (random < 1.0)
                return EV_PlayDialogue("0137C032");
            if (random >= 2.0)
                return EV_PlayDialogue("0137C034");
            return EV_PlayDialogue("0137C033");
        }
        if (opponent.name != areas::DART_JON.name) {
            if (opponent.name == areas::DART_HCY.name) {
                if (random >= 1.0) {
                    if (random >= 2.0)
                        return EV_PlayDialogue("0137F040");
                    else
                        return EV_PlayDialogue("0137F039");
                } else {
                    return EV_PlayDialogue("0137F038");
                }
            } else if (random >= 0.5) {
                if (random >= 1.0) {
                    if (random >= 1.5 && (random < 2.0 || random >= 2.5))
                        return EV_PlayDialogue("0137E037");
                    else
                        return EV_PlayDialogue("0137E036");
                } else {
                    return EV_PlayDialogue("0137E035");
                }
            } else {
                return EV_PlayDialogue("0137E034");
            }
        }
        if (random < 1.0)
            return EV_PlayDialogue("0137B025");
    LABEL_27:
        if (random >= 2.0)
            return EV_PlayDialogue("0137B027");
        else
            return EV_PlayDialogue("0137B026");
    }
    if (opponent.name != areas::DART_FCJ.name) {
        if (opponent.name == areas::DART_HLN.name) {
            if (random < 1.0)
                return EV_PlayDialogue("0137C017");
            if (random >= 2.0)
                return EV_PlayDialogue("0137C019");
            return EV_PlayDialogue("0137C018");
        }
        if (opponent.name != areas::DART_JON.name) {
            if (opponent.name == areas::DART_HCY.name) {
                if (random >= 1.0) {
                    if (random >= 2.0)
                        return EV_PlayDialogue("0137F024");
                    else
                        return EV_PlayDialogue("0137F023");
                } else {
                    return EV_PlayDialogue("0137F022");
                }
            } else if (random >= 0.5) {
                if (random >= 1.0) {
                    if (random >= 2.0) {
                        if (random >= 2.5)
                            return EV_PlayDialogue("0137E025");
                        else
                            return EV_PlayDialogue("0137E027");
                    } else {
                        return EV_PlayDialogue("0137E026");
                    }
                } else {
                    return EV_PlayDialogue("0137E024");
                }
            } else {
                return EV_PlayDialogue("0137E023");
            }
        }
        if (random < 1.0)
            return EV_PlayDialogue("0137B025");
        goto LABEL_27;
    }
    if (random < 1.0)
        return EV_PlayDialogue("0137D020");
    v8 = "0137D021";
    if (random >= 2.0)
        v8 = "0137D022";
    return EV_PlayDialogue(v8);
}

ConversationBuffer *PlayDartsUpperMiddleScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    if (mode != 0)
        return PlayDartsHighScoreResponse(mode, playerTurn, opponent);
    else
        return EV_PlayDialogue("0113A014");
}

ConversationBuffer *PlayDartsBullseyeResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v6;
    if (mode != 0 && opponent.name == areas::DART_XHO.name) {
        random = GetRandomNumberInRange(3.0);
        if (playerTurn != 0) {
            if (random < 1.0)
                return EV_PlayDialogue("0137E031");
            if (random < 1.5)
                return EV_PlayDialogue("0137E032");
            if (random < 2.5)
                return EV_PlayDialogue("0137E033");
        } else {
            if (random < 1.0)
                return EV_PlayDialogue("0137E019");
            if (random < 1.5)
                return EV_PlayDialogue("0137E020");
            v6 = "0137E021";
            if (random < 2.5)
                return EV_PlayDialogue(v6);
        }
        v6 = "0137E022";
        return EV_PlayDialogue(v6);
    }
    return PlayDartsHighScoreResponse(mode, playerTurn, opponent);
}

ConversationBuffer *PlayDartsHighScoreResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v8;
    random = GetRandomNumberInRange(3.0);
    if (mode == 0)
        return EV_PlayDialogue("0113A015");
    if (playerTurn != 0) {
        if (opponent.name == areas::DART_FCJ.name) {
            if (random < 1.0)
                return EV_PlayDialogue("0137D032");
            if (random >= 2.0)
                return EV_PlayDialogue("0137D034");
            return EV_PlayDialogue("0137D033");
        }
        if (opponent.name == areas::DART_HLN.name) {
            if (random < 1.0)
                return EV_PlayDialogue("0137C030");
            if (random >= 2.0)
                return EV_PlayDialogue("0137C031");
            return EV_PlayDialogue("0137C029");
        }
        if (opponent.name != areas::DART_JON.name) {
            if (opponent.name == areas::DART_HCY.name) {
                if (random >= 1.0) {
                    if (random >= 1.5) {
                        if (random >= 2.5)
                            return EV_PlayDialogue("0137F037");
                        else
                            return EV_PlayDialogue("0137F036");
                    } else {
                        return EV_PlayDialogue("0137F035");
                    }
                } else {
                    return EV_PlayDialogue("0137F034");
                }
            } else if (random >= 0.5) {
                if (random >= 1.0) {
                    if (random >= 1.5 && (random < 2.0 || random >= 2.5))
                        return EV_PlayDialogue("0137E037");
                    else
                        return EV_PlayDialogue("0137E036");
                } else {
                    return EV_PlayDialogue("0137E035");
                }
            } else {
                return EV_PlayDialogue("0137E034");
            }
        }
        if (random < 1.0)
            return EV_PlayDialogue("0137B021");
        if (random < 1.5)
            return EV_PlayDialogue("0137B022");
    LABEL_29:
        if (random >= 2.5)
            return EV_PlayDialogue("0137B024");
        else
            return EV_PlayDialogue("0137B023");
    }
    if (opponent.name != areas::DART_FCJ.name) {
        if (opponent.name == areas::DART_HLN.name) {
            if (random < 1.0)
                return EV_PlayDialogue("0137C014");
            if (random >= 2.0)
                return EV_PlayDialogue("0137C016");
            return EV_PlayDialogue("0137C015");
        }
        if (opponent.name != areas::DART_JON.name) {
            if (opponent.name == areas::DART_HCY.name) {
                if (random >= 1.0) {
                    if (random >= 2.0)
                        return EV_PlayDialogue("0137F021");
                    else
                        return EV_PlayDialogue("0137F020");
                } else {
                    return EV_PlayDialogue("0137F019");
                }
            } else if (random >= 0.5) {
                if (random >= 1.0) {
                    if (random >= 2.0) {
                        if (random >= 2.5)
                            return EV_PlayDialogue("0137E025");
                        else
                            return EV_PlayDialogue("0137E027");
                    } else {
                        return EV_PlayDialogue("0137E026");
                    }
                } else {
                    return EV_PlayDialogue("0137E024");
                }
            } else {
                return EV_PlayDialogue("0137E023");
            }
        }
        if (random < 1.0)
            return EV_PlayDialogue("0137B021");
        if (random < 1.5)
            return EV_PlayDialogue("0137B022");
        goto LABEL_29;
    }
    if (random < 1.0)
        return EV_PlayDialogue("0137D017");
    v8 = "0137D018";
    if (random >= 1.5)
        v8 = "0137D019";
    return EV_PlayDialogue(v8);
}

ConversationBuffer *PlayDartsEndThrowsResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v5;
    random = GetRandomNumberInRange(3.0);
    if (playerTurn == 0)
        goto LABEL_5;
    if (random < 1.0)
        return EV_PlayDialogue("0113A023");
    v5 = "0113A024";
    if (random >= 2.0)
    LABEL_5:
        v5 = "0113A025";
    return EV_PlayDialogue(v5);
}

ConversationBuffer *PlayDartsRoundResultResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v8;
    random = GetRandomNumberInRange(3.0);
    if (mode != 0) {
        if (playerTurn != 0) {
            if (dartsPlayerWon != 0) {
                switch (opponent.name) {
                case areas::DART_FCJ.name:
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137D067");
                        else
                            return EV_PlayDialogue("0137D066");
                    } else {
                        return EV_PlayDialogue("0137D065");
                    }
                case areas::DART_HLN.name:
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137C055");
                        else
                            return EV_PlayDialogue("0137C054");
                    } else {
                        return EV_PlayDialogue("0137C053");
                    }
                case areas::DART_JON.name:
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137B061");
                        else
                            return EV_PlayDialogue("0137B060");
                    } else {
                        return EV_PlayDialogue("0137B059");
                    }
                case areas::DART_HCY.name:
                    if (random >= 1.0) {
                        if (random >= 2.0)
                            return EV_PlayDialogue("0137F063");
                        else
                            return EV_PlayDialogue("0137F062");
                    } else {
                        return EV_PlayDialogue("0137F061");
                    }
                default:
                    return EV_PlayDialogue("0137E049");
                }
            } else {
                return EV_PlayDialogue("0113A035");
            }
        } else {
            switch (opponent.name) {
            case areas::DART_FCJ.name:
                if (random >= 1.0) {
                    v8 = "0137D060";
                    if (random >= 2.0)
                        v8 = "0137D061";
                    return EV_PlayDialogue(v8);
                } else {
                    return EV_PlayDialogue("0137D059");
                }
            case areas::DART_HLN.name:
                if (random >= 1.0) {
                    if (random >= 2.0)
                        return EV_PlayDialogue("0137C049");
                    else
                        return EV_PlayDialogue("0137C048");
                } else {
                    return EV_PlayDialogue("0137C047");
                }
            case areas::DART_JON.name:
                if (random >= 1.0) {
                    if (random >= 2.0)
                        return EV_PlayDialogue("0137B055");
                    else
                        return EV_PlayDialogue("0137B054");
                } else {
                    return EV_PlayDialogue("0137B053");
                }
            case areas::DART_HCY.name:
                if (random >= 1.0) {
                    if (random >= 2.0)
                        return EV_PlayDialogue("0137F057");
                    else
                        return EV_PlayDialogue("0137F056");
                } else {
                    return EV_PlayDialogue("0137F055");
                }
            default:
                return EV_PlayDialogue("0137E019");
            }
        }
    } else if (random >= 1.0) {
        if (random >= 2.0)
            return EV_PlayDialogue("0113A028");
        else
            return EV_PlayDialogue("0113A027");
    } else {
        return EV_PlayDialogue("0113A026");
    }
}

ConversationBuffer *PlayDartsBonusResultResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v7;
    random = GetRandomNumberInRange(3.0);
    if (playerTurn != 0)
        return EV_PlayDialogue("0113A035");
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        v7 = "0137D068";
        if (random >= 1.5)
            v7 = "0137D069";
        return EV_PlayDialogue(v7);
    case areas::DART_HLN.name:
        if (random >= 1.5)
            return EV_PlayDialogue("0137C057");
        else
            return EV_PlayDialogue("0137C056");
    case areas::DART_JON.name:
        if (random >= 1.5)
            return EV_PlayDialogue("0137B063");
        else
            return EV_PlayDialogue("0137B062");
    case areas::DART_HCY.name:
        if (random >= 1.5)
            return EV_PlayDialogue("0137F065");
        else
            return EV_PlayDialogue("0137F064");
    default:
        if (random >= 1.0)
            return EV_PlayDialogue("0137E057");
        else
            return EV_PlayDialogue("0137E056");
    }
}

ConversationBuffer *PlayDartsGameOverResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v2;
    random = GetRandomNumberInRange(3.0);
    if (random < 1.0)
        return EV_PlayDialogue("0113A035");
    v2 = "0113A036";
    if (random >= 2.0)
        v2 = "0113A037";
    return EV_PlayDialogue(v2);
}

ConversationBuffer *PlayDartsReplayResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v2;
    random = GetRandomNumberInRange(3.0);
    if (random < 1.0)
        return EV_PlayDialogue("0113A038");
    v2 = "0113A039";
    if (random >= 2.0)
        v2 = "0113A040";
    return EV_PlayDialogue(v2);
}

ConversationBuffer *PlayDartsStopResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v2;
    random = GetRandomNumberInRange(3.0);
    if (random < 1.0)
        return EV_PlayDialogue("0113A041");
    v2 = "0113A042";
    if (random >= 2.0)
        v2 = "0113A043";
    return EV_PlayDialogue(v2);
}

ConversationBuffer *PlayDartsOpponentPaymentResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    GetRandomNumberInRange(3.0);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        return EV_PlayDialogue("0137D058");
    case areas::DART_HLN.name:
        return EV_PlayDialogue("0137C046");
    case areas::DART_JON.name:
        return EV_PlayDialogue("0137B049");
    case areas::DART_HCY.name:
        return EV_PlayDialogue("0137F052");
    default:
        break;
    }
    return EV_PlayDialogue("0137E050");
}

ConversationBuffer *PlayDartsOpponentReplayResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    Character *Chara;
    Chara = FindCharacterByScenarioName(opponent);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 30.0, 1.0);
    GetRandomNumberInRange(3.0);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        return EV_PlayDialogue("0137D055");
    case areas::DART_HLN.name:
        return EV_PlayDialogue("0137C044");
    case areas::DART_JON.name:
      EVT_AT_SetVoice("0137B047", 1, 0); // @text EN: "Yo man!\nHow'z another?" | JA: "ヘイ！　ボーイ！\nワンモア・ショウブ、ショウブ！"
      EVT_AT_SetVoice("0137B073", 1, 110); // @text EN: "What's your bet?" | JA: "サァ！　イクラ、カケル？"
        return nullptr;
    case areas::DART_HCY.name:
        return EV_PlayDialogue("0137F049");
    default:
        return EV_PlayDialogue("0137E063");
    }
}

ConversationBuffer *PlayDartsOpponentBonusReplayResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    Character *Chara;
    Chara = FindCharacterByScenarioName(opponent);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 30.0, 1.0);
    GetRandomNumberInRange(3.0);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        return EV_PlayDialogue("0137D074");
    case areas::DART_HLN.name:
        return EV_PlayDialogue("0137C062");
    case areas::DART_JON.name:
        return EV_PlayDialogue("0137B047");
    case areas::DART_HCY.name:
        return EV_PlayDialogue("0137F049");
    default:
        break;
    }
  EVT_AT_SetVoice("0137E064", 1, 0); // @text EN: "All right!\nLet's have another round!" | JA: "ヨ～シッ！\nもうひと勝負いこうぜ。"
  EVT_AT_SetVoice("0137E065", 1, 70); // @text EN: "Again?" | JA: "やるだろ？"
    return nullptr;
}

ConversationBuffer *PlayDartsOpponentFinalResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    Character *Chara;
    Chara = FindCharacterByScenarioName(opponent);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 30.0, 1.0);
    GetRandomNumberInRange(3.0);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        return EV_PlayDialogue("0137D057");
    case areas::DART_HLN.name:
        return EV_PlayDialogue("0137C044");
    case areas::DART_JON.name:
        return EV_PlayDialogue("0137B047");
    case areas::DART_HCY.name:
        return EV_PlayDialogue("0137F051");
    default:
        break;
    }
    return EV_PlayDialogue("0137E049");
}

ConversationBuffer *PlayDartsThrowStartResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    Character *Chara;
    float random;
    const char *v7;
    Chara = FindCharacterByScenarioName(opponent);
    SetHighModelAndFaceFlagsForChara(Chara, 0, 30.0, 1.0);
    random = GetRandomNumberInRange(3.0);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        if (random >= 1.0) {
            v7 = "0137D015";
            if (random >= 2.0)
                v7 = "0137D016";
            return EV_PlayDialogue(v7);
        } else {
            return EV_PlayDialogue("0137D014");
        }
    case areas::DART_HLN.name:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137C013");
            else
                return EV_PlayDialogue("0137C012");
        } else {
            return EV_PlayDialogue("0137C011");
        }
    case areas::DART_JON.name:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137B020");
            else
                return EV_PlayDialogue("0137B019");
        } else {
            return EV_PlayDialogue("0137B018");
        }
    case areas::DART_HCY.name:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137F018");
            else
                return EV_PlayDialogue("0137F017");
        } else {
            return EV_PlayDialogue("0137F016");
        }
    default:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137E018");
            else
                return EV_PlayDialogue("0137E017");
        } else {
            return EV_PlayDialogue("0137E016");
        }
    }
}

ConversationBuffer *PlayDartsThrowResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v6;
    random = GetRandomNumberInRange(3.0);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        if (random >= 1.0) {
            v6 = "0137D030";
            if (random >= 2.0)
                v6 = "0137D031";
            return EV_PlayDialogue(v6);
        } else {
            return EV_PlayDialogue("0137D029");
        }
    case areas::DART_HLN.name:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137C028");
            else
                return EV_PlayDialogue("0137C027");
        } else {
            return EV_PlayDialogue("0137C026");
        }
    case areas::DART_JON.name:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137B034");
            else
                return EV_PlayDialogue("0137B033");
        } else {
            return EV_PlayDialogue("0137B032");
        }
    case areas::DART_HCY.name:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137F033");
            else
                return EV_PlayDialogue("0137F032");
        } else {
            return EV_PlayDialogue("0137F031");
        }
    default:
        if (random >= 1.0) {
            if (random >= 2.0)
                return EV_PlayDialogue("0137E030");
            else
                return EV_PlayDialogue("0137E029");
        } else {
            return EV_PlayDialogue("0137E028");
        }
    }
}

ConversationBuffer *PlayDartsRoundOverResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    float random;
    const char *v8;
    random = GetRandomNumberInRange(3.0);
    if (mode != 0) {
        if (playerTurn != 0) {
            v8 = "0113A035";
            return EV_PlayDialogue(v8);
        }
        switch (opponent.name) {
        case areas::DART_FCJ.name:
            if (random >= 1.0) {
                v8 = "0137D063";
                if (random >= 2.0)
                    v8 = "0137D064";
            } else {
                v8 = "0137D062";
            }
            return EV_PlayDialogue(v8);
        case areas::DART_HLN.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    v8 = "0137C052";
                else
                    v8 = "0137C051";
            } else {
                v8 = "0137C050";
            }
            return EV_PlayDialogue(v8);
        case areas::DART_JON.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    v8 = "0137B058";
                else
                    v8 = "0137B057";
            } else {
                v8 = "0137B056";
            }
            return EV_PlayDialogue(v8);
        case areas::DART_HCY.name:
            if (random >= 1.0) {
                if (random >= 2.0)
                    v8 = "0137F060";
                else
                    v8 = "0137F059";
            } else {
                v8 = "0137F058";
            }
            return EV_PlayDialogue(v8);
        default:
            break;
        }
    }
    return nullptr;
}

ConversationBuffer *PlayDartsFinalResultResponse(std::int32_t mode, bool playerTurn, shen_name opponent) {
    const float random = GetRandomNumberInRange(3.0f);
    if (!mode) {
        if (random < 1.0f)
            return EV_PlayDialogue("0113A029");
        return EV_PlayDialogue(random < 2.0f ? "0113A030" : "0113A031");
    }
    if (playerTurn) {
        if (!dartsPlayerWon)
            return EV_PlayDialogue("0113A035");
        switch (opponent.name) {
        case areas::DART_FCJ.name:
            return EV_PlayDialogue(random < 1.0f ? "0137D065" : random < 2.0f ? "0137D066" : "0137D067");
        case areas::DART_HLN.name:
            return EV_PlayDialogue(random < 1.0f ? "0137C053" : random < 2.0f ? "0137C054" : "0137C055");
        case areas::DART_JON.name:
            return EV_PlayDialogue(random < 1.0f ? "0137B059" : random < 2.0f ? "0137B060" : "0137B061");
        case areas::DART_HCY.name:
            return EV_PlayDialogue(random < 1.0f ? "0137F061" : random < 2.0f ? "0137F062" : "0137F063");
        default:
            return EV_PlayDialogue("0137E049");
        }
    }
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        return EV_PlayDialogue(random < 1.0f ? "0137D059" : random < 2.0f ? "0137D060" : "0137D061");
    case areas::DART_HLN.name:
        return EV_PlayDialogue(random < 1.0f ? "0137C047" : random < 2.0f ? "0137C048" : "0137C049");
    case areas::DART_JON.name:
        return EV_PlayDialogue(random < 1.0f ? "0137B053" : random < 2.0f ? "0137B054" : "0137B055");
    case areas::DART_HCY.name:
        return EV_PlayDialogue(random < 1.0f ? "0137F055" : random < 2.0f ? "0137F056" : "0137F057");
    default:
        return EV_PlayDialogue("0137E019");
    }
}

ConversationBuffer *PlayDartsDeclineOpponentResponse(std::int32_t, bool, shen_name opponent) {
    if (opponent.name == areas::DART_HCY.name) {
        EV_PlayDialogue("0137A012");
        EVT_AT_SetVoice("0137F010", 1, 80); // @text EN: "Really.\nThat's too bad." | JA: "そうか…\nそれは残念だな。"
        EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 80);
    } else
        PlayDartsOpponentExit(opponent);
    return nullptr;
}

ConversationBuffer *PlayDartsWonBetResponse(std::int32_t, bool, shen_name opponent) {
    GetRandomNumberInRange(3.0f);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        EV_PlayDialogue("0137D070");
        EVT_AT_SetVoice("0137D071", 1, 350); // @text EN: "This is your money." | JA: "ホレッ、賭け金じゃ。"
        EVT_AT_SetVoice("0137D054", 1, 400); // @text EN: "How disappointing." | JA: "しかし、くやしいのう。"
        break;
    case areas::DART_HLN.name:
        EV_PlayDialogue("0137C045");
        EVT_AT_SetVoice("0137C060", 1, 240); // @text EN: "Here's your money." | JA: "はい、かけ金…"
        EVT_AT_SetVoice("0137C061", 1, 290); // @text EN: "Darn, I wasn't that lucky." | JA: "あ～あ、ツイてないやぁ…"
        return nullptr;
    case areas::DART_JON.name:
        EVT_AT_SetVoice("0137B046", 1, 0); // @text EN: "Got me man!" | JA: "オー・マイ・ガット！\nアイム・ロスト！"
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 502)) {
            EVT_AT_SetVoice("0137B070", 1, 120); // @text EN: "Really cool!" | JA: "アンビリーバブルネ！\nシンジラレナイヨ～！"
            EVT_AT_SetVoice("0137B071", 1, 270); // @text EN: "All yours!!" | JA: "モッテケ、ドロボー！！"
        } else {
            EVT_AT_SetVoice("0137B067", 1, 120); // @text EN: "Yo tough!\nYo very tough." | JA: "ユー・ベリー・ストロングネ！\nツヨスギルヨ～！"
            EVT_AT_SetVoice("0137B068", 1, 270); // @text EN: "All yours!" | JA: "モッテケ、ドロボー！"
        }
        break;
    case areas::DART_HCY.name:
        EVT_AT_SetVoice("0137F048", 1, 0); // @text EN: "G-good game=@\nBravo!" | JA: "な、なかなかやるじゃないか…\nブラボー！"
        EVT_AT_SetVoice("0137F066", 1, 160); // @text EN: "Ha ha ha=@ha=@" | JA: "ハハッ…ハハハハハ…\nハア～ッ…"
        EVT_AT_SetVoice("0137F067", 1, 360); // @text EN: "Your bet=@" | JA: "賭け金だ…"
        break;
    default:
        EVT_AT_SetVoice("0137E060", 1, 0); // @text EN: "Damn!\nHe got me!" | JA: "ク～ッ！　クッソ～！\nやられちまったぜ～！"
        EVT_AT_SetVoice("0137E061", 1, 160); // @text EN: "You didn't say you played baseball." | JA: "なんだよ、おまえも\n野球やってたのかぁ…"
        EVT_AT_SetVoice("0137E062", 1, 380); // @text EN: "Here's your money." | JA: "ホレッ、かけ金だ。"
        break;
    }
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(opponent), 5, 30.0f, 1.0f);
    return nullptr;
}

ConversationBuffer *PlayDartsLostBetResponse(std::int32_t, bool, shen_name opponent) {
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(opponent), 3, 30.0f, 1.0f);
    GetRandomNumberInRange(3.0f);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        EV_PlayDialogue("0137D072");
        EVT_AT_SetVoice("0137D056", 1, 150); // @text EN: "Excuse me, but I win.\nI'll ask you for my money now." | JA: "悪いが、ワシの勝ちじゃな。\n賭け金はいただきじゃのう。"
        EVT_AT_SetVoice("0137D073", 1, 310); // @text EN: "I am sorry for you." | JA: "ウホ～ッ、こりゃ、すまんのう。"
        break;
    case areas::DART_HLN.name:
        EV_PlayDialogue("0137C043");
        EVT_AT_SetVoice("0137C058", 1, 100); // @text EN: "$5." | JA: "早く５ドルちょうだい。"
        EVT_AT_SetVoice("0137C059", 1, 190); // @text EN: "Thanks, big brother." | JA: "へへっ…謝謝、お兄ちゃん。"
        break;
    case areas::DART_JON.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 502)) {
            EVT_AT_SetVoice("0137B066", 1, 0); // @text EN: "Man!\nYo lose!" | JA: "イエ～！　ユー・ロスト！"
            EVT_AT_SetVoice("0137B044", 1, 60); // @text EN: "Yo man!\nI'm a gifted man!" | JA: "イヤッホ～！\nノウアルタカハ、ツメヲカクスネ。"
        } else {
            EVT_AT_SetVoice("0137B066", 1, 0); // @text EN: "Man!\nYo lose!" | JA: "イエ～！　ユー・ロスト！"
            EVT_AT_SetVoice("0137B064", 1, 60); // @text EN: "Yo!\nThanks for yo money man." | JA: "イヤッホ～！\nカケキン、ゴッツァンデス。"
        }
        EVT_AT_SetVoice("0137B065", 1, 230); // @text EN: "Arigato man." | JA: "ド～モ、アリガトネ。"
        break;
    case areas::DART_HCY.name:
        EVT_AT_SetVoice("0137F068", 1, 0); // @text EN: "Looks like you lost this game." | JA: "どうやらユーの負けだね。"
        EVT_AT_SetVoice("0137F050", 1, 90); // @text EN: "You are paying me\nto observe my beautiful form." | JA: "５０ドルはミーの美しい\nフォームの見学代だと思いな。"
        EVT_AT_SetVoice("0137F069", 1, 240); // @text EN: "Thank you.\nThat's $50." | JA: "これはどうも。確かに。"
        break;
    default:
        EVT_AT_SetVoice("0137E058", 1, 0); // @text EN: "I win!\nYahoo!" | JA: "オレの勝ちだ。\nイヤッホ～！"
        EVT_AT_SetVoice("0137E047", 1, 90); // @text EN: "Sorry, but you can leave your\nthirty dollars here." | JA: "ワリイけど、\n３０ドルおいてってもらおうか。"
        EVT_AT_SetVoice("0137E059", 1, 260); // @text EN: "Luck's involved in any game." | JA: "まあ、勝負は時の運だからよ。"
        break;
    }
    return nullptr;
}

void PrepareDartsOpponentAdmission(shen_name opponent) {
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 478))
            EV_PlayDialogue("0137D008");
        else { EV_PlayDialogue("0137D001"); EVT_AT_SetVoice("0137A011", 1, 50); } // @text EN: "Ah, nothing." | JA: "いえ、すいません。"
        break;
    case areas::DART_HLN.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 477)) EVT_AT_SetVoice("0137C007", 1, 0); // @text EN: "Oh, you're here again?" | JA: "あれ、また来たの？"
        else { EVT_AT_SetVoice("0137C001", 1, 0); EVT_AT_SetVoice("0137A010", 1, 50); } // @text 0137C001 EN: "Do you wanna play?" | JA: "あ、お兄ちゃん、やる？" ; 0137A010 EN: "No, I'm just watching=@" | JA: "いや、見てるだけだから…"
        break;
    case areas::DART_JON.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 476)) EVT_AT_SetVoice("0137B007", 1, 0); // @text EN: "Yo man!\nI knew yo'd be back." | JA: "ヘイ！　ボーイ！　ヨォ、ヨォ、\nマタ、キタノ！"
        else { EVT_AT_SetVoice("0137B001", 1, 0); EVT_AT_SetVoice("0137B002", 1, 120); } // @text 0137B001 EN: "Yo man! What's up?\nPlaying darts?" | JA: "ヘイ！　ボーイ！　カモン！\nシャル・ウィ・ダーツ？" ; 0137B002 EN: "What's the matter man?" | JA: "アー・ユー・オーケー？\nダイジョウブ？"
        break;
    case areas::DART_HCY.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 479)) EVT_AT_SetVoice("0137F008", 1, 0); // @text EN: "Hi. You're here again?" | JA: "やあ。また、来たのかい？"
        else { EVT_AT_SetVoice("0137F001", 1, 0); EVT_AT_SetVoice("0137A011", 1, 50); } // @text 0137F001 EN: "Hm?" | JA: "何か？" ; 0137A011 EN: "Ah, nothing." | JA: "いえ、すいません。"
        break;
    default:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 475)) {
            EVT_AT_SetVoice("0137E010", 1, 0); EVT_AT_SetVoice("0137E011", 1, 150); // @text 0137E010 EN: "What, you're here again?" | JA: "なんだ、また来たのか。" ; 0137E011 EN: " " | JA: "ウォリャーッ！"
        } else { EVT_AT_SetVoice("0137E052", 1, 0); EVT_AT_SetVoice("0137E053", 1, 180); } // @text 0137E052 EN: "Okay, a straight pitch next!" | JA: "よ～し、次は直球勝負だ！" ; 0137E053 EN: "Bull's eye!" | JA: "どまん中～っ！"
        break;
    }
}

std::int32_t PlayDartsOpponentAdmission(shen_name opponent) {
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 478)) {
            EV_PlayDialogue("0137D009");
            return 120;
        }
        EV_PlayDialogue("0137D003");
        EVT_AT_SetVoice("0137D004", 1, 100); EVT_AT_SetVoice("0137D005", 1, 160); EVT_AT_SetVoice("0137D006", 1, 270); // @text 0137D004 EN: "The bet is $10." | JA: "賭け金は１０ドルじゃ。" ; 0137D005 EN: "Five darts.\nThe higher score wins." | JA: "５本投げて\n得点の多い方が勝ち。" ; 0137D006 EN: "If you score 120 or 240,\nyou can throw five more darts." | JA: "１２０点、２４０点を出すと\nさらに５本ずつ投げられるんじゃ。"
        EVT_AT_SetVoice("0137D007", 1, 450); // @text EN: "So, how about it?" | JA: "どうじゃ、\nやってもらえんかのう？"
        return 540;
    case areas::DART_HLN.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 477)) { EVT_AT_SetVoice("0137C008", 1, 0); return 60; } // @text EN: "Do you want to place a five-dollar bet?" | JA: "かけ金５ドルで勝負してく？"
        EVT_AT_SetVoice("0137C002", 1, 0); EVT_AT_SetVoice("0137C003", 1, 100); EVT_AT_SetVoice("0137C004", 1, 170); // @text 0137C002 EN: "I was asking\nif you'd like to make a bet." | JA: "そういう意味じゃないよ。\n勝負しようって言ったの。" ; 0137C003 EN: "You can start with $5." | JA: "かけ金は５ドルに\nしといてあげるよ。" ; 0137C004 EN: "Five throws.\nThe one with the higher score wins." | JA: "５本ずつ投げて\nハイスコアを出した方が勝ち。"
        EVT_AT_SetVoice("0137C005", 1, 260); EVT_AT_SetVoice("0137C006", 1, 390); // @text 0137C005 EN: "With more than 120 or 240 points,\nyou'll get five more." | JA: "１２０点、２４０点をクリアすると\nまた５本ずつ投げられるよ。" ; 0137C006 EN: "So, do you want to play?" | JA: "どうする？　やるの？"
        return 450;
    case areas::DART_JON.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 476)) {
            EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 502);
            EVT_AT_SetVoice("0137B003", 1, 0); EVT_AT_SetVoice("0137B006", 1, 110); return 230; // @text 0137B003 EN: "The bet's $10!\nHave a game!" | JA: "カケキン１０ドルデ\nショウブ！　ショウブ！" ; 0137B006 EN: "How about it man?" | JA: "サア、ボーイ！\nドウスル、ドウスルノ！"
        }
        EVT_AT_SetVoice("0137B003", 1, 0); EVT_AT_SetVoice("0137B004", 1, 110); // @text 0137B003 EN: "The bet's $10!\nHave a game!" | JA: "カケキン１０ドルデ\nショウブ！　ショウブ！" ; 0137B004 EN: "Throw five.\nLet's see who wins!" | JA: "５回ナゲテ、カチマケキメル！"
        EVT_AT_SetVoice("0137B005", 1, 210); EVT_AT_SetVoice("0137B006", 1, 340); // @text 0137B005 EN: "Get 120 or 240.\nYo get another game!" | JA: "１２０！　２４０！\nダセバ、サラニナゲラレル。" ; 0137B006 EN: "How about it man?" | JA: "サア、ボーイ！\nドウスル、ドウスルノ！"
        return 450;
    case areas::DART_HCY.name:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 479)) { EVT_AT_SetVoice("0137F009", 1, 0); return 160; } // @text EN: "How about a lesson for a bet of $50?" | JA: "どう？　賭け金５０ドルで\nレッスン受けてくかい？"
        EVT_AT_SetVoice("0137F002", 1, 0); EVT_AT_SetVoice("0137F003", 1, 130); EVT_AT_SetVoice("0137F004", 1, 270); // @text 0137F002 EN: "Missed by a tenth of an inch=@" | JA: "う～ん。２ミリばかり、ブレたか…" ; 0137F003 EN: "I'm an honorary member of\nthe Hong Kong Darts Association." | JA: "ミーは香港ダーツ協会の\n名誉会員でね。" ; 0137F004 EN: "I can be your coach for a bet of $50." | JA: "賭け金５０ドルで、\n特別にコーチしてあげるよ。"
        EVT_AT_SetVoice("0137F005", 1, 400); EVT_AT_SetVoice("0137F006", 1, 530); EVT_AT_SetVoice("0137F007", 1, 700); // @text 0137F005 EN: "Each of us throws five darts and\nthe one with the higher score wins." | JA: "５本ずつ投げて\n得点の多い方が勝ち。" ; 0137F006 EN: "With a score of 120 or 240,\nwe'll have five more darts to throw." | JA: "１２０点、２４０点を出すと\nさらに５本ずつ投げられる。" ; 0137F007 EN: "How about it? You won't have\nmany chances to play a game with me." | JA: "どうする？　ミーと勝負できる\nなんてメッタにないことだよ。"
        return 890;
    default:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 475)) { EVT_AT_SetVoice("0137E012", 1, 0); return 70; } // @text EN: "How about a thirty-dollar game?" | JA: "かけ金３０ドルで勝負してくかい？"
        EVT_AT_SetVoice("0137E054", 1, 0); EVT_AT_SetVoice("0137A017", 1, 60); EVT_AT_SetVoice("0137E055", 1, 90); // @text 0137E054 EN: "Yo!\nDo you like baseball?" | JA: "よう！　おまえ、野球好きか？" ; 0137A017 EN: "I guess=@" | JA: "まあ…" ; 0137E055 EN: "Good!" | JA: "だったら、話は早え。"
        EVT_AT_SetVoice("0137E007", 1, 130); EVT_AT_SetVoice("0137E008", 1, 190); EVT_AT_SetVoice("0137E009", 1, 320); // @text 0137E007 EN: "Let's have a thirty-dollar game." | JA: "かけ金３０ドルで勝負しようぜ。" ; 0137E008 EN: "The rules are easy. Five throws. The one\nwho hits the center the most, wins." | JA: "ルールは簡単。５回投げて、\n多くまん中に当てた方が勝ちだ。" ; 0137E009 EN: "Let's have a game, quick." | JA: "つっ立ってねえで、\n早えトコやろうぜ。"
        return 390;
    }
}

std::int32_t PlayDartsBetResponse(shen_name opponent, std::int32_t difficulty, std::int32_t bet) {
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        EV_PlayDialogue("0137A014"); EVT_AT_SetVoice("0137D013", 1, 30); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Smile, 30, 1.0f, 50); return 120; // @text EN: "I'm glad." | JA: "おう、これはうれしいのう。"
    case areas::DART_HLN.name:
        EV_PlayDialogue("0137A003"); EVT_AT_SetVoice("0137C010", 1, 30); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Smile, 30, 1.0f, 40); return 120; // @text EN: "Good!\nThat's the spirit!" | JA: "やったあ。そうこなくちゃね。"
    case areas::DART_JON.name:
        if (!difficulty) { EVT_AT_SetVoice("0137A003", 1, 0); EVT_AT_SetVoice("0137B016", 1, 50); } // @text 0137A003 EN: "All right." | JA: "わかった。" ; 0137B016 EN: "Yeah!\nLet's darts man!" | JA: "ＯＫ！　レッツ・プレイ・ナウ。"
        else {
            switch (bet) {
            case 10: EVT_AT_SetVoice("0137A004", 1, 0); EVT_AT_SetVoice("0137B016", 1, 50); break; // @text 0137A004 EN: "$10." | JA: "１０ドルだ。" ; 0137B016 EN: "Yeah!\nLet's darts man!" | JA: "ＯＫ！　レッツ・プレイ・ナウ。"
            case 20: EVT_AT_SetVoice("0137A005", 1, 0); EVT_AT_SetVoice("0137B016", 1, 50); break; // @text 0137A005 EN: "$20." | JA: "２０ドルだ。" ; 0137B016 EN: "Yeah!\nLet's darts man!" | JA: "ＯＫ！　レッツ・プレイ・ナウ。"
            case 30: EVT_AT_SetVoice("0137A006", 1, 0); EVT_AT_SetVoice("0137B016", 1, 50); break; // @text 0137A006 EN: "$30." | JA: "３０ドルだ。" ; 0137B016 EN: "Yeah!\nLet's darts man!" | JA: "ＯＫ！　レッツ・プレイ・ナウ。"
            case 100: EVT_AT_SetVoice("0137A007", 1, 0); EVT_AT_SetVoice("0137B017", 1, 50); break; // @text 0137A007 EN: "$100." | JA: "１００ドルだ。" ; 0137B017 EN: "Man!\nIt's show time!" | JA: "ＯＫ！　イッツ・ショータイム。"
            case 200: EVT_AT_SetVoice("0137A008", 1, 0); EVT_AT_SetVoice("0137B017", 1, 50); break; // @text 0137A008 EN: "$200." | JA: "２００ドルだ。" ; 0137B017 EN: "Man!\nIt's show time!" | JA: "ＯＫ！　イッツ・ショータイム。"
            case 300: EVT_AT_SetVoice("0137A009", 1, 0); EVT_AT_SetVoice("0137B017", 1, 50); break; // @text 0137A009 EN: "$300." | JA: "３００ドルだ。" ; 0137B017 EN: "Man!\nIt's show time!" | JA: "ＯＫ！　イッツ・ショータイム。"
            default:
                break;
            }
        }
        EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Smile, 30, 1.0f, 50);
        return 130;
    case areas::DART_HCY.name:
        EVT_AT_SetVoice("0137A016", 1, 0); EVT_AT_SetVoice("0137F015", 1, 100); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Smile, 30, 1.0f, 100); return 160; // @text 0137A016 EN: "All right.\nI will accept your offer." | JA: "わかりました。\nその勝負、受けます。" ; 0137F015 EN: "Good." | JA: "よろしい。"
    default:
        EVT_AT_SetVoice("0137A003", 1, 0); EVT_AT_SetVoice("0137E015", 1, 30); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Smile, 30, 1.0f, 40); return 130; // @text 0137A003 EN: "All right." | JA: "わかった。" ; 0137E015 EN: "That's the spirit. Now, play ball!" | JA: "そうこなくちゃな。\nじゃあ、プレイボールだ！"
    }
}

std::int32_t PlayDartsOpponentResult(shen_name opponent, std::int32_t bet) {
    const float random = GetRandomNumberInRange(3.0f);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        EV_PlayDialogue("0137A013");
        if (bet == 5) { EVT_AT_SetVoice("0137D012", 1, 60); return -140; } // @text EN: "$3.\nNo, how about $1?" | JA: "３ドル。いや、１ドルでどうじゃ。"
        if (bet == 10) { EVT_AT_SetVoice("0137D011", 1, 60); return -120; } // @text EN: "Well then, what about $5?" | JA: "だったら、５ドルでどうじゃ。"
        EVT_AT_SetVoice("0137D010", 1, 60); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 80); return 170; // @text EN: "Oh.\nThat is too bad." | JA: "そうか、それは残念じゃのう。"
    case areas::DART_HLN.name:
        EV_PlayDialogue("0137A002"); EVT_AT_SetVoice("0137C009", 1, 50); return 120; // @text EN: "Oh, that's too bad." | JA: "そっか。しかたないね…"
    case areas::DART_JON.name:
        EVT_AT_SetVoice("0137A002", 1, 0); // @text EN: "I don't have much money." | JA: "今は持ち合わせがない。"
        EVT_AT_SetVoice(random < 1.0f ? "0137B010" : random < 1.5f ? "0137B011" : "0137B012", 1, 50); // @text 0137B010 EN: "Oh man!" | JA: "オー・ノー！" ; 0137B011 EN: "Why, man?" | JA: "オー・ホワイ？" ; 0137B012 EN: "No way!" | JA: "リアリー？"
        EVT_AT_SetVoice(random < 1.0f ? "0137B013" : random < 1.5f ? "0137B014" : "0137B015", 1, 100); // @text 0137B013 EN: "Get outta here man!" | JA: "グッバイ・ボーイ。" ; 0137B014 EN: "See you man!" | JA: "ソウ・バイバイ。" ; 0137B015 EN: "Sayonara!" | JA: "サヨナーラ。"
        EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 50);
        return 160;
    case areas::DART_HCY.name:
        EVT_AT_SetVoice("0137A012", 1, 0); EVT_AT_SetVoice("0137F010", 1, 80); return 200; // @text 0137A012 EN: "Thank you, but I'll decline." | JA: "せっかくですが、やめておきます。" ; 0137F010 EN: "Really.\nThat's too bad." | JA: "そうか…\nそれは残念だな。"
    default:
        EVT_AT_SetVoice("0137A002", 1, 0); EVT_AT_SetVoice("0137E014", 1, 50); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 50); return 140; // @text 0137A002 EN: "I don't have much money." | JA: "今は持ち合わせがない。" ; 0137E014 EN: "Don't tell me you're broke." | JA: "なんだよ、しけたヤローだな。"
    }
}

std::int32_t PlayDartsOpponentExit(shen_name opponent) {
    const float random = GetRandomNumberInRange(3.0f);
    switch (opponent.name) {
    case areas::DART_FCJ.name:
        EV_PlayDialogue("0137A012"); EVT_AT_SetVoice("0137D010", 1, 80); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 80); return 190; // @text EN: "Oh.\nThat is too bad." | JA: "そうか、それは残念じゃのう。"
    case areas::DART_HLN.name:
        EV_PlayDialogue("0137A001"); EVT_AT_SetVoice("0137C009", 1, 60); return 120; // @text EN: "Oh, that's too bad." | JA: "そっか。しかたないね…"
    case areas::DART_JON.name:
        EVT_AT_SetVoice("0137A001", 1, 0); // @text EN: "Sorry, not this time." | JA: "悪いけど、やめとくよ。"
        EVT_AT_SetVoice(random < 1.0f ? "0137B010" : random < 1.5f ? "0137B011" : "0137B012", 1, 50); // @text 0137B010 EN: "Oh man!" | JA: "オー・ノー！" ; 0137B011 EN: "Why, man?" | JA: "オー・ホワイ？" ; 0137B012 EN: "No way!" | JA: "リアリー？"
        EVT_AT_SetVoice(random < 1.0f ? "0137B013" : random < 1.5f ? "0137B014" : "0137B015", 1, 100); // @text 0137B013 EN: "Get outta here man!" | JA: "グッバイ・ボーイ。" ; 0137B014 EN: "See you man!" | JA: "ソウ・バイバイ。" ; 0137B015 EN: "Sayonara!" | JA: "サヨナーラ。"
        EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 50);
        return 160;
    case areas::DART_HCY.name:
        EVT_AT_SetVoice("0137A012", 1, 0); EVT_AT_SetVoice("0137F010", 1, 80); // @text 0137A012 EN: "Thank you, but I'll decline." | JA: "せっかくですが、やめておきます。" ; 0137F010 EN: "Really.\nThat's too bad." | JA: "そうか…\nそれは残念だな。"
        EVT_AT_SetVoice(random < 1.0f ? "0137F011" : random < 1.5f ? "0137F012" : random < 2.5f ? "0137F013" : "0137F014", 1, 210); // @text 0137F011 EN: "Well, I've got to attend a meeting for\nthe Darts Association anyway." | JA: "まあ、ミーももうすぐダーツ協会の\n会議に行かなきゃいけないしな…" ; 0137F012 EN: "I think I had a meeting at\nthe Darts Association today=@" | JA: "さあて、今日はダーツ協会の\n会議があったっけなあ…" ; 0137F013 EN: "It's not very often that you'll have\nthe privilege of playing darts with me=@" | JA: "こういうチャンスは\nメッタにないんだけどなあ…" ; 0137F014 EN: "You're turning down my offer\nto teach you=@?" | JA: "このミーが教えてあげようって\n言ってるのに…"
        return 400;
    default:
        EVT_AT_SetVoice("0137A001", 1, 0); EVT_AT_SetVoice("0137E013", 1, 60); EVT_AT_SetFaceLooks(opponent, FacialExpressionId::Sad, 30, 1.0f, 80); return 150; // @text 0137A001 EN: "Sorry, not this time." | JA: "悪いけど、やめとくよ。" ; 0137E013 EN: "Man, you're no fun." | JA: "なんだよ、つまんねえヤローだな。"
    }
}

const DartsResponseCallback dartsResponseCallbacks[30]{PlayDartsOpeningResponse, PlayDartsReadyResponse, PlayDartsExitResponse, PlayDartsContinueResponse,
    PlayDartsLastThrowResponse, PlayDartsZeroScoreResponse, PlayDartsLowScoreResponse, PlayDartsLowerMiddleScoreResponse, PlayDartsMiddleScoreResponse,
    PlayDartsUpperMiddleScoreResponse, PlayDartsUpperMiddleScoreResponse, PlayDartsBullseyeResponse, PlayDartsHighScoreResponse, PlayDartsEndThrowsResponse,
    PlayDartsRoundResultResponse, PlayDartsBonusResultResponse, PlayDartsFinalResultResponse, PlayDartsGameOverResponse, PlayDartsReplayResponse,
    PlayDartsStopResponse, PlayDartsDeclineOpponentResponse, PlayDartsWonBetResponse, PlayDartsOpponentPaymentResponse, PlayDartsLostBetResponse,
    PlayDartsOpponentReplayResponse, PlayDartsOpponentBonusReplayResponse, PlayDartsOpponentFinalResponse, PlayDartsThrowStartResponse, PlayDartsThrowResponse,
    PlayDartsRoundOverResponse};

ConversationBuffer *ExecuteDartsResponse(std::int32_t index, std::int32_t mode, bool playerTurn, shen_name opponent) {
    return dartsResponseCallbacks[index](mode, playerTurn, opponent);
}

} // namespace shenmue::s2
