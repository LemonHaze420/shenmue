#include <shendk/files/animation/motn_reader.h>

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace {

bool extract(const std::filesystem::path &path) {
    std::filesystem::path output = path;
    output.replace_extension(".h");
    if (std::filesystem::exists(output))
        return false;
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file || file.tellg() < 32)
        throw std::runtime_error("Cannot read motion file: " + path.string());
    std::vector<std::byte> bytes(static_cast<std::size_t>(file.tellg()));
    file.seekg(0);
    file.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    shendk::MotnBankHeader header;
    if (!file || !shendk::ReadMotnBankHeader(bytes, header) || header.fileSize != bytes.size())
        throw std::runtime_error("Invalid motion header: " + path.string());
    shendk::MotnBankRange range;
    if (!shendk::ReadMotnBankRange(bytes, header, range))
        throw std::runtime_error("Invalid motion name table: " + path.string());
    std::map<std::string, std::uint32_t> names;
    for (std::uint32_t index = 0; index < range.count; ++index) {
        std::string_view name;
        if (!shendk::ReadMotnName(bytes, header, index, name))
            throw std::runtime_error("Invalid motion name: " + path.string());
        if (name.empty())
            continue;
        bool valid = true;
        for (const unsigned char character : name)
            valid &= std::isalnum(character) || character == '_';
        if (valid)
            names.emplace("MN_" + std::string(name), range.firstId + index);
    }
    std::ofstream result(output, std::ios::binary);
    result << "#pragma once\n\n";
    for (const std::pair<const std::string, std::uint32_t> &entry : names)
        result << "#ifndef " << entry.first << "\n#define " << entry.first << " 0x" << std::hex << std::uppercase << entry.second << "\n#endif\n";
    result.close();
    if (!result)
        throw std::runtime_error("Cannot write motion header: " + output.string());
    return true;
}

} // namespace

int main(int argc, char **argv) {
    try {
        if (argc != 2)
            throw std::runtime_error("Usage: motextract <motion file or directory>");
        const std::filesystem::path input(argv[1]);
        std::size_t count = 0;
        if (std::filesystem::is_directory(input)) {
            for (const std::filesystem::directory_entry &entry : std::filesystem::recursive_directory_iterator(input))
                if (entry.is_regular_file() && entry.path().extension() == ".bin")
                    count += extract(entry.path());
        } else {
            count = extract(input);
        }
        std::cout << "Generated " << count << " motion headers\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
