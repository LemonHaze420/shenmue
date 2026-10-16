#include "s1/task/meve.hpp"
#include "s1/task/ctrl.hpp"

namespace shenmue::s1 {

void MapEventTask_Callback(HLib::TaskData *) {
    MapControl_UpdateMapEvents();
}

} // namespace shenmue::s1
