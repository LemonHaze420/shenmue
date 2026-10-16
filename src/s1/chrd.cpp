#include "s1/chrd.hpp"

#include "s1/boot.hpp"
#include "s1/collision.hpp"
#include "s1/face.hpp"
#include "s1/file_loader.hpp"
#include "s1/mt5_types.hpp"
#include "s1/player_control.hpp"
#include "s1/scene_memory.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/cuff.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/lamp.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/telm.hpp"
#include "common/utility.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <span>
#include <string_view>
#include <thread>

namespace shenmue::s1 {

extern void *g_shadowMeshData;

std::array<ShadowModelCacheEntry, 10> g_shadowModelCache{};
std::int32_t g_shadowModelCacheCount{};

namespace {

// TokenFinder writes a 0x100-byte record, with 0x18-byte decoded arguments.
// A block's word count excludes its own count word. String displacements are
// unsigned and relative to the argument word, not the enclosing token.
#pragma pack(push, 1)
struct SceneTokenArgument {
    std::uint32_t type;
    std::uint32_t reserved04;
    const std::byte *source;
    std::uint64_t value;

    float Number() const {
        return std::bit_cast<float>(static_cast<std::uint32_t>(value));
    }
    const char *String() const {
        return type == 3 ? reinterpret_cast<const char *>(value) : nullptr;
    }
    const std::byte *BlockBegin() const {
        return source + 4;
    }
    const std::byte *BlockEnd() const {
        return BlockBegin() + static_cast<std::uint32_t>(value) * 4ULL;
    }
};

struct SceneToken {
    const std::byte *record;
    std::uint32_t index;
    std::uint32_t count;
    SceneTokenArgument arguments[10];

    std::span<const SceneTokenArgument> Arguments() const {
        return {arguments, count};
    }
};

struct ModelRead {
    MdlmImageEntry *destination;
    char directory[64];
    char filename[16];
    FileState *file;
    void *bytes;
};

#pragma pack(pop)

static_assert(sizeof(SceneTokenArgument) == 0x18);
static_assert(offsetof(SceneTokenArgument, source) == 8);
static_assert(offsetof(SceneTokenArgument, value) == 0x10);
static_assert(sizeof(SceneToken) == 0x100);
static_assert(sizeof(ModelRead) == 0x68);
static_assert(offsetof(ModelRead, file) == 0x58);
static_assert(offsetof(ModelRead, bytes) == 0x60);

constexpr std::array<std::string_view, 4> kDocumentTokens{"Character", "DefImage", "LocalPos", "PreLoad"};
constexpr std::array<std::string_view, 5> kAttributeTokens{"Position", "Angle", "Layer", "Local", "HasColi"};
constexpr std::array<std::string_view, 52> kCharacterTokens{"Player", "Image", "Entry", "Flags", "Coli", "Human", "Object", "Party", "Face", "Hand", "Item",
    "Range", "ColiOff", "LineWalk", "ForkLift", "MotClip", "Shadow", "ShadowOff", "Book", "KeysRing", "Fight", "Group", "MotOnly", "CasetteDeck", "CasetteTape",
    "Handcuffs", "Cloth", "TinyMot", "Tel", "Step", "Walk3D", "CommonMotionWork", "FootMark", "RefBoard", "Osage", "Knife", "TParty", "Trans", "Vehicle",
    "Bone", "DummyChara", "Bicycle", "Patrol", "Vender", "LowQuality", "DesignOK", "Clock", "ProjectionTrans", "TailLamp", "TinyGame", "Calendar",
    "CutMvShadow"};
constexpr std::array<std::string_view, 5> kImageDefinitionTokens{"Image", "Avail", "NotAvail", "Dynamic", "Avail_Ext"};
constexpr std::array<std::string_view, 4> kImageOptions{"Num", "Loop", "Wait", "Kind"};
constexpr std::array<std::string_view, 5> kCollisionOptions{"Level", "Adjust", "Contact", "Size", "Weight"};
constexpr std::array<std::string_view, 3> kContactOptions{"Range", "Radius", "Height"};
constexpr std::array<std::string_view, 1> kFightOptions{"Blur"};
constexpr std::array<std::string_view, 6> kShadowOptions{"Image", "Mode", "Position", "Scale", "Yang", "Height"};
constexpr std::array<std::string_view, 21> kFightStyles{
    "AKI", "BIG", "CHA", "DEB", "KKO", "GEN", "IWA", "JIJ", "KOD", "KOK", "OHS", "REN", "SYP", "YKI", "YUA", "SIN", "CAT", "YAG", "DOG", "PNW", "KCT"};

unsigned char Upper(unsigned char character) {
    return character >= 'a' && character <= 'z' ? static_cast<unsigned char>(character - ('a' - 'A')) : character;
}

int CompareNames(std::string_view left, std::string_view right) {
    std::size_t index = 0;
    while (index < left.size() && index < right.size()) {
        const int difference = Upper(left[index]) - Upper(right[index]);
        if (difference)
            return difference;
        ++index;
    }
    return left.size() < right.size() ? -1 : left.size() != right.size();
}

bool IsName(const char *value, std::string_view name) {
    return value && CompareNames(value, name) == 0;
}

std::uint32_t FindToken(std::string_view name, std::span<const std::string_view> table) {
    std::uint32_t index = 0;
    while (index < table.size() && CompareNames(name, table[index]) != 0)
        ++index;
    return index;
}

std::int32_t TruncateNumber(float value) {
    if (!std::isfinite(value) || value >= 2147483648.0f || value < -2147483648.0f) {
        return std::numeric_limits<std::int32_t>::min();
    }
    return static_cast<std::int32_t>(value);
}

std::int32_t FixedAngle(float degrees) {
    return TruncateNumber((degrees * 8192.0f) / 45.0f);
}

bool ReadString(const std::byte *source, std::uint32_t offset, const std::byte *resourceEnd, const char *&result) {
    if (source >= resourceEnd || offset >= static_cast<std::size_t>(resourceEnd - source)) {
        return false;
    }
    const std::byte *const text = source + offset;
    if (!std::memchr(text, 0, static_cast<std::size_t>(resourceEnd - text)))
        return false;
    result = reinterpret_cast<const char *>(text);
    return true;
}

const std::byte *DecodeToken(
    const std::byte *cursor, const std::byte *end, const std::byte *resourceEnd, std::span<const std::string_view> table, SceneToken &token) {
    if (cursor >= end || end - cursor < 8)
        return nullptr;
    token.record = cursor;
    const char *name = nullptr;
    if (!ReadString(cursor, read_unaligned<std::uint32_t>(cursor), resourceEnd, name)) {
        return nullptr;
    }
    token.index = FindToken(name, table);
    token.count = 0;
    std::uint32_t types = read_unaligned<std::uint32_t>(cursor + 4);
    cursor += 8;
    while (types) {
        if (token.count == std::size(token.arguments) || end - cursor < 4)
            return nullptr;
        SceneTokenArgument &argument = token.arguments[token.count++];
        argument.type = types & 7U;
        argument.reserved04 = 0;
        argument.source = cursor;
        argument.value = read_unaligned<std::uint32_t>(cursor);
        if (argument.type == 3) {
            const char *text = nullptr;
            if (!ReadString(cursor, static_cast<std::uint32_t>(argument.value), resourceEnd, text)) {
                return nullptr;
            }
            argument.value = reinterpret_cast<std::uintptr_t>(text);
        } else if (argument.type == 4) {
            const std::uint64_t bytes = argument.value * 4;
            if (bytes > static_cast<std::size_t>(end - cursor - 4))
                return nullptr;
            cursor += bytes;
        } else if (argument.type != 1 && argument.type != 2) {
            return nullptr;
        }
        cursor += 4;
        types >>= 3;
    }
    return cursor;
}

MapControlData *MapControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

PackTaskData *PackTask() {
    return g_packTaskData ? g_packTaskData : g_packTask ? reinterpret_cast<PackTaskData *>(g_packTask->task_data_ptr) : nullptr;
}

class ModelReadBatch {
  public:
    ~ModelReadBatch() {
        Publish();
    }

    void Queue(MdlmImageEntry *destination, const char *directory) {
        const std::int64_t previous = g_TotalSceneFileReadBytes;
        auto *const request = static_cast<ModelRead *>(AllocateSceneTransient(sizeof(ModelRead)));
        request->destination = destination;
        std::strcpy(request->directory, directory);
        std::strcpy(request->filename, destination->imageName);
        request->file = Archive_GetNextUnusedFileState();
        const bool exists = PrepareDirectoryPathForFile(request->file, request->directory, request->filename);
        const std::uint32_t size = exists ? static_cast<std::uint32_t>(OpenFileInternal(request->file->path)) : 0U;
        request->bytes = HLib_AllocHeapMemBlock(size, make_shen_name('M', 'D', 'L', ' '));
        OpenFile(request->file, request->bytes, static_cast<std::int32_t>(size));
        if (!count_) {
            first_ = reinterpret_cast<std::byte *>(request);
            originalOffset_ = previous;
        }
        ++count_;
    }

    void Publish() {
        if (!count_)
            return;
        // FBXReader orders completed model publication by directory/name.
        for (std::size_t index = 1; index < count_; ++index) {
            const ModelRead value = At(index);
            std::size_t destination = index;
            while (destination && ComesAfter(At(destination - 1), value)) {
                At(destination) = At(destination - 1);
                --destination;
            }
            At(destination) = value;
        }
        bool pending;
        do {
            pending = false;
            for (std::size_t index = 0; index < count_; ++index) {
                ModelRead &request = At(index);
                if (!request.file)
                    continue;
                if (Archive_HasPendingRead(request.file->handle)) {
                    pending = true;
                    continue;
                }
                if (request.bytes) {
                    char path[0x100];
                    const int length = std::sprintf(path, "/%s/%s", request.directory, request.filename);
                    std::memcpy(path + length - 3, "fbx", 4);
                    PackModel *const model = MT5_InitRawModelAndTextures(request.bytes, request.filename, path);
                    if (model) {
                        Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), path, 0, 0x22);
                        const auto kind = static_cast<std::int16_t>(static_cast<Mt5RuntimeNode *>(model->model)->nodeKind);
                        model->field_14 = kind > 28672 ? kind - 28673 : 0;
                        MDLM_PublishImage(request.destination, model, true);
                    } else {
                        HLib_FreeTaskData(static_cast<HLib::TaskData *>(request.bytes), 0);
                    }
                }
                ReadFile_0(0, &request.file->handle);
                request.file = nullptr;
            }
            if (pending)
                std::this_thread::yield();
        } while (pending);
        g_TotalSceneFileReadBytes = originalOffset_;
        first_ = nullptr;
        count_ = 0;
    }

  private:
    ModelRead &At(std::size_t index) {
        return *reinterpret_cast<ModelRead *>(first_ + index * (sizeof(ModelRead) + 4));
    }
    static bool ComesAfter(const ModelRead &left, const ModelRead &right) {
        const int directoryOrder = CompareNames(left.directory, right.directory);
        return directoryOrder > 0 || (!directoryOrder && CompareNames(left.filename, right.filename) > 0);
    }
    std::byte *first_ = nullptr;
    std::size_t count_ = 0;
    std::int64_t originalOffset_ = 0;
};

bool MatchesEntry(const SceneTokenArgument &argument, std::uint32_t entryPoint) {
    return argument.type == 1 ? argument.Number() == static_cast<float>(entryPoint) : argument.type == 2 && argument.value == entryPoint;
}

void LoadImageDefinition(const SceneToken &definition, const std::byte *resourceEnd, std::uint32_t entryPoint, ModelReadBatch &batch) {
    if (definition.count < 2 || definition.arguments[0].type != 3 || definition.arguments[1].type != 4) {
        return;
    }
    const SceneTokenArgument &block = definition.arguments[1];
    const std::byte *cursor = block.BlockBegin();
    const std::byte *const end = block.BlockEnd();
    std::int32_t imageCount = 0;
    std::int32_t availability = 1;
    bool selectedAvailable = false;
    while (cursor < end) {
        SceneToken token;
        cursor = DecodeToken(cursor, end, resourceEnd, kImageDefinitionTokens, token);
        if (!cursor)
            return;
        if (token.index == 0) {
            ++imageCount;
        } else if (token.index == 1 || token.index == 4) {
            availability = 0;
            for (const SceneTokenArgument &argument : token.Arguments()) {
                if (MatchesEntry(argument, entryPoint)) {
                    selectedAvailable = true;
                    availability = 1;
                }
            }
        } else if (token.index == 2) {
            availability = 1;
            selectedAvailable = true;
            for (const SceneTokenArgument &argument : token.Arguments()) {
                if (MatchesEntry(argument, entryPoint)) {
                    availability = 0;
                    selectedAvailable = false;
                }
            }
        } else if (token.index == 3) {
            availability = 2;
        }
    }
    PackTaskData *const pack = PackTask();
    PackSlot *const reader = pack && pack->chidParseGuard ? pack->activeReader : nullptr;
    MdlmRecord *const record = MDLM_DefineImages(definition.arguments[0].String(), imageCount, reader);
    if (!record)
        return;
    const bool load = selectedAvailable || availability == 1;
    cursor = block.BlockBegin();
    std::int32_t populated = 0;
    while (cursor < end) {
        SceneToken token;
        cursor = DecodeToken(cursor, end, resourceEnd, kImageDefinitionTokens, token);
        if (!cursor)
            return;
        if (token.index != 0)
            continue;
        if (token.count < 2 || token.arguments[1].type != 3)
            return;
        const char *filename = token.arguments[1].String();
        const char prefix = *filename;
        if (prefix == '@' || prefix == '$')
            ++filename;
        MdlmImageEntry *const image = MDLM_InsertImageDefinition(record, populated++, filename, TruncateNumber(token.arguments[0].Number()));
        if (!image)
            return;
        image->flags = static_cast<std::uint8_t>(prefix);
    }
    for (std::int32_t index = 0; index < imageCount; ++index) {
        MdlmImageEntry *const image = &record->images[index];
        const char prefix = static_cast<char>(image->flags);
        image->flags = 0;
        if (!load)
            continue;
        const char *const filename = image->imageName;
        char directory[64];
        if (prefix == '$') {
            std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_sceneId, static_cast<char>(g_areaId), static_cast<char>(g_areaId >> 8),
                static_cast<char>(g_areaId >> 16), static_cast<char>(g_areaId >> 24));
        } else {
            const bool hand = prefix != '@' && std::strlen(filename) >= 7 && filename[3] == '_' && filename[4] == 'U' &&
                (filename[5] == 'L' || filename[5] == 'R') && filename[6] == '.';
            std::snprintf(directory, sizeof(directory), hand ? "scene/%02d/model/hand" : "scene/%02d/model/chara", g_sceneId);
        }
        if (reader) {
            PackModel *const packed = PACK_FindCharacterModel(reader, filename);
            if (packed) {
                MDLM_PublishImage(image, packed, false);
            } else {
                batch.Queue(image, directory);
                batch.Publish();
            }
        } else {
            batch.Queue(image, directory);
        }
    }
}

std::uint32_t ApplyAttribute(const SceneToken &token, CharacterIdTransform &transform) {
    if (!token.count)
        return 0;
    const SceneTokenArgument &argument = token.arguments[0];
    switch (token.index) {
    case 0:
        if (token.count < 3)
            return 0;
        transform.node.position = {argument.Number(), token.arguments[1].Number(), token.arguments[2].Number()};
        return 1;
    case 1:
        if (token.count == 1) {
            transform.node.rotation.y = FixedAngle(argument.Number());
        } else if (token.count >= 3) {
            transform.node.rotation = {FixedAngle(argument.Number()), FixedAngle(token.arguments[1].Number()), FixedAngle(token.arguments[2].Number())};
        }
        return 2;
    case 2:
        transform.layer = static_cast<std::uint32_t>(TruncateNumber(argument.Number()));
        return 4;
    case 3: {
        MapControlData *const control = MapControl();
        transform.node.child = nullptr;
        if (argument.type == 1) {
            const std::int32_t index = TruncateNumber(argument.Number());
            if (control && index >= 0 && static_cast<std::size_t>(index) < control->characterIdTable.size()) {
                transform.node.child = &control->characterIdTable[index].node;
            }
        } else if (control) {
            Character *const parent = CHID_Find(control->scenarioTask, static_cast<std::uint32_t>(argument.value));
            if (parent)
                transform.node.child = &parent->entity.node;
        }
        return 8;
    }
    case 4: {
        const std::uint32_t name = argument.type == 3 ? read_unaligned<std::uint32_t>(argument.String()) : static_cast<std::uint32_t>(argument.value);
        transform.collisionData = COLS_FindQuerySet(name);
        return 0x10;
    }
    default:
        return 0;
    }
}

void CopyAttributes(CharacterIdTransform &destination, const CharacterIdTransform &source, std::uint32_t mask) {
    if (mask & 1U)
        destination.node.position = source.node.position;
    if (mask & 2U)
        destination.node.rotation = source.node.rotation;
    if (mask & 4U)
        destination.layer = source.layer;
    if (mask & 8U)
        destination.node.child = source.node.child;
    if (mask & 0x10U)
        destination.collisionData = source.collisionData;
}

struct CollisionDefinition {
    std::int8_t level = 0;
    std::uint32_t flags = 7;
    std::uint32_t maxAngle = 0;
    float distanceLimit = 0.0f;
    std::uint32_t interactionHeight = 0;
    Vector3 extents{0.4f, 1.5f, 0.4f};
    float radius = 0.4f;
    std::uint32_t shape = 3;
    std::uint32_t weight = 0x42700000U;
};

void ParseCollision(const SceneTokenArgument &block, const std::byte *resourceEnd, CollisionDefinition &result) {
    if (block.type != 4)
        return;
    const std::byte *cursor = block.BlockBegin();
    while (cursor < block.BlockEnd()) {
        SceneToken token;
        cursor = DecodeToken(cursor, block.BlockEnd(), resourceEnd, kCollisionOptions, token);
        if (!cursor)
            return;
        if (!token.count)
            continue;
        const SceneTokenArgument &argument = token.arguments[0];
        switch (token.index) {
        case 0:
            result.level = static_cast<std::int8_t>(TruncateNumber(argument.Number()));
            break;
        case 1:
            result.flags = 1;
            for (const SceneTokenArgument &option : token.Arguments()) {
                if (IsName(option.String(), "MAP"))
                    result.flags |= 2U;
                else if (IsName(option.String(), "CHAR"))
                    result.flags |= 4U;
                else if (IsName(option.String(), "FLOAT"))
                    result.flags |= 0x80U;
            }
            break;
        case 2:
            if (argument.type == 4) {
                const std::byte *contactCursor = argument.BlockBegin();
                while (contactCursor < argument.BlockEnd()) {
                    SceneToken contact;
                    contactCursor = DecodeToken(contactCursor, argument.BlockEnd(), resourceEnd, kContactOptions, contact);
                    if (!contactCursor)
                        return;
                    if (!contact.count)
                        continue;
                    if (contact.index == 0)
                        result.maxAngle = FixedAngle(contact.arguments[0].Number());
                    else if (contact.index == 1)
                        result.distanceLimit = contact.arguments[0].Number();
                    else if (contact.index == 2)
                        result.interactionHeight = static_cast<std::uint32_t>(contact.arguments[0].value);
                }
            }
            break;
        case 3:
            if (token.count <= 3) {
                result.extents.x = argument.Number();
                result.extents.z = argument.Number();
                result.radius = argument.Number();
                result.shape = 3;
                if (token.count >= 2)
                    result.extents.y = token.arguments[1].Number();
                if (token.count == 3) {
                    result.extents.z = token.arguments[2].Number();
                    result.radius = std::sqrt(result.extents.z * result.extents.z + result.extents.x * result.extents.x);
                    result.shape = 5;
                }
            }
            break;
        case 4:
            result.weight = static_cast<std::uint32_t>(argument.value);
            break;
        }
    }
}

const char *FindFightBlurFilename(const SceneToken &definition, const std::byte *resourceEnd) {
    for (const SceneTokenArgument &argument : definition.Arguments()) {
        if (argument.type != 4)
            continue;
        const std::byte *cursor = argument.BlockBegin();
        while (cursor < argument.BlockEnd()) {
            SceneToken option;
            cursor = DecodeToken(cursor, argument.BlockEnd(), resourceEnd, kFightOptions, option);
            if (!cursor)
                return nullptr;
            if (option.index == 0 && option.count && option.arguments[0].type == 3) {
                return option.arguments[0].String();
            }
        }
    }
    return nullptr;
}

void ApplyFightStyle(Character *character, const char *name) {
    ImageModuleWork *const image = IMGM_GetWork(character);
    PackModel *model = nullptr;
    if (image && image->images && image->requestedImage < image->imageCount) {
        if (MdlmImageSlot *const slot = image->images[image->requestedImage])
            model = slot->selectedModel;
    }
    for (std::size_t index = 0; index < kFightStyles.size(); ++index) {
        if (std::strcmp(kFightStyles[index].data(), name) == 0) {
            model->field_14 = static_cast<std::int32_t>(index);
            return;
        }
    }
}
void *LoadShadowModel(const char *sourceName) {
    const char prefix = *sourceName;
    const std::size_t filenameOffset = prefix == '@' || prefix == '$' ? 1U : 0U;
    const char *const stem = sourceName + filenameOffset;

    char directory[64];
    if (prefix == '@') {
        std::sprintf(directory, "scene/%02d/model", g_sceneId);
    } else if (prefix == '$') {
        std::sprintf(directory, "scene/%02d/%c%c%c%c", g_sceneId, static_cast<char>(g_areaId), static_cast<char>(g_areaId >> 8U),
            static_cast<char>(g_areaId >> 16U), static_cast<char>(g_areaId >> 24U));
    } else {
        std::sprintf(directory, "/model/chara");
    }

    char filename[16];
    std::sprintf(filename, "%s.MT5", stem);
    PackTaskData *const pack = PackTask();
    if (pack->chidParseGuard)
        return PACK_FindCharacterModel(pack->activeReader, filename)->model;

    for (std::int32_t index = 0; index < g_shadowModelCacheCount; ++index) {
        if (std::strcmp(g_shadowModelCache[index].name, filename) == 0 && g_shadowModelCache[index].model)
            return g_shadowModelCache[index].model;
    }

    void *const bytes = ReadFile(directory, filename, make_shen_name(stem[0], stem[1], stem[2], 'S'));
    PackModel *const loaded = MT5_InitRawModelAndTextures(bytes, filename, nullptr);
    void *const model = loaded->model;
    if (g_shadowModelCacheCount < static_cast<std::int32_t>(g_shadowModelCache.size())) {
        ShadowModelCacheEntry &entry = g_shadowModelCache[g_shadowModelCacheCount++];
        entry.model = model;
        std::memcpy(entry.name, filename, sizeof(entry.name));
    }
    return model;
}



void ParseShadowOptions(Character *character, const SceneToken &definition, const std::byte *resourceEnd) {
    for (const SceneTokenArgument &argument : definition.Arguments()) {
        const std::byte *cursor = argument.BlockBegin();
        while (cursor < argument.BlockEnd()) {
            SceneToken option;
            cursor = DecodeToken(cursor, argument.BlockEnd(), resourceEnd, kShadowOptions, option);
            switch (option.index) {
            case 0:
                character->shadowMode = 1;
                character->shadowModel = LoadShadowModel(reinterpret_cast<const char *>(option.arguments[0].value));
                break;
            case 1: {
                const std::int32_t mode = TruncateNumber(option.arguments[0].Number());
                character->shadowMode = mode >= 12 ? 0 : mode;
                break;
            }
            case 2:
                character->shadowPosition = {option.arguments[0].Number(), option.arguments[1].Number(), option.arguments[2].Number()};
                break;
            case 3:
                character->shadowScale = {option.arguments[0].Number(), option.arguments[1].Number(), option.arguments[2].Number()};
                break;
            case 4:
                character->shadowYaw = static_cast<std::uint16_t>(FixedAngle(option.arguments[0].Number()));
                break;
            case 5:
                character->flags.value |= CharacterFlagSkipShadowGroundCheck;
                character->shadowHeight = option.arguments[0].Number();
                break;
            }
        }
    }
}

void CreateImage(Character *character, const SceneToken &definition, const std::byte *resourceEnd) {
    ImageModuleWork *const image = IMGM_Alloc(character);
    std::uint32_t count = 0;
    if (definition.count == 2) {
        const SceneTokenArgument &block = definition.arguments[1];
        const std::byte *cursor = block.BlockBegin();
        while (cursor < block.BlockEnd()) {
            SceneToken token;
            cursor = DecodeToken(cursor, block.BlockEnd(), resourceEnd, kImageOptions, token);
            const SceneTokenArgument &value = token.arguments[0];
            switch (token.index) {
            case 0:
                count = TruncateNumber(value.Number());
                break;
            case 1:
                image->repeatCount = TruncateNumber(value.Number());
                break;
            case 2:
                image->frameDelay = static_cast<std::uint8_t>(TruncateNumber(value.Number()));
                break;
            case 3:
                image->cycleMode = IsName(reinterpret_cast<const char *>(value.value), "NONE") ? 0 :
                    IsName(reinterpret_cast<const char *>(value.value), "REPEAT")              ? 1 :
                                                                                                 2;
                break;
            }
        }
    }
    image->cycleFlags = 0;
    image->frameCounter = 0;
    MDLM_ActivateCharacterImage(character, reinterpret_cast<const char *>(definition.arguments[0].value), count);
}

void ApplyCharacterFlags(Character *character, const SceneToken &flags, bool player) {
    if (player) {
        MapEvent_AllocateComponent(character, true, false);
    } else if (flags.record) {
        for (const SceneTokenArgument &argument : flags.Arguments()) {
            if (IsName(argument.String(), "MAPEV"))
                MapEvent_AllocateComponent(character, true, false);
        }
    }
    if (!flags.record)
        return;
    for (const SceneTokenArgument &argument : flags.Arguments()) {
        const char *const name = argument.String();
        std::uint32_t disabledComponent = 0;
        if (IsName(name, "DISP")) {
            const bool wasVisible = (character->motionFlags & 4U) != 0;
            character->motionFlags |= 4U;
            if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
                CHAR_LogNPCVisibility(character, true);
        } else if (IsName(name, "KILL")) {
            character->flags.value &= ~CharacterFlagUnknown;
            if (MapControlData *const control = MapControl())
                ++control->pendingCharacterRetirements;
        } else if (IsName(name, "NOMAPEV")) {
            disabledComponent = SHEN_NAME(MEVW);
        } else if (IsName(name, "NOCLIPPER")) {
            disabledComponent = SHEN_NAME(CLIP);
        } else if (IsName(name, "NOIMGMODULE")) {
            disabledComponent = SHEN_NAME(IMGM);
        } else if (IsName(name, "SLEEP")) {
            SetCharacterVisibility(character, false);
        } else if (IsName(name, "NOTMIRRORED")) {
            character->flags.value &= ~CharacterFlagReflectInScriptPlane;
        }
        if (disabledComponent) {
            if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, disabledComponent)) {
                component->flags &= ~1U;
            }
        }
    }
}

void ParseSceneDefinition(
    Character *character, const SceneTokenArgument &block, const std::byte *resourceEnd, std::uint32_t entryPoint, std::int32_t groupToken) {
    const std::int64_t originalOffset = g_TotalSceneFileReadBytes;
    auto *const tokens = static_cast<SceneToken *>(AllocateSceneTransient(sizeof(SceneToken) * kCharacterTokens.size()));
    if (!tokens)
        return;
    for (std::size_t index = 0; index < kCharacterTokens.size(); ++index)
        tokens[index].record = nullptr;
    CharacterIdTransform attributes{};
    CharacterIdTransform entryAttributes{};
    std::uint32_t entryMask = 0;
    SceneToken entryFlags{};
    SceneToken entryGroup{};
    SceneToken entryCommonMotion{};
    const std::byte *cursor = block.BlockBegin();
    while (cursor < block.BlockEnd()) {
        SceneToken token;
        const std::byte *const next = DecodeToken(cursor, block.BlockEnd(), resourceEnd, kAttributeTokens, token);
        if (!next)
            break;
        if (token.index < kAttributeTokens.size()) {
            ApplyAttribute(token, attributes);
        } else {
            const char *name = nullptr;
            ReadString(cursor, read_unaligned<std::uint32_t>(cursor), resourceEnd, name);
            token.index = FindToken(name, kCharacterTokens);
            if (token.index == 2 && token.count >= 2 && token.arguments[1].type == 4 && token.arguments[0].Number() == static_cast<float>(entryPoint)) {
                const SceneTokenArgument &entry = token.arguments[1];
                const std::byte *entryCursor = entry.BlockBegin();
                while (entryCursor < entry.BlockEnd()) {
                    SceneToken option;
                    const std::byte *const entryNext = DecodeToken(entryCursor, entry.BlockEnd(), resourceEnd, kAttributeTokens, option);
                    if (!entryNext)
                        break;

                    if (option.index < kAttributeTokens.size()) {
                        entryMask |= ApplyAttribute(option, entryAttributes);
                    } else {
                        const char *entryName = nullptr;
                        ReadString(entryCursor, read_unaligned<std::uint32_t>(entryCursor), resourceEnd, entryName);
                        if (IsName(entryName, "Flags"))
                            entryFlags = option;
                        else if (IsName(entryName, "Group"))
                            entryGroup = option;
                        else if (IsName(entryName, "CommonMotionWork"))
                            entryCommonMotion = option;
                    }
                    entryCursor = entryNext;
                }
            } else if (token.index < kCharacterTokens.size()) {
                tokens[token.index] = token;
            }
        }
        cursor = next;
    }

    if (entryFlags.record)
        tokens[3] = entryFlags;
    if (entryGroup.record)
        tokens[21] = entryGroup;
    if (entryCommonMotion.record)
        tokens[31] = entryCommonMotion;

    CopyAttributes(attributes, entryAttributes, entryMask);
    character->entity.node = attributes.node;
    character->eventGroup = attributes.layer;
    character->collisionQuery = const_cast<void *>(attributes.collisionData);
    const auto has = [tokens](std::size_t index) { return tokens[index].record != nullptr; };
    const bool player = has(0);
    if (has(1))
        CreateImage(character, tokens[1], resourceEnd);
    character->shadowModel = g_shadowMeshData;
    if (has(22)) {
        character->flags.value &= ~CharacterFlagActive;
        MOTN_Alloc(character, has(31));
    } else {
        character->flags.value |= CharacterFlagActive;
        if (has(17))
            character->flags.value &= ~CharacterFlagActive;
        if (has(16))
            ParseShadowOptions(character, tokens[16], resourceEnd);
        if (!has(5) && !has(9) && !has(51))
            MT5_PrepareStaticCharacterModel(character->model);
        if (has(5)) {
            if (player)
                AllocateCPCT(character);
            MOTN_Alloc(character, has(31));
            Clipper_Alloc(character);
            if (has(7))
                PTMV_Allocate(character);
            if (has(36))
                PTTM_Allocate(character);
            if (has(20)) {
                FIGP_Allocate(character, FindFightBlurFilename(tokens[20], resourceEnd));
            }
            if (has(25))
                CUFF_Initialize();
            const bool clothOff = has(26) && tokens[26].count && IsName(tokens[26].arguments[0].String(), "OFF");
            if (!clothOff) {
                const std::uint8_t mode = has(26) && tokens[26].count ? static_cast<std::uint8_t>(TruncateNumber(tokens[26].arguments[0].Number())) : 0;
                CLMD_Alloc(character, mode);
            }
            OSAGE_Alloc(character);
            if (has(29) && tokens[29].count) {
                const char *const surface = tokens[29].arguments[0].String();
                const std::uint8_t surfaceType = IsName(surface, "HARD") ? 1 : IsName(surface, "NORMAL") ? 2 : 0;
                const std::uint8_t mode = tokens[29].count == 2 ? static_cast<std::uint8_t>(TruncateNumber(tokens[29].arguments[1].Number())) : 0;
                STEP_Allocate(character, surfaceType, mode);
            }
            if (has(20)) {
                const char *const style = has(39) && tokens[39].count && tokens[39].arguments[0].type == 3 ? tokens[39].arguments[0].String() : "";
                ApplyFightStyle(character, style);
            }
            if (has(38))
                VEHI_Allocate(character);
            if (has(42))
                PATW_Allocate(character);
            MHND_Initialize(character);
            AllocREFB(character, has(33) ? 1 : 2);
            if (player) {
                if (MotionManagerWork *const motion = MOTM_GetWork(character))
                    motion->flags |= 1U;
                character->flags.value |= CharacterFlagHasCpct;
                character->drawCallback = MOTM_DrawPlayerHuman;
            } else {
                CPAM_Alloc(character);
                character->flags.value |= CharacterFlagHasCpct;
            }
        } else if (has(6)) {
            AllocateFIXO(character);
            if (has(37))
                character->drawCallback = CharacterHandler_AlphaDraw;
            if (has(47))
                CPAM_Alloc(character);
            if (has(43)) {
                const std::int32_t variant = tokens[43].count && tokens[43].arguments[0].type == 1 ? TruncateNumber(tokens[43].arguments[0].Number()) : 0;
                VEND_Initialize(character, variant);
            }
            if (has(48))
                LAMP_Init(character);
            if (has(49))
                GAME_Allocate(character);
            if (has(33))
                AllocREFB(character, 6);
            if (has(50))
                CALE_Allocate(character);
        } else if (has(14)) {
            AllocFLMW(character);
        }
        if (has(28)) {
            const std::int32_t variant = tokens[28].count && tokens[28].arguments[0].type == 1 ? TruncateNumber(tokens[28].arguments[0].Number()) : 0;
            EnqueueTELM(character, variant);
        }
        if (has(46)) {
            const std::int16_t minuteOffset =
                tokens[46].count && tokens[46].arguments[0].type == 1 ? static_cast<std::int16_t>(TruncateNumber(tokens[46].arguments[0].Number())) : 0;
            CLOK_Allocate(character, minuteOffset);
        }
        if (!has(12)) {
            CCOW_Allocate(character);
            CollisionDefinition collision;
            if (has(4)) {
                for (const SceneTokenArgument &argument : tokens[4].Arguments())
                    ParseCollision(argument, resourceEnd, collision);
            }
            CCOW_ApplyAuthoredDefinition(character, collision.level, false, collision.flags, collision.maxAngle, collision.distanceLimit,
                collision.interactionHeight, collision.extents, collision.radius, collision.shape, collision.weight);
        }
        if (has(8) && tokens[8].count)
            AllocateFACE(character, tokens[8].arguments[0].String());
        if (has(9) && tokens[9].count >= 2) {
            const std::uint8_t flags = tokens[9].count == 3 ? static_cast<std::uint8_t>(TruncateNumber(tokens[9].arguments[2].Number())) : 0;
            AllocateAndLoadArms(character, tokens[9].arguments[0].String(), tokens[9].arguments[1].String(), flags);
        }
        if (has(11) && tokens[11].count)
            character->clipAngle = tokens[11].arguments[0].Number();
        if (has(13))
            LNWK_Allocate(character);
        if (has(30))
            W3DW_Allocate(character);
        if (has(27))
            TMNM_Alloc(character);
        if (player)
            CPCT_RebindMotionWithNavParams(character);
    }
    if (has(15) && tokens[15].count) {
        if (MotionManagerWork *const motion = MOTM_GetWork(character)) {
            const char *const mode = tokens[15].arguments[0].String();
            motion->motion.poseEvaluationStage = IsName(mode, "NONE") ? 0 : IsName(mode, "LOW") ? 1 : IsName(mode, "HIGH") ? 2 : 3;
        }
    }
    if (has(35))
        character->figurePoseCallback = KNIF_FigurePose;
    if (has(41))
        LoadAndSetupFBGG(character);
    ApplyCharacterFlags(character, tokens[3], player);
    if (!has(21)) {
        character->groupId = groupToken;
    } else if (tokens[21].count) {
        const SceneTokenArgument &group = tokens[21].arguments[0];
        if (group.type == 2)
            character->groupId = static_cast<std::uint32_t>(group.value);
        else if (group.type == 3)
            character->groupId = read_unaligned<std::uint32_t>(group.String());
    }
    g_TotalSceneFileReadBytes = originalOffset;
}

void ParseCharacter(const SceneToken &token, const std::byte *resourceEnd, std::uint32_t entryPoint, std::int32_t groupToken) {
    CharacterHandlerPriority priority = CharacterHandlerPriority::Normal;
    Character *character = nullptr;
    std::uint32_t id = 0;
    bool hasId = false;
    for (const SceneTokenArgument &argument : token.Arguments()) {
        if (argument.type == 1) {
            priority = static_cast<CharacterHandlerPriority>(TruncateNumber(argument.Number()));
        } else if (argument.type == 2) {
            id = static_cast<std::uint32_t>(argument.value);
            hasId = true;
        } else if (argument.type == 3) {
            if (IsName(argument.String(), "HIGH"))
                priority = CharacterHandlerPriority::High;
            else if (IsName(argument.String(), "LOW"))
                priority = CharacterHandlerPriority::Low;
        } else if (argument.type == 4) {
            MapControlData *const control = MapControl();
            HLib::Task *const scenario = control ? control->scenarioTask : nullptr;
            if (hasId) {
                character = CHID_Find(scenario, id);
            }
            if (!character) {
                character = AddCharacterHandler(priority);
                if (!character)
                    return;

                if (hasId)
                    CHID_Init(scenario, id, character);

                ParseSceneDefinition(character, argument, resourceEnd, entryPoint, groupToken);
            }
        }
    }
    if (!character) {
        character = AddCharacterHandler(priority);
        if (character && hasId) {
            MapControlData *const control = MapControl();
            CHID_Init(control ? control->scenarioTask : nullptr, id, character);
        }
    }
}

void ParseLocalPosition(const SceneToken &token, const std::byte *resourceEnd) {
    if (token.count < 2 || token.arguments[1].type != 4)
        return;
    const std::int32_t index = TruncateNumber(token.arguments[0].Number());
    MapControlData *const control = MapControl();
    if (!control || index < 0 || static_cast<std::size_t>(index) >= control->characterIdTable.size()) {
        return;
    }
    CharacterIdTransform &transform = control->characterIdTable[index];
    const SceneTokenArgument &block = token.arguments[1];
    const std::byte *cursor = block.BlockBegin();
    while (cursor < block.BlockEnd()) {
        SceneToken attribute;
        cursor = DecodeToken(cursor, block.BlockEnd(), resourceEnd, kAttributeTokens, attribute);
        if (!cursor)
            return;
        ApplyAttribute(attribute, transform);
    }
}

void ParseDocument(void *data, std::uint32_t size, std::uint32_t entryPoint, std::int32_t groupToken) {
    if (!data || size < 8)
        return;

    const auto *const document = static_cast<const std::byte *>(data);
    const std::uint32_t bodySize = read_unaligned<std::uint32_t>(document + 4);
    if (read_unaligned<std::uint32_t>(document) != SHEN_NAME(CHRS) || bodySize < 8 || bodySize > size) {
        return;
    }
    const std::byte *const end = document + bodySize;
    const std::byte *const resourceEnd = document + size;
    ModelReadBatch models;
    PackTaskData *pack = PackTask();
    if (!pack || !pack->chidParseGuard) {
        const std::byte *cursor = document + 8;
        while (cursor < end) {
            SceneToken token;
            cursor = DecodeToken(cursor, end, resourceEnd, kDocumentTokens, token);
            if (!cursor)
                return;

            if (token.index == 1)
                LoadImageDefinition(token, resourceEnd, entryPoint, models);
        }
        models.Publish();
    }
    const std::byte *cursor = document + 8;
    while (cursor < end) {
        SceneToken token;
        cursor = DecodeToken(cursor, end, resourceEnd, kDocumentTokens, token);
        if (!cursor)
            return;

        if (token.index == 0) {
            ParseCharacter(token, resourceEnd, entryPoint, groupToken);
        } else if (token.index == 1) {
            pack = PackTask();
            if (pack && pack->chidParseGuard) {
                LoadImageDefinition(token, resourceEnd, entryPoint, models);


                models.Publish();
            }
        } else if (token.index == 2) {
            ParseLocalPosition(token, resourceEnd);
        }
    }
}

} // namespace

void CHRD_Init(void *data, std::uint32_t entryPoint, std::int32_t allocationToken) {
    if (!data)
        return;

    const std::uint32_t bodySize = read_unaligned<std::uint32_t>(static_cast<const std::byte *>(data) + 4);
    const PackTaskData *const pack = PackTask();
    const std::uint32_t size = pack && pack->chrtData == data ? pack->chrtSize : bodySize;
    ParseDocument(data, size, entryPoint, allocationToken);
}

void CHRD_InitMapInfoChunk(void *data, std::uint32_t size, std::uint32_t entryPoint, std::int32_t allocationToken) {
    ParseDocument(data, size, entryPoint, allocationToken);
}

void CHRD_InitCurrentMapInfo() {
    std::uint32_t size = 0;
    constexpr std::uint32_t token = SHEN_NAME(CHRD);
    const std::byte *const data = MAPINFO_FindChunkPayload(token, &size);
    CHRD_InitMapInfoChunk(const_cast<std::byte *>(data), size, g_entryPoint, static_cast<std::int32_t>(token));
}

} // namespace shenmue::s1
