#include "s2/areas/area_4nin.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/load_title.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/memo.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstddef>

namespace shenmue::s2::areas {
namespace {
struct RenRouteStorage {
    NpcRouteNode node{offsetof(RenRouteStorage, points), 26, offsetof(RenRouteStorage, lengths), {}, {}, offsetof(RenRouteStorage, lanes),
        offsetof(RenRouteStorage, auxiliary), shen_name{827213138U}, 0};
    Vector3 points[26]{{211.67066955566406f, 0.0f, 427.1578063964844f}, {220.22560119628906f, 0.0f, 429.2832946777344f},
        {228.78053283691406f, 0.0f, 431.4087829589844f}, {239.13406372070312f, 0.0f, 432.3352966308594f}, {248.7242889404297f, 0.0f, 434.624267578125f},
        {248.39732360839844f, 0.0f, 442.1451110839844f}, {246.87161254882812f, 0.0f, 448.2490234375f}, {240.60525512695312f, 0.0f, 447.8675231933594f},
        {233.41258239746094f, 0.0f, 447.8675231933594f}, {233.46665954589844f, 0.0f, 454.73443603515625f}, {233.6305389404297f, 0.0f, 458.7128601074219f},
        {239.29708862304688f, 0.0f, 458.76739501953125f}, {247.36424255371094f, 0.0f, 457.9498596191406f}, {247.14364624023438f, 0.0f, 462.6368408203125f},
        {246.60137939453125f, 0.0f, 470.48468017578125f}, {251.8297882080078f, 0.0f, 470.15771484375f}, {258.8589782714844f, 0.0f, 470.0487365722656f},
        {263.3298034667969f, 0.0f, 470.6481628417969f}, {264.58306884765625f, 0.0f, 474.0271301269531f}, {282.1878662109375f, 0.0f, 472.9371643066406f},
        {281.9670104980469f, 0.0f, 479.25909423828125f}, {283.98602294921875f, 0.0f, 486.2894592285156f}, {294.7750244140625f, 0.0f, 486.56195068359375f},
        {306.54486083984375f, 0.0f, 485.85345458984375f}, {310.1412048339844f, 0.0f, 485.6899719238281f}, {313.737548828125f, 0.0f, 485.5264892578125f}};
    float lengths[24]{9.649639129638672f, 9.861083984375f, 8.608020782470703f, 6.63024377822876f, 5.736050128936768f, 5.675768852233887f, 5.991607189178467f,
        5.421294212341309f, 4.048713207244873f, 5.147708892822266f, 6.273675918579102f, 4.62244176864624f, 5.981783866882324f, 4.821096897125244f,
        6.213950157165527f, 4.461771011352539f, 4.981778144836426f, 11.820623397827148f, 6.4808030128479f, 6.707159996032715f, 9.493331909179688f,
        10.13359546661377f, 5.195018768310547f, 0.0f};
    float lanes[46]{2.4000000953674316f, 2.299999952316284f, 2.5999999046325684f, 1.2999999523162842f, 3.0f, 0.5f, 0.4000000059604645f, 1.2999999523162842f,
        0.5f, 0.4000000059604645f, 1.5f, 0.4000000059604645f, 0.6000000238418579f, 1.100000023841858f, 0.20000000298023224f, 1.5f, 0.4000000059604645f, 1.5f,
        0.4000000059604645f, 0.4000000059604645f, 0.6000000238418579f, 0.30000001192092896f, 0.699999988079071f, 0.20000000298023224f, 0.30000001192092896f,
        0.6000000238418579f, 0.30000001192092896f, 0.5f, 0.6000000238418579f, 0.6000000238418579f, 0.0f, 0.0f, 0.800000011920929f, 0.0f, 1.100000023841858f,
        1.0f, 3.0f, 0.6000000238418579f, 2.299999952316284f, 3.0f, 1.899999976158142f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
    std::uint8_t auxiliary[24]{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
};
RenRouteStorage renRoute;
struct SamRouteStorage {
    NpcRouteNode node{offsetof(SamRouteStorage, points), 12, offsetof(SamRouteStorage, lengths), {}, {}, offsetof(SamRouteStorage, lanes),
        offsetof(SamRouteStorage, auxiliary), shen_name{1296908627U}, 0};
    Vector3 points[12]{{193.6357879638672f, 0.0f, 416.7452392578125f}, {207.15408325195312f, 0.0f, 424.79949951171875f},
        {220.67237854003906f, 0.0f, 432.853759765625f}, {239.8071746826172f, 0.0f, 432.9641418457031f}, {248.30706787109375f, 0.0f, 433.9742126464844f},
        {248.14097595214844f, 0.0f, 439.53289794921875f}, {247.73220825195312f, 0.0f, 448.055419921875f}, {241.46051025390625f, 0.0f, 447.9872131347656f},
        {233.89389038085938f, 0.0f, 448.1917724609375f}, {233.5529022216797f, 0.0f, 452.07806396484375f}, {233.30319213867188f, 0.0f, 455.0069274902344f},
        {233.05348205566406f, 0.0f, 457.935791015625f}};
    float lengths[10]{71.95285034179688f, 15.744865417480469f, 16.316492080688477f, 9.00100326538086f, 5.549670219421387f, 6.74118185043335f,
        5.777067184448242f, 6.168703079223633f, 3.5999960899353027f, 3.0538690090179443f};
    float lanes[18]{3.0f, 2.0f, 3.0f, 0.5f, 3.0f, 0.8999999761581421f, 3.0f, 1.2000000476837158f, 0.699999988079071f, 0.800000011920929f, 2.0f, 0.5f,
        0.699999988079071f, 1.2000000476837158f, 0.20000000298023224f, 1.5f, 0.800000011920929f, 1.2999999523162842f};
    std::uint8_t auxiliary[10]{255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
};
SamRouteStorage samRoute;
struct LlyRouteStorage {
    NpcRouteNode node{offsetof(LlyRouteStorage, points), 31, offsetof(LlyRouteStorage, lengths), {}, {}, offsetof(LlyRouteStorage, lanes),
        offsetof(LlyRouteStorage, auxiliary), shen_name{1498169676U}, 0};
    Vector3 points[31]{{253.2860565185547f, 0.0f, 366.5527648925781f}, {251.90797424316406f, 0.0f, 356.7822570800781f},
        {250.52989196777344f, 0.0f, 347.0117492675781f}, {248.48118591308594f, 0.0f, 326.76202392578125f}, {236.9606170654297f, 0.0f, 325.2620544433594f},
        {226.25807189941406f, 0.0f, 325.1257019042969f}, {222.91400146484375f, 0.0f, 330.171142578125f}, {221.0735626220703f, 0.0f, 349.3980407714844f},
        {220.73272705078125f, 0.0f, 361.32965087890625f}, {221.82342529296875f, 0.0f, 367.05682373046875f}, {227.0768280029297f, 0.0f, 371.2158203125f},
        {238.80429077148438f, 0.0f, 378.51123046875f}, {246.84585571289062f, 0.0f, 379.9429626464844f}, {254.68768310546875f, 0.0f, 379.80657958984375f},
        {257.48260498046875f, 0.0f, 381.23846435546875f}, {259.7205505371094f, 0.0f, 391.1070556640625f}, {259.54901123046875f, 0.0f, 396.7685546875f},
        {259.7205505371094f, 0.0f, 402.9447326660156f}, {266.7533264160156f, 0.0f, 404.66033935546875f}, {269.1547546386719f, 0.0f, 409.6355895996094f},
        {270.1839599609375f, 0.0f, 412.8094482421875f}, {270.26971435546875f, 0.0f, 417.44158935546875f}, {267.09625244140625f, 0.0f, 418.8741149902344f},
        {257.8205871582031f, 0.0f, 419.0786437988281f}, {254.75299072265625f, 0.0f, 419.8286437988281f}, {254.13946533203125f, 0.0f, 425.146728515625f},
        {253.3739013671875f, 0.0f, 432.1100158691406f}, {242.8466339111328f, 0.0f, 430.9159851074219f}, {226.2693634033203f, 0.0f, 430.31317138671875f},
        {217.443359375f, 0.0f, 424.1985168457031f}, {208.6173553466797f, 0.0f, 418.0838623046875f}};
    float lengths[29]{228.9832000732422f, 11.378642082214355f, 15.683469772338867f, 11.0443115234375f, 9.56643295288086f, 8.071304321289062f,
        15.520991325378418f, 11.98548412322998f, 6.6720170974731445f, 7.389444828033447f, 11.431571006774902f, 8.807185173034668f, 6.961660861968994f,
        4.564375877380371f, 7.895944118499756f, 6.368566989898682f, 5.5384111404418945f, 5.870255947113037f, 5.07470178604126f, 3.827112913131714f,
        3.8682329654693604f, 4.1235880851745605f, 7.145157814025879f, 4.07008695602417f, 4.878814220428467f, 5.997517108917236f, 9.789488792419434f,
        14.184952735900879f, 11.273486137390137f};
    float lanes[56]{1.2999999523162842f, 3.0f, 3.0f, 3.0f, 0.699999988079071f, 3.0f, 0.8999999761581421f, 2.5999999046325684f, 0.6000000238418579f, 3.0f,
        0.8999999761581421f, 3.0f, 1.2000000476837158f, 3.0f, 0.20000000298023224f, 3.0f, 0.20000000298023224f, 3.0f, 3.0f, 3.0f, 0.6000000238418579f, 1.5f,
        1.0f, 0.8999999761581421f, 1.399999976158142f, 0.30000001192092896f, 0.10000000149011612f, 1.399999976158142f, 0.10000000149011612f, 0.5f,
        0.800000011920929f, 0.6000000238418579f, 1.899999976158142f, 0.8999999761581421f, 2.0f, 1.600000023841858f, 2.200000047683716f, 0.8999999761581421f,
        2.0999999046325684f, 1.2000000476837158f, 1.899999976158142f, 0.8999999761581421f, 1.5f, 1.5f, 0.800000011920929f, 2.0f, 0.800000011920929f,
        1.7999999523162842f, 1.7999999523162842f, 0.6000000238418579f, 3.0f, 0.4000000059604645f, 2.799999952316284f, 0.8999999761581421f, 3.0f,
        2.0999999046325684f};
    std::uint8_t auxiliary[29]{
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
};
LlyRouteStorage llyRoute;
constexpr Vector3 p9cc138{256.5f, 100.2239990234375f, 304.5f};
constexpr Vector3i p9cc178{0, 49152, 0};
constexpr Vector3i p9cc260{0, 32768, 0};
constexpr Vector3 p9cc270{237.3000030517578f, 98.32499694824219f, 463.1000061035156f};
constexpr Vector3 p9cc280{254.0f, 100.2239990234375f, 306.79998779296875f};
constexpr Vector3 p9cc2d8{252.4499969482422f, 100.2239990234375f, 366.9200134277344f};
constexpr Vector3i p9cc428{0, 32768, 0};
constexpr Vector3 p9cc580{256.0f, 100.2239990234375f, 305.75f};
constexpr Vector3 p9cc590{233.3000030517578f, 101.56999969482422f, 455.79998779296875f};
constexpr Vector3 p9cc5a0{233.0f, 98.31999969482422f, 463.5f};
constexpr Vector3 p9cc5b0{283.0f, 100.0f, 487.0f};
constexpr Vector3 p9cc5c0{232.0f, 100.25f, 431.5f};
constexpr Vector3i p9cc688{0, 16384, 0};
constexpr Vector3 p9cc698{256.0f, 100.2239990234375f, 305.0f};
constexpr Vector3 p9cc790{252.0f, 100.2239990234375f, 364.5f};
constexpr Vector3 p9cc7a0{255.9241943359375f, 100.2239990234375f, 305.4206848144531f};
constexpr Vector3 p9cc928{253.5f, 100.2239990234375f, 365.9200134277344f};
constexpr Vector3 p9cc990{237.5f, 98.30000305175781f, 461.8999938964844f};
constexpr Vector3i p9cc9a0{0, 57344, 0};
constexpr Vector3i p9cc9b0{0, 57344, 0};
constexpr Vector3i p9cc9c0{0, 49152, 0};
constexpr Vector3 cljPoints[7]{{210.0f, 99.30000305175781f, 415.0f}, {217.0f, 100.0f, 409.0f}, {222.5f, 100.0f, 380.0f}, {221.0f, 100.0f, 345.0f},
    {227.0f, 100.0f, 325.0f}, {238.0f, 100.0f, 321.0f}, {256.0f, 100.0f, 305.0f}};

constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, sam{SHEN_NAME(SAM_)}, lly{SHEN_NAME(LLY_)}, clj{SHEN_NAME(CLJ_)}, a014{SHEN_NAME(a014)},
    dummy{SHEN_NAME(DUMY)};
constexpr shen_name npcNames[]{ren, sam, lly, clj, shen_name{0xffffffffU}};
constexpr shen_name allNames[]{ryo, ren, sam, lly, clj};
constexpr std::uint32_t Init = SHEN_NAME(INIT), Run = make_shen_name('R', 'U', 'N', ' '), Start = SHEN_NAME(STRT), Auth = SHEN_NAME(AUTH),
                        Exit = SHEN_NAME(EXIT), Idle = SHEN_NAME(IDLE), Sleep = SHEN_NAME(SLEP), Goal = SHEN_NAME(GOAL), Capture = SHEN_NAME(CAPT),
                        Finish = make_shen_name('F', 'I', 'N', ' '), Fence = SHEN_NAME(FENC), Battle = SHEN_NAME(BATL), Exit2 = SHEN_NAME(EXT2);
struct ChaseContext {
    shen_name character;
    std::uint32_t state;
    Vector3 position;
    Vector3i rotation;
    float normalSpeed, nearSpeed;
    std::int32_t pointCount, cursor;
    std::int64_t phase;
};
static_assert(offsetof(ChaseContext, position) == 8);
static_assert(offsetof(ChaseContext, normalSpeed) == 0x20);
static_assert(offsetof(ChaseContext, phase) == 0x30);
static_assert(sizeof(ChaseContext) == 0x38);
ChaseContext renChase{}, samChase{}, llyChase{}, cljChase{};
Vector3 playerPosition{};
std::uint32_t mainState{}, subState{};
std::int32_t authorFlag{}, renGrace{}, prelude{}, escaped{};
MotionWork *MotionFor(shen_name name) {
    return FindMotionWork(FindCharacterByScenarioName(name));
}
void Film(std::uint32_t frames, std::uint8_t start, std::uint8_t target) {
    SetScreenFilmFade(frames, start, 0, 0, 0, target, 0, 0, 0);
}
void Play(shen_name name, std::uint16_t motion) {
    mtwkRequestMotion(MotionFor(name), motion, MotionRegion::LowerBody);
}
void Place(shen_name name, const Vector3 &position, const Vector3i &rotation) {
    EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, rotation);
}
void Controllers(shen_name name, bool enabled) {
    const auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(name, 2, state);
    EV_SetCharaControllerFlags(name, 3, state);
}
void RetireNpc(shen_name name) {
    PlaceNpcAtAreaPoint(name, shen_name{}, nullptr, 0);
    EV_npcLockUnlock(name);
    EV_npcFree(name);
}
void Hide(shen_name name) {
    EV_DisableCharacterDisplay(name);
    EV_ToggleCharacterFlag(name);
}
void AuthName(shen_name name) {
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(name.name));
}
void Package(std::int32_t mode, const char *name, std::int32_t category = 0) {
    EV_ControlPackageResources(category, mode, reinterpret_cast<std::uintptr_t>(name), 0);
}
void LoadAuthor(const char *name) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(name));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
void ResetCharacter(shen_name name) {
    auto *work = MotionFor(name);
    EV_mtwkSetEnabled(work, 1);
    mtwkSetExtraFlag(work, 0, 0);
    SetMotionRegionFlag(work, 66, 0, MotionRegion::LowerBody);
    EV_SetCharacterAlphaEnabled(name, false);
    mtwkSetRequestedRootMode(work, 0);
}
void RetireInteractions() {
    if (GetShouldRenderIcons(RenderIconSlot::ItemSelect))
        RefreshPadControlData(1, 1);
    if (IsItemSelectable())
        RequestMemoClose();
    if (srchHasActiveLockonTarget())
        srchReset();
    if (CheckTextureData() == 1)
        RefreshPadControlData(7, 1);
}
float DistanceSquared(const Vector3 &position) {
    const float x = position.x - playerPosition.x, z = position.z - playerPosition.z;
    return x * x + z * z;
}
void FaceTarget(shen_name name, const Vector3 &target) {
    Vector3 position{};
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{0, FixedAtan2(position.x - target.x, position.z - target.z), 0});
}
bool UpdatePace(ChaseContext &context) {
    const float distance = DistanceSquared(context.position);
    const bool touching = distance < 4, near = distance<49, far = distance> 900;
    if (context.character.name == clj.name)
        SetMotionSpeed(MotionFor(clj), touching ? 1 : near ? context.nearSpeed : context.normalSpeed, MotionRegion::LowerBody);
    else {
        if (!touching)
            if (auto *route = FindNpcRouteStateContext(context.character))
                SetNpcRouteMotionState(*route, near ? 1 : !far, false);
        SetNpcScriptRouteSpeed(nullptr, context.character, context.normalSpeed);
    }
    return touching;
}
void UpdateRouteCapture(ChaseContext &context) {
    if (UpdatePace(context) && context.character.name == lly.name) {
        bool ready = false;
        if (!ryoNotBusy()) {
            if (GetShouldRenderIcons(RenderIconSlot::ItemSelect)) {
                RefreshPadControlData(1, 1);
                ready = true;
            } else if (IsItemSelectable()) {
                RequestMemoClose();
                ready = true;
            } else if (srchHasActiveLockonTarget()) {
                srchReset();
                ready = true;
            }
        } else
            ready = true;
        if (CheckTextureData()) {
            RefreshPadControlData(7, 1);
            ready = true;
        }
        Vector3i rotation{};
        GetScenarioCharacterEulerAngles(context.character, &rotation, EventTransformFlags::World);
        if (ready) {
            ReleaseNpcRouteControl(nullptr, context.character, rotation.y);
            RetireInteractions();
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(2);
            context.state = Capture;
            Film(15, 0, 255);
        }
    } else if (!IsNpcRouteActive(context.character)) {
        context.state = Goal;
        context.phase = 0;
    }
}
void AdmitRoute(ChaseContext &context, const NpcRouteNode &route, const Vector3 &origin, std::int32_t yaw, bool clearSecond) {
    FaceTarget(context.character, p9cc5c0);
    BindNpcScriptRoute(context.character, &route, &origin, yaw);
    if (auto *live = FindNpcRouteStateContext(context.character))
        SetNpcRouteMotionState(*live, 1, false);
    if (clearSecond)
        SetNpcScriptRouteFlags(nullptr, context.character, 0, 2);
    SetNpcScriptRouteFlags(nullptr, context.character, 0, 4);
    context.state = Run;
}
void ReadTransform(ChaseContext &context) {
    EV_GetCharaPosition(context.character, CHARACTER_TRANSLATION_ROOT, &context.position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(context.character, &context.rotation, EventTransformFlags::World);
}
void SamExtra() {
    auto *work = MotionFor(sam);
    switch (samChase.phase) {
    case 0:
        EV_SetCharaControllerFlags(sam, 2, NpcCharacterControllerState::Disabled);
        Place(sam, p9cc590, p9cc260);
        Play(sam, 0x11c6);
        SetMotionLayerLoopStartTime(work, 30, MotionRegion::LowerBody);
        SetMotionLayerEndFrame(work, 377, MotionRegion::LowerBody);
        SetMotionSpeed(work, 1, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(work, 0);
        samChase.phase = 2;
        break;
    case 2:
        if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody)) {
            Place(sam, p9cc5a0, p9cc9c0);
            mtwkSetRequestedRootMode(work, 1);
            SetMotionSpeed(work, 1.5f, MotionRegion::LowerBody);
            CopyMotionTransition(work, &p9cc270, 49152, 0, 0);
            SetMotionTimeRelated(work, 1, 5, 4549);
            samChase.phase = 3;
        } else if (GetMotionFrame(work, MotionRegion::LowerBody) > 240 && ComputeAngle(Vector3{233.3f, 98.33f, 465}, 3, 0, 0x8000, playerPosition)) {
            RetireInteractions();
            SetPlayerControlEnabled(0);
            Film(15, 0, 255);
            samChase.state = Capture;
            samChase.phase = 0;
        }
        break;
    case 3:
        if (!mtwkSetExtraFlag(work, 58, 1)) {
            Play(sam, 0x11c5);
            mtwkSetRequestedRootMode(work, 1);
            SetMotionSpeed(work, 1, MotionRegion::LowerBody);
            SetMotionRegionFlag(work, 69, 0, MotionRegion::LowerBody);
            samChase.phase = 4;
        }
        break;
    case 4:
        if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody)) {
            samChase.phase = 0;
            samChase.state = Sleep;
        }
        break;
    default:
        break;
    }
}
void LlyExtra() {
    auto *work = MotionFor(lly);
    switch (llyChase.phase) {
    case 0:
        CopyMotionTransition(work, &p9cc928, 17294, 0, 0);
        SetMotionTimeRelated(work, 1, 5, 4545);
        mtwkSetRequestedRootMode(work, 1);
        SetMotionSpeed(work, 1.5f, MotionRegion::LowerBody);
        Controllers(lly, false);
        llyChase.phase = 1;
        break;
    case 1:
        if (!mtwkSetExtraFlag(work, 58, 1)) {
            mtwkSetRequestedRootMode(work, 1);
            SetMotionSpeed(work, 1, MotionRegion::LowerBody);
            llyChase.phase = 2;
        }
        break;
    case 2:
        if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody)) {
            llyChase.phase = 0;
            llyChase.state = Sleep;
        }
        break;
    default:
        break;
    }
}
void CljExtra() {
    auto *work = MotionFor(clj);
    switch (cljChase.phase) {
    case 0:
        Controllers(clj, false);
        Place(clj, p9cc280, p9cc9a0);
        Play(clj, 0x11cc);
        cljChase.phase = 2;
        break;
    case 1:
        if (!mtwkSetExtraFlag(work, 58, 1)) {
            mtwkSetRequestedRootMode(work, 1);
            cljChase.phase = 2;
        }
        break;
    case 2:
        if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody)) {
            cljChase.phase = 0;
            cljChase.state = Sleep;
        }
        break;
    default:
        break;
    }
}
struct MessageWork {
    const char *text;
    std::int32_t frames;
};
void WaitMessage(void *data) {
    auto &work = *static_cast<MessageWork *>(data);
    if (work.frames-- == 1) {
        ctrlStopSubtitles();
        HLib::DestroyCurrentTask();
    }
}
void PrintMessage(void *data) {
    dbgPrintEventMessage(static_cast<MessageWork *>(data)->text);
    HLib::SetCurrentTaskTo(WaitMessage);
}
void UpdateRen(void *) {
    auto &context = renChase;
    if (!DoesScenarioCharacterExist(ren) || EV_GetShenmueFlag(0x375) || EV_GetShenmueFlag(0x377)) {
        HLib::DestroyCurrentTask();
        return;
    }
    ReadTransform(context);
    switch (context.state) {
    case Init:
        ResetCharacter(ren);
        context.pointCount = 17;
        context.cursor = 0;
        context.state = prelude ? Start : Idle;
        break;
    case Start:
        AdmitRoute(context, renRoute.node, p9cc5b0, -1, true);
        break;
    case Run:
        UpdateRouteCapture(context);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit30, reinterpret_cast<void *>(1));
        break;
    case Capture: {
        auto *owner = HLib::EnqueueTaskWithParameter(PrintMessage, 6, 11, sizeof(MessageWork), SHEN_NAME(EVNT));
        auto *work = static_cast<MessageWork *>(HLib::GetTaskParameter(owner));
        work->text = "あばよ！";
        work->frames = 60;
        if (auto *route = FindNpcRouteStateContext(ren))
            SetNpcRouteMotionState(*route, 1, false);
        SetPlayerControlEnabled(1);
        context.state = Run;
        context.phase = 0;
        break;
    }
    case Exit:
        if (!IsNpcRouteActive(ren))
            ReleaseNpcRouteControl(nullptr, ren, -1);
        context.state = Idle;
        break;
    case Goal:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit30, reinterpret_cast<void *>(1));
        Hide(ren);
        context.state = Finish;
        break;
    case Finish:
        if (renGrace != 0) {
            --renGrace;
            SetPlayerGameFlags(PlayerGameFlagMode::Bit30, reinterpret_cast<void *>(1));
            break;
        }
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1425, 1);
        [[fallthrough]];
    case Idle:
        RetireNpc(ren);
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void StartBattle(ChaseContext &context) {
    if (filmCheckFadeTrackColour())
        return;
    shenToggleAreaFlags(0);
    Play(ryo, 0xf003);
    shenLoadEventFrom("4BTL.EV3", EventFunctionType::Event3);
    ExecuteAreaMainFunction("4BTL.EV3", static_cast<std::uintptr_t>(context.phase));
    context.state = Idle;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
}
void BeginCapture(ChaseContext &context, bool transition) {
    RetireInteractions();
    SetPlayerControlEnabled(0);
    shenToggleAreaFlags(2);
    if (transition)
        CopyMotionTransition(MotionFor(context.character), nullptr, 0, 1, 0);
    subState = 0;
    context.state = Capture;
    Film(15, 0, 255);
}
void UpdateSam(void *) {
    auto &context = samChase;
    if (!DoesScenarioCharacterExist(sam) || EV_GetShenmueFlag(0x377)) {
        HLib::DestroyCurrentTask();
        return;
    }
    ReadTransform(context);
    switch (context.state) {
    case Init:
        ResetCharacter(sam);
        context.pointCount = 6;
        context.cursor = 0;
        if (prelude) {
            context.state = Start;
            context.phase = 0;
        } else if (EV_GetShenmueFlag(0x81))
            context.state = Sleep;
        else {
            RetireNpc(sam);
            HLib::DestroyCurrentTask();
            context.state = Idle;
        }
        break;
    case Start:
        AdmitRoute(context, samRoute.node, Vector3{233.3000030517578f, 101.56999969482422f, 457.6499938964844f}, 0x8000, true);
        break;
    case Run:
        UpdateRouteCapture(context);
        break;
    case Goal:
        context.state = Fence;
        context.phase = 0;
        break;
    case Sleep:
        Controllers(sam, false);
        Place(sam, p9cc990, p9cc428);
        Play(sam, 0x11c7);
        context.state = Finish;
        break;
    case Finish:
        if (ryoNotBusy() && DistanceSquared(context.position) < 4) {
            BeginCapture(context, false);
            context.phase = 1;
        }
        break;
    case Battle:
        subState = 0;
        RetireInteractions();
        context.phase = 1;
        context.state = Capture;
        Film(15, 0, 255);
        break;
    case Fence:
        if (ryoNotBusy() && DistanceSquared(context.position) < 4 && context.phase > 2) {
            BeginCapture(context, true);
            Vector3 position = context.position;
            position.y = 98.32f;
            EV_AT_SetPos(sam, EventTransformFlags::AllAxes, position);
            context.phase = 0;
        } else
            SamExtra();
        break;
    case Capture:
        StartBattle(context);
        break;
    case Exit:
        if (!IsNpcRouteActive(sam))
            ReleaseNpcRouteControl(nullptr, sam, -1);
        CopyMotionTransition(MotionFor(sam), nullptr, 0, 13, 0);
        context.state = Exit2;
        break;
    case Exit2:
        RetireNpc(sam);
        HLib::DestroyCurrentTask();
        break;
    case Idle:
        EV_stepSetFlags(0, 0x2be);
        subState = 0;
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void UpdateLly(void *) {
    auto &context = llyChase;
    if (!DoesScenarioCharacterExist(lly) || EV_GetShenmueFlag(0x375)) {
        HLib::DestroyCurrentTask();
        return;
    }
    ReadTransform(context);
    switch (context.state) {
    case Init:
        ResetCharacter(lly);
        context.pointCount = 14;
        context.cursor = 0;
        if (prelude) {
            context.state = Start;
            context.phase = 2;
        } else if (EV_GetShenmueFlag(0x82))
            context.state = Sleep;
        else {
            RetireNpc(lly);
            HLib::DestroyCurrentTask();
            context.state = Idle;
        }
        break;
    case Start:
        AdmitRoute(context, llyRoute.node, p9cc790, 0x8000, false);
        break;
    case Run:
        UpdateRouteCapture(context);
        break;
    case Sleep:
        Controllers(lly, false);
        Place(lly, p9cc2d8, p9cc178);
        Play(lly, 0x11c2);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
        context.state = Finish;
        break;
    case Finish:
        if (ryoNotBusy() && DistanceSquared(context.position) < 4) {
            BeginCapture(context, false);
            context.phase = 3;
        }
        break;
    case Goal:
        if (ryoNotBusy() && DistanceSquared(context.position) < 4) {
            BeginCapture(context, true);
            if (context.phase == 1 && GetMotionFrame(MotionFor(lly), MotionRegion::LowerBody) > 220)
                context.phase = 3;
            else
                context.phase = context.phase == 2 ? 3 : 2;
        } else
            LlyExtra();
        break;
    case Capture:
        StartBattle(context);
        break;
    case Exit:
        if (!IsNpcRouteActive(lly))
            ReleaseNpcRouteControl(nullptr, lly, -1);
        CopyMotionTransition(MotionFor(lly), nullptr, 0, 13, 0);
        context.state = Exit2;
        break;
    case Exit2:
        RetireNpc(lly);
        HLib::DestroyCurrentTask();
        break;
    case Idle:
        subState = Run;
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void SetupCoolJAuthor() {
    shenToggleAreaFlags(0);
    Play(ryo, 0xf003);
    Play(clj, 0xf003);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EV_LoadCutsceneFromCurrentScene("0062A000");
    EV_LoadAudio("0062A000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0062");
    LoadAuthor("0062A");
    SetLowQualityHands(FindCharacterByScenarioName(clj));
    Controllers(ryo, false);
    Controllers(clj, false);
    EV_SetCharacterAlphaEnabled(clj, false);
}
void UpdateCoolJAuthor(void *) {
    switch (cljChase.phase) {
    case 0:
        if (!filmCheckFadeTrackColour()) {
            Film(30, 255, 0);
            SetupCoolJAuthor();
            EV_EnableCharacterDisplay(a014);
            AuthName(SHEN_NAME(COLJ));
            cljChase.phase = 4;
        }
        break;
    case 1:
        if (!filmCheckFadeTrackColour()) {
            SetupCoolJAuthor();
            EV_DisableCharacterDisplay(ryo);
            Package(4, "CHASE4", 2);
            EVS_LoopCamera(4, dummy, dummy);
            Place(clj, p9cc7a0, p9cc9b0);
            Play(clj, 0x11ca);
            cljChase.phase = 2;
            Film(30, 255, 0);
            HLib::SetupTaskDelay(30);
        }
        break;
    case 2:
        EV_PlayDialogue("0062B016");
        HLib::SetupTaskDelay(50);
        cljChase.phase = 3;
        break;
    case 3:
        EV_EnableCharacterDisplay(ryo);
        EV_EnableCharacterDisplay(a014);
        AuthName(SHEN_NAME(COLJ));
        cljChase.phase = 4;
        break;
    case 4:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            Place(ryo, p9cc138, p9cc688);
            Play(ryo, 0xf003);
            Play(clj, 0xf003);
            camCtrlSetState(CameraState::Init);
            SetCharaKill(dummy, 1);
            Package(6, "0062A");
            EV_FreeAuthorPak(0, "0062");
            EV_DisableCharacterDisplay(a014);
            EV_DisableCharacterDisplay(clj);
            EV_NPC_NewLoadSw(1);
            Film(30, 255, 0);
            cljChase.phase = 5;
        }
        break;
    case 5:
        EV_stepSetFlags(0, 0x83);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(AR02) | 0x1000000U, 0);
        break;
    default:
        break;
    }
}
void SteerClj() {
    auto &context = cljChase;
    const Vector3 &target = cljPoints[context.cursor];
    const float x = target.x - context.position.x, z = target.z - context.position.z;
    if (x * x + z * z < 9) {
        if (context.cursor == context.pointCount - 1) {
            context.state = Goal;
            return;
        }
        ++context.cursor;
    }
    const auto desired = FixedAtan2(-x, -z);
    const auto difference = static_cast<std::uint16_t>(context.rotation.y - desired);
    Vector3i rotation{0, desired, 0};
    if (static_cast<std::uint32_t>(difference - 910) <= 0xf8e3)
        rotation.y = context.rotation.y + (difference > 0x8000 ? 546 : 64990);
    EV_AT_SetAngY(clj, EventTransformFlags::AllAxes, rotation);
}
void UpdateClj(void *) {
    auto &context = cljChase;
    if (!DoesScenarioCharacterExist(clj)) {
        HLib::DestroyCurrentTask();
        return;
    }
    ReadTransform(context);
    switch (context.state) {
    case Init:
        ResetCharacter(clj);
        context.pointCount = 7;
        context.cursor = 1;
        escaped = 0;
        context.state = prelude ? Start : EV_GetShenmueFlag(0x83) ? Sleep : Idle;
        break;
    case Start:
        FaceTarget(clj, cljPoints[context.cursor]);
        SetMotionRegionFlag(MotionFor(clj), 66, 0, MotionRegion::LowerBody);
        Play(clj, 0x11c9);
        SetMotionLayerEndFrame(MotionFor(clj), 15, MotionRegion::LowerBody);
        context.state = Run;
        break;
    case Run:
        UpdatePace(context);
        SetMotionRegionFlag(MotionFor(clj), 66, 0, MotionRegion::LowerBody);
        SteerClj();
        break;
    case Goal:
        if (ryoNotBusy() && DistanceSquared(context.position) < 4) {
            RetireInteractions();
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(2);
            CopyMotionTransition(MotionFor(clj), nullptr, 0, 1, 0);
            subState = 0;
            context.state = Capture;
        } else
            CljExtra();
        break;
    case Sleep:
        Controllers(clj, false);
        Place(clj, p9cc7a0, p9cc9b0);
        Play(clj, 0x11cb);
        context.state = Finish;
        escaped = 1;
        break;
    case Finish:
        if (ryoNotBusy()) {
            Vector3 position{};
            EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::TurnMotion);
            if (ComputeAngle(p9cc698, 5, 0, 0x8000, position)) {
                RetireInteractions();
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(2);
                subState = 0;
                context.state = Capture;
            }
        }
        break;
    case Capture:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
        mtwkSetRequestedRootMode(MotionFor(ryo), 1);
        Play(ryo, 0xf003);
        Film(30, 0, 255);
        spmFadeOut();
        CreateEventCharacter(CharacterCreationHighModel, nullptr, dummy);
        EV_AT_SetPos(dummy, EventTransformFlags::World | EventTransformFlags::AllAxes, p9cc580);
        EV_AT_SetAngY(dummy, EventTransformFlags::AllAxes, Vector3i{-1, -1, -1});
        HLib::SetCurrentTaskTo(UpdateCoolJAuthor);
        context.state = Idle;
        subState = 0;
        context.phase = escaped ? 1 : 0;
        break;
    case Exit:
        if (!IsNpcRouteActive(clj))
            ReleaseNpcRouteControl(nullptr, clj, -1);
        context.state = Idle;
        break;
    case Idle:
        subState = 0;
        RetireNpc(clj);
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void SetupChasePositions() {
    Place(ren, {202, 98.87f, 420}, {0, 43691, 0});
    Place(sam, {203, 98.87f, 423}, {0, 45512, 0});
    Place(lly, {201.5f, 98.87f, 415}, {0, 43691, 0});
    Place(clj, {210, 99.29f, 415}, {0, 56543, 0});
    for (shen_name name : npcNames) {
        if (name.name == 0xffffffffU)
            break;
        Play(name, 0xf006);
        Controllers(name, true);
    }
}
void StartChase() {
    SetupChasePositions();
    HLib::EnqueueTaskWithoutParameter(UpdateRen, 4, 11, static_cast<std::int32_t>(SHEN_NAME(tren)));
    renChase.character = ren;
    renChase.normalSpeed = 1.18f;
    renChase.nearSpeed = 1.2f;
    renChase.state = Init;
    renChase.phase = 0;
    HLib::EnqueueTaskWithoutParameter(UpdateSam, 4, 11, static_cast<std::int32_t>(SHEN_NAME(tsam)));
    samChase.character = sam;
    samChase.normalSpeed = 1;
    samChase.nearSpeed = 1.1f;
    samChase.state = Init;
    samChase.phase = 0;
    HLib::EnqueueTaskWithoutParameter(UpdateLly, 4, 11, static_cast<std::int32_t>(SHEN_NAME(tlly)));
    llyChase.character = lly;
    llyChase.normalSpeed = .83f;
    llyChase.nearSpeed = 1;
    llyChase.state = Init;
    llyChase.phase = 0;
    HLib::EnqueueTaskWithoutParameter(UpdateClj, 4, 11, static_cast<std::int32_t>(SHEN_NAME(tclj)));
    cljChase.character = clj;
    cljChase.normalSpeed = 1.15f;
    cljChase.nearSpeed = 1.25f;
    cljChase.state = Init;
    cljChase.phase = 0;
    SetPlayerControlEnabled(1);
    camCtrlSetState(CameraState::Init);
    camCtrlSetFovTransitionTo(true);
    EV_NPC_NewLoadSw(1);
}
void StartAuthors() {
    if (authorFlag == 0) {
        SetPlayerControlEnabled(0);
        Controllers(ryo, false);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        EV_FreeAuthorPak(1, "0061");
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        AuthName(SHEN_NAME(SAKU));
        authorFlag = 4;
        samChase.state = Sleep;
    } else if (authorFlag == 4 && !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EV_FreeAuthorPak(0, "0061");
        subState = 0;
        authorFlag = 0;
        Controllers(sam, false);
        Place(sam, p9cc990, p9cc428);
        Play(sam, 0x11c7);
        samChase.state = Battle;
    }
}
void NightAuthorModels(std::int32_t frame, bool second) {
    if (!EV_GetShenmueFlag(0xff))
        return;
    for (shen_name name : allNames) {
        auto *character = FindCharacterByScenarioName(name);
        SetPluralControlFor(character, 1);
        SetScenarioCharacterType(name, CharacterTypeDefault);
        SetHighModelXYZ(character, 0, 0, 0);
        if (name.name != ryo.name || !second || frame < 1297)
            SetHighModelXYZ(character, 0, 1, 0);
    }
}
void UpdateMain(void *) {
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (GetCurrentAuthorId().name == SHEN_NAME(PRE2)) {
        if (frame == 458)
            snapSetDesc(1);
        else if (frame == 480)
            snapSetDesc(0);
    }
    if (EV_GetShenmueFlag(0x375) || EV_GetShenmueFlag(0x377)) {
        mainState = Exit;
        const shen_name other = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 306) ? lly : sam;
        RetireNpc(ren);
        RetireNpc(other);
        RetireNpc(clj);
    }
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::World);
    switch (mainState) {
    case Init:
        for (shen_name name : npcNames) {
            if (name.name == 0xffffffffU)
                break;
            EV_npcUnknownFunc(name);
        }
        subState = 0;
        if (EV_GetShenmueFlag(0x7f)) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1425, 0);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 1);
            npcResetDateTimeIfNeeded();
            Play(ryo, 0xf003);
            LoadAuthor("PREAUTH");
            for (shen_name name : npcNames) {
                if (name.name == 0xffffffffU)
                    break;
                SetLowQualityHands(FindCharacterByScenarioName(name));
            }
            EV_FreeAuthorPak(1, "0061");
            EV_LoadCutsceneFromCurrentScene("0061A000");
            camCtrlCacheUtil(4);
            AuthName(SHEN_NAME(PRE1));
            for (shen_name name : allNames)
                SetPluralControlFor(FindCharacterByScenarioName(name), 1);
            mainState = Auth;
            prelude = 1;
        } else {
            SetPlayerControlEnabled(1);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1425, 1);
            StartChase();
            mainState = Run;
            subState = Run;
            prelude = 0;
        }
        break;
    case Auth:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            switch (frame) {
            case 151:
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 1);
                break;
            case 350:
                camCtrlSetFarDistance(100);
                break;
            case 390:
                EV_DisableCharacterDisplay(lly);
                break;
            case 405:
                camCtrlSetFarDistance(80);
                break;
            case 410:
                EV_DisableCharacterDisplay(sam);
                break;
            default:
                break;
            }
            NightAuthorModels(frame, false);
        } else {
            AuthName(SHEN_NAME(PRE2));
            camCtrlSetFarDistance(150);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 1);
            mainState = Start;
        }
        break;
    case Start:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            Hide(a014);
            EV_FreeAuthorPak(0, "0061");
            Package(6, "PREAUTH");
            for (shen_name name : npcNames) {
                if (name.name == 0xffffffffU)
                    break;
                hndlSetFlags(FindCharacterByScenarioName(name));
            }
            for (shen_name name : allNames)
                SetPluralControlFor(FindCharacterByScenarioName(name), 0);
            EV_stepSetFlags(0, 0x7f);
            DateTime time{};
            GetCurrentDateTime(time);
            npcAdvanceTime(time.hour, time.minute);
            Place(ryo, {195.4f, 98.87f, 414.8f}, {0, 43690, 0});
            StartChase();
            EV_LoadAudio("0061A000", 1);
            EV_spmBeginSampleExecute();
            mainState = Run;
            subState = Run;
            camCtrlCacheUtil(5);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 0);
            Film(0, 0, 255);
            Film(30, 255, 0);
        } else {
            switch (frame) {
            case 406:
                camCtrlSetFarDistance(60);
                break;
            case 776:
            case 1273:
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 0);
                break;
            case 890:
                camCtrlSetFarDistance(95);
                break;
            case 1045:
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 1);
                break;
            default:
                break;
            }
            NightAuthorModels(frame, true);
        }
        break;
    case Run:
        if (subState == Run) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 305) && !EV_GetShenmueFlag(0x2be)) {
                subState = Fence;
                authorFlag = 0;
                StartAuthors();
            }
            if (playerPosition.x > 245.5f && playerPosition.x < 249.5f && playerPosition.z > 444.5f && playerPosition.z < 448)
                renChase.normalSpeed = renChase.nearSpeed = .7f;
            if (playerPosition.x > 257 && playerPosition.x < 261 && playerPosition.z > 431 && playerPosition.z < 450)
                renChase.normalSpeed = renChase.nearSpeed = 1.3f;
        } else if (subState == Fence)
            StartAuthors();
        break;
    case Exit:
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
        Package(6, "CHASE4");
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void Initialize4Nin() {
    mainState = subState = 0;
    authorFlag = renGrace = prelude = escaped = 0;
    renChase = {};
    samChase = {};
    llyChase = {};
    cljChase = {};
    playerPosition = {};
}
void ChaseModuleNoOp() {
}
void Start4Nin(std::uintptr_t) {
    ChaseModuleNoOp();
    mainState = Init;
    subState = 0;
    authorFlag = 0;
    renGrace = 90;
    SetPlayerControlEnabled(0);
    Film(0, 255, 255);
    FinishLoading();
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{});
    if (g_currentMap.name == SHEN_NAME(4NIN)) {
        EV_SetObjectFlagByIndex(14, 0);
        SetWeatherUpdateEnabledTo(1);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
    }
    auto *owner = HLib::EnqueueTaskWithoutParameter(UpdateMain, 6, 11, static_cast<std::int32_t>(SHEN_NAME(4nin)));
    SetGenericNpcLowQualityHands(false);
    Queue_EVT_UTL_LoadNPCChara(npcNames, owner);
}
} // namespace
const ScriptAreaDefinition area4NinScriptDefinition{.areaId = SHEN_NAME(4NIN), .field04 = 0, .init = Initialize4Nin, .main = Start4Nin};
} // namespace shenmue::s2::areas
