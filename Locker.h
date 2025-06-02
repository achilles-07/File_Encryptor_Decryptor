#pragma once

#include<string>

class Locker
{
public:
    static void lock(const std::string& filePath, const std::string& password);
    static void unlock(const std::string& filePath, const std::string& password);
};

