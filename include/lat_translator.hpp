#ifndef LAT_TRANSLATOR_HPP
#define LAT_TRANSLATOR_HPP

#include <string>
#include <fstream>

#include "accent.hpp"
#include "lat_utils.hpp"
#include "lat_file.hpp"

class LAT_Translator
{
private:
LAT_Accent my_accent;
LAT_File main_file;

public:
LAT_Translator(LAT_Accent my_accent, LAT_File main_file);

void my_accent_to_theirs();
void their_accent_to_mine();

};

#endif