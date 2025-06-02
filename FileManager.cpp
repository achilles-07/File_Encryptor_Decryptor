#include "FileManager.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void FileManager::readFile(const std::string& path, std::vector<char> &buffer)
{
    // 1. Construct Complete Path
    fs::path completeFilePath = fs::current_path() / path;

    // 2. Read the file data
    std::ifstream file(completeFilePath, std::ios::binary);
    // It is assumed if this function is getting hit, filepath is valid.
    // Still adding a check for safety.
    if (!file) {
        throw std::runtime_error("Could not open file: " + completeFilePath.string());
    }

    // Get file size
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read content into vector
    buffer.resize(size);
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Error reading file: " + completeFilePath.string());
    }
}

std::string FileManager::writeFile(const std::string& path, const std::vector<char>& data, bool isForUnlock)
{
    std::string newFilePath;
    fs::path inputPath = fs::absolute(path);

    if (isForUnlock) {
        // Decryption

        // Create/Check the decrypted folder
        fs::path newDir = fs::absolute((static_cast<fs::path>(path)).parent_path()) / "decrypted";
        if (!fs::exists(newDir)) {
            fs::create_directory(newDir);
        }

        // Extract original filename without '.locked'
        fs::path originalName = inputPath.stem();
        fs::path fullPath = newDir / originalName;

        newFilePath = fullPath.string();
    }
    else {
        // Encryption
        newFilePath = inputPath.string() + ".locked";
    }


    fs::path absolutePath = fs::absolute(newFilePath);

    std::ofstream file(absolutePath, std::ios::binary | std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Could not open file for writing: " + absolutePath.string());
    }

    if (!file.write(data.data(), data.size())) {
        throw std::runtime_error("Failed to write data to file: " + absolutePath.string());
    }
    return absolutePath.string();
}

bool FileManager::fileExists(const std::string& path, bool isForUnlock)
{
    fs::path completeFilePath = fs::current_path() / path;

    if (!fs::exists(completeFilePath) || !fs::is_regular_file(completeFilePath)) {
        return false;
    }

    if (isForUnlock && completeFilePath.extension() != ".locked") {
        throw std::runtime_error("Only .locked files can be decrypted!");
    }

    return true;
}