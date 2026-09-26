#include "accent.hpp"

/// The helper of helpers. Defined in a random .cpp file.
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

LAT_Accent::LAT_Accent(std::string filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        push_error("Could not open file " + filename);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string trimmed_line = trim(line);
        if (trimmed_line.empty()) continue;

        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) {
            push_error("Error in LAT Accent file, '=' not found between terms.");
            continue;
        }

        std::string my_word = trim(line.substr(0, eq_pos));
        std::string their_word = trim(line.substr(eq_pos + 1));

        if (my_word.empty() || their_word.empty()) {
            push_error("Error in LAT Accent file, malformed pair.");
            continue;
        }

        words.push_back({my_word, their_word});
    }
}