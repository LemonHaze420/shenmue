#pragma once
#include "d3t/threads.hpp"
namespace shenmue::s1 {
using d3t::D3TTaskCallback;
using d3t::D3TTaskCompletion;
using d3t::D3TTaskDescriptor;
using d3t::D3TTaskRecord;
using d3t::d3t_CreateThreads;
using d3t::d3t_IsTaskActive;
using d3t::d3t_QueueTask;
using d3t::RunTask;
using d3t::ExecuteADynamicFunctionOnce;
using d3t::g_d3tDeferredCompletionCount;
using d3t::g_d3tLowPriorityThreadFlag;
using d3t::g_d3tShutdownThreadFlag;
using d3t::g_d3tTaskCount;
using d3t::g_d3tTasks;
using d3t::g_d3tThreadsInitialized;
using d3t::ShutdownThreads;
}
