#include "s1/task/wzog.hpp"
#include "s1/boot.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/input.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/player_stats.hpp"
#include "s1/render_array.hpp"
#include "s1/render_passes.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/vfma.hpp"
#include "s1/ui_runtime.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

extern std::uint32_t g_searchLockOnFlags;
extern std::int32_t g_searchAnyInputPressedLatched;
extern std::int32_t g_searchAPressedLatched;
extern std::int32_t g_busy_reset;

namespace {

constexpr std::int32_t makeToken(char a, char b, char c, char d) noexcept {
    return static_cast<std::int32_t>(static_cast<std::uint8_t>(a)) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(b)) << 8) |
        (static_cast<std::int32_t>(static_cast<std::uint8_t>(c)) << 16) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(d)) << 24);
}

constexpr std::int32_t kPracticeHudElement = 5;
constexpr std::int32_t kGameBackgroundElement = 7;
constexpr std::int32_t kMovesElement = 16;
constexpr std::int32_t kYesNoPromptElement = 56;
constexpr std::int32_t kInformationBarElement = 57;

constexpr std::uint16_t kTransitionFlagMovesView = 0x10U;
constexpr std::uint16_t kRefreshActive = 1;
constexpr std::uint16_t kRefreshCloseRequested = 2;
constexpr std::uint16_t kRefreshClosing = 3;
constexpr std::int32_t kAuthorStateScripted = 9;
constexpr std::int32_t kAuthorStateCombat = 10;

constexpr std::array<std::uint8_t, 5> kAnimSetTypes{0, 0, 32, 64, 80};



Vector3 g_movesScreenSavedEye{};
Vector3 g_movesScreenSavedTarget{};
std::int32_t g_movesScreenRequestedAuthorState = 0;
Character *CurrentRyo() {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

bool DestroyElementForReset(UIBaseRuntime &element) {
    if (element.isVisible && element.destroy && element.destroy() != 1 && g_busy_reset != 1) {
        return false;
    }
    element.isVisible = 0;
    return true;
}

void ResetAuthToNone() {
    const std::int32_t currentState = g_currUIState;
    const std::uint32_t state = static_cast<std::uint32_t>(currentState);
    if (((state + 1U) & ~8U) == 0) {
        UIBaseRuntime &background = g_uiElements[static_cast<std::size_t>(kGameBackgroundElement)];
        if (background.isVisible && background.destroy && background.destroy() != 1 && g_busy_reset != 1) {


        } else {
            background.isVisible = 0;
        }
    }

    UIBaseRuntime &moves = g_uiElements[static_cast<std::size_t>(kMovesElement)];
    if ((currentState == -1 || currentState == kMovesElement) && DestroyElementForReset(moves)) {
        moves.isVisible = 0;
    }

    UIBaseRuntime &yesNo = g_uiElements[static_cast<std::size_t>(kYesNoPromptElement)];
    DestroyElementForReset(yesNo);

    UIBaseRuntime &informationBar = g_uiElements[static_cast<std::size_t>(kInformationBarElement)];
    DestroyElementForReset(informationBar);

    ResetSearchToFree();
    g_previousAuthorState = kAuthorStateCombat;
    ApplyPendingGameState(kAuthorStateCombat);
    if (g_pMovesScreenTask) {
        HLib_DestroyTask(g_pMovesScreenTask);
        g_pMovesScreenTask = nullptr;
    }
}

std::int64_t HandleMovesScreenUi();

std::int64_t HandleMovesScreenUi() {
    UIBaseRuntime &moves = g_uiElements[static_cast<std::size_t>(kMovesElement)];

    if (!moves.isVisible) {
        ExitSearchAndMoveCameraRelative(1.6f, 0.17f, static_cast<std::uint16_t>(g_fCamAccumYaw));

        const std::int32_t currentState = g_currUIState;
        if (currentState == -1 || currentState == kPracticeHudElement) {
            UIBaseRuntime &practice = g_uiElements[static_cast<std::size_t>(kPracticeHudElement)];
            if (practice.isVisible && practice.destroy && practice.destroy() != 1 && g_busy_reset != 1) {


            } else {
                practice.isVisible = 0;
            }
        }

        const std::uint32_t state = static_cast<std::uint32_t>(currentState);
        if (((state + 1U) & ~8U) == 0) {
            UIBaseRuntime &background = g_uiElements[static_cast<std::size_t>(kGameBackgroundElement)];
            if (background.isVisible) {
                background.isHandledExternally = 0;
            } else {
                std::int64_t (*callback)() = nullptr;
                if (background.render) {
                    background.render();
                    callback = background.tick;
                } else if (background.initialize) {
                    background.initialize();
                    callback = background.postInit;
                }
                if (callback) {
                    g_waitingForNextTick = 0;
                    (void)callback();
                    g_waitingForNextTick = 1;
                }
                background.isVisible = 1;
                background.isHandledExternally = 0;
            }
        }

        d3t::ActivateElementByIdx(kMovesElement);
        d3t::ActivateElementByIdx(kInformationBarElement);
    }

    std::int64_t result = g_movesScreenRefreshState;
    const std::uint16_t refreshState = g_movesScreenRefreshState;
    if (refreshState != kRefreshActive) {
        if (refreshState == kRefreshClosing) {
            if (g_movesScreenViewActive == 1) {
                const std::uint16_t moveResult = static_cast<std::uint16_t>(VFMA_RequestMove(VFMoveRequestFlags::ReturnAbsolute));
                result = (result & ~0xffffLL) | static_cast<std::int64_t>((static_cast<std::uint16_t>(result) & 0x00ffU) | (moveResult & 0xff00U));
                result = (result & ~0xffLL) | 1;
                g_movesScreenTransitionFlags = 0;
                g_pendingMovePopupMoveId = 0;
                g_pendingMovePopupAnimSetId = 0;
                g_movesScreenViewActive = 0;
                g_movesScreenRefreshState = 0;
                return result;
            }
        } else if (static_cast<std::int16_t>(refreshState) < static_cast<std::int16_t>(kRefreshClosing)) {
            // Continue through the transition gate below.
            goto transition_gate;
        }
        g_movesScreenRefreshState = 0;
        return (result & ~0xffLL) | 1;
    }

    g_movesScreenTransitionOffset = -50.0f;
transition_gate:
    if ((g_movesScreenTransitionFlags & 0xf0U) == 0) {
        BeginMovesScreenViewTransition();
        result = 1;
        g_movesScreenTransitionFlags = kTransitionFlagMovesView;
        g_movesScreenRefreshState = kRefreshActive;
    }
    if (g_movesScreenTransitionGate) {
        g_movesScreenTransitionGate = 0;
    } else if (result == kRefreshCloseRequested) {
        result = kRefreshClosing;
        g_movesScreenRefreshState = kRefreshClosing;
    }
    return result & ~0xffLL;
}

} // namespace


void ResetSearchToFree() {
    Character *const ryo = CurrentRyo();
    if (ryo) {
        const bool wasVisible = (ryo->motionFlags & 4U) != 0;
        ryo->motionFlags = static_cast<std::uint8_t>(ryo->motionFlags | 4U);
        if (g_bDebugLogVisibility && (ryo->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
            CHAR_LogNPCVisibility(ryo, true);
    }
    const std::int32_t requestedState = g_movesScreenRequestedAuthorState;
    g_bIsConversationCamInactive = 0;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = requestedState;
    ApplyPendingGameState(requestedState);
    TranslateCameraRelative(g_movesScreenSavedEye);
    if (!bLockCamera) {
        g_CameraTargetPos = g_movesScreenSavedTarget;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
}


void ExitSearchAndMoveCameraRelative(float eyeHeight, float targetDistance, std::uint16_t yaw) {
    g_movesScreenSavedEye = g_CameraEyePos;
    g_movesScreenRequestedAuthorState = g_nAuthorState;
    g_movesScreenSavedTarget = g_CameraTargetPos;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = kAuthorStateScripted;
    ApplyPendingGameState(kAuthorStateScripted);

    Character *const ryo = CurrentRyo();
    if (ryo) {
        const bool wasVisible = (ryo->motionFlags & 4U) != 0;
        ryo->motionFlags = static_cast<std::uint8_t>(ryo->motionFlags & ~4U);
        if (g_bDebugLogVisibility && (ryo->flags.value & CharacterFlagHasCpct) != 0 && wasVisible)
            CHAR_LogNPCVisibility(ryo, false);
    }

    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    Vector3 relativeEye = ryoPosition;
    relativeEye.y += eyeHeight;
    TranslateCameraRelative(relativeEye);

    const float yawSine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const std::int16_t foldedYaw = yaw <= 0x8000U ? static_cast<std::int16_t>(yaw) : static_cast<std::int16_t>(-static_cast<std::int32_t>(yaw));
    const Vector3 target{
        ryoPosition.x + yawSine * targetDistance, relativeEye.y, ryoPosition.z - sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedYaw)) * targetDistance};
    if (bLockCamera)
        return;
    g_CameraTargetPos = target;
    shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
}

const std::array<MoveListRecord, 19> g_handMoveList{{{"UIMOVETGKN", 0}, {"UIMOVEELSL", 1}, {"UIMOVETWKN", 2}, {"UIMOVEELAS", 3}, {"UIMOVEUPKN", 4}, // @text UIMOVETGKN EN: "Tiger Knuckle" | JA: "虎菱" ; UIMOVEELSL EN: "Elbow Slam" | JA: "小車打ち" ; UIMOVETWKN EN: "Twist Knuckle" | JA: "捻り菱" ; UIMOVEELAS EN: "Elbow Assault" | JA: "裡門頂肘" ; UIMOVEUPKN EN: "Upper Knuckle" | JA: "昇り菱"
    {"UIMOVESLST", 5}, {"UIMOVERATH", 6}, {"UIMOVEBIWH", 7}, {"UIMOVETWHA", 8}, {"UIMOVEBAWI", 9}, {"UIMOVEAVLA", 10}, {"UIMOVEKAMI", 11}, // @text UIMOVESLST EN: "Sleeve Strike" | JA: "袖突き" ; UIMOVERATH EN: "Rain Thrust" | JA: "村雨突き" ; UIMOVEBIWH EN: "Big Wheel" | JA: "大車" ; UIMOVETWHA EN: "Twin Hand Waves" | JA: "双手波" ; UIMOVEBAWI EN: "Backfist Willow" | JA: "裏柳" ; UIMOVEAVLA EN: "Avalanche Lance" | JA: "崩山槍" ; UIMOVEKAMI EN: "Katana Mist Slash" | JA: "太刀霞"
    {"UIMOVEMIFL", 12}, {"UIMOVEPITB", 1}, {"UIMOVEDBLB", 8}, {"UIMOVESWAL", 13}, {"UIMOVERISF", 14}, {"UIMOVETWIB", 15}, {"UIMOVESTBA", 16}}}; // @text UIMOVEMIFL EN: "Mistral Flash" | JA: "薙風" ; UIMOVEPITB EN: "Pit Blow" | JA: "水月突き" ; UIMOVEDBLB EN: "Double Blow" | JA: "重ね当て" ; UIMOVESWAL EN: "Swallow Flip" | JA: "燕旋擺柳" ; UIMOVERISF EN: "Rising Flash" | JA: "裂晃閃" ; UIMOVETWIB EN: "Twin Blades" | JA: "双刃" ; UIMOVESTBA EN: "Stab Armor" | JA: "鎧通"
const std::array<MoveListRecord, 18> g_legMoveList{{{"UIMOVECRKI", 0}, {"UIMOVETRKI", 1}, {"UIMOVESIRE", 2}, {"UIMOVEAGCA", 3}, {"UIMOVESUSL", 4}, // @text UIMOVECRKI EN: "Crescent Kick" | JA: "三日月蹴り" ; UIMOVETRKI EN: "Trample Kick" | JA: "踏み蹴り" ; UIMOVESIRE EN: "Side Reaper Kick" | JA: "払い薙" ; UIMOVEAGCA EN: "Against Cascade" | JA: "瀧昇り" ; UIMOVESUSL EN: "Surplice Slash" | JA: "袈裟斬り"
    {"UIMOVETHKI", 5}, {"UIMOVEHOAG", 6}, {"UIMOVEBRTI", 7}, {"UIMOVEDAMO", 8}, {"UIMOVECYKI", 9}, {"UIMOVEWIND", 10}, {"UIMOVESWDI", 2}, // @text UIMOVETHKI EN: "Thunder Kick" | JA: "雷蹴り" ; UIMOVEHOAG EN: "Hold Against Leg" | JA: "出足止め" ; UIMOVEBRTI EN: "Brutal Tiger" | JA: "虎乱" ; UIMOVEDAMO EN: "Dark Moon" | JA: "新月" ; UIMOVECYKI EN: "Cyclone Kick" | JA: "疾風" ; UIMOVEWIND EN: "Windmill" | JA: "風車" ; UIMOVESWDI EN: "Swallow Dive" | JA: "燕旋降脚"
    {"UIMOVETOKI", 11}, {"UIMOVENOSK", 6}, {"UIMOVECRCY", 12}, {"UIMOVEMUDS", 13}, {"UIMOVETWSW", 14}, {"UIMOVESHRE", 15}}}; // @text UIMOVETOKI EN: "Tornado Kick" | JA: "竜巻キック" ; UIMOVENOSK EN: "Nothing Skill" | JA: "未修得" ; UIMOVECRCY EN: "Crawl Cyclone" | JA: "旋風" ; UIMOVEMUDS EN: "Mud Spider" | JA: "土蜘蛛" ; UIMOVETWSW EN: "Twin Swallow Leap" | JA: "飛燕連刈" ; UIMOVESHRE EN: "Shadow Reaper" | JA: "影狩り"
const std::array<MoveListRecord, 14> g_throwMoveList{{{"UIMOVEOVER", 0}, {"UIMOVESWTH", 1}, {"UIMOVEVOTH", 2}, {"UIMOVEMIRE", 3}, {"UIMOVEDEDR", 4}, // @text UIMOVEOVER EN: "Overthrow" | JA: "背負い投げ" ; UIMOVESWTH EN: "Sweep Throw" | JA: "払い腰" ; UIMOVEVOTH EN: "Vortex Throw" | JA: "巴投げ" ; UIMOVEMIRE EN: "Mist Reaper" | JA: "霞刈り" ; UIMOVEDEDR EN: "Demon Drop" | JA: "羅刹落とし"
    {"UIMOVESHBU", 5}, {"UIMOVETEDR", 6}, {"UIMOVEDAHA", 7}, {"UIMOVEBATW", 8}, {"UIMOVESHTE", 9}, {"UIMOVEARBR", 10}, {"UIMOVETIST", 11}, // @text UIMOVESHBU EN: "Shoulder Buster" | JA: "巻き固め" ; UIMOVETEDR EN: "Tengu Drop" | JA: "天狗勝" ; UIMOVEDAHA EN: "Darkside Hazuki" | JA: "裏芭月" ; UIMOVEBATW EN: "Back Twist Drop" | JA: "裏落とし" ; UIMOVESHTE EN: "Shadow Step" | JA: "影身" ; UIMOVEARBR EN: "Arm Break Fire" | JA: "朧無双" ; UIMOVETIST EN: "Tiger Storm" | JA: "虎身崩し"
    {"UIMOVESHBL", 12}, {"UIMOVECRCH", 13}}}; // @text UIMOVESHBL EN: "Shadow Blade" | JA: "影刀" ; UIMOVECRCH EN: "Cross Charge" | JA: "残月"
const std::array<MoveCommandRecord, 17> g_handMoveCommands{{{1, 0, 0, 0, 0, 0, 0}, {2, 2, 0, 0, 0, 0, 0}, {2, 3, 0, 0, 0, 0, 0}, {3, 2, 2, 0, 0, 0, 0},
    {3, 3, 3, 0, 0, 0, 0}, {3, 2, 3, 0, 0, 0, 0}, {3, 3, 2, 0, 0, 0, 0}, {3, 0, 7, 1, 0, 0, 0}, {4, 2, 0, 7, 1, 0, 0}, {4, 3, 0, 7, 1, 0, 0},
    {5, 2, 2, 0, 7, 1, 0}, {5, 3, 3, 0, 7, 1, 0}, {2, 4, 0, 0, 0, 0, 0}, {3, 3, 0, 1, 0, 0, 0}, {4, 2, 3, 3, 0, 0, 0}, {4, 3, 2, 2, 0, 0, 0},
    {6, 2, 3, 3, 0, 7, 1}}};
const std::array<MoveCommandRecord, 16> g_legMoveCommands{{{1, 1, 0, 0, 0, 0, 0}, {2, 2, 1, 0, 0, 0, 0}, {2, 3, 1, 0, 0, 0, 0}, {3, 2, 2, 1, 0, 0, 0},
    {3, 3, 3, 1, 0, 0, 0}, {3, 2, 3, 1, 0, 0, 0}, {3, 3, 2, 1, 0, 0, 0}, {5, 2, 3, 0, 7, 1, 0}, {5, 3, 2, 0, 7, 1, 0}, {2, 4, 1, 0, 0, 0, 0},
    {4, 4, 0, 7, 1, 0, 0}, {4, 2, 2, 1, 1, 0, 0}, {4, 2, 3, 3, 1, 0, 0}, {4, 3, 2, 2, 1, 0, 0}, {6, 3, 2, 2, 0, 7, 1}, {4, 4, 6, 7, 1, 0, 0}}};
const std::array<MoveCommandRecord, 14> g_throwMoveCommands{{{1, 5, 0, 0, 0, 0, 0}, {2, 2, 5, 0, 0, 0, 0}, {2, 3, 5, 0, 0, 0, 0}, {3, 2, 2, 5, 0, 0, 0},
    {3, 3, 3, 5, 0, 0, 0}, {3, 2, 3, 5, 0, 0, 0}, {3, 3, 2, 5, 0, 0, 0}, {1, 5, 0, 0, 0, 0, 0}, {1, 5, 0, 0, 0, 0, 0}, {4, 2, 6, 7, 5, 0, 0},
    {4, 2, 3, 3, 5, 0, 0}, {5, 3, 2, 2, 5, 5, 0}, {5, 2, 6, 7, 5, 0, 0}, {5, 2, 2, 6, 7, 5, 0}}};
const std::array<const char *, 19> g_handMoveNames{"UIWAZHND00", "UIWAZHND01", "UIWAZHND02", "UIWAZHND03", "UIWAZHND04", "UIWAZHND05", "UIWAZHND06", // @text UIWAZHND00 EN: "A punch aimed at a point between the\nnose and mouth." | JA: "鼻と口の間にあるツボ、「人中」を狙う技である。" ; UIWAZHND01 EN: "A downward rotating blow aimed at\nthe opponent's collarbone." | JA: "腕を縦方向の回転運動で打ち下ろし、\n相手の鎖骨を狙う技である。" ; UIWAZHND02 EN: "Unlike the Elbow Slam, it is thrown\nhorizontally in a sweeping hook." | JA: "小車打ちとは異なり、水平方向に振る、\n大振りのいわゆるフックである。" ; UIWAZHND03 EN: "A move targeted at the center of\nthe opponent's body with a rush." | JA: "体当たりに近い感覚で行う肘打ち。\n相手の身体の正中線を狙う。" ; UIWAZHND04 EN: "A strike on the jaw swung by\nmomentarily lowering and rising." | JA: "いったん体を下に沈め、伸び上がりながら\n下から相手の顎を振り打つ技。" ; UIWAZHND05 EN: "A punch at a mid-level target while\ntaking a step forward." | JA: "後ろ足を前に一歩進めて、中段を正拳で\n突く技である。" ; UIWAZHND06 EN: "A straight punch aimed at a pressure\npoint on the opponent's side neck." | JA: "ストレートパンチを相手の頸部に打ち出す技。"
    "UIWAZHND07", "UIWAZHND08", "UIWAZHND09", "UIWAZHND10", "UIWAZHND11", "UIWAZHND12", "UIWAZHND13", "UIWAZHND14", "UIWAZHND15", "UIWAZHND16", // @text UIWAZHND07 EN: "A diagonal blow on the opponent's\ncollarbone while closing in." | JA: "一気に間合いを詰め、鎖骨近辺を狙って、\n斜めに振り打たれる。" ; UIWAZHND08 EN: "A strike on the jaw and chest by\nusing the palm and forearm." | JA: "手のひらや腕全体を使って\n相手の胸や顎を攻撃する技である。" ; UIWAZHND09 EN: "A powerful surprise attack with\na spinning backfist." | JA: "回転して裏拳を放つ強力な奇襲技。" ; UIWAZHND10 EN: "A strike on the pit of the stomach\nby the elbow bent at an acute angle." | JA: "相手の攻撃をかわし、回転の勢いを利用して\n鋭角に曲げた肘で相手の水月を突く技である。" ; UIWAZHND11 EN: "A slashing blow with the side of the\npalm with an advancing step." | JA: "前足を一歩進め、手刀の部分で\n相手に斬りつける技である。" ; UIWAZHND12 EN: "A diagonal blow to the area between\nthe shoulder and a nape of the neck." | JA: "走るスピードを利用し、敵の肩口から\n首筋にかけて、肘打ちを斜めに放つ技である。" ; UIWAZHND13 EN: "A mid-level strike by a second\npunch on the same target." | JA: "中段突きで同じ場所を再度攻撃する技。" ; UIWAZHND14 EN: "A blow by one hand on the opponent\nand slamming it with the other." | JA: "相手に密着させた掌をもう一方の掌で\n激しく叩く事で威力を浸透させる荒業。" ; UIWAZHND15 EN: "Deflecting the opponent's attack and\nkicking his leg at the same time." | JA: "攻撃を受け流すと同時に相手の足を\n 蹴って倒す、高度な技である。" ; UIWAZHND16 EN: "A blow to the opponent's chin thrust\nby lowering the body and springing." | JA: "いったん体勢を低くしたあと\n腰のバネを活かし、相手のアゴを突き上げる技。"
    "UIWAZHND17", "UIWAZHND18"}; // @text UIWAZHND17 EN: "A slashing blow thrusted by\nswinging down the arm and up again." | JA: "腕を大きく振りおろしたのちに\n勢いを利用し振り上げる二連撃の技。" ; UIWAZHND18 EN: "A penetrating blow with the palms\nusing power from the back twist." | JA: "裏回転で生じた力を手刀で相手に叩きこむ。"
const std::array<const char *, 18> g_legMoveNames{"UIWAZLEG00", "UIWAZLEG01", "UIWAZLEG02", "UIWAZLEG03", "UIWAZLEG04", "UIWAZLEG05", "UIWAZLEG06", // @text UIWAZLEG00 EN: "An upward sword-stroke kick, aimed\nat the opponent's head." | JA: "下から太刀（たち）を摺り上げるように\n繰り出される蹴り技。相手の頭部を狙う。" ; UIWAZLEG01 EN: "A front kick aiming the front torso\nof the opponent with the toes." | JA: "いわゆる前蹴り。足裏全体を使って\n主に人体の正面にある急所を狙う。" ; UIWAZLEG02 EN: "A low, decisive kick aimed at the\noutside of the opponent's knee." | JA: "相手の膝の外側を狙い、小さく、かつ鋭く\n放たれるローキックの一種。" ; UIWAZLEG03 EN: "A flying knee move made by\nleaping upward to kick." | JA: "伸び上がるように蹴る、一種の飛び膝蹴り。\n上達すると、跳躍も威力も高くなる。" ; UIWAZLEG04 EN: "A back roundhouse kick striking\nin a downward diagonal motion." | JA: "後ろ回し蹴りの一種で、相手の上段から\n斜め下方向に振り下ろされる技。" ; UIWAZLEG05 EN: "A middle rotating kick followed by\na heel drop." | JA: "上段を狙った外回し蹴りから変化して、\nカカト落しを放つ技である。" ; UIWAZLEG06 EN: "A low to mid-level kick using the\npower of the forward leg." | JA: "体を前方に大きく移動させながら\n足刀で相手の下段から中段を蹴る技。"
    "UIWAZLEG07", "UIWAZLEG08", "UIWAZLEG09", "UIWAZLEG10", "UIWAZLEG11", "UIWAZLEG12", "UIWAZLEG13", "UIWAZLEG14", "UIWAZLEG15", "UIWAZLEG16", // @text UIWAZLEG07 EN: "A powerful high-speed reverse\nroundhouse kick." | JA: "高速の反転後ろ回し蹴り。" ; UIWAZLEG08 EN: "A kick delivered while flipping\nforward in mid-air." | JA: "前方に空中回転をしながら蹴りを放つ技。\n芭月流の中でも特異なわざと言われている。" ; UIWAZLEG09 EN: "A flying back roundhouse kick with\nforce by jump and rotation." | JA: "跳び後ろ回し蹴り。\n回転と跳躍の力が加わるため、威力が高い。" ; UIWAZLEG10 EN: "A flying back kick, aiming\ndiagonally at the opponent's neck." | JA: "飛び後ろ蹴り。相手の首筋を\n袈裟（けさ）切りに蹴る大技である。" ; UIWAZLEG11 EN: "After luring the opponent in,\na reaping kick is delivered." | JA: "攻撃を受け流すと同時に\n相手の足を刈って倒す技。" ; UIWAZLEG12 EN: "Using the spinning momentum of the\n1st kick, a 2nd kick is delivered." | JA: "空中で回し蹴りを出したあと、遠心力を活かし、\nもう１回転して蹴りを放つ荒技。" ; UIWAZLEG13 EN: "A heel drop kick made\nafter a spinning kick." | JA: "回し蹴りを放った後に回転カカト落としを放つ技。  " ; UIWAZLEG14 EN: "Sweeping out the opponent's leg\nwhile spinning on the pivot leg." | JA: "軸足を中心に急旋回して、もう一方の足で\n相手の足元を払う技である。" ; UIWAZLEG15 EN: "A kick on the chin by sliding toward\nthe opponent and making a handstand." | JA: "相手に向かって少しすべり込み、\n逆立ちしながら相手のアゴ先を蹴る奇襲的な技。" ; UIWAZLEG16 EN: "A heel drop kick made\nafter a spinning kick." | JA: "回し蹴りを放った後に回転カカト落としを放つ技。  "
    "UIWAZLEG17"}; // @text EN: "Sliding the ground forward to sweep\nthe feet out." | JA: "走っている最中に滑り込み、相手の足元を払う技。"
const std::array<const char *, 14> g_throwMoveNames{"UIWAZTHR00", "UIWAZTHR01", "UIWAZTHR02", "UIWAZTHR03", "UIWAZTHR04", "UIWAZTHR05", "UIWAZTHR06", // @text UIWAZTHR00 EN: "The opponent is lifted over the back\nand is thrown over." | JA: "相手の腕を取り、襟を引き絞りながら、\n背負うようにして投げ落とす技。" ; UIWAZTHR01 EN: "The opponent's inner thigh is\nswept up to throw." | JA: "相手の腕を取り、強く引き寄せながら、\n相手の腰の部分を払って投げ飛ばす技。" ; UIWAZTHR02 EN: "The practitioner falls backward to\nthrow the opponent overhead to rear." | JA: "自分が後方に倒れ込みながら、\n相手を大きく後ろに投げる技。" ; UIWAZTHR03 EN: "Grabbing the opponent's neck and\nsweeping the opponent's pivot leg." | JA: "相手の首に腕を巻きつけ、足を相手の裏側から\n手前に引き寄せて倒す、大外刈りの一種。" ; UIWAZTHR04 EN: "While throwing the opponent, the\npractitioner flips and lands on him." | JA: "相手を投げるのと同時に、自らも前方に空中回転し\n相手を地面に叩きつける技。" ; UIWAZTHR05 EN: "Grasping the arm and executing\nan armlock to pin." | JA: "相手の腕を取り逆関節を決めて押さえつける技。" ; UIWAZTHR06 EN: "Lifting the opponent on the shoulder\nand dropping him headfirst." | JA: "相手を肩に担ぎ上げ、自ら横方向に倒れ込みながら\n相手の頭から地面に担ぎ落す技。"
    "UIWAZTHR07", "UIWAZTHR08", "UIWAZTHR09", "UIWAZTHR10", "UIWAZTHR11", "UIWAZTHR12", "UIWAZTHR13"}; // @text UIWAZTHR07 EN: "Closing in on the opponent's side\nto throw down while spinning." | JA: "相手の側面から接近し、相手を半回転させながら\n投げ落とす技。芭月流の代表的な投げ技の一つ。" ; UIWAZTHR08 EN: "The throw is executed with\na twist toward the rear." | JA: "プロレスのバックドロップに似た技。\n相手を後方にひねり落すように投げる。" ; UIWAZTHR09 EN: "Evading the opponent and moving\nback into his blind spot to attack." | JA: "相手の攻撃をかわしつつ、背後に回り込み\n敵の死角をつく技。" ; UIWAZTHR10 EN: "A complex move damaging the\nopponent's arms in multiple areas." | JA: "相手の腕関節に数カ所同時に\nダメージを与える、複雑な複合技。" ; UIWAZTHR11 EN: "Crouching momentarily and lunging\nheadfirst into the opponent." | JA: "一瞬、身体を低くして力をため、\n瞬発力をもって、相手に強烈な頭突きをする技。" ; UIWAZTHR12 EN: "Striking the back of the neck after\nmoving into opponent's blind spot." | JA: "影身の派生技。後方に回ったあと、\n無防備となった相手の首筋に手刀を打ち込む。" ; UIWAZTHR13 EN: "Evading the opponent's attack\ndiagonally to make body attack." | JA: "相手の攻撃をななめにかわすのと同時に\n体当たりする技。"
const std::array<MoveAnimationRecord, 19> g_handMoveAnimations{{{0x0, 792, 0, 0, 0, 0}, {0x0, 847, 0, 1, 1, 0}, {0x0, 794, 0, 2, 1, 0},
    {0x0, 1007, 0, 3, 1, 0}, {0x0, 837, 0, 4, 1, 0}, {0x0, 867, 0, 5, 1, 0}, {0x0, 793, 0, 6, 1, 0}, {0x0, 1133, 1427, 14, 2, 0},
    {0x0, 1422, 0, 15, 1, 0}, {0x0, 898, 0, 16, 1, 0}, {0x0, 619, 1290, 17, 2, 0}, {0x0, 1289, 0, 18, 1, 0}, {0x0, 1203, 0, 213, 1, 0},
    {0x800000, 713, 1156, 1, 2, 0}, {0x4000000, 1330, 571, 15, 2, 0}, {0x40400000, 585, 0, 31, 2, 0}, {0x40, 1144, 1145, 32, 2, 0},
    {0x2, 716, 587, 33, 2, 0}, {0x20, 1396, 1428, 36, 2, 0}}};
const std::array<MoveAnimationRecord, 18> g_legMoveAnimations{{{0x0, 870, 0, 7, 1, 0}, {0x0, 849, 0, 8, 1, 0}, {0x0, 788, 0, 9, 2, 0},
    {0x0, 1029, 1438, 10, 2, 0}, {0x0, 1328, 0, 11, 0, 0}, {0x0, 1202, 910, 12, 0, 0}, {0x0, 789, 0, 13, 1, 0}, {0x0, 1136, 1432, 19, 2, 0},
    {0x0, 894, 1431, 20, 2, 0}, {0x0, 868, 1142, 214, 2, 0}, {0x0, 1329, 0, 215, 1, 0}, {0x40200000, 1288, 1433, 9, 0, 0}, {0x1000000, 909, 0, 30, 2, 0},
    {0x4, 1483, 0, 30, 2, 0}, {0x10, 1106, 1107, 34, 2, 0}, {0x8, 1148, 1149, 35, 2, 0}, {0x1000, 736, 589, 37, 0, 0}, {0x200, 1397, 1443, 216, 2, 0}}};
const std::array<MoveAnimationRecord, 14> g_throwMoveAnimations{{{0x0, 931, 0, 21, 0, 0}, {0x0, 1476, 0, 22, 0, 0}, {0x0, 1205, 0, 23, 0, 0},
    {0x0, 1204, 0, 24, 0, 0}, {0x0, 1292, 0, 25, 0, 0}, {0x0, 1526, 0, 26, 0, 0}, {0x0, 1150, 0, 27, 0, 0}, {0x0, 1048, 0, 21, 0, 0},
    {0x0, 1046, 0, 21, 0, 0}, {0x100, 1539, 0, 28, 0, 0}, {0x2000, 918, 0, 38, 0, 0}, {0x80, 912, 881, 39, 0, 0}, {0x4000, 882, 0, 29, 0, 0},
    {0x8000, 929, 0, 40, 0, 0}}};

Character *g_pRyoMovesScreen = nullptr;
std::uint16_t g_movesScreenAnimationId = 0;

namespace {

const MoveAnimationRecord &MoveAnimation(std::uint8_t animSet, std::uint8_t moveId) {
    return animSet == 1 ? g_handMoveAnimations[moveId] : animSet == 2 ? g_legMoveAnimations[moveId] : g_throwMoveAnimations[moveId];
}


void RequestMovesScreenRyoAnimation(std::uint16_t animation) {
    MotionManagerWork *const motion = MOTM_GetWork(g_pRyoMovesScreen);
    motion->motion.motionFlags &= ~2U;
    motion->motion.pendingAnimId = animation;
    motion->motion.motionFlags |= 0x10000000U;
    motion->motion.motionFlags |= 8U;
    motion->motion.motionFlags |= 4U;
}

} // namespace



std::uint16_t ResolveAnimID(std::uint8_t animSet, std::uint16_t moveId, std::uint16_t experience) {
    if (animSet < 1 || animSet > 3)
        return experience;
    MoveSetRuntimeState &set = g_playerStats.moveSets[animSet - 1U];
    std::array<std::uint16_t, 20> &newExperience = g_playerStats.moveUsageCounts[animSet - 1U];
    const std::int64_t count = static_cast<std::int8_t>(set.count);
    std::int32_t index = 0;
    while (true) {
        if (index >= count)
            return experience;
        if (set.moveValue[static_cast<std::size_t>(index)] == moveId)
            break;
        ++index;
    }
    if (experience) {
        newExperience[static_cast<std::size_t>(index)] = static_cast<std::uint16_t>(newExperience[static_cast<std::size_t>(index)] + experience);
        return experience;
    }
    const std::int32_t gained = static_cast<std::int16_t>(newExperience[static_cast<std::size_t>(index)]);
    const std::int32_t progress = static_cast<std::int16_t>(set.learnValue[static_cast<std::size_t>(index)]);
    return static_cast<std::uint16_t>(gained <= 0 ? progress : gained + progress);
}


std::uint16_t ResolveAnimationById(std::uint8_t animSet, std::uint8_t moveId) {
    const MoveAnimationRecord &record = MoveAnimation(animSet, moveId);
    if (static_cast<std::int16_t>(ResolveAnimID(animSet, moveId, 0)) < 56 || !record.masteredAnimation)
        return record.animation;
    return record.masteredAnimation;
}


void RequestAnimation(std::uint8_t animSet, std::uint8_t moveId) {
    if (!g_pRyoMovesScreen)
        return;
    const MoveAnimationRecord &record = MoveAnimation(animSet, moveId);
    std::uint16_t animation = record.masteredAnimation;
    if (static_cast<std::int16_t>(ResolveAnimID(animSet, moveId, 0)) < 56 || !animation)
        animation = record.animation;
    if (g_movesScreenAnimationId == animation)
        return;
    g_movesScreenAnimationId = animation;
    RequestMovesScreenRyoAnimation(animation);
}




std::int32_t InitMoves(std::int16_t move, std::int16_t mode, std::int16_t animSet) {
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    const std::int32_t handCount = static_cast<std::int8_t>(sets[0].count);
    const std::int32_t legCount = static_cast<std::int8_t>(sets[1].count);
    const std::int32_t throwCount = static_cast<std::int8_t>(sets[2].count);
    std::int16_t index = move;
    if (!animSet) {
        if (move < handCount) {
            animSet = 1;
        } else if (move < legCount + handCount) {
            index = static_cast<std::int16_t>(move - handCount);
            animSet = 2;
        } else if (move >= legCount + handCount + throwCount) {
            return 0;
        } else {
            animSet = 3;
            index = static_cast<std::int16_t>(move - (handCount + legCount));
        }
    }
    const std::size_t set = animSet == 1 ? 0 : animSet == 2 ? 1 : 2;
    const std::int16_t moveId = static_cast<std::int16_t>(sets[set].moveValue[static_cast<std::size_t>(index)]);
    const std::int16_t progress = static_cast<std::int16_t>(sets[set].learnValue[static_cast<std::size_t>(index)]);
    if (moveId == -1)
        return 0;
    if (mode) {
        if (mode == 2)
            return static_cast<std::uint16_t>(moveId) | (animSet << 8);
        if (mode != 1)
            return 0;
        if (progress < 12) {
            shenExecuteAICACommand(0x800abU, 0, 0);
            return 1;
        }
        if (static_cast<std::uint8_t>(g_movesScreenCurrentAnimSet) == animSet && static_cast<std::int8_t>(g_movesScreenCurrentAnimSet >> 8) == index)
            return 1;
        g_movesScreenCurrentAnimSet = static_cast<std::uint16_t>(static_cast<std::uint8_t>(animSet) | (static_cast<std::uint8_t>(index) << 8));
        const std::uint8_t fightMove = static_cast<std::uint8_t>(moveId + kAnimSetTypes[static_cast<std::size_t>(animSet)]);
        if (static_cast<std::uint32_t>(animSet - 1) <= 2U && static_cast<std::uint16_t>(moveId) < 20U && fightMove != 0xffU)
            g_playerStats.currentMove = static_cast<std::uint16_t>((g_playerStats.currentMove & 0x00ffU) | (fightMove << 8));
        return 0;
    }
    std::uint16_t animation = 1327;
    if (progress >= 12) {
        const MoveCommandRecord &commands = animSet == 1 ?
            g_handMoveCommands[static_cast<std::size_t>(g_handMoveList[static_cast<std::size_t>(moveId)].commandIndex)] :
            animSet == 2 ? g_legMoveCommands[static_cast<std::size_t>(g_legMoveList[static_cast<std::size_t>(moveId)].commandIndex)] :
                           g_throwMoveCommands[static_cast<std::size_t>(g_throwMoveList[static_cast<std::size_t>(moveId)].commandIndex)];
        lcdmRenderButtonGlyphs(commands.data());
        animation = ResolveAnimationById(static_cast<std::uint8_t>(animSet), static_cast<std::uint8_t>(moveId));
    } else {
        lcdmInit(0);
    }
    g_movesScreenAnimationId = animation;
    if (g_pRyoMovesScreen)
        RequestMovesScreenRyoAnimation(animation);
    return 0;
}

namespace {

// The demo Ryo's model and the first node of it that he draws, and his ground's model and its root.
PackModel *g_movesRyoModel = nullptr;
void *g_movesRyoModelNode = nullptr;
PackModel *g_movesGroundModel = nullptr;
void *g_movesGroundRoot = nullptr;
// Placements left before the demo Ryo shows (-1 once shown, or without him).
std::int8_t g_movesRyoRevealCountdown = -1;
// The right stick the demo Ryo turns by (x, then y inverted; 0x80 is centred).
std::array<std::uint8_t, 2> g_movesRyoStick{0x80, 0x80};
// The demo Ryo's pitch and yaw, and the camera yaw he stands at.
std::int32_t g_movesRyoPitch = 0;
std::int32_t g_movesRyoYaw = 0;
std::int32_t g_movesRyoCameraYaw = 0;
// The demo's roll and scale as loaded, and a third value the load only stores.
std::int32_t g_movesRyoRoll = 0;
float g_movesRyoScale = 0.0f;
float g_movesRyoStoredValue = 0.0f;
// Cleared by the load and not read.
std::int32_t g_movesRyoCleared = 0;
// Where the demo Ryo stands, and the transform his motion is rooted at.
Vector3 g_movesRyoPosition{};
Matrix4x4 g_movesRyoTransform{};




constexpr float kMovesRyoMinScale = 0.044f;
constexpr float kMovesRyoMaxScale = 0.044f;
constexpr float kMovesRyoDistance = 0.1475f;
constexpr float kMovesRyoDrop = 0.041f;
constexpr float kMovesRyoSideOffset = 0.0f;
constexpr float kMovesRyoScaleYawFactor = 1.111f;
constexpr std::int32_t kMovesRyoRestPitch = 0;
constexpr std::int32_t kMovesRyoFacing = 0x4000;
constexpr std::int32_t kMovesRyoRestRoll = 0;
constexpr float kMovesRyoStoredValue = 0.0f;
constexpr std::int32_t kMovesGroundYawOffset = 0;
constexpr float kMovesGroundScaleYawFactor = 0.0f;
constexpr std::int32_t kMovesRyoYawStep = 0x5b0;
constexpr std::int32_t kMovesRyoPitchStep = 0x5b0;

// The demo Ryo's stand for the camera yaw: `distance` before the camera look-at, `drop` below it.
void PlaceMovesRyo(float distance, float sideOffset, float drop) {
    const Vector3 &lookAt = g_CameraTargetPos;
    g_movesRyoPosition.x = sin_fixed16(static_cast<std::int16_t>(g_movesRyoCameraYaw)) * distance + lookAt.x + sideOffset;
    auto folded = static_cast<std::uint16_t>(g_movesRyoCameraYaw);
    if (folded > 0x8000U)
        folded = static_cast<std::uint16_t>(0x10000U - folded);
    g_movesRyoPosition.z = lookAt.z - sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * distance;
    g_movesRyoPosition.y = lookAt.y - drop;
}

MapControlData *MovesRyoControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

} // namespace

void LoadRyoForMovesScreen(std::int16_t animSet, std::int16_t move, bool revealSooner) {
    g_movesRyoModel = nullptr;
    g_movesRyoModelNode = nullptr;
    g_pRyoMovesScreen = nullptr;
    g_movesGroundModel = nullptr;
    g_movesGroundRoot = nullptr;
    std::array<char, 32> directory{};
    std::snprintf(directory.data(), directory.size(), "scene/%02d/model/chara", g_sceneId);
    void *const file = ReadFile(directory.data(), "YKB_M.MT5", SHEN_NAME(WZMD));
    if (!file)
        return;
    std::array<char, 261> remapContext{};
    std::snprintf(remapContext.data(), remapContext.size(), "%s/%s", directory.data(), "YKB_M.MT5");
    PackModel *const model = MT5_InitRawModelAndTextures(file, "YKB_M.MT5", remapContext.data());
    if (!model)
        return;
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), remapContext.data(), 0, 64);
    if (!model->model)
        return;
    SetupEngineObject(reinterpret_cast<std::uintptr_t>(model->model), reinterpret_cast<std::uintptr_t>(model->model), 8);
    g_movesRyoModel = model;
    const auto *const root = static_cast<const Mt5RuntimeNode *>(model->model);
    g_movesRyoModelNode = root->childOffset ? MT5_ModelBaseForNode(root) + root->childOffset : nullptr;
    void *const groundFile = ReadFile("model/item", "WAZ0508G.MT5", SHEN_NAME(WZMZ));
    if (!groundFile)
        return;
    PackModel *const ground = MT5_InitRawModelAndTextures(groundFile, "WAZ0508G.MT5", nullptr);
    Character *ryo = nullptr;
    if (ground && ground->model) {
        g_movesGroundModel = ground;
        g_movesGroundRoot = ground->model;
        ryo = AddCharacterHandler(CharacterHandlerPriority::Low);
    }
    if (!ryo) {
        DestroyForMovesScreen();
        return;
    }
    g_pRyoMovesScreen = ryo;
    if (MapControlData *const control = MovesRyoControl())
        CHID_Init(control->scenarioTask, SHEN_NAME(MMJL), ryo);
    ryo->model = g_movesRyoModelNode;
    ryo->renderOpacity = 1.0f;
    MOTN_Alloc(ryo, false);
    NPCLogger(ryo, 3, 0);
    g_movesRyoPitch = kMovesRyoRestPitch;
    g_movesRyoCameraYaw = g_fCamAccumYaw - 4551;
    g_movesRyoYaw = 0x8000 - g_movesRyoCameraYaw;
    g_movesRyoRoll = kMovesRyoRestRoll;
    g_movesRyoScale = kMovesRyoMinScale;
    g_movesRyoStoredValue = kMovesRyoStoredValue;
    g_movesRyoCleared = 0;
    g_movesRyoStick = {0x80, 0x80};
    MotionManagerWork *const manager = MOTM_GetWork(ryo);
    g_vfPlayerAggroCombat = 0;
    if (manager) {
        manager->motion.currentMotionScale = kMovesRyoMinScale;
        manager->motion.motionFlags &= ~0x4000U;
        manager->motion.targetMotionScale = g_movesRyoScale;
    }
    PlaceMovesRyo(kMovesRyoDistance, 0.0f, kMovesRyoDrop);
    AllocREFB(ryo, 1);
    if (ReferenceBoardWork *const board = FindReferenceBoardWork(ryo)) {
        board->status = 1;
        board->colour = {0.55f, 0.55f, 0.55f};
    }
    shenApplyTranslationToAll(ryo->entity.node, g_movesRyoPosition);
    ryo->entity.node.rotation.y = kMovesRyoFacing;
    InitMoves(move, 0, animSet);
    g_movesRyoRevealCountdown = revealSooner ? 1 : 3;
}

void DestroyForMovesScreen() {
    lcdmInit(0);
    g_bShouldDraw = 1;
    if (Character *const ryo = g_pRyoMovesScreen) {
        NPCLogger(ryo, 3, 0);
        ryo->flags.value &= ~CharacterFlagUnknown;
        if (MapControlData *const control = MovesRyoControl())
            ++control->pendingCharacterRetirements;
    }
    g_pRyoMovesScreen = nullptr;
    if (g_movesRyoModel) {
        if (auto *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(g_movesRyoModel->model)))) {
            if (entry->retire != 2)
                entry->retire = 1;
            g_hmdlEntryDirty = 1;
        }
        FreePVR(g_movesRyoModel);
    }
    if (g_movesGroundModel)
        FreePVR(g_movesGroundModel);
    g_movesRyoModel = nullptr;
    g_movesRyoModelNode = nullptr;
    g_movesGroundModel = nullptr;
    g_movesGroundRoot = nullptr;
    g_movesRyoRevealCountdown = -1;
    g_vfPlayerAggroCombat = 1;
}

void MovesScreen_PlaceAndDrawRyo() {
    const bool frozen = (Shenmue_nPauseState & 2) != 0 || g_moveHelpPopupState != 0;
    if (!frozen) {
        const std::uint16_t stick = g_inputState.slots[0].rightStick;
        g_movesRyoStick = {static_cast<std::uint8_t>(stick), static_cast<std::uint8_t>(-static_cast<std::uint8_t>(stick >> 8U))};
    }
    Character *const ryo = g_pRyoMovesScreen;
    if (!ryo)
        return;
    if (g_movesRyoRevealCountdown >= 0 && --g_movesRyoRevealCountdown < 0)
        NPCLogger(ryo, 3, 1);
    MotionManagerWork *const manager = MOTM_GetWork(ryo);
    float scale = kMovesRyoMaxScale;
    if (g_movesRyoScale < kMovesRyoMaxScale)
        scale = std::fmax(kMovesRyoMinScale, g_movesRyoScale);
    if (manager) {
        manager->motion.currentMotionScale = scale;
        manager->motion.targetMotionScale = scale;
    }
    const float growth = scale - kMovesRyoMinScale;
    const float scaledGrowth = growth * 1000.0f;
    g_movesRyoCameraYaw = g_fCamAccumYaw - static_cast<std::uint16_t>(static_cast<std::int32_t>(scaledGrowth * kMovesRyoScaleYawFactor * 8192.0f / 45.0f));
    PlaceMovesRyo(kMovesRyoDistance - growth, kMovesRyoSideOffset, kMovesRyoDrop);
    g_movesRyoPosition.y += growth;
    if (!frozen) {
        const std::int32_t targetYaw = (g_movesRyoStick[0] << 8) - g_movesRyoCameraYaw;
        if (g_movesRyoStick[0] == 0x80) {
            g_movesRyoYaw = targetYaw;
        } else if (static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>((g_movesRyoStick[0] - 128) << 13) / 45.0f)) <= 0x8000U) {
            g_movesRyoYaw += kMovesRyoYawStep;
            if (g_movesRyoYaw >= targetYaw)
                g_movesRyoYaw = targetYaw;
        } else {
            g_movesRyoYaw -= kMovesRyoYawStep;
            if (g_movesRyoYaw <= targetYaw)
                g_movesRyoYaw = targetYaw;
        }
        const std::int32_t targetPitch = kMovesRyoRestPitch + ((g_movesRyoStick[1] - 128) << 7);
        if (g_movesRyoStick[1] == 0x80) {
            g_movesRyoPitch = targetPitch;
        } else if (static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>((g_movesRyoStick[1] - 128) << 13) / 45.0f)) <= 0x8000U) {
            g_movesRyoPitch += kMovesRyoPitchStep / 2;
            if (g_movesRyoPitch >= targetPitch)
                g_movesRyoPitch = targetPitch;
        } else {
            g_movesRyoPitch -= kMovesRyoPitchStep / 2;
            if (g_movesRyoPitch <= targetPitch)
                g_movesRyoPitch = targetPitch;
        }
    }
    shenApplyTranslationToAll(ryo->entity.node, g_movesRyoPosition);
    ryo->entity.node.rotation.y = kMovesRyoFacing;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(g_movesRyoPosition.x, g_movesRyoPosition.y + scale, g_movesRyoPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(g_movesRyoYaw, g_movesRyoPitch, g_movesRyoRoll);
    g_movesRyoTransform = workTransformMatrix;
    Matrix4x4::Pop(1);
    if (manager) {
        manager->motion.externalTransform = &g_movesRyoTransform;
        manager->motion.externalTransformAnchorNodeId = -1;
    }
    g_drawingImageModel = g_movesRyoModel;
    Character_SubmitForDrawing(ryo);
    g_drawingImageModel = nullptr;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(g_movesRyoPosition.x, g_movesRyoPosition.y + scale, g_movesRyoPosition.z);
    const auto groundYaw = static_cast<std::uint16_t>(static_cast<std::int32_t>(scaledGrowth * kMovesGroundScaleYawFactor * 8192.0f / 45.0f));
    Matrix4x4::SetViewMatrixFromEuler(g_movesRyoYaw + groundYaw - kMovesGroundYawOffset, g_movesRyoPitch, g_movesRyoRoll);
    Matrix4x4::Scale(scale, scale, scale);
    Matrix4x4::preTranslate(0.0f, -1.0f, 0.0f);
    d3t::MT5_SubmitObjectNode(g_movesGroundRoot, 1.0f);
    Matrix4x4::Pop(1);
}

void (*g_pMovesScreenSceneBuffer)() = nullptr;

namespace {


D3TDeviceContext *g_movesScreenContext = nullptr;



void MovesScreen_DrawTexturedQuad(D3TDeviceContext *context, std::uint32_t source, const Vector4 &uv, std::uint32_t target) {
    std::uint32_t sourceWidth = 0;
    std::uint32_t sourceHeight = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    if (!GetTexture2DDimensions(source, &sourceWidth, &sourceHeight) || !GetTexture2DDimensions(target, &width, &height))
        return;
    SetRenderTarget(context, target);
    SetViewport(context, static_cast<float>(width), static_cast<float>(height));
    SetShaderState(context, g_texturedQuadVertexShader, g_texturedQuadPixelShader, g_fvfLayouts[34]);
    SetCommonQuadState(context, 0xf0000000ULL);
    const std::array<Vector4, 4> vertices{{{-1.0f, -1.0f, uv.x, uv.w}, {1.0f, -1.0f, uv.z, uv.w}, {-1.0f, 1.0f, uv.x, uv.y}, {1.0f, 1.0f, uv.z, uv.y}}};
    if (void *const mapped = GetMappedConstantBuffer(context, 0, 64))
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    SetTopology(context, 1);
    BindPixelTexture8(context, source);
    SubmitQuad(context);
}

} // namespace

void MovesScreen_RenderScene() {
    MovesScreen_PlaceAndDrawRyo();
    EFFE_ReplayQueuedEffects(0);
    ComposeEffectABuffer();
}

void InitMovesScreenPipelineContext() {
#if defined(_WIN32)
    AcquireScenePipelineContext();
    g_movesScreenContext = DX11::g_ppD3DDeviceContext;
    if (!g_movesScreenContext || !g_movesScreen.sceneTextureBound)
        return;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    float sceneWidth = 0.0f;
    float sceneHeight = 0.0f;
    if (GetTexture2DDimensions(g_fullScreenTexture2D, &width, &height)) {
        sceneWidth = static_cast<float>(width);
        sceneHeight = static_cast<float>(height);
    } else {
        sceneWidth = static_cast<float>(GetScreenResolution()->width);
        sceneHeight = static_cast<float>(GetScreenResolution()->height);
    }
    ScaleViewport(sceneWidth, sceneHeight);
    constexpr std::array<float, 4> kClearColour{};
    UnmapAndClearViews(g_movesScreenContext, g_fullScreenTexture2D, 0, kClearColour.data());
    UnmapAndClearViews(g_movesScreenContext, g_originalStripsTexture2D, 0, kClearColour.data());
    SetupPPFX(g_movesScreenContext);
#endif
}

void RenderMovesScreenSceneToTexture() {
#if defined(_WIN32)
    if (!g_movesScreenContext)
        return;
    std::uint32_t sceneWidthPixels = 0;
    std::uint32_t sceneHeightPixels = 0;
    if (GetTexture2DDimensions(g_fullScreenTexture2D, &sceneWidthPixels, &sceneHeightPixels)) {
        const MovesScreenElement &moves = g_movesScreen;
        const auto width = static_cast<float>(sceneWidthPixels);
        const auto height = static_cast<float>(sceneHeightPixels);
        const auto targetWidth = static_cast<float>(moves.sceneTextureWidth);
        const auto targetHeight = static_cast<float>(moves.sceneTextureHeight);
        float cropWidth = width;
        float cropHeight = height;
        if (height / width <= targetHeight / targetWidth) {
            if (targetHeight / targetWidth > height / width)
                cropWidth = height / targetHeight * targetWidth;
        } else {
            cropHeight = width / targetWidth * targetHeight;
        }
        const float left = width * 0.5f - cropWidth * 0.5f;
        const float top = height * 0.5f - cropHeight * 0.5f;
        Vector4 uv{};
        uv.x = left == 0.0f || width == 0.0f ? 0.0f : left / width;
        uv.y = top == 0.0f || height == 0.0f ? 0.0f : top / height;
        uv.z = (cropWidth == 0.0f || width == 0.0f ? 0.0f : cropWidth / width) + uv.x;
        uv.w = (cropHeight == 0.0f || height == 0.0f ? 0.0f : cropHeight / height) + uv.y;
        MovesScreen_DrawTexturedQuad(g_movesScreenContext, g_fullScreenTexture2D, uv, moves.sceneTexture);
    }
    SubmitAndReleasePipelineContext();
    g_movesScreenContext = nullptr;
    ScaleViewport(-1.0f, -1.0f);
#endif
}

std::uint16_t g_movesScreenCurrentAnimSet = 0;
std::uint8_t g_movesScreenSyncedMoveSet = 0;
std::uint8_t g_selectedMoveListIndex = 0;
std::array<std::uint8_t, 4> g_moveListMoveIds{};
std::array<std::uint8_t, 4> g_moveListFocusFlags{};
std::array<std::uint8_t, 3> g_moveListCounts{};
std::array<std::uint16_t, 20> g_movesScreenMoveProgress{};
std::array<std::uint16_t, 20> g_movesScreenMoveUsage{};



void SyncMovesScreenRuntimeState() {
    std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    for (std::size_t set = 0; set < sets.size(); ++set) {
        g_moveListMoveIds[set + 1] = static_cast<std::uint8_t>(sets[set].countOrUnknown & 0x3fU);
        g_moveListCounts[set] = sets[set].count;
        g_moveListFocusFlags[set + 1] = static_cast<std::uint8_t>(sets[set].countOrUnknown & 0xc0U);
    }
    std::uint16_t &focus = g_playerStats.movesScreenMoveSetAndSelectionState;
    std::uint8_t moveSet = static_cast<std::uint8_t>(focus);
    if ((g_moveListFocusFlags[1] & 0xc0U) != 0)
        moveSet = 1;
    else if ((g_moveListFocusFlags[2] & 0xc0U) != 0)
        moveSet = 2;
    else if ((g_moveListFocusFlags[3] & 0xc0U) != 0)
        moveSet = 3;
    focus = static_cast<std::uint16_t>((focus & 0xff00U) | moveSet);
    g_selectedMoveListIndex = moveSet;
    g_movesScreenSyncedMoveSet = moveSet;
    if (static_cast<std::int8_t>(g_moveListFocusFlags[moveSet]) < 0) {
        std::int8_t move = static_cast<std::int8_t>(g_moveListMoveIds[moveSet]);
        g_playerStats.movesScreenSelectedMoveIndex = static_cast<std::uint8_t>(move);
        MoveSetRuntimeState &set = sets[moveSet == 1 ? 0 : moveSet == 2 ? 1 : 2];
        const std::int8_t count = static_cast<std::int8_t>(set.count);
        if (move < count) {
            do {
                if (static_cast<std::int16_t>(set.learnValue[static_cast<std::size_t>(move)]) < 100 || move >= count - 1)
                    break;
                ++move;
            } while (move < count);
            g_playerStats.movesScreenSelectedMoveIndex = static_cast<std::uint8_t>(move);
        }
        set.countOrUnknown = static_cast<std::uint8_t>((set.countOrUnknown & 0xc0U) | static_cast<std::uint8_t>(move));
    }
    g_moveListMoveIds[1] = 0;
    g_moveListMoveIds[2] = 0;
    g_moveListMoveIds[3] = 0;
    const std::uint8_t currentMove = static_cast<std::uint8_t>(g_playerStats.currentMove >> 8);
    if (currentMove != 0xffU) {
        for (std::uint8_t animSet = 1; animSet < 4; ++animSet) {
            if (currentMove >= kAnimSetTypes[animSet + 1U])
                continue;
            const auto moveId = static_cast<std::int8_t>(currentMove - kAnimSetTypes[animSet]);
            const MoveSetRuntimeState &set = sets[animSet - 1U];
            const std::int16_t count = static_cast<std::int8_t>(set.count);
            std::int16_t index = 0;
            while (index < count && static_cast<std::int16_t>(set.moveValue[static_cast<std::size_t>(index)]) != moveId)
                ++index;
            g_movesScreenCurrentAnimSet = static_cast<std::uint16_t>(animSet | (index < count ? static_cast<std::uint8_t>(index) << 8 : 0));
            return;
        }
    }
    g_movesScreenCurrentAnimSet = 1;
}

namespace {

// The g_movesScreenMoveProgress/g_movesScreenMoveUsage copies of one AnimSet's progress and usage that SetMovesScreenMoveSetAndSelection
// and SetTrainingFocusMoveSet take for the moves list.
void CopyMovesScreenMoveSet(std::size_t set) {
    const MoveSetRuntimeState &moves = g_playerStats.moveSets[set];
    const std::int8_t count = static_cast<std::int8_t>(moves.count);
    for (std::int8_t index = 0; index < count; ++index) {
        g_movesScreenMoveProgress[static_cast<std::size_t>(index)] = moves.learnValue[static_cast<std::size_t>(index)];
        g_movesScreenMoveUsage[static_cast<std::size_t>(index)] = g_playerStats.moveUsageCounts[set][static_cast<std::size_t>(index)];
    }
}

void StoreMovesScreenFocus(std::uint8_t set1, std::uint8_t set2, std::uint8_t set3) {
    g_playerStats.moveSets[0].countOrUnknown = set1;
    g_playerStats.moveSets[1].countOrUnknown = set2;
    g_playerStats.moveSets[2].countOrUnknown = set3;
}

} // namespace



std::uint32_t SetMovesScreenMoveSetAndSelection(std::int32_t moveSet, std::int32_t move) {
    g_movesScreenTransitionFlags = 1;
    SyncMovesScreenRuntimeState();
    (void)VFMA_RequestMove(VFMoveRequestFlags::ApplyEasedCommitTrigger);
    if (moveSet >= 1 && moveSet <= 3 && static_cast<std::int8_t>(g_playerStats.moveSets[static_cast<std::size_t>(moveSet - 1)].count) > 0)
        CopyMovesScreenMoveSet(static_cast<std::size_t>(moveSet - 1));
    g_moveListFocusFlags[1] &= 0x3fU;
    g_moveListFocusFlags[2] &= 0x3fU;
    g_moveListFocusFlags[3] &= 0x3fU;
    std::uint16_t &focus = g_playerStats.movesScreenMoveSetAndSelectionState;
    g_selectedMoveListIndex = static_cast<std::uint8_t>(moveSet);
    focus = static_cast<std::uint16_t>((focus & 0xff00U) | static_cast<std::uint8_t>(moveSet));
    if (moveSet)
        g_moveListFocusFlags[static_cast<std::uint8_t>(moveSet)] |= 0x40U;
    if (move == -1) {
        focus &= 0x00ffU;
    } else {
        g_moveListMoveIds[static_cast<std::uint8_t>(focus)] = static_cast<std::uint8_t>(move);
        if (static_cast<std::int16_t>(g_movesScreenMoveProgress[g_moveListMoveIds[g_selectedMoveListIndex]]) >= 100)
            return 0;
        g_playerStats.movesScreenSelectedMoveIndex = g_moveListMoveIds[static_cast<std::uint8_t>(focus)];
        g_moveListFocusFlags[g_selectedMoveListIndex] |= 0x80U;
        focus = static_cast<std::uint16_t>((focus & 0x00ffU) | 0x0100U);
    }
    StoreMovesScreenFocus(static_cast<std::uint8_t>(g_moveListFocusFlags[1] | g_moveListMoveIds[1]),
        static_cast<std::uint8_t>(g_moveListFocusFlags[2] | g_moveListMoveIds[2]), static_cast<std::uint8_t>(g_moveListFocusFlags[3] | g_moveListMoveIds[3]));
    g_movesScreenTransitionFlags = 0;
    return 1;
}





std::uint32_t SetTrainingFocusMoveSet(std::int32_t trainingFocus, std::int32_t move) {
    std::uint16_t &focus = g_playerStats.movesScreenMoveSetAndSelectionState;
    if (trainingFocus == 1) {
        g_movesScreenTransitionFlags = 1;
        SyncMovesScreenRuntimeState();
        (void)VFMA_RequestMove(VFMoveRequestFlags::ApplyEasedCommitTrigger);
        const auto set1 = static_cast<std::uint8_t>(g_moveListFocusFlags[1] & 0x3fU);
        const auto set2 = static_cast<std::uint8_t>(g_moveListFocusFlags[2] & 0x3fU);
        const auto set3 = static_cast<std::uint8_t>(g_moveListFocusFlags[3] & 0x3fU);
        g_selectedMoveListIndex = 0;
        g_moveListFocusFlags[1] &= 0x3fU;
        g_moveListFocusFlags[2] &= 0x3fU;
        g_moveListFocusFlags[3] &= 0x3fU;
        focus &= 0xff00U;
        if (move == -1) {
            focus &= 0x00ffU;
        } else {
            g_moveListMoveIds[0] = static_cast<std::uint8_t>(move);
            if (static_cast<std::int16_t>(g_movesScreenMoveProgress[static_cast<std::size_t>(move)]) >= 100)
                return 0;
            g_moveListFocusFlags[0] |= 0x80U;
            g_playerStats.movesScreenSelectedMoveIndex = static_cast<std::uint8_t>(move);
            focus = static_cast<std::uint16_t>((focus & 0x00ffU) | 0x0100U);
        }
        g_movesScreenTransitionFlags = 0;
        StoreMovesScreenFocus(static_cast<std::uint8_t>(set1 | g_moveListMoveIds[1]), static_cast<std::uint8_t>(set2 | g_moveListMoveIds[2]),
            static_cast<std::uint8_t>(set3 | g_moveListMoveIds[3]));
        return 1;
    }
    if (trainingFocus == 2)
        return SetMovesScreenMoveSetAndSelection(1, move);
    if (trainingFocus != 3 && trainingFocus != 4)
        return 0;
    const std::size_t focused = trainingFocus == 3 ? 2 : 3;
    g_movesScreenTransitionFlags = 1;
    SyncMovesScreenRuntimeState();
    (void)VFMA_RequestMove(VFMoveRequestFlags::ApplyEasedCommitTrigger);
    if (static_cast<std::int8_t>(g_playerStats.moveSets[focused - 1].count) > 0)
        CopyMovesScreenMoveSet(focused - 1);
    std::array<std::uint8_t, 4> flags{0, static_cast<std::uint8_t>(g_moveListFocusFlags[1] & 0x3fU), static_cast<std::uint8_t>(g_moveListFocusFlags[2] & 0x3fU),
        static_cast<std::uint8_t>(g_moveListFocusFlags[3] & 0x3fU)};
    flags[focused] |= 0x40U;
    g_selectedMoveListIndex = static_cast<std::uint8_t>(focused);
    g_moveListFocusFlags[1] &= 0x3fU;
    g_moveListFocusFlags[2] &= 0x3fU;
    g_moveListFocusFlags[3] &= 0x3fU;
    focus = static_cast<std::uint16_t>((focus & 0xff00U) | focused);
    g_moveListFocusFlags[focused] = flags[focused];
    if (move == -1) {
        focus &= 0x00ffU;
    } else {
        g_moveListMoveIds[focused] = static_cast<std::uint8_t>(move);
        if (static_cast<std::int16_t>(g_movesScreenMoveProgress[static_cast<std::size_t>(move)]) >= 100)
            return 0;
        flags[focused] |= 0x80U;
        g_playerStats.movesScreenSelectedMoveIndex = static_cast<std::uint8_t>(move);
        g_moveListFocusFlags[focused] = flags[focused];
        focus = static_cast<std::uint16_t>((focus & 0x00ffU) | 0x0100U);
    }
    StoreMovesScreenFocus(static_cast<std::uint8_t>(flags[1] | g_moveListMoveIds[1]), static_cast<std::uint8_t>(flags[2] | g_moveListMoveIds[2]),
        static_cast<std::uint8_t>(flags[3] | g_moveListMoveIds[3]));
    g_movesScreenTransitionFlags = 0;
    return 1;
}

std::uint8_t BeginMovesScreenViewTransition() {
    if (g_movesScreenViewActive)
        return g_movesScreenNativeMoveCount;
    SyncMovesScreenRuntimeState();
    (void)VFMA_RequestMove(VFMoveRequestFlags::UseEasing);
    g_movesScreenRuntimePrimed = 1;
    const std::array<MoveSetRuntimeState, 3> &sets = g_playerStats.moveSets;
    g_movesScreenNativeMoveCount = static_cast<std::uint8_t>(sets[0].count + sets[1].count + sets[2].count + 1);
    g_movesScreenTransitionGate = 0;
    g_moveListMoveIds[0] = 0;
    g_movesScreenViewActive = 1;
    return g_movesScreenNativeMoveCount;
}

HLib::Task *g_pMovesScreenTask = nullptr;
const MoveListRecord *g_movesScreenTextBank = nullptr;
MoveSetRuntimeState *g_movesScreenMoveTable = nullptr;
std::uint16_t g_movesScreenRefreshState = 0;
std::uint16_t g_movesScreenTransitionFlags = 0;
std::int32_t g_movesScreenViewActive = 0;
std::uint8_t g_movesScreenTransitionGate = 0;
float g_movesScreenTransitionOffset = -50.0f;
std::uint8_t g_movesScreenRuntimePrimed = 0;
std::uint8_t g_movesScreenNativeMoveCount = 0;

std::uint16_t WZOG_GetTransitionFlags() {
    return g_movesScreenTransitionFlags;
}

std::uint16_t WZOG_GetRefreshState() {
    return g_movesScreenRefreshState;
}

bool WZOG_IsViewActive() {
    return g_movesScreenViewActive != 0;
}

void EnqueueWZOG() {
    if (g_pMovesScreenTask)
        return;
    g_pMovesScreenTask = HLib_EnqueueTaskWithoutParameter(WZOG_Callback, 15, 0, makeToken('W', 'Z', 'O', 'G'));
    g_moveListMoveIds[0] = 0;
    (void)HandleMovesScreenUi();
}

void WZOG_RequestClose() {
    if ((g_movesScreenTransitionFlags & kTransitionFlagMovesView) == 0) {
        return;
    }
    if (g_pMovesScreenTask) {
        if (static_cast<std::int16_t>(g_movesScreenRefreshState) < static_cast<std::int16_t>(kRefreshCloseRequested)) {
            g_movesScreenRefreshState = kRefreshCloseRequested;
        }
        return;
    }
    (void)VFMA_RequestMove(VFMoveRequestFlags::ReturnAbsolute);
    g_movesScreenTransitionFlags = 0;
    g_pendingMovePopupMoveId = 0;
    g_pendingMovePopupAnimSetId = 0;
    g_movesScreenViewActive = 0;
    g_movesScreenRefreshState = 0;
}

void shenRefreshUI() {
    if (g_uiElements[static_cast<std::size_t>(kMovesElement)].isVisible == 1) {
        ResetAuthToNone();
    }

    if ((g_movesScreenTransitionFlags & kTransitionFlagMovesView) == 0) {
        return;
    }
    if (g_pMovesScreenTask) {
        if (static_cast<std::int16_t>(g_movesScreenRefreshState) < static_cast<std::int16_t>(kRefreshCloseRequested)) {
            g_movesScreenRefreshState = kRefreshCloseRequested;
        }
        return;
    }
    if (g_movesScreenViewActive == 1) {
        (void)VFMA_RequestMove(VFMoveRequestFlags::ReturnAbsolute);
        g_movesScreenTransitionFlags = 0;
        g_pendingMovePopupMoveId = 0;
        g_pendingMovePopupAnimSetId = 0;
        g_movesScreenViewActive = 0;
    }
    g_movesScreenRefreshState = 0;
}

void WZOG_Callback(HLib::TaskData *) {
    const std::int64_t result = HandleMovesScreenUi();
    if (!result || !g_pMovesScreenTask)
        return;
    HLib_DestroyTask(g_pMovesScreenTask);
    g_pMovesScreenTask = nullptr;
    g_movesScreenRefreshState = 0;
}

void WZOG_Destroy() {
    if (g_pMovesScreenTask) {
        HLib_DestroyTask(g_pMovesScreenTask);
        g_pMovesScreenTask = nullptr;
    }

    UIBaseRuntime &moves = g_uiElements[static_cast<std::size_t>(kMovesElement)];
    if (moves.isVisible && moves.destroy)
        (void)moves.destroy();
    moves.isVisible = 0;

    g_moveListMoveIds[0] = 0;
    g_movesScreenRefreshState = 0;
    g_movesScreenTransitionFlags = 0;
    g_movesScreenViewActive = 0;
    g_movesScreenTransitionGate = 0;
    g_movesScreenTransitionOffset = -50.0f;
    g_pendingMovePopupMoveId = 0;
    g_pendingMovePopupAnimSetId = 0;
    g_movesScreenRequestedAuthorState = 0;
}

} // namespace shenmue::s1
