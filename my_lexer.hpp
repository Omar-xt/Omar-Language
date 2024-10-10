#pragma once
#include <vector>

#include "tokenizer.hpp"

class Lexer
{
private:
    Tokenizer tokenizer;

public:
    void lex_token(std::vector<std::string> list, std::vector<Token> &token_list)
    {
        for (auto &str : list)
        {
            Token token = tokenizer.get_token(str.c_str());
            if (token.type != "space")
                token_list.push_back(token);
        }
    }
};