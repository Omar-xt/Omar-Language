#pragma once
#include <map>

#include "utils.hpp"
#include "print_utils.hpp"

class VariableTracker
{
private:
    std::map<std::string, std::string> variables;
    unsigned int length = 0;

public:
    VariableTracker() {}

    void store(std::string name, std::string type)
    {
        length += 1;
        variables[name] = type;
    }

    std::string get(std::string &name)
    {
        return map_iterator(name, variables);
    }

    bool is_empty()
    {
        return length == 0;
    }
};