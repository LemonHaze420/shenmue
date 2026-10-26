#if defined(SHENMUE_STEAM)
#include "d3t/steam.hpp"
#endif

#include "s2/ui.hpp"
#include "s2/easy_option.hpp"
#include "s2/opening_staff_roll.hpp"
#include "s2/magic_room.hpp"
#include "s2/ctrl_speak_icon.hpp"
#include "s2/now_qte.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/ui_scene.hpp"
#include "s2/ui_grid.hpp"
#include "s2/ui_collection.hpp"
#include "s2/ui_moves.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/ui_prompt.hpp"
#include "s2/input_bindings.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/area_0001.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/omak_collection_ui.hpp"

#include "d3t/content.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "platform/performance_counter.hpp"
#include "s2/input.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/item_wf.hpp"
#include "s2/item_selector.hpp"
#include "s2/main_icon.hpp"
#include "s2/flag.hpp"
#include "s2/item_selector_actions.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/math.hpp"
#include "s2/screen_film.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/conversation.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/scene_data.hpp"
#include "s2/font.hpp"
#include "s2/archive_data.hpp"
#include "s2/pak.hpp"
#include "s2/directory_cache.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/inventory.hpp"
#include "s2/date_time.hpp"
#include "s2/gauge.hpp"
#include "s2/npc_package.hpp"
#include "s2/sound_program.hpp"
#include "s2/navigation.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/author.hpp"
#include "s2/help.hpp"
#include "s2/pause.hpp"
#include "s2/memo.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/display_configuration.hpp"
#include "s2/audio_data.hpp"
#include "s2/hand.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/pawn.hpp"
#include "s2/game_runtime.hpp"
#include <json/json.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <charconv>
#include <cctype>
#include <cmath>
#include <limits>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <stdexcept>
#include <vector>
#include "s2/area_names.hpp"

#include "s2/ui_internal.hpp"

namespace shenmue::s2 {
using namespace ui_detail;

namespace ui_detail {

std::int32_t LoadSnapshotImageTask(void *data) {
    auto *const request = *static_cast<SnapshotImageLoadRequest **>(data);
    if (!request || request->slot >= snapshotImages.size()) {
        delete request;
        return -1;
    }
    std::vector<std::byte> bytes;
#if defined(SHENMUE_STEAM)
    bytes = ::shenmue::d3t::ReadSteamFileBytes(request->path.filename().string().c_str());
#else
    std::ifstream input(request->path, std::ios::binary | std::ios::ate);
    if (input) {
        const std::streamoff end = input.tellg();
        if (end > 0 && static_cast<std::uint64_t>(end) <= static_cast<std::uint64_t>((std::numeric_limits<std::streamsize>::max)()) &&
            static_cast<std::uint64_t>(end) <= static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
            bytes.resize(static_cast<std::size_t>(end));
            input.seekg(0, std::ios::beg);
            input.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            if (!input)
                bytes.clear();
        }
    }
#endif
    const std::uint32_t texture = bytes.empty() ? 0 : d3t::CreateImageTextureFromMemory(bytes.data(), bytes.size(), false);
    SnapshotImageRuntime &runtime = snapshotImages[request->slot];
    if (runtime.generation.load(std::memory_order_acquire) == request->generation) {
        const std::uint32_t previous = runtime.texture.exchange(texture, std::memory_order_acq_rel);
        if (previous)
            d3t::InitTexture2DResource(previous);
        runtime.state.store(texture ? 2U : 3U, std::memory_order_release);
    } else if (texture) {
        d3t::InitTexture2DResource(texture);
    }
    delete request;
    return texture ? 0 : -1;
}

void QueueSnapshotImageLoad(std::uint32_t slot) {
    if (slot >= snapshotImages.size())
        return;
    SnapshotImageRuntime &runtime = snapshotImages[slot];
    const std::uint32_t state = runtime.state.load(std::memory_order_acquire);
    if (state == 1 || state == 2)
        return;
    auto *const request = new (std::nothrow) SnapshotImageLoadRequest{};
    if (!request)
        return;
    request->slot = slot;
    request->generation = runtime.generation.fetch_add(1, std::memory_order_acq_rel) + 1;
    request->path = S2Save_SnapshotPath(slot);
    SnapshotImageLoadRequest *queuedRequest = request;
    const d3t::D3TTaskDescriptor descriptor{&queuedRequest, LoadSnapshotImageTask, nullptr, 2};
    runtime.state.store(1, std::memory_order_release);
    if (!d3t::d3t_QueueTask(&descriptor, sizeof(queuedRequest))) {
        runtime.state.store(3, std::memory_order_release);
        delete request;
    }
}

void RefreshSnapshotOrder() {
    snapshotOrderCount = 0;
    const SnapshotMetadataOwner *const owner = GetSnapshotMetadataOwner();
    for (std::uint32_t slot = 0; slot < owner->records.size(); ++slot) {
        if (owner->records[slot].use != 1)
            continue;
        snapshotOrder[snapshotOrderCount++] = slot;
    }
    const std::int32_t option = GetSnapshotDataOption();
    if (option >= 0 && option < 3) {
        std::sort(snapshotOrder.begin(), snapshotOrder.begin() + snapshotOrderCount, [owner, option](std::uint32_t left, std::uint32_t right) {
            return static_cast<std::uint32_t>(owner->records[left].sortKeys[static_cast<std::size_t>(option)]) <
                static_cast<std::uint32_t>(owner->records[right].sortKeys[static_cast<std::size_t>(option)]);
        });
    }
}

void DeleteSnapshotSlot(std::uint32_t slot) {
    if (slot >= snapshotImages.size())
        return;
    SnapshotMetadataRecord &record = GetSnapshotMetadataOwner()->records[slot];
    record = {};
    record.id = static_cast<std::int32_t>(slot);
    SnapshotImageRuntime &image = snapshotImages[slot];
    image.generation.fetch_add(1, std::memory_order_acq_rel);
    const std::uint32_t texture = image.texture.exchange(0, std::memory_order_acq_rel);
    if (texture)
        d3t::InitTexture2DResource(texture);
    image.state.store(0, std::memory_order_release);
    SaveSnapshotMetadata();
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::DeleteSteamFile(S2Save_SnapshotPath(slot).filename().string().c_str());
#else
    std::error_code error;
    std::filesystem::remove(S2Save_SnapshotPath(slot), error);
#endif
    RefreshSnapshotOrder();
}
// @verified - p
std::int64_t SetWarningMessageParameters(std::int32_t message, std::int32_t number) {
    warningMessageIndex = message < 30 ? (message < 0 ? 0 : message) : 0;
    warningMessageNumber = number <= 99999 ? (number < 0 ? 0 : number) : 99999;
    return 0;
}
void InitializeSnapshotDebugTextResources() {
    snapshotDebugTextResources.text.count = 0;
    snapshotDebugTextResources.text.capacity = 14;
    std::byte *const bytes = reinterpret_cast<std::byte *>(&snapshotDebugTextResources);
    for (std::uint32_t index = 0; index < 14; ++index) {
        std::byte *const row = bytes + SNAPSHOT_TEXT_STRIDE * index;
        const std::uint32_t idHigh = ((index + 48U) << 24U) | (SNAPSHOT_TEXT_PTX.name & 0x00ffffffU);
        ParseGuiPtx(row, 640, 16, SNAPSHOT_TEXT_TEXTURE.name, idHigh);
        snapshotDebugTextResources.text.textures[index] = static_cast<std::int32_t>(ReadTexnTexture(row));
    }
    snapshotDebugTextResources.secret.count = 0;
    snapshotDebugTextResources.secret.capacity = 1;
    std::byte *const secret = bytes + offsetof(SnapshotDebugTextResources, secret);
    ParseGuiPtx(secret, 640, 16, SNAPSHOT_TEXT_TEXTURE.name, SNAPSHOT_TEXT_PTX.name);
    snapshotDebugTextResources.secret.textures[0] = static_cast<std::int32_t>(ReadTexnTexture(secret));
}
// @verified - p
std::int32_t SnapshotDebugTextRasterNullsub(void *) {
    return 0;
}
std::int64_t CreateSnapshotDebugText(SnapshotDebugTextBank &bank, const char *text) {
    std::int32_t line = 0, length = 0;
    char character;
    do {
        character = *text;
        if (character == '\n' || character == 0 || ++length >= 42) {
            std::byte *const row = reinterpret_cast<std::byte *>(&bank) + SNAPSHOT_TEXT_STRIDE * line;
            SnapshotDebugTextRasterNullsub(row);
            ReplacePtxTexture(static_cast<std::uint32_t>(bank.textures[line]), row);
            ++line;
            length = 0;
        }
        ++text;
    } while (line < bank.capacity && character != 0);
    if (length > 0) {
        std::byte *const row = reinterpret_cast<std::byte *>(&bank) + SNAPSHOT_TEXT_STRIDE * line;
        SnapshotDebugTextRasterNullsub(row);
        ReplacePtxTexture(static_cast<std::uint32_t>(bank.textures[line]), row);
        ++line;
    }
    bank.count = line;
    return line;
}
std::int64_t DrawSnapshotDebugText(const SnapshotDebugTextBank &bank, std::int32_t x, std::int32_t y, float depth) {
    if (bank.count <= 0)
        return 0;
    std::int32_t line = 0;
    do {
        LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
        AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x, y, 640, 16, ScreenMode::AdjustedWithOffset);
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(bank.textures[line]), depth);
        y += 16;
    } while (++line < bank.count);
    return line;
}
std::int32_t LoadSnapshotDebugMetadata() {
    snapshotDebugMetadata = GetSnapshotMetadataOwner();
    if (!snapshotDebugMetadata)
        return 0;
    for (std::size_t index = 0; index < snapshotDebugOrder.size(); ++index) {
        SnapshotMetadataRecord *const record = &snapshotDebugMetadata->records[index];
        snapshotDebugOrder[index] = record;
        if (record->use) {
            const std::uint32_t time = record->second | ((static_cast<std::uint32_t>(record->year) << 26U) - 0x40000000U) |
                ((record->minute | ((record->hour | (32U * (record->day | (32U * record->month)))) << 6U)) << 6U);
            record->sortKeys[0] = std::bit_cast<std::int32_t>(~time);
            record->sortKeys[1] = std::bit_cast<std::int32_t>(time);
        } else
            record->sortKeys[0] = -1;
        UpdateSnapshotAreaSortKey(*record);
        record->sortKeys[3] = GetSnapshotBioSortKey(record->lockOnId.name);
    }
    return 1;
}

void LoadSnapshotDebugThumbnails() {
    for (std::uint32_t index = 0; index < snapshotDebugThumbnails.size(); ++index) {
        std::array<char, 128> filename{};
        std::snprintf(filename.data(), filename.size(), "Snapshots/Thumbnail/CharThumb%02u.bmp", index);
        std::ifstream input(filename.data(), std::ios::binary | std::ios::ate);
        if (!input)
            continue;
        const auto length = input.tellg();
        if (length <= 0)
            continue;
        std::vector<std::byte> bytes(static_cast<std::size_t>(length));
        input.seekg(0);
        if (!input.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size())))
            continue;
        snapshotDebugThumbnails[index] = d3t::CreateImageTextureFromMemory(bytes.data(), bytes.size(), false);
    }
}
void SortSnapshotDebugRecords(std::int32_t key) {
    for (std::int32_t first = 0; first < 125; ++first) {
        for (std::int32_t next = first + 1; next < 126; ++next) {
            if (static_cast<std::uint32_t>(snapshotDebugOrder[first]->sortKeys[key]) <= static_cast<std::uint32_t>(snapshotDebugOrder[next]->sortKeys[key]))
                continue;
            SnapshotMetadataRecord *const record = snapshotDebugOrder[first];
            snapshotDebugOrder[first] = snapshotDebugOrder[next];
            snapshotDebugOrder[next] = record;
        }
    }
}
void SortSnapshotDebugSubset(std::int32_t first, std::int32_t count, std::int32_t key) {
    if (count <= 0)
        return;
    const std::int32_t last = first + count;
    for (std::int32_t index = first; index < last - 1; ++index) {
        for (std::int32_t next = index + 1; next < last; ++next) {
            if (static_cast<std::uint32_t>(snapshotDebugOrder[index]->sortKeys[key]) <= static_cast<std::uint32_t>(snapshotDebugOrder[next]->sortKeys[key]))
                continue;
            SnapshotMetadataRecord *const record = snapshotDebugOrder[index];
            snapshotDebugOrder[index] = snapshotDebugOrder[next];
            snapshotDebugOrder[next] = record;
        }
    }
}
void GroupSnapshotDebugBiographies(SnapshotDebugWork &work) {
    SortSnapshotDebugRecords(3);
    std::int32_t record = 0, biography = 0;
    while (biography < 48 && record < 126) {
        const std::int32_t index = SNAPSHOT_BIOGRAPHIES[biography].index;
        const std::int32_t group = biography;
        std::int32_t count = 0, key = snapshotDebugOrder[record]->sortKeys[3];
        if (index == key) {
            work.biographyPictures[group][1] = record;
            do {
                ++count;
                if (++record == 126)
                    break;
                key = snapshotDebugOrder[record]->sortKeys[3];
            } while (index == key);
        }
        if (index < key)
            ++biography;
        else
            ++record;
        work.biographyPictures[group][0] = count;
    }
    for (std::int32_t index = 0; index < 48; ++index)
        SortSnapshotDebugSubset(work.biographyPictures[index][1], work.biographyPictures[index][0], 0);
    work.reserved88 = work.count;
}
std::uint32_t UnlockSnapshotDebugBiographies() {
    SortSnapshotDebugRecords(3);
    std::uint32_t changed = 0;
    for (std::uint32_t biography = 0; biography < 48; ++biography) {
        for (std::uint32_t record = 0; record < 126; ++record) {
            if (snapshotDebugOrder[record]->sortKeys[3] != static_cast<std::int32_t>(biography) || snapshotDebugMetadata->omakeFlags[biography])
                continue;
            snapshotDebugMetadata->omakeFlags[biography] = 1;
            changed = 1;
            break;
        }
    }
    for (std::uint32_t first = 0; first < 45; first += 9) {
        if (snapshotDebugMetadata->omakeFlags[first + 8])
            continue;
        std::uint32_t count = 0;
        for (; count < 8; ++count)
            if (!snapshotDebugMetadata->omakeFlags[first + count])
                break;
        if (count == 8) {
            snapshotDebugMetadata->omakeFlags[first + 8] = 1;
            changed = 1;
        }
    }
    return changed;
}
// @verified - p
void SnapshotDebugInputNullsub(std::intptr_t) {
}
void SnapshotDebugGridInput(SnapshotDebugWork &work, std::uint16_t pressed) {
    std::int32_t page = work.first / 9, column = work.selected % 3, row = (work.selected - work.first) / 3;
    if (pressed & 0x200U) {
        work.phase = 11;
        SnapshotDebugInputNullsub((work.selected - work.first));
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    if (!work.count)
        return;
    const std::int32_t remaining = work.count - work.first - 1, last = work.visible - 1;
    std::int32_t lastRow = 2;
    if (remaining < 9) {
        lastRow = remaining / 3;
        if (row > lastRow)
            row = lastRow;
        if (row == lastRow && column > remaining % 3)
            column = remaining % 3;
    }
    if (pressed & 0x80U) {
        const std::int32_t maximum = (work.count - 1) / 9;
        const bool advanced = ++page <= maximum;
        if (!advanced)
            page = maximum;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (work.selectionKind > 0)
            work.phase = 5;
    }
    if (pressed & 0x40U) {
        const bool advanced = --page >= 0;
        if (!advanced)
            page = 0;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (work.selectionKind > 0)
            work.phase = 5;
    }
    if (pressed & 0x8000U) {
        const std::int32_t next = column + 1;
        column = row < lastRow ? (next < 3 ? next : 0) : (next <= last % 3 ? next : 0);
    }
    if (pressed & 0x4000U) {
        if (--column < 0)
            column = row < lastRow ? 2 : last % 3;
    }
    if (pressed & 0x2000U) {
        if (++row == lastRow) {
            if (column > last % 3)
                column = last % 3;
        } else if (row > lastRow)
            row = 0;
    }
    if ((pressed & 0x1000U) && --row < 0) {
        row = lastRow;
        if (column > last % 3)
            column = last % 3;
    }
    work.page = page;
    work.first = 9 * page;
    work.selected = column + 3 * row + 9 * page;
    bool clamped = false;
    if (work.selected >= work.count) {
        work.selected = work.count - 1;
        column = work.selected % 3;
        row = (work.selected - 9 * page) / 3;
        clamped = true;
    }
    if (pressed & 0x400U) {
        const bool used = snapshotDebugOrder[work.selected]->use == 1;
        if (used)
            work.phase = 7;
        ExecuteAicaCommand(used ? 0x900abU : 0x800abU, 0, 0);
    }
    if (pressed & 4U) {
        if (++work.sort == 4)
            work.sort = 0;
        work.phase = work.sort == 3 ? 12 : 4;
        work.page = 0;
        work.selected = 0;
        work.first = 0;
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    if (work.sort != 3 && (pressed & 2U)) {
        work.returnPhase = work.drawnPhase;
        work.phase = 23;
        work.answer = 0;
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    if (column != work.column || row != work.row) {
        ExecuteAicaCommand(0x200abU, 0, 0);
        work.column = column;
        work.row = row;
        if (!clamped) {
            work.visible = work.count - work.first < 9 ? work.count - work.first : 9;
            work.drawnSelectionKind = work.selectionKind;
            work.drawnPage = work.page;
            work.drawnSelected = work.selected;
            work.drawnSort = work.sort;
            work.drawnFirst = work.first;
            work.drawnVisible = work.visible;
            work.drawnColumn = work.column;
            work.drawnRow = work.row;
        }
    }
}
void SnapshotDebugBiographyInput(SnapshotDebugWork &work, std::uint16_t pressed) {
    std::int32_t page = work.first / 9, column = work.selected % 3, row = (work.selected - work.first) / 3;
    const std::array<std::int32_t, 2> *const pictures = &work.biographyPictures[work.selected];
    if (pressed & 0x200U) {
        work.phase = 19;
        SnapshotDebugInputNullsub(reinterpret_cast<std::intptr_t>(pictures));
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    if (!work.count)
        return;
    const std::int32_t remaining = 47 - work.first, last = work.visible - 1;
    std::int32_t lastRow = 2;
    if (remaining < 9) {
        lastRow = remaining / 3;
        if (row > lastRow)
            row = lastRow;
        if (row == lastRow && column > remaining % 3)
            column = remaining % 3;
    }
    if (pressed & 0x80U) {
        const bool advanced = ++page <= 5;
        if (!advanced)
            page = 5;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (work.selectionKind > 0)
            work.phase = 13;
    }
    if (pressed & 0x40U) {
        const bool advanced = --page >= 0;
        if (!advanced)
            page = 0;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (work.selectionKind > 0)
            work.phase = 13;
    }
    if (pressed & 0x8000U) {
        const std::int32_t next = column + 1;
        column = row < lastRow ? (next < 3 ? next : 0) : (next <= last % 3 ? next : 0);
    }
    if ((pressed & 0x4000U) && --column < 0)
        column = row < lastRow ? 2 : last % 3;
    if (pressed & 0x2000U) {
        if (++row == lastRow) {
            if (column > last % 3)
                column = last % 3;
        } else if (row > lastRow)
            row = 0;
    }
    if ((pressed & 0x1000U) && --row < 0) {
        row = lastRow;
        if (column > last % 3)
            column = last % 3;
    }
    work.page = page;
    work.first = 9 * page;
    work.selected = column + 3 * row + 9 * page;
    if (work.selected >= 48) {
        work.selected = 47;
        column = 2;
        row = (47 - 9 * page) / 3;
    }
    if (pressed & 0x400U) {
        work.selectedBiography = work.selected;
        work.biographyPhoto = 0;
        bool admitted = false;
        if (work.selected % 9 == 8) {
            if (snapshotDebugMetadata->omakeFlags[work.selected]) {
                work.selected = 0;
                work.phase = 20;
                admitted = true;
            }
        } else if ((*pictures)[0] > 0) {
            work.selected = (*pictures)[1];
            work.phase = 15;
            admitted = true;
        }
        ExecuteAicaCommand(admitted ? 0x900abU : 0x800abU, 0, 0);
    }
    if (pressed & 4U) {
        if (++work.sort == 4)
            work.sort = 0;
        work.phase = 4;
        work.page = 0;
        work.selected = 0;
        work.first = 0;
        work.selectionKind = (work.count - 1) / 9;
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    if (column != work.column || row != work.row) {
        ExecuteAicaCommand(0x200abU, 0, 0);
        work.column = column;
        work.row = row;
        work.visible = 48 - work.first < 9 ? 48 - work.first : 9;
        work.drawnSelectionKind = work.selectionKind;
        work.drawnPage = work.page;
        work.drawnSelected = work.selected;
        work.drawnSort = work.sort;
        work.drawnFirst = work.first;
        work.drawnVisible = work.visible;
        work.drawnColumn = work.column;
        work.drawnRow = work.row;
    }
}
void SnapshotDebugBiographyPhotoInput(SnapshotDebugWork &work, std::uint16_t pressed, std::uint16_t zoom) {
    const std::array<std::int32_t, 2> &pictures = work.biographyPictures[work.selectedBiography];
    if (pressed & 0x40U) {
        const bool advanced = --work.biographyPhoto >= 0;
        if (!advanced)
            work.biographyPhoto = 0;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (pictures[0] > 1) {
            work.selected = work.biographyPhoto + pictures[1];
            work.phase = 15;
        }
    }
    if (pressed & 0x80U) {
        const bool advanced = ++work.biographyPhoto != pictures[0];
        if (!advanced)
            work.biographyPhoto = pictures[0] - 1;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (pictures[0] > 1) {
            work.selected = work.biographyPhoto + pictures[1];
            work.phase = 15;
        }
    }
    if (zoom > 0xf0U && work.zoomHeight + 288 < 480) {
        work.zoomWidth += 6;
        work.zoomHeight += 4;
    }
    if (zoom < 0x10U && work.zoomHeight + 288 > 120) {
        work.zoomWidth -= 6;
        work.zoomHeight -= 4;
    }
    if (pressed & 0x400U) {
        work.selected = work.biographyPhoto + pictures[1];
        work.phase = 17;
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    if (pressed & 0x200U) {
        work.selected = work.selectedBiography;
        work.phase = 18;
        ExecuteAicaCommand(0x900abU, 0, 0);
        work.visible = 48 - work.first < 9 ? 48 - work.first : 9;
        work.drawnSelectionKind = work.selectionKind;
        work.drawnPage = work.page;
        work.drawnSelected = work.selected;
        work.drawnSort = work.sort;
        work.drawnFirst = work.first;
        work.drawnVisible = work.visible;
        work.drawnColumn = work.column;
        work.drawnRow = work.row;
    }
}
void SnapshotDebugSecretPhotoInput(SnapshotDebugWork &work, std::uint16_t pressed, std::uint16_t held, std::uint16_t zoom) {
    const std::array<std::int32_t, 4> &picture = SNAPSHOT_SECRET_PICTURES[work.page];
    if (pressed & 0x40U) {
        const bool advanced = --work.biographyPhoto >= 0;
        if (!advanced)
            work.biographyPhoto = 0;
        else
            work.phase = 20;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
    }
    if (pressed & 0x80U) {
        const bool advanced = ++work.biographyPhoto != picture[1];
        if (!advanced)
            work.biographyPhoto = picture[1] - 1;
        else
            work.phase = 20;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
    }
    if (held & 0x1000U) {
        work.panY -= 8;
        const double minimum = -(picture[3] * 1.5);
        if (work.panY < minimum)
            work.panY = static_cast<std::int32_t>(minimum);
    }
    if (held & 0x2000U) {
        work.panY += 8;
        const double maximum = picture[3] * 1.5;
        if (work.panY > maximum)
            work.panY = static_cast<std::int32_t>(maximum);
    }
    if (held & 0x8000U) {
        work.panX += 8;
        const double minimum = -(picture[2] * 1.5);
        if (work.panX < minimum)
            work.panX = static_cast<std::int32_t>(minimum);
    }
    if (held & 0x4000U) {
        work.panX -= 8;
        const double maximum = picture[2] * 1.5;
        if (work.panX > maximum)
            work.panX = static_cast<std::int32_t>(maximum);
    }
    if (zoom > 0xf0U && work.zoomHeight + picture[3] < picture[3] * 2.5) {
        work.zoomWidth += 11;
        work.zoomHeight += 14;
    }
    if (zoom < 0x10U && work.zoomHeight + picture[3] > picture[3] * 0.25) {
        work.zoomWidth -= 11;
        work.zoomHeight -= 14;
    }
    if (pressed & 0x200U) {
        work.selected = work.selectedBiography;
        work.hideHud = 0;
        work.phase = 22;
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
}
// @verified - p
void SnapshotDebugMissingMetadataNullsub() {
}
void PrepareSnapshotDebug(SnapshotDebugWork &work) {
    if (work.mode)
        EV_EventSetUpHelpIcon_0(0x9a, 0);
    else {
        work.previousClockSuppression = IsClockAndCompassSuppressed();
        SetRenderClockAndCompassTo(1);
        EV_EventSetUpHelpIcon_0(0x99, 0);
        CheckHelpIcon(32, 0, CheckHelpIconOp::Remove);
    }
    CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
    FinishLoading();
    if (work.mode == 1) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        WaitForNextTask_0(control->screenFilmTask, 0, 30);
    }
    std::error_code error;
    const std::filesystem::path path = S2Save_GetDirectory() / "SnapshotData";
    std::int32_t loaded = std::filesystem::exists(path, error) && !std::filesystem::is_directory(path, error) ? 1 : 0;
    if (loaded == 1) {
        loaded = LoadSnapshotDebugMetadata();
        if (!loaded) {
            work.suppressContent = 1;
            work.loadingPhase = 25;
            work.phase = 28;
            CheckHelpIcon(1, 0, CheckHelpIconOp::Remove);
            if (!work.mode) {
                CheckHelpIcon(2, 0, CheckHelpIconOp::Remove);
                CheckHelpIcon(256, 0, CheckHelpIconOp::Remove);
                CheckHelpIcon(64, 0, CheckHelpIconOp::Remove);
            }
            SnapshotDebugMissingMetadataNullsub();
            work.returnPhase = work.mode ? 6 : 3;
            return;
        }
        work.count = 0;
        work.selectionKind = 0;
        for (std::size_t index = 0; index < 126; ++index) {
            if (snapshotDebugMetadata->records[index].use == 1)
                std::memcpy(snapshotDebugStorage.images.data() + 32 + 36 * index, &d3t::g_defaultWhiteTexture, 4);
        }
        LoadSnapshotDebugThumbnails();
    } else {
        work.count = 0;
        work.selectionKind = 0;
    }
    if (work.mode) {
        work.phase = 5;
        work.depth = -2.1f;
        if (loaded == 1) {
            if (UnlockSnapshotDebugBiographies() == 1)
                QueueSnapshotMetadataWrite(snapshotDebugMetadata);
            SortSnapshotDebugRecords(0);
        }
    } else {
        work.phase = 1;
        work.depth = 0.5f;
        if (loaded == 1) {
            SortSnapshotDebugRecords(0);
            CheckHelpIcon(2, 0, CheckHelpIconOp::Set);
        }
        work.previousNpcLoad = static_cast<std::uint32_t>(GetNpcNewLoadState());
        EV_ResetFreeTalkFlags();
        EV_NPC_NewLoadSw(0);
    }
    work.drawnSelectionKind = work.selectionKind;
    work.drawnPage = work.page;
    work.drawnSelected = work.selected;
    work.drawnSort = work.sort;
    work.drawnFirst = work.first;
    work.drawnVisible = work.visible;
    work.drawnColumn = work.column;
    work.drawnRow = work.row;
}
void FinishSnapshotDebug() {
    d3t::SetSceneClearColour(0xff000000U);
    if (!snapshotDebugWork)
        return;
    SnapshotDebugWork &work = *snapshotDebugWork;
    const std::int32_t mode = work.mode;
    EV_EventSetUpHelpIcon_0(work.previousHelp, 0);
    if (work.waitingTask)
        HLib::ResolveDeferredCallback(work.waitingTask);
    DispatchPostFxFilterRender(1);
    if (mode) {
        ReinitTextureData();
        g_titleFlagShouldRun = 20;
        g_actNumber = 1;
        g_entryPoint = 0;
        g_loadedActNumber = 1;
        SetNextSceneNumber(1);
        hotLoadModule = SystemModule::Cold;
        for (std::int32_t index = 0; index < snapshotDebugTextResources.text.capacity; ++index)
            ReleaseNamedTexture(snapshotDebugTextResources.text.textures[index]);
        for (std::int32_t index = 0; index < snapshotDebugTextResources.secret.capacity; ++index)
            ReleaseNamedTexture(snapshotDebugTextResources.secret.textures[index]);
        ChangeDisc();
        ChangeSystemModule(SystemModule::Cold, 0);
        HLib::SetupTaskDelay(0);
    } else {
        EV_NPC_NewLoadSw(work.previousNpcLoad);
        if (work.drawnPhase == 31) {
            HLib::DestroyCurrentTask();
            ReinitTextureData();
        } else {
            SetWarningMessageParameters(0, 0);
            HLib::SetupTimedTask(snapshotDebugTask, 0);
        }
    }
}
void DrawSnapshotDebugPageCounter(const SnapshotDebugWork &work) {
    LegacyIconSprite sprite{};
    InitializeLegacyIconSprite(sprite);
    sprite.y = work.mode ? 436.0f : 420.0f;
    sprite.depth = work.depth + 0.05f;
    sprite.flags = 1024;
    sprite.width = sprite.height = 16;
    sprite.alignment = 4;
    const auto digits = [](std::int32_t value) {
        return std::array<std::int16_t, 3>{static_cast<std::int16_t>(value >= 100 ? value / 100 : -1),
            static_cast<std::int16_t>(value >= 10 ? (value % 100) / 10 : -1), static_cast<std::int16_t>(value % 10)};
    };
    const auto left = digits(work.drawnPage + 1);
    for (std::size_t index = 0; index < left.size(); ++index) {
        if (left[index] < 0)
            continue;
        sprite.x = 264.0f + static_cast<float>(index * 16);
        sprite.icon = static_cast<std::uint16_t>(1030 + left[index]);
        PrepareDrawWithMode(sprite, ScreenMode::Centered);
    }
    sprite.x = 312.0f;
    sprite.icon = 1050;
    PrepareDrawWithMode(sprite, ScreenMode::Centered);
    float x = 328.0f;
    for (const auto digit : digits(work.drawnSelectionKind + 1)) {
        if (digit < 0)
            continue;
        sprite.x = x;
        sprite.icon = static_cast<std::uint16_t>(1030 + digit);
        PrepareDrawWithMode(sprite, ScreenMode::Centered);
        x += 16.0f;
    }
}
// @verified - p
std::int32_t DrawSnapshotDebugRect(const SnapshotDebugRect &rect, std::uint32_t colour, float depth) {
    const float x = static_cast<float>(rect.x), y = static_cast<float>(rect.y);
    const float right = x + static_cast<float>(rect.width), bottom = y + static_cast<float>(rect.height);
    LegacyColourQuad packet{.quad = {.flags = 1024, .corners = {x, y, right, y, x, bottom, right, bottom}}, .colour = colour};
    shenSubmitQuad(packet, depth);
    return 1;
}
// @verified - p
std::int32_t DrawSnapshotDebugTexture(std::uint32_t texture, const SnapshotDebugRect &rect, const std::array<float, 8> &uv, std::uint32_t colour, float depth) {
    const float x = static_cast<float>(rect.x), y = static_cast<float>(rect.y);
    const float right = x + static_cast<float>(rect.width), bottom = y + static_cast<float>(rect.height);
    LegacySpritePacket packet{.quad = {.flags = 1536, .corners = {x, y, right, y, x, bottom, right, bottom}},
        .colour = colour,
        .secondaryColour = 0,
        .uv = {uv[0], uv[4], uv[3], uv[7]}};
    LegacySpriteNullsub(packet, texture, depth);
    return 1;
}
void DrawSnapshotDebugGridBackground(const SnapshotDebugWork &work, float depth) {
    std::int32_t column = 0, y = 96;
    for (std::int32_t index = work.drawnFirst; index < work.drawnFirst + work.drawnVisible; ++index) {
        SnapshotDebugRect rect{.x = 154 * column + 100, .y = work.mode ? y : y - 36, .width = 132, .height = 92};
        DrawSnapshotDebugRect(rect, 0xffc0c0c0U, depth - 0.05f);
        rect.x += 6;
        rect.y += 6;
        DrawSnapshotDebugRect(rect, 0xa0101010U, depth - 0.06f);
        if (++column == 3) {
            column = 0;
            y += 114;
        }
    }
}
void DrawSnapshotDebugFullImage(const SnapshotDebugWork &work) {
    if (work.suppressContent == 1)
        return;
    CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
    SnapshotDebugRect rect;
    if (work.fullscreen)
        rect = {.x = -40, .y = 0, .width = 720, .height = 480};
    else {
        rect.width = work.zoomWidth + 460;
        rect.x = 320 - rect.width / 2;
        rect.y = 2 * (48 - work.zoomHeight / 28) - work.zoomHeight / 2;
        rect.height = work.zoomHeight + 316;
        DrawSnapshotDebugRect(rect, 0xffc0c0c0U, -2.05f);
        SnapshotDebugRect shadow{.x = rect.x + 7, .y = rect.y + 7, .width = rect.width, .height = rect.height};
        DrawSnapshotDebugRect(shadow, 0xa0101010U, -2.0599999f);
        const std::int32_t inset = static_cast<std::int32_t>((28.0f - static_cast<float>(work.zoomWidth + 28) / 432.0f) * -0.5f);
        rect.x -= inset;
        rect.y -= inset;
        rect.width = work.zoomWidth + 432;
        rect.height = work.zoomHeight + 288;
    }
    const SnapshotMetadataRecord &record = *snapshotDebugOrder[work.selected];
    std::uint32_t texture;
    std::memcpy(&texture, snapshotDebugStorage.images.data() + 36 * record.id + 32, sizeof(texture));
    DrawSnapshotDebugTexture(texture, rect, SNAPSHOT_FULL_IMAGE_UV, 0xffffffffU, -2.0f);
}

// @verified - p
void DrawSnapshotDebugSprite(std::int32_t index, std::uint32_t colour, float scale, float depth) {
    const SnapshotDebugSprite &sprite = SNAPSHOT_DEBUG_SPRITES[index];
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = colour, .secondaryColour = 0, .uv = sprite.uv, .reservedTail = 0};
    TransformScreenSpace(
        packet.quad, static_cast<ScreenAlignment>(sprite.alignment), sprite.x, sprite.y, sprite.width, sprite.height, scale, scale, 0, ScreenMode::Centered);
    packet.quad.flags |= 1024U;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(snapshotDebugSprites[sprite.texture]), depth);
}
// @verified - p
void DrawSnapshotDebugSelection(const SnapshotDebugWork &work) {
    if (work.suppressSelection == 1)
        return;
    const std::int32_t angle = static_cast<std::int32_t>((((static_cast<float>(work.frames % 64) * 0.015625f) * 3.1415927f) * 65536.0f) / 6.2831855f + 0.5f);
    const std::uint32_t alpha = static_cast<std::uint8_t>(static_cast<std::int64_t>(sin_fixed16(static_cast<std::uint16_t>(angle)) * 128.0f));
    const std::uint32_t colour = (0xff000000U - (alpha << 24U)) | 0x00ff0000U;
    if (work.drawnSort == 3 && work.drawnSelected % 9 == 8)
        DrawSnapshotDebugSprite(22, colour, 1.15f, -2.1499999f);
    else
        DrawSnapshotDebugRect({.x = 154 * work.drawnColumn + 95, .y = 114 * work.drawnRow + 91, .width = 143, .height = 103}, colour, -2.1499999f);
}
// @verified - p
void DrawSnapshotDebugPageArrows(const SnapshotDebugWork &work) {
    if (!work.mode) {
        CheckHelpIcon(64, 0, work.drawnPage < work.drawnSelectionKind ? CheckHelpIconOp::Add : CheckHelpIconOp::Remove);
        CheckHelpIcon(256, 0, work.drawnPage > 0 ? CheckHelpIconOp::Add : CheckHelpIconOp::Remove);
        return;
    }
    if (work.drawnPage > 0) {
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[3], 0xff000000U, ScreenMode::Centered);
        DrawTitleSpriteRect(SNAPSHOT_ARROW_UV[0].data(), {.x = 41, .y = 258, .z = -2.09f}, 8, 1.0f);
    }
    if (work.drawnPage < work.drawnSelectionKind) {
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[2], 0xff000000U, ScreenMode::Centered);
        DrawTitleSpriteRect(SNAPSHOT_ARROW_UV[1].data(), {.x = 596, .y = 258, .z = -2.09f}, 8, 1.0f);
    }
}
// @verified - p
void DrawSnapshotDebugBiographyArrows(const SnapshotDebugWork &work) {
    if (work.suppressContent == 1)
        return;
    CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
    if (work.biographyPhoto > 0) {
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[3], 0xff000000U, ScreenMode::Centered);
        DrawTitleSpriteRect(SNAPSHOT_ARROW_UV[0].data(), {.x = 41, .y = 258, .z = -2.09f}, 8, 1.0f);
    }
    if (work.biographyPhoto < work.biographyPictures[work.selectedBiography][0] - 1) {
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[2], 0xff000000U, ScreenMode::Centered);
        DrawTitleSpriteRect(SNAPSHOT_ARROW_UV[1].data(), {.x = 596, .y = 258, .z = -2.09f}, 8, 1.0f);
    }
}
// @verified - p
void DrawSnapshotDebugSecretArrows(const SnapshotDebugWork &work) {
    if (work.suppressContent == 1)
        return;
    const std::int32_t width = SNAPSHOT_SECRET_PICTURES[work.drawnPage][2];
    if (work.biographyPhoto > 0)
        DrawTitleSpriteRect(SNAPSHOT_ARROW_UV[0].data(), {.x = static_cast<float>((640 - width) / 2) - 32.0f, .y = 258, .z = -2.09f}, 8, 1.0f);
    if (work.biographyPhoto < work.biographyPictures[work.selectedBiography][0] - 1)
        DrawTitleSpriteRect(SNAPSHOT_ARROW_UV[1].data(), {.x = static_cast<float>((width + 640) / 2) + 30.0f, .y = 258, .z = -2.09f}, 8, 1.0f);
}
// @verified - p
void DrawSnapshotDebugBiographies(const SnapshotDebugWork &work) {
    std::int32_t column = 0, y = 96;
    for (std::int32_t index = 0; index < work.drawnVisible; ++index) {
        const std::int32_t biography = index + work.drawnFirst;
        SnapshotDebugRect rect{.x = 154 * column + 100, .y = y, .width = 132, .height = 92};
        if (biography % 9 == 8) {
            if (snapshotDebugMetadata->omakeFlags[biography]) {
                DrawSnapshotDebugSprite(work.drawnPage + 7, 0xffffffffU, 1.0f, -2.1f);
                DrawSnapshotDebugSprite(work.drawnPage + 13, 0xffffffffU, 3.5f, -2.1999998f);
            } else {
                DrawSnapshotDebugSprite(19, 0xffffffffU, 1.0f, -2.1f);
                DrawSnapshotDebugSprite(20, 0xffffffffU, 3.5f, -2.1999998f);
            }
            DrawSnapshotDebugSprite(21, 0xa0101010U, 3.5f, -2.2499998f);
        } else {
            DrawSnapshotDebugRect(rect, 0xffc0c0c0U, -2.1499999f);
            rect.x += 6;
            rect.y += 6;
            rect.width = 120;
            rect.height = 80;
            DrawSnapshotDebugTexture(snapshotDebugThumbnails[biography], rect, SNAPSHOT_FULL_IMAGE_UV,
                snapshotDebugMetadata->omakeFlags[biography] ? 0xffffffffU : 0x80ffffffU, -2.1f);
            if (!snapshotDebugMetadata->omakeFlags[biography])
                DrawSnapshotDebugRect({.x = 154 * (index % 3) + 106, .y = 114 * (index / 3) + 102, .width = 120, .height = 80}, 0xffc0a440U, -2.1399999f);
            rect.width = 132;
            rect.height = 92;
            DrawSnapshotDebugRect(rect, 0xa0101010U, -2.1599998f);
        }
        if (++column == 3) {
            column = 0;
            y += 114;
        }
    }
}
// @verified - p
void DrawSnapshotDebugGrid(const SnapshotDebugWork &work) {
    if (work.suppressContent == 1)
        return;
    DrawSnapshotDebugPageCounter(work);
    if (!work.mode)
        CheckHelpIcon(2, 0, CheckHelpIconOp::Set);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
    if (!work.count)
        return;
    DrawSnapshotDebugGridBackground(work, work.depth);
    DrawSnapshotDebugPageArrows(work);
    if (work.mode == 1) {
        DrawSnapshotDebugSelection(work);
        DrawSnapshotDebugSprite(0, 0xffffffffU, 1.0f, -2.1499999f);
        DrawSnapshotDebugSprite(work.drawnSort + 1, 0xffffffffU, 1.0f, -2.1f);
    }
}
// @verified - p
void DrawSnapshotDebugOverlay(const SnapshotDebugWork &work) {
    DrawTitleBackground();
    if (work.hideHud == 1)
        return;
    DrawTitleSpriteRect(SNAPSHOT_TAB_UV[0].data(), {.x = 160, .y = 59, .z = -2.8f}, 7, 1.0f);
    DrawTitleSpriteRect(SNAPSHOT_TAB_UV[1].data(), {.x = 480, .y = 59, .z = -2.8f}, 7, 1.0f);
    PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[6], 0xff000000U, ScreenMode::Centered);
    if (work.count <= 0)
        return;
    switch (work.drawnPhase) {
    case 5:
    case 6:
    case 11:
    case 13:
    case 14:
    case 19:
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[4], 0xff000000U, ScreenMode::Centered);
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[7], 0xff000000U, ScreenMode::Centered);
        break;
    default:
        break;
    }
    PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[5], 0xff000000U, ScreenMode::Centered);
    if (static_cast<std::uint32_t>(work.drawnPhase - 5) <= 3U || work.drawnPhase == 11)
        PrepareDrawSimple(SNAPSHOT_DEBUG_ICONS[8], 0xff000000U, ScreenMode::Centered);
}
// @verified - p
void DrawSnapshotDebugTextBorder(std::int32_t y, std::int16_t height, std::uint32_t fill, std::uint32_t border) {
    LegacySolidRect rect{
        .x = 320, .y = static_cast<float>(y + height / 2), .scaleX = 1, .scaleY = 1, .depth = 0.09f, .colour = fill, .width = 608, .height = height};
    DrawLegacySolidRect(rect);
    LegacySolidRect edge = rect;
    edge.height = 2;
    edge.colour = border;
    edge.y = rect.y - static_cast<float>(height / 2);
    DrawLegacySolidRect(edge);
    edge.y = rect.y + static_cast<float>(height / 2);
    DrawLegacySolidRect(edge);
    edge.width = 2;
    edge.height = height;
    edge.y = rect.y;
    edge.x = 16;
    DrawLegacySolidRect(edge);
    edge.x = 624;
    DrawLegacySolidRect(edge);
}
// @verified - p
void AppendSnapshotDebugLocation(const SnapshotMetadataRecord &record, std::span<char> text) {
    const std::uint32_t area = static_cast<std::uint32_t>(record.sortKeys[2]);
    const char *label = nullptr;
    bool trailingSpace = true;
    switch (area) {
    case 0x1000e: label="UIHELPL026"; break; // @text EN: "Lucky Plaza" | JA: "好運中心"
    case 0x1001d: label="UIHELPL022"; break; // @text EN: "Beverly Hills Wharf" | JA: "ビバリーヒルズワーフ"
    case 0x2000d: case 0x2000e: label="UIHELPL103"; break; // @text EN: "South Carmain Qr." | JA: "紅南街"
    case 0x20022: label="UIHELPL069"; break; // @text EN: "Dou Jiang Diner" | JA: "豆将軒"
    case 0x2004b: case 0x2004c: case 0x2004d: label="UIHELPL091"; break; // @text EN: "Man Mo Temple" | JA: "文武廟"
    case 0x20053: label="UIHELPL113"; break; // @text EN: "White Dynasty Qr." | JA: "白秦街"
    case 0x30044: label="UIHELPL179"; break; // @text EN: "Yellow Head Bldg." | JA: "黄天楼"
    case 0x40003: label="UIHELPL182"; break; // @text EN: "Yingshuihe" | JA: "緑野"
    case 0x30000:
    case 0x40000:
    case 0x40010:
        break;
    case 0x80000: {
        const std::size_t offset = std::strlen(text.data());
        std::snprintf(text.data() + offset, text.size() - offset, "???");
        break;
    }
    default:
        label = GetLocationLabelId(record.area.name);
        const char *const subArea = GetLocationLabelId(record.subArea.name);
        if (const char *const shop = GetLocationLabelId(record.shop.name))
            label = shop;
        if (!label || (subArea && std::strcmp(subArea, label) < 0))
            label = subArea;
        trailingSpace = false;
        break;
    }
    if (label) {
        const std::string_view location = GetLocalizedSceneText(label);
        const std::size_t offset = std::strlen(text.data());
        std::snprintf(text.data() + offset, text.size() - offset, trailingSpace ? "%.*s " : "%.*s", static_cast<int>(location.size()), location.data());
    }
    const std::uint32_t region = (area >> 16U) - 1U;
    if (region < 4) {
        const std::string_view name = GetLocalizedSceneText(SNAPSHOT_REGION_LABELS[region]);
        const std::size_t offset = std::strlen(text.data());
        std::snprintf(text.data() + offset, text.size() - offset, ", %.*s", static_cast<int>(name.size()), name.data());
    }
}
// @verified - p
void DrawSnapshotDebugBiographyText(const SnapshotDebugWork &work) {
    SnapshotMetadataRecord &record = *snapshotDebugOrder[work.drawnSelected];
    if (!record.lockOnId.name)
        record.lockOnId = SNAPSHOT_EMPTY_AREA;
    std::array<char, 592> text;
    std::snprintf(
        text.data(), text.size(), "%s %d, %d - %02d:%02d\n", SNAPSHOT_MONTH_NAMES[record.month - 1], record.day, record.year, record.hour, record.minute);
    AppendSnapshotDebugLocation(record, text);
    std::int32_t y = 200;
    if (static_cast<std::uint32_t>(record.sortKeys[3]) < 48U) {
        const SnapshotBiography &biography = SNAPSHOT_BIOGRAPHIES[record.sortKeys[3]];
        const std::size_t offset = std::strlen(text.data());
        std::snprintf(text.data() + offset, text.size() - offset, "\n\n\nName : %s\nAge  : %s\nBirth: %s, %s\nBlood: %s\n\n%s", biography.name, biography.age,
            biography.birthDate, biography.starSign, biography.bloodType, biography.description);
        DrawSnapshotDebugTextBorder(208, 144, 0x3b00008cU, 0xff4040c0U);
        y = 128;
    }
    DrawSnapshotDebugTextBorder(y, 64, 0x3b008c00U, 0xff40c040U);
    CreateSnapshotDebugText(snapshotDebugTextResources.text, text.data());
    DrawSnapshotDebugText(snapshotDebugTextResources.text, 32, y + 16, 0.1f);
    vfSubmitQuad(0xaf000000U, 0.079999998f);
}
// @verified - p
void DrawSnapshotDebugLoading(const SnapshotDebugWork &work) {
    if (!work.mode) {
        SetWarningMessageParameters(work.loadingPhase, 0);
        return;
    }
    std::int32_t sprite;
    switch (work.loadingPhase) {
    case 25:
        sprite = 23;
        break;
    case 26:
        sprite = 24;
        break;
    case 27:
        sprite = 25;
        break;
    case 29:
        sprite = 27;
        break;
    default:
        sprite = 26;
        break;
    }
    vfSubmitQuad(0xaf000000U, 0.079999998f);
    DrawSnapshotDebugSprite(sprite, 0xffffffffU, 1.0f, 13.0f);
}
// @verified - p
void DrawSnapshotDebugWindow(const SnapshotDebugWork &work) {
    if (work.suppressContent == 1)
        return;
    const float aspect = GetS2AspectRatioAdjustment();
    const float x = 100.0f - (((1.0f - aspect) * 3.0f) * 100.0f);
    const std::int32_t columns = static_cast<std::int32_t>(((1.0f - aspect) * 1.5f) * 12.0f + 12.0f);
    DrawLegacyWindowTiles(x, 60.0f, columns, 10, 0.19999999f, 1024, ScreenMode::Centered);
    DrawLegacyWindowBackground(x - 20.0f, 40.0f, static_cast<float>(40 * columns), 399.0f, 0.3f, 1024, ScreenMode::Centered);
    DrawLegacyWindowEdges(x - 24.0f, 40.0f, 10, columns, 0.5f, 1024, ScreenMode::Centered);
    DrawLegacyWindowHorizontal(x - 36.0f, 40.0f, columns, 1, 0, 0.5f, 1024, ScreenMode::Centered);
    DrawLegacyWindowHorizontal(x - 36.0f, 400.0f, columns, 0, 0, 0.5f, 1024, ScreenMode::Centered);
    DrawLegacyWindowHorizontal(x - 36.0f, 440.0f, columns, 1, 1, 0.5f, 1024, ScreenMode::Centered);
}
// @verified - p
void RenderSnapshotDebug(const SnapshotDebugWork &work) {
    switch (work.drawnPhase) {
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 23:
        DrawSnapshotDebugGrid(work);
        break;
    case 7:
    case 8:
    case 10:
        DrawSnapshotDebugFullImage(work);
        break;
    case 9:
        DrawSnapshotDebugBiographyText(work);
        DrawSnapshotDebugFullImage(work);
        break;
    case 11:
    case 19: {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        WaitForNextTask_0(control->screenFilmTask, 1, 30);
        break;
    }
    case 12:
    case 13:
    case 14:
        if (work.suppressContent != 1) {
            DrawSnapshotDebugPageCounter(work);
            DrawSnapshotDebugBiographies(work);
            CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
            DrawSnapshotDebugPageArrows(work);
            DrawSnapshotDebugSelection(work);
            DrawSnapshotDebugSprite(0, 0xffffffffU, 1.0f, -2.1499999f);
            DrawSnapshotDebugSprite(work.drawnSort + 1, 0xffffffffU, 1.0f, -2.1f);
        }
        break;
    case 15:
    case 16:
    case 18:
        DrawSnapshotDebugBiographyArrows(work);
        DrawSnapshotDebugFullImage(work);
        break;
    case 17:
        DrawSnapshotDebugBiographyArrows(work);
        DrawSnapshotDebugBiographyText(work);
        DrawSnapshotDebugFullImage(work);
        break;
    case 20:
    case 21:
    case 22:
        DrawSnapshotDebugSecretArrows(work);
        break;
    case 24:
        vfSubmitQuad(0xaf000000U, 0.079999998f);
        DrawTitleSpriteRect(nullptr, {.x = 320, .y = 320, .z = 12.9f}, 16, 1.0f);
        DrawSnapshotDebugSprite(5, 0xffffffffU, 1.0f, 13.0f);
        DrawTitleSelection(320, 220, 90, work.answer);
        DrawSnapshotDebugFullImage(work);
        break;
    case 25:
    case 26:
        if (work.mode)
            vfSubmitQuad(0xaf000000U, 0.079999998f);
        else
            SetWarningMessageParameters(0, 0);
        if (work.returnPhase == 6)
            DrawSnapshotDebugGrid(work);
        else if (work.returnPhase == 8)
            DrawSnapshotDebugFullImage(work);
        break;
    case 27:
        if (work.mode == 1)
            vfSubmitQuad(0xaf000000U, 0.079999998f);
        if (work.phase == 6)
            DrawSnapshotDebugGrid(work);
        else if (work.phase == 8)
            DrawSnapshotDebugFullImage(work);
        break;
    case 28:
    case 29: {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (IsScreenFilmNotShowing(control->screenFilmTask) == 1)
            DrawSnapshotDebugLoading(work);
        break;
    }
    default:
        break;
    }
    if (work.mode)
        DrawSnapshotDebugOverlay(work);
    else {
        const std::uint32_t phase = static_cast<std::uint32_t>(work.drawnPhase);
        if (phase > 31U || !(0xbc000009U & (1U << phase)))
            DrawSnapshotDebugWindow(work);
    }
}
// @verified - p
void FreeSnapshotDebugMetadata(SnapshotMetadataOwner *metadata) {

    if (metadata && metadata != GetSnapshotMetadataOwner())
        HLib::FreeTaskData(metadata);
}
void SnapshotDebugPageInput(SnapshotDebugWork &work, std::uint16_t pressed) {
    std::int32_t page = work.first / 9;
    if (pressed & 0x80U) {
        const std::int32_t maximum = (work.count - 1) / 9;
        const bool advanced = ++page <= maximum;
        if (!advanced)
            page = maximum;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (work.selectionKind > 0)
            work.phase = 1;
    }
    if (pressed & 0x40U) {
        const bool advanced = --page >= 0;
        if (!advanced)
            page = 0;
        ExecuteAicaCommand(advanced ? 0x400abU : 0x800abU, 0, 0);
        if (work.selectionKind > 0)
            work.phase = 1;
    }
    if (pressed & 0x200U) {
        work.phase = 3;
        ExecuteAicaCommand(0x900abU, 0, 0);
    }
    work.page = page;
    work.first = 9 * page;
}
void LoadSnapshotDebugSecretPhoto(SnapshotDebugWork &work) {
    work.panX = 0;
    work.panY = 0;
    work.zoomWidth = 0;
    work.zoomHeight = 0;
    work.loadingPhase = 26;
    work.phase = 28;
    CheckHelpIcon(1, 0, CheckHelpIconOp::Remove);
    if (!work.mode) {
        CheckHelpIcon(2, 0, CheckHelpIconOp::Remove);
        CheckHelpIcon(256, 0, CheckHelpIconOp::Remove);
        CheckHelpIcon(64, 0, CheckHelpIconOp::Remove);
    }
    SnapshotDebugMissingMetadataNullsub();
    work.suppressContent = 1;
    work.selected = work.selectedBiography;
    work.returnPhase = 14;
}
void UpdateSnapshotDebug(SnapshotDebugWork &work) {
    std::uint16_t pressed, repeated = 0, zoom = 0;
    if (work.mode) {
        pressed = g_inputBuffer[0].pressedPriority;
        repeated = g_inputBuffer[0].repeatedPresses;
        std::memcpy(&zoom, reinterpret_cast<const std::byte *>(&g_inputBuffer[0]) + 15, 2);
    } else
        pressed = static_cast<std::uint16_t>(GetPadControlData(1));
    if (g_loadingFlag & 2U)
        return;
    work.drawnPhase = work.phase;
    switch (work.phase) {
    case 0:
        PrepareSnapshotDebug(work);
        break;
    case 1:
        work.phase = 2;
        goto capturePage;
    case 2:
        SnapshotDebugPageInput(work, pressed);
        break;
    case 3:
        FreeSnapshotDebugMetadata(snapshotDebugMetadata);
        SetRenderClockAndCompassTo(work.previousClockSuppression == 0);
        CheckHelpIcon(32, 0, CheckHelpIconOp::Add);
        CheckHelpIcon(256, 0, CheckHelpIconOp::Remove);
        CheckHelpIcon(64, 0, CheckHelpIconOp::Remove);
        work.phase = 31;
        break;
    case 4:
        if (work.sort == 0 || work.sort == 1)
            SortSnapshotDebugRecords(work.sort);
        else if (work.sort == 2) {
            SortSnapshotDebugRecords(0);
            SortSnapshotDebugRecords(2);
        } else if (work.sort == 3)
            GroupSnapshotDebugBiographies(work);
        work.phase = 6;
        goto capturePage;
    case 5:
        work.phase = 6;
        goto capturePage;
    case 6:
        SnapshotDebugGridInput(work, pressed);
        break;
    case 7:
        work.phase = 8;
        goto captureImage;
    case 8:
        if (pressed & 0x400U) {
            work.phase = 9;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
        if (pressed & 0x200U) {
            work.phase = 10;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
        if (pressed & 2U) {
            work.returnPhase = work.drawnPhase;
            work.phase = 24;
            work.answer = 0;
        }
        if (zoom > 0xf0U && work.zoomHeight + 288 < 480) {
            work.zoomWidth += 6;
            work.zoomHeight += 4;
        }
        if (zoom < 0x10U && work.zoomHeight + 288 > 120) {
            work.zoomWidth -= 6;
            work.zoomHeight -= 4;
        }
        break;
    case 9:
        if (pressed & 0x400U) {
            work.phase = 8;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
        if (pressed & 0x200U) {
            work.phase = 10;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
        break;
    case 10:
        work.phase = 6;
        break;
    case 11:
    case 19:
        FreeSnapshotDebugMetadata(snapshotDebugMetadata);
        work.phase = 30;
        work.waitFrames = 30;
        break;
    case 12:
        if (work.sort == 0 || work.sort == 1)
            SortSnapshotDebugRecords(work.sort);
        else if (work.sort == 2) {
            SortSnapshotDebugRecords(0);
            SortSnapshotDebugRecords(2);
        } else if (work.sort == 3)
            GroupSnapshotDebugBiographies(work);
        [[fallthrough]];
    case 13:
        work.phase = 14;
        work.selectionKind = 5;
        goto captureBiography;
    case 14:
        SnapshotDebugBiographyInput(work, pressed);
        break;
    case 15:
        work.phase = 16;
        goto captureImage;
    case 16:
        SnapshotDebugBiographyPhotoInput(work, pressed, zoom);
        break;
    case 17:
        if (pressed & 0x400U) {
            work.phase = 16;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
        if (pressed & 0x200U) {
            work.phase = 14;
            ExecuteAicaCommand(0x900abU, 0, 0);
            work.selected = work.selectedBiography;
            goto captureBiography;
        }
        break;
    case 18:
        work.phase = 14;
        break;
    case 20:
        LoadSnapshotDebugSecretPhoto(work);
        break;
    case 21:
        SnapshotDebugSecretPhotoInput(work, pressed, repeated, zoom);
        break;
    case 22:
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        work.phase = 14;
        break;
    case 23:
        work.phase = 24;
        goto captureCount;
    case 24:
        if (pressed & 0x4000U) {
            if (work.answer != 1)
                ExecuteAicaCommand(0x400abU, 0, 0);
            work.answer = 1;
        }
        if (pressed & 0x8000U) {
            if (work.answer != 0)
                ExecuteAicaCommand(0x400abU, 0, 0);
            work.answer = 0;
        }
        if (pressed & 0x400U) {
            if (work.answer == 1) {
                work.phase = 25;
                ExecuteAicaCommand(0x900abU, 0, 0);
            } else {
                work.phase = work.returnPhase;
                work.returnPhase = -1;
                ExecuteAicaCommand(0x800abU, 0, 0);
            }
        }
        break;
    case 26:
        if (QueueSnapshotMetadataWrite(snapshotDebugMetadata)) {
            work.loadingPhase = 27;
            work.phase = 28;
            CheckHelpIcon(1, 0, CheckHelpIconOp::Remove);
            if (!work.mode) {
                CheckHelpIcon(2, 0, CheckHelpIconOp::Remove);
                CheckHelpIcon(256, 0, CheckHelpIconOp::Remove);
                CheckHelpIcon(64, 0, CheckHelpIconOp::Remove);
            }
            SnapshotDebugMissingMetadataNullsub();
            if (work.sort != 3) {
                SortSnapshotDebugRecords(work.sort);
                if (work.returnPhase == 8)
                    work.returnPhase = 6;
            }
        } else
            work.phase = 27;
        break;
    case 27: {
        std::int32_t count = 48;
        if (work.sort == 3)
            work.selected = work.selectedBiography;
        else {
            SortSnapshotDebugRecords(work.sort);
            if (work.returnPhase == 8)
                work.returnPhase = 6;
            count = work.count;
        }
        work.phase = work.returnPhase;
        work.returnPhase = -1;
        work.suppressContent = 0;
        work.visible = count - work.first < 9 ? count - work.first : 9;
        work.drawnSelectionKind = work.selectionKind;
        work.drawnPage = work.page;
        work.drawnSelected = work.selected;
        work.drawnSort = work.sort;
        work.drawnFirst = work.first;
        work.drawnVisible = work.visible;
        work.drawnColumn = work.column;
        work.drawnRow = work.row;
        FinishLoading();
        if (!work.mode)
            CheckHelpIcon(2, 0, CheckHelpIconOp::Set);
        break;
    }
    case 28: {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (IsScreenFilmNotShowing(control->screenFilmTask) && (pressed & 0xc00U)) {
            work.phase = work.returnPhase;
            work.suppressContent = 0;
            if (!work.mode)
                work.loadingPhase = 0;
            ExecuteAicaCommand(0x900abU, 0, 0);
            FinishLoading();
        }
        break;
    }
    case 29: {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        IsScreenFilmNotShowing(control->screenFilmTask);
        break;
    }
    case 30:
        if (--work.waitFrames < 0)
            work.phase = 31;
        break;
    case 31:
        FinishSnapshotDebug();
        break;
    default:
        break;
    }
    goto advanceFrame;
capturePage:
    work.visible = work.count - work.first < 9 ? work.count - work.first : 9;
    work.drawnSelectionKind = work.selectionKind;
    work.drawnPage = work.page;
    work.drawnSelected = work.selected;
    work.drawnSort = work.sort;
    work.drawnFirst = work.first;
    work.drawnVisible = work.visible;
    work.drawnColumn = work.column;
    work.drawnRow = work.row;
    if (!work.mode)
        CheckHelpIcon(2, 0, CheckHelpIconOp::Set);
    goto advanceFrame;
captureImage:
    if (work.sort == 3)
        goto captureBiography;
captureCount:
    work.visible = work.count - work.first < 9 ? work.count - work.first : 9;
    goto captureSelection;
captureBiography:
    work.visible = 48 - work.first < 9 ? 48 - work.first : 9;
captureSelection:
    work.drawnSelectionKind = work.selectionKind;
    work.drawnPage = work.page;
    work.drawnSelected = work.selected;
    work.drawnSort = work.sort;
    work.drawnFirst = work.first;
    work.drawnVisible = work.visible;
    work.drawnColumn = work.column;
    work.drawnRow = work.row;
advanceFrame:
    snapshotDebugPulse = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(snapshotDebugPulse + snapshotDebugPulseStep));
    if (snapshotDebugPulse <= 0) {
        snapshotDebugPulse = 0;
        snapshotDebugPulseStep = 7;
    } else if (snapshotDebugPulse >= 127) {
        snapshotDebugPulse = 127;
        snapshotDebugPulseStep = -7;
    }
    if (static_cast<std::uint32_t>(work.drawnPhase) - 24U > 3U && work.drawnPhase != 29)
        ++work.frames;
}
void SnapshotDebugCallback(void *parameter) {
    SnapshotDebugWork &work = *static_cast<SnapshotDebugWork *>(parameter);
    UpdateSnapshotDebug(work);
    RenderSnapshotDebug(work);
}
std::optional<std::uint64_t> EnqueueSnapshotDebug(std::int32_t mode, HLib::Task *waitingTask) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SnapshotDebugCallback, 0x0f, 0x0b, sizeof(SnapshotDebugWork), SNAPSHOT_DEBUG_TASK);
    SnapshotDebugWork *const work = static_cast<SnapshotDebugWork *>(HLib::GetTaskParameter(task));
    *work = {};
    work->mode = mode;
    work->waitingTask = waitingTask;
    work->previousHelp = helpGetCurrentIconIdx(1);
    snapshotDebugTask = task;
    if (!snapshotDebugInitialized) {
        InitializeSnapshotViewer();
        if (!snapshotDebugInitialized) {
            work->loadingPhase = 28;
            work->phase = 28;
            CheckHelpIcon(1, 0, CheckHelpIconOp::Remove);
            if (!work->mode) {
                CheckHelpIcon(2, 0, CheckHelpIconOp::Remove);
                CheckHelpIcon(256, 0, CheckHelpIconOp::Remove);
                CheckHelpIcon(64, 0, CheckHelpIconOp::Remove);
            }
            SnapshotDebugMissingMetadataNullsub();
            work->returnPhase = mode ? 6 : 2;
        }
    }
    snapshotDebugWork = work;
    const std::optional<std::uint64_t> result = DispatchPostFxFilterRender(0);
    if (mode == 1) {
        InitShotsImages();
        LoadAreaSpriteImages();
        SetPlayerControlEnabled(0);
        EV_DisableCharacterDisplay(SNAPSHOT_DEBUG_PLAYER);
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        EV_SetObjectFlagByIndex(0, 0);
        SetWeatherUpdateEnabledTo(0);
        SetIsSkySetup(0);
        shenSetTimeSimulationTo(0);
        SetRenderClockAndCompassTo(1);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit22, reinterpret_cast<void *>(std::uintptr_t{1}));
        void *const allocation = LoadLegacySceneFile("sprite", "SHOTSVW.SPR", SNAPSHOT_DEBUG_TASK);
        if (allocation) {
            const std::byte *bytes = static_cast<const std::byte *>(allocation);
            std::uint32_t signature;
            std::memcpy(&signature, bytes, 4);
            std::int32_t count = 0;
            while (signature == SHEN_NAME(TEXN)) {
                snapshotDebugSprites[count++] = static_cast<std::int32_t>(ReadTexnTexture(bytes));
                std::uint32_t length;
                std::memcpy(&length, bytes + 4, 4);
                bytes += length;
                std::memcpy(&signature, bytes, 4);
            }
            snapshotDebugSpriteCount = count;
            HLib::FreeTaskData(allocation);
        } else
            snapshotDebugSpriteCount = -1;
        InitializeSnapshotDebugTextResources();
        EV_LoadAudio("0002S000", 0);
    }
    return result;
}
std::int64_t BeginSnapshotDebug() {
    if (snapshotDebugTask) {
        const auto resumed = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(snapshotDebugTask->delayedCallback));
        HLib::ResolveDeferredCallback(snapshotDebugTask);
        return resumed;
    }
    const std::optional<std::uint64_t> result = EnqueueSnapshotDebug(0, nullptr);



    if (!result)
        throw std::logic_error("Unsupported native STVW return: unchanged postfx dispatch exposes stack-cookie EAX");
    return static_cast<std::uint32_t>(*result);
}
} // namespace ui_detail
// @verified - p
void InitializeSnapshotViewer() {
    snapshotDebugStorage = {};
    if (!snapshotDebugInitialized)
        snapshotDebugInitialized = 1;
    snapshotDebugWork = nullptr;
    snapshotDebugPulse = 0;
    snapshotDebugPulseStep = 7;
    d3t::SetSceneClearColour(0xff000000U);
}
// @verified - p
void RunSnapshotViewer(std::uintptr_t) {
    EnqueueSnapshotDebug(1, nullptr);
}
// @verified - p
std::int64_t CloseSnapshotViewer(std::int32_t mode) {
    if (mode != 1)
        return 0;
    FinishSnapshotDebug();
    return 1;
}

// @verified - p
std::int64_t CheckTextureData() {
    return snapshotDebugWork != nullptr;
}

// @verified - p
void ReinitTextureData() {
    if (snapshotDebugInitialized == 1) {
        snapshotDebugInitialized = 0;


        for (std::size_t index = 0; index < 125; ++index) {
            std::uint32_t texture{};
            std::memcpy(&texture, snapshotDebugStorage.images.data() + 0x20 + index * 0x24, sizeof(texture));
            if (texture)
                d3t::InitTexture2DResource(texture);
        }
        for (std::uint32_t texture : snapshotDebugThumbnails)
            d3t::InitTexture2DResource(texture);
    }
    snapshotDebugTask = nullptr;
    snapshotDebugWork = nullptr;
}

std::int64_t EnableSnapshotViewer() {
    DestroyElementByIndex(UIElement::SnapshotViewer);
    RefreshSnapshotOrder();
    SetUIGridDictionary(collectionChild[5].grid, static_cast<std::int32_t>(snapshotOrderCount), reinterpret_cast<const std::int32_t *>(snapshotOrder.data()));
    return snapshotOrderCount;
}

bool ShowSnapshotViewer(std::uint32_t slot) {
    if (slot >= snapshotImages.size() || GetSnapshotMetadataOwner()->records[slot].use != 1 || snapshotImages[slot].state.load(std::memory_order_acquire) != 2)
        return false;
    RefreshSnapshotOrder();
    for (std::uint32_t index = 0; index < snapshotOrderCount; ++index) {
        if (snapshotOrder[index] != slot)
            continue;
        OpenCollectionSnapshotViewer(std::span{snapshotOrder.data(), snapshotOrderCount}, index, static_cast<UIElement>(0), true);
        return IsHelpOverlayVisible(UIElement::SnapshotViewer);
    }
    return false;
}
} // namespace shenmue::s2
