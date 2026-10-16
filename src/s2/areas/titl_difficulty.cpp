#include "s2/areas/area_titl.hpp"
#include "s2/minigame_random.hpp"
namespace shenmue::s2::areas {
void AdvanceTitleDifficulty(TitleWork *work) {
    unsigned int successCount;
    unsigned int difficulty;
    int initialDifficulty;
    int selection;
    int extraTiming;

    successCount = work->successes;
    if (successCount > 0x352) {
        work->stepScore += 500;
        difficulty = 145;
        work->successes = 711;
        goto SelectPattern;
    }
    if (successCount > 0x348) {
        difficulty = 145;
        goto SelectPattern;
    }
    if (successCount > 0x33E) {
        difficulty = 140;
        goto SelectPattern;
    }
    if (successCount > 0x334) {
        difficulty = 135;
        goto SelectPattern;
    }
    if (successCount > 0x32A) {
        difficulty = 130;
        goto SelectPattern;
    }
    if (successCount > 0x320) {
        difficulty = 125;
        goto SelectPattern;
    }
    if (successCount > 0x316) {
        difficulty = 115;
    EnableSimultaneous:
        work->advancedPatternDivisor = 2.0;
        work->simultaneousEnabled = 1;
        goto SelectPattern;
    }
    if (successCount > 0x302) {
        difficulty = 130;
        work->triplePatterns = 6.0;
        goto SelectPattern;
    }
    if (successCount > 0x2EE) {
        difficulty = 125;
        work->triplePatterns = 4.0;
        goto SelectPattern;
    }
    if (successCount > 0x2DA) {
        difficulty = 120;
        work->triplePatterns = 3.0;
        goto SelectPattern;
    }
    if (successCount > 0x2C6) {
        work->triplePatterns = 2.0;
        difficulty = 115;
        work->advancedPatternDivisor = 3.0;
        work->simultaneousEnabled = 0;
        work->fakeEnabled = 0;
        if (work->stepScore < 0x7D0)
            work->stepScore = 2000;
        goto SelectPattern;
    }
    if (successCount > 0x2BC) {
        difficulty = 130;
        goto SelectPattern;
    }
    if (successCount > 0x2B2) {
        difficulty = 125;
        goto SelectPattern;
    }
    if (successCount > 0x2A8) {
        difficulty = 120;
        goto SelectPattern;
    }
    if (successCount > 0x29E) {
        difficulty = 115;
        goto SelectPattern;
    }
    if (successCount > 0x294) {
        difficulty = 110;
        goto SelectPattern;
    }
    if (successCount > 0x28A) {
        difficulty = 100;
        goto EnableSimultaneous;
    }
    if (successCount > 0x276) {
        difficulty = 130;
        work->triplePatterns = 6.0;
        goto SelectPattern;
    }
    if (successCount > 0x262) {
        difficulty = 120;
        work->triplePatterns = 4.0;
        goto SelectPattern;
    }
    if (successCount > 0x24E) {
        difficulty = 110;
        work->triplePatterns = 3.0;
        goto SelectPattern;
    }
    if (successCount > 0x23A) {
        work->triplePatterns = 2.0;
        difficulty = 100;
        work->advancedPatternDivisor = 3.0;
        work->simultaneousEnabled = 0;
        work->stepScore = 2500;
        goto SelectPattern;
    }
    if (successCount > 0x230) {
        difficulty = 120;
        goto SelectPattern;
    }
    if (successCount > 0x226) {
        difficulty = 115;
        goto SelectPattern;
    }
    if (successCount > 0x21C) {
        difficulty = 105;
        goto SelectPattern;
    }
    if (successCount > 0x212) {
        difficulty = 95;
        goto SelectPattern;
    }
    if (successCount > 0x208) {
        difficulty = 85;
        goto SelectPattern;
    }
    if (successCount <= 0x1FE) {
        if (successCount > 0x1EA) {
            difficulty = 115;
            work->triplePatterns = 6.0;
            goto SelectPattern;
        }
        if (successCount > 0x1D6) {
            difficulty = 105;
            work->triplePatterns = 4.0;
            goto SelectPattern;
        }
        if (successCount > 0x1C2) {
            difficulty = 95;
        EnableFake:
            work->triplePatterns = 3.0;
            work->fakeEnabled = 1;
            goto SelectPattern;
        }
        if (successCount > 0x1AE) {
            work->triplePatterns = 2.0;
            difficulty = 75;
            work->advancedPatternDivisor = 3.0;
            work->simultaneousEnabled = 0;
            work->fakeEnabled = 0;
            work->stepScore = 2000;
            goto SelectPattern;
        }
        if (successCount > 0x1A4) {
            difficulty = 110;
            goto SelectPattern;
        }
        if (successCount > 0x19A) {
            difficulty = 105;
            goto SelectPattern;
        }
        if (successCount > 0x190) {
            difficulty = 100;
            goto SelectPattern;
        }
        if (successCount > 0x186) {
            difficulty = 95;
            goto SelectPattern;
        }
        if (successCount > 0x17C) {
            difficulty = 85;
            goto SelectPattern;
        }
        if (successCount <= 0x172) {
            if (successCount > 0x15E) {
                difficulty = 110;
                work->triplePatterns = 6.0;
                goto SelectPattern;
            }
            if (successCount > 0x14A) {
                difficulty = 90;
                work->triplePatterns = 4.0;
                goto SelectPattern;
            }
            if (successCount > 0x136) {
                difficulty = 70;
                goto EnableFake;
            }
            if (successCount > 0x122) {
                difficulty = 50;
            EnableTriple:
                work->triplePatterns = 2.0;
                work->tripleEnabled = 1;
                goto SelectPattern;
            }
            if (successCount > 0x10E) {
                difficulty = 100;
                work->doublePatterns = 6.0;
                goto SelectPattern;
            }
            if (successCount > 0xFA) {
                difficulty = 75;
                work->doublePatterns = 4.0;
                goto SelectPattern;
            }
            if (successCount > 0xE6) {
                work->doublePatterns = 2.0;
                difficulty = 50;
                work->advancedPatternDivisor = 3.0;
                work->tripleEnabled = 0;
                work->simultaneousEnabled = 0;
                work->stepScore = 1500;
                goto SelectPattern;
            }
            if (successCount > 0xDC) {
                difficulty = 100;
                goto SelectPattern;
            }
            if (successCount > 0xD2) {
                difficulty = 95;
                goto SelectPattern;
            }
            if (successCount > 0xC8) {
                difficulty = 90;
                goto SelectPattern;
            }
            if (successCount > 0xBE) {
                difficulty = 85;
                goto SelectPattern;
            }
            if (successCount > 0xB4) {
                difficulty = 80;
                goto SelectPattern;
            }
            if (successCount <= 0xAA) {
                if (successCount > 0x96) {
                    difficulty = 100;
                    work->triplePatterns = 6.0;
                    goto SelectPattern;
                }
                if (successCount > 0x82) {
                    difficulty = 75;
                    work->triplePatterns = 4.0;
                    goto SelectPattern;
                }
                if (successCount > 0x6E) {
                    difficulty = 50;
                    work->triplePatterns = 3.0;
                    goto SelectPattern;
                }
                if (successCount > 0x5A) {
                    difficulty = 25;
                    goto EnableTriple;
                }
                if (successCount <= 0x46) {
                    if (successCount <= 0x32) {
                        if (successCount <= 0x1E) {
                            if (successCount <= 0x14) {
                                initialDifficulty = 0;
                                if (successCount > 0xA)
                                    initialDifficulty = 50;
                                difficulty = static_cast<unsigned int>(initialDifficulty);
                            } else {
                                difficulty = 100;
                            }
                        } else {
                            work->doublePatterns = 2.0;
                            difficulty = 50;
                            work->advancedPatternDivisor = 3.0;
                        }
                    } else {
                        difficulty = 75;
                        work->doublePatterns = 4.0;
                    }
                } else {
                    difficulty = 100;
                    work->doublePatterns = 6.0;
                }
            SelectPattern:
                if (static_cast<std::uint16_t>(static_cast<std::int64_t>(IncrementMinigameRandFloat() * work->advancedPatternDivisor)) != 0 ||
                    work->successes <= 0x1E) {
                    work->pattern[0] = 0;
                } else {
                    selection = static_cast<std::uint16_t>(static_cast<std::int64_t>(static_cast<double>(IncrementMinigameRandFloat()) * 6.0));
                    if (selection == 0) {
                        if (work->simultaneousEnabled == 1 && work->extraTiming == 0) {
                            work->pattern[0] = 1;
                            goto ClampTiming;
                        }
                        goto DoublePattern;
                    }
                    if (selection != 1 && selection != 2) {
                        if ((selection == 3 || selection == 4) && work->fakeEnabled == 1 && work->extraTiming == 0) {
                            work->pattern[0] = 4;
                            goto ClampTiming;
                        }
                        goto DoublePattern;
                    }
                    if (work->fakeEnabled != 1 || work->extraTiming != 0) {
                    DoublePattern:
                        work->pattern[0] = 2;
                        goto ClampTiming;
                    }
                    work->pattern[0] = 3;
                    if (difficulty >= 0x73) {
                        difficulty = 115;
                    CalculateTiming:
                        work->timingFrames = 31 - difficulty / 5;
                        goto ApplyExtraTiming;
                    }
                }
            ClampTiming:
                if (difficulty < 0x91)
                    goto CalculateTiming;
                work->timingFrames = 1;
            ApplyExtraTiming:
                extraTiming = work->extraTiming;
                work->timingFrames += extraTiming;
                if (extraTiming != 0) {
                    extraTiming = static_cast<std::uint16_t>(extraTiming - 1);
                    work->extraTiming = static_cast<std::uint16_t>(extraTiming);
                }
                return;
            }
        }
    }
    difficulty = 75;
    goto EnableSimultaneous;
}
} // namespace shenmue::s2::areas
