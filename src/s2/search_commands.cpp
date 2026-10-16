#include "s2/search_internal.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
namespace {
ConversationBuffer *searchDialogue{};
ConversationBuffer *randomSearchDialogue{};
char randomSearchBank[32]{};
constexpr shen_name searchButtons[4]{SHEN_NAME(BTCC), SHEN_NAME(BTEV), SHEN_NAME(BTFT), SHEN_NAME(BTIF)};
constexpr const char *searchMoveNames[26]{"Twin Blades", "Shadow Reaper", "Stab Armor", "Twin Swallow Leap", "Mud Spider", "Rising Flash", "Crawl Cyclone",
    "Tiger Storm", "Arm Break Fire", "Poetry Scroll", "Mysterious Scroll", "Dragon Spin", "Serpent Coil", "Rear Foot Sweep", "Diagonal Wipe", "Oblique Slam",
    "Twin Palm Thrust", "Horseshoe Kick", "Mantis Combo", "Stepping Strike", "Oblique Air Kick", "Heel Sweep", "Monkey Roll Drop", "Machinegun Fist",
    "Double Storm Kick", "Demon's Triangle"};
constexpr const char* searchMoveLabels[26]{"UISKILTWBL","UISKILSHRE","UISKILSTAR","UISKILTSL0","UISKILMUSP","UISKILRIFL","UISKILCRCY", // @text UISKILTWBL EN: "Twin Blades" | JA: "双刃の技書" ; UISKILSHRE EN: "Shadow Reaper" | JA: "影狩りの技書" ; UISKILSTAR EN: "Stab Armor" | JA: "鎧通の技書" ; UISKILTSL0 EN: "Twin Swallow Leap" | JA: "飛燕連刈の技書" ; UISKILMUSP EN: "Mud Spider" | JA: "土蜘蛛の技書" ; UISKILRIFL EN: "Rising Flash" | JA: "裂晃閃の技書" ; UISKILCRCY EN: "Crawl Cyclone" | JA: "旋風の技書"
    "UISKILTIST","UISKILABF0","UISKILPOSC","UISKILMYSC","UISKILDRSP","UISKILSECO","UISKILRFS0","UISKILDIWI","UISKILOBSL","UISKILTPT0", // @text UISKILTIST EN: "Tiger Storm" | JA: "虎身崩しの技書" ; UISKILABF0 EN: "Arm Break Fire" | JA: "朧無双の技書" ; UISKILPOSC EN: "Poetry Scroll" | JA: "謎の巻物" ; UISKILMYSC EN: "Mysterious Scroll" | JA: "歌訣の巻物" ; UISKILDRSP EN: "Dragon Spin" | JA: "降龍脚の技書" ; UISKILSECO EN: "Serpent Coil" | JA: "蛇身貼の技書" ; UISKILRFS0 EN: "Rear Foot Sweep" | JA: "大展伯の技書" ; UISKILDIWI EN: "Diagonal Wipe" | JA: "斜弧捶の技書" ; UISKILOBSL EN: "Oblique Slam" | JA: "斜歩劈斬の技書" ; UISKILTPT0 EN: "Twin Palm Thrust" | JA: "双掌打の技書"
    "UISKILHOKI","UISKILMACO","UISKILSTST","UISKILOAK0","UISKILHESW","UISKILMRD0","UISKILMAFI","UISKILDSK0","UISKILDETR"}; // @text UISKILHOKI EN: "Horseshoe Kick" | JA: "馬蹄脚の技書" ; UISKILMACO EN: "Mantis Combo" | JA: "連環劈の技書" ; UISKILSTST EN: "Stepping Strike" | JA: "順歩捶の技書" ; UISKILOAK0 EN: "Oblique Air Kick" | JA: "旋空脚の技書" ; UISKILHESW EN: "Heel Sweep" | JA: "伏身脚の技書" ; UISKILMRD0 EN: "Monkey Roll Drop" | JA: "白猿落の技書" ; UISKILMAFI EN: "Machinegun Fist" | JA: "連手砲の技書" ; UISKILDSK0 EN: "Double Storm Kick" | JA: "連環腿の技書" ; UISKILDETR EN: "Demon's Triangle" | JA: "閻王三点手の巻物"
std::uint16_t U16(const std::byte *pointer) {
    std::uint16_t result;
    std::memcpy(&result, pointer, 2);
    return result;
}
std::int16_t I16(const std::byte *pointer) {
    return std::bit_cast<std::int16_t>(U16(pointer));
}
std::uint32_t U32(const std::byte *pointer) {
    std::uint32_t result;
    std::memcpy(&result, pointer, 4);
    return result;
}
std::int32_t I32(const std::byte *pointer) {
    return std::bit_cast<std::int32_t>(U32(pointer));
}
std::uint8_t U8(const std::byte *pointer) {
    return std::to_integer<std::uint8_t>(*pointer);
}
MapControlData &Control() {
    return *static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
}
std::int32_t WaitCamera() {
    return g_searchPropIKSlots[searchPropIKIndex].active ? 1 : 3;
}
void ReleaseCommandControl(SearchResourceContext &context) {
    if (context.flags & 0x10U) {
        SetPlayerControlEnabled(1);
        context.flags &= ~0x10U;
    }
}
void AcquireCommandControl(SearchResourceContext &context) {
    if (SetPlayerControlEnabled(0))
        context.flags |= 0x10U;
}
const char *LocalizeMoveName(const char *text) {
    for (std::size_t index = 0; index < 26; ++index) {
        const unsigned char *input = reinterpret_cast<const unsigned char *>(text);
        const unsigned char *reference = reinterpret_cast<const unsigned char *>(searchMoveNames[index]);
        while (*input) {
            if (!*reference)
                break;
            if (std::isspace(*input))
                ++input;
            else {
                if (!std::isspace(*reference)) {
                    if (std::tolower(*input) != std::tolower(*reference))
                        break;
                    ++input;
                }
                ++reference;
            }
        }
        if (*input == *reference)
            return GetLocalizedSceneText(searchMoveLabels[index]).data();
    }
    return text;
}

// @verified - p
std::int32_t SearchCommandEnd(SearchTaskData &search) {
    activeSearchConfig = nullptr;
    search.context->cursor = nullptr;
    return 1;
}

// @verified - p
std::int32_t SearchCommandItem(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    if (!(context.flags & 1U)) {
        AddInventoryItem(U16(command + 2), U8(command + 4), U8(command + 5));
        srchFinishPropIKItem(U8(command + 5) != 0);
        if (!U8(command + 5)) {
            context.cursor = command + 8;
            return 2;
        }
        context.flags |= 1U;
        AcquireCommandControl(context);
    } else if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage)) {
        ReleaseCommandControl(context);
        context.flags &= ~1U;
        context.cursor = command + 8;
        return 2;
    }
    return 0;
}

// @verified - p
std::int32_t SearchCommandFlagRestore(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    if (shenEvaluateFlagFor(static_cast<std::int32_t>(search.currentTarget->name.name), static_cast<std::int8_t>(U16(command + 2))))
        srchSetLookMode();
    search.context->cursor = command + 4;
    return 2;
}

// @verified - p
std::int32_t SearchCommandFlagWait(SearchTaskData &search) {
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    shenEvaluateFlagFor(static_cast<std::int32_t>(search.currentTarget->name.name), static_cast<std::int8_t>(U16(search.context->cursor + 2)));
    return 1;
}

// @verified - p
std::int32_t SearchCommandChangeArea(SearchTaskData &search) {
    const std::byte *const command = search.context->cursor;
    shenQueueChangeModuleAndScene(SystemModule::Game, g_actNumber, shen_name{U32(command + 4)}, U16(command + 2));
    srchRequestRelease();
    search.context->cursor = nullptr;
    srchResetIK();
    return 1;
}

// @verified - p
std::int32_t SearchCommandFilm(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    if (!(context.flags & 2U)) {
        WaitForNextTask_0(Control().screenFilmTask, 1, 30);
        AcquireCommandControl(context);
        context.flags |= 2U;
        return 1;
    }
    if (DoesScreenFilmColourExist(Control().screenFilmTask))
        return 1;
    ReleaseCommandControl(context);
    context.flags &= ~2U;
    context.cursor = command + 8;
    return 2;
}

// @verified - p
std::int32_t SearchCommandIfStoryFlag(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    search.context->cursor = command + (EV_GetShenmueFlag(U16(command + 2)) ? 8 : U32(command + 4));
    return 2;
}

// @verified - p
std::int32_t SearchCommandMessage(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    if (context.flags & 4U) {
        context.remainingTicks -= 1;
        if (context.remainingTicks <= 0) {
            ReleaseCommandControl(context);
            scenDestroySubtitles(Control().scenarioTask);
            context.flags &= ~4U;
            context.cursor = command + 12;
            return 2;
        }
        return 1;
    }
    if (U16(command + 6)) {
        context.cursor = command + 16;
        return 2;
    }
    if (!U16(command + 4)) {
        const std::uint16_t one = 1;
        std::memcpy(command + 6, &one, 2);
    }
    if (!context.strings) {
        context.cursor = nullptr;
        return 1;
    }
    const char *message = reinterpret_cast<const char *>(context.strings + U16(command + 8));
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(Control().scenarioTask));
    scenario->windowTextState.scenarioStateFlags |= 0x40U;
    std::int32_t speaker = 2;
    if (message[0] == '\\' && message[1] == 'b') {
        message += 2;
        speaker = 3;
    }
    shenPrepareDialogue(Control().scenarioTask, 1, LocalizeMoveName(message), speaker);
    context.flags |= 4U;
    AcquireCommandControl(context);
    context.remainingTicks = U16(command + 2);
    return 1;
}

// @verified - p
std::int32_t SearchCommandLight(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    SetLightEntryIndex(U16(command + 2), U16(command + 4) != 0);
    search.context->cursor = command + 8;
    return 2;
}

// @verified - p
std::int32_t SearchCommandSetInternalFlag(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(U16(command + 2)), U16(command + 4) != 0);
    search.context->cursor = command + 8;
    return 2;
}

// @verified - p
std::int32_t SearchCommandRandomMessage(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    if (context.flags & 4U) {
        context.remainingTicks -= 1;
        if (context.remainingTicks <= 0) {
            ReleaseCommandControl(context);
            scenDestroySubtitles(Control().scenarioTask);
            context.flags &= ~4U;
            context.cursor = command + 20;
            return 2;
        }
        return 1;
    }
    if (U16(command + 6)) {
        context.cursor = command + 20;
        return 2;
    }
    if (!U16(command + 4)) {
        const std::uint16_t one = 1;
        std::memcpy(command + 6, &one, 2);
    }
    const char *choices[3];
    std::int32_t count = 0;
    for (std::size_t i = 0; i < 3; ++i) {
        const std::int32_t offset = I32(command + 8 + 4 * i);
        if (offset != -1)
            choices[count++] = reinterpret_cast<const char *>(context.strings + offset);
    }
    if (!count) {
        context.cursor = command + 20;
        return 2;
    }
    const std::int32_t index = static_cast<std::int32_t>(IncrementSharedPrngFloat() * count);
    SCEN_ShowEventMessage(Control().scenarioTask, 1, choices[index]);
    context.remainingTicks = U16(command + 2);
    context.flags |= 4U;
    AcquireCommandControl(context);
    return 1;
}

// @verified - p
std::int32_t SearchCommandDialogue(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    if (!(context.flags & 8U)) {
        if (U8(command + 3)) {
            context.cursor = command + 12;
            return 2;
        }
        if (!U8(command + 2))
            command[3] = std::byte{1};
        const char *const bank = reinterpret_cast<const char *>(context.strings + U32(command + 4));
        const std::uint32_t voice = U32(command + 8);
        char code[16];
        std::snprintf(code, sizeof(code), "%s%c%c%c%c", bank, static_cast<char>(voice), static_cast<char>(voice >> 8U), static_cast<char>(voice >> 16U),
            static_cast<char>(voice >> 24U));
        if (EV_FreeAuthorPak(1, bank)) {
            searchDialogue = EV_PlayDialogue(code);
            if (!searchDialogue)
                return 3;
            context.flags |= 8U;
            AcquireCommandControl(context);
        }
        return 1;
    }
    if (cvsHasValidConverseData(searchDialogue))
        return 1;
    ReleaseCommandControl(context);
    context.flags &= ~8U;
    context.cursor = command + 12;
    return 2;
}

// @verified - p
std::int32_t SearchCommandRandomDialogue(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    if (!(context.flags & 8U)) {
        if (U8(command + 3)) {
            context.cursor = command + 20;
            return 2;
        }
        if (!U8(command + 2))
            command[3] = std::byte{1};
        std::memset(randomSearchBank, 0, sizeof(randomSearchBank));
        randomSearchDialogue = nullptr;
        std::uint32_t choices[3]{};
        std::int32_t count = 0;
        for (std::size_t i = 0; i < 3; ++i) {
            const std::uint32_t choice = U32(command + 8 + 4 * i);
            if (choice)
                choices[count++] = choice;
        }
        if (count) {
            const std::int32_t index = static_cast<std::int32_t>(IncrementSharedPrngFloat() * count);
            const std::uint32_t voice = choices[index];
            std::snprintf(randomSearchBank, sizeof(randomSearchBank), "%s", reinterpret_cast<const char *>(context.strings + U32(command + 4)));
            char code[32];
            std::snprintf(code, sizeof(code), "%s%c%c%c%c", randomSearchBank, static_cast<char>(voice), static_cast<char>(voice >> 8U),
                static_cast<char>(voice >> 16U), static_cast<char>(voice >> 24U));
            if (EV_FreeAuthorPak(1, randomSearchBank)) {
                randomSearchDialogue = EV_PlayDialogue(code);
                context.flags |= 8U;
                AcquireCommandControl(context);
            }
        } else
            context.cursor = command + 20;
    } else if (!cvsHasValidConverseData(randomSearchDialogue)) {
        ReleaseCommandControl(context);
        context.flags &= ~8U;
        context.cursor = command + 20;
        return 2;
    }
    return 1;
}

// @verified - p
std::int32_t SearchCommandWaitProp(SearchTaskData &search) {
    const std::uint32_t active = g_searchPropIKSlots[searchPropIKIndex].active;
    if ((active & 1U) || !(active & 4U))
        return 0;
    search.context->cursor += 8;
    return 2;
}

// @verified - p
std::int32_t SearchCommandButtons(SearchTaskData &search) {
    if (g_searchPropIKSlots[searchPropIKIndex].active & 1U)
        return 0;
    std::byte *const command = search.context->cursor;
    const std::uint16_t mask = U16(command + 2);
    constexpr std::uint16_t tests[4]{0x210, 0x24, 0x400, 2};
    constexpr std::uint32_t mediation[4]{ButtonMediationBindY | ButtonMediationShowButtonHint, ButtonMediationBindA | ButtonMediationShowButtonHint,
        ButtonMediationBindX | ButtonMediationShowButtonHint, ButtonMediationBindB | ButtonMediationShowButtonHint};
    for (std::size_t i = 0; i < 4; ++i)
        if (mask & tests[i]) {
            if (btnCheck(searchButtons[i]) || (!i && searchExitRequested)) {
                search.context->cursor = command + 8;
                return 2;
            }
            RegisterButtonMediation(searchButtons[i], nullptr, mediation[i]);
        }
    search.context->cursor = command + U32(command + 4);
    if (I16(search.context->cursor) == 16 && SearchCommandButtons(search) == 2)
        return 2;
    search.context->cursor = command;
    return 0;
}

// @verified - p
std::int32_t SearchCommandBindProp(SearchTaskData &search) {
    srchBindPropIK(search);
    search.context->cursor += 4;
    return 2;
}

// @verified - p
std::int32_t SearchCommandReturnProp(SearchTaskData &search) {
    srchBeginPropIKReturn();
    search.context->cursor += 4;
    return 2;
}

// @verified - p
std::int32_t SearchCommandRestart(SearchTaskData &search) {
    activeSearchConfig = nullptr;
    search.context->cursor = nullptr;
    SearchPropIKSlot &slot = g_searchPropIKSlots[searchPropIKIndex];
    if (slot.active)
        slot.active |= 0x80U;
    else {
        if (!srchLockTarget(search, search.currentTarget->name, searchPropIKIndex == 0))
            srchRequestRelease();
        search.state &= ~4U;
    }
    return 1;
}

// @verified - p
std::int32_t SearchCommandChoices(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    constexpr std::int16_t choices[2][4]{{82, 83, 0, 0}, {80, 81, 0, 0}};
    const std::uint16_t row = U16(command + 2);
    if (context.flags & 0x20U) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 1;
        const std::uint32_t selected = shenGetCtrlSpeakIconFlag();
        ReleaseCommandControl(context);
        if (row == 1)
            CloseMoneyIcon();
        context.flags &= ~0x20U;
        context.cursor = command + (selected == static_cast<std::uint16_t>(choices[row][0]) ? 12 : U32(command + 8));
        return 2;
    }
    if (row > 1) {
        context.cursor = command + U32(command + 8);
        return 2;
    }
    std::int16_t options[4]{-1, choices[row][0], choices[row][1], -1};
    const std::int16_t timeout = U16(command + 4) ? I16(command + 4) : 32767;
    SetupControlSpeakAndUI_Internal(options, timeout, -1, U16(command + 6) ? 2 : 0, 0);
    if (row == 1) {
        AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
        KeepMoneyIconOpen();
    }
    AcquireCommandControl(context);
    context.flags |= 0x20U;
    return 1;
}

// @verified - p
std::int32_t SearchCommandDisableExamine(SearchTaskData &search) {
    search.state |= 8U;
    search.context->cursor += 4;
    return 2;
}

// @verified - p
std::int32_t SearchCommandStepFlag(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    if (camCtrlIsTransitionActive())
        return WaitCamera();
    EV_stepSetFlags(static_cast<std::uint8_t>(U16(command + 4)), U16(command + 2));
    search.context->cursor = command + 8;
    return 2;
}

// @verified - p
std::int32_t SearchCommandIfInternalFlag(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    search.context->cursor = command + (GetInternalFlag(U16(command + 2)) ? 8 : U32(command + 4));
    return 2;
}

// @verified - p
std::int32_t SearchCommandIfCash(SearchTaskData &search) {
    std::byte *const command = search.context->cursor;
    search.context->cursor = command + (GetCurrentCash() < I32(command + 4) ? U32(command + 8) : 12);
    return 2;
}

// @verified - p
std::int32_t SearchCommandTurnPlayer(SearchTaskData &search) {
    SearchResourceContext &context = *search.context;
    std::byte *const command = context.cursor;
    const std::uint16_t requested = U16(command + 2);
    MotionWork *const motion = FindMotionWork(static_cast<Character *>(Control().ryo));
    if (searchPropIKIndex || g_searchPropIKSlots[0].active) {
        context.cursor = command + 4;
        return 2;
    }
    Vector3 target, base;
    srchGetLookTarget(&target);
    srchGetLookBase(&base);
    Vector3 direction{base.x - target.x, base.y - target.y, base.z - target.z};
    const std::int16_t yaw = static_cast<std::int16_t>(atan2_fixed16(-direction.x, -direction.z));
    const std::int16_t delta = static_cast<std::int16_t>(requested - yaw);
    std::int16_t step = delta;
    if (step > 910)
        step = 910;
    if (step < -910)
        step = -910;
    if (delta || direction.y != 0) {
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(step));
        Matrix4x4::TransformPoint(direction, direction);
        Matrix4x4::Pop(1);
        base = {direction.x + target.x, target.y, direction.z + target.z};
        if (delta)
            base.y += (1 - std::fabs(static_cast<float>(step)) / std::fabs(static_cast<float>(delta))) * direction.y;
        srchSetLookBase(&base);
    }
    if (!(context.flags & 0x40U)) {
        mtwkRequestMotion(motion, 0, MotionRegion::LowerBody);
        CopyMotionTransition(motion, nullptr, requested, 13, 0);
        camCtrlSwitchToLockedOn(6);
        AcquireCommandControl(context);
        context.flags |= 0x40U;
    } else if (std::abs(static_cast<std::int32_t>(delta)) < 910 && std::fabs(direction.y) < std::numeric_limits<float>::epsilon() &&
        !camCtrlIsTransitionActive() && !mtwkSetExtraFlag(motion, 58, 1)) {
        context.flags &= ~0x40U;
        ReleaseCommandControl(context);
        srchRequestRelease();
        context.cursor = command + 4;
        return 2;
    }
    return 1;
}
using SearchCommand = std::int32_t (*)(SearchTaskData &);
constexpr SearchCommand commands[26]{SearchCommandEnd, SearchCommandItem, SearchCommandFlagRestore, SearchCommandFlagWait, SearchCommandChangeArea, nullptr,
    SearchCommandFilm, nullptr, SearchCommandIfStoryFlag, SearchCommandMessage, SearchCommandLight, SearchCommandSetInternalFlag, SearchCommandRandomMessage,
    SearchCommandDialogue, SearchCommandRandomDialogue, SearchCommandWaitProp, SearchCommandButtons, SearchCommandBindProp, SearchCommandReturnProp,
    SearchCommandRestart, SearchCommandChoices, SearchCommandDisableExamine, SearchCommandStepFlag, SearchCommandIfInternalFlag, SearchCommandIfCash,
    SearchCommandTurnPlayer};
}

// @verified - p
std::int32_t srchExecuteCommands(SearchTaskData &search) {
    if (!search.context->cursor)
        return 0;
    std::int16_t operation = I16(search.context->cursor);
    while (operation >= 0 && operation < 26) {
        const SearchCommand command = commands[operation];
        if (!command) {
            search.context->cursor = nullptr;
            activeSearchConfig = nullptr;
            return 0;
        }
        const std::int32_t result = command(search);
        if (!(result & ~3) && result != 2)
            return result;
        operation = I16(search.context->cursor);
    }
    return 0;
}

// @verified - p
bool srchProcessSearchIK(SearchTaskData &search) {
    CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
    const std::int32_t result = srchExecuteCommands(search);
    if (result == 1)
        return true;
    if (result == 3)
        return false;
    return srchUpdatePropIK(search);
}

} // namespace shenmue::s2
