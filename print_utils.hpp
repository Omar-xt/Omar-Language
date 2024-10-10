#pragma once
#include <iostream>
#include <vector>

template <typename T>
void print(std::vector<T> &vec)
{
    for (auto element : vec)
        std::cout << element << std::endl;
}

void print(std::string text)
{
    std::cout << text << std::endl;
}