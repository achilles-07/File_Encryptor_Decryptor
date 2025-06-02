#pragma once
#include <vector>
#include <string>

class FileManager
{
public:
    static void readFile(const std::string& path, std::vector<char>& buffer);
    static std::string writeFile(const std::string& path, const std::vector<char>& data, bool isForUnlock = 0);
    static bool fileExists(const std::string& path, bool isForUnlock=0);
};

