#pragma once

#include "s2/hlib.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2


namespace shenmue { namespace s2 {

#define TASK_SYSM SHEN_NAME4('S', 'Y', 'S', 'M')
extern char systemManageSource[0x14];
extern std::int32_t systemManageFrames;
void SetSystemManageSource(const char *source);
void CtrlSystemManage_Callback(void *payload);
void initCtrlSystemManage();

} } // namespace shenmue::s2

#endif
