#include "s2/snapshot.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/duck_race.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/gauge.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/area_WKKT.hpp"
#include "s2/ui.hpp"
#include "s2/warning.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

namespace shenmue::s2 {

SnapControlWork *g_snapTaskData{};
std::uint32_t g_snapshotTexture2D = 0;

// @verified - p
void SetScreenDimensions(std::uint32_t width, std::uint32_t height) {
    DX_CreateTexture2D(width, height, 1, 3, &g_snapshotTexture2D, 0x10);
}


// @verified - p
void CreateTextureCopy() {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t photoWidth = 0;
    std::uint32_t photoHeight = 0;
#if defined(_WIN32)
    d3t::GetTexture2DDimensions(d3t::CreateNewTexture2D(), &width, &height);
#endif
    d3t::GetTexture2DDimensions(g_snapshotTexture2D, &photoWidth, &photoHeight);
    if (photoWidth != width || photoHeight != height) {
        ReleaseTexture2DHandle(&g_snapshotTexture2D);
        DX_CreateTexture2D(width, height, 1, 3, &g_snapshotTexture2D, 0x10);
    }
    CopyBackBufferCaptureTexture(g_snapshotTexture2D);
}


// @verified - p
void CopyTempResourceGpu() {
#if defined(_WIN32)
    d3t::Texture2D::CopyResource(g_snapshotTexture2D);
#endif
}

namespace {


SnapshotMetadataRecord *snapActiveRecord{};


std::int32_t SnapNameSortKey(std::uint32_t name) noexcept {
    return static_cast<std::int32_t>((name << 24U) | ((name & 0xff00U) << 8U) | ((name >> 8U) & 0xff00U) | (name >> 24U));
}



void snapRestorePresentation(SnapControlWork &work) {
    shenSetClockAndCompassRenderTo(work.watchSuppressed == 0);
    SetNaviTaskEnabled(work.naviEnabled != 0);
    SetSnapshotDataOption(work.snapshotOption);
    wndEnableMessageDraw();
    SetNowQteDrawEnabled(false);
    vfmaSetGaugeSuppressed(0);
    if (g_pFightStage) {
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{static_cast<std::uint32_t>(work.battleFlags)}, 1);
        g_hasColouredText = work.colouredText;
    }
    SetFightGaugeVisible(work.fightGaugeVisible);
    areas::wkktSetGaugeVisible(work.wkktGaugeVisible);
    duckSetPlayerOverlayDisabled(work.duckRaceOverlayDisabled);
}



// @verified - p
std::int32_t snapSaveTexture(const SnapshotMetadataRecord &record) {
#if defined(_WIN32)
    std::uint32_t rowPitch = 0;
    const std::byte *source = static_cast<const std::byte *>(d3t::Texture2D::MapGpu(g_snapshotTexture2D, &rowPitch));
    d3t::Texture2DDescription photo{};
    d3t::GetTexture2DDescription(g_snapshotTexture2D, &photo);
    const std::size_t packedPitch = static_cast<std::size_t>(photo.width) * 4U;
    std::vector<std::byte> pixels(packedPitch * photo.height);
    if (source) {
        for (std::uint32_t row = 0; row < photo.height; ++row)
            std::memcpy(pixels.data() + row * packedPitch, source + static_cast<std::size_t>(row) * rowPitch, packedPitch);
    }
    d3t::Texture2D::UnmapGpu(g_snapshotTexture2D);
    QueueSnapshotSave(
        static_cast<std::uint32_t>(record.id), rowPitch, photo.width, photo.height, d3t::GetD3TNativeTextureFormat(photo.format), std::move(pixels));
#else
    static_cast<void>(record);
#endif
    IncrementSteamStat(27);
    return 0;
}


std::int32_t snapSaveSnapshot(SnapControlWork &work) {
    FreeMainMap();
    const std::int32_t result = snapActiveRecord ? snapSaveTexture(*snapActiveRecord) : 3;
    char path[64];
    const char *const map = reinterpret_cast<const char *>(&g_currentMap.name);
    std::snprintf(path, sizeof(path), "scene/%02d/%c%c%c%c", g_actNumber, map[0], map[1], map[2], map[3]);
    LoadMainMapByFilepath(path);
    SetMapModelReader(GetMapsModel(), 32);
    if (result != 0) {
        work.state = SnapState::WaitContext;
        return 0;
    }
    work.state = SnapState::Restore;
    return 1;
}


std::int32_t snapIsSearchSubjectFree() {
    if (doorGetSearchRegion().name != 0)
        return 0;
    if (g_currentMap.name == SHEN_NAME(WSY0)) {
        if (srchHasActiveLockonTarget() == 1)
            return 0;
    } else if (g_currentMap.name == SHEN_NAME(AR02)) {
        if (std::memcmp(GetAberdeenSearchData(), "0014A", 6) == 0)
            return 0;
        if (std::memcmp(GetAberdeenSearchData(), "SYSP", 5) == 0)
            return 0;
    } else if (g_currentMap.name == SHEN_NAME(QEDJ) && srchHasActiveLockonTarget() == 1) {
        return 0;
    }
    return 1;
}


void snapCheckShutter(SnapControlWork &work, std::uint8_t pressed) {
    if ((pressed & 8U) == 0)
        return;
    work.snapshotOption = GetSnapshotDataOption();
    SetSnapshotDataOption(1);
    if (authGetLoaded() == 0) {
        const bool quiet = IsItemSelectable() || plymIsSearchStartRequested() ? *GetAberdeenSearchData() == 0 : snapIsSearchSubjectFree() == 0;
        if (quiet)
            aicaSetMainCommand(SNAP_SHUTTER_COMMAND);
    }
    work.state = SnapState::Allocate;
    work.fightGaugeVisible = static_cast<std::int32_t>(SetFightGaugeVisible(0));
    work.wkktGaugeVisible = areas::wkktSetGaugeVisible(0);
    work.duckRaceOverlayDisabled = duckSetPlayerOverlayDisabled(1);
    work.watchSuppressed = shenIsClockAndCompassSuppressed() ? 1U : 0U;
    work.naviEnabled = IsNaviTaskEnabled() ? 1 : 0;
    if (g_pFightStage) {
        work.battleFlags = static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x30001U}, 0));
        work.colouredText = g_hasColouredText;
        g_hasColouredText = 0;
    }
}


std::int32_t snapIsFilmCaptureAllowed() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::uint32_t alpha = static_cast<std::uint32_t>(GetScreenFilmAlpha(control->screenFilmTask));
    std::uint32_t limit = 0;
    switch (g_currentMap.name) {
    case SHEN_NAME(KWQA):
        limit = 0xa85660;
        break;
    case SHEN_NAME(ARKQ):
        limit = 0x500000;
        break;
    case SHEN_NAME(QFRR):
        limit = 0x750000;
        break;
    case SHEN_NAME(GABT):
        limit = 0x3a0000;
        break;
    case SHEN_NAME(0278):
    case SHEN_NAME(SYW0):
    case SHEN_NAME(Q300):
        limit = 0x460000;
        break;
    case SHEN_NAME(0504):
    case SHEN_NAME(0523):
        limit = 0xcc0000;
        break;
    case SHEN_NAME(KSH1):
        limit = 0x960000;
        break;
    case SHEN_NAME(WET0):
        limit = 0x150000;
        break;
    case SHEN_NAME(QRR0): {
        const char *const afs = GetCurrentAfsName();
        const std::int64_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
        if (std::memcmp(afs, "0201", 5) != 0 || static_cast<std::uint32_t>(frame - 101) > 0x62U)
            return 0;
        limit = 0xa00000;
        break;
    }
    default:
        return 0;
    }
    return alpha <= limit ? 1 : 0;
}



std::int32_t snapIsCaptureAllowed(const SnapControlWork &work) {
    if (work.mode == 1 || shenIsGamePaused() != 0 || GetPostFxFilterTransitionState() == 1 || GetShouldRenderIcons(RenderIconSlot::Any) || IsItemSelectable() ||
        IsWazamakiInactive() == 1 || IsSaveAllowed() != 0 || IsEasyOptionActive()) {
        return 0;
    }
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (IsScreenFilmNotShowing(control->screenFilmTask))
        return 1;
    return snapIsFilmCaptureAllowed();
}


void snapAllocateAlbumRecord(SnapControlWork &work) {
    SnapshotMetadataOwner *const owner = GetSnapshotMetadataOwner();
    std::int32_t index = 0;
    snapActiveRecord = nullptr;
    while (owner->records[static_cast<std::size_t>(index)].use != 0) {
        if (++index >= SNAP_ALBUM_CAPACITY) {
            work.state = SnapState::Result;
            work.warning = SNAP_ALBUM_FULL_WARNING;
            work.warningValue = SNAP_ALBUM_CAPACITY;
            qteSetupMaskFor90(2);
            return;
        }
    }
    snapActiveRecord = &owner->records[static_cast<std::size_t>(index)];
    snapActiveRecord->id = index;
    work.state = SnapState::Capture;
    work.captureCountdown = 4;
}


std::int32_t snapIsDescribedScene() {
    if (authGetLoaded() != 0)
        return 1;
    if (IsItemSelectable() || plymIsSearchStartRequested())
        return *GetAberdeenSearchData() != 0;
    return snapIsSearchSubjectFree();
}


SnapshotCharacterIdRecord *snapFindCharacterIdRecord(void *characterIdData) {
    if (!characterIdData)
        return nullptr;
    std::uint32_t afs{};
    std::memcpy(&afs, GetCurrentAfsName(), sizeof(afs));
    if (afs == SHEN_NAME(0360))
        afs = SHEN_NAME(0018);
    const std::int32_t key = SnapNameSortKey(afs);
    auto *const bytes = static_cast<std::byte *>(characterIdData);
    std::int32_t count{};
    std::memcpy(&count, bytes + 12, sizeof(count));
    auto *const records = reinterpret_cast<SnapshotCharacterIdRecord *>(bytes + 16);
    std::int32_t low = 0;
    std::int32_t high = count - 1;
    while (low <= high) {
        const std::int32_t middle = (high + low) / 2;
        SnapshotCharacterIdRecord &record = records[middle];
        const std::int32_t candidate = SnapNameSortKey(record.afs.name);
        if (key < candidate) {
            high = middle - 1;
        } else if (key > candidate) {
            low = middle + 1;
        } else {
            if (record.radius < 1.0f)
                record.radius = 20.0f;
            return &record;
        }
    }
    return nullptr;
}


void snapSavePresentation(SnapControlWork &work) {
    work.watchSuppressed = shenIsClockAndCompassSuppressed() ? 1U : 0U;
    work.naviEnabled = IsNaviTaskEnabled() ? 1 : 0;
    if (g_pFightStage) {
        work.battleFlags = static_cast<std::int32_t>(EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x30001U}, 0));
        work.colouredText = g_hasColouredText;
        g_hasColouredText = 0;
    }
}



void snapApplyCharacterIdArea(SnapshotMetadataRecord &record, SnapshotCharacterIdRecord &characterId) {
    if (record.area.name != 0)
        return;
    std::uint32_t afs{};
    std::memcpy(&afs, GetCurrentAfsName(), sizeof(afs));
    if (afs == SHEN_NAME(0120) || afs == SHEN_NAME(0112) || afs == SHEN_NAME(0185) || afs == SHEN_NAME(0146))
        characterId.area = SHEN_NAME(PLAC);
    if (characterId.area.name == SHEN_NAME(PLAC))
        record.area = g_currentMap;
    else if (characterId.area.name == SHEN_NAME(QQQQ))
        record.area = SHEN_NAME(QQQQ);
    else
        record.area = characterId.area;
}



void snapSelectCharacterIdLockOn(SnapshotMetadataRecord &record, const SnapshotCharacterIdRecord *characterId) {
    if (!characterId) {
        record.lockOnId = {};
        return;
    }
    const SnapshotSearchCandidate *candidate = CollectSnapshotSearchCandidates();
    const float radiusSquared = characterId->radius * characterId->radius;
    shen_name selected{};
    float selectedScore = 0.0f;
    bool found = false;
    for (; candidate->name.name != 0; ++candidate) {

        const std::byte *id = reinterpret_cast<const std::byte *>(characterId->characterIds.data());
        std::uint32_t name;
        for (;;) {
            std::memcpy(&name, id, sizeof(name));
            if (!name || (name == candidate->name.name && !(radiusSquared <= candidate->cameraDistanceSquared)))
                break;
            id += sizeof(name);
        }
        if (!name)
            continue;
        float score = 0.0f;
        if (characterId->sortRule == 1)
            score = std::sqrt(candidate->screenDistanceSquared) * candidate->cameraDistanceSquared;
        else if (characterId->sortRule == 2)
            score = candidate->screenDistanceSquared;
        else if (characterId->sortRule == 3)
            score = candidate->cameraDistanceSquared;
        if (!found || selectedScore > score) {
            selected = candidate->name;
            selectedScore = score;
            found = true;
        }
    }
    record.lockOnId = selected;
}


void snapApplyEventCameraRules(const SnapshotAuthoredMap &map, SnapshotMetadataRecord &record) {
    const std::uint32_t camera = GetEventCamName();
    const SnapshotAuthoredSequence *sequence = nullptr;
    std::int32_t index = 0;
    for (; index < map.count; ++index) {
        sequence = map.sequences[index];
        if (sequence->name.name == camera)
            break;
    }
    if (index == map.count)
        return;
    const std::int32_t frame = static_cast<std::int32_t>(spliGetCurrentFrmCnt());
    if (!sequence)
        return;
    const SnapshotAuthoredRule *rule = sequence->rules;
    while (true) {
        if (!rule->afs || std::strcmp(rule->afs, GetCurrentAfsName()) == 0) {
            if (rule->firstFrame <= frame && frame <= rule->lastFrame)
                break;
        }
        ++rule;
        if (rule->type == 0)
            return;
    }
    switch (rule->type) {
    case 1:
        record.lockOnId = {};
        record.area = {};
        break;
    case 2:
        record.lockOnId = rule->lockOn;
        record.area = {};
        break;
    case 3:
    case 4:
        record.lockOnId = rule->type == 4 ? rule->lockOn : shen_name{};
        record.area = rule->area;
        record.subArea = {};
        record.shop = {};
        record.reserved14 = 0;
        break;
    case 5:
        record.area = rule->area;
        record.subArea = {};
        record.shop = {};
        record.reserved14 = 0;
        break;
    default:
        break;
    }
}


void snapApplyAuthorRules(const SnapshotAuthoredSequence *sequence, SnapshotMetadataRecord &record, std::int32_t frame) {
    if (!sequence)
        return;
    const SnapshotAuthoredRule *rule = sequence->rules;
    while (true) {
        if (!rule->afs || std::strcmp(rule->afs, GetCurrentAfsName()) == 0) {
            if (rule->firstFrame <= frame && frame <= rule->lastFrame)
                break;
        }
        ++rule;
        if (rule->type == 0)
            return;
    }
    switch (rule->type) {
    case 1:
        record.lockOnId = {};
        record.area = {};
        break;
    case 2:
        record.area = {};
        record.lockOnId = rule->lockOn;
        break;
    case 3:
    case 4:
        record.lockOnId = rule->type == 4 ? rule->lockOn : shen_name{};
        record.area = rule->area;
        record.subArea = {};
        record.shop = {};
        record.reserved14 = 0;
        break;
    case 5:
        record.subArea = {};
        record.shop = {};
        record.reserved14 = 0;
        record.area = rule->area;
        break;
    default:
        break;
    }
}


void snapApplyAuthoredRules(SnapshotMetadataRecord &record) {
    const std::int32_t key = SnapNameSortKey(g_currentMap.name);
    std::int32_t low = 0;
    std::int32_t high = static_cast<std::int32_t>(SNAPSHOT_AUTHORED_MAPS.size()) - 1;
    const SnapshotAuthoredMap *map = nullptr;
    while (low <= high) {
        const std::int32_t middle = (high + low) / 2;
        const std::int32_t candidate = SnapNameSortKey(SNAPSHOT_AUTHORED_MAPS[static_cast<std::size_t>(middle)].map.name);
        if (key < candidate) {
            high = middle - 1;
        } else if (key > candidate) {
            low = middle + 1;
        } else {
            map = &SNAPSHOT_AUTHORED_MAPS[static_cast<std::size_t>(middle)];
            break;
        }
    }
    if (!map)
        return;
    const shen_name author = GetCurrentAuthorId();
    const SnapshotAuthoredSequence *sequence = nullptr;
    std::int32_t index = 0;
    for (; index < map->count; ++index) {
        sequence = map->sequences[index];
        if (sequence->name.name == author.name)
            break;
    }
    if (index == map->count)
        snapApplyEventCameraRules(*map, record);
    else
        snapApplyAuthorRules(sequence, record, static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)));
}



void snapDescribeRecord(SnapControlWork &work, SnapshotMetadataRecord &record) {
    bool described = authGetLoaded() != 0;
    if (!described) {
        described = IsItemSelectable() || plymIsSearchStartRequested() ? *GetAberdeenSearchData() != 0 : snapIsSearchSubjectFree() == 1;
    }
    if (described) {
        SnapshotCharacterIdRecord *const characterId = snapFindCharacterIdRecord(work.characterIdData);
        if (!characterId)
            return;
        record.area = {};
        if (work.descriptionSet == 0)
            snapSelectCharacterIdLockOn(record, characterId);
        snapApplyAuthoredRules(record);
        snapApplyCharacterIdArea(record, *characterId);
        return;
    }
    for (std::uint32_t index = 0; index < 3; ++index) {
        record.lockOnId = srchGetTargetName(index);
        if (record.lockOnId.name == 0)
            continue;
        for (const SnapshotSearchCandidate *candidate = CollectSnapshotSearchCandidates(); candidate->name.name != 0; ++candidate)
            if (candidate->name.name == record.lockOnId.name)
                return;
        record.lockOnId = {};
    }
    const shen_name eyeAnchor = camCtrlGetConversationEyeAnchorName();
    record.lockOnId = GetCameraSpecialTargetName();
    if (record.lockOnId.name != 0) {
        const bool flagged = camCtrlHasConversationFlag8000();
        if (eyeAnchor.name != SHEN_NAME(RYO_) || flagged)
            record.lockOnId = {};
    }
    if (g_currentMap.name == SHEN_NAME(KWW1)) {
        if (work.specialAreaFrames < 209)
            record.lockOnId = {};
        if (static_cast<std::uint32_t>(work.specialAreaFrames - 390) <= 114U)
            record.lockOnId = {};
    }
}


void SNAP_LoadCharacterIdSnapshot(SnapControlWork &work) {
    work.characterIdData = LoadLegacySceneFile("Snapshots/CharID", "SnapChrID.cft", SNAP_CHARACTER_ID_FILE);
}

} // namespace

// @verified - p
std::int32_t snapResetMetadataRecord(SnapshotMetadataRecord &record) {
    const std::int32_t id = record.id;
    record = {};
    record.id = id;
    for (unsigned index = 0; index < 4; ++index)
        record.sortKeys[index] = -1;
    return id;
}


void Queue_SNAP_Control() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SNAP_Control_Callback, 15, 11, sizeof(SnapControlWork), SNAP_TASK);
    auto *const snap = static_cast<SnapControlWork *>(HLib::GetTaskParameter(task));
    std::memset(snap, 0, sizeof(SnapControlWork));
    snap->task = task;
    snap->mode = 0;
    snap->state = SnapState::Idle;
    snap->reserved34 = 0;
    snap->padControl = AllocatePadControlEntry();
    snap->reserved88 = 0;
    snap->reserved9c = 0;
    g_snapTaskData = snap;
    const std::uint32_t map = g_currentMap.name;
    if (map == SHEN_NAME(0001) || map == SHEN_NAME(OP02) || map == SHEN_NAME(0004) || map == SHEN_NAME(OMAK) || map == SHEN_NAME(STVW)) {
        snap->mode = 1;
    } else {
        SNAP_LoadCharacterIdSnapshot(*snap);
        wndEnableMessageDraw();
    }
}


void SNAP_Control_Callback(void *parameter) {
    SnapControlWork &work = *static_cast<SnapControlWork *>(parameter);
    if (work.descriptionSetDelay > 0) {
        if (--work.descriptionSetDelay == 0 && g_snapTaskData)
            g_snapTaskData->descriptionSet = 1;
    } else if (work.descriptionClearDelay > 0) {
        if (--work.descriptionClearDelay == 0 && g_snapTaskData)
            g_snapTaskData->descriptionSet = 0;
    }
    if (snapIsCaptureAllowed(work) == 0) {
        SetQteControlState(0);
        return;
    }
    if (GetSnapshotMetadataCount() == static_cast<std::uint32_t>(SNAP_ALBUM_CAPACITY) && work.state == SnapState::Idle) {
        if (work.delay >= 0.0f) {
            work.delay -= shenGetScaledDeltaTickTime(1.0f);
            return;
        }
        SetQteControlState(1);
        qteSetupMaskFor90(2);
        return;
    }
    if (g_currentMap.name == SHEN_NAME(KWW1) && authGetLoaded() == 0) {
        const bool counting = IsItemSelectable() || plymIsSearchStartRequested() ? *GetAberdeenSearchData() == 0 : snapIsSearchSubjectFree() == 0;
        if (counting)
            ++work.specialAreaFrames;
    }
    SetQteControlState(1);
    switch (work.state) {
    case SnapState::Idle:
        if (IsSnapshotSaveInProgress() == 0)
            snapCheckShutter(work, static_cast<std::uint8_t>(g_inputBuffer[0].pressedPriority));
        break;
    case SnapState::Allocate:
        snapAllocateAlbumRecord(work);
        break;
    case SnapState::Capture:
        qteSetupMaskFor90(3);
        shenSetClockAndCompassRenderTo(1);
        SetNaviTaskEnabled(false);
        SetSnapshotDataOption(1);
        wndDisableMessageDraw();
        SetNowQteDrawEnabled(true);
        vfmaSetGaugeSuppressed(1);
        if (g_pFightStage) {
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x30001U}, 0);
            g_hasColouredText = 0;
        }
        SetFightGaugeVisible(0);
        areas::wkktSetGaugeVisible(0);
        duckSetPlayerOverlayDisabled(1);
        if (work.captureCountdown-- == 1) {
            CopyTempResourceGpu();
            SnapshotMetadataRecord &record = *snapActiveRecord;
            work.state = SnapState::Save;
            const std::int32_t id = record.id;
            record = {};
            record.id = id;
            record.use = 1;
            record.area = g_currentMap;
            const std::span<const shen_name> mapNames = GetScenarioEventObjectMapNames();
            record.subArea = mapNames.empty() ? shen_name{} : mapNames[0];
            record.shop = drGetCurrentAuthorId();
            LocalTimeSnapshot taken{};
            shenGetLocalTimeSnapshot(&taken);
            record.year = taken.year;
            record.month = taken.month;
            record.day = taken.day;
            record.hour = taken.hour;
            record.minute = taken.minute;
            record.second = taken.second;
            record.week = taken.dayOfWeek;
            snapDescribeRecord(work, record);
        } else if (work.captureCountdown == 1) {
            CreateTextureCopy();
        }
        break;
    case SnapState::Save: {
        PauseAuthorMultichannelAudio(1);
        PauseConversation(nullptr, true);
        AssignPreviousFrameTimeDelta(1);
        const std::int32_t saved = snapSaveSnapshot(work);
        AssignPreviousFrameTimeDelta(0);
        PauseConversation(nullptr, false);
        PauseAuthorMultichannelAudio(0);
        SetQteControlState(1);
        qteSetupMaskFor90(saved == 1 ? 1U : 2U);
        break;
    }
    case SnapState::Result:
        SetSnapshotDataOption(work.snapshotOption);
        if (snapIsDescribedScene() != 0) {
            work.state = SnapState::WaitContext;
        } else {
            WRN_SetWarning(work.warning, work.warningValue);
            work.state = SnapState::Hold;
            work.previousPadControl = BindPadControlEntry(work.padControl);
            GetCurrentDateTime(work.dateTime);
            npcResetDateTimeIfNeeded();
            shenSetTimeSimulationTo(0);
        }
        snapRestorePresentation(work);
        work.presentationRestored = 1;
        work.delay = 90.0f;
        break;
    case SnapState::Hold:
        qteSetupMaskFor90(2);
        if ((g_inputBuffer[0].pressedPriority & 0xc00U) != 0) {
            work.state = SnapState::Restore;
            WRN_SetWarning(0, 0);
            BindPadControlEntry(work.previousPadControl);
            FinishLoading();
            npcAdvanceTime(work.dateTime.hour, work.dateTime.minute);
            shenSetTimeSimulationTo(1);
        }
        break;
    case SnapState::WaitContext:
        qteSetupMaskFor90(2);
        if (snapIsDescribedScene() == 0) {
            work.state = SnapState::Result;
            snapSavePresentation(work);
        }
        break;
    case SnapState::Restore:
        if (work.presentationRestored == 0)
            snapRestorePresentation(work);
        work.state = SnapState::Idle;
        work.presentationRestored = 0;
        work.delay = 90.0f;
        break;
    case SnapState::Retake:
        work.state = SnapState::Capture;
        work.captureCountdown = 4;
        break;
    default:
        break;
    }
}


void snapStopControl() noexcept {
    if (!g_snapTaskData)
        return;
    g_snapTaskData->descriptionSet = 0;
    g_snapTaskData->descriptionSetDelay = 0;
    g_snapTaskData->descriptionClearDelay = 0;
}


void snapSetDesc(std::int32_t set) noexcept {
    if (g_snapTaskData)
        g_snapTaskData->descriptionSet = set;
}


bool snapCheckState() noexcept {
    return g_snapTaskData && g_snapTaskData->state == SnapState::Hold;
}


void snapAssignFrmCntFlags(std::int32_t setDelay, std::int32_t clearDelay) noexcept {
    if (!g_snapTaskData)
        return;
    g_snapTaskData->descriptionSetDelay = setDelay;
    g_snapTaskData->descriptionClearDelay = clearDelay;
}

std::int32_t GetSnapshotControlPhotoCount() noexcept {
    if (!g_snapTaskData)
        return -1;
    return static_cast<std::int32_t>(GetSnapshotMetadataCount());
}

} // namespace shenmue::s2
