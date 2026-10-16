#include "s2/street_fight.hpp"
#include "s2/street_fight_tables.hpp"
#include "s2/area_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/ui.hpp"
#include "s2/sound_program.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/street_fight_conversation.hpp"

#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {

std::int32_t g_streetFightBusy{1};
std::int32_t g_streetFightCompletionSignal{1};
std::int32_t streetFightMaximumOpponents{};
shen_name streetFightLoadCharacters[2]{};
namespace {
std::int32_t streetFightActionChoicePhase{};
}

// @verified - p
void SetSFCharaForCurrentArea(void *, StreetFightAreaSelection *selection) {
    shen_name area{};
    std::int32_t scene = 99;
    switch (g_currentMap.name) {
    case STREET_FIGHT_AB00.name:
        area = STREET_FIGHT_SFBR;
        scene = 0;
        break;
    case STREET_FIGHT_Q100.name:
        area = STREET_FIGHT_SFKK;
        scene = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{66}, 0, -1));
        break;
    case STREET_FIGHT_QTB1.name:
        area = STREET_FIGHT_SFTS;
        scene = 0;
        break;
    case STREET_FIGHT_QSFA.name:
        (void)EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 731);
        area = STREET_FIGHT_SFM2;
        scene = 0;
        break;
    case STREET_FIGHT_QSFB.name:
        (void)EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 715);
        area = STREET_FIGHT_SFS2;
        scene = 0;
        break;
    case STREET_FIGHT_QSFC.name:
        (void)EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 741);
        area = STREET_FIGHT_SFKM;
        scene = 0;
        break;
    case STREET_FIGHT_QSFD.name:
        (void)EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 719);
        area = STREET_FIGHT_SFTC;
        scene = 0;
        break;
    case STREET_FIGHT_QGBT.name:
        area = STREET_FIGHT_SFWF;
        scene = 0;
        break;
    case STREET_FIGHT_QJBT.name:
        area = STREET_FIGHT_SFJF;
        scene = 0;
        break;
    case STREET_FIGHT_QKBT.name:
        area = STREET_FIGHT_SFLU;
        scene = 0;
        break;
    case STREET_FIGHT_QLBT.name:
        area = STREET_FIGHT_SFSR;
        scene = 0;
        break;
    default:
        break;
    }
    selection->area = area;
    selection->scene = scene;
    selection->reserved = 0;
}
// @verified - p
std::uint8_t GetHelpIconForArea() {
    const shen_name area = g_currentMap;
    const bool bestOfThree = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) != 0;
    switch (area.name) {
    case STREET_FIGHT_Q100.name:
        return STREET_FIGHT_HELP_ACROBAT;
    case STREET_FIGHT_AB00.name:
        return STREET_FIGHT_HELP_BEAST;
    case STREET_FIGHT_QTB1.name:
        return STREET_FIGHT_HELP_IRON_THRUST;
    case STREET_FIGHT_QSFA.name:
        return STREET_FIGHT_HELP_MONGOLIAN;
    case STREET_FIGHT_QSFB.name:
        return STREET_FIGHT_HELP_PASSIVE_RAID;
    case STREET_FIGHT_QSFC.name:
        return STREET_FIGHT_HELP_ONE_EYED;
    case STREET_FIGHT_QSFD.name:
        return STREET_FIGHT_HELP_IRON_HEAD;
    case STREET_FIGHT_QGBT.name:
        return bestOfThree ? STREET_FIGHT_HELP_GREG_THREE : STREET_FIGHT_HELP_GREG;
    case STREET_FIGHT_QJBT.name:
        return bestOfThree ? STREET_FIGHT_HELP_ROD_THREE : STREET_FIGHT_HELP_ROD;
    case STREET_FIGHT_QKBT.name:
        return bestOfThree ? STREET_FIGHT_HELP_BAIHU_THREE : STREET_FIGHT_HELP_BAIHU;
    case STREET_FIGHT_QLBT.name:
        return bestOfThree ? STREET_FIGHT_HELP_CHUNYAN_THREE : STREET_FIGHT_HELP_CHUNYAN;
    default:
        return STREET_FIGHT_HELP_FREE;
    }
}

// @verified - p
std::int64_t CanPerformForCurrentAreaFlags() {
    const shen_name area = g_currentMap;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) != 0)
        return 0;
    std::int32_t result = 0;
    switch (area.name) {
    case STREET_FIGHT_Q100.name:
        result = EV_GetShenmueFlag(0x25a) != 0;
        break;
    case STREET_FIGHT_QTB1.name:
        result = EV_GetShenmueFlag(0x259) != 0;
        break;
    case STREET_FIGHT_QSFA.name:
        result = EV_GetShenmueFlag(0x25b) != 0;
        break;
    case STREET_FIGHT_QSFB.name:
        result = EV_GetShenmueFlag(0x25c) != 0;
        break;
    case STREET_FIGHT_QSFC.name:
        result = EV_GetShenmueFlag(0x25d) != 0;
        break;
    case STREET_FIGHT_QSFD.name:
        result = EV_GetShenmueFlag(0x25e) != 0;
        break;
    case STREET_FIGHT_QGBT.name:
        if (EV_GetShenmueFlag(0x126) != 0)
            result = 2;
        break;
    case STREET_FIGHT_QJBT.name:
        if (EV_GetShenmueFlag(0x127) != 0)
            result = 2;
        break;
    case STREET_FIGHT_QLBT.name:
        if (EV_GetShenmueFlag(0x128) != 0)
            result = 2;
        break;
    default:
        break;
    }
    return result == 1 ? 1 : result == 2 ? 2 : 0;
}

// @verified - p
void StartStreetFightForArea(StreetFightStartParameters *parameters) {
    const shen_name area = parameters->area;
    if (area.name == 0)
        return;
    SetPlayerControlEnabled(0);
    parameters->signal = &g_streetFightCompletionSignal;
    parameters->reserved18 = 0;
    parameters->flags = 0;
    if ((GetFightModeFlags() & 8) != 0)
        parameters->flags |= 0x10;
    parameters->flags |= 1;
    char eventName[16];
    std::memcpy(eventName, &area.name, 4);
    std::memcpy(eventName + 4, ".EV3", 5);
    (void)LoadAreaFunc_Wrapper_0(eventName, 0);
    (void)ExecuteCurrAreaMainFunc(eventName, reinterpret_cast<std::uintptr_t>(parameters));
}

// @verified - p
void PrepareStreetFightPlayerAngle() {
    Vector3 position;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const float x = 266.22f - position.x;
    const float z = 526.8f - position.z;
    const float distance = std::sqrt(z * z + x * x);
    EventSetAngle request{.cleanup = 0,
        .character = STREET_FIGHT_RYO,
        .parent = STREET_FIGHT_INVALID,
        .degrees = static_cast<float>(InverseFixedAtan2(x / distance, z / distance)) / 182.04445f - 20.0f,
        .delay = 0.0f};
    EVT_AT_SetAngY_Callback(&request);
}

// @verified - p
std::int64_t RestoreStreetFightPlayer(void *parameters) {
    std::byte *const bytes = static_cast<std::byte *>(parameters);
    EventReadCharactersWork *characters;
    HLib::Task *audience;
    std::memcpy(&characters, bytes + 0x88, sizeof(characters));
    std::memcpy(&audience, bytes + 0x98, sizeof(audience));
    ET_AT_KillWork(characters);
    if (DoesScenarioCharacterExist(STREET_FIGHT_CNT1))
        SetScenarioCharacterKillMode(STREET_FIGHT_CNT1, 1);
    if (DoesScenarioCharacterExist(STREET_FIGHT_CNT2))
        SetScenarioCharacterKillMode(STREET_FIGHT_CNT2, 1);
    SetPlayerControlEnabled(0);
    const std::int64_t result = EV_FreeAuthorPak(0, "0100");
    if (!audience)
        return result;
    (void)HLib::GetTaskParameter(audience);
    return HLib::DestroyTaskInternal(audience);
}

// @verified - p
HLib::Task *QueueStreetFightAudience() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightAudienceCallback, 6, 0x0b, sizeof(StreetFightAudienceWork), STREET_FIGHT_EVENT_TASK);
    StreetFightAudienceWork *work = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->reserved22 = 0;
    work->mode = 0;
    work->actionFrames = 0;
    work->soundFrequency = 2000;
    work->soundVolume = 90;
    work->radius = 4.75f;
    work->radiusRange = 2.0f;
    work->corridorStart = Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    work->corridorEnd = Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    work->corridorLength = 0.0f;
    work->soundCentre = Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    work->entityCount = 30;
    work->loopDelay = 0;
    work->lastSound = 0;
    switch (g_currentMap.name) {
    case STREET_FIGHT_AB00.name:
        work->characterNames = STREET_FIGHT_AB00_AUDIENCE + 1;
        break;
    case STREET_FIGHT_QSFB.name:
        work->characterNames = STREET_FIGHT_QSFB_AUDIENCE;
        break;
    case STREET_FIGHT_QGBT.name:
        work->characterNames = STREET_FIGHT_QGBT_AUDIENCE;
        break;
    default:
        work->characterNames = STREET_FIGHT_AB00_AUDIENCE;
        break;
    }
    if (task) {
        work = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(task));
        work->fighters[0].name = 0;
        work->fighters[1].name = 0;
        work->fighters[2].name = 0;
        work->fighters[3].name = 0;
        work->fighterCount = 0;
    }
    return task;
}

// @verified - p
HLib::Task *QueueStreetFightAudienceCount(std::uint8_t count) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightAudienceCallback, 6, 0x0b, sizeof(StreetFightAudienceWork), STREET_FIGHT_EVENT_TASK);
    StreetFightAudienceWork *work = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->entityCount = count < 30 ? count : 30;
    work->state = 0;
    work->reserved22 = 0;
    work->mode = 0;
    work->actionFrames = 0;
    work->soundFrequency = 2000;
    work->soundVolume = 90;
    work->radius = 4.75f;
    work->radiusRange = 2.0f;
    work->corridorStart = Vector3{};
    work->corridorEnd = Vector3{};
    work->corridorLength = 0.0f;
    work->soundCentre = Vector3{};
    work->loopDelay = 0;
    work->lastSound = 0;
    switch (g_currentMap.name) {
    case STREET_FIGHT_AB00.name:
        work->characterNames = STREET_FIGHT_AB00_AUDIENCE + 1;
        break;
    case STREET_FIGHT_QSFB.name:
        work->characterNames = STREET_FIGHT_QSFB_AUDIENCE;
        break;
    case STREET_FIGHT_QGBT.name:
        work->characterNames = STREET_FIGHT_QGBT_AUDIENCE;
        break;
    default:
        work->characterNames = STREET_FIGHT_AB00_AUDIENCE;
        break;
    }
    if (task) {
        work = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(task));
        work->fighters[0].name = 0;
        work->fighters[1].name = 0;
        work->fighters[2].name = 0;
        work->fighters[3].name = 0;
        work->fighterCount = 0;
    }
    return task;
}

// @verified - p
void StreetFightAudienceCallback(void *taskData) {
    StreetFightAudienceWork *const work = static_cast<StreetFightAudienceWork *>(taskData);
    switch (work->state) {
    case 0:
    case 1:
    case 2:
        work->state = 2;
        for (std::int32_t entity = 0; entity < work->entityCount; ++entity)
            (void)FindStreetFightAudienceSpace(work, entity);
        work->state = 3;
        return;
    case 3:
        PlayStreetFightAudienceArrivalSounds(work);
        if (IsFightActive() != 0)
            work->state = 5;
        return;
    case 4:
        PlayStreetFightAudienceOpeningSounds(work);
        if (IsFightActive() != 0)
            work->state = 5;
        return;
    case 5:
        if (IsFightActive() == 0)
            return;
        PlayStreetFightAudienceBattleSounds(work);
        if (EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{2}, -1) != 0)
            return;
        break;
    case 6:
        PlayStreetFightAudienceDefeatSounds(work);
        if (IsFightActive() == 0)
            return;
        break;
    case 7:
        PlayStreetFightAudienceVictorySounds(work);
        if (IsFightActive() == 0)
            return;
        break;
    default:
        return;
    }
    ControlStreetFightAudienceVisibility(work);
    ControlStreetFightAudiencePositions(work);
}

// @verified - p
void ControlStreetFightAudienceVisibility(StreetFightAudienceWork *work) {
    Vector3 nextPosition, cameraPosition, characterPosition;
    EulerRotation16 cameraRotation;
    camCtrlGetNextCamPos(&nextPosition);
    (void)camCtrlGetTargetPosition(&cameraPosition);
    (void)camCtrlGetRot(&cameraRotation);
    cameraRotation.pitch = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(cameraRotation.pitch + 4096));
    const float cameraDistance = GetPlanarDistance(&nextPosition, &cameraPosition);
    for (const shen_name *character = work->characterNames; character->name != STREET_FIGHT_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        EV_GetCharaPosition(*character, CHARACTER_TRANSLATION_ROOT, &characterPosition, EventTransformFlags::World);
        const std::int16_t cameraAngle = InverseFixedAtan2(cameraPosition.x - nextPosition.x, cameraPosition.z - nextPosition.z);
        const std::int16_t characterAngle = InverseFixedAtan2(characterPosition.x - nextPosition.x, characterPosition.z - nextPosition.z);
        const float projection = cos_fixed16(static_cast<std::uint16_t>(cameraAngle - characterAngle)) * GetPlanarDistance(&nextPosition, &characterPosition);
        if (work->mode & 1) {
            if (cameraDistance <= projection) {
                EV_EnableCharacterDisplay(*character);
                EV_EnsureCharacterDraw(*character);
            } else {
                EV_DisableCharacterDisplay(*character);
                EV_ToggleCharacterFlag(*character);
            }
        } else {
            float alpha = 1.0f;
            float step = 0.05f;
            if (cameraDistance > projection) {
                const float target = 1.0f - static_cast<float>(cameraRotation.pitch) * 0.0001953125f;
                alpha = target <= 1.0f ? std::fmax(0.2f, target) : 1.0f;
                step = 0.1f;
            }
            const float current = charaGetAlphaDraw(ctrlGetCharaByName(*character));
            float change = alpha - current;
            if (std::fabs(change) > step)
                change = change >= 0.0f ? step : -step;
            const float nextAlpha = current + change;
            SetScenarioCharacterTransparency(*character, nextAlpha <= 1.0f ? std::fmax(0.0f, nextAlpha) : 1.0f);
        }
    }
}

// @verified - p
void ControlStreetFightAudiencePositions(StreetFightAudienceWork *work) {
    if (vfmDispatchControl(FightControlOp::BattleType, shen_name{}, 0, 0) != 8)
        return;
    Vector3 first, second, position;
    EV_GetCharaPosition(work->fighters[0], CHARACTER_TRANSLATION_ROOT, &first, EventTransformFlags::World);
    EV_GetCharaPosition(work->fighters[1], CHARACTER_TRANSLATION_ROOT, &second, EventTransformFlags::World);
    for (const shen_name *character = work->characterNames; character->name != STREET_FIGHT_INVALID.name; ++character) {
        if (!DoesScenarioCharacterExist(*character))
            continue;
        EV_GetCharaPosition(*character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const float firstDistance = GetSquaredDistanceXZ(&position, &first);
        float distance = GetSquaredDistanceXZ(&position, &second);
        float x;
        float z = 0.0f;
        if (distance <= firstDistance) {
            x = second.x;
            z = second.z;
        } else {
            x = first.x;
            distance = firstDistance;
        }
        if (distance >= 1.4400001f)
            continue;
        const float separation = static_cast<float>(std::sqrt(static_cast<double>(distance)));
        const float push = 1.2f - separation;
        float directionX = position.x - x;
        float directionZ = position.z - z;
        const float magnitude = std::sqrt((directionX * directionX + 0.0f) + directionZ * directionZ);
        if (magnitude > 0.0f) {
            directionX *= 1.0f / magnitude;
            directionZ *= 1.0f / magnitude;
        }
        position.x += push * directionX;
        position.z += push * directionZ;
        EV_AT_SetPos(*character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    }
}

// @verified - p
std::uint32_t IsStreetFightAudienceCorridorBlocked(StreetFightAudienceWork *work, const Vector3 *point) {
    if (std::fabs(work->corridorLength) < 0.00000011920929f)
        return 0;
    const std::uint8_t segments = static_cast<std::uint8_t>(static_cast<std::int32_t>(work->corridorLength / 1.5f + 0.5f));
    Vector3 position{.x = work->corridorStart.x, .y = 0.0f, .z = work->corridorStart.z};
    const float stepX = (work->corridorEnd.x - position.x) / static_cast<float>(segments);
    const float stepZ = (work->corridorEnd.z - position.z) / static_cast<float>(segments);
    for (std::uint8_t segment = 0; segment < segments; ++segment) {
        if (GetSquaredDistanceXZ(&position, point) < 2.25f)
            return 1;
        position.x += stepX;
        position.z += stepZ;
    }
    return 0;
}

// @verified - p
std::int64_t FindStreetFightAudienceSpace(StreetFightAudienceWork *work, std::int32_t entity) {
    std::int32_t attempts = 0;
    Vector3 position;
    for (;;) {
        const std::int32_t angle = -shenGetRandomInt32(0xffff);
        const float radius = GetRandomNumberInRange(work->radiusRange) + work->radius;
        position.x = sin_fixed16(static_cast<std::int16_t>(angle)) * radius + work->soundCentre.x;
        position.y = work->soundCentre.y;
        position.z = work->soundCentre.z - cos_fixed16(static_cast<std::uint16_t>(angle)) * radius;
        if (attempts > 3000)
            break;
        bool overlaps = false;
        for (std::int32_t index = 0; index < entity; ++index) {
            if (GetSquaredDistanceXZ(&position, &work->soundPositions[index]) < 1.0f) {
                overlaps = true;
                ++attempts;
                break;
            }
        }
        if (IsStreetFightAudienceCorridorBlocked(work, &position) != 0)
            ++attempts;
        else if (!overlaps)
            break;
    }
    work->soundPositions[entity] = position;
    return 3LL * entity;
}

// @verified - p
void PlayStreetFightAudienceArrivalSounds(StreetFightAudienceWork *work) {
    if (work->mode & 2)
        return;
    std::uint32_t flags = 0x6e000000U;
    if (((g_currentMap.name - STREET_FIGHT_QJBT.name) & 0xfffffcffU) == 0 && g_currentMap.name != STREET_FIGHT_QMBT.name)
        flags = 0x006e0000U;
    for (std::uint8_t entity = 0; entity < work->entityCount; ++entity) {
        if (shenGetRandomInt32(4000) >= 10)
            continue;
        std::uint16_t sound;
        do {
            sound = STREET_FIGHT_ARRIVAL_SOUNDS[shenGetRandomInt32(7)];
        } while (sound == work->lastSound);
        work->lastSound = sound;
        work->materialBank = static_cast<std::uint8_t>(FindMaterialSoundBank(sound));
        (void)Execute3DSoundCommand(flags | sound, work->soundPositions[entity], 2.0f, 30.0f);
    }
}

// @verified - p
void PlayStreetFightAudienceOpeningSounds(StreetFightAudienceWork *work) {
    if (work->mode & 2)
        return;
    const std::uint32_t bank = ((g_currentMap.name - STREET_FIGHT_QJBT.name) & ~0x300U) != 0 || g_currentMap.name == STREET_FIGHT_QMBT.name ? 0U : 13U;
    if (work->loopDelay != 0) {
        work->loopDelay = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(work->loopDelay - 1));
        if (work->loopDelay == 0) {
            (void)Loop3DSoundEffect((bank << 19) | 0x7f00097fU);
            (void)Loop3DSoundEffect((bank << 19) | 0x7f000980U);
        }
        return;
    }
    for (std::uint8_t entity = 0; entity < work->entityCount; ++entity) {
        if (shenGetRandomInt32(8000) >= 10)
            continue;
        std::uint16_t sound;
        do {
            sound = STREET_FIGHT_OPENING_SOUNDS[shenGetRandomInt32(4)];
        } while (sound == work->lastSound);
        work->lastSound = sound;
        work->materialBank = static_cast<std::uint8_t>(FindMaterialSoundBank(sound));
        (void)Execute3DSoundCommand(sound | ((bank | 0xdc0U) << 19), work->soundPositions[entity], 2.0f, 30.0f);
    }
}

// @verified - p
void PlayStreetFightAudienceBattleSounds(StreetFightAudienceWork *work) {
    if (work->mode & 2)
        return;
    std::int32_t actorFlags = 0;
    std::int32_t moveFlags = 0;
    for (std::uint32_t fighter = 0; fighter < work->fighterCount; ++fighter) {
        actorFlags |= static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::ActorField, work->fighters[fighter], 0));
        moveFlags |= static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::ActorField, work->fighters[fighter], 1));
    }
    if (actorFlags == -1 || moveFlags == -1)
        return;
    std::int32_t frequency;
    if ((actorFlags & 0x140000) == 0x40000)
        frequency = 400;
    else if (actorFlags & 0x10000000)
        frequency = 500;
    else if (moveFlags & 0x4000008)
        frequency = 800;
    else if (work->actionFrames <= 100)
        frequency = 1500;
    else {
        frequency = 2000;
        work->actionFrames = 100;
    }
    if (actorFlags & 0x100)
        work->actionFrames = 0;
    else
        ++work->actionFrames;
    std::int16_t volume = 90;
    if (!cvsHasValidConverseData())
        volume = static_cast<std::int16_t>(static_cast<std::int32_t>(115.0f - work->soundFrequency * 0.0125f));
    const std::int32_t change = static_cast<std::int32_t>(static_cast<double>(frequency - work->soundFrequency) * -0.5);
    work->soundFrequency = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(work->soundFrequency - change));
    std::int16_t volumeChange = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(volume - work->soundVolume));
    if (Abs16(volumeChange) > 1)
        volumeChange = volumeChange < 0 ? -1 : 1;
    work->soundVolume = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(work->soundVolume + volumeChange));
    const std::uint32_t bank =
        ((g_currentMap.name - STREET_FIGHT_QJBT.name) & 0xfffffcffU) != 0 || g_currentMap.name == STREET_FIGHT_QMBT.name ? 0U : 0x680000U;
    for (std::uint32_t entity = 0; entity < work->entityCount; ++entity) {
        if (work->soundFrequency <= 0 || shenGetRandomInt32(work->soundFrequency) >= 10)
            continue;
        std::uint16_t sound;
        do {
            sound = STREET_FIGHT_BATTLE_SOUNDS[shenGetRandomInt32(24)];
        } while (sound == work->lastSound);
        work->lastSound = sound;
        work->materialBank = static_cast<std::uint8_t>(FindMaterialSoundBank(sound));
        const std::uint32_t volumeFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(work->soundVolume)) << 24;
        (void)Execute3DSoundCommand(bank | sound | volumeFlags, work->soundPositions[entity], 2.0f, 30.0f);
    }
}

// @verified - p
void PlayStreetFightAudienceDefeatSounds(StreetFightAudienceWork *work) {
    if (work->mode & 2)
        return;
    const std::uint32_t bank = ((g_currentMap.name - STREET_FIGHT_QJBT.name) & 0xfffffcffU) != 0 || g_currentMap.name == STREET_FIGHT_QMBT.name ? 0U : 13U;
    if (work->loopDelay != 0) {
        work->loopDelay = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(work->loopDelay - 1));
        if (work->loopDelay == 0) {
            (void)Loop3DSoundEffect((bank << 19) | 0x7f00097fU);
            (void)Loop3DSoundEffect((bank << 19) | 0x7f000980U);
        }
        return;
    }
    for (std::uint8_t entity = 0; entity < work->entityCount; ++entity) {
        if (shenGetRandomInt32(8000) >= 10)
            continue;
        std::uint16_t sound;
        do {
            sound = STREET_FIGHT_DEFEAT_SOUNDS[shenGetRandomInt32(4)];
        } while (sound == work->lastSound);
        work->lastSound = sound;
        work->materialBank = static_cast<std::uint8_t>(FindMaterialSoundBank(sound));
        (void)Execute3DSoundCommand(sound | ((bank | 0xdc0U) << 19), work->soundPositions[entity], 2.0f, 30.0f);
    }
}

// @verified - p
void PlayStreetFightAudienceVictorySounds(StreetFightAudienceWork *work) {
    if (work->mode & 2)
        return;
    const std::uint32_t bank = ((g_currentMap.name - STREET_FIGHT_QJBT.name) & 0xfffffcffU) != 0 || g_currentMap.name == STREET_FIGHT_QMBT.name ? 0U : 13U;
    if (work->loopDelay != 0) {
        work->loopDelay = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(work->loopDelay - 1));
        if (work->loopDelay == 0) {
            (void)Loop3DSoundEffect((bank << 19) | 0x7f00097fU);
            (void)Loop3DSoundEffect((bank << 19) | 0x7f000980U);
        }
        return;
    }
    for (std::uint8_t entity = 0; entity < work->entityCount; ++entity) {
        if (shenGetRandomInt32(4000) >= 10)
            continue;
        std::uint16_t sound;
        do {
            sound = STREET_FIGHT_VICTORY_SOUNDS[shenGetRandomInt32(5)];
        } while (sound == work->lastSound);
        work->lastSound = sound;
        work->materialBank = static_cast<std::uint8_t>(FindMaterialSoundBank(sound));
        (void)Execute3DSoundCommand(sound | ((bank | 0xdc0U) << 19), work->soundPositions[entity], 2.0f, 30.0f);
    }
}

// @verified - p
std::int64_t QueueStreetFightNpcWait(std::uint8_t flags) {
    if (!(flags & 0x10U))
        return 0;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightNpcWaitBeginCallback, 5, 0x0b, sizeof(StreetFightNpcWaitWork), STREET_FIGHT_EVENT_TASK);
    StreetFightNpcWaitWork *const work = static_cast<StreetFightNpcWaitWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->frames = 0;
    work->parent = HLib::GetCurrentTask();
    HLib::SetupTaskDelay(0);
    return 1;
}

// @verified - p
void StreetFightNpcWaitBeginCallback(void *parameter) {
    StreetFightNpcWaitWork *const work = static_cast<StreetFightNpcWaitWork *>(parameter);
    ++work->frames;
    if (NpcWait() || work->frames > 60U) {
        work->frames = 0;
        HLib::SetCurrentTaskTo(StreetFightNpcWaitEndCallback);
    }
}

// @verified - p
void StreetFightNpcWaitEndCallback(void *parameter) {
    StreetFightNpcWaitWork *const work = static_cast<StreetFightNpcWaitWork *>(parameter);
    ++work->frames;
    if (!NpcWait() || CountNpcLiveStreams() >= 5U || work->frames > 150U) {
        (void)HLib::ResolveDeferredCallback(work->parent);
        (void)HLib::DestroyCurrentTask();
    }
}

// @verified - p
std::int32_t GetStreetFightStageFlag(std::int32_t stage) {
    switch (stage) {
    case 0:
        return 14;
    case 1:
        return 22;
    case 2:
        return 17;
    case 3:
        return 19;
    case 4:
        return 24;
    case 5:
        return 15;
    case 7:
        return 20;
    case 8:
        return 23;
    case 10:
        return 18;
    case 11:
        return 16;
    case 12:
        return 21;
    default:
        return 0;
    }
}

// @verified - p
std::int32_t GetStreetFightStageForCurrentArea() {
    switch (g_currentMap.name) {
    case STREET_FIGHT_QSFD.name:
        return 10;
    case STREET_FIGHT_QGBT.name:
        return 12;
    case STREET_FIGHT_QJBT.name:
        return 1;
    case STREET_FIGHT_QKBT.name:
        return 4;
    case STREET_FIGHT_QLBT.name:
        return 8;
    case STREET_FIGHT_QSFC.name:
        return 3;
    case STREET_FIGHT_Q100.name:
        return 2;
    case STREET_FIGHT_QTB1.name:
        return 11;
    case STREET_FIGHT_QSFA.name:
        return 5;
    case STREET_FIGHT_QSFB.name:
        return 7;
    default:
        return 0;
    }
}

// @verified - p
std::int64_t RecordStreetFightVictory(std::int32_t stage) {
    const shen_name wins{static_cast<std::uint32_t>(stage) | 0x40U};
    std::int32_t value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, wins, 0, -1));
    std::int32_t limit = 0;
    switch (stage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
        limit = 15;
        break;
    default:
        break;
    }
    if (value < limit)
        ++value;
    (void)vfmDispatchControl(FightControlOp::MotionNibble, wins, static_cast<std::uintptr_t>(value), 1);
    (void)vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(stage) | 0x80U}, 0, 1);
    return vfmDispatchControl(FightControlOp::MotionNibble, wins, 0, -1);
}

// @verified - p
std::int64_t RecordStreetFightDefeat(std::int32_t stage) {
    const shen_name wins{static_cast<std::uint32_t>(stage) | 0x40U};
    const shen_name losses{static_cast<std::uint32_t>(stage) | 0x80U};
    std::int32_t value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, wins, 0, -1));
    if (value > 0)
        --value;
    (void)vfmDispatchControl(FightControlOp::MotionNibble, wins, static_cast<std::uintptr_t>(value), 1);
    value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, losses, 0, -1));
    std::int32_t limit = 0;
    switch (stage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
        limit = 15;
        break;
    default:
        break;
    }
    if (value < limit)
        ++value;
    (void)vfmDispatchControl(FightControlOp::MotionNibble, losses, static_cast<std::uintptr_t>(value), 1);
    return vfmDispatchControl(FightControlOp::MotionNibble, wins, 0, -1);
}

// @verified - p
StreetFightLookWork *QueueStreetFightLook(shen_name character, shen_name target) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightLookCallback, 6, 0x0b, sizeof(StreetFightLookWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(task, 1);
    StreetFightLookWork *const work = static_cast<StreetFightLookWork *>(HLib::GetTaskParameter(task));
    work->target = target;
    work->character = character;
    work->reserved08 = 16;
    work->frames = 0;
    work->flags = 0x1101;
    return work;
}

// @verified - p
void StreetFightLookCallback(void *parameter) {
    StreetFightLookWork *const work = static_cast<StreetFightLookWork *>(parameter);
    if (work->frames || (work->flags & 1U)) {
        if (work->flags & 0x11U) {
            work->flags &= ~1U;
            if (DoesScenarioCharacterExist(work->character) && DoesScenarioCharacterExist(work->target)) {
                Vector3 position;
                EV_GetCharaPosition(work->target, 5, &position, EventTransformFlags::World);
                (void)UpdateMotionWorkWithPosition(scenFindMotionwork(work->character), &position, 0);
                if (work->flags & 0x100U)
                    (void)UpdateMotionWorkWithPosition(scenFindMotionwork(work->character), nullptr, 3);
            }
        }
        if (work->frames)
            --work->frames;
    } else {
        if (!(work->flags & 0x1000U) && DoesScenarioCharacterExist(work->character))
            (void)UpdateMotionWorkWithPosition(scenFindMotionwork(work->character), nullptr, 0);
        (void)HLib::DestroyCurrentTask();
    }
}

// @verified - p
void ReadStreetFightBookiePosition() {
    Vector3 position;
    EV_GetCharaPosition(STREET_FIGHT_BOOKIE, 5, &position, EventTransformFlags::World);
}

// @verified - p
std::uint32_t PollStreetFightActionChoice() {
    if (!streetFightActionChoicePhase) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++streetFightActionChoicePhase;
        return 0;
    }
    if (streetFightActionChoicePhase == 1) {
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++streetFightActionChoicePhase;
        return 0;
    }
    if (streetFightActionChoicePhase == 2) {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        streetFightActionChoicePhase = 0;
        return choice;
    }
    return 0;
}

// @verified - p
std::uint32_t ControlStreetFightActionChoice(std::int32_t enabled) {
    if (!enabled) {
        streetFightActionChoicePhase = 0;
        return 0;
    }
    switch (streetFightActionChoicePhase) {
    case 0:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++streetFightActionChoicePhase;
        return 0;
    case 1:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++streetFightActionChoicePhase;
        return 0;
    case 2: {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        streetFightActionChoicePhase = 0;
        return choice;
    }
    default:
        return 0;
    }
}

// @verified - p
std::int64_t ReleaseStreetFightCharacter(shen_name character) {
    (void)EV_npcFree(character);
    const std::int64_t loaded = static_cast<std::int64_t>(IsNpcLoadedOrVisible(character));
    return loaded ? EV_npcLockUnlock(character) : loaded;
}

// @verified - p
std::int64_t QueueStreetFightCharacterLoad(HLib::Task *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(StreetFightCharacterLoadCallback, 6, 0x0b, sizeof(StreetFightCharacterLoadWork), STREET_FIGHT_EVENT_TASK);
    StreetFightCharacterLoadWork *const work = static_cast<StreetFightCharacterLoadWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(parent, 0);
    work->task = task;
    work->characters = streetFightLoadCharacters;
    work->parent = parent;
    work->index = 0;
    work->state = 0;
    return 0;
}

// @verified - p
void StreetFightCharacterLoadCallback(void *parameter) {
    StreetFightCharacterLoadWork *const work = static_cast<StreetFightCharacterLoadWork *>(parameter);
    if (!work->state) {
        for (const shen_name *character = work->characters; character->name != STREET_FIGHT_INVALID.name; ++character)
            (void)EV_CheckWarmStartNpcRoute(*character);
        ++work->state;
        work->index = 0;
        return;
    }
    const shen_name character = work->characters[work->index];
    if (work->state == 1) {
        if (character.name == STREET_FIGHT_INVALID.name) {
            (void)HLib::ResolveDeferredCallback(work->parent);
            (void)HLib::DestroyTaskInternal(work->task);
        } else {
            work->state = DoesScenarioCharacterExist(character) ? 3 : 2;
        }
        return;
    }
    if (work->state == 2) {
        const std::int32_t result = static_cast<std::int32_t>(EV_npcStreamOrLoadHuman(character));
        if (result == -4 || result == -3)
            (void)EV_ResetFreeTalkFlags();
        else if (result == 1)
            ++work->state;
        return;
    }
    if (work->state != 3)
        return;
    (void)EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
    (void)figpSetFlagsBlendForBlur(character, 2, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
    (void)mtwkSetExtraFlag(scenFindMotionwork(character), 0, 0);
    (void)mtwkSetExtraFlag(scenFindMotionwork(character), 3, 2);
    (void)mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    (void)SetMotionFrameTime(scenFindMotionwork(character), 7, MotionRegion::LowerBody);
    (void)SetMotionRegionFlag(scenFindMotionwork(character), 66, 2, MotionRegion::LowerBody);
    (void)SetMotionRegionFlag(scenFindMotionwork(character), 69, 2, MotionRegion::LowerBody);
    (void)SetMotionRegionFlag(scenFindMotionwork(character), 71, 2, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(character);
    EV_EnsureCharacterDraw(character);
    SetScenarioCharacterType(character, CharacterTypeStencilShadow);
    AT_SetLocalBaseChara(character, STREET_FIGHT_CONVERSATION_BASE);
    ++work->index;
    work->state = 1;
}

} // namespace shenmue::s2
