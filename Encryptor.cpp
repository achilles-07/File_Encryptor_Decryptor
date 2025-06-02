#include "Encryptor.h"
#include "MagicHeader.h"
#include <stdexcept>

void Encryptor::xorEncrypt(const std::vector<char>& data, const std::string& password, std::vector<char>& res)
{
    const std::string MAGIC = MagicHeader::MAGIC;  // Magic header to verify password correctness

    // Prepare data with magic header + input data
    std::vector<char> dataWithMagic;
    dataWithMagic.reserve(MAGIC.size() + data.size());
    dataWithMagic.insert(dataWithMagic.end(), MAGIC.begin(), MAGIC.end());
    dataWithMagic.insert(dataWithMagic.end(), data.begin(), data.end());

    res.resize(dataWithMagic.size());

    size_t pwdLen = password.size();
    for (size_t i = 0; i < dataWithMagic.size(); ++i) {
        res[i] = dataWithMagic[i] ^ password[i % pwdLen];
    }
}


void Encryptor::xorDecrypt(const std::vector<char>& data, const std::string& password, std::vector<char>& res)
{
    const std::string MAGIC = MagicHeader::MAGIC;

    res.resize(data.size());

    size_t pwdLen = password.size();
    for (size_t i = 0; i < data.size(); ++i) {
        res[i] = data[i] ^ password[i % pwdLen];
    }

    // Check magic header at start of decrypted data
    if (res.size() < MAGIC.size() || !std::equal(MAGIC.begin(), MAGIC.end(), res.begin())) {
        throw std::runtime_error("Incorrect password or corrupted file (magic header mismatch)");
    }

    // Remove magic header before returning actual decrypted data
    res.erase(res.begin(), res.begin() + MAGIC.size());
}
