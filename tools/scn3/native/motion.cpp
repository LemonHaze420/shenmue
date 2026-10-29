#include "scn3/native/motion.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"

#include <charconv>

namespace shenmue::scenario {
namespace {

struct MotionHeader {
    std::filesystem::path path;
    std::string include;
    std::set<std::string> names;
};

struct MotionNames {
    std::map<uint32_t, std::string> names;
    std::map<std::string, uint32_t, std::less<>> ids;
    std::set<uint32_t> ambiguousIds;
    std::set<std::string, std::less<>> ambiguousNames;
    std::vector<MotionHeader> headers;
    std::vector<std::filesystem::path> paths;
    std::filesystem::path root;
    bool forceIncludes = false;
};

MotionNames g_motionNames;

std::string lowerCase(std::string text) {
    for (char &ch : text)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return text;
}

bool readMotionHeader(const std::filesystem::path &path, const std::string &include) {
    for (const MotionHeader &known : g_motionNames.headers)
        if (known.path == path)
            return true;
    std::ifstream file(path);
    if (!file)
        return false;
    MotionHeader header{.path = path, .include = include, .names = {}};
    for (std::string line; std::getline(file, line);) {
        std::string_view text(line);
        const size_t begin = text.find_first_not_of(" \t");
        if (begin == std::string_view::npos || !text.substr(begin).starts_with("#define "))
            continue;
        text.remove_prefix(begin + 8);
        const size_t start = text.find_first_not_of(" \t");
        if (start == std::string_view::npos)
            continue;
        text.remove_prefix(start);
        const size_t end = text.find_first_of(" \t");
        if (end == std::string_view::npos || !text.starts_with("MN_"))
            continue;
        const std::string name(text.substr(0, end));
        text.remove_prefix(end);
        const size_t value = text.find_first_not_of(" \t");
        if (value == std::string_view::npos)
            continue;
        text.remove_prefix(value);
        const int base = text.starts_with("0x") || text.starts_with("0X") ? 16 : text.size() > 1 && text[0] == '0' ? 8 : 10;
        if (base == 16)
            text.remove_prefix(2);
        uint32_t id;
        const std::from_chars_result parsed = std::from_chars(text.data(), text.data() + text.size(), id, base);
        if (parsed.ec != std::errc{})
            continue;
        const size_t extra = text.find_first_not_of("uUlL \t\r", static_cast<size_t>(parsed.ptr - text.data()));
        if (extra != std::string_view::npos && !text.substr(extra).starts_with("//"))
            continue;
        defineMotionName(name, id);
        header.names.insert(name);
    }
    if (header.names.empty())
        return false;
    g_motionNames.paths.push_back(path);
    g_motionNames.headers.push_back(std::move(header));
    return true;
}

void readMotionSourceIncludes(const std::string &text, const std::filesystem::path &directory, std::set<std::filesystem::path> &visited) {
    for (size_t at = 0; at < text.size();) {
        const size_t end = text.find('\n', at);
        std::string_view line(text.data() + at, (end == std::string::npos ? text.size() : end) - at);
        at = end == std::string::npos ? text.size() : end + 1;
        const size_t start = line.find_first_not_of(" \t");
        if (start == std::string_view::npos || !line.substr(start).starts_with("#include"))
            continue;
        const size_t quote = line.find('"', start + 8), close = quote == std::string_view::npos ? quote : line.find('"', quote + 1);
        if (close == std::string_view::npos)
            continue;
        const std::string name(line.substr(quote + 1, close - quote - 1));
        const std::optional<std::filesystem::path> path = findScenarioCHeader(name, directory);
        if (!path || !visited.insert(*path).second)
            continue;
        readMotionHeader(*path, name);
        const std::vector<uint8_t> bytes = readFile(path->string());
        readMotionSourceIncludes(std::string(bytes.begin(), bytes.end()), path->parent_path(), visited);
    }
}

} // namespace

void defineMotionName(const std::string &name, uint32_t id) {
    if (!g_motionNames.ambiguousIds.contains(id)) {
        const auto known = g_motionNames.names.find(id);
        if (known == g_motionNames.names.end())
            g_motionNames.names.emplace(id, name);
        else if (known->second != name) {
            g_motionNames.names.erase(known);
            g_motionNames.ambiguousIds.insert(id);
        }
    }
    if (!g_motionNames.ambiguousNames.contains(name)) {
        const auto known = g_motionNames.ids.find(name);
        if (known == g_motionNames.ids.end())
            g_motionNames.ids.emplace(name, id);
        else if (known->second != id) {
            g_motionNames.ids.erase(known);
            g_motionNames.ambiguousNames.insert(name);
        }
    }
}

void loadScriptMotionNames(const Program &program, const std::filesystem::path &path) {
    g_motionNames = {};
    g_motionNames.forceIncludes = true;
    const std::filesystem::path scenario = std::filesystem::absolute(path);
    setScenarioCIncludeDirectory(scenario.parent_path());
    const std::optional<std::filesystem::path> global = findScenarioCHeader("motion/motion.h");
    g_motionNames.root = global ? global->parent_path().parent_path() : scenario.parent_path().parent_path();
    addScenarioCIncludeDirectory(g_motionNames.root);
    const auto read = [&](std::filesystem::path relative) {
        relative.replace_extension(".h");
        const std::optional<std::filesystem::path> header = findScenarioCHeader(relative.generic_string(), g_motionNames.root);
        if (header)
            readMotionHeader(*header, relative.generic_string());
    };
    read("motion/motion.h");
    const std::string stem = lowerCase(scenario.stem().string());
    const bool named =
        stem.size() > 5 && stem.starts_with("act") && std::isdigit(static_cast<unsigned char>(stem[3])) && std::isdigit(static_cast<unsigned char>(stem[4]));
    const std::optional<uint32_t> act = named ? std::optional<uint32_t>((stem[3] - '0') * 10U + (stem[4] - '0')) : std::nullopt;
    const abi::CommandSpec *load = abi::findCommandByName("Script_motiLoad").command;
    for (const std::pair<const uint32_t, FunctionInfo> &function : program.functions) {
        for (const VmCallSite &call : analyzeVmCalls(program, function.second)) {
            if (call.control != Control::CallShenmue || call.target != load->publicIndex || call.arguments.size() < 3)
                continue;
            const VmValue &scene = call.arguments[0], &area = call.arguments[1], &file = call.arguments[2];
            if (file.kind != VmValueKind::DataAddress || file.bits >= program.blob.size())
                continue;
            std::string name;
            for (uint32_t at = file.bits; at < program.blob.size() && program.blob[at]; ++at)
                name += static_cast<char>(program.blob[at]);
            name = lowerCase(name);
            if (name == "m_mobj.bin" || name == "m_mdor.bin") {
                read(std::filesystem::path("misc") / name);
                continue;
            }
            const std::optional<uint32_t> sceneId = scene.kind == VmValueKind::Integer ? std::optional<uint32_t>(scene.bits) : act;
            if (area.kind != VmValueKind::Integer || !sceneId || *sceneId > 99)
                continue;
            std::string areaName;
            for (uint32_t byte = 0; byte < 4; ++byte)
                areaName += static_cast<char>(area.bits >> (byte * 8U));
            const std::string sceneName = std::string(1, static_cast<char>('0' + *sceneId / 10)) + static_cast<char>('0' + *sceneId % 10);
            read(std::filesystem::path("scene") / sceneName / lowerCase(areaName) / name);
        }
    }
}

void loadMotionSourceHeaders(const std::string &text, const std::filesystem::path &directory) {
    g_motionNames = {};
    std::set<std::filesystem::path> visited;
    readMotionSourceIncludes(text, directory, visited);
}

bool loadMotionInclude(const std::string &name) {
    const std::optional<std::filesystem::path> path = findScenarioCHeader(name);
    return path && readMotionHeader(*path, name);
}

std::optional<std::string_view> motionName(uint32_t id) {
    const auto found = g_motionNames.names.find(id);
    if (found == g_motionNames.names.end() || motionIdByName(found->second) != id)
        return std::nullopt;
    return found->second;
}

std::optional<uint32_t> motionIdByName(std::string_view name) {
    const auto found = g_motionNames.ids.find(name);
    return found == g_motionNames.ids.end() ? std::nullopt : std::optional<uint32_t>(found->second);
}

const std::vector<std::filesystem::path> &loadedMotionHeaders() {
    static const std::vector<std::filesystem::path> empty;
    return g_motionNames.forceIncludes ? g_motionNames.paths : empty;
}

std::string usedMotionIncludes(const std::string &text) {
    std::set<std::string> needed;
    for (size_t at = text.find("MN_"); at != std::string::npos; at = text.find("MN_", at + 3)) {
        if (at && (std::isalnum(static_cast<unsigned char>(text[at - 1])) || text[at - 1] == '_'))
            continue;
        size_t end = at + 3;
        while (end < text.size() && (std::isalnum(static_cast<unsigned char>(text[end])) || text[end] == '_'))
            ++end;
        const std::string name = text.substr(at, end - at);
        if (const std::optional<uint32_t> id = motionIdByName(name); id && motionName(*id) == name)
            needed.insert(name);
    }
    std::string includes;
    for (const MotionHeader &header : g_motionNames.headers) {
        bool used = header.include == "motion/motion.h";
        for (const std::string &name : header.names)
            used |= needed.erase(name) != 0;
        if (used)
            includes += "#include \"" + header.include + "\"\n";
    }
    return includes;
}

std::string insertMotionIncludes(std::string text) {
    const std::string include = "#include \"scenario.h\"\n", headers = usedMotionIncludes(text);
    const size_t at = text.find(include);
    return headers.empty() || at == std::string::npos ? text : text.insert(at + include.size(), headers);
}

} // namespace shenmue::scenario
