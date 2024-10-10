#include <iostream>

int compile(std::string filename)
{
    std::string sort_name = filename.substr(0, filename.length() - 5);
    std::string command = "g++ " + sort_name + ".cpp" + " -o " + sort_name;
    std::string execute_command = sort_name + ".exe";

    system(command.c_str());
    return 0;
}

int compile_and_run(std::string filename)
{
    std::string sort_name = filename.substr(0, filename.length() - 5);
    std::string command = "g++ " + sort_name + ".cpp" + " -o " + sort_name;
    std::string execute_command = sort_name + ".exe";

    system(command.c_str());
    system(execute_command.c_str());
    return 0;
}