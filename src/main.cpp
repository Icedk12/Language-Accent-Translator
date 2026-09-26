#include <iostream>
#include <string>
#include <cstdlib>

#include "lat_errors.hpp"
#include "lat_translator.hpp"
#include "file_reader.hpp"
#include "lat_file.hpp"
#include "accent.hpp"

int print_main_menu(const std::string& accent_file, const std::string& target_file);
char print_translation_menu();

int main()
{
    // Default initial file paths
    std::string accent_path = "test/accent.lat";
    std::string target_path = "test/lat_test.cpp";

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