#include "s2/areas/area_ro10.hpp"
#include "s2/areas/ro10_hand_data.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr float F(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}
const std::int32_t commonSmall[]{4077, 4078, 4079, 4080, 4081, 4082, 4087, 4088};
const std::int32_t commonLarge[]{4072, 4073, 4074, 4075, 4076, 4083, 4084, 4085, 4086};
const std::int32_t rareSmall[]{4162, 4163, 4164}, rareLarge[]{4068, 4069, 4070};
const std::int32_t prize0Special[]{4019, 4122}, prize0Normal[]{4066, 4067, 4094, 4095, 4096, 4097, 4098, 4099};
const std::int32_t prize1Special[]{4222, 4223}, prize1Normal[]{4224, 4225, 4226, 4232, 4233, 4235, 4236, 4237, 4238, 4242};
const std::int32_t prize2Special[]{4016, 4017}, prize2Normal[]{4212, 4213, 4214, 4215, 4216, 4217, 4218, 4219, 4220, 4221};
const std::int32_t prize3Normal[]{4202, 4203, 4204, 4205, 4206, 4207, 4208, 4209, 4210, 4211};
const std::int32_t prize45Special[]{4044, 4045}, prize4Normal[]{4025, 4027, 4029, 4031, 4033, 4035, 4037, 4039, 4041, 4043};
const std::int32_t prize5Normal[]{4024, 4026, 4028, 4030, 4032, 4034, 4036, 4038, 4040, 4042};
struct PrizePool {
    const std::int32_t *common;
    std::int32_t commonCount;
    const std::int32_t *special;
    const std::int32_t *normal;
    std::int32_t normalCount;
    const std::int32_t *rare;
};
constexpr PrizePool pools[]{{.common = commonSmall, .commonCount = 8, .special = prize0Special, .normal = prize0Normal, .normalCount = 8, .rare = rareSmall},
    {.common = commonLarge, .commonCount = 9, .special = prize1Special, .normal = prize1Normal, .normalCount = 10, .rare = rareLarge},
    {.common = commonSmall, .commonCount = 8, .special = prize2Special, .normal = prize2Normal, .normalCount = 10, .rare = rareSmall},
    {.common = commonLarge, .commonCount = 9, .special = prize2Special, .normal = prize3Normal, .normalCount = 10, .rare = rareLarge},
    {.common = commonSmall, .commonCount = 8, .special = prize45Special, .normal = prize4Normal, .normalCount = 10, .rare = rareSmall},
    {.common = commonLarge, .commonCount = 9, .special = prize45Special, .normal = prize5Normal, .normalCount = 10, .rare = rareLarge}};
bool AwardNextItem(Ro10Work &work) {
    for (std::int32_t index = 0; index < 3; ++index)
        if (work.awards[index]) {
            AddInventoryItem(static_cast<std::uint32_t>(work.awards[index]), 1, 1);
            work.awards[index] = 0;
            return true;
        }
    return false;
}
void MoveShopHand(Ro10Work &work, const Vector3 &offset, bool retract) {
    const Ro10Shop &shop = *work.definition;
    Vector3 position;
    Vector3i rotation;
    GetRo10ItemTransform(shop, work.column, work.row, &position, &rotation, nullptr, false);
    const float step = static_cast<float>(retract ? 320 - work.handFrame : work.handFrame - 100);
    const float x = Ro10FlushSmall(Ro10FlushSmall(offset.x * .050000001f) * step);
    const float y = Ro10FlushSmall(Ro10FlushSmall(offset.y * .050000001f) * step);
    const float z = Ro10FlushSmall(Ro10FlushSmall(offset.z * .050000001f) * step);
    Vector3 translated;
    if (retract) {
        const float elapsed = static_cast<float>(work.handFrame - 300);
        translated.x = Ro10FlushSmall(Ro10FlushSmall(Ro10FlushSmall(elapsed * .02f) + .07f) + x);
        translated.y = Ro10FlushSmall(Ro10FlushSmall(Ro10FlushSmall(elapsed * .0099999998f) + .60000002f) + y);
    } else {
        const float elapsed = static_cast<float>(120 - work.handFrame);
        translated.x = Ro10FlushSmall(Ro10FlushSmall(Ro10FlushSmall(elapsed * .0099999998f) + .07f) + x);
        translated.y = Ro10FlushSmall(y + .60000002f);
    }
    translated.z = Ro10FlushSmall(z + .16f);
    const Vector3i handRotation{32768, 27306 - shop.rotation.y, 32768};
    ApplyScenarioTranslationAndRotation(&position, &shop.rotation, &translated);
    EV_AT_SetPos(RO10_HAND, EventTransformFlags::World | EventTransformFlags::AllAxes, translated);
    EV_AT_SetAngY(RO10_HAND, EventTransformFlags::World | EventTransformFlags::AllAxes, handRotation);
}
} // namespace
void PrepareRo10Award(Ro10Work &work) {
    const std::int32_t index = work.column + work.row * work.definition->columns;
    work.handFrame = 0;
    work.grip = work.definition->grips[index];
    const std::int32_t item = work.definition->items[index];
    if (item >= 0) {
        work.awards[0] = item;
        work.awards[1] = 0;
        return;
    }
    const std::int32_t category = item + 6;
    std::int32_t count = 0;
    while (count < 3) {
        const std::int32_t chance = shenGetRandomInt32(100);
        std::int32_t selected = 0;
        if (static_cast<std::uint32_t>(category) <= 5U) {
            const PrizePool &pool = pools[category];
            if (chance < 20)
                selected = pool.common[shenGetRandomInt32(pool.commonCount)];
            else if (chance < 22)
                selected = pool.special[shenGetRandomInt32(2)];
            else if (chance < 90)
                selected = pool.normal[shenGetRandomInt32(pool.normalCount)];
            else
                selected = pool.rare[shenGetRandomInt32(3)];
        }
        if (!selected) {
            while (count < 3)
                work.awards[count++] = 0;
            return;
        }
        std::int32_t found = 0;
        while (found < count && work.awards[found] != selected)
            ++found;
        if (found >= count)
            work.awards[count++] = selected;
    }
}
void UpdateRo10Hand(Ro10Work &work) {
    const Ro10Shop &shop = *work.definition;
    const std::int32_t index = work.column + work.row * shop.columns;
    const Vector3 offset = work.grip == 1 ? Vector3{F(1008981770U), F(3178611343U), 0} :
        work.grip == 2                    ? Vector3{F(1022739087U), F(3178611343U), F(3170222735U)} :
                                            Vector3{0, 0, 0};
    if (work.handFrame == 0) {
        work.flags |= 0x10U;
        if (work.cameraDistance > .49900001f && work.cameraDistance < .50099999f) {
            work.handFrame = 100;
            EV_EnableCharacterDisplay(RO10_HAND);
            EV_EnsureCharacterDraw(RO10_HAND);
        } else {
            work.cameraDistance = Ro10FlushSmall(work.cameraDistance);
            work.cameraDistance = Ro10FlushSmall(work.cameraDistance * .80000001f);
            work.cameraDistance = Ro10FlushSmall(work.cameraDistance + .1f);
            UpdateRo10ItemCamera(shop, work.column, work.row, work.cameraDistance, 0);
            return;
        }
    }
    if (work.handFrame >= 100 && work.handFrame <= 120) {
        Character *hand = ctrlGetCharaByName(RO10_HAND);
        const Vector3i rotation = work.grip == 1 ? Vector3i{57891, 61714, 364} : work.grip == 2 ? Vector3i{53704, 60622, 2184} : Vector3i{52976, 61350, 3458};
        ApplyCharacterHandRotation(hand, 0, 21, &rotation);
        if (work.handFrame == 100) {
            std::int32_t(*pose)[3] = work.grip == 1 ? ro10ReachSmall : work.grip == 2 ? ro10ReachTall : ro10ReachPacket;
            ApplyCharacterHandPoseCommand(hand, 1, reinterpret_cast<std::byte *>(pose), 1);
        } else if (work.handFrame == 101) {
            std::int32_t(*pose)[3] = work.grip == 1 ? ro10GripSmall : work.grip == 2 ? ro10GripTall : ro10GripPacket;
            ApplyCharacterHandPoseCommand(hand, 1, reinterpret_cast<std::byte *>(pose), 30);
        }
        MoveShopHand(work, offset, false);
        ++work.handFrame;
        return;
    }
    if (work.handFrame >= 121 && work.handFrame <= 130) {
        ++work.handFrame;
        return;
    }
    if (work.handFrame == 131) {
        work.handFrame = 200;
        return;
    }
    if (work.handFrame == 200) {
        AT_SetLocalBaseChara(shop.objects[index], RO10_HAND);
        work.handFrame = 300;
        return;
    }
    if (work.handFrame >= 300 && work.handFrame <= 320) {
        if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
            AwardNextItem(work);
        MoveShopHand(work, offset, true);
        if (++work.handFrame == 321)
            work.handFrame = 330;
        return;
    }
    if (work.handFrame == 330) {
        if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage) && !AwardNextItem(work)) {
            work.handFrame = 400;
            AT_SetLocalBaseChara(shop.objects[index], SHEN_NAME($fre));
            EV_DisableCharacterDisplay(RO10_HAND);
            EV_DisableCharacterDisplay(shop.objects[index]);
        }
        return;
    }
    if (work.handFrame == 400) {
        work.handFrame = 500;
        HLib::SetupTaskDelay(-1);
        PlayRo10Payment(work.shop, false);
        return;
    }
    if (work.handFrame == 500) {
        AddThisToCashOrSegaCoins(-shop.prices[index], ECashOrCoins::Cash2);
        work.handFrame = 600;
        return;
    }
    if (work.handFrame == 600) {
        work.handFrame = 700;
        HLib::SetupTaskDelay(-1);
        PlayRo10Payment(work.shop, true);
        return;
    }
    if (work.handFrame == 700 && !GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
        HLib::SetCurrentTaskTo(Ro10EnterItems);
}
} // namespace shenmue::s2::areas
