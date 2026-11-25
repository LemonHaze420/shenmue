#pragma once

#include "s2/hlib.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/input_dc.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct SavedSceneInfo {
    std::int32_t actNumber;
    shen_name mapName;
    std::int32_t entryPoint;
};
typedef char Layout_save_screen_state_dc_0[(sizeof(SavedSceneInfo) == 0x0c) ? 1 : -1];
typedef char Layout_save_screen_state_dc_1[(offsetof(SavedSceneInfo, mapName) == 4) ? 1 : -1];
typedef char Layout_save_screen_state_dc_2[(offsetof(SavedSceneInfo, entryPoint) == 8) ? 1 : -1];
void SetSceneInfos(std::int32_t actNumber, shen_name mapName, std::int32_t entryPoint) ;
void GetSceneInfos(std::int32_t *actNumber, shen_name *mapName, std::int32_t *entryPoint) ;

struct PadControlEntry;
struct SaveGlyphRecord {
    std::uint16_t code;
    std::uint8_t x, y, width, height;
};
struct SaveTaskData {
    std::byte resumeData[0x575];
    std::byte retained575[3];
    const SaveGlyphRecord *glyphTables[3];
    std::int32_t textures[16];
    std::uint8_t choice;
    std::byte retained5c5[3];
    std::int32_t returnToGame, retained5cc;
    PadControlEntry *padData, *previousPadData;
    LegacyInputSlot *inputBuffer;
    HLib::TaskCallback updateCallback, preRenderCallback;
    std::int32_t frame, selectedSlot, discTextures[8];
    std::uint32_t previousHelpFlags;
    std::int32_t previousWarning, screenColour, initializationMode, screenItem, fadeTarget;
    float fadeCurrent;
    std::int32_t screenRow;
};
typedef char SaveGlyphRecordSizeCheck[sizeof(SaveGlyphRecord) == 6 ? 1 : -1];
typedef char SaveGlyphTablesOffsetCheck[offsetof(SaveTaskData, glyphTables) == 0x578 ? 1 : -1];
typedef char SaveTexturesOffsetCheck[offsetof(SaveTaskData, textures) == 0x584 ? 1 : -1];
typedef char SaveChoiceOffsetCheck[offsetof(SaveTaskData, choice) == 0x5c4 ? 1 : -1];
typedef char SaveFrameOffsetCheck[offsetof(SaveTaskData, frame) == 0x5e4 ? 1 : -1];
typedef char SaveDiscTexturesOffsetCheck[offsetof(SaveTaskData, discTextures) == 0x5ec ? 1 : -1];
typedef char SavePreviousHelpOffsetCheck[offsetof(SaveTaskData, previousHelpFlags) == 0x60c ? 1 : -1];
typedef char SavePreviousWarningOffsetCheck[offsetof(SaveTaskData, previousWarning) == 0x610 ? 1 : -1];
typedef char SaveScreenColourOffsetCheck[offsetof(SaveTaskData, screenColour) == 0x614 ? 1 : -1];
typedef char SaveScreenItemOffsetCheck[offsetof(SaveTaskData, screenItem) == 0x61c ? 1 : -1];
typedef char SaveScreenRowOffsetCheck[offsetof(SaveTaskData, screenRow) == 0x628 ? 1 : -1];

typedef char Layout_save_screen_state_dc_3[(sizeof(SaveTaskData) == 0x62c) ? 1 : -1];
typedef char Layout_save_screen_state_dc_4[(offsetof(SaveTaskData, padData) == 0x5d0) ? 1 : -1];
typedef char Layout_save_screen_state_dc_5[(offsetof(SaveTaskData, inputBuffer) == 0x5d8) ? 1 : -1];
typedef char Layout_save_screen_state_dc_6[(offsetof(SaveTaskData, updateCallback) == 0x5dc) ? 1 : -1];
typedef char Layout_save_screen_state_dc_7[(offsetof(SaveTaskData, preRenderCallback) == 0x5e0) ? 1 : -1];
typedef char Layout_save_screen_state_dc_8[(offsetof(SaveTaskData, initializationMode) == 0x618) ? 1 : -1];
typedef char Layout_save_screen_state_dc_9[(offsetof(SaveTaskData, fadeTarget) == 0x620) ? 1 : -1];
typedef char Layout_save_screen_state_dc_10[(offsetof(SaveTaskData, fadeCurrent) == 0x624) ? 1 : -1];
extern LegacyInputSlot saveScreenInput;
void SM_Init_Enqueue(std::int32_t initializationMode);
#else
struct SaveTaskData;
#endif
static const std::size_t kSaveScreenUpdateCallbackOffset = 0x5dc;
extern SaveTaskData *saveTaskData;
extern std::uint32_t currentSaveState;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern std::int32_t g_saveScreenAdmission;
std::int32_t GetSaveScreenAdmission();
void SetSaveScreenAdmission(std::int32_t enabled);
#endif
void SaveScreenUpdate(void *data);
std::uint32_t StartShowScreenOfId(std::uint32_t screenId) ;
std::uint32_t IsSaveAllowed() ;

} } // namespace shenmue::s2

#endif
