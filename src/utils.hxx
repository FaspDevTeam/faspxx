
#ifndef FASPXX_UTILS_HXX
#define FASPXX_UTILS_HXX

#include <string>
#include <algorithm>
#include <sstream>

/// Process a line. Return false if the line is a comment or empty.
static bool ProcessLine(std::string& line, std::string& param, std::string& value)
{
    // Remove spaces in the beginning and end of each line
    line.erase(0, line.find_first_not_of(' '));
    line.erase(line.find_last_not_of(' ') + 1);

    if ( line.empty()   ) return false; // Skip empty lines
    if ( line[0] == '#' ) return false; // Skip comments
    if ( line[0] == '%' ) return false; // Skip comments

    std::string buff;
    std::stringstream ss(line);
    ss >> param >> value >> buff;
    return true;
}

static char ToLower(char in)
{
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

static char ToUpper(char in)
{
    if (in <= 'z' && in >= 'a')
        return in - ('a' - 'A');
    return in;
}

static bool JudgeBool(const std::string& value)
{
    std::string copy(value);
    std::transform(value.begin(), value.end(), copy.begin(),
                   [](unsigned char c){ return std::tolower(c);});

    if (copy == "true") {
        return true;
    }
    else if (copy == "false") {
        return false;
    }
    else { // 其余情况就是int型整数
        return (bool) (std::stoi(copy));
    }
}

#endif //FASPXX_UTILS_HXX
