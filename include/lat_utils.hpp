#ifndef LAT_UTILS_HPP
#define LAT_UTILS_HPP

#include <string>

/// @brief A helper function to replace all occurences of a specific string with another.
/// @param str The string you want to replace in.
/// @param from What to replace with "to".
/// @param to What to replace "from" with.
inline void LAT_replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

#endif