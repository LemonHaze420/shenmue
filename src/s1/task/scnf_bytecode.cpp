#include "s1/task/scen_state.hpp"

namespace shenmue::s1::scen_detail {

const std::byte *ScenScriptEnd(const ScenRuntimeObject &object) {
    const auto *const record = static_cast<const ScenRuntimeRecord *>(object.fileHeader);
    return record ? reinterpret_cast<const std::byte *>(record) + record->size : nullptr;
}

bool ScenCanRead(const ScenRuntimeObject &object, const std::byte *pointer, std::size_t size) {
    const std::byte *const begin = reinterpret_cast<const std::byte *>(object.fileHeader);
    const std::byte *const end = ScenScriptEnd(object);
    return begin && pointer && pointer >= begin && pointer <= end && size <= static_cast<std::size_t>(end - pointer);
}

std::byte *ScenAddRelative(ScenRuntimeObject &object, std::byte *base, std::int32_t delta) {
    const std::byte *const begin = reinterpret_cast<const std::byte *>(object.fileHeader);
    const std::byte *const end = ScenScriptEnd(object);
    if (!begin || !end || !base)
        return nullptr;
    const auto address = reinterpret_cast<std::uintptr_t>(base);
    const auto target = delta < 0 ? address - static_cast<std::uintptr_t>(-static_cast<std::int64_t>(delta)) : address + static_cast<std::uintptr_t>(delta);
    const auto beginAddress = reinterpret_cast<std::uintptr_t>(begin);
    const auto endAddress = reinterpret_cast<std::uintptr_t>(end);
    if (target < beginAddress || target > endAddress)
        return nullptr;
    return reinterpret_cast<std::byte *>(target);
}

std::int32_t ScenSignExtend(std::uint32_t value, std::uint32_t signBit) {
    return (value & signBit) != 0 ? static_cast<std::int32_t>(value | ~(signBit * 2U - 1U)) : static_cast<std::int32_t>(value);
}

std::size_t ScenWordSlotIndex(const ScenRuntimeObject &object) {
    return static_cast<std::size_t>(object.scriptSlot) * kScntWordsPerSlot;
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

void SetStatOrNotebookFlag(std::uint16_t flag, bool memo) {
    if (flag >= 1024U)
        return;
    std::uint8_t &byte = g_shenmueFlags[flag >> 3U];
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (flag & 7U));
    if (memo)
        MMTK_UpdateEntryForFlag(flag);
    byte = memo ? static_cast<std::uint8_t>(byte | mask) : static_cast<std::uint8_t>(byte & ~mask);
    RegisterInventoryItemGroup(flag);
}

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

std::int32_t ResolveScenExprValue(const ScenExprValue &expression) {
    switch (expression.type) {
    case 1:
    case 5:
    case 6:
        return expression.value;
    case 2:
        if (expression.value >= 0 && expression.value < 1024) {
            const auto id = static_cast<std::uint32_t>(expression.value);
            return (g_shenmueFlags[id >> 3U] & static_cast<std::uint8_t>(1U << (id & 7U))) != 0;
        }
        break;
    case 3:
        if (expression.value >= 0 && expression.value < 64) {
            const auto *const bits = g_scenarioStepLut.bitFlags64;
            const auto id = static_cast<std::uint32_t>(expression.value);
            return (bits[id >> 3U] & static_cast<std::uint8_t>(1U << (id & 7U))) != 0;
        }
        break;
    case 4:
        if (expression.value >= 0 && expression.value < 32) {
            return g_scenarioStepLut.steps[expression.value];
        }
        break;
    default:
        break;
    }
    return 0;
}

void CommitScenExprValue(const ScenExprValue &expression, std::int32_t value) {
    switch (expression.type) {
    case 2:
        if (expression.value >= 0)
            SetStatOrNotebookFlag(static_cast<std::uint16_t>(expression.value), value != 0);
        break;
    case 3:
        if (expression.value >= 0 && expression.value < 64) {
            auto *const bits = g_scenarioStepLut.bitFlags64;
            const auto id = static_cast<std::uint32_t>(expression.value);
            const auto mask = static_cast<std::uint8_t>(1U << (id & 7U));
            bits[id >> 3U] = value ? static_cast<std::uint8_t>(bits[id >> 3U] | mask) : static_cast<std::uint8_t>(bits[id >> 3U] & ~mask);
        }
        break;
    case 4:
        if (expression.value >= 0 && expression.value < 32) {
            g_scenarioStepLut.steps[expression.value] = static_cast<std::uint8_t>(value);
        }
        break;
    default:
        break;
    }
}

std::int32_t ScenCycleContextValue(const ScenRuntimeObject &object, std::uint16_t operand) {
    const std::uint32_t target = operand && operand < kValidScenCharacterNames.size() ? kValidScenCharacterNames[operand] : object.name.name;
    for (const CycleProgramList list : {CycleProgramList_SimSet, CycleProgramList_Running}) {
        for (const CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(list); npc; npc = CycleManager::NextProgramInList(list, npc)) {
            if (npc->cycle->moveConfig->obj_id.name == target && (list == CycleProgramList_Running || npc->cycle->motnSteeringActive))
                return npc->cycle->contextValue;
        }
    }
    return -1;
}

std::int32_t PlayerInsideScenSpecialZone(std::uint16_t region) {
    struct Zone {
        std::uint32_t area;
        float x;
        float z;
        float radiusSquared;
    };
    constexpr std::array<Zone, 13> zones{{
        {SHEN_NAME(D000), 6.0F, 84.0F, 225.0F},
        {SHEN_NAME(D000), 12.0F, 46.0F, 196.0F},
        {SHEN_NAME(D000), -127.0F, 70.0F, 400.0F},
        {SHEN_NAME(D000), -20.0F, 72.0F, 225.0F},
        {SHEN_NAME(D000), -14.0F, 28.0F, 225.0F},
        {SHEN_NAME(D000), -103.0F, 98.0F, 100.0F},
        {SHEN_NAME(D000), -41.0F, 75.0F, 225.0F},
        {SHEN_NAME(D000), 38.0F, 10.0F, 225.0F},
        {SHEN_NAME(D000), -57.0F, 44.0F, 225.0F},
        {SHEN_NAME(D000), 22.0F, 36.0F, 225.0F},
        {SHEN_NAME(MFSY), 14.0F, 142.0F, 1225.0F},
        {SHEN_NAME(MFSY), -30.0F, 91.0F, 1225.0F},
        {SHEN_NAME(MFSY), 124.0F, 51.0F, 400.0F},
    }};
    Character *const ryo = CurrentScenarioRyo();
    if (!region || region > zones.size() || !ryo)
        return 0;
    const Zone &zone = zones[region - 1U];
    if (zone.area != g_areaId)
        return 0;
    const Vector3 position = GetPositionForEntity(ryo->entity.node);
    const float dx = position.x - zone.x;
    const float dz = position.z - zone.z;
    return dx * dx + dz * dz < zone.radiusSquared ? 1 : 0;
}

std::byte *DecodeScenExpression(std::byte *ip, ScenRuntimeObject &object) {
    std::vector<ScenExprValue> stack;
    stack.reserve(32);
    for (std::uint32_t guard = 0; guard < 2048; ++guard) {
        if (!ScenCanRead(object, ip, 1))
            return nullptr;
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(*ip++);
        switch (opcode & 0xe0U) {
        case 0x00:
        case 0x20:
        case 0x40:
        case 0x60: {
            if (!ScenCanRead(object, ip, 1))
                return nullptr;
            const std::int32_t value = std::to_integer<std::uint8_t>(*ip++) | static_cast<std::int32_t>((opcode & 0xfU) << 8U);
            ScenExprValue expression{};
            expression.value = value;
            if ((opcode & 0xe0U) == 0x00)
                expression.type = 2;
            else if ((opcode & 0xe0U) == 0x20)
                expression.type = 3;
            else if ((opcode & 0xe0U) == 0x40) {
                expression.type = value == 4095 ? 1 : 4;
                if (value == 4095)
                    expression.value = -1;
            } else {
                expression.type = 1;
            }
            stack.push_back(expression);
            break;
        }
        case 0x80: {
            const std::uint8_t operation = opcode & 0x1fU;
            if (operation == 0) {
                if (stack.empty())
                    return nullptr;
                g_scenRuntimeState.conditionResult = ResolveScenExprValue(stack.back());
                object.runtimeFlags &= ~0x200U;
                return ip;
            }
            if (operation == 1) {
                if (stack.size() < 2)
                    return nullptr;
                const std::int32_t value = ResolveScenExprValue(stack.back());
                stack.pop_back();
                CommitScenExprValue(stack.back(), value);
                stack.pop_back();
                break;
            }
            if (operation >= 2 && operation <= 4) {
                if (stack.empty())
                    return nullptr;
                std::int32_t value = ResolveScenExprValue(stack.back());
                if (operation == 2)
                    ++value;
                else if (operation == 3)
                    --value;
                else
                    value = value == 0 ? 1 : 0;
                if (operation != 4)
                    CommitScenExprValue(stack.back(), value);
                else
                    stack.back() = {1, value};
                break;
            }
            if (stack.size() < 2)
                return nullptr;
            const std::int32_t right = ResolveScenExprValue(stack.back());
            stack.pop_back();
            const std::int32_t left = ResolveScenExprValue(stack.back());
            stack.pop_back();
            std::int32_t result = 0;
            switch (operation) {
            case 5:
                result = left && right;
                break;
            case 6:
                result = left || right;
                break;
            case 7:
                result = left == right;
                break;
            case 8:
                result = left != right;
                break;
            case 9:
                result = left < right;
                break;
            case 10:
                result = left <= right;
                break;
            case 11:
                result = left > right;
                break;
            case 12:
                result = left >= right;
                break;
            default:
                return nullptr;
            }
            stack.push_back({1, result});
            break;
        }
        case 0xa0: {
            if (!ScenCanRead(object, ip, 1))
                return nullptr;
            const std::uint8_t type = std::to_integer<std::uint8_t>(*ip++);
            std::int32_t value = 0;
            switch (type) {
            case 0:
                value = g_currentDateTime.year;
                break;
            case 1:
                value = g_currentDateTime.month;
                break;
            case 2:
                value = g_currentDateTime.day;
                break;
            case 3:
                value = g_currentDateTime.dayOfWeek;
                break;
            case 4:
                value = g_currentDateTime.hours;
                break;
            case 5:
                value = g_currentDateTime.minutes;
                break;
            case 6:
                value = g_playerStats.cash;
                break;
            default:
                break;
            }
            stack.push_back({1, value});
            break;
        }
        case 0xc0:
        case 0xe0: {
            if (!ScenCanRead(object, ip, 1))
                return nullptr;
            const std::uint16_t operand = static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(*ip++) | ((opcode & 0xfU) << 8U));
            std::int32_t value = (opcode & 0xe0U) == 0xc0 ? ScenCycleContextValue(object, operand) : PlayerInsideScenSpecialZone(operand);
            if ((opcode & 0xe0U) == 0xc0 && g_debug_talk_status >= 0)
                value = g_debug_talk_status;
            stack.push_back({static_cast<std::uint8_t>((opcode & 0xe0U) == 0xc0 ? 5 : 6), value});
            break;
        }
        default:
            return nullptr;
        }
    }
    return nullptr;
}

std::byte *ResolveScenBranchPointer(ScenRuntimeObject &object) {
    if (!object.scriptStart)
        return nullptr;
    std::byte *ip = nullptr;
    const std::uint8_t state = object.runtimeState;
    const std::size_t slot = ScenWordSlotIndex(object);
    if ((object.runtimeFlags & 2U) != 0 || state == 6) {
        ip = object.instructionPtr;
    } else if ((ScenFlags() & 0x40U) != 0 && slot < g_scntWord20.size() && g_scntWord20[slot]) {
        ScenFlags() &= ~0x40U;
        ip = object.scriptStart + g_scntWord20[slot];
    } else if (state) {
        if (state >= 4) {
            ip = object.instructionPtr;
        } else if (state == 1 && slot < g_scntWord20.size()) {
            const std::uint16_t word24 = g_scntWord24[slot];
            const std::uint16_t word26 = g_scntWord26[slot];
            if (word24 <= word26) {
                const std::uint16_t word22 = g_scntWord22[slot];
                if (word22 <= word26) {
                    ip = object.scriptStart + word26;
                } else {
                    object.instructionPtr = object.scriptStart + word22;
                    object.runtimeState = 5;
                    return nullptr;
                }
            } else {
                ip = object.scriptStart + (std::max)(0, static_cast<int>(word24) - 2);
            }
        } else {
            ip = object.scriptStart;
        }
    } else if ((ScenFlags() & kScenarioFlagActiveRuntimeObject) == 0 || object.field28 == -1) {
        ip = object.scriptStart;
    } else {
        std::byte *cursor = object.scriptStart;
        for (std::int32_t branch = 0; branch < 4096; ++branch) {
            std::uint8_t opcode = 0;
            do {
                if (!ScenCanRead(object, cursor, 1))
                    return nullptr;
                opcode = std::to_integer<std::uint8_t>(*cursor++);
            } while ((opcode & 0xf0U) != 0x10U);
            if (!ScenCanRead(object, cursor, 1))
                return nullptr;
            const std::uint32_t offset = std::to_integer<std::uint8_t>(*cursor) | ((opcode & 0xfU) << 8U);
            std::byte *const next = cursor + 1;
            if (branch == object.field28) {
                ip = next - 2;
                break;
            }
            cursor = ScenAddRelative(object, next, static_cast<std::int32_t>(offset));
            if (!cursor)
                return nullptr;
        }
    }
    if (!ip || !ScenCanRead(object, ip, 0))
        return nullptr;
    const std::uint32_t flags = object.runtimeFlags;
    if ((flags & 0xcU) != 0) {
        if ((flags & 4U) != 0 && object.field24)
            object.field24 = 1;
        if ((flags & 8U) != 0) {
            const auto match = std::find(g_scenRuntimeState.actionSelectorBranchTokens.begin(), g_scenRuntimeState.actionSelectorBranchTokens.end(),
                static_cast<std::int16_t>(object.field24));
            object.field24 = match == g_scenRuntimeState.actionSelectorBranchTokens.end() ?
                0 :
                static_cast<std::uint16_t>(match - g_scenRuntimeState.actionSelectorBranchTokens.begin() + 1);
        }
        object.runtimeFlags = flags & 0xfffffff3U;
        ip += 2U * object.field24;
    }
    return ScenCanRead(object, ip, 0) ? ip : nullptr;
}



void IncrementInventoryItemCountAndTrackStep(std::uint16_t itemId) {
    std::uint8_t &count = g_playerStats.pages.charaItemCounts[itemId];
    if (count != 0xff)
        ++count;
    std::int16_t index = 0;
    while (index < g_playerStats.inventoryItemCount && g_playerStats.inventoryItemIds[static_cast<std::size_t>(index)] != itemId)
        ++index;
    if (index >= g_playerStats.inventoryItemCount && g_playerStats.inventoryItemCount < 512)
        g_playerStats.inventoryItemIds[static_cast<std::size_t>(g_playerStats.inventoryItemCount++)] = static_cast<std::int16_t>(itemId);
    const std::int16_t key = static_cast<std::int16_t>(itemId);
    if (g_playerStats.recentItemKeyOldest == key || g_playerStats.recentItemKeyPrevious == key || g_playerStats.recentItemKeyLatest == key ||
        g_playerStats.scenarioStepUniqueItemCount >= 32000)
        return;
    g_playerStats.recentItemKeyOldest = g_playerStats.recentItemKeyPrevious;
    g_playerStats.recentItemKeyPrevious = g_playerStats.recentItemKeyLatest;
    g_playerStats.recentItemKeyLatest = key;
    ++g_playerStats.scenarioStepUniqueItemCount;
}

std::byte *ScanScenConditionalFlow(ScenRuntimeObject &object, std::byte *ip) {
    for (std::uint32_t guard = 0; guard < 2048; ++guard) {
        if (!ScenCanRead(object, ip, 1))
            return nullptr;
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(*ip++);
        if (opcode < 0x80U) {
            if (!ScenCanRead(object, ip, 1))
                return nullptr;
            const std::uint32_t value = std::to_integer<std::uint8_t>(*ip++) | ((opcode & 0xfU) << 8U);
            if ((opcode & 0xf0U) == 0x10U)
                return ip;
            if ((opcode & 0xf0U) == 0x40U && g_scenRuntimeState.conditionResult)
                continue;
            if ((opcode & 0xf0U) == 0x40U || (opcode & 0xf0U) == 0x50U) {
                ip = ScenAddRelative(object, ip, ScenSignExtend(value, 0x800U));
                if (!ip)
                    return nullptr;
                continue;
            }
            if ((opcode & 0xf0U) != 0x00U)
                return nullptr;
            continue;
        }
        if (opcode < 0xe0U) {
            if ((opcode & 0xf0U) != 0xd0U || !ScenCanRead(object, ip, 2))
                return nullptr;
            const std::uint32_t value = std::to_integer<std::uint8_t>(ip[1]) | ((std::to_integer<std::uint8_t>(ip[0]) + ((opcode & 0xfU) << 8U)) << 8U);
            ip += 2;
            ip = ScenAddRelative(object, ip, ScenSignExtend(value, 0x80000U));
            if (!ip)
                return nullptr;
            continue;
        }
        if (opcode == 0xf0U) {
            ip = DecodeScenExpression(ip, object);
            if (!ip)
                return nullptr;
            continue;
        }
        if (opcode == 0xf1U)
            continue;
        return nullptr;
    }
    return nullptr;
}

std::byte *DecodeScenLowOpcode(ScenRuntimeObject &object, std::byte *ip, bool &stop) {
    if (!ScenCanRead(object, ip, 2))
        return nullptr;
    const std::uint8_t opcode = std::to_integer<std::uint8_t>(ip[0]);
    const std::uint32_t index = std::to_integer<std::uint8_t>(ip[1]) | ((opcode & 0xfU) << 8U);
    std::byte *next = ip + 2;
    const std::size_t slot = ScenWordSlotIndex(object);
    switch (opcode & 0xf0U) {
    case 0x10:
        if (object.scriptSlot < 301U && (object.runtimeFlags & 0x302U) == 0 && slot < g_scntWord20.size()) {
            if (!object.relocatedPtrA8) {
                const std::byte *const saved = object.scriptStart + g_scntWord20[slot];
                if (next == saved) {
                    if (g_scntWord26[slot])
                        next = object.scriptStart + g_scntWord26[slot];
                    const std::byte *const word24 = object.scriptStart + g_scntWord24[slot];
                    if (next < word24)
                        next = const_cast<std::byte *>(word24 - 2);
                    const std::byte *const word22 = object.scriptStart + g_scntWord22[slot];
                    if (next < word22) {
                        object.runtimeState = 5;
                        stop = true;
                        return const_cast<std::byte *>(word22);
                    }
                } else {
                    const auto offset = next - object.scriptStart;
                    if (offset >= 0 && offset <= std::numeric_limits<std::uint16_t>::max())
                        g_scntWord26[slot] = static_cast<std::uint16_t>(offset);
                    g_scntWord24[slot] = 0;
                }
            }
            g_scntWord22[slot] = 0;
        }
        return next;
    case 0x20: {


        object.activeLineOrMarker = static_cast<ScenLineEntry *>(object.lineTable) + index;
        std::byte *const markerEnd = BuildScenConversationMarkers(object, ip);
        for (std::size_t referenced = 1; g_scenRuntimeState.referencedCharacterIds[referenced]; ++referenced) {
            const std::uint32_t name = g_scenRuntimeState.referencedCharacterIds[referenced];
            if (name == object.name.name) {
                IncrementInventoryItemCountAndTrackStep(object.scriptSlot);
                continue;
            }
            std::uint16_t characterSlot = 0;
            while (characterSlot < kValidScenCharacterNames.size() && kValidScenCharacterNames[characterSlot] != name)
                ++characterSlot;
            if (characterSlot != 0 && characterSlot < kValidScenCharacterNames.size())
                IncrementInventoryItemCountAndTrackStep(characterSlot);
        }
        object.runtimeFlags |= 0x5040U;
        object.runtimeState = 4;
        object.field78 = nullptr;
        ScenFlags() &= ~kScenarioFlagSubtitleLineActive;
        g_scntFrameTimer = 15;
        stop = true;
        return markerEnd;
    }
    case 0x30:
        if (index == 16)
            g_bGlobalFlag = 1;
        else {
            SCNF_LogStateChange(object, static_cast<std::int32_t>(index));
            object.runtimeFlags |= kRuntimeFlagPendingWait;
            object.waitCount = 0;
            ScenFlags() &= ~kScenarioFlagConversationCamera;
            object.runtimeState = 7;
            stop = true;
        }
        return next;
    case 0x40:
        if (g_scenRuntimeState.conditionResult)
            return next;
        [[fallthrough]];
    case 0x50:
        return ScenAddRelative(object, next, ScenSignExtend(index, 0x800U));
    case 0x60:
        SetStatOrNotebookFlag(static_cast<std::uint16_t>(index), true);
        return next;
    case 0x70:
        SetStatOrNotebookFlag(static_cast<std::uint16_t>(index), false);
        return next;
    default:
        return next;
    }
}

std::byte *DecodeScenMiddleOpcode(ScenRuntimeObject &object, std::byte *ip) {
    if (!ScenCanRead(object, ip, 1))
        return nullptr;
    const std::uint8_t opcode = std::to_integer<std::uint8_t>(*ip);
    switch (opcode & 0xf0U) {
    case 0x80: {
        if (!ScenCanRead(object, ip, 2))
            return nullptr;
        const std::uint32_t value = std::to_integer<std::uint8_t>(ip[1]) | ((opcode & 0xfU) << 8U);
        std::byte *const base = ip + 2;
        object.alternateInstructionPtr = base;
        return ScenAddRelative(object, base, ScenSignExtend(value, 0x800U));
    }
    case 0x90: {
        if (!ScenCanRead(object, ip, 2))
            return nullptr;
        std::byte *const next = ip + 2;
        if ((object.runtimeFlags & 0x302U) == 0 && object.scriptSlot < kScntScriptSlotCount) {
            const std::size_t slot = ScenWordSlotIndex(object);
            const auto offset = next - object.scriptStart;
            if (slot < g_scntWord24.size() && offset >= 0 && offset <= std::numeric_limits<std::uint16_t>::max()) {
                g_scntWord24[slot] = static_cast<std::uint16_t>(offset);
                object.relocatedPtrA8 = nullptr;
            }
        }
        object.runtimeFlags |= 0x800000U;
        return next;
    }
    case 0xc0:
        if (!ScenCanRead(object, ip, 2))
            return nullptr;
        switch (opcode & 0xfU) {
        case 1:
            if (g_scenarioFunctionWork)
                g_scenarioFunctionWork->scntVariable18 = std::to_integer<std::uint8_t>(ip[1]);
            break;
        case 2:
            if (g_scenarioFunctionWork)
                g_scenarioFunctionWork->scntVariable19 = std::to_integer<std::uint8_t>(ip[1]);
            break;
        case 3:
            object.byte0f = std::to_integer<std::uint8_t>(ip[1]);
            break;
        case 4:
            object.byte10 = std::to_integer<std::uint8_t>(ip[1]);
            break;
        case 5:
            object.byte11 = std::to_integer<std::uint8_t>(ip[1]);
            break;
        case 6:
            object.byte12 = std::to_integer<std::uint8_t>(ip[1]);
            break;
        case 7:
            object.field14 = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(ip[1]));
            break;
        default:
            break;
        }
        return ip + 2;
    case 0xd0: {
        if (!ScenCanRead(object, ip, 3))
            return nullptr;
        const std::uint32_t value = std::to_integer<std::uint8_t>(ip[2]) | ((std::to_integer<std::uint8_t>(ip[1]) + ((opcode & 0xfU) << 8U)) << 8U);
        return ScenAddRelative(object, ip + 3, ScenSignExtend(value, 0x80000U));
    }
    default:
        return ip + 1;
    }
}

std::byte *DecodeScenBranchOpcode(ScenRuntimeObject &object, std::byte *ip, bool &stop) {
    if (!ScenCanRead(object, ip, 2))
        return nullptr;
    const std::uint8_t opcode = std::to_integer<std::uint8_t>(ip[0]);
    const std::uint8_t count = std::to_integer<std::uint8_t>(ip[1]);
    std::byte *cursor = ip + 2;
    const std::uint8_t mode = opcode - 0xe0U;
    if (mode == 0) {
        if (!count)
            return cursor;
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        std::int32_t branch = static_cast<std::int32_t>(static_cast<float>((g_EnvPrngSeed >> 16U) & 0x7fffU) * (1.0F / 32768.0F) * count);
        if (g_scenRuntimeState.randomBranchCount < 3) {
            const std::size_t index = static_cast<std::size_t>(g_scenRuntimeState.randomBranchCount);
            if (g_scenRuntimeState.randomBranchValues[index] == branch)
                branch = branch ? branch - 1 : count - 1;
            g_scenRuntimeState.randomBranchValues[index] = branch;
            ++g_scenRuntimeState.randomBranchCount;
        }
        while (branch-- > 0) {
            if (!ScenCanRead(object, cursor, 2))
                return nullptr;
            const std::uint32_t offset = std::to_integer<std::uint8_t>(cursor[1]) | ((std::to_integer<std::uint8_t>(cursor[0]) & 0xfU) << 8U);
            cursor = ScenAddRelative(object, cursor + 2, static_cast<std::int32_t>(offset));
            if (!cursor)
                return nullptr;
        }
        return cursor;
    }
    if (mode == 1) {
        if (!ScenCanRead(object, cursor, 9))
            return nullptr;
        object.gap26 = 600;
        for (std::size_t index = 0; index < 4; ++index) {
            const std::uint16_t high = std::to_integer<std::uint8_t>(cursor[1 + 2 * index]);
            const std::uint16_t low = std::to_integer<std::uint8_t>(cursor[2 + 2 * index]);
            g_scenRuntimeState.actionSelectorBranchTokens[index] = static_cast<std::int16_t>((high << 8U) | low);
        }
        UI_ICSpRequest({g_scenRuntimeState.actionSelectorBranchTokens[0], g_scenRuntimeState.actionSelectorBranchTokens[1],
            g_scenRuntimeState.actionSelectorBranchTokens[2], g_scenRuntimeState.actionSelectorBranchTokens[3], 600, -1, 0});
        object.runtimeFlags |= 0x18U;
        object.runtimeState = 6;
        stop = true;
        return cursor + 9;
    }
    if (mode == 2) {
        UI_ICSpRequest({-1, 0, 1, -1, 0x7fffU, -1, 3});
        object.runtimeFlags |= 0x14U;
        object.runtimeState = 6;
        stop = true;
        return cursor;
    }
    if (mode == 3)
        return cursor;
    if (mode == 4) {
        if (!ScenCanRead(object, cursor, count))
            return nullptr;
        const auto *const participants = static_cast<const std::uint32_t *>(object.participants);
        const std::int32_t participant = (std::to_integer<std::int32_t>(cursor[0]) << 8) | std::to_integer<std::int32_t>(cursor[1]);
        const std::uint32_t id = participants ? participants[participant - 'A'] : 0;
        if (CHID_Find(g_scenarioFunctionWork->scenarioTask, id)) {
            const std::int32_t location = (static_cast<std::int8_t>(cursor[2]) << 8) | std::to_integer<std::int32_t>(cursor[3]);
            SCNF_EnqueueLocationEvent(object,
                location < 0 ? 0 : static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(object.locations)) + static_cast<std::uint32_t>(location) * 12U);
        }
        ScenFlags() &= ~kScenarioFlagConversationCamera;
        ScenFlags() |= kScenarioFlagInteractionResume;
        object.runtimeFlags |= kRuntimeFlagPendingWait;
        object.waitCount = 0;
        object.runtimeState = 7;
        stop = true;
    }
    return ScenAddRelative(object, cursor, static_cast<std::int32_t>(count));
}

std::byte *DecodeScenHighOpcode(ScenRuntimeObject &object, std::byte *ip, bool &stop) {
    if (!ScenCanRead(object, ip, 1))
        return nullptr;
    const std::uint8_t opcode = std::to_integer<std::uint8_t>(*ip);
    switch (opcode) {
    case 0xf0:
        return DecodeScenExpression(ip + 1, object);
    case 0xf2:
        if (ScenCanRead(object, ip, 5) && (std::to_integer<std::uint8_t>(ip[4]) & 0xf0U) == 0x90U)
            return ip + 4;
        if ((object.runtimeFlags & 0x302U) == 0 && object.scriptSlot < kScntScriptSlotCount) {
            const std::size_t slot = ScenWordSlotIndex(object);
            const auto offset = ip + 1 - object.scriptStart;
            if (slot < g_scntWord22.size() && offset >= 0 && offset <= std::numeric_limits<std::uint16_t>::max())
                g_scntWord22[slot] = static_cast<std::uint16_t>(offset);
        }
        if (g_areaId == AREA_MFSY.name && SCEN_HasActiveScript())
            g_bGlobalFlag = 1;
        object.runtimeState = 5;
        stop = true;
        return ip + 1;
    case 0xf3:
        if (!ScenInteractionBlocked() || (object.runtimeFlags & 0x20U) != 0) {
            object.runtimeState = 3;
            stop = true;
        } else {
            if (g_areaId != AREA_MFSY.name)
                g_bGlobalFlag = 1;
            BeginScenInteraction(object);
            object.runtimeFlags |= 0x400020U;
        }
        return ip + 1;
    case 0xf5: {
        if (!ScenCanRead(object, ip, 4))
            return nullptr;
        const std::uint32_t value =
            std::to_integer<std::uint8_t>(ip[3]) | ((std::to_integer<std::uint8_t>(ip[2]) + (std::to_integer<std::uint8_t>(ip[1]) << 8U)) << 8U);
        object.relocatedPtrA8 = ScenAddRelative(object, ip + 4, ScenSignExtend(value, 0x800000U));
        return ip + 4;
    }
    case 0xf6: {
        std::byte *next = ip + 1;
        if (object.scriptSlot >= kScntScriptSlotCount || (object.runtimeFlags & 0x2000U) != 0) {
            SCNF_LogStateChange(object, -3);
            object.runtimeState = 1;
        } else {
            object.runtimeFlags |= 0x2000U;
            SCNF_LogStateChange(object, -5);
            next = object.scriptStart;
            object.instructionPtr = next;
            object.runtimeState = 3;
        }
        object.runtimeFlags = (object.runtimeFlags & 0xff7ffd7fU) | 0x800000U;
        stop = true;
        return next;
    }
    case 0xf7:
        SCNF_LogStateChange(object, -4);
        object.runtimeFlags &= 0xfffffd7fU;
        object.runtimeState = 2;
        stop = true;
        return ip + 1;
    case 0xf8:
        SCNF_LogStateChange(object, -5);
        object.instructionPtr = object.scriptStart;
        object.runtimeFlags &= 0xfffffd7fU;
        object.runtimeState = 3;
        stop = true;
        return object.scriptStart;
    case 0xf9:
        if ((object.runtimeFlags & 2U) != 0) {
            ScenFlags() &= ~kScenarioFlagInteractionModal;
            std::byte *const saved = object.savedInstructionPtr ? object.savedInstructionPtr : ip + 1;
            object.runtimeFlags &= 0xffffff7dU;
            object.savedInstructionPtr = nullptr;
            object.runtimeState = 5;
            stop = true;
            return saved;
        }
        if ((object.runtimeFlags & 0x100U) != 0) {
            std::byte *const saved = object.savedInstructionPtr ? object.savedInstructionPtr : ip + 1;
            object.runtimeFlags &= 0xfffffeffU;
            object.savedInstructionPtr = nullptr;
            return saved;
        }
        if (object.alternateInstructionPtr) {
            std::byte *const alternate = object.alternateInstructionPtr;
            object.alternateInstructionPtr = nullptr;
            return alternate;
        }
        SCNF_LogStateChange(object, -3);
        object.runtimeFlags &= ~640U;
        object.runtimeState = 1;
        stop = true;
        return ip + 1;
    case 0xfc:
        ScenFlags() &= ~kScenarioFlagConversationCamera;
        object.runtimeFlags |= kRuntimeFlagPendingWait;
        object.runtimeState = 6;
        object.waitCount = 0;
        stop = true;
        return ip + 1;
    case 0xfd:
        object.runtimeFlags |= 0x4000U;
        object.field78 = nullptr;
        ScenFlags() &= ~0x1000U;
        return ip + 1;
    case 0xff:
        object.runtimeFlags &= 0xfffffd7fU;
        object.runtimeState = 3;
        stop = true;
        return ip + 1;
    default:
        return ip + 1;
    }
}

bool RunScenBytecode(ScenRuntimeObject &object) {
    if (!object.name.name) {
        SetScenActiveObjectName(0);
        SetScenPreviousObjectName(0);
        return false;
    }
    std::byte *ip = ResolveScenBranchPointer(object);
    if (!ip && object.runtimeState == 5)
        return true;
    if (!ip)
        return false;
    bool stop = false;
    for (std::uint32_t guard = 0; !stop && guard < 4096; ++guard) {
        if (!ScenCanRead(object, ip, 1))
            return false;
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(*ip);
        if (opcode >= 0xe0U)
            ip = opcode >= 0xf0U ? DecodeScenHighOpcode(object, ip, stop) : DecodeScenBranchOpcode(object, ip, stop);
        else if (opcode >= 0x80U)
            ip = DecodeScenMiddleOpcode(object, ip);
        else
            ip = DecodeScenLowOpcode(object, ip, stop);
        if (!ip)
            return false;
    }
    if (!stop)
        return false;
    object.instructionPtr = ip;
    if (object.runtimeState == 1)
        return true;
    if (object.runtimeState == 4) {
        if ((object.runtimeFlags & kRuntimeFlagStateTransitionLatch) == 0) {
            object.runtimeFlags |= kRuntimeFlagStateTransitionLatch;
            if ((object.runtimeFlags & 2U) == 0) {
                object.failCount1 = 0;
                object.runtimeFlags |= kRuntimeFlagPendingFailure1;
                SCNF_LogStateChange(object, -1);
                ScenFlags() |= kScenarioFlagAuthorGameState;
                CaptureScenPadContext();
            }
            if (g_scenarioFunctionWork) {
                g_scenarioFunctionWork->field20 = nullptr;
                g_scenarioFunctionWork->field28 = nullptr;
            }
        }
    } else if (object.runtimeState == 5) {
        if ((object.runtimeFlags & kRuntimeFlagStateTransitionLatch) != 0) {
            object.failCount3 = 0;
            object.runtimeFlags = (object.runtimeFlags & 0xfff7dfffU) | kRuntimeFlagPendingFailure3;
            SCNF_LogStateChange(object, -2);
            RestoreScenPadContext();
        }
    } else if (object.runtimeState < 6 || object.runtimeState > 8) {
        if ((object.runtimeFlags & kRuntimeFlagStateTransitionLatch) != 0) {
            object.failCount3 = 0;
            object.runtimeFlags = (object.runtimeFlags & 0xfff7dfffU) | kRuntimeFlagPendingFailure3;
            SCNF_LogStateChange(object, -2);
            object.runtimeFlags |= kRuntimeFlagPendingFailure2;
            object.failCount2 = 0;
            SCNF_LogStateChange(object, -8);
            ClearScenAuthorGameState();
            RestoreScenPadContext();
        }
    }
    return true;
}

} // namespace shenmue::s1::scen_detail
