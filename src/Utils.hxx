
#ifndef UTILS_HXX
#define UTILS_HXX

#include <string>

bool ProcessLine(std::string& line, std::string& param, std::string& value)
{
    // 如果该行不包含有效内容(即除去空白行,注释行等), 返回false; 否则, 返回true

    line.erase(0, line.find_first_not_of(' ')); //删除行首空格
    line.erase(line.find_last_not_of(' ') + 1); //删除行末空格

    if (line.empty()) return false; // 跳过空白行
    if (line[0] == '#') return false; // 跳过注释行

    std::string buff;
    std::stringstream ss(line);
    ss >> param >> value >> buff;
//    cout << "line|||" << line << "|||" << endl;
//    cout << "param|||" << param << "|||" << endl;
//    cout << "value|||" << value << "|||" << endl;

    return true;
}

#endif // UTILS_HXX
