#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

#if defined(_WIN32) && !defined(LAT_GUI_MODE)
#include <cwchar>
#include <filesystem>
#include <windows.h>
#endif

#include "lat_errors.hpp"
#include "lat_translator.hpp"
#include "file_reader.hpp"
#include "lat_file.hpp"
#include "accent.hpp"

int print_main_menu(const std::string& accent_file, const std::string& target_file);
char print_translation_menu();

#ifdef LAT_GUI_MODE
int main()
{
    // Default initial file paths
    std::string accent_path = "accent.lat";
    std::string target_path = "main.cpp";

    #ifdef LAT_GUI_MODE
    std::system("cls");
    #endif

    // Fully infinite loop that does not break
    while (true)
    {
        int main_choice = print_main_menu(accent_path, target_path);

        switch (main_choice)
        {
            case 1: // Proceed to translation menu
            {
                LAT_Accent my_accent = {accent_path};
                LAT_File file = {target_path};
                if (LAT_HAS_ERRORS) { 
                    break; 
                }

                LAT_FileReader lat_reader = {};
                lat_reader.readFile(file);
                if (LAT_HAS_ERRORS) { 
                    break; 
                }
                
                LAT_Translator translator = {my_accent, file};

                char decision = print_translation_menu();
                
                switch (decision)
                {
                    case '0':
                        translator.my_accent_to_theirs();
                        break;
                    case '1':
                        translator.their_accent_to_mine();
                        break;
                    default:
                        std::cout << "\nThat is not a valid translation option.\n";
                        break;
                }
                break;
            }
            case 2: // Re-enter accent file name
            {
                std::cout << "\nEnter new accent file name: ";
                std::cin >> accent_path;
                std::cout << "[Updated] Accent file set to: " << accent_path << "\n";
                break;
            }
            case 3: // Re-enter target file name
            {
                std::cout << "\nEnter new target file name: ";
                std::cin >> target_path;
                std::cout << "[Updated] Target file set to: " << target_path << "\n";
                break;
            }
            default:
            {
                std::cout << "\nInvalid choice. Please enter 1, 2, or 3.\n";
                break;
            }
        }
        
        std::system("cls");
    }

    return 0;
}

#else

namespace
{
const char* const settings_filename = ".ulatconfig";

#ifdef _WIN32
void add_executable_directory_to_user_path()
{
    std::vector<wchar_t> executable_buffer(32768);
    const DWORD executable_length = GetModuleFileNameW(
        nullptr,
        executable_buffer.data(),
        static_cast<DWORD>(executable_buffer.size()));
    if (executable_length == 0 || executable_length >= executable_buffer.size())
    {
        return;
    }

    const std::filesystem::path executable_path(
        std::wstring(executable_buffer.data(), executable_length));
    if (_wcsicmp(executable_path.filename().c_str(), L"lat.exe") != 0)
    {
        return;
    }

    const std::wstring install_directory = executable_path.parent_path().wstring();
    HKEY environment_key = nullptr;
    if (RegCreateKeyExW(
            HKEY_CURRENT_USER,
            L"Environment",
            0,
            nullptr,
            0,
            KEY_QUERY_VALUE | KEY_SET_VALUE,
            nullptr,
            &environment_key,
            nullptr) != ERROR_SUCCESS)
    {
        std::cerr << "Warning: could not open the current user's environment settings.\n";
        return;
    }

    DWORD value_type = REG_EXPAND_SZ;
    DWORD value_size = 0;
    LONG result = RegQueryValueExW(
        environment_key, L"Path", nullptr, &value_type, nullptr, &value_size);
    if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND)
    {
        RegCloseKey(environment_key);
        std::cerr << "Warning: could not read the current user's PATH.\n";
        return;
    }

    std::wstring user_path;
    if (result == ERROR_SUCCESS)
    {
        if (value_type != REG_EXPAND_SZ && value_type != REG_SZ)
        {
            RegCloseKey(environment_key);
            std::cerr << "Warning: the current user's PATH has an unsupported registry type.\n";
            return;
        }

        std::vector<wchar_t> path_buffer(value_size / sizeof(wchar_t) + 1, L'\0');
        result = RegQueryValueExW(
            environment_key,
            L"Path",
            nullptr,
            &value_type,
            reinterpret_cast<LPBYTE>(path_buffer.data()),
            &value_size);
        if (result != ERROR_SUCCESS)
        {
            RegCloseKey(environment_key);
            std::cerr << "Warning: could not read the current user's PATH.\n";
            return;
        }
        user_path.assign(path_buffer.data());
    }

    const auto contains_directory = [](const std::wstring& path, const std::wstring& directory)
    {
        std::wstring normalized_directory = directory;
        while (normalized_directory.size() > 3 &&
               (normalized_directory.back() == L'\\' || normalized_directory.back() == L'/'))
        {
            normalized_directory.pop_back();
        }

        size_t start = 0;
        while (start <= path.size())
        {
            const size_t end = path.find(L';', start);
            std::wstring entry = path.substr(start, end == std::wstring::npos ? end : end - start);
            while (!entry.empty() && (entry.back() == L'\\' || entry.back() == L'/'))
            {
                entry.pop_back();
            }
            if (_wcsicmp(entry.c_str(), normalized_directory.c_str()) == 0)
            {
                return true;
            }
            if (end == std::wstring::npos)
            {
                break;
            }
            start = end + 1;
        }
        return false;
    };

    if (contains_directory(user_path, install_directory))
    {
        RegCloseKey(environment_key);
        return;
    }

    const std::wstring updated_path = user_path.empty()
        ? install_directory
        : user_path + L";" + install_directory;
    const DWORD updated_type = value_type == REG_SZ ? REG_SZ : REG_EXPAND_SZ;
    const LONG write_result = RegSetValueExW(
        environment_key,
        L"Path",
        0,
        updated_type,
        reinterpret_cast<const BYTE*>(updated_path.c_str()),
        static_cast<DWORD>((updated_path.size() + 1) * sizeof(wchar_t)));
    RegCloseKey(environment_key);
    if (write_result != ERROR_SUCCESS)
    {
        std::cerr << "Warning: could not add the program directory to the current user's PATH.\n";
        return;
    }

    DWORD process_path_size = GetEnvironmentVariableW(L"Path", nullptr, 0);
    std::vector<wchar_t> process_path_buffer(process_path_size ? process_path_size : 1, L'\0');
    if (process_path_size)
    {
        GetEnvironmentVariableW(L"Path", process_path_buffer.data(), process_path_size);
    }
    std::wstring process_path(process_path_buffer.data());
    if (!contains_directory(process_path, install_directory))
    {
        if (!process_path.empty())
        {
            process_path += L';';
        }
        process_path += install_directory;
        SetEnvironmentVariableW(L"Path", process_path.c_str());
    }

    DWORD_PTR message_result = 0;
    SendMessageTimeoutW(
        HWND_BROADCAST,
        WM_SETTINGCHANGE,
        0,
        reinterpret_cast<LPARAM>(L"Environment"),
        SMTO_ABORTIFHUNG,
        5000,
        &message_result);

    std::cout << "Added the application folder to your user PATH. "
              << "Open a new terminal to use 'lat' by name.\n";
}
#endif

struct LAT_Settings
{
    std::string accent_path = "accent.lat";
    std::string target_path = "main.cpp";
};

void load_settings(LAT_Settings& settings)
{
    std::ifstream input(settings_filename);
    std::string accent_path;
    std::string target_path;
    if (input && std::getline(input, accent_path) && !accent_path.empty() &&
        std::getline(input, target_path) && !target_path.empty())
    {
        settings.accent_path = accent_path;
        settings.target_path = target_path;
    }
}

bool save_settings(const LAT_Settings& settings)
{
    std::ofstream output(settings_filename, std::ios::trunc);
    if (!output)
    {
        push_error(std::string("Could not save settings to ") + settings_filename);
        return false;
    }

    output << settings.accent_path << '\n' << settings.target_path << '\n';
    output.close();
    if (!output)
    {
        push_error(std::string("Failed to finish saving settings to ") + settings_filename);
        return false;
    }
    return true;
}

void print_usage()
{
    std::cout << "Usage:\n"
              << "  lat accent <path>      Set the accent rules file\n"
              << "  lat file <path>        Set the source file to translate\n"
              << "  lat translate -m       Translate into your accent\n"
              << "  lat translate -o       Translate into the other accent\n"
              << "  lat status             Show current file settings\n"
              << "  lat cls                Clear the console\n"
              << "  lat help               Show this help\n";
}

int translate_file(const LAT_Settings& settings, bool to_my_accent)
{
    LAT_Accent accent{settings.accent_path};
    if (LAT_HAS_ERRORS)
    {
        return LAT_EXIT_CODE;
    }

    LAT_File file{settings.target_path};
    LAT_FileReader reader;
    reader.readFile(file);
    if (LAT_HAS_ERRORS)
    {
        return LAT_EXIT_CODE;
    }

    LAT_Translator translator{accent, file};
    if (to_my_accent)
    {
        translator.their_accent_to_mine();
    }
    else
    {
        translator.my_accent_to_theirs();
    }
    return LAT_EXIT_CODE;
}

int execute_command(const std::vector<std::string>& args, LAT_Settings& settings)
{
    if (args.empty() || args[0] == "help" || args[0] == "--help" || args[0] == "-h")
    {
        print_usage();
        return 0;
    }

    const std::string& command = args[0];
    if (command == "cls")
    {
        if (args.size() != 1)
        {
            print_usage();
            return 2;
        }
        return std::system("cls");
    }

    if (command == "accent" || command == "file")
    {
        if (args.size() != 2 || args[1].empty())
        {
            print_usage();
            return 2;
        }

        if (command == "accent")
        {
            settings.accent_path = args[1];
        }
        else
        {
            settings.target_path = args[1];
        }

        if (!save_settings(settings))
        {
            return LAT_EXIT_CODE;
        }
        std::cout << "Updated " << (command == "accent" ? "accent" : "source")
                  << " file: " << args[1] << '\n';
        return 0;
    }

    if (command == "status")
    {
        if (args.size() != 1)
        {
            print_usage();
            return 2;
        }
        std::cout << "Accent file: " << settings.accent_path << '\n'
                  << "Source file: " << settings.target_path << '\n';
        return 0;
    }

    if (command == "translate")
    {
        if (args.size() != 2 || (args[1] != "-m" && args[1] != "-o"))
        {
            print_usage();
            return 2;
        }
        return translate_file(settings, args[1] == "-m");
    }

    std::cerr << "Unknown command: " << command << '\n';
    print_usage();
    return 2;
}

int run_interactive_cli(LAT_Settings& settings)
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream input(line);
        std::string executable;
        if (!(input >> executable))
        {
            continue;
        }
        if (executable != "lat")
        {
            std::cerr << "Commands must begin with 'lat'.\n";
            continue;
        }

        std::string command;
        if (!(input >> command))
        {
            execute_command({}, settings);
            continue;
        }
        if (command == "exit" || command == "quit")
        {
            return 0;
        }

        std::string argument;
        std::getline(input >> std::ws, argument);
        if (argument.size() >= 2 && argument.front() == '"' && argument.back() == '"')
        {
            argument = argument.substr(1, argument.size() - 2);
        }

        std::vector<std::string> args{command};
        if (!argument.empty())
        {
            args.push_back(argument);
        }
        execute_command(args, settings);
    }
    return 0;
}
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
    add_executable_directory_to_user_path();
#endif

    LAT_Settings settings;
    load_settings(settings);

    if (argc == 1)
    {
        std::cout << "Language Accent Translator. Copyright 2026 Tom Patton-Low\n";
        return run_interactive_cli(settings);
    }

    std::vector<std::string> args;
    for (int index = 1; index < argc; ++index)
    {
        args.emplace_back(argv[index]);
    }
    return execute_command(args, settings);
}

#endif

#ifdef LAT_GUI_MODE
int print_main_menu(const std::string& accent_file, const std::string& target_file)
{
    std::cout << "===============================================================\n";
    std::cout << "=  Language Accent Translator, Copyright Tom Patton-Low 2026  =\n";
    std::cout << "===============================================================\n";
    std::cout << "Current Accent File: " << accent_file << "\n";
    std::cout << "Current Target File: " << target_file << "\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << "COMMANDS: \n";
    std::cout << "\t[1] Proceed to Translation Menu\n";
    std::cout << "\t[2] Change LAT Accent File\n";
    std::cout << "\t[3] Change Target File\n";
    std::cout << "Enter your choice (1-3): ";
   
    int choice;
    std::cin >> choice;
    return choice;
}

char print_translation_menu()
{
    std::cout << "\nCHOOSE TRANSLATION: \n\t[0] Translate to other accent. \n\t[1] Translate to my accent.\n";
    std::cout << "Enter choice: ";
   
    char temp;
    std::cin >> temp;
    return temp;
}
#endif