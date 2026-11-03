#pragma once

#include <cstdint>

namespace shenmue::s1 {

struct TrialNotebookLine {
    std::int16_t page;
    std::int16_t flag;
    const char *text;
};

extern const TrialNotebookLine kNotebookFullLines[];
extern const TrialNotebookLine kNotebookTrialLines[];
extern const TrialNotebookLine kNotebookExtendedLines[];

} // namespace shenmue::s1
