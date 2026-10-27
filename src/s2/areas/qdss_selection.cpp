#include "s2/areas/area_qdss.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/item_wf.hpp"
#include "s2/motion.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstdlib>
namespace shenmue::s2::areas {
namespace {
constexpr float F(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}
void PlaceCarouselKey(shen_name character, float fraction) {
    const std::int32_t angle = TruncateToInt32(fraction * 65535.0f);
    const std::int16_t folded = static_cast<std::int16_t>(static_cast<std::uint32_t>(angle) <= 0x8000U ? angle : -angle);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const Vector3 position{-.15000001f * sine, 0, -.15000001f * cosine};
    const Vector3i rotation{0x4000, 0, 4551};
    const float value = 1.0f - static_cast<float>(std::abs(angle)) / 65535.0f;
    const Vector3 scale{value, value, value};
    AT_SetLocalBaseChara(character, QDSS_KEYS_BASE);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotation);
    mtwkSetMotionworkScale3(character, &scale);
    EVT_AT_ObjectDisp_1(character, 0, 0);
}
void DescribeSelectedKey(QdssWork &work) {
    std::int32_t delay = 0;
    switch (work.selected) {
    case 0:
        EVT_AT_SetVoice("0247A067", 0); // @text EN: "This is=@" | JA: "この棒は…"
        EVT_AT_SetVoice("0247A171", 40); // @text EN: "The one hidden beneath the uniform." | JA: "胴着の下に隠してあった棒だ。"
        delay = 100;
        break;
    case 1:
        EVT_AT_SetVoice("0247A168", 0); // @text EN: "Zhang gave me this one." | JA: "これは、張さんからもらったものだ。"
        delay = 90;
        break;
    case 2:
        EVT_AT_SetVoice("0247A043", 0); // @text EN: "This is=@" | JA: "これは…"
        EVT_AT_SetVoice("0247A169", 30); // @text EN: "The one I found behind the frame\nof the Yin and Yang." | JA: "八陰八陽の、額の裏にあったやつだ。"
        delay = 110;
        break;
    case 3:
        EVT_AT_SetVoice("0247A043", 0); // @text EN: "This is=@" | JA: "これは…"
        EVT_AT_SetVoice("0247A170", 30); // @text EN: "It was inside the jar." | JA: "ツボの中に入っていたやつだ。"
        delay = 100;
        break;
    case 4:
        EVT_AT_SetVoice("0247A067", 0); // @text EN: "This is=@" | JA: "この棒は…"
        EVT_AT_SetVoice("0247A224", 40); // @text EN: "The one on the desk." | JA: "机の上にあった棒だな。"
        delay = 110;
        break;
    case 5:
        EVT_AT_SetVoice("0247A043", 0); // @text EN: "This is=@" | JA: "これは…"
        EVT_AT_SetVoice("0247A190", 30); // @text EN: "The one inside the drawer." | JA: "引き出しの中にあった棒だ。"
        delay = 120;
        break;
    case 6:
        EVT_AT_SetVoice("0247A067", 0); // @text EN: "This is=@" | JA: "この棒は…"
        EVT_AT_SetVoice("0247A170", 40); // @text EN: "It was inside the jar." | JA: "ツボの中に入っていたやつだ。"
        delay = 110;
        break;
    case 7:
        EVT_AT_SetVoice("0247A043", 0); // @text EN: "This is=@" | JA: "これは…"
        EVT_AT_SetVoice("0247A174", 30); // @text EN: "The one under the turtle shell." | JA: "亀の甲羅の下に置いてあった棒だ。"
        delay = 95;
        break;
    default:
        break;
    }
    const Vector3 scale{1, 1, 1};
    for (std::int32_t index = 0; index < 8; ++index)
        if (work.available[index].owned) {
            EVT_AT_ObjectDisp_1(work.keys[index].character, delay, 1);
            QueueQdssScale(work.keys[index].character, scale, delay);
        }
    if (delay)
        HLib::SetupTaskDelay(delay);
}
bool OutsideOpeningHours() {
    DateTime date;
    GetCurrentDateTime(date);
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(date.hour) - 2) <= 2U;
}
void RegisterSelectionButtons(bool diagram) {
    GetBattleDataConfig(0, SHEN_NAME(BTEV));
    GetBattleDataConfig(0, SHEN_NAME(BTCC));
    if (diagram)
        GetBattleDataConfig(0, SHEN_NAME(BTIL));
}
} // namespace
bool PrepareQdssKeySelection(QdssWork &work) {
    work.itemCount = 0;
    for (std::int32_t index = 0; index < 8; ++index) {
        if (GetItemState(static_cast<std::uint32_t>(work.keys[index].item))) {
            work.available[index].owned = 1;
            work.available[index].ordinal = work.itemCount++;
        } else
            work.available[index].owned = 0;
    }
    if (!work.itemCount) {
        QdssFade(30, true, 0);
        HLib::SetupTaskDelay(30);
        return false;
    }
    EV_DisableCharacterDisplay(QDSS_HAND);
    EV_DisableCharacterDisplay(QDSS_RYO);
    EVT_AT_SetMot(QDSS_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1020, QDSS_BASE, QDSS_BASE, 0, 0, 0);
    for (std::int32_t index = 0; index < 4; ++index)
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(work.keys[index].flag)))
            work.inserted |= 1U << index;
    if (!work.available[work.selected].owned) {
        for (std::int32_t index = 0; index < 8; ++index)
            if (work.available[index].owned) {
                work.selected = index;
                break;
            }
    }
    const Vector3 position{F(0xbcdd2f1b), F(0x3fc56042), -.99199998f};
    const Vector3i rotation{910, 30219, 0};
    AT_SetLocalBaseChara(QDSS_KEYS_BASE, QDSS_BASE);
    EV_AT_SetPos(QDSS_KEYS_BASE, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(QDSS_KEYS_BASE, EventTransformFlags::AllAxes, rotation);
    for (std::int32_t index = 0; index < 8; ++index)
        if (work.available[index].owned) {
            const float fraction =
                static_cast<float>(work.itemCount + work.available[work.selected].ordinal - work.available[index].ordinal) / static_cast<float>(work.itemCount);
            PlaceCarouselKey(work.keys[index].character, fraction);
        }
    work.slide = 0;
    work.remaining = 1800;
    return true;
}
std::int32_t UpdateQdssKeySelection(QdssWork &work) {
    if (IsModeBitActive(1))
        return 100;
    for (std::int32_t index = 0; index < 8; ++index)
        if (work.available[index].owned)
            EV_EnableCharacterDisplay(work.keys[index].character);
    const bool closing = OutsideOpeningHours();
    if (closing || work.remaining < 1) {
        work.selected = 8;
        DescribeSelectedKey(work);
        QdssFade(30, true, 0);
        HLib::SetupTaskDelay(30);
        return closing ? -1 : 0;
    }
    const bool diagram = GetItemState(10) != 0;
    CheckHelpIcon(49185, 0, CheckHelpIconOp::FrameOverlay);
    EV_EventSetUpHelpIcon_0(diagram ? 114 : 115, 0);
    if (work.slide >= 1) {
        --work.slide;
        for (std::int32_t index = 0; index < 8; ++index)
            if (work.available[index].owned) {
                float fraction = static_cast<float>(work.itemCount + work.available[work.selected].ordinal - work.available[index].ordinal) /
                    static_cast<float>(work.itemCount);
                if (work.itemCount != 1) {
                    const float offset = static_cast<float>(work.slide) / static_cast<float>(10 * work.itemCount);
                    fraction += work.direction ? offset : -offset;
                }
                PlaceCarouselKey(work.keys[index].character, fraction);
            }
        RegisterSelectionButtons(diagram);
    } else {
        GetBattleDataConfig(0, SHEN_NAME(BTEV));
        if (GetBattleDataConfig(2, SHEN_NAME(BTEV))) {
            DescribeSelectedKey(work);
            return 1;
        }
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        if (GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
            work.selected = 8;
            DescribeSelectedKey(work);
            QdssFade(30, true, 0);
            HLib::SetupTaskDelay(30);
            return 0;
        }
        const std::uint32_t input = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_REPEAT));
        if (input & 0x8000U) {
            std::int32_t index = (work.selected + 1) % 8;
            while (!work.available[index].owned)
                index = (index + 1) % 8;
            work.selected = index;
            work.slide = 10;
            work.direction = 0;
            return 100;
        }
        if (input & 0x4000U) {
            std::int32_t index = (work.selected + 7) % 8;
            while (!work.available[index].owned)
                index = (index + 7) % 8;
            work.selected = index;
            work.slide = 10;
            work.direction = 1;
            return 100;
        }
        if (diagram) {
            GetBattleDataConfig(0, SHEN_NAME(BTIL));
            if (GetBattleDataConfig(2, SHEN_NAME(BTIL))) {
                UpdateSelectedItemStateFlag(10);
                for (std::int32_t index = 0; index < 8; ++index)
                    if (work.available[index].owned)
                        EV_DisableCharacterDisplay(work.keys[index].character);
                return 100;
            }
        }
    }
    --work.remaining;
    return 100;
}
void PrepareQdssHoleSelection(QdssWork &work) {
    EV_DisableCharacterDisplay(QDSS_RYO);
    EventSetMotion motion{.character = QDSS_RYO, .animation = 0xf003, .speed = 1, .flags = 0x40000000U};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0xf003);
    work.hole = 8;
    for (std::int32_t index = 3; index >= 0; --index)
        if (!(work.inserted & (1U << index))) {
            work.hole = index;
            break;
        }
    const QdssKey &key = work.keys[work.hole];
    AT_SetLocalBaseChara(QDSS_KEYS_BASE, QDSS_BASE);
    EV_AT_SetPos(QDSS_KEYS_BASE, EventTransformFlags::AllAxes, key.position);
    EV_AT_SetAngY(QDSS_KEYS_BASE, EventTransformFlags::AllAxes, key.rotation);
    const Vector3 zero{0, 0, 0};
    const Vector3i keyRotation{0, 0, 57344};
    const QdssKey &selected = work.keys[work.selected];
    AT_SetLocalBaseChara(selected.character, QDSS_KEYS_BASE);
    EV_AT_SetPos(selected.character, EventTransformFlags::AllAxes, zero);
    EV_AT_SetAngY(selected.character, EventTransformFlags::AllAxes, keyRotation);
    EV_EnableCharacterDisplay(selected.character);
    Character *hand = ctrlGetCharaByName(QDSS_HAND);
    SetLowQualityHands(hand);
    SetHighModelXYZ(hand, 1, 1, 0);
    ApplyQdssKeyGrip(QDSS_HAND, 1);
    const Vector3i handRotation{8736, 4004, 0};
    ApplyCharacterHandRotation(hand, 0, 21, &handRotation);
    EV_EnableCharacterDisplay(QDSS_HAND);
    const Vector3 handPosition{F(0x3e3126e9), F(0x3e3851ec), -.58399999f};
    const Vector3i baseRotation{0, 0x8000, 0};
    AT_SetLocalBaseChara(QDSS_HAND, QDSS_KEYS_BASE);
    EV_AT_SetPos(QDSS_HAND, EventTransformFlags::AllAxes, handPosition);
    EV_AT_SetAngY(QDSS_HAND, EventTransformFlags::AllAxes, baseRotation);
    work.slide = 0;
    work.remaining = 1800;
    EVS_LoopCamera(1010, QDSS_HAND, QDSS_HAND);
}
std::int32_t UpdateQdssHoleSelection(QdssWork &work) {
    if (OutsideOpeningHours() || work.remaining < 1) {
        work.hole = 8;
        return -1;
    }
    CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
    EV_EventSetUpHelpIcon_0(113, 0);
    if (work.slide >= 1) {
        const QdssKey &key = work.keys[work.hole];
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(QDSS_KEYS_BASE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(QDSS_KEYS_BASE, &rotation, EventTransformFlags::Local);
        const float divisor = static_cast<float>(work.slide);
        position.x += (key.position.x - position.x) / divisor;
        position.y += (key.position.y - position.y) / divisor;
        position.z += (key.position.z - position.z) / divisor;
        rotation.x += (key.rotation.x - rotation.x) / work.slide;
        rotation.y += (key.rotation.y - rotation.y) / work.slide;
        rotation.z += (key.rotation.z - rotation.z) / work.slide;
        EV_AT_SetPos(QDSS_KEYS_BASE, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(QDSS_KEYS_BASE, EventTransformFlags::AllAxes, rotation);
        --work.slide;
        RegisterSelectionButtons(false);
    } else {
        GetBattleDataConfig(0, SHEN_NAME(BTEV));
        if (GetBattleDataConfig(2, SHEN_NAME(BTEV)))
            return 1;
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        if (GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
            work.hole = 8;
            return 0;
        }
        const std::uint32_t input = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_PRESSED));
        if ((input & 0x1000U) && work.hole != 1 && !(work.inserted & 2U)) {
            work.hole = 1;
            work.slide = 10;
            return 100;
        }
        if ((input & 0x8000U) && work.hole != 2 && !(work.inserted & 4U)) {
            work.hole = 2;
            work.slide = 10;
            return 100;
        }
        if ((input & 0x4000U) && work.hole != 0 && !(work.inserted & 1U)) {
            work.hole = 0;
            work.slide = 10;
            return 100;
        }
        if ((input & 0x2000U) && work.hole != 3 && !(work.inserted & 8U)) {
            work.hole = 3;
            work.slide = 10;
            return 100;
        }
    }
    --work.remaining;
    return 100;
}
} // namespace shenmue::s2::areas
