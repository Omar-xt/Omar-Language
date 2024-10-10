#pragma once
#include <string>
#include <map>

#include "utils.hpp"

class Transpiler
{
private:
    std::map<std::string, std::string> keywords;

public:
    Transpiler()
    {
        keywords["Assignment"] = " = ";
        keywords["FloatLiteral"] = "float ";
        keywords["IntLiteral"] = "int ";
        keywords["StringLiteral"] = "std::string ";
        keywords["endline"] = ";";

        // --operators---
        keywords["Add_OP"] = "+";
        keywords["Sub_OP"] = "-";
        keywords["Mul_OP"] = "*";
        keywords["Div_OP"] = "/";
    }

    std::string get_keyword(std::string &key)
    {
        return map_iterator(key, keywords);
    }

    std::string get_keyword(std::string &&key)
    {
        return map_iterator(key, keywords);
    }
};