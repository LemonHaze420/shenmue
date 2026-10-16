#pragma once
#include "s2/areas/wbbk_tables.hpp"
namespace shenmue::s2::areas {
struct BookModelState {
    shen_name stacks[15], placed[14], created[256];
    std::uint8_t types[14][20];
    std::int32_t createdCount, currentStack, currentBook, animationFrame;
    std::uint8_t displayMode;
    std::int32_t mirror;
};
extern BookModelState bookModelState;
void CreateBookStackModels();
void CreatePutBookModels();
void UpdateBookStackDisplay();
void AttachBookStackToPlayer();
void CreatePlacedBookStack(float x, float z);
void UpdatePlacedBookStack(std::int32_t index, std::int32_t mode);
} // namespace shenmue::s2::areas
