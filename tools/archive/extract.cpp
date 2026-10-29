#include "common/archive_data.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <regex>
#include <optional>
#include <json/json.h>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    try {
        if (argc < 4)
            throw std::runtime_error("Usage: tacextract <game directory> <file list or JSON catalogue> <output directory> [--missing] [--match pattern]");
        bool missingOnly = false;
        std::optional<std::regex> pattern;
        for (int i = 4; i < argc; ++i) {
            const std::string option(argv[i]);
            if (option == "--missing")
                missingOnly = true;
            else if (option == "--match" && i + 1 < argc)
                pattern.emplace(argv[++i]);
            else
                throw std::runtime_error("Invalid extraction option: " + option);
        }
        std::ifstream input(argv[2], std::ios::binary);
        if (!input)
            throw std::runtime_error("Cannot open file list");
        if (!shenmue::archive_data::ArchiveData_LoadArchives(argv[1], "dx11", 2))
            throw std::runtime_error("Cannot open game archives");
        std::set<std::string> paths;
        if (std::filesystem::path(argv[2]).extension() == ".json") {
            Json::CharReaderBuilder reader;
            Json::Value catalogue;
            std::string error;
            if (!Json::parseFromStream(reader, input, &catalogue, &error) || !catalogue.isArray())
                throw std::runtime_error("Invalid filename catalogue");
            for (const Json::Value &entry : catalogue)
                if (entry["Path"].isString())
                    paths.insert(entry["Path"].asString());
        } else {
            for (std::string path; std::getline(input, path);) {
                if (!path.empty() && path.back() == '\r')
                    path.pop_back();
                if (!path.empty())
                    paths.insert(path);
            }
            if (!input.eof())
                throw std::runtime_error("Cannot read file list");
        }
        const std::filesystem::path destination(argv[3]);
        std::size_t count = 0, matched = 0;
        for (const std::string &path : paths) {
            if ((pattern && !std::regex_match(path, *pattern)) || !shenmue::archive_data::ArchiveData_IsPackedFile(path.c_str()))
                continue;
            const std::filesystem::path relative = std::filesystem::path(path).relative_path();
            for (const std::filesystem::path &component : relative)
                if (component == "..")
                    throw std::runtime_error("Archive path leaves the output directory");
            const std::filesystem::path target = destination / relative;
            ++matched;
            if (missingOnly && std::filesystem::is_regular_file(target))
                continue;
            const std::uint64_t size = shenmue::archive_data::ArchiveData_GetFileSize(path.c_str());
            std::vector<std::byte> bytes(static_cast<std::size_t>(size));
            if (!shenmue::archive_data::ArchiveData_Read(path.c_str(), 0, bytes.data(), size))
                throw std::runtime_error("Cannot read archive member: " + path);
            std::filesystem::create_directories(target.parent_path());
            std::ofstream output(target, std::ios::binary);
            output.write(reinterpret_cast<const char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            output.close();
            if (!output)
                throw std::runtime_error("Cannot write archive member: " + target.string());
            ++count;
        }
        shenmue::archive_data::ArchiveData_ClearArchives();
        if (!matched)
            throw std::runtime_error("No matching archive entries found");
        std::cout << "Extracted " << count << " files\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
