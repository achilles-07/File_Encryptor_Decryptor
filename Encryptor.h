#pragma once

#include <vector>
#include <string>

class Encryptor
{
public:
	static void xorEncrypt(const std::vector<char>& data, const std::string& password, std::vector<char>& res);
	static void xorDecrypt(const std::vector<char>& data, const std::string& password, std::vector<char>& res);
};

