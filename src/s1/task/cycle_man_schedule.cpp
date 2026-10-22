#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1::CycleManager {

bool ResolveChunkRelativeRange(uint32_t chunkOffset, uint32_t relativeOffset, uint32_t count, uint32_t stride, uint32_t &absoluteOffset) {
    if (!CycleManager::IsReady() || !relativeOffset || !stride)
        return false;
    const uint64_t begin = static_cast<uint64_t>(chunkOffset) + relativeOffset;
    const uint64_t bytes = static_cast<uint64_t>(count) * stride;
    if (begin > g_cycleManagerRuntime.bytes.size() || bytes > g_cycleManagerRuntime.bytes.size() - static_cast<size_t>(begin)) {
        return false;
    }
    absoluteOffset = static_cast<uint32_t>(begin);
    return true;
}

} // namespace shenmue::s1::CycleManager

namespace shenmue::s1 {
namespace CycleManager {


uint8_t *MoveObj_FindEntry(CycleScheduleEntry *entries, uint32_t requestCycle, uint64_t unused, int32_t size) {
    (void)unused;
    while (entries && size >= 0) {
        const int32_t middle = size / 2;
        CycleScheduleEntry *const entry = entries + middle;
        if (static_cast<uint32_t>(entry->time) == requestCycle) {
            if (entry->offset <= 0 || static_cast<uint32_t>(entry->offset) >= g_cycleManagerRuntime.bytes.size()) {
                return nullptr;
            }
            return g_cycleManagerRuntime.bytes.data() + entry->offset;
        }
        if (static_cast<uint32_t>(entry->time) > requestCycle) {
            size = middle - 1;
        } else {
            entries += middle + 1;
            size -= middle + 1;
        }
    }
    return nullptr;
}

bool SelectCurrentSection(uint32_t token) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    if (!cm.loaded || cm.schedule.empty() || !token)
        return false;
    uint8_t *const section = CycleManager::MoveObj_FindEntry(cm.schedule.data(), token, 0, static_cast<int32_t>(cm.schedule.size()) - 1);
    if (!section || cm.bytes.empty())
        return false;
    const uintptr_t begin = reinterpret_cast<uintptr_t>(cm.bytes.data());
    const uintptr_t address = reinterpret_cast<uintptr_t>(section);
    if (address < begin || address - begin >= cm.bytes.size())
        return false;
    cm.currentSectionOffset = static_cast<uint32_t>(address - begin);
    cm.currentSectionToken = token;
    cm.currentSectionSize = 0;
    for (const CycleManager::Section &candidate : cm.sections) {
        if (candidate.offset == cm.currentSectionOffset) {
            cm.currentSectionToken = candidate.token;
            cm.currentSectionSize = candidate.size;
            break;
        }
    }
    return true;
}

uint32_t ResolveOffsetFromChara(uint32_t charaHeaderOffset, int32_t offset) {
    if (offset <= 0)
        return 0;
    const uint32_t rawOffset = static_cast<uint32_t>(offset);
    if (rawOffset < charaHeaderOffset) {
        const uint64_t resolved = static_cast<uint64_t>(charaHeaderOffset) + rawOffset;
        return resolved <= std::numeric_limits<uint32_t>::max() ? static_cast<uint32_t>(resolved) : 0;
    }
    return rawOffset;
}

void LoadMNRBAndMCIR() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    cm.mnrbEntries.clear();
    cm.mnrbReservationPoolAvailable = false;
    for (CycleManager::MNRBEntry::Reservation &reservation : cm.mnrbReservationPool) {
        reservation.prev = nullptr;
        reservation.next = nullptr;
        reservation.owner = nullptr;
        reservation.seatOrderKey = 0;
    }
    cm.mnrbOffset = 0;
    cm.mcirOffset = 0;
    if (!cm.loaded || cm.schedule.empty())
        return;

    if (uint8_t *mcir = CycleManager::MoveObj_FindEntry(cm.schedule.data(), SHEN_NAME(MCIR), 0, static_cast<int32_t>(cm.schedule.size()) - 1)) {
        const uint32_t offset = static_cast<uint32_t>(mcir - cm.bytes.data());
        uint32_t magic = 0;
        uint32_t version = 0;
        uint32_t charGroupCount = 0;
        uint32_t chairGroupCount = 0;
        uint32_t chairCount = 0;
        uint32_t posInfoCount = 0;
        uint32_t rootPoseCount = 0;
        if (ReadValue(cm.bytes, offset + 0x00, magic) && ReadValue(cm.bytes, offset + 0x04, version) && ReadValue(cm.bytes, offset + 0x08, charGroupCount) &&
            ReadValue(cm.bytes, offset + 0x0C, chairGroupCount) && ReadValue(cm.bytes, offset + 0x10, chairCount) &&
            ReadValue(cm.bytes, offset + 0x14, posInfoCount) && ReadValue(cm.bytes, offset + 0x18, rootPoseCount) && magic == SHEN_NAME(ISU_) &&
            version == Token("0.25")) {
            const uint64_t tableBytes = 0x1Cu + static_cast<uint64_t>(charGroupCount) * 8u + static_cast<uint64_t>(chairGroupCount) * 32u +
                static_cast<uint64_t>(chairCount) * 32u + static_cast<uint64_t>(posInfoCount) * 36u + static_cast<uint64_t>(rootPoseCount) * 12u;
            if (offset <= cm.bytes.size() && tableBytes <= cm.bytes.size() - offset)
                cm.mcirOffset = offset;
        }
    }

    uint8_t *mnrb = CycleManager::MoveObj_FindEntry(cm.schedule.data(), SHEN_NAME(MNRB), 0, static_cast<int32_t>(cm.schedule.size()) - 1);
    if (!mnrb)
        return;

    const uint32_t chunkOffset = static_cast<uint32_t>(mnrb - cm.bytes.data());
    uint32_t entryCount = 0;
    uint32_t indexRelativeOffset = 0;
    if (!ReadValue(cm.bytes, chunkOffset + 0x08, entryCount) || !ReadValue(cm.bytes, chunkOffset + 0x0C, indexRelativeOffset) || entryCount > 4096) {
        return;
    }

    uint32_t indexOffset = 0;
    if (!CycleManager::ResolveChunkRelativeRange(chunkOffset, indexRelativeOffset, entryCount, 8, indexOffset)) {
        return;
    }

    cm.mnrbOffset = chunkOffset;
    cm.mnrbReservationPoolAvailable = true;
    cm.mnrbEntries.reserve(entryCount);
    for (uint32_t i = 0; i < entryCount; ++i) {
        const uint32_t rowOffset = indexOffset + i * 8u;
        CycleManager::MNRBEntry entry{};
        uint32_t lineRelativeOffset = 0;
        entry.sourceIndexOffset = rowOffset;
        if (!ReadValue(cm.bytes, rowOffset + 0, entry.name.name) || !ReadValue(cm.bytes, rowOffset + 4, lineRelativeOffset) || !entry.name.name) {
            continue;
        }

        uint32_t lineOffset = 0;
        if (!CycleManager::ResolveChunkRelativeRange(chunkOffset, lineRelativeOffset, 1, 40, lineOffset)) {
            continue;
        }
        entry.lineDataOffset = lineOffset;
        ReadValue(cm.bytes, lineOffset + 0x00, entry.entryKind);
        ReadValue(cm.bytes, lineOffset + 0x04, entry.radius);
        ReadValue(cm.bytes, lineOffset + 0x08, entry.ownerTokenCount);
        ReadValue(cm.bytes, lineOffset + 0x0C, entry.ownerTokenOffset);
        ReadValue(cm.bytes, lineOffset + 0x10, entry.areaPointCount);
        ReadValue(cm.bytes, lineOffset + 0x14, entry.areaPointOffset);
        ReadValue(cm.bytes, lineOffset + 0x18, entry.ownerPointCount);
        ReadValue(cm.bytes, lineOffset + 0x1C, entry.ownerPointOffset);
        ReadValue(cm.bytes, lineOffset + 0x20, entry.activeTimeStart);
        ReadValue(cm.bytes, lineOffset + 0x24, entry.activeTimeEnd);

        uint32_t resolved = 0;
        if (entry.ownerTokenCount && CycleManager::ResolveChunkRelativeRange(chunkOffset, entry.ownerTokenOffset, entry.ownerTokenCount, 4, resolved)) {
            entry.ownerTokenOffset = resolved;
        } else {
            entry.ownerTokenOffset = 0;
            entry.ownerTokenCount = 0;
        }
        if (entry.areaPointCount && CycleManager::ResolveChunkRelativeRange(chunkOffset, entry.areaPointOffset, entry.areaPointCount, 12, resolved)) {
            entry.areaPointOffset = resolved;
        } else {
            entry.areaPointOffset = 0;
            entry.areaPointCount = 0;
        }
        if (entry.ownerPointCount && CycleManager::ResolveChunkRelativeRange(chunkOffset, entry.ownerPointOffset, entry.ownerPointCount, 12, resolved)) {
            entry.ownerPointOffset = resolved;
        } else {
            entry.ownerPointOffset = 0;
            entry.ownerPointCount = 0;
        }
        cm.mnrbEntries.push_back(entry);
    }

}


void CopyCycleValues() {
    for (CycleManager::ShutRow &row : g_cycleManagerRuntime.shutRows) {
        row.pos.y = row.minY;
        row.byte31 = 0;
    }
}

void LoadSHUT() {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    cm.shutRows.clear();
    cm.shutAreaID.name = 0;
    if (!cm.loaded || cm.schedule.empty())
        return;

    uint8_t *shutPtr = CycleManager::MoveObj_FindEntry(cm.schedule.data(), SHEN_NAME(SHUT), 0, static_cast<int32_t>(cm.schedule.size()) - 1);
    if (!shutPtr)
        return;

    const uint32_t shutOffset = static_cast<uint32_t>(shutPtr - cm.bytes.data());
    uint32_t signature = 0;
    uint32_t area = 0;
    uint32_t count = 0;
    if (!ReadValue(cm.bytes, shutOffset, signature) || !ReadValue(cm.bytes, shutOffset + 4, area) || !ReadValue(cm.bytes, shutOffset + 8, count) ||
        signature != SHEN_NAME(SHF2) || count > 128) {
        return;
    }

    const uint32_t rowSize = 72;
    const uint32_t rowsOffset = shutOffset + 12;
    const uint64_t requiredBytes = static_cast<uint64_t>(rowsOffset) + static_cast<uint64_t>(count) * rowSize;
    if (requiredBytes > cm.bytes.size()) {
        return;
    }

    cm.shutAreaID.name = area;

    cm.shutRows.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
        const uint32_t rowOffset = rowsOffset + i * rowSize;
        CycleManager::ShutRow row{};
        row.sourceOffset = rowOffset;
        ReadValue(cm.bytes, rowOffset + 0, row.token.name);
        ReadValue(cm.bytes, rowOffset + 4, row.modelToken.name);
        ReadValue(cm.bytes, rowOffset + 8, row.pos.x);
        ReadValue(cm.bytes, rowOffset + 12, row.pos.y);
        ReadValue(cm.bytes, rowOffset + 16, row.pos.z);
        ReadValue(cm.bytes, rowOffset + 20, row.targetPos.x);
        ReadValue(cm.bytes, rowOffset + 24, row.targetPos.y);
        ReadValue(cm.bytes, rowOffset + 28, row.targetPos.z);
        ReadValue(cm.bytes, rowOffset + 32, row.angle1Degrees);
        ReadValue(cm.bytes, rowOffset + 36, row.angle2Degrees);
        ReadValue(cm.bytes, rowOffset + 40, row.float20);


        ReadValue(cm.bytes, rowOffset + 52, row.dword34);
        ReadValue(cm.bytes, rowOffset + 56, row.qword38);
        ReadValue(cm.bytes, rowOffset + 64, row.dword40);
        ReadValue(cm.bytes, rowOffset + 68, row.angle3Degrees);
        row.angle1 = CycleManager::AngleDegreesToFixed16(row.angle1Degrees);
        row.angle2 = CycleManager::AngleDegreesToFixed16(row.angle2Degrees);
        row.angle3 = CycleManager::AngleDegreesToFixed16(row.angle3Degrees);
        row.dword20AsFloat = row.pos.y;
        row.yStep = 0.0f;
        row.minY = row.pos.y;
        row.maxY = row.pos.y + row.float20;
        row.cycleIndex = i + 1;
        row.byte31 = 0;
        row.objectInstanceIndex = -1;

        if (!row.token.name || !CycleManager::IsReasonablePosition(row.pos))
            continue;
        cm.shutRows.push_back(row);
    }

}

uint32_t CommandRecordSize(uint32_t opcode, bool &stop, uint32_t recordOffset) {
    stop = false;
    if (opcode >= 0x3E) {
        stop = true;
        return 0;
    }
    if (opcode == CycleOpcode_RemoveCharacter) {
        stop = true;
        return sizeof(uint32_t);
    }

    if (opcode == CycleOpcode_StartLinkedCycle) {
        uint32_t dwordCount = 0;
        if (recordOffset == UINT32_MAX || !CycleManager::IsReady() ||
            !ReadValue(g_cycleManagerRuntime.bytes, static_cast<size_t>(recordOffset) + 16u, dwordCount) || dwordCount < 5 || dwordCount > UINT32_MAX / 4u) {
            return 0;
        }
        return dwordCount * 4u;
    }

    static constexpr std::array<uint8_t, 62> kCommandSizes = {4, 16, 8, 20, 8, 24, 4, 8, 8, 8, 4, 24, 12, 4, 4, 8, 48, 8, 4, 32, 4, 20, 20, 56, 40, 24, 16, 20,
        28, 36, 4, 4, 4, 12, 12, 4, 28, 4, 4, 4, 8, 4, 36, 8, 4, 48, 8, 20, 16, 4, 4, 12, 20, 8, 16, 8, 8, 12, 4, 4, 12, 4};
    if (opcode == CycleOpcode_ScheduleBoundary || opcode == CycleOpcode_WaitHold)
        stop = true;
    return kCommandSizes[opcode];
}

bool IsReasonablePosition(const Vector3 &pos) {
    return std::isfinite(pos.x) && std::isfinite(pos.y) && std::isfinite(pos.z) && std::abs(pos.x) < 100000.0f && std::abs(pos.y) < 100000.0f &&
        std::abs(pos.z) < 100000.0f;
}

template <typename T> bool ReadCommandValue(uint32_t recordOffset, uint32_t payloadOffset, T &out) {
    if (!CycleManager::IsReady())
        return false;
    const uint64_t absoluteOffset = static_cast<uint64_t>(recordOffset) + payloadOffset;
    if (absoluteOffset > std::numeric_limits<size_t>::max())
        return false;
    return ReadValue(g_cycleManagerRuntime.bytes, static_cast<size_t>(absoluteOffset), out);
}
} // namespace CycleManager


template bool CycleManager::ReadCommandValue<float>(uint32_t recordOffset, uint32_t payloadOffset, float &out);
template bool CycleManager::ReadCommandValue<uint16_t>(uint32_t recordOffset, uint32_t payloadOffset, uint16_t &out);
template bool CycleManager::ReadCommandValue<int32_t>(uint32_t recordOffset, uint32_t payloadOffset, int32_t &out);
template bool CycleManager::ReadCommandValue<uint32_t>(uint32_t recordOffset, uint32_t payloadOffset, uint32_t &out);
namespace CycleManager {


uint32_t ReadCommandDwordOrZero(uint32_t recordOffset, uint32_t payloadOffset) {
    uint32_t value = 0;
    CycleManager::ReadCommandValue(recordOffset, payloadOffset, value);
    return value;
}

bool WriteCommandDword(uint32_t absoluteOffset, uint32_t value) {
    if (!CycleManager::IsReady())
        return false;
    std::vector<uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    if (absoluteOffset > bytes.size() || sizeof(value) > bytes.size() - absoluteOffset)
        return false;
    std::memcpy(bytes.data() + absoluteOffset, &value, sizeof(value));
    return true;
}





bool RelinkRelativeOffsetField(uint32_t charaHeaderOffset, uint32_t fieldOffset) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    int32_t rawOffset = 0;
    if (!ReadValue(cm.bytes, fieldOffset, rawOffset))
        return false;

    if (cm.relinkedOffsetFields.size() < cm.bytes.size())
        cm.relinkedOffsetFields.resize(cm.bytes.size());
    if (cm.relinkedOffsetFields[fieldOffset])
        return false;
    cm.relinkedOffsetFields[fieldOffset] = true;

    const int64_t resolved = static_cast<int64_t>(charaHeaderOffset) + rawOffset;
    if (resolved <= 0 || resolved > std::numeric_limits<uint32_t>::max()) {
        CycleManager::WriteCommandDword(fieldOffset, 0);
        return true;
    }

    CycleManager::WriteCommandDword(fieldOffset, static_cast<uint32_t>(resolved));
    return true;
}

bool CommandStreamRelinkStop(uint32_t opcode) {
    return opcode == CycleOpcode_ScheduleBoundary || opcode == CycleOpcode_WaitHold || opcode >= 0x3Eu;
}

template <typename T> bool ReadCharaConfigValue(uint32_t charaHeaderOffset, uint32_t configOffset, T &out) {
    if (!CycleManager::IsReady())
        return false;
    const uint64_t absoluteOffset = static_cast<uint64_t>(charaHeaderOffset) + configOffset;
    if (absoluteOffset > std::numeric_limits<size_t>::max())
        return false;
    return ReadValue(g_cycleManagerRuntime.bytes, static_cast<size_t>(absoluteOffset), out);
}
} // namespace CycleManager



int32_t Humans_n_GetIndexForCharacter(uint32_t charaName) {
    const shen_name *const names = &HumansIndexTable->name;
    for (int32_t index = 0; index < HumansIndexTable->numCharas; ++index) {
        if (names[index].name == charaName)
            return index;
    }
    return 0;
}
namespace CycleManager {





PackSlot *SetCyclePtrByCharaID(uint32_t chara) {
    if (!HUMANS_AFS_Ptr || g_bHasLoadedHuman || PACK_HasValidPack())
        return nullptr;
    const CycleManager::NpcRecord *record = nullptr;
    for (const CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
        if (npc.cycle->moveConfig->obj_id.name == chara) {
            record = &npc;
            break;
        }
    }
    const int32_t humansIdx = record ? record->cycle->humansIdx : Humans_n_GetIndexForCharacter(chara);
    PackSlot *const slot = PACK_LoadHumanByIndex(HUMANS_AFS_Ptr, static_cast<uint32_t>(2 * humansIdx), static_cast<uint32_t>(2 * humansIdx + 1));
    if (slot) {
        g_bHasLoadedHuman = 1;
        g_cycleScriptHumanChara = chara;
    }
    return slot;
}
} // namespace CycleManager


timeContextType_t ConstructTimeContext(TimeContext *output) {
    const GameDateTime &current = g_currentDateTime;
    output->year = current.year;
    output->month = current.month;
    output->day = current.day;
    output->hours = current.hours;
    output->minutes = current.minutes;
    output->seconds = current.seconds;
    output->dayInWeek = current.dayOfWeek;

    timeContextType_t type = NORMAL;
    if (output->dayInWeek == 0 || output->dayInWeek == 6)
        type = WEEKEND;

    if (output->month == 12 && static_cast<std::uint8_t>(output->day - 15u) <= 10u) {
        type = type == WEEKEND ? XMAS_WEEKEND : XMAS;
    }
    if (output->month == 1 && static_cast<std::uint8_t>(output->day - 1u) <= 2u) {
        type = NEW_YEAR;
    }
    return type;
}
namespace CycleManager {






uint32_t GetGameTime(CycleManager::MobjCycle *ctx, char *command, int32_t time, int32_t *inOutTime) {
    const int32_t *const word = reinterpret_cast<const int32_t *>(command);
    ctx->activeAnimId = static_cast<uint16_t>(word[1]);
    uint32_t motionToken = ctx->forcedMotionToken;
    if (!motionToken) {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        motionToken = ctx->moveConfig->motionParamIds[((static_cast<uint32_t>(g_EnvPrngSeed) >> 16) & 3u) + 1u];
    }
    ctx->selectedMotionToken = motionToken;
    float timeScale = ctx->moveConfig->motionRadiusTimingScalar;
    ctx->pathMoveSpeed = timeScale;
    ctx->baseMoveSpeed = timeScale;
    ctx->animMoveSpeedScale = 0.041666668f;
    ctx->pathPointCount = static_cast<uint16_t>(word[2]);
    ctx->pathPoints = reinterpret_cast<Vector3 *>(CycleManager::FilePointer(static_cast<uint32_t>(word[3])));
    ctx->pathPointIndex = 0;
    ctx->bbbbbbbb = 5;
    ctx->steeringState = CycleSteerState_Normal;
    ctx->steeringCommand = CycleSteerCmd_None;
    ctx->reversePath = 0;
    ctx->pathTurnRequested = 0;
    ctx->steeringBlockedFrames = 0;
    ctx->reversePathFrames = 0;
    ctx->steeringWaitFrames = 0;
    if (!ctx->moveObjRuntime && ctx->moveConfig->cycleClassCode - 4u > 3u) {
        timeScale *= 0.041666668f;
        const uint16_t opcode = ctx->currentOpcode;
        const bool walks = opcode == CycleOpcode_InitTimedPath || opcode == CycleOpcode_BeginTalkMoveToTaggedPosition || opcode == CycleOpcode_UseMnrBEntry ||
            opcode == CycleOpcode_WalkPath3D;
        if (!walks && (opcode != CycleOpcode_StartLinkedCycle || ctx->moveObjSubOpcode != CycleOpcode_WalkPath3D))
            timeScale = 0.0f;
    }

    uint32_t result = 1;
    const float scaledTime = g_cycleTimeMultiplier * timeScale;
    float remaining = static_cast<float>(time - *inOutTime) * scaledTime / g_cycleTimeMultiplier;
    remaining *= remaining;
    const Vector3 *const points = ctx->pathPoints;
    Vector3 position = points[0];
    ctx->currentWaypointPos = position;
    if (ctx->pathPointCount > 1) {
        bool last = false;
        float dx = 0.0f;
        float dz = 0.0f;
        while (true) {
            int16_t next = static_cast<int16_t>(ctx->pathPointIndex + 1);
            if (static_cast<uint16_t>(next) >= ctx->pathPointCount)
                next = static_cast<int16_t>(ctx->pathPointCount - 1);
            ctx->pathPointIndex = next;
            const Vector3 point = points[next];
            last = static_cast<uint16_t>(next + 1) >= ctx->pathPointCount;
            dx = point.x - position.x;
            const float dy = point.y - position.y;
            dz = point.z - position.z;
            ctx->currentWaypointPos = point;
            remaining -= dy * dy + dx * dx + dz * dz;
            if (remaining < 0.0f) {
                result = 0;
                break;
            }
            position = point;
            if (last)
                break;
        }
        ctx->desiredPathYaw = CycleManager::Atan2Fixed16(-dx, -dz);
        if (last) {
            ctx->activeAnimId = static_cast<uint16_t>(ctx->selectedMotionToken);
            *inOutTime = time - TruncateToInt32(std::sqrt(remaining) * g_cycleTimeMultiplier / scaledTime);
        }
    }
    ctx->pos = position;
    ctx->pathSegmentDistanceOrWork = 0.0f;
    return result;
}





uint32_t ExecuteProgramStep(CycleManager::MobjCycle *ctx, int32_t time, int32_t *inOutTime) {
    char *command = ctx->cycleCommandPtr;
    const int32_t opcode = *reinterpret_cast<const int32_t *>(command);
    if (opcode == 0 || opcode >= 62) {
        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        return 0;
    }
    ctx->currentOpcode = static_cast<CycleOpcode>(opcode);
    uint32_t result = 1;
    if (CycleManager::CheckPointer(time, ctx, &command, &result)) {
        ctx->cycleCommandPtr = command;
        return result;
    }

    const int32_t *const word = reinterpret_cast<const int32_t *>(command);
    switch (opcode) {
    case CycleOpcode_InitTimedPath:
        result = CycleManager::GetGameTime(ctx, command, time, inOutTime);
        ctx->cycleCommandPtr = command + 0x10;
        return result;
    case CycleOpcode_InitTimedPathIfAreaMatches:
        ctx->cycleCommandPtr = command + 0x10;
        return result;
    case CycleOpcode_WaitForTaggedCycleInRadius:
    case CycleOpcode_WalkPath3D:
    case CycleOpcode_WalkPath3DIfAreaMatches:
    case CycleOpcode_WaitForTaggedTalkMoveInRadius:
        ctx->cycleCommandPtr = command + 0xC;
        return result;
    case CycleOpcode_SetActiveAnimAndMotionVariant:
        CycleManager::ApplyActiveAnimAndMotionVariant(*CycleManager::CycleRecord(ctx), static_cast<uint16_t>(word[1]), false);
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_SetPositionAndHeading:
        ctx->pos = {std::bit_cast<float>(word[1]), std::bit_cast<float>(word[2]), std::bit_cast<float>(word[3])};
        ctx->desiredPathYaw = static_cast<int16_t>(word[4]);
        ctx->cycleCommandPtr = command + 0x14;
        return result;
    case CycleOpcode_ScheduleBoundary:
        ctx->cycleCommandPtr = command + 8;
        return 0;
    case CycleOpcode_WaitSeconds:
        *inOutTime += word[1];
        ctx->cycleCommandPtr = command + 8;
        return *inOutTime <= time ? result : 0;
    case CycleOpcode_WaitSecondsIfAreaMatches:
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_RideVehiclePath2D:
        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        ctx->cycleCommandPtr = command + 0x18;
        return result;
    case CycleOpcode_BeginTalkMoveToTaggedPosition:
        ctx->cycleCommandPtr = command + 0x18;
        return result;
    case CycleOpcode_DriveForkliftPath2D:
        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        ctx->cycleCommandPtr = command + 0x1C;
        return result;
    case CycleOpcode_SetAreaTag:
        if (ctx->areaID.name != static_cast<uint32_t>(word[1])) {
            ctx->areaID = static_cast<uint32_t>(word[1]);
            ctx->pos = {10000.0f, 10000.0f, 10000.0f};
        }
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_SetActionMode:
        ctx->modeValue = static_cast<uint32_t>(word[1]);
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_SetModeByte:
        ctx->proximityGroup = static_cast<uint8_t>(word[1]);
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_SetScriptVisibleState:
        ctx->scriptContextValue = word[1];
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_BeginObjectInteraction:
        CycleManager::QueueObjectInteraction(*CycleManager::CycleRecord(ctx), CycleManager::FileOffset(command));
        ctx->cycleCommandPtr = command + 0x30;
        return result;
    case CycleOpcode_EndObjectInteraction:
        if (CycleManager::TaggedPayload *const payload = CycleManager::FindTaggedPayload(*CycleManager::CycleRecord(ctx), static_cast<uint32_t>(word[1])))
            payload->lifetime = 7683;
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_Transform:
        CommitXForm_RotateTranslateZYX(*CycleManager::CycleRecord(ctx), CycleManager::FileOffset(command));
        ctx->cycleCommandPtr = command + 0xC;
        return result;
    case CycleOpcode_WaitHold:
        ctx->cycleCommandPtr = command + 4;
        return 0;
    case CycleOpcode_RequestNearestDoorTransition:
    case CycleOpcode_WaitTurnAnimation:
    case CycleOpcode_WaitOutsideRadius:
    case CycleOpcode_Reserved3A:
    case CycleOpcode_Reserved3B:
    case CycleOpcode_FalseReturnMarker:
        ctx->cycleCommandPtr = command + 4;
        return result;
    case CycleOpcode_PayloadYield:
        ctx->currentOpcode = CycleOpcode_RemoveCharacter;
        ctx->cycleCommandPtr = command + 0x20;
        return result;
    case CycleOpcode_StartLinkedCycle: {
        CycleManager::NpcRecord &npc = *CycleManager::CycleRecord(ctx);
        const int32_t endTime = word[2] >= 0 ? word[2] : *inOutTime - word[2];
        if (*inOutTime + 300 > time || endTime > time) {
            if (CycleManager::InitializeMoveObjContext(npc, CycleManager::FileOffset(command), true))
                result = 0;
        } else if (CycleManager::InitializeMoveObjContext(npc, CycleManager::FileOffset(command), true)) {
            CycleManager::EndMoveObjForHistory(npc);
        }
        ctx->cycleCommandPtr = command + 4 * word[4];
        return result;
    }
    case CycleOpcode_PlayRootedAnim: {
        CycleManager::NpcRecord &npc = *CycleManager::CycleRecord(ctx);
        if (ctx->doorAnimActive == 1)
            CycleManager::FreeIfDoorOpeningAnim(npc);
        CycleManager::PlayAnim_Wrapper(npc, CycleManager::FileOffset(command));
        ctx->cycleCommandPtr = command + 0x18;
        return result;
    }
    case CycleOpcode_SetAuxiliaryAnim:
        PlayAnim_Wrapper_0(*CycleManager::CycleRecord(ctx), CycleManager::FileOffset(command));
        ctx->cycleCommandPtr = command + 0x10;
        return result;
    case CycleOpcode_SetAnimationState:
        CycleManager::ApplyAnimationStateCommand(ctx, command);
        ctx->cycleCommandPtr = command + 0x10;
        return result;
    case CycleOpcode_FollowStairPath2D:
    case CycleOpcode_DriveVehiclePath2D:
        ctx->cycleCommandPtr = command + 0x14;
        return result;
    case CycleOpcode_ArmOutsideRadiusFallback:
        ctx->cycleCommandPtr = command + 0x24;
        return result;
    case CycleOpcode_SavePointerMarker:
        ctx->branchResumeCommandPtr = command + 4;
        ctx->cycleCommandPtr = command + 4;
        return result;
    case CycleOpcode_SetVehicleActiveState: {

        CycleManager::NpcRecord &npc = *CycleManager::CycleRecord(ctx);
        npc.vehicleToken = static_cast<uint32_t>(word[1]);
        npc.vehicleCommandPos = {std::bit_cast<float>(word[2]), std::bit_cast<float>(word[3]), std::bit_cast<float>(word[4])};
        npc.vehicleCommandYaw = static_cast<int16_t>(word[5]);
        npc.vehicleCommandEnabled = word[6] != 0;
        CycleManager::BeginVehicleActiveState(npc, false);
        ctx->cycleCommandPtr = command + 0x1C;
        return result;
    }
    case CycleOpcode_SetShopShutterState:
        ctx->morsCommandArg = static_cast<uint32_t>(word[1]);
        ctx->morsCommandStatus = static_cast<uint32_t>(word[2]);
        CycleManager::DispatchSceneTask(static_cast<uint32_t>(word[1]), static_cast<uint32_t>(word[2] + 4), static_cast<uint32_t>(*inOutTime));
        ctx->cycleCommandPtr = command + 0x24;
        return result;
    case CycleOpcode_GetOtherChara:
        ctx->humansIdx = Humans_n_GetIndexForCharacter(static_cast<uint32_t>(word[1]));
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_CreateCharacter:
        CycleManager::ApplyNpcCommandState(*CycleManager::CycleRecord(ctx), CycleOpcode_CreateCharacter, CycleManager::FileOffset(command));
        ctx->cycleCommandPtr = command + 0x14;
        return result;
    case CycleOpcode_SetPositionAndHeadingImmediate:
        ctx->pos = {std::bit_cast<float>(word[1]), std::bit_cast<float>(word[2]), std::bit_cast<float>(word[3])};
        ctx->desiredPathYaw = static_cast<int16_t>(word[4]);
        ctx->currentOpcode = CycleOpcode_AdvanceScript;
        ctx->cycleCommandPtr = command + 0x14;
        return result;
    case CycleOpcode_SetForcedMotionToken:
        ctx->forcedMotionToken = static_cast<uint32_t>(word[1]);
        ctx->cycleCommandPtr = command + 8;
        return result;
    case CycleOpcode_SetSteeringProfile:
        ctx->steeringProfile = word[1] != 0;
        CycleManager::RecomputeSteeringExtents(*CycleManager::CycleRecord(ctx));
        ctx->cycleCommandPtr = command + 8;
        return result;
    default: {
        const uint32_t index = g_TotalActiveCycles >= 10u ? 0u : g_TotalActiveCycles;
        g_CycleTable[2u * index] = ctx->moveConfig->obj_id.name;
        g_TotalActiveCycles = index + 1u;
        ctx->cycleCommandPtr = command;
        return result;
    }
    }
}

void ScanCommandStream(uint32_t commandOffset, CycleManager::ProgramScan &scan) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    uint32_t cursor = commandOffset;
    for (uint32_t records = 0; records < 4096; ++records) {
        uint32_t opcode = 0;
        if (!ReadValue(cm.bytes, cursor, opcode)) {
            ++scan.truncatedStreams;
            ++cm.truncatedCommandStreams;
            return;
        }

        if (CycleManager::CommandStreamRelinkStop(opcode))
            return;

        bool stop = false;
        const uint32_t size = CycleManager::CommandRecordSize(opcode, stop, cursor);
        if (stop)
            return;
        if (!size || cursor > cm.bytes.size() || size > cm.bytes.size() - cursor) {
            ++scan.truncatedStreams;
            ++cm.truncatedCommandStreams;
            return;
        }

        ++scan.opcodeCount;
        ++cm.totalOpcodeCount;
        ++cm.opcodeCounts[opcode];
        if (opcode == CycleOpcode_SetShopShutterState) {
            if (cm.shopShutterCommandOffsetSet.insert(cursor).second)
                cm.shopShutterCommandOffsets.push_back(cursor);
        }
        cursor += size;
    }

    ++scan.truncatedStreams;
    ++cm.truncatedCommandStreams;
}




bool GetType(uint32_t charaHeaderOffset, uint32_t &cursor) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    uint32_t opcode = 0;
    ReadValue(cm.bytes, cursor, opcode);
    switch (opcode) {
    case CycleOpcode_UseMnrBEntry:
        cursor += 56;
        return true;
    case CycleOpcode_ScheduleMnrBEntry:
        cursor += 40;
        return true;
    case CycleOpcode_PlayEntryStateAnimPoolUntilTime: {
        int32_t poolOffset = 0;
        int32_t first = 0;
        ReadValue(cm.bytes, cursor + 8u, poolOffset);
        if (poolOffset && ReadValue(cm.bytes, static_cast<size_t>(static_cast<int64_t>(charaHeaderOffset) + poolOffset), first) && first != -1)
            CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, cursor + 8u);
        cursor += 12;
        return true;
    }
    default:
        return false;
    }
}




CycleManager::ProgramScan RelinkCycleProgram(uint32_t charaHeaderOffset) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    CycleManager::ProgramScan scan{};
    scan.headerOffset = charaHeaderOffset;

    struct_InCharaCycleFileDataPtr charaCycle{};
    if (!ReadValue(cm.bytes, charaHeaderOffset, charaCycle))
        return scan;

    scan.chara_id = charaCycle.chara_id;
    const uint32_t timeTableOffset = static_cast<int32_t>(charaCycle.offset) > 0 ? charaHeaderOffset + charaCycle.offset : 0;
    scan.timeTableOffset = timeTableOffset;
    if (!timeTableOffset || timeTableOffset >= cm.bytes.size())
        return scan;

    for (uint32_t i = 0; i < 2048; ++i) {
        CycleTimetableRow entry{};
        const uint32_t entryOffset = timeTableOffset + i * static_cast<uint32_t>(sizeof(entry));
        if (!ReadValue(cm.bytes, entryOffset, entry))
            break;
        if (entry.timeOrEndMarker == -1)
            break;

        ++scan.timeTableEntries;
        ++cm.totalTimeTableEntries;
        CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, entryOffset + 4);
        int32_t relinkedCommand = 0;
        ReadValue(cm.bytes, entryOffset + 4, relinkedCommand);
        const uint32_t commandOffset = relinkedCommand > 0 ? static_cast<uint32_t>(relinkedCommand) : 0;
        if (commandOffset && commandOffset < cm.bytes.size()) {
            ++scan.commandStreams;
            ++cm.totalCommandStreams;
            scan.commandStreamOffsets.push_back(commandOffset);
            uint32_t cursor = commandOffset;
            uint32_t opcode = 0;
            while (ReadValue(cm.bytes, cursor, opcode) && opcode != CycleOpcode_RemoveCharacter) {
                if (opcode == CycleOpcode_ScheduleBoundary || opcode == CycleOpcode_WaitHold || opcode >= 0x3Eu)
                    break;
                if (CycleManager::GetType(charaHeaderOffset, cursor))
                    continue;
                switch (opcode) {
                case CycleOpcode_InitTimedPath:
                case CycleOpcode_InitTimedPathIfAreaMatches:
                    CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, cursor + 12);
                    cursor += 16;
                    break;
                case CycleOpcode_SetActiveAnimAndMotionVariant:
                case CycleOpcode_ScheduleBoundary:
                case CycleOpcode_WaitSeconds:
                case CycleOpcode_SetAreaTag:
                case CycleOpcode_SetActionMode:
                case CycleOpcode_SetScriptVisibleState:
                case CycleOpcode_EndObjectInteraction:
                case CycleOpcode_SetModeByte:
                case CycleOpcode_GetOtherChara:
                case CycleOpcode_EndLinkedObjectInteraction:
                case CycleOpcode_SetForcedMotionToken:
                case CycleOpcode_WaitSecondsIfAreaMatches:
                case CycleOpcode_SetSteeringProfile:
                    cursor += 8;
                    break;
                case CycleOpcode_SetPositionAndHeading:
                case CycleOpcode_StartLinkedCycle:
                case CycleOpcode_CreateCharacter:
                case CycleOpcode_SetPositionAndHeadingImmediate:
                    cursor += 20;
                    break;
                case CycleOpcode_RideVehiclePath2D:
                    CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, cursor + 16);
                    cursor += 24;
                    break;
                case CycleOpcode_BeginTalkMoveToTaggedPosition:
                case CycleOpcode_PlayRootedAnim:
                    cursor += 24;
                    break;
                case CycleOpcode_WaitForTaggedCycleInRadius:
                case CycleOpcode_Transform:
                case CycleOpcode_WaitForTaggedTalkMoveInRadius:
                    cursor += 12;
                    break;
                case CycleOpcode_RequestNearestDoorTransition:
                case CycleOpcode_SavePointerMarker:
                case CycleOpcode_ModeByteMarker:
                case CycleOpcode_Reserved3A:
                case CycleOpcode_Reserved3B:
                case CycleOpcode_FalseReturnMarker:
                    cursor += 4;
                    break;
                case CycleOpcode_BeginObjectInteraction:
                case CycleOpcode_BeginLinkedObjectInteraction:
                    cursor += 48;
                    break;
                case CycleOpcode_PayloadYield:
                    cursor += 32;
                    break;
                case CycleOpcode_FollowStairPath2D:
                case CycleOpcode_DriveVehiclePath2D:
                    CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, cursor + 12);
                    cursor += 20;
                    break;
                case CycleOpcode_SetAnimationState:
                case CycleOpcode_SetAuxiliaryAnim:
                    cursor += 16;
                    break;
                case CycleOpcode_DriveForkliftPath2D:
                    CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, cursor + 16);
                    cursor += 28;
                    break;
                case CycleOpcode_SetVehicleActiveState:
                    cursor += 28;
                    break;
                case CycleOpcode_ArmOutsideRadiusFallback:
                case CycleOpcode_SetShopShutterState:
                    cursor += 36;
                    break;
                case CycleOpcode_WalkPath3D:
                case CycleOpcode_WalkPath3DIfAreaMatches:
                    CycleManager::RelinkRelativeOffsetField(charaHeaderOffset, cursor + 8);
                    cursor += 12;
                    break;
                default: {
                    uint32_t index = g_TotalActiveCycles;
                    if (index >= 10u)
                        index = 0;
                    g_CycleTable[2u * index] = charaCycle.chara_id.name;
                    g_TotalActiveCycles = index + 1;
                    cursor += 4;
                    break;
                }
                }
            }
            CycleManager::ScanCommandStream(commandOffset, scan);
        }
    }

    return scan;
}





bool SelectBranchByFlagsAndDate(uint32_t charaHeaderOffset, timeContextType_t type, const TimeContext &context) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    int32_t current = 0;
    uint32_t conditionCount = 0;
    int32_t conditionsStart = 0;
    std::array<int32_t, 16> timetables{};
    if (!ReadValue(cm.bytes, charaHeaderOffset, current) || !ReadValue(cm.bytes, charaHeaderOffset + 0x74u, conditionCount) ||
        !ReadValue(cm.bytes, charaHeaderOffset + 0x78u, conditionsStart))
        return false;
    for (uint32_t i = 0; i < timetables.size(); ++i)
        ReadValue(cm.bytes, charaHeaderOffset + 0xBCu + i * 4u, timetables[i]);

    timeContextType_t dayType = type;
    if (type == XMAS || type == XMAS_WEEKEND) {
        if (timetables[0] != timetables[2])
            dayType = XMAS;
        else if (type == XMAS_WEEKEND)
            dayType = WEEKEND;
    } else if (type == NEW_YEAR && timetables[0] == timetables[3]) {
        dayType = WEEKEND;
    }
    int32_t selected = timetables[static_cast<uint32_t>(dayType)];
    for (int32_t i = 0; i < static_cast<int32_t>(conditionCount); ++i) {
        ConditionRow row{};
        if (!ReadValue(cm.bytes, charaHeaderOffset + static_cast<uint32_t>(conditionsStart) + static_cast<uint32_t>(i) * 0x10u, row))
            break;
        if (GetFlag(row.clearFlagA) || (row.clearFlagB && GetFlag(row.clearFlagB)))
            continue;
        if (row.endMonth && row.endDay && row.endMonth <= context.month && row.endDay <= context.day)
            continue;
        if (!GetFlag(row.setFlagA) || (row.setFlagB && !GetFlag(row.setFlagB)))
            continue;
        if (row.startMonth && row.startDay && (row.startMonth > context.month || row.startDay > context.day))
            continue;
        if (row.dayType >= 0 && row.dayType != static_cast<int16_t>(dayType))
            continue;
        ReadValue(cm.bytes, charaHeaderOffset + 0xBCu + static_cast<uint32_t>(static_cast<int32_t>(row.timetable) * 4), selected);
        break;
    }
    CycleManager::WriteCommandDword(charaHeaderOffset, static_cast<uint32_t>(selected));
    CycleManager::RelinkCycleProgram(charaHeaderOffset);
    return current != selected;
}




int32_t ReadCyclemanForScene(const char *InFilename) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    const char *const path = InFilename ? InFilename : "";
    bool shouldRunCycle = g_MOBJ_needs_new_update != 0;
    if (std::strcmp(g_tmp_cm_path, path) != 0) {
        std::snprintf(g_tmp_cm_path, sizeof(g_tmp_cm_path), "%s", path);
        CycleManager::MoveObj_FileLoad(std::strstr(path, "D000") ? "cm_d000.bin" : std::strstr(path, "MFSY") ? "cm_mfsy.bin" : "cm_jomo.bin");
        cm.ready = cm.loaded;
        cm.currentSectionToken = 0;
        cm.currentSectionOffset = 0;
        cm.currentSectionSize = 0;
        if (cm.loaded) {
            if (!cm.schedule.empty()) {
                if (!CycleManager::SelectCurrentSection(CycleManager::CurrentAreaId()) && !CycleManager::SelectCurrentSection(SHEN_NAME(MSCR)) &&
                    !cm.sections.empty() && cm.sections.front().offset < cm.bytes.size()) {
                    cm.currentSectionToken = cm.sections.front().token;
                    cm.currentSectionOffset = cm.sections.front().offset;
                    cm.currentSectionSize = cm.sections.front().size;
                }
            }
        }
        shouldRunCycle = true;
        g_MOBJ_needs_new_update = 1;
    }

    cm.mscrOffset = 0;
    cm.mscrCharaCount = 0;
    if (!cm.loaded || cm.schedule.empty())
        return 0;
    uint8_t *mscrPtr = CycleManager::MoveObj_FindEntry(cm.schedule.data(), SHEN_NAME(MSCR), 0, static_cast<int32_t>(cm.schedule.size()) - 1);
    if (!mscrPtr) {
        return 0;
    }
    const uint32_t mscrOffset = static_cast<uint32_t>(mscrPtr - cm.bytes.data());
    struct_v16_2 MSCR{};
    if (!ReadValue(cm.bytes, mscrOffset, MSCR))
        return 0;
    if (MSCR.charaCnt > 1024) {
        return 0;
    }
    cm.mscrOffset = mscrOffset;
    cm.mscrCharaCount = MSCR.charaCnt;
    if (!shouldRunCycle)
        return static_cast<int32_t>(cm.mscrCharaCount);

    cm.programs.clear();
    cm.opcodeCounts.clear();
    cm.shopShutterCommandOffsets.clear();
    cm.shopShutterCommandOffsetSet.clear();
    cm.localNpcCharactersCreated = false;
    cm.totalTimeTableEntries = 0;
    cm.totalCommandStreams = 0;
    cm.totalOpcodeCount = 0;
    cm.truncatedCommandStreams = 0;
    for (uint32_t i = 0; i < MSCR.charaCnt; ++i) {
        uint32_t charaRelativeOffset = 0;
        if (!ReadValue(cm.bytes, cm.mscrOffset + 4 + i * 4, charaRelativeOffset))
            break;
        cm.programs.push_back(CycleManager::RelinkCycleProgram(cm.mscrOffset + charaRelativeOffset));
    }


    return static_cast<int32_t>(cm.mscrCharaCount);
}






void ParseForScript(uint32_t programName, bool enabled) {
    if (!CycleManager::IsReady())
        return;

    const auto find = [programName](CycleManager::MobjEntry *entry) {
        while (entry && entry->ctx->moveConfig->obj_id.name != programName)
            entry = entry->nextDormantProgram;
        return entry;
    };
    if (enabled) {
        static constexpr std::array<CycleProgramList, 5> kEnableSearchOrder{
            CycleProgramList_Dormant, CycleProgramList_Near, CycleProgramList_SimSet, CycleProgramList_Promote, CycleProgramList_Mid};
        for (CycleProgramList list : kEnableSearchOrder) {
            CycleManager::MobjEntry *const entry = find(CycleManager::ProgramListHead(list));
            if (!entry)
                continue;
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
            if (list != CycleProgramList_Dormant && list != CycleProgramList_Near)
                CycleManager::UpdateSndStreaming(npc);
            CycleManager::MoveProgramToList(npc, CycleProgramList_Running, CycleListInsert::Head);
            return;
        }
        if (g_LoadQueue) {
            if (g_LoadQueue->ctx->moveConfig->obj_id.name == programName) {
                g_LoadQueue->ctx->loadQueueDisposition = 1;
                g_LoadQueue->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
                return;
            }
            if (CycleManager::MobjEntry *const entry = find(g_LoadQueue->nextDormantProgram)) {
                entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
                CycleManager::MoveProgramToList(*CycleManager::EntryRecord(entry), CycleProgramList_Running, CycleListInsert::Head);
                return;
            }
        }
        if (CycleManager::MobjEntry *const entry = find(g_FarQueue)) {
            CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
            entry->ctx->currentOpcode = CycleOpcode_RemoveCharacter;
            CycleManager::UpdateSndStreaming(npc);
            CycleManager::MoveProgramToList(npc, CycleProgramList_Running, CycleListInsert::Head);
        }
        return;
    }

    CycleManager::MobjEntry *const entry = find(g_RunningPrograms);
    if (!entry)
        return;
    CycleManager::NpcRecord &npc = *CycleManager::EntryRecord(entry);
    entry->ctx->loadedBinHandle = nullptr;
    CycleManager::MoveProgramToList(npc, CycleProgramList_Dormant, CycleListInsert::Head);
    TimeContext timeContext{};
    const timeContextType_t timeContextType = ConstructTimeContext(&timeContext);
    CycleManager::SelectBranchByFlagsAndDate(npc.moveConfigOffset, timeContextType, timeContext);
    CycleManager::UpdateCurrentCursor(entry->ctx);
    CycleManager::Update(entry);
}




PackSlot *ParseForScript_InternalForHuman(uint32_t programName) {
    const auto find = [programName](CycleManager::MobjEntry *entry) {
        while (entry && entry->ctx->moveConfig->obj_id.name != programName)
            entry = entry->nextDormantProgram;
        return entry;
    };
    CycleManager::MobjEntry *entry = find(g_SimSetHead);
    if (!entry)
        entry = find(g_PromoteList);
    if (!entry)
        entry = find(g_MidQueue);
    if (!entry) {
        CycleManager::ParseForScript(programName, true);
        return nullptr;
    }
    CycleManager::MobjCycle *const ctx = entry->ctx;
    ctx->currentOpcode = CycleOpcode_RemoveCharacter;
    CycleManager::MoveProgramToList(*CycleManager::EntryRecord(entry), CycleProgramList_Running, CycleListInsert::Head);
    if (Character *const obj = ctx->obj) {
        const bool wasVisible = (obj->motionFlags & 0x3Fu) != 0;
        obj->motionFlags |= 0x3Fu;
        if (g_bDebugLogVisibility && (obj->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
            CHAR_LogNPCVisibility(obj, true);
    }
    if (ctx->pFace)
        FACE_UpdateExpression(ctx->obj, 1, 0, 2);
    return ctx->filename;
}




uint32_t GetNextTaskPointer(PackSlot *slot) {
    if (!PACK_IsValidSlot(slot))
        return 0;
    g_bHasLoadedHuman = 0;
    CycleManager::MobjEntry *entry = g_RunningPrograms;
    while (entry && entry->ctx->moveConfig->obj_id.name != g_cycleScriptHumanChara)
        entry = entry->nextDormantProgram;
    if (!entry)
        return 1;
    CycleManager::MobjCycle *const ctx = entry->ctx;
    ctx->filename = slot;
    std::memcpy(g_cycle_file_str, &ctx->moveConfig->obj_id.name, 4);
    ctx->loadedBinHandle = LoadFileIntoHeapMem(slot, g_cycle_file_str, make_shen_name('B', 'I', 'N', ' '));
    ctx->obj = CycleManager::FindCharacter(ctx->moveConfig->obj_id.name);
    ctx->mot = MOTN_getMOTM(ctx->obj);
    npcSetupFace(ctx);
    return 1;
}




void ParseProgramForChara(uint32_t charaToken, uint32_t timetableIndex) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    for (CycleManager::NpcRecord &npc : cm.npcRecords) {
        CycleManager::MobjData *const header = npc.cycle->moveConfig;
        if (header->obj_id.name != charaToken)
            continue;
        CycleManager::WriteCommandDword(npc.moveConfigOffset, static_cast<uint32_t>(header->timetableOffsets[timetableIndex]));
        CycleManager::RelinkCycleProgram(npc.moveConfigOffset);
        uint32_t index = g_TotalActiveCycles;
        if (index >= 10u)
            index = 0;
        g_CycleTable[2u * index] = charaToken;
        g_TotalActiveCycles = index + 1;
        return;
    }
}



void MoveObj_FileLoad(const char *InFilename) {
    CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    cm.loaded = false;
    cm.bytes.clear();
    cm.relinkedOffsetFields.clear();
    cm.sections.clear();
    cm.schedule.clear();
    const std::string rel = std::string("misc/") + InFilename;
    if (!CycleManager::ReadCurrentFile(rel, cm.bytes)) {
        return;
    }

    cm.sourcePath = rel;
    uint32_t signature = 0;
    uint32_t count = 0;
    ReadValue(cm.bytes, 0, signature);
    ReadValue(cm.bytes, 4, count);
    if (signature != SHEN_NAME(MOBJ) || count > 256 || 8ull + static_cast<uint64_t>(count) * 8ull > cm.bytes.size()) {
        return;
    }

    for (uint32_t i = 0; i < count; ++i) {
        CycleManager::Section section{};
        ReadValue(cm.bytes, 8 + static_cast<size_t>(i) * 8, section.token);
        ReadValue(cm.bytes, 12 + static_cast<size_t>(i) * 8, section.offset);
        cm.schedule.push_back({static_cast<int32_t>(section.token), static_cast<int32_t>(section.offset)});
        cm.sections.push_back(section);
    }

    for (CycleManager::Section &section : cm.sections) {
        uint32_t nextOffset = static_cast<uint32_t>(cm.bytes.size());
        for (const CycleManager::Section &other : cm.sections) {
            if (other.offset > section.offset && other.offset < nextOffset)
                nextOffset = other.offset;
        }
        if (section.offset < cm.bytes.size() && nextOffset >= section.offset)
            section.size = nextOffset - section.offset;
    }

    cm.loaded = true;
}
} // namespace CycleManager


} // namespace shenmue::s1
