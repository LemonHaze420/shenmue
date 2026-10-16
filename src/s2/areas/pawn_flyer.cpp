#include "s2/areas/area_pawn.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr float F(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}
struct PawnFlyerCamera {
    std::int32_t id, delay;
};
struct PawnFlyerVoice {
    const char *token;
    std::int32_t delay;
};
struct PawnFlyerScene {
    std::uint32_t playerMotion, ownerMotion;
    std::int32_t duration;
    float playerFrame, ownerFrame;
    Vector3 ownerPosition;
    Vector3i ownerRotation;
    std::int32_t ownerDelay, ownerDuration;
    Vector3 playerPosition;
    Vector3i playerRotation;
    std::int32_t playerDelay, playerDuration, finalDelay, award;
    bool ownerCamera;
    PawnFlyerCamera cameras[5];
    PawnFlyerVoice voices[5];
};
const PawnFlyerScene flyerScenes[12]{{.playerMotion = 7303,
                                         .ownerMotion = 7315,
                                         .duration = 570,
                                         .playerFrame = 80,
                                         .ownerFrame = 80,
                                         .ownerPosition = {.193f, .155f, -.063f},
                                         .ownerRotation = {34388, 17694, 57107},
                                         .ownerDelay = 180,
                                         .ownerDuration = 59,
                                         .playerPosition = {.243f, .049f, -.084f},
                                         .playerRotation = {60438, 48969, 55705},
                                         .playerDelay = 240,
                                         .playerDuration = 330,
                                         .finalDelay = 571,
                                         .award = 500,
                                         .ownerCamera = false,
                                         .cameras = {{1600, 0}, {1601, 100}, {1602, 180}, {1604, 240}, {1603, 340}},
                                         .voices = {{"0121B014", 33}, {"0121A005", 127}, {"0121B015", 165}, {"0121B016", 270}, {"0121B017", 470}}},
    {.playerMotion = 7310,
        .ownerMotion = 7301,
        .duration = 800,
        .playerFrame = 540,
        .ownerFrame = 540,
        .ownerPosition = {.212f, F(0x3dd4fdf4), -.069f},
        .ownerRotation = {58285, 20201, 15848},
        .ownerDelay = 323,
        .ownerDuration = 85,
        .playerPosition = {.256f, .057f, .024f},
        .playerRotation = {34770, 53703, 14745},
        .playerDelay = 409,
        .playerDuration = 170,
        .finalDelay = 580,
        .award = 500,
        .ownerCamera = true,
        .cameras = {{1610, 0}, {1611, 260}, {1612, 330}, {1613, 409}, {1614, 649}},
        .voices = {{"0121G002", 35}, {"0121A005", 175}, {"0121G003", 276}, {"0121G004", 433}, {"0121G005", 640}}},
    {.playerMotion = 7306,
        .ownerMotion = 7298,
        .duration = 600,
        .playerFrame = 290,
        .ownerFrame = 290,
        .ownerPosition = {F(0x3e591687), F(0x3e0c49ba), -.08f},
        .ownerRotation = {62077, 17093, 20024},
        .ownerDelay = 230,
        .ownerDuration = 94,
        .playerPosition = {.216f, .061f, -.119f},
        .playerRotation = {48787, 50972, 3640},
        .playerDelay = 329,
        .playerDuration = 100,
        .finalDelay = 430,
        .award = 340,
        .ownerCamera = true,
        .cameras = {{1620, 0}, {1621, 100}, {1622, 329}, {1623, 451}, {0, 0}},
        .voices = {{"0121E002", 10}, {"0121A005", 120}, {"0121E003", 170}, {"0121E004", 380}, {"0121E005", 510}}},
    {.playerMotion = 7303,
        .ownerMotion = 7320,
        .duration = 740,
        .playerFrame = 100,
        .ownerFrame = 100,
        .ownerPosition = {.243f, .046f, -.066f},
        .ownerRotation = {182, 21663, 19660},
        .ownerDelay = 154,
        .ownerDuration = 75,
        .playerPosition = {.242f, .075f, -.066f},
        .playerRotation = {61713, 48241, 56433},
        .playerDelay = 230,
        .playerDuration = 335,
        .finalDelay = 566,
        .award = 240,
        .ownerCamera = false,
        .cameras = {{1630, 0}, {1631, 154}, {1635, 220}, {1634, 566}, {0, 0}},
        .voices = {{"0121H002", 10}, {"0121A005", 100}, {"0121H003", 150}, {"0121H004", 295}, {"0121H005", 631}}},
    {.playerMotion = 7302,
        .ownerMotion = 7299,
        .duration = 640,
        .playerFrame = 0,
        .ownerFrame = 0,
        .ownerPosition = {F(0x3e6a7efa), F(0x3d71a9fc), -.085f},
        .ownerRotation = {48787, 21299, 1820},
        .ownerDelay = 200,
        .ownerDuration = 94,
        .playerPosition = {.234f, .057f, -.093f},
        .playerRotation = {41506, 53521, 10558},
        .playerDelay = 295,
        .playerDuration = 330,
        .finalDelay = 626,
        .award = 300,
        .ownerCamera = false,
        .cameras = {{1650, 0}, {1651, 215}, {1652, 245}, {1653, 295}, {1654, 420}},
        .voices = {{"0121D002", 76}, {"0121A005", 148}, {"0121D003", 168}, {"0121D004", 420}, {"0121D005", 580}}},
    {.playerMotion = 7308,
        .ownerMotion = 7300,
        .duration = 500,
        .playerFrame = 309,
        .ownerFrame = 309,
        .ownerPosition = {.222f, .029f, -.092f},
        .ownerRotation = {26214, 13107, 44418},
        .ownerDelay = 200,
        .ownerDuration = 74,
        .playerPosition = {.23f, .09f, -.098f},
        .playerRotation = {48423, 52974, 5825},
        .playerDelay = 276,
        .playerDuration = 114,
        .finalDelay = 391,
        .award = 280,
        .ownerCamera = false,
        .cameras = {{1660, 0}, {1661, 200}, {1662, 275}, {0, 0}, {0, 0}},
        .voices = {{"0121M002", 1}, {"0121A005", 130}, {"0121M003", 190}, {"0121M004", 290}, {"0121M005", 414}}},
    {.playerMotion = 7307,
        .ownerMotion = 7317,
        .duration = 515,
        .playerFrame = 280,
        .ownerFrame = 270,
        .ownerPosition = {.249f, .072f, -.056f},
        .ownerRotation = {1638, 18022, 21481},
        .ownerDelay = 250,
        .ownerDuration = 59,
        .playerPosition = {.225f, .071f, -.13f},
        .playerRotation = {50426, 52974, 2548},
        .playerDelay = 310,
        .playerDuration = 89,
        .finalDelay = 400,
        .award = 315,
        .ownerCamera = true,
        .cameras = {{1670, 0}, {1671, 125}, {1672, 215}, {1673, 310}, {1674, 340}},
        .voices = {{"0121K002", 30}, {"0121A005", 130}, {"0121K003", 220}, {"0121K004", 315}, {"0121K005", 420}}},
    {.playerMotion = 7309,
        .ownerMotion = 7319,
        .duration = 540,
        .playerFrame = 250,
        .ownerFrame = 250,
        .ownerPosition = {F(0x3e72b021), F(0x3d4ccccd), -.119f},
        .ownerRotation = {50244, 19114, 3276},
        .ownerDelay = 182,
        .ownerDuration = 75,
        .playerPosition = {.24f, .035f, -.093f},
        .playerRotation = {49698, 51154, 65353},
        .playerDelay = 258,
        .playerDuration = 92,
        .finalDelay = 351,
        .award = 260,
        .ownerCamera = false,
        .cameras = {{1690, 0}, {1691, 182}, {1692, 258}, {0, 0}, {0, 0}},
        .voices = {{"0121L002", 40}, {"0121A005", 140}, {"0121L003", 210}, {"0121L004", 350}, {"0121L005", 470}}},
    {.playerMotion = 7304,
        .ownerMotion = 7314,
        .duration = 260,
        .playerFrame = 320,
        .ownerFrame = 320,
        .ownerPosition = {F(0x3e810625), F(0x3d178d50), -.033f},
        .ownerRotation = {728, 17476, 17476},
        .ownerDelay = 225,
        .ownerDuration = 59,
        .playerPosition = {.252f, .056f, -.012f},
        .playerRotation = {63897, 44983, 52082},
        .playerDelay = 321,
        .playerDuration = 110,
        .finalDelay = 431,
        .award = 300,
        .ownerCamera = false,
        .cameras = {{1680, 0}, {1681, 130}, {1682, 200}, {1683, 250}, {1684, 321}},
        .voices = {{"0121C002", 30}, {"0121A005", 130}, {"0121C003", 250}, {"0121C004", 320}, {"0121C005", 430}}},
    {.playerMotion = 7305,
        .ownerMotion = 7297,
        .duration = 700,
        .playerFrame = 130,
        .ownerFrame = 130,
        .ownerPosition = {F(0x3e6b851f), F(0x3d83126f), -.08f},
        .ownerRotation = {1638, 17476, 21663},
        .ownerDelay = 230,
        .ownerDuration = 109,
        .playerPosition = {.256f, .067f, -.059f},
        .playerRotation = {49880, 48605, 1820},
        .playerDelay = 340,
        .playerDuration = 120,
        .finalDelay = 461,
        .award = 350,
        .ownerCamera = false,
        .cameras = {{1640, 0}, {1641, 275}, {1642, 295}, {1643, 340}, {1644, 400}},
        .voices = {{"0121F002", 1}, {"0121A005", 130}, {"0121F003", 200}, {"0121F004", 400}, {"0121F005", 550}}},
    {.playerMotion = 7310,
        .ownerMotion = 7301,
        .duration = 775,
        .playerFrame = 540,
        .ownerFrame = 540,
        .ownerPosition = {F(0x3e6978d5), F(0x3da1cac1), -.08f},
        .ownerRotation = {3276, 17658, 24758},
        .ownerDelay = 330,
        .ownerDuration = 78,
        .playerPosition = {.256f, .057f, .024f},
        .playerRotation = {34770, 53703, 14745},
        .playerDelay = 409,
        .playerDuration = 169,
        .finalDelay = 580,
        .award = 410,
        .ownerCamera = false,
        .cameras = {{1700, 0}, {1701, 260}, {1702, 330}, {1705, 409}, {1704, 649}},
        .voices = {{"0121J002", 1}, {"0121A005", 150}, {"0121J003", 200}, {"0121J004", 400}, {"0121J005", 600}}},
    {.playerMotion = 7305,
        .ownerMotion = 7297,
        .duration = 700,
        .playerFrame = 130,
        .ownerFrame = 130,
        .ownerPosition = {F(0x3e1fbe77), F(0x3de147ae), -.141f},
        .ownerRotation = {46603, 24557, 6553},
        .ownerDelay = 230,
        .ownerDuration = 110,
        .playerPosition = {.244203f, .045056999f, -.084f},
        .playerRotation = {7281, 45693, 43690},
        .playerDelay = 341,
        .playerDuration = 119,
        .finalDelay = 461,
        .award = 350,
        .ownerCamera = false,
        .cameras = {{1720, 0}, {1721, 275}, {1722, 295}, {1723, 340}, {1724, 400}},
        .voices = {{"0121I002", 1}, {"0121A005", 130}, {"0121I003", 200}, {"0121I004", 450}, {"0121I005", 600}}}};
std::int32_t pawnRightHandCommand[19][3]{{0, 730, 63895}, {0, 0, 364}, {0, 64079, 1999}, {0, 0, 57163}, {0, 65172, 53522}, {0, 65172, 58802}, {0, 0, 0},
    {0, 0, 56251}, {0, 0, 48060}, {0, 0, 55887}, {0, 0, 0}, {0, 0, 58073}, {0, 182, 45876}, {0, 364, 54431}, {0, 18, 0}, {0, 65354, 55339}, {725, 65172, 47695},
    {1456, 729, 54796}, {0, 65445, 65172}};
struct PawnFlyerAwardWork {
    std::int32_t elapsed, frame, item;
};
void PawnFlyerAward(void *parameter) {
    PawnFlyerAwardWork *work = static_cast<PawnFlyerAwardWork *>(parameter);
    if (work && work->frame >= 0 && work->frame <= ++work->elapsed) {
        AddInventoryItem(static_cast<std::uint32_t>(work->item), 1, 1);
        work->frame = -1;
    }
}
void QueueFlyerAttachment(shen_name character, shen_name flyer, const Vector3 &position, const Vector3i &rotation, std::int32_t delay, std::int32_t duration) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AreaLoopAttachmentCallback, 4, 0x0b, sizeof(AreaLoopAttachmentWork), PAWN_EVENT);
    if (!task)
        return;
    AreaLoopAttachmentWork *work = static_cast<AreaLoopAttachmentWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->elapsed = 0;
    work->character = character;
    work->attachmentFlags = 14;
    work->target = flyer;
    work->flags = 0;
    work->position = position;
    work->rotation = rotation;
    work->duration = duration;
    work->completion = nullptr;
    HLib::SetupTimedTask(task, delay);
}
void QueueFlyerMotion(
    shen_name character, std::uint32_t motion, std::int32_t duration, float start, float speed, std::int32_t delay, AreaLoopCompletion completion) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), PAWN_EVENT);
    if (!task)
        return;
    AreaLoopMotionWork *work = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->character = character;
    work->flags = 6;
    work->motion = motion;
    work->duration = duration;
    work->elapsed = 0;
    work->startFrame = start;
    work->endFrame = -1;
    work->speed = speed;
    work->frameTime = 0;
    work->completion = completion;
    if (delay)
        HLib::SetupTimedTask(task, delay);
    else {
        work->flags |= 0x80000000U;
        AreaLoopMotionCallback(work);
        if (work->flags & 0x80000000U)
            work->flags &= 0x7fffffffU;
        else
            HLib::DestroyTask(task);
    }
}
void ResetPawnHands(shen_name character) {
    Character *owner = ctrlGetCharaByName(character);
    GetCharacterMotionCompatibilityResult(owner);
    hndlSetFlags(owner);
}
void PreparePawnHands(shen_name character) {
    Character *owner = ctrlGetCharaByName(character);
    GetCharacterMotionCompatibilityResult(owner);
    SetLowQualityHands(owner);
    SetHighModelXYZ(owner, 1, 0, 0);
    SetHighModelXYZ(owner, 1, 1, 0);
    ApplyCharacterHandPoseCommand(owner, 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 1);
    ApplyCharacterHandPoseCommand(owner, 1, reinterpret_cast<std::byte *>(pawnRightHandCommand), 1);
}
} // namespace
void PawnBeginFlyer(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(PawnFlyerDone);
        return;
    }
    const PawnShopScene &shop = FindPawnShopScene(work->shop);
    if (!shop.owner.name || !shop.flyer.name) {
        HLib::SetCurrentTaskTo(PawnFlyerDone);
        return;
    }
    const std::int32_t index = static_cast<std::int32_t>(&shop - pawnShopScenes);
    const PawnFlyerScene &scene = flyerScenes[index];
    mtwkSetExtraFlag(scenFindMotionwork(PAWN_RYO), 17, 0);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(PAWN_RYO, &rotation, EventTransformFlags::World);
    rotation.y += 0x8000;
    EV_AT_SetAngY(PAWN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_FreeAuthorPak(1, "0121");
    PreparePawnHands(PAWN_RYO);
    PreparePawnHands(shop.owner);
    HLib::SetCurrentTaskTo(PawnFlyerDone);
    HLib::SetupTaskDelay(-1);
    const Vector4 reflector{1, 1, 1, 1};
    EV_SetRefBoardParamsFor(shop.flyer, 1);
    SetScenarioReflectorBoardValues(shop.flyer, &reflector);
    QueueFlyerMotion(PAWN_RYO, scene.playerMotion, scene.duration, scene.playerFrame, 1, 0, index == 8 ? nullptr : ResolveAreaLoopParent);
    QueueFlyerMotion(shop.owner, scene.ownerMotion, scene.duration, scene.ownerFrame, 1, 0, nullptr);
    if (index == 8) {
        QueueFlyerMotion(PAWN_RYO, 7304, 60, 560, 0, 261, nullptr);
        QueueFlyerMotion(shop.owner, 7314, 60, 560, 0, 261, nullptr);
        QueueFlyerMotion(PAWN_RYO, 7304, 200, 600, 1, 321, ResolveAreaLoopParent);
        QueueFlyerMotion(shop.owner, 7314, 200, 600, 1, 321, nullptr);
    }
    QueueFlyerAttachment(shop.owner, shop.flyer, scene.ownerPosition, scene.ownerRotation, scene.ownerDelay, scene.ownerDuration);
    QueueFlyerAttachment(PAWN_RYO, shop.flyer, scene.playerPosition, scene.playerRotation, scene.playerDelay, scene.playerDuration);
    const Vector3 farAway{1000, 1000, 1000};
    QueueFlyerAttachment(PAWN_RYO, shop.flyer, farAway, scene.playerRotation, scene.finalDelay, 1);
    if (index == 0) {
        const Vector3i playerHand{0, 0, 0}, ownerHand{3094, 0, 3640};
        ApplyCharacterHandRotation(ctrlGetCharaByName(PAWN_RYO), 0, 21, &playerHand);
        ApplyCharacterHandRotation(ctrlGetCharaByName(shop.owner), 0, 21, &ownerHand);
    }
    for (const PawnFlyerCamera &camera : scene.cameras) {
        if (!camera.id)
            continue;
        const shen_name base = scene.ownerCamera && !(index == 2 && camera.id == 1620) ? shop.owner : PAWN_BASE;
        QueueAreaLoopCamera("", camera.id, base, nullptr, camera.delay);
    }
    for (const PawnFlyerVoice &voice : scene.voices)
        QueueAreaLoopVoice(voice.token, 0, -1, nullptr, voice.delay);
    work->flyerTask = HLib::EnqueueTaskWithParameter(PawnFlyerAward, 4, 0x0b, sizeof(PawnFlyerAwardWork), PAWN_EVENT);
    if (work->flyerTask) {
        PawnFlyerAwardWork *award = static_cast<PawnFlyerAwardWork *>(HLib::GetTaskParameter(work->flyerTask));
        if (award) {
            award->elapsed = 0;
            award->frame = scene.award;
            award->item = 9000 + index;
        } else {
            HLib::DestroyTask(work->flyerTask);
            work->flyerTask = nullptr;
        }
    }
}
void PawnFlyerDone(void *parameter) {
    PawnEventWork *work = static_cast<PawnEventWork *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(PawnFinish);
        return;
    }
    const PawnShopScene &shop = FindPawnShopScene(work->shop);
    EV_SetRefBoardParamsFor(shop.flyer, 0);
    ResetPawnHands(PAWN_RYO);
    if (shop.owner.name)
        ResetPawnHands(shop.owner);
    mtwkSetExtraFlag(scenFindMotionwork(PAWN_RYO), 17, 0);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(PAWN_RYO, &rotation, EventTransformFlags::World);
    rotation.y += 0x8000;
    EV_AT_SetAngY(PAWN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_FreeAuthorPak(0, "0121");
    RestorePawnTransactionPose(*work);
    HLib::SetCurrentTaskTo(PawnBeginSelect);
}
} // namespace shenmue::s2::areas
