
#include <sstream>

#include "utils.hpp"

std::string read_file (const std::string &file_path)
{
        std::string output;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(file_path);
        
        std::stringstream file_stream;
        
        file_stream << file.rdbuf();
        file.close();
        
        return file_stream.str();
}