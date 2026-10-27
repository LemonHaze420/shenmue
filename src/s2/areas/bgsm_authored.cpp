#include "s2/areas/area_bgsm.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/battle_qte.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/scene_data.hpp"
namespace shenmue::s2::areas {
std::int32_t bgsmOpenHand[57]{1999, 65354, 62444, 1638, 180, 63900, 546, 64719, 3366, 0, 64809, 60257, 0, 64809, 59528, 0, 0, 62623, 0, 0, 64628, 0, 0, 58983,
    0, 0, 58073, 0, 65172, 61897, 0, 0, 65354, 0, 729, 60074, 0, 65173, 57163, 0, 65172, 63171, 0, 0, 0, 0, 547, 56252, 0, 1639, 60074, 0, 64809, 61715, 64080,
    182, 65535};
std::int32_t bgsmBetHand[57]{0, 0, 56071, 0, 0, 64626, 0, 62623, 2912, 0, 0, 60257, 0, 0, 61167, 0, 0, 62988, 0, 546, 1820, 0, 0, 59712, 0, 0, 59893, 0, 0,
    62623, 0, 546, 2547, 0, 0, 61896, 0, 0, 59166, 0, 0, 64808, 0, 546, 1818, 0, 0, 52978, 0, 0, 53160, 0, 0, 57709, 0, 546, 1091};
std::int32_t bgsmReleaseHand[57]{0, 0, 61896, 0, 0, 364, 0, 62623, 3639, 0, 0, 60257, 0, 0, 61167, 0, 0, 63352, 0, 546, 2001, 0, 0, 59712, 0, 0, 59893, 0, 0,
    62805, 0, 546, 2730, 0, 0, 61896, 0, 0, 59166, 0, 0, 64989, 0, 546, 2182, 0, 0, 52978, 0, 0, 53160, 0, 0, 57709, 0, 546, 1091};
std::int32_t bgsmPotHand[57]{0, 726, 63534, 0, 0, 65353, 0, 65353, 2912, 0, 0, 60257, 0, 0, 61167, 0, 0, 58437, 0, 546, 1820, 0, 0, 58801, 0, 65172, 60986, 0,
    0, 59530, 0, 546, 1638, 0, 65172, 59348, 0, 64990, 58801, 0, 0, 62260, 0, 546, 1273, 0, 64627, 54070, 0, 64990, 182, 0, 0, 59530, 0, 546, 1091};
void BgsmCamera(std::int32_t camera, shen_name base) {
    EventSetCamera request{.cleanup = 0, .camera = camera, .eye = base, .target = base, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void BgsmMotion(shen_name character, std::uint32_t motion, std::uint32_t flags, float speed) {
    EVT_AT_SetMot(character, motion, 0, speed, flags, 0);
}
void BgsmPosition(shen_name character, shen_name base, const Vector3 &position) {
    EventSetPosition request{.cleanup = 0, .character = character, .parent = base, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&request);
}
void BgsmAngle(shen_name character, shen_name base, float degrees) {
    EventSetAngle request{.cleanup = 0, .character = character, .parent = base, .degrees = degrees, .delay = 0};
    EVT_AT_SetAngY_Callback(&request);
}
void BgsmRawAngle(shen_name character, shen_name base, const Vector3i &rotation) {
    EventSetRawAngle request{.cleanup = 0, .character = character, .parent = base, .rotation = rotation, .delay = 0};
    EVT_AT_SetAng_Callback(&request);
}
void BgsmHand(shen_name character, std::int32_t *pose, std::uint32_t frames) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(character), 0, reinterpret_cast<std::byte *>(pose), frames);
}
void BgsmCreateBoard(BgsmWork &work) {
    const GamblingTablePlacement &table = aberdeenBS[work.table];
    work.minimumBet = table.firstDigits;
    work.maximumBet = table.secondDigits;
    work.board = table.objectKinds[0];
    CreateEventCharacter(CharacterCreationHighModel, nullptr, BGSM_BASE);
    BgsmPosition(BGSM_BASE, BGSM_INVALID, table.position);
    const Vector3i angle{0, static_cast<std::int32_t>(((table.angleY + 180.0f) * 8192.0) / 45.0), 0};
    BgsmRawAngle(BGSM_BASE, BGSM_INVALID, angle);
    npcSetAreaName(BGSM_REGION, &table.position, 2, 1);
}
void BgsmPlaceCharacters(BgsmWork &work) {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, BGSM_HAND_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, BGSM_CAMERA_BASE);
    for (shen_name character : {BGSM_HAND, BGSM_POT}) {
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, ApplyGravity | UpdateHeight, NpcCharacterControllerState::Disabled);
    }
    for (shen_name die : BGSM_DICE)
        EV_EnableCharacterDisplay(die);
    EV_DisableCharacterDisplay(BGSM_POT);
    EV_DisableCharacterDisplay(BGSM_HAND);
    EV_DisableCharacterDisplay(BGSM_CASH);
    EV_EnableCharacterDisplay(BGSM_HAND_BASE);
    const Vector3 potScale{1, .6f, 1};
    mtwkSetMotionworkScale3(BGSM_POT, &potScale);
    BgsmPosition(BGSM_HAND_BASE, BGSM_BASE, {});
    BgsmAngle(BGSM_HAND_BASE, BGSM_BASE, 0);
    BgsmPosition(BGSM_POT, BGSM_BASE, {-.4f, 0, .75f});
    BgsmAngle(BGSM_POT, BGSM_BASE, 0);
    BgsmPosition(BGSM_HAND, BGSM_HAND_BASE, {.25f, .05f, -.44f});
    BgsmRawAngle(BGSM_HAND, BGSM_HAND_BASE, {32768, 0, 24576});
    BgsmPosition(BGSM_DICE[0], BGSM_BASE, {-.153f, .76f, .063f});
    BgsmPosition(BGSM_DICE[1], BGSM_BASE, {-.231f, .76f, .134f});
    BgsmPosition(BGSM_DICE[2], BGSM_BASE, {-.183f, .76f, .118f});
    BgsmRawAngle(BGSM_DICE[0], BGSM_BASE, {0, 0, 49152});
    BgsmRawAngle(BGSM_DICE[1], BGSM_BASE, {});
    BgsmRawAngle(BGSM_DICE[2], BGSM_BASE, {49152, 0, 0});
    AT_SetLocalBaseChara(BGSM_HAND, BGSM_HAND_BASE);
    AT_SetLocalBaseChara(BGSM_CASH, BGSM_HAND_BASE);
    AT_SetLocalBaseChara(BGSM_CASH, BGSM_BASE);
    for (shen_name die : BGSM_DICE)
        AT_SetLocalBaseChara(die, BGSM_BASE);
    const std::int32_t motion = GetMotionRigBoneId(scenFindMotionwork(work.opponent));
    bgsmOpponentPose = motion == 7 ? 0 : (motion != 9) + 1;
    const Vector4 *spots = BGSM_SPOT_COUNTS[work.board] == 4 ? BGSM_FOUR_SPOTS : BGSM_NINE_SPOTS;
    for (std::int32_t index = 0; index < BGSM_SPOT_COUNTS[work.board]; ++index) {
        const shen_name name = BGSM_SPOTS[index];
        const Vector4 &spot = spots[index];
        CreateEventCharacter(CharacterCreationHighModel, nullptr, name);
        EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, ApplyGravity | UpdateHeight, NpcCharacterControllerState::Disabled);
        AT_SetLocalBaseChara(name, BGSM_BASE);
        BgsmPosition(name, BGSM_BASE, {spot.x, spot.y, spot.z});
        BgsmRawAngle(name, BGSM_BASE, {0, static_cast<std::int32_t>((spot.w * 8192.0) / 45.0), 0});
    }
    if (((g_currentMap.name - SHEN_NAME(AKT0)) & 0xfcffffffU) == 0) {
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(BGSM_RYO), 0);
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    }
}
} // namespace shenmue::s2::areas
