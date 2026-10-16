#include "d3t/archives.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/threads.hpp"

#include <algorithm>
#include <cstring>
#include <mutex>

namespace shenmue::d3t {

std::uint32_t g_archiveReadTask = 0;
D3TArchiveState g_archiveState{};

namespace {

std::mutex g_archiveMutex;

std::int32_t ReadArchivesTask(void *data) {
    return d3t_ReadArchives_Internal(static_cast<D3TArchiveReadContext *>(data));
}

} // namespace

std::int32_t d3t_ReadArchives_Internal(D3TArchiveReadContext *context) {
    if (!context) {
        return 0;
    }

    std::scoped_lock lock(g_archiveMutex);
    D3TArchiveState *const state = context->state ? context->state : &g_archiveState;
    ArchiveData_ClearArchives();
    *state = {};

    ArchiveDataLoadResult loaded{};
    if (!ArchiveData_LoadArchives(context->root, context->backend, context->type, &loaded)) {
        return 0;
    }

    state->type = context->type;
    if (context->root) {
        const std::size_t length = (std::min)(std::strlen(context->root), sizeof(state->root) - 1);
        std::memcpy(state->root, context->root, length);
        state->root[length] = '\0';
        std::replace(state->root, state->root + std::strlen(state->root), '\\', '/');
    }
    state->backend = context->backend;
    state->archiveCount = loaded.archiveCount;
    state->archiveCapacity = loaded.archiveCount;
    return 0;
}

void d3t_ReadArchives(const void *directoryPointer) {
    if (!directoryPointer) {
        return;
    }
    const auto &directory = *static_cast<const D3TArchiveDirectory *>(directoryPointer);
    D3TArchiveReadContext context{};
    std::memcpy(&context, &directory, sizeof(context));

    if (directory.asynchronous == 0) {
        d3t_ReadArchives_Internal(&context);
        return;
    }

    const D3TTaskDescriptor task{
        &context,
        ReadArchivesTask,
        0,
        0,
    };
    g_archiveReadTask = d3t_QueueTask(&task, sizeof(context));
}

} // namespace shenmue::d3t
