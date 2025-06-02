#include <iostream>

#include "Locker.h"
#include "Encryptor.h"
#include "FileManager.h"
#include "Colors.h"

using namespace std;

void Locker::lock(const std::string& filePath, const std::string& password)
{
    try {
        // 1. Read file
        std::vector<char> buffer;
        FileManager::readFile(filePath, buffer);

        // 2. Encrypt
        std::vector<char> encrypted;
        Encryptor::xorEncrypt(buffer, password, encrypted);

        // 3. Write to file
        std::string fp = FileManager::writeFile(filePath, encrypted);
        cout << Color::GREEN << "Data encrypted to the file: " << fp << " successfully!" << Color::RESET << endl;

    }
    catch (const std::exception& ex) {
        cout << Color::RED << "Error: " << ex.what() << Color::RESET << endl;
        throw;
    }
}

void Locker::unlock(const std::string& filePath, const std::string& password)
{
    try {

        // 1. Read file
        std::vector<char> buffer;
        FileManager::readFile(filePath, buffer);

        // 2. Decrypt
        std::vector<char> decrypted;
        Encryptor::xorDecrypt(buffer, password, decrypted);

        // 3. Write to decrypted/filename
        cout << "File Path to Decryptor: " << filePath << endl;
        std::string fp = FileManager::writeFile(filePath, decrypted, true);
        cout << Color::GREEN << "Data decrypted to the file: " << fp << " successfully!" << Color::RESET << endl;

    }
    catch (const std::exception& ex) {
        cout << Color::RED << "Error: " << ex.what() << Color::RESET << endl;
        throw;
    }
}
