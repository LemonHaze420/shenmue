#include "s1_multiplayer_fight.hpp"
#if SHENMUE_ENHANCEMENT_STEAM_S1_MP && SHENMUE_IS_S1 && SHENMUE_IS_PC
#include "s1/task/vf_internal.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/char_hand.hpp"
#include "s1/task/char_osage.hpp"
#include "s1/task/char_cloth.hpp"
#include "s1/motion_registry.hpp"
#include "s1/face.hpp"

namespace shenmue::s1 {
namespace {
Character *g_fighters[kMultiplayerFighterCount]{};
MapControlData *g_owner = nullptr;
Vector3 g_returnPosition{};
Vector3i g_returnRotation{};
CharacterNode *g_returnParent = nullptr;
std::int32_t g_motionHandle = 0;
bool g_fightCreated = false;
bool g_retiring = false;
void *g_pendingStyles[64]{};
std::uint32_t g_fightRandomSeed = kS1MultiplayerMagic;
unsigned g_nextEnemyAsset = 0;
S1MultiplayerCombatProfile g_savedProfile{};
bool g_hasSavedProfile = false;

void WriteCombatProfile(const S1MultiplayerCombatProfile &profile) {
    g_playerStats.currentVfSkill = profile.skills;
    g_playerStats.moveSets = profile.moves;
    g_playerStats.moveUsageCounts = profile.usage;
    g_playerStats.defaultHealth = profile.health;
    g_playerStats.cameraShakeAmount = profile.cameraShake;
    g_playerStats.fightAssistScale = profile.assist;
    g_playerStats.currentMove = profile.currentMove;
    g_playerStats.vfHpRegenerationProfile = profile.regeneration;
    g_activeHpRegenProfile = profile.activeRegeneration;
    g_defaultHpRegenProfile = profile.defaultRegeneration;
}

void RetireFighters() {
    for (Character *&character : g_fighters) {
        if (!character)
            continue;
        NPCLogger(character, 0, 0);
        character->flags.value = (character->flags.value & ~3U) | 2U;
        ++g_owner->pendingCharacterRetirements;
        character = nullptr;
    }
    for (void *&style : g_pendingStyles) {
        if (style)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(style), 0);
        style = nullptr;
    }
    g_retiring = true;
}

bool LoadFighterAsset(const S1MultiplayerFighterAsset &asset) {
    if (!g_pendingStyles[asset.style])
        g_pendingStyles[asset.style] = ReadFile("scene/03/MFBT", asset.styleFile, static_cast<std::int32_t>(kMultiplayerStyleToken.name));
    if (!g_pendingStyles[asset.style])
        return false;
    void *data = ReadFile("scene/03/model/chara", asset.model, static_cast<std::int32_t>(kMultiplayerModelToken.name));
    char path[128];
    const int length = std::snprintf(path, sizeof(path), "/scene/03/model/chara/%s", asset.model);
    std::memcpy(path + length - 3, "fbx", 4);
    PackModel *model = MT5_InitRawModelAndTextures(data, asset.model, path);
    if (!model) {
        if (data)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(data), 0);
        return false;
    }
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), path, 0, 0x22);
    const std::int16_t kind = static_cast<std::int16_t>(static_cast<Mt5RuntimeNode *>(model->model)->nodeKind);
    model->field_14 = kind > 0x7000 ? kind - 0x7001 : 0;
    char image[5]{};
    std::memcpy(image, &asset.image.name, 4);
    if (MDLM_RegisterImage(image, asset.model, model, 0, nullptr, true))
        return true;
    FreePVR(model);
    return false;
}

bool PlaceFighter(Character *character, Vector3 position, std::int32_t yaw) {
    character->entity.node.rotation = {0, yaw, 0};
    shenApplyTranslationToAll(character->entity.node, position);
    if (!SelectCharacterMapQuery(character))
        return false;
    position.y = FindHighestValidCollisionBelow(character->entity.node.position, 0.1f, 0.1f);
    shenApplyTranslationToAll(character->entity.node, position);
    character->rootMotionDelta = {};
    return true;
}

void RecycleEnemies(VFMAData *game) {
    if (game->state != vf_detail::VFS_InBattle)
        return;
    VFFightStage *stage = g_pFightStage;
    for (unsigned index = 0; index < kMultiplayerEnemyLimit; ++index) {
        Character *character = g_fighters[index + 1];
        VFEnemySlot *slot = nullptr;
        for (unsigned entry = 0; entry < stage->maxEnemies; ++entry)
            if (stage->enemySlots[entry].character == character)
                slot = &stage->enemySlots[entry];
        if (!slot || (slot->state != vf_detail::SS_Cooldown && slot->state != vf_detail::SS_Inactive))
            continue;
        if (slot->state == vf_detail::SS_Cooldown) {
            if (slot->tickFrames < kMultiplayerEnemyRecycleFrames)
                continue;
            vf_detail::vfTransitionEnemySlotToInactive(game, slot);
            vf_detail::vfApplyEnemySlotAxisOffsetDecay(static_cast<std::int8_t>(slot->registryIndex));
        }
        VFFormationGroup *next = nullptr;
        for (unsigned entry = 0; entry < stage->formationCapacity; ++entry) {
            VFFormationGroup *wave = &stage->formations[entry];
            if (wave->character == character && wave->waveId == 0) {
                next = wave;
                break;
            }
        }
        if (!next)
            continue;
        const S1MultiplayerFighterAsset *asset = nullptr;
        for (const S1MultiplayerFighterAsset &candidate : kMultiplayerEnemyAssets)
            if (candidate.style == next->fightStyle)
                asset = &candidate;
        if (!asset)
            continue;
        VFFormationGroup *finished = slot->wave;
        char image[5]{};
        std::memcpy(image, &asset->image.name, 4);
        IMGM_SetupForScript(character, image, 0);
        const S1MultiplayerFighterSpawn &spawn = kMultiplayerFighters[index + 1];
        if (!PlaceFighter(character, spawn.position, spawn.yaw))
            continue;
        character->renderOpacity = 1.0f;
        NPCLogger(character, 3, 1);
        if (!vf_detail::vfRegisterFormationSlotForBattle(character))
            continue;
        const S1MultiplayerFighterAsset &future = kMultiplayerEnemyAssets[g_nextEnemyAsset];
        g_nextEnemyAsset = (g_nextEnemyAsset + 1) % std::size(kMultiplayerEnemyAssets);
        finished->waveId = 0;
        finished->hpCurrent = future.hp;
        finished->hpTotal = future.hp;
        finished->fightStyle = future.style;
        ++stage->teamRespawnTimer[1];
    }
}
} // namespace

S1MultiplayerCombatProfile S1Multiplayer_GetCombatProfile() {
    return {.skills = g_playerStats.currentVfSkill,
        .moves = g_playerStats.moveSets,
        .usage = g_playerStats.moveUsageCounts,
        .health = g_playerStats.defaultHealth,
        .cameraShake = g_playerStats.cameraShakeAmount,
        .assist = g_playerStats.fightAssistScale,
        .currentMove = g_playerStats.currentMove,
        .regeneration = g_playerStats.vfHpRegenerationProfile,
        .activeRegeneration = g_activeHpRegenProfile,
        .defaultRegeneration = g_defaultHpRegenProfile};
}

bool S1Multiplayer_ApplyCombatProfile(const S1MultiplayerCombatProfile &profile) {
    if (g_hasSavedProfile || profile.health <= 0 || profile.health > 32767 || profile.cameraShake > 1000 || profile.assist > 1000)
        return false;
    for (const MoveSetRuntimeState &moves : profile.moves)
        if (moves.count > 20)
            return false;
    g_savedProfile = S1Multiplayer_GetCombatProfile();
    g_hasSavedProfile = true;
    WriteCombatProfile(profile);

    return true;
}

void S1Multiplayer_RestoreCombatProfile() {
    if (!g_hasSavedProfile || g_bIsFighting || S1Multiplayer_OwnsFight())
        return;
    WriteCombatProfile(g_savedProfile);
    g_hasSavedProfile = false;
}

S1MultiplayerFightRandomScope::S1MultiplayerFightRandomScope(bool active) : savedSeed_(g_EnvPrngSeed), active_(active) {
    if (active_)
        g_EnvPrngSeed = g_fightRandomSeed;
}

S1MultiplayerFightRandomScope::~S1MultiplayerFightRandomScope() {
    if (!active_)
        return;
    g_fightRandomSeed = g_EnvPrngSeed;
    g_EnvPrngSeed = savedSeed_;
}

bool S1Multiplayer_IsFightCharacter(const Character *character) {
    if (!g_owner || !g_fightCreated || !g_bIsFighting || !character || !g_mapControlTask ||
        g_mapControlTask->task_data_ptr != reinterpret_cast<HLib::TaskData *>(g_owner))
        return false;
    if (g_owner->ryo == character)
        return true;
    for (const Character *fighter : g_fighters)
        if (fighter == character)
            return true;
    return false;
}

bool S1Multiplayer_CanStartFight() {
    if (g_areaId != SHEN_NAME(D000) || g_bIsFighting || g_pFightStage || !g_mapControlTask || !g_mapControlTask->task_data_ptr || g_owner ||
        !g_pControlPadController || g_pControlPadController->blockedCanControl != 1 || SCEN_HasActiveScript())
        return false;
    MapControlData *control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    Character *ryo = static_cast<Character *>(control->ryo);
    return ryo && ryo->model && MOTM_GetWork(ryo) && control->scenarioTask && g_mdlmTask;
}

bool S1Multiplayer_CreateFight() {
    if (!S1Multiplayer_CanStartFight())
        return false;
    g_owner = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    Character *ryo = static_cast<Character *>(g_owner->ryo);
    g_returnPosition = GetPositionForEntity(ryo->entity.node);
    g_returnRotation = ryo->entity.node.rotation;
    g_returnParent = ryo->entity.node.child;
    g_motionHandle = MOTN_AllocateMOTN(3, SHEN_NAME(MFBT), "M_FGT1.BIN");
    g_pendingStyles[0] = ReadFile("scene/01/D000", "EN_RYOU.BIN", static_cast<std::int32_t>(kMultiplayerStyleToken.name));
    if (!g_motionHandle || !g_pendingStyles[0] || !LoadFighterAsset(kMultiplayerGuizhangAsset)) {
        RetireFighters();
        return false;
    }
    for (const S1MultiplayerFighterAsset &asset : kMultiplayerEnemyAssets) {
        if (!LoadFighterAsset(asset)) {
            RetireFighters();
            return false;
        }
    }
    for (unsigned index = 0; index < kMultiplayerFighterCount; ++index) {
        const S1MultiplayerFighterSpawn &spawn = kMultiplayerFighters[index];
        const S1MultiplayerFighterAsset &asset = index ? kMultiplayerEnemyAssets[index - 1] : kMultiplayerGuizhangAsset;
        Character *character = AddCharacterHandler(CharacterHandlerPriority::Low);
        if (!character) {
            RetireFighters();
            return false;
        }
        g_fighters[index] = character;
        CHID_Init(g_owner->scenarioTask, spawn.id.name, character);
        char image[5]{};
        std::memcpy(image, &asset.image.name, 4);
        IMGM_Alloc(character);
        IMGM_Create(character, image);
        if (!character->model) {
            RetireFighters();
            return false;
        }
        character->entity.node.child = nullptr;
        if (!PlaceFighter(character, spawn.position, spawn.yaw)) {
            RetireFighters();
            return false;
        }
        character->flags.value |= CharacterFlagActive | CharacterFlagHasCpct;
        NPCLogger(character, 3, 1);
        character->renderOpacity = 1.0f;
        character->shadowModel = ryo->shadowModel;
        MOTN_Alloc(character, false);
        Clipper_Alloc(character);
        FIGP_Allocate(character, nullptr);
        CLMD_Alloc(character, 0);
        OSAGE_Alloc(character);
        MHND_Initialize(character);
        AllocREFB(character, 2);
        CPAM_Alloc(character);
    }
    MotionInformationWork &ryoMotion = MOTM_GetWork(ryo)->motion;
    void *const motionCallback = ryoMotion.motionCallback;
    void *const motionCallbackUser = ryoMotion.motionCallbackUser;
    MOTI_Free(ryoMotion);
    ryoMotion.motionCallback = motionCallback;
    ryoMotion.motionCallbackUser = motionCallbackUser;
    ryo->entity.node.child = nullptr;
    if (!PlaceFighter(ryo, kMultiplayerRyoPosition, kMultiplayerRyoYaw)) {
        ryo->entity.node.child = g_returnParent;
        ryo->entity.node.rotation = g_returnRotation;
        shenApplyTranslationToAll(ryo->entity.node, g_returnPosition);
        RetireFighters();
        return false;
    }
    shenStartFGST(2 + 2 * kMultiplayerEnemyLimit, kMultiplayerFormationCount, 0, 0);
    g_pFightStage->ringBoundary = {4.8f, 41.61f, 22.6551f};
    for (unsigned style = 0; style < std::size(g_pendingStyles); ++style) {
        g_pFightStage->styles[style] = g_pendingStyles[style];
        g_pendingStyles[style] = nullptr;
    }
    vfStageSetupUtil(reinterpret_cast<std::uintptr_t>(ryo), nullptr, 0, 0, 200, 0, 0, 0, 0, 0, 0);
    for (unsigned index = 0; index < kMultiplayerFighterCount; ++index) {
        const S1MultiplayerFighterAsset &asset = index ? kMultiplayerEnemyAssets[index - 1] : kMultiplayerGuizhangAsset;
        vfStageSetupUtil(reinterpret_cast<std::uintptr_t>(g_fighters[index]), nullptr, index ? 1 : 2, index ? 1 : 0, asset.hp, asset.style, 0, 0, 0, 0, 0);
    }
    vfConfigureFormationGroup(g_fighters[0], 0, kMultiplayerGuizhangHealthFloorFlag, kMultiplayerGuizhangHealthFloor);
    for (unsigned index = 0; index < kMultiplayerEnemyLimit; ++index) {
        const S1MultiplayerFighterAsset &asset = kMultiplayerEnemyAssets[kMultiplayerEnemyLimit + index];
        vfStageSetupUtil(reinterpret_cast<std::uintptr_t>(g_fighters[index + 1]), nullptr, 1, 1, asset.hp, asset.style, 0, 0, 0, 0, 0);
    }
    g_nextEnemyAsset = 0;
    g_fightRandomSeed = kS1MultiplayerMagic;
    {
        S1MultiplayerFightRandomScope randomScope(true);
        EnqueueVFGI(2 + kMultiplayerEnemyLimit, 2 + kMultiplayerEnemyLimit, 1.0f, 1, -1);
    }
    SetNewAuthorGameState(0);
    g_fightCreated = true;
    VFMAData *game = vfMainGetCurrent();
    if (game) {
        PCCTL_DisablePlayerControl();
        game->inputLockOnStart = 1;
    }
    VFActor *guizhang = vfFindFighterFor(g_fighters[0]);
    if (!game || game->playerCharacter != ryo || !guizhang || game->slotTeamId[guizhang->slotIndex] != 0 || game->slotHpPoolMode[guizhang->slotIndex] != 2) {
        S1Multiplayer_EndFight();
        return false;
    }

    return true;
}

bool S1Multiplayer_OwnsFight() {
    return g_fightCreated;
}

void S1Multiplayer_EndFight() {
    if (g_fightCreated && g_bIsFighting) {
        if (VFMAData *game = vfMainGetCurrent())
            game->state = vf_detail::VFS_Teardown;
    }
}

void S1Multiplayer_UpdateFight() {
    if (!g_owner)
        return;
    MapControlData *control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control != g_owner) {
        g_owner = nullptr;
        g_fightCreated = g_retiring = false;
        g_motionHandle = 0;
        for (Character *&character : g_fighters)
            character = nullptr;
        return;
    }
    if (g_fightCreated && g_bIsFighting) {
        VFMAData *game = vfMainGetCurrent();
        S1MultiplayerFightRandomScope randomScope(true);
        VFActor *ryo = vfFindFighterFor(static_cast<Character *>(g_owner->ryo));
        VFActor *guizhang = vfFindFighterFor(g_fighters[0]);
        if (!ryo || !guizhang || ryo->currentHp <= 0 || guizhang->currentHp <= 0) {
            S1Multiplayer_EndFight();
            return;
        }
        RecycleEnemies(game);
        return;
    }
    if (!g_retiring) {
        Character *ryo = static_cast<Character *>(g_owner->ryo);
        ryo->entity.node.child = g_returnParent;
        ryo->entity.node.rotation = g_returnRotation;
        shenApplyTranslationToAll(ryo->entity.node, g_returnPosition);
        PCCTL_ResetAuthAndInputStates();
        RetireFighters();
        g_fightCreated = false;
        return;
    }
    if (g_owner->pendingCharacterRetirements)
        return;
    if (g_motionHandle)
        ReallocateUNFO(g_motionHandle);
    MDLM_UnloadImage(kMultiplayerGuizhangAsset.image);
    for (const S1MultiplayerFighterAsset &asset : kMultiplayerEnemyAssets)
        MDLM_UnloadImage(asset.image);
    g_owner = nullptr;
    g_motionHandle = 0;
    g_retiring = false;
}

std::uint32_t S1Multiplayer_FightChecksum() {
    if (!g_fightCreated || !g_bIsFighting)
        return 0;
    const VFMAData *game = vfMainGetCurrent();
    std::uint32_t checksum = 2166136261U;
    const auto add = [&checksum](std::uint32_t value) { checksum = (checksum ^ value) * 16777619U; };
    add(game->frameCount);
    add(game->state);
    add(g_fightRandomSeed);
    for (unsigned slot = 0; slot < kBattleSlots; ++slot) {
        add(game->characters[slot] != nullptr);
        if (!game->characters[slot])
            continue;
        const VFActor *actor = game->actors[slot];
        add(std::bit_cast<std::uint32_t>(actor->position.x));
        add(std::bit_cast<std::uint32_t>(actor->position.y));
        add(std::bit_cast<std::uint32_t>(actor->position.z));
        add(static_cast<std::uint16_t>(actor->currentHp));
        add(actor->facingYaw);
        add(actor->fightStyleIndex);
    }
    return checksum;
}
} // namespace shenmue::s1
#endif
