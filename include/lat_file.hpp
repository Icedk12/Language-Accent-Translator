#ifndef LAT_FILE_HPP
#define LAT_FILE_HPP

#include <vector>
#include <string>

class LAT_File 
{
public:
    std::string filename;
    std::string contents;

    LAT_File(std::string filename);
};


#endif