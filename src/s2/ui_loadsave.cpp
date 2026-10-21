#include "s2/ui_loadsave.hpp"
#include "s2/save.hpp"
#include "d3t/ui_runtime.hpp"

#include <bit>
#include <cstdio>

namespace shenmue::s2 {

std::array<SaveDataInfoText, 7> g_saveDataInfoText{};

// @verified - p
SaveDataInfoText *GetSaveDataInfoText(std::uint32_t index) {
    return index < g_saveDataInfoText.size() ? &g_saveDataInfoText[index] : nullptr;
}

// @verified - p
std::int32_t FormatSaveDataInfoByIndex(std::int32_t index, const S2SaveDataInfo *data, std::int32_t source) {
    SaveDataInfoText &text = g_saveDataInfoText.data()[index];
    if (data->saveKind == 0) {
        text.valid = 0;
        return index;
    }
    text.valid = 1;
    std::sprintf(text.gameTime.data(), "%c%d:%d%d", data->gameHour / 10 + '0', data->gameHour % 10, data->gameMinute / 10, data->gameMinute % 10);
    std::sprintf(text.saveTime.data(), "%c%d:%d%d", data->systemHour / 10 + '0', data->systemHour % 10, data->systemMinute / 10, data->systemMinute % 10);
    const std::uint16_t gameYear = static_cast<std::uint16_t>(data->gameYear);
    const char *const gameWeekday = GetStringForUILabel(UI_SAVE_WEEKDAYS.data()[data->gameWeek]);
    std::sprintf(text.gameDate.data(), "%d%d/%d%d/19%d%d (%s)", data->gameDay / 10, data->gameDay % 10, data->gameMonth / 10, data->gameMonth % 10,
        gameYear / 10, gameYear % 10, gameWeekday);
    const char *const saveWeekday = GetStringForUILabel(UI_SAVE_WEEKDAYS.data()[data->gameWeek]);
    std::sprintf(text.saveDate.data(), "%d%d/%d%d/%d%d (%s)", data->systemDay / 10, data->systemDay % 10, data->systemMonth / 10, data->systemMonth % 10,
        data->systemYear / 10, data->systemYear % 10, saveWeekday);
    std::int32_t location = -1, area = -1;
    if (source == 0) {
        area = ui_detail::SaveMenuAreaIndex(data->mapName);
        location = ui_detail::SaveMenuLocationIndex(area);
    }
    const char *const locationToken = source != 0 || location < 0 ? "UISAVES1CL" /* @text UISAVES1CL EN: "SM1 Save" | JA: "Ｓ１セーブデータ" */ :
        location == 5                                             ? UI_SAVE_AREA_TOKENS[0] :
                                                                    UI_SAVE_LOCATION_TOKENS.data()[location];
    const char *const locationText = GetStringForUILabel(locationToken);
    std::snprintf(text.location.data(), text.location.size(), source == 0 && location >= 0 && data->saveKind == 99 ? "DBG: %s" : "%s", locationText);
    const char *const areaText =
        GetStringForUILabel(area < 0 ? "UISAVES1LC" /* @text UISAVES1LC EN: "Cleared Game" | JA: "クリアデータ" */ : UI_SAVE_AREA_TOKENS.data()[area]);
    const std::int32_t result = std::snprintf(text.area.data(), text.area.size(), "%s", areaText);
    return result < 0 ? -1 : result;
}

// @verified - p
std::uint32_t FormatAllSaveDataInfo(std::int32_t first, std::int32_t source) {
    for (std::uint32_t index = 0; index < 5; ++index) {
        const std::uint32_t ordinal = static_cast<std::uint32_t>(first) + index;
        const S2SaveDataInfo *const data = source != 0 ? GetS1BackupSaveDataInfo(ordinal) : GetBackupSaveDataInfo(std::bit_cast<std::int32_t>(ordinal));
        if (data && data->saveKind != 0)
            FormatSaveDataInfoByIndex(static_cast<std::int32_t>(index), data, source);
        else
            g_saveDataInfoText[index].valid = 0;
    }
    return 5;
}

} // namespace shenmue::s2
