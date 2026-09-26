#include "lat_translator.hpp"

namespace
{
bool write_translation(const LAT_File& file)
{
    std::ofstream output(file.filename, std::ios::binary | std::ios::trunc);
    if (!output)
    {
        push_error("Failed to open target file for writing: " + file.filename);
        return false;
    }

    output.write(file.contents.data(), static_cast<std::streamsize>(file.contents.size()));
    if (!output)
    {
        push_error("Failed to write translated source to file: " + file.filename);
        return false;
    }

    output.close();
    if (!output)
    {
        push_error("Failed to finish writing translated source to file: " + file.filename);
        return false;
    }

    return true;
}
}

LAT_Translator::LAT_Translator(LAT_Accent accent, LAT_File main_file)
    : my_accent(accent), main_file(main_file) {}

void LAT_Translator::my_accent_to_theirs()
{
    for (LAT_Pair& pair : my_accent.words)
    {
        LAT_replaceAll(main_file.contents, pair.my_word, pair.their_word);
    }

    if (write_translation(main_file))
    {
        std::cout << "LANGUAGE ACCENT TRANSLATOR WROTE TO FILE: " << main_file.filename << std::endl;
    }
}

void LAT_Translator::their_accent_to_mine()
{
    for (LAT_Pair& pair : my_accent.words)
    {
        LAT_replaceAll(main_file.contents, pair.their_word, pair.my_word);
    }

    if (write_translation(main_file))
    {
        std::cout << "LANGUAGE ACCENT TRANSLATOR WROTE TO FILE: " << main_file.filename << std::endl;
    }
}
