#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_info.hpp"

namespace shenmue {
namespace s2 {

struct AuthorTableEntry {
    std::byte *file;
    const MapInfoChunk *track;
    const MapInfoChunk *view;
    std::int32_t fileOwned;
    shen_name name;
};
typedef char Layout_author_table_dc_0[(sizeof(AuthorTableEntry) == 0x14) ? 1 : -1];
typedef char Layout_author_table_dc_1[(offsetof(AuthorTableEntry, track) == 4) ? 1 : -1];
typedef char Layout_author_table_dc_2[(offsetof(AuthorTableEntry, name) == 0x10) ? 1 : -1];

enum { AUTHOR_TRACK_COUNT = 0, AUTHOR_TRACK_FIRST = 1 };
struct AuthorTrackSlots {
    std::uint8_t slots[28];
};
struct AuthorPackageRecord {
    const void *package;
    AuthorTrackSlots tracks;
};
typedef char Layout_author_table_dc_3[(sizeof(AuthorPackageRecord) == 0x20) ? 1 : -1];
typedef char Layout_author_table_dc_4[(offsetof(AuthorPackageRecord, tracks) == 4) ? 1 : -1];

extern AuthorTableEntry authorTable[70];
extern AuthorPackageRecord authorPackageRecords[6];
void RegisterAuthorTracks(const MapInfoChunk *bundle, std::uint8_t *slot);
void ClearAuthorTableEntry(std::int32_t index);
void ClearAuthorTable();
void ClearAuthorPackageRecord(std::int32_t index);
void ClearAuthorPackageRecords();
void SCEX_Reader(const MapInfoChunk *bundle);
std::int32_t ReadAUTH(const std::byte *bundle, const void *package);
std::int32_t FreeAUTH(const void *package);

} // namespace s2
} // namespace shenmue

#endif
