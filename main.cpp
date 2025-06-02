#include <iostream>
#include <sstream>
#include <regex>
#include <filesystem>


#include "FileManager.h"
#include "Locker.h"
#include "Colors.h"

using namespace std;
namespace fs = std::filesystem;

static bool callExit = 0;

void showMenu() {
    cout << "Welcome to File Encryptor/Decryptor" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "1. Perform Lock Operation: <filelockerExecutablePath> lock <pathToFile> <customKey>" << endl;
    cout << "2. Perform Unlock Operation: <filelockerExecutablePath> unlock <pathToLockedFile> <lockingKey>" << endl;
    cout << endl;
    cout << "Enter the command you want to execute below:" << endl;
    cout << "\033[1;36mNote: The paths must be passed within double quotes(\"<path>\")\033[0m" << endl;

}


bool parseCommand(const std::string& input, std::vector<std::string>& args) {
    static const std::regex pattern(R"((\"[^\"]*\"|\S+))");
    std::sregex_iterator it(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    for (; it != end; ++it) {
        std::string token = (*it)[0].str();
        if (token.front() == '"' && token.back() == '"') {
            token = token.substr(1, token.size() - 2); // remove quotes
        }
        args.push_back(token);
    }
    return true;
}

bool isValidCommand(string& command, vector<string>& commandArgs)
{
    commandArgs.clear();
    if (!parseCommand(command, commandArgs)) {
        cout << Color::RED << "Failed to parse command." << Color::RESET << endl;
        return false;
    }

    if (commandArgs.size() != 4) {
        cout << Color::RED << "Invalid number of arguments. Expected 4." << Color::RESET << endl;
        return false;
    }

    if (!FileManager::fileExists(commandArgs[0])) {
        cout << Color::RED << "Invalid Path for filelocker executable!" << Color::RESET << endl;
        return false;
    }

    if (commandArgs[1] != "lock" && commandArgs[1] != "unlock") {
        cout << Color::RED << "Unknown action: " << commandArgs[1] << Color::RESET << endl;
        return false;
    }

    bool isForUnlock = commandArgs[1] == "unlock";
    if (!FileManager::fileExists(commandArgs[2]), isForUnlock) {
        cout << Color::RED << "Input file does not exist!" << Color::RESET << endl;
        return false;
    }

    if (commandArgs[3].empty()) {
        cout << Color::RED << "Password can't be empty." << Color::RESET << endl;
        return false;
    }

    return true;
}


void handleInvalidCommand()
{
    cout << "----------------------------" << endl;

    while (true) {
        cout << "1. Refer Doc" << endl;
        cout << "2. Try again" << endl;
        cout << "3. Exit" << endl;
        fs::path cwd = fs::current_path();
        cout << cwd.string() << "> ";

        int num;
        if (!(cin >> num)) {
            cout << Color::RED << "Error: Please enter a valid number!" << Color::RESET << endl;
            cin.clear(); // clear error state
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline

        switch (num) {
        case 1:
            showMenu();
            return;
        case 2:
            cout << "Enter the command you want to execute below:" << endl;
            cout << Color::YELLOW << "Note: The paths must be passed within double quotes (\"<path>\")" << Color::RESET << endl;
            return;
        case 3:
            callExit = 1;
            return;
        default:
            cout << Color::RED << "Error: Invalid command!" << Color::RESET << endl;
            break;
        }
    }
}


int main()
{
    // Show Start Menu
    showMenu();
    
    // Declare Command DS
    vector<string> commandArgs;
    string command;

    // Handle User Interaction
    while (!callExit) {
        fs::path cwd = fs::current_path();
        cout << cwd.string() << "> ";
        
        getline(cin, command, '\n');

        if (!isValidCommand(command, commandArgs)) {
            handleInvalidCommand();
        }
        else {
            // Handle valid command
            if (commandArgs[1] == "lock")
            {
                try {
                    Locker::lock(commandArgs[2], commandArgs[3]);
                }
                catch (std::exception& ex) {
                    handleInvalidCommand();
                }
            }
            else
            {
                try {
                    Locker::unlock(commandArgs[2], commandArgs[3]);
                }
                catch (std::exception& ex) {
                    handleInvalidCommand();
                }
            }
        }
        commandArgs.clear();
        command = "";
    }

    return 0;
}
