#include "s2/scene_data.hpp"

#include "s2/archive_data.hpp"
#include "s2/cold_archive.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/input_bindings.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <new>
#include <span>
#include <sstream>
#include <string>
#include <vector>


namespace shenmue::s2 {

SceneTextHeap* g_sceneTextHeap = nullptr;
std::uint64_t g_sceneTextHeapCursor = 0;
std::byte* g_lcdMemory = nullptr;
std::uint32_t g_lcdMemorySize = 0;
std::byte* g_systemTalkData = nullptr;
std::byte* g_weatherData = nullptr;
std::uint32_t g_weatherDataSize = 0;
std::uint32_t* g_weatherCloudTimelineNodes = nullptr;
std::uint32_t* g_weatherRainTimelineNodes = nullptr;
std::uint32_t* g_weatherSnowTimelineNodes = nullptr;
std::int32_t g_weatherCloudTimelineNodeCount = 0;
std::int32_t g_weatherRainTimelineNodeCount = 0;
std::int32_t g_weatherSnowTimelineNodeCount = 0;
std::array<std::byte, 0xf60> g_weatherConditions{};
std::byte* g_flagDescriptorData = nullptr;
std::uint64_t g_flagDescriptorDataSize = 0;
std::uint16_t* g_flagWork = nullptr;
std::byte* g_memoData = nullptr;
std::byte* g_memoMasterTable = nullptr;
std::byte* g_memoRubyTable = nullptr;
std::uint32_t g_memoRecordCount = 0;
std::int32_t g_sceneTextLanguage = 0;
bool g_sceneTextAvailable = false;
std::int32_t g_s2JapaneseVersion = 0;
bool g_s2AllSubtitleLanguages = false;
std::int32_t g_s2SelectedSubtitleLanguage = 0;

namespace {

struct SceneMemoryEntry {
    const void* owner;
    void* data;
    std::int32_t retained;
    std::int32_t size;
};
static_assert(sizeof(SceneMemoryEntry) == 0x18);
static_assert(offsetof(SceneMemoryEntry, data) == 8);
static_assert(offsetof(SceneMemoryEntry, retained) == 0x10);
static_assert(offsetof(SceneMemoryEntry, size) == 0x14);
static_assert(sizeof(HLib::TaskData) == 0x40);

std::array<SceneMemoryEntry, 1000> sceneMemoryEntries{};
std::array<SceneMemoryEntry, 1500> modelVertexMemoryEntries{};
std::int32_t sceneMemoryCount{};
std::int32_t sceneMemoryBytes{};
std::int32_t sceneMemoryPeak{};
std::int32_t modelVertexMemoryCount{};
std::int32_t modelVertexMemoryBytes{};
std::int32_t modelVertexMemoryPeak{};
void* sceneMemoryArena{};
std::int32_t sceneMemoryArenaSize{};
std::int32_t sceneAllocationCount{};
HLib::TaskData* sceneMemoryFirst{};
HLib::TaskData* sceneMemoryLast{};
constexpr std::uint32_t sceneMemoryUsed = make_shen_name('U', 'S', 'E', ' ');
constexpr std::uint32_t sceneMemoryFree = SHEN_NAME(fREe);

constexpr shen_name kSceneTextHeap{
    SHEN_NAME(STCH)};
constexpr shen_name kLcdMemory{
    SHEN_NAME(LCDM)};



constexpr char kActionSelectorSuffixes[] =
    "DUMMMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSMISSCHALDOPLDOCA"
    "TRY0DOTRMISSMISSMISSMISSMISSMISSMISSQUITMISSPLAYCONTDO00DONTBUY0DOBUMISSMISSMISSMISSMISSMISSMISSTEFO"
    "MISSMISSMISSMISSMISSMISSMISSMISSMISSMISSDECLNO00ACCEMISSMISSMISSRIDEMISSRIGHLEFTPAY0DOPALISTNOLIYES0"
    "NO00PRMUFAIRNOYEPROBBUY0DOBUYES0NO00GUIDNOGUTAPHDOTKCUP0BOWLYES0NO00VEGOGOODSOSONOGOPA5DGOBADGB0GIMO"
    "DOGIANYTVEGDMEATWHTANOZDLADIGUIHIGS0NOCETRUTREVEIWT0BEAUVESTISSHYOTONOTINO00NOTHLIMALOGGNORMLOGOWACL"
    "TBH0NOHAI'PRNFS0WHTHSORRPRETFANMTHALILT0METOIDK0ALM0YES0YES0IWF0NWD0ITHOOFCOCOAGDRT1DRT2QTE1QTE2WAIT"
    "RTS0PRACCONTNO00QUITDOWASPARYES0KNOWDOKNGOINDGI0WATCDOWTBAAGABERWOPIFOPIBHW0MMT0WIMEDYA0SOCAGRMAWACH"
    "TIRENOTHANTRREALBAVIGUILAGE0SIBLTEACEVADMISTPHMICATAFATHYO00SCARJAPADOTCREVESEARFUKUNOONITO0TOM0GUIZ"
    "PUINDPI0SELLDOSEITEMCOLLNADAAGONUZUNPARKPUSHRIDE10D020D030D0100D200D300DSHOWDOSHNOZOTATOPASSDOTEGORO"
    "GIVEDOGICROWDOG0CAT0TABIHEADTAILFRYELISTLTS0TALKSET0SINGGON0DAN0JIE0YI00ZHONREN0XIAOXIN0LI00YAN0KOEN"
    "STANDRSTSTBYFRHECOG0PHARDOJIMAARLUCHGUSCSTPESUSHLOPAJIANZHSHTHBLDOTS5D00MAMO50D0RIVEWATEOCEAPOBRPOSH"
    "GOLDFIGHGAMBPAWNPAJODARTARWRSTFIDIGALUHICACRSHOPSHMAANSHCOLTKIRAMAANBISTRESTLLE0LILIFHD0FORTCACAHETE"
    "BRTEMMR0MMB0NITEEVERAPRIDRSPTHWHOPTATSS0TEBRDIBLMOCHHEAVGAHOBLMA8000REN0Z000SAM0LARRYUANBERIFJT0EATE"
    "HKT0GOTEGUT0NOYEDOTLRAFIMTFUSAMUNEGOINTRTAG0ARCABASETHIEPRIDLUCKOFGUTHWHKNSHORPHBLSKHTS0ROD0GREGCHUN"
    "BEAUSTRODIFFTHISYOCAAIR0STREMAATPEOPMEALROOMMEATVEGESTRGENEMSO-SNICOPOCUSTMERULETRHUREVEAMMOYUZHWULI"
    "KAPPRACCINGOOLFRWOMAFLORCATOENTRXIUYBJS0HTS0HKT0ABERTHBLFORESHENAHEAANIMDANGHUONYEFLDETOLANDGO00DOGO"
    "MMP0WMK0OTSTLEGEVILLFLOWMOPASHDALIFEFAMIMEMOGOL0SHHOLIFETHVIWEATSHTRHOTOCAANJAMTFON0FTN0ALAMDRAGTIGE"
    "BIRDTURTDOSLSLEEROARARBLCAMAPROMGREEWONGFANGJOY0PIPAHIDEDOHIDOWOIMAGYES0NO00LOVEMONETRHADEOGCUTEBEAU"
    "MASTID00NO00THSODTS0AWHIFORVHERBFLOWFRWOFRSTCATCCOMPTHPAPATHOTANFOX0CARPCOCKLANSMOHADOWNWINDDRWADOCT"
    "STLIFLOAWATFTOVITIRQPADD";
constexpr std::array<std::string_view, 5> kActionSelectorFinalIds{
    "UIMENUSAVE", "UIMAINOPTS", "UIBUTTCANC", "UIACTSSLEE", // @text UIMENUSAVE EN: "Save" | JA: "記録" ; UIMAINOPTS EN: "Options" | JA: "設定" ; UIBUTTCANC EN: "Cancel" | JA: "キャンセル" ; UIACTSSLEE EN: "Sleep" | JA: "寝る"
    "UIASBETMAX"}; // @text EN: "MAX" | JA: "MAX"
static_assert(sizeof(kActionSelectorSuffixes) - 1 == 481 * 4);

std::vector<std::byte> sceneTextData;
std::int32_t sceneTextVersion = 0;
std::int32_t sceneTextRecordCount = 0;
std::size_t sceneTextRecordsOffset = 0;
std::size_t sceneTextRecordStride = 0;
std::size_t sceneTextStringsOffset = 0;


std::vector<std::array<std::byte, 12>> sceneTextJapaneseRemap;


std::array<SceneTextCsvEntry, kSceneTextCsvOverrideCapacity> sceneTextCsvOverride{};
UIStringBuffer uiStringBuffer{};
template <typename Value>
bool ReadValue(std::span<const std::byte> data, std::size_t offset,
               Value& value);


const char* SubsPathForLanguage(std::int32_t language) {
    switch (language) {
    case 1: return "/subs/japanese.sub";
    case 2: return "/subs/french.sub";
    case 3: return "/subs/german.sub";
    case 4: return "/subs/korean.sub";
    case 5: return "/subs/chineset.sub";
    case 6: return "/subs/chineses.sub";
    default: return "/subs/english.sub";
    }
}

void StringBuffer_Init(UIStringBuffer& buffer, char* base, std::uint32_t capacity) {
    buffer.base = base;
    buffer.used = 0;
    buffer.capacity = capacity;
}

// The pooled offset of a copy of `text` with its terminator, or 0xffffffff when the pool is full.
std::uint32_t StringBuffer_Append(UIStringBuffer& buffer, const char* text) {
    const std::uint32_t length = static_cast<std::uint32_t>(std::strlen(text) + 1);
    const std::uint32_t used = buffer.used;
    const std::uint32_t next = used + length;
    if (static_cast<std::uint64_t>(used) + length > buffer.capacity || next < used) return 0xffffffffU;
    buffer.used = next;
    std::memcpy(buffer.base + used, text, length);
    return used;
}

const char* StringBuffer_Get(const UIStringBuffer& buffer, std::uint32_t offset) {
    return offset < buffer.used ? buffer.base + offset : nullptr;
}


const char* GetPathExtension(const char* path) {
    std::size_t length = std::strlen(path);
    while (length > 1 && static_cast<unsigned char>(path[length - 1] - '.') > 1U)
        --length;
    return length && path[length - 1] == '.' ? path + length : nullptr;
}



bool SceneTextFileExists(const char* path) {
    while (*path == '/' || *path == '\\')
        ++path;
    std::error_code error;
    return std::filesystem::is_regular_file(path, error);
}




void LoadSceneTextCsvOverride(const char* fileName) {
    StringBuffer_Init(uiStringBuffer, static_cast<char*>(std::malloc(0x500000)), 5242880);
    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    if (!std::getline(file, line)) return;
    SceneTextCsvEntry* entry = sceneTextCsvOverride.data();
    do {
        std::stringstream stream(line);
        std::string key;
        key.reserve(8);
        std::getline(stream, key, ';');
        std::memcpy(entry->key.data(), key.data(), entry->key.size());
        std::string text;
        std::getline(stream, text);
        entry->textOffset = StringBuffer_Append(uiStringBuffer, text.c_str());
        ++entry;
    } while (entry != sceneTextCsvOverride.data() + sceneTextCsvOverride.size() && std::getline(file, line));
}


void BuildSceneTextJapaneseRemap() {
    if (!sceneTextJapaneseRemap.empty()) return;
    for (std::int32_t index = 0; index < sceneTextRecordCount; ++index) {
        const std::size_t record = sceneTextRecordsOffset + static_cast<std::size_t>(index) * 28;
        if (record + 14 > sceneTextData.size()) break;
        if (sceneTextData[record] != std::byte{'Z'} || sceneTextData[record + 1] != std::byte{'Z'}) continue;
        std::array<std::byte, 12> entry;
        std::memcpy(entry.data(), sceneTextData.data() + record + 2, entry.size());
        sceneTextJapaneseRemap.push_back(entry);
    }
}




const char* FindSceneTextKey(const std::byte* key) {
    if (!g_sceneTextAvailable) {
        for (const SceneTextCsvEntry& entry : sceneTextCsvOverride) {
            if (std::memcmp(entry.key.data(), key, entry.key.size()) == 0) return StringBuffer_Get(uiStringBuffer, entry.textOffset);
        }
        return "";
    }
    if (sceneTextVersion < 1 || sceneTextVersion > 3) return "";
    const std::size_t keyBytes = sceneTextVersion == 1 ? 8U : 12U;
    const std::size_t offsetField = sceneTextVersion == 1 ? 8U : sceneTextVersion == 2 ? 12U : 24U;
    for (std::int32_t index = 0; index < sceneTextRecordCount; ++index) {
        const std::size_t record = sceneTextRecordsOffset + static_cast<std::size_t>(index) * sceneTextRecordStride;
        if (std::memcmp(sceneTextData.data() + record, key, keyBytes) != 0) continue;
        std::uint32_t textOffset = 0;
        if (!ReadValue(std::span<const std::byte>{sceneTextData}, record + offsetField, textOffset) ||
            textOffset >= sceneTextData.size() - sceneTextStringsOffset) return "";
        return reinterpret_cast<const char*>(sceneTextData.data() + sceneTextStringsOffset + textOffset);
    }
    return "";
}

std::string_view FindSceneText(std::string_view key) {
    if (!g_sceneTextAvailable || key.empty() || key.size() > 12) return {};
    const std::span<const std::byte> data{sceneTextData};
    const std::size_t recordKeyBytes = sceneTextVersion == 1 ? 8U : 12U;
    for (std::int32_t index = 0; index < sceneTextRecordCount; ++index) {
        const std::size_t record = sceneTextRecordsOffset +
            static_cast<std::size_t>(index) * sceneTextRecordStride;
        bool matches = true;
        for (std::size_t byte = 0; byte < recordKeyBytes; ++byte) {
            unsigned char wanted = byte < key.size()
                ? static_cast<unsigned char>(key[byte]) : 0;
            if (wanted >= 'a' && wanted <= 'z') {
                wanted = static_cast<unsigned char>(wanted - 'a' + 'A');
            }
            if (std::to_integer<unsigned char>(data[record + byte]) != wanted) {
                matches = false;
                break;
            }
        }
        if (!matches) continue;
        std::uint32_t textOffset = 0;
        const std::size_t offsetField = record +
            (sceneTextVersion == 1 ? 8 :
             sceneTextVersion == 2 ? 12 : 24);
        if (!ReadValue(data, offsetField, textOffset) ||
            textOffset > data.size() - sceneTextStringsOffset) {
            return {};
        }
        const char* const text = reinterpret_cast<const char*>(
            data.data() + sceneTextStringsOffset + textOffset);
        const std::size_t remaining =
            data.size() - sceneTextStringsOffset - textOffset;
        const void* const end = std::memchr(text, '\0', remaining);
        if (!end || end == text) return {};
        return {text, static_cast<std::size_t>(
            static_cast<const char*>(end) - text)};
    }
    return {};
}

template <typename Value>
bool ReadValue(std::span<const std::byte> data, std::size_t offset,
               Value& value) {
    if (offset > data.size() || sizeof(Value) > data.size() - offset) {
        return false;
    }
    std::memcpy(&value, data.data() + offset, sizeof(Value));
    return true;
}

std::byte* LoadMiscFile(const char* filename, shen_name allocationName,
                        bool persistent, std::uint64_t* loadedSize = nullptr) {
    std::array<char, 96> path{};
    const int length = std::snprintf(path.data(), path.size(), "/misc/%s",
                                     filename);
    if (length <= 0 || static_cast<std::size_t>(length) >= path.size()) {
        return nullptr;
    }
    const std::uint64_t size = ArchiveData_GetFileSize(path.data());
    if (!size || size > std::numeric_limits<std::size_t>::max()) {
        return nullptr;
    }
    auto* const data = static_cast<std::byte*>(persistent
        ? HLib::AllocateStaticOrDynamicBlock(size, allocationName)
        : HLib::AllocateTaskData(size, allocationName));
    if (!data || !ArchiveData_Read(path.data(), 0, data, size)) {
        if (data) {
            HLib::FreeTaskData(data);
        }
        return nullptr;
    }
    if (loadedSize) {
        *loadedSize = size;
    }
    return data;
}

struct WeatherChunk {
    std::uint32_t name;
    std::uint32_t nextByteOffset;
    std::uint32_t firstValue;
};

static_assert(sizeof(WeatherChunk) == 0x0c);

WeatherChunk* FindWeatherChunk(const char name[4],
                               std::span<const std::byte> data) {
    if (!name || data.size() < sizeof(WeatherChunk)) {
        return nullptr;
    }
    std::uint32_t wanted = 0;
    std::memcpy(&wanted, name, sizeof(wanted));
    std::size_t offset = 0;
    for (;;) {
        auto* const chunk = reinterpret_cast<WeatherChunk*>(
            const_cast<std::byte*>(data.data()) + offset);
        if (chunk->name == wanted) {
            return chunk;
        }
        if (!chunk->nextByteOffset) {
            return nullptr;
        }
        const std::uint64_t advance =
            static_cast<std::uint64_t>(chunk->nextByteOffset);
        if (advance < 8 ||
            advance > data.size() - offset) {
            return nullptr;
        }
        offset += static_cast<std::size_t>(advance);
        if (data.size() - offset < sizeof(WeatherChunk)) {
            return nullptr;
        }
    }
}

bool BindWeatherTimeline(const char name[4],
                         std::span<const std::byte> data,
                         std::uint32_t*& nodes, std::int32_t& count) {
    WeatherChunk* const chunk = FindWeatherChunk(name, data);
    if (!chunk || chunk->nextByteOffset < 8) {
        nodes = nullptr;
        count = 0;
        return false;
    }
    nodes = &chunk->firstValue;
    count = static_cast<std::int32_t>(chunk->nextByteOffset / 4U - 2U);
    return true;
}

} // namespace

// @verified - p
std::int64_t HLib::GetFreeHeapStats(std::uint64_t* totalFreeSpace, std::uint64_t* largestFree) {
    std::uint32_t total = 0;
    std::uint64_t largest = 0;
    for (TaskData* entry = sceneMemoryFirst; entry;) {
        if ((reinterpret_cast<std::uintptr_t>(entry + 1) & 31U) != 0) break;
        if (entry->status.name != sceneMemoryUsed || entry->name.name == 0) {
            const std::uint64_t usable = entry->contentSize - 64;
            total += static_cast<std::uint32_t>(usable);
            if (largest < usable) largest = usable;
        }
        entry = entry->next;
        if (entry == sceneMemoryLast) break;
    }
    *totalFreeSpace = total;
    *largestFree = static_cast<std::uint32_t>(largest);
    return 0;
}

// @verified - p
std::int64_t HLib::TryGetFreeHeapStats(std::uint64_t* totalFreeSpace, std::uint64_t* largestFree) {
    const std::int64_t result = GetFreeHeapStats(totalFreeSpace, largestFree);
    if (static_cast<std::uint32_t>(result) != 0) {
        *largestFree = 0;
        *totalFreeSpace = 0;
        return 0;
    }
    return result;
}

// @verified - p
void* HLib::AnalyzeFreeList(std::uint64_t* totalUsable, std::uint64_t* largestBlock, std::uint64_t* fragmentationOut) {
    std::uint64_t total = 0;
    std::uint64_t largest = 0;
    void* largestData = nullptr;
    for (TaskData* entry = sceneMemoryFirst; entry;) {
        void* const data = entry + 1;
        if ((reinterpret_cast<std::uintptr_t>(data) & 31U) != 0) break;
        if (entry->status.name != sceneMemoryUsed || entry->name.name == 0) {
            const std::uint64_t usable = entry->contentSize - sizeof(TaskData);
            total += usable;
            if (largest < usable) { largest = usable; largestData = data; }
        }
        entry = entry->next;
        if (entry == sceneMemoryLast) break;
    }
    *totalUsable = total;
    *largestBlock = largest;
    *fragmentationOut = total - largest;
    return largestData;
}

std::int64_t AllocateHLibMemory_USE_Space(std::int32_t size) {
    if (size <= 0) size = 0x1e00000;
    sceneMemoryArena = ::operator new(static_cast<std::size_t>(size), std::align_val_t{32}, std::nothrow);
    sceneMemoryArenaSize = size;
    if (!sceneMemoryArena) return 0;
    std::memset(sceneMemoryArena, 0, static_cast<std::size_t>(size) & ~std::size_t{3});
    HLib::TaskData* const root = static_cast<HLib::TaskData*>(sceneMemoryArena);
    sceneMemoryFirst = root + 1;
    sceneMemoryLast = reinterpret_cast<HLib::TaskData*>(static_cast<std::byte*>(sceneMemoryArena) + size - 0x40);
    root->name.name = SHEN_NAME(HLib);
    root->status.name = sceneMemoryUsed;
    root->alignment = 128;
    root->contentSize = 0;
    root->previous = nullptr;
    root->next = sceneMemoryFirst;
    sceneMemoryFirst->name.name = sceneMemoryFree;
    sceneMemoryFirst->status.name = sceneMemoryFree;
    sceneMemoryFirst->alignment = 0;
    sceneMemoryFirst->contentSize = static_cast<std::uint64_t>(size - 0x80);
    sceneMemoryFirst->previous = root;
    sceneMemoryFirst->next = sceneMemoryLast;
    sceneMemoryLast->name.name = SHEN_NAME(HLib);
    sceneMemoryLast->status.name = sceneMemoryUsed;
    sceneMemoryLast->alignment = 128;
    sceneMemoryLast->contentSize = 0;
    sceneMemoryLast->previous = sceneMemoryFirst;
    sceneMemoryLast->next = nullptr;
    sceneAllocationCount = 0;
    return 1;
}

std::int64_t InitializeHLibMemory(std::int32_t size) {
    sceneMemoryEntries = {};
    modelVertexMemoryEntries = {};
    sceneMemoryCount = sceneMemoryBytes = sceneMemoryPeak = 0;
    modelVertexMemoryCount = modelVertexMemoryBytes = modelVertexMemoryPeak = 0;
    return AllocateHLibMemory_USE_Space(size);
}

std::byte* AllocateVMPL(std::uint64_t size) {
    if (size > 32768) {
        HLib::TaskData* entry = sceneMemoryLast;
        const std::uint64_t required = (size + 95) & ~std::uint64_t{31};
        for (;;) {
            if (!entry) return nullptr;
            if (entry->status.name != sceneMemoryUsed && entry->contentSize >= required) break;
            entry = entry->previous;
        }
        const std::uint64_t available = entry->contentSize;
        ++sceneAllocationCount;
        if (available <= required + 64) {
            entry->name.name = SHEN_NAME(VMPL);
            entry->status.name = sceneMemoryUsed;
            entry->alignment = 4;
            return reinterpret_cast<std::byte*>(entry + 1);
        }
        const std::uint64_t remaining = available - required;
        HLib::TaskData* const allocated = reinterpret_cast<HLib::TaskData*>(reinterpret_cast<std::byte*>(entry) + remaining);
        allocated->previous = entry;
        allocated->next = entry->next;
        entry->next->previous = allocated;
        entry->next = allocated;
        entry->contentSize = remaining;
        allocated->name.name = SHEN_NAME(VMPL);
        allocated->status.name = sceneMemoryUsed;
        allocated->contentSize = required;
        allocated->alignment = 4;
        return reinterpret_cast<std::byte*>(allocated + 1);
    }
    HLib::TaskData* entry = sceneMemoryFirst;
    if (size < 32) size = 32;
    const std::uint64_t required = (size + 95) & ~std::uint64_t{31};
    for (;;) {
        if (!entry) return nullptr;
        if (entry->status.name != sceneMemoryUsed && entry->contentSize >= required) break;
        entry = entry->next;
    }
    const std::uint64_t available = entry->contentSize;
    ++sceneAllocationCount;
    if (available <= required + 64) {
        entry->name.name = SHEN_NAME(VMPL);
        entry->status.name = sceneMemoryUsed;
        entry->alignment = 0;
        return reinterpret_cast<std::byte*>(entry + 1);
    }
    HLib::TaskData* const remaining = reinterpret_cast<HLib::TaskData*>(reinterpret_cast<std::byte*>(entry) + required);
    remaining->previous = entry;
    remaining->next = entry->next;
    remaining->status.name = sceneMemoryFree;
    remaining->name.name = sceneMemoryFree;
    remaining->contentSize = available - required;
    entry->next->previous = remaining;
    entry->next = remaining;
    entry->name.name = SHEN_NAME(VMPL);
    entry->status.name = sceneMemoryUsed;
    entry->alignment = 0;
    entry->contentSize = required;
    return reinterpret_cast<std::byte*>(entry + 1);
}

std::int64_t FreeSceneBuffer(void* data) {
    if (!data) return 0;
    HLib::TaskData* entry = static_cast<HLib::TaskData*>(data) - 1;
    if (entry->status.name != sceneMemoryUsed || ((entry->alignment & 0x10) && entry->name.name)) return 0;
    HLib::TaskData* const previous = entry->previous;
    HLib::TaskData* const next = entry->next;
    std::uint64_t size = entry->contentSize;
    entry->status.name = sceneMemoryFree;
    entry->name.name = sceneMemoryFree;
    if (previous->status.name == sceneMemoryFree) {
        size += previous->contentSize;
        entry = previous;
        previous->next = next;
    }
    next->previous = entry;
    if (next->status.name == sceneMemoryFree) {
        size += next->contentSize;
        entry->next = next->next;
        next->next->previous = entry;
    }
    --sceneAllocationCount;
    entry->contentSize = size;
    return 1;
}

std::int64_t AllocateModelVertexMemory(std::int32_t size, const void* modelRoot, void** data) {
    if (!data) return 0;
    for (SceneMemoryEntry& entry : modelVertexMemoryEntries) {
        if (entry.owner) continue;
        entry.owner = modelRoot;
        entry.data = AllocateVMPL(static_cast<std::uint64_t>(static_cast<std::int64_t>(size)));
        entry.retained = 0;
        entry.size = size;
        *data = entry.data;
        if (!entry.data) return 0;
        ++modelVertexMemoryCount;
        modelVertexMemoryBytes += size;
        if (modelVertexMemoryBytes > modelVertexMemoryPeak) modelVertexMemoryPeak = modelVertexMemoryBytes;
        return 1;
    }
    *data = nullptr;
    return 0;
}

std::int64_t FreeModelVertexMemory(const void* modelRoot) {
    for (SceneMemoryEntry& entry : modelVertexMemoryEntries) {
        if (entry.owner != modelRoot) continue;
        const std::int32_t size = entry.size;
        if (entry.data) FreeSceneBuffer(entry.data);
        entry = {};
        --modelVertexMemoryCount;
        modelVertexMemoryBytes -= size;
        if (modelVertexMemoryBytes > modelVertexMemoryPeak) modelVertexMemoryPeak = modelVertexMemoryBytes;
        return 1;
    }
    return 0;
}

std::int64_t InitializeSceneData() {
    for (std::size_t index = sceneMemoryEntries.size(); index-- > 0;) {
        SceneMemoryEntry& entry = sceneMemoryEntries[index];
        if (entry.retained) continue;
        const std::int32_t size = entry.size;
        if (entry.data) FreeSceneBuffer(entry.data);
        entry = {};
        sceneMemoryBytes -= size;
    }
    if (sceneMemoryBytes > sceneMemoryPeak) sceneMemoryPeak = sceneMemoryBytes;
    sceneMemoryCount = 0;
    return 1;
}

std::int64_t FreeUnusedSceneData() {
    for (std::size_t index = modelVertexMemoryEntries.size(); index-- > 0;) {
        SceneMemoryEntry& entry = modelVertexMemoryEntries[index];
        if (entry.retained) continue;
        const std::int32_t size = entry.size;
        if (entry.data) FreeSceneBuffer(entry.data);
        entry = {};
        modelVertexMemoryBytes -= size;
    }
    modelVertexMemoryCount = 0;
    if (modelVertexMemoryBytes > modelVertexMemoryPeak) modelVertexMemoryPeak = modelVertexMemoryBytes;
    return 1;
}

bool GetS2IsEnglishVersion() noexcept {
    return g_s2JapaneseVersion != 0;
}

std::span<const std::int32_t> GetS2AvailableSubtitleLanguages() noexcept {
    static constexpr std::array<std::int32_t, 1> japanese{1};
    static constexpr std::array<std::int32_t, 6> international{0, 2, 3, 4, 5, 6};
    static constexpr std::array<std::int32_t, 7> all{0, 1, 2, 3, 4, 5, 6};
    if (g_s2AllSubtitleLanguages) return all;
    return GetS2IsEnglishVersion() ? std::span<const std::int32_t>{japanese}
                                  : std::span<const std::int32_t>{international};
}

std::int32_t GetLoadingScreenTextType(std::int32_t requestedLanguage) noexcept {


    const std::int32_t language = requestedLanguage < 0 ? 0 : requestedLanguage;
    const auto available = GetS2AvailableSubtitleLanguages();
    for (const std::int32_t candidate : available)
        if (candidate == language) return language;
    return available.front();
}

void ApplyS2SubtitleLanguage(std::int32_t requestedLanguage) noexcept {
    const std::int32_t language = GetLoadingScreenTextType(requestedLanguage);
    if (static_cast<std::uint32_t>(language + 1) <= 7U)
        g_s2SelectedSubtitleLanguage = language;
}

std::int32_t ResolveSceneTextLanguage(std::int32_t requestedLanguage) {
    return GetLoadingScreenTextType(requestedLanguage < 0
        ? GetRuntimeUserConfiguration().audio.subtitleLanguage : requestedLanguage);
}

bool LoadLocalizedSceneText(std::int32_t requestedLanguage) {
    g_sceneTextLanguage = requestedLanguage < 0 ? ResolveSceneTextLanguage(-1) : requestedLanguage;
    g_sceneTextAvailable = false;
    sceneTextData.clear();
    sceneTextVersion = 0;
    sceneTextRecordCount = 0;



    const char* const fileName = SubsPathForLanguage(g_sceneTextLanguage);
    if (SceneTextFileExists(fileName) && std::strcmp(GetPathExtension(fileName), ".csv") == 0) {
        LoadSceneTextCsvOverride(fileName);
        return false;
    }

    // OpenFile resolves /subs through the D3T content store: /tex/assets<path>.<hash>.<variant>,
    // hashed over the lowercase path.
    std::array<char, 64> lowercase{};
    std::snprintf(lowercase.data(), lowercase.size(), "%s", SubsPathForLanguage(g_sceneTextLanguage));
    for (char& character : lowercase) {
        if (character >= 'A' && character <= 'Z') character = static_cast<char>(character + ('a' - 'A'));
    }
    std::array<char, 128> contentPath{};
    std::snprintf(contentPath.data(), contentPath.size(), "/tex/assets%s.%08x.%08x", lowercase.data(),
        d3t_HashData(lowercase.data(), static_cast<std::uint32_t>(std::strlen(lowercase.data()))), 0U);
    const char* const path = contentPath.data();
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (size < 12 || size > std::numeric_limits<std::size_t>::max()) {
        return false;
    }
    sceneTextData.resize(static_cast<std::size_t>(size));
    if (!ArchiveData_Read(path, 0, sceneTextData.data(), size)) {
        sceneTextData.clear();
        return false;
    }
    const std::span<const std::byte> data{sceneTextData};
    if (!ReadValue(data, 0, sceneTextVersion) ||
        !ReadValue(data, 4, sceneTextRecordCount) ||
        sceneTextRecordCount < 0) {
        sceneTextData.clear();
        return false;
    }
    switch (sceneTextVersion) {
    case 1:
        sceneTextRecordsOffset = 8;
        sceneTextRecordStride = 12;
        break;
    case 2:
        sceneTextRecordsOffset = 16;
        sceneTextRecordStride = 16;
        break;
    case 3:
        sceneTextRecordsOffset = 16;
        sceneTextRecordStride = 28;
        break;
    default:
        sceneTextData.clear();
        return false;
    }
    const std::size_t count =
        static_cast<std::size_t>(sceneTextRecordCount);
    if (sceneTextRecordsOffset > data.size() ||
        count > (data.size() - sceneTextRecordsOffset) /
                    sceneTextRecordStride) {
        sceneTextData.clear();
        return false;
    }
    sceneTextStringsOffset =
        sceneTextRecordsOffset + count * sceneTextRecordStride;
    BuildSceneTextJapaneseRemap();
    g_sceneTextAvailable = true;
    return true;
}

std::int32_t ResetSceneTextLanguage() {
    if (g_sceneTextLanguage == -1) LoadLocalizedSceneText(0);
    return g_sceneTextLanguage;
}

std::string_view GetActionSelectorLabel(std::uint16_t labelId) {
    if (labelId >= 486) return {};
    std::array<char, 10> key{};
    if (labelId < 481) {
        const char* const suffix =
            kActionSelectorSuffixes + static_cast<std::size_t>(labelId) * 4;
        if (std::memcmp(suffix, "MISS", 4) == 0 ||
            std::memcmp(suffix, "DUMM", 4) == 0) {
            return {};
        }
        std::memcpy(key.data(), "UIACTS", 6);
        std::memcpy(key.data() + 6, suffix, 4);
    } else {
        const std::string_view nativeId =
            kActionSelectorFinalIds[labelId - 481];
        std::memcpy(key.data(), nativeId.data(), key.size());
    }
    return FindSceneText({key.data(), key.size()});
}

const char* FindSceneTextRemapped(const char* key, std::int32_t japaneseMode) {
    const std::byte* const bytes = reinterpret_cast<const std::byte*>(key);
    if (!japaneseMode || sceneTextJapaneseRemap.empty()) return FindSceneTextKey(bytes);
    for (const std::array<std::byte, 12>& entry : sceneTextJapaneseRemap) {
        if (std::memcmp(entry.data(), bytes, entry.size()) != 0) continue;
        std::array<std::byte, 12> alternate{std::byte{'Z'}, std::byte{'Z'}};
        std::memcpy(alternate.data() + 2, bytes, 10);
        return FindSceneTextKey(alternate.data());
    }
    return FindSceneTextKey(bytes);
}

std::string_view GetLocalizedSceneText(std::string_view token) {
    const std::string_view text = FindSceneText(token);
    return text.empty() ? std::string_view{""} : text;
}

SceneTextHeap* AllocateSTCH() {
    g_sceneTextHeap = static_cast<SceneTextHeap*>(
        HLib::AllocateTaskData(sizeof(SceneTextHeap), kSceneTextHeap));
    g_sceneTextHeapCursor = 0;
    return g_sceneTextHeap;
}

void ClearSTCH() {
    // FreeHeapMemPtr. STCH remains allocated;
    // only the current scene-string byte offset is cleared.
    g_sceneTextHeapCursor = 0;
}

std::byte* allocateInGlobalHeap(std::int64_t size) {
    if (!size) return nullptr;
    std::byte* const allocation = g_sceneTextHeap->buffer + g_sceneTextHeapCursor;
    g_sceneTextHeapCursor += (static_cast<std::uint64_t>(size) + 7) & ~std::uint64_t{3};
    const std::uint32_t tag = SHEN_NAME(USSH);
    std::memcpy(allocation, &tag, sizeof(tag));
    return allocation + 4;
}

void SetTaskHeapData(void* allocation) {
    if (allocation) g_sceneTextHeapCursor = static_cast<std::byte*>(allocation) - g_sceneTextHeap->buffer - 4;
}

std::byte* AllocateLCDM() {


    const ColdArchiveSection* const descriptor =
        GetColdArchiveSectionDescriptor(1);
    g_lcdMemorySize = descriptor ? descriptor->size : 0;
    g_lcdMemory = static_cast<std::byte*>(
        CopyColdArchiveSection(1, false, kLcdMemory));
    if (!g_lcdMemory)
        g_lcdMemorySize = 0;
    return g_lcdMemory;
}

std::byte* LoadSystemTalk() {
    g_systemTalkData = LoadMiscFile(
        "systalk.bin", SHEN_NAME(SCST),
        false);
    LoadLocalizedSceneText();
    return g_systemTalkData;
}


bool InitializeWeatherParameters() {
    g_weatherCloudTimelineNodeCount = 0;
    g_weatherRainTimelineNodeCount = 0;
    g_weatherSnowTimelineNodeCount = 0;
    g_weatherData = nullptr;
    g_weatherDataSize = 0;
    g_weatherCloudTimelineNodes = nullptr;
    g_weatherRainTimelineNodes = nullptr;
    g_weatherSnowTimelineNodes = nullptr;

    const std::span<const std::byte> source = GetColdArchiveSection(2);
    if (source.empty() || source.size() > UINT32_MAX) {
        return false;
    }
    g_weatherData = static_cast<std::byte*>(CopyColdArchiveSection(
        2, false, SHEN_NAME(WEDT)));
    if (!g_weatherData) {
        return false;
    }
    g_weatherDataSize = static_cast<std::uint32_t>(source.size());
    const std::span<const std::byte> data{g_weatherData, g_weatherDataSize};
    const WeatherChunk* const wdat = FindWeatherChunk("WDAT", data);
    const bool cloud = BindWeatherTimeline(
        "CLUD", data, g_weatherCloudTimelineNodes,
        g_weatherCloudTimelineNodeCount);
    const bool rain = BindWeatherTimeline(
        "RAIN", data, g_weatherRainTimelineNodes,
        g_weatherRainTimelineNodeCount);
    const bool snow = BindWeatherTimeline(
        "SNOW", data, g_weatherSnowTimelineNodes,
        g_weatherSnowTimelineNodeCount);
    return wdat && cloud && rain && snow;
}

bool LoadWeatherConditionData() {
    std::uint64_t size = 0;
    std::byte* const source = LoadMiscFile(
        "W_COND.BIN", SHEN_NAME(WCND),
        false, &size);
    if (!source || size < g_weatherConditions.size()) {
        if (source) {
            HLib::FreeTaskData(source);
        }
        return false;
    }
    std::memcpy(g_weatherConditions.data(), source,
                g_weatherConditions.size());
    HLib::FreeTaskData(source);
    return true;
}

const char* Filepath_Gen(FilepathGenMode mode) {
    static std::array<char, 0x40> directory{};
    const char* area = reinterpret_cast<const char*>(&g_currentMap.name);
    switch (mode) {
    case FilepathGenMode::SceneDirectory:
        std::snprintf(directory.data(), directory.size(), "SCENE/%02d/%.4s", g_actNumber, area);
        return directory.data();
    case FilepathGenMode::MiscDirectory:
        std::snprintf(directory.data(), directory.size(), "MISC");
        return directory.data();
    case FilepathGenMode::SoundDirectory:
        std::snprintf(directory.data(), directory.size(), "SCENE/%02d/SOUND", g_actNumber);
        return directory.data();
    case FilepathGenMode::StreamDirectory:
        std::snprintf(directory.data(), directory.size(), "SCENE/%02d/STREAM", g_actNumber);
        return directory.data();
    case FilepathGenMode::SpriteDirectory:
        std::snprintf(directory.data(), directory.size(), "SPRITE");
        return directory.data();
    case FilepathGenMode::ItemDirectory:
        std::snprintf(directory.data(), directory.size(), "MODEL/ITEM");
        return directory.data();
    case FilepathGenMode::CharaDirectory:
        std::snprintf(directory.data(), directory.size(), "SCENE/%02d/MODEL/CHARA", g_actNumber);
        return directory.data();
    case FilepathGenMode::PackDirectory:
        std::snprintf(directory.data(), directory.size(), "SCENE/%02d/PACK", g_actNumber);
        return directory.data();
    }
    return nullptr;
}

bool AllocateFlagWork(std::int32_t actNumber) {
    std::array<char, 10> filename{};
    std::memcpy(filename.data(), "FLAG0.BIN", filename.size());
    filename[4] = static_cast<char>(filename[4] + actNumber);
    g_flagDescriptorDataSize = 0;
    g_flagDescriptorData = LoadMiscFile(
        filename.data(), SHEN_NAME(FLGB),
        true, &g_flagDescriptorDataSize);
    if (!g_flagDescriptorData) {
        return false;
    }
    HLib::SetTaskDataMotionFlag(g_flagDescriptorData);

    g_flagWork = static_cast<std::uint16_t*>(HLib::AllocateTaskData(
        0xbb6, SHEN_NAME(FLGW)));
    if (!g_flagWork) {
        return false;
    }
    HLib::SetTaskDataMotionFlag(g_flagWork);

    const std::span<const std::byte> data{
        g_flagDescriptorData,
        static_cast<std::size_t>(g_flagDescriptorDataSize)};
    std::int32_t count = 0;
    std::int32_t sourceWordOffset = 0;
    if (!ReadValue(data, 0x0c, count) ||
        !ReadValue(data, 0x10, sourceWordOffset) || count < 0 ||
        count > static_cast<std::int32_t>(0xbb6 / sizeof(std::uint16_t)) ||
        sourceWordOffset < 0) {
        return false;
    }
    for (std::int32_t index = 0; index < count; ++index) {
        std::uint32_t value = 0;
        const std::uint64_t offset =
            (static_cast<std::uint64_t>(sourceWordOffset) + index) * 4U;
        if (offset > data.size() || sizeof(value) > data.size() - offset) {
            return false;
        }
        std::memcpy(&value, data.data() + offset, sizeof(value));
        g_flagWork[index] = static_cast<std::uint16_t>(value);
    }
    return true;
}

bool LoadMemoData() {
    g_memoMasterTable = nullptr;
    g_memoData = nullptr;
    g_memoRubyTable = nullptr;
    g_memoRecordCount = 0;

    std::uint64_t size = 0;
    g_memoData = LoadMiscFile(
        "MEMODATA.BIN", SHEN_NAME(MWRK),
        false, &size);
    if (!g_memoData) {
        return false;
    }
    const std::span<const std::byte> data{g_memoData,
                                         static_cast<std::size_t>(size)};
    std::uint32_t chunkOffset = 0;
    if (!ReadValue(data, 4, chunkOffset) || chunkOffset < 8 ||
        chunkOffset > data.size() ||
        sizeof(std::uint64_t) > data.size() - chunkOffset) {
        HLib::FreeTaskData(g_memoData);
        g_memoData = nullptr;
        return false;
    }
    g_memoRecordCount = (chunkOffset - 8) / 0x28;
    std::byte* chunk = g_memoData + chunkOffset;
    std::uint32_t name = 0;
    std::uint32_t advance = 0;
    std::memcpy(&name, chunk, sizeof(name));
    if (name == SHEN_NAME(RUBI)) {
        std::memcpy(&advance, chunk + 4, sizeof(advance));
        if (advance < 8 || advance > data.size() - chunkOffset) {
            return false;
        }
        g_memoRubyTable = chunk + 8;
        chunk += advance;
        chunkOffset += advance;
    }
    if (sizeof(std::uint64_t) <= data.size() - chunkOffset) {
        std::memcpy(&name, chunk, sizeof(name));
        if (name == SHEN_NAME(MSTR)) {
            g_memoMasterTable = chunk + 8;
        }
    }
    HLib::SetTaskDataMotionFlag(g_memoData);
    return true;
}

} // namespace shenmue::s2
