#pragma once

#include "common/archive_data.hpp"

namespace shenmue::s1 {

using archive_data::ArchiveDataLoadResult;
using archive_data::ArchiveData_ClearArchives;
using archive_data::ArchiveData_GetFileSize;
using archive_data::ArchiveData_GetPathHash;
using archive_data::ArchiveData_HasArchives;
using archive_data::ArchiveData_LoadArchives;
using archive_data::ArchiveData_Read;
using archive_data::d3t_GenerateHash;
using archive_data::d3t_HashData;
using archive_data::FilepathCleaner;

} // namespace shenmue::s1
