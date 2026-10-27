#include "s1/task/vf_internal.hpp"

namespace shenmue::s1 {

using namespace vf_detail;

using namespace vf_detail;

std::int32_t g_vfBattleForceEndRequested = 1;

VFFightStage *g_pFightStage = nullptr;

std::int16_t g_lcdmScoreBlinkTimer = 0;

std::uint8_t g_lcdmScoreBlinkPhase = 0;

std::int32_t g_vfLcdmEndShown = 0;

std::uint8_t g_prevHelpIconFlag = 0;

std::uint8_t g_shenFightState = 0;

std::uint8_t g_defaultHpRegenProfile = 0;

std::uint8_t g_activeHpRegenProfile = 0;

std::uint8_t g_shenFightState2 = 0;

std::uint32_t g_vfRuntimeAccumFightTicks = 0;

void vfFreeStageObjects();

} // namespace shenmue::s1

namespace shenmue::s1::vf_detail {

std::int32_t g_vfAirborneAngle = 0;

std::array<std::array<std::uint8_t, 20>, 3> g_vfMoveAnimTypeLut{};

VFGW *g_pVFGW_Widget = nullptr;

// Set by vfUpdateMain for the update callbacks; never cleared.
VFMAData *g_pFightMain = nullptr;

std::int32_t g_vfShouldLoad70mbAssets = 0;

std::uint16_t g_vfPostBtlWaitFrms = 0;

std::uint8_t g_b70MBState = 0;

std::int32_t g_70mbLastRankIndex = 0;

std::uint32_t g_70mbLastTimeTicks = 0xffffffffU;

float g_maxDist = 0.0f;

Character *g_vfWantsCamera = nullptr;

std::uint16_t g_vfRunButtonId = 64;

std::uint16_t g_vfConfirmButtonMask = 128;

std::array<VFRegistryEntry, 16> g_vfFightRegistry{};

Character *const kVFFormationSpawnCharacter = reinterpret_cast<Character *>(std::uintptr_t{0xffffffffU});

std::int32_t shenQueryFightTaskOrRegistryValue(Character *character);

static_assert(sizeof(VFAnchor) == 0x10);

static_assert(sizeof(VFAreaMask) == 0x40);

static_assert(sizeof(VFFormationGroup) == 0x58);

static_assert(sizeof(VFEnemySlot) == 0xf0);

static_assert(offsetof(VFEnemySlot, fighter) == 0x50);

static_assert(sizeof(VFFightStage) == 0x2a0);

static_assert(offsetof(VFFightStage, ringBoundary) == 0x228);

static_assert(sizeof(VFMAData) == 0x510);

static_assert(offsetof(VFMAData, task) == 0x08);

static_assert(offsetof(VFMAData, state) == 0x34);

static_assert(offsetof(VFMAData, characters) == 0xa0);

static_assert(offsetof(VFMAData, actors) == 0x168);

static_assert(offsetof(VFMAData, slotRuntime) == 0x1a8);

static_assert(offsetof(VFMAData, collisionData) == 0x1e8);

static_assert(offsetof(VFMAData, maxActiveEnemies) == 0x262);

static_assert(sizeof(VOBJ) == 0xc0);

static_assert(offsetof(VOBJ, entries) == 0x24);

static_assert(offsetof(VOBJ, hitGuardStrength) == 0x78);

static_assert(offsetof(VOBJ, animatedNodeKinds) == 0x80);

static_assert(offsetof(VOBJ, keyframeData) == 0x98);

static_assert(offsetof(VOBJ, groundOffset) == 0xb0);

static_assert(offsetof(VOBJ, axisRotateStep) == 0xbc);

static_assert(sizeof(VFStageObject) == 0xf0);

static_assert(offsetof(VFStageObject, object) == 0x08);

static_assert(offsetof(VFStageObject, position) == 0x20);

static_assert(offsetof(VFStageObject, rotation) == 0x2c);

static_assert(offsetof(VFStageObject, localVelocity) == 0x44);

static_assert(offsetof(VFStageObject, savedCollisionFlags) == 0x78);

static_assert(offsetof(VFStageObject, character) == 0x80);

static_assert(offsetof(VFStageObject, worldShapes) == 0x88);

static_assert(offsetof(VFStageObject, frameIndex) == 0xdc);

static_assert(offsetof(VFStageObject, keyframeCursors) == 0xe0);

static_assert(offsetof(VFStageObject, next) == 0xe8);

static_assert(sizeof(VFSlotRuntimeState) == 0xc0);

static_assert(offsetof(VFSlotRuntimeState, currentHpRegenerationPhase) == 0x74);

static_assert(offsetof(VFSlotRuntimeState, hpPhase) == 0x75);

static_assert(offsetof(VFSlotRuntimeState, cameraScriptStep) == 0x76);

static_assert(offsetof(VFSlotRuntimeState, cameraScriptWait) == 0x78);

static_assert(offsetof(VFSlotRuntimeState, savedCameraControlFlags) == 0x7a);

static_assert(offsetof(VFSlotRuntimeState, reactionAnimationHistory) == 0x7c);

static_assert(offsetof(VFSlotRuntimeState, rankHistory) == 0x84);

static_assert(offsetof(VFSlotRuntimeState, downedFrames) == 0x8a);

static_assert(offsetof(VFSlotRuntimeState, repeatedReactionAnimation) == 0x8c);

static_assert(offsetof(VFSlotRuntimeState, repeatedRankCount) == 0x8e);

static_assert(sizeof(VFActor) == 0xb10);

static_assert(offsetof(VFActor, character) == 0x10);

static_assert(offsetof(VFActor, position) == 0x78);

static_assert(offsetof(VFActor, separation) == 0x9c);

static_assert(offsetof(VFActor, scale) == 0x2a8);

static_assert(offsetof(VFActor, nextAction) == 0x378);

static_assert(offsetof(VFActor, currentHp) == 0x3c8);

static_assert(offsetof(VFActor, hitReactionAnimSelectFlags) == 0x3f9);

static_assert(offsetof(VFActor, activeContactCount) == 0x42e);

static_assert(offsetof(VFActor, targetGroundY) == 0x454);

static_assert(offsetof(VFActor, knockbackBaseParameter) == 0x470);

static_assert(offsetof(VFActor, reactionPriority) == 0x4e0);

static_assert(offsetof(VFActor, reactionRingHead) == 0x660);

static_assert(offsetof(VFActor, scan) == 0x662);

static_assert(offsetof(VFActor, slots) == 0x662);

static_assert(offsetof(VFActor, delayedReactionStateActive) == 0x66e);

static_assert(offsetof(VFActor, delayedReactionTimer) == 0x670);

static_assert(offsetof(VFActor, reactionResumeProgramCounter) == 0x680);

static_assert(offsetof(VFActor, reactionVmProgramCounter) == 0x688);

static_assert(offsetof(VFActor, rootReactionVmProgramCounter) == 0x690);

static_assert(offsetof(VFActor, reactionStateStack) == 0x6d8);

static_assert(offsetof(VFActor, reactionConditionFlags) == 0x8d8);

static_assert(offsetof(VFActor, connectionResultMetadata) == 0x900);

static_assert(offsetof(VFActor, connectionMask) == 0x904);

static_assert(offsetof(VFActor, previousContactProbePositions) == 0x908);

static_assert(offsetof(VFActor, contactProfileMasks) == 0x980);

static_assert(offsetof(VFActor, contactVolumeRecords) == 0xad0);

static_assert(offsetof(VFActor, fighterRankId) == 0x9e3);

static_assert(offsetof(VFActor, headGroundDelta) == 0xa40);

static_assert(offsetof(VFActor, copiedGroundDelta) == 0xa44);

static_assert(offsetof(VFActor, triggerMask) == 0xa48);

static_assert(offsetof(VFActor, facingQuadrantPath) == 0xa8c);

static_assert(offsetof(VFActor, pairedActor) == 0xaf0);

const std::uint32_t *Words(void *params) {
    return static_cast<const std::uint32_t *>(params);
}

void SetResult(ScenarioContext *context, std::uint32_t value) {
    if (context && context->stack)
        context->stack->accumulator = value;
}

void SetResult(ScenarioContext *context, float value) {
    SetResult(context, std::bit_cast<std::uint32_t>(value));
}

Character *FindCharacter(ScenarioContext *context, std::uint32_t id) {
    HLib::Task *task = context && context->task ? context->task->task : nullptr;
    if (!task && g_mapControlTask && g_mapControlTask->task_data_ptr) {
        task = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->scenarioTask;
    }
    return task && id ? CHID_Find(task, id) : nullptr;
}

Character *CurrentRyo() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return static_cast<Character *>(reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr)->ryo);
}

VFRegistryEntry *FindRegistry(const Character *character) {
    for (VFRegistryEntry &entry : g_vfFightRegistry) {
        if (entry.character == character)
            return &entry;
    }
    return nullptr;
}

void AllocateFGST() {
    auto *const stage = static_cast<VFFightStage *>(HLib_AllocHeapMemBlock(sizeof(VFFightStage), static_cast<std::int32_t>(SHEN_NAME(FGST))));
    g_pFightStage = stage;
    if (stage)
        HLib_EnableTokenCheck(stage);
    stage->ringBoundary = {};
    stage->accumulatedHp = 0;
    stage->useCenterBias = 0;
    stage->enemySlotCount = 0;
    stage->formationCount[0] = 0;
    stage->formationCount[1] = 0;
    stage->readyEnemySlots[0] = 0;
    stage->readyEnemySlots[1] = 0;
    stage->activeFightersByTeam[0] = 0;
    stage->activeFightersByTeam[1] = 0;
    for (std::uint8_t &counter : stage->groupCounters)
        counter = 0;
    stage->teamRespawnTimer[0] = 0;
    stage->teamRespawnTimer[1] = 0;
    for (void *&style : stage->styles)
        style = nullptr;
}

std::uint32_t shenStartFGST(std::uint8_t enemies, std::uint8_t groups, std::int8_t areaMasks, std::uint8_t generators) {
    AllocateFGST();
    AllocateFCMG(enemies);
    AllocateFRMG(groups);
    AllocateFCEL(areaMasks);
    AllocateFGEN(generators);
    for (VFRegistryEntry &entry : g_vfFightRegistry)
        entry.character = nullptr;
    EFFE_InitVFFightGroundEffects();
    g_vfShouldLoad70mbAssets = 0;
    g_pVOBJ.fill(nullptr);
    g_vfStageObjs.fill(nullptr);
    g_maxDist = 16.0f;
    g_vfObjectRoot = nullptr;
    g_battle70Index = nullptr;
    g_battle70Afs = nullptr;
    if (g_areaId == SHEN_NAME(MFBT)) {
        g_battle70Index = static_cast<HLib::TaskData *>(ReadFile("scene/03/MFBT", "BATTLE70.idx", static_cast<std::int32_t>(SHEN_NAME(BTPI))));
        g_battle70Afs = PACK_ReadAFS("scene/03/MFBT", "BATTLE70.afs");
    }
    g_vfRunButtonId = 64;
    g_vfConfirmButtonMask = 128;
    return 1;
}

constexpr std::array<const char *, 58> kFightStyleNames{{
    "RYOU",
    "ZAKO1",
    "ZAKO2",
    "ZAKO3",
    "ZAKO4",
    "ENOKI",
    "CHAI",
    "ZAKO5",
    "NAGAS",
    "TSUKA",
    "MARU",
    "SERA",
    "FUKU",
    "MURA",
    "HATA",
    "HAMA",
    "HOGAN",
    "SATOS",
    "TERRY",
    "MBOS1",
    "KISYO",
    "DEKU",
    "TONY",
    "JONES",
    "DICK",
    "HARRY",
    "SMITH",
    "CHAI2",
    "KISY2",
    "KISY3",
    "COMM1",
    "GORO",
    "ZAKO6",
    "PEDRO",
    "WARU1",
    "WARU2",
    "CHALY",
    "TAKO1",
    "TAKO2",
    "TAKO3",
    "TAKO4",
    "TAKO5",
    "RBERT",
    "BOB",
    "PAURO",
    "OKA",
    "CHARY",
    "MURA2",
    "SATS2",
    "PEDR2",
    "TIME1",
    "TIME2",
    "TIME3",
    "TIME4",
    "TIME5",
    "COMM4",
    "COMM5",
    "FUKU2",
}};

void vfLoadFighterStyle(std::uint8_t style) {
    if (g_pFightStage->styles[style])
        return;
    char directory[32]{};
    std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_sceneId, static_cast<char>(g_areaId), static_cast<char>(g_areaId >> 8U),
        static_cast<char>(g_areaId >> 16U), static_cast<char>(g_areaId >> 24U));
    char filename[24]{};
    std::snprintf(filename, sizeof(filename), "EN_%s.BIN", kFightStyleNames[style]);
    g_pFightStage->styles[style] = ReadFile(directory, filename, static_cast<std::int32_t>(SHEN_NAME(FIGH)));
}

std::uint32_t vfStageSetupUtil(std::uintptr_t command, Character *blockedBy, std::int8_t hpPoolMode, std::uint8_t groupId, std::uint16_t hpTotal,
    std::uint8_t fightStyle, std::int16_t animationId, std::int16_t spawnYaw, float centerX, float centerY, float centerZ) {
    constexpr std::uintptr_t kSetBattleCenter = 0xfffffffcU;
    constexpr std::uintptr_t kAddAnchor = 0xfffffffeU;
    constexpr std::uintptr_t kFormationSpawn = 0xffffffffU;
    VFFightStage *const stage = g_pFightStage;
    if (command == kSetBattleCenter) {
        stage->battleCenter = {centerX, centerY, centerZ};
        stage->useCenterBias = 1;
        return 1;
    }
    if (command == kAddAnchor) {
        VFAnchor &anchor = stage->anchors[g_fgenNumAnchors++];
        anchor.position = {centerX, centerY, centerZ};
        anchor.enabled = 1;
        anchor.excludeFromSelection = 0;
        return 1;
    }
    const std::int8_t enemySlotCount = stage->enemySlotCount;
    const auto formationIndex = static_cast<std::uint8_t>(stage->formationCount[0] + stage->formationCount[1]);
    if (static_cast<std::uint8_t>(enemySlotCount) >= stage->maxEnemies || formationIndex >= stage->formationCapacity)
        return 0;
    VFFormationGroup &formation = stage->formations[formationIndex];
    formation.character = reinterpret_cast<Character *>(command);
    formation.blockedBy = blockedBy;
    formation.hpCurrent = hpTotal;
    formation.hpTotal = hpTotal;
    formation.fightStyle = fightStyle;
    formation.hpPoolMode = hpPoolMode;
    formation.teamId = static_cast<std::int8_t>(groupId);
    formation.waveId = 0;
    formation.selectFlags = 0;
    formation.actorFlags = 0;
    formation.waveFlags = 0;
    if (groupId == 1)
        stage->accumulatedHp += hpTotal;
    if (command == kFormationSpawn) {
        formation.spawnPosition = {centerX, centerY, centerZ};
        formation.animationId = static_cast<std::uint16_t>(animationId);
        formation.spawnYaw = static_cast<std::uint16_t>(spawnYaw);
        g_frmgFormationIndex = formationIndex;
        formation.slot = nullptr;
    } else {
        VFEnemySlot *slot = nullptr;
        for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
            if (stage->enemySlots[index].character == reinterpret_cast<Character *>(command)) {
                slot = &stage->enemySlots[index];
                break;
            }
        }
        if (slot) {
            formation.slot = slot;
        } else {
            VFEnemySlot &group = stage->enemySlots[enemySlotCount];
            formation.slot = &group;
            group.character = reinterpret_cast<Character *>(command);
            group.wave = nullptr;
            group.fighter = nullptr;
            group.battleIndex = 0;
            group.quadrant = -1;
            group.state = SS_Allocated;
            group.tickFrames = 0;
            group.isRunning = 0;
            group.teamId = groupId;
            group.fadeAlpha = 1.0f;
            group.referenceRamp = 0.0f;
            group.modelSlotIndex = -1;
            group.model = nullptr;
            group.registryIndex = enemySlotCount;
            group.referenceOffset.x = 0.0f;
            group.referenceOffset.y = 0.0f;
            group.referenceOffset.z = 0.0f;
            g_vfFightRegistry[static_cast<std::uint8_t>(enemySlotCount)] = {reinterpret_cast<Character *>(command), 0, {}, 1.0f};
            ++stage->enemySlotCount;
        }
    }
    vfLoadFighterStyle(fightStyle);
    ++g_pFightStage->formationCount[groupId];
    ++g_pFightStage->teamRespawnTimer[groupId];
    return 1;
}

std::int32_t shenQueryFightTaskOrRegistryValue(Character *character) {
    switch (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(character))) {
    case 0xfffffffcU: {
        const VFMAData *const game = vfMainGetCurrent();
        return game && (game->flags & FreeBattleExitActive) != 0 ? 1 : 0;
    }
    case 0xfffffffdU: {
        const VFMAData *const game = vfMainGetCurrent();
        return game && (game->flags & FreeBattleIntroActive) != 0 ? 1 : 0;
    }
    case 0xfffffffeU: {
        const VFMAData *const game = vfMainGetCurrent();
        return game ? static_cast<std::int32_t>(game->extraValue) : 0;
    }
    case 0xffffffffU: {
        const VFMAData *const game = vfMainGetCurrent();
        return game && (game->flags & FreeBattleExitPrompt) != 0 ? 1 : 0;
    }
    default:
        break;
    }
    for (const VFRegistryEntry &entry : g_vfFightRegistry) {
        if (entry.character == character)
            return static_cast<std::uint8_t>(entry.slot);
    }
    return -1;
}

constexpr std::uint32_t kVFForceDefeat = 0x20U;

std::uint8_t vfPickOpponentSlotByPrioRank(VFMAData *game, std::uint8_t rank) {
    float distance = 0.1f;
    std::uint8_t selected = 99;
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        VFActor *const actor = game->actors[index];
        if (!game->characters[index])
            continue;
        const std::int8_t playerSlot = game->playerSlot;
        if (index == static_cast<std::uint8_t>(playerSlot) || game->slotTeamId[index] == game->slotTeamId[playerSlot] ||
            game->battleMeta.groups[index]->tickFrames < 15 || (actor->moveFlags & kVFForceDefeat) != 0 ||
            ((actor->hitFlags | actor->hitStateFlags2) & 0xc5010U) != 0 || ((actor->hitStateFlags | actor->motionFlags) & 0x800c00U) != 0) {
            continue;
        }
        const std::uint8_t priorityRank = static_cast<std::uint8_t>(actor->priorityRank);
        if (priorityRank > rank || (priorityRank == rank && actor->contactPoints[playerSlot] > distance)) {
            if (priorityRank == rank)
                distance = actor->contactPoints[playerSlot];
            selected = index;
            rank = priorityRank;
        }
    }
    return selected;
}

constexpr void (*g_vfmaCoreCallbacks[])(VFActor &) = {
    vfUpdateWorldTransformAndFacingAngle,
    vfUpdateMain,
    vfmaUpdateNextReaction,
    vfResolvePairInteraction,
    vfUpdatePoseWithSfx,
    vfApplyYawRecursive,
};

void vfAssignNearestOpponents(VFMAData *game) {
    float distances[kBattleSlots * kBattleSlots];
    std::int16_t relativeYaw[kBattleSlots * kBattleSlots];
    std::uint8_t pairSlots[kBattleSlots];
    for (float &distance : distances)
        distance = 9999.0f;
    for (std::uint8_t &pair : pairSlots)
        pair = 0xffU;
    std::int8_t primaryEnemySlot = -1;
    const std::int8_t playerSlot = game->playerSlot;
    for (std::uint8_t source = 0; source < kBattleSlots; ++source) {
        if (!game->characters[source])
            continue;
        const VFActor *const sourceActor = game->actors[source];
        const std::uint8_t sourceTeam = game->slotTeamId[source];
        const float sourceX = sourceActor->position.x;
        const float sourceZ = sourceActor->position.z;
        const std::uint16_t sourceFacing = sourceActor->temporaryFacingAngle;
        if (sourceTeam == 1 && (game->battleMeta.waves[source]->selectFlags & 0xa0U) == 0)
            primaryEnemySlot = static_cast<std::int8_t>(source);
        for (std::uint8_t target = 0; target < kBattleSlots; ++target) {
            if (target == source || !game->characters[target] || game->slotTeamId[target] == sourceTeam)
                continue;
            const VFActor *const targetActor = game->actors[target];
            const float targetX = targetActor->position.x;
            const float targetZ = targetActor->position.z;
            const float distance = std::sqrt((sourceX - targetX) * (sourceX - targetX) + (sourceZ - targetZ) * (sourceZ - targetZ));
            distances[source + kBattleSlots * target] = distance;
            distances[target + kBattleSlots * source] = distance;
            relativeYaw[target + kBattleSlots * source] = static_cast<std::int16_t>(sourceFacing - atan2_fixed16(sourceX - targetX, sourceZ - targetZ));
            relativeYaw[source + kBattleSlots * target] =
                static_cast<std::int16_t>(targetActor->temporaryFacingAngle - atan2_fixed16(targetX - sourceX, targetZ - sourceZ));
        }
    }
    game->primaryTargetSlot = static_cast<std::uint8_t>(primaryEnemySlot);

    std::int16_t partyYawDelta = 0;
    std::uint8_t lastPairedCandidate = 0xffU;
    for (std::uint8_t self = 0; self < kBattleSlots; ++self) {
        if (!game->characters[self])
            continue;
        VFActor *const actor = game->actors[self];
        bool hasCloseFrontCandidate = false;
        std::uint8_t candidateSlot = 0xffU;
        float bestDistance = 9999.0f;
        if (self == static_cast<std::uint8_t>(playerSlot))
            partyYawDelta = static_cast<std::int16_t>(game->partyFacingAngle - actor->temporaryFacingAngle);
        const std::size_t row = kBattleSlots * self;
        for (std::uint8_t candidate = 0; candidate < kBattleSlots; ++candidate) {
            if (!game->characters[candidate] || game->slotTeamId[candidate] == game->slotTeamId[self])
                continue;
            float distance = distances[candidate + row];
            if (!partyYawDelta && (game->actors[candidate]->moveFlags & kVFForceDefeat) != 0)
                distance = 9998.9004f;
            if (static_cast<std::uint16_t>(partyYawDelta + relativeYaw[candidate + row] + 9100) > 18200U || distance >= 3.0f) {
                if (!hasCloseFrontCandidate && bestDistance > distance) {
                    bestDistance = distance;
                    candidateSlot = candidate;
                }
            } else if (!hasCloseFrontCandidate || bestDistance > distance) {
                hasCloseFrontCandidate = true;
                bestDistance = distance;
                candidateSlot = candidate;
            }
        }
        lastPairedCandidate = candidateSlot;
        const std::uint8_t pairedFighterId = actor->pairedFighterId;
        const bool keepPairing = candidateSlot == 0xffU ||
            (pairedFighterId != 0xffU &&
                ((candidateSlot == pairedFighterId && game->characters[candidateSlot] == actor->pairedCharacterCache) ||
                    std::bit_cast<std::uint64_t>(actor->deferredPairAction) != 0 || ((actor->hitFlags | actor->hitStateFlags2) & 0xc0000U) != 0 ||
                    ((actor->hitStateFlags | actor->motionFlags) & 0xc00U) != 0));
        partyYawDelta = 0;
        if (!keepPairing) {
            actor->pairedFighterId = candidateSlot;
            Character *const previousPairedCharacter = actor->pairedCharacterCache;
            actor->pairedActor = game->actors[candidateSlot];
            actor->pairedCharacterCache = game->characters[candidateSlot];
            if (previousPairedCharacter != actor->pairedCharacterCache)
                actor->contactDecisionMask = 0;
        }
        pairSlots[self] = actor->pairedFighterId;
    }

    const std::uint8_t player = static_cast<std::uint8_t>(playerSlot);
    for (std::uint8_t self = 0; self < kBattleSlots; ++self) {
        if (!game->characters[self])
            continue;
        const VFActor *const actor = game->actors[self];
        if ((actor->motionFlags & 0x2000000U) != 0 && actor->targetingCharacter && actor->targetActor &&
            (actor->targetActor->moveFlags & kVFForceDefeat) == 0) {
            pairSlots[self] = static_cast<std::uint8_t>(actor->targetSlotIndex);
            continue;
        }
        if (player == 0xffU || !game->characters[player] || self == player)
            continue;
        if (self == static_cast<std::uint8_t>(primaryEnemySlot)) {
            pairSlots[self] = player;
            continue;
        }
        std::uint8_t mode = 0;
        if (game->slotTeamId[self] != 1)
            mode = 2;
        else if (primaryEnemySlot == -1)
            mode = 0;
        else if (lastPairedCandidate == player && distances[static_cast<std::uint8_t>(primaryEnemySlot) + kBattleSlots * player] < 4.0f)
            mode = 1;
        else
            continue;
        std::int8_t nearestEnemy = -1;
        std::int8_t nearestAlly = -1;
        std::int8_t nearestGroundedEnemy = -1;
        float nearestEnemyDistance = 9999.0f;
        float nearestAllyDistance = 9999.0f;
        float nearestGroundedEnemyDistance = 9999.0f;
        const std::uint8_t playerPair = game->actors[player]->pairedFighterId;
        const std::size_t row = kBattleSlots * self;
        for (std::uint8_t scan = 0; scan < kBattleSlots; ++scan) {
            if (!game->characters[scan] || scan == player)
                continue;
            const VFActor *const candidate = game->actors[scan];
            if ((candidate->moveFlags & kVFForceDefeat) != 0)
                continue;
            const float distance = distances[scan + row];
            const float playerDistance = distances[scan + kBattleSlots * player];
            if (game->slotTeamId[scan] == 1 && nearestGroundedEnemyDistance > playerDistance && (candidate->hitFlags & 0x80000U) == 0 &&
                (candidate->hitStateFlags & 0x800U) == 0) {
                nearestGroundedEnemy = static_cast<std::int8_t>(scan);
                nearestGroundedEnemyDistance = playerDistance;
            }
            const std::uint8_t team = game->slotTeamId[scan];
            if (!team && nearestAllyDistance > distance) {
                nearestAlly = static_cast<std::int8_t>(scan);
                nearestAllyDistance = distance;
            }
            if (team == 1 && scan != playerPair && nearestEnemyDistance > distance) {
                nearestEnemy = static_cast<std::int8_t>(scan);
                nearestEnemyDistance = distance;
            }
        }
        if (mode == 1) {
            if (nearestAlly != -1)
                pairSlots[self] = static_cast<std::uint8_t>(nearestAlly);
        } else if (mode == 2) {
            if (nearestEnemy != -1 && distances[row + static_cast<std::int8_t>(pairSlots[self])] > 1.5f)
                pairSlots[self] = static_cast<std::uint8_t>(nearestEnemy);
        } else if (nearestGroundedEnemy != -1 && nearestAlly != -1) {
            if (static_cast<std::int8_t>(self) == nearestGroundedEnemy && distances[row + static_cast<std::uint8_t>(nearestAlly)] > 1.5f)
                pairSlots[self] = player;
            else
                pairSlots[self] = static_cast<std::uint8_t>(nearestAlly);
        }
    }

    std::int32_t cameraActive = g_VF_camera_active;
    for (std::uint8_t self = 0; self < kBattleSlots; ++self) {
        if (!game->characters[self])
            continue;
        const std::uint8_t target = pairSlots[self];
        VFActor *const actor = game->actors[self];
        if (target == 0xffU)
            continue;
        Character *const targetCharacter = game->characters[target];
        if (!targetCharacter || (static_cast<std::int8_t>(target) == actor->targetSlotIndex && targetCharacter == actor->targetingCharacter))
            continue;
        actor->targetSlotIndex = static_cast<std::int8_t>(target);
        Character *const previousTarget = actor->targetingCharacter;
        actor->targetActor = game->actors[target];
        actor->targetingCharacter = game->characters[target];
        if (((game->battleType - 1U) & 0xfbU) == 0 && game->playerSlot == static_cast<std::int8_t>(self)) {
            g_vfWantsCamera = actor->targetingCharacter;
            if (!g_vfWantsCamera)
                cameraActive = 0;
            g_VF_camera_active = cameraActive;
        }
        if (previousTarget != actor->targetingCharacter)
            actor->facingQuadrantStep = -1;
    }
}

bool vfSetupContactSlotAndUpdateState(VFMAData *game, VFFormationGroup *formation) {
    const std::uint8_t index = vfRegisterContactSlot(game, formation->slot);
    if (index == 99)
        return false;
    vfAssignNearestOpponents(game);
    vfResetActorReactionStateForContact(game, index);
    vfUpdateGuardLevel(game, index);
    return true;
}

void vfReactionUpdate(VFMAData *game, VFEnemySlot *group) {
    VFFightStage *stage = g_pFightStage;
    if (stage->activeFightersByTeam[0] + stage->activeFightersByTeam[1] == 6)
        return;
    for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
        VFFormationGroup *const formation = &stage->formations[index];
        if (formation->waveId || !formation->character || formation->character == kVFFormationSpawnCharacter || formation->slot->state != SS_Ready ||
            formation->blockedBy != group->character || static_cast<std::uint8_t>(formation->teamId) != group->teamId) {
            continue;
        }
        const std::uint8_t opposingTeam = group->teamId ^ 1U;
        if (stage->activeFightersByTeam[opposingTeam] || stage->teamRespawnTimer[opposingTeam]) {
            vfSetupContactSlotAndUpdateState(game, formation);
            stage = g_pFightStage;
        }
    }
}

void vfUpdateDefeatAndRecycleSlotsOnDownedActors(VFMAData *game) {
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        VFActor *const actor = game->actors[index];
        if (!actor->currentHp && (actor->moveFlags & kVFForceDefeat) == 0)
            actor->moveFlags |= kVFForceDefeat;
        if ((actor->moveFlags & (kVFAirborne | kVFForceDefeat)) != kVFForceDefeat || (actor->hitStateFlags2 & 0x81010U) != 0 ||
            actor->currentAnimationEndFrame < actor->reactionAnimationEndFrame) {
            continue;
        }
        VFEnemySlot *const group = game->battleMeta.groups[index];
        vfTransitionEnemySlotToCooldown(game, group);
        if (static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]) > game->maxActiveEnemies)
            vfmaRecycleDefeatedEnemySlots(game);
        vfReactionUpdate(game, group);
    }
}

void vfHandleAggroStateChange(VFMAData *game, VFEnemySlot *group, VFFormationGroup *formation) {
    if (!game->playerCharacter)
        return;

    std::uint8_t reactionType = 0;
    if (formation->fightStyle == 0x21U)
        reactionType = 0;
    if (formation->fightStyle == 0x11U)
        reactionType = 1;
    if (formation->fightStyle == 0x0dU)
        reactionType = 2;
    const VFAggroReactionConfig &config = g_vfAggroReactionConfig[reactionType];
    const auto *const motion = static_cast<const MotionInformationWork *>(group->motion);
    if (formation->scriptParameter == 1) {
        if (motion->frameTimer >= motion->frameEnd) {
            vfUpdateDefaultReactionAnim(group, config.followupReactionId);
            ++formation->scriptParameter;
        }
        return;
    }
    if (formation->scriptParameter == 2) {
        if (motion->frameTimer >= motion->frameEnd) {
            formation->contactType = 0;
            vfRegisterFormationSlotForBattle(group->character);
            game->flags &= ~PlayerAggro;
            g_vfPlayerAggroCombat = 1;
        }
        return;
    }
    if (formation->scriptParameter)
        return;
    if (game->remainingEnemies >= config.enemyCountThreshold)
        return;
    const VFActor *const player = game->actors[game->playerSlot];
    if (player->currentHp <= 0 || (player->moveFlags & kVFForceDefeat) != 0)
        return;
    vfUpdateDefaultReactionAnim(group, config.initialReactionId);
    ++formation->scriptParameter;
    if (!reactionType) {
        game->flags |= PlayerAggro | EnemyAggro;
    } else {
        game->flags |= PlayerAggro;
        if (reactionType == 2 && group->character) {
            Character *const character = group->character;
            const bool wasVisible = ((character->motionFlags >> 2U) & 1U) != 0;
            character->motionFlags |= 4U;
            if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
                CHAR_LogNPCVisibility(character, true);
        }
    }
    g_vfPlayerAggroCombat = 0;
}

void vfUpdateAliveCounters(VFMAData *game) {
    VFFightStage *stage = g_pFightStage;
    std::uint8_t totalActive = static_cast<std::uint8_t>(stage->activeFightersByTeam[0] + stage->activeFightersByTeam[1]);
    for (; totalActive > 4; --totalActive) {
        const std::uint8_t rank = vfPickOpponentSlotByPrioRank(game, 0);
        if (rank == 99)
            break;
        vfEnemySlotResetAndRestartCycle(game, game->battleMeta.groups[rank]);
    }
    std::uint8_t aliveCounters[2]{};
    std::uint8_t normalCounters[2]{};
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        const VFActor *const actor = game->actors[index];
        if ((actor->moveFlags & kVFForceDefeat) != 0)
            continue;
        const std::uint8_t team = game->slotTeamId[index];
        ++aliveCounters[team];
        if ((actor->motionFlags & 0x800U) == 0)
            ++normalCounters[team];
    }
    for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
        VFFormationGroup *const formation = &stage->formations[index];
        const std::uint8_t opposingTeam = static_cast<std::uint8_t>(formation->teamId) ^ 1U;
        if (!stage->activeFightersByTeam[opposingTeam] && !stage->teamRespawnTimer[opposingTeam])
            continue;
        Character *const character = formation->character;
        if (formation->waveId || !character || character == kVFFormationSpawnCharacter || formation->blockedBy || formation->slot->state != SS_Ready ||
            formation->contactType) {
            continue;
        }
        const std::uint32_t waveFlags = formation->waveFlags;
        bool inImmediateRange = false;
        bool activate = false;
        if (((waveFlags & 1U) != 0 && totalActive < 4) ||
            (((game->battleType - 1U) & 0xfbU) == 0 && (formation->selectFlags & 0xa0U) == 0 && totalActive < 4)) {
            activate = true;
        } else if (game->playerSlot != -1) {
            float activationRadius = 2.5f;
            if ((waveFlags & 8U) != 0)
                activationRadius = 10.0f;
            else if ((waveFlags & 4U) != 0)
                activationRadius = 7.0f;
            else if ((waveFlags & 2U) != 0)
                activationRadius = 5.0f;
            const Vector3 position = GetPositionForEntity(character->entity.node);
            const VFActor *const player = game->actors[game->playerSlot];
            const float distance = std::sqrt(
                (position.x - player->position.x) * (position.x - player->position.x) + (position.z - player->position.z) * (position.z - player->position.z));
            if (activationRadius > distance) {
                inImmediateRange = distance < 2.5f;
                activate = true;
            }
        }
        if (((waveFlags & 0x800U) == 0 || normalCounters[static_cast<std::uint8_t>(formation->teamId)]) && !activate)
            continue;
        const std::uint8_t teamB = stage->activeFightersByTeam[1];
        const std::uint8_t teamA = stage->activeFightersByTeam[0];
        if (static_cast<std::uint8_t>(teamA + teamB) >= 4) {
            if (!inImmediateRange)
                continue;
            const std::uint8_t rank = vfPickOpponentSlotByPrioRank(game, static_cast<std::uint8_t>(formation->selectFlags));
            if (rank != 99)
                vfEnemySlotResetAndRestartCycle(game, game->battleMeta.groups[rank]);
            else if (teamA + teamB == 6)
                continue;
        }
        const bool registered = vfSetupContactSlotAndUpdateState(game, formation);
        stage = g_pFightStage;
        if (registered)
            ++aliveCounters[static_cast<std::uint8_t>(formation->teamId)];
    }
    stage->aliveByTeam[0] = aliveCounters[0];
    stage->aliveByTeam[1] = aliveCounters[1];
}

void vfSetReferenceBoard(Character *character, std::uint32_t sector, bool clearMode) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, SHEN_NAME(REFB)) : nullptr;
    auto *const refboard = component ? reinterpret_cast<std::byte *>(component) + 0x40 : nullptr;
    if (!refboard)
        return;
    std::memcpy(refboard + 0x18, &sector, sizeof(sector));
    if (clearMode) {
        constexpr std::int32_t kMode = 0;
        std::memcpy(refboard + 0x1c, &kMode, sizeof(kMode));
    }
    constexpr std::uint32_t kStatus = 1;
    std::memcpy(refboard + 0x28, &kStatus, sizeof(kStatus));
}

void vfPublishReferenceBoard(VFEnemySlot *group) {
    CharacterComponentNode *const component = group->character ? HLib_GetCharacterComponent(group->character, SHEN_NAME(REFB)) : nullptr;
    auto *const refboard = component ? reinterpret_cast<std::byte *>(component) + 0x40 : nullptr;
    if (!refboard)
        return;
    constexpr std::uint32_t kStatus = 1;
    std::memcpy(refboard + 0x28, &kStatus, sizeof(kStatus));
    std::memcpy(refboard + 0x0c, &group->referenceOffset, sizeof(group->referenceOffset));
}

// @verified - p
void vfSetupRefb(VFMAData *game) {
    constexpr Vector3 kDelta{1.0f, 0.2f, 0.2f};
    constexpr float kScaleDelta = 0.33f;
    if (!g_updateRenderFrameMode)
        return;
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot *const group = &stage->enemySlots[index];
        if (!group->character)
            continue;
        if ((game->flags & PlayerAggro) != 0)
            return;
        if (CurrentRyo() == group->character)
            continue;
        const std::uint8_t fightStyle = group->wave->fightStyle;
        if (((fightStyle - 20U) & 0xf6U) == 0 && fightStyle != 0x15U)
            continue;
        if ((game->gameFlags & 0x4000000U) == 0 && static_cast<std::uint8_t>(group->state - 2) <= 1) {
            group->referenceRamp += kDelta.x / 10.0f;
            group->referenceOffset.x += kDelta.y / 10.0f;
            group->referenceOffset.y += kDelta.z / 10.0f;
            group->referenceOffset.z += kScaleDelta / 10.0f;
            if (group->referenceRamp > kDelta.x)
                group->referenceRamp = kDelta.x;
            if (group->referenceOffset.x > kDelta.y)
                group->referenceOffset.x = kDelta.y;
            if (group->referenceOffset.y > kDelta.z)
                group->referenceOffset.y = kDelta.z;
            if (group->referenceOffset.z > kScaleDelta)
                group->referenceOffset.z = kScaleDelta;
            vfSetReferenceBoard(group->character, 1, false);
            vfPublishReferenceBoard(group);
            continue;
        }
        group->referenceRamp -= kDelta.x / 10.0f;
        group->referenceOffset.x -= kDelta.y / 10.0f;
        group->referenceOffset.y -= kDelta.z / 10.0f;
        group->referenceOffset.z -= kScaleDelta / 10.0f;
        if (0.0f > group->referenceOffset.x)
            group->referenceOffset.x = 0.0f;
        if (0.0f > group->referenceOffset.y)
            group->referenceOffset.y = 0.0f;
        if (0.0f > group->referenceOffset.z)
            group->referenceOffset.z = 0.0f;
        if (0.0f > group->referenceRamp) {
            group->referenceRamp = 0.0f;
            vfSetReferenceBoard(group->character, 0, true);
            continue;
        }
        vfSetReferenceBoard(group->character, 1, false);
        vfPublishReferenceBoard(group);
    }
}

void vfUpdateBattleCamTargets(VFMAData *game) {
    bool nearby[kBattleSlots]{};
    std::int32_t playerSlot = game->playerSlot;
    if (playerSlot == -1 || !game->characters[playerSlot] || (game->actors[playerSlot]->moveFlags & kVFForceDefeat) != 0) {
        playerSlot = 0;
        while (!game->characters[playerSlot] || (game->actors[playerSlot]->moveFlags & kVFForceDefeat) != 0) {
            if (++playerSlot >= static_cast<std::int32_t>(kBattleSlots))
                return;
        }
    }
    const Vector3 playerPosition = game->characters[playerSlot] ? GetPositionForEntity(game->characters[playerSlot]->entity.node) : Vector3{};
    Vector3 positions[kBattleSlots]{};
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (game->characters[index])
            positions[index] = GetPositionForEntity(game->characters[index]->entity.node);
    }
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (game->characters[index] && (game->actors[index]->moveFlags & kVFForceDefeat) == 0 &&
            (positions[index].z - playerPosition.z) * (positions[index].z - playerPosition.z) +
                    (positions[index].x - playerPosition.x) * (positions[index].x - playerPosition.x) <
                16.0f) {
            nearby[index] = true;
        }
    }
    std::uint8_t first = 0;
    std::uint8_t second = 0;
    float farthest = 0.0f;
    for (std::uint8_t left = 0; left < kBattleSlots; ++left) {
        if (!nearby[left])
            continue;
        for (std::uint8_t right = left + 1; right < kBattleSlots; ++right) {
            if (!nearby[right])
                continue;
            const float distance = (positions[right].z - positions[left].z) * (positions[right].z - positions[left].z) +
                (positions[right].x - positions[left].x) * (positions[right].x - positions[left].x);
            if (distance > farthest) {
                farthest = distance;
                first = left;
                second = right;
            }
        }
    }
    if (farthest == 0.0f)
        return;
    Character *const targetA = game->characters[first];
    Character *const targetB = game->characters[second];
    if ((targetA == game->primaryCameraTarget && targetB == game->secondaryCameraTarget) ||
        (targetA == game->secondaryCameraTarget && targetB == game->primaryCameraTarget))
        return;
    const bool firstToggle = game->cameraTargetToggled == 0;
    game->primaryCameraTarget = targetA;
    game->secondaryCameraTarget = targetB;
    if (!firstToggle)
        return;
    shenResetCombatCameraForTransition();
    g_bIsConversationCamInactive = 0;
    g_VF_camera_active = g_vfWantsCamera != nullptr;
    if (game->battleType != 1) {
        if ((g_searchLockOnFlags & 1U) != 0) {
            g_searchLockOnFlags &= ~7U;
            SRCH_Reset();
        }
        g_searchAnyInputPressedLatched = 0;
        g_searchAPressedLatched = 0;
        g_previousAuthorState = 10;
        ApplyPendingGameState(10);
    }
    game->cameraTargetToggled = 1;
}

void vfmaUpdateActorTriggerFlags(VFMAData *game) {
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        VFActor *const actor = game->actors[index];
        const std::uint32_t mask = actor->triggerMask;
        if ((mask & 0x10U) != 0 && actor->fighterRankId != 0xffU)
            actor->triggerState |= 0x10U;
        if ((mask & 0x4U) != 0 && static_cast<std::int8_t>(actor->fighterRankId) == static_cast<std::int32_t>(actor->requiredFighterRank))
            actor->triggerState |= 0x4U;
        if ((mask & 0x40U) != 0 && (actor->hitStateFlags2 & 0x2000U) != 0)
            actor->triggerState |= 0x40U;
        if ((mask & 0x80U) != 0 && (actor->hitStateFlags2 & 0x4000U) != 0)
            actor->triggerState |= 0x80U;
        if ((mask & 0x100U) != 0 && actor->cameraActiveCounterFrames == actor->cameraActiveTriggerFrames) {
            actor->triggerState |= 0x100U;
            actor->cameraActiveCounterFrames = 0;
        }
        if ((mask & 0x200U) != 0 && actor->cameraIdleCounterFrames == actor->cameraIdleTriggerFrames) {
            actor->triggerState |= 0x200U;
            actor->cameraIdleCounterFrames = 0;
        }
    }
}

void vfmaUpdateEnemyPriority(VFMAData *game) {
    static constexpr std::uint8_t kEngagementGroups[16]{3, 3, 2, 2, 3, 1, 2, 3, 2, 1, 2, 1, 1, 1, 1, 0};
    game->remainingEnemies =
        static_cast<std::uint8_t>(g_pFightStage->readyEnemySlots[1] + g_pFightStage->activeFightersByTeam[1] + g_pFightStage->teamRespawnTimer[1]);
    const std::int8_t playerSlot = game->playerSlot;
    if (playerSlot == -1) {
        for (std::int8_t &rank : game->enemyRanks)
            rank = 1;
        return;
    }
    std::uint8_t slots[kBattleSlots];
    float distances[kBattleSlots];
    const VFActor *const actor = game->actors[playerSlot];
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        game->enemyRanks[index] = 0;
        slots[index] = index;
        if (!game->characters[index])
            distances[index] = 9999.9004f;
        else if (index == static_cast<std::uint8_t>(game->playerSlot))
            distances[index] = 9998.9004f;
        else if (static_cast<std::int8_t>(index) == static_cast<std::int8_t>(game->primaryTargetSlot))
            distances[index] = 0.1f;
        else if (static_cast<std::int8_t>(index) == actor->targetSlotIndex)
            distances[index] = 0.2f;
        else
            distances[index] = actor->contactPoints[index];
    }
    for (std::uint8_t selected = 0; selected < kBattleSlots - 1; ++selected) {
        for (std::uint8_t candidate = selected + 1; candidate < kBattleSlots; ++candidate) {
            const float selectedDistance = distances[selected];
            const float candidateDistance = distances[candidate];
            if (selectedDistance > candidateDistance) {
                const std::uint8_t slot = slots[selected];
                slots[selected] = slots[candidate];
                distances[selected] = candidateDistance;
                slots[candidate] = slot;
                distances[candidate] = selectedDistance;
            }
        }
    }
    std::uint8_t count = 0;
    if (!game->comboTimer)
        count = game->remainingEnemies ? kEngagementGroups[game->remainingEnemies / 5U] : 3;
    game->engagedEnemies = static_cast<std::int8_t>(count);
    for (std::uint8_t rank = 0; rank < count; ++rank)
        game->enemyRanks[slots[rank]] = static_cast<std::int8_t>(rank + 1);
}

bool vfTransitionToInactive(Character *character) {
    VFMAData *const game = vfMainGetCurrent();
    if (!game)
        return false;
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot *const group = &stage->enemySlots[index];
        if (group->character != character)
            continue;
        if (group->state != SS_Cooldown)
            return false;
        vfTransitionEnemySlotToInactive(game, group);
        vfApplyEnemySlotAxisOffsetDecay(static_cast<std::int8_t>(index));
        return true;
    }
    return false;
}

void vfUpdateInputTimers(VFMAData *game) {
    if (game->guardEffectTimer != 0xffffffffU && game->state < VFS_EvaluateResult)
        --game->guardEffectTimer;
    if (game->soundTimer)
        --game->soundTimer;
    if (game->soundResetTimer)
        --game->soundResetTimer;
    EFFE_FadeVFFightGroundEffects();
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        VFActor *const actor = game->actors[index];
        if (actor->hpRegenerationFrames < 0xfde8U)
            ++actor->hpRegenerationFrames;
    }
    bool dialogueActive = false;
    for (std::size_t index = 0; index < 2; ++index) {
        if (g_pCVWK->slots[index].callback)
            dialogueActive = true;
    }
    if (dialogueActive || game->voiceCodeIndex != -1)
        game->timer6 = 240;
    else if (game->timer6)
        --game->timer6;
    if (game->hpDerivedTimer)
        --game->hpDerivedTimer;
    if (game->runningCooldownFrames)
        --game->runningCooldownFrames;
    if (game->runningFrames && (g_inputState.slots[static_cast<std::uint8_t>(game->playerSlot)].held & g_vfRunButtonId) == 0)
        --game->runningFrames;
    if (game->comboTimer)
        --game->comboTimer;
}

void vfPrepareFightStartPad(VFMAData *game) {
    game->padCaptureEntry = AllocateClearedPADCEntry();
    game->inputBuffer = shenUpdateInputsInto(static_cast<PadCaptureEntry *>(game->padCaptureEntry));
}

void vfFreeFightStageAndAll() {
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        if (!stage->enemySlots[index].character)
            continue;
        VFActor *const fighter = stage->enemySlots[index].fighter;
        HLib_DisableTokenCheck(fighter->slot);
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fighter->slot), 0);
        HLib_DisableTokenCheck(fighter);
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fighter), 0);
    }
    HLib_DisableTokenCheck(stage->enemySlots);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(stage->enemySlots), 0);
    stage->enemySlots = nullptr;
}

void vfFightTaskTeardown(VFMAData *game) {
    if (g_vfShouldLoad70mbAssets) {
        MFBT_ReleaseResources();
        g_lcdmHelpIconSpritesLoaded = 1;
        shenLoadSprite(7, 0, 3);
        g_70ManIconTransitionState = 0;
    }
    g_vfShouldLoad70mbAssets = 0;
    g_VF_camera_active = 0;
    if ((g_searchLockOnFlags & 1U) != 0) {
        g_searchLockOnFlags &= ~7U;
        SRCH_Reset();
    }
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 9;
    ApplyPendingGameState(9);
    if ((g_searchLockOnFlags & 1U) != 0) {
        g_searchLockOnFlags &= ~7U;
        SRCH_Reset();
    }
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 0;
    ApplyPendingGameState(0);
    shenUpdateInputsInto(static_cast<PadCaptureEntry *>(game->inputBuffer));
    ReleasePADCEntry(static_cast<PadCaptureEntry *>(game->padCaptureEntry));
    if (game->inputLockOnStart == 1)
        navResetAuthAndInputStates();
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot *const group = &stage->enemySlots[index];
        if (group->character && group->state != SS_Inactive) {
            group->character->renderOpacity = 1.0f;
            vfTransitionEnemySlotToInactive(game, group);
        }
    }
    EFFE_FreeVFFightGroundEffects();
    vfFreeStageObjects();
    for (void *&style : stage->styles) {
        if (style)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(style), 0);
        style = nullptr;
    }
    if (g_pVFGW_Widget)
        HLib_DisableTokenCheck(g_pVFGW_Widget);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_pVFGW_Widget), 0);
    g_pVFGW_Widget = nullptr;
    vfFreeBattlePack(game);
    g_hazukiResidenceGatedFlag = 1;
    vfUpdateUI(game);
    if (game->battleType == 5)
        UI_SICNDispatch(1, 0, UISicnPausePolicy::ClearOnPause);
    UI_CloseRetry70ManBattlePrompt();
    SCEN_SetHelpToken(game->helpIconId < 68 ? game->helpIconId : 0);
    g_vfPlayerAggroCombat = 1;
    g_bIsFighting = 0;
    lcdmInit(0);
    if (g_globalFlags[0] == 80)
        VFMA_TaskPtr = nullptr;
    HLib_DestroyTask(game->gameInfoTask);
    HLib_DestroyTask(game->collisionTask);
    HLib_DestroyTask(CurrentTask);
    vfFreeFightStageAndAll();
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(stage->formations), 0);
    stage->formations = nullptr;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(stage->areaMasks), 0);
    stage->areaMasks = nullptr;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(stage->anchors), 0);
    stage->anchors = nullptr;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(stage), 0);
    g_pFightStage = nullptr;
}

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

VFMAData *vfMainGetCurrent() {
    HLib::Task *const root = &g_TaskInfo.taskQueue.rootTask;
    HLib::Task *task = root;
    while (task->r8b != 9U) {
        task = task->nextInQueue;
        if (task == root)
            return nullptr;
    }
    return reinterpret_cast<VFMAData *>(task->task_data_ptr);
}

VFActor *vfFindFighterFor(Character *character) {
    VFFightStage *const stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        if (stage->enemySlots[index].character == character)
            return stage->enemySlots[index].fighter;
    }
    return nullptr;
}

void vfg_Callback_GameInfoEntry(HLib::TaskData *taskData) {
    auto *const game = reinterpret_cast<VFMAData *>(taskData);
    if (!game)
        return;
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (game->characters[index]) {
            vfUpdateHitFacingAndKnockback(*game->actors[index]);
            vfUpdateContactSlotsAndGroundFlags(*game->actors[index]);
        }
    }
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (game->characters[index])
            vfSelectDefaultReactionAndGuardAnims(*game->actors[index]);
    }
    HLib::Task *const infoTask = game->gameInfoTask;
    HLib::Task *const fightTask = game->task;
    if (infoTask && fightTask && !fightTask->task_data_ptr) {
        fightTask->task_data_ptr = infoTask->task_data_ptr;
        infoTask->task_data_ptr = nullptr;
    }
}

void vfm_Callback_FightTaskEntry_Callback(HLib::TaskData *taskData) {
    auto *const game = reinterpret_cast<VFMAData *>(taskData);
    VFFightStage *stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        if (stage->enemySlots[index].character)
            stage->enemySlots[index].isRunning &= ~1;
    }
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (game->characters[index])
            game->actors[index]->triggerState &= ~0xf00U;
    }
    const bool inBattle = static_cast<std::uint32_t>(game->state) < VFS_EvaluateResult;
    game->extraValue = 0;
    if (inBattle) {
        for (void (*const callback)(VFActor &) : g_vfmaCoreCallbacks) {
            for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
                if (game->characters[index])
                    callback(*game->actors[index]);
            }
        }
    }
    vfUpdateInputTimers(game);
    if (game->battleType == 5)
        g_uiElements[kUi70ManBattle].isActive = 1;
    std::uint16_t resultRank = 0;
    switch (game->state) {
    case VFS_Init:
        game->helpIconId = SCEN_GetCurrentHelpToken();
        game->field2ec = 0;
        game->state = VFS_AttachAndFlag;
        if (game->battleType == 2)
            SCEN_SetHelpToken(0x37);
        else if (game->helpIconId != 0x0f)
            SCEN_SetHelpToken(0x14);
        if (g_globalFlags[0] == 80 && !VFMA_TaskPtr)
            VFMA_TaskPtr = reinterpret_cast<void *>(std::uintptr_t{1});
        [[fallthrough]];
    case VFS_AttachAndFlag:
        game->state = VFS_SetupParticipants;
        if (game->packData)
            game->packData->flags |= BTPKF_ActivateRequested;
        [[fallthrough]];
    case VFS_SetupParticipants:
        InitVFGW();
        for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
            if (game->characters[index])
                vfUpdateGuardLevel(game, index);
        }
        game->state = VFS_InBattle;
        [[fallthrough]];
    case VFS_InBattle: {
        vfUpdateRenderOpacityForAll(game);
        vfUpdateDefeatAndRecycleSlotsOnDownedActors(game);
        vfUpdateEnemySlotFormationAndYaw(game);
        if ((game->flags & EnemyAggro) != 0) {
            for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
                if (game->characters[index] && !game->slotTeamId[index] && index != static_cast<std::uint8_t>(game->playerSlot))
                    vfTransitionEnemySlotToInactive(game, game->battleMeta.groups[index]);
            }
        }
        vfUpdateAliveCounters(game);
        vfSetupRefb(game);
        stage = g_pFightStage;
        const std::uint8_t teamA = stage->activeFightersByTeam[0];
        const std::uint8_t teamB = stage->activeFightersByTeam[1];
        if ((teamA || stage->teamRespawnTimer[0]) && (teamB || stage->teamRespawnTimer[1])) {
            if (game->guardEffectTimer) {
                vfAssignNearestOpponents(game);
                vfUpdateBattleCamTargets(game);
                stage = g_pFightStage;
            } else {
                std::uint8_t results[2]{};
                if (!stage->aliveByTeam[1]) {
                    results[0] = stage->aliveByTeam[0] ? 3 : 4;
                    results[1] = stage->aliveByTeam[0] ? 4 : 4;
                } else if (!stage->aliveByTeam[0]) {
                    results[0] = 4;
                    results[1] = 3;
                } else if (teamA > teamB) {
                    results[0] = 3;
                    results[1] = 4;
                } else if (teamA < teamB) {
                    results[0] = 4;
                    results[1] = 3;
                } else {
                    results[0] = 7;
                    results[1] = 7;
                }
                for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
                    VFFormationGroup *const formation = &stage->formations[index];
                    if (formation->character && formation->waveId == 1) {
                        const std::uint8_t result = results[static_cast<std::uint8_t>(formation->teamId)];
                        formation->waveId = static_cast<std::int8_t>(result);
                        g_vfFightRegistry[static_cast<std::uint8_t>(formation->slot->registryIndex)].slot = static_cast<std::int8_t>(result);
                    }
                }
                game->state = VFS_EvaluateResult;
            }
        } else {
            for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
                VFFormationGroup *const formation = &stage->formations[index];
                if (formation->character && formation->waveId == 1) {
                    formation->waveId = 3;
                    g_vfFightRegistry[static_cast<std::uint8_t>(formation->slot->registryIndex)].slot = 3;
                }
            }
            game->state = VFS_EvaluateResult;
        }
        vfmaUpdateActorTriggerFlags(game);
        vfmaUpdateEnemyPriority(game);
        for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
            VFEnemySlot &group = stage->enemySlots[index];
            if (!group.character || group.state != SS_Cooldown)
                continue;
            group.triggerFlags &= ~8U;
            if ((group.triggerFlags & 4U) != 0)
                group.triggerFlags |= 8U;
            group.triggerFlags &= ~4U;
        }
        if (g_areaId == SHEN_NAME(MFBT) && game->defeatedEnemies != static_cast<std::int32_t>(g_mfbtCurrentDefeatedEnemies)) {
            if (game->battleType == 1)
                lcdmSetValues(static_cast<std::int8_t>(g_defaultHpRegenProfile), game->defeatedEnemies);
            else if (game->battleType == 5)
                lcdmSetValues(0, game->defeatedEnemies);
        }
        for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
            const VFEnemySlot &group = stage->enemySlots[index];
            if (!group.character)
                continue;
            float health = 0.0f;
            if (group.state == SS_Active) {
                const VFActor *const fighter = group.fighter;
                if (fighter->maximumHp) {
                    health = static_cast<float>(fighter->currentHp) / static_cast<float>(fighter->maximumHp);
                    if (std::fabs(health) < 0.00000011920929f)
                        health = 0.0f;
                }
            } else if (static_cast<std::uint8_t>(group.state - 3) > 1) {
                health = 1.0f;
            }
            g_vfFightRegistry[index].health = health;
        }
        vfgwUpdateGauge(*game->actors[game->playerSlot]);
        if (game->battleType == 4 && (game->flags & FreeBattleExitPrompt) == 0) {
            const std::uint32_t flags = game->flags;
            const std::int32_t pauseState = Shenmue_nPauseState;
            std::uint8_t previousHelpIconFlag = g_prevHelpIconFlag;
            if (g_updateRenderFrameMode != 0) {
                if ((pauseState & 2) != 0)
                    previousHelpIconFlag = 1;
                g_prevHelpIconFlag = previousHelpIconFlag;
            }
            const bool eventStopActive = (g_shenmueFlags[0x320U >> 3U] & static_cast<std::uint8_t>(1U << (0x320U & 7U))) != 0;
            VFActor *const player = game->actors[game->playerSlot];
            if (((pauseState & 2) == 0 && previousHelpIconFlag == 1) || (game->activeBattleFrames != 0 && game->activeBattleFrames % 1920U == 0) ||
                static_cast<std::uint16_t>(player->cameraIdleCounterFrames) > 320U || eventStopActive) {
                if ((flags & (FreeBattleIntroActive | FreeBattleExitActive)) == 0) {
                    game->flags = flags | FreeBattleExitPrompt;
                    shenUpdateInputsInto(static_cast<PadCaptureEntry *>(game->inputBuffer));
                    g_suppressDefaultHudPromptIcons = 1;
                    g_pControlPadController->blockedCanControl = 0;
                    g_pControlPadController->state = 2;
                    g_tmexValHigh = 0;
                    game->actors[game->playerSlot]->cameraActiveCounterFrames = 0;
                    game->actors[game->playerSlot]->cameraIdleCounterFrames = 0;
                }
                g_prevHelpIconFlag = 0;
            }
        }
        if ((game->flags & PlayerAggro) == 0)
            ++game->runtimeTicks;
        break;
    }
    case VFS_EvaluateResult: {
        resultRank = 99;
        const bool ranked = shenQueryFightTaskOrRegistryValue(game->playerCharacter) == 3;
        if (ranked) {
            if (g_vfShouldLoad70mbAssets) {
                shenComputeNumEnemies(game->runtimeTicks, g_mfbtCurrentDefeatedEnemies);
                if ((game->flags & DisableRankUpdate) == 0)
                    resultRank = shenInsertTimeIntoTop5AndGetRank(game->runtimeTicks);
            }
            if (game->playerSlot != -1) {
                VFActor *const player = game->actors[game->playerSlot];
                if ((player->hitStateFlags2 & 0x1d010U) == 0 && player->previousHitStateFlags != 4U) {
                    VFEnemySlot *const group = game->battleMeta.groups[game->playerSlot];
                    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
                    motion->motionFlags &= ~8U;
                    motion->mobjCallback = nullptr;
                    static constexpr std::uint16_t kWinAnimations[3]{0x04acU, 0x04aeU, 0x0482U};
                    vfUpdateDefaultReactionAnim(group, resultRank ? kWinAnimations[game->frameCount % 3U] : 0x04ebU);
                    if (group->character) {
                        if (FightPoseWork *const figp = FIGP_GetWork(group->character))
                            figp->active = 0;
                    }
                }
            }
        }
        g_vfPostBtlWaitFrms = 0;
        if (game->battleType == 5)
            g_b70MBState = g_vfShouldLoad70mbAssets && ranked && resultRank < 5 ? 5 : 6;
        else
            g_b70MBState = 0;
        vfSetupRefb(game);
        vfgwUpdateGauge(*game->actors[game->playerSlot]);
        std::snprintf(g_70mbLabelText[2].data(), g_70mbLabelText[2].size(), "%s",
            InitAndRetrieveStringFromID(g_b70MBState == 5 && resultRank < 5 ? k70mbStringIds[resultRank + 2U] : k70mbStringIds[1]));
        g_70ManIconTransitionState = 2;
        game->state = VFS_ResultTimeline;
        break;
    }
    case VFS_ResultTimeline:
        vfSetupRefb(game);
        vfgwUpdateGauge(*game->actors[game->playerSlot]);
        switch (g_b70MBState) {
        case 1:
            if ((game->flags & DisableRankUpdate) == 0)
                shenComputeNumEnemies(game->runtimeTicks, g_mfbtCurrentDefeatedEnemies);
            [[fallthrough]];
        case 3:
            if (g_vfPostBtlWaitFrms == 0)
                shenLoadSprite(1, 30, g_b70MBState == 1 ? 6U : 1U);
            if (++g_vfPostBtlWaitFrms > 105) {
                ++g_b70MBState;
                g_vfPostBtlWaitFrms = 0;
            }
            break;
        case 2:
            if (g_vfPostBtlWaitFrms == 0)
                shenLoadSprite(0, 30, 6);
            if (++g_vfPostBtlWaitFrms > 30) {
                g_b70MBState = game->runtimeTicks >= 5400 ? 6 : 5;
                g_vfPostBtlWaitFrms = 0;
            }
            break;
        case 4:
            if (g_vfPostBtlWaitFrms == 0)
                shenLoadSprite(0, 30, 1);
            if (++g_vfPostBtlWaitFrms > 30) {
                g_b70MBState = 6;
                g_vfPostBtlWaitFrms = 0;
            }
            break;
        case 5:
        case 6:
            if (++g_vfPostBtlWaitFrms > 90)
                game->state = VFS_PostResultSetup;
            break;
        default:
            game->state = VFS_PostResultSetup;
            break;
        }
        break;
    case VFS_PostResultSetup:
        game->state = VFS_WaitSettle;
        g_vfPostBtlWaitFrms = 0;
        vfSetupRefb(game);
        vfgwUpdateGauge(*game->actors[game->playerSlot]);
        if (game->battleType == 5U && g_vfShouldLoad70mbAssets && shenQueryFightTaskOrRegistryValue(game->playerCharacter) == 3)
            shenComputeNumEnemies(game->runtimeTicks, g_mfbtCurrentDefeatedEnemies);
        break;
    case VFS_WaitSettle: {
        if (game->battleType == 5U && g_vfShouldLoad70mbAssets && shenQueryFightTaskOrRegistryValue(game->playerCharacter) == 3)
            shenComputeNumEnemies(game->runtimeTicks, g_mfbtCurrentDefeatedEnemies);
        std::uint8_t settled = 0;
        for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
            Character *const character = game->characters[index];
            if (!character) {
                ++settled;
                continue;
            }
            const VFActor *const actor = game->actors[index];
            MotionManagerWork *const manager = MOTM_GetWork(character);
            const auto *const motion = static_cast<const MotionInformationWork *>(actor->motion);
            if ((manager->motion.motionFlags & 0x400U) == 0 && ((actor->hitStateFlags2 & 0x40000U) == 0 || motion->frameEnd <= motion->frameTimer))
                ++settled;
        }
        if (settled == kBattleSlots || ++g_vfPostBtlWaitFrms > 90) {
            game->state = VFS_PostBattleDelay;
            g_vfPostBtlWaitFrms = 60;
            g_70ManIconTransitionState = 3;
            if (g_vfShouldLoad70mbAssets) {
                g_lcdmHelpIconSpritesLoaded = 1;
                g_helpIconHeight = 128;
                g_helpIconWidth = 342;
                shenLoadSprite(7, 0, 3);
            }
        }
        if (g_areaId == SHEN_NAME(MFBT) && ((game->battleType - 1U) & 0xfbU) == 0) {
            LcdmTaskData *const lcdm = g_lcdmTaskData;
            if (lcdm) {
                lcdm->data = lcdmPackedBitmapTable[1];
                lcdm->field_000 = 1;
            }
            g_vfPostBtlWaitFrms = 90;
            g_vfLcdmEndShown = 0;
            g_lcdmScoreBlinkTimer = 15;
            g_lcdmScoreBlinkPhase = 0;
            for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
                if (stage->formations[index].character != game->playerCharacter)
                    continue;
                if (stage->formations[index].waveId == 3) {
                    g_vfLcdmEndShown = 1;
                    if (lcdm)
                        lcdmInit(reinterpret_cast<std::uintptr_t>(lcdm->endText));
                }
                break;
            }
        }
        vfSetupRefb(game);
        vfgwUpdateGauge(*game->actors[game->playerSlot]);
        break;
    }
    case VFS_PostBattleDelay: {
        std::int8_t round = static_cast<std::int8_t>(g_defaultHpRegenProfile);
        if (--g_vfPostBtlWaitFrms == 0) {
            vfmaAccumulatePlayerCombatStats(*game);
            game->state = VFS_Teardown;
            if (game->battleType == 5) {
                g_70ManIconTransitionState = 0;
                UI_ShowRetry70ManBattlePrompt(HandleRetry70ManBattlePrompt);
                game->state = VFS_WaitRetryDialog;
            }
            if (LcdmTaskData *const lcdm = g_lcdmTaskData) {
                lcdm->data = lcdmPackedBitmapTable[0];
                lcdm->field_000 = 1;
            }
            const std::int32_t scenario = shenQueryFightTaskOrRegistryValue(game->playerCharacter);
            g_playerStats.vfHpRegenerationProfile = g_defaultHpRegenProfile;
            if (((game->battleType - 2U) & 0xfcU) != 0 || game->battleType == 3) {
                if (scenario == 2 || scenario == 5) {
                    ++g_activeHpRegenProfile;
                } else {
                    round = 0;
                    g_defaultHpRegenProfile = 0;
                }
            } else {
                round = 0;
                g_defaultHpRegenProfile = 0;
            }
        }
        if (g_areaId == SHEN_NAME(MFBT) && ((game->battleType - 1U) & 0xfbU) == 0 && !g_vfLcdmEndShown) {
            bool blank = false;
            if (--g_lcdmScoreBlinkTimer < 0) {
                g_lcdmScoreBlinkTimer = 15;
                blank = ++g_lcdmScoreBlinkPhase > 3;
            }
            if (blank || !g_lcdmScoreBlinkPhase) {
                lcdmInit(reinterpret_cast<std::uintptr_t>(g_lcdmBlankBitmap));
                g_lcdmScoreBlinkPhase = 1;
            } else if (g_lcdmScoreBlinkPhase == 2) {
                lcdmSetValues(game->battleType == 5 ? 0 : round, game->defeatedEnemies);
                g_lcdmScoreBlinkPhase = 3;
            }
        }
        if (g_vfShouldLoad70mbAssets)
            HLib_InertTaskCallback(nullptr);
        shenRefreshUI();
        break;
    }
    case VFS_Teardown:
        vfFightTaskTeardown(game);
        return;
    default:
        break;
    }
    ++game->frameCount;
    if ((game->flags & (FreeBattleIntroActive | FreeBattleExitActive)) == 0)
        ++game->activeBattleFrames;
    stage = g_pFightStage;
    g_mfbtCurrentDefeatedEnemies = static_cast<std::uint32_t>(game->defeatedEnemies);
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot &group = stage->enemySlots[index];
        if (!group.character)
            continue;
        if (group.tickFrames < 65500)
            ++group.tickFrames;
        if ((group.isRunning & 1) == 0)
            continue;
        Vector3 scratch{};
        VFActor *const fighter = group.fighter;
        if (fighter && group.state == SS_Active)
            motCalcDeltaXZFromAnimTime(*static_cast<MotionInformationWork *>(group.motion), fighter->knockbackStrength, fighter->moveDelta);
        else
            motCalcDeltaXZFromAnimTime(*static_cast<MotionInformationWork *>(group.motion), 1.0f, scratch);
    }
    vfQueueVoiceReaction(*game);
    HLib::Task *const fightTask = game->task;
    HLib::Task *const infoTask = game->gameInfoTask;
    if (fightTask && infoTask && !infoTask->task_data_ptr) {
        infoTask->task_data_ptr = fightTask->task_data_ptr;
        fightTask->task_data_ptr = nullptr;
    }
}

std::uint32_t EnqueueVFGI(std::int32_t startSlot, std::int32_t maximumEnemies, float knockbackAmount, std::uint8_t battleType, std::int8_t guardEffectSeconds) {
    g_prevHelpIconFlag = 0;
    if (battleType == 99) {
        VFMAData *const game = vfMainGetCurrent();
        if (!game || game->battleType != 4)
            return 0;
        g_suppressDefaultHudPromptIcons = 1;
        g_pControlPadController->blockedCanControl = 0;
        g_pControlPadController->state = 2;
        g_tmexValHigh = 0;
        game->inputBuffer = shenUpdateInputsInto(static_cast<PadCaptureEntry *>(game->padCaptureEntry));
        game->flags &= ~FreeBattleExitPrompt;
        game->runtimeTicks = 0;
        return 1;
    }
    if (battleType == 5)
        g_70ManIconTransitionState = 1;
    auto *const game = static_cast<VFMAData *>(vfQueueStartGameMain());
    VFFightStage *stage = g_pFightStage;
    game->name = shen_name{kFightName};
    game->playerSlot = -1;
    game->primaryTargetSlot = 0xffU;
    game->playerCharacter = nullptr;
    game->field160 = 0;
    game->state = VFS_Init;
    game->knockbackSlideDragScale = 10.0f;
    game->field6c = -0.5f;
    game->field70 = 0.60000002f;
    game->knockbackGravityScale = 0.010888889f;
    game->guardEffectSeconds = static_cast<std::uint32_t>(static_cast<std::int32_t>(guardEffectSeconds));
    g_shenFightState2 = battleType;
    g_bIsFighting = 1;
    game->battleType = battleType;
    game->maxActiveEnemies = static_cast<std::uint8_t>(maximumEnemies) > stage->maxEnemies ? stage->maxEnemies : static_cast<std::uint8_t>(maximumEnemies);
    game->packData = nullptr;
    game->playerReactionCameraScale = static_cast<float>(g_playerStats.cameraShakeAmount) / 100.0f;
    game->playerAssistScale = battleType == 5 ? 0.40000001f : static_cast<float>(g_playerStats.fightAssistScale) / 100.0f;
    game->gameFlags = 0;
    game->knockbackAmount = knockbackAmount >= 0.2f ? std::fmin(1.0f, knockbackAmount) : 0.2f;
    game->knockbackDragScale = 0.5f;
    game->flags = 0;
    game->extraValue = 0;
    game->cameraTargetToggled = 0;
    game->secondaryCameraTarget = nullptr;
    game->primaryCameraTarget = nullptr;
    game->ringBoundary[0] = stage->ringBoundary.x;
    game->ringBoundary[1] = stage->ringBoundary.y;
    game->ringBoundary[2] = stage->ringBoundary.z;
    for (Character *&character : game->characters)
        character = nullptr;
    game->frameCount = 0;
    game->activeBattleFrames = 0;
    game->runtimeTicks = 0;
    game->timer6 = 0;
    game->runningFrames = 0;
    game->runningCooldownFrames = 0;
    game->comboTimer = 0;
    game->partyFacingFrames = 0;
    game->hpDerivedTimer = battleType == 7 ? 5400 : 13 * stage->accumulatedHp + 12600;
    game->guardEffectTimer = game->guardEffectSeconds == 0xffffffffU ? 0xffffffffU : 30U * game->guardEffectSeconds;
    game->remainingEnemies = static_cast<std::uint8_t>(stage->readyEnemySlots[1] + stage->activeFightersByTeam[1] + stage->teamRespawnTimer[1]);
    vfMonitorDrawMode(game);
    game->soundResetTimer = 0;
    game->soundFlag = 0;
    game->soundFrames = 0;
    game->soundTimer = 0;
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        game->soundSlotOrder[index] = 0;
        game->soundSlots[index] = 0;
        game->soundSlotState[index] = 0;
        if (game->characters[index])
            game->actors[index]->slot->slotIndex = 0xffU;
    }
    vfSetupAreaMaskTable();
    stage = g_pFightStage;
    for (std::uint8_t index = 0; index < stage->maxEnemies; ++index) {
        VFEnemySlot *const group = &stage->enemySlots[index];
        if (!group->character)
            continue;
        auto *const fighter = static_cast<VFActor *>(HLib_AllocHeapMemBlock(kNativeActorAllocationSize, static_cast<std::int32_t>(kActorToken)));
        if (fighter)
            HLib_EnableTokenCheck(fighter);
        std::memset(fighter, 0, kNativeActorAllocationSize);
        group->fighter = fighter;
        auto *const runtime = static_cast<VFSlotRuntimeState *>(HLib_AllocHeapMemBlock(sizeof(VFSlotRuntimeState), static_cast<std::int32_t>(kSlotToken)));
        if (runtime)
            HLib_EnableTokenCheck(runtime);
        std::memset(runtime, 0, sizeof(VFSlotRuntimeState));
        group->fighter->slot = runtime;
        vfEnemySlotResetAndRestartCycle(game, group);
        group->shouldSetPosition = 0;
    }
    stage->aliveByTeam[0] = 0;
    stage->aliveByTeam[1] = 0;
    std::uint8_t requested = startSlot ? static_cast<std::uint8_t>(startSlot) : 1;
    if (requested > 4)
        requested = 4;
    std::uint8_t registered = 0;
    for (std::uint8_t index = 0; index < stage->formationCapacity; ++index) {
        VFFormationGroup *const formation = &stage->formations[index];
        if (formation->character && formation->character != kVFFormationSpawnCharacter && !formation->blockedBy &&
            vfRegisterContactSlot(game, formation->slot) != 99) {
            ++stage->aliveByTeam[formation->slot->teamId];
            ++registered;
        }
        if (registered == requested)
            break;
    }
    vfInitReactTable(*game);
    game->otherOne = 0;
    vfInitAnimTypeLUT(*game);
    for (std::size_t index = 0; index < 128; ++index) {
        game->skillTable[index] = 0;
        game->runtimeSkills[index] = 0;
    }
    game->timer2 = 0;
    game->defeatedEnemies = 0;
    game->previousDefeatedEnemies = 0;
    g_mfbtCurrentDefeatedEnemies = 0;
    if (g_areaId == SHEN_NAME(MFBT) && ((game->battleType - 1U) & 0xfbU) == 0) {
        if (g_lcdmTaskData)
            lcdmInit(reinterpret_cast<std::uintptr_t>(g_lcdmTaskData->startText));
        std::memset(g_lcdmBlankBitmap, 0, sizeof(g_lcdmBlankBitmap));
    }
    MFBT_ResetSurfaceIds();
    if (g_vfShouldLoad70mbAssets) {
        MFBT_Init(g_70mbBestTimesTicks, g_mfbtCurrentDefeatedEnemies);
    }
    game->collisionTask = EnqueueVFCL(game);
    game->gameInfoTask = HLib_EnqueueTaskWithoutParameter(vfg_Callback_GameInfoEntry, 8, 10, static_cast<std::int32_t>(SHEN_NAME(VFGI)));
    vfAssignNearestOpponents(game);
    game->packTask = EnqueueBTPK(game);
    game->playerCharacter = CurrentRyo();
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        vfResetActorReactionStateForContact(game, index);
        HND_ResetVfActorHands(game->characters[index]);
    }
    if (!g_NumFramesProcessed && CurrentRyo() && g_pControlPadController && g_tmex_val <= 1U && g_pControlPadController->blockedCanControl == 1) {
        game->inputLockOnStart = 1;
        g_pControlPadController->blockedCanControl = 0;
        g_pControlPadController->state = 2;
        g_tmexValHigh = 0;
        g_suppressDefaultHudPromptIcons = 1;
    } else {
        game->inputLockOnStart = 0;
    }
    vfPrepareFightStartPad(game);
    const std::uint8_t hpProfile = g_defaultHpRegenProfile ? g_activeHpRegenProfile : 0;
    g_activeHpRegenProfile = hpProfile;
    g_defaultHpRegenProfile = static_cast<std::uint8_t>(g_defaultHpRegenProfile + 1);
    if (game->battleType != 5) {
        if (hpProfile >= 4)
            game->actors[game->playerSlot]->triggerMask |= 0x4000000U;
        else
            game->playerAssistScale = std::fmin(1.0f, static_cast<float>(hpProfile) * 8.0f / 100.0f + game->playerAssistScale);
    }
    if (g_areaId == SHEN_NAME(JHD0))
        g_hazukiResidenceGatedFlag = 0;
    return 1;
}

std::int32_t g_70ManIconTransitionState = 0;

std::uint32_t g_70mbBestTimesTicks[5]{};

std::uint32_t g_mfbtCurrentDefeatedEnemies = 0;

std::int32_t g_VFForcePlayerGodmode = 0;

std::int32_t g_vfPlayerAggroCombat = 1;

std::uint8_t g_bIsFighting = 0;

bool VFMA_IsPlayerGodmodeForced() {
    return g_VFForcePlayerGodmode != 0;
}

std::uint8_t VFMA_ResolveEffectQuadrant(float x, float z, std::uint8_t prior) {
    return vfStageResolveQuadrantForPointXZ(x, z, prior);
}

std::int16_t VFMA_RequestMove(VFMoveRequestFlags flags) {
    const std::uint8_t rawFlags = static_cast<std::uint8_t>(flags);
    const bool commit = (rawFlags & 1U) != 0;
    const bool easing = (rawFlags & 2U) != 0;
    std::int16_t result = 0;

    const auto adjust = [easing, rawFlags](std::int16_t delta, std::int16_t current) -> std::int16_t {
        std::int32_t amount = delta;
        if (easing) {
            const std::int32_t original = amount;
            if (current < 56) {
                if (current >= 12) {
                    if (current + amount / 10 < 56) {
                        amount /= 10;
                    } else {
                        amount = 56 - current + (amount - (56 - current)) / 10;
                    }
                }
            } else {
                amount /= 20;
            }
            if (original && !amount && current < 100)
                amount = -1;
        }
        if ((rawFlags & 1U) != 0) {
            std::int32_t absolute = current;
            if (amount > 0)
                absolute += amount;
            if (absolute >= 100)
                absolute = 100;
            return static_cast<std::int16_t>(absolute);
        }
        if (current + amount < 100)
            return static_cast<std::int16_t>(amount);
        return static_cast<std::int16_t>(100 - current);
    };

    for (std::size_t index = 0; index < 20; ++index) {
        std::int16_t bankResult = 0;
        for (std::size_t bank = 0; bank < g_playerStats.moveSets.size(); ++bank) {
            MoveSetRuntimeState &moves = g_playerStats.moveSets[bank];
            const std::int16_t current = static_cast<std::int16_t>(moves.learnValue[index]);
            const std::int16_t delta = static_cast<std::int16_t>(g_playerStats.moveUsageCounts[bank][index]);
            const std::int16_t adjusted = adjust(delta, current);
            bankResult = adjusted;
            if (commit) {
                moves.learnValue[index] = static_cast<std::uint16_t>(adjusted);
                const std::int16_t moveId = static_cast<std::int16_t>(moves.moveValue[index]);
                if (moveId != -1 && current < 12 && adjusted >= 12) {
                    const std::uint16_t previous = SYGH_GetMoveSkill(static_cast<std::uint16_t>(bank + 1U), static_cast<std::uint16_t>(moveId));
                    SYGH_SetMoveSkill(static_cast<std::uint16_t>(bank + 1U), static_cast<std::uint16_t>(moveId), static_cast<std::uint16_t>(adjusted));
                    if (bank == 2U)
                        bankResult = static_cast<std::int16_t>(previous);
                }
                g_playerStats.moveUsageCounts[bank][index] = 0;
            } else {
                g_playerStats.moveUsageCounts[bank][index] = static_cast<std::uint16_t>(adjusted);
            }
        }
        result = bankResult;
    }
    return result;
}

bool VFMA_IsBattleActive() {
    const VFMAData *const game = vfMainGetCurrent();
    return game && game->state == VFS_InBattle;
}

bool VFMA_IsFighting() {
    return g_bIsFighting != 0;
}

void VFMA_SetFieldOfViewForSearchOrClear(std::uint8_t mode) {
    HLib::Task *const root = &g_TaskInfo.taskQueue.rootTask;
    HLib::Task *task = root;
    while (task && task->r8b != 9U) {
        task = task->nextInQueue;
        if (task == root)
            return;
    }
    auto *const game = task ? reinterpret_cast<VFMAData *>(task->task_data_ptr) : nullptr;
    if (!game)
        return;
    game->battleType = mode;
    g_VF_camera_active = g_vfWantsCamera ? 1 : 0;
    if (((mode - 1U) & 0xfbU) != 0) {
        SetNewAuthorGameState(10);
        return;
    }
    const VFActor *const active = game->actors[game->playerSlot];
    g_vfWantsCamera = active ? active->targetingCharacter : nullptr;
    g_VF_camera_active = g_vfWantsCamera ? 1 : 0;
    if ((g_searchLockOnFlags & 1U) != 0) {
        g_searchLockOnFlags &= ~7U;
        SRCH_Reset();
    }
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 0;
    ApplyPendingGameState(0);
}

bool VFMA_HasBoundFightStyles() {
    const VFMAData *const game = vfMainGetCurrent();
    if (!game || game->state != VFS_InBattle)
        return false;
    bool observedActor = false;
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        const VFActor *const actor = game->actors[index];
        if (!actor || !game->characters[index])
            continue;
        observedActor = true;
        if (!actor->reactionScriptBase || !actor->cameraScriptTable || !actor->styleTuning || !actor->contactProfile || !actor->animationTable) {
            return false;
        }
    }
    return observedActor;
}

bool VFMA_GetBattleCameraTargets(Character *&player, Character *&target) {
    player = nullptr;
    target = nullptr;
    VFMAData *const game = vfMainGetCurrent();
    if (!game || game->state != VFS_InBattle)
        return false;
    const std::int32_t playerSlot = game->playerSlot >= 0 && game->playerSlot < static_cast<std::int32_t>(kBattleSlots) ? game->playerSlot : 0;
    player = game->characters[playerSlot] ? game->characters[playerSlot] : game->playerCharacter;
    if (!player)
        return false;
    VFActor *const playerActor = game->actors[playerSlot];
    if (playerActor && playerActor->targetActor && playerActor->targetActor->currentHp > 0 && playerActor->targetingCharacter) {
        target = playerActor->targetingCharacter;
        return true;
    }
    if (game->primaryCameraTarget) {
        for (std::size_t index = 0; index < kBattleSlots; ++index) {
            if (game->characters[index] == game->primaryCameraTarget && game->actors[index] && game->actors[index]->currentHp > 0) {
                target = game->primaryCameraTarget;
                return true;
            }
        }
    }
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        if (static_cast<std::int32_t>(index) == playerSlot || !game->characters[index] || !game->actors[index] || game->actors[index]->currentHp <= 0 ||
            game->slotTeamId[index] == game->slotTeamId[playerSlot]) {
            continue;
        }
        target = game->characters[index];
        break;
    }
    return target != nullptr;
}

void *vfQueueStartGameMain() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(vfm_Callback_FightTaskEntry_Callback, 5, 9, static_cast<std::int32_t>(kVfmaToken));
    if (!task)
        return nullptr;

    auto *const data = static_cast<VFMAData *>(HLib_AllocHeapMemBlock(0x510, static_cast<std::int32_t>(kTaskDataToken)));
    if (!data) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }
    std::construct_at(data);
    HLib_EnableTokenCheck(data);
    data->task = task;
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    return data;
}

void Script_vfTransitionToInactive(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    SetResult(context, params && vfTransitionToInactive(FindCharacter(context, params[0])) ? 1U : 0U);
}

void Script_vfUtils(ScenarioContext *context, void *rawParams) {
    struct StageSetupParameters {
        std::uint32_t command;
        std::int32_t blockScnfKey;
        std::int8_t hpPoolMode;
        std::byte reserved09[3];
        std::int8_t groupId;
        std::byte reserved0d[3];
        std::uint16_t hpTotal;
        std::byte reserved12[2];
        std::uint8_t fightStyle;
        std::byte reserved15[3];
        std::int16_t animationId;
        std::byte reserved1a[2];
        std::int16_t spawnYaw;
        std::byte reserved1e[2];
        float centerX;
        float centerY;
        float centerZ;
    };
    static_assert(sizeof(StageSetupParameters) == 0x2c);
    const StageSetupParameters *const setup = ScenarioPointer<StageSetupParameters>(Words(rawParams)[0]);
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;
    std::int16_t animationId = 0;
    std::int16_t spawnYaw = 0;
    std::uintptr_t command = setup->command;
    switch (setup->command) {
    case 0xfffffff9U:
        g_defaultHpRegenProfile = setup->fightStyle;
        g_activeHpRegenProfile = g_defaultHpRegenProfile;
        g_playerStats.vfHpRegenerationProfile = g_defaultHpRegenProfile;
        SetResult(context, 1U);
        return;
    case 0xfffffffaU: {
        const std::uint8_t index = setup->fightStyle;
        g_playerStats.pages.vfHpRegenProfiles[index] = g_playerStats.vfHpRegenerationProfile;
        const std::uint32_t ticks = g_playerStats.pages.vfBestFightTicks[index];
        if (!ticks || g_vfRuntimeAccumFightTicks < ticks)
            g_playerStats.pages.vfBestFightTicks[index] = g_vfRuntimeAccumFightTicks;
        (void)VFMA_RequestMove(VFMoveRequestFlags::ApplyEasedCommitTrigger);
        SetResult(context, 1U);
        return;
    }
    case 0xfffffffbU:
        vfLoadFighterStyle(setup->fightStyle);
        SetResult(context, 1U);
        return;
    case 0xfffffffcU:
        centerX = setup->centerX;
        centerZ = setup->centerZ;
        break;
    case 0xffffffffU:
        animationId = setup->animationId;
        spawnYaw = setup->spawnYaw;
        [[fallthrough]];
    case 0xfffffffeU:
        centerX = setup->centerX;
        centerY = setup->centerY;
        centerZ = setup->centerZ;
        break;
    default:
        command = reinterpret_cast<std::uintptr_t>(setup->command ? FindCharacter(context, setup->command) : nullptr);
        break;
    }
    Character *const blockedBy = setup->blockScnfKey ? FindCharacter(context, static_cast<std::uint32_t>(setup->blockScnfKey)) : nullptr;
    SetResult(context,
        vfStageSetupUtil(command, blockedBy, setup->hpPoolMode, static_cast<std::uint8_t>(setup->groupId), setup->hpTotal, setup->fightStyle, animationId,
            spawnYaw, centerX, centerY, centerZ));
}


void SCN_Enqueue_VFGI(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params) {
        SetResult(context, 0U);
        return;
    }
    SetResult(context,
        EnqueueVFGI(static_cast<std::int32_t>(params[0] & 0xffU), static_cast<std::int32_t>(params[1] & 0xffU), std::bit_cast<float>(params[2]),
            static_cast<std::uint8_t>(params[3]), static_cast<std::int8_t>(params[4])));
}

void Script_CPCTRelated(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params)
        return;
    (void)FindCharacter(context, params[0]);
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(offsetof(PlayerControlWork, motionMetadata)) +
        static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(params[1])) * static_cast<std::ptrdiff_t>(sizeof(PlayerMotionMetadata));
    if (g_pControlPadController)
        std::memcpy(reinterpret_cast<std::byte *>(g_pControlPadController) + offset, &params[2], sizeof(params[2]));
}

void Script_vfSetup(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    SetResult(context,
        static_cast<std::uint32_t>(static_cast<std::int8_t>(shenStartFGST(static_cast<std::uint8_t>(params[1]), static_cast<std::uint8_t>(params[2]),
            static_cast<std::int8_t>(params[3]), static_cast<std::uint8_t>(params[4])))));
}

void Script_ptmvSetFollowTuning(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params)
        return;
    Character *const character = FindCharacter(context, params[0]);
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, kPtmvToken) : nullptr;
    if (!component)
        return;
    auto *const payload = reinterpret_cast<std::byte *>(component + 1);
    const float value = std::bit_cast<float>(params[2]);
    switch (params[1]) {
    case 0: {
        const float speed = std::min(value, 1.0f);
        std::memcpy(payload + 0x30, &speed, sizeof(speed));
        if (speed != 1.0f) {
            constexpr float walk = 1.5f;
            constexpr float run = 3.5f;
            constexpr float lateral = -0.40000001f;
            constexpr float forward = 0.0f;
            std::memcpy(payload + 0x34, &walk, sizeof(walk));
            std::memcpy(payload + 0x38, &run, sizeof(run));
            std::memcpy(payload + 0x3c, &lateral, sizeof(lateral));
            std::memcpy(payload + 0x40, &forward, sizeof(forward));
        }
        break;
    }
    case 1:
        std::memcpy(payload + 0x3c, &value, sizeof(value));
        break;
    case 2: {
        const float run = std::bit_cast<float>(params[3]);
        std::memcpy(payload + 0x34, &value, sizeof(value));
        std::memcpy(payload + 0x38, &run, sizeof(run));
        break;
    }
    default:
        break;
    }
}

void Script_GetFightingStats(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params)
        return;
    const std::uint32_t id = params[0];
    switch (params[1] & 0xffU) {
    case 0:
        SetResult(context, static_cast<std::uint32_t>(shenQueryFightTaskOrRegistryValue(FindCharacter(context, id))));
        break;
    case 1:
        SetResult(context, static_cast<std::uint32_t>(static_cast<std::int8_t>(g_mfbtCurrentDefeatedEnemies)));
        break;
    case 2:
    case 3:
        SetResult(context, static_cast<std::uint32_t>(shenQueryFightTaskOrRegistryValue(reinterpret_cast<Character *>(static_cast<std::uintptr_t>(id)))));
        break;
    case 4: {
        const VFRegistryEntry *const entry = FindRegistry(FindCharacter(context, id));
        SetResult(context, entry ? entry->health : -1.0f);
        break;
    }
    case 5:
        SetResult(context, static_cast<std::uint32_t>(g_activeHpRegenProfile));
        break;
    case 6:
        FREE_70MB();
        break;
    default:
        break;
    }
}


void SCN_GetIsFighting(ScenarioContext *context, void *) {
    SetResult(context, 2U - (g_bIsFighting ? 1U : 0U));
}

void Script_CheckIsInsideCullVolume(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    const Vector3 *const point = params ? ScenarioPointer<Vector3>(params[0]) : nullptr;
    SetResult(context, point && shenIsLocalSphereInsideViewCullVolume(*point, std::bit_cast<float>(params[1])) ? 0xffffffffU : 0U);
}

void Script_vfToggleBattleState(ScenarioContext *, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    VFMAData *const game = vfMainGetCurrent();
    if (!params || !game || game->state != VFS_InBattle)
        return;
    switch (params[0]) {
    case 0:
        game->state = VFS_PostResultSetup;
        break;
    case 1:
        game->state = VFS_PostBattleDelay;
        g_vfPostBtlWaitFrms = 1;
        break;
    case 2:
        game->flags |= ExitConfirmPending;
        break;
    default:
        break;
    }
}

void Script_vfStageAssignRingBoundary(ScenarioContext *, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    const Vector3 *const point = ScenarioPointer<Vector3>(params[0]);
    g_pFightStage->ringBoundary.y = point->x;
    g_pFightStage->ringBoundary.x = std::bit_cast<float>(params[1]);
    g_pFightStage->ringBoundary.z = point->z;
}

void Script_CPCT_related_0(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params)
        return;
    switch (params[0]) {
    case 1:
        g_cpctOperandEnabled = 1;
        SetResult(context, 0U);
        break;
    case 2:
        g_cpctLastOperand = params[1];
        SetResult(context, 0U);
        break;
    case 3:
        SetResult(
            context, params[1] == g_cpctLastOperand || params[1] == g_cpctPreviousOperand || params[1] == g_scenarioStepLut.pendingCharacterName ? 1U : 0U);
        break;
    default:
        g_cpctOperandEnabled = 0;
        SetResult(context, 0U);
        break;
    }
}

void Script_vfSetupAreaMasks(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint8_t *>(rawParams);
    std::uint32_t offset = 0;
    std::memcpy(&offset, params + 4, sizeof(offset));
    const VFAreaMask *const source = ScenarioPointer<VFAreaMask>(offset);
    VFAreaMask &destination = g_pFightStage->areaMasks[params[0]];
    if (destination.active) {
        SetResult(context, 0U);
        return;
    }
    std::memcpy(&destination, source, sizeof(VFAreaMask));
    destination.active = 1;
    SetResult(context, 1U);
}

std::uint32_t VFMA_GetRemainingFrames() {
    HLib::Task *const root = &g_TaskInfo.taskQueue.rootTask;
    for (HLib::Task *task = root; task; task = task->nextInQueue) {
        if (task->r8b == 9) {
            if (!task->task_data_ptr)
                return 0xffffffffU;
            std::uint32_t frames{};
            std::memcpy(&frames, reinterpret_cast<const std::byte *>(task->task_data_ptr) + 0x54, sizeof(frames));
            return frames == 0xffffffffU ? frames : frames / 30U;
        }
        if (!task->nextInQueue || task->nextInQueue == root)
            break;
    }
    return 0xffffffffU;
}


std::uint32_t VFMA_GetCharacterFightFlags(Character *character) {
    HLib::Task *const root = &g_TaskInfo.taskQueue.rootTask;
    HLib::Task *task = root;
    while (task && task->r8b != 9U) {
        task = task->nextInQueue;
        if (task == root)
            return 0;
    }
    auto *const game = task ? reinterpret_cast<VFMAData *>(task->task_data_ptr) : nullptr;
    if (!game)
        return 0;
    std::uint8_t slot = 0;
    while (slot < kBattleSlots && game->characters[slot] != character)
        ++slot;
    if (slot >= kBattleSlots || !game->characters[slot])
        return 0;
    return game->actors[slot]->triggerState;
}

} // namespace shenmue::s1
