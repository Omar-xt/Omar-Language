#pragma once
#include <iostream>
#include <map>
#include <ctype.h>

#include "utils.hpp"
#define UNKNOWN "unknown"

std::string cross_check_dig(std::string &str)
{
    bool floatliteral = false;
    unsigned int count = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (std::isdigit(str[i]))
            continue;
        else if (str[i] == '.')
        {
            count += 1;
            if (count > 1)
                return "unknown";
            floatliteral = true;
        }
        else
            return "unknown";
    }
    if (floatliteral)
        return "FloatLiteral";
    if (str.size() == 0)
        return "space";
    return "IntLiteral";
}

std::string cross_check_str(std::string &str)
{
    //-- 34 = " (double quote)   and     39 = ' (single quote)
    if (str[0] == 34 && str[str.length() - 1] == 34)
        return "StringLiteral";
    else if (str[0] == 39 && str[str.length() - 1] == 39)
        return "StringLiteral";

    return "unknown";
}

std::string cross_check(std::string &str, std::string type)
{
    if (type == "unknown")
        type = cross_check_dig(str);
    if (type == "unknown")
        type = cross_check_str(str);

    return type;
}

struct Token
{
    std::string type;
    std::string value;

    friend std::ostream &operator<<(std::ostream &out, Token const &token)
    {
        out << "[" << token.value << "]"
            << " : ";
        out << "\"" << token.type << "\"";
        return out;
    }
};

class Tokenizer
{
public:
    std::map<std::string, std::string> tokens;

public:
    Tokenizer()
    {
        tokens["let"] = "Declare_var";
        tokens["endline"] = "command";
        tokens["="] = "Assignment";
        tokens["+"] = "Add_OP";
        tokens["-"] = "Sub_OP";
        tokens["*"] = "Mul_OP";
        tokens["/"] = "Div_OP";
        tokens["\n"] = "NewLine";
        tokens["("] = "start_parentheses";
        tokens[")"] = "end_parentheses";
        tokens["{"] = "start_curly";
        tokens["}"] = "end_curly";

        // --keywords -------
        tokens["fn"] = "Func";

        // --builtin functions ----------
        tokens["print"] = "builtin_function";
    }

    Token get_token(std::string str)
    {
        std::string type = map_iterator(str, tokens);

        type = cross_check(str, type);

        return Token{type, str};
    }
};