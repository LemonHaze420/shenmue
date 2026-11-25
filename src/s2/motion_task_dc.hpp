#pragma once

#include "s2/hlib.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#define TASK_MOTF SHEN_NAME4('M', 'O', 'T', 'F')
void MotFileManageTaskEntry_Callback(void *data);
void Queue_mot_Callback_MotFileManageTaskEntry(std::uint8_t nextFunctionIndex);

} } // namespace shenmue::s2

#endif
