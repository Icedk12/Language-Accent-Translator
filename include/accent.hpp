#ifndef LAT_ACCENT_HPP
#define LAT_ACCENT_HPP

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include "lat_errors.hpp"

struct LAT_Pair
{
    std::string my_word;
    std::string their_word;
};

class LAT_Accent
{
public:
    std::vector<LAT_Pair> words; /// All words defined by the user to be translated to other accents.

    LAT_Accent(std::string filename);
};

#endif