#include "file_reader.hpp"

void LAT_FileReader::readFile(LAT_File& file_to_read)
{
    std::ifstream file(file_to_read.filename);

    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf(); // Read file buffer

        file_to_read.contents = buffer.str();
        #ifdef LAT_DEBUG
        std::cout << "DEBUG FILE READ:\n" << file_to_read.contents << std::endl;
        #endif
    }
    else
    {
        push_error("Failed to open source file.");
    }
}