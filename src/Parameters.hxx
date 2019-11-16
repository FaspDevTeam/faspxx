
#ifndef FASPXX_PARAMETERS_HXX
#define FASPXX_PARAMETERS_HXX

#include <vector>
#include <iostream>

class Parameters
{
public:
    enum ParamType {_BOOL, _INT, _DOUBLE, _STRING}; // 允许的参数类型

private:
    struct Param // 每一个参数都是一个结构体
    {
        ParamType type;
        void* param_ptr;
        const char* param_name;
        const char* help;
        Param(ParamType _type, void* _ptr, const char* _name, const char* _help)
                : type(_type), param_ptr(_ptr), param_name(_name), help(_help) {}
    };
    std::vector<Param> params; // 保存所有的参数
    int argc;
    char** argv;

public:
    Parameters(int _argc, char* _argv[]): argc(_argc), argv(_argv) {}
    ~Parameters() {}

    // TODO: required or optional for every parameterfff
    void AddParam(bool* _ptr, const char* _name, const char* _help);

    void AddParam(int* _ptr, const char* _name, const char* _help);

    void AddParam(double* _ptr, const char* _name, const char* _help);

    void AddParam(const char** _ptr, const char* _name, const char* _help);

    void Parse();

    void PrintParams(std::ostream &out = std::cout) const;

    void PrintHelp(std::ostream &out = std::cout) const;
};




#endif //FASPXX_PARAMETERS_HXX
