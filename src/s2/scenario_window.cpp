#include "s2/scenario_window.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {

void *AllocateSCNW(HLib::Task *task) {
    auto *const record = static_cast<HLib::Task **>(HLib::AllocateTaskData(0x18, SHEN_NAME(SCNW)));
    record[1] = nullptr;
    record[2] = nullptr;
    record[0] = task;
    g_pSCNW = &scenarioWindowRecord;
    scenarioWindowRecord = record;
    return record;
}

// Characters that may not open a line, bucketed by JIS code modulo 7.
constexpr std::array<std::array<std::uint16_t, 6>, 7> scenarioLineStartForbidden{{
    {0x2139, 0x2465, 0x2529, 0x2576, 0, 0},
    {0x2427, 0x2443, 0x2523, 0, 0, 0},
    {0x2421, 0x2467, 0x2563, 0x2126, 0, 0},
    {0x2429, 0x2525, 0x213c, 0, 0, 0},
    {0x2423, 0x2565, 0x2144, 0x213d, 0, 0},
    {0x2122, 0x2463, 0x2527, 0x2543, 0x2129, 0},
    {0x2123, 0x2425, 0x2521, 0x2567, 0x212a, 0},
}};

std::uint16_t ScenarioWindowJisCode(std::uint32_t legacyEncodingMode, std::uint16_t character) {
    return legacyEncodingMode ? static_cast<std::uint16_t>(character & 0x7f7fU) : ConvertWindowShiftJisCharacter(character);
}

bool IsScenarioLineStartForbidden(std::uint32_t legacyEncodingMode, std::uint16_t character) {
    const std::uint16_t code = ScenarioWindowJisCode(legacyEncodingMode, character);
    for (const std::uint16_t candidate : scenarioLineStartForbidden[code % 7U]) {
        if (!candidate)
            return false;
        if (candidate == code)
            return true;
    }
    return false;
}



std::int32_t AdvanceScenarioTextDecoder(ScenarioTextDecoder &decoder) {
    decoder.currentCharacter = *decoder.cursor;
    for (;;) {
        if (decoder.currentCharacter == 0) {
            decoder.currentCharacterWidth = 0;
            return 0;
        }
        if (decoder.pendingPrefix != 0 && decoder.currentCharacter == 127) {
            decoder.pendingPrefix = 0;
            decoder.currentCharacter = *++decoder.cursor;
            continue;
        }
        if (decoder.pendingPrefix != 0 && decoder.currentCharacter < 253) {
            decoder.currentCharacter |= decoder.pendingPrefix;
            ++decoder.cursor;
            decoder.currentCharacterWidth = 2;
            return 1;
        }
        if (decoder.legacyEncodingMode == 0 && decoder.currentCharacter >= 253) {
            decoder.pendingPrefix = static_cast<std::uint16_t>((decoder.currentCharacter - 124) << 8);
            decoder.currentCharacter = *++decoder.cursor;
            continue;
        }
        break;
    }
    if (decoder.isLeadByte(static_cast<std::uint8_t>(decoder.currentCharacter)) && decoder.isTrailByte(decoder.cursor[1])) {
        decoder.currentCharacter = static_cast<std::uint16_t>((decoder.currentCharacter << 8) | decoder.cursor[1]);
        decoder.cursor += 2;
        decoder.currentCharacterWidth = 2;
        return 1;
    }
    decoder.currentCharacterWidth = decoder.currentCharacter != '\n' ? 1U : 0U;
    ++decoder.cursor;
    return 1;
}


// open a line. The !| escape takes !| (a literal bar), @D/@V (colour modes 1/0, which end the page without voice) and

std::int32_t SCNF_RenderWindowText(WindowSlot *window, ScenarioWindowTextState *state) {
    ScenarioTextDecoder &decoder = state->decoder;
    std::uint16_t escape = 0;
    std::int32_t result = 0;
    do {
        bool write = true;
        if (decoder.currentCharacter != 0) {
            std::int32_t column = 0;
            std::int32_t row = 0;
            GetWindowWritePosition(window, &column, &row);
            const std::int32_t lastColumn = state->columnCount - 2;
            if (column > lastColumn ||
                (column == lastColumn &&
                    (decoder.currentCharacterWidth != 2 || !IsScenarioLineStartForbidden(decoder.legacyEncodingMode, decoder.currentCharacter))))
                AdvanceWindowLine(window);
            if (decoder.currentCharacterWidth == 2) {
                const std::uint16_t code = ScenarioWindowJisCode(decoder.legacyEncodingMode, decoder.currentCharacter);
                if (code == 0x2175) {
                    GetWindowWritePosition(window, &column, &row);
                    if (row == 0 || column != 0)
                        AdvanceWindowLine(window);
                    write = false;
                } else if (static_cast<std::uint16_t>(code - 0x2156) <= 1) {
                    write = false;
                } else {
                    const std::uint16_t special = code == 0x217c || code == 0x4044 || code == 0x4056 || code == 0x4e50 ? code : 0;
                    if (special == 0) {
                        if (escape == 0x217c)
                            write = false;
                        escape = 0;
                    } else if (special == 0x217c) {
                        if (escape != 0x217c) {
                            escape = special;
                            write = false;
                        } else
                            escape = 0;
                    } else if (escape != 0x217c) {
                        escape = 0;
                    } else if (special == 0x4e50) {
                        SetWindowColourMode(window, 3);
                        escape = 0;
                        write = false;
                    } else {
                        SetWindowColourMode(window, special == 0x4044 ? 1 : 0);
                        escape = 0;
                        result = GetVoiceSettingEnabled() ? 1 : 0;
                        if (!result)
                            break;
                        write = false;
                    }
                }
            }
        }
        if (write) {
            result = WriteWindowCharacter(window, decoder.currentCharacter, static_cast<std::int32_t>(decoder.legacyEncodingMode));
            if (result & 0x8000)
                return result;
        }
        result = AdvanceScenarioTextDecoder(decoder);
    } while (result != 0);
    state->scenarioStateFlags |= 1U;
    return result;
}



void SCNF_Handler(WindowSlot *window, WindowCallbackEvent event, void *payload) {
    auto *const state = static_cast<ScenarioWindowTextState *>(payload);
    if (event != WindowCallbackEvent::Tick)
        state->lastWindowEvent = static_cast<std::int32_t>(event);
    if (!state->callbackEnabled || !(state->scenarioStateFlags & 0x80U))
        return;
    if (event == WindowCallbackEvent::Tick) {
        const std::int32_t frames = state->surfaceRecoveryFrames;
        if (frames > 0 && !state->surfaceReady) {
            state->surfaceRecoveryFrames = frames + 1;
            if (frames > 4) {
                RecoverWindowSurface(window);
                state->surfaceRecoveryFrames = 0;
                state->surfaceReady = 1;
            }
        }
    } else if (event == WindowCallbackEvent::GlyphRevealComplete) {
        if (state->scenarioStateFlags & 8U) {
            ShowWindowSlot(window);
            state->scenarioStateFlags ^= 8U;
        } else
            state->scenarioStateFlags |= 4U;
    } else if (event == WindowCallbackEvent::PageRotationComplete) {
        if (state->scenarioStateFlags & 1U)
            state->scenarioStateFlags |= 2U;
        else {
            ClearWindowText(window);
            SCNF_RenderWindowText(window, state);
            RevealWindowGlyphs(window);
        }
    }
}
} // namespace event_runtime_detail


// FTBL 18/19 retain the scenario UI owner rather than a host debug logger.
std::uint64_t dbgPrintEventMessage(const char *message) {
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return SCEN_ShowEventMessage(control->scenarioTask, 1, message);
}

void ctrlStopSubtitles() {
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    scenDestroySubtitles(control->scenarioTask);
}

} // namespace shenmue::s2
