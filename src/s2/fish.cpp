#include "s2/fish.hpp"

#include "s2/character.hpp"
#include "s2/maze.hpp"

namespace shenmue::s2 {
namespace {

constexpr Vector3 fishBoxes[] = {
    {-3.5f, -1.7999999523162842f, -4.5f},
    {-1.0f, -1.7999999523162842f, -0.10000000149011612f},
    {-1.0f, -1.7999999523162842f, -4.5f},
    {3.799999952316284f, -1.7999999523162842f, -5.5f},
    {-3.0f, -1.100000023841858f, 4.0f},
    {-6.550000190734863f, -1.100000023841858f, 0.17000000178813934f},
    {-6.550000190734863f, -1.100000023841858f, 0.17000000178813934f},
    {-0.5299999713897705f, -1.100000023841858f, -9.800000190734863f},
};
}

// @verified - p

void FISH_Draw(const MazeAuthoredObject &object, const MazeRoad &context) {
    const std::uint32_t box = object.name.name == SHEN_NAME(HIY1) ? 0U : 4U;
    const Character *const character = FindCharacterByScenarioName(TASK_FISH);
    const MT7Model *const model = character ? static_cast<const MT7Model *>(character->model) : object.modelReader->data;
    const std::int32_t seed = static_cast<std::int32_t>(static_cast<std::uint32_t>(context.unit) + (static_cast<std::uint32_t>(context.block) << 7));
    MAZ_fishCtrlUpdate(&fishBoxes[box], &fishBoxes[box + 1], 9, seed, model);
    MAZ_fishCtrlUpdate(&fishBoxes[box + 2], &fishBoxes[box + 3], 9, seed, model);
}
} // namespace shenmue::s2
