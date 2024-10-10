#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <map>

//-- remove this haders later --
#include <iostream>
#include "print_utils.hpp"

std::string map_iterator(std::string &str, std::map<std::string, std::string> &token_map)
{
    for (auto pair : token_map)
        if (pair.first == str)
            return pair.second;
    return "unknown";
}

int map_iterator(std::string &str, std::map<std::string, int> &token_map)
{
    for (auto pair : token_map)
        if (pair.first == str)
            return pair.second;
    return -404;
}

bool check_parentheses(std::string text, std::vector<std::string> &out)
{
    bool has_parentheses = false;

    if (text.find('(') != std::string::npos)
        has_parentheses = true;

    return has_parentheses;
}

bool check_quotation(std::string text)
{
    bool has_quotation = false;

    if (text.find('"') != std::string::npos)
        has_quotation = true;
    // if (text.find('\'') != std::string::npos)
    //     has_quotation = true;

    return has_quotation;
}

bool contain_binary_operators(std::string &text, char &sep)
{
    bool contain_binary = false;
    bool count = 0;

    if (text.find('+') != std::string::npos)
    {
        contain_binary = true;
        if (count == 1)
        {
            std::cout << "bineary operator erroe\n";
            contain_binary = false;
        }
        else
            count += 1;
        sep = '+';
    }
    if (text.find('-') != std::string::npos)
    {
        contain_binary = true;
        if (count == 1)
        {
            std::cout << "bineary operator erroe\n";
            contain_binary = false;
        }
        else
            count += 1;
        sep = '-';
    }
    if (text.find('*') != std::string::npos)
    {
        contain_binary = true;
        if (count == 1)
        {
            std::cout << "bineary operator erroe\n";
            contain_binary = false;
        }
        else
            count += 1;
        sep = '*';
    }
    if (text.find('/') != std::string::npos)
    {
        contain_binary = true;
        if (count == 1)
        {
            std::cout << "bineary operator erroe\n";
            contain_binary = false;
        }
        else
            count += 1;
        sep = '/';
    }

    return contain_binary;
}

void manage_quotation(std::string &text, std::vector<std::string> &out, std::vector<std::string> &code_bloks)
{
    unsigned int start_quot_index = -1;
    unsigned int end_quot_index = text.size();

    for (unsigned int i = 0; i < text.size(); i++)
    {
        if (text[i] == '"' && start_quot_index == -1)
            start_quot_index = i;
        else if (text[i] == '"')
        {
            end_quot_index = i;
            break;
        }
    }

    unsigned int count = end_quot_index - start_quot_index;
    code_bloks.push_back(text.substr(0, start_quot_index));
    code_bloks.push_back(text.substr(start_quot_index, count + 1));
    count = text.size() - end_quot_index;
    if (count > 1) // -- (c>1) because size is awlays greater than index by 1.
        code_bloks.push_back(text.substr(end_quot_index + 1, text.size() - count));

    // for (int i = 0; i < code_bloks.size(); i++)
    //     std::cout << code_bloks[i] << std::endl;

    // std::cout << code_bloks.size() << std::endl;
}

void manage_binary_operators(std::string &text, char &sep, std::vector<std::string> &out)
{
    unsigned int start_index = 0;

    for (unsigned int i = 0; i < text.size(); i++)
    {
        if (text[i] == sep)
            start_index = i;
    }

    out.push_back(text.substr(0, start_index));
    out.push_back(text.substr(start_index, 1));
    out.push_back(text.substr(start_index + 1, text.size()));
}

std::string remove_whitespace(const std::string &text)
{
    std::string new_text;

    for (unsigned int i = 0; i < text.size(); i++)
    {
        if (text[i] == ' ')
            continue;
        new_text.push_back(text[i]);
    }
    return new_text;
}

bool manage_parentheses(std::string text, std::vector<std::string> &out, std::vector<std::string> &code_bloks)
{
    unsigned int start_parentheses_index = 0;
    unsigned int end_parentheses_index = text.size();

    for (unsigned int i = 0; i < text.size(); i++)
    {
        if (text[i] == '(')
            start_parentheses_index = i;
        if (text[i] == ')')
            end_parentheses_index = i;
    }

    unsigned int count = end_parentheses_index - start_parentheses_index - 1;
    code_bloks.push_back(text.substr(0, start_parentheses_index));
    code_bloks.push_back(text.substr(start_parentheses_index, 1));

    //-- removing whitespace --
    std::string new_str = remove_whitespace(text.substr(start_parentheses_index + 1, count));

    code_bloks.push_back(new_str);
    code_bloks.push_back(text.substr(end_parentheses_index));

    // std::cout << text.substr(start_parentheses_index + 1, count) << std::endl;

    // for (int i = 0; i < code_bloks.size(); i++)
    //     std::cout << code_bloks[i] << std::endl;
}

void split(std::string &text, char seperator, std::vector<std::string> &out, bool is_block = false)
{
    std::string str;
    std::stringstream Text(text);
    std::vector<std::string> code_blocks;

    if (text.size() == 1 && is_block)
    {
        out.push_back(text);
        return;
    }

    if (check_quotation(text))
    {
        manage_quotation(text, out, code_blocks);
        split(code_blocks[0], ' ', out, true);
        out.push_back(code_blocks[1]);
        if (code_blocks.size() > 2) // -- generally code block size can be 2 or 3.
            split(code_blocks[2], ' ', out, true);
        if (!is_block)
            out.push_back("endline");
        return;
    }

    if (check_parentheses(text, out))
    {
        manage_parentheses(text, out, code_blocks);
        char seperator = ' ';
        for (unsigned int i = 0; i < code_blocks.size(); i++)
        {
            seperator = ' ';
            if (i == 2) //-- as manage_parentheses always make 4 part of the main string
                seperator = ',';
            split(code_blocks[i], seperator, out, true);
        }
        out.push_back("endline");
        return;
    }

    char sep;
    while (getline(Text, str, seperator))
    {
        if (contain_binary_operators(str, sep))
        {
            manage_binary_operators(str, sep, out);
            continue;
        }
        out.push_back(str);
    }
    if (!is_block)
        out.push_back("endline");
}

// std::vector<Token> get_line(unsigned int ind)
// {
//     return NULL;
// }