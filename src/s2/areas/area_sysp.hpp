#pragma once

#include "s2/area_event.hpp"
#include "s2/conversation.hpp"
#include "common/math.hpp"
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name SYSP_TASK{SHEN_NAME(SYSP)};
inline constexpr shen_name SYSP_CANCEL_BUTTON{SHEN_NAME(BTCC)};
inline constexpr std::uint8_t SYSP_HELP_MONEY_NEEDED = 41;
inline constexpr shen_name SYSP_SON_{SHEN_NAME(SON_)};
inline constexpr shen_name SYSP_QRN_{SHEN_NAME(QRN_)};
inline constexpr shen_name SYSP_ANT_{SHEN_NAME(ANT_)};
inline constexpr shen_name SYSP_SNC_{SHEN_NAME(SNC_)};
inline constexpr shen_name SYSP_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name SYSP_BF1_{SHEN_NAME(BF1_)};
inline constexpr shen_name SYSP_BF6_{SHEN_NAME(BF6_)};
inline constexpr shen_name SYSP_BFA_{SHEN_NAME(BFA_)};
inline constexpr shen_name SYSP_OK9_{SHEN_NAME(OK9_)};
inline constexpr shen_name SYSP_KUK_{SHEN_NAME(KUK_)};
inline constexpr shen_name SYSP_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name SYSP_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name SYSP_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name SYSP_TNZ_{SHEN_NAME(TNZ_)};

enum class SyspEventType : std::int32_t { Simple = 1, WithNpc = 2, SimpleAlt = 3, WithItem = 4, RyoOnly = 5, Other = 6, WarpIfOutOfRange = 7, StickyRange = 8 };
enum SyspEventState : std::int32_t {
    SyspFinished = -1,
    SyspInitialize = 0,
    SyspBlendIn = 1,
    SyspRyoSetup = 2,
    SyspDialogueStart = 3,
    SyspDialogueActive = 4,
    SyspWaitOrBranch = 5,
    SyspCheckBattle = 6,
    SyspPostRotate = 7,
    SyspTrackRyoMove = 8,
    SyspFinalRotate = 9,
    SyspDialogueCameraSwap = 40
};
enum SyspItemAlphaState : std::int32_t { SyspItemInactive, SyspItemWaitEvent, SyspItemFadeIn, SyspItemFadeOut };
inline constexpr std::uint32_t SYSP_RELATIVE_XYZ_RECURSIVE = 0x78000000U;
inline constexpr std::int32_t SYSP_RIGHT_UPPER_LEG = 5;
inline constexpr std::int32_t SYSP_ITEM_BEGIN_MOTION = 0xf121;
inline constexpr std::int32_t SYSP_ITEM_LOOP_MOTION = 0xf120;
inline constexpr std::int32_t SYSP_ITEM_END_MOTION = 0xf11f;
inline constexpr char syspEmptyVoice[] = "";

inline constexpr char syspVoice0179A011[] = "0179A011\0";
inline constexpr char syspVoice0179A010[] = "0179A010\0";
inline constexpr char syspVoice0179A009[] = "0179A009\0";
inline constexpr char syspVoice0179A001[] = "0179A001\0";
inline constexpr char syspVoice0179A004[] = "0179A004\0";
inline constexpr char syspVoice0179A005[] = "0179A005\0";
inline constexpr char syspVoice0179A025[] = "0179A025\0";
inline constexpr char syspVoice0179A026_0179A027[] = "0179A026\0"
                                                     "0179A027\0";
inline constexpr char syspVoice0179A007[] = "0179A007\0";
inline constexpr char syspVoice0179A006[] = "0179A006\0";
inline constexpr char syspVoice0179A024[] = "0179A024\0";
inline constexpr char syspVoice0179A022[] = "0179A022\0";
inline constexpr char syspVoice0179A021[] = "0179A021\0";
inline constexpr char syspVoice0179A020[] = "0179A020\0";
inline constexpr char syspVoice0179B003_0179B006[] = "0179B003\0"
                                                     "0179B006\0";
inline constexpr char syspVoice0179B002_0179B005[] = "0179B002\0"
                                                     "0179B005\0";
inline constexpr char syspVoice0179B001_0179B004[] = "0179B001\0"
                                                     "0179B004\0";
inline constexpr char syspVoice0179C003_0179C006[] = "0179C003\0"
                                                     "0179C006\0";
inline constexpr char syspVoice0179C002_0179C005[] = "0179C002\0"
                                                     "0179C005\0";
inline constexpr char syspVoice0179C001_0179C004[] = "0179C001\0"
                                                     "0179C004\0";
inline constexpr char syspVoice0180A016[] = "0180A016\0";
inline constexpr char syspVoice0180A015[] = "0180A015\0";
inline constexpr char syspVoice0180A014[] = "0180A014\0";
inline constexpr char syspVoice0180C003_0180C006[] = "0180C003\0"
                                                     "0180C006\0";
inline constexpr char syspVoice0180C002_0180C005[] = "0180C002\0"
                                                     "0180C005\0";
inline constexpr char syspVoice0180C001_0180C004[] = "0180C001\0"
                                                     "0180C004\0";
inline constexpr char syspVoice0180D003_0180D006[] = "0180D003\0"
                                                     "0180D006\0";
inline constexpr char syspVoice0180D002_0180D005[] = "0180D002\0"
                                                     "0180D005\0";
inline constexpr char syspVoice0180D001_0180D004[] = "0180D001\0"
                                                     "0180D004\0";
inline constexpr char syspVoice0179A002[] = "0179A002\0";
inline constexpr char syspVoice0179A008[] = "0179A008\0";
inline constexpr char syspVoice0179A003[] = "0179A003\0";
inline constexpr char syspVoice0179A012[] = "0179A012\0";
inline constexpr char syspVoice0179A013[] = "0179A013\0";
inline constexpr char syspVoice0179A014[] = "0179A014\0";
inline constexpr char syspVoice0179A015[] = "0179A015\0";
inline constexpr char syspVoice0179A016[] = "0179A016\0";
inline constexpr char syspVoice0179A018[] = "0179A018\0";
inline constexpr char syspVoice0179A019[] = "0179A019\0";
inline constexpr char syspVoice0179A072[] = "0179A072\0";
inline constexpr char syspVoice0179A073[] = "0179A073\0";
inline constexpr char syspVoice0180A024[] = "0180A024\0";
inline constexpr char syspVoice0180A023[] = "0180A023\0";
inline constexpr char syspVoice0180A022[] = "0180A022\0";
inline constexpr char syspVoice0180A001_0180A002[] = "0180A001\0"
                                                     "0180A002\0";
inline constexpr char syspVoice0180A009[] = "0180A009\0";
inline constexpr char syspVoice0180A010[] = "0180A010\0";
inline constexpr char syspVoice0180A011[] = "0180A011\0";
inline constexpr char syspVoice0180A012[] = "0180A012\0";
inline constexpr char syspVoice0180A025[] = "0180A025\0";
inline constexpr char syspVoice0180A026[] = "0180A026\0";
inline constexpr char syspVoice0180A027[] = "0180A027\0";
inline constexpr char syspVoice0180A028[] = "0180A028\0";
inline constexpr char syspVoice0180A029[] = "0180A029\0";
inline constexpr char syspVoice0180A030[] = "0180A030\0";
inline constexpr char syspVoice0180B001_0180B004[] = "0180B001\0"
                                                     "0180B004\0";
inline constexpr char syspVoice0180B002_0180B003[] = "0180B002\0"
                                                     "0180B003\0";
inline constexpr char syspVoice0180B001_0180B003[] = "0180B001\0"
                                                     "0180B003\0";
inline constexpr char syspVoice0180E003_0180E008_0180E009[] = "0180E003\0"
                                                              "0180E008\0"
                                                              "0180E009\0";
inline constexpr char syspVoice0180E002_0180E006_0180E007[] = "0180E002\0"
                                                              "0180E006\0"
                                                              "0180E007\0";
inline constexpr char syspVoice0180E001_0180E004_0180E005[] = "0180E001\0"
                                                              "0180E004\0"
                                                              "0180E005\0";
inline constexpr char syspVoice0180A019_0180A021[] = "0180A019\0"
                                                     "0180A021\0";
inline constexpr char syspVoice0180E010_0180E013_0180E015[] = "0180E010\0"
                                                              "0180E013\0"
                                                              "0180E015\0";
inline constexpr char syspVoice0180E003_0180E013_0180E009[] = "0180E003\0"
                                                              "0180E013\0"
                                                              "0180E009\0";
inline constexpr char syspVoice0180E016_0180A076_0180E017_0180E018[] = "0180E016\0"
                                                                       "0180A076\0"
                                                                       "0180E017\0"
                                                                       "0180E018\0";
inline constexpr char syspVoice0180E016_0180A074_0180E014_0180E015[] = "0180E016\0"
                                                                       "0180A074\0"
                                                                       "0180E014\0"
                                                                       "0180E015\0";
inline constexpr char syspVoice0180A006_0180A007[] = "0180A006\0"
                                                     "0180A007\0";
inline constexpr char syspVoice0180A006_0180A008[] = "0180A006\0"
                                                     "0180A008\0";
inline constexpr char syspVoice0180A006[] = "0180A006\0";
inline constexpr char syspVoice0327A007_0327B001[] = "0327A007\0"
                                                     "0327B001\0";
inline constexpr char syspVoice0327A001_0327A018[] = "0327A001\0"
                                                     "0327A018\0";
inline constexpr char syspVoice0327A019[] = "0327A019\0";
inline constexpr char syspVoice0327A002[] = "0327A002\0";
inline constexpr char syspVoice0327A017[] = "0327A017\0";
inline constexpr char syspVoice0327F001_0327F002[] = "0327F001\0"
                                                     "0327F002\0";
inline constexpr char syspVoice0327G001_0327G002[] = "0327G001\0"
                                                     "0327G002\0";
inline constexpr char syspVoice0327H001_0327H002[] = "0327H001\0"
                                                     "0327H002\0";
inline constexpr char syspVoice0327A004[] = "0327A004\0";
inline constexpr char syspVoice0327A021[] = "0327A021\0";
inline constexpr char syspVoice0327A064[] = "0327A064\0";
inline constexpr char syspVoice0327E001_0327A066[] = "0327E001\0"
                                                     "0327A066\0";
inline constexpr char syspVoice0327E001_0327A065[] = "0327E001\0"
                                                     "0327A065\0";
inline constexpr char syspVoice0327D001_0327D002_0327A066[] = "0327D001\0"
                                                              "0327D002\0"
                                                              "0327A066\0";
inline constexpr char syspVoice0327D001_0327D002_0327A065[] = "0327D001\0"
                                                              "0327D002\0"
                                                              "0327A065\0";
inline constexpr char syspVoice0327A006[] = "0327A006\0";
inline constexpr char syspVoice0327A012_0327A013[] = "0327A012\0"
                                                     "0327A013\0";
inline constexpr char syspVoice0327A014[] = "0327A014\0";
inline constexpr char syspVoice0327A005[] = "0327A005\0";
inline constexpr char syspVoice0327A010_0327A011[] = "0327A010\0"
                                                     "0327A011\0";
inline constexpr char syspVoice0327A008[] = "0327A008\0";
inline constexpr char syspVoice0327A074[] = "0327A074\0";
inline constexpr char syspVoice0327A023[] = "0327A023\0";
inline constexpr char syspVoice0327A027[] = "0327A027\0";
inline constexpr char syspVoice0327A015_0327A016[] = "0327A015\0"
                                                     "0327A016\0";
inline constexpr char syspVoice0327A029[] = "0327A029\0";
inline constexpr char syspVoice0327A063[] = "0327A063\0";
inline constexpr char syspVoice0327A062[] = "0327A062\0";
inline constexpr char syspVoice0327A061[] = "0327A061\0";
inline constexpr char syspVoice0327B019_0327B022[] = "0327B019\0"
                                                     "0327B022\0";
inline constexpr char syspVoice0327B018_0327B021[] = "0327B018\0"
                                                     "0327B021\0";
inline constexpr char syspVoice0327B017_0327B020[] = "0327B017\0"
                                                     "0327B020\0";
inline constexpr char syspVoice0327A048_0327B004[] = "0327A048\0"
                                                     "0327B004\0";
inline constexpr char syspVoice0327A049_0327B005[] = "0327A049\0"
                                                     "0327B005\0";
inline constexpr char syspVoice0327A050_0327B006[] = "0327A050\0"
                                                     "0327B006\0";
inline constexpr char syspVoice0327A051_0327B007[] = "0327A051\0"
                                                     "0327B007\0";
inline constexpr char syspVoice0327A052_0327B008[] = "0327A052\0"
                                                     "0327B008\0";
inline constexpr char syspVoice0327A053_0327B009[] = "0327A053\0"
                                                     "0327B009\0";
inline constexpr char syspVoice0327A054_0327B010[] = "0327A054\0"
                                                     "0327B010\0";
inline constexpr char syspVoice0327A055_0327B011[] = "0327A055\0"
                                                     "0327B011\0";
inline constexpr char syspVoice0327B023_0327B026[] = "0327B023\0"
                                                     "0327B026\0";
inline constexpr char syspVoice0327B024_0327B027[] = "0327B024\0"
                                                     "0327B027\0";
inline constexpr char syspVoice0327B023_0327B025[] = "0327B023\0"
                                                     "0327B025\0";
inline constexpr char syspVoice0327B030_0327B033[] = "0327B030\0"
                                                     "0327B033\0";
inline constexpr char syspVoice0327B029_0327B032[] = "0327B029\0"
                                                     "0327B032\0";
inline constexpr char syspVoice0327B028_0327B031[] = "0327B028\0"
                                                     "0327B031\0";
inline constexpr char syspVoice0327B036_0327B039[] = "0327B036\0"
                                                     "0327B039\0";
inline constexpr char syspVoice0327B035_0327B038[] = "0327B035\0"
                                                     "0327B038\0";
inline constexpr char syspVoice0327B034_0327B037[] = "0327B034\0"
                                                     "0327B037\0";
inline constexpr char syspVoice0327B042_0327B045[] = "0327B042\0"
                                                     "0327B045\0";
inline constexpr char syspVoice0327B041_0327B044[] = "0327B041\0"
                                                     "0327B044\0";
inline constexpr char syspVoice0327B040_0327B043[] = "0327B040\0"
                                                     "0327B043\0";
inline constexpr char syspVoice0327B014[] = "0327B014\0";
inline constexpr char syspVoice0327B015_0327B013[] = "0327B015\0"
                                                     "0327B013\0";
inline constexpr char syspVoice0327B012[] = "0327B012\0";
inline constexpr char syspVoice0327A057_0327B016[] = "0327A057\0"
                                                     "0327B016\0";
inline constexpr char syspVoice0327A031[] = "0327A031\0";
inline constexpr char syspVoice0327A030[] = "0327A030\0";
inline constexpr char syspVoice0327A036_0327A037[] = "0327A036\0"
                                                     "0327A037\0";
inline constexpr char syspVoice0327A039_0327A040[] = "0327A039\0"
                                                     "0327A040\0";
inline constexpr char syspVoice0327A038_0327A040[] = "0327A038\0"
                                                     "0327A040\0";
inline constexpr char syspVoice0327B003_0327A042[] = "0327B003\0"
                                                     "0327A042\0";
inline constexpr char syspVoice0327B002_0327A041[] = "0327B002\0"
                                                     "0327A041\0";
inline constexpr char syspVoice0327A033_0327A035[] = "0327A033\0"
                                                     "0327A035\0";
inline constexpr char syspVoice0327A032_0327A034[] = "0327A032\0"
                                                     "0327A034\0";
inline constexpr char syspVoice0327A045_0327A047[] = "0327A045\0"
                                                     "0327A047\0";
inline constexpr char syspVoice0327A045_0327A046[] = "0327A045\0"
                                                     "0327A046\0";
inline constexpr char syspVoice0327A070[] = "0327A070\0";
inline constexpr char syspVoice0327A069[] = "0327A069\0";
inline constexpr char syspVoice0327A068[] = "0327A068\0";
inline constexpr char syspVoice0327A071[] = "0327A071\0";
inline constexpr char syspVoice0327A072[] = "0327A072\0";
inline constexpr char syspVoice0327A073[] = "0327A073\0";
inline constexpr char syspVoice0327A045_0327A069[] = "0327A045\0"
                                                     "0327A069\0";
inline constexpr char syspVoice0327A059[] = "0327A059\0";
inline constexpr char syspVoice0327A058[] = "0327A058\0";
inline constexpr char syspVoice0327A060[] = "0327A060\0";
inline constexpr char syspVoice0327C001_0327A056[] = "0327C001\0"
                                                     "0327A056\0";
inline constexpr char syspVoice0546A007[] = "0546A007\0";
inline constexpr char syspVoice0546A004[] = "0546A004\0";
inline constexpr char syspVoice0546A003[] = "0546A003\0";
inline constexpr char syspVoice0546A023[] = "0546A023\0";
inline constexpr char syspVoice0546A027[] = "0546A027\0";
inline constexpr char syspVoice0546A024[] = "0546A024\0";
inline constexpr char syspVoice0546B001_0546B009_0546A013[] = "0546B001\0"
                                                              "0546B009\0"
                                                              "0546A013\0";
inline constexpr char syspVoice0546B001_0546B008_0546A011[] = "0546B001\0"
                                                              "0546B008\0"
                                                              "0546A011\0";
inline constexpr char syspVoice0546A006[] = "0546A006\0";
inline constexpr char syspVoice0546B005_0546B016_0546A015[] = "0546B005\0"
                                                              "0546B016\0"
                                                              "0546A015\0";
inline constexpr char syspVoice0546B006_0546A013[] = "0546B006\0"
                                                     "0546A013\0";
inline constexpr char syspVoice0546A010[] = "0546A010\0";
inline constexpr char syspVoice0546B006_0546A015[] = "0546B006\0"
                                                     "0546A015\0";
inline constexpr char syspVoice0546B001_0546B017[] = "0546B001\0"
                                                     "0546B017\0";
inline constexpr char syspVoice0546A009[] = "0546A009\0";
inline constexpr char syspVoice0546B006_0546A012[] = "0546B006\0"
                                                     "0546A012\0";
inline constexpr char syspVoice0546A008[] = "0546A008\0";
inline constexpr char syspVoice0546B001_0546B011_0546B010[] = "0546B001\0"
                                                              "0546B011\0"
                                                              "0546B010\0";
inline constexpr char syspVoice0546A022[] = "0546A022\0";
inline constexpr char syspVoice0546A021[] = "0546A021\0";
inline constexpr char syspVoice0546A018[] = "0546A018\0";
inline constexpr char syspVoice0546A017[] = "0546A017\0";
inline constexpr char syspVoice0546A016[] = "0546A016\0";
inline constexpr char syspVoice0546A019[] = "0546A019\0";
inline constexpr char syspVoice0546A026[] = "0546A026\0";
inline constexpr char syspVoice0546A025[] = "0546A025\0";
inline constexpr char syspVoice0180A003[] = "0180A003\0";
inline constexpr char syspVoice0327A028[] = "0327A028\0";
inline constexpr char syspVoice0180A013[] = "0180A013\0";

inline constexpr Vector3 syspCameraTargetMode1{.x = 206.000000f, .y = 108.790001f, .z = 405.899994f};
inline constexpr Vector3 syspCameraTargetMode3{.x = 391.000000f, .y = 105.000000f, .z = 427.000000f};
inline constexpr Vector3 syspCameraTargetMode15{.x = 94.6827011f, .y = 107.180000f, .z = 226.676697f};
inline constexpr Vector3 syspCameraTargetMode16{.x = 149.070007f, .y = 107.599998f, .z = 180.300003f};
inline constexpr Vector3 syspCameraTargetMode17{.x = 154.204498f, .y = 109.217499f, .z = 270.899994f};
inline constexpr Vector3 syspCameraTargetMode18{.x = 158.500000f, .y = 107.500000f, .z = 271.000000f};
inline constexpr Vector3 syspCameraTargetMode19{.x = 207.399994f, .y = 106.699997f, .z = 228.000000f};
inline constexpr Vector3 syspCameraTargetMode20{.x = 266.399994f, .y = 119.000000f, .z = 277.000000f};
inline constexpr Vector3 syspCameraTargetMode21{.x = 280.515411f, .y = 121.404999f, .z = 280.803406f};
inline constexpr Vector3 syspCameraTargetMode23{.x = 304.200012f, .y = 103.699997f, .z = 481.799988f};
inline constexpr Vector3 syspCameraTargetMode24{.x = 305.000000f, .y = 103.699997f, .z = 481.799988f};
inline constexpr Vector3 syspCameraTargetMode25{.x = 306.299988f, .y = 101.000000f, .z = 480.000000f};
inline constexpr Vector3 syspCameraTargetMode26{.x = 257.399994f, .y = 103.000000f, .z = 392.500000f};
inline constexpr Vector3 syspCameraTargetMode27{.x = 267.100006f, .y = 103.900002f, .z = 401.299988f};
inline constexpr Vector3 syspCameraTargetMode28{.x = 52.3199997f, .y = 103.050003f, .z = 231.000000f};
inline constexpr Vector3 syspCameraTargetMode29{.x = 52.3199997f, .y = 103.050003f, .z = 248.399994f};
inline constexpr Vector3 syspCameraTargetMode30{.x = 223.949997f, .y = 105.500000f, .z = 173.250000f};
inline constexpr Vector3 syspCameraTargetMode31{.x = 223.949997f, .y = 105.500000f, .z = 177.699997f};
inline constexpr Vector3 syspCameraTargetMode32{.x = 224.199997f, .y = 107.300003f, .z = 175.500000f};
inline constexpr Vector3 syspCameraTargetMode33{.x = 99.0999985f, .y = 103.000000f, .z = 204.399994f};
inline constexpr Vector3 syspCameraTargetMode34{.x = 228.399994f, .y = 105.900002f, .z = 155.100006f};
inline constexpr Vector3 syspCameraTargetMode35{.x = 97.6999969f, .y = 106.099998f, .z = 294.399994f};
inline constexpr Vector3 syspCameraTargetMode38{.x = 150.300003f, .y = 103.750000f, .z = 109.300003f};
inline constexpr Vector3 syspCameraTargetMode45{.x = 65.4000015f, .y = 104.300003f, .z = 303.000000f};
inline constexpr Vector3 syspCameraTargetMode46{.x = 224.399994f, .y = 115.599998f, .z = 276.200012f};
inline constexpr Vector3 syspCameraTargetMode47{.x = 177.899994f, .y = 108.500000f, .z = 301.299988f};
inline constexpr Vector3 syspCameraTargetMode48{.x = 214.000000f, .y = 110.279999f, .z = 330.670013f};
inline constexpr Vector3 syspCameraTargetMode49{.x = 168.000000f, .y = 109.000000f, .z = 269.700012f};
inline constexpr Vector3 syspCameraTargetMode50{.x = 214.580002f, .y = 106.739998f, .z = 210.449997f};
inline constexpr Vector3 syspCameraTargetMode51{.x = 211.949997f, .y = 106.870003f, .z = 200.000000f};
inline constexpr Vector3 syspCameraTargetMode52{.x = 264.899994f, .y = 106.820000f, .z = 201.500000f};
inline constexpr Vector3 syspCameraTargetMode53{.x = 262.700012f, .y = 116.900002f, .z = 323.899994f};
inline constexpr Vector3 syspCameraTargetMode54{.x = 271.000000f, .y = 101.730003f, .z = 251.899994f};
inline constexpr Vector3 syspCameraTargetMode55{.x = 274.570007f, .y = 108.000000f, .z = 251.899994f};
inline constexpr Vector3 syspCameraTargetMode66{.x = 263.100006f, .y = 102.300003f, .z = 475.500000f};
inline constexpr Vector3 syspCameraTargetMode78{.x = 200.970001f, .y = 102.849998f, .z = 195.619995f};
inline constexpr Vector3 syspCameraTargetMode79{.x = 281.000000f, .y = 104.099998f, .z = 151.300003f};
inline constexpr Vector3 syspCameraTargetMode80{.x = 302.696991f, .y = 101.672501f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode81{.x = 302.696991f, .y = 108.872498f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode82{.x = 302.696991f, .y = 112.472504f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode83{.x = 302.696991f, .y = 116.072502f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode84{.x = 302.696991f, .y = 119.672501f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode85{.x = 302.696991f, .y = 123.272499f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode86{.x = 302.696991f, .y = 126.872498f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode87{.x = 302.696991f, .y = 130.472504f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode88{.x = 302.696991f, .y = 134.072495f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode89{.x = 302.696991f, .y = 137.672501f, .z = 169.009399f};
inline constexpr Vector3 syspCameraTargetMode90{.x = 302.649994f, .y = 101.760002f, .z = 167.600006f};
inline constexpr Vector3 syspCameraTargetMode91{.x = 294.419006f, .y = 103.254204f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode92{.x = 294.419006f, .y = 106.854202f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode93{.x = 294.419006f, .y = 110.454201f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode94{.x = 294.419006f, .y = 117.654198f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode95{.x = 294.419006f, .y = 121.254204f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode96{.x = 294.419006f, .y = 124.854202f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode97{.x = 294.419006f, .y = 128.454193f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode98{.x = 294.419006f, .y = 132.054199f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode99{.x = 294.419006f, .y = 135.654205f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode100{.x = 294.419006f, .y = 139.254196f, .z = 125.243599f};
inline constexpr Vector3 syspCameraTargetMode104{.x = 251.399994f, .y = 101.699997f, .z = 201.500000f};
inline constexpr Vector3 syspCameraTargetMode105{.x = 262.200012f, .y = 101.699997f, .z = 201.500000f};
inline constexpr Vector3 syspCameraTargetMode106{.x = 286.484192f, .y = 101.775002f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode107{.x = 286.484192f, .y = 108.974998f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode108{.x = 286.484192f, .y = 112.574997f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode109{.x = 286.484192f, .y = 116.175003f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode110{.x = 286.484192f, .y = 119.775002f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode111{.x = 286.484192f, .y = 123.375000f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode112{.x = 286.484192f, .y = 126.974998f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode113{.x = 286.484192f, .y = 130.574997f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode114{.x = 286.484192f, .y = 134.175003f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode115{.x = 286.484192f, .y = 137.774994f, .z = 205.069397f};
inline constexpr Vector3 syspCameraTargetMode116{.x = 248.713394f, .y = 101.722504f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode117{.x = 248.713394f, .y = 108.922501f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode118{.x = 248.713394f, .y = 112.522499f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode119{.x = 248.713394f, .y = 116.122498f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode120{.x = 248.713394f, .y = 119.722504f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode121{.x = 248.713394f, .y = 123.322502f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode122{.x = 248.713394f, .y = 126.922501f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode123{.x = 248.713394f, .y = 130.522507f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode124{.x = 248.713394f, .y = 134.122498f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode125{.x = 248.713394f, .y = 137.722504f, .z = 207.716095f};
inline constexpr Vector3 syspCameraTargetMode126{.x = 289.299988f, .y = 103.000000f, .z = 94.5000000f};
inline constexpr Vector3 syspCameraTargetMode127{.x = 266.607208f, .y = 101.757500f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode128{.x = 266.607208f, .y = 108.957497f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode129{.x = 266.607208f, .y = 112.557503f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode130{.x = 266.607208f, .y = 116.157501f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode131{.x = 266.607208f, .y = 119.757500f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode132{.x = 266.607208f, .y = 123.357498f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode133{.x = 266.607208f, .y = 126.957497f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode134{.x = 266.607208f, .y = 130.557495f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode135{.x = 266.607208f, .y = 134.157501f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode136{.x = 266.607208f, .y = 137.757507f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode137{.x = 266.607208f, .y = 141.357498f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode138{.x = 266.607208f, .y = 144.957504f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode139{.x = 266.607208f, .y = 148.557495f, .z = 62.4207993f};
inline constexpr Vector3 syspCameraTargetMode140{.x = 294.200012f, .y = 131.199997f, .z = 75.5500031f};
inline constexpr Vector3 syspCameraTargetMode143{.x = 225.899994f, .y = 102.400002f, .z = 194.800003f};
inline constexpr Vector3 syspCameraTargetMode144{.x = 248.689102f, .y = 101.822502f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode145{.x = 248.689102f, .y = 109.022499f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode146{.x = 248.689102f, .y = 112.622498f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode147{.x = 248.689102f, .y = 116.222504f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode148{.x = 248.689102f, .y = 119.822502f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode149{.x = 248.689102f, .y = 123.422501f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode150{.x = 248.689102f, .y = 127.022499f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode151{.x = 248.689102f, .y = 130.622498f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode152{.x = 248.689102f, .y = 134.222504f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode153{.x = 248.689102f, .y = 137.822495f, .z = 206.302902f};
inline constexpr Vector3 syspCameraTargetMode165{.x = 241.800003f, .y = 141.800003f, .z = 216.100006f};
inline constexpr Vector3 syspCameraTargetMode166{.x = 188.444397f, .y = 104.059998f, .z = 119.075104f};
inline constexpr Vector3 syspCameraTargetMode167{.x = 190.580002f, .y = 101.712502f, .z = 75.0000000f};
inline constexpr Vector3 syspCameraTargetMode168{.x = 190.580002f, .y = 119.712502f, .z = 75.0000000f};
inline constexpr Vector3 syspCameraTargetMode169{.x = 165.899994f, .y = 114.370003f, .z = 121.370003f};
inline constexpr Vector3 syspCameraTargetMode170{.x = 165.899994f, .y = 125.169998f, .z = 121.370003f};
inline constexpr Vector3 syspCameraTargetMode171{.x = 172.199997f, .y = 118.000000f, .z = 97.1999969f};
inline constexpr Vector3 syspCameraTargetMode186{.x = 267.399994f, .y = 102.599998f, .z = 233.199997f};
inline constexpr Vector3 syspCameraTargetMode187{.x = 182.300003f, .y = 103.500000f, .z = 131.350006f};
inline constexpr Vector3 syspCameraTargetMode188{.x = 165.880005f, .y = 113.800003f, .z = 118.709999f};
inline constexpr Vector3 syspCameraTargetMode189{.x = 165.880005f, .y = 125.000000f, .z = 118.709999f};
inline constexpr Vector3 syspCameraTargetMode197{.x = 249.550003f, .y = 98.6999969f, .z = 122.379997f};
inline constexpr Vector3 syspCameraTargetMode201{.x = 264.600006f, .y = 103.800003f, .z = 167.399994f};
inline constexpr Vector3 syspCameraTargetMode202{.x = 263.700012f, .y = 111.099998f, .z = 140.399994f};
inline constexpr Vector3 syspCameraTargetMode203{.x = 264.600006f, .y = 114.800003f, .z = 167.399994f};
inline constexpr Vector3 syspCameraTargetMode204{.x = 263.700012f, .y = 118.400002f, .z = 140.399994f};
inline constexpr Vector3 syspCameraTargetMode205{.x = 264.600006f, .y = 122.000000f, .z = 167.399994f};
inline constexpr Vector3 syspCameraTargetMode206{.x = 263.700012f, .y = 125.199997f, .z = 140.399994f};
inline constexpr Vector3 syspCameraTargetMode207{.x = 264.600006f, .y = 139.899994f, .z = 167.399994f};
inline constexpr Vector3 syspCameraTargetMode208{.x = 224.100006f, .y = 147.199997f, .z = 186.300003f};
inline constexpr Vector3 syspCameraTargetMode209{.x = 264.600006f, .y = 158.000000f, .z = 167.399994f};
inline constexpr Vector3 syspCameraTargetMode215{.x = 241.000000f, .y = 148.000000f, .z = 155.000000f};
inline constexpr Vector3 syspCameraTargetMode216{.x = 249.199997f, .y = 138.600006f, .z = 167.699997f};
inline constexpr Vector3 syspCameraTargetMode247{.x = -400.359985f, .y = 27.7800007f, .z = -803.609985f};
inline constexpr Vector3 syspCameraTargetMode250{.x = 150.759995f, .y = 100.510002f, .z = 215.169998f};
inline constexpr Vector3 syspNpcTargetMode199{.x = 217.399994f, .y = 93.0199966f, .z = 129.800003f};
inline constexpr Vector3 syspNpcTargetMode201{.x = 252.600006f, .y = 100.000000f, .z = 166.800003f};
inline constexpr Vector3 syspNpcTargetMode202{.x = 250.600006f, .y = 107.199997f, .z = 141.000000f};
inline constexpr Vector3 syspNpcTargetMode203{.x = 252.600006f, .y = 110.800003f, .z = 167.800003f};
inline constexpr Vector3 syspNpcTargetMode204{.x = 250.600006f, .y = 114.400002f, .z = 141.000000f};
inline constexpr Vector3 syspNpcTargetMode205{.x = 252.000000f, .y = 118.000000f, .z = 166.399994f};
inline constexpr Vector3 syspNpcTargetMode206{.x = 250.199997f, .y = 121.599998f, .z = 139.399994f};
inline constexpr Vector3 syspNpcTargetMode207{.x = 252.600006f, .y = 136.000000f, .z = 166.300003f};
inline constexpr Vector3 syspNpcTargetMode208{.x = 219.300003f, .y = 143.199997f, .z = 178.399994f};
inline constexpr Vector3 syspNpcTargetMode209{.x = 252.600006f, .y = 154.000000f, .z = 166.300003f};
inline constexpr Vector3 syspNpcTargetMode211{.x = 200.300003f, .y = 121.599998f, .z = 176.699997f};
inline constexpr Vector3 syspNpcTargetMode212{.x = 212.899994f, .y = 121.599998f, .z = 146.800003f};
inline constexpr Vector3 syspNpcTargetMode213{.x = 248.699997f, .y = 121.599998f, .z = 138.000000f};
inline constexpr Vector3 syspNpcTargetMode214{.x = 253.800003f, .y = 132.399994f, .z = 140.199997f};
inline constexpr Vector3 syspNpcTargetMode218{.x = 253.800003f, .y = 150.399994f, .z = 140.199997f};
inline constexpr Vector3 syspNpcTargetMode220{.x = 210.000000f, .y = 154.000000f, .z = 178.000000f};
inline constexpr Vector3 syspNpcTargetMode242{.x = -201.600006f, .y = 18.0249996f, .z = -175.000000f};
inline constexpr Vector3 syspNpcTargetMode246{.x = -363.200012f, .y = 17.0249996f, .z = -623.500000f};

extern std::int32_t syspState, syspFlags, syspPackageLoaded, syspElapsedFrames, syspRyoNearRen, syspNpcWasVisible;
extern std::int32_t syspCameraPhase, syspCameraPhaseStep, syspPreviousPerspective, syspFovPercent, syspRetryCount, syspEventMode, syspNextDialogueMode;
extern std::uint32_t syspEventYaw, syspDefaultYaw, syspAlternateYaw, syspNextEvent, syspWaitTicks;
extern std::int32_t syspSavedMotion, syspItemAlphaState;
extern float syspSavedMotionFrame, syspSecondaryPhaseScale, syspItemAlpha;
extern shen_name syspNpc;
extern const char *syspVoiceScript;
extern const char *syspAttachedItem;
extern const Vector3 *syspTargetNpcPosition;
extern ConversationBuffer *syspConversation;
extern Vector3 syspTargetPosition, syspRyoTargetPosition, syspPreviousCameraPosition, syspPreviousNextCameraPosition, syspSavedNpcPosition;
extern Vector3i syspSavedNpcRotation;
extern Vector4 syspPreviousRyoPosition;
void SyspInitNullsub();
void SyspMain(std::uintptr_t argument);
void SyspStartEvent(SyspEventType type, std::uintptr_t unused, const char *voice, const Vector3 *target, std::uint32_t yaw, shen_name npc,
    std::uint32_t fovPercent, const char *item);
void SyspSimpleCallback(void *data);
void SyspNpcCallback(void *data);
void SyspNpcCharacterCallback(void *data);
void SyspItemCallback(void *data);
void SyspRyoCallback(void *data);
void SyspOtherCallback(void *data);
void SyspEndCallback(void *data);
std::int16_t SyspSwitchToEventCamera();
bool SyspUpdateCameraBlendAndRyoFade(std::int32_t skipRyoFade);
extern const ScriptAreaDefinition syspScriptDefinition;
} // namespace shenmue::s2::areas
