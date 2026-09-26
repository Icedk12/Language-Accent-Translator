#ifndef LAT_FILE_READER_HPP
#define LAT_FILE_READER_HPP

#include <iostream>
#include <fstream>

#include <sstream>
#include <string>
#include <vector>

#include "lat_file.hpp"
#include "accent.hpp"
#include "lat_errors.hpp"

class LAT_FileReader 
{
private:

public:
    void readFile(LAT_File& file_to_read);

    LAT_FileReader() = default;
};

#endif