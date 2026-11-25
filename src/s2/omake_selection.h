#pragma once

typedef struct OmakeSelection {
    int category, selected[3], scroll[3];
} OmakeSelection;
typedef char OmakeSelectionSizeCheck[sizeof(OmakeSelection) == 0x1c ? 1 : -1];
