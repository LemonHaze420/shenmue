#include "s2/areas/area_sysp.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"

namespace shenmue::s2::areas {
void SyspMain(std::uintptr_t argument) {
    const std::uint32_t mode = static_cast<std::uint32_t>(argument);
    std::int32_t selection, choice;
    const Vector3 *npcTarget;
    const char *voice = syspEmptyVoice;
    SetPlayerControlEnabled(0);
    syspState = 0;
    syspTargetNpcPosition = 0;
    syspFlags = 0;
    EV_debug_log("SYSP\\eSYSP_et_sysp.c", 1982, "MODE = %d\n", mode);
    switch (mode) {
    case 0:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0179A009;
            break;
        case 1:
            voice = syspVoice0179A010;
            break;
        case 2:
            voice = syspVoice0179A011;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xF1C8u, {}, 0, nullptr);
        return;
    case 1:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A001, &syspCameraTargetMode1, 0, {}, 1000u, nullptr);
        return;
    case 2:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0179A004, nullptr, 16384u, {}, 0, nullptr);
        return;
    case 3:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A005, &syspCameraTargetMode3, 0, {}, 1000u, nullptr);
        return;
    case 4:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0179A025, nullptr, 0, {}, 0, "WMEM200G");
        return;
    case 5:
        syspFlags |= 1u;
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0179A026_0179A027, nullptr, 0, {}, 0, "WMEM200G");
        return;
    case 6:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0179A006;
            break;
        case 1:
            voice = syspVoice0179A007;
            break;
        case 2:
            voice = syspVoice0179A025;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, syspDefaultYaw, {}, 0, nullptr);
        return;
    case 7:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0179A006;
            break;
        case 1:
            voice = syspVoice0179A007;
            break;
        case 2:
            voice = syspVoice0179A025;
            break;
        default:
            break;
        }
        goto talkFacingEast;
    case 8:
        syspFlags |= 2u;
        SyspStartEvent(SyspEventType::Other, 0, syspVoice0179A024, nullptr, 0, {}, 0x3E8u, nullptr);
        InitiateCleanupForSceneChara(SYSP_TNZ_);
        return;
    case 9:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0179A020;
            break;
        case 1:
            voice = syspVoice0179A021;
            break;
        case 2:
            voice = syspVoice0179A022;
            break;
        default:
            break;
        }
        goto talkFacingWest;
    case 10:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0179B001_0179B004;
            break;
        case 1:
            voice = syspVoice0179B002_0179B005;
            break;
        case 2:
            voice = syspVoice0179B003_0179B006;
            break;
        default:
            break;
        }
        goto talkSonFacingWest;
    case 11:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0179C001_0179C004;
            break;
        case 1:
            voice = syspVoice0179C002_0179C005;
            break;
        case 2:
            voice = syspVoice0179C003_0179C006;
            break;
        default:
            break;
        }
        goto talkQrnFacingSouth;
    case 12:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180A014;
            break;
        case 1:
            voice = syspVoice0180A015;
            break;
        case 2:
            voice = syspVoice0180A016;
            break;
        default:
            break;
        }
        goto talkFacingWest;
    case 13:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180C001_0180C004;
            break;
        case 1:
            voice = syspVoice0180C002_0180C005;
            break;
        case 2:
            voice = syspVoice0180C003_0180C006;
            break;
        default:
            break;
        }
    talkSonFacingWest:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xC000u, SYSP_SON_, 0, nullptr);
        return;
    case 14:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180D001_0180D004;
            break;
        case 1:
            voice = syspVoice0180D002_0180D005;
            break;
        case 2:
            voice = syspVoice0180D003_0180D006;
            break;
        default:
            break;
        }
    talkQrnFacingSouth:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x8000u, SYSP_QRN_, 0, nullptr);
        return;
    case 15:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A002, &syspCameraTargetMode15, 0, {}, 0x3E8u, nullptr);
        return;
    case 16:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A002, &syspCameraTargetMode16, 0, {}, 0x3E8u, nullptr);
        return;
    case 17:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A002, &syspCameraTargetMode17, 0, {}, 0x3E8u, nullptr);
        return;
    case 18:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A008, &syspCameraTargetMode18, 0, {}, 0, nullptr);
        return;
    case 19:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A008, &syspCameraTargetMode19, 0, {}, 0x14u, nullptr);
        return;
    case 20:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A008, &syspCameraTargetMode20, 0, {}, 0x3E8u, nullptr);
        return;
    case 21:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A003, &syspCameraTargetMode21, 0, {}, 0x3E8u, nullptr);
        return;
    case 22:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A003, &syspCameraTargetMode21, 0, {}, 0x3E8u, nullptr);
        return;
    case 23:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A012, &syspCameraTargetMode23, 0, {}, 0x3E8u, nullptr);
        return;
    case 24:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A012, &syspCameraTargetMode24, 0, {}, 0x3E8u, nullptr);
        return;
    case 25:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A012, &syspCameraTargetMode25, 0, {}, 0, nullptr);
        return;
    case 26:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A013, &syspCameraTargetMode26, 0, {}, 0xAu, nullptr);
        return;
    case 27:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A013, &syspCameraTargetMode27, 0, {}, 0xAu, nullptr);
        return;
    case 28:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A014, &syspCameraTargetMode28, 0, {}, 0xAu, nullptr);
        return;
    case 29:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A014, &syspCameraTargetMode29, 0, {}, 0xAu, nullptr);
        return;
    case 30:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A015, &syspCameraTargetMode30, 0, {}, 0, nullptr);
        return;
    case 31:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A015, &syspCameraTargetMode31, 0, {}, 0, nullptr);
        return;
    case 32:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A015, &syspCameraTargetMode32, 0, {}, 0x3FCu, nullptr);
        return;
    case 33:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A016, &syspCameraTargetMode33, 0, {}, 0xAu, nullptr);
        return;
    case 34:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A018, &syspCameraTargetMode34, 0, {}, 0xAu, nullptr);
        return;
    case 35:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A019, &syspCameraTargetMode35, 0, {}, 0xAu, nullptr);
        return;
    case 36:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A019, &syspCameraTargetMode35, 0, {}, 0x3F2u, nullptr);
        return;
    case 37:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A019, &syspCameraTargetMode35, 0, {}, 0x3F2u, nullptr);
        return;
    case 38:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0179A072, &syspCameraTargetMode38, 0, {}, 0x1Eu, nullptr);
        return;
    case 39:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0179A073, nullptr, 0xBC72u, {}, 0, nullptr);
        return;
    case 40:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180A022;
            break;
        case 1:
            voice = syspVoice0180A023;
            break;
        case 2:
            voice = syspVoice0180A024;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x5C71u, {}, 0, nullptr);
        return;
    case 41:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180A022;
            break;
        case 1:
            voice = syspVoice0180A023;
            break;
        case 2:
            voice = syspVoice0180A024;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x71C7u, {}, 0, nullptr);
        return;
    case 42:
        selection = shenGetRandomInt32(3);
        if (!selection) {
            voice = syspVoice0180A022;
            goto talkFacingSouth;
        }
        choice = selection - 1;
        if (choice) {
            if (choice != 1)
                goto talkFacingSouth;
            SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180A024, nullptr, 0x8000u, {}, 0, nullptr);
        } else {
            SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180A023, nullptr, 0x8000u, {}, 0, nullptr);
        }
        return;
    case 43:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180A022;
            break;
        case 1:
            voice = syspVoice0180A023;
            break;
        case 2:
            voice = syspVoice0180A024;
            break;
        default:
            break;
        }
        goto talkFacingWest;
    case 44:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180A022;
            break;
        case 1:
            voice = syspVoice0180A023;
            break;
        case 2:
            voice = syspVoice0180A024;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xA38Fu, {}, 0, nullptr);
        return;
    case 45:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A001_0180A002, &syspCameraTargetMode45, 0, {}, 0x3E8u, nullptr);
        return;
    case 46:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A009, &syspCameraTargetMode46, 0, {}, 0x3E8u, nullptr);
        return;
    case 47:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A010, &syspCameraTargetMode47, 0, {}, 0x3E8u, nullptr);
        return;
    case 48:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A011, &syspCameraTargetMode48, 0, {}, 0x3E8u, nullptr);
        return;
    case 49:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A012, &syspCameraTargetMode49, 0, {}, 0x3E8u, nullptr);
        return;
    case 50:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A025, &syspCameraTargetMode50, 0, {}, 0x3E8u, nullptr);
        return;
    case 51:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A026, &syspCameraTargetMode51, 0, {}, 0x3E8u, nullptr);
        return;
    case 52:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A027, &syspCameraTargetMode52, 0, {}, 0x3E8u, nullptr);
        return;
    case 53:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A028, &syspCameraTargetMode53, 0, {}, 0x3E8u, nullptr);
        return;
    case 54:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A029, &syspCameraTargetMode54, 0, {}, 0x3E8u, nullptr);
        return;
    case 55:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A030, &syspCameraTargetMode55, 0, {}, 0x3E8u, nullptr);
        return;
    case 60:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180B001_0180B003;
            break;
        case 1:
            voice = syspVoice0180B002_0180B003;
            break;
        case 2:
            voice = syspVoice0180B001_0180B004;
            break;
        default:
            break;
        }
        goto talkAntFacingNorth;
    case 61:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180B001_0180B003;
            break;
        case 1:
            voice = syspVoice0180B002_0180B003;
            break;
        case 2:
            voice = syspVoice0180B001_0180B004;
            break;
        default:
            break;
        }
        goto talkAntFacingNorthWest;
    case 62:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180B001_0180B003;
            break;
        case 1:
            voice = syspVoice0180B002_0180B003;
            break;
        case 2:
            voice = syspVoice0180B001_0180B004;
            break;
        default:
            break;
        }
    talkAntFacingNorth:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xFC72u, SYSP_ANT_, 0, nullptr);
        return;
    case 63:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180B001_0180B003;
            break;
        case 1:
            voice = syspVoice0180B002_0180B003;
            break;
        case 2:
            voice = syspVoice0180B001_0180B004;
            break;
        default:
            break;
        }
    talkAntFacingNorthWest:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xDC72u, SYSP_ANT_, 0, nullptr);
        return;
    case 64:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180E001_0180E004_0180E005;
            break;
        case 1:
            voice = syspVoice0180E002_0180E006_0180E007;
            break;
        case 2:
            voice = syspVoice0180E003_0180E008_0180E009;
            break;
        default:
            break;
        }
        goto talkSncFacingWest;
    case 65:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180E001_0180E004_0180E005;
            break;
        case 1:
            voice = syspVoice0180E002_0180E006_0180E007;
            break;
        case 2:
            voice = syspVoice0180E003_0180E008_0180E009;
            break;
        default:
            break;
        }
    talkSncFacingWest:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xC000u, SYSP_SNC_, 0, nullptr);
        return;
    case 66:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A019_0180A021, &syspCameraTargetMode66, 0, SYSP_SNC_, 0, nullptr);
        syspNextEvent = 70;
        return;
    case 67:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A019_0180A021, &syspCameraTargetMode66, 0, SYSP_SNC_, 0, nullptr);
        syspNextEvent = 71;
        return;
    case 68:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A019_0180A021, &syspCameraTargetMode66, 0, SYSP_SNC_, 0, nullptr);
        syspNextEvent = 72;
        return;
    case 69:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0180A019_0180A021, &syspCameraTargetMode66, 0, SYSP_SNC_, 0, nullptr);
        syspNextEvent = 73;
        return;
    case 70:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180E010_0180E013_0180E015, nullptr, 0xBF4Au, SYSP_SNC_, 0, nullptr);
        return;
    case 71:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180E010_0180E013_0180E015, nullptr, 0x38Eu, SYSP_SNC_, 0, nullptr);
        return;
    case 72:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180E010_0180E013_0180E015, nullptr, 0xBF4Au, SYSP_SNC_, 0, nullptr);
        return;
    case 73:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180E010_0180E013_0180E015, nullptr, 0x38Eu, SYSP_SNC_, 0, nullptr);
        return;
    case 74:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180E016_0180A074_0180E014_0180E015;
            break;
        case 1:
            voice = syspVoice0180E016_0180A076_0180E017_0180E018;
            break;
        case 2:
            voice = syspVoice0180E003_0180E013_0180E009;
            break;
        default:
            break;
        }
        goto talkSncAlternateYaw;
    case 75:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0180E016_0180A074_0180E014_0180E015;
            break;
        case 1:
            voice = syspVoice0180E016_0180A076_0180E017_0180E018;
            break;
        case 2:
            voice = syspVoice0180E003_0180E013_0180E009;
            break;
        default:
            break;
        }
    talkSncAlternateYaw:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xC38Fu, SYSP_SNC_, 0, nullptr);
        return;
    case 76:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0180A006_0180A008;
            break;
        case 1:
            voice = syspVoice0180A006_0180A007;
            break;
        default:
            break;
        }
        goto talkFacingEast;
    case 77:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0180A006, nullptr, 0x4000u, {}, 0, nullptr);
        return;
    case 78:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A007_0327B001, &syspCameraTargetMode78, 0, SYSP_REN_, 0, nullptr);
        return;
    case 79:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode79, 0, {}, 0xAu, nullptr);
        return;
    case 80:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode80, 0, {}, 0x3E8u, nullptr);
        return;
    case 81:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode81, 0, {}, 0x3E8u, nullptr);
        return;
    case 82:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode82, 0, {}, 0x3E8u, nullptr);
        return;
    case 83:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode83, 0, {}, 0x3E8u, nullptr);
        return;
    case 84:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode84, 0, {}, 0x3E8u, nullptr);
        return;
    case 85:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode85, 0, {}, 0x3E8u, nullptr);
        return;
    case 86:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode86, 0, {}, 0x3E8u, nullptr);
        return;
    case 87:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode87, 0, {}, 0x3E8u, nullptr);
        return;
    case 88:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode88, 0, {}, 0x3E8u, nullptr);
        return;
    case 89:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A001_0327A018, &syspCameraTargetMode89, 0, {}, 0x3E8u, nullptr);
        return;
    case 90:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A019, &syspCameraTargetMode90, 0, {}, 0, nullptr);
        return;
    case 91:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode91, 0, {}, 0x32u, nullptr);
        return;
    case 92:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode92, 0, {}, 0x32u, nullptr);
        return;
    case 93:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode93, 0, {}, 0x32u, nullptr);
        return;
    case 94:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode94, 0, {}, 0x32u, nullptr);
        return;
    case 95:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode95, 0, {}, 0x32u, nullptr);
        return;
    case 96:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode96, 0, {}, 0x32u, nullptr);
        return;
    case 97:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A002, &syspCameraTargetMode97, 0, {}, 0x32u, nullptr);
        return;
    case 98:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A017, &syspCameraTargetMode98, 0, {}, 0x32u, nullptr);
        return;
    case 99:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A017, &syspCameraTargetMode99, 0, {}, 0x32u, nullptr);
        return;
    case 100:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A017, &syspCameraTargetMode100, 0, {}, 0x32u, nullptr);
        return;
    case 101:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0327F001_0327F002, nullptr, 0x4000u, SYSP_BF1_, 0, nullptr);
        return;
    case 102:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0327G001_0327G002, nullptr, 0x4E38u, SYSP_BF6_, 0, nullptr);
        return;
    case 103:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0327H001_0327H002, nullptr, 0xC000u, SYSP_BFA_, 0, nullptr);
        return;
    case 104:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode104, 0, {}, 0x3E8u, nullptr);
        return;
    case 105:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode105, 0, {}, 0x3E8u, nullptr);
        return;
    case 106:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode106, 0, {}, 0x3E8u, nullptr);
        return;
    case 107:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode107, 0, {}, 0x3E8u, nullptr);
        return;
    case 108:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode108, 0, {}, 0x3E8u, nullptr);
        return;
    case 109:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode109, 0, {}, 0x3E8u, nullptr);
        return;
    case 110:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode110, 0, {}, 0x3E8u, nullptr);
        return;
    case 111:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode111, 0, {}, 0x3E8u, nullptr);
        return;
    case 112:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode112, 0, {}, 0x3E8u, nullptr);
        return;
    case 113:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode113, 0, {}, 0x3E8u, nullptr);
        return;
    case 114:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode114, 0, {}, 0x3E8u, nullptr);
        return;
    case 115:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode115, 0, {}, 0x3E8u, nullptr);
        return;
    case 116:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode116, 0, {}, 0x3E8u, nullptr);
        return;
    case 117:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode117, 0, {}, 0x3E8u, nullptr);
        return;
    case 118:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode118, 0, {}, 0x3E8u, nullptr);
        return;
    case 119:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode119, 0, {}, 0x3E8u, nullptr);
        return;
    case 120:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode120, 0, {}, 0x3E8u, nullptr);
        return;
    case 121:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode121, 0, {}, 0x3E8u, nullptr);
        return;
    case 122:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode122, 0, {}, 0x3E8u, nullptr);
        return;
    case 123:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode123, 0, {}, 0x3E8u, nullptr);
        return;
    case 124:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode124, 0, {}, 0x3E8u, nullptr);
        return;
    case 125:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A004, &syspCameraTargetMode125, 0, {}, 0x3E8u, nullptr);
        return;
    case 126:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode126, 0, {}, 0x3E8u, nullptr);
        return;
    case 127:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode127, 0, {}, 0x14u, nullptr);
        return;
    case 128:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode128, 0, {}, 0x14u, nullptr);
        return;
    case 129:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode129, 0, {}, 0x14u, nullptr);
        return;
    case 130:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode130, 0, {}, 0x14u, nullptr);
        return;
    case 131:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode131, 0, {}, 0x14u, nullptr);
        return;
    case 132:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode132, 0, {}, 0x14u, nullptr);
        return;
    case 133:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode133, 0, {}, 0x14u, nullptr);
        return;
    case 134:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode134, 0, {}, 0x14u, nullptr);
        return;
    case 135:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode135, 0, {}, 0x14u, nullptr);
        return;
    case 136:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode136, 0, {}, 0x14u, nullptr);
        return;
    case 137:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode137, 0, {}, 0x14u, nullptr);
        return;
    case 138:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode138, 0, {}, 0x14u, nullptr);
        return;
    case 139:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A021, &syspCameraTargetMode139, 0, {}, 0x14u, nullptr);
        return;
    case 140:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A064, &syspCameraTargetMode140, 0, {}, 0x3E8u, nullptr);
        return;
    case 141:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327E001_0327A065;
            break;
        case 1:
            voice = syspVoice0327E001_0327A066;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x1555u, SYSP_OK9_, 0, nullptr);
        return;
    case 142:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327D001_0327D002_0327A065;
            break;
        case 1:
            voice = syspVoice0327D001_0327D002_0327A066;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x58E3u, SYSP_KUK_, 0, nullptr);
        return;
    case 143:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode143, 0, {}, 0xAu, nullptr);
        return;
    case 144:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode144, 0, {}, 0xAu, nullptr);
        return;
    case 145:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode145, 0, {}, 0xAu, nullptr);
        return;
    case 146:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode146, 0, {}, 0xAu, nullptr);
        return;
    case 147:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode147, 0, {}, 0xAu, nullptr);
        return;
    case 148:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode148, 0, {}, 0xAu, nullptr);
        return;
    case 149:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode149, 0, {}, 0xAu, nullptr);
        return;
    case 150:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode150, 0, {}, 0xAu, nullptr);
        return;
    case 151:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode151, 0, {}, 0xAu, nullptr);
        return;
    case 152:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode152, 0, {}, 0xAu, nullptr);
        return;
    case 153:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A006, &syspCameraTargetMode153, 0, {}, 0xAu, nullptr);
        return;
    case 154:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode143, 0, {}, 0xAu, nullptr);
        return;
    case 155:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode144, 0, {}, 0xAu, nullptr);
        return;
    case 156:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode145, 0, {}, 0xAu, nullptr);
        return;
    case 157:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode146, 0, {}, 0xAu, nullptr);
        return;
    case 158:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode147, 0, {}, 0xAu, nullptr);
        return;
    case 159:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode148, 0, {}, 0xAu, nullptr);
        return;
    case 160:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode149, 0, {}, 0xAu, nullptr);
        return;
    case 161:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode150, 0, {}, 0xAu, nullptr);
        return;
    case 162:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode151, 0, {}, 0xAu, nullptr);
        return;
    case 163:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode152, 0, {}, 0xAu, nullptr);
        return;
    case 164:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A012_0327A013, &syspCameraTargetMode153, 0, {}, 0xAu, nullptr);
        return;
    case 165:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A014, &syspCameraTargetMode165, 0, {}, 0x3E8u, nullptr);
        return;
    case 166:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A005, &syspCameraTargetMode166, 0, {}, 0x3E8u, nullptr);
        return;
    case 167:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A005, &syspCameraTargetMode167, 0, {}, 0x3E8u, nullptr);
        return;
    case 168:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A005, &syspCameraTargetMode168, 0, {}, 0x3E8u, nullptr);
        return;
    case 169:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A005, &syspCameraTargetMode169, 0, {}, 0x3E8u, nullptr);
        return;
    case 170:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A005, &syspCameraTargetMode170, 0, {}, 0x3E8u, nullptr);
        return;
    case 171:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A010_0327A011, &syspCameraTargetMode171, 0, {}, 0x3F2u, nullptr);
        return;
    case 172:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x3E8u, "QEME203I");
        return;
    case 173:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 174:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 175:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 176:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 177:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 178:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 179:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 180:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 181:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 182:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 183:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 184:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 185:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A008, nullptr, 0, {}, 0x14u, "QEME203I");
        return;
    case 186:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A074, &syspCameraTargetMode186, 0, {}, 0x50u, nullptr);
        return;
    case 187:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A023, &syspCameraTargetMode187, 0, {}, 0x3E8u, nullptr);
        return;
    case 188:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A023, &syspCameraTargetMode188, 0, {}, 0x3E8u, nullptr);
        return;
    case 189:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A023, &syspCameraTargetMode189, 0, {}, 0x3E8u, nullptr);
        return;
    case 190:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A027, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 191:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A027, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 192:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A027, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 193:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A027, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 194:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A027, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 195:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A027, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 196:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A015_0327A016, nullptr, 0, {}, 0, "HKSM201I");
        return;
    case 197:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A029, &syspCameraTargetMode197, 0, {}, 0x1Eu, nullptr);
        return;
    case 198:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A061;
            break;
        case 1:
            voice = syspVoice0327A062;
            break;
        case 2:
            voice = syspVoice0327A063;
            break;
        default:
            break;
        }
    talkFacingEast:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x4000u, {}, 0, nullptr);
        return;
    case 199:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327B017_0327B020;
            break;
        case 1:
            voice = syspVoice0327B018_0327B021;
            break;
        case 2:
            voice = syspVoice0327B019_0327B022;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode199;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0xC000u, SYSP_REN_, 0, nullptr);
        return;
    case 201:
        syspTargetNpcPosition = &syspNpcTargetMode201;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A048_0327B004, &syspCameraTargetMode201, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 202:
        syspTargetNpcPosition = &syspNpcTargetMode202;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A049_0327B005, &syspCameraTargetMode202, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 203:
        syspTargetNpcPosition = &syspNpcTargetMode203;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A049_0327B005, &syspCameraTargetMode203, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 204:
        syspTargetNpcPosition = &syspNpcTargetMode204;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A050_0327B006, &syspCameraTargetMode204, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 205:
        syspTargetNpcPosition = &syspNpcTargetMode205;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A051_0327B007, &syspCameraTargetMode205, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 206:
        syspTargetNpcPosition = &syspNpcTargetMode206;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A052_0327B008, &syspCameraTargetMode206, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 207:
        syspTargetNpcPosition = &syspNpcTargetMode207;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A053_0327B009, &syspCameraTargetMode207, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 208:
        syspTargetNpcPosition = &syspNpcTargetMode208;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A054_0327B010, &syspCameraTargetMode208, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 209:
        syspTargetNpcPosition = &syspNpcTargetMode209;
        SyspStartEvent(SyspEventType::WarpIfOutOfRange, 0, syspVoice0327A055_0327B011, &syspCameraTargetMode209, 0, SYSP_REN_, 0x1Eu, nullptr);
        return;
    case 210:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327B023_0327B025;
            break;
        case 1:
            voice = syspVoice0327B024_0327B027;
            break;
        case 2:
            voice = syspVoice0327B023_0327B026;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x71C7u, SYSP_REN_, 0, nullptr);
        return;
    case 211:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327B028_0327B031;
            break;
        case 1:
            voice = syspVoice0327B029_0327B032;
            break;
        case 2:
            voice = syspVoice0327B030_0327B033;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode211;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0x3C71u, SYSP_REN_, 0, nullptr);
        return;
    case 212:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327B034_0327B037;
            break;
        case 1:
            voice = syspVoice0327B035_0327B038;
            break;
        case 2:
            voice = syspVoice0327B036_0327B039;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode212;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0xC38Fu, SYSP_REN_, 0, nullptr);
        return;
    case 213:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327B040_0327B043;
            break;
        case 1:
            voice = syspVoice0327B041_0327B044;
            break;
        case 2:
            voice = syspVoice0327B042_0327B045;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode213;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0x838Fu, SYSP_REN_, 0, nullptr);
        return;
    case 214:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327B012;
            break;
        case 1:
            voice = syspVoice0327B015_0327B013;
            break;
        case 2:
            voice = syspVoice0327B014;
            break;
        default:
            break;
        }
        npcTarget = &syspNpcTargetMode214;
        goto talkRenTargetPosition;
    case 215:
        SyspStartEvent(SyspEventType::Simple, 0, syspVoice0327A057_0327B016, &syspCameraTargetMode215, 0, {}, 0x3F2u, nullptr);
        return;
    case 216:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327A030;
            break;
        case 1:
            voice = syspVoice0327A031;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::RyoOnly, 0, voice, &syspCameraTargetMode216, 0xEAABu, {}, 0, nullptr);
        return;
    case 217:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0327A036_0327A037, nullptr, 0x40B6u, {}, 0, nullptr);
        return;
    case 218:
        selection = shenGetRandomInt32(3);
        if (selection) {
            choice = selection - 1;
            if (!choice) {
                voice = syspVoice0327B014;
                npcTarget = &syspNpcTargetMode218;
                goto talkRenTargetPosition;
            }
            if (choice == 1) {
                voice = syspVoice0327B012;
                npcTarget = &syspNpcTargetMode218;
                goto talkRenTargetPosition;
            }
        } else {
            voice = syspVoice0327B015_0327B013;
        }
        npcTarget = &syspNpcTargetMode218;
    talkRenTargetPosition:
        syspTargetNpcPosition = npcTarget;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0xB8E4u, SYSP_REN_, 0, nullptr);
        return;
    case 219:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327A038_0327A040;
            break;
        case 1:
            voice = syspVoice0327A039_0327A040;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xBF4Au, {}, 0, nullptr);
        return;
    case 220:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327B002_0327A041;
            break;
        case 1:
            voice = syspVoice0327B003_0327A042;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode220;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0xC0B7u, SYSP_REN_, 0, nullptr);
        return;
    case 221:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327A032_0327A034;
            break;
        case 1:
            voice = syspVoice0327A033_0327A035;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xC0B7u, {}, 0, nullptr);
        return;
    case 222:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0327A045_0327A046;
            break;
        case 1:
            voice = syspVoice0327A045_0327A047;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x3F49u, {}, 0, nullptr);
        return;
    case 223:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A068;
            break;
        case 1:
            voice = syspVoice0327A069;
            break;
        case 2:
            voice = syspVoice0327A070;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 224:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A069;
            break;
        case 1:
            voice = syspVoice0327A070;
            break;
        case 2:
            voice = syspVoice0327A071;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 225:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A070;
            break;
        case 1:
            voice = syspVoice0327A071;
            break;
        case 2:
            voice = syspVoice0327A072;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 226:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A071;
            break;
        case 1:
            voice = syspVoice0327A072;
            break;
        case 2:
            voice = syspVoice0327A073;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 227:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A072;
            break;
        case 1:
            voice = syspVoice0327A073;
            break;
        case 2:
            voice = syspVoice0327A068;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 228:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A073;
            break;
        case 1:
            voice = syspVoice0327A068;
            break;
        case 2:
            voice = syspVoice0327A069;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 229:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A068;
            break;
        case 1:
            voice = syspVoice0327A069;
            break;
        case 2:
            voice = syspVoice0327A070;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 230:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A069;
            break;
        case 1:
            voice = syspVoice0327A070;
            break;
        case 2:
            voice = syspVoice0327A071;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 231:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A070;
            break;
        case 1:
            voice = syspVoice0327A071;
            break;
        case 2:
            voice = syspVoice0327A072;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 232:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A071;
            break;
        case 1:
            voice = syspVoice0327A072;
            break;
        case 2:
            voice = syspVoice0327A073;
            break;
        default:
            break;
        }
        goto dialogueWithoutCamera;
    case 233:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A072;
            break;
        case 1:
            voice = syspVoice0327A073;
            break;
        case 2:
            voice = syspVoice0327A068;
            break;
        default:
            break;
        }
    dialogueWithoutCamera:
        SyspStartEvent(SyspEventType::Other, 0, voice, nullptr, 0, {}, 0, nullptr);
        return;
    case 234:
        SyspStartEvent(SyspEventType::Other, 0, syspVoice0327A045_0327A069, nullptr, 0, {}, 0, nullptr);
        return;
    case 235:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0327A060;
            break;
        case 1:
            voice = syspVoice0327A058;
            break;
        case 2:
            voice = syspVoice0327A059;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x1555u, {}, 0, nullptr);
        return;
    case 236:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0327C001_0327A056, nullptr, 0xB8E4u, SYSP_JOP_, 0, nullptr);
        return;
    case 237:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A003;
            break;
        case 1:
            voice = syspVoice0546A004;
            break;
        case 2:
            voice = syspVoice0546A007;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xAAAu, {}, 0, nullptr);
        return;
    case 238:
        selection = shenGetRandomInt32(3);
        if (!selection) {
            voice = syspVoice0546A023;
            goto talkFacingSouth;
        }
        choice = selection - 1;
        if (choice) {
            if (choice == 1) {
                SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0546A027, nullptr, 0x8000u, {}, 0, nullptr);
                return;
            }
        talkFacingSouth:
            SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x8000u, {}, 0, nullptr);
            return;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0546A024, nullptr, 0x8000u, {}, 0, nullptr);
        return;
    case 239:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A023;
            break;
        case 1:
            voice = syspVoice0546A024;
            break;
        case 2:
            voice = syspVoice0546A027;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x9C72u, {}, 0, nullptr);
        return;
    case 240:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A023;
            break;
        case 1:
            voice = syspVoice0546A024;
            break;
        case 2:
            voice = syspVoice0546A027;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xF556u, {}, 0, nullptr);
        return;
    case 241:
        SyspStartEvent(SyspEventType::WithNpc, 0, syspVoice0546B001_0546B009_0546A013, nullptr, 0xF1C8u, SYSP_SIN_, 0x424u, nullptr);
        return;
    case 242:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546B001_0546B009_0546A013;
            break;
        case 1:
            voice = syspVoice0546B001_0546B008_0546A011;
            break;
        case 2:
            voice = syspVoice0546A007;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode242;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0x471Cu, SYSP_SIN_, 0, nullptr);
        return;
    case 243:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546B006_0546A013;
            break;
        case 1:
            voice = syspVoice0546B005_0546B016_0546A015;
            break;
        case 2:
            voice = syspVoice0546A006;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xAAAu, SYSP_SIN_, 0x3E8u, nullptr);
        return;
    case 244:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A007;
            break;
        case 1:
            voice = syspVoice0546A004;
            break;
        case 2:
            voice = syspVoice0546A010;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xC000u, SYSP_SIN_, 0x3E8u, nullptr);
        return;
    case 245:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A007;
            break;
        case 1:
            voice = syspVoice0546B006_0546A015;
            break;
        case 2:
            voice = syspVoice0546A003;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x18E3u, SYSP_SIN_, 0, nullptr);
        return;
    case 246:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546B006_0546A012;
            break;
        case 1:
            voice = syspVoice0546A009;
            break;
        case 2:
            voice = syspVoice0546B001_0546B017;
            break;
        default:
            break;
        }
        syspTargetNpcPosition = &syspNpcTargetMode246;
        SyspStartEvent(SyspEventType::StickyRange, 0, voice, nullptr, 0x5C71u, SYSP_SIN_, 0, nullptr);
        return;
    case 247:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0546B001_0546B011_0546B010;
            break;
        case 1:
            voice = syspVoice0546A008;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::RyoOnly, 0, voice, &syspCameraTargetMode247, 0xE000u, {}, 0x3E8u, nullptr);
        return;
    case 248:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A010;
            break;
        case 1:
            voice = syspVoice0546A021;
            break;
        case 2:
            voice = syspVoice0546A022;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x71Cu, {}, 0, nullptr);
        return;
    case 249:
        switch (shenGetRandomInt32(2)) {
        case 0:
            voice = syspVoice0546A021;
            break;
        case 1:
            voice = syspVoice0546A022;
            break;
        default:
            break;
        }
    talkFacingWest:
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xC000u, {}, 0, nullptr);
        return;
    case 250:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A016;
            break;
        case 1:
            voice = syspVoice0546A017;
            break;
        case 2:
            voice = syspVoice0546A018;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::RyoOnly, 0, voice, &syspCameraTargetMode250, 0xAAAu, {}, 0x1Eu, nullptr);
        return;
    case 251:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A027;
            break;
        case 1:
            voice = syspVoice0546A019;
            break;
        case 2:
            voice = syspVoice0546A024;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0x6AAAu, {}, 0, nullptr);
        return;
    case 252:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A023;
            break;
        case 1:
            voice = syspVoice0546A024;
            break;
        case 2:
            voice = syspVoice0546A026;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, syspAlternateYaw, {}, 0, nullptr);
        return;
    case 253:
        switch (shenGetRandomInt32(3)) {
        case 0:
            voice = syspVoice0546A025;
            break;
        case 1:
            voice = syspVoice0546A003;
            break;
        case 2:
            voice = syspVoice0546A010;
            break;
        default:
            break;
        }
        SyspStartEvent(SyspEventType::WithNpc, 0, voice, nullptr, 0xD556u, {}, 0, nullptr);
        return;
    case 1001:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0180A003, nullptr, 0, {}, 0, "BSKM2HOI");
        syspState = 1;
        return;
    case 1002:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0180A003, nullptr, 0, {}, 0, "TOSM29JI");
        syspState = 1;
        return;
    case 1004:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0327A028, nullptr, 0, {}, 0, "HKSM201I");
        syspState = 1;
        return;
    case 1005:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0180A013, nullptr, 0, {}, 0, "TYOM28JI");
        syspState = 1;
        return;
    case 1006:
        SyspStartEvent(SyspEventType::WithItem, 0, syspVoice0180A003, nullptr, 0, {}, 0, "BKOI200I");
        syspState = 1;
        return;
    default:
        SetPlayerControlEnabled(1);
        return;
    }
}

const ScriptAreaDefinition syspScriptDefinition{.areaId = AREA_SYSP, .field04 = 0, .init = SyspInitNullsub, .main = SyspMain};
} // namespace shenmue::s2::areas
